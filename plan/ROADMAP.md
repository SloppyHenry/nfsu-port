# Roadmap: NFS Underground nativ und plattformübergreifend

## Ziel
Ein natives Programm für macOS, Linux und Windows (64 Bit), das mit den Original-Spieldaten in `game/pc/` läuft und sich wie das PC-Original verhält. Kein Wine, keine Emulation zur Laufzeit.

## Strategie in einem Satz
Das PC-Binary wird **nicht** als Ganzes nach C übersetzt und zum Kompilieren gezwungen. Stattdessen wird das Spiel **Subsystem für Subsystem als sauberer C++-Code neu aufgebaut**. Das Ghidra-Dekompilat dient dabei als Nachschlagewerk. Bibliotheken werden durch moderne Gegenstücke ersetzt, und die Windows-Schicht wird gegen SDL3/Metal ausgetauscht.

## Schlüsselfund: GameCube-ELF mit vollen Symbolen (2026-10-20)
Das ISO des Nutzers (GameCube, `GNDD69`) enthält eine **nicht gestrippte `Speed.elf`** mit 10 650 benannten Funktionen, 4,2 MB DWARF-Typinformation und Quellpfaden (`../indep/src/World/Player.cpp` …). Siehe `FAKTEN.md` und WP-07.
Daraus folgt die **Zwei-Quellen-Strategie**:
- **GameCube-ELF = Quelle für Namen, Typen, Klassen und Quellbaum.** Der Port übernimmt den Originalquellbaum (`port/src/indep/...`), Klassen- und Funktionsnamen und Strukturlayouts.
- **Plattformunabhängiger Code (`indep/`)** wird bevorzugt aus dem **GameCube-Dekompilat** rekonstruiert (mit Typen, Namen und sauberen PPC-Registerargumenten) und gegen das PC-Gegenstück abgeglichen (D8).
- **Die PC-Exe bleibt maßgeblich** für PC-spezifischen Code (Direct3D-9-Renderer, Eingabe, PC-Dateiformate wie DXT-Texturen und Little-Endian-Daten) und für das Verhalten. Gespielt wird mit den PC-Daten.
- WP-08 überträgt die GC-Namen auf die PC-Exe. Danach sind beide Dekompilate symbolisiert.

## Warum nicht die Linie aus `decompiled/full/build2` weiterführen?
Der Vollexport `decompiled/full/out2` ist wertvoll und bleibt die **Referenzquelle** (alle 10 378 Funktionen als Text, ohne Token-Kosten über MCP). Das generierte Projekt `build2` taugt für macOS aber nicht als Grundlage:
1. Es greift auf Daten über **absolute 32-Bit-Originaladressen** zu (`*(undefined4*)0x00732104`). Unter macOS arm64 gibt es weder 32-Bit-Prozesse noch diese Adressen.
2. Rund 29 000 Registerartefakte (`in_EAX`, `unaff_ESI`) sind im C-Code **uninitialisierte Variablen**. Selbst fehlerfrei kompiliert wäre das Programm falsch.
3. Es enthält D3DX, die CRT und zlib als Dekompilat. Diese Teile wollen wir ersetzen, nicht übersetzen.
4. Selbst im besten Fall entstünde eine Windows-32-Bit-EXE, die nur unter Wine läuft. Das ist nicht das Ziel.

→ `build2` wird **eingefroren**, nicht gelöscht. `gen.py`, `check.sh` und die Ghidra-Skripte bleiben als Werkzeuge erhalten.

## Annahmen (nicht geprüft, je Paket zu verifizieren)
- A1: Rund 6 000–6 500 Funktionen sind echter Spiel- oder Engine-Code. Der Rest ist CRT, D3DX, zlib und EA-Bibliotheken, die ersetzt werden. **Geprüft in WP-02 (2026-10-21):** 5 620 Funktionen (54 %) sind zu rekonstruieren (Spiel, Engine, EAGLAnim, CSIS), 2 740 (26 %) werden ersetzt (CRT, D3DX9, EAGL, Dateien, RCMP, EA-Audio, PC-Plattform), 2 018 (19 %) entfallen (Online/Netz, Unwind-Funclets, Initialisierer, Thunks). Die Annahme stimmt: rund 5 600 statt 6 000–6 500.
- A2: Der `indep/`-Code von GC und PC stammt aus derselben Quelle und ist weitgehend gleich. Unterschiede gibt es bei Plattformcode, Inlining und eventuell Build-Stand. (Prüfung: WP-08)
- A2b: NFS Most Wanted (nfsmw-Decomp) teilt Engine-Teile. ~~Nach dem GC-Fund vor allem als Werkzeugvorbild nützlich.~~ **Neu bewertet 2026-10-27:** 1 237 der 6 177 `indep`-Funktionen von NFSU haben in MW dieselbe GNU-v2-Signatur (789 davon mit Größe ±10 %). Lesbarer C++-Code (CC0) liegt dort vor allem für FEng, TrackStreamer/World, bWare und EAXSound. Zusätzlich benennt `bchunks.txt` 68 unserer bisher namenlosen Chunk-IDs. Ungeprüft ist, wie oft der Code auch **inhaltlich** gleich ist. Das klärt die Stichprobe in WP-03. Die Layouts können abweichen (eSolid, WP-14). nfsmw ist damit **Code-Spender** für Phase 3. Quellen und Grenzen: `wp/WP-03-referenzen.md`.
- A3: Die Spieldaten enthalten 32-Bit-Zeiger bzw. Offsets, die beim Laden „in place“ umgeschrieben werden. Das ist das Hauptproblem bei 64 Bit. (Prüfung: WP-11)

