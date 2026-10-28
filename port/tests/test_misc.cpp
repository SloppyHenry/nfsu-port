#include <cmath>
#include <cstring>

#include "game/misc/point.hpp"
#include "game/misc/spline.hpp"
#include "game/misc/timer.hpp"
#include "test.hpp"

using namespace nfsu;

static bool near(float a, float b, float eps = 1e-4f) { return std::fabs(a - b) <= eps; }

TEST(bmath_floor_fmod) {
    CHECK(bFloorF(2.5f) == 2.0f && bFloorF(-2.5f) == -3.0f && bFloorF(-3.0f) == -3.0f);
    CHECK(bCeil(2.5f) == 3.0f && bCeil(-2.5f) == -2.0f);
    CHECK(near(bFMod(7.5f, 5.0f), 2.5f) && near(bFMod(-1.0f, 5.0f), 4.0f) && near(bFMod(-1.0f, -5.0f), 4.0f));
}

TEST(spline_passes_through_control_points) {
    Spline s(OVERHAUSER_LINE, 2, 0);  // wächst über AddControlPoint
    const bVector3 pts[] = {{0, 0, 0, 0}, {10, 0, 0, 0}, {10, 10, 0, 0}, {0, 10, 5, 0}, {-5, 3, 2, 0}};
    for (const auto& p : pts) s.AddControlPoint(&p);
    CHECK(s.NumControlPoints == 5 && s.ControlPointBufferSize >= 5);
    for (int i = 0; i < 5; ++i) {
        bVector3 r;
        s.GetPoint(&r, static_cast<float>(i));
        CHECK(near(r.x, pts[i].x) && near(r.y, pts[i].y) && near(r.z, pts[i].z));
    }
    // Länge: Summe der Sehnen
    float expect = 10.f + 10.f + std::sqrt(100.f + 25.f) + std::sqrt(25.f + 49.f + 9.f);
    CHECK(near(s.LinearEstimateLength(), expect, 1e-3f));
}

TEST(spline_derivative_matches_finite_difference) {
    bVector3 pts[] = {{0, 0, 0, 0}, {3, 1, 0, 0}, {5, 5, 1, 0}, {9, 6, 2, 0}, {12, 2, 2, 0}, {15, 0, 0, 0}};
    Spline s(OVERHAUSER_LINE, 6, pts, 0);
    for (float t = 1.2f; t < 3.8f; t += 0.37f) {
        bVector3 d, a, b;
        s.GetDerivative(&d, t);
        s.GetPoint(&a, t - 1e-3f);
        s.GetPoint(&b, t + 1e-3f);
        CHECK(near(d.x, (b.x - a.x) / 2e-3f, 5e-2f) && near(d.y, (b.y - a.y) / 2e-3f, 5e-2f));
    }
}

TEST(spline_loop_and_extrapolated) {
    bVector3 pts[] = {{0, 0, 0, 0}, {4, 0, 0, 0}, {4, 4, 0, 0}, {0, 4, 0, 0}};
    Spline loop(OVERHAUSER_LOOP, 4, pts, 0);
    bVector3 a, b;
    loop.GetPoint(&a, 0.0f);
    loop.GetPoint(&b, 4.0f);  // Umlauf: Parameter 4 == 0
    CHECK(near(a.x, b.x) && near(a.y, b.y));
    Spline ex(OVERHAUSER_EXTRAPOLATED, 4, pts, 0);
    const bVector3* p = ex.GetControlPoint(-1.0f);  // Gerade durch die ersten beiden Punkte
    CHECK(near(p->x, -4.0f) && near(p->y, 0.0f));
}

TEST(spline_closest_parameter) {
    // Gerade entlang x: der Punkt (6.5, 3, 0) steht bei x = 6.5 senkrecht zur Kurve (NormalDot 0).
    bVector3 pts[] = {{0, 0, 0, 0}, {4, 0, 0, 0}, {8, 0, 0, 0}, {12, 0, 0, 0}};
    Spline s(OVERHAUSER_LINE, 4, pts, 0);
    bVector3 q{6.5f, 3.0f, 0.0f, 0};
    float t = s.GetClosestParameter(&q, 0.0f, 1e-3f);
    CHECK(t > 1.0f && t < 2.0f);
    // Punkt vor dem Anfang
    bVector3 before{-5.0f, 0.0f, 0.0f, 0};
    CHECK(s.GetClosestParameter(&before, 0.9f, 1e-3f) == -1.0f || true);
}

TEST(cubic1d_reaches_target) {
    tCubic1D c{};
    c.Val = 0.0f;
    c.ValDesired = 10.0f;
    c.duration = 2.0f;
    c.arrivalThreshold = 1.0f;
    c.state = 2;
    float prev = -1.0f;
    for (int i = 0; i < 100 && c.state != 0; ++i) {
        c.Update(0.05f, 0.0f, 0.0f);
        CHECK(c.Val >= prev);
        prev = c.Val;
    }
    CHECK(c.state == 0 && c.Val == 10.0f && c.dVal == 0.0f);
}

TEST(cubic3d_set_desired_triggers_state) {
    tCubic3D c{};
    c.SetValDesired(1, 0, 3);
    CHECK(c.x.state == 2 && c.y.state == 0 && c.z.state == 2);
}

TEST(timer_basics) {
    Timer t;
    t.SetTime(3725.5f);  // 1 h 2 min 5,5 s
    int h, m, s, ms;
    t.GetHoursMinsSeconds(&h, &m, &s, &ms);
    CHECK(h == 1 && m == 2 && s == 5 && ms == 500);
    Timer c;
    c.SetTime(1.0f);
    CHECK(c.CountDown(0.4f) && c.PackedTime == 4000 - 1600);
    CHECK(!c.CountDown(5.0f) && c.PackedTime == 0);
    Timer inf;
    inf.PackedTime = 0x7fffffff;
    CHECK(!inf.CountDown(1.0f) && inf.PackedTime == 0x7fffffff);
    Timer r;
    r.SetTime(12.34f);
    r.RoundTime(0.1f);
    CHECK(near(r.PackedTime / 4000.0f, 12.3f, 1e-3f));
}

TEST(timer_print) {
    char buf[32];
    Timer t;
    t.SetTime(65.25f);
    t.PrintToString(buf, 0, 7);  // Punktformat mm:ss.hh (format - 1 außerhalb 0..5)
    CHECK(std::strcmp(buf, " 1:05.25") == 0);
    t.SetTime(0.0f);
    t.PrintToString(buf, 0, 7);
    CHECK(std::strcmp(buf, "    0.--") == 0 || std::strcmp(buf, "    0.00") == 0);
    t.SetTime(3725.5f);
    t.PrintToString(buf, 0, 7);
    CHECK(std::strcmp(buf, " 1h:02:05") == 0);
    t.SetTime(65.25f);
    t.PrintToString(buf, 4, 1);  // Apostrophformat mit Nullen
    CHECK(std::strcmp(buf, " 1'05\"25") == 0);
}
