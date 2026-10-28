# WP-07 GameCube-ELF auswerten: Namen, Typen, Quellbaum

**Phase 0 · Modell: Opus · Umfang: 1–2 Sessions · Voraussetzungen: WP-00 · Entscheidung D9 (dtk herunterladen) erteilt**

## Befund (2026-10-20)
Das ISO `game/gc/Need for Speed - Underground (Germany).iso` (Spiel-ID `GNDD69`) enthält neben `Speed.dol` eine **nicht gestrippte `Speed.elf`**:
- Offset 936 535 612, 8 252 556 Byte, PowerPC Big-Endian, GCC 2.95.3 (SN Systems) — anfangs fälschlich als CodeWarrior notiert
- `.symtab` mit 17 295 Symbolen, darunter **10 650 Funktionen** mit GNU-v2-gemangelten C++-Namen
- `.debug` mit 4,2 MB **DWARF 1** (Typen, Strukturlayouts, Parameter, lokale Variablen), dazu `.line`, `.debug_srcinfo`, `.debug_sfnames`, `.debug_pubnames`
- Quellpfade im DOL, z. B. `../indep/src/World/Player.cpp`, `../indep/bWare/Inc/bWare.hpp`, `src/ecstasy/EcstasyE.cpp`, `src/file/gc/filedev.cpp`

`indep/` ist der **plattformunabhängige Code**, den GameCube und PC gemeinsam nutzen. Die Plattformteile (`gc`, `ecstasy`-Renderer, Speicherkarte) unterscheiden sich.

## Ziel
Alles, was die ELF über das Spiel verrät, liegt maschinenlesbar in `symbols/` und `refs/gc/`. Zusätzlich gibt es ein Ghidra-Projekt für den GameCube-Code mit Namen und Typen und einen Headless-Export davon.

## Schritte
1. **Entpacken:** `Speed.elf` (und `Speed.dol`) über die FST des ISO nach `refs/gc/` kopieren. Nur lesen, mit Python wie in `plan/log/2026-10-20-planung.md`. Prüfen, dass die ELF zum DOL passt (Abschnittsadressen und -größen vergleichen).
2. **decomp-toolkit (dtk)** (D9 erteilt): feste Release-Version aus `encounter/decomp-toolkit` (GitHub) nach `refs/tools/` laden, Version und Prüfsumme ins Log. Damit:
   - `dtk dwarf dump refs/gc/Speed.elf -o refs/gc/dwarf_dump.cpp` erzeugen
   - nach Quelldatei aufteilen (Vorbild: `split_dwarf_info.py` aus nfsmw, WP-03) → `refs/gc/src/<pfad>.cpp|hpp`
3. **Symbole:** `symbols/gc_functions.csv` mit `gc_addr,size,mangled,demangled,klasse,methode,quelldatei,zeilen`. Demangling GNU v2: eigenes Skript `tools/gcc2_demangle.py` (dtk kann nur CodeWarrior).
4. **Typen:** `symbols/gc_types.h` mit allen Strukturen, Klassen (inkl. Vererbung und Vtable-Lage), Enums und Typedefs aus dem DWARF-Dump. Zählen: Klassen, Strukturen, Enums.
5. **Quellbaum:** `plan/QUELLBAUM.md`: alle Quelldateien des Spiels mit Funktionsanzahl, gruppiert nach `indep/bWare`, `indep/src/<Bereich>` und den Plattformordnern. **Das ist die Grundlage für die Phase-3-Module.**
6. **Ghidra-GC-Projekt:** `decompiled/gc/project_persist/`
   - ELF importieren (Sprache `PowerPC:BE:32:Gekko_Broadway`, sonst `PowerPC:BE:32:default`)
   - Namen aus `.symtab` kommen automatisch
   - Ghidra liest DWARF 1 vermutlich nicht. Dann `gc_types.h` per C-Parser einspielen und Prototypen aus dem Dump per Skript setzen (`ApplyGcTypes.java`).
   - Headless-Export nach `decompiled/gc/out/` im gleichen Format wie `out2` (mit `ExportAll3.java` aus WP-01)
   - `tools/fn.py` um `--gc` erweitern
7. Stichprobe: 10 Funktionen aus verschiedenen Bereichen ansehen und die Qualität des GC-Pseudocodes gegen das PC-Gegenstück bewerten (Log).

## Fertig, wenn
- `gc_functions.csv` hat ≈ 10 650 Funktionen, ≥ 95 % davon mit Quelldatei.
- `gc_types.h` lässt sich mit `clang -fsyntax-only` als C++ übersetzen (notfalls mit Vorwärtsdeklarationen).
- `QUELLBAUM.md` existiert, und der Ghidra-GC-Export liegt vor und ist über `fn.py --gc` abfragbar.

## Hinweis
Die ELF, die DWARF-Dumps und `refs/` sind Inhalte aus dem Spiel und werden **nicht** committet. In `symbols/` stehen nur Namen, Adressen und Layouts.
