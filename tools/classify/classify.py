#!/usr/bin/env python3
"""WP-02: Klassifikation aller PC-Funktionen (kind, module) und Modultabelle.

Verfahren (Belege je Funktion in symbols/module_belege.csv):
 1. Zonen: feste Adressbereiche aus Linkreihenfolge, Ausrichtung und Marker-Strings
    (ZONES unten, Belege in plan/log/2026-10-21-WP-02.md).
 2. Startpunkte (Seeds) je Zone:
    - GC-Anker aus pc_gc_map.csv (sicher/wahrscheinlich) → GC-Objekt (Quelldatei)
    - Namen, Strings und Importe nach den Regeln der Zone (SEEDS)
    Ausreißer (Seed, dessen Label von beiden Nachbar-Seeds abweicht, die unter sich
    gleich sind) behalten ihr Label, begrenzen aber keine Segmente.
 3. Lücken zwischen zwei Seeds: gleiches Label → Label übernehmen ("segment").
    Verschiedene Labels → Trennstelle mit den wenigsten Widersprüchen zum nächstgelegenen
    Seed im Aufrufgraph (Breitensuche über Funktionen ohne Seed) ("grenze-aufruf");
    ohne Aufrufbelege Trennung in der Mitte ("grenze-mitte").
 4. Aus dem Label folgen kind, behandlung und (für Bibliotheken) status.

Aufruf: python3 tools/classify/classify.py [--dry]   (schreibt functions.csv, modules.csv, module_belege.csv)
"""
import collections
import csv
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, 'tools/classify'))
import pe_imports  # noqa: E402

P = lambda *a: os.path.join(ROOT, *a)
FN = P('symbols/functions.csv')
OUT3 = P('decompiled/full/out3')

# ---------------------------------------------------------------- Zonen
# (start, ende, zone). Ende exklusiv. Grenzen siehe Log WP-02.
ZONES = [
    (0x401000, 0x41de40, 'pcplat'),    # PC-Plattformcode (Gegenstück zu GC-Objekten 0–35) + PC-only
    (0x41de40, 0x52a5a0, 'indep'),     # Spielcode, Linkreihenfolge = GC (BB_ObjsWorld … BB_ObjsEcstasy)
    (0x52a5a0, 0x546cc0, 'online'),    # PC-only: Online-Lobby (ps2nfs04.ea.com) und MU_Online*-Bildschirme
    (0x546cc0, 0x59ed50, 'indep'),     # bWare-Teil, indep-Einzeldateien, Rest der Sammeldateien, LZ/JDLZ
    (0x59ed50, 0x611550, 'd3dx'),      # D3DX9 Effekt- und Shadercompiler (nicht 16-Byte-ausgerichtet)
    (0x611550, 0x6641fc, 'ealib'),     # EA-Bibliotheken: EAGL, EAGLAnim, Font, SHAPE, FILESYS, RealCore, RCMP, CSIS, Sound, Netz
    (0x6641fc, 0x678000, 'crt'),       # MSVC-CRT (mit EA ASYNCFILE 0x6767b0–0x677120)
    (0x678000, 0x67e660, 'unwind'),    # Unwind@-Funclets der C++-Ausnahmebehandlung
    (0x67e660, 0x688000, 'staticinit'),  # dynamische Initialisierer globaler Objekte ($E…)
]

# Feste Teilbereiche (überschreiben Zone, vor der Segmentierung): (start, ende, label, beleg)
FIXED = [
    (0x59d8e0, 0x59dd70, 'pc/utillib', 'Hilfsfunktionen am Ende des Spielcodes, keine GC-Anker'),
    (0x59dd70, 0x59e020, '../indep/src/misc/LZCompress.cpp', 'JDLZ/COMP-Dekompression (WP-05), GC-Hinweise LZCompress'),
    (0x59e020, 0x59ed50, 'pc/utillib', 'Hilfsfunktionen (Bild-Compositing für CompositeSkin, Zeichenketten), keine GC-Anker'),
    (0x6767b0, 0x677120, 'lib/filesys', 'hlafile.cpp ASYNCFILE_* (GC-Anker, 16-Byte-Ausrichtung zwischen CRT)'),
]

