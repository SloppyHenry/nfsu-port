#!/usr/bin/env python3
"""Erzeugt symbols/gc_types.h aus refs/gc/dwarf_dump.cpp (dtk dwarf dump der GC-Speed.elf).

- pro Typname wird die vollständigste Definition aller Übersetzungseinheiten genommen
- Methoden fallen weg; virtuelle Methoden bleiben als Kommentar (Deklarationsreihenfolge)
- Template-Instanzen werden zu gültigen Bezeichnern (bTNode<Mover> -> bTNode_Mover_)
- Ausgabe: Enums, Vorwärtsdeklarationen, Typedefs, Strukturen in Abhängigkeitsreihenfolge

Aufruf: tools/gc_types.py [--stats]
"""
import os
import re
import sys
from collections import defaultdict

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DUMP = os.path.join(ROOT, 'refs', 'gc', 'dwarf_dump.cpp')
OUT = os.path.join(ROOT, 'symbols', 'gc_types.h')

HEAD_RE = re.compile(r'^(\s*)(struct|class|union|enum)\s*(.*?)\s*\{\s*$')
EMPTY_RE = re.compile(r'^(\s*)(struct|class|union) ([^(){};]+?)\s*\{\};\s*$')
TYPEDEF_RE = re.compile(r'^typedef .*;$')
KEYWORDS = {'struct', 'class', 'union', 'enum', 'const', 'volatile', 'unsigned', 'signed', 'char',
            'short', 'int', 'long', 'float', 'double', 'void', 'bool', 'static', 'public',
            'private', 'protected', 'virtual', 'wchar_t'}


def sanitize(text):
    """Template-Argumente und Punkte in Bezeichner umwandeln."""
    def repl(m):
        inner = m.group(0)
        return re.sub(r'[^A-Za-z0-9_]', '_', inner)
    # wiederholt innerste <...> ersetzen
    prev = None
    while prev != text:
        prev = text
        text = re.sub(r'\b\w+<[^<>]*>', repl, text)
    text = text.replace('_vptr.', '_vptr_')
    return text


class Block:
    def __init__(self, kind, name, bases, size):
        self.kind, self.name, self.bases, self.size = kind, name, bases, size
        self.lines = []          # Ausgabezeilen (relativ eingerückt)
        self.virtuals = []
        self.members = 0
        self.hoisted = []        # innere benannte Typen
        self.outer = None


def parse_empty(me, size):
    name, bases = me.group(3), ''
    if ' : ' in name:
        name, bases = name.split(' : ', 1)
    return Block(me.group(2), sanitize(name.strip()), sanitize(bases.strip()), size)


def parse_block(lines, i, kind, rest, size):
    """lines[i] ist die Kopfzeile. Gibt (Block, nächster Index) zurück."""
    name, bases = rest, ''
    if ' : ' in rest:
        name, bases = rest.split(' : ', 1)
    blk = Block(kind, sanitize(name.strip()), sanitize(bases.strip()), size)
    i += 1
    pending_size = None
    while i < len(lines):
        ln = lines[i]
        s = ln.strip()
        if s.startswith('};') or (s.startswith('}') and '// offset' in s) or s == '}':
            return blk, i + 1, s
        me = EMPTY_RE.match(ln)
        if me:
            sub = parse_empty(me, pending_size)
            pending_size = None
            sub.outer = blk.name
            blk.hoisted.append(sub)
            i += 1
            continue
        m = HEAD_RE.match(ln)
        if m and '(' not in m.group(3):
            sub, i, close = parse_block(lines, i, m.group(2), m.group(3), pending_size)
            pending_size = None
            if sub.name:
                # benannte innere Typen kommen auf die oberste Ebene
                sub.outer = blk.name
                blk.hoisted.append(sub)
                blk.hoisted.extend(sub.hoisted)
                continue
            blk.hoisted.extend(sub.hoisted)
            if '// offset' not in close:
                continue  # nur Deklaration des Typs eines folgenden Members
            if True:
                # anonyme verschachtelte Struktur als Member
                blk.lines.append(f'{sub.kind} {{')
                blk.lines += ['    ' + x for x in sub.lines]
                blk.lines.append(sanitize(close))
                blk.members += 1
            else:
                blk.lines.append(f'{sub.kind} {sub.name}' + (f' : {sub.bases}' if sub.bases else '') + ' {')
                blk.lines += ['    ' + x for x in sub.lines]
                blk.lines.append('};')
            continue
        if s.startswith('// total size:'):
            pending_size = s
            i += 1
            continue
        if '// offset 0x' in s:
            # Array von Funktionszeigern: dtk schreibt R (* n)(A)[N]
            s = re.sub(r'\(\* (\w+)\)(\(.*\))((?:\[\d+\])+);', r'(* \1\3)\2;', s)
            blk.lines.append(sanitize(s))
            blk.members += 1
        elif kind == 'enum' and s and not s.startswith('//'):
            blk.lines.append(s)
            blk.members += 1
        elif s.startswith('virtual ') or ' override' in s:
            sig = re.sub(r'\s*(\{\}|;|\{)\s*$', '', s)
            blk.virtuals.append(sig)
        # Funktionsrümpfe überspringen
        if '(' in s and s.endswith('{') and '// offset' not in s:
            depth = 1
            i += 1
            while depth and i < len(lines):
                depth += lines[i].count('{') - lines[i].count('}')
                i += 1
            continue
        i += 1
    return blk, i, ''


