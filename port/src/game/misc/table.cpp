#include "game/misc/table.hpp"

#include <cmath>
#include <cstdlib>

namespace nfsu {

float Table::GetValue(float x) const {
    int n = NumEntries;
    float f = (x - MinArg) * IndexMultiplier;
    int i = static_cast<int>(f);
    if (i < 0) return pTable[0];
    if (n - 1 <= i) return pTable[n - 1];
    double fl = i;
    if (f < fl) fl -= 1.0;
    double frac = f - fl;
    return static_cast<float>(frac * pTable[i + 1] + (1.0 - frac) * pTable[i]);
}

void bBlend(float* out, const float* a, const float* b, float t) { *out = t * *a + (1.0f - t) * *b; }

Average::Average(int slots) {
    nSlots = static_cast<uint8_t>(slots);
    pData = SmallDataBuffer;
    if (slots > 5) pData = static_cast<float*>(std::malloc(slots * 4));
    for (int i = 0; i < slots; ++i) pData[i] = 0.0f;
}
Average::~Average() {
    if (pData != SmallDataBuffer) std::free(pData);
}

void Average::Record(float v) {
    if (nSamples < nSlots) ++nSamples;
    int cur = nCurrentSlot;
    float t = v + fTotal;
    fTotal = t;
    fTotal = t - pData[cur];
    pData[cur] = v;
    uint8_t next = static_cast<uint8_t>(nCurrentSlot + 1);
    nCurrentSlot = next;
    fAverage = fTotal / static_cast<float>(nSamples);
    if (nSlots <= next) nCurrentSlot = 0;
}

void Average::Reset(float v) {
    for (int i = 0; i < nSlots; ++i) pData[i] = v;
    nSamples = 0;
    fTotal = static_cast<float>(nSlots) * v;
}

void Average::Flush(float v) {
    for (int i = 0; i < nSlots; ++i) pData[i] = v;
    nSamples = nSlots;
    fAverage = v;
    fTotal = static_cast<float>(nSlots) * v;
}

AverageWindow::AverageWindow(float timeWindow, float frequency)
    : Average(static_cast<int>(timeWindow * frequency)), fTimeWindow(timeWindow) {
    pTimeData = static_cast<float*>(std::malloc(nSlots * 4u));
    for (int i = 0; i < nSlots; ++i) pTimeData[i] = 0.0f;
}
AverageWindow::~AverageWindow() { std::free(pTimeData); }

void AverageWindow::Reset(float v) {
    for (int i = 0; i < nSlots; ++i) {
        pData[i] = v;
        pTimeData[i] = 0.0f;
    }
    nSamples = 0;
    iOldestValue = 0;
    nCurrentSlot = 0;
    fTotal = static_cast<float>(nSlots) * v;
}

void AverageWindow::Record(float v, float time) {
    if (nSamples < nSlots) ++nSamples;
    int cur = nCurrentSlot;
    float t = v + fTotal;
    fTotal = t;
    fTotal = t - pData[cur];
    pTimeData[cur] = time;
    pData[cur] = v;
    int n = 0;
    if (fTimeWindow < time - pTimeData[iOldestValue]) {
        do {
            if (nSlots <= n) break;
            if (0.0f < pTimeData[iOldestValue]) {
                fTotal -= pData[iOldestValue];
                pData[iOldestValue] = 0.0f;
                pTimeData[iOldestValue] = 0.0f;
                if (nSlots <= ++iOldestValue) iOldestValue = 0;
                --nSamples;
            }
            ++n;
        } while (fTimeWindow < time - pTimeData[iOldestValue]);
    }
    if (n != nSlots) {
        uint8_t next = static_cast<uint8_t>(nCurrentSlot + 1);
        nCurrentSlot = next;
        fAverage = fTotal / static_cast<float>(nSamples);
        if (nSlots <= next) nCurrentSlot = 0;
        return;
    }
    Reset(0.0f);
}

void PidError::Record(float error, float dt, bool skipDerivative, bool skipIntegral) {
    fPreviousError = fCurrentError;
    float d = error - fPreviousError;
    fCurrentError = error;
    float integral = skipIntegral ? 0.0f : (d * 0.5f + fPreviousError) * dt;
    float derivative = skipDerivative ? 0.0f : d / dt;
    aTimes.Record(dt);
    aIntegral.Record(integral);
    aDerivative.Record(derivative);
}

void PidError::ResetDerivative(float v) {
    fCurrentError = 0.0f;
    fPreviousError = v;
    aDerivative.Reset(v);
}

void PidError::Reset(float v) {
    fCurrentError = 0.0f;
    fPreviousError = v;
    aIntegral.Reset(v);
    fPreviousError = v;
    fCurrentError = 0.0f;
    aDerivative.Reset(v);
}

void Linear::Init(float x1, float y1, float x2, float y2) {
    float slope = (y2 - y1) / (x2 - x1);
    m = slope;
    b = y1 - slope * x1;
}
float Linear::GetValue(float x) const { return static_cast<float>(static_cast<double>(x) * m + b); }
float Linear::GetInverse(float y) const {
    if (std::fabs(m) <= 0.001f) return b;
    return static_cast<float>((static_cast<double>(y) - b) / m);
}

}  // namespace nfsu
