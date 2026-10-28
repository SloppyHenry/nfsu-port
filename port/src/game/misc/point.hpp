#pragma once
// ../indep/src/misc/point.cpp: kubische Interpolation (tCubic1D/3D) und cPoint.
#include <cstdint>

#include "game/base/bmath.hpp"

namespace nfsu {

// GC-Layout (gc_types.h), Größe 0x30. Auf PC gleich (Offsets aus Zugriffen in 0x56ef60 bestätigt).
struct tCubic1D {
    float Val;             // 0x00
    float dVal;            // 0x04
    float ValDesired;      // 0x08
    float dValDesired;     // 0x0c
    float Coeff[4];        // 0x10
    float time;            // 0x20
    float duration;        // 0x24
    float arrivalThreshold;// 0x28
    int16_t state;         // 0x2c  0 = ruhend, 1 = läuft, 2 = Start angefordert
    int16_t flags;         // 0x2e

    void MakeCoeffs();
    float GetVal(float t) const;
    float GetdVal(float t) const;
    float GetddVal(float t) const;
    void ClampDerivative(float fMag);
    void ClampSecondDerivative(float fMag);
    void Update(float fSeconds, float fDClamp, float fDDClamp);
};
static_assert(sizeof(tCubic1D) == 0x30);

struct tCubic3D {
    tCubic1D x, y, z;

    void SetValDesired(float vx, float vy, float vz);
    void SetValDesired(const bVector3* v) { SetValDesired(v->x, v->y, v->z); }
    void GetVal(bVector3* out, float t) const;
    void Update(float fSeconds, float fDClamp, float fDDClamp);
};
static_assert(sizeof(tCubic3D) == 0x90);

// cPoint (Größe 0x198) kommt mit der Kamera/NIS (WP-36); hier nur die Suchfunktionen, die auf tCubic wirken.
struct cPoint {
    // orig: 0x56f0e0 cPoint::SplineSeek(tCubic1D*, float, float, float): wie tCubic1D::Update, aber ohne den Schutz gegen
    // duration <= 1e-5 (dann entsteht Inf/NaN wie im Original)
    static void SplineSeek(tCubic1D* p, float time, float fDClamp, float fDDClamp);
    // orig: 0x56f1e0 cPoint::SplineSeek(tCubic3D*, float): alle drei Achsen ohne Begrenzung
    static void SplineSeek(tCubic3D* p, float time);
};

}  // namespace nfsu
