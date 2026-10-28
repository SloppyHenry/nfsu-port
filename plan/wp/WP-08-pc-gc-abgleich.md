# WP-08 Abgleich PC ↔ GameCube (Namen auf die PC-Exe übertragen)

**Phase 0 · Modell: Opus · Umfang: 2–3 Sessions · Voraussetzungen: WP-01, WP-07**

## Ziel
Möglichst jede Spielfunktion der PC-`Speed.exe` ist ihrem GameCube-Gegenstück zugeordnet und übernimmt dessen Namen, Klasse, Quelldatei und Prototyp. Danach ist das PC-Dekompilat „symbolisiert“.

## Belege (nach Stärke)
1. **Gleiche Strings**, die nur in einer Funktion vorkommen (Debug-, Fehler-, Datei- und Chunk-Namen)
2. **Gleiche Konstanten** (Chunk-IDs, Hashes, ungewöhnliche Float-Literale, Tabellenadressen-Muster)
3. **Aufrufgraph:** Ist A↔A' gesichert, werden die Aufrufziele in gleicher Reihenfolge abgeglichen (Ausbreitung von gesicherten Paaren).
4. **Linkreihenfolge:** Beide Compiler legen die Objektdateien (Quelldateien) zusammenhängend ab. Innerhalb einer Datei bleibt die Funktionsreihenfolge oft gleich. Mit gesicherten Ankern lassen sich so ganze Dateien zuordnen.
5. **Form:** Parameteranzahl, Vtable-Slot-Nummer, Größe im Verhältnis (x86 zu PPC ungefähr 0,6–1,0, nur schwacher Beleg)

## Schritte
1. `tools/match/` schreiben: Merkmale je Funktion aus `out3` (PC) und `decompiled/gc/out` (GC). Dazu gehören Strings, Konstanten, Aufrufe, Größe und Vtable-Bezüge.
2. Anker aus 1 und 2 bilden, dann iterativ nach 3 und 4 ausbreiten. Jede Zuordnung bekommt `beleg` und `confidence`.
3. Ergebnis `symbols/pc_gc_map.csv`: `pc_addr,gc_addr,gc_name,quelldatei,beleg,confidence`. Übernahme in `functions.csv` (`source=gc`, `module=<quelldatei>`) ab `wahrscheinlich`.
4. Inlining beachten: Eine GC-Funktion kann auf PC eingebettet sein und umgekehrt. Solche Fälle markieren, nicht erzwingen.
5. PC-only-Funktionen (Renderer D3D9, DirectInput, PC-Optionen, Online) als `kind=win32glue` bzw. `pc_only` kennzeichnen.
6. Reexport (`tools/reexport.sh`) und Stichprobe: 20 zufällige Zuordnungen per Hand prüfen.

## Fertig, wenn
- ≥ 45 % der PC-Funktionen in den Spielbereichen (ohne CRT, D3DX, zlib, Bibliotheken) sind mindestens `wahrscheinlich` zugeordnet, und ≥ 70 % in beliebiger Stufe (Ziel am 2026-10-28 angepasst; Stand: 42,7 % ≥ wahrscheinlich, also 2,3 Punkte darunter, 66,5 % aller Stufen, also 3,5 Punkte darunter; vom Nutzer trotzdem als erledigt akzeptiert, Begründung in `plan/log/2026-10-27-WP-08b.md`).
- Die Stichprobe hat ≤ 1 Fehler, und die Quote je Quelldatei steht im Log.
