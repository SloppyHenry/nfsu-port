# NFSU-Port

Rekonstruktion von *Need for Speed Underground* (PC, 2003) als natives, plattformübergreifendes Programm (C++17, SDL3; Ziel: macOS, Linux, Windows).
Entwickelt wird derzeit auf macOS; die Grafik hängt bis WP-19 noch an Metal.
**Frühes Stadium:** Datenformate (Chunks, Texturen, Geometrie, Audio, Video) und ein Strecken-Viewer laufen, die Spiellogik ist erst zu einem kleinen Teil rekonstruiert. Spielbar ist noch nichts.

Dieses Repository enthält **keine Spieldaten** und nichts, was aus `Speed.exe` kopiert ist. Zum Bauen braucht man eine eigene NFSU-PC-Installation; die nötigen Tabellen werden beim Konfigurieren daraus erzeugt (Prüfung per SHA-256).

Voraussetzungen: CMake ≥ 3.24, Ninja, Python 3, C++17-Compiler (Clang, GCC oder MSVC), SDL3, zlib, FFmpeg-Bibliotheken.

```
cd port
cmake --preset release -DNFSU_GAME_DIR=/pfad/zu/NFSU
cmake --build --preset release
```

- `-DNFSU_FETCH_DEPS=ON` baut SDL3 und zlib selbst (praktisch unter Windows)
- `-DFFMPEG_ROOT=…` zeigt auf ein FFmpeg-Entwicklerpaket, wenn kein pkg-config da ist (Windows)
- `-DNFSU_STUB_TABLES=ON` baut ohne `Speed.exe` (nur Kompilierprüfung; so arbeitet die CI unter `.github/workflows/build.yml` für Linux, macOS und Windows)
- Grafik: Shader gibt es bis WP-19 nur für Metal; auf Linux und Windows baut der Viewer, rendert aber noch nicht.

Aufbau: `port/` (Quellcode), `symbols/` (Namen und Typen), `tools/` (Hilfsskripte), `plan/` (Roadmap, Pakete, Logs).
