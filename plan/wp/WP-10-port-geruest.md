# WP-10 Port-Gerüst (CMake, SDL3, Tests, Dateipfade)

**Phase 2 · Modell: Sonnet · Umfang: 1 Session · Voraussetzungen: WP-00**

## Ziel
Ein baubares, leeres Port-Projekt unter `port/`, in das alle weiteren Pakete Code ablegen.

## Struktur
```
port/
  CMakeLists.txt, CMakePresets.json   (debug: ASan+UBSan, release; arm64; C++17; -Wall -Wextra)
  src/core/        plattformfreie Engine-Basis (später bWare-Äquivalent)
  src/formats/     Dateiformate (JDLZ, Chunks, Texturen, Geometrie, Audio)
  src/game/        rekonstruierter Spielcode (Phase 3, Unterordner je Modul)
  src/platform/    SDL3-Schicht: Fenster, Eingabe, Audio, Zeit, Dateien (Phase 4)
  src/render/      Renderer (SDL_GPU)
  apps/nfsu/       das Spiel
  apps/nfsu-tool/  Kommandozeilenwerkzeug (dump, extract, convert)
  apps/viewer/     Strecken-/Modellviewer (WP-18)
  tests/           Unit-Tests; testlib/ für die Emulator-Brücke (libnfsu_core.dylib, WP-05)
  docs/            ADRs (Architekturentscheidungen)
```

## Schritte
1. CMake mit den Zielen `nfsu_core` (statisch), `nfsu_platform`, `nfsu`, `nfsu-tool`, `nfsu_tests`, `nfsu_core_shared` (dylib für WP-05).
2. SDL3 aus Homebrew per `find_package(SDL3)`. Test-Framework: ohne Download ein kleines eigenes Makro-Framework, oder mit Rückfrage doctest einbinden.
3. **Spieldatenpfad:**
   - Reihenfolge: `--data <pfad>`, dann `NFSU_DATA`, dann Standard `<repo>/game/pc`
   - Pfadauflösung **ohne Beachtung der Groß- und Kleinschreibung**, `\` → `/`. Das Spiel fragt `TRACKS\TRACKB%d.LZC` an, die Datei heißt `game/pc/Tracks/TRACKB1001.lzc`.
   - Ein Verzeichnis-Cache, damit das Nachschlagen nicht bei jedem Zugriff den Ordner durchsucht.
4. `apps/nfsu`: öffnet ein SDL3-Fenster mit einem SDL_GPU-Device (Metal), löscht den Bildschirm und beendet sich mit Esc.
5. `nfsu-tool ls <muster>`: zeigt aufgelöste Dateien. Das ist der Test für die Pfadauflösung.
6. `port/README.md`: Bauen in drei Befehlen.

## Fertig, wenn
- `cmake --preset debug && cmake --build --preset debug` läuft ohne Warnungen durch.
- Das Fenster öffnet sich, die Tests laufen, und `nfsu-tool ls 'TRACKS\TRACKB1001.LZC'` findet die Datei.
