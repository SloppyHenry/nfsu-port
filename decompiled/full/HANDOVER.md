# Übergabe: Speed.exe (Need for Speed Underground), Vollständige Dekompilierung

> **Hinweis (2026-10-20, Planungssession):** Die Linie „build2 kompilierbar machen“ (Abschnitte 6–8) ist **eingefroren**, weil sie zu einer Windows-32-Bit-EXE führt und nicht zu einem nativen macOS-Port. `out2/` bleibt die Referenzquelle. Maßgeblich sind jetzt `CLAUDE.md`, `plan/ROADMAP.md` und `plan/STATUS.md`. Die Punkte 6.A (Typexport) und 6.D (Namen übernehmen) sind in WP-04 und WP-01 aufgegangen.


Stand: 2026-10-20. Ziel des Nutzers: **Das gesamte Spiel dekompilieren, bis es sich selbst kompilieren lässt.**
Diese Datei ist für den nächsten Agenten. Sie sagt, was fertig ist, wie man es reproduziert, was noch fehlt und wo die Fallen sind.

## 1. Kurzfassung
- **Vollständiger Roh-Export ist fertig:** alle Funktionen von `Speed.exe` sind mit Ghidra 12.1.4 **per Headless-Skript** nach Dateien dekompiliert (nicht über MCP, daher ohne Token-Kosten). 10 377 von 10 378 Funktionen erfolgreich.
- **Ein generiertes C-Projekt existiert** (`build2/`), wird aber **noch nicht fehlerfrei kompiliert**: 26 094 Syntaxfehler (Clang, Syntaxcheck). Am Anfang waren es 136 216. Die Fehler sind überwiegend **fehlende Typdefinitionen**, nicht Logikfehler.
- **Nichts davon ist bisher gelaufen oder gelinkt.** Es gibt keine Windows-Toolchain auf diesem Mac, und das Spiel wurde nie ausgeführt.
- Die frühere Handarbeit (`../speed/`, 93 + 258 Funktionen benannt und per Hand nach C übersetzt) ist eine **eigene, kleinere Linie**. Der Vollexport ersetzt sie nicht und übernimmt ihre Namen noch nicht (siehe 6).

## 2. Umgebung
| Punkt | Wert |
|---|---|
| Arbeitsordner | `/Users/henryroth/Projekte/Recompilations/NFSU/decompiled/full` |
| Binary | `/Users/henryroth/Projekte/Recompilations/NFSU/game/pc/Speed.exe`, PE32 i386, Image Base 0x400000, SHA-256 `1f72abe1…db235` |
| Ghidra | `/Users/henryroth/rea-stack/ghidra_12.1.4_PUBLIC` (`support/analyzeHeadless`) |
| JDK | muss gesetzt werden: `JAVA_HOME=/opt/homebrew/Cellar/openjdk@21/21.0.12.1/libexec/openjdk.jdk/Contents/Home` und im `PATH`. Ohne das bricht Ghidra mit „Unable to prompt user for JDK path“ ab |
| Rechner | macOS arm64, 8 Kerne, 16 GB |
| Compiler | nur Apple `clang` (kann `-target i686-pc-windows-msvc -fsyntax-only`, aber nicht linken). **Kein** `i686-w64-mingw32-gcc`, kein Wine |
| Schutz | Keine Packer/Verschlüsselung gefunden (Entropie .text 6,74). SafeDisc-Begleitdateien (`SECDRV.SYS`, `DrvMgt.dll`) liegen dabei, im Binary selbst nicht belegt |

