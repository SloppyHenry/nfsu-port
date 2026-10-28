#pragma once
// ../indep/src/misc/LZCompress.cpp (WP-30.3). JLZ ('JDLZ', Version 2) steht in formats/jdlz.hpp.
#include <cstdint>

namespace nfsu {

// orig: 0x59dfb0 LZValidHeader: 'COMP' Version 1 oder 'JDLZ' Version 2
bool LZValidHeader(const uint8_t* packet);
// orig: 0x59deb0 OldLZDecompress ('COMP', 16-Byte-Kopf: +6 int16 == 1 heißt unkomprimiert, +0xc Gesamtgröße)
// Rückgabe: entpackte Bytes, 0 bei falschem Kopf. Kopien sind wie im Original nicht gegen dst begrenzt.
int32_t OldLZDecompress(const uint8_t* packet, uint8_t* dst);
// orig: 0x59dfe0 LZDecompress: wählt nach Kennung
int32_t LZDecompress(const uint8_t* packet, uint8_t* dst);

}  // namespace nfsu
