#pragma once
// ../indep/src/misc/Spline.cpp: Catmull-Rom-Spline ("Overhauser") über bVector3-Stützpunkte.
// Reines Laufzeitobjekt (ADR-001): native Zeiger, Layout nicht dateigebunden.
#include "game/base/bmath.hpp"

namespace nfsu {

enum SplineType { OVERHAUSER_LINE = 0, OVERHAUSER_LOOP = 1, OVERHAUSER_EXTRAPOLATED = 2 };

struct Spline {
    int ControlPointBufferSize = 0;
    int NumControlPoints = 0;
    SplineType Type = OVERHAUSER_LINE;
    bVector3* pControlPoints = nullptr;      // aktive Tabelle (eigene oder fremde)
    bVector3* pControlPointArray = nullptr;  // eigene Tabelle, die der Destruktor freigibt
    int Dirty = 1;
    int TableSize = 0;
    int CalLevel = 0;
    float Length = 0.0f;
    float CalTableStep = 0.0f;
    int CalTableSize = 0;
    float* pCalTable = nullptr;

    Spline(SplineType type, int expected_number_of_control_points, int table_size);
    Spline(SplineType type, int number_of_control_points, bVector3* control_point_table, int table_size);
    ~Spline();
    Spline(const Spline&) = delete;
    Spline& operator=(const Spline&) = delete;

    void SetControlPoints(bVector3* pPoints, int nPoints);
    void SetControlPoint(int i, const bVector3* p);
    void AddControlPoint(const bVector3* p);
    // Liefert einen Zeiger in die Tabelle bzw. auf einen internen statischen Puffer (Extrapolation).
    const bVector3* GetControlPoint(float t);
    bVector3* Evaluate(bVector3* pV, float fFloor, float b0, float b1, float b2, float b3);
    bVector3* GetPoint(bVector3* point, float parameter);
    bVector3* GetDerivative(bVector3* slope, float parameter);
    bVector3* GetSecondDerivative(bVector3* ddv, float parameter);
    float LinearEstimateLength();
    float NormalDot(const bVector3* p, float fParam);
    float GetClosestParameter(const bVector3* p, float fDotDesired, float fTolerance);
};

}  // namespace nfsu
