// ../indep/bWare/Src: bMath, bVector, bMatrix, bQuaternion, bString, bRandom (WP-30.2).
// Gleitkomma: Das Original rechnet im x87-Stack; Zwischenergebnisse bleiben dort ungerundet. Die Ports rechnen dort
// in double, wo das Original mehrere Operationen ohne Speichern verkettet (bSin, bASin).
#include "game/base/bware.hpp"

#include <cfloat>
#include <cmath>
#include <cstring>

namespace nfsu {

// ---------------------------------------------------------------- Winkel
// orig: 0x546ea0 bSin
float bSin(uint16_t angle) {
    double x = static_cast<double>(angle) * 9.58738e-05f;
    double sign = 1.0;
    if (x < 4.712389f) {
        if (1.5707964f <= x) {
            x = x - 3.1415927f;
            sign = -1.0;
        }
    } else {
        x = x - 6.2831855f;
    }
    double x2 = x * x;
    double x5 = x2 * x * x2;  // x^5
    double x7 = x5 * x2;      // x^7
    double r = (x7 * x2 * 2.7557319e-06f + ((x5 * 0.008333334f + (x - x2 * x * 0.16666667f)) - x7 * 0.0001984127f)) * sign;
    return static_cast<float>(r);
}

// orig: 0x546f60 bSinCos
void bSinCos(float* s, float* c, uint16_t angle) {
    double x = static_cast<double>(angle) * 9.58738e-05f;
    double sign = 1.0;
    if (x < 4.712389f) {
        if (1.5707964f <= x) {
            x = x - 3.1415927f;
            sign = -1.0;
        }
    } else {
        x = x - 6.2831855f;
    }
    double x2 = x * x;
    double x5 = x2 * x * x2;
    double x6 = x2 * x2 * x2;
    double x7 = x5 * x2;
    double x8 = x6 * x2;
    *s = static_cast<float>((x7 * x2 * 2.7557319e-06f + ((x5 * 0.008333334f + (x - x2 * x * 0.16666667f)) - x7 * 0.0001984127f)) * sign);
    *c = static_cast<float>(((x8 * 2.4801588e-05f + ((x2 * x2 * 0.041666668f + (1.0 - x2 * 0.5f)) - x6 * 0.0013888889f)) -
                             x8 * x2 * 2.755732e-07f) * sign);
}

// __ftol des Originals: Kürzen gegen 0
static int32_t ftol(double v) { return static_cast<int32_t>(v); }

// orig: 0x547060 bASin
int16_t bASin(float v) {
    bool neg = v < 0.0f;
    float a = neg ? -v : v;
    if (!(a < 1.0f)) return static_cast<int16_t>(neg ? 0xC000 : 0x4000);
    int32_t n = ftol(static_cast<double>(a) * 65536.0f);
    int32_t width = 0x8000, bound = 0x8000, level = 0;
    while (n >= bound && level < 11) {
        width >>= 1;
        bound += width;
        ++level;
    }
    int32_t base = bound - width;
    int32_t sub = (n - base) >> (11 - level);
    int32_t rest = (width >> 4) * sub + base;
    int32_t idx = level * 16 + sub;
    const BAsinEntry& e = kBAsinTable[idx];
    float slope;
    std::memcpy(&slope, &e.slopeBits, 4);
    double t = static_cast<double>(rest) * 1.52587890625e-05f;
    int32_t d = ftol((static_cast<double>(a) - t) * slope * -65536.0f);
    uint16_t res = static_cast<uint16_t>(static_cast<uint16_t>(e.angle) - static_cast<uint16_t>(d));
    return static_cast<int16_t>(neg ? static_cast<uint16_t>(0x10000 - res) : res);
}

// orig: 0x547140 bATan(x, y): Winkel von (x, y) in 1/65536 Umdrehung, also atan2(y, x)
int32_t bATan(float x, float y) {
    int quad = x < 0.0f ? 1 : 0;
    if (y < 0.0f) quad ^= 3;
    float ax = std::fabs(x), ay = std::fabs(y);
    float local = 0.0f;
    float r2 = y * y + x * x;
    int32_t v;
    if (ax <= ay) {
        if (0.0001f < r2) local = (x == 0.0f) ? 0.0f : ax / std::sqrt(r2);
        v = 0x4000 - static_cast<uint16_t>(bASin(local));
    } else {
        if (0.0001f < r2) local = (y == 0.0f) ? 0.0f : ay / std::sqrt(r2);
        v = static_cast<uint16_t>(bASin(local));
    }
    switch (quad) {
        case 0: return v & 0xffff;
        case 3: return 0x10000 - v;
        case 1: return 0x8000 - v;
        default: return v + 0x8000;
    }
}

// orig: 0x547290 bFixATan(int)
int32_t bFixATan(int32_t ratio) {
    bool neg = ratio < 0;
    if (neg) ratio = -ratio;
    int32_t lo, hi, frac;
    if (ratio < 0x200000) {
        if (ratio < 0x40000) {
            lo = kBFixATanFine[ratio >> 11];
            hi = kBFixATanFine[(ratio >> 11) + 1];
            frac = (ratio & 0x7ff) << 5;
        } else {
            lo = kBFixATanCoarse[ratio >> 14];
            hi = kBFixATanCoarse[(ratio >> 14) + 1];
            frac = (ratio & 0x3fff) << 2;
        }
    } else if (ratio < 0x1000000) {
        lo = 0x3eba;
        hi = 0x3fd7;
        frac = ratio >> 8;
    } else {
        lo = hi = 0x3fff;
        frac = 0;
    }
    int32_t r = ((hi - lo) * frac >> 16) + lo;
    return neg ? -r : r;
}

// orig: 0x547330 bFixATan(int, int)
int32_t bFixATan(int32_t x, int32_t y) {
    int quad = 0;
    if (x < 0) {
        quad = 1;
        x = -x;
    }
    if (y < 0) {
        quad ^= 3;
        y = -y;
    }
    int32_t v;
    if (x <= (y >> 14)) {
        v = 0x4000;
    } else {
        int32_t ratio = 0;
        if (x != 0) {
            int64_t q = static_cast<int64_t>(0x7fffffff / x) * y;
            ratio = static_cast<int32_t>(static_cast<uint32_t>(q >> 16) << 1);
        }
        v = static_cast<uint16_t>(bFixATan(ratio));
    }
    switch (quad) {
        case 0: return v;
        case 1: return 0x8000 - v;
        case 2: return v - 0x8000;
        default: return -v;
    }
}

// ---------------------------------------------------------------- Zufall
uint32_t gRandomState = 0x12345678;  // orig: 0x6e46b4

uint32_t bRandomStep(uint32_t* state, uint32_t n) {
    uint32_t s = *state;
    uint32_t r = s % n;
    uint32_t a = s ^ 0x1d872b41u;
    uint32_t b = (a >> 5) ^ a;
    *state = (b << 27) ^ b ^ a;
    return r;
}

// orig: 0x546de0 bRandom
uint32_t bRandom(uint32_t n) { return bRandomStep(&gRandomState, n); }

// orig: 0x546e10 bRandom(float) (Ghidra "rng_next_scaled")
float bRandom(float scale) {
    uint32_t r = bRandomStep(&gRandomState, 0x7fffffffu);
    return static_cast<float>(static_cast<double>(r) * 4.656613e-10f * scale);
}

// ---------------------------------------------------------------- Zeichenketten
// orig: 0x59aa80 bStringHash
int32_t bStringHash(const char* s, int32_t seed) {
    uint32_t h = static_cast<uint32_t>(seed);
    for (; *s; ++s) h = h * 33u + static_cast<uint8_t>(*s);
    return static_cast<int32_t>(h);
}

// orig: 0x59ab30 bStrCmp
int32_t bStrCmp(const char* a, const char* b) {
    char ca, cb;
    do {
        ca = *a++;
        cb = *b++;
        if (ca == '\0' || cb == '\0') break;
    } while (ca == cb);
    return static_cast<int32_t>(ca) - static_cast<int32_t>(cb);
}

// orig: 0x59ab60 bStrNCmp
int32_t bStrNCmp(const char* a, const char* b, int32_t n) {
    char ca, cb;
    int32_t left;
    do {
        left = n - 1;
        if (n == 0 || (ca = *a) == '\0' || (cb = *b) == '\0') break;
        ++a;
        ++b;
        n = left;
    } while (ca == cb);
    if (left < 0) return 0;
    if (*a != '\0') return 1;
    if (*b != '\0') return -1;
    return static_cast<int32_t>(a[-1]) - static_cast<int32_t>(b[-1]);
}

// orig: 0x59abc0 bStrCat
void bStrCat(char* dst, const char* a, const char* b) {
    int32_t i = 0;
    for (; *a; ++a) dst[i++] = *a;
    for (; *b; ++b) dst[i++] = *b;
    dst[i] = '\0';
}

// orig: 0x59aad0 bStrCpy
void bStrCpy(char* dst, const char* src) {
    while ((*dst++ = *src++) != '\0') {
    }
}

// orig: 0x59ab00 bStrNCpy
void bStrNCpy(char* dst, const char* src, int32_t n) {
    for (; n != 0; --n) {
        char c = *src++;
        *dst++ = c;
        if (c == '\0') return;
    }
}

// orig: 0x59ac20 bStrICmp
int32_t bStrICmp(const char* a, const char* b) {
    uint8_t ca, cb;
    do {
        ca = static_cast<uint8_t>(*a++);
        if (static_cast<int8_t>(ca) > '`' && static_cast<int8_t>(ca) < '{') ca &= 0x5f;
        cb = static_cast<uint8_t>(*b++);
        if (static_cast<int8_t>(cb) > '`' && static_cast<int8_t>(cb) < '{') cb &= 0x5f;
    } while (ca != 0 && cb != 0 && ca == cb);
    return static_cast<int32_t>(static_cast<int8_t>(ca)) - static_cast<int32_t>(static_cast<int8_t>(cb));
}

// ---------------------------------------------------------------- Vektoren
// orig: 0x599d10 distance3
float bDistance(const bVector3* a, const bVector3* b) {
    double dx = static_cast<double>(a->x) - b->x, dy = static_cast<double>(a->y) - b->y, dz = static_cast<double>(a->z) - b->z;
    return static_cast<float>(std::sqrt(dx * dx + dy * dy + dz * dz));
}

// orig: 0x599d50 bNormalize(bVector2*, const bVector2*)
void bNormalize(bVector2* out, const bVector2* in) {
    float len = std::sqrt(in->x * in->x + in->y * in->y);
    if (len != 0.0f) {
        float iy = in->y, inv = 1.0f / len;
        out->x = inv * in->x;
        out->y = inv * iy;
        return;
    }
    out->x = 1.0f;
    out->y = 0.0f;
}

// orig: 0x599db0 bNormalize(bVector2*, const bVector2*, float)
void bNormalize(bVector2* out, const bVector2* in, float scale) {
    float len = std::sqrt(in->x * in->x + in->y * in->y) / scale;
    if (len != 0.0f) {
        float iy = in->y, inv = 1.0f / len;
        out->x = inv * in->x;
        out->y = inv * iy;
        return;
    }
    out->x = 1.0f;
    out->y = 0.0f;
}

// orig: 0x599e90 bNormalize(bVector3*, const bVector3*, float)
void bNormalize(bVector3* out, const bVector3* in, float scale) {
    float len = std::sqrt(in->z * in->z + in->y * in->y + in->x * in->x) / scale;
    if (len != 0.0f) {
        float inv = 1.0f / len, iz = in->z, iy = in->y;
        out->x = inv * in->x;
        out->y = iy * inv;
        out->z = iz * inv;
        return;
    }
    out->x = scale;
    out->y = 0.0f;
    out->z = 0.0f;
}

// orig: 0x599f10 bNormalize(bVector3*, const bVector4*)
void bNormalize(bVector3* out, const bVector4* in) {
    float len = std::sqrt(in->z * in->z + in->y * in->y + in->x * in->x + in->w + in->w);
    if (len != 0.0f) {
        float inv = 1.0f / len, iz = in->z, iy = in->y;
        out->x = inv * in->x;
        out->y = iy * inv;
        out->z = iz * inv;
        return;
    }
    out->x = 1.0f;
    out->y = 0.0f;
    out->z = 0.0f;
}

// orig: 0x599f90 bScaleAdd(bVector2*, ...)
void bScaleAdd(bVector2* out, const bVector2* a, const bVector2* b, float s) {
    float by = b->y, ay = a->y;
    out->x = s * b->x + a->x;
    out->y = s * by + ay;
}

// orig: 0x59a000 bScaleAdd(bVector4*, ...)
void bScaleAdd(bVector4* out, const bVector4* a, const bVector4* b, float s) {
    float by = b->y, ay = a->y, bz = b->z, az = a->z, bx = b->x, ax = a->x;
    out->w = s * b->w + a->w;
    out->x = s * bx + ax;
    out->y = s * by + ay;
    out->z = s * bz + az;
}

// orig: 0x59a050 bEqual(bVector2)
bool bEqual(const bVector2* a, const bVector2* b, float eps) {
    return std::fabs(a->x - b->x) <= eps && std::fabs(a->y - b->y) <= eps;
}

// orig: 0x59a090 bEqual(bVector3)
bool bEqual(const bVector3* a, const bVector3* b, float eps) {
    return std::fabs(a->x - b->x) <= eps && std::fabs(a->y - b->y) <= eps && std::fabs(a->z - b->z) <= eps;
}

// orig: 0x59a0e0 bCross (out = a × b)
void bCross(bVector3* out, const bVector3* a, const bVector3* b) {
    float bx = b->x, by = b->y, bz = b->z, ax = a->x, ay = a->y, az = a->z;
    out->x = bz * ay - by * az;
    out->y = bx * az - bz * ax;
    out->z = by * ax - ay * bx;
}

// ---------------------------------------------------------------- Bounding Box
void bInitializeBoundingBox(bVector2* mn, bVector2* mx) {  // orig: 0x59a120
    mn->x = mn->y = FLT_MAX;
    mx->x = mx->y = -FLT_MAX;
}
void bInitializeBoundingBox(bVector3* mn, bVector3* mx) {  // orig: 0x59a410
    mn->x = mn->y = mn->z = FLT_MAX;
    mx->x = mx->y = mx->z = -FLT_MAX;
}
void bInitializeBoundingBox(bVector2* mn, bVector2* mx, const bVector2* p) {  // orig: 0x59a150
    float x = p->x, y = p->y;
    mn->x = mx->x = x;
    mn->y = mx->y = y;
}
void bInitializeBoundingBox(bVector3* mn, bVector3* mx, const bVector3* p) {  // orig: 0x59a440
    float x = p->x, y = p->y, z = p->z;
    mn->x = mx->x = x;
    mn->y = mx->y = y;
    mn->z = mx->z = z;
}

static inline void minTo(float* a, float v) {
    if (v < *a) *a = v;
}
static inline void maxTo(float* a, float v) {
    if (*a < v) *a = v;
}

void bExpandBoundingBox(bVector2* mn, bVector2* mx, const bVector2* p) {  // orig: 0x59a170
    minTo(&mn->x, p->x);
    minTo(&mn->y, p->y);
    maxTo(&mx->x, p->x);
    maxTo(&mx->y, p->y);
}
void bExpandBoundingBox(bVector3* mn, bVector3* mx, const bVector3* p) {  // orig: 0x59a470
    minTo(&mn->x, p->x);
    minTo(&mn->y, p->y);
    minTo(&mn->z, p->z);
    maxTo(&mx->x, p->x);
    maxTo(&mx->y, p->y);
    maxTo(&mx->z, p->z);
}
void bExpandBoundingBox(bVector2* mn, bVector2* mx, const bVector2* p, float r) {  // orig: 0x59a1f0
    minTo(&mn->x, p->x - r);
    minTo(&mn->y, p->y - r);
    maxTo(&mx->x, r + p->x);
    maxTo(&mx->y, r + p->y);
}
void bExpandBoundingBox(bVector3* mn, bVector3* mx, const bVector3* p, float r) {  // orig: 0x59a510
    minTo(&mn->x, p->x - r);
    minTo(&mn->y, p->y - r);
    minTo(&mn->z, p->z - r);
    maxTo(&mx->x, r + p->x);
    maxTo(&mx->y, r + p->y);
    maxTo(&mx->z, r + p->z);
}
void bExpandBoundingBox(bVector2* mn, bVector2* mx, const bVector2* lo, const bVector2* hi) {  // orig: 0x59a280
    minTo(&mn->x, lo->x);
    minTo(&mn->y, lo->y);
    maxTo(&mx->x, hi->x);
    maxTo(&mx->y, hi->y);
}
void bExpandBoundingBox(bVector3* mn, bVector3* mx, const bVector3* lo, const bVector3* hi) {  // orig: 0x59a5e0
    minTo(&mn->x, lo->x);
    minTo(&mn->y, lo->y);
    minTo(&mn->z, lo->z);
    maxTo(&mx->x, hi->x);
    maxTo(&mx->y, hi->y);
    maxTo(&mx->z, hi->z);
}

// orig: 0x59a6a0
bool bBoundingBoxIsInside(const bVector3* mn, const bVector3* mx, const bVector3* p, float r) {
    return mn->x - r <= p->x && p->x <= r + mx->x && mn->y - r <= p->y && p->y <= r + mx->y && mn->z - r <= p->z &&
           p->z <= r + mx->z;
}
// orig: 0x59a310
bool bBoundingBoxIsInside2D(const bVector3* mn, const bVector3* mx, const bVector3* p, float r) {
    return mn->x - r <= p->x && p->x <= r + mx->x && mn->y - r <= p->y && p->y <= r + mx->y;
}
// orig: 0x59a370: Box (c..d) liegt in Box (a..b)
bool bBoundingBoxContains(const bVector2* a, const bVector2* b, const bVector2* c, const bVector2* d) {
    return a->x <= c->x && d->x <= b->x && a->y <= c->y && d->y <= b->y;
}
// orig: 0x59a3c0
bool bBoundingBoxOverlapping(const bVector2* a, const bVector2* b, const bVector2* c, const bVector2* d) {
    return c->x < b->x && a->x < d->x && c->y < b->y && a->y < d->y;
}
// orig: 0x59a720
bool bBoundingBoxOverlapping(const bVector3* a, const bVector3* b, const bVector3* c, const bVector3* d) {
    return c->x < b->x && a->x < d->x && c->y < b->y && a->y < d->y && c->z < b->z && a->z < d->z;
}

// orig: 0x59a790 (vom Compiler vierfach entrollt, hier die Schleife über Kanten (i-1, i))
bool bPointInPolygon(const bVector2* point, const bVector2* verts, int32_t n) {
    float px = point->x, py = point->y;
    bool inside = false;
    int32_t j = n - 1;
    for (int32_t i = 0; i < n; j = i++) {
        float yi = verts[i].y, yj = verts[j].y;
        bool cross = (yi > py && yj <= py) || (yi <= py && yj > py);
        if (!cross) continue;
        float xint = ((py - yi) * (verts[j].x - verts[i].x)) / (yj - yi) + verts[i].x;
        if (px < xint) inside = !inside;
    }
    return inside;
}

// ---------------------------------------------------------------- Matrix, Quaternion
// orig: 0x598300 bIdentity
void bIdentity(bMatrix4* o) {
    std::memset(o, 0, sizeof(*o));
    o->m[0] = o->m[5] = o->m[10] = o->m[15] = 1.0f;
}

// orig: 0x598340 bInvertAffine
void bInvertAffine(bMatrix4* out, const bMatrix4* in) {
    const float* m = in->m;
    float* o = out->m;
    float m0 = m[0], m1 = m[1], m2 = m[2], m4 = m[4], m5 = m[5], m6 = m[6], m8 = m[8], m9 = m[9], m10 = m[10];
    float t0 = m[12], t1 = m[13], t2 = m[14];
    float c0 = m5 * m10 - m9 * m6;
    float c1 = m10 * m4 - m8 * m6;
    float c2 = m9 * m4 - m8 * m5;
    float d = 1.0f / (c2 * m2 + (c0 * m0 - c1 * m1));
    o[3] = 0.0f;
    o[0] = c0 * d;
    o[1] = -((m10 * m1 - m9 * m2) * d);
    o[2] = (m6 * m1 - m5 * m2) * d;
    o[4] = -(c1 * d);
    o[7] = 0.0f;
    o[11] = 0.0f;
    o[15] = 1.0f;
    o[5] = (m10 * m0 - m8 * m2) * d;
    o[6] = -((m6 * m0 - m4 * m2) * d);
    o[8] = c2 * d;
    o[9] = -((m9 * m0 - m8 * m1) * d);
    o[10] = (m5 * m0 - m4 * m1) * d;
    o[12] = -(o[0] * t0 + o[4] * t1 + o[8] * t2);
    o[13] = -(o[1] * t0 + o[5] * t1 + o[9] * t2);
    o[14] = -(o[2] * t0 + o[6] * t1 + o[10] * t2);
}

// orig: 0x598550 bTranspose
void bTranspose(bMatrix4* out, const bMatrix4* in) {
    float t[16];
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) t[c * 4 + r] = in->m[r * 4 + c];
    std::memcpy(out->m, t, sizeof(t));
}

