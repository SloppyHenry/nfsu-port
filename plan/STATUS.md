# Status der Arbeitspakete

Status: `offen` (Voraussetzungen fehlen) · `bereit` · `in Arbeit` · `teilweise` · `erledigt` · `wartet` (Nutzerentscheidung)

**Startprompt für eine neue Session** (WP-Nummer einsetzen):
> Lies `CLAUDE.md`, `plan/STATUS.md` und `plan/wp/WP-xx-….md`. Arbeite WP-xx ab und halte dich an den Session-Ablauf aus `CLAUDE.md`.

| WP | Titel | Phase | Voraussetzungen | Status | Zuletzt | Ergebnis |
|---|---|---|---|---|---|---|
| 00 | Repository und Struktur | 0 | – | **erledigt** | 2026-10-20 | Git-Repo, privat auf GitHub (`SloppyHenry/NFSU`) |
| 01 | Symbol-DB, dauerhaftes Ghidra, Werkzeuge | 0 | 00 | **erledigt** | 2026-10-20 | `symbols/functions.csv` (10 378, 343 Altnamen), `project_persist`, `reexport.sh` → `out3` mit Callgraph usw., `fn.py`, `progress.py`; Rundlauf bewiesen |
| 07 | **GC-ELF auswerten (Namen, Typen, Quellbaum)** | 0 | 00 | **erledigt** | 2026-10-20 | `gc_functions.csv` (10 650, alle C++-Namen demangelt, 97 % mit Objekt, 90 % sicher), `gc_types.h` (751 Strukturen, Größen geprüft), `QUELLBAUM.md`, Ghidra-GC-Export (0 Fehler), `fn.py --gc`; Compiler ist GCC 2.95 (SN), nicht CodeWarrior |
| 08 | **Abgleich PC ↔ GC** | 0 | 01, 07 | **erledigt** | 2026-10-28 | `pc_gc_map.csv` (4 776 Paare), 2 569 Namen in `functions.csv`; Spielbereich 66,5 % zugeordnet, 42,7 % ≥ wahrscheinlich (Ziel angepasst auf 45 % ≥ wahrscheinlich bzw. 70 % aller Stufen, knapp verfehlt (42,7 / 66,5 %), vom Nutzer als erledigt akzeptiert; Literal-Beleg ergänzt; Kreuzvalidierung 94–96 %; Vorschlag: Ziel anpassen, siehe Log 08b) |
| 02 | Klassifikation Rest-PC (Bibliotheken), Module | 0 | 01, 08 | **erledigt** | 2026-10-21 | alle 10 378 Funktionen mit `kind`/`module` (0 unbekannt); `modules.csv` (88+ Module), `plan/MODULE.md` mit 10 Bereichen WP-30…39 und 48 Paketen; 5 620 rekonstruieren / 2 740 ersetzen / 2 018 entfällt |
| 03 | **nfsmw als Code-Spender** | 0 | 01, 07, 08 | **erledigt** | 2026-10-27 | `nfsmw_map.csv` (1 506: 737 wahrscheinlich, 455 möglich, 314 Name), Stichprobe 30 Paare (wahrscheinlich 8/8 brauchbar, möglich 3/8, Name 3/10), `chunks.csv` +`name_mw` (92/116), Anteil je Paket in `pakete.csv`; Fund: 873 GC-Funktionen im Export leer (`halt_baddata`) |
| 04 | Typen auf PC übertragen, Vtables, COM | 0 | 01, 07, 08 | **erledigt** | 2026-10-21 | `vtables.csv` (462, 209 benannt), COM-Tabellen (33 Schnittstellen, 272 Aufrufe zugeordnet), `types.h` (755 Strukturen, PC-Layout berechnet), `ApplyTypes.java`; Offsetausdrücke −10 % |
| 05 | Emulations-Harness (Unicorn) | 1 | 00 | **erledigt** | 2026-10-20 | `tools/emu` (Unicorn 2.1.4): 3 Funktionen × 10 000 Zufallseingaben bitgleich zum Port (name_hash, reset_identity_matrix, JDLZ) |
| 06 | Original unter Wine (optional) | 1 | – | **erledigt** | 2026-10-21 | GPTK-Wine 7.7; mit gepatchter Kopie (`tools/wine/patch_copy.py`, ohne Sound) läuft `Speed.exe` dauerhaft; Absturzursachen geklärt; Screenshots macht der Nutzer selbst, Dateiprotokoll bewusst nicht aufgenommen (Nutzen gering, siehe Log) |
| 10 | Port-Gerüst | 2 | 00 | **erledigt** | 2026-10-20 | CMake/Ninja, SDL3+SDL_GPU (Metal) Fenster, Pfadauflösung ohne Groß/Klein, nfsu-tool ls, Tests |
| 11 | 64-Bit-Strategie (ADR) | 2 | 01, 10 | **erledigt** | 2026-10-21 | `ADR-001-64bit.md`: `ptr32<T>` (Arena-Offset) für dateigebundene Strukturen, native Zeiger für Laufzeitobjekte; Prototyp `TextureInfo` (0x7c) gegen 1 820 echte Einträge; unter-4-GiB-Speicher auf macOS arm64 nicht machbar |
| 12 | JDLZ und Chunks | 2 | 10, (05, 01) | **erledigt** | 2026-10-27 | Chunk-Parser + `nfsu-tool chunks/unpack`; 253/253 Chunkdateien parsen restlos, 34/34 `.lzc` entpacken und sind **im Emulator bitgleich zum Original** (`tools/emu/compare_lzc.py`); `chunks.csv` 116 IDs (24 ohne Namen) |
| 13 | Texturen | 2 | 12 | **erledigt** | 2026-10-21 | 27 458/27 458 Texturen aus 347 Paketen als PNG (`nfsu-tool tex`); DXT1/3, ARGB, P8 (Vinyls), Einzel-JDLZ je Textur; TextureInfo/Kopf/PlatInfo mit `static_assert` (PC-Kopf ≠ GC); Kontaktbögen Auto/Strecke/Vinyl geprüft |
| 14 | Geometrie | 2 | 12, 13 | **erledigt** (Geometrie), Streckentopologie/Kollision offen (WP-32) | 2026-10-27 | eSolid/eModel-Kette (0x523f30/0x524010/0x40b760), 3 Vertexformate (36/24/60 B); Material je Draw-Batch (StripEntry); alle 39 Autos (9389 Solids) und alle 5 Streckenpakete (8794 Solids, 272 leere übersprungen) als glTF (`nfsu-tool mesh`) |
| 15 | Audioformate | 2 | 10 | **erledigt** | 2026-10-21 | eigener EA-XA-R2-Decoder, 6/6 `.ast` bitgleich zu FFmpeg (SpecialStream je Strom); 106/106 `.abk` mit 563 Sounds (PCM16 + EA-XA) lesbar und exportierbar; Objektabbild der Bänke (IDs, Hüllkurven) offen für WP-39; nicht angehört |
| 16 | Video (MAD) | 2 | 10, (15) | **erledigt** | 2026-10-27 | libavcodec (ADR-002); 45/45 Filme bitgleich zu ffmpeg; Gleichlauf (Ton vom Nutzer angehört, sauber): Ton vorab im Strom, Bildtakt ab Tonstart (Verspätung im Mittel < 4 ms); Seitenverhältnis gehört zu den FEng-Daten (WP-37/38) |
| 17 | Shader und Renderzustände | 2 | 10, 04 | **erledigt** (bis auf Lauf im Viewer) | 2026-10-27 | 7 Effekte, Setzstellen und Vertexdeklarationen dokumentiert (`plan/RENDER-ZUSTAENDE.md`), `nfsu-tool efx-decode`, `ADR-003`; alle 7 Effekte als MSL neu geschrieben und kompiliert (nicht gelaufen); Test im Viewer gehört zu WP-18 |
| 18 | Strecken-Viewer | 2 | 13, 14, 17 | **erledigt** (Tasteneingabe im Fenster ungetestet) | 2026-10-27 | `nfsu-viewer` rendert alle Regionen texturiert mit Streckenführung aus TRACKB, Materialtransparenz (Test/Blend/additiv), 60 fps; Szenerie-Schlüssel ist `name_hash(Solidname)`; offen: Nebel/Licht, dynamisches LOD, Eingabetest |
| 19 | **Plattformunabhängige Grafik und Build** (D10) | 2 | 10, 17, 18 | bereit | 2026-10-28 | Shader HLSL → SPIR-V/DXIL/MSL, Device ohne Metal-Zwang; einziger macOS-gebundener Teil |
| 30 | Basis (bWare), Hilfsfunktionen, Dateien, Spielablauf | 3 | 01, 02, 03, 04, 05, 07, 08, 10, 11 | **teilweise** (30.1: Spline, Point, Timer; 30.2: bWare-Mathe, QuickSpline, Listen; 30.3: Table, Average/PidError, LZ) | 2026-10-28 | 436 Fkt., 3 Pakete; 30.1 hat 80 von 144 Funktionen; 30.2 (Teil): bSin/bASin/bATan/bFixATan, Zufall, Zeichenketten, Vektoren, Boxen, Matrizen, Quaternionen, QuickSpline (13/14), bList; 58 Funktionen im Emulator getestet. Rest von 30.2: bFile (WP-43-nah), Localize, Slider, EasterEggs, Speicherpools, Plattformzeitgeber; 30.3 (Teil): Table/Average/PidError/Linear/LZCompress, 26 Fkt.; Rest (Speicherpool, bOutput, utillib-Block) offen |
| 31 | Ecstasy-Engine, Render-Spielseite | 3 | wie 30, nach 30 | offen | | 257 Fkt., 3 Pakete |
| 32 | Welt-Infrastruktur (Streaming, Topologie, Weltobjekte) | 3 | wie 30, nach 30 | offen | | 409 Fkt., 4 Pakete |
| 33 | Spielwelt und Rennen, Fahrzeugdaten, Karriere, Replay | 3 | wie 30, nach 30–32 | offen | | 768 Fkt., 7 Pakete |
| 34 | Physik und Fahrzeugdynamik | 3 | wie 30, nach 32, 33 | offen | | 339 Fkt., 3 Pakete |
| 35 | KI und Verkehr | 3 | wie 30, nach 32–34 | offen | | 466 Fkt., 4 Pakete |
| 36 | Kamera, EAGLAnim, NIS | 3 | wie 30, nach 32 | offen | | 417 Fkt., 4 Pakete |
| 37 | Frontend-Engine (FEng, Fe) | 3 | wie 30, nach 31 | offen | | 815 Fkt., 6 Pakete |
| 38 | Frontend-Bildschirme (FePkg) | 3 | wie 30, nach 37 | offen | | 1 242 Fkt., 10 Pakete |
| 39 | Audio-Spiellogik (EAXSound, CSIS) | 3 | wie 30, nach 33 | offen | | 471 Fkt., 4 Pakete |
| 40 | Renderer | 4 | 04, 10, 17 | offen | | |
| 41 | Eingabe | 4 | 04, 10 | offen | | |
| 42 | Audio-Ausgabe | 4 | 10, 15 | offen | | |
| 43 | System, Dateien, Speicherstände | 4 | 02, 10 | offen | | |
| 44 | Netzwerk-Stub | 4 | 02 | offen | | |
| 50 | Boot bis Frontend | 5 | Phase 3 (Basis, Frontend), 40, 41, 43 | offen | | |
| 51 | Erstes Rennen | 5 | 50, Phase 3 (Rest) | offen | | |
| 52 | Vollständigkeit | 5 | 51 | offen | | |
| 53 | App-Bundle | 5 | 52 | offen | | |