## Phasen und Pakete

| Phase | Pakete | Ergebnis am Ende der Phase |
|---|---|---|
| **0 Fundament** | WP-00 Repo · WP-01 Symbol-DB · **WP-07 GC-ELF** · **WP-08 PC↔GC-Abgleich** · WP-02 Klassifikation · WP-03 nfsmw · WP-04 Typen | Beide Binaries sind symbolisiert und typisiert, der Quellbaum ist bekannt, der echte Umfang ist gezählt |
| **1 Prüfbarkeit** | WP-05 Emulations-Harness · WP-06 Original unter Wine (optional) | Einzelne Originalfunktionen lassen sich mit eigenen Eingaben ausführen und vergleichen |
| **2 Daten & Bausteine** | WP-10 Port-Gerüst · WP-11 64-Bit-Strategie · WP-12 JDLZ+Chunks · WP-13 Texturen · WP-14 Geometrie · WP-15 Audio · WP-16 Video · WP-17 Shader · WP-18 Strecken-Viewer | **Erster sichtbarer Meilenstein:** Eine Strecke lässt sich auf dem Mac frei abfliegen |
| **3 Spielcode** | WP-30 … WP-3x je Engine-Modul (Vorlage: `wp/WP-3x-VORLAGE.md`) | Logik verhaltensgleich in `port/src/` |
| **4 Plattform** | WP-40 Renderer · WP-41 Eingabe · WP-42 Audio-Ausgabe · WP-43 System/Dateien/Speicherstände · WP-44 Netzwerk-Stub | Alle Win32/DirectX-Aufrufe ersetzt |
| **5 Integration** | WP-50 Boot bis Frontend · WP-51 Erstes Rennen · WP-52 Vollständigkeit · WP-53 App-Bundle | Spielbares `.app` |

## Abhängigkeiten
```
WP-00 ─┬─ WP-07 (GC-ELF) ─────┐
       ├─ WP-01 ─┬────────────┴─ WP-08 (PC↔GC) ─┬─ WP-02 ──┬─ WP-30… (Spielcode, modulweise)
       │         ├─ WP-03 (nfsmw, Code-Spender) ─┤          │
       │         └───────────────────────────────┴─ WP-04 ──┘
       ├─ WP-05 (braucht nur WP-00; danach Begleiter aller Code-Pakete)
       └─ WP-10 ─┬─ WP-11 ─┬─ WP-12 ─┬─ WP-13 ─┐
                 │         │         ├─ WP-14 ─┼─ WP-18 (Viewer) ── WP-40
                 │         │         └─ WP-17 ─┘
                 ├─ WP-15, WP-16 (unabhängig, früh möglich)
                 └─ WP-41, WP-42, WP-43
WP-30… + WP-40…44 ── WP-50 ── WP-51 ── WP-52 ── WP-53
```
**Parallel möglich** (eigene Sessions, getrennte Dateien): WP-01, WP-07 und WP-10 nach WP-00. WP-12/15/16 nach WP-10. WP-13/14/17 nach WP-12. WP-02 und WP-04 nach WP-08.

