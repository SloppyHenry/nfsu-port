#include "game/misc/queuedfile.hpp"

#include <algorithm>
#include <cstring>

#include "game/misc/joylog.hpp"

namespace nfsu {

// orig: 0x565e80 QueuedFileBundle::TestAddQueuedFile
bool QueuedFileBundle::TestAddQueuedFile(QueuedFile* f, int memoryBudget) {
    int n = static_cast<int>(QueuedFiles.size());
    if (n >= 16 || f->NumRead != 0) return false;
    if (n > 0) {
        if (f->Filename != QueuedFiles[0]->Filename || f->DisableJoylog != 0) return false;
    }
    int end = f->NumBytes + f->FilePos;
    int bytes = NumBytesQueued + f->NumBytes;
    int start = f->FilePos & ~0x7ff;
    if (n > 0) {
        start = std::min(start, ReadBufferBot);
        end = std::max(end, ReadBufferTop);
    }
    int span = end - start;
    if (span <= 0x40000 && span - bytes < 0x8000 && span <= memoryBudget - 0x40000) {
        ReadBufferBot = start;
        ReadBufferTop = end;
        NumBytesQueued = bytes;
        QueuedFiles.push_back(f);
        return true;
    }
    return false;
}

// orig: 0x566370 AddQueuedFile (mit Rückruf-Parameter im Funktionsobjekt statt Param1/Param2); hängt hinten an
int QueuedFileSystem::AddQueuedFile(void* buf, const char* name, int pos, int nbytes, std::function<void(int)> cb) {
    auto f = std::make_unique<QueuedFile>();  // orig: 0x565d10 QueuedFile::QueuedFile
    f->pBuf = buf;
    f->Filename = name;
    f->FilePos = pos;
    f->NumBytes = nbytes;
    f->Callback = std::move(cb);
    f->Handle = nextHandle++;
    if (nextHandle > 100000) nextHandle = 1;
    int h = f->Handle;
    waiting.push_back(std::move(f));
    return h;
}

// orig: 0x566320 FindQueuedFile: erst die laufenden, dann die wartenden
QueuedFile* QueuedFileSystem::FindQueuedFile(int handle) {
    for (auto& f : reading)
        if (f->Handle == handle) return f.get();
    for (auto& f : waiting)
        if (f->Handle == handle) return f.get();
    return nullptr;
}

void QueuedFileSystem::ChangeQueuedFileParams(int handle, int priority, int blockSize, int disableJoylog) {
    if (QueuedFile* f = FindQueuedFile(handle)) {
        f->DisableJoylog = disableJoylog;
        f->Priority = priority;
        f->BlockSize = blockSize;
    }
}

// orig: 0x565f40 QueuedFileBundle::BeginRead: ein gemeinsamer Lesevorgang über den Bereich aller Teilanfragen.
// Der Rückruf kopiert die Teile in die Zielpuffer (orig: 0x565fd0 ReadCallback) und ruft deren Rückrufe.
int QueuedFileSystem::CreateBundleFile(QueuedFileBundle* b) {
    int bot = b->ReadBufferBot, top = b->ReadBufferTop;
    b->ReadBuffer.assign(static_cast<size_t>(top - bot), 0);
    // Der Bundle gehört nach dem Aufruf dem Rückruf.
    auto owned = std::make_shared<QueuedFileBundle>(std::move(*b));
    int handle = AddQueuedFile(owned->ReadBuffer.data(), owned->QueuedFiles[0]->Filename.c_str(), bot, top - bot,
                               [owned](int error) {
                                   for (QueuedFile* qf : owned->QueuedFiles) {
                                       std::memcpy(qf->pBuf, owned->ReadBuffer.data() + (qf->FilePos - owned->ReadBufferBot),
                                                   static_cast<size_t>(qf->NumBytes));
                                       qf->CallCallback(error);
                                   }
                               });
    return handle;
}

// orig: 0x566080 ServiceQueuedFiles
void QueuedFileSystem::Service() {
    if (runTasks) runTasks();

    if (!reading.empty()) {
        QueuedFile* first = reading.front().get();
        int status = first->Status;
        if (first->DisableJoylog == 0) {
            if (!Joylog::replaying) {
                if (Joylog::recording) Joylog::AddData(static_cast<uint32_t>(status), 4, 6);
            } else {
                status = static_cast<int>(Joylog::GetData(4, 6));
                if (status != 1) {
                    while (first->Status == QF_READING) {
                        if (runTasks) runTasks();
                    }
                }
            }
        }
        if (status == QF_DONE) {
            if (!first->Aborted) {
                if (first->NumRead != first->NumBytes) {
                    // Rest steht noch aus: wieder hinten in die Warteschlange
                    first->Status = QF_WAITING;
                    waiting.push_back(std::move(reading.front()));
                    reading.pop_front();
                    first = nullptr;
                } else {
                    first->CallCallback(0);
                }
            }
            if (first) reading.pop_front();
        } else if (status == QF_ERROR) {
            first->CallCallback(1);
            reading.pop_front();
        }
    }

    while (reading.size() < 3 && !waiting.empty()) {
        waiting.sort([](const auto& a, const auto& b) { return a->Priority < b->Priority; });  // stabil, kleinere Zahl zuerst
        if (!reading.empty() && waiting.front()->Filename != lastFilename) break;

        QueuedFileBundle bundle;
        for (auto& w : waiting)
            if (!bundle.TestAddQueuedFile(w.get(), memoryBudget)) break;
        if (bundle.QueuedFiles.size() >= 2) {
            // Teilanfragen aus der Warteschlange nehmen (Besitz bleibt im Bundle-Rückruf über rohe Zeiger)
            std::vector<std::unique_ptr<QueuedFile>> members;
            for (QueuedFile* m : bundle.QueuedFiles) {
                auto it = std::find_if(waiting.begin(), waiting.end(), [&](const auto& p) { return p.get() == m; });
                members.push_back(std::move(*it));
                waiting.erase(it);
            }
            // Mitglieder müssen bis zum Rückruf leben: dem Rückruf übergeben (siehe unten)
            int h = CreateBundleFile(&bundle);
            auto it = std::find_if(waiting.begin(), waiting.end(), [&](const auto& p) { return p->Handle == h; });
            auto merged = std::move(*it);
            waiting.erase(it);
            auto keep = std::make_shared<std::vector<std::unique_ptr<QueuedFile>>>(std::move(members));
            auto inner = merged->Callback;
            merged->Callback = [keep, inner](int e) { inner(e); };
            waiting.push_front(std::move(merged));
        }

        std::unique_ptr<QueuedFile> next = std::move(waiting.front());
        waiting.pop_front();
        lastFilename = next->Filename;

        // QueuedFile::BeginRead (orig: 0x565d90) mit der Dateiebene
        QueuedFile* f = next.get();
        if (f->NumBytes == 0) {
            f->Status = QF_DONE;
        } else {
            int size = 0;
            int len = std::min(f->NumBytes - f->NumRead, f->BlockSize);
            int pos = f->FilePos + f->NumRead;
            if (!io || !io->FileSize(f->Filename, &size) || size < pos + len) {
                f->Status = QF_ERROR;
            } else {
                f->Status = io->Read(f->Filename, pos, len, static_cast<uint8_t*>(f->pBuf) + f->NumRead) ? QF_DONE : QF_ERROR;
                f->NumRead += len;
            }
        }
        reading.push_back(std::move(next));
    }
}

}  // namespace nfsu
