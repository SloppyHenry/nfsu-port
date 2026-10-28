// SDL3-Fenster mit SDL_GPU-Device (Metal). Mehr kommt in WP-40.
#pragma once

namespace nfsu {

// Öffnet ein Fenster, löscht den Bildschirm je Frame und endet bei Esc oder Schließen.
// frames > 0 beendet nach so vielen Frames (für automatische Läufe). Rückgabe: Exit-Code.
int run_clear_window(int frames);

}  // namespace nfsu
