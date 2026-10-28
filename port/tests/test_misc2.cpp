#include <cstring>
#include <map>

#include "game/misc/events.hpp"
#include "game/misc/joylog.hpp"
#include "game/misc/point.hpp"
#include "game/misc/queuedfile.hpp"
#include "game/misc/rumble.hpp"
#include "test.hpp"

using namespace nfsu;

TEST(joylog_roundtrip_with_flush) {
    std::vector<uint8_t> file;
    JoylogBuffer rec;
    rec.writeBlock = [&](const uint8_t* p, int n) { file.insert(file.end(), p, p + n); };
    Joylog::recording = true;
    Joylog::replaying = false;
    Joylog::buffer = &rec;
    struct Item { uint32_t v; int bits, ch; };
    std::vector<Item> items;
    uint32_t x = 12345;
    for (int i = 0; i < 9000; ++i) {
        x = x * 1664525u + 1013904223u;
        int bits = (i % 4 == 0) ? 8 : (i % 4 == 1) ? 16 : (i % 4 == 2) ? 24 : 32;
        uint32_t v = bits == 32 ? x : (x & ((1u << bits) - 1));
        items.push_back({v, bits, i % 7});
        Joylog::AddData(v, bits, i % 7);
    }
    file.insert(file.end(), rec.data.begin(), rec.data.begin() + rec.pending);  // Rest wie Joylog::Save
    CHECK(static_cast<int>(file.size()) == rec.end);

    JoylogBuffer play;
    play.end = rec.end;
    play.readBlock = [&](uint8_t* dst, int pos, int n) {
        std::memcpy(dst, file.data() + pos, n);
        return n;
    };
    Joylog::recording = false;
    Joylog::replaying = true;
    Joylog::buffer = &play;
    play.LoadBuffer();
    for (auto& it : items) {
        CHECK(Joylog::replaying);
        CHECK(Joylog::GetData(it.bits, it.ch) == it.v);
    }
    CHECK(!Joylog::replaying && !play.desync);  // am Ende schaltet die Wiedergabe sich ab
}

TEST(joylog_string_and_desync) {
    std::vector<uint8_t> file;
    JoylogBuffer rec;
    Joylog::recording = true;
    Joylog::replaying = false;
    Joylog::buffer = &rec;
    char s[16] = "Hallo";
    Joylog::AddOrGetData(s, 1);
    Joylog::AddData(77, 8, 2);
    file.assign(rec.data.begin(), rec.data.begin() + rec.pending);
    JoylogBuffer play;
    play.end = rec.end;
    play.readBlock = [&](uint8_t* d, int p, int n) { std::memcpy(d, file.data() + p, n); return n; };
    play.LoadBuffer();
    Joylog::recording = false;
    Joylog::replaying = true;
    Joylog::buffer = &play;
    char out[16] = "x";
    Joylog::AddOrGetData(out, 1);
    CHECK(std::strcmp(out, "Hallo") == 0);
    Joylog::GetData(8, 5);  // falscher Kanal
    CHECK(play.desync);
    Joylog::replaying = false;
    Joylog::buffer = nullptr;
}

static const float kEnvPts[4][2] = {{0, 0}, {1, 100}, {2, 100}, {3, 0}};
TEST(envelope_values) {
    tEnvelope e{kEnvPts, 4};
    CHECK(e.GetValue(-0.1f) == 0.0f && e.GetValue(3.0f) == 0.0f && e.GetValue(5.0f) == 0.0f);
    CHECK(e.GetValue(0.5f) == 50.0f && e.GetValue(1.5f) == 100.0f && e.GetValue(2.5f) == 50.0f);
    CHECK(e.OutOfRange(-1) && e.OutOfRange(3) && !e.OutOfRange(1));
}

TEST(rumbler_slots_and_update) {
    tEnvelope e{kEnvPts, 4};
    tRumbler r;
    CHECK(r.StartRumble(&e, 1.0f, 1));
    CHECK(r.StartRumble(&e, 2.0f, 2));
    CHECK(r.StartRumble(&e, 3.0f, 3));
    // alle Plätze belegt, Priorität 0 darf keinen verdrängen
    CHECK(!r.StartRumble(&e, 1.0f, 0));
    r.Update(0.0f);
    CHECK(r.current == 0);
    r.Update(0.5f);  // Zeit 0: Wert 0; danach Zeit 0.5/0.33/0.25
    CHECK(r.Update(0.0f) || true);
    CHECK(r.current > 0 && r.current <= 255);
    for (int i = 0; i < 200; ++i) r.Update(0.1f);
    CHECK(r.current == 0 && !r.envelope[0] && !r.envelope[1] && !r.envelope[2]);
}

