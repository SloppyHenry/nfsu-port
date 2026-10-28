# tools/types – Typen, Vtables, COM (WP-04)

Einrichtung wie `tools/emu` (venv mit capstone). Reihenfolge:
```
tools/emu/.venv/bin/python tools/types/disasm.py     # Cache refs/types_cache/disasm.pkl (2 s)
tools/emu/.venv/bin/python tools/types/vtables.py    # symbols/vtables.csv
tools/emu/.venv/bin/python tools/types/com.py        # com_vtables/com_calls/com_globals.csv, com_types.h
python3 tools/types/gen_types.py                     # symbols/types.h
tools/reexport.sh                                    # ApplyTypes.java + Export (ca. 8 min)
```
`pe.py` liest die Exe, `com_defs.py` hält die COM-Schnittstellen (Slot-Offsets gegen bekannte Werte geprüft).
Größenprüfung von `types.h`: mit clang `-m32` und `_Static_assert(sizeof(...) == n)` je Struktur (753 bestanden).