## 3. Dateien
```
full/
  ghidra_scripts/ExportAll.java    Export ohne Parameter-ID-Analyse (Variante 1)
  ghidra_scripts/ExportAll2.java   Export MIT "Decompiler Parameter ID" + analyzeAll (Variante 2, besser)
  out/    Export 1: 10 145 Funktionen (Aufrufkonvention fast überall "unknown")
  out2/   Export 2: 10 378 Funktionen  <- ZU VERWENDEN
          all_functions.c  (alle Dekompilate, Trenner: /* ==== addr name ==== */)
          functions.tsv    addr, name, Größe, isThunk, Konvention, Prototyp
          data.tsv         89 526 Symbole: addr, name, Ghidra-Datentyp, Länge
          failed.txt       1 Fehlschlag: 00666f5d ("Overlapping input varnodes")
  gen.py     erzeugt build*/ aus einem Exportordner
  check.sh   Clang-Syntaxcheck über build*/src, Fehlerstatistik nach build*/errors.txt
  build2/    ghidra.h, protos.h, data.h, renamed.json, src/part_0000..0069.c (150 Funktionen je Datei)
  build1/    veraltet (aus Export 1)
  run.log, run2.log   Ghidra-Protokolle
```
Wichtig: `out*/`, `build*/` und `project/` sind erzeugte Daten. `gen.py`, `check.sh` und die Skripte sind die Quelle der Wahrheit.

## 4. Reproduzieren
```bash
cd /Users/henryroth/Projekte/Recompilations/NFSU/decompiled/full
export JAVA_HOME=/opt/homebrew/Cellar/openjdk@21/21.0.12.1/libexec/openjdk.jdk/Contents/Home
export PATH=$JAVA_HOME/bin:$PATH
/Users/henryroth/rea-stack/ghidra_12.1.4_PUBLIC/support/analyzeHeadless project nfsu2 \
  -import /Users/henryroth/Projekte/Recompilations/NFSU/game/pc/Speed.exe \
  -scriptPath ghidra_scripts -postScript ExportAll2.java $PWD/out2 -deleteProject
python3 gen.py out2 build2 150
./check.sh build2 25
```
Laufzeit des Exports: etwa 10 Minuten (Parameter-ID-Analyse zusätzlich einige Minuten). Er läuft im Hintergrund problemlos, nicht mit `sleep` pollen.

## 5. Was `gen.py` macht (und welche Entscheidungen drinstecken)
1. Zerlegt `all_functions.c` in Einzelfunktionen und **entfernt alle `/* … */`-Kommentare**. Das war nötig, weil mehrzeilige Ghidra-Kommentare („Library: Visual Studio …“) die Prototyp-Erkennung zerstörten.
2. **Benennt alle Funktionen auf `FUN_<adresse>` um** (Ghidra hat per Bibliothekserkennung C++-Namen wie `CGlobalUtils::CGlobalUtils` vergeben, die kein gültiges C sind). Die Zuordnung steht in `build2/renamed.json`.
3. Leitet **Prototypen aus der Signaturzeile des Dekompilats** ab (`protos.h`).
4. **Datenzugriffe über Originaladressen:** `DAT_00732104` wird zu `#define DAT_00732104 (*(undefined4 *)0x00732104)` (`data.h`), Typ aus `data.tsv`, Strukturen als `uchar[len]`. Das setzt voraus, dass das Zielprogramm das **Original-Datenabbild an den Originaladressen** bereitstellt (siehe 7).
5. `ghidra.h` liefert die Ghidra-Typen (`undefined1/2/4/8`, `byte`, `uint`, `code`, `float10`), `CONCATnm`, `SUBnm`, `ZEXT`, `SEXT`, `CARRY4`, `SBORROW4` usw. und provisorische Windows-Typen (`DWORD`, `HWND`, …).

