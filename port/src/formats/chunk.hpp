// Chunk-Container der PC-Spieldaten: Kopf {uint32 id, uint32 size}, danach size Bytes Nutzdaten.
// Ist Bit 31 der ID gesetzt, besteht die Nutzlast aus weiteren Chunks (Container).
// ID 0 ist ein Füllchunk (Ausrichtung), Nutzdaten sind Nullen.
#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace nfsu {

struct ChunkView {
    uint32_t id = 0;
    uint32_t size = 0;
    const uint8_t* data = nullptr;  // Nutzdaten
    int depth = 0;
    size_t offset = 0;              // Offset des Kopfes im Wurzelpuffer
    bool container() const { return (id & 0x80000000u) != 0; }
};

// Durchläuft alle Chunks (Tiefensuche, Container vor ihren Kindern).
// Rückgabe: leer bei Erfolg, sonst Fehlertext (Grenzverletzung). Bei Erfolg endet die
// Kette exakt am Pufferende (keine Restbytes).
// visit liefert false, um den Abstieg in diesen Container zu überspringen.
std::string walk_chunks(const uint8_t* buf, size_t len,
                        const std::function<bool(const ChunkView&)>& visit);

// Entpackt eine .lzc-Datei (JDLZ). Leer bei falschem Kopf.
std::vector<uint8_t> unpack_lzc(const uint8_t* src, size_t len);

}  // namespace nfsu
