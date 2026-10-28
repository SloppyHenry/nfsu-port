# WP-03 nfsmw als Code-Spender

**Phase 0 · Modell: Sonnet (Zuordnung), Opus für die Stichprobe · Umfang: 1–2 Sessions · Voraussetzungen: WP-01, WP-07, WP-08 (alle erledigt) · Entscheidung D3 erteilt · vor WP-30, WP-32, WP-37, WP-39 und der Fortsetzung von WP-12/WP-14**

> **Neuer Zuschnitt (2026-10-27, Recherche, siehe `plan/log/2026-10-27-recherche.md`):** Bisher galt nfsmw nur als Werkzeugvorbild. Inzwischen enthält die Decomp so viel lesbaren C++-Code für Engine-Teile, die NFSU mit MW teilt, dass sie für Phase 3 eine **zweite Vorlage neben dem GC-Dekompilat** ist. Namen und Typen kommen weiter aus der eigenen GC-ELF (WP-07). Die Ziele von früher (150 Zuordnungen per String-Anker, Werkzeugvorbild dtk) entfallen.

## Hintergrund (geprüft 2026-10-27)
- `refs/nfsmw` = `dbalatoni13/nfsmw`, Commit `80993f3` (10.10.2026), Lizenz **CC0 1.0**. Ziel des Projekts: Decomp von MW 2005 (GC `GOWE69`, dazu X360, PS2, PC 1.3), byte-identisch mit ProDG GC 3.9.3 (SN/GCC). Laut README nutzt es LLMs für erste Entwürfe, jeder Code wird von Hand geprüft. **Nicht jede Funktion dort ist schon „matching“.**
- `src/Speed/Indep/**/*.cpp`: 966 Dateien, rund 191 000 Zeilen.
- **Gleiche GNU-v2-Signatur** (mangled Name aus `symbols/gc_functions.csv` gegen `config/GOWE69/symbols.txt`): **1 237 von 6 177** `indep`-Funktionen von NFSU, davon **789** mit einer Größe innerhalb von ±10 %.
- **Gleicher Name `Klasse::Methode`**, in der nfsmw-Quelle definiert: 1 427. Am dichtesten bei FEngine 39/42, FEPackageReader 23/23, FEPackage 21/21, FECodeListBox 24/24, FEPackageManager 18/20, cFEngRender 15/16, TrackStreamer 24/32, eStreamPackLoader 23/24, World 24/30, SpaceNode 13/13, QuickSpline 19/19, MemoryPool 14/14, SlotPool 11/11, EAXSound 24/55, EAXFrontEnd 14/15 sowie bWare (160 von 173 Funktionen aus `../indep/bWare`).
- `symbols/bchunks.txt`: Chunk-ID → Name. 68 der 92 IDs in `symbols/chunks.csv`, die noch keinen Namen haben, sind dort benannt. Die 24 IDs, die WP-13 und WP-14 selbst benannt haben, stimmen inhaltlich alle mit nfsmw überein (z. B. 0x134b01 „Vertexdaten“ = `MeshVertexBuffer`).
- **Grenze:** Dateilayouts können sich zwischen 2003 und 2005 unterscheiden. WP-14 hat das für `eSolid` gezeigt: Namen stimmen, Byte-Layout nicht. Layouts kommen deshalb immer aus `gc_types.h` bzw. `types.h` und der Prüfung an echten Dateien, nie aus nfsmw-Headern.

## Ziel
Jede Phase-3-Session weiß je Funktion, ob es in nfsmw eine lesbare Vorlage gibt, wie gut sie passt und wo sie liegt.

## Schritte
1. **Klon aktualisieren** (`git -C refs/nfsmw pull`, durch D3 gedeckt). Commit-Hash und Datum im Log festhalten.
2. **Index der nfsmw-Definitionen**, `tools/nfsmw_index.py`: aus `src/Speed/Indep/**` alle Definitionen mit Klasse, Methode, Datei und Zeilenbereich; aus `config/GOWE69/symbols.txt` mangled Name und Größe. Nur lesen, Ergebnis lokal (`refs/nfsmw_index.tsv`).
3. **Zuordnung** `symbols/nfsmw_map.csv`: `gc_addr,gc_name,mw_mangled,mw_file,mw_zeilen,ug_size,mw_size,stufe,confidence`
   - `stufe=signatur`: gleicher mangled Name. Bei Größe ±10 % ist das `wahrscheinlich`, sonst `moeglich`.
   - `stufe=name`: gleiches `Klasse::Methode`, andere Signatur → `moeglich`
   - Sprachbezogene Funktionen (`FUN_…`, Thunks, Initialisierer) nicht zuordnen.
   - `functions.csv` bekommt keine neuen Namen, weil die Namen schon aus der GC-ELF kommen. Erlaubt ist höchstens eine Spalte `vorlage` (`nfsmw` bzw. leer), die `progress.py` mitzählt.