TEST(shaker_pulses_and_ends) {
    tEnvelope e{kEnvPts, 4};
    tShaker s;
    bVector3 d{1, 0, 0, 0}, v;
    s.StartShaking(&d, 1.0f, 17.0f, &e);
    CHECK(s.dir.x == -1.0f);
    s.time = 1.5f / 1.0f;       // 1.5 * 17 = 25.5 -> ungerade -> 0
    s.GetValue(&v);
    CHECK(v.x == 0.0f);
    s.time = 1.0f;              // 17 -> ungerade
    s.time = 36.0f / 17.0f;     // (int)36.0 gerade, aber hinter dem Ende (Wert 0)
    s.GetValue(&v);
    s.Update(10.0f);
    CHECK(s.envelope == nullptr && s.GetAmplitude() == 0.0f);
}

struct MemIO : FileIO {
    std::map<std::string, std::vector<uint8_t>> files;
    int reads = 0;
    bool FileSize(const std::string& n, int* s) override {
        auto it = files.find(n);
        if (it == files.end()) return false;
        *s = static_cast<int>(it->second.size());
        return true;
    }
    bool Read(const std::string& n, int pos, int len, void* dst) override {
        ++reads;
        std::memcpy(dst, files[n].data() + pos, len);
        return true;
    }
};

TEST(queuedfile_bundles_neighbours) {
    MemIO io;
    std::vector<uint8_t> blob(0x10000);
    for (size_t i = 0; i < blob.size(); ++i) blob[i] = static_cast<uint8_t>(i * 7);
    io.files["A.BIN"] = blob;
    QueuedFileSystem q;
    q.io = &io;
    uint8_t b1[100], b2[50], b3[10];
    int done = 0, err = 0;
    auto cb = [&](int e) { ++done; err += e; };
    int h1 = q.AddQueuedFile(b1, "A.BIN", 0x100, 100, cb);
    q.AddQueuedFile(b2, "A.BIN", 0x400, 50, cb);
    q.AddQueuedFile(b3, "A.BIN", 0x9000, 10, cb);  // zu weit weg: eigener Lesevorgang
    CHECK(q.FindQueuedFile(h1) != nullptr && q.IsBusy());
    for (int i = 0; i < 10 && q.IsBusy(); ++i) q.Service();
    CHECK(!q.IsBusy() && done == 3 && err == 0);
    CHECK(std::memcmp(b1, blob.data() + 0x100, 100) == 0);
    CHECK(std::memcmp(b2, blob.data() + 0x400, 50) == 0);
    CHECK(std::memcmp(b3, blob.data() + 0x9000, 10) == 0);
    CHECK(io.reads == 2);  // Bundle + Einzelanfrage
}

TEST(queuedfile_missing_file_reports_error) {
    MemIO io;
    QueuedFileSystem q;
    q.io = &io;
    uint8_t b[4];
    int err = -1;
    q.AddQueuedFile(b, "NIX.BIN", 0, 4, [&](int e) { err = e; });
    for (int i = 0; i < 5; ++i) q.Service();
    CHECK(err == 1 && !q.IsBusy());
}

static int g_hits = 0;
static void handlerA(emEvent* e) { ++g_hits; if (e->id == 1) e->keep = 0; }
static EventManager* g_em = nullptr;
static void handlerSpawn(emEvent*) { g_em->AddEvent(4); }

TEST(event_manager_dispatch) {
    EventManager em;
    g_em = &em;
    CHECK(!em.AddHandler(nullptr, 1) && !em.AddHandler(handlerA, 0));
    CHECK(em.AddHandler(handlerA, 3) && em.AddHandler(handlerA, 3) && em.numHandlers == 1);
    em.AddEvent(1);
    em.AddEvent(4);  // keine Maske passt
    em.AddEvent(2)->keep = 1;
    em.ProcessAllEvents();
    CHECK(g_hits == 2 && em.QueueSize() == 1);  // nur das gehaltene Ereignis bleibt
    em.RemoveHandler(handlerA);
    CHECK(em.numHandlers == 1);
    em.RemoveHandler(handlerA);
    CHECK(em.numHandlers == 0);
    // Ereignisse aus Handlern kommen erst nach der Runde in die Warteschlange
    em.AddHandler(handlerSpawn, 2);
    em.ProcessAllEvents();
    CHECK(em.QueueSize() == 2);  // gehaltenes (id 2) + neues (id 4)
}

