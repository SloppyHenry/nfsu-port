#!/usr/bin/env python3
"""WP-02: Phase-3-Pakete aus symbols/modules.csv bilden → plan/MODULE.md, symbols/pakete.csv.

Bereiche (WP-30 … WP-39) fassen Module mit `behandlung=rekonstruieren` zusammen. Jeder Bereich wird
in Pakete (WP-3x.y) zu höchstens MAX_FN Funktionen zerlegt, in Adressreihenfolge; Module bleiben
ganz, außer sie sind selbst größer als MAX_FN (dann Teilung nach Adresse).
Abhängigkeiten: Aufrufkanten zwischen Bereichen (callgraph.tsv aus out3).

Aufruf: python3 tools/classify/module_md.py
"""
import collections
import csv
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
P = lambda *a: os.path.join(ROOT, *a)
MAX_FN = 150

# (WP, Titel, Regex auf den Modulnamen). Reihenfolge = vorgeschlagene Bearbeitungsreihenfolge.
AREAS = [
    ('WP-30', 'Basis (bWare), Hilfsfunktionen, Dateien und Spielablauf',
     r'BB_ObjsBWare|bMemoryOverloads|/misc/(Table|Timer|Spline|QuickSpline|point|LZCompress|bFile|QueuedFile)\.cpp|'
     r'^pc/utillib$|BB_ObjsMisc|^Localize|^EventManager|^Joylog|^Joystick|^Rumble|^EasterEggs|^Slider'),
    ('WP-31', 'Ecstasy-Engine und Render-Spielseite',
     r'BB_ObjsEcstasy|eMathE|bba_com|AcidFXE|CarRender|SkyRender|TrackMarkers|TrackMap|^lib/(shape|font)$|dbFiles|DemoDisc'),
    ('WP-32', 'Welt-Infrastruktur: Streaming, Topologie, Weltobjekte',
     r'BB_ObjsWorld[23]|Subworld|WorldModel|SpaceNode|SpaceCadet|MobileTopology|FernFlattener|TerrainEffects|'
     r'Smokeable|WorldEntity|WorldObject|CarWorldObject|SomeWorldObjects|AnimationMover|WorldObjectCollisionBody|'
     r'Clans|Avoidable|TrafficMover'),
    ('WP-33', 'Spielwelt und Rennen (World, Player, Car, Race), Fahrzeugdaten, Karriere, Replay',
     r'BB_ObjsWorld\.cpp|CarInfo|CarLoader|CarSkin|CareerMode|Replay|DemoModeReplay|RCMP_Player'),
    ('WP-34', 'Physik und Fahrzeugdynamik',
     r'BB_ObjsPhysics|RigidBody|Wheel\.cpp|Suspension|Skids|CarDamage|BodyDamage|PhysicsUpgrades'),
    ('WP-35', 'KI und Verkehr',
     r'BB_ObjsAI|Traffic(Controller|Teleporter|Intersection)|CrossTrafficEmitter|FuturePoints|TrackedController|'
     r'RealController|DrivingLocation'),
    ('WP-36', 'Kamera, Animation (EAGLAnim), NIS',
     r'BB_ObjsCamera|BB_ObjsAnim|^lib/eaglanim$|NIS\.cpp|NFSSkeleton'),
    ('WP-37', 'Frontend-Engine (FEng) und Frontend-Grundlagen (Fe)', r'BB_ObjsFeng|BB_ObjsFe\.cpp'),
    ('WP-38', 'Frontend-Bildschirme (FePkg, PC-Frontend)', r'BB_ObjsFePkg|^pc/frontend$|FEPkg_MapScreen'),
    ('WP-39', 'Audio-Spiellogik (EAXSound, CSIS)', r'BB_ObjsEAXSound|^lib/csis$'),
]


def short(m):
    return m.replace('../indep/src/generated/', '').replace('../indep/src/', '').replace('../indep/', '')


