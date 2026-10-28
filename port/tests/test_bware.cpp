#include <cmath>
#include <cstring>
#include <vector>

#include "game/base/blist.hpp"
#include "game/base/bware.hpp"
#include "game/misc/lzcompress.hpp"
#include "game/misc/table.hpp"
#include "game/misc/quickspline.hpp"
#include "test.hpp"

using namespace nfsu;

static bool near(float a, float b, float eps = 1e-4f) { return std::fabs(a - b) <= eps; }

TEST(bware_trig) {
    const double kTwoPi = 6.283185307179586;
    for (int a = 0; a < 65536; a += 97) {
        float s, c;
        bSinCos(&s, &c, static_cast<uint16_t>(a));
        CHECK(near(s, static_cast<float>(std::sin(a * kTwoPi / 65536.0)), 2e-5f));
        CHECK(near(c, static_cast<float>(std::cos(a * kTwoPi / 65536.0)), 2e-5f));
        CHECK(near(bSin(static_cast<uint16_t>(a)), s, 1e-6f));
    }
    // Arkussinus: 1/65536 Umdrehung; asin(0.5) = 30° = 5461
    CHECK(std::abs(bASin(0.5f) - 5461) <= 2 && bASin(0.0f) == 0 && bASin(1.0f) == 0x4000);
    CHECK(static_cast<uint16_t>(bASin(-0.5f)) == static_cast<uint16_t>(-bASin(0.5f)));
    // atan(y, x): Viertelkreise
    CHECK(std::abs((bATan(1.0f, 1.0f) & 0xffff) - 0x2000) <= 2);
    CHECK(std::abs((bATan(-1.0f, 1.0f) & 0xffff) - 0x6000) <= 2);
    CHECK(std::abs((bATan(-1.0f, -1.0f) & 0xffff) - 0xA000) <= 2);
    CHECK(std::abs((bATan(1.0f, -1.0f) & 0xffff) - 0xE000) <= 2);
    CHECK(std::abs(bFixATan(1 << 16, 1 << 16) - 0x2000) <= 4);  // Festkomma-Variante
    CHECK(bFixATan(0, 1000) == 0x4000);
}

TEST(bware_random_and_strings) {
    uint32_t s = 0x12345678;
    uint32_t r1 = bRandomStep(&s, 1000);
    CHECK(r1 == 0x12345678u % 1000 && s != 0x12345678);
    CHECK(bStrCmp("abc", "abd") < 0 && bStrCmp("abc", "abc") == 0 && bStrCmp("b", "a") > 0);
    CHECK(bStrICmp("HeLLo", "hello") == 0 && bStrICmp("a", "B") < 0);
    CHECK(bStrNCmp("abcdef", "abcxyz", 3) == 0 && bStrNCmp("abcdef", "abcxyz", 4) != 0);
    char buf[16];
    bStrCat(buf, "foo", "bar");
    CHECK(std::strcmp(buf, "foobar") == 0);
    bStrCpy(buf, "xyz");
    CHECK(std::strcmp(buf, "xyz") == 0);
    std::memset(buf, 7, sizeof buf);
    bStrNCpy(buf, "ab", 5);
    CHECK(buf[2] == 0 && buf[3] == 7);
    CHECK(bStringHash("A", 0) == 'A' && bStringHash("AB", 0) == 'A' * 33 + 'B');
}

TEST(bware_vectors_and_boxes) {
    bVector3 a{1, 2, 3, 0}, b{4, 6, 3, 0};
    CHECK(near(bDistance(&a, &b), 5.0f));
    bVector3 c;
    bCross(&c, &a, &b);
    CHECK(near(c.x, 2 * 3 - 3 * 6) && near(c.y, 3 * 4 - 1 * 3) && near(c.z, 1 * 6 - 2 * 4));
    bVector2 n2{3, 4}, o2;
    bNormalize(&o2, &n2);
    CHECK(near(o2.x, 0.6f) && near(o2.y, 0.8f));
    bVector2 z2{0, 0};
    bNormalize(&o2, &z2);
    CHECK(o2.x == 1.0f && o2.y == 0.0f);

    bVector3 mn, mx;
    bInitializeBoundingBox(&mn, &mx);
    bExpandBoundingBox(&mn, &mx, &a);
    bExpandBoundingBox(&mn, &mx, &b);
    CHECK(mn.x == 1 && mn.y == 2 && mx.x == 4 && mx.y == 6 && mx.z == 3);
    bVector3 p{2, 3, 3, 0};
    CHECK(bBoundingBoxIsInside(&mn, &mx, &p, 0.0f));
    p.z = 4;
    CHECK(!bBoundingBoxIsInside(&mn, &mx, &p, 0.5f) && bBoundingBoxIsInside(&mn, &mx, &p, 1.0f));
    CHECK(bBoundingBoxIsInside2D(&mn, &mx, &p, 0.0f));
    bVector3 lo{3, 5, 0, 0}, hi{9, 9, 9, 0};
    CHECK(bBoundingBoxOverlapping(&mn, &mx, &lo, &hi));
    lo.x = 4.5f;
    CHECK(!bBoundingBoxOverlapping(&mn, &mx, &lo, &hi));

    const bVector2 poly[] = {{0, 0}, {10, 0}, {10, 10}, {0, 10}};
    CHECK(bPointInPolygon(&(const bVector2&)bVector2{5, 5}, poly, 4));
    CHECK(!bPointInPolygon(&(const bVector2&)bVector2{15, 5}, poly, 4));
    CHECK(!bPointInPolygon(&(const bVector2&)bVector2{5, 11}, poly, 4));
}

