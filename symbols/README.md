# Symbol-Datenbank (Quelle der Wahrheit für Namen, Typen, Module). Aufbau: plan/wp/WP-01-symbol-db.md

## pc_gc_map.csv (WP-08)
`pc_addr,gc_addr,gc_name,quelldatei,beleg,confidence,score,gc_demangled` — erzeugt von `tools/match/match.py`, übernommen von `tools/match/apply_map.py` (`source=gc` in `functions.csv`, Notiz `gc 0x… <beleg> <score>`; `moeglich` nur als `gc?:`-Notiz). Zusätzlich `kind=win32glue` mit `confidence=vermutet` für nicht zugeordnete Funktionen mit Windows-Importaufrufen.

## modules.csv, module_belege.csv, pakete.csv (WP-02)
`modules.csv`: `module,start,end,kind,anzahl,bytes,folgen,im_hauptbereich,gc_link_index,behandlung,ersatz,belege` (start/end = längste zusammenhängende Folge). `module_belege.csv`: Beleg je Funktion (`gc-anker`, `segment`, `grenze-aufruf`, `grenze-mitte`, `regel`, `rand`, `zone`, `fest`, `thunk`). `pakete.csv`: Phase-3-Pakete (WP-3x.y) mit Modul und Adressbereich. Erzeugt von `tools/classify/`.

## WP-04 (Typen, Vtables, COM)
- `vtables.csv` (`tools/types/vtables.py`): `vtable_addr,klasse,slot,ziel,ziel_name,gc_ziel,vptr_offset,beleg`. `klasse=?` = unbekannt, `X?` = nur aus Slotnamen vermutet, `X/Basis` = Zweit-Vtable. `ziel_name=neu:FUN_…` = Slotziel, das Ghidra nicht als Funktion kennt (nur über Vtable erreichbar). `beleg`: `ctor` (Konstruktor schreibt den Vptr) / `ref`, dahinter `gc-vtable(n)`, `ctor-name(n)`, `dtor-aufruf`, `slot-namen(n)`.
- `types.h` (`tools/types/gen_types.py`, aus `gc_types.h`): PC-Layout MSVC (bool 1 Byte, Vptr am Anfang, Basis als Feld `base`). Nicht von Hand bearbeiten.
- `com_vtables.csv`, `com_types.h`, `com_calls.csv`, `com_globals.csv` (`tools/types/com.py`, Definitionen in `com_defs.py`).
- Einspielen: `ApplyTypes.java` (läuft in `tools/reexport.sh` nach `ApplySymbols.java`).

## WP-30.1: Funktionen, die der Ghidra-Export nicht kennt
Der Bereich 0x553c20–0x5541a0 (Joystick-Scanner) steht nicht in `functions.csv`; Ghidra hat dort keine Funktionen angelegt. Aus dem Maschinencode (Emulator, capstone) rekonstruiert:
`0x553c20 ReadButtonPair`, `0x553ce0 Scan_TypeChanged`, `0x553d20 Scan_ButtonPress`, `0x553d80 Scan_ButtonChange`, `0x553dd0 Scan_TwoButtonAxis`, `0x553e60 Scan_AnalogRepeat`, `0x554080 Scan_AnalogGraph`. Der Port hat sie in `port/src/game/misc/joystick.cpp`; die Tabellen daraus erzeugt `tools/gen_joystick_tables.py`.
