#include "core/hash.hpp"

namespace nfsu {

// orig: 0x59aa50 name_hash
uint32_t name_hash(const char* s) {
    uint32_t h = 0xFFFFFFFFu;
    for (auto p = reinterpret_cast<const unsigned char*>(s); *p; ++p) h = h * 0x21u + *p;
    return h;
}

}  // namespace nfsu
