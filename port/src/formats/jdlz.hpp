// JDLZ-Dekompressor (Version 2). Vollständige Formatbeschreibung folgt in WP-12.
#pragma once
#include <cstdint>

namespace nfsu {

// orig: 0x59dd70 jdlz_decompress
// src beginnt mit dem 0x12 Byte langen Kopf ('JDLZ', Version 2). dst muss mindestens die
// im Kopf genannte Größe fassen. Literale werden an dst + Größe begrenzt, Kopien nicht
// (wie im Original). Rückgabe: entpackte Größe, 0 bei falschem Kopf.
uint32_t jdlz_decompress(const uint8_t* src, uint8_t* dst);

}  // namespace nfsu
