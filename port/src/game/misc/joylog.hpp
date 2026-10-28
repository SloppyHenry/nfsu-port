#pragma once
// Joylog.cpp: Aufzeichnung und Wiedergabe von Eingaben und Zufallswerten ("Joylog").
// Eintrag im Puffer: 1 Byte Kopf (Größe in Bytes << 4 | Kanal), danach der Wert little-endian.
// Dateizugriff und Soundsystem sind Haken, damit der Kern ohne Plattformcode läuft (Phase 4).
#include <cstdint>
#include <functional>
#include <vector>

namespace nfsu {

class JoylogBuffer {
public:
    static constexpr int kBlock = 0x4000;
    static constexpr int kSlack = 0x100;

    int pos = 0;         // orig +0x00: Position im Gesamtstrom
    int pending = 0;     // orig +0x04: seit dem letzten Block geschriebene Bytes
    int loadLimit = 0;   // orig +0x08: Ende des geladenen Fensters (nur Wiedergabe)
    int base = 0;        // orig +0x0c: Strompositon von data[0]
    int end = 0;         // orig +0x10: Länge des Stroms
    int lastChannel = 0; // orig +0x14
    uint32_t lastValue = 0;  // orig +0x1c
    int repeatCount = 0;     // orig +0x20
    int repeatLimit[16] = {};  // orig Tabelle 0x6e35b0 (Schritt 0xc): Grenze für Service-Aufruf, 0 = keine
    std::vector<uint8_t> data = std::vector<uint8_t>(kBlock + 2 * kSlack);
    bool desync = false;  // Original löst hier einen Haltepunkt aus

    std::function<void(const uint8_t*, int)> writeBlock;  // Datei anhängen
    std::function<int(uint8_t*, int, int)> readBlock;     // (dst, streamPos, n) -> gelesene Bytes
    std::function<void(int)> service;                     // orig: SNDSYS_service(1)

    struct Entry {
        int channel, size;
        uint32_t value;
    };

    void AddData(uint32_t value, int bits, int channel);   // orig: 0x559530
    uint32_t GetData(int channel);                          // orig: 0x559600 (Bitzahl steht im Eintrag)
    void LoadBuffer();                                      // orig: 0x5594d0
    int ReadEntry(Entry* out, int at) const;                // orig: 0x559670, liefert neue Position
};

// Globale Zustände des Originals: 0x7295e0 (Wiedergabe), 0x7295e4 (Aufnahme), 0x7295e8 (Puffer).
struct Joylog {
    static bool replaying;
    static bool recording;
    static JoylogBuffer* buffer;

    static void StopReplaying();                                   // orig: 0x559700
    static uint32_t GetData(int bits, int channel);                // orig: 0x559720
    static void AddData(uint32_t value, int bits, int channel);    // Aufnahme-Zweig, inline im Original
    static void AddOrGetData(char* str, int channel);              // orig: 0x559770
};

}  // namespace nfsu