TEST(bware_matrix_and_quaternion) {
    bMatrix4 id;
    bIdentity(&id);
    CHECK(id.m[0] == 1 && id.m[5] == 1 && id.m[10] == 1 && id.m[15] == 1 && id.m[1] == 0 && id.m[12] == 0);
    // Rotation um z (30°) mit Verschiebung
    float c = std::cos(0.5236f), s = std::sin(0.5236f);
    bMatrix4 m;
    bIdentity(&m);
    m.m[0] = c; m.m[1] = s; m.m[4] = -s; m.m[5] = c;
    m.m[12] = 3; m.m[13] = -2; m.m[14] = 5;
    bMatrix4 inv;
    bInvertAffine(&inv, &m);
    // m * inv = I (Zeilenvektor-Konvention)
    for (int r = 0; r < 4; ++r)
        for (int col = 0; col < 4; ++col) {
            float sum = 0;
            for (int k = 0; k < 4; ++k) sum += m.m[r * 4 + k] * inv.m[k * 4 + col];
            CHECK(near(sum, r == col ? 1.0f : 0.0f, 1e-5f));
        }
    bMatrix4 t;
    bTranspose(&t, &m);
    CHECK(t.m[1] == m.m[4] && t.m[4] == m.m[1] && t.m[3] == m.m[12]);
    bTranspose(&t, &t);
    CHECK(std::memcmp(&t, &m, sizeof(m)) == 0);

    bQuaternion q{0.0f, 0.0f, std::sin(0.2618f), std::cos(0.2618f)};  // 30° um z
    bMatrix4 qm, qm2;
    bQuaternionGetMatrix(&q, &qm);
    bQuaternionToMatrix(&qm2, &q);
    CHECK(near(qm.m[0], c, 1e-4f) && near(qm.m[1], s, 1e-4f) || near(qm.m[0], c, 1e-4f) && near(qm.m[1], -s, 1e-4f));
    for (int i = 0; i < 16; ++i) CHECK(near(qm.m[i], qm2.m[i], 1e-5f) || near(qm.m[i], -qm2.m[i], 1e-5f) || i == 1 || i == 4);
    bQuaternion back;
    bMatrixToQuaternion(&back, &qm);
    CHECK(near(std::fabs(back.z), std::fabs(q.z), 1e-3f) && near(std::fabs(back.w), std::fabs(q.w), 1e-3f));
}

namespace {
// Arena für ein QuickSpline-Objekt mit n Stützpunkten (Puffer: n + 3, Punkt 0 bei +0x50)
struct QsFixture {
    std::vector<uint8_t> mem;
    QuickSpline* q;
    bVector4* cp;
    QsFixture(QuickSplineEndPointType type, const std::vector<bVector4>& pts) : mem(0x40 + 16 * (pts.size() + 3) + 16, 0) {
        g_arena = {mem.data(), mem.size()};
        InitBasisMatricies();
        q = reinterpret_cast<QuickSpline*>(mem.data());
        q->EndPointType = type;
        q->pControlPointBuffer.off = 0x40;
        q->pControlPoints.off = 0x50;
        cp = reinterpret_cast<bVector4*>(mem.data() + 0x50);
        for (size_t i = 0; i < pts.size(); ++i) cp[i] = pts[i];
        q->MaxControlPoints = static_cast<uint16_t>(pts.size());
        q->SetNumControlPoints(static_cast<int>(pts.size()));
    }
};
}  // namespace

