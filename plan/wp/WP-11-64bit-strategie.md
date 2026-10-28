# WP-11 64-Bit-Strategie für geladene Daten (Architekturentscheidung)

**Phase 2 · Modell: Opus · Umfang: 1–2 Sessions · Voraussetzungen: WP-01, WP-10 (ideal WP-12 begonnen)**

## Problem
Das Original ist 32-Bit-Code. EA-Engines dieser Zeit laden Chunks oft direkt in den Speicher und schreiben darin enthaltene Offsets in **32-Bit-Zeiger** um („in place fixup“). Strukturen wie TextureInfo oder Solid-Header haben dann Zeigerfelder mit 4 Byte. Unter arm64 sind Zeiger 8 Byte groß, und das Layout passt nicht mehr. Davon hängt die Architektur aller Lade- und Laufzeitstrukturen ab.

## Schritte
1. **Befund:**
   - In `out2`/`out3` die Ladefunktionen der Chunks finden (Konstanten aus `consts.tsv`, Aufrufe aus dem JDLZ- und Chunk-Code).
   - Für jeden Chunk-Typ festhalten: Enthält er Zeiger oder Offsets? Werden sie beim Laden umgeschrieben? Bleibt der Puffer danach als Struktur in Gebrauch?
   - Ergebnis: Spalte `zeiger` in `symbols/chunks.csv` (WP-12).
2. **Optionen bewerten**, jede mit einem kleinen Prototyp:
   - (a) **Deserialisieren** in native Strukturen: sicher, aber mehr Code, und es weicht vom Originalablauf ab.
   - (b) **`ptr32<T>`**: ein 32-Bit-Offset relativ zu einer Arena- oder Chunkbasis mit `operator->`. Das Layout bleibt originalgleich (`static_assert(sizeof)`), der Code bleibt nah am Original.
   - (c) **Speicher unterhalb von 4 GB** (`-pagezero_size`, `mmap` mit Hinweisadresse): auf macOS arm64 prüfen, ob das überhaupt geht. Vermutlich nicht verlässlich.
3. Empfehlung und Regeln als `port/docs/ADR-001-64bit.md`, zum Beispiel:
   - Dateigebundene Strukturen: `ptr32<T>` bzw. `off32<T>`, `#pragma pack` wie im Original, `static_assert` auf Größe und Offsets.
   - Reine Laufzeitobjekte: native Zeiger, Layout frei.
   - Wie Listen und Knoten (`bNode`) behandelt werden, die beides sind.
4. Die Regeln kurz in `CLAUDE.md` (Abschnitt „Harte Regeln“) nachtragen.

## Fertig, wenn
- Das ADR ist geschrieben, ein Prototyp mit einem echten Chunk-Typ besteht seinen Test, und `CLAUDE.md` ist ergänzt.