## 6. Offene Arbeit, nach Priorität
**A. Typen aus Ghidra exportieren (größter Hebel).** Die häufigsten Restfehler (Zahl = Vorkommen in `errors.txt`):
`FILE` 2059, `_s_HandlerType` 426, `_CONTEXT` 355, `_locale_t`/`_CRT_DOUBLE`/`_func_void_void_ptr` je 284, `_s_CatchableType` 213, `LCTYPE`, `HCALL`, `threadlocinfo`, `_ptiddata`, `LPLINECALLPARAMS` u. a.
Das sind Strukturen/Typen aus Ghidras Datentyparchiv (MSVC-CRT, Windows). Weg: im Java-Skript `ghidra.program.model.data.DataTypeWriter` (oder GUI „File → Export Program → C/C++ Header“) über den `ProgramBasedDataTypeManager` laufen lassen und als `types.h` ausgeben. Alternativ die echten Windows- und CRT-Header über mingw einbinden und nur die fehlenden Ghidra-Typen ergänzen (Vorsicht: Doppeldefinitionen mit `ghidra.h`).

**B. Weitere Fehlerklassen (Zahlen aus build2):**
- `LAB_<adr>` nicht deklariert (1 388): Adressen von Codestellen ohne Funktion (meist SEH-Handler, `&LAB_…`). Entweder als Funktion in Ghidra anlegen (`createFunction` im Skript, dann neu exportieren) oder als `extern void LAB_x();` deklarieren und die Stellen als Funktionen nachziehen.
- `int3` (455), `stack0x…` (404), `fRam…`/`uRam…` (216/103), `ExceptionList`: Ghidra-Pseudo-Variablen. `int3` = Breakpoint-Instruktion (`__debugbreak()`), `stack0x…` = Adresse eines Stackslots (Aufrufer-Variable, bräuchte manuelle Behandlung), `ExceptionList` = FS:[0] (SEH, in `ghidra.h` als `extern void *ExceptionList` angelegt, muss real auf `__readfsdword(0)` zeigen).
- Undeklarierte Funktionen: `ABS`, `SQRT`, `NAN`, `PackedFloatingMUL/ADD`, `builtin_strncpy` (Ghidra-Intrinsics → Makros/Inline-Funktionen), `QueryPerformanceCounter`, `GetLastError`, `CloseHandle`, `Sleep` usw. (Windows-API-Importe, kommen mit den echten Windows-Headern).
- Rest: falsche Zeiger/Integer-Mischung (`incompatible pointer to integer conversion`), Argumentanzahl-Konflikte durch ungenaue Prototypen. Das sind echte Dekompilat-Artefakte und werden am Ende durch Casts oder Nachtypisierung behoben.

**C. Registerparameter.** Export 2 hat die Konventionen weitgehend bestimmt (cdecl 1 617, fastcall 1 947, stdcall 5 001, thiscall 1 779, unknown 34). Trotzdem gibt es noch **11 485 `in_E?X`** und **17 723 `unaff_…`**-Zugriffe: das sind Registerwerte, die Ghidra als „von außen kommend“ behandelt. Sie sind im C-Code **uninitialisierte Variablen und damit semantisch falsch**, auch wenn sie kompilieren. Prüfen, ob eigene Konventionen (z. B. `__usercall`-artig bei MSVC-Optimierung) in `x86win.cspec` ergänzt oder einzelne Prototypen per `FunctionSignature` im Skript gesetzt werden müssen.

**D. Namen übernehmen.** `../speed/NOTES.md` und `../speed/src/*.c` enthalten ~350 von Hand benannte Funktionen (Adresse → Name, z. B. `0x4448b0 game_main`, `0x4446f0 game_init_subsystems`, `0x444bd0 game_frame_loop`, `0x401140 init_memory_system`). Diese Tabelle aus `NOTES.md` extrahieren und in `gen.py` als Umbenennung einbauen. Die handgeschriebenen Rekonstruktionen sind nur teilweise verhaltensgleich (66 exakt, 177 Skizzen, siehe `NOTES.md`).

