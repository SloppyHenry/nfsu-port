#include "formats/jdlz.hpp"

#include <cstddef>
#include <cstring>

namespace nfsu {

static uint32_t rd32(const uint8_t* p) { uint32_t v; std::memcpy(&v, p, 4); return v; }

// orig: 0x59dd70 jdlz_decompress
// Kopf: +0 'JDLZ', +4 Version (2), +8 entpackte Größe, +12 Gesamtgröße der Eingabe,
// +0x10 und +0x11 sind die ersten beiden Flag-Bytes, ab +0x12 folgen die Daten.
// Flag A (Bit 0): 0 = Literal, 1 = Kopie. Flag B (Bit 0) wählt bei einer Kopie die Form:
//   B=0: 2 Bytes, Länge (b0 & 0x1f) + 3, Abstand ((b0 & 0xe0) << 3 | b1) + 0x11
//   B=1: 2 Bytes, Länge ((b0 & 0xf0) << 4 | b1) + 3, Abstand (b0 & 0xf) + 1
// Ist ein Flag-Byte bis auf das Markerbit 0x100 aufgebraucht (== 1), wird das nächste Byte geladen.
uint32_t jdlz_decompress(const uint8_t* src, uint8_t* dst) {
    if (rd32(src) != 0x5a4c444au || src[4] != 2) return 0;
    const uint32_t out_size = rd32(src + 8);
    uint8_t* const end = dst + out_size;
    const uint8_t* in = src + 0x12;
    int32_t remaining = static_cast<int32_t>(rd32(src + 12)) - 0x12;
    uint32_t flag_a = src[0x10] | 0x100u;
    uint32_t flag_b = src[0x11] | 0x100u;
    uint8_t* out = dst;
    while (remaining != 0) {
        if ((flag_a & 1) == 0) {
            if (out < end) *out++ = *in;
            ++in;
            --remaining;
        } else {
            const uint8_t b0 = in[0], b1 = in[1];
            int len;
            uint32_t dist;
            if ((flag_b & 1) == 0) {
                len = (b0 & 0x1f) + 3;
                dist = (((b0 & 0xe0u) << 3) | b1) + 0x11;
            } else {
                len = static_cast<int>((((b0 & 0xf0u) << 4) | b1) + 3);
                dist = (b0 & 0xfu) + 1;
            }
            for (int i = 0; i < len; ++i) out[i] = out[i - static_cast<std::ptrdiff_t>(dist)];
            out += len;
            flag_b >>= 1;
            in += 2;
            remaining -= 2;
        }
        flag_a >>= 1;
        if (flag_a == 1) {
            flag_a = *in++ | 0x100u;
            --remaining;
        }
        if (flag_b == 1) {
            flag_b = *in++ | 0x100u;
            --remaining;
        }
    }
    return out_size;
}

}  // namespace nfsu