def parse(path):
    lines = open(path, encoding='latin1').read().split('\n')
    blocks = defaultdict(list)
    typedefs = {}
    i = 0
    pending_size = None
    while i < len(lines):
        ln = lines[i]
        if ln.startswith('// total size:'):
            pending_size = ln
        me = EMPTY_RE.match(ln)
        if me and not me.group(1):
            b = parse_empty(me, pending_size)
            pending_size = None
            blocks[b.name].append(b)
            i += 1
            continue
        m = HEAD_RE.match(ln)
        if m and not m.group(1) and '(' not in m.group(3) and m.group(3):
            blk, i, _ = parse_block(lines, i, m.group(2), m.group(3), pending_size)
            pending_size = None
            for b in [blk] + blk.hoisted:
                blocks[b.name].append(b)
            continue
        if TYPEDEF_RE.match(ln):
            t = sanitize(ln)
            m2 = re.search(r'\(\s*\*\s*(\w+)\)', t) or re.search(r'(\w+)\s*\(', t) \
                or re.search(r'(\w+)\s*(?:\[\d+\])*;$', t)
            nm = m2.group(1) if m2 else None
            if nm:
                typedefs.setdefault(nm, t)
        i += 1
    return blocks, typedefs


def choose(cands):
    return max(cands, key=lambda b: (b.members, len(b.virtuals), len(b.lines)))


def deps_of(blk, names):
    """Typen, die vollständig bekannt sein müssen (Basis, Member als Wert)."""
    d = set()
    for b in re.findall(r'\w+', blk.bases):
        if b in names:
            d.add(b)
    for ln in blk.lines:
        if '// offset' not in ln:
            continue
        decl = ln.split(';')[0]
        if '(' in decl:          # Funktionszeiger
            continue
        toks = re.findall(r'[\w]+|\*|&', decl)
        if '*' in toks or '&' in toks:
            continue
        for t in toks[:-1]:
            if t in names:
                d.add(t)
    return d


def rename_refs(blk, old, new):
    pat = re.compile(r'\b(struct|union|class|enum) ' + re.escape(old) + r'\b')
    blk.lines = [pat.sub(r'\1 ' + new, x) for x in blk.lines]


