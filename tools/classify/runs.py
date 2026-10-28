"""Folgen gleicher Module in Adressreihenfolge (nach classify.py). Aufruf: runs.py [lo hi] [--min N]"""
import csv, sys
args = [a for a in sys.argv[1:] if not a.startswith('--')]
lo, hi = (int(args[0], 16), int(args[1], 16)) if len(args) >= 2 else (0, 1 << 32)
mn = int(sys.argv[sys.argv.index('--min') + 1]) if '--min' in sys.argv else 1
rows = sorted(csv.DictReader(open('symbols/functions.csv')), key=lambda r: int(r['addr'], 16))
runs = []
for r in rows:
    a = int(r['addr'], 16)
    if not lo <= a < hi:
        continue
    if runs and runs[-1][2] == r['module']:
        runs[-1][1] = a; runs[-1][3] += 1
    else:
        runs.append([a, a, r['module'], 1])
for s, e, m, n in runs:
    if n >= mn:
        print(f'{s:#x}-{e:#x} {n:4d} {m}')
print(len(runs), 'Folgen')
