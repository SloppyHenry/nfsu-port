# tools/match – Abgleich PC ↔ GameCube (WP-08)

| Datei | Zweck |
|---|---|
| `features.py` | Merkmale je Funktion: PC aus `decompiled/full/out3`, GC aus `decompiled/gc/out` plus den PPC-Befehlen (`bl`/`b`, `lis`/`addi`-Strings und -Globals direkt aus `refs/gc/Speed.elf`, weil Ghidra bei Paired-Single abbricht). Cache: `refs/match_cache.pkl` |
| `match.py` | Anker (eindeutige Strings) → Ausbreitung (Aufrufgraph) → Ausrichtung in Linkreihenfolge (Needleman-Wunsch zwischen gesicherten Ankern) → Globals und Vtables. Schreibt `symbols/pc_gc_map.csv` |
| `apply_map.py` | übernimmt die Map in `symbols/functions.csv` (idempotent, CRLF) |
| `lits.py` | Zahlenliterale je Funktion (Jaccard mit IDF) als weiterer Beleg; Cache `refs/match_lits.pkl` |

```
python3 tools/match/features.py         # Cache neu bauen (nach Export-Änderung)
python3 tools/match/match.py            # voller Lauf, ca. 1 min
python3 tools/match/match.py --cv       # Kreuzvalidierung (4 Läufe, 40 % der String-Anker ohne Strings verborgen)
python3 tools/match/apply_map.py        # functions.csv aktualisieren
tools/reexport.sh                       # Namen nach Ghidra, neuer Export
```

`confidence` in `pc_gc_map.csv` (Kreuzvalidierung ohne Strings, 4 Läufe):
`sicher` (eindeutig gemeinsamer String oder Score ≥ 14, 98 %), `wahrscheinlich` (Score ≥ 4, 93 %), `moeglich` (84 %; nur als Notiz `gc?:` in `functions.csv`, keine Umbenennung).
`beleg`: `string`, `graph` (Aufrufgraph), `vtable`, `link` (Linkreihenfolge), `link-schwach` (Reihenfolge + Größe + Nachbarn).
