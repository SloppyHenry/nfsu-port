#include "game/misc/joystick.hpp"

#include <algorithm>

#include "game/misc/joylog.hpp"

namespace nfsu {

static inline uint32_t bit(const uint32_t* p, int n) { return p[n >> 5] >> (n & 31); }

void JoystickAxis::Init() {  // orig: 0x5532d0 (je Achse): Kennlinie 0,128,128,255 und Standardwerte
    pts[0] = {0, 0};
    pts[1] = {128, 128};
    pts[2] = {128, 128};
    pts[3] = {255, 255};
    graph.Points = pts;
    graph.NumPoints = 4;
    hysteresis = 0;
}

Joystick::Joystick() {
    for (auto& a : axis) a.Init();
}

void JoyQueue::Push(uint16_t result, int port, int event, JoyHandler fn, void* param) {
    if (count < 0x100) entries[count++] = {result, static_cast<uint8_t>(port), static_cast<uint8_t>(event), fn, param};
}

// orig: 0x553c20: Taste 1 (Shift1/Xor1) und Taste 2 (Shift2/Xor2). Ohne Xor2: beide gedrückt. Mit Xor2 (Modifikator):
// Taste 1 neu gedrückt (Flanke gegenüber b) und Taste 2 in beiden Bildern gehalten.
uint32_t ReadButtonPair(const ScannerConfig& c, const uint32_t* a, const uint32_t* b) {
    uint32_t cur1 = 1;
    if (c.Shift1) cur1 = (bit(a, c.Shift1) ^ c.Xor1) & 1;
    uint32_t r = 1;
    if (c.Shift2) {
        r = (bit(a, c.Shift2) ^ c.Xor2) & 1;
        if (c.Xor2) {
            uint32_t prev1 = (bit(b, c.Shift1) ^ c.Xor1) & 1;
            uint32_t rise = cur1 & (prev1 ^ cur1);
            uint32_t prev2 = (bit(b, c.Shift2) ^ c.Xor2) & 1;
            uint32_t held = (prev2 ^ r) == 0 ? 1 : 0;
            return (r & held) & rise;
        }
    }
    return r & cur1;
}

// Meldet `value`, wenn es sich vom letzten unterscheidet oder Fresh gesetzt ist (orig: gemeinsames Muster der Scanner)
static uint16_t ReportOnChange(EventNode& n, uint32_t value) {
    if (value == n.Last && n.Fresh == 0) return 0;
    n.Last = value;
    n.Fresh = 0;
    return static_cast<uint16_t>((value << 8) + n.Event);
}

// Die sechs Scanner (orig: 0x553ce0 / 0x553d20 / 0x553d80 / 0x553dd0 / 0x553e60 / 0x554080)
uint16_t JoystickSystem::Scan(EventNode& n, const uint32_t* a, const uint32_t* b, const ScannerConfig& c, Joystick& j) {
    switch (c.Scanner) {
        case SK_TypeChanged:  // 0x553ce0
            return ReportOnChange(n, static_cast<uint8_t>(j.type));
        case SK_ButtonPress: {  // 0x553d20: nur Drücken wird gemeldet, Loslassen still
            uint32_t v = ReadButtonPair(c, a, b);
            if (v && c.Param) v = c.Param;
            if (n.Fresh) {
                n.Fresh = 0;
                n.Last = v;
            }
            if (v != n.Last) {
                n.Last = v;
                if (v) return static_cast<uint16_t>((v << 8) + n.Event);
            }
            return 0;
        }
        case SK_ButtonChange: {  // 0x553d80
            uint32_t v = ReadButtonPair(c, a, b);
            if (v && c.Param) v = c.Param;
            return ReportOnChange(n, v);
        }
        case SK_TwoButtonAxis: {  // 0x553dd0: (1,1) und (0,0) = 0x80 (Mitte), (1,0) = 0, (0,1) = 0xff
            uint32_t b1 = (bit(a, c.Shift1) ^ c.Xor1) & 1;
            uint32_t b2 = (bit(a, c.Shift2) ^ c.Xor2) & 1;
            uint32_t v = 0x80;
            if (b1) {
                if (!b2) v = 0;
            } else if (b2) {
                v = 0xff;
            }
            return ReportOnChange(n, v);
        }
        case SK_AnalogRepeat: {  // 0x553e60: Schwelle überschreiten = drücken, gehalten = Wiederholung (25, dann alle 6 Bilder)
            uint32_t v1 = (bit(a, c.Shift1) ^ c.Xor1) & 0xff;
            uint32_t v2 = 0x80;
            if (c.Shift2) v2 = (bit(a, c.Shift2) ^ c.Xor2) & 0xff;
            if (n.Fresh) {
                n.Fresh = 0;
                n.Last = (v2 << 8) | v1;
                return 0;
            }
            const auto press = [&]() { n.RepeatAt = frame + 0x19; return static_cast<uint16_t>(n.Event + 0x100); };
            const auto repeat = [&]() { n.RepeatAt = frame + 6; return static_cast<uint16_t>(n.Event + 0x100); };
            const auto release = [&]() { return static_cast<uint16_t>(n.Event); };
            if (c.Param == 0) {
                int thr = kAnalogThresholds[2];
                int prev = static_cast<int>(n.Last & 0xff);
                n.Last = v1;
                int cur = static_cast<int>(v1);
                if (prev < thr) {
                    if (cur > thr) return press();
                    return 0;
                }
                if (prev > thr && cur < thr) return release();
                if (prev <= thr || cur <= thr) return 0;
                if (frame > n.RepeatAt) return repeat();
                return 0;
            }
            if (c.Param == 1 || c.Param == 2) {
                int prev1 = static_cast<int>(n.Last & 0xff), prev2 = static_cast<int>((n.Last >> 8) & 0xff);
                n.Last = (v2 << 8) | v1;
                for (int pass = 0; pass < 2; ++pass) {
                    int cur = static_cast<int>(pass == 0 ? v1 : v2);
                    int prv = pass == 0 ? prev1 : prev2;
                    if (c.Param == 1) {
                        int thr = kAnalogThresholds[0];
                        if (!(prv < thr)) {
                            if (cur < thr) return press();
                            if (prv > thr) continue;
                        }
                        if (cur > thr) return release();
                        if (prv > thr) continue;
                        if (cur >= thr) continue;
                        if (frame > n.RepeatAt) return repeat();
                    } else {
                        int thr = kAnalogThresholds[1];
                        if (!(prv > thr)) {
                            if (cur > thr) return press();
                            if (prv < thr) continue;
                        }
                        if (cur < thr) return release();
                        if (prv < thr) continue;
                        if (cur <= thr) continue;
                        if (frame > n.RepeatAt) return repeat();
                    }
                }
            }
            return 0;
        }
        case SK_AnalogGraph: {  // 0x554080: Achse über Kennlinie, mit Hysterese
            uint32_t raw = (bit(a, c.Shift1) ^ c.Xor1) & 0xff;
            JoystickAxis& ax = j.axis[c.AnalogDeviceNumber];
            uint32_t hyst = ax.hysteresis;
            uint32_t v = static_cast<uint32_t>(static_cast<int>(ax.graph.GetValue(static_cast<float>(raw))));
            if (c.Param == 1) {
                v = v < 0x7f ? (0x7f - v) * 2 : 0;
                if (v == 0xfe) v = 0xff;
            } else if (c.Param == 2) {
                v = v > 0x80 ? (v - 0x80) * 2 : 0;
                if (v == 0xfe) v = 0xff;
            }
            if (n.Fresh) {
                n.Fresh = 0;
                n.Last = v;
                return static_cast<uint16_t>((v << 8) + n.Event);
            }
            if (c.Shift2 && ((bit(a, c.Shift2) ^ c.Xor2) & 1) == 0) return 0;
            if (v == 0 || v == 0x80 || v == 0xff) hyst = 0;
            if (v + hyst < n.Last || v > n.Last + hyst) {
                n.Last = v;
                return static_cast<uint16_t>((v << 8) + n.Event);
            }
            return 0;
        }
    }
    return 0;
}

// orig: 0x5541a0: Konfigurationsnummern eines Gerätetyps (nur Einträge ≠ 0)
int GetValidConfigs(int joystickType, int* out, int max) {
    for (int i = 0; i < kNumValidConfigs; ++i) {
        if (kValidConfigs[i].JoystickType != joystickType) continue;
        int n = 0;
        for (int k = 0; k < 10; ++k)
            if (kValidConfigs[i].Configs[k] != 0 && n < max) out[n++] = static_cast<int8_t>(kValidConfigs[i].Configs[k]);
        return n;
    }
    return 0;
}

// orig: 0x554290: gewünschte Nummer, falls für den Typ gültig, sonst die erste gültige, sonst 0
int ChooseJoystickConfigurationNumber(int joystickType, int requested) {
    int list[10];
    int n = GetValidConfigs(joystickType, list, 10);
    if (n == 0) return 0;
    for (int i = 0; i < n; ++i)
        if (list[i] == requested) return requested;
    return list[0];
}

// orig: 0x5542e0 FindScannerConfig
const ScannerConfig* FindScannerConfig(int event, int joystickType, int requestedConfig) {
    int cfg = ChooseJoystickConfigurationNumber(joystickType, requestedConfig);
    for (int i = 0; i < kNumScannerConfigs; ++i) {
        const ScannerConfig& s = kScannerConfigs[i];
        if (s.Event != event) continue;
        for (int k = 0; k < 8; ++k) {
            if (cfg == 0) {
                if (event == 1) return &s;
            } else {
                int8_t c = s.ConfigurationNumbers[k];
                if (c != 0) {
                    if (c == -1) return &s;
                    if (c == cfg) return &s;
                }
            }
        }
    }
    return nullptr;
}

// orig: 0x554350: Totzone und Hysterese der Achse aus der Tabelle des Gerätetyps (Standard 0x20 / 0x0c)
void ChooseDeadZoneAndHysterysis(JoystickAxis& axis, int axisIndex, int joystickType) {
    int dead = 0x20;
    uint8_t hyst = 0x0c;
    for (int i = 0; i < kNumValidConfigs; ++i) {
        if (kValidConfigs[i].JoystickType != joystickType) continue;
        const uint8_t* d = kValidConfigs[i].DeadZone;
        int k = axisIndex == 2 ? 2 : axisIndex == 3 ? 4 : 0;  // Achse 0 und 1 teilen sich das erste Paar
        hyst = d[k];
        dead = d[k + 1];
    }
    axis.hysteresis = hyst;
    axis.pts[1].x = 128.0f - static_cast<float>(dead);
    axis.pts[2].x = static_cast<float>(dead) + 128.0f;
}

// orig: 0x553af0 InitJoystick (ohne Slot-Pool und Zufallszahl)
void JoystickSystem::Init() {
    for (int p = 0; p < 2; ++p) {
        joys[p].port = p;
        portType[p] = 0xff;
        AddJoyHandler(p, JOY_EVENT_TYPE_CHANGED, nullptr, nullptr);
    }
}

// orig: 0x553ac0
void JoystickSystem::ReInitTypeMonitoring() {
    for (int p = 0; p < 2; ++p) {
        RemoveJoyHandler(p, JOY_EVENT_TYPE_CHANGED);
        AddJoyHandler(p, JOY_EVENT_TYPE_CHANGED, nullptr, nullptr);
    }
}

// orig: 0x553960: an die Liste anhängen (ein vorhandener Eintrag mit gleichem Ereignis bleibt, wie im Original)
bool JoystickSystem::AddJoyHandler(int port, int event, JoyHandler fn, void* param) {
    Joystick& j = joys[port];
    EventNode n;
    n.Event = event;
    n.Handler = fn;
    n.Param = param;
    n.Config = FindScannerConfig(event, j.type, j.config);
    n.Fresh = 1;
    n.Last = 0;
    j.nodes.push_back(n);
    return true;
}

// orig: 0x5539f0: bereits eingereihte Ereignisse dieses Handlers verwerfen, dann Eintrag entfernen
void JoystickSystem::RemoveJoyHandler(int port, int event) {
    for (int i = 0; i < queue.count; ++i)
        if (queue.entries[i].port == port && queue.entries[i].event == event) queue.entries[i].result = 0;
    auto& l = joys[port].nodes;
    for (auto it = l.begin(); it != l.end(); ++it) {
        if (it->Event == event) {
            l.erase(it);
            return;
        }
    }
}

bool JoystickSystem::IsJoyHandlerAdded(int port, int event) const {
    for (const auto& n : joys[port].nodes)
        if (n.Event == event) return true;
    return false;
}

// orig: 0x5533f0 Joystick::GenerateEventsFromScanners. `type` = vom Gerät gemeldeter Typ.
// Ein neuer Typ gilt erst, wenn er 32 Bilder in Folge gemeldet wurde; dann werden Totzonen und Scanner neu gewählt.
int JoystickSystem::GenerateEventsFromScanners(Joystick& j, int type, const uint32_t* a, const uint32_t* b) {
    if (type != j.type) {
        if (type == j.pendingType) {
            if (++j.typeCounter < 32) return 0;
            j.type = type;
            for (int i = 0; i < 4; ++i) ChooseDeadZoneAndHysterysis(j.axis[i], i, type);
            for (auto& n : j.nodes) {
                n.Config = FindScannerConfig(n.Event, j.type, j.config);
                n.Fresh = 1;
                n.Last = 0;
            }
        } else {
            j.typeCounter = 0;
            j.pendingType = type;
            return 0;
        }
    }
    int produced = 0;
    for (auto& n : j.nodes) {
        const ScannerConfig* c = n.Config;
        if (!c) continue;
        if (repeatActive && !c->ValidDuringReplay) continue;
        uint16_t r = Scan(n, a, b, *c, j);
        if (r != 0) {
            queue.Push(r, j.port, n.Event, n.Handler, n.Param);
            ++produced;
        }
    }
    return produced;
}

// orig: 0x553570: Ereignis aus dem Joylog (19 Bit: Ergebnis, Port) wieder einreihen
void JoystickSystem::AddLoggedJoystickEvent(uint32_t packed) {
    int port = static_cast<int>((packed >> 16) & 7);
    if (port > 1) return;
    for (auto& n : joys[port].nodes) {
        if (n.Event == static_cast<int>(packed & 0xff)) {
            queue.Push(static_cast<uint16_t>(packed), port, n.Event, n.Handler, n.Param);
            return;
        }
    }
}

// orig: 0x5535f0 GenerateJoyEvents (Joylog-Schicht; die zweite Aufzeichnungsschicht 0x72960c ist nicht portiert)
void JoystickSystem::GenerateJoyEvents() {
    queue.count = 0;
    if (!Joylog::replaying) {
        while (inputDirty) {
            inputDirty = false;
            for (int p = 0; p < 2; ++p) {
                const uint32_t *a = nullptr, *b = nullptr;
                int type = 0;
                if (poll) poll(p, &a, &b, &type);
                if (!a || !b) continue;
                if (GenerateEventsFromScanners(joys[p], type, a, b) != 0 && lastInput < now) lastInput = now;
            }
        }
        inputDirty = false;
        if (Joylog::recording) {
            Joylog::buffer->AddData(static_cast<uint32_t>(queue.count), 8, 2);
            for (int i = 0; i < queue.count; ++i) {
                const auto& e = queue.entries[i];
                Joylog::buffer->AddData(static_cast<uint32_t>(e.result) | (static_cast<uint32_t>(e.port) << 16), 0x13, 2);
            }
        }
    } else {
        int n = static_cast<int>(Joylog::GetData(8, 2));
        for (int i = 0; i < n; ++i) {
            uint32_t packed = Joylog::replaying ? Joylog::GetData(0x13, 2) : 0;
            AddLoggedJoystickEvent(packed);
            if (lastInput < now) lastInput = now;
        }
    }
    for (int i = 0; i < queue.count; ++i) {
        const auto& e = queue.entries[i];
        int event = e.result & 0xff;
        if (event == 0) continue;
        joys[e.port].eventState[event] = static_cast<uint8_t>(e.result >> 8);
        if (e.fn) e.fn(e.port, event, static_cast<uint8_t>(e.result >> 8), e.param);
    }
}

}  // namespace nfsu
