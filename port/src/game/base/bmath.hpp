#pragma once
// Kleine bWare-Mathehelfer, die WP-30.1 braucht (Spline, Timer, Point).
// Die vollständige Basisbibliothek (BB_ObjsBWare.cpp) folgt in WP-30.2/30.3 und ersetzt diese Datei nicht,
// sondern ergänzt sie.
#include <cmath>
#include <cstdint>

namespace nfsu {

struct bVector2 {
    float x, y;
};

// orig: GC bVector3 (Größe 0x10, pad bleibt unberührt)
struct bVector3 {
    float x, y, z, pad;
};
static_assert(sizeof(bVector3) == 0x10);

// orig: 0x4361d0 bTruncF (Ghidra-Name "RaceParameters::GetDriverNumber"): Rückgabe von __ftol, d. h. Kürzen gegen 0.
inline float bTruncF(float v) { return static_cast<float>(static_cast<int32_t>(v)); }

// Boden wie im Original: Kürzen, dann bei Wert < Ergebnis um 1 verringern.
inline float bFloorF(float v) {
    float f = bTruncF(v);
    if (v < f) f -= 1.0f;
    return f;
}

// orig: 0x4361f0 bCeil (Ghidra "FUN_004361f0")
inline float bCeil(float v) {
    float f = bTruncF(v);
    if (f < v) f += 1.0f;
    return f;
}

// orig: 0x546e60 bFMod (Ghidra "FUN_00546e60"): Rest mit Vorzeichen des Dividenden-Brucharteils, Modul immer |m|.
inline float bFMod(float a, float m) {
    float am = std::fabs(m);
    float q = a / am;
    float f = bFloorF(q);
    return (q - f) * am;
}

// orig: 0x599e10 bNormalize (3 Komponenten; Nullvektor ergibt (1,0,0))
inline void bNormalize(bVector3* out, const bVector3* in) {
    float len = std::sqrt(in->z * in->z + in->y * in->y + in->x * in->x);
    if (len != 0.0f) {
        float inv = 1.0f / len;
        float z = in->z, y = in->y;
        out->x = inv * in->x;
        out->y = y * inv;
        out->z = z * inv;
        return;
    }
    out->x = 1.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}

// orig: 0x599fc0 bScaleAdd: out = a + b * s
inline void bScaleAdd(bVector3* out, const bVector3* a, const bVector3* b, float s) {
    float by = b->y, ay = a->y, bz = b->z, az = a->z;
    out->x = s * b->x + a->x;
    out->y = s * by + ay;
    out->z = s * bz + az;
}

// orig: bDistBetween (Abstand zweier Punkte, 3 Komponenten)
inline float bDistBetween(const bVector3* a, const bVector3* b) {
    float dx = a->x - b->x, dy = a->y - b->y, dz = a->z - b->z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

}  // namespace nfsu
