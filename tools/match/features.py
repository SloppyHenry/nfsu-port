#!/usr/bin/env python3
"""Merkmale je Funktion für den Abgleich PC <-> GC (WP-08).

PC: decompiled/full/out3 (strrefs, callgraph, consts, functions)
GC: decompiled/gc/out (all_functions.c wird textuell zerlegt) + symbols/gc_functions.csv
Ergebnis (pickle): {'pc': {addr: Fn}, 'gc': {addr: Fn}}, Adressen als int.
"""
import csv, os, pickle, re, sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PC = os.path.join(ROOT, 'decompiled/full/out3')
GC = os.path.join(ROOT, 'decompiled/gc/out')
CACHE = os.environ.get('MATCH_CACHE', os.path.join(ROOT, 'refs/match_cache.pkl'))

KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else', 'do', 'case'}


class Fn:
    __slots__ = ('addr', 'name', 'size', 'strings', 'callees', 'consts', 'nparams', 'extra', 'globs')

    def __init__(self, addr, name, size):
        self.addr, self.name, self.size = addr, name, size
        self.strings = set()   # normalisierte Strings
        self.callees = []      # Adressen in Aufrufreihenfolge (mit Wiederholungen)
        self.consts = set()    # ungewöhnliche Konstanten (int)
        self.nparams = 0
        self.extra = {}
        self.globs = set()     # Adressen globaler Variablen (PC: >= 0x6c6000; GC: aus data.tsv-Namen)


def norm_str(s):
    s = s.replace('\\\\', '\\').replace('\\"', '"')
    s = s.replace('\\n', '\n').replace('\\t', '\t')
    return s


def load_pc():
    fns = {}
    for line in open(f'{PC}/functions.tsv'):
        p = line.rstrip('\n').split('\t')
        a = int(p[0], 16)
        f = Fn(a, p[1], int(p[2]))
        sig = p[5] if len(p) > 5 else ''
        m = re.search(r'\((.*)\)\s*$', sig)
        f.nparams = 0 if not m or m.group(1).strip() in ('', 'void') else m.group(1).count(',') + 1
        f.extra['conv'] = p[4] if len(p) > 4 else ''
        fns[a] = f
    for line in open(f'{PC}/strrefs.tsv'):
        p = line.rstrip('\n').split('\t')
        a = int(p[0], 16)
        if a in fns and len(p) > 2:
            fns[a].strings.add(norm_str(p[2]))
    cg = []
    for line in open(f'{PC}/callgraph.tsv'):
        p = line.split()
        try:
            cg.append((int(p[0], 16), int(p[1], 16), int(p[2], 16)))
        except ValueError:   # EXTERNAL:...
            pass
    cg.sort(key=lambda t: (t[0], t[2]))
    for a, b, site in cg:
        if a in fns and b in fns:
            fns[a].callees.append(b)
    for line in open(f'{PC}/datarefs.tsv'):
        p = line.split()
        a, d = int(p[0], 16), int(p[1], 16)
        if a in fns and d >= 0x6c6000:
            fns[a].globs.add(d)
    for line in open(f'{PC}/consts.tsv'):
        p = line.split()
        a = int(p[0], 16)
        v = int(p[1], 16)
        if a in fns:
            fns[a].consts.add(v)
    return fns


HDR = re.compile(r'^/\* ==== ([0-9a-f]{8}) (\S+) ==== \*/$', re.M)
STR = re.compile(r'"((?:[^"\\\n]|\\.)*)"')
CALL = re.compile(r'(?<![\w.>])([A-Za-z_~][\w:~<>]*)\s*\(')
HEX = re.compile(r'\b0x([0-9a-f]{5,8})\b')