// orig: 0x599300 bQuaternion::GetMatrix
void bQuaternionGetMatrix(const bQuaternion* q, bMatrix4* out) {
    float* m = out->m;
    float x2 = q->x + q->x, y2 = q->y + q->y, z2 = q->z + q->z;
    float xx = x2 * q->x, yy = y2 * q->y, zy = z2 * q->y, zz = z2 * q->z;
    float xw = x2 * q->w, yw = y2 * q->w, zw = z2 * q->w;
    m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
    m[0] = 1.0f - (zz + yy);
    m[15] = 1.0f;
    m[1] = y2 * q->x - zw;
    m[2] = yw + z2 * q->x;
    m[4] = zw + y2 * q->x;
    m[5] = 1.0f - (zz + xx);
    m[6] = zy - xw;
    m[8] = z2 * q->x - yw;
    m[9] = xw + zy;
    m[10] = 1.0f - (yy + xx);
}

// orig: 0x599400
void bQuaternionToMatrix(bMatrix4* out, const bQuaternion* q) {
    float* m = out->m;
    float x = q->x, y = q->y, z = q->z, w = q->w;
    m[3] = m[7] = m[11] = m[12] = m[13] = m[14] = 0.0f;
    m[15] = 1.0f;
    float yy2 = y * y + y * y;
    float zz2 = z * z + z * z;
    m[0] = (1.0f - yy2) - zz2;
    float yx2 = y * x + y * x;
    float wz2 = w * z + w * z;
    m[1] = yx2 - wz2;
    float wy2 = w * y + w * y;
    float zx2 = z * x + z * x;
    m[2] = zx2 + wy2;
    m[4] = wz2 + yx2;
    float xx = 1.0f - (x * x + x * x);
    m[5] = xx - zz2;
    float zy2 = z * y + z * y;
    float wx2 = w * x + w * x;
    m[6] = zy2 - wx2;
    m[8] = zx2 - wy2;
    m[9] = wx2 + zy2;
    m[10] = xx - yy2;
}

