#!/usr/bin/env python3
"""Erzeugt symbols/chunks.csv: alle Chunk-IDs der Spieldateien samt Handler (aus consts.tsv).
Aufruf: tools/chunk_catalog.py   (braucht port/build/release/nfsu-tool)"""
import csv, os, re, subprocess, sys, collections
R = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOOL = f"{R}/port/build/release/nfsu-tool"
GAME = f"{R}/game/pc"
stats = {}  # id -> dict
files = []
for d, _, fs in os.walk(GAME):
    for f in fs:
        if f.lower().endswith(('.bin', '.bun', '.lzc')) and f != 'EReg.bin':
            files.append(os.path.join(d, f))
for p in sorted(files):
    out = subprocess.run([TOOL, "chunks", p], capture_output=True, text=True)
    if out.returncode: sys.exit(f"Fehler in {p}: {out.stderr}")
    for line in out.stdout.splitlines():
        m = re.match(r"\s*([0-9a-f]{8}) (C|-) size=(\d+)", line)
        i, c, sz = m.group(1), m.group(2), int(m.group(3))
        s = stats.setdefault(i, dict(files=set(), n=0, mn=sz, mx=sz, c=c))
        s['files'].add(os.path.relpath(p, GAME).split(os.sep)[0] + '/*' + os.path.splitext(p)[1].lower())
        s['n'] += 1; s['mn'] = min(s['mn'], sz); s['mx'] = max(s['mx'], sz)
names = {}
for r in csv.DictReader(open(f"{R}/symbols/functions.csv")):
    names[int(r['addr'], 16)] = r['name']
consts = collections.defaultdict(set)
for l in open(f"{R}/decompiled/full/out3/consts.tsv"):
    a = l.split('\t')
    consts[int(a[1], 16)].add(int(a[0], 16))
known = {}
ZEIGER = {'33310001': 'leer', '33310004': 'leer', '00134002': 'leer'}  # ADR-001
try:
    for r in csv.DictReader(open(f"{R}/symbols/chunks.csv")):
        known[r['id']] = r
except FileNotFoundError:
    pass
with open(f"{R}/symbols/chunks.csv", "w", newline="") as fo:
    w = csv.writer(fo)
    w.writerow("id,name,dateien,anzahl,min_size,max_size,container,handler_addr,zeiger,note".split(','))
    for i in sorted(stats):
        s = stats[i]
        hs = sorted(consts.get(int(i, 16), ()))
        h = ';'.join(f"0x{a:x}" for a in hs[:4]) or "nicht gefunden"
        k = known.get(i, {})
        w.writerow([i, k.get('name') or '', ';'.join(sorted(s['files']))[:120], s['n'], s['mn'], s['mx'],
                    'ja' if s['c'] == 'C' else 'nein', h, k.get('zeiger') or ZEIGER.get(i, ''), k.get('note', '')])
print(len(stats), "IDs,", len(files), "Dateien")
