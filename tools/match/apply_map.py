#!/usr/bin/env python3
"""Übernimmt symbols/pc_gc_map.csv in symbols/functions.csv (WP-08).

 - sicher/wahrscheinlich: Name (Klasse::Methode), module = Quelldatei, source=gc
 - moeglich: nur Notiz `gc?: Name`, keine Umbenennung
 - nicht zugeordnete Funktionen im Spielbereich, die Windows-Importe aufrufen: kind=win32glue
Läuft idempotent (setzt auf Basis der aktuellen Zeilen, Altname bleibt in der Notiz).
"""
import csv, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
FN = os.path.join(ROOT, 'symbols/functions.csv')
MAP = os.path.join(ROOT, 'symbols/pc_gc_map.csv')
CG = os.path.join(ROOT, 'decompiled/full/out3/callgraph.tsv')
KEEP_NAME = {'rekonstruiert', 'getestet', 'ersetzt'}


def kind_for(src):
    return 'game' if 'indep' in src else 'engine'


def main():
    rows = list(csv.DictReader(open(FN)))
    cols = list(rows[0].keys())
    by = {int(r['addr'], 16): r for r in rows}
    mp = {int(r['pc_addr'], 16): r for r in csv.DictReader(open(MAP))}
    ext = set()
    for line in open(CG):
        p = line.split()
        if p[1].startswith('EXTERNAL'):
            ext.add(int(p[0], 16))
    st = dict(named=0, kept=0, hint=0, glue=0)
    for a, m in mp.items():
        r = by.get(a)
        if r is None or r['source'] == 'fid':
            continue
        conf = m['confidence']
        tag = f"gc {m['gc_addr']} {m['beleg']} {m['score']}"
        old = r['name']
        note = re.sub(r'(; )?(gc\??[: ].*?)(?=$)', '', r['note']).strip('; ')
        if conf == 'moeglich':
            r['note'] = (note + '; ' if note else '') + f"gc?: {m['gc_name']} ({tag})"
            st['hint'] += 1
            continue
        if r['status'] in KEEP_NAME:
            r['note'] = (note + '; ' if note else '') + f"gc: {m['gc_name']} ({tag})"
            st['kept'] += 1
            continue
        if old and old != m['gc_name'] and not old.startswith(('FUN_', 'thunk_')) and 'Altname' not in note:
            note = (note + '; ' if note else '') + f'Altname: {old}'
        r['name'] = m['gc_name']
        if r['kind'] in ('', 'unbekannt'):      # Einordnung aus WP-02 (kind/module) nicht überschreiben
            r['kind'] = kind_for(m['quelldatei'])
        if not r['module']:
            r['module'] = m['quelldatei']
        if r['status'] == 'unbearbeitet':
            r['status'] = 'benannt'
        r['confidence'] = conf
        r['source'] = 'gc'
        r['note'] = (note + '; ' if note else '') + tag
        st['named'] += 1
    for a, r in by.items():
        if 0x401000 <= a < 0x5a0000 and a not in mp and a in ext and r['kind'] == 'unbekannt':
            r['kind'] = 'win32glue'
            r['confidence'] = r['confidence'] or 'vermutet'
            st['glue'] += 1
    with open(FN, 'w', newline='') as fh:
        w = csv.DictWriter(fh, fieldnames=cols, lineterminator='\r\n')
        w.writeheader()
        w.writerows(rows)
    print(st)


if __name__ == '__main__':
    main()
