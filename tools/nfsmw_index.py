#!/usr/bin/env python3
"""WP-03: Index der nfsmw-Definitionen und Zuordnung zu NFSU-GC-Funktionen.

Lesen:  refs/nfsmw (Quelle + config/GOWE69/symbols.txt), symbols/gc_functions.csv
Schreibt: refs/nfsmw_index.tsv (lokal, nicht committen)
          symbols/nfsmw_map.csv
Aufruf:  tools/nfsmw_index.py
"""
import csv
import glob
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
from gcc2_demangle import demangle  # noqa: E402

MW = os.path.join(ROOT, 'refs/nfsmw')
DEF = re.compile(r'^[ \t]*[\w:<>\*&\s~,]*?\b(?:(\w+)::)?(~?\w+)\s*\(([^;{}]*?)\)\s*(const)?\s*(?::[^;{}]*)?\{', re.M)
SKIP = {'if', 'for', 'while', 'switch', 'return', 'sizeof'}


def block_end(t, i):
    """Index nach der zur '{' bei i passenden '}'."""
    d = 0
    n = len(t)
    while i < n:
        c = t[i]
        if c == '{':
            d += 1
        elif c == '}':
            d -= 1
            if d == 0:
                return i + 1
        i += 1
    return n


def norm_args(s):
    """Parametertypen grob normalisieren: Namen weg, Leerraum weg."""
    out = []
    depth = 0
    cur = ''
    for c in s + ',':
        if c in '<(':
            depth += 1
        elif c in '>)':
            depth -= 1
        if c == ',' and depth == 0:
            out.append(cur.strip())
            cur = ''
        else:
            cur += c
    res = []
    for a in out:
        if not a or a == 'void':
            continue
        a = re.sub(r'=.*$', '', a)
        a = re.sub(r'\b(const|struct|class|unsigned|signed)\b', lambda m: m.group(1) if m.group(1) in ('unsigned',) else '', a)
        a = re.sub(r'\s+', '', a)
        a = re.sub(r'(?<=[\w\*&>])\w+$', '', a) if re.search(r'[\*&]\w+$', a) or re.search(r'\w\w+$', a) and not re.fullmatch(r'\w+', a) else a
        res.append(a)
    return res


def build_index():
    rows = []
    files = sorted(glob.glob(MW + '/src/Speed/Indep/**/*.cpp', recursive=True))
    for f in files:
        t = open(f, errors='ignore').read()
        rel = os.path.relpath(f, MW)
        for m in DEF.finditer(t):
            name = m.group(2)
            if name in SKIP:
                continue
            klass = m.group(1) or ''
            start = t.count('\n', 0, m.start(2)) + 1
            end = t.count('\n', 0, block_end(t, m.end() - 1)) + 1
            rows.append((klass, name, rel, start, end, len(norm_args(m.group(3))), m.group(3).strip().replace('\n', ' ')[:200]))
    return rows


def load_mw_symbols():
    d = {}
    pat = re.compile(r'^(\S+) = (\S+):0x([0-9A-Fa-f]+); // type:function size:0x([0-9A-Fa-f]+)')
    for ln in open(MW + '/config/GOWE69/symbols.txt', errors='ignore'):
        m = pat.match(ln)
        if m and not m.group(1).startswith('fn_'):
            d.setdefault(m.group(1), (int(m.group(4), 16), m.group(3)))
    return d


def main():
    idx = build_index()
    with open(ROOT + '/refs/nfsmw_index.tsv', 'w') as o:
        o.write('klasse\tmethode\tdatei\tvon\tbis\tnargs\tparameter\n')
        for r in idx:
            o.write('\t'.join(map(str, r)) + '\n')
    print('Definitionen:', len(idx), file=sys.stderr)
    by = {}
    for r in idx:
        by.setdefault((r[0], r[1]), []).append(r)

    mws = load_mw_symbols()
    # mangled -> (klasse, methode, nargs)
    mw_dem = {}
    for mg, (sz, _) in mws.items():
        try:
            d = demangle(mg)
        except Exception:
            d = None
        if d is None or d.special or not d.method:
            continue
        mw_dem[mg] = d

    def pick(klass, meth, nargs):
        c = by.get((klass, meth), [])
        if not c:
            return None
        same = [r for r in c if r[5] == nargs]
        return (same or c)[0], len(same or c)

    # Namen-Index der MW-Symbole: (klasse, methode) -> [mangled]
    mw_by_name = {}
    for mg, d in mw_dem.items():
        mw_by_name.setdefault((d.klass, d.method), []).append(mg)

    out = []
    with open(ROOT + '/symbols/gc_functions.csv') as fh:
        gc = [r for r in csv.DictReader(fh) if 'indep' in r['quelldatei']]
    skipped = 0
    for r in gc:
        mg = r['mangled']
        meth = r['methode']
        klass = r['klasse']
        if not meth or meth.startswith(('FUN_', 'fn_', '_GLOBAL_', '__sinit', '__static')) or 'thunk' in r['demangled'].lower():
            skipped += 1
            continue
        ug = int(r['size'])
        if mg in mws:
            mwsz = mws[mg][0]
            hit = pick(klass, meth, len(r['argumente'].split(';')) if r['argumente'] else 0)
            # Größe prüfen
            ratio = abs(ug - mwsz) / max(ug, 1)
            conf = 'wahrscheinlich' if ratio <= 0.10 else 'moeglich'
            f = hit[0][2] if hit else ''
            z = f'{hit[0][3]}-{hit[0][4]}' if hit else ''
            out.append((r['gc_addr'], r['demangled'], mg, f, z, ug, mwsz, 'signatur', conf))
            continue
        cand = mw_by_name.get((klass, meth))
        hit = pick(klass, meth, len(r['argumente'].split(';')) if r['argumente'] else 0)
        if cand or hit:
            mwsz = mws[cand[0]][0] if cand else ''
            mgn = cand[0] if cand else ''
            f = hit[0][2] if hit else ''
            z = f'{hit[0][3]}-{hit[0][4]}' if hit else ''
            if not f:
                continue
            out.append((r['gc_addr'], r['demangled'], mgn, f, z, ug, mwsz, 'name', 'moeglich'))
    with open(ROOT + '/symbols/nfsmw_map.csv', 'w', newline='') as o:
        w = csv.writer(o)
        w.writerow(['gc_addr', 'gc_name', 'mw_mangled', 'mw_file', 'mw_zeilen', 'ug_size', 'mw_size', 'stufe', 'confidence'])
        w.writerows(out)
    from collections import Counter
    print(len(gc), 'GC-indep; übersprungen', skipped, file=sys.stderr)
    print(Counter((r[7], r[8]) for r in out), file=sys.stderr)


if __name__ == '__main__':
    main()
