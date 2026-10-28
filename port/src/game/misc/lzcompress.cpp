#include "game/misc/lzcompress.hpp"

#include <cstring>

#include "formats/jdlz.hpp"

namespace nfsu {

static uint32_t rd32(const uint8_t* p) {
    uint32_t v;
    std::memcpy(&v, p, 4);
    return v;
}

bool LZValidHeader(const uint8_t* p) {
    if (rd32(p) == 0x504d4f43) return p[4] == 1;  // 'COMP'
    if (rd32(p) == 0x5a4c444a) return p[4] == 2;  // 'JDLZ'
    return false;
}

int32_t OldLZDecompress(const uint8_t* p, uint8_t* dst) {
    if (!p || !dst || rd32(p) != 0x504d4f43) return 0;
    int32_t total = static_cast<int32_t>(rd32(p + 12));
    int16_t stored;
    std::memcpy(&stored, p + 6, 2);
    const uint8_t* src = p + 16;
    const uint8_t* end = p + total;
    if (stored == 1) {
        std::memcpy(dst, src, total - 16);
        return total - 16;
    }
    uint8_t* out = dst;
    uint32_t flags = 1;
    while (src != end) {
        if (flags == 1) {
            flags = src[0] | 0x10000u | (static_cast<uint32_t>(src[1]) << 8);
            src += 2;
        }
        int count = (end - 0x20 < src) ? 1 : 16;  // am Ende nur noch ein Schritt je Flaggenwort
        do {
            if ((flags & 1) == 0) {
                *out++ = *src++;
            } else {
                uint8_t b = src[0];
                const uint8_t* from = out - (((b & 0xf0u) << 4) | src[1]);
                src += 2;
                *out++ = from[0];
                *out++ = from[1];
                *out++ = from[2];
                from += 3;
                for (uint32_t k = b & 0xf; k != 0; --k) *out++ = *from++;
            }
            flags >>= 1;
        } while (--count != 0);
    }
    return static_cast<int32_t>(out - dst);
}

int32_t LZDecompress(const uint8_t* p, uint8_t* dst) {
    if (!p || !dst) return 0;
    if (rd32(p) == 0x504d4f43) return OldLZDecompress(p, dst);
    if (rd32(p) == 0x5a4c444a) return static_cast<int32_t>(jdlz_decompress(p, dst));
    return 0;
}

}  // namespace nfsu
