#pragma once
// ../indep/src/misc/QuickSpline.cpp: Spline aus dem SPLINE_PACK-Chunk (dateigebunden, ADR-001): Größe 0x2c,
// Zeigerfelder als ptr32. Die Basismatrizen liegen im Port statisch (kein Zeigerfeld in der Datei).
#include <cstdint>

#include "core/ptr32.hpp"
#include "game/base/bware.hpp"

namespace nfsu {

enum QuickSplineEndPointType { QUICKSPLINE_LOOP = 0, QUICKSPLINE_LINE = 1, QUICKSPLINE_EXTRAPOLATED = 2 };
enum QuickSplineBasisType { QUICKSPLINE_OVERHAUSER = 0 };

#pragma pack(push, 1)
struct QuickSpline {
    uint32_t nHash;                 // 0x00
    int32_t EndPointType;           // 0x04
    int32_t BasisType;              // 0x08
    float MaxParam;                 // 0x0c
    float MinParam;                 // 0x10
    float Length;                   // 0x14
    int8_t ControlPointsDirty;      // 0x18
    int8_t BufferWasAllocated;      // 0x19
    int8_t MinControlPoints;        // 0x1a
    int8_t pad1b;                   // 0x1b
    uint16_t MaxControlPoints;      // 0x1c
    uint16_t NumControlPoints;      // 0x1e
    ptr32<bVector4> pControlPointBuffer;  // 0x20  (ein Punkt vor pControlPoints)
    ptr32<bVector4> pControlPoints;       // 0x24
    uint32_t pBasisMatricies;       // 0x28  im Original Zeiger auf 3 Matrizen; im Port nicht benutzt

    // orig: 0x5778d0 QuickSpline::SetNumControlPoints
    void SetNumControlPoints(int n);
    // orig: 0x577940 QuickSpline::FixupEndpoints
    void FixupEndpoints();
    // orig: 0x577bd0 (ohne Namen, GC "ClampParam"): Parameter in den gültigen Bereich bringen
    float ClampParam(float param) const;
    // orig: 0x577b70 (GC "EvaluateSpline"): point = ((1,t,t²,t³) · Basis[deriv]) · cps
    void EvaluateSpline(bVector4* point, float t, int deriv, const bVector4* cps) const;
    // orig: 0x577c60 QuickSpline::GenerateExtrapolatedControlPoints: out[4] für die Indizes first..first+3
    void GenerateExtrapolatedControlPoints(bVector4* out, int first) const;
    // orig: 0x577d90 QuickSpline::GetPoint
    bVector4* GetPoint(bVector4* point, float param, int deriv = 0);
    // orig: 0x577e70 QuickSpline::FindClosestControlPoint
    int FindClosestControlPoint(const bVector3* point, int start) const;
    // orig: 0x577f30 QuickSpline::FindClosestParam
    float FindClosestParam(const bVector3* point, float initial, float tolerance);
    // orig: 0x578130 QuickSpline::FindClosestLateralOffset
    float FindClosestLateralOffset(const bVector3* point, float initial, float* closest_param, float tolerance);
    // orig: 0x578230 QuickSpline::GetParam: Parameter zur Strecke `distance` (w-Komponente der Stützpunkte)
    float GetParam(float distance);
    // orig: 0x5783f0 QuickSpline::CalibrateLength
    void CalibrateLength(int num_steps);
    // orig: 0x578550 (GC "HasKink"): Knick zwischen aufeinanderfolgenden Segmenten?
    bool HasKink();
};
#pragma pack(pop)
static_assert(sizeof(QuickSpline) == 0x2c);

// orig: 0x5775e0 InitBasisMatricies (Overhauser-Matrizen für Ableitung 0..2, transponiert)
void InitBasisMatricies();
extern bMatrix4 OverhauserBasisMatricies[3];  // orig: 0x72c4b0
extern int nQuickSplineDistanceWorstCaseIterations;   // orig: 0x7294a4
extern float fQuickSplineDistanceWorstCaseError;      // orig: 0x7294a8

}  // namespace nfsu
