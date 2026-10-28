#!/usr/bin/env python3
"""Vtables der PC-Speed.exe finden und benennen (WP-04, Schritt 1). Kein RTTI, daher:

1. Kandidaten: Folgen von Dwords in .rdata/.data, die auf Funktionsanfänge zeigen.
2. Grenzen: jede vom Code referenzierte Adresse innerhalb einer Folge beginnt eine eigene Vtable.
3. Beleg: `mov dword ptr [reg(+d)], vt` (auch über `mov reg, vt`) = Konstruktor/Destruktor schreibt
   den Vptr (beleg=ctor). Nur gelesen/geladen = beleg=ref. Zugriff mit Index [r*4+tab] = Tabelle (kein Vtable).
4. Klasse: Abgleich mit den GC-Vtables `_vt.<Klasse>` (Slots über symbols/pc_gc_map.csv), sonst
   Mehrheit aus Destruktor-Slot und Namen der schreibenden Konstruktoren.

  tools/emu/.venv/bin/python tools/types/vtables.py      -> symbols/vtables.csv (+ Übersicht)
Spalten: vtable_addr,klasse,slot,ziel,ziel_name,gc_ziel,vptr_offset,beleg
  klasse: `Klasse` oder `Klasse/Basis` (Zweit-Vtable für die Basis bei Mehrfachvererbung); `?` = unbekannt
  gc_ziel: GC-Funktion im gleichen Slot der zugeordneten GC-Vtable (Namensvorschlag)
  vptr_offset: Offset im Objekt, an das die Konstruktoren schreiben (leer = nur referenziert)
"""
import collections
import csv
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(HERE))
from pe import Pe, ROOT
import disasm
from gc_elf import Elf
from gcc2_demangle import demangle

OUT = os.path.join(ROOT, 'symbols', 'vtables.csv')
HEX = re.compile(r'0x[0-9a-f]+')
STORE = re.compile(r'^dword ptr \[(\w+)(?: \+ (0x[0-9a-f]+|\d+))?\], (0x[0-9a-f]+|e\w\w)$')
SCALED = re.compile(r'\*\s*4 \+ (0x[0-9a-f]+)\]')


def load_functions():
    rows = {}
    with open(os.path.join(ROOT, 'symbols', 'functions.csv'), newline='') as fh:
        for r in csv.DictReader(fh):
            rows[int(r['addr'], 16)] = r
    return rows


def load_map():
    m = {}
    with open(os.path.join(ROOT, 'symbols', 'pc_gc_map.csv'), newline='') as fh:
        for r in csv.DictReader(fh):
            m[int(r['pc_addr'], 16)] = (int(r['gc_addr'], 16), r['confidence'])
    return m