**E. Übergang zum echten Link (noch gar nicht begonnen):**
1. Toolchain: `brew install mingw-w64` (liefert `i686-w64-mingw32-gcc`), erst nach Rückfrage beim Nutzer installieren.
2. Datenabbild: `.data`, `.rdata` aus der Original-EXE (VA 0x6c6000 ff. bzw. 0x688000 ff.) als Rohblock einbinden und per Linkerskript **genau an die Originaladresse** legen (kein ASLR, Base 0x400000, `-Wl,--image-base`/`--section-start`). Der Code adressiert die Daten über absolute Adressen (siehe 5.4).
3. **Funktionszeiger im Datenabbild** (Vtables, Tabellen) zeigen auf *alte* Codeadressen. Entweder jede rekompilierte Funktion per eigener Section an ihre Originaladresse zwingen (klappt nur, wenn sie nicht größer wird), oder zur Laufzeit alle 4-Byte-Werte im Datenabbild, die einer Funktions-Einstiegsadresse aus `functions.tsv` entsprechen, auf die neuen Adressen umschreiben (Reloc-Tabelle).
4. Importe: Ghidra nennt Windows-API-Aufrufe direkt beim Namen. Mit mingw-Headern und passenden Importbibliotheken (kernel32, user32, gdi32, ddraw/dinput/dsound/winmm …) linken. Importtabelle des Originals im Datenabbild wird nicht benutzt.
5. Laufen lassen geht nur unter Windows oder Wine/CrossOver, **nicht nativ auf dem Mac**. Spieldaten (`Cars/`, `Tracks/` …, seit 2026-10-20 unter `game/pc/`) liegen im Ordner `NFSU/`.

## 7. Fallen und ehrliche Einschätzung
- **Kompilierbar ≠ lauffähig.** Syntaktisch sauber zu bekommen ist realistisch (Typen, Makros, Casts). Dass das Programm danach **korrekt läuft**, ist offen: Registerparameter (C), Fließkomma-Details (`float10`, x87), Compiler-Optimierungen, SEH/C++-Exceptions und das Zeigerabbild (E.3) sind echte Risiken. Ein Spiel dieser Größe läuft aus Rohdekompilat erfahrungsgemäß nicht ohne lange Handarbeit pro Subsystem.
- **Kein Quellcode-Äquivalent.** Das Ergebnis ist Ghidra-Pseudocode mit `FUN_`-Namen, ohne Strukturen, ohne Klassen. Struktur- und Vtable-Layouts sind nur als Offsets im Code vorhanden.
- **Ghidra-Sitzungsnamen sind verloren.** Die früheren Umbenennungen in der REA-Sitzung wurden beim Schließen verworfen. Gedächtnis sind nur `../speed/NOTES.md` und `../speed/src/`.
- **Nicht über MCP dekompilieren.** `rea_batch_decompile` kostet ca. 25 000 Token pro 12 Funktionen. Immer das Headless-Skript benutzen.
- Der Export 2 hat 233 Funktionen mehr gefunden als Export 1 (Analyse hat Code entdeckt). Immer `out2` verwenden.
- Die Strings-Zählung weicht zwischen CLI (5 572) und REA (5 574) leicht ab, ungeklärt, unwichtig.
- Der Ordner enthält zusätzlich `uniws.exe`/`uniws/` (Widescreen-Patch, fremdes Binary), nicht Teil der Analyse.

## 8. Empfohlene Reihenfolge für den nächsten Agenten
1. Punkt 6.A (Typexport als `types.h`), danach `./check.sh build2` und Fehlerzahl notieren.
2. 6.B Intrinsics und Pseudo-Variablen in `ghidra.h`.
3. 6.D Namen aus `../speed/NOTES.md` einspielen.
4. Wenn die Fehlerzahl unter ~2 000 liegt: Nutzer um Erlaubnis für `brew install mingw-w64` bitten und mit echtem Compile (`-c`) statt `-fsyntax-only` weitermachen, danach Link nach 6.E.
5. Fortschritt nach jedem Schritt in dieser Datei (Abschnitt 1) und in `../speed/NOTES.md` nachziehen.
