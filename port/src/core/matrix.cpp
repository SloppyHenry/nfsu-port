#include "core/matrix.hpp"

namespace nfsu {

// orig: 0x40a7c0 reset_identity_matrix
void reset_identity_matrix(float matrix[16], uint32_t zeroed[16]) {
    for (int i = 0; i < 16; ++i) zeroed[i] = 0;
    for (int i = 0; i < 16; ++i) matrix[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}

}  // namespace nfsu
