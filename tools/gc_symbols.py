#!/usr/bin/env python3
"""Erzeugt symbols/gc_functions.csv und symbols/gc_objects.csv aus refs/gc/Speed.elf.

Quelldatei je Funktion, von genau nach grob:
  zeilen  – DWARF-Zeilentabelle (.line + .debug_srcinfo + .debug_sfnames), nur für die 62 Einheiten mit -g
  objekt  – Objektdatei aus den STT_FILE-Symbolen und der Adresslage der lokalen Symbole (gcc2_compiled.)
Spalte quelle_art sagt, welche der beiden Angaben gilt.

Aufruf: tools/gc_symbols.py
"""
import bisect
import csv
import os
import re
import struct
from collections import Counter, defaultdict

from gc_elf import Elf, ROOT
from gcc2_demangle import demangle

DUMP = os.path.join(ROOT, 'refs', 'gc', 'dwarf_dump.cpp')
OUT_FN = os.path.join(ROOT, 'symbols', 'gc_functions.csv')
OUT_OBJ = os.path.join(ROOT, 'symbols', 'gc_objects.csv')
CODE = ('.init', '.text')
LINKONCE = {'name': '(linkonce)', 'idx': -1, 'funcs': set()}


def objects(e):
    """Objektdateien in Link-Reihenfolge: [(name, start, set(lokale Funktionsadressen))]."""
    objs, cur = [], None
    for s in e.symbols:
        if s.type == 4:
            cur = {'name': s.name, 'idx': len(objs), 'addrs': [], 'funcs': set()}
            objs.append(cur)
        elif cur is not None and s.bind == 0 and e.section_of(s) in CODE and s.type in (0, 2):
            cur['addrs'].append(s.value)
            if s.type == 2:
                cur['funcs'].add(s.value)
    for o in objs:
        o['start'] = min(o['addrs']) if o['addrs'] else None
    return objs


def line_info(e):
    """Adresse -> (Quelldatei, Zeile) aus der DWARF-1-Zeilentabelle."""
    si = e.section_bytes('.debug_srcinfo')
    sf = e.section_bytes('.debug_sfnames')
    ln = e.section_bytes('.line')
    text = open(DUMP, encoding='latin1').read()
    cus = re.findall(r'Compile unit: (\S+).*?GCC Source File Name Offset: (0x[0-9A-F]+)\s+'
                     r'GCC Source Info Offset: (0x[0-9A-F]+)', text, re.S)

    def cstr(o):
        return sf[o:sf.index(b'\0', o)].decode('latin1')
    entries = []   # (addr, file, line)
    units = []
    bounds = sorted(int(x[2], 16) for x in cus) + [len(si)]
    for cu, sfo, sio in cus:
        sfo, sio = int(sfo, 16), int(sio, 16)
        end = next(b for b in bounds if b > sio)
        line_off, _, lo, hi = struct.unpack('>4I', si[sio:sio + 16])
        length, base = struct.unpack('>II', ln[line_off:line_off + 8])
        units.append((cu, lo, hi))
        # Paare (Offset im Zeilenblock, Dateiname) bis 0xffffffff bzw. Blockende
        pairs, p = [], sio + 20
        while p + 8 <= end:
            off, name = struct.unpack('>II', si[p:p + 8])
            if off == 0xffffffff or off >= length or (pairs and off < pairs[-1][0]) or sfo + name >= len(sf):
                break
            pairs.append((off, cstr(sfo + name)))
            p += 8
        k = 0
        for off in range(8, length, 10):
            line, _, delta = struct.unpack('>IHI', ln[line_off + off:line_off + off + 10])
            while k + 1 < len(pairs) and pairs[k + 1][0] <= off:
                k += 1
            if base + delta < hi:   # der letzte Eintrag markiert nur das Ende der Einheit
                entries.append((base + delta, pairs[k][1] if pairs else cu, line))
    entries.sort()
    return entries, units


def path_map(e, units, entries):
    """Dateiname -> bekannter Pfad (aus DWARF und Pfad-Strings im Programm)."""
    known = set(u[0] for u in units) | set(f for _, f, _ in entries)
    known |= set(x.decode('latin1') for x in e.section_bytes('.debug_sfnames').split(b'\0') if x)
    for sec in ('.rodata', '.data', '.sdata', '.sdata2'):
        known |= set(m.decode('latin1') for m in re.findall(rb'[\w./\\:-]+/[\w.-]+\.(?:cpp|c)\b', e.section_bytes(sec)))
    m = defaultdict(set)
    for p in known:
        if p.startswith('D:'):
            continue
        m[p.replace('\\', '/').split('/')[-1].lower()].add(p.replace('\\', '/'))
    return m