# Seed-Regeln je Zone: (label, art, regex). art: name | str | imp
SEEDS = {
    'pcplat': [
        ('pc/frontend', 'str', r'\.fng$|^PCHELP_|^Res_%02d|SliderBase|^on_option_|^menubar%d|^Player_DATA_0'),
        ('pc/savegame', 'str', r'^\\\\NFS Underground\\\\'),
        ('pc/savegame', 'imp', r'^SHGetFolderPathA$'),
        ('pc/input', 'str', r'DInput|wheel on chan|enumerate devices|EZ Wheel|force gain|Setting gain'),
        ('pc/main', 'imp', r'^(CreateWindowExA|AdjustWindowRect|GetMessageA|PeekMessageA|SetErrorMode|DestroyWindow|GetPriorityClass|CreateMutexA)$'),
        ('pc/main', 'str', r'^Software\\\\EA Games|^NFS Underground Game$|^Main Pool$'),
        ('pc/render', 'str', r'\.efx$|^WorldViewProj$|^HEADLIGHTS$|^RVM_MASK$|^WHITE16X16'),
    ],
    'online': [],
    'ealib': [
        ('lib/eaglanim', 'name', r'^EAGLAnim::'),
        ('lib/eaglanim', 'str', r'EAGLAnim|^Facing:|turn align|AnimationBank|Bad signature channel'),
        ('lib/eagl', 'name', r'^EAGL::'),
        ('lib/eagl', 'str', r'EAGL|IDirect3D|pDevice|^Window Name$'),
        ('lib/font', 'name', r'^FONT_'),
        ('lib/shape', 'name', r'^SHAPE_'),
        ('lib/shape', 'str', r'^SHP%dx'),
        ('lib/filesys', 'name', r'^FILESYS_|^reserveop$'),
        ('lib/filesys', 'str', r'^FILE_|^FILESYS_|^getdirectory|^File System$'),
        ('lib/filesys', 'imp', r'^(CreateFileMappingA|FindFirstFileA|GetLogicalDrives|MapViewOfFile)$'),
        ('lib/realcore', 'name', r'^(SYNCTASK_|REAL_|SYSTEM_abort|iFILE_|timer_init|init_thread_handle)'),
        ('lib/realcore', 'str', r'^TIMER_|^SYNCTASK|GenuineIntel|^PRINT_|^ERROR: %s|^FILE %s LINE|Pentium'),
        ('lib/realcore', 'imp', r'^(timeSetEvent|timeKillEvent|keybd_event)$'),
        ('lib/rcmp', 'name', r'^(RCMP::|AV::)'),
        ('lib/rcmp', 'str', r'RCMP|AV::|avplayer|codec|rcmpbase'),
        ('lib/csis', 'name', r'^Csis::'),
        ('lib/easound', 'str', r'^SNDAUTHOR'),
        ('lib/net', 'str', r'GmUtil|^TCP:|connecting to|^recv:|comm/datamodem|^TAPI%d|^localhost$|^SERIAL$|^@server$|^IDENT$|^DIRECT$|hello world|paranoid'),
        ('lib/net', 'imp', r'^(Ordinal_\d+|WSAIoctl|line[A-Z]\w+|SetCommMask|PurgeComm|GetCommState|Netbios)$|^#\d+$'),
    ],
}

