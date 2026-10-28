#pragma once
// ../indep/bWare/Src: bMath, bVector, bMatrix, bQuaternion, bString, bRandom (WP-30.2, BB_ObjsBWare.cpp).
// Ergänzt bmath.hpp (bTruncF, bFMod, 3D-bNormalize, 3D-bScaleAdd aus WP-30.1).
#include <cstdint>

#include "game/base/bmath.hpp"

namespace nfsu {

struct bVector4 {
    float x, y, z, w;
};
// 4x4, zeilenweise: m[0..3] ist die erste Zeile (X-Achse), m[12..14] die Verschiebung.
struct bMatrix4 {
    float m[16];
};
struct bQuaternion {
    float x, y, z, w;
};
static_assert(sizeof(bMatrix4) == 0x40 && sizeof(bQuaternion) == 0x10);

// ---------------------------------------------------------------- Winkel (16 Bit = ganzer Kreis)
struct BAsinEntry {  // 8 Byte, Tabelle ab 0x6e46b8
    int16_t angle;
    int16_t pad;
    uint32_t slopeBits;  // float, als Bitmuster abgelegt
};
extern const BAsinEntry kBAsinTable[208];
extern const uint16_t kBFixATanFine[132];    // orig: 0x6e5148
extern const uint16_t kBFixATanCoarse[132];  // orig: 0x6e5250

// orig: 0x546ea0 bSin (Polynom 7. Grades, x87-Zwischenwerte; im Port double)
float bSin(uint16_t angle);
// orig: 0x546f40 (ohne Namen): Kosinus = bSin(ftol(x * 65536) ...) – nur der Rumpf, siehe .cpp
// orig: 0x546f60 bSinCos
void bSinCos(float* s, float* c, uint16_t angle);
// orig: 0x547060 bASin: Arkussinus per Tabelle, Ergebnis in 1/65536 Umdrehung (Rückgabe: unterste 16 Bit gültig)
int16_t bASin(float v);
// orig: 0x547140 bATan(x, y): Winkel von (x, y) in 1/65536 Umdrehung (atan2(y, x)); nur die unteren 16 Bit sind gültig
int32_t bATan(float x, float y);
// orig: 0x547290 bFixATan(int ratio16_16): Tabellensuche, Vorzeichen bleibt erhalten
int32_t bFixATan(int32_t ratio);
// orig: 0x547330 bFixATan(int x, int y)
int32_t bFixATan(int32_t x, int32_t y);

// ---------------------------------------------------------------- Zufall
// orig: 0x6e46b4 (Startwert 0x12345678)
extern uint32_t gRandomState;
// orig: 0x546de0 bRandom(n): Rest des Zustands durch n, danach Zustand weiterschalten
uint32_t bRandom(uint32_t n);
// orig: 0x546e10 bRandom(float): Gleitkommazahl in [0, scale)
float bRandom(float scale);
// Schrittfunktion ohne Global (Test)
uint32_t bRandomStep(uint32_t* state, uint32_t n);

// ---------------------------------------------------------------- Zeichenketten
// orig: 0x59aa80 bStringHash: h = h * 33 + c, Startwert frei wählbar
int32_t bStringHash(const char* s, int32_t seed);
// orig: 0x59ab30 bStrCmp (Vorzeichen aus signed char)
int32_t bStrCmp(const char* a, const char* b);
// orig: 0x59ab60 bStrNCmp
int32_t bStrNCmp(const char* a, const char* b, int32_t n);
// orig: 0x59abc0 bStrCat(dst, a, b): dst = a + b
void bStrCat(char* dst, const char* a, const char* b);
// orig: 0x59aad0 bStrCpy
void bStrCpy(char* dst, const char* src);
// orig: 0x59ab00 bStrNCpy (Ghidra "TableBase::SetMinMax"): höchstens n Zeichen, bricht nach dem Terminator ab, füllt nicht auf
void bStrNCpy(char* dst, const char* src, int32_t n);
// orig: 0x59ac20 bStrICmp (nur a..z werden gefaltet)
int32_t bStrICmp(const char* a, const char* b);

// ---------------------------------------------------------------- Vektoren
// orig: 0x599d10 bDistance3 (Ghidra "distance3")
float bDistance(const bVector3* a, const bVector3* b);
// orig: 0x599d50 bNormalize(bVector2*, const bVector2*)
void bNormalize(bVector2* out, const bVector2* in);
// orig: 0x599db0 bNormalize(bVector2*, const bVector2*, float): Länge geteilt durch scale
void bNormalize(bVector2* out, const bVector2* in, float scale);
// orig: 0x599e90 bNormalize(bVector3*, const bVector3*, float): wie oben, Nullfall liefert (scale, 0, 0)
void bNormalize(bVector3* out, const bVector3* in, float scale);
// orig: 0x599f10 bNormalize(bVector3*, const bVector4*): Länge = sqrt(x²+y²+z²+w+w) (so im Original, kein w²)
void bNormalize(bVector3* out, const bVector4* in);
// orig: 0x599f90 bScaleAdd(bVector2*, ...)
void bScaleAdd(bVector2* out, const bVector2* a, const bVector2* b, float s);
// orig: 0x59a000 bScaleAdd(bVector4*, ...)
void bScaleAdd(bVector4* out, const bVector4* a, const bVector4* b, float s);
// orig: 0x59a050 bEqual(bVector2)
bool bEqual(const bVector2* a, const bVector2* b, float eps);
// orig: 0x59a090 bEqual(bVector3)
bool bEqual(const bVector3* a, const bVector3* b, float eps);
// orig: 0x59a0e0 bCross: out = b × a (Reihenfolge wie im Original: out.x = b.z*a.y - b.y*a.z)
void bCross(bVector3* out, const bVector3* a, const bVector3* b);

// ---------------------------------------------------------------- Bounding Box
// orig: 0x59a120 / 0x59a410: min = +FLT_MAX, max = -FLT_MAX
void bInitializeBoundingBox(bVector2* mn, bVector2* mx);
void bInitializeBoundingBox(bVector3* mn, bVector3* mx);
// orig: 0x59a150 / 0x59a440: min = max = p
void bInitializeBoundingBox(bVector2* mn, bVector2* mx, const bVector2* p);
void bInitializeBoundingBox(bVector3* mn, bVector3* mx, const bVector3* p);
// orig: 0x59a170 / 0x59a470: Punkt aufnehmen
void bExpandBoundingBox(bVector2* mn, bVector2* mx, const bVector2* p);
void bExpandBoundingBox(bVector3* mn, bVector3* mx, const bVector3* p);
// orig: 0x59a1f0 / 0x59a510: Punkt mit Rand r aufnehmen
void bExpandBoundingBox(bVector2* mn, bVector2* mx, const bVector2* p, float r);
void bExpandBoundingBox(bVector3* mn, bVector3* mx, const bVector3* p, float r);
// orig: 0x59a280 / 0x59a5e0: Box (lo, hi) aufnehmen
void bExpandBoundingBox(bVector2* mn, bVector2* mx, const bVector2* lo, const bVector2* hi);
void bExpandBoundingBox(bVector3* mn, bVector3* mx, const bVector3* lo, const bVector3* hi);
// orig: 0x59a310 (nur x,y) / 0x59a6a0 (x,y,z): Punkt in Box mit Rand r
bool bBoundingBoxIsInside(const bVector3* mn, const bVector3* mx, const bVector3* p, float r);       // 3D
bool bBoundingBoxIsInside2D(const bVector3* mn, const bVector3* mx, const bVector3* p, float r);     // nur x,y
// orig: 0x59a370: 2D-Box (mn1..mx1) liegt komplett in (mn2..mx2)? Aufruf (outerMin, innerMin?, ...) siehe .cpp
bool bBoundingBoxContains(const bVector2* a, const bVector2* b, const bVector2* c, const bVector2* d);
// orig: 0x59a3c0 / 0x59a720: Überlappung
bool bBoundingBoxOverlapping(const bVector2* a, const bVector2* b, const bVector2* c, const bVector2* d);
bool bBoundingBoxOverlapping(const bVector3* a, const bVector3* b, const bVector3* c, const bVector3* d);
// orig: 0x59a790: Punkt-in-Polygon (Strahlverfahren über x, Ecken als (x,y)-Paare)
bool bPointInPolygon(const bVector2* point, const bVector2* verts, int32_t n);

// ---------------------------------------------------------------- Matrix, Quaternion
// orig: 0x598300 bIdentity
void bIdentity(bMatrix4* m);
// orig: 0x598340 bInvertAffine (orthonormal + Verschiebung; Ghidra "FUN_00598340", GC-Zuordnung falsch)
void bInvertAffine(bMatrix4* out, const bMatrix4* in);
// orig: 0x598550 bTranspose (GC-Zuordnung "bNormalize" stimmt nicht)
void bTranspose(bMatrix4* out, const bMatrix4* in);
// orig: 0x599300 bQuaternion::GetMatrix(bMatrix4*)
void bQuaternionGetMatrix(const bQuaternion* q, bMatrix4* out);
// orig: 0x599400 bQuaternionToMatrix(bMatrix4*, const bQuaternion*) (zweite Fassung, andere Rechenreihenfolge)
void bQuaternionToMatrix(bMatrix4* out, const bQuaternion* q);
// orig: 0x5994e0 bMatrixToQuaternion
void bMatrixToQuaternion(bQuaternion* q, const bMatrix4* m);

}  // namespace nfsu
