#include "game/misc/timer.hpp"

#include <cstring>

#include "game/base/bmath.hpp"

namespace nfsu {

// orig: 0x8025e444 (GC) Timer::SetTime
void Timer::SetTime(float seconds) { PackedTime = static_cast<int>(seconds * 4000.0f + 0.5f); }

// orig: 0x562600 Timer::GetHoursMinsSeconds
// PC weicht von GC ab: Die Tausendstel berechnet PC als (Rest * 1000) / 4000, GC als Rest / 4 (gleiches Ergebnis
// für Rest >= 0).
void Timer::GetHoursMinsSeconds(int* hours, int* minutes, int* seconds, int* thousandths) const {
    int total = PackedTime / kTicksPerSecond;
    *hours = total / 3600;
    total %= 3600;
    *minutes = total / 60;
    *seconds = total % 60;
    *thousandths = ((PackedTime % kTicksPerSecond) * 1000) / kTicksPerSecond;
}

// orig: 0x562690 Timer::PrintToString
void Timer::PrintToString(char* string, int flags, int format) const {
    if (!IsRunning()) {
        nfsu::PrintToString(string, static_cast<uint32_t>(flags), 0, 0, 0, 0, format);
        return;
    }
    int h, m, s, ms;
    Timer abs_t;
    abs_t.PackedTime = PackedTime < 0 ? -PackedTime : PackedTime;
    abs_t.GetHoursMinsSeconds(&h, &m, &s, &ms);
    if (PackedTime < 0) flags |= 1;
    nfsu::PrintToString(string, static_cast<uint32_t>(flags), h, m, s, ms / 10, format);
}

// orig: 0x562750 timer_tick_down (GC 0x801933d4 Timer::CountDown)
// Zieht fSeconds ab (Kürzen), bei < 0 wird 0. Rückgabe: läuft noch.
bool Timer::CountDown(float fSeconds) {
    if (IsRunning()) {
        PackedTime -= static_cast<int>(fSeconds * 4000.0f);
        if (PackedTime < 0) PackedTime = 0;
    }
    return IsRunning();
}

// orig: 0x5627a0 Timer::RoundTime (PC liefert das gerundete Ergebnis in Sekunden, GC 0x80193468 GetSecondsRounded)
float Timer::GetSecondsRounded(float fIotaSeconds) const {
    float q = (static_cast<float>(PackedTime) * 0.00025f) / fIotaSeconds;
    float f = bFloorF(q);
    if (0.5f <= q - f) f = f + 1.0f;
    return bCeil(f * fIotaSeconds * 4000.0f) * 0.00025f;
}

// orig: 0x8019356c (GC) Timer::RoundTime
void Timer::RoundTime(float fIotaSeconds) { PackedTime = static_cast<int>(GetSecondsRounded(fIotaSeconds) * 4000.0f + 0.5f); }

// orig: 0x561d30 PrepareRealTimestep. Die Zeit wird auf mindestens ein und höchstens maxFrames Bilder begrenzt, auf 1/3600 s
// quantisiert (das ist der Wert, der im Joylog steht) und der Rest zur 1/4000-s-Rasterung für den nächsten Schritt gemerkt.
void GameClock::PrepareRealTimestep(float video_time_elapsed, int replayQ) {
    int q;
    if (replayQ >= 0) {
        q = replayQ;
    } else {
        float frame = ntsc ? 0.016666668f : 0.02f;
        float x = video_time_elapsed * 1.0167f + RealTimeQuantError;
        float lim = maxFrames * frame;
        if (!(x > frame)) x = frame;
        if (lim < x) x = lim;
        q = static_cast<int>(x * 3600.0f + 0.5f);
        int ticks = static_cast<int>(static_cast<float>(q) * 1.1111112f + 0.5f);
        RealTimeQuantError = x - static_cast<float>(ticks) * 0.00025f;
    }
    RealTimeQuantized = q;
    RealTimeElapsed = static_cast<float>(q) * 0.00027777778f;
}

// orig: 0x561e80 AdvanceRealTime
void GameClock::AdvanceRealTime() {
    float sum = RealTimeElapsed + RealTimeElapsedFrame;
    RealTimeFramesElapsed = static_cast<int>(sum * 60.0f);
    RealTimeElapsedFrame = sum - static_cast<float>(RealTimeFramesElapsed) * 0.016666668f;
    if (RealTimeFramesElapsed == 0) RealTimeFramesElapsed = 1;
    RealTimeFrames += RealTimeFramesElapsed;
    RealTimer += static_cast<int>(RealTimeElapsed * 4000.0f + 0.5f);
    ++RealLoopCounter;
}

// GC 0x80192a0c
void GameClock::ResetWorldTime() {
    WorldTimer = 4000;
    WorldTimeSeconds = 1.0f;
    WorldTimeFrames = 1;
    WorldTimeElapsedFrame = 0.0f;
    WorldLoopCounter = 0;
    WorldTimeFramesElapsed = 0;
    WorldTimeElapsed = 0.0f;
}

// GC 0x80192a58
void GameClock::PrepareWorldTimestep(float elapsed) {
    WorldTimeElapsed = elapsed;
    NeedToPrepareWorldTimestep = 0;
}

// orig: 0x561f10 AdvanceWorldTime: steht die Welt (Schritt 0), bleibt alles stehen
void GameClock::AdvanceWorldTime() {
    NeedToPrepareWorldTimestep = 1;
    if (WorldTimeElapsed != 0.0f) {
        float sum = WorldTimeElapsedFrame + WorldTimeElapsed;
        WorldTimeFramesElapsed = static_cast<int>(sum * 60.0f);
        WorldTimeFrames += WorldTimeFramesElapsed;
        WorldTimeElapsedFrame = sum - static_cast<float>(WorldTimeFramesElapsed) * 0.016666668f;
        WorldTimer += static_cast<int>(WorldTimeElapsed * 4000.0f + 0.5f);
        ++WorldLoopCounter;
        WorldTimeElapsed = 0.0f;
        WorldTimeSeconds = static_cast<float>(WorldTimer) * 0.00025f;
    }
}

// orig: 0x5620b0 GetDebugRealTime: Bildzähler mal Bilddauer plus die seit Bildbeginn verstrichene Zeit (höchstens ein Bild)
float GameClock::GetDebugRealTime() const {
    float frame = ntsc ? 0.016666668f : 0.02f;
    float base = static_cast<float>(frameCounter) * frame;
    if (frameStartQpc == 0 || !qpc) return base;
    int d = (qpc() >> qpcShift) - frameStartQpc;
    if (d < 0) d = 0;
    float elapsed = static_cast<float>(1 << qpcShift) * static_cast<float>(d) * timeScale * 0.001f;
    return (frame < elapsed ? frame : elapsed) + base;
}

namespace {

// Schreibt `digits` Ziffern von `value` rückwärts ab `last`; führende Null bei `blank_zero` als Leerzeichen
// (nur wenn der Rest der Zahl schon 0 und der Wert < 10, d. h. die Zehnerstelle).
void put_digits(char* last, int value, int digits, bool blank_leading) {
    for (int i = 0; i < digits; ++i) {
        if (blank_leading && value % 10 == 0 && value < 10)
            *last = ' ';
        else
            *last = static_cast<char>(value % 10 + '0');
        --last;
        value /= 10;
    }
}

const char* pick(int format, bool zeros, const char* apos_nozero, const char* apos_zero, const char* colon_nozero,
                 const char* colon_zero, const char* dot_nozero, const char* dot_zero) {
    switch (format) {
        case 0:
        case 2: return zeros ? apos_zero : apos_nozero;
        case 1:
        case 3:
        case 5: return zeros ? colon_zero : colon_nozero;
        default: return zeros ? dot_zero : dot_nozero;
    }
}

// Platz für das Vorzeichen: Text um eine Stelle nach rechts schieben, Zeichen 0 bleibt (orig: strlen + zweimal str_copy_n).
void shift_right_one(char* s) {
    size_t n = std::strlen(s);
    char tmp[32];
    std::memcpy(tmp, s, n - 1);
    tmp[n - 1] = 0;
    std::memcpy(s + 1, tmp, n - 1);
    s[n] = 0;  // orig: str_copy_n kopiert n-1 Zeichen und schließt mit 0 ab
}

}  // namespace

// orig: 0x5621a0 PrintToString (Zeichenvorlagen aus der .rdata, Ziffern von rechts eingesetzt)
// `format` = Wert des Einheitenschalters (Global 0x6e0f60); das Original rechnet mit `format - 1`.
void PrintToString(char* out, uint32_t flags, int hours, int minutes, int seconds, int hundredths, int format) {
    const int fmt = format - 1;
    const bool zeros = (flags & 4) != 0;
    const char* tpl;
    if (hours == 0) {
        if (minutes == 0) {
            if (seconds == 0) {
                tpl = pick(fmt, zeros, "    0\"--", " 0'00\"00", "    0:--", " 0:00:00", "    0.--", " 0:00.00");
                std::strcpy(out, tpl);
                if (!zeros) {
                    put_digits(out + 7, hundredths, 2, false);
                } else {
                    out[1] = '0';
                    put_digits(out + 4, 0, 2, false);
                    put_digits(out + 7, hundredths, 2, false);
                }
            } else {
                tpl = pick(fmt, zeros, "   --\"--", " 0'00\"00", "   --:--", " 0:00:00", "   --.--", " 0:00.00");
                std::strcpy(out, tpl);
                if (!zeros) {
                    put_digits(out + 4, seconds, 2, true);
                    put_digits(out + 7, hundredths, 2, false);
                } else {
                    out[1] = '0';
                    put_digits(out + 4, seconds, 2, false);
                    put_digits(out + 7, hundredths, 2, false);
                }
            }
        } else {
            tpl = pick(fmt, zeros, "--'--\"--", "00'00\"00", "--:--:--", "00:00:00", "--:--.--", "00:00.00");
            std::strcpy(out, tpl);
            put_digits(out + 1, minutes, 2, true);
            put_digits(out + 4, seconds, 2, false);
            put_digits(out + 7, hundredths, 2, false);
            if ((flags & 3) != 0 && minutes > 9) shift_right_one(out);
        }
    } else {
        switch (fmt) {
            case 0:
            case 2: tpl = zeros ? "00h:00'00\"" : "--h:--'--\""; break;
            default: tpl = zeros ? "00h:00:00" : "--h:--:--"; break;
        }
        std::strcpy(out, tpl);
        put_digits(out + 1, hours, 2, true);
        put_digits(out + 5, minutes, 2, false);
        put_digits(out + 8, seconds, 2, false);
        if ((flags & 3) != 0 && hours > 9) shift_right_one(out);
    }
    if (flags & 1)
        out[0] = '-';
    else if (flags & 2)
        out[0] = '+';
}

}  // namespace nfsu
