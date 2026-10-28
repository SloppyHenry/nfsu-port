#pragma once
// Rumble.cpp: Verlaufskurven (tEnvelope), Controller-Rütteln (tRumbler) und Kamerawackeln (tShaker).
#include "game/base/bmath.hpp"

namespace nfsu {

// Stützstellen als (x, y)-Paare, aufsteigend nach x.
struct tEnvelope {
    const float (*points)[2] = nullptr;
    int count = 0;

    int GetIndex(float x) const;       // orig: 0x554410 (GC 0x8017b974)
    bool OutOfRange(float x) const;    // x vor dem ersten oder ab dem letzten Punkt
    float GetSlope(int i) const;       // orig: GC 0x8017b9fc
    float GetValue(float x) const;     // orig: 0x554440 sample_curve
};

// Drei Plätze; der mit der höchsten Priorität bzw. stärksten Kurve gewinnt.
struct tRumbler {
    const tEnvelope* envelope[3];
    float time[3];
    float scale[3];     // Stärke: Wert = Kurve * scale; zugleich Teiler des Zeitfortschritts (dt / scale)
    signed char priority[3];
    int current = 0;    // Ausgabe 0..255
    int previous = 0;

    tRumbler() { Reset(); }
    void Reset();                                                         // orig: 0x5544c0
    bool StartRumble(const tEnvelope* env, float scale, signed char prio);  // orig: 0x5544f0
    int GetEnvelopeValue(int slot) const;                                    // orig: GC 0x8017bad0
    bool Update(float dt);                                                // orig: 0x5545c0, true bei Änderung
};

struct tShaker {
    bVector3 dir{0, 0, 0, 0};        // orig +0x00
    const tEnvelope* envelope = nullptr;  // +0x10
    float time = 0.0f;               // +0x14
    float duration = 1.0f;           // +0x18
    float pulseRate = 17.0f;         // +0x1c: ungerade Intervalle (time*rate) ergeben 0

    void Reset();                                                   // orig: 0x554700
    void StartShaking(const bVector3* d, float dur, float rate, const tEnvelope* env);  // orig: 0x554730
    void Update(float dt);                                          // orig: GC 0x8017c0d0
    float GetAmplitude() const;                                     // orig: GC 0x8017c04c
    void GetValue(bVector3* out) const;                             // orig: GC 0x8017c130
};

}  // namespace nfsu
