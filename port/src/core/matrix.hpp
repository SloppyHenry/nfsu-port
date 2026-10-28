#pragma once
#include <cstdint>

namespace nfsu {

// orig: 0x40a7c0 reset_identity_matrix
// Das Original schreibt in zwei Globals: 16 Floats ab 0x77d7e0 (Einheitsmatrix) und
// 16 Wörter ab 0x77d820 (null). Der Port bekommt beide Blöcke übergeben.
void reset_identity_matrix(float matrix[16], uint32_t zeroed[16]);

}  // namespace nfsu
