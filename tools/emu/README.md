# Emulations-Harness (WP-05)

Führt **einzelne** Funktionen aus `game/pc/Speed.exe` in Unicorn (x86-32) aus; das Spiel startet nie.
Einrichtung (einmalig, venv ist gitignored):
`python3 -m venv tools/emu/.venv && tools/emu/.venv/bin/pip install unicorn capstone`

## Aufruf aus Python
```python
import sys; sys.path.insert(0, "tools/emu")
from nfsu_emu import Emu
e = Emu()                                   # mappt Abschnitte an die Originaladressen
s = e.alloc(b"Hello\0")                     # Puffer im Scratch-Bereich (0x30000000..)
r = e.call(0x59aa50, "cdecl", [s])          # r.eax, r.edx, r.st0 (x87-Rückgabe), r.diff
r = e.call(0x40a7c0, track=True)            # track=True: Diff der Globals (langsam, ~30 ms)
for addr, alt, neu in r.diff: ...
```
- `conv`: `cdecl` · `stdcall` · `fastcall` (ECX, EDX, Rest Stack) · `thiscall` (`this=`, ECX) · Registerparameter über `regs={"eax": 5}`.
  Bei `stdcall`/`thiscall` räumt die Funktion selbst auf; der Harness setzt ESP ohnehin pro Aufruf neu.
- Floats als Argument: `f2i(1.5)` (Bitmuster); Rückgabe `i2f(r.eax)` bzw. `r.st0`.
- Speicher: `e.read/write/u32/f32(addr)`, `e.reset_scratch()`, `e.heap_alloc(n)`.
- Importe (254 Stück) zeigen auf Stubs, die `ImportCalled` auslösen. Nachbilden:
  `def heap_alloc(emu): return emu.heap_alloc(emu.arg(3)); heap_alloc.pop = 12; e.stubs["HeapAlloc"] = heap_alloc`
  (`emu.arg(n)` = n-tes Stack-Argument, 1-basiert; `pop` = stdcall-Bytes).
- Umgebung: Stack 0x1F0000–0x200000, TEB/FS (`fs:[0x18]` = Selbstzeiger), SSE/x87 an (FPCW 0x27F).
  Instruktionslimit 50 Mio. → `EmuFault`. Schreibzugriffe auf `.text` sind möglich (Probecode), wirken bis zum Prozessende.

## Vergleich mit dem Port
```
cmake --preset release && cmake --build port/build/release --target nfsu_core_shared
tools/emu/.venv/bin/python tools/emu/compare.py [--n 10000] [--seed 1] [fall ...]
```
Der Port exportiert Testeinstiege `t_<name>` aus `port/src/core/testapi.cpp` (`extern "C"`, Debug-Build mit ASan
ist per ctypes nicht ladbar, daher Release). Neuer Fall: `case_<name>(emu, lib, rng)` in `compare.py` schreiben,
mit `check(bedingung, info)` prüfen und in `CASES` eintragen. Gültige JDLZ-Ströme erzeugt `jdlz_encode`.

## Grenzen
Keine Windows-APIs, kein Dateizugriff, keine Threads. Funktionen mit Aufrufen in Spielglobals brauchen
vorbereitete Globals (`e.write(addr, …)`). Unicorn 2.1.4: `UC_X86_REG_FS_BASE` ist veraltet, daher GDT.