def gc_vtables():
    e = Elf()
    names = {s.value: s.name for s in e.symbols if s.type == 2}
    out = {}
    for s in e.symbols:
        if not (s.name.startswith('_vt.') or s.name.startswith('_vt$')):
            continue
        sec = e.sections[s.shndx]
        o = sec.offset + s.value - sec.addr
        w = struct.unpack('>%dI' % (s.size // 4), e.data[o:o + s.size])
        slots = [w[k] for k in range(3, len(w), 2)]
        while slots and slots[-1] == 0:
            slots.pop()
        parts = []
        for p in s.name[4:].replace('$', '.').split('.'):
            d = demangle('__' + p)
            parts.append(d.klass if d and d.klass else p)
        out[s.name] = ('/'.join(parts), slots)
    return out, names


def gc_label(a, names):
    n = names.get(a)
    if not n:
        return ''
    d = demangle(n)
    return d.qualname if d else n


def scan_refs(code, pe):
    """Alle Code-Referenzen auf .rdata/.data: {ziel: [(funktion, befehl, art, basis, disp)]}."""
    refs = collections.defaultdict(list)
    for f, ins in code.items():
        regimm = {}
        for a, mn, ops in ins:
            for m in SCALED.finditer(ops):
                refs[int(m.group(1), 16)].append((f, a, 'tab', None, None))
            st = STORE.match(ops) if mn == 'mov' else None
            if st:
                base, disp, src = st.groups()
                disp = int(disp, 0) if disp else 0
                val = int(src, 16) if src.startswith('0x') else regimm.get(src)
                if val is not None:
                    refs[val].append((f, a, 'store', base, disp))
                    continue
            if mn == 'mov':
                dst, _, src = ops.partition(', ')
                if re.fullmatch(r'e\w\w', dst):
                    regimm[dst] = int(src, 16) if re.fullmatch(r'0x[0-9a-f]+', src) else None
            elif mn in ('call', 'jmp') or mn.startswith('j'):
                regimm.clear()
            for h in HEX.findall(ops):
                v = int(h, 16)
                if pe.section_of(v) in ('.rdata', '.data') and not SCALED.search(ops):
                    refs[v].append((f, a, 'ref', None, None))
    return refs


def slot_targets(pe, funcs):
    """Gültige Slot-Ziele: bekannte Funktionsanfänge und .text-Adressen, die in keiner bekannten
    Funktion liegen (virtuelle Methoden, die nur über die Vtable erreichbar sind, fehlen in Ghidra)."""
    import bisect
    starts = sorted(funcs)
    ends = [a + max(1, int(funcs[a]['size'] or 1)) for a in starts]

    def ok(v):
        if v in funcs:
            return True
        if pe.section_of(v) != '.text':
            return False
        k = bisect.bisect_right(starts, v) - 1
        return k < 0 or v >= ends[k]
    return ok


def find_vtables(pe, funcs, refs):
    out = []
    ok = slot_targets(pe, funcs)
    for sec in ('.rdata', '.data'):
        base, words = pe.words(sec)
        i, n = 0, len(words)
        while i < n:
            if not ok(words[i]):
                i += 1
                continue
            j = i
            while j < n and ok(words[j]):
                j += 1
            run = [base + k * 4 for k in range(i, j)]
            cuts = [k for k, a in enumerate(run) if a in refs or k == 0]
            for c, k in enumerate(cuts):
                end = cuts[c + 1] if c + 1 < len(cuts) else len(run)
                va = run[k]
                kinds = {r[2] for r in refs.get(va, ())}
                if 'tab' in kinds and 'store' not in kinds:
                    continue
                slots = list(words[i + k:i + end])
                beleg = 'ctor' if 'store' in kinds else 'ref' if kinds else 'unreferenziert'
                if beleg == 'unreferenziert' or (beleg == 'ref' and len(slots) < 2):
                    continue
                out.append((va, slots, beleg))
            i = j
    return out


CTOR = re.compile(r'(?:(.*)::)?(\w+)::(~?)(\w+)')


def writer_classes(refs, funcs):
    """Konstruktor `X::X` schreibt zuletzt seine eigene Vtable (vorher die der Basen, eingebettet),
    Destruktor `X::~X` zuerst die eigene. Ergebnis {vtable: Counter(klasse)} je Objektoffset."""
    stores = collections.defaultdict(list)
    for va, lst in refs.items():
        for f, a, kind, base, disp in lst:
            if kind == 'store':
                stores[f].append((a, disp, va))
    out = collections.defaultdict(collections.Counter)
    for f, lst in stores.items():
        m = CTOR.fullmatch(funcs[f]['name']) if f in funcs else None
        if not m or m.group(2) != m.group(4):
            continue
        klass = (m.group(1) + '::' if m.group(1) else '') + m.group(2)
        lst.sort()
        per = collections.defaultdict(list)
        for a, disp, va in lst:
            per[disp].append(va)
        for disp, vas in per.items():
            va = vas[0] if m.group(3) else vas[-1]
            out[va][klass if disp == 0 else '%s@0x%x' % (klass, disp)] += 1
    return out


def dtor_classes(vts, funcs, code, pe):
    """Skalarer Löschdestruktor: ruft `X::~X` auf und testet dann [esp+8]&1 (MSVC ??_G)."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = {}
    for va, slots, _ in vts:
        for p in slots[:3]:
            ins = code.get(p) or [(i.address, i.mnemonic, i.op_str) for i in md.disasm(pe.read(p, 64), p)]
            ins = ins[:14]
            if not any(mn == 'test' and 'esp + 8], 1' in o for _, mn, o in ins):
                continue
            for _, mn, o in ins:
                if mn == 'call' and o.startswith('0x'):
                    m = CTOR.fullmatch(funcs.get(int(o, 16), {}).get('name', ''))
                    if m and m.group(3) and m.group(2) == m.group(4):
                        out[va] = (m.group(1) + '::' if m.group(1) else '') + m.group(2)
                    break
            break
    return out


def deleting_dtors(vts, code, pe):
    """Slot-Funktionen mit dem Muster des MSVC-Löschdestruktors (??_G): test byte [esp+8], 1."""
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = set()
    for p in {p for _, sl, _ in vts for p in sl}:
        ins = code.get(p) or [(i.address, i.mnemonic, i.op_str) for i in md.disasm(pe.read(p, 64), p)]
        if any(mn == 'test' and 'esp + 8], 1' in o for _, mn, o in ins[:14]):
            out.add(p)
    return out


def align_slots(pcs, gcs, M, sdd, gc_dtor):
    """Slot-Ausrichtung PC -> GC: Anker (gleiche Funktion laut M, Löschdestruktor <-> GC-Destruktor),
    längste monotone Ankerkette, dazwischen 1:1 nur bei gleich großen Lücken. {pc_index: gc_index}"""
    anchors = [(i, j) for i, p in enumerate(pcs) for j, g in enumerate(gcs)
               if M.get(p) == g or (p in sdd and g in gc_dtor)]
    # längste Kette (i und j streng steigend)
    best = []
    for k, (i, j) in enumerate(anchors):
        prev = max((c for c in best if c[-1][0] < i and c[-1][1] < j), key=len, default=[])
        best.append(prev + [(i, j)])
    chain = max(best, key=len, default=[])
    out = dict(chain)
    pts = [(-1, -1)] + chain + [(len(pcs), len(gcs))]
    for (i0, j0), (i1, j1) in zip(pts, pts[1:]):
        if i1 - i0 == j1 - j0:
            for k in range(1, i1 - i0):
                out[i0 + k] = j0 + k
    return out


def match_gc(vts, pcmap, gcvt, by_fn, sdd=frozenset(), gc_dtor=frozenset()):
    """Iterativ: PC-Vtable <-> GC-Vtable über zugeordnete Slot-Funktionen (Stimmen gewichtet mit
    1/Anzahl GC-Vtables, die die Funktion enthalten), 1:1, keine Widersprüche. Jede neue Zuordnung
    überträgt ihre Slots als Funktionspaare (für die nächsten Runden)."""
    shared = collections.Counter(p for _, sl, _ in vts for p in set(sl))
    M = {p: g for p, (g, _) in pcmap.items() if shared[p] <= 1}
    R = {g: p for p, g in M.items()}
    assign, used = {}, set()
    while True:
        best = []
        for va, slots, _ in vts:
            if va in assign:
                continue
            votes = collections.defaultdict(float)
            for i, p in enumerate(slots):
                g = M.get(p)
                if g is None:
                    continue
                hits = by_fn.get(g, ())
                for gv, j in hits:
                    if gv not in used:
                        votes[(gv, j - i)] += 1.0 / len(hits)
            cands = []
            for (gv, delta), w in votes.items():
                gsl = gcvt[gv][1]
                bad = sum(1 for i, p in enumerate(slots) if p in M and 0 <= i + delta < len(gsl) and M[p] != gsl[i + delta])
                lendiff = abs(len(gsl) - delta - len(slots))
                cands.append((w - 2 * bad - 0.3 * abs(delta) - 0.05 * lendiff, w, bad, gv, delta, lendiff))
            cands.sort(reverse=True)
            if not cands:
                continue
            sc, w, bad, gv, delta, lendiff = cands[0]
            if bad or sc <= 0 or (len(cands) > 1 and cands[1][0] >= sc - 0.25):
                continue
            if w < 0.5 and lendiff:
                continue
            best.append((sc, va, gv, delta))
        best.sort(reverse=True)
        new = 0
        slots_of = {va: sl for va, sl, _ in vts}
        for sc, va, gv, delta in best:
            if gv in used or va in assign:
                continue
            assign[va] = (gv, delta, sc)
            used.add(gv)
            new += 1
            gsl = gcvt[gv][1]
            for i, j in align_slots(slots_of[va], gsl, M, sdd, gc_dtor).items():
                p = slots_of[va][i]
                if p not in M and gsl[j] not in R and not (p in sdd) != (gsl[j] in gc_dtor):
                    if shared[p] <= 1:
                        M[p] = gsl[j]
                        R[gsl[j]] = p
        if not new:
            return assign, M


def main():
    pe = Pe()
    funcs = load_functions()
    code = disasm.load()
    pcmap = load_map()
    gcvt, gcnames = gc_vtables()
    by_fn = collections.defaultdict(list)
    for gv, (_, sl) in gcvt.items():
        for j, g in enumerate(sl):
            by_fn[g].append((gv, j))
    refs = scan_refs(code, pe)
    vts = find_vtables(pe, funcs, refs)

    sdd = deleting_dtors(vts, code, pe)
    gc_dtor = {g for _, sl in gcvt.values() for g in sl if '_$_' in gcnames.get(g, '') or '_._' in gcnames.get(g, '')}
    assign, M = match_gc(vts, pcmap, gcvt, by_fn, sdd, gc_dtor)
    writers = writer_classes(refs, funcs)
    dtors = dtor_classes(vts, funcs, code, pe)
    shared_dtor = collections.Counter(dtors.values())
    shared = collections.Counter(p for _, sl, _ in vts for p in set(sl))
    used_gv = {gv for gv, _, _ in assign.values()}
    gc_by_class = {}
    for gv, (k, _) in gcvt.items():
        gc_by_class.setdefault(k, gv)
    rows, summary = [], collections.Counter()
    used = collections.Counter()
    for va, slots, beleg in vts:
        best, klasse, how = None, '?', ''
        if va in assign:
            gv, delta, n = assign[va]
            best, klasse, how = (gv, delta), gcvt[gv][0], 'gc-vtable(%.1f)' % n
        if best is None:
            w = writers.get(va)
            if w:
                (k, n), = w.most_common(1)
                if len(w) == 1 or w.most_common(2)[1][1] < n:
                    klasse, how = k, 'ctor-name(%d)' % n
            if klasse == '?' and va in dtors and shared_dtor[dtors[va]] == 1:
                klasse, how = dtors[va], 'dtor-aufruf'
            if klasse == '?':
                v = collections.Counter()
                for p in slots:
                    m = re.fullmatch(r'(.+)::~?\w+', funcs[p]['name'] if p in funcs and shared[p] == 1 else '')
                    if m:
                        v[m.group(1)] += 1
                if v:
                    (k, n), = v.most_common(1)
                    if n >= 2 and (len(v) == 1 or v.most_common(2)[1][1] * 2 <= n):
                        klasse, how = k + '?', 'slot-namen(%d)' % n
            gv = gc_by_class.get(klasse)
            if gv and gv not in used_gv and len(gcvt[gv][1]) == len(slots):
                best = (gv, 0)
                used_gv.add(gv)
                how += '+gc-laenge'
        offs = sorted({r[4] for r in refs.get(va, ()) if r[2] == 'store'})
        summary[how.split('(')[0] or 'unbekannt'] += 1
        used[klasse] += 1
        al = align_slots(slots, gcvt[best[0]][1], M, sdd, gc_dtor) if best else {}
        for i, p in enumerate(slots):
            gz = gc_label(gcvt[best[0]][1][al[i]], gcnames) if i in al else ''
            rows.append(dict(vtable_addr='0x%06x' % va, klasse=klasse, slot=i, ziel='0x%06x' % p,
                             ziel_name=(funcs[p]['name'] or 'FUN_%08x' % p) if p in funcs else 'neu:FUN_%08x' % p, gc_ziel=gz,
                             vptr_offset=' '.join('0x%x' % o for o in offs), beleg=beleg + ('/' + how if how else '')))
    with open(OUT, 'w', newline='') as fh:
        w = csv.DictWriter(fh, fieldnames=list(rows[0].keys()))
        w.writeheader()
        w.writerows(rows)
    nb = collections.Counter(b for _, _, b in vts)
    print('Vtables: %d (ctor %d, ref %d), Slots %d' % (len(vts), nb['ctor'], nb['ref'], len(rows)))
    print('Klasse über:', dict(summary))
    print('benannt: %d Vtables, %d verschiedene Klassen; mehrfach vergeben: %s' % (
        len(vts) - summary['unbekannt'], len(used) - ('?' in used),
        [k for k, n in used.items() if n > 1 and k != '?'][:20]))


if __name__ == '__main__':
    main()