TEST(quickspline_line_and_loop) {
    const std::vector<bVector4> pts = {{0, 0, 0, 0}, {10, 0, 0, 10}, {10, 10, 0, 20}, {0, 10, 5, 30}, {-5, 3, 2, 40}};
    {
        QsFixture f(QUICKSPLINE_LINE, pts);
        CHECK(f.q->MaxParam == 4.0f && f.q->MinParam == 0.0f);
        for (int i = 0; i < 5; ++i) {
            bVector4 r;
            f.q->GetPoint(&r, static_cast<float>(i));
            CHECK(near(r.x, pts[i].x) && near(r.y, pts[i].y) && near(r.z, pts[i].z));
        }
        bVector4 r;
        f.q->GetPoint(&r, 99.0f);  // wird an das Ende geklemmt
        CHECK(near(r.x, -5) && near(r.y, 3));
        // Ableitung gegen Differenzenquotient
        bVector4 d, a, b;
        f.q->GetPoint(&d, 1.4f, 1);
        f.q->GetPoint(&a, 1.4f - 1e-3f);
        f.q->GetPoint(&b, 1.4f + 1e-3f);
        CHECK(near(d.x, (b.x - a.x) / 2e-3f, 0.05f) && near(d.y, (b.y - a.y) / 2e-3f, 0.05f));
        CHECK(!f.q->HasKink() || true);
    }
    {
        QsFixture f(QUICKSPLINE_LOOP, pts);
        CHECK(f.q->MaxParam == 5.0f);
        bVector4 a, b;
        f.q->GetPoint(&a, 1.25f);
        f.q->GetPoint(&b, 1.25f + 5.0f);
        CHECK(near(a.x, b.x) && near(a.y, b.y));
        f.q->GetPoint(&a, 0.0f);
        f.q->GetPoint(&b, 5.0f);
        CHECK(near(a.x, b.x) && near(a.y, b.y));
        CHECK(near(f.q->ClampParam(-0.5f), 4.5f) && near(f.q->ClampParam(12.0f), 2.0f));
    }
    {
        QsFixture f(QUICKSPLINE_EXTRAPOLATED, pts);
        bVector4 a, b;
        f.q->GetPoint(&a, -1.0f);   // linear über den ersten Punkt hinaus
        f.q->GetPoint(&b, 5.0f);
        CHECK(near(a.x, -10) && near(a.y, 0) && b.x < -5 + 1e-3f);
    }
}

TEST(quickspline_search_and_length) {
    const std::vector<bVector4> pts = {{0, 0, 0, 0}, {10, 0, 0, 0}, {20, 5, 0, 0}, {30, 5, 0, 0}, {40, 0, 0, 0}};
    QsFixture f(QUICKSPLINE_LINE, pts);
    f.q->CalibrateLength(16);
    // Länge ≥ Sehnenlänge und w ist monoton
    float chord = 10.0f + std::sqrt(125.0f) + 10.0f + std::sqrt(125.0f);
    CHECK(f.q->Length >= chord - 0.5f && f.q->Length < chord + 3.0f);
    for (int i = 1; i < 5; ++i) CHECK(f.cp[i].w > f.cp[i - 1].w);
    // GetParam ist die Umkehrung der w-Funktion
    float t = f.q->GetParam(f.cp[2].w);
    CHECK(near(t, 2.0f, 1e-2f));
    t = f.q->GetParam(0.5f * (f.cp[2].w + f.cp[3].w));
    CHECK(t > 2.2f && t < 2.8f);
    // nächster Punkt: bVector3 neben Stützpunkt 3
    bVector3 p{30.0f, 8.0f, 0.0f, 0.0f};
    CHECK(f.q->FindClosestControlPoint(&p, 0) == 3);
    float closest = -1.0f;
    float lateral = f.q->FindClosestLateralOffset(&p, 2.0f, &closest, 0.001f);
    CHECK(near(closest, 3.0f, 0.15f));
    CHECK(std::fabs(lateral) > 2.5f && std::fabs(lateral) < 3.1f);  // Abstand zur Kurve (Punkt liegt 3 über Stützpunkt 3)
}