def main():
    blocks, typedefs = parse(DUMP)
    # innere Typen: bei Namenskollision als Outer_Inner führen
    top = {n for n, cs in blocks.items() if any(c.outer is None for c in cs)}
    renames = defaultdict(list)   # outer -> [(alt, neu)]
    merged = defaultdict(list)
    for n, cs in blocks.items():
        outers = {c.outer for c in cs if c.outer}
        clash = n in top or len(outers) > 1
        for c in cs:
            if c.outer and clash:
                c.name = f'{c.outer}_{n}'
                renames[c.outer].append((n, c.name))
            merged[c.name].append(c)
    blocks = merged
    chosen = {n: choose(c) for n, c in blocks.items() if n}
    for outer, pairs in renames.items():
        for o in [b for cs in blocks.values() for b in cs if b.name == outer]:
            for old, new in pairs:
                rename_refs(o, old, new)
        # abgeleitete Klassen sehen die inneren Typen der Basis ebenfalls
        for b in chosen.values():
            if re.search(r'\b' + re.escape(outer) + r'\b', b.bases):
                for old, new in pairs:
                    if old not in {x.name for x in b.hoisted}:
                        rename_refs(b, old, new)
    conflicts = {n: sorted({c.size for c in cs if c.size}) for n, cs in blocks.items()
                 if len({c.size for c in cs if c.size}) > 1}
    enums = {n: b for n, b in chosen.items() if b.kind == 'enum'}
    recs = {n: b for n, b in chosen.items() if b.kind != 'enum'}

    # Enumeratoren sind global: doppelte Namen mit Enum-Präfix
    used = set()
    for n in sorted(enums):
        new = []
        for x in enums[n].lines:
            m = re.match(r'(\w+)(.*)', x)
            if m and m.group(1) in used:
                x = f'{n}_{m.group(1)}{m.group(2)} // im Original: {m.group(1)}'
            elif m:
                used.add(m.group(1))
            new.append(x)
        enums[n].lines = new

    # fehlende Template-Instanzen aus einer Schwester-Instanz klonen (bTNode_X_, bTList_X_ ...)
    def all_text():
        return '\n'.join([b.bases + '\n' + '\n'.join(b.lines) for b in recs.values()] + list(typedefs.values()))
    for ref in sorted(set(re.findall(r'\b([A-Za-z]\w*?)_(\w+)_\b', all_text()))):
        tname, arg = ref
        full = f'{tname}_{arg}_'
        if full in recs or full in enums:
            continue
        sib = [b for k, b in recs.items() if k.startswith(tname + '_') and k.endswith('_') and not b.outer]
        if not sib:
            continue
        src = max(sib, key=lambda b: b.members)
        sarg = src.name[len(tname) + 1:-1]
        c = Block(src.kind, full, src.bases, src.size)
        c.lines = [re.sub(r'\b' + re.escape(sarg) + r'\b', arg, x) for x in src.lines]
        c.members = src.members
        c.cloned = src.name
        recs[full] = c

    # Typedef-Aliasse für Abhängigkeiten auflösen
    alias = {}
    for n, t in typedefs.items():
        m = re.match(r'typedef (?:const )?(?:struct|union|class) (\w+) (\w+);', t)
        if m:
            alias[m.group(2)] = m.group(1)
    known_scalar = set(enums) | set(typedefs) | KEYWORDS

    # unbekannte Typen als Wert: durch Bytearray ersetzen; unbekannte Basen ebenso
    missing = set()
    for b in recs.values():
        new = []
        for x in b.lines:
            m = re.match(r'((?:const |volatile )*)(?:(struct|union|class|enum) )?(\w+) (\w+)((?:\[\d+\])*); // offset (0x[0-9A-F]+), size (0x[0-9A-F]+)', x)
            if m and m.group(3) not in recs and m.group(3) not in known_scalar and alias.get(m.group(3)) not in recs \
                    or (m and m.group(2) == 'enum' and m.group(3) not in enums):
                missing.add(m.group(3))
                x = f'unsigned char {m.group(4)}[{int(m.group(7), 16)}]; // offset {m.group(6)}, Typ {m.group(3)}{m.group(5)} fehlt im DWARF'
            new.append(x)
        b.lines = new
        bases = []
        for base in [x.strip() for x in b.bases.split(',') if x.strip()]:
            bn = base.split()[-1]
            if bn in recs:
                bases.append(base)
            else:
                missing.add(bn)
                first = re.search(r'offset (0x[0-9A-F]+)', '\n'.join(b.lines))
                sz = int(first.group(1), 16) if first else 0
                if sz:
                    b.lines.insert(0, f'unsigned char _base_{bn}[{sz}]; // Basis {bn} fehlt im DWARF')
        b.bases = ', '.join(bases)
    for n in missing:
        typedefs.pop(n, None) if n in typedefs and n not in alias else None

    names = set(recs) | set(alias)
    order, seen = [], set()

    def visit(n, stack=()):
        n = alias.get(n, n)
        if n in seen or n not in recs or n in stack:
            return
        for d in sorted(deps_of(recs[n], names)):
            visit(d, stack + (n,))
        seen.add(n)
        order.append(n)
    for n in sorted(recs):
        visit(n)

    text_all = all_text()
    mentioned = (set(re.findall(r'\b(?:struct|class|union) (\w+)', text_all)) | set(recs)) - set(enums)
    out = ['// Automatisch erzeugt von tools/gc_types.py aus dem DWARF der GC-Speed.elf (dtk dwarf dump).',
           '// Layouts gelten für die 32-Bit-PowerPC-Zielplattform (Zeiger = 4 Byte). Offsets stehen als Kommentar.',
           '// Virtuelle Methoden stehen als Kommentar in Deklarationsreihenfolge (GCC 2.95: Vptr ist ein echtes Feld _vptr_<Klasse>).',
           '// Innere Typen stehen auf oberster Ebene; bei Namenskollision als <Äußere>_<Innere>.',
           '// ACHTUNG: In diesem GCC-2.95-SN-Build belegt bool 4 Byte (DWARF meldet size 0x1, die Offsets zeigen 4).',
           '//          Größenprüfung daher mit -Dbool=int:  tools/gc_types.py --check',
           '// Nicht von Hand bearbeiten.', '#pragma once', '',
           'struct __vtbl_ptr_type;', '']
    out.append('// ---- Enums ----')
    for n in sorted(enums):
        b = enums[n]
        if b.outer:
            out.append(f'// innerer Typ von {b.outer}')
        out.append(f'enum {n} {{')
        out += ['    ' + x for x in b.lines]
        out.append('};')
    out.append('\n// ---- Vorwärtsdeklarationen ----')
    for n in sorted(mentioned):
        kind = recs[n].kind if n in recs else 'struct'
        out.append(f'{"struct" if kind == "class" else kind} {n};')
    def emit(n):
        b = recs[n]
        kind = 'struct' if b.kind == 'class' else b.kind
        if b.size:
            out.append(b.size)
        if b.outer:
            out.append(f'// innerer Typ von {b.outer}')
        if getattr(b, 'cloned', None):
            out.append(f'// Template-Instanz, Layout von {b.cloned} übernommen')
        if n in conflicts:
            out.append(f'// Achtung: verschiedene Größen in den Übersetzungseinheiten: {conflicts[n]}')
        if b.virtuals:
            out.append('// virtuell: ' + ' | '.join(b.virtuals))
        out.append(f'{kind} {n}' + (f' : {b.bases}' if b.bases else '') + ' {')
        out.extend('    ' + x for x in b.lines)
        out.append('};')
    # Strukturen, die Typedefs als Wert brauchen (z. B. va_list), vorziehen
    early = [m for t in typedefs.values() for m in re.findall(r'^typedef (?:struct|union) (\w+) \w+\[', t)
             if m in recs and not deps_of(recs[m], names)]
    if early:
        out.append('\n// ---- vorgezogene Strukturen ----')
        for n in early:
            emit(n)
    out.append('\n// ---- Typedefs ----')
    for n in typedefs:  # Reihenfolge wie im Dump (Abhängigkeiten)
        if n in recs or n in enums:
            continue
        out.append(typedefs[n])
    out.append('\n// ---- Strukturen und Klassen ----')
    for n in order:
        if n in early:
            continue
        emit(n)
    open(OUT, 'w').write('\n'.join(out) + '\n')
    write_c_variant(out, recs)
    if '--check' in sys.argv:
        check()
    if True:
        n_class = sum(1 for b in recs.values() if b.virtuals or b.bases)
        print(f'Enums {len(enums)}, Strukturen {sum(1 for b in recs.values() if b.kind != "union")}'
              f' (davon mit Basis oder virtuell {n_class}), Unions {sum(1 for b in recs.values() if b.kind == "union")},'
              f' Typedefs {len(typedefs)}, Größenkonflikte {len(conflicts)}, fehlende Typen {len(missing)}: {sorted(missing)}')


