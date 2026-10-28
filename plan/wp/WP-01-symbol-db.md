# WP-01 Symbol-Datenbank, dauerhaftes Ghidra-Projekt, Werkzeuge

**Phase 0 · Modell: Opus oder Sonnet · Umfang: 1–2 Sessions · Voraussetzungen: WP-00**

## Problem
Bisher gingen alle Umbenennungen beim Schließen der Ghidra-Sitzung verloren (REA-Sitzung und `-deleteProject`). Ohne dauerhafte Namen fängt jede Session von vorn an.

## Ziel
`symbols/` ist die einzige Quelle der Wahrheit. Ein Skript spielt sie in ein **dauerhaftes** Ghidra-Projekt ein und exportiert danach neu. Jede Session kann eine Funktion für wenige hundert Token nachschlagen.

## Ergebnisse
1. **`symbols/functions.csv`**, eine Zeile je Funktion (10 378), Spalten:
   `addr,name,module,kind,status,confidence,source,size,convention,prototype,note`
   - `kind`: `game | engine | crt | d3dx | zlib | rcmp | sound | net | win32glue | thunk | unbekannt`
   - `status`: `unbearbeitet | benannt | typisiert | skizze | rekonstruiert | getestet | ersetzt | entfällt`
   - `confidence`: `sicher | wahrscheinlich | vermutet`
   - `source`: `notes` (alte Handarbeit) `| fid` (Ghidra) `| nfsmw | manuell | string`
2. **`symbols/data.csv`**: benannte Globals (`addr,name,type,size,module,note`), anfangs aus `decompiled/speed/src/game.h` und `NOTES.md`.
3. **Übernahme der Altnamen:** alle ≈350 Namen aus `decompiled/speed/NOTES.md` (Tabellen Ebene 1/2/3) und `decompiled/speed/inventar/ebene3_progress.txt` bzw. aus den Kommentaren in `decompiled/speed/src/*.c`. Status nach `NOTES.md` übernehmen (exakt → `rekonstruiert`, Skizze → `skizze`). Achtung: Das ist alter C-Code, kein Port-Code.
4. **Dauerhaftes Ghidra-Projekt** unter `decompiled/full/project_persist/` (Import von `game/pc/Speed.exe` einmalig **ohne** `-deleteProject`, mit denselben Analyseoptionen wie `ExportAll2.java`).
5. **Ghidra-Skripte** in `decompiled/full/ghidra_scripts/`:
   - `ApplySymbols.java`: liest `functions.csv`/`data.csv` und setzt Namen, Prototypen (sofern angegeben) und Kommentare
   - `ExportAll3.java`: wie `ExportAll2`, zusätzlich
     - `callgraph.tsv` (caller, callee, Aufrufadresse)
     - `strrefs.tsv` (Funktion, String-Adresse, String)
     - `datarefs.tsv` (Funktion, Global-Adresse, lesen/schreiben)
     - `consts.tsv` (Funktion, 32-Bit-Konstanten ≥ 0x10000, für Chunk-IDs und Hashes)
   - Lauf: `-process` auf das dauerhafte Projekt, `-preScript ApplySymbols.java -postScript ExportAll3.java out3`
   - `tools/reexport.sh`: genau dieser Lauf, mit JAVA_HOME
6. **`tools/fn.py`**:
   - `fn.py 0x4448b0` oder `fn.py game_main` zeigt den Pseudocode aus dem neuesten Export
   - `--callers` und `--callees` (mit Namen aus `functions.csv`), `--strings`
   - `--brief`: nur Signatur, Größe und Aufrufe (sehr billig)
   - Index über Byte-Offsets in `all_functions.c` cachen, damit ein Aufruf < 1 s dauert
7. **`tools/progress.py`**: Quote je `status`, `kind` und `module`, als Markdown-Tabelle.

## Schritte
1. CSV aus `out2/functions.tsv` erzeugen. Ghidra-FID-Namen (CRT) mit `source=fid`, `kind=crt` übernehmen.
2. Altnamen einmischen. Konflikte (gleiche Adresse, anderer Name) im Log auflisten.
3. Persistentes Projekt importieren (≈10 min, Hintergrund).
4. ApplySymbols + ExportAll3 schreiben. **Rundlauf beweisen:** Eine Funktion in der CSV umbenennen, reexportieren, der Name steht in `out3` und die Aufrufer zeigen ihn.
5. `fn.py` und `progress.py` schreiben und mit 3 Beispielen im Log zeigen.

## Fertig, wenn
- `functions.csv` hat 10 378 Zeilen, die Altnamen sind übernommen und die Konflikte dokumentiert.
- `tools/reexport.sh` läuft durch, und `out3/` enthält Export, callgraph, strrefs, datarefs und consts.
- `fn.py game_main --brief` liefert Ergebnis.
- `progress.py` gibt eine Tabelle aus, und die steht im Log.

## Hinweise
- `gen.py` hat Funktionsnamen auf `FUN_<adr>` zurückgesetzt (siehe `HANDOVER.md` §5). Das gilt nur für `build2`, nicht für die CSV.
- Namen mit `::` (C++) sind in der CSV erlaubt. Im Port werden daraus Klassen oder Methoden.
