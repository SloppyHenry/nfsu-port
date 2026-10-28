# NFSU macOS-Port

Rekonstruktion von *Need for Speed Underground* (PC, 2003) als natives macOS-arm64-Programm (C++17, SDL3, Metal).
**Frühes Stadium:** Datenformate (Chunks, Texturen, Geometrie, Audio, Video) und ein Strecken-Viewer laufen, die Spiellogik ist erst zu einem kleinen Teil rekonstruiert. Spielbar ist noch nichts.

Dieses Repository enthält **keine Spieldaten** und nichts, was aus `Speed.exe` kopiert ist. Zum Bauen braucht man eine eigene NFSU-PC-Installation; die nötigen Tabellen werden beim Konfigurieren daraus erzeugt (Prüfung per SHA-256).

```
brew install cmake ninja sdl3
cd port
cmake --preset release -DNFSU_GAME_DIR=/pfad/zu/NFSU
cmake --build --preset release
```

Aufbau: `port/` (Quellcode), `symbols/` (Namen und Typen), `tools/` (Hilfsskripte), `plan/` (Roadmap, Pakete, Logs).
