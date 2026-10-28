# WP-3x Vorlage: ein Engine- oder Spielmodul rekonstruieren

**Phase 3 · Modell: Opus für schwierige Module, sonst Sonnet · Umfang: 100–150 Funktionen je Session (mit GC-Symbolen) · Voraussetzungen: WP-01, WP-02, WP-04, WP-05, WP-07, WP-08, WP-10, WP-11**

**Quellen je Funktion** (Entscheidung D8):
1. **GC-Dekompilat** (`tools/fn.py --gc <name>`): mit Namen, Typen und lokalen Variablen aus DWARF. Das ist die primäre Vorlage für `indep/`-Code.
2. **PC-Dekompilat** (`tools/fn.py <pc_addr>`, Zuordnung aus `pc_gc_map.csv`): zum Abgleich. Bei Abweichungen gilt die PC-Fassung, weil mit PC-Daten gespielt wird. Abweichungen im Code kommentieren (`// PC weicht ab: …`).
3. **DWARF-Dump** (`refs/gc/src/<pfad>`): Deklarationen, Parameter- und Variablennamen, Zeilenbezug.
4. **nfsmw-Quelle** (WP-03, CC0): `symbols/nfsmw_map.csv` zeigt je GC-Funktion Datei und Zeilen unter `refs/nfsmw/`. Lesbarer C++-Entwurf, kein Maßstab für Layouts. Zuverlässigkeit (Stichprobe WP-03):
   - `signatur/wahrscheinlich` (gleicher mangled Name, Größe ±10 %): **Ausgangspunkt**. 8 von 8 beurteilbaren Stichproben waren gleich oder gleich bis auf Felder/Konstanten. Trotzdem gegen das GC-Dekompilat prüfen.
   - `signatur/moeglich`: Größe weicht ab. Bis etwa 20 % Abweichung meist brauchbar, darüber (MW 2005 hat mehr Logik) fast immer anders (3 von 8 insgesamt). Nur zum Mitlesen.
   - `name/moeglich`: nur mit Größe ±10 % Ausgangspunkt (3 von 3), sonst anders (0 von 7). Oft hat MW einen anderen Parametersatz.
   - `mw_zeilen` mit gleicher Start- und Endzeile (179 Fälle) sind leere Stubs, keine Vorlage.
   - Übernommener Code bekommt `// vorlage: nfsmw <datei>:<zeile>` neben `// orig: …`. Feldnamen und Offsets kommen aus `gc_types.h`/`types.h`, nie aus MW-Headern (Strukturvergleich in `plan/FAKTEN.md`).
   - Was MW 2005 neu hat (Attrib/VLT, Polizei, Heat, Physik-Umbau), nicht übernehmen.

**Port-Struktur:** Der Originalquellbaum wird gespiegelt: `port/src/indep/src/World/Player.cpp` usw. Klassen- und Methodennamen kommen aus der ELF.

Die konkreten Module (WP-30, WP-31 …) legt WP-02 in `plan/MODULE.md` fest, auf Basis von `plan/QUELLBAUM.md`. Jede Session nimmt ein Modul bzw. einen Abschnitt davon. Die Funktionsliste ergibt sich aus `symbols/functions.csv` mit `module=<name>` und `status` vor `rekonstruiert`.

## Vermutete Reihenfolge (WP-02 bestätigt oder ändert sie)
1. **Basis** (bWare-Äquivalent): Speicher und Pools, Listen und Knoten, Strings und Hashes, Mathe (Vektor, Matrix, Quaternion), Zufall, Zeit
2. **Dateien und Ressourcen**: Dateisystem und Ladewarteschlange (`FILESYS_*`), Chunk-Loader-Registrierung, Ressourcenpakete, Sprachtabellen
3. **Ereignisse, Nachrichten, Zustandsmaschine** (`g_state_fn`, Hauptschleife, `decompiled/speed`)
4. **Streaming der Strecke** (Abschnitte, Sichtbarkeit)
5. **Welt und Rendering** (Szene, Sichtbarkeit, Solids, Licht, Effekte, Partikel) → spricht nur noch die Render-Schnittstelle aus WP-40 an
6. **Kamera**
7. **Fahrzeug**: Modell, Tuning und Teile, Physik, Kollision, Schaden bzw. Optik
8. **KI und Verkehr**, Pfade
9. **Rennlogik**: Modi (Circuit, Sprint, Drag, Drift, Knockout), Regeln, Zeitnahme, Ergebnis
10. **Frontend/UI** (FEng-Paket, Menüs, Garage, Karriere, Shop) und HUD
11. **Audio-Spiellogik** (Motor-Synthese, Ereignisse, Musik, Sprache) → über die Audio-Schnittstelle aus WP-42
12. **Karriere, Fortschritt, Speicherstand, Optionen**

## Ablauf je Session
1. `python3 tools/progress.py --module <m>` und Funktionsliste holen. Mit den **Blättern** des Aufrufgraphs anfangen (Funktionen, deren Aufrufziele schon rekonstruiert, ersetzt oder entfallen sind).
2. Je Funktion: `tools/fn.py <adr>` (Pseudocode), `--callers`/`--callees`, Typen aus `types.h`, Vorlage aus `nfsmw_map.csv` (Quelle 4).
3. Sauberes C++ nach `port/src/game/<modul>/` schreiben:
   - echte Typen, Klassen mit Methoden statt `thiscall`-Funktionen, keine Originaladressen, keine `in_E?X`
   - Registerargumente aus den Aufrufern ableiten (Assembler über MCP `procedure_assembly` nur bei Bedarf)
   - x87-Gleitkomma: auf Rechenreihenfolge achten. Bei physikrelevantem Code das Ergebnis mit dem Emulator vergleichen.
4. **Testen:**
   - Reine Funktionen gegen das Original im Emulator (WP-05) mit `tools/emu/compare.py` → `status=getestet`
   - Zustandsreiche Funktionen mindestens über einen Unit-Test mit echten Daten → `status=rekonstruiert`
5. Namen, Typen und Status in `symbols/` nachziehen, `tools/reexport.sh` laufen lassen, wenn sich viele Namen geändert haben (damit spätere Sessions besseres Pseudocode sehen).
6. Baut alles ohne Warnungen? Laufen die Tests? Dann Log schreiben und committen.

## Fertig (je Modul), wenn
- Alle Funktionen des Moduls haben `status` ≥ `rekonstruiert` oder `ersetzt`/`entfällt` mit Begründung.
- Mindestens 50 % der reinen Funktionen sind `getestet`.
- Das Modul ist in `port/` ohne Warnungen gebaut, und seine Unit-Tests laufen.

## Nicht tun
- Kein „Skizze“ als „fertig“ melden. Leere Körper oder `TODO` bleiben `skizze`.
- Den Plattformcode (D3D9, DSOUND, DINPUT, Win32) nicht nachbauen. An diesen Stellen ruft der Spielcode die Schnittstellen aus Phase 4 auf.
