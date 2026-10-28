"""Kompakte Sicht auf einen Adressbereich: je Funktion Größe, kind, Name/GC-Treffer, erster String, Importe.
Aufruf: python3 tools/classify/view.py 0x584000 0x5a0000 [--runs] [--info]"""
import csv, sys, collections
sys.path.insert(0, 'tools/classify')
import pe_imports

lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
runs = '--runs' in sys.argv
fs = [f for f in csv.DictReader(open('symbols/functions.csv')) if lo <= int(f['addr'], 16) < hi]
gm = {int(r['pc_addr'], 16): r for r in csv.DictReader(open('symbols/pc_gc_map.csv'))}
s1 = {}
for l in open('decompiled/full/out3/strrefs.tsv', encoding='utf-8', errors='replace'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 3:
        s1.setdefault(int(p[0], 16), p[2][:38])
ext = pe_imports.external_names()
imp = collections.defaultdict(set)
for l in open('decompiled/full/out3/callgraph.tsv'):
    a, b, _ = l.split('\t')
    if b.startswith('EXTERNAL'):
        imp[int(a, 16)].add(ext[b][1])
prev = None
for f in fs:
    a = int(f['addr'], 16)
    g = gm.get(a)
    q = (g['quelldatei'].replace('../indep/src/generated/', '') + ('' if g['confidence'] != 'moeglich' else '?')) if g else ''
    if runs:
        key = q.rstrip('?') or '-'
        if prev and prev[0] == key:
            prev[2] = a; prev[3] += 1
            continue
        if prev: print(f'{prev[1]:#x}-{prev[2]:#x} {prev[3]:4d} {prev[0]}')
        prev = [key, a, a, 1]
        continue
    if '--info' in sys.argv and not (q or a in s1 or imp[a] or (f['name'] or '')):
        continue
    print(f"{a:#x} {f['size']:>5} {f['kind'][:6]:6} {(f['name'] or '')[:34]:34} {q[:28]:28} {s1.get(a, '')!r:.40} {','.join(sorted(imp[a]))[:50]}")
if runs and prev: print(f'{prev[1]:#x}-{prev[2]:#x} {prev[3]:4d} {prev[0]}')