def assign_objects(e, objs, units, funcs):
    """Objekt je Funktion. Rückgabe {sym.idx: (obj, 'ja'|'name'|'nein')}.

    Anker eines Objekts: gcc2_compiled. (exakter Start), lokale Code-Symbole, DWARF-Bereich.
    Die STT_FILE-Reihenfolge ist die Link-Reihenfolge (Adressen monoton), daher liegen die
    Kandidaten für eine Funktion zwischen dem letzten Objekt mit Anker <= Adresse und dem nächsten
    Objekt mit exaktem Start > Adresse.
    """
    exact, end = {}, {}
    k = -1
    for sym in e.symbols:
        if sym.type == 4:
            k += 1
        elif sym.name == 'gcc2_compiled.' and k >= 0:
            exact.setdefault(k, sym.value)
        elif k >= 0 and sym.bind == 0 and sym.type == 2 and (
                sym.name.startswith('_GLOBAL_.') or sym.name == '__static_initialization_and_destruction_0'):
            # statische Initialisierung steht am Objektende (geprüft an den DWARF-Bereichen)
            end[k] = max(end.get(k, 0), sym.value + sym.size)
    # DWARF-Einheiten den Objekten zuordnen (Name, Reihenfolge)
    for cu, lo, hi in units:
        base = cu.replace('\\', '/').split('/')[-1].lower()
        for o in objs:
            if o['name'].lower() == base and (o['start'] is None or lo <= o['start'] < hi or o['start'] == lo):
                exact.setdefault(o['idx'], lo)
                end[o['idx']] = max(end.get(o['idx'], 0), hi)
                break
    anchor = []
    for o in objs:
        a = [x for x in [exact.get(o['idx']), o['start']] if x is not None]
        anchor.append(min(a) if a else None)
    anc = sorted((a, i) for i, a in enumerate(anchor) if a is not None)
    akeys = [a for a, _ in anc]
    member = {}
    for o in objs:
        for a in o['funcs']:
            member[a] = o
    res = {}
    for s in funcs:
        if s.bind == 0 and s.value in member:
            res[s.idx] = (member[s.value], 'ja')
            continue
        j = bisect.bisect_right(akeys, s.value) - 1
        if e.section_of(s) == '.init':
            res[s.idx] = (None, 'nein')     # Startcode (crt), ohne FILE-Symbol
            continue
        last = len(objs) - 1
        if last in end and s.value >= end[last]:
            res[s.idx] = (LINKONCE, 'nein')  # .gnu.linkonce.t: Inline-/Template-Funktionen am Textende
            continue
        if j < 0:
            # .text beginnt mit dem ersten Objekt der Link-Reihenfolge
            res[s.idx] = (objs[0], 'ja')
            continue
        a = anc[j][1]
        # gleich große Anker: das spätere Objekt gewinnt (bisect_right)
        cands = [a]
        b = a + 1
        while b < len(objs):
            if exact.get(b) is not None and exact[b] <= s.value:
                cands = [b]
            elif exact.get(b) is not None:
                break
            elif anchor[b] is not None and anchor[b] > s.value:
                cands.append(b)   # Start unbekannt, könnte vor der Adresse liegen
                break
            else:
                cands.append(b)
            b += 1
        cands = [c for c in cands if not (c in end and end[c] <= s.value)] or cands
        # liegt die Funktion hinter dem Ende von A, gehört sie zum nächsten Objekt
        if len(cands) > 1 and cands[0] in end:
            # vor dem Ende von A: sicher A; dahinter: nicht A
            cands = cands[:1] if s.value < end[cands[0]] else cands[1:]
        if len(cands) == 1:
            res[s.idx] = (objs[cands[0]], 'ja')
            continue
        low = s.name.lower()
        hit = [c for c in cands if objs[c]['name'].rsplit('.', 1)[0].lower() in low
               and len(objs[c]['name']) > 4]
        if len(hit) == 1:
            res[s.idx] = (objs[hit[0]], 'name')
        else:
            res[s.idx] = (objs[cands[0]], 'nein')
    return res


