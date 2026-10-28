#include "game/misc/quickspline.hpp"

#include <cmath>
#include <cstring>

namespace nfsu {

bMatrix4 OverhauserBasisMatricies[3];
static int BasisMatrixInitDone = 0;  // orig: 0x7294a0
int nQuickSplineDistanceWorstCaseIterations = 0;
float fQuickSplineDistanceWorstCaseError = 0.0f;

// orig: 0x5775e0 InitBasisMatricies
void InitBasisMatricies() {
    if (BasisMatrixInitDone) return;
    static const float kRows[3][16] = {
        {0.0f, -1.0f, 2.0f, -1.0f, 2.0f, 0.0f, -5.0f, 3.0f, 0.0f, 1.0f, 4.0f, -3.0f, 0.0f, 0.0f, -1.0f, 1.0f},
        {-1.0f, 4.0f, -3.0f, 0.0f, 0.0f, -10.0f, 9.0f, 0.0f, 1.0f, 8.0f, -9.0f, 0.0f, 0.0f, -2.0f, 3.0f, 0.0f},
        {4.0f, -6.0f, 0.0f, 0.0f, -10.0f, 18.0f, 0.0f, 0.0f, 8.0f, -18.0f, 0.0f, 0.0f, -2.0f, 6.0f, 0.0f, 0.0f}};
    for (int k = 0; k < 3; ++k) {
        for (int i = 0; i < 16; ++i) OverhauserBasisMatricies[k].m[i] = kRows[k][i] * 0.5f;
        bTranspose(&OverhauserBasisMatricies[k], &OverhauserBasisMatricies[k]);
    }
    BasisMatrixInitDone = 1;
}

// orig: 0x5778d0 QuickSpline::SetNumControlPoints
void QuickSpline::SetNumControlPoints(int n) {
    ControlPointsDirty = 1;
    NumControlPoints = static_cast<uint16_t>(n);
    if (static_cast<int16_t>(n) == 0) {
        MaxParam = 0.0f;
        MinParam = 0.0f;
        return;
    }
    uint32_t u = static_cast<uint16_t>(n);
    if (EndPointType == QUICKSPLINE_LOOP) {
        MinParam = 0.0f;
        MaxParam = static_cast<float>(u);
    } else if (EndPointType == QUICKSPLINE_LINE) {
        MinParam = 0.0f;
        MaxParam = static_cast<float>(static_cast<int>(u) - 1);
    } else if (EndPointType == QUICKSPLINE_EXTRAPOLATED) {
        MinParam = -9999999.0f;
        MaxParam = 9999999.0f;
    }
}

// orig: 0x577940 QuickSpline::FixupEndpoints
void QuickSpline::FixupEndpoints() {
    if (NumControlPoints == 0) return;
    bVector4* cp = pControlPoints.get();
    int n = NumControlPoints;
    switch (EndPointType) {
        case QUICKSPLINE_LOOP:
            cp[-1] = cp[n - 1];
            cp[n] = cp[0];
            cp[n + 1] = cp[1];
            if (Length != 0.0f) {
                cp[-1].w = cp[n - 1].w - Length;
                cp[n].w = Length + cp[0].w;
                cp[n + 1].w = cp[1].w + Length;
            }
            break;
        case QUICKSPLINE_LINE:
            cp[-1] = cp[0];
            cp[n] = cp[n - 1];
            if (Length != 0.0f) {
                cp[-1].w = -cp[1].w;
                cp[n].w = (Length + Length) - cp[n - 1].w;
            }
            break;
        case QUICKSPLINE_EXTRAPOLATED: {
            bVector4 v{cp[1].x - cp[0].x, cp[1].y - cp[0].y, cp[1].z - cp[0].z, cp[1].w - cp[0].w};
            bScaleAdd(&cp[-1], &cp[0], &v, -1.0f);
            v = {cp[n - 1].x - cp[n - 2].x, cp[n - 1].y - cp[n - 2].y, cp[n - 1].z - cp[n - 2].z, cp[n - 1].w - cp[n - 2].w};
            bScaleAdd(&cp[n], &cp[n - 1], &v, 1.0f);
            if (Length != 0.0f) {  // PC: derselbe w-Abschnitt wie bei LINE (MW nur dort)
                cp[-1].w = -cp[1].w;
                cp[n].w = (Length + Length) - cp[n - 1].w;
            }
            break;
        }
    }
}

// orig: 0x577bd0 QuickSpline::ClampParam
float QuickSpline::ClampParam(float param) const {
    if (EndPointType == QUICKSPLINE_LOOP) {
        float L = MaxParam;
        float q = param / std::fabs(L);
        float f = bTruncF(q);
        if (q < f) f -= 1.0f;
        return (q - f) * std::fabs(L);
    }
    if (EndPointType == QUICKSPLINE_LINE) {
        float hi = MaxParam - 1e-07f;
        float v = MinParam;
        if (v < param) v = param;
        return hi < v ? hi : v;
    }
    return param;
}

// orig: 0x577b70 QuickSpline::EvaluateSpline (im Original zwei eMulVector = D3DXVec4Transform, Zeilenvektor mal Matrix)
void QuickSpline::EvaluateSpline(bVector4* point, float t, int deriv, const bVector4* cps) const {
    const float v[4] = {1.0f, t, t * t, t * t * t};
    const float* b = OverhauserBasisMatricies[deriv].m;
    float tmp[4];
    for (int j = 0; j < 4; ++j) tmp[j] = v[0] * b[j] + v[1] * b[4 + j] + v[2] * b[8 + j] + v[3] * b[12 + j];
    const float* c = &cps[0].x;
    float out[4];
    for (int j = 0; j < 4; ++j) out[j] = tmp[0] * c[j] + tmp[1] * c[4 + j] + tmp[2] * c[8 + j] + tmp[3] * c[12 + j];
    point->x = out[0];
    point->y = out[1];
    point->z = out[2];
    point->w = out[3];
}

// orig: 0x577c60 QuickSpline::GenerateExtrapolatedControlPoints
void QuickSpline::GenerateExtrapolatedControlPoints(bVector4* out, int first) const {
    const bVector4* cp = pControlPoints.get();
    int n = NumControlPoints;
    for (int k = 0; k < 4; ++k) {
        int idx = first + k;
        if (idx < -1) {
            bVector4 d{cp[0].x - cp[1].x, cp[0].y - cp[1].y, cp[0].z - cp[1].z, cp[0].w - cp[1].w};
            bScaleAdd(&out[k], &cp[0], &d, static_cast<float>(-idx));
        } else if (n < idx) {
            const bVector4& a = cp[n - 1];
            const bVector4& b = cp[n - 2];
            bVector4 d{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
            bScaleAdd(&out[k], &a, &d, static_cast<float>((1 - n) + idx));
        } else {
            out[k] = cp[idx];
        }
    }
}

// orig: 0x577d90 QuickSpline::GetPoint
bVector4* QuickSpline::GetPoint(bVector4* point, float param, int deriv) {
    if (ControlPointsDirty) {
        FixupEndpoints();
        ControlPointsDirty = 0;
    }
    if (NumControlPoints == 0) {
        point->x = point->y = point->z = point->w = 0.0f;
        return point;
    }
    float t = ClampParam(param);
    float fl = bTruncF(t);
    if (!(fl <= t)) fl -= 1.0f;
    int idx = static_cast<int>(fl);
    float frac = t - fl;
    bVector4 tmp[4];
    const bVector4* cps;
    if (EndPointType == QUICKSPLINE_EXTRAPOLATED && (idx < 0 || idx >= NumControlPoints - 1)) {
        GenerateExtrapolatedControlPoints(tmp, idx - 1);
        cps = tmp;
    } else {
        cps = pControlPoints.get() + idx - 1;
    }
    EvaluateSpline(point, frac, deriv, cps);
    return point;
}

// orig: 0x577e70 QuickSpline::FindClosestControlPoint
int QuickSpline::FindClosestControlPoint(const bVector3* p, int start) const {
    const bVector4* cp = pControlPoints.get();
    auto dist = [&](int i) { return bDistance(p, reinterpret_cast<const bVector3*>(&cp[i])); };
    float best = dist(start);
    int idx = start;
    for (int i = start + 1; i < NumControlPoints; ++i) {
        float d = dist(i);
        if (best <= d) break;
        best = d;
        idx = i;
    }
    for (int i = start - 1; i >= 0; --i) {
        float d = dist(i);
        if (!(d < best)) break;
        best = d;
        idx = i;
    }
    return idx;
}

// orig: 0x577f30 QuickSpline::FindClosestParam
float QuickSpline::FindClosestParam(const bVector3* p, float initial, float tolerance) {
    if (ControlPointsDirty) {
        FixupEndpoints();
        ControlPointsDirty = 0;
    }
    if (NumControlPoints == 0) return 0.0f;
    float t = initial, limit = 1.0f, prevStep = 0.0f, prevErr = 0.0f;
    for (int iter = 0x3e6; iter != 0; --iter) {
        bVector4 pos, tan;
        GetPoint(&pos, t, 0);
        GetPoint(&tan, t, 1);
        float len = std::sqrt(tan.x * tan.x + tan.y * tan.y + tan.z * tan.z);
        if (len == 0.0f) return t;
        float err = ((p->x - pos.x) * tan.x + (p->y - pos.y) * tan.y + (p->z - pos.z) * tan.z) / len;
        if (std::fabs(err) < tolerance) return t;
        float divisor = len;
        if (prevStep != 0.0f) {
            divisor = -((err - prevErr) / prevStep);
            if (divisor == 0.0f) return t;
        }
        float step = err / divisor;
        if (step <= -limit) step = -limit;
        if (limit < step) step = limit;
        t = ClampParam(step + t);
        if (step * prevStep < 0.0f) limit *= 0.5f;
        prevStep = step;
        prevErr = err;
    }
    return t;
}

// orig: 0x578130 QuickSpline::FindClosestLateralOffset
float QuickSpline::FindClosestLateralOffset(const bVector3* p, float initial, float* closest_param, float tolerance) {
    float c = FindClosestParam(p, initial, tolerance);
    bVector4 pos, tan;
    GetPoint(&pos, c, 0);
    GetPoint(&tan, c, 1);
    bVector3 tangent{tan.x, tan.y, tan.z, 0.0f};
    bVector3 up{0.0f, 0.0f, 1.0f, 0.0f};
    bVector3 diff{p->x - pos.x, p->y - pos.y, p->z - pos.z, 0.0f};
    bVector3 side;
    bCross(&side, &up, &tangent);
    bNormalize(&side, &side);
    if (closest_param) *closest_param = c;
    return static_cast<float>(static_cast<double>(diff.x) * side.x + static_cast<double>(side.y) * diff.y +
                              static_cast<double>(side.z) * diff.z);
}

// orig: 0x578230 QuickSpline::GetParam
float QuickSpline::GetParam(float distance) {
    float lo = MinParam, hi = MaxParam;
    if (EndPointType == QUICKSPLINE_LOOP) {
        hi = hi - 0.001f;
    } else if (EndPointType == QUICKSPLINE_EXTRAPOLATED) {
        lo = static_cast<float>(-static_cast<int>(NumControlPoints));
        hi = static_cast<float>(NumControlPoints);
        hi = hi + hi;
    }
    if (lo < hi) {
        bVector4 q;
        GetPoint(&q, lo, 0);
        float wlo = q.w;
        GetPoint(&q, hi, 0);
        float whi = q.w;
        if (whi != wlo && distance > wlo) {
            if (whi <= distance) return hi;
            int iter = 0;
            float mid = lo, err;
            do {
                float frac = (distance - wlo) / (whi - wlo);
                mid = frac * hi + (1.0f - frac) * lo;
                GetPoint(&q, mid, 0);
                ++iter;
                err = std::fabs(distance - q.w);
                if (err <= 0.001f) break;
                if (distance < q.w) {
                    whi = q.w;
                    hi = mid;
                }
                if (q.w < distance) {
                    wlo = q.w;
                    lo = mid;
                }
            } while (iter < 0x28);
            if (iter < nQuickSplineDistanceWorstCaseIterations) iter = nQuickSplineDistanceWorstCaseIterations;
            nQuickSplineDistanceWorstCaseIterations = iter;
            if (err < fQuickSplineDistanceWorstCaseError) err = fQuickSplineDistanceWorstCaseError;
            fQuickSplineDistanceWorstCaseError = err;
            return mid;
        }
    }
    return lo;
}

// orig: 0x5783f0 QuickSpline::CalibrateLength
void QuickSpline::CalibrateLength(int num_steps) {
    float total = 0.0f;
    for (int i = 0; i < NumControlPoints; ++i) {
        pControlPoints.get()[i].w = total;
        ControlPointsDirty = 1;
        float fi = static_cast<float>(i);
        if (fi < MaxParam) {
            bVector4 prev, cur;
            GetPoint(&prev, fi, 0);
            for (int k = 1; k <= num_steps; ++k) {
                GetPoint(&cur, static_cast<float>(k) / static_cast<float>(num_steps) + fi, 0);
                total = static_cast<float>(static_cast<double>(bDistance(reinterpret_cast<bVector3*>(&cur), reinterpret_cast<bVector3*>(&prev))) + total);
                prev = cur;
            }
        }
    }
    Length = total;
    FixupEndpoints();
    ControlPointsDirty = 0;
}

// orig: 0x578550 QuickSpline::HasKink
bool QuickSpline::HasKink() {
    if (ControlPointsDirty) {
        FixupEndpoints();
        ControlPointsDirty = 0;
    }
    int n = NumControlPoints + 3;
    if (n < 3) return false;
    const bVector4* p = pControlPointBuffer.get();
    bool kink = false;
    for (int i = 2; i < n; ++i) {
        float d = (p[i].x - p[i - 1].x) * (p[i - 1].x - p[i - 2].x) + (p[i].y - p[i - 1].y) * (p[i - 1].y - p[i - 2].y) +
                  (p[i].z - p[i - 1].z) * (p[i - 1].z - p[i - 2].z);
        if (d < 0.0f) kink = true;
    }
    return kink;
}

}  // namespace nfsu