# Label → (kind, behandlung, ersatz/bemerkung, port_wp)
LIB = {
    'lib/d3dx9': ('d3dx', 'ersetzen', 'Effekte offline nach Metal übersetzen (WP-17); Funktionen selbst nicht portieren', 'WP-17'),
    'lib/eagl': ('ealib', 'ersetzen', 'EA Grafikbibliothek (D3D9/D3D8-Wrapper) → Render-Schnittstelle WP-40', 'WP-40'),
    'lib/eaglanim': ('ealib', 'rekonstruieren', 'EAGLAnim-Animationsbibliothek, plattformunabhängig (GC-Objekte 338–369)', 'Phase 3'),
    'lib/font': ('ealib', 'rekonstruieren', 'EA Schriftbibliothek (FONT_*), klein', 'Phase 3'),
    'lib/shape': ('ealib', 'rekonstruieren', 'EA SHAPE-Texturformat', 'WP-13'),
    'lib/filesys': ('ealib', 'ersetzen', 'EA FILESYS/ASYNCFILE → Dateischicht WP-43 (Schnittstelle nachbilden)', 'WP-43'),
    'lib/realcore': ('ealib', 'ersetzen', 'EA RealCore (Timer, Threads, CPU, Fenster, Abbruch) → WP-43', 'WP-43'),
    'lib/rcmp': ('rcmp', 'ersetzen', 'EA RCMP/AV-Player/MAD → FFmpeg (WP-16)', 'WP-16'),
    'lib/csis': ('sound', 'rekonstruieren', 'CSIS-Sound-Schnittstelle (Spiel ↔ EA-Audio)', 'WP-42'),
    'lib/easound': ('sound', 'ersetzen', 'EA Audio (SNDSYS/AEMS, Mixer, Decoder) → Audio-Ausgabe WP-42', 'WP-42'),
    'lib/net': ('net', 'entfällt', 'EA-Netzwerk (DirtySock-artig: TCP/UDP, TAPI-Modem, seriell)', 'WP-44'),
    'msvc/crt': ('crt', 'ersetzen', 'Host-libc/libc++', '–'),
    'msvc/unwind': ('crt', 'entfällt', 'vom Compiler erzeugte Unwind-Funclets (C++-Ausnahmen/Destruktoren)', '–'),
    'msvc/staticinit': ('game', 'entfällt', 'dynamische Initialisierer globaler Objekte; im Port Teil der Globals des jeweiligen Moduls', '–'),
    'win32/thunks': ('thunk', 'entfällt', 'Sprungstellen auf Importe', '–'),
    'pc/online': ('net', 'entfällt', 'PC-Online-Lobby und MU_Online*-Bildschirme (Server abgeschaltet)', 'WP-44'),
    'pc/main': ('win32glue', 'ersetzen', 'WinMain, Fenster, Registry, Einzelinstanz, Hauptschleife → SDL3 (WP-43/50)', 'WP-43'),
    'pc/render': ('win32glue', 'ersetzen', 'PC-Ecstasy-Renderer (D3D9) → WP-40', 'WP-40'),
    'pc/input': ('win32glue', 'ersetzen', 'DirectInput, Logitech-Lenkrad, Force Feedback → WP-41', 'WP-41'),
    'pc/savegame': ('win32glue', 'ersetzen', 'Speicherstände unter Eigene Dateien → WP-43', 'WP-43'),
    'pc/utillib': ('engine', 'rekonstruieren', 'PC-Hilfsfunktionen ohne GC-Gegenstück (Ende des Spielcodes)', 'Phase 3'),
    'pc/frontend': ('game', 'rekonstruieren', 'PC-only-Frontend (Tastatur-/Grafikoptionen, PC-Hilfe, Menüleiste)', 'Phase 3'),
}

# GC-Plattformobjekte (Link-Index 0–35): Behandlung auf dem PC
PLAT = {
    'eMathE.cpp': ('engine', 'rekonstruieren', 'Ecstasy-Mathematik, plattformunabhängig'),
    'bba_com.cpp': ('engine', 'rekonstruieren', 'AcidEffectSystem (Partikel), plattformunabhängig'),
    'AcidFXE.cpp': ('engine', 'rekonstruieren', 'Acid-Effekte'),
    'MovieE.cpp': ('win32glue', 'ersetzen', 'Filmwiedergabe → WP-16'),
    'Stream.cpp': ('win32glue', 'ersetzen', 'Datenstrom-Ein/Ausgabe → WP-43'),
    'GC_MemoryCard.cpp': ('win32glue', 'ersetzen', 'Speicherstände → WP-43'),
    'JoyloggableLoadSavegame.cpp': ('win32glue', 'ersetzen', 'Speicherstände → WP-43'),
    'dbFiles.cpp': ('engine', 'rekonstruieren', 'Frontend-Datenbankdateien'),
    'DemoDisc.cpp': ('engine', 'rekonstruieren', 'Demo-Steuerung'),
}
PLAT_INPUT = {'JoyE.cpp', 'Wheels.cpp', 'Ramp.cpp', 'Periodic.cpp', 'LGWheels.cpp', 'Force.cpp',
              'Constant.cpp', 'Condition.cpp'}
# GC-Linkindex → Gruppe. GC-only-Objekte (Dolphin-SDK, newlib, Realmc, Startcode) gibt es auf dem PC nicht:
# Anker dorthin sind Fehlzuordnungen und werden verworfen.
LIB_BY_LINK = [((298, 309), 'lib/rcmp'), ((310, 337), 'lib/eagl'), ((338, 369), 'lib/eaglanim'),
               ((370, 376), 'lib/filesys'), ((377, 382), 'lib/realcore'), ((391, 409), 'lib/realcore'),
               ((410, 420), 'lib/font'), ((421, 432), 'lib/shape'), ((433, 433), 'lib/csis'),
               ((434, 570), 'lib/easound')]