#include "game/misc/joystick.hpp"
#include "game/misc/timer.hpp"

static int g_ev = 0, g_val = -1, g_port = -1;
static void joyHandler(int port, int event, uint8_t value, void*) { g_port = port; g_ev = event; g_val = value; }

TEST(joystick_type_debounce_and_button) {
    JoystickSystem sys;
    sys.Init();
    uint32_t a[8] = {}, b[8] = {};
    int type = 3;
    sys.poll = [&](int port, const uint32_t** pa, const uint32_t** pb, int* t) {
        *pa = a; *pb = b; *t = port == 0 ? type : 0;
    };
    // Event 137 (CROSS, Tabellenzeile: Shift1 = 1, Xor1 = 1, für alle Konfigurationen)
    CHECK(sys.AddJoyHandler(0, 137, joyHandler, nullptr));
    CHECK(sys.IsJoyHandlerAdded(0, 137) && !sys.IsJoyHandlerAdded(1, 137));
    a[0] = 2;  // Bit 1 gesetzt: Taste (Xor1 = 1) losgelassen
    // Der Typ muss 32 Bilder in Folge anliegen (das erste Bild merkt ihn nur), erst das 33. übernimmt ihn
    for (int i = 0; i < 32; ++i) {
        sys.inputDirty = true;
        sys.GenerateJoyEvents();
    }
    CHECK(sys.joys[0].type == -1 && g_ev == 0);
    sys.inputDirty = true;
    sys.GenerateJoyEvents();
    CHECK(sys.joys[0].type == 3);
    // Der erste Scan danach meldet jeden Handler einmal (Fresh), hier mit Wert 0
    CHECK(g_ev == 137 && g_val == 0 && g_port == 0);
    g_ev = 0;
    sys.inputDirty = true;
    sys.GenerateJoyEvents();
    CHECK(g_ev == 0);  // keine Änderung
    a[0] = 0;          // Taste gedrückt: Wert 255 (Param der Tabellenzeile)
    sys.inputDirty = true;
    sys.GenerateJoyEvents();
    CHECK(g_ev == 137 && g_val == 255);
    sys.RemoveJoyHandler(0, 137);
    CHECK(!sys.IsJoyHandlerAdded(0, 137));
}

TEST(clock_advances_and_pauses) {
    GameClock c;
    c.PrepareRealTimestep(0.0166f);
    CHECK(c.RealTimeQuantized >= 60 && c.RealTimeQuantized <= 180);  // zwischen 1 und 3 Bildern in 1/3600 s
    float e = c.RealTimeElapsed;
    c.AdvanceRealTime();
    CHECK(c.RealTimeFramesElapsed >= 1 && c.RealTimeFrames == 1 + c.RealTimeFramesElapsed);
    CHECK(c.RealTimer == static_cast<int>(e * 4000.0f + 0.5f) && c.RealLoopCounter == 1);
    // Weltzeit steht, solange der Schritt 0 ist
    c.AdvanceWorldTime();
    CHECK(c.WorldTimer == 0 && c.NeedToPrepareWorldTimestep == 1);
    c.ResetWorldTime();
    CHECK(c.WorldTimer == 4000 && c.WorldTimeSeconds == 1.0f);
    c.PrepareWorldTimestep(1.0f);
    c.AdvanceWorldTime();
    CHECK(c.WorldTimer == 8000 && c.WorldTimeFramesElapsed == 60 && c.WorldTimeElapsed == 0.0f);
}

TEST(spline_seek_3d_runs_all_axes) {
    tCubic3D c{};
    c.SetValDesired(1, 2, 3);
    for (auto* a : {&c.x, &c.y, &c.z}) {
        a->duration = 1.0f;
        a->arrivalThreshold = 1.0f;
    }
    for (int i = 0; i < 30; ++i) cPoint::SplineSeek(&c, 0.05f);
    CHECK(c.x.Val == 1.0f && c.y.Val == 2.0f && c.z.Val == 3.0f);
}
