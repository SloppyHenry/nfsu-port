// Namens-Prüfsumme des Spiels.
#pragma once
#include <cstdint>

namespace nfsu {

// orig: 0x59aa50 name_hash
// Startwert -1, je Byte h = h * 33 + byte. Terminator ist ein 0-Byte.
uint32_t name_hash(const char* s);

}  // namespace nfsu
