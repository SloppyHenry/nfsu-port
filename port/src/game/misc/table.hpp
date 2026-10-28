#pragma once
// ../indep/src/misc/Table.cpp: Tabellen, gleitende Mittel, PID-Fehler, Linear (WP-30.3).
// Laufzeitobjekte (ADR-001): native Zeiger. Die DoSnapshot-Funktionen warten auf ReplaySnapshot (WP-33.5).
#include <cstdint>
#include <memory>

#include "game/misc/graph.hpp"

namespace nfsu {

// orig: GC TableBase (0x14) + Table
struct Table {
    int NumEntries = 0;
    float MinArg = 0, MaxArg = 0, IndexMultiplier = 0;
    float* pTable = nullptr;
    // orig: 0x59ceb0 Table::GetValue: lineare Interpolation, außerhalb klemmen
    float GetValue(float x) const;
};

// orig: 0x59cf30 bLerp-artiger Helfer: *out = t * *a + (1 - t) * *b
void bBlend(float* out, const float* a, const float* b, float t);

// orig: 0x59d1e0 Average, PC-Layout: vptr 0, nSize 4, nSlots 5, nSamples 6, nCurrentSlot 7, fTotal 8, fAverage 0xc
struct Average {
    uint8_t nSize = 4, nSlots = 0, nSamples = 0, nCurrentSlot = 0;
    float fTotal = 0, fAverage = 0;
    float* pData = nullptr;
    float SmallDataBuffer[5] = {};

    explicit Average(int slots);
    virtual ~Average();  // orig: 0x59d260
    Average(const Average&) = delete;
    Average& operator=(const Average&) = delete;
    // orig: 0x59d320 Average::Record
    virtual void Record(float v);
    // orig: 0x59d380 (GC-Name unbekannt): alle Plätze auf v, Anzahl Messungen 0
    virtual void Reset(float v);
    // orig: 0x59d3c0 Average::Flush: alle Plätze auf v, Anzahl Messungen voll
    void Flush(float v);
};

// orig: 0x59d410 AverageWindow(float timeWindow, float frequency): Plätze = ftol(window * frequency)
struct AverageWindow : Average {
    float fTimeWindow;
    int iOldestValue = 0;
    float* pTimeData;
    AverageWindow(float timeWindow, float frequency);
    ~AverageWindow() override;  // orig: 0x59d4a0
    using Average::Record;
    // orig: 0x59d5b0 AverageWindow::Record(value, time)
    void Record(float v, float time);
    // orig: 0x59d560
    void Reset(float v) override;
};

// orig: 0x59d6c0 PidError (drei Mittel + Fehlerzustand)
struct PidError {
    Average aTimes, aIntegral, aDerivative;
    float fFrequency = 0, fCurrentError = 0, fPreviousError = 0;
    PidError(int slots) : aTimes(slots), aIntegral(slots), aDerivative(slots) {}
    // orig: 0x59d6c0 PidError::Record
    void Record(float error, float dt, bool skipDerivative, bool skipIntegral);
    // orig: 0x59d750: nur die Ableitung zurücksetzen
    void ResetDerivative(float v);
    // orig: 0x59d8a0 PidError::Reset
    void Reset(float v);
};

// orig: GC Linear
struct Linear {
    float m = 0, b = 0;
    // orig: 0x59d770 Linear::Init(x1, y1, x2, y2)
    void Init(float x1, float y1, float x2, float y2);
    // orig: 0x59d7a0 Linear::GetValue
    float GetValue(float x) const;
    // orig: 0x59d7b0 Linear::GetInverse (bei |m| <= 0.001 liefert es b, wie im Original)
    float GetInverse(float y) const;
};

}  // namespace nfsu