def load_gc():
    fns, byname = {}, {}
    # Symboltabellen-Größe ist verlässlicher (Ghidra bricht bei Paired-Single-Befehlen ab)
    symsize = {}
    for r in csv.DictReader(open(os.path.join(ROOT, 'symbols/gc_functions.csv'))):
        symsize[int(r['gc_addr'], 16)] = int(r['size'] or 0)
    for line in open(f'{GC}/functions.tsv'):
        p = line.rstrip('\n').split('\t')
        a = int(p[0], 16)
        f = Fn(a, p[1], max(int(p[2]), symsize.get(a, 0)))
        sig = p[5] if len(p) > 5 else ''
        m = re.search(r'\((.*)\)\s*$', sig)
        f.nparams = 0 if not m or m.group(1).strip() in ('', 'void') else m.group(1).count(',') + 1
        fns[a] = f
        byname.setdefault(p[1], []).append(a)
    gnames = {}
    for line in open(f'{GC}/data.tsv'):
        p = line.rstrip('\n').split('\t')
        try:
            a = int(p[0], 16)
        except ValueError:
            continue
        if a >= 0x80000000 and not p[1].startswith(('s_', 'PTR_s_', '_vt')) and 'switch' not in p[1]:
            gnames[p[1]] = a
    ident = re.compile(r'[A-Za-z_][A-Za-z0-9_]*')
    text = open(f'{GC}/all_functions.c', errors='replace').read()
    hs = list(HDR.finditer(text))
    for i, h in enumerate(hs):
        a = int(h.group(1), 16)
        body = text[h.end(): hs[i + 1].start() if i + 1 < len(hs) else len(text)]
        f = fns.get(a)
        if f is None:
            continue
        for s in STR.findall(body):
            f.strings.add(norm_str(s))
        # Kopf (Signatur + Kommentar) nicht als Aufruf zählen
        k = body.find('\n{')
        code = body[k:] if k >= 0 else body
        for m in CALL.finditer(code):
            n = m.group(1)
            if n in KEYWORDS:
                continue
            t = byname.get(n)
            if t and len(t) == 1 and t[0] != a:
                f.callees.append(t[0])
        for m in HEX.finditer(code):
            f.consts.add(int(m.group(1), 16))
        for m in ident.finditer(code):
            ga = gnames.get(m.group(0))
            if ga:
                f.globs.add(ga)
    add_bl_callees(fns, gnames)
    return fns


def add_bl_callees(fns, gnames=None):
    """Aufrufe direkt aus den PPC-Befehlen (bl/b), unabhängig von Ghidras Abbruch bei Paired-Single."""
    import struct
    sys.path.insert(0, os.path.join(ROOT, 'tools'))
    from gc_elf import Elf
    e = Elf()
    sec = e.by_name['.text']
    code = e.data[sec.offset:sec.offset + sec.size]
    secs = [x for x in e.sections if x.addr and x.type == 1]
    inv = {v: k for k, v in (gnames or {}).items()}

    def cstr(addr):
        for x in secs:
            if x.addr <= addr < x.addr + x.size and x.name in ('.rodata', '.data', '.sdata', '.sdata2'):
                o = x.offset + addr - x.addr
                end = e.data.find(b'\0', o, o + 400)
                if end - o >= 4:
                    b = e.data[o:end]
                    if all(32 <= c < 127 or c in (9, 10) for c in b):
                        return b.decode('latin1')
                return None
        return None
    for a, f in fns.items():
        o = a - sec.addr
        if o < 0 or o + f.size > len(code):
            continue
        f.callees = []
        n = f.size // 4
        hi = {}
        for i, w in enumerate(struct.unpack('>%dI' % n, code[o:o + n * 4])):
            op = w >> 26
            if op == 15 or op == 14 or 32 <= op <= 47:
                rt, ra, d = (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
                if d & 0x8000:
                    d -= 0x10000
                addr = None
                if op == 15 and ra == 0:
                    hi[rt] = d << 16
                elif ra in hi and op != 15:
                    addr = (hi[ra] + d) & 0xFFFFFFFF
                    if op == 14:
                        hi.pop(rt, None) if rt != ra else None
                elif ra == 13:
                    addr = 0x803C8640 + d
                elif ra == 2:
                    addr = 0x803D8640 + d
                if addr:
                    t = cstr(addr)
                    if t:
                        f.strings.add(norm_str(t.replace('\n', '\\n')) if False else t)
                    if addr in inv:
                        f.globs.add(addr)
            if w >> 26 != 18 or w & 2:
                continue
            li = w & 0x03FFFFFC
            if li & 0x02000000:
                li -= 0x04000000
            t = a + i * 4 + li
            if (w & 1 or not (a <= t < a + f.size)) and t in fns and t != a:
                f.callees.append(t)


def load(force=False):
    if not force and os.path.exists(CACHE):
        return pickle.load(open(CACHE, 'rb'))
    d = {'pc': load_pc(), 'gc': load_gc()}
    pickle.dump(d, open(CACHE, 'wb'))
    return d


if __name__ == '__main__':
    import features   # damit pickle die Klasse als features.Fn findet
    d = features.load(True)
    for k in ('pc', 'gc'):
        fs = d[k].values()
        print(k, len(d[k]), 'mit Strings', sum(1 for f in fs if f.strings),
              'mit Aufrufen', sum(1 for f in fs if f.callees))
