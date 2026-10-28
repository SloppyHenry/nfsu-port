#include "game/misc/spline.hpp"

namespace nfsu {

// orig: 0x8018db38 (GC) Spline::Spline(SplineType,int,int)
Spline::Spline(SplineType type, int expected_number_of_control_points, int table_size) {
    Type = type;
    Dirty = 1;
    ControlPointBufferSize = expected_number_of_control_points;
    TableSize = table_size;
}

// orig: 0x8018db7c (GC) Spline::Spline(SplineType,int,bVector3*,int)
Spline::Spline(SplineType type, int number_of_control_points, bVector3* control_point_table, int table_size) {
    Type = type;
    NumControlPoints = number_of_control_points;
    Dirty = 1;
    TableSize = table_size;
    pControlPoints = control_point_table;
    ControlPointBufferSize = number_of_control_points;
}

// orig: 0x55f0a0 Spline::~Spline
Spline::~Spline() {
    delete[] pControlPointArray;
    delete[] pCalTable;
}

// orig: 0x8018dc14 (GC) Spline::SetControlPoints
void Spline::SetControlPoints(bVector3* pPoints, int nPoints) {
    if (pControlPointArray) {
        delete[] pControlPointArray;
        pControlPointArray = nullptr;
    }
    NumControlPoints = nPoints;
    pControlPoints = pPoints;
}

// orig: 0x8018dc64 (GC) Spline::SetControlPoint (pad bleibt unberührt)
void Spline::SetControlPoint(int i, const bVector3* p) {
    pControlPoints[i].x = p->x;
    pControlPoints[i].z = p->z;
    pControlPoints[i].y = p->y;
}

// orig: 0x55f0c0 Spline::AddControlPoint
// Eine fremde Tabelle (pControlPoints != Array) wird bei freiem Platz direkt beschrieben, erst bei Überlauf kopiert.
void Spline::AddControlPoint(const bVector3* p) {
    Dirty = 1;
    bool append = false;
    if (pControlPoints) {
        if (NumControlPoints != ControlPointBufferSize)
            append = true;
        else
            ControlPointBufferSize <<= 1;
    }
    if (!append) {
        // Port-Absicherung: Das Original würde bei Puffergröße 0 über das Ende schreiben.
        if (ControlPointBufferSize < 1) ControlPointBufferSize = 1;
        bVector3* fresh = new bVector3[ControlPointBufferSize]();
        for (int i = 0; i < NumControlPoints; ++i) fresh[i] = pControlPoints[i];
        delete[] pControlPointArray;
        pControlPoints = fresh;
        pControlPointArray = fresh;
    }
    bVector3* dst = &pControlPoints[NumControlPoints];
    ++NumControlPoints;
    dst->x = p->x;
    dst->y = p->y;
    dst->z = p->z;
}

// orig: 0x55f1a0 Spline::GetControlPoint
const bVector3* Spline::GetControlPoint(float t) {
    static bVector3 vTemp;  // orig: 0x780dc0
    float limit;
    switch (Type) {
        case OVERHAUSER_LOOP: limit = static_cast<float>(NumControlPoints); break;
        case OVERHAUSER_LINE:
        case OVERHAUSER_EXTRAPOLATED: limit = static_cast<float>(NumControlPoints - 1); break;
        default: limit = 0.0f; break;
    }
    float idx = t;
    if (Type == OVERHAUSER_LOOP) {
        idx = bFMod(t, limit);
    } else if (Type == OVERHAUSER_LINE) {
        if (t < 0.0f)
            idx = 0.0f;
        else if (limit < t)
            idx = limit;
    } else if (Type == OVERHAUSER_EXTRAPOLATED) {
        if (t < 0.0f) {
            const bVector3* a = pControlPoints;
            vTemp.x = (a[1].x - a[0].x) * t + a[0].x;
            vTemp.y = a[0].y + (a[1].y - a[0].y) * t;
            vTemp.z = a[0].z + (a[1].z - a[0].z) * t;
            return &vTemp;
        }
        if (limit < t) {
            const bVector3* a = pControlPoints;
            int n = NumControlPoints;
            float over = t - limit;
            vTemp.x = (a[n - 1].x - a[n - 2].x) * over + a[n - 1].x;
            vTemp.y = a[n - 1].y + over * (a[n - 1].y - a[n - 2].y);
            vTemp.z = a[n - 1].z + over * (a[n - 1].z - a[n - 2].z);
            return &vTemp;
        }
    }
    return pControlPoints + static_cast<int>(idx);
}

// orig: 0x55f3c0 Spline::Evaluate
bVector3* Spline::Evaluate(bVector3* pV, float fFloor, float b0, float b1, float b2, float b3) {
    bVector3 acc;
    const bVector3* c = GetControlPoint(fFloor - 1.0f);
    acc.x = c->x * b0;
    acc.y = c->y * b0;
    acc.z = c->z * b0;
    c = GetControlPoint(fFloor);
    bScaleAdd(&acc, &acc, c, b1);
    c = GetControlPoint(fFloor + 1.0f);
    bScaleAdd(&acc, &acc, c, b2);
    c = GetControlPoint(fFloor + 2.0f);
    bScaleAdd(&acc, &acc, c, b3);
    pV->z = acc.z;
    pV->x = acc.x;
    pV->y = acc.y;
    return pV;
}

// orig: 0x55f490 Spline::GetPoint (Basis aus GC 0x8018e13c)
bVector3* Spline::GetPoint(bVector3* point, float parameter) {
    float fl = bFloorF(parameter);
    float u = parameter - fl;
    float u2 = u * u;
    float u3 = u2 * u;
    return Evaluate(point, fl, (((-u3 + u2 + u2) - u) + 0.0f) * 0.5f, ((u3 * 3.0f - u2 * 5.0f) + 0.0f + 2.0f) * 0.5f,
                    (u2 * 4.0f + -(u3 * 3.0f) + u + 0.0f) * 0.5f, ((u3 - u2) + 0.0f + 0.0f) * 0.5f);
}

// orig: 0x55f540 Spline::GetDerivative (Basis aus GC 0x8018e23c)
bVector3* Spline::GetDerivative(bVector3* slope, float parameter) {
    float fl = bFloorF(parameter);
    float u = parameter - fl;
    float u2 = u * u;
    return Evaluate(slope, fl, ((u2 * -3.0f + u * 4.0f) - 1.0f) * 0.5f, (u2 * 9.0f - u * 10.0f) * 0.5f,
                    (u2 * -9.0f + u * 8.0f + 1.0f) * 0.5f, (u2 * 3.0f - (u + u)) * 0.5f);
}

// orig: 0x55f600 Spline::GetSecondDerivative (Basis aus GC 0x8018e334)
bVector3* Spline::GetSecondDerivative(bVector3* ddv, float parameter) {
    float fl = bFloorF(parameter);
    float u = parameter - fl;
    return Evaluate(ddv, fl, (u * -6.0f + 4.0f) * 0.5f, (u * 18.0f - 10.0f) * 0.5f, (u * -18.0f + 8.0f) * 0.5f,
                    (u * 6.0f - 2.0f) * 0.5f);
}

// orig: 0x55f6a0 Spline::LinearEstimateLength
float Spline::LinearEstimateLength() {
    if (Dirty) {
        Length = 0.0f;
        for (int i = 1; i < NumControlPoints; ++i) Length = bDistBetween(&pControlPoints[i], &pControlPoints[i - 1]) + Length;
        Dirty = 0;
    }
    return Length;
}

// orig: 0x55f700 Spline::NormalDot
// Skalarprodukt aus der normierten Richtung (Kurvenpunkt -> p) und der normierten Tangente bei fParam.
float Spline::NormalDot(const bVector3* p, float fParam) {
    bVector3 pt, tan;
    GetPoint(&pt, fParam);
    GetDerivative(&tan, fParam);
    pt.x -= p->x;
    pt.y -= p->y;
    pt.z -= p->z;
    bNormalize(&pt, &pt);
    bNormalize(&tan, &tan);
    return tan.y * pt.y + tan.z * pt.z + pt.x * tan.x;
}

// orig: 0x55f7a0 Spline::GetClosestParameter
// Sucht den Parameter, bei dem NormalDot den Wert fDotDesired schneidet: erst Vorzeichenwechsel zwischen
// Stützpunkten (bei mehreren der mit dem kleinsten Abstand zur Segmentmitte), dann bis zu 10 Halbierungsschritte.
// Rückgabe -1 vor dem Anfang, sonst eine Position hinter dem Ende (je nach Typ).
float Spline::GetClosestParameter(const bVector3* p, float fDotDesired, float fTolerance) {
    float prev = NormalDot(p, 0.0f) - fDotDesired;
    float best = 0.0f;
    float bestDist = 0.0f;
    float cur = 0.0f;
    if (NumControlPoints > 1) {
        for (int i = 1; i < NumControlPoints; ++i) {
            cur = NormalDot(p, static_cast<float>(i)) - fDotDesired;
            if (prev * cur < 0.0f) {
                const bVector3* b = &pControlPoints[i];
                const bVector3* a = &pControlPoints[i - 1];
                bVector3 mid;
                mid.x = a->x * 0.5f;
                mid.y = a->y * 0.5f;
                mid.z = a->z * 0.5f;
                bScaleAdd(&mid, &mid, b, 0.5f);
                float dist = bDistBetween(p, &mid);
                if (best == 0.0f || dist < bestDist) {
                    bestDist = dist;
                    best = static_cast<float>(i);
                }
            }
            prev = cur;
        }
        if (best != 0.0f) {
            float lo = static_cast<float>(static_cast<int>(best) - 1);
            float hi = static_cast<float>(static_cast<int>(best));
            float mid = lo;
            float v = NormalDot(p, lo);
            float fLo = lo;  // untere Grenze
            if (fTolerance < std::fabs(v - fDotDesired)) {
                int iter = 0;
                float lastSign = cur;
                float lowEnd = fLo;
                float highEnd = hi;
                do {
                    if (iter > 9) break;
                    mid = (highEnd + lowEnd) * 0.5f;
                    v = NormalDot(p, mid) - fDotDesired;
                    ++iter;
                    float newLow = mid;
                    if (0.0f <= lastSign * v) {
                        lastSign = v;
                        newLow = lowEnd;
                        highEnd = mid;
                    }
                    lowEnd = newLow;
                } while (fTolerance < std::fabs(v));
            }
            return mid;
        }
    }
    if (0.0f <= prev) return -1.0f;
    if (Type == OVERHAUSER_LOOP) return static_cast<float>(NumControlPoints) + 1.0f;
    if (Type == OVERHAUSER_LINE || Type == OVERHAUSER_EXTRAPOLATED) return static_cast<float>(NumControlPoints - 1) + 1.0f;
    return 0.0f + 1.0f;
}

}  // namespace nfsu