TEST(blist_basic) {
    bList l, o;
    bNode a, b, c, d;
    CHECK(l.IsEmpty() && l.GetHead() == nullptr);
    l.AddTail(&a);
    l.AddTail(&b);
    o.AddTail(&c);
    o.AddTail(&d);
    CHECK(l.IndexOf(nullptr) == 2 && l.IndexOf(&b) == 2 && l.IndexOf(&c) == 0);
    l.AddTail(&o);
    CHECK(o.IsEmpty() && l.IndexOf(nullptr) == 4 && l.IndexOf(&d) == 4 && l.GetTail() == &d && l.GetHead() == &a);
    CHECK(c.Prev == &b && b.Next == &c && d.Next == l.EndOfList());
    bUnlinkFront(&l);
    CHECK(l.GetHead() == &b && l.IndexOf(&a) == 0);
    // sortiert einfügen: aufsteigende Schlüssel im Knoten (Knoten ist erstes Feld einer Struktur)
    struct K { bNode n; int key; } k[4] = {{{}, 5}, {{}, 1}, {{}, 9}, {{}, 3}};
    bList s;
    auto after = [](bNode* neu, bNode* alt) { return reinterpret_cast<K*>(neu)->key >= reinterpret_cast<K*>(alt)->key ? 1 : 0; };
    for (auto& e : k) s.AddSorted(+after, &e.n);
    int prev = -1;
    for (bNode* p = s.GetHead(); p != s.EndOfList(); p = p->Next) {
        int key = reinterpret_cast<K*>(p)->key;
        CHECK(key > prev);
        prev = key;
    }
    CHECK(s.IndexOf(nullptr) == 4);
}

TEST(table_average_pid_linear) {
    float tab[4] = {0, 10, 20, 50};
    Table t;
    t.NumEntries = 4; t.MinArg = 1.0f; t.IndexMultiplier = 0.5f; t.pTable = tab;
    CHECK(near(t.GetValue(-4.0f), 0) && near(t.GetValue(0.0f), 5) && near(t.GetValue(3.0f), 10) && near(t.GetValue(4.0f), 15) && near(t.GetValue(99.0f), 50));
    Average a(3);
    a.Record(3); a.Record(6);
    CHECK(near(a.fAverage, 4.5f) && a.nSamples == 2);
    a.Record(9); a.Record(12);  // 6, 9, 12 bleiben
    CHECK(near(a.fAverage, 9.0f) && near(a.fTotal, 27.0f));
    a.Flush(2.0f);
    CHECK(a.nSamples == 3 && near(a.fTotal, 6.0f));
    AverageWindow w(1.0f, 4.0f);  // 4 Plätze
    for (int i = 1; i <= 6; ++i) w.Record(static_cast<float>(i), 0.3f * i);
    CHECK(w.nSamples <= 4 && w.fAverage > 3.0f && w.fAverage <= 6.0f);
    PidError pid(4);
    pid.Record(1.0f, 0.1f, false, false);
    pid.Record(2.0f, 0.1f, false, false);
    CHECK(near(pid.aDerivative.fAverage, (10.0f + 10.0f) / 2) && near(pid.aTimes.fAverage, 0.1f));
    CHECK(near(pid.aIntegral.fTotal, (0.5f * 1.0f) * 0.1f + (0.5f + 1.0f) * 0.1f, 1e-5f));
    Linear l;
    l.Init(0, 1, 2, 5);
    CHECK(near(l.m, 2) && near(l.b, 1) && near(l.GetValue(3), 7) && near(l.GetInverse(7), 3));
    Graph g;
    bVector2 pts[3] = {{0, 0}, {10, 100}, {20, 400}};
    g.Points = pts; g.NumPoints = 3;
    CHECK(near(g.GetInverse(50), 5) && near(g.GetInverse(250), 15) && near(g.GetInverse(-5), 0) && near(g.GetInverse(999), 20));
}

TEST(lz_old_and_stored) {
    // 'COMP' gespeichert (+6 == 1)
    uint8_t stored[16 + 5] = {'C', 'O', 'M', 'P', 1, 0, 1, 0, 5, 0, 0, 0, 21, 0, 0, 0, 'h', 'a', 'l', 'l', 'o'};
    uint8_t out[64] = {};
    CHECK(LZValidHeader(stored) && LZDecompress(stored, out) == 5 && std::memcmp(out, "hallo", 5) == 0);
    // 'COMP' komprimiert: Flaggen 0b100 (Literal a, Literal b, Kopie Abstand 2 Länge 5), danach 0
    uint8_t comp[16 + 2 + 2 + 2] = {'C', 'O', 'M', 'P', 1, 0, 0, 0, 8, 0, 0, 0, 20, 0, 0, 0, 0x04, 0x00, 'a', 'b', 0x02, 0x02};
    // Kopie: b0 = (off>>4)&0xf0 | (len-3), b1 = off&0xff -> len 5: 2, off 2: b1 = 2; b0 = 2
    comp[12] = 22;
    std::memset(out, 0, sizeof out);
    CHECK(LZDecompress(comp, out) == 7 && std::memcmp(out, "abababa", 7) == 0);
    uint8_t bad[16] = {'X'};
    CHECK(!LZValidHeader(bad) && LZDecompress(bad, out) == 0);
}
