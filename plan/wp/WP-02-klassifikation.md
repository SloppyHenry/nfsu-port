# WP-02 Klassifikation aller Funktionen und Modulgrenzen

**Phase 0 · Modell: Opus · Umfang: 1 Session · Voraussetzungen: WP-01, WP-08**

> **Seit dem GC-Fund ist das Paket kleiner:** Für die Spielfunktionen liefert WP-08 das Modul direkt (`module` = Quelldatei aus der GC-ELF, z. B. `indep/src/World/Player.cpp`). Hier geht es vor allem noch um den **Rest der PC-Exe**: Bibliotheken (CRT, D3DX, zlib, RCMP, EA-Sound), PC-only-Code und nicht zugeordnete Funktionen. Für die Modulliste von Phase 3 dient `plan/QUELLBAUM.md` (WP-07) als Ausgangspunkt. Ein Modul ist dort ein Quellordner bzw. eine Gruppe zusammengehöriger Quelldateien.

## Ziel
Für jede der 10 378 Funktionen steht fest: **was es ist** (`kind`) und **zu welchem Modul es gehört** (`module`). Daraus folgt der echte Umfang von Phase 3 und die Liste der Module als WP-30 ff.

## Methoden (kombinieren, Beleg je Modul notieren)
1. **Linkreihenfolge:** MSVC legt die Objektdateien zusammenhängend ab. Ein Modul ist meist ein zusammenhängender Adressbereich. Grenzen findest du an Ausrichtungslücken (`int3`/`0xCC`-Füllung), am Wechsel der Aufrufmuster und an String-Clustern in `.rdata`, die in derselben Reihenfolge liegen.
2. **Strings:** Debug-, Assert- und Dateinamen (`src/av/cmn/avplayer.cpp`, `OnlineManager::Update`, `SNDSYS_inited()`, `TRACKS\TRACKB%d.LZC`, `LoadingFrontEnd` …).
3. **Bibliotheken:**
   - CRT: Ghidra-FID (bereits 1 387 erkannt), Rest über die Aufrufer
   - zlib 1.1.4: Tabellen (CRC32, `inflate`-Längentabellen) als Konstanten
   - D3DX: Effekt- und Shadercompiler-Strings (0x68bf9c–0x6d90f4), Aufrufgraph von dort
   - RCMP/AV-Player
   - EA-Sound
   - Netzwerk (WS2_32, TAPI)
4. **Aufrufgraph:** Eine Funktion, die nur aus Bibliothekscode aufgerufen wird und selbst nur Bibliothekscode aufruft, gehört zur Bibliothek.
5. **Imports:** Wrapper um D3D9-, DSOUND-, DINPUT- und Win32-Aufrufe → `win32glue`. Das ist die spätere Plattformschicht (Phase 4).

## Ergebnisse
- `symbols/modules.csv`: `module,start,end,kind,anzahl,behandlung,belege`
  - `behandlung`:
    - `rekonstruieren` (Spiel/Engine → Phase 3)
    - `ersetzen` (durch Bibliothek oder Plattformschicht, mit Angabe womit)
    - `entfällt` (CRT-Interna, SafeDisc-Reste, Online, TAPI-Modem …)
- `functions.csv`: `kind` und `module` für alle Zeilen gesetzt, `status=entfällt|ersetzt` wo zutreffend.
- `plan/MODULE.md`: Tabelle der Phase-3-Module mit Funktionsanzahl, Größe in Bytes, Abhängigkeiten (aus dem Aufrufgraph) und **vorgeschlagener Reihenfolge** (Basis zuerst). Jedes Modul bekommt eine Paketnummer WP-30, WP-31 … (siehe `WP-3x-VORLAGE.md`).
- Aktualisierung von Annahme A1 in `ROADMAP.md` mit der echten Zahl.

## Fertig, wenn
- Höchstens 5 % der Funktionen haben `kind=unbekannt`.
- `MODULE.md` existiert, und `STATUS.md` enthält eine Zeile je Phase-3-Modul.
- Stichprobe: 20 zufällige Funktionen manuell gegen ihre Zuordnung geprüft, Ergebnis im Log.
