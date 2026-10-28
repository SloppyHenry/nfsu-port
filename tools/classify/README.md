# WP-02 Klassifikation
- `classify.py` setzt `kind`/`module` in `symbols/functions.csv`, schreibt `symbols/modules.csv` und `symbols/module_belege.csv` (Beleg je Funktion). Idempotent, Zonen und Regeln stehen oben im Skript.
- `module_md.py` erzeugt `plan/MODULE.md` und `symbols/pakete.csv` (Bereichstabelle AREAS im Skript).
- `view.py <von> <bis> [--runs|--info]` zeigt Funktionen eines Bereichs mit Größe, GC-Treffer, erstem String, Importen. `runs.py` zeigt Modulfolgen.
- `pe_imports.py` löst Ghidras `EXTERNAL:…` zu DLL!Funktion auf.
Reihenfolge nach Änderungen an Zuordnungen: `classify.py`, dann `module_md.py`.