def link_group(li):
    if li is None:
        return None
    if li <= 35:
        return 'plat'
    if li <= 139:
        return 'indep'
    for (a, b), lab in LIB_BY_LINK:
        if a <= li <= b:
            return lab
    return 'gconly'


ENGINE_OBJ = re.compile(r'BB_Objs(BWare|Ecstasy|Feng|Anim)\.cpp$|/misc/|/bWare/')


def load():
    rows = list(csv.DictReader(open(FN, newline='')))
    cols = list(rows[0].keys())
    by = {int(r['addr'], 16): r for r in rows}
    addrs = sorted(by)
    # GC-Objekte
    link = {}
    for r in csv.DictReader(open(P('symbols/gc_objects.csv'))):
        link.setdefault(r['objekt'], int(r['link_index']))
    gobj = {}
    for r in csv.DictReader(open(P('symbols/gc_functions.csv'))):
        gobj[r['gc_addr'].upper()] = r['objekt']
    anchors = {}
    for r in csv.DictReader(open(P('symbols/pc_gc_map.csv'))):
        if r['confidence'] in ('sicher', 'wahrscheinlich'):
            o = gobj.get(r['gc_addr'].upper())
            if o and o != '(linkonce)':
                anchors[int(r['pc_addr'], 16)] = o
    # Aufrufgraph (intern) und Importe
    ext = pe_imports.external_names()
    nb = collections.defaultdict(list)
    imps = collections.defaultdict(set)
    for line in open(os.path.join(OUT3, 'callgraph.tsv')):
        a, b, _ = line.rstrip('\n').split('\t')
        a = int(a, 16)
        if b.startswith('EXTERNAL'):
            imps[a].add(ext[b][1])
            continue
        b = int(b, 16)
        if a != b:
            nb[a].append(b)
            nb[b].append(a)
    strs = collections.defaultdict(list)
    for line in open(os.path.join(OUT3, 'strrefs.tsv'), encoding='utf-8', errors='replace'):
        p = line.rstrip('\n').split('\t')
        if len(p) >= 3:
            strs[int(p[0], 16)].append(p[2])
    return rows, cols, by, addrs, link, anchors, nb, imps, strs


def zone_of(a):
    for s, e, z in ZONES:
        if s <= a < e:
            return z
    return None


def bfs_labels(seq, seeds, nb):
    """Nächstgelegener Seed im Aufrufgraph (Wege nur über Funktionen ohne Seed) → {addr: (label, dist)}."""
    inzone = set(seq)
    best = {}
    frontier = [(a, l) for a, l in seeds.items()]
    dist = 0
    seen = {a: l for a, l in seeds.items()}
    while frontier:
        dist += 1
        nxt = {}
        for a, l in frontier:
            for b in nb.get(a, ()):
                if b in inzone and b not in seen:
                    nxt.setdefault(b, set()).add(l)
        frontier = []
        for b, ls in nxt.items():
            if len(ls) == 1:
                l = next(iter(ls))
                seen[b] = l
                best[b] = (l, dist)
                frontier.append((b, l))
            else:
                seen[b] = None      # mehrdeutig: nicht weiterleiten
    return best


def segment(seq, seeds, nb):
    """seq: Adressen einer Zone (sortiert); seeds: {addr: label}. Füllt Lücken, gibt {addr: (label, beleg)}."""
    out = {a: (l, 'seed') for a, l in seeds.items()}
    idx = [i for i, a in enumerate(seq) if a in seeds]
    # Ausreißer: Seed weicht von beiden Nachbarn ab, die gleich sind
    core = []
    for j, i in enumerate(idx):
        l = seeds[seq[i]]
        if 0 < j < len(idx) - 1:
            lp, ln = seeds[seq[idx[j - 1]]], seeds[seq[idx[j + 1]]]
            if lp == ln and lp != l:
                out[seq[i]] = (l, 'seed-ausreisser')
                continue
        core.append(i)
    if not core:
        return out
    bfs = bfs_labels(seq, {seq[i]: seeds[seq[i]] for i in core}, nb)
    # Rand vor dem ersten / nach dem letzten Seed
    for i in range(0, core[0]):
        if seq[i] not in out:
            out[seq[i]] = (seeds[seq[core[0]]], 'rand')
    for i in range(core[-1] + 1, len(seq)):
        if seq[i] not in out:
            out[seq[i]] = (seeds[seq[core[-1]]], 'rand')
    for i0, i1 in zip(core, core[1:]):
        L, R = seeds[seq[i0]], seeds[seq[i1]]
        gap = [seq[i] for i in range(i0 + 1, i1) if seq[i] not in out]
        if not gap:
            continue
        if L == R:
            for a in gap:
                out[a] = (L, 'segment')
            continue
        # Trennstelle k: gap[:k] → L, gap[k:] → R; Kosten = Funktionen, deren nächster Seed
        # im Aufrufgraph (BFS) die jeweils andere Seite ist (Gewicht 1/Abstand)
        def w(a, lab):
            x = bfs.get(a)
            return 1.0 / x[1] if x and x[0] == lab else 0.0
        vl = [w(a, L) for a in gap]
        vr = [w(a, R) for a in gap]
        best, bk = None, None
        n = len(gap)
        for k in range(n + 1):
            cost = sum(vr[:k]) + sum(vl[k:])
            key = (cost, abs(k - n / 2))
            if best is None or key < best:
                best, bk = key, k
        evid = sum(vl) + sum(vr) > 0
        for i, a in enumerate(gap):
            out[a] = ((L if i < bk else R), 'grenze-aufruf' if evid else 'grenze-mitte')
    return out


