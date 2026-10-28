#!/usr/bin/env python3
"""Erzeugt plan/QUELLBAUM.md aus symbols/gc_functions.csv und symbols/gc_objects.csv.

Die Gruppen sind Bereiche der Link-Reihenfolge (link_index aus gc_objects.csv). Die Grenzen
stammen aus der Durchsicht der Objektnamen (WP-07) und stehen unten in GROUPS.
"""
import csv
import os
from collections import Counter, defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FN = os.path.join(ROOT, 'symbols', 'gc_functions.csv')
OBJ = os.path.join(ROOT, 'symbols', 'gc_objects.csv')
OUT = os.path.join(ROOT, 'plan', 'QUELLBAUM.md')

# (erster, letzter link_index, Gruppe, Pfadvorschlag, Port-Relevanz)
GROUPS = [
    (0, 35, 'GC-Plattformcode des Spiels', 'GameCube/src/…', 'ersetzen (Plattformschicht: Renderer, Eingabe, Speicherkarte, Film)'),
    (36, 56, 'indep: Sammel-Übersetzungseinheiten (Unity-Build)', '../indep/src/generated/BB_Objs*.cpp', '**portieren** (Spielcode)'),
    (57, 139, 'indep: Einzeldateien', '../indep/src/<Bereich>/…', '**portieren** (Spielcode)'),
    (140, 145, 'Startcode (Assembler, crt)', '—', 'entfällt'),
    (146, 147, 'EA DVD-/Streaming-Hilfen (FSasync, sndvd)', '?', 'ersetzen'),
    (148, 200, 'C-Laufzeit (newlib, libgcc, libm) Teil 1', '—', 'entfällt (Host-libc)'),
    (201, 299, 'Nintendo Dolphin SDK (OS, DVD, VI, PAD, CARD, GX, EXI, SI, IP)', 'DolphinSDK1.0', 'entfällt'),
    (300, 309, 'EA Videospieler (avplayer, RCMP, MAD-Codec)', '?', 'nachbauen (WP-16)'),
    (310, 337, 'EA Grafikbibliothek (rendercontext, device, model, pcode …)', '?', 'ersetzen (WP-40)'),
    (338, 369, 'EAGLAnim (Animationsbibliothek)', '?', 'portieren (indep-nah)'),
    (370, 376, 'EA Dateisystem (filesys, BIG-Archive)', 'src/file/{gc,cmn}/', 'teilweise portieren (WP-43)'),
    (377, 382, 'EA RealMath', '?', 'prüfen'),
    (383, 390, 'Realmc (Speicherkarten-Bibliothek)', '?', 'ersetzen (WP-43)'),
    (391, 409, 'EA RealCore (Threads, Timer, Ausgabe)', 'e:/core/realcore/5/', 'ersetzen'),
    (410, 420, 'EA Schrift und Lokalisierung (font*, loccore)', '?', 'portieren'),
    (421, 432, 'EA SHAPE (Texturformat)', '?', 'portieren (WP-13)'),
    (433, 433, 'CSIS (Sound-Schnittstelle)', '?', 'portieren (WP-15/42)'),
    (434, 570, 'EA Audio (SNDI, AEMS, Mixer, Decoder)', '?', 'nachbauen (WP-15/42)'),
    (571, 624, 'C-Laufzeit (newlib stdio, libm) Teil 2', '—', 'entfällt'),
    (625, 648, 'Nintendo Dolphin SDK (OSFatal, AR, AX, DSP, CARD)', 'DolphinSDK1.0', 'entfällt'),
    (649, 652, 'Netzwerk-Hilfen (eth, md5, base64)', '?', 'Stub (WP-44)'),
    (653, 653, 'EA Codec (refsize)', 'src/codec/cmn/', 'portieren (WP-12)'),
]