SIG_PRE = re.compile(r'^((?:static |virtual |inline )*)')


def parse_sig(line):
    """'static int K::f(const char * a /* r3 */, int b) const {}' -> dict oder None."""
    line = re.sub(r'/\*.*?\*/', '', line).strip()
    line = re.sub(r'\s*(\{\}|\{|;)$', '', line)
    pre = SIG_PRE.match(line).group(1)
    body = line[len(pre):]
    k = body.find('operator()')
    k = body.find('(', k + len('operator()') if k >= 0 else 0)
    if k <= 0:
        return None
    head, depth, j = body[:k].rstrip(), 0, k
    for j in range(k, len(body)):
        depth += {'(': 1, ')': -1}.get(body[j], 0)
        if depth == 0:
            break
    args_s, post = body[k + 1:j], body[j + 1:]
    m = re.search(r'((?:[\w<>,]+::)*(?:operator\s*.+|~?[\w<>,]+))$', head) if 'operator' in head else \
        re.search(r'((?:\w+(?:<[^()]*?>)?::)*~?\w+)$', head)
    if not m:
        return None
    name = m.group(1).strip()
    ret = head[:m.start()].strip()
    args, depth, cur = [], 0, ''
    for ch in args_s:
        if ch == ',' and depth == 0:
            args.append(cur.strip())
            cur = ''
            continue
        depth += {'(': 1, ')': -1, '<': 1, '>': -1}.get(ch, 0)
        cur += ch
    if cur.strip():
        args.append(cur.strip())
    args = [re.sub(r'\b(struct|class|union|enum) ', '', a) for a in args if a != 'void']
    return {'static': 'static' in pre, 'virtual': 'virtual' in pre or 'override' in post,
            'ret': re.sub(r'\b(struct|class|union|enum) ', '', ret), 'name': name, 'args': args,
            'const': post.strip().startswith('const')}


def dwarf_signatures():
    """(1) Adresse -> Signatur aus '// Range:'-Funktionen, (2) (Klasse, Methode, n) -> Deklaration."""
    by_addr, by_decl = {}, {}
    lines = open(DUMP, encoding='latin1').read().split('\n')
    cls, depth = None, 0
    for i, ln in enumerate(lines):
        if not ln.startswith(' ') and ln.startswith('// Range: '):
            j = i + 1
            this = False
            while j < len(lines) and lines[j].startswith('//'):
                this |= lines[j].startswith('// this:')
                j += 1
            sig = parse_sig(lines[j])
            if sig:
                sig['this'] = this
                by_addr[int(ln.split()[2], 16)] = sig
            continue
        m = re.match(r'^(struct|class|union) ([^(){};]+?)(?: : .*)? \{$', ln)
        if m:
            cls, depth = m.group(2).strip(), 1
            continue
        if cls:
            if ln.startswith('};'):
                cls = None
                continue
            if ln.startswith('    ') and not ln.startswith('     ') and '(' in ln and '// offset' not in ln \
                    and not ln.strip().startswith('//'):
                sig = parse_sig(ln.strip())
                if sig:
                    key = (cls, sig['name'].split('::')[-1], len(sig['args']))
                    if key in by_decl and by_decl[key]['ret'] != sig['ret']:
                        by_decl[key]['ret'] = '?'
                    else:
                        by_decl.setdefault(key, sig)
    return by_addr, by_decl


