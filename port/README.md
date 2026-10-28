# macOS-Port (CMake, C++17, SDL3)

Aufbau und Ziele: `plan/wp/WP-10-port-geruest.md`. Voraussetzungen: `brew install cmake ninja sdl3`.

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Starten:

```bash
build/debug/nfsu --data ../game/pc        # Fenster, Esc beendet
build/debug/nfsu-tool ls 'TRACKS\TRACKB1001.LZC'
```

Spieldatenpfad: `--data <pfad>`, dann `NFSU_DATA`, dann `<repo>/game/pc`. Pfade werden ohne Beachtung der Groß-/Kleinschreibung aufgelöst, `\` gilt als `/`.

Ordner: `src/core` (plattformfrei), `src/formats`, `src/game` (rekonstruierter Code), `src/platform` (SDL3), `src/render`, `apps/`, `tests/`, `docs/` (ADRs).

## Spieldaten
Das Repository enthält keine Spieldaten und nichts, was aus `Speed.exe` abgeleitet ist.
Beim Konfigurieren liest CMake die Datentabellen aus der eigenen NFSU-PC-Installation (`tools/gen_*_tables.py`, Prüfung per SHA-256 in `tools/pe_image.py`) und legt sie unter `build/<preset>/generated/` ab.
Standard ist `game/pc/`, sonst: `cmake --preset debug -DNFSU_GAME_DIR=/pfad/zu/NFSU`.