4. **Stichprobe** (Opus): 30 Paare über alle Stufen und Module verteilt. GC-Dekompilat (`tools/fn.py --gc`) gegen die nfsmw-Quelle lesen und jeweils urteilen: „gleich“, „gleich bis auf Konstanten/Felder“ oder „anders“. Die Trefferquote je Stufe kommt ins Log und nach `FAKTEN.md`. Daraus folgt, ab welcher Stufe eine Vorlage in Phase 3 als Ausgangspunkt taugt.
5. **Chunk-Namen** in `symbols/chunks.csv` als neue Spalte `name_mw` eintragen, die bestehende Spalte `name` bleibt unverändert. Danach `chunks.csv` nach Familien sortiert ins Log, vor allem 0x34xxx (Scenery, VisibleSection, TrackPath, Topologie) als Hinweis für die Fortsetzung von WP-14 und für WP-32.
6. **Kernstrukturen**: für bNode/bList, bChunk, ResourceFile, TextureInfo, eSolid, eModel, bVector, bMatrix4, Camera, eView, FEObject, FEPackage, TrackStreamer den Vergleich `gc_types.h` ↔ nfsmw-Header als `gleich`, `Felder verschoben` oder `anders`. Damit wird sichtbar, wo der nfsmw-Code ohne Anpassung an die Felder nicht passt.
7. **Je Modul** aus `symbols/pakete.csv` bzw. `plan/MODULE.md` zählen, welcher Anteil der Funktionen eine Vorlage der Stufe `wahrscheinlich` hat. Das Ergebnis als Spalte in `pakete.csv`, die Aufwandsschätzung in `ROADMAP.md` neu rechnen.
8. **Arbeitsweise in `wp/WP-3x-VORLAGE.md`** nachtragen (siehe „Regeln für den Port“ unten): nfsmw als Quelle Nr. 4 je Funktion.

## Fertig, wenn
- `nfsmw_map.csv` deckt alle Signatur- und Namenstreffer ab, jede Zeile mit Stufe und Confidence.
- Die Stichprobe (≥ 30 Paare) ist im Log ausgewertet, die Trefferquote je Stufe steht in `FAKTEN.md`.
- `chunks.csv` hat die Spalte `name_mw`.
- Für die Kernstrukturen aus Schritt 6 ist ein Urteil eingetragen.
- `pakete.csv` bzw. `MODULE.md` zeigen je Modul den Anteil mit Vorlage, und die ROADMAP-Schätzung für Phase 3 ist aktualisiert.

## Regeln für den Port (gelten ab diesem Paket für WP-3x)
- Die Lizenz CC0 erlaubt, Code aus nfsmw zu übernehmen. Übernommener Code bekommt neben `// orig: 0x<pc_adresse> <name>` den Kommentar `// vorlage: nfsmw <datei>:<zeile>`.
- **Maßgeblich bleibt NFSU:** Die Logik prüfen wir gegen das GC-Dekompilat. Bei PC-Abweichungen gilt die PC-Fassung, und mit WP-05 vergleichen wir im Emulator. Feldoffsets kommen nur aus `gc_types.h`/`types.h`. Was MW 2005 neu hat (Attrib/VLT, Polizei, Heat, World-Physik-Umbau), gibt es in NFSU nicht und wird nicht übernommen.
- ADR-001 gilt weiter: Dateistrukturen behalten `ptr32<T>` und `static_assert`, auch wenn nfsmw native 32-Bit-Zeiger benutzt.

## Nicht tun
- Den Port nicht nach nfsmw-Layouts bauen, ohne sie an NFSU-Dateien oder am NFSU-Dekompilat zu prüfen (Beispiel eSolid, WP-14).
- nfsmw nicht bauen (keine ProDG-Toolchain, kein wibo). Wir nutzen es nur als Lesereferenz.

## Weitere externe Quellen (Recherche 2026-10-27, keine Arbeit in diesem Paket)
- `dbalatoni13/nfsug`: Decomp von NFSU **GameCube**, Ziel ist der Prototyp `GNDP8P` (10.09.2003), nicht unser `GNDD69`. CC0, letzter Commit 04.02.2026, `src/` ist fast nur Dolphin-SDK und ein paar Ecstasy-Dateien. Lohnt sich derzeit nicht. Bei Phase 3 erneut prüfen, ob das Projekt gewachsen ist.
- `whoismept/OpenUG2`: Clean-Room-Nachbau der UG2-Engine (C, SDL2/OpenGL, macOS arm64, MIT). Liest STREAM*.BUN, GEOMETRY.BIN, TPK und Paths. Taugt als Gegenprobe für Formate (WP-14 drittes Vertexformat, WP-18), nicht als Codequelle.
- `bdrtr/PryHUB` (UG2-Chunk-Browser, Rust), `NFSTools/Binary` / `SpeedReflect/Binary` (BIN/BUN/LZC-Editoren): Formathinweise zu UG2.