def seed_rules(zone, addrs, by, imps, strs):
    seeds = {}
    for a in addrs:
        r = by[a]
        for lab, art, rx in SEEDS.get(zone, []):
            if art == 'name':
                hit = re.search(rx, r['name'] or '')
            elif art == 'str':
                hit = any(re.search(rx, s) for s in strs.get(a, ()))
            else:
                hit = any(re.search(rx, i) for i in imps.get(a, ()))
            if hit:
                seeds[a] = lab
                break
    return seeds


def main():
    dry = '--dry' in sys.argv
    rows, cols, by, addrs, link, anchors, nb, imps, strs = load()
    label = {}   # addr -> (module, beleg)
    for s, e, lab, _ in FIXED:
        for a in addrs:
            if s <= a < e:
                label[a] = (lab, 'fest')
    dropped = {}   # verworfene GC-Anker (falsche Zone oder GC-only-Objekt)
    zones = collections.defaultdict(list)
    for a in addrs:
        if a not in label:
            zones[zone_of(a)].append(a)
    for z, seq in zones.items():
        if z in ('d3dx', 'crt', 'unwind', 'staticinit'):
            lab = {'d3dx': 'lib/d3dx9', 'crt': 'msvc/crt', 'unwind': 'msvc/unwind', 'staticinit': 'msvc/staticinit'}[z]
            for a in seq:
                label[a] = (lab, 'zone')
            continue
        if z == 'online':
            for a in seq:
                o = anchors.get(a)
                ok = o and link_group(link.get(os.path.basename(o))) == 'indep'
                label[a] = (o, 'gc-anker') if ok else ('pc/online', 'zone')
            continue
        seeds = {}
        for a in seq:
            o = anchors.get(a)
            if o is None:
                continue
            g = link_group(link.get(os.path.basename(o)))
            if z in ('indep', 'pcplat') and g in ('plat', 'indep'):
                seeds[a] = o
            elif z == 'ealib' and g and g.startswith('lib/'):
                seeds[a] = g
            else:
                dropped[a] = o
        rs = seed_rules(z, seq, by, imps, strs)
        for a, l in rs.items():
            if a not in seeds:
                seeds[a] = l
        res = segment(seq, seeds, nb)
        for a, (l, b) in res.items():
            if b in ('seed', 'seed-ausreisser'):
                b = 'gc-anker' if a in anchors and (anchors[a] == l or z == 'ealib') and a not in rs else 'regel'
            label[a] = (l, b)
    # Thunks auf Importe
    for a in addrs:
        r = by[a]
        if r['kind'] == 'thunk' and (imps.get(a) or re.match(r'^(Ordinal_\d+|[A-Z]\w+)$', r['name'] or '') and int(r['size'] or 0) == 6):
            label[a] = ('win32/thunks', 'thunk')
    print('verworfene GC-Anker:', len(dropped), collections.Counter(
        link_group(link.get(os.path.basename(o))) for o in dropped.values()))
    return finish(rows, cols, by, addrs, link, label, anchors, imps, dry)