def main():
    fns = sorted(csv.DictReader(open(P('symbols/functions.csv'), newline='')), key=lambda r: int(r['addr'], 16))
    mods = {r['module']: r for r in csv.DictReader(open(P('symbols/modules.csv')))}
    area_of_mod = {}
    for m, r in mods.items():
        if r['behandlung'] != 'rekonstruieren':
            continue
        for wp, _, rx in AREAS:
            if re.search(rx, m):
                area_of_mod[m] = wp
                break
        else:
            raise SystemExit(f'Modul ohne Bereich: {m}')
    by_mod = collections.defaultdict(list)
    for r in fns:
        by_mod[r['module']].append(r)
    # Pakete
    pakete = []   # (paket, wp, module, start, end, n, bytes, benannt)
    for wp, title, _ in AREAS:
        ms = sorted([m for m, a in area_of_mod.items() if a == wp], key=lambda m: int(mods[m]['start'], 16))
        cur, cur_n, idx = [], 0, 1
        units = []
        for m in ms:
            fl = by_mod[m]
            if len(fl) <= MAX_FN:
                units.append((m, fl))
            else:
                k = -(-len(fl) // MAX_FN)
                step = -(-len(fl) // k)
                for i in range(k):
                    units.append((m, fl[i * step:(i + 1) * step]))
        for m, fl in units:
            if cur and cur_n + len(fl) > MAX_FN:
                pakete.extend((f'{wp}.{idx}', wp, mm, ff) for mm, ff in cur)
                idx += 1
                cur, cur_n = [], 0
            cur.append((m, fl))
            cur_n += len(fl)
        if cur:
            pakete.extend((f'{wp}.{idx}', wp, mm, ff) for mm, ff in cur)
    pk_of = {}
    for pk, wp, m, fl in pakete:
        for r in fl:
            pk_of[r['addr']] = pk
    with open(P('symbols/pakete.csv'), 'w', newline='') as fh:
        w = csv.writer(fh, lineterminator='\n')
        w.writerow(['paket', 'module', 'start', 'end', 'anzahl', 'bytes'])
        for pk, wp, m, fl in pakete:
            a0 = int(fl[0]['addr'], 16)
            a1 = int(fl[-1]['addr'], 16) + int(fl[-1]['size'] or 0)
            w.writerow([pk, m, f'{a0:#x}', f'{a1:#x}', len(fl), sum(int(r['size'] or 0) for r in fl)])
    # Abhängigkeiten zwischen Bereichen
    area_of_fn = {int(r['addr'], 16): area_of_mod.get(r['module'], '·' + (mods[r['module']]['behandlung'] if r['module'] in mods else '?'))
                  for r in fns}
    mod_of_fn = {int(r['addr'], 16): r['module'] for r in fns}
    dep = collections.defaultdict(collections.Counter)
    for line in open(P('decompiled/full/out3/callgraph.tsv')):
        a, b, _ = line.split('\t')
        if b.startswith('EXTERNAL'):
            continue
        a, b = int(a, 16), int(b, 16)
        fa, fb = area_of_fn.get(a), area_of_fn.get(b)
        if fa and fb and fa != fb and fa.startswith('WP'):
            dep[fa][fb if fb.startswith('WP') else short(mod_of_fn[b])] += 1
    write_md(fns, mods, area_of_mod, pakete, dep)


def write_md(fns, mods, area_of_mod, pakete, dep):
    titles = {wp: t for wp, t, _ in AREAS}
    tot = collections.Counter()
    for r in fns:
        tot[mods[r['module']]['behandlung']] += 1
    L = []
    L.append('# Module und Phase-3-Pakete\n')
    L.append('Erzeugt von `tools/classify/module_md.py` (WP-02) aus `symbols/modules.csv` und `symbols/functions.csv`. '
             'Nicht von Hand bearbeiten, sondern die Bereichstabelle im Skript anpassen und neu erzeugen.\n')
    L.append('**Lesart:** Ein *Modul* ist eine GC-Quelldatei (Objekt der Linkreihenfolge) oder ein PC-eigener Block (`pc/…`, `lib/…`, `msvc/…`). '
             'Ein *Bereich* (WP-30 … WP-39) fasst Module zusammen, ein *Paket* (WP-3x.y) ist eine Session mit höchstens '
             f'{MAX_FN} Funktionen. Die Funktionsliste eines Pakets steht in `symbols/pakete.csv` (Modul + Adressbereich).\n')
    L.append('## Gesamtumfang\n')
    L.append('| Behandlung | Funktionen | Anteil |\n|---|---:|---:|')
    n = len(fns)
    for k in ('rekonstruieren', 'ersetzen', 'entfällt'):
        L.append(f'| {k} | {tot[k]} | {100 * tot[k] / n:.1f} % |')
    L.append(f'| **Summe** | {n} | |\n')
    # Bereiche
    L.append('## Phase-3-Bereiche (Reihenfolge = Vorschlag)\n')
    L.append('| WP | Bereich | Module | Funktionen | Bytes | davon mit GC-Namen | Pakete | hängt ab von (Aufrufe) |')
    L.append('|---|---|---:|---:|---:|---:|---:|---|')
    pk_by_wp = collections.defaultdict(set)
    for pk, wp, m, fl in pakete:
        pk_by_wp[wp].add(pk)
    for wp, title, _ in AREAS:
        ms = [m for m, a in area_of_mod.items() if a == wp]
        fl = [r for r in fns if r['module'] in ms]
        named = sum(1 for r in fl if r['source'] == 'gc')
        deps = [(k, v) for k, v in dep[wp].most_common() if k.startswith('WP') and v >= 10]
        ds = ', '.join(f'{k} ({v})' for k, v in deps[:6])
        L.append(f'| {wp} | {title} | {len(ms)} | {len(fl)} | {sum(int(r["size"] or 0) for r in fl)} | '
                 f'{named} ({100 * named / max(1, len(fl)):.0f} %) | {len(pk_by_wp[wp])} | {ds} |')
    L.append('')
    L.append('Aufrufe aus Phase-3-Bereichen in Module, die ersetzt werden oder entfallen (Schnittstellen, die Phase 4 bzw. Stubs bereitstellen müssen):\n')
    L.append('| WP | Ziele außerhalb Phase 3 (Aufrufe) |\n|---|---|')
    for wp, _, _ in AREAS:
        ext = [(k, v) for k, v in dep[wp].most_common() if not k.startswith('WP')]
        L.append(f'| {wp} | ' + ', '.join(f'`{k}` ({v})' for k, v in ext[:8]) + ' |')
    L.append('')
    L.append('**Begründung der Reihenfolge:** WP-30 (bWare: Speicher, Listen, Mathe, Zeichenketten, Dateien) wird von allen Bereichen aufgerufen und ruft selbst fast nur CRT/RealCore. '
             'Danach die Engine (WP-31 Ecstasy) und die Welt-Infrastruktur (WP-32), auf denen Spielwelt (WP-33), Physik (WP-34), KI (WP-35) und Kamera (WP-36) aufsetzen. '
             'Frontend (WP-37/38) und Audio (WP-39) hängen von der Spielwelt ab, werden aber von ihr kaum aufgerufen und können parallel zu WP-34 … WP-36 laufen. '
             'Zyklen (z. B. World ↔ Physics ↔ AI) lassen sich nicht vermeiden; dort gilt: Blätter des Aufrufgraphs zuerst (siehe `WP-3x-VORLAGE.md`).\n')
    # Pakete
    L.append('## Pakete\n')
    L.append('| Paket | Module (Teil) | Funktionen | Bytes | Adressbereich |\n|---|---|---:|---:|---|')
    agg = collections.OrderedDict()
    for pk, wp, m, fl in pakete:
        g = agg.setdefault(pk, dict(m=[], n=0, b=0, lo=1 << 32, hi=0))
        g['m'].append(f'{short(m)} ({len(fl)})' if len(fl) != len([r for r in fns if r['module'] == m]) else short(m))
        g['n'] += len(fl)
        g['b'] += sum(int(r['size'] or 0) for r in fl)
        g['lo'] = min(g['lo'], int(fl[0]['addr'], 16))
        g['hi'] = max(g['hi'], int(fl[-1]['addr'], 16))
    for pk, g in agg.items():
        L.append(f"| {pk} | {', '.join(g['m'])} | {g['n']} | {g['b']} | {g['lo']:#x}–{g['hi']:#x} |")
    L.append('')
    # Nicht Phase 3
    L.append('## Module außerhalb von Phase 3\n')
    L.append('| Modul | Funktionen | Bytes | Behandlung | Ersatz / Grund |\n|---|---:|---:|---|---|')
    groups = collections.OrderedDict()
    for m, r in mods.items():
        if r['behandlung'] == 'rekonstruieren':
            continue
        key = (r['ersatz'], r['behandlung'])
        g = groups.setdefault(key, dict(m=[], n=0, b=0))
        g['m'].append(short(m))
        g['n'] += int(r['anzahl'])
        g['b'] += int(r['bytes'])
    for (txt, beh), g in sorted(groups.items(), key=lambda kv: -kv[1]['n']):
        L.append(f"| {', '.join(g['m'])} | {g['n']} | {g['b']} | {beh} | {txt} |")
    L.append('')
    open(P('plan/MODULE.md'), 'w').write('\n'.join(L))
    print(f'{len(agg)} Pakete, {sum(1 for _ in area_of_mod)} Module')


if __name__ == '__main__':
    main()
