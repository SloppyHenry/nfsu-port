#pragma once
// ../indep/src/misc/Timer.cpp: Zeit als Ganzzahl in 1/4000 s ("PackedTime").
#include <cstdint>

namespace nfsu {

struct Timer {
    int32_t PackedTime = 0;  // 0 = nicht gesetzt, 0x7fffffff = unendlich

    static constexpr int kTicksPerSecond = 4000;

    bool IsRunning() const { return PackedTime != 0 && PackedTime != 0x7fffffff; }
    void SetTime(float seconds);
    void GetHoursMinsSeconds(int* hours, int* minutes, int* seconds, int* thousandths) const;
    // Formatiert in `string` (mind. 12 Byte). `format` ist der Wert des Einheitenschalters (orig: Global 0x6e0f60).
    void PrintToString(char* string, int flags, int format) const;
    bool CountDown(float fSeconds);
    float GetSecondsRounded(float fIotaSeconds) const;
    void RoundTime(float fIotaSeconds);
};

// orig: 0x5621a0 PrintToString. flags: 1 = Minus, 2 = Plus (beide schieben eine zweistellige Zahl nach rechts, damit
// das Vorzeichen Platz hat), 4 = Nullen statt Platzhalter ("--"). `format` wie in Timer::PrintToString.
void PrintToString(char* out, uint32_t flags, int hours, int minutes, int seconds, int hundredths, int format);


// Zeitgeber des Spiels (Timer.cpp). Zwei Uhren: "Echtzeit" (läuft immer) und "Weltzeit" (steht bei Pause).
// Beide zählen Bilder zu 1/60 s und Ticks zu 1/4000 s. Im Original sind das Globals (siehe Kommentare).
struct GameClock {
    // Echtzeit
    float RealTimeElapsed = 0.0f;       // 0x72ca1c: Schrittdauer in Sekunden (auf 1/3600 s quantisiert)
    float RealTimeElapsedFrame = 0.0f;  // 0x7295b0: Rest unterhalb eines Bildes
    float RealTimeQuantError = 0.0f;    // 0x7295b4: Rest der Quantisierung auf 1/4000 s
    int RealTimeQuantized = 0;          // 0x7295ac: Schritt in 1/3600 s
    int RealTimeFramesElapsed = 0;      // 0x6e30bc
    int RealTimeFrames = 1;             // 0x6e30b8 (RealTime 0x6e30b4 ist dieselbe Zahl)
    int RealTimer = 0;                  // 0x72ca20: Ticks 1/4000 s
    int RealLoopCounter = 0;            // 0x7295b8
    // Weltzeit
    float WorldTimeElapsed = 0.0f;      // 0x7295c0
    float WorldTimeElapsedFrame = 0.0f; // 0x7295c4
    int WorldTimeFramesElapsed = 0;     // 0x7295bc
    int WorldTimeFrames = 1;            // 0x6e30cc (WorldTime 0x6e30c8)
    int WorldTimer = 0;                 // 0x72ca18: Ticks 1/4000 s
    int WorldLoopCounter = 0;           // 0x6e30d0
    float WorldTimeSeconds = 1.0f;      // 0x6e30c4
    int NeedToPrepareWorldTimestep = 1; // 0x6e30d4
    // Einstellungen
    bool ntsc = true;                   // 0x6e30b0: 60 Hz statt 50 Hz
    float maxFrames = 3.0f;             // 0x6e30c0: längster Schritt in Bildern
    int frameCounter = 0;               // 0x7295a4
    int frameStartQpc = 0;              // 0x7295a8, 0 = nicht gestartet
    int qpcShift = 0;                   // 0x7299fc
    float timeScale = 1.0f;             // g_time_scale
    int (*qpc)() = nullptr;             // QueryPerformanceCounter (obere Hälfte, vor dem Verschieben)

    static int GetQuantizedRealTimeElapsed(float t) { return static_cast<int>(t * 3600.0f + 0.5f); }  // GC 0x801927e4
    static float GetRealTimeElapsedFromQuantized(int q) { return static_cast<float>(q) * 0.00027777778f; }  // GC 0x801927a8

    // `replayQ`: bei Wiedergabe der Wert aus dem Joylog (Kanal 3), sonst < 0
    void PrepareRealTimestep(float video_time_elapsed, int replayQ = -1);  // orig: 0x561d30
    void AdvanceRealTime();                                                 // orig: 0x561e80
    void ResetWorldTime();                                                  // GC 0x80192a0c
    void PrepareWorldTimestep(float elapsed);                               // GC 0x80192a58
    void AdvanceWorldTime();                                                // orig: 0x561f10
    float GetDebugRealTime() const;                                         // orig: 0x5620b0
};

}  // namespace nfsu