def main():
    e = Elf()
    objs = objects(e)
    entries, units = line_info(e)
    eaddrs = [x[0] for x in entries]
    paths = path_map(e, units, entries)
    sig_addr, sig_decl = dwarf_signatures()
    assign = assign_objects(e, objs, units, funcs := sorted(
        (s for s in e.symbols if s.type == 2 and e.section_of(s) in CODE), key=lambda s: s.value))
    rows, per_obj = [], defaultdict(list)
    for s in funcs:
        own, sicher = assign[s.idx]
        oname = own['name'] if own else ''
        if own and own['idx'] >= 0:
            per_obj[own['idx']].append(s)
        cand = paths.get(oname.lower(), set()) if oname and not oname.startswith('(') else set()
        if not own:
            oname = '(init)'
        if oname.startswith('BB_Objs') or oname in ('Global_.cpp', 'NFSU_Streams.cpp'):
            cand = cand or {('../indep/src/generated/' if oname.startswith('BB_') else '../indep/src/eaxsound/') + oname}
        opath = sorted(cand, key=len)[0] if len(cand) >= 1 else oname
        # Zeilen
        lo = bisect.bisect_left(eaddrs, s.value)
        hi = bisect.bisect_left(eaddrs, s.value + max(s.size, 1))
        src, lines, art = opath, '', 'objekt'
        if hi > lo:
            fl = [entries[k] for k in range(lo, hi)]
            cpp = [f for _, f, _ in fl if not f.endswith(('.hpp', '.h'))]
            src = cpp[0] if cpp else Counter(f for _, f, _ in fl).most_common(1)[0][0]
            ls = [l for _, f, l in fl if f == src and l]
            lines = f'{min(ls)}-{max(ls)}' if ls else ''
            art = 'zeilen'
        d = demangle(s.name)
        decl = sig_decl.get((d.klass, d.method, len([a for a in d.args if a != 'void']))) if d and d.klass else None
        sig, sq = sig_addr.get(s.value), 'dwarf-bereich'
        if sig is None:
            sig, sq = decl, 'dwarf-deklaration'
        if sig is None:
            sq = ''
        # static nur aus der Klassendeklaration; 'this' fehlt im Bereich auch, wenn es wegoptimiert ist
        static = ('ja' if decl['static'] else 'nein') if decl else ('nein' if sig and sig.get('this') else '')
        if sig and decl and not sig['ret']:
            sig = dict(sig, ret=decl['ret'])
        rows.append({
            'gc_addr': f'0x{s.value:08X}', 'size': s.size, 'mangled': s.name,
            'demangled': d.full if d else s.name,
            'argumente': '; '.join(a for a in d.args if a != 'void') if d and not d.special else '',
            'klasse': d.klass if d else '', 'methode': d.method if d else s.name,
            'rueckgabe': sig['ret'] if sig and sig['ret'] != '?' else '',
            'statisch': static,
            'virtuell': ('ja' if decl['virtual'] else 'nein') if decl else '',
            'parameter': '; '.join(sig['args']) if sig else '',
            'signatur_quelle': sq,
            'quelldatei': src, 'zeilen': lines, 'quelle_art': art, 'objekt': opath,
            'objekt_index': own['idx'] if own else -1,
            'objekt_sicher': sicher,
            'bindung': {0: 'lokal', 1: 'global', 2: 'schwach'}.get(s.bind, s.bind),
        })
    os.makedirs(os.path.dirname(OUT_FN), exist_ok=True)
    with open(OUT_FN, 'w', newline='') as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    with open(OUT_OBJ, 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(['link_index', 'objekt', 'pfad', 'start', 'ende', 'funktionen', 'bytes'])
        for o in objs:
            fs = per_obj.get(o['idx'], [])
            cand = paths.get(o['name'].lower(), set())
            w.writerow([o['idx'], o['name'], sorted(cand, key=len)[0] if cand else '',
                        f"0x{min(x.value for x in fs):08X}" if fs else '',
                        f"0x{max(x.value + x.size for x in fs):08X}" if fs else '',
                        len(fs), sum(x.size for x in fs)])
    n = len(rows)
    print(f'{n} Funktionen; in einem Objekt {sum(1 for r in rows if not r["objekt"].startswith("("))}, '
          f'davon sicher {sum(1 for r in rows if r["objekt_sicher"] != "nein")}, '
          f'mit Zeileninfo {sum(1 for r in rows if r["quelle_art"] == "zeilen")}, '
          f'demangled {sum(1 for r in rows if r["demangled"] != r["mangled"])}, '
          f'Signatur aus DWARF {sum(1 for r in rows if r["signatur_quelle"])} '
          f'(Bereich {sum(1 for r in rows if r["signatur_quelle"] == "dwarf-bereich")})')
    # Gegenprobe: DWARF-Einheiten gegen Objektzuordnung
    bad = 0
    for cu, lo, hi in units:
        base = cu.split('/')[-1].lower()
        for r in rows:
            a = int(r['gc_addr'], 16)
            if lo <= a < hi and r['objekt'].split('/')[-1].lower() != base:
                bad += 1
    print(f'Gegenprobe DWARF-Bereiche: {bad} Funktionen mit abweichendem Objekt')


if __name__ == '__main__':
    main()