def main():
    fns = list(csv.DictReader(open(FN)))
    objs = list(csv.DictReader(open(OBJ)))
    by_obj = defaultdict(list)
    for r in fns:
        by_obj[r['objekt']].append(r)
    # Objektpfade aus der Funktionstabelle (bekannter Pfad, sonst Dateiname)
    opath = {}
    for o in objs:
        opath[int(o['link_index'])] = o['pfad'] or o['objekt']
    fn_by_idx = defaultdict(list)
    for r in fns:
        fn_by_idx[int(r['objekt_index'])].append(r)

    out = ['# Quellbaum der GameCube-`Speed.elf`', '',
           'Automatisch erzeugt von `tools/gc_quellbaum.py` (WP-07). Nicht von Hand bearbeiten, sondern das Skript anpassen.', '',
           '**Lesart**', '',
           '- Ein *Objekt* ist eine Übersetzungseinheit im Link (Symbol `STT_FILE`). Die Reihenfolge ist die Link-Reihenfolge.',
           '- Der Spielcode liegt größtenteils in **Sammel-Übersetzungseinheiten** `BB_Objs*.cpp` (Unity-Build). Welche `.cpp` darin stecken, verrät nur DWARF, und das gibt es nur für 62 Einheiten.',
           '  Für die übrigen nennt die Tabelle die häufigsten Klassen. Sie sind die Grundlage für die Phase-3-Module.',
           '- „sicher“ = Zuordnung über lokale Symbole, `gcc2_compiled.`, Objektende (statische Initialisierung) oder DWARF-Bereich; „Name“ = über den Dateinamen geraten.',
           '- 301 Inline- und Template-Funktionen am Ende von `.text` (linkonce) gehören zu keinem Objekt. Sie stehen am Schluss nach Klassen.', '']
    tot = Counter()
    summary = []
    for lo, hi, title, path, port in GROUPS:
        rows = [r for i in range(lo, hi + 1) for r in fn_by_idx.get(i, [])]
        summary.append((title, hi - lo + 1, len(rows), sum(int(r['size']) for r in rows), port))
        tot['f'] += len(rows)
    out += ['## Übersicht', '', '| Gruppe | Objekte | Funktionen | Bytes | Port |', '|---|---:|---:|---:|---|']
    for t, no, nf, nb, port in summary:
        out.append(f'| {t} | {no} | {nf} | {nb:,} | {port} |'.replace(f'{nb:,}', f'{nb:,}'.replace(',', '.')))
    lk = by_obj.get('(linkonce)', [])
    out.append(f'| Inline/Template am Textende (linkonce) | – | {len(lk)} | {sum(int(r["size"]) for r in lk)} | mit der jeweiligen Klasse |')
    out.append(f'| **Summe** | {len(objs)} | {tot["f"] + len(lk) + len(by_obj.get("(init)", []))} | | |')
    out.append('')

    for lo, hi, title, path, port in GROUPS:
        out += [f'## {title}', '', f'Link-Index {lo}–{hi} · Pfad: `{path}` · Port: {port}', '']
        detail = lo <= 139 or (300 <= lo <= 433) or lo == 653
        if not detail:
            names = [f'{objs[i]["objekt"]} ({len(fn_by_idx.get(i, []))})' for i in range(lo, hi + 1)]
            out += [', '.join(names), '']
            continue
        out += ['| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |',
                '|---:|---|---:|---:|---:|---|']
        for i in range(lo, hi + 1):
            rows = fn_by_idx.get(i, [])
            sure = sum(1 for r in rows if r['objekt_sicher'] != 'nein')
            subs = Counter(r['quelldatei'] for r in rows if r['quelle_art'] == 'zeilen')
            if subs:
                content = ', '.join(f'`{s}` ({n})' for s, n in subs.most_common())
            else:
                kl = Counter(r['klasse'].split('<')[0] for r in rows if r['klasse'])
                free = sum(1 for r in rows if not r['klasse'])
                content = ', '.join(f'{k} ({n})' for k, n in kl.most_common(12 if lo == 36 else 6))
                if free:
                    content += (', ' if content else '') + f'frei: {free}'
            out.append(f'| {i} | `{opath[i]}` | {len(rows)} | {sum(int(r["size"]) for r in rows)} | {sure} | {content} |')
        out.append('')
    out += ['## Inline- und Template-Funktionen am Textende (linkonce)', '']
    kl = Counter(r['klasse'].split('<')[0] or '(frei)' for r in lk)
    out += [', '.join(f'{k} ({n})' for k, n in kl.most_common()), '']
    open(OUT, 'w').write('\n'.join(out) + '\n')
    print(f'{OUT}: {len(objs)} Objekte, {len(fns)} Funktionen')


if __name__ == '__main__':
    main()
