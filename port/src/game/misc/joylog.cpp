#include "game/misc/joylog.hpp"

#include <cstring>

namespace nfsu {

bool Joylog::replaying = false;
bool Joylog::recording = false;
JoylogBuffer* Joylog::buffer = nullptr;

// orig: 0x559530 JoylogBuffer::AddData
void JoylogBuffer::AddData(uint32_t value, int bits, int channel) {
    int bytes = (bits + 7) >> 3;
    if (value == lastValue && channel == lastChannel) {
        int n = ++repeatCount;
        int limit = repeatLimit[channel & 15];
        if (limit != 0 && limit < n && service) service(1);
    } else {
        lastValue = value;
        lastChannel = channel;
    }
    uint8_t* p = &data[pos - base];
    p[0] = static_cast<uint8_t>(bytes * 16 + channel);
    for (int i = 0; i < bytes; ++i) {
        p[1 + i] = static_cast<uint8_t>(value);
        value >>= 8;
    }
    int newPos = pos + bytes + 1;
    pending = newPos - base;
    pos = newPos;
    end = newPos;
    if (base - newPos + kBlock < kSlack && pending > 0) {
        if (writeBlock) writeBlock(data.data(), pending);
        base = pos;
        pending = 0;
    }
}

// orig: 0x5594d0 JoylogBuffer::LoadBuffer
void JoylogBuffer::LoadBuffer() {
    int n = end - pos;
    if (n > kBlock)
        n = kBlock;
    else if (n < 1)
        return;
    if (readBlock) readBlock(data.data(), pos, n);
    base = pos;
    loadLimit = pos + n;
}

// orig: 0x559670 JoylogBuffer::AddEntry (liest einen Eintrag ab Position `at`)
int JoylogBuffer::ReadEntry(Entry* out, int at) const {
    const uint8_t* p = &data[at - base];
    out->size = p[0] >> 4;
    out->channel = p[0] & 0xf;
    uint32_t v = 0;
    for (int i = out->size; i > 0; --i) v = (v << 8) | p[i];
    out->value = v;
    return at + out->size + 1;
}

// orig: 0x559600 JoylogBuffer::GetData
// Stimmt der Kanal nicht, ist die Wiedergabe aus dem Takt; das Original hält mit einem Haltepunkt an.
uint32_t JoylogBuffer::GetData(int channel) {
    if (loadLimit - pos < kSlack && loadLimit != end) LoadBuffer();
    Entry e;
    pos = ReadEntry(&e, pos);
    if (e.channel != channel) {
        desync = true;
        return 0;
    }
    return e.value;
}

// orig: 0x559700 Joylog::StopReplaying
void Joylog::StopReplaying() {
    if (replaying) {
        buffer = nullptr;
        replaying = false;
    }
}

// orig: 0x559720 Joylog::GetData; nach dem letzten Eintrag endet die Wiedergabe von selbst
uint32_t Joylog::GetData(int /*bits*/, int channel) {
    if (!replaying) return 0;
    uint32_t v = buffer->GetData(channel);
    if (buffer->end <= buffer->pos && replaying) {
        buffer = nullptr;
        replaying = false;
    }
    return v;
}

void Joylog::AddData(uint32_t value, int bits, int channel) {
    if (recording) buffer->AddData(value, bits, channel);
}

// orig: 0x559770 Joylog::AddOrGetData(char*, channel)
// Wiedergabe: Länge (16 Bit) und Zeichen (je 8 Bit) aus dem Log in `str` schreiben. Aufnahme: den Inhalt von `str` ablegen.
void Joylog::AddOrGetData(char* str, int channel) {
    if (replaying) {
        int len = static_cast<int>(GetData(16, channel));
        int i = 0;
        for (; i < len; ++i) str[i] = replaying ? static_cast<char>(GetData(8, channel)) : '\0';
        str[i] = '\0';
    }
    if (recording) {
        int len = static_cast<int>(std::strlen(str));
        AddData(static_cast<uint32_t>(len), 16, channel);
        for (int i = 0; i < len; ++i) AddData(static_cast<uint32_t>(static_cast<int>(str[i])), 8, channel);
    }
}

}  // namespace nfsu
