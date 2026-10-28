# 2026-10-28 – Release-Hygiene (Teil 1)

Ziel: Repository so halten, dass es ohne Spieldaten veröffentlicht werden kann; Abgeleitetes entsteht beim Bauen aus der eigenen Installation.

## Erledigt
- `tools/pe_image.py`: liest Speed.exe direkt über die PE-Abschnittstabelle (ohne Unicorn), prüft SHA-256.
- `gen_bware_tables.py`, `gen_joystick_tables.py`: nehmen `[Speed.exe] [Ausgabe]`, nutzen `PEImage`; Ausgabe bytegleich zur vorher eingecheckten Fassung.
- CMake: `NFSU_GAME_DIR` (Cache, Standard `game/pc`), Tabellen per `add_custom_command` nach `build/<preset>/generated/`; Abbruch mit Hinweis, wenn Speed.exe fehlt.
- `bware_tables.cpp`, `joystick_tables.cpp` und `decompiled/speed/src/` aus Git genommen (lokal bleibt `decompiled/speed/src` erhalten), `.gitignore` ergänzt.
- Build debug grün, ctest 100 %.

## Offen (Fortsetzung)
- **Git-Historie** enthält die entfernten Dateien weiterhin. Vor einer Veröffentlichung: Historie bereinigen (`git filter-repo`, braucht Rückfrage) oder öffentliches Repo frisch ohne Historie anlegen.
- `symbols/`, `plan/log/`, `decompiled/full/HANDOVER.md`, `tools/` auf eingebettete Spielinhalte (Strings, Bytes, Pseudocode-Auszüge) prüfen.
- Regel für künftige Pakete: Daten aus Speed.exe nur über Generator + `NFSU_GENERATED_SOURCES`.
