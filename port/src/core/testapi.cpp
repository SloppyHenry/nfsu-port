// extern-"C"-Einstiege für den Emulator-Vergleich (tools/emu/compare.py). Nur Tests, nicht das Spiel.
#include <cstdint>
#include <cstring>

#include "core/hash.hpp"
#include "core/matrix.hpp"
#include "formats/jdlz.hpp"
#include "game/base/bware.hpp"
#include "game/misc/joystick.hpp"
#include "game/misc/lzcompress.hpp"
#include "game/misc/point.hpp"
#include "game/misc/table.hpp"
#include "game/misc/quickspline.hpp"
#include "game/misc/rumble.hpp"
#include "game/misc/timer.hpp"

extern "C" {
uint32_t t_name_hash(const char* s) { return nfsu::name_hash(s); }
void t_reset_identity_matrix(float* m, uint32_t* z) { nfsu::reset_identity_matrix(m, z); }
uint32_t t_jdlz_decompress(const uint8_t* src, uint8_t* dst) { return nfsu::jdlz_decompress(src, dst); }
void t_timer_hms(int32_t packed, int* out4) {
    nfsu::Timer t;
    t.PackedTime = packed;
    t.GetHoursMinsSeconds(&out4[0], &out4[1], &out4[2], &out4[3]);
}
float t_timer_rounded(int32_t packed, float iota) {
    nfsu::Timer t;
    t.PackedTime = packed;
    return t.GetSecondsRounded(iota);
}
void t_cubic1d_update(void* c48, float seconds, float dclamp, float ddclamp) {
    static_cast<nfsu::tCubic1D*>(c48)->Update(seconds, dclamp, ddclamp);
}
float t_envelope_value(const float* pts, int n, float x) {
    nfsu::tEnvelope e{reinterpret_cast<const float(*)[2]>(pts), n};
    return e.GetValue(x);
}
// Rumbler-Zustand als 0x30 Byte wie im Original: 3 Zeiger-Indizes (0 = leer, sonst i+1 in `envs`), Zeiten, Dauern, Prioritäten.
// Ablauf: StartRumble(env_index, dur, prio) -> Rückgabe, Zustand danach in out (3x time, 3x duration, 3x prio, 3x env-index).
int t_rumble_start(const float* pts, const int* ns, int nenv, const int* state_env, const float* state_time,
                   const float* state_dur, const signed char* state_prio, int new_env, float dur, signed char prio,
                   float* out_time, float* out_dur, signed char* out_prio, int* out_env) {
    nfsu::tEnvelope envs[8];
    int off = 0;
    for (int i = 0; i < nenv; ++i) {
        envs[i] = {reinterpret_cast<const float(*)[2]>(pts + off), ns[i]};
        off += ns[i] * 2;
    }
    nfsu::tRumbler r;
    for (int i = 0; i < 3; ++i) {
        r.envelope[i] = state_env[i] ? &envs[state_env[i] - 1] : nullptr;
        r.time[i] = state_time[i];
        r.scale[i] = state_dur[i];
        r.priority[i] = state_prio[i];
    }
    int ok = r.StartRumble(&envs[new_env], dur, prio);
    for (int i = 0; i < 3; ++i) {
        out_time[i] = r.time[i];
        out_dur[i] = r.scale[i];
        out_prio[i] = r.priority[i];
        out_env[i] = r.envelope[i] ? static_cast<int>(r.envelope[i] - envs) + 1 : 0;
    }
    return ok;
}
// Scanner-Vergleich: node[4] = {event, last, fresh, repeatAt}, cfgb[7] = {xor1, xor2, shift1, shift2, param, analog, valid}
int t_joy_scan(int kind, int* node, const unsigned char* cfgb, const unsigned* a, const unsigned* b, int type, const float* pts8,
               int hyst, int frame) {
    nfsu::JoystickSystem sys;
    sys.frame = frame;
    nfsu::Joystick& j = sys.joys[0];
    j.type = type;
    for (auto& ax : j.axis) {
        for (int i = 0; i < 4; ++i) ax.pts[i] = {pts8[i * 2], pts8[i * 2 + 1]};
        ax.hysteresis = static_cast<uint8_t>(hyst);
    }
    nfsu::ScannerConfig c{};
    c.Scanner = static_cast<nfsu::ScannerKind>(kind);
    c.Xor1 = cfgb[0]; c.Xor2 = cfgb[1]; c.Shift1 = cfgb[2]; c.Shift2 = cfgb[3]; c.Param = cfgb[4];
    c.AnalogDeviceNumber = cfgb[5]; c.ValidDuringReplay = cfgb[6];
    nfsu::EventNode n;
    n.Event = node[0]; n.Last = static_cast<uint32_t>(node[1]); n.Fresh = node[2]; n.RepeatAt = node[3];
    int r = sys.Scan(n, a, b, c, j);
    node[1] = static_cast<int>(n.Last); node[2] = n.Fresh; node[3] = n.RepeatAt;
    return r;
}
unsigned t_button_pair(const unsigned char* cfgb, const unsigned* a, const unsigned* b) {
    nfsu::ScannerConfig c{};
    c.Xor1 = cfgb[0]; c.Xor2 = cfgb[1]; c.Shift1 = cfgb[2]; c.Shift2 = cfgb[3];
    return nfsu::ReadButtonPair(c, a, b);
}
// GameClock-Vergleich: f[0..4] = {RealTimeElapsed, RealTimeElapsedFrame, QuantError, WorldTimeElapsed, WorldTimeElapsedFrame},
// i[0..7] = {RealTimeFrames, RealTimer, RealLoop, WorldTimeFrames, WorldTimer, WorldLoop, ntsc, maxFramesBits}. Aufruf wählt which.
void t_clock(int which, float* f, int* i, float arg) {
    nfsu::GameClock c;
    c.RealTimeElapsed = f[0]; c.RealTimeElapsedFrame = f[1]; c.RealTimeQuantError = f[2];
    c.WorldTimeElapsed = f[3]; c.WorldTimeElapsedFrame = f[4];
    c.RealTimeFrames = i[0]; c.RealTimer = i[1]; c.RealLoopCounter = i[2];
    c.WorldTimeFrames = i[3]; c.WorldTimer = i[4]; c.WorldLoopCounter = i[5];
    c.ntsc = i[6] != 0;
    c.maxFrames = 3.0f;
    if (which == 0) c.PrepareRealTimestep(arg);
    else if (which == 1) c.AdvanceRealTime();
    else c.AdvanceWorldTime();
    f[0] = c.RealTimeElapsed; f[1] = c.RealTimeElapsedFrame; f[2] = c.RealTimeQuantError;
    f[3] = c.WorldTimeElapsed; f[4] = c.WorldTimeElapsedFrame;
    i[0] = c.RealTimeFrames; i[1] = c.RealTimer; i[2] = c.RealLoopCounter;
    i[3] = c.WorldTimeFrames; i[4] = c.WorldTimer; i[5] = c.WorldLoopCounter;
    i[7] = c.RealTimeFramesElapsed; i[8] = c.WorldTimeFramesElapsed; i[9] = c.RealTimeQuantized;
}
void t_splineseek1d(void* c48, float t, float dclamp, float ddclamp) {
    nfsu::cPoint::SplineSeek(static_cast<nfsu::tCubic1D*>(c48), t, dclamp, ddclamp);
}
void t_splineseek3d(void* c144, float t) { nfsu::cPoint::SplineSeek(static_cast<nfsu::tCubic3D*>(c144), t); }

// ---- WP-30.2: bWare (Winkel, Zufall, Zeichenketten, Vektoren, Boxen, Matrizen)
float t_bsin(unsigned a) { return nfsu::bSin(static_cast<uint16_t>(a)); }
void t_bsincos(unsigned a, float* out2) { nfsu::bSinCos(&out2[0], &out2[1], static_cast<uint16_t>(a)); }
int t_basin(float v) { return nfsu::bASin(v); }
int t_batan(float y, float x) { return nfsu::bATan(y, x); }
int t_bfixatan1(int r) { return nfsu::bFixATan(r); }
int t_bfixatan2(int x, int y) { return nfsu::bFixATan(x, y); }
unsigned t_brandom(unsigned* state, unsigned n) { return nfsu::bRandomStep(state, n); }
float t_brandomf(unsigned* state, float scale) {
    nfsu::gRandomState = *state;
    float r = nfsu::bRandom(scale);
    *state = nfsu::gRandomState;
    return r;
}
int t_strfn(int which, const char* a, const char* b, int n) {
    switch (which) {
        case 0: return nfsu::bStrCmp(a, b);
        case 1: return nfsu::bStrNCmp(a, b, n);
        case 2: return nfsu::bStrICmp(a, b);
        default: return nfsu::bStringHash(a, n);
    }
}
void t_strcat(char* dst, const char* a, const char* b) { nfsu::bStrCat(dst, a, b); }
// a: Ergebnispuffer (16 float), b, c: Eingaben
int t_vecop(int which, float* a, const float* b, const float* c, float s) {
    using namespace nfsu;
    auto v2 = [](const float* f) { return reinterpret_cast<const bVector2*>(f); };
    auto v3 = [](const float* f) { return reinterpret_cast<const bVector3*>(f); };
    switch (which) {
        case 0: a[0] = bDistance(v3(b), v3(c)); return 0;
        case 1: bNormalize(reinterpret_cast<bVector2*>(a), v2(b)); return 0;
        case 2: bNormalize(reinterpret_cast<bVector2*>(a), v2(b), s); return 0;
        case 3: bNormalize(reinterpret_cast<bVector3*>(a), v3(b), s); return 0;
        case 4: bNormalize(reinterpret_cast<bVector3*>(a), reinterpret_cast<const bVector4*>(b)); return 0;
        case 5: bScaleAdd(reinterpret_cast<bVector2*>(a), v2(b), v2(c), s); return 0;
        case 6: bScaleAdd(reinterpret_cast<bVector4*>(a), reinterpret_cast<const bVector4*>(b), reinterpret_cast<const bVector4*>(c), s); return 0;
        case 7: return bEqual(v2(b), v2(c), s);
        case 8: return bEqual(v3(b), v3(c), s);
        default: bCross(reinterpret_cast<bVector3*>(a), v3(b), v3(c)); return 0;
    }
}
// Boxen: mn, mx (je 4 float, 2D/3D in-place), p, q Eingaben
int t_bbox(int which, float* mn, float* mx, const float* p, const float* q, float r) {
    using namespace nfsu;
    auto w2 = [](float* f) { return reinterpret_cast<bVector2*>(f); };
    auto w3 = [](float* f) { return reinterpret_cast<bVector3*>(f); };
    auto c2 = [](const float* f) { return reinterpret_cast<const bVector2*>(f); };
    auto c3 = [](const float* f) { return reinterpret_cast<const bVector3*>(f); };
    switch (which) {
        case 0: bInitializeBoundingBox(w2(mn), w2(mx)); return 0;
        case 1: bInitializeBoundingBox(w3(mn), w3(mx)); return 0;
        case 2: bInitializeBoundingBox(w2(mn), w2(mx), c2(p)); return 0;
        case 3: bInitializeBoundingBox(w3(mn), w3(mx), c3(p)); return 0;
        case 4: bExpandBoundingBox(w2(mn), w2(mx), c2(p)); return 0;
        case 5: bExpandBoundingBox(w3(mn), w3(mx), c3(p)); return 0;
        case 6: bExpandBoundingBox(w2(mn), w2(mx), c2(p), r); return 0;
        case 7: bExpandBoundingBox(w3(mn), w3(mx), c3(p), r); return 0;
        case 8: bExpandBoundingBox(w2(mn), w2(mx), c2(p), c2(q)); return 0;
        case 9: bExpandBoundingBox(w3(mn), w3(mx), c3(p), c3(q)); return 0;
        case 10: return bBoundingBoxIsInside(c3(mn), c3(mx), c3(p), r);
        case 11: return bBoundingBoxIsInside2D(c3(mn), c3(mx), c3(p), r);
        case 12: return bBoundingBoxContains(c2(mn), c2(mx), c2(p), c2(q));
        case 13: return bBoundingBoxOverlapping(c2(mn), c2(mx), c2(p), c2(q));
        default: return bBoundingBoxOverlapping(c3(mn), c3(mx), c3(p), c3(q));
    }
}
int t_poly(const float* pt, const float* verts, int n) {
    return nfsu::bPointInPolygon(reinterpret_cast<const nfsu::bVector2*>(pt), reinterpret_cast<const nfsu::bVector2*>(verts), n);
}
void t_matop(int which, float* out, const float* in) {
    using namespace nfsu;
    auto M = [](float* f) { return reinterpret_cast<bMatrix4*>(f); };
    auto C = [](const float* f) { return reinterpret_cast<const bMatrix4*>(f); };
    auto Q = [](const float* f) { return reinterpret_cast<const bQuaternion*>(f); };
    switch (which) {
        case 0: bIdentity(M(out)); break;
        case 1: bInvertAffine(M(out), C(in)); break;
        case 2: bTranspose(M(out), C(in)); break;
        case 3: bQuaternionGetMatrix(Q(in), M(out)); break;
        case 4: bQuaternionToMatrix(M(out), Q(in)); break;
        default: bMatrixToQuaternion(reinterpret_cast<bQuaternion*>(out), C(in)); break;
    }
}

// ---- WP-30.2: QuickSpline. blob = Arena (Objekt bei 0, Stützpunkte ab 0x40), in: 8 float, out: 16 float
int t_qs(int which, uint8_t* blob, uint32_t size, const float* in, float* out) {
    using namespace nfsu;
    InitBasisMatricies();
    g_arena = {blob, size};
    auto* q = reinterpret_cast<QuickSpline*>(blob);
    auto* pt = reinterpret_cast<const bVector3*>(in);
    int ret = 0;
    switch (which) {
        case 0: q->SetNumControlPoints(static_cast<int>(in[0])); break;
        case 1: q->FixupEndpoints(); break;
        case 2: out[0] = q->ClampParam(in[0]); break;
        case 3: q->GenerateExtrapolatedControlPoints(reinterpret_cast<bVector4*>(out), static_cast<int>(in[0])); break;
        case 4: q->GetPoint(reinterpret_cast<bVector4*>(out), in[0], static_cast<int>(in[1])); break;
        case 5: ret = q->FindClosestControlPoint(pt, static_cast<int>(in[3])); break;
        case 6: out[0] = q->FindClosestParam(pt, in[3], in[4]); break;
        case 7: out[0] = q->FindClosestLateralOffset(pt, in[3], &out[1], in[4]); break;
        case 8: out[0] = q->GetParam(in[0]); out[1] = fQuickSplineDistanceWorstCaseError; ret = nQuickSplineDistanceWorstCaseIterations; break;
        case 9: q->CalibrateLength(static_cast<int>(in[0])); break;
        case 10: ret = q->HasKink(); break;
        default: std::memcpy(out, OverhauserBasisMatricies, sizeof(OverhauserBasisMatricies)); break;
    }
    return ret;
}

// ---- WP-30.3: Table, Graph, Average, Linear, LZ
float t_table_value(int n, float mn, float idxmul, const float* tab, float x) {
    nfsu::Table t;
    t.NumEntries = n; t.MinArg = mn; t.IndexMultiplier = idxmul; t.pTable = const_cast<float*>(tab);
    return t.GetValue(x);
}
float t_graph_inverse(const float* pts, int n, float y) {
    nfsu::Graph g;
    g.Points = const_cast<nfsu::bVector2*>(reinterpret_cast<const nfsu::bVector2*>(pts)); g.NumPoints = n;
    return g.GetInverse(y);
}
// st = {nSamples, nCur, oldest}, f = {total, avg, window}
void t_avg(int which, int n, int* st, float* f, float* data, float* time, float a, float b) {
    using namespace nfsu;
    if (which <= 2) {
        Average av(n);
        av.nSamples = static_cast<uint8_t>(st[0]); av.nCurrentSlot = static_cast<uint8_t>(st[1]);
        av.fTotal = f[0]; av.fAverage = f[1];
        for (int i = 0; i < n; ++i) av.pData[i] = data[i];
        if (which == 0) av.Record(a); else if (which == 1) av.Reset(a); else av.Flush(a);
        st[0] = av.nSamples; st[1] = av.nCurrentSlot; f[0] = av.fTotal; f[1] = av.fAverage;
        for (int i = 0; i < n; ++i) data[i] = av.pData[i];
    } else {
        AverageWindow w(static_cast<float>(n), 1.0f);
        w.nSamples = static_cast<uint8_t>(st[0]); w.nCurrentSlot = static_cast<uint8_t>(st[1]); w.iOldestValue = st[2];
        w.fTotal = f[0]; w.fAverage = f[1]; w.fTimeWindow = f[2];
        for (int i = 0; i < n; ++i) { w.pData[i] = data[i]; w.pTimeData[i] = time[i]; }
        if (which == 3) w.Record(a, b); else w.Reset(a);
        st[0] = w.nSamples; st[1] = w.nCurrentSlot; st[2] = w.iOldestValue; f[0] = w.fTotal; f[1] = w.fAverage;
        for (int i = 0; i < n; ++i) { data[i] = w.pData[i]; time[i] = w.pTimeData[i]; }
    }
}
void t_linear(int which, float* mb, float a, float b, float c, float d) {
    nfsu::Linear l; l.m = mb[0]; l.b = mb[1];
    if (which == 0) { l.Init(a, b, c, d); mb[0] = l.m; mb[1] = l.b; }
    else if (which == 1) mb[2] = l.GetValue(a);
    else mb[2] = l.GetInverse(a);
}
void t_blend(float* out, const float* a, const float* b, float t) { nfsu::bBlend(out, a, b, t); }
int t_lz(const uint8_t* src, uint8_t* dst) { return nfsu::LZDecompress(src, dst); }
int t_lz_valid(const uint8_t* src) { return nfsu::LZValidHeader(src); }
}