def classify_label(lab, link):
    """→ (kind, behandlung, bemerkung)"""
    if lab in LIB:
        k, b, t, _ = LIB[lab]
        return k, b, t
    li = link.get(os.path.basename(lab))
    base = os.path.basename(lab)
    if li is not None and li <= 35:
        if base in PLAT:
            return PLAT[base]
        if base in PLAT_INPUT:
            return 'win32glue', 'ersetzen', 'Eingabe/Lenkrad → WP-41'
        return 'win32glue', 'ersetzen', 'Ecstasy-Plattformrenderer → WP-40'
    if li is not None and 300 <= li <= 309:
        return 'rcmp', 'ersetzen', 'RCMP → WP-16'
    if ENGINE_OBJ.search(lab):
        return 'engine', 'rekonstruieren', 'Engine (indep)'
    return 'game', 'rekonstruieren', 'Spielcode (indep)'


def finish(rows, cols, by, addrs, link, label, anchors, imps, dry):
    stats = collections.Counter()
    beleg_rows = []
    for a in addrs:
        r = by[a]
        lab, bel = label.get(a, (None, None))
        if lab is None:
            stats['ohne'] += 1
            continue
        kind, beh, _ = classify_label(lab, link)
        old_mod = r['module']
        if r['kind'] == 'crt' and r['source'] == 'fid':
            kind = 'crt'          # FID-Treffer bleiben CRT (auch außerhalb der CRT-Zone, z. B. memcpy-Kopien)
        if r['kind'] == 'thunk':
            kind = 'thunk'
        if old_mod and old_mod != lab and old_mod.startswith('speed/') and 'Altmodul' not in r['note']:
            r['note'] = (r['note'] + '; ' if r['note'] else '') + f'Altmodul: {old_mod}'
        r['module'] = lab
        r['kind'] = kind
        if r['status'] in ('unbearbeitet', 'benannt', 'typisiert', 'skizze'):
            if beh == 'entfällt' or lab == 'win32/thunks':
                r['status'] = 'entfällt'
            elif lab in ('msvc/crt', 'lib/d3dx9') or (kind == 'crt' and beh == 'ersetzen'):
                r['status'] = 'ersetzt'
        stats[bel] += 1
        beleg_rows.append((r['addr'], lab, bel))
    if dry:
        print(stats)
        return rows, by, label
    with open(FN, 'w', newline='') as fh:
        w = csv.DictWriter(fh, fieldnames=cols, lineterminator='\r\n')
        w.writeheader()
        w.writerows(rows)
    with open(P('symbols/module_belege.csv'), 'w', newline='') as fh:
        w = csv.writer(fh, lineterminator='\n')
        w.writerow(['addr', 'module', 'beleg'])
        w.writerows(beleg_rows)
    write_modules(rows, link)
    print(stats)
    return rows, by, label


def write_modules(rows, link):
    """modules.csv: start/end = Hauptbereich (längste zusammenhängende Folge), folgen = Anzahl Folgen."""
    srt = sorted(rows, key=lambda r: int(r['addr'], 16))
    agg = collections.OrderedDict()
    prev = None
    for r in srt:
        m = r['module']
        a = int(r['addr'], 16)
        e = a + int(r['size'] or 0)
        g = agg.setdefault(m, dict(n=0, bytes=0, runs=[]))
        if m != prev:
            g['runs'].append([a, e, 0])
        g['runs'][-1][1] = e
        g['runs'][-1][2] += 1
        g['n'] += 1
        g['bytes'] += int(r['size'] or 0)
        prev = m
    bel = collections.defaultdict(collections.Counter)
    for x in csv.DictReader(open(P('symbols/module_belege.csv'))):
        bel[x['module']][x['beleg']] += 1
    with open(P('symbols/modules.csv'), 'w', newline='') as fh:
        w = csv.writer(fh, lineterminator='\n')
        w.writerow(['module', 'start', 'end', 'kind', 'anzahl', 'bytes', 'folgen', 'im_hauptbereich',
                    'gc_link_index', 'behandlung', 'ersatz', 'belege'])
        items = []
        for m, g in agg.items():
            main = max(g['runs'], key=lambda x: x[2])
            items.append((main[0], m, g, main))
        for _, m, g, main in sorted(items):
            kind, beh, txt = classify_label(m, link)
            li = link.get(os.path.basename(m), '')
            belege = ' '.join(f'{k}={v}' for k, v in bel[m].most_common())
            w.writerow([m, f"{main[0]:#x}", f"{main[1]:#x}", kind, g['n'], g['bytes'], len(g['runs']), main[2], li,
                        beh, txt if beh != 'rekonstruieren' else '', belege])


if __name__ == '__main__':
    main()
