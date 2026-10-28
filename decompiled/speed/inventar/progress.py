#!/usr/bin/env python3
"""Fortschritt Ebene 3. Aufruf:
  progress.py mark STATUS addr [addr ...]   setzt Status (z.B. DONE-exakt, DONE-skizze)
  progress.py next MODUL [n]                 zeigt die nächsten n offenen Adressen des Moduls
  progress.py summary                        zählt die Stati
"""
import sys, pathlib
P = pathlib.Path(__file__).with_name('ebene3_progress.txt')
rows = [l.split(None, 2) for l in P.read_text().splitlines() if l.strip()]
cmd = sys.argv[1]
if cmd == 'mark':
    status, addrs = sys.argv[2], set(sys.argv[3:])
    for r in rows:
        if r[0] in addrs:
            r[2] = status
    P.write_text(''.join(f'{r[0]} {r[1]} {r[2]}\n' for r in rows))
    print('marked', len(addrs))
elif cmd == 'next':
    mod = sys.argv[2]; n = int(sys.argv[3]) if len(sys.argv) > 3 else 12
    open_ = [r[0] for r in rows if r[1] == mod and r[2].startswith('OFFEN')]
    print(' '.join(open_[:n]), f'(offen gesamt: {len(open_)})')
elif cmd == 'summary':
    from collections import Counter
    print(Counter(r[2].split()[0] for r in rows))