## Parallel startbar (sobald bereit)
- jetzt bereit (WP-00 erledigt): **WP-01**, **WP-07**, **WP-10**, **WP-05** (getrennte Ordner)
- 2026-10-27: **WP-03** (nfsmw als Code-Spender) ist bereit und läuft parallel zu WP-17 und den Fortsetzungen von WP-12 und WP-14. Es schreibt `symbols/nfsmw_map.csv` und die neue Spalte `name_mw` in `chunks.csv`. Solange eine andere Session an `chunks.csv` arbeitet, kommt die Spalte nach `symbols/pending/WP-03-chunks.csv`.
- nach WP-01 + WP-07: **WP-08**; danach **WP-02** und **WP-04**
- nach WP-10: **WP-12**, **WP-15**, **WP-16**
- Zwei Sessions dürfen nie gleichzeitig dieselbe Datei in `symbols/` schreiben. Bei paralleler Arbeit schreibt jede in `symbols/pending/<WP>-*.csv`, und die nächste Session ohne Konkurrenz führt zusammen.

## Nutzerentscheidungen
| ID | Frage | Stand |
|---|---|---|
| D1 | Original unter Wine/CrossOver starten | **ja** (2026-10-20) |
| D2 | Unicorn installieren (Funktions-Emulator) | **ja** (2026-10-20) |
| D3 | nfsmw-Repository klonen | **ja** (2026-10-20) |
| D4 | GameCube-Dump auf Symbole prüfen | **ja**: geprüft, `Speed.elf` mit vollen Symbolen und DWARF |
| D5 | SDL3 + SDL_GPU (Metal) | vorgeschlagen |
| D6 | C++17 | vorgeschlagen |
| D7 | verhaltensgleich statt byte-identisch | vorgeschlagen |
| D8 | `indep/`-Code aus GC-Dekompilat rekonstruieren, PC als Abgleich | **ja** (2026-10-20) |
| D9 | decomp-toolkit (dtk) herunterladen | **ja** (2026-10-20) |
| D10 | Zielplattformen | **macOS, Linux, Windows** (2026-10-28) |