## Offene Entscheidungen (Nutzer)
| ID | Frage | Empfehlung | Betrifft |
|---|---|---|---|
| D1 | Darf das Original unter Wine/CrossOver gestartet werden (Referenzbilder, Verhaltensvergleich)? | **erteilt 2026-10-20** | WP-06 |
| D2 | Darf Unicorn (`pip install unicorn`) installiert werden, um **einzelne** Originalfunktionen in einer Sandbox auszuführen? | **erteilt 2026-10-20** | WP-05 |
| D3 | Darf das nfsmw-Repository geklont werden (nur als Lesereferenz)? | **erteilt 2026-10-20**. Seit 2026-10-27 auch als Code-Vorlage (Lizenz CC0, Regeln in WP-03) | WP-03, WP-3x |
| D4 | GameCube-Fassung als Symbolquelle (eigener Dump)? | **erteilt 2026-10-20**. Das ISO liegt in `game/gc/`, die ELF hat volle Symbole | WP-07 |
| D8 | Aus welchem Binary wird `indep/`-Code rekonstruiert? | **erteilt 2026-10-20**: GC-Dekompilat (mit Typen) als Vorlage, PC-Exe als Abgleich und Verhaltensreferenz | WP-3x |
| D9 | Darf decomp-toolkit (`dtk`, Release-Binary von GitHub) heruntergeladen werden? | **erteilt 2026-10-20** | WP-07 |
| D5 | Grafik-Backend | SDL3 + SDL_GPU (Metal, Vulkan, D3D12), Shader per SDL_shadercross aus HLSL | WP-17, WP-19, WP-40 |
| D10 | Zielplattformen | macOS, Linux, Windows (64 Bit); Spielcode ohne Plattformcode (2026-10-28, Nutzer) | alle |
| D6 | Sprache des Ports | C++17 (Original ist MSVC-C++ mit thiscall-Klassen) | alle |
| D7 | Treue | Verhaltensgleich, **nicht** byte-identisch (kein Matching-Decomp) | alle |

## Aufwand (grobe Schätzung, ehrlich)
- Phase 0–2: etwa 15–25 Sessions. Danach gibt es einen Strecken-Viewer und alle Asset-Formate.
- Phase 3: Hauptlast. Ohne Symbole wären es bei ca. 6 000 Funktionen **100+ Sessions**. Mit Namen, Typen und Quellbaum aus der GC-ELF wird jede Funktion deutlich schneller und sicherer. Realistisch sind 100–150 statt 40–80 Funktionen pro Session, also etwa **40–70 Sessions**. Diese Schätzung wird nach WP-07 und WP-08 neu gemacht. Mit nfsmw-Vorlagen (rund 20 % der `indep`-Funktionen, gebündelt in FEng, Streaming, bWare und Audio) dürften WP-30, WP-32, WP-37 und WP-39 spürbar schneller gehen. **WP-03 hat nachgerechnet (2026-10-27):** Von den 5 620 zu rekonstruierenden PC-Funktionen haben 281 (5 %) eine Vorlage `wahrscheinlich` und weitere 416 eine `moeglich`; ungleich verteilt: WP-30 16 %, WP-31 14 %, WP-37 13 %, WP-32 5 %, WP-36 5 %, alle anderen unter 2 %. Dabei fehlen die PC-Funktionen ohne GC-Zuordnung (WP-08: 59 % des Spielbereichs). Direkt auf GC-Seite haben 742 von 6 177 `indep`-Funktionen eine Vorlage `wahrscheinlich`. Wirkung: WP-30, WP-31 und WP-37 gewinnen je 10–15 % Aufwand, die übrigen Pakete praktisch nichts. **Die Gesamtschätzung von 40–70 Sessions bleibt.** nfsmw hilft bei FEng, Basis und Streaming, nicht bei Rennlogik, KI und Physik.
- Phase 4–5: etwa 15–30 Sessions.
- Modellwahl: Opus für Architektur-Pakete (WP-04, 11, 17, 40, 50) und schwierige Module. Sonnet reicht für mechanische Pakete (WP-01, 02, 12, 13, 15, 16) und gut vorstrukturierte Module.
- **Haiku** nur für Pakete mit klarer Anleitung und automatischer Prüfung: WP-00, WP-06, Teile von WP-10. In Phase 3 nur für einfache Blattfunktionen, und nur, wenn jede Funktion per Emulator-Vergleich (WP-05) geprüft wird. **Nicht** für WP-01, 07, 08, 04 und 11. Deren Ergebnisse (Namen, Typen, Zuordnungen) sind die Quelle der Wahrheit, und ein Fehler dort wirkt in jedes spätere Paket hinein. Die frühere Haiku-Session hat viele Skizzen statt vollständiger Übersetzungen geliefert (177 von 258).

## Messgröße für Fortschritt
`symbols/functions.csv`, Spalte `status` (siehe WP-01): `unbearbeitet → benannt → typisiert → rekonstruiert → getestet` bzw. `ersetzt` oder `entfällt`. Ein Skript (`tools/progress.py`, WP-01) gibt die Quote je Modul aus. Diese Zahl ist die einzige Fortschrittsangabe, die zählt.
