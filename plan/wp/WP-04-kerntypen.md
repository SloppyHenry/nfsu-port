# WP-04 Kerntypen, Klassen und Vtables

**Phase 0 · Modell: Opus · Umfang: 1–2 Sessions · Voraussetzungen: WP-01, WP-07, WP-08**

> **Seit dem GC-Fund:** Die Klassen, Strukturen und Vtable-Layouts stehen in `symbols/gc_types.h` (aus DWARF, WP-07). Die Aufgabe ist jetzt, sie **auf die PC-Exe zu übertragen und zu prüfen**, nicht mehr, sie herzuleiten. Beide Builds sind 32-Bit, also sind die Layouts meist gleich. Abweichungen gibt es bei plattformabhängigen Feldern, `#ifdef`-Teilen und Ausrichtung. Schritt 1 (Vtables) wird zum Abgleich: GC-Vtable-Layout ↔ PC-Vtable in `.rdata`. Schritt 3 wird zu: `types.h` = `gc_types.h` plus PC-Korrekturen, jede Korrektur mit dem Code-Beleg. Schritt 2 (DirectX-COM-Tabellen) bleibt unverändert, denn das ist PC-only.

## Ziel
Die zentralen Datenstrukturen sind als echte Typen definiert und in Ghidra eingespielt. Das Dekompilat zeigt dann Feldnamen statt `*(int *)(param_1 + 0x24)`. Das ist der größte Hebel für die Lesbarkeit in Phase 3.

## Schritte
1. **Vtables finden** (es gibt kein RTTI):
   - in `.rdata` nach Folgen von ≥ 2 Zeigern auf Funktionsanfänge suchen (`functions.tsv`)
   - Konstruktoren daran erkennen, dass sie eine solche Adresse nach `[ecx]` bzw. `[this]` schreiben
   - Ergebnis `symbols/vtables.csv`: `vtable_addr,klasse,slot,ziel,ziel_name`
2. **DirectX-Schnittstellen**: Tabelle der Methoden-Offsets für `IDirect3D9`, `IDirect3DDevice9`, `IDirect3DTexture9`, Vertex- und Indexbuffer, `IDirectSound(Buffer)`, `IDirectInput8`/`IDirectInputDevice8` nach `symbols/com_vtables.csv`. Dann im Export die Aufrufe `(**(code **)(*p + 0x...))` den Methodennamen zuordnen. Das ist die Grundlage für Phase 4.
3. **`symbols/types.h`**: C-Strukturen der Kernklassen (Felder mit Offset-Kommentar, `static_assert`-taugliche Größen). Start mit den Typen aus WP-03 und den Offsets aus `decompiled/speed/src/game.h`.
4. **`ApplyTypes.java`**: `types.h` mit Ghidras C-Parser einlesen, Typen auf Funktionsparameter (`this`) und Globals anwenden (Spalte `type` in `functions.csv`/`data.csv`), dann reexportieren.
5. Erfolg messen: Anzahl der Ausdrücke `+ 0x` mit Zeigercast im Export vor und nach dem Lauf.

## Fertig, wenn
- `vtables.csv` deckt alle gefundenen Vtables ab (Zahl im Log).
- `com_vtables.csv` ist für D3D9, DSOUND und DINPUT8 vollständig.
- Mindestens 25 Kerntypen sind in `types.h` definiert und angewendet, und der Reexport zeigt Feldnamen.
- Ab jetzt gilt für Phase 3: Neue Typen werden in `types.h` ergänzt und eingespielt.