OUT_C = os.path.join(ROOT, 'refs', 'gc', 'gc_types_c.h')


def write_c_variant(lines, recs):
    """C-Fassung für Ghidras C-Parser (refs/gc/gc_types_c.h, nicht committen).

    Basisklassen werden zum ersten Feld '_base_<Name>', leere Basen entfallen, bool wird zu
    int (4 Byte wie im GC-Build), Referenzen werden Zeiger.
    """
    out = []
    for ln in lines:
        if ln.startswith('#pragma'):
            continue
        m = re.match(r'(struct|union) (\w+) : (.+) \{$', ln)
        if m:
            out.append(f'{m.group(1)} {m.group(2)} {{')
            for base in [x.strip().split()[-1] for x in m.group(3).split(',')]:
                if base in recs and recs[base].members + len(recs[base].bases) > 0:
                    out.append(f'    struct {base} _base_{base};')
            continue
        ln = re.sub(r'\bbool\b(?=[^:]*:\s*\d)', 'unsigned int', ln)   # Bitfelder
        ln = re.sub(r'\bbool\b', 'int', ln)
        ln = re.sub(r'(\w|\*) ?&', r'\1 *', ln) if not ln.lstrip().startswith('//') else ln
        out.append(ln)
    # C braucht für unqualifizierte Namen Typedefs
    tdnames = set(re.findall(r'^typedef .*?(\w+)\)?(?:\(.*\))?;$', '\n'.join(out), re.M))
    tdnames |= set(re.findall(r'^typedef .*\b(\w+)\s*(?:\[\d+\])*;$', '\n'.join(out), re.M))
    decl = []
    for ln in out:
        m = re.match(r'^(struct|union|enum) (\w+)(?:;| \{)$', ln)
        if m and m.group(2) not in tdnames:
            tdnames.add(m.group(2))
            decl.append(f'typedef {m.group(1)} {m.group(2)} {m.group(2)};')
    i = next(k for k, x in enumerate(out) if 'Vorwärtsdeklarationen' in x)
    j = next(k for k, x in enumerate(out) if 'Typedefs ----' in x)
    out = out[:j] + ['// C-Typedefs'] + decl + out[j:]
    open(OUT_C, 'w').write('\n'.join(out) + '\n')