// orig: 0x5994e0 bMatrixToQuaternion
void bMatrixToQuaternion(bQuaternion* q, const bMatrix4* mat) {
    const float* m = mat->m;
    float trace = m[10] + m[5] + m[0];
    if (0.0f < trace) {
        float s = std::sqrt(trace + 1.0f);
        q->w = s * 0.5f;
        s = 0.5f / s;
        q->x = (m[6] - m[9]) * s;
        q->y = (m[8] - m[2]) * s;
        q->z = (m[1] - m[4]) * s;
        return;
    }
    static const int next[3] = {1, 2, 0};
    int i = m[0] < m[5] ? 1 : 0;
    if (m[i * 5] < m[10]) i = 2;
    int j = next[i], k = next[j];
    float out[4];
    float s = std::sqrt((m[i * 5] - (m[k * 5] + m[j * 5])) + 1.0f);
    out[i] = s * 0.5f;
    if (s != 0.0f) s = 0.5f / s;
    out[3] = (m[k + j * 4] - m[j + k * 4]) * s;
    out[j] = (m[i + j * 4] + m[j + i * 4]) * s;
    out[k] = (m[i + k * 4] + m[k + i * 4]) * s;
    q->x = out[0];
    q->y = out[1];
    q->z = out[2];
    q->w = out[3];
}

}  // namespace nfsu
