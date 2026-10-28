#include "game/misc/point.hpp"

namespace nfsu {

// orig: 0x801a4bd0 (GC) tCubic1D::MakeCoeffs; auf PC inline in Update (0x56ef60)
void tCubic1D::MakeCoeffs() {
    float d = ValDesired - Val;
    Coeff[2] = dVal;
    Coeff[3] = Val;
    Coeff[0] = (dValDesired + dVal) - (d + d);
    Coeff[1] = (d * 3.0f - dValDesired) - (dVal + dVal);
}

// orig: 0x801a4c18 (GC) tCubic1D::GetVal; PC inline
float tCubic1D::GetVal(float t) const { return ((Coeff[0] * t + Coeff[1]) * t + Coeff[2]) * t + Coeff[3]; }

// orig: 0x801a4c38 (GC) tCubic1D::GetdVal; PC inline
float tCubic1D::GetdVal(float t) const { return (Coeff[0] * 3.0f * t + Coeff[1] + Coeff[1]) * t + Coeff[2]; }

// orig: 0x801a4c60 (GC) tCubic1D::GetddVal; PC inline
float tCubic1D::GetddVal(float t) const { return Coeff[0] * 6.0f * t + Coeff[1] + Coeff[1]; }

// orig: 0x56ee40 tCubic1D::ClampDerivative
// Die Steigung am Ende der Kurve (t = 1/duration * duration) wird auf fMag begrenzt, indem dValDesired
// neu gesetzt wird. Rechenreihenfolge wie im PC-Maschinencode.
void tCubic1D::ClampDerivative(float fMag) {
    float inv = 1.0f / duration;
    float t = inv * duration;
    float d = ((Coeff[1] + Coeff[1] + t * Coeff[0] * 3.0f) * t + Coeff[2]) * inv;
    float a = d < 0.0f ? -d : d;
    if (fMag < a) dValDesired = (a / d) * fMag * duration;
}

// orig: 0x56ee90 tCubic1D::ClampSecondDerivative
// Zweite Ableitung am Anfang (a0) und Ende (a1) der Kurve auf +-fMag begrenzen und die Koeffizienten neu aufbauen.
void tCubic1D::ClampSecondDerivative(float fMag) {
    float inv = 1.0f / duration;
    float c1x2 = Coeff[1] + Coeff[1];
    float a0 = (inv * 0.0f * Coeff[0] * 6.0f + c1x2) * inv * inv;
    float abs0 = a0 < 0.0f ? -a0 : a0;
    float a1 = (inv * duration * Coeff[0] * 6.0f + c1x2) * inv * inv;
    if (fMag < abs0) a0 = (abs0 / a0) * fMag;
    float abs1 = a1 < 0.0f ? -a1 : a1;
    // x87-Vergleich: NaN (duration == 0) gilt als "<=" und verlässt die Funktion unverändert, daher negiert formuliert.
    if (!(abs1 > fMag)) {
        if (!(abs0 > fMag)) return;
    } else {
        a1 = (abs1 / a1) * fMag;
    }
    float d2 = duration * duration;
    a0 = a0 * d2;
    Coeff[1] = a0 * 0.5f;
    Coeff[0] = (d2 * a1 - a0) * 0.16666667f;
}

// orig: 0x56ef60 tCubic1D::Update
// PC weicht vom Namen ab (im Emulator belegt): ClampSecondDerivative bekommt fDClamp, nicht fDDClamp. Die Bedingung
// prüft fDDClamp > 0, begrenzt wird aber mit fDClamp (Fehler im Original, hier bewusst nachgebaut).
void tCubic1D::Update(float fSeconds, float fDClamp, float fDDClamp) {
    if (state == 0) return;
    if (state != 1) {
        if (state != 2) return;
        time = 0.0f;
        if (flags == 0) state = 1;
        if (0.0f < fDClamp) ClampDerivative(fDClamp);
        MakeCoeffs();
        if (0.0f < fDDClamp) ClampSecondDerivative(fDClamp);
    }
    if (duration <= 1e-05f)
        time = 1.0f;
    else
        time = fSeconds / duration + time;
    if (time < arrivalThreshold) {
        float t = time;
        float c = t * Coeff[0];
        Val = ((c + Coeff[1]) * t + Coeff[2]) * t + Coeff[3];
        dVal = (Coeff[1] + Coeff[1] + c * 3.0f) * t + Coeff[2];
    } else {
        state = 0;
        if (1.0f <= arrivalThreshold) {
            Val = ValDesired;
            dVal = dValDesired;
        }
    }
}

// orig: 0x47ad80 tCubic3D::SetValDesired
void tCubic3D::SetValDesired(float vx, float vy, float vz) {
    x.ValDesired = vx;
    if (vx != x.Val) x.state = 2;
    y.ValDesired = vy;
    if (vy != y.Val) y.state = 2;
    z.ValDesired = vz;
    if (vz != z.Val) z.state = 2;
}

// orig: 0x56f080 tCubic3D::GetVal(bVector3*, float)
void tCubic3D::GetVal(bVector3* out, float t) const {
    out->x = ((t * x.Coeff[0] + x.Coeff[1]) * t + x.Coeff[2]) * t + x.Coeff[3];
    out->y = ((t * y.Coeff[0] + y.Coeff[1]) * t + y.Coeff[2]) * t + y.Coeff[3];
    out->z = ((t * z.Coeff[0] + z.Coeff[1]) * t + z.Coeff[2]) * t + z.Coeff[3];
}

// orig: 0x801a5284 (GC) tCubic3D::Update; PC inline
void tCubic3D::Update(float fSeconds, float fDClamp, float fDDClamp) {
    x.Update(fSeconds, fDClamp, fDDClamp);
    y.Update(fSeconds, fDClamp, fDDClamp);
    z.Update(fSeconds, fDClamp, fDDClamp);
}

// orig: 0x56f0e0 cPoint::SplineSeek(tCubic1D*, ...) — die PC-Dekompilierung hält das Objekt für ein cPoint, der Zugriff
// (Val bei +0, State bei +0x2c) zeigt aber auf ein tCubic1D. Auch hier ClampSecondDerivative mit fDClamp.
void cPoint::SplineSeek(tCubic1D* p, float time, float fDClamp, float fDDClamp) {
    if (p->state == 0) return;
    if (p->state != 1) {
        if (p->state != 2) return;
        p->time = 0.0f;
        if (p->flags == 0) p->state = 1;
        if (0.0f < fDClamp) p->ClampDerivative(fDClamp);
        p->MakeCoeffs();
        if (0.0f < fDDClamp) p->ClampSecondDerivative(fDClamp);
    }
    p->time = time / p->duration + p->time;
    if (p->time < p->arrivalThreshold) {
        float t = p->time;
        float c = t * p->Coeff[0];
        p->Val = ((c + p->Coeff[1]) * t + p->Coeff[2]) * t + p->Coeff[3];
        p->dVal = (p->Coeff[1] + p->Coeff[1] + c * 3.0f) * t + p->Coeff[2];
    } else {
        p->state = 0;
        if (1.0f <= p->arrivalThreshold) {
            p->Val = p->ValDesired;
            p->dVal = p->dValDesired;
        }
    }
}

// orig: 0x56f1e0 cPoint::SplineSeek(tCubic3D*, float)
void cPoint::SplineSeek(tCubic3D* p, float time) {
    SplineSeek(&p->x, time, 0.0f, 0.0f);
    SplineSeek(&p->y, time, 0.0f, 0.0f);
    SplineSeek(&p->z, time, 0.0f, 0.0f);
}

}  // namespace nfsu