def check():
    """sizeof jeder Struktur gegen die DWARF-Gesamtgröße prüfen (clang, 32-Bit-PowerPC)."""
    import subprocess
    import tempfile
    asserts, size = [f'#include "{OUT}"'], None
    for ln in open(OUT).read().split('\n'):
        m = re.match(r'// total size: (0x[0-9A-F]+)', ln)
        if m:
            size = int(m.group(1), 16)
            continue
        m = re.match(r'(struct|union) (\w+)( :.*)? \{$', ln)
        if m and size is not None:
            asserts.append(f'static_assert(sizeof({m.group(1)} {m.group(2)}) == {size}, "{m.group(2)}");')
        if not ln.startswith('//'):
            size = None
    with tempfile.NamedTemporaryFile('w', suffix='.cpp', delete=False) as f:
        f.write('\n'.join(asserts) + '\n')
    r = subprocess.run(['clang++', '-std=c++17', '--target=powerpc-unknown-eabi', '-Dbool=int', '-fsyntax-only',
                        '-ferror-limit=0', f.name], capture_output=True, text=True)
    bad = re.findall(r'requirement .sizeof\((?:struct |union )?(\w+)\)', r.stderr)
    other = [x for x in r.stderr.split('\n') if 'error:' in x and 'static assertion' not in x]
    print(f'Größenprüfung: {len(asserts) - 1} Strukturen, {len(bad)} abweichend: {bad}; andere Fehler: {len(other)}')
    os.unlink(f.name)


if __name__ == '__main__':
    main()
