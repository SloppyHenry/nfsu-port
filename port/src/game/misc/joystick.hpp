#pragma once
// Joystick.cpp: Eingabeereignisse. Pro Port eine Liste von Handlern ("EventNode"), jeder ist an einen Scanner gebunden,
// der aus den Rohbits des Geräts einen Wert (0..255) und damit ein Ereignis ableitet. Die Rohbits liefert die
// Plattformschicht (WP-41) über JoystickSystem::poll.
#include <cstdint>
#include <functional>
#include <list>

#include "game/misc/graph.hpp"

namespace nfsu {

enum JoyEvent { JOY_EVENT_NULL = 0, JOY_EVENT_TYPE_CHANGED = 1 };  // weitere Werte: gc_types.h enum JoyEvent

using JoyHandler = void (*)(int port, int event, uint8_t value, void* param);

// Scanner-Arten: Adressen der Originalfunktionen 0x553ce0, 0x553d20, 0x553d80, 0x553dd0, 0x553e60, 0x554080.
enum ScannerKind { SK_TypeChanged, SK_ButtonPress, SK_ButtonChange, SK_TwoButtonAxis, SK_AnalogRepeat, SK_AnalogGraph };

struct ScannerConfig {  // 0x20 Byte im Original
    int8_t ConfigurationNumbers[8];  // 0 = unbenutzt, -1 = für alle Konfigurationen
    int Event;
    ScannerKind Scanner;
    uint8_t Xor1, Xor2, Shift1, Shift2;  // Bitnummern in den Rohdaten und Invertierung
    uint8_t Param;
    uint8_t AnalogDeviceNumber;
    uint8_t ValidDuringReplay;
    uint32_t ButtonTexture;
    const char* ButtonName;
};

struct ValidJoystickConfiguration {  // 0x14 Byte im Original
    int JoystickType;
    uint8_t Configs[10];
    uint8_t DeadZone[6];  // je Achse 1..3: Hysterese, Totzone
};

extern const unsigned char kAnalogThresholds[3];
extern const int kNumScannerConfigs;
extern const ScannerConfig kScannerConfigs[];
extern const int kNumValidConfigs;
extern const ValidJoystickConfiguration kValidConfigs[];

struct EventNode {
    int Event = 0;                          // +0x08
    const ScannerConfig* Config = nullptr;  // +0x0c
    JoyHandler Handler = nullptr;           // +0x10
    void* Param = nullptr;                  // +0x14
    uint32_t Last = 0;                      // +0x18: letzter Wert (bei 2 Achsen: hohes Byte = zweite Achse)
    int Fresh = 0;                          // +0x1c: ≠ 0 -> nächster Scan übernimmt den Wert still bzw. meldet ihn
    int RepeatAt = 0;                       // +0x20: Bild, ab dem die Wiederholung feuert
};

struct JoystickAxis {  // 0x2c Byte
    bVector2 pts[4];
    Graph graph;
    uint8_t hysteresis = 0;
    void Init();
};

struct Joystick {  // 0x18c Byte im Original
    std::list<EventNode> nodes;
    JoystickAxis axis[4];
    int port = 0;           // +0x178
    int type = -1;          // +0x17c
    int pendingType = -1;   // +0x180
    int typeCounter = 0;    // +0x184
    int config = 0;         // +0x188 gewählte Konfigurationsnummer
    uint8_t eventState[0xbd] = {};  // +0xb8, letzter Wert je Ereignis
    Joystick();
};

struct JoyQueue {
    struct Entry {
        uint16_t result;  // (Wert << 8) + Ereignis
        uint8_t port;
        uint8_t event;
        JoyHandler fn;
        void* param;
    };
    int count = 0;
    Entry entries[0x100];
    void Push(uint16_t result, int port, int event, JoyHandler fn, void* param);
};

struct JoystickSystem {
    Joystick joys[2];
    JoyQueue queue;
    int frame = 0;          // orig: 0x7295a4
    int now = 0;            // orig: 0x72ca20
    int lastInput = 0;      // orig: 0x780474
    bool repeatActive = false;  // orig: *0x72960c ≠ 0 (zweite Aufzeichnungsschicht läuft): nur Scanner mit ValidDuringReplay
    bool inputDirty = false;  // orig: g_b324
    // liefert Rohbits (a aktuell, b vorher) und den Gerätetyp eines Ports
    std::function<void(int port, const uint32_t** a, const uint32_t** b, int* type)> poll;
    std::function<void(int port, int newType)> onTypeChanged;  // orig: 0x553a80 (Fahrer informieren, Tabellen leeren)
    int portType[2] = {0xff, 0xff};  // orig: 0x727dfc

    void Init();                                                       // orig: 0x553af0 InitJoystick
    void ReInitTypeMonitoring();                                       // orig: 0x553ac0
    bool AddJoyHandler(int port, int event, JoyHandler fn, void* param);  // orig: 0x553960
    void RemoveJoyHandler(int port, int event);                        // orig: 0x5539f0
    bool IsJoyHandlerAdded(int port, int event) const;                 // orig: 0x553a50
    int GenerateEventsFromScanners(Joystick& j, int type, const uint32_t* a, const uint32_t* b);  // orig: 0x5533f0
    void AddLoggedJoystickEvent(uint32_t packed);                      // orig: 0x553570
    void GenerateJoyEvents();                                          // orig: 0x5535f0
    uint16_t Scan(EventNode& n, const uint32_t* a, const uint32_t* b, const ScannerConfig& c, Joystick& j);
};

const ScannerConfig* FindScannerConfig(int event, int joystickType, int requestedConfig);  // orig: 0x5542e0
int GetValidConfigs(int joystickType, int* out, int max);                                  // orig: 0x5541a0
int ChooseJoystickConfigurationNumber(int joystickType, int requested);                    // orig: 0x554290
void ChooseDeadZoneAndHysterysis(JoystickAxis& axis, int axisIndex, int joystickType);      // orig: 0x554350
uint32_t ReadButtonPair(const ScannerConfig& c, const uint32_t* a, const uint32_t* b);      // orig: 0x553c20

}  // namespace nfsu
