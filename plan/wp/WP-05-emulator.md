# WP-05 Emulations-Harness: einzelne Originalfunktionen ausführen und vergleichen

**Phase 1 · Modell: Opus · Umfang: 1–2 Sessions · Voraussetzungen: WP-00 · Entscheidung: D2 (Unicorn installieren)**

## Ziel
Eine Prüfmöglichkeit **ohne Windows**: Eine beliebige Originalfunktion aus `Speed.exe` wird in einer x86-32-Sandbox (Unicorn) mit eigenen Eingaben aufgerufen. Das Ergebnis wird mit der Port-Funktion verglichen. Das ist der Standardtest für alle reinen Funktionen (Mathe, Hashes, Dekompression, Parser, Physikschritte).

## Aufbau (`tools/emu/`)
1. **Loader:** PE-Abschnitte von `game/pc/Speed.exe` an die Originaladressen mappen (`.text`, `.rdata`, `.data`, BSS bis 0x785d98). Mit Python lesen, nicht über Windows laden.
2. **Umgebung:** Stack, ein minimaler TEB/TIB für `FS:[0]`/`FS:[0x18]`, x87/SSE-Grundzustand.
3. **Importe:** Jede Importadresse zeigt auf einen Stub, der **laut abbricht** und den Namen meldet. Einzelne Stubs (z. B. `HeapAlloc` → eigener Heap im Emulator) dürfen gezielt nachgebildet werden. Kein Durchreichen an das Host-System.
4. **Aufruf-API:** `call(addr, conv, args, this=None, regs={})` mit Unterstützung für cdecl, stdcall, fastcall, thiscall und Registerparameter. Rückgabe EAX/EDX und ST0 (Float-Rückgaben über x87).
5. **Speicherhelfer:** Strukturen und Puffer im Emulator anlegen, lesen und vergleichen. Speicher-Snapshot vor und nach dem Aufruf mit Diff („welche Globals hat die Funktion geändert?“).
6. **Brücke zum Port:** Der Port baut eine `libnfsu_core.dylib` mit `extern "C"`-Testeinstiegen. `tools/emu/compare.py` ruft beide mit Zufalls- und Grenzwerten auf (Property-Test) und meldet die erste Abweichung.

## Erste Prüflinge
- `reset_identity_matrix` (0x40a7c0, aus `decompiled/speed`)
- die String-Hash-Funktion (über WP-01/03 finden, MW: `bStringHash`)
- der JDLZ-Dekompressor (WP-12, Funktion über die Konstante `'JDLZ'` = 0x5A4C444A in `consts.tsv` finden)

## Fertig, wenn
- Drei Funktionen sind über ≥ 10 000 Zufallseingaben bitgleich zum Port bzw. zu einer Python-Referenz.
- `tools/emu/README.md` erklärt die Nutzung in ≤ 30 Zeilen, damit spätere Sessions den Harness ohne Einarbeitung nutzen.

## Hinweis GameCube
Unicorn kann auch PowerPC 32 Big-Endian. Getestet wird aber gegen die **PC-Exe**, weil sie die Verhaltensreferenz ist. Ein PPC-Modus für GC-Funktionen ist optional und nur sinnvoll, wenn eine Funktion auf PC eingebettet und deshalb nicht einzeln aufrufbar ist. Daten liegen dann Big-Endian vor.

## Sicherheit
Nur ausgewählte Funktionen laufen im Emulator, ohne Zugriff auf Host, Dateien oder Netz. Das Spiel wird damit **nicht** gestartet.
