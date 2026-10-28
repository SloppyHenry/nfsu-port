#include "game/misc/rumble.hpp"

namespace nfsu {

// orig: 0x554410 (PC "FUN_00554410"), GC tEnvelope::GetIndex
int tEnvelope::GetIndex(float x) const {
    int i = 0;
    if (count > 0) {
        while (i < count) {
            if (x < points[i][0]) break;
            ++i;
        }
        if (i > 0) --i;
    }
    return i;
}

bool tEnvelope::OutOfRange(float x) const { return x < points[0][0] || points[count - 1][0] <= x; }

// orig: GC 0x8017b9fc
float tEnvelope::GetSlope(int i) const {
    if (i >= 0 && i <= count - 2) return (points[i + 1][1] - points[i][1]) / (points[i + 1][0] - points[i][0]);
    return 0.0f;
}

// orig: 0x554440 sample_curve (GC 0x8017ba4c tEnvelope::GetValue): außerhalb 0, sonst linear zwischen den Punkten
float tEnvelope::GetValue(float x) const {
    if (x < points[0][0] || points[count - 1][0] <= x) return 0.0f;
    int i = GetIndex(x);
    return (x - points[i][0]) * GetSlope(i) + points[i][1];
}

// orig: 0x5544c0
void tRumbler::Reset() {
    for (int i = 0; i < 3; ++i) {
        envelope[i] = nullptr;
        time[i] = 0.0f;
        scale[i] = 0.0f;
        priority[i] = 0;
    }
}

// orig: 0x5544f0 tRumbler::StartRumble
// Platzwahl: ein freier Platz mit Priorität <= prio gewinnt sofort. Sonst wird unter den Plätzen mit Priorität <= prio der
// mit der niedrigsten Priorität gewählt, bei Gleichstand der mit dem kleineren aktuellen Wert; ein Platz mit Priorität
// <= prio gilt nur dann als Ersatz, wenn seine Priorität unter prio liegt oder sein scale kleiner als das neue ist.
// (Das Original ist stdcall mit drei Stapelargumenten: Kurve, scale, Priorität.)
bool tRumbler::StartRumble(const tEnvelope* env, float newScale, signed char prio) {
    int best = -1;
    for (int i = 0; i < 3; ++i) {
        signed char cur = priority[i];
        if (cur <= prio) {
            if (!envelope[i]) {
                best = i;
                break;
            }
            if (best >= 0) {
                signed char bp = priority[best];
                if (bp < cur) continue;
                if (cur == bp) {
                    int vb = GetEnvelopeValue(best);
                    int vi = GetEnvelopeValue(i);
                    if (vb < vi) continue;
                }
            }
            if (prio <= priority[i]) {
                if (!(scale[i] < newScale)) continue;
            }
            best = i;
        }
    }
    if (best < 0) return false;
    scale[best] = newScale;
    envelope[best] = env;
    time[best] = 0.0f;
    priority[best] = prio;
    return true;
}

// orig: GC 0x8017bad0, PC inline: (int)(Kurve(time) * scale)
int tRumbler::GetEnvelopeValue(int i) const { return static_cast<int>(envelope[i]->GetValue(time[i]) * scale[i]); }

// orig: 0x5545c0 (PC "update_interpolators")
bool tRumbler::Update(float dt) {
    previous = current;
    current = 0;
    for (int i = 0; i < 3; ++i) {
        if (!envelope[i]) continue;
        if (envelope[i]->OutOfRange(time[i])) {
            envelope[i] = nullptr;
            priority[i] = -1;
        } else {
            int v = GetEnvelopeValue(i);
            if (v > current) current = v;
            time[i] = dt / scale[i] + time[i];
        }
    }
    if (current < 0) current = 0;
    if (current > 0xff) current = 0xff;
    return current != previous;
}

// orig: 0x554700 (zurücksetzen auf Standardwerte)
void tShaker::Reset() {
    dir = {0, 0, 0, 0};
    envelope = nullptr;
    time = 0.0f;
    duration = 1.0f;
    pulseRate = 17.0f;
}

// orig: 0x554730 tShaker::StartShaking; die Richtung wird umgekehrt gespeichert
void tShaker::StartShaking(const bVector3* d, float dur, float rate, const tEnvelope* env) {
    dir.x = d->x * -1.0f;
    dir.y = d->y * -1.0f;
    dir.z = d->z * -1.0f;
    envelope = env;
    time = 0.0f;
    duration = dur;
    pulseRate = rate;
}

// orig: GC 0x8017c0d0 (PC inline in 0x554780)
void tShaker::Update(float dt) {
    if (!envelope) return;
    time = time + dt / duration;
    if (envelope->OutOfRange(time)) envelope = nullptr;
}

float tShaker::GetAmplitude() const { return envelope ? envelope->GetValue(time) : 0.0f; }

void tShaker::GetValue(bVector3* out) const {
    double a = 0.0;
    bool odd = (static_cast<int>(time * pulseRate) & 1) != 0;
    if (!odd && envelope) a = envelope->GetValue(time);
    out->x = static_cast<float>(dir.x * a);
    out->y = static_cast<float>(dir.y * a);
    out->z = static_cast<float>(dir.z * a);
}

}  // namespace nfsu
