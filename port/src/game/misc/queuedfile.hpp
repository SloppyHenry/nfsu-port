#pragma once
// ../indep/src/misc/QueuedFile.cpp: Warteschlange für Dateilesevorgänge. Benachbarte Anfragen aus derselben Datei werden
// zu einem Lesevorgang ("Bundle") zusammengefasst, bis zu 3 Lesevorgänge laufen gleichzeitig.
// Dateizugriff läuft über FileIO (Phase 4: WP-43); hier liest die Standardumsetzung sofort.
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace nfsu {

enum QueuedFileStatus { QF_WAITING = 0, QF_READING = 1, QF_DONE = 2, QF_ERROR = 3 };

struct QueuedFile {
    void* pBuf = nullptr;        // 0x08
    std::string Filename;        // 0x0c (48 Zeichen im Original)
    int FilePos = 0;             // 0x3c
    int NumBytes = 0;            // 0x40
    int NumRead = 0;             // 0x44
    int BlockSize = 0x100000;    // 0x48
    int Priority = 5;            // 0x4c
    int DisableJoylog = 0;       // 0x50
    std::function<void(int error)> Callback;  // 0x54..0x60 (Parameter 1/2 stecken im Funktionsobjekt)
    int Aborted = 0;             // 0x6c (im Original "AbortFlag")
    int Handle = 0;              // 0x64
    QueuedFileStatus Status = QF_WAITING;  // 0x68
    float StartReadTime = 0.0f;  // 0x70

    void CallCallback(int error) const { if (Callback) Callback(error); }  // orig: 0x565cb0
};

struct QueuedFileBundle {
    std::vector<uint8_t> ReadBuffer;
    int ReadBufferBot = 0;
    int ReadBufferTop = 0;
    int NumBytesQueued = 0;
    std::vector<QueuedFile*> QueuedFiles;  // höchstens 16

    bool TestAddQueuedFile(QueuedFile* f, int memoryBudget);  // orig: 0x565e80
};

struct FileIO {
    virtual ~FileIO() = default;
    virtual bool FileSize(const std::string& name, int* size) = 0;
    virtual bool Read(const std::string& name, int pos, int len, void* dst) = 0;
};

struct QueuedFileSystem {
    FileIO* io = nullptr;
    int memoryBudget = 0x6300000;   // orig: g_heaps != 0 ? 0x6300000 : 0
    std::function<void()> runTasks; // orig: SYNCTASK_run(0) + process_queue
    float (*debugTime)() = nullptr;  // orig: GetDebugRealTime

    std::list<std::unique_ptr<QueuedFile>> waiting;  // orig: 0x72c9bc
    std::list<std::unique_ptr<QueuedFile>> reading;  // orig: 0x72c9b4
    std::string lastFilename;                        // orig: 0x728288
    int nextHandle = 1;                              // orig: 0x6e2f54, läuft 1..100000

    int AddQueuedFile(void* buf, const char* name, int pos, int nbytes, std::function<void(int)> cb);  // orig: 0x566370 / 0x5663f0
    QueuedFile* FindQueuedFile(int handle);       // orig: 0x566320
    void ChangeQueuedFileParams(int handle, int priority, int blockSize, int disableJoylog);  // GC 0x80198f08
    bool IsBusy() const { return reading.empty() && waiting.empty() ? false : true; }  // GC 0x80198d38
    void Service();                               // orig: 0x566080 ServiceQueuedFiles

private:
    int CreateBundleFile(QueuedFileBundle* b);    // orig: 0x565f40
};

}  // namespace nfsu
