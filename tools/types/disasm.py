#!/usr/bin/env python3
"""Disassembliert alle Funktionen aus symbols/functions.csv (Capstone, x86-32) und legt
{funktion: [(adresse, mnemonic, operanden), ...]} als Cache in refs/types_cache/disasm.pkl ab (WP-04).

  tools/emu/.venv/bin/python tools/types/disasm.py      # einmalig, ca. 1 min
Import:  from disasm import load;  code = load()
"""
import csv
import os
import pickle
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pe import Pe, ROOT

CACHE = os.path.join(ROOT, 'refs', 'types_cache', 'disasm.pkl')


def functions():
    with open(os.path.join(ROOT, 'symbols', 'functions.csv'), newline='') as fh:
        return [(int(r['addr'], 16), int(r['size'] or 0), r) for r in csv.DictReader(fh)]


def build():
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    pe = Pe()
    out = {}
    for a, size, _ in functions():
        if size <= 0 or pe.section_of(a) != '.text':
            continue
        code = pe.read(a, size)
        out[a] = [(i.address, i.mnemonic, i.op_str) for i in md.disasm(code, a)]
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    with open(CACHE, 'wb') as fh:
        pickle.dump(out, fh, protocol=pickle.HIGHEST_PROTOCOL)
    print('disasm: %d Funktionen, %d Befehle -> %s' % (len(out), sum(map(len, out.values())), CACHE))


def load():
    if not os.path.exists(CACHE):
        sys.exit('Cache fehlt: tools/emu/.venv/bin/python tools/types/disasm.py')
    with open(CACHE, 'rb') as fh:
        return pickle.load(fh)


if __name__ == '__main__':
    build()
