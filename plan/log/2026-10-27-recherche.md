# Recherche externe Projekte (2026-10-27), kein Arbeitspaket

Frage des Nutzers: Gibt es online Erkenntnisse aus anderen Projekten, die die Arbeit verkürzen?

## Ergebnis
- **nfsmw-Decomp** (`refs/nfsmw`, schon geklont, CC0) ist weit mehr als ein Werkzeugvorbild:
  - 1 237 von 6 177 NFSU-`indep`-Funktionen haben dieselbe GNU-v2-Signatur wie in `config/GOWE69/symbols.txt`, 789 davon mit Größe ±10 %.
  - 1 427 haben eine Definition mit gleichem `Klasse::Methode` in `src/Speed/Indep` (966 `.cpp`, rund 191 000 Zeilen).
  - Am dichtesten: FEng (WP-37), TrackStreamer/World/eStreamPackLoader (WP-32), bWare/MemoryPool/SlotPool (WP-30), EAXSound (WP-39).
  - `symbols/bchunks.txt` benennt 68 der 92 namenlosen IDs in `chunks.csv`. Die 24 bereits benannten stimmen inhaltlich alle überein.
- **nfsug-Decomp** (NFSU GC, Prototyp `GNDP8P`): bisher kaum Code, derzeit ohne Nutzen.
- **OpenUG2** (UG2-Nachbau, macOS-fähig, MIT): Gegenprobe für Formate, keine Codequelle.
- Weitere Quellen (PryHUB, Binary, XtendedInput, Widescreen Fix, Prototypen) siehe `FAKTEN.md`, Abschnitt „Recherche externe Projekte“.

## Geändert
- `wp/WP-03-referenzen.md` neu geschrieben: nfsmw als Code-Spender. Ergebnis: `nfsmw_map.csv` mit Stufen, Stichprobe (≥ 30 Paare), Spalte `name_mw` in `chunks.csv`, Strukturvergleich, Anteil je Modul, Regeln für den Port.
- `wp/WP-12-jdlz-chunks.md`, `wp/WP-14-geometrie.md`: Hinweis zu den Chunk-Namen und den Einstiegen über `Loader…(bChunk *)` (57 in `gc_functions.csv`).
- `ROADMAP.md`: A2b neu bewertet, Abhängigkeitsbild, Aufwand Phase 3, D3 erweitert.
- `FAKTEN.md`: Tabelle „Recherche externe Projekte“. Die alte Aussage „kein NFSU-Decomp gefunden“ ist korrigiert.
- `STATUS.md`: WP-03 ist `bereit` und Voraussetzung für WP-30 (und damit für alle „wie 30“).

## Wie gezählt (nachvollziehbar)
- Signaturen: `mangled` aus `symbols/gc_functions.csv` (nur `quelldatei` mit „indep“) gegen die Namen mit `type:function` in `refs/nfsmw/config/GOWE69/symbols.txt`, Größe aus `size:`.
- Namen: Regex `\b(\w+)::(~?\w+)\s*\([^;{]*\)\s*(const)?\{` über `src/Speed/Indep/**/*.cpp` gegen (`klasse`, `methode`).
- Chunks: `bchunks.txt` (`Name = 0x…`) gegen `symbols/chunks.csv`, Spalte `name` leer.

## Offen
- Ob der nfsmw-Code **inhaltlich** zu NFSU passt, ist ungeprüft (Stichprobe in WP-03).
- Die Layouts weichen mindestens bei `eSolid` ab (WP-14). Die übrigen prüft WP-03, Schritt 6.
