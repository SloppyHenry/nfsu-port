#!/usr/bin/env python3
"""symbols/types.h für die PC-Exe aus symbols/gc_types.h berechnen (WP-04, Schritt 3).

Regeln für den MSVC-Build (Beleg: plan/FAKTEN.md, WP-04):
 - bool = 1 Byte (GC-Build: 4), sonst gleiche Größen (int/long/Zeiger 4, double 8)
 - natürliche Ausrichtung, Strukturgröße auf die größte Ausrichtung aufgerundet
 - Basisklassen als erstes Feld `base`; MSVC legt den Vptr der Klasse, die ihn einführt, an den
   Anfang (vor die Basis), GCC 2.95 hinter die Felder (`_vptr_X`)
 - leere Strukturen: 1 Byte
Die Ausgabe ist C (Ghidra-CParser). Jede Struktur trägt `// size 0x..` und je Feld `// +0x..`.
Korrekturen gegenüber dem GC-Dekompilat stehen in tools/types/pc_overrides.py mit Beleg.

  python3 tools/types/gen_types.py        -> symbols/types.h
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
ROOT = os.path.dirname(os.path.dirname(HERE))
SRC = os.path.join(ROOT, 'symbols', 'gc_types.h')
OUT = os.path.join(ROOT, 'symbols', 'types.h')

try:
    from pc_overrides import OVERRIDES
except ImportError:
    OVERRIDES = {}

BASIC = {'char': 1, 'signed char': 1, 'unsigned char': 1, 'short': 2, 'unsigned short': 2, 'int': 4,
         'unsigned int': 4, 'unsigned': 4, 'long': 4, 'unsigned long': 4, 'float': 4, 'double': 8,
         'long long': 8, 'unsigned long long': 8, 'wchar_t': 2, 'bool': 1, 'void': 1, 'long double': 8}

HEAD = re.compile(r'^(struct|union) (\w+)(?: : public (\w+))? \{$')
FIELD = re.compile(r'^    (.*?)((?: \*)*) (\w+)((?:\[\d+\])*); // offset 0x([0-9A-Fa-f]+), size 0x([0-9A-Fa-f]+)$')
FNPTR = re.compile(r'^    (.*\(\* ?(\w+)\)\(.*\)); // offset 0x([0-9A-Fa-f]+), size 0x([0-9A-Fa-f]+)$')
BITF = re.compile(r'^    ([\w ]+?) (\w+) : (\d+); // offset 0x([0-9A-Fa-f]+), size 0x([0-9A-Fa-f]+)$')
BLOCK_END = re.compile(r'^    \}(?: (\w+))?((?:\[\d+\])*); // offset 0x([0-9A-Fa-f]+), size 0x([0-9A-Fa-f]+)$')
TYPEDEF = re.compile(r'^typedef (.*) (\w+);$')
ENUM = re.compile(r'^enum (\w+) \{')


def parse():
    structs, enums, typedefs = {}, set(), {}
    cur = None
    nested = False
    for line in open(SRC, encoding='utf-8'):
        line = line.rstrip('\n')
        m = ENUM.match(line)
        if m:
            enums.add(m.group(1))
        m = HEAD.match(line)
        if m:
            cur = dict(kind=m.group(1), name=m.group(2), base=m.group(3), fields=[], virt=False)
            structs[cur['name']] = cur
            continue
        if line == '};':
            cur = None
            continue
        m = TYPEDEF.match(line)
        if m and '(' not in line:
            typedefs[m.group(2)] = m.group(1)
        if cur is not None:
            if line in ('    union {', '    struct {'):
                nested = True
                continue
            if nested:
                m = BLOCK_END.match(line)
                if m:
                    nm, dims, off, sz = m.groups()
                    cur['fields'].append(dict(type='char', ptr=0, name=nm or 'anon%d' % len(cur['fields']),
                                              dims=[int(sz, 16)], off=int(off, 16), gcsize=int(sz, 16), opaque=True))
                    nested = False
                continue
            m = BITF.match(line)
            if m:
                typ, nm, bits, off, sz = m.groups()
                off, sz = int(off, 16), int(sz, 16)
                last = cur['fields'][-1] if cur['fields'] else None
                if last and last.get('bits') and last['off'] == off:
                    last['bits'].append('%s:%s' % (nm, bits))
                    last['name'] = '_bits_0x%x' % off
                else:
                    cur['fields'].append(dict(type='unsigned int' if sz == 4 else 'unsigned char' if sz == 1 else 'unsigned short',
                                              ptr=0, name='_bits_0x%x' % off, dims=[], off=off, gcsize=sz, bits=['%s:%s' % (nm, bits)]))
                continue
            m = FNPTR.match(line)
            if m:
                decl, nm, off, sz = m.groups()
                cur['fields'].append(dict(type=decl, ptr=0, name=nm, dims=[], off=int(off, 16), gcsize=4, fnptr=True))
                continue
            m = FIELD.match(line)
            if m:
                typ, stars, name, dims, off, sz = m.groups()
                cur['fields'].append(dict(type=typ, ptr=stars.count('*'), name=name,
                                          dims=[int(d) for d in re.findall(r'\[(\d+)\]', dims)], off=int(off, 16), gcsize=int(sz, 16)))
                if name.startswith('_vptr'):
                    cur['virt'] = True
    # leere Strukturen mit Vorwärtsdeklaration unterscheiden: nur bei `struct X {` gefüllt
    return structs, enums, typedefs


class Layout:
    def __init__(self):
        self.structs, self.enums, self.typedefs = parse()
        self.cache = {}
        self.fn_ptr = re.compile(r'\(\*')

    def scalar(self, typ, depth=0):
        """(size, align) eines Typnamens ohne Zeiger/Array."""
        t = typ.replace('const ', '').replace('volatile ', '').replace('struct ', '').replace('union ', '').replace('enum ', '').strip()
        if t in BASIC:
            s = BASIC[t]
            return s, min(s, 8)
        if t in self.enums:
            return 4, 4
        if t in self.structs:
            r = self.struct(t)
            return r['size'], r['align']
        if t in self.typedefs and depth < 10:
            return self.scalar(self.typedefs[t], depth + 1)
        raise KeyError(t)

    def field(self, f):
        if f.get('opaque'):
            return f['gcsize'], 4 if f['gcsize'] % 4 == 0 else 1
        if f.get('bits'):
            return f['gcsize'], f['gcsize']
        if f['ptr'] or '(*' in f['type']:
            s, a = 4, 4
        else:
            s, a = self.scalar(f['type'])
        n = 1
        for d in f['dims']:
            n *= d
        return s * n, a

    def struct(self, name):
        if name in self.cache:
            return self.cache[name]
        st = self.structs[name]
        self.cache[name] = dict(size=1, align=1, fields=[], placeholder=True)  # Rekursionsschutz
        fields = list(st['fields'])
        out = []
        off = 0
        align = 1

        def place(typ, nm, size, al, dims, ptr, label, src=None):
            nonlocal off, align
            if al:
                off = (off + al - 1) // al * al
            out.append(dict(type=typ, name=nm, dims=dims, ptr=ptr, off=off, size=size, label=label, **({'opaque': True} if src and src.get('opaque') else {}), **({'bits': src['bits']} if src and src.get('bits') else {})))
            off += size
            align = max(align, al or 1)

        vptr = [f for f in fields if f['name'].startswith('_vptr')]
        rest = [f for f in fields if not f['name'].startswith('_vptr')]
        base_has_vptr = st['base'] and self.has_vptr(st['base'])
        if st['kind'] == 'union':
            size = 0
            for f in fields:
                s, a = self.field(f)
                out.append(dict(type=f['type'], name=f['name'], dims=f['dims'], ptr=f['ptr'], off=0, size=s, label=''))
                size = max(size, s)
                align = max(align, a)
            size = (size + align - 1) // align * align if size else 1
            r = dict(size=size, align=align, fields=out, kind='union', base=None, gc=st)
            self.cache[name] = r
            return r
        if vptr and not base_has_vptr:
            place('void', 'vptr', 4, 4, [], 2, 'Vptr (MSVC: am Anfang)')
        if st['base']:
            b = self.struct(st['base'])
            place('struct ' + st['base'], 'base', b['size'], b['align'], [], 0, 'Basisklasse')
        for f in sorted(rest, key=lambda f: f['off']):
            s, a = self.field(f)
            place(f['type'], f['name'], s, a, f['dims'], f['ptr'], '', f)
        size = max(off, 1)
        size = (size + align - 1) // align * align
        r = dict(size=size, align=align, fields=out, kind='struct', base=st['base'], gc=st)
        self.cache[name] = r
        return r

    def has_vptr(self, name):
        st = self.structs[name]
        return st['virt'] or (st['base'] and self.has_vptr(st['base']))


def emit(L):
    names = sorted(L.structs)
    lines = ['// Automatisch erzeugt von tools/types/gen_types.py aus symbols/gc_types.h (WP-04).',
             '// PC-Layout (MSVC, 32 Bit): bool = 1 Byte, Vptr am Anfang der einführenden Klasse, Basisklasse als Feld `base`.',
             '// Offsets und Größen sind berechnet; Abweichungen mit Code-Beleg in tools/types/pc_overrides.py.',
             '// Nicht von Hand bearbeiten.', '#pragma once', '']
    enums = []
    gc = open(SRC, encoding='utf-8').read()
    for m in re.finditer(r'^enum (\w+) \{\n(.*?)^\};', gc, re.M | re.S):
        enums.append((m.group(1), m.group(2)))
    for n, body in enums:
        lines += ['enum %s {' % n, body.rstrip('\n'), '};', 'typedef enum %s %s;' % (n, n)]
    lines += ['']
    for n in names:
        k = L.structs[n]['kind']
        lines.append('typedef %s %s %s;' % (k, n, n))
    lines += ['', '// ---- typedefs ----']
    for line in open(SRC, encoding='utf-8'):
        if line.startswith('typedef ') and line.rstrip().endswith(';'):
            m = re.search(r'\(\*\s*(\w+)\)', line)
            if m:
                lines.append('typedef void * %s; // Funktionszeiger' % m.group(1))
            elif '__va_list' not in line and 'va_list' not in line:
                lines.append(line.rstrip())
    lines += ['', '// ---- Strukturen (Abhängigkeitsreihenfolge) ----']
    done = set()
    skipped = []

    def dep(n):
        if n in done:
            return
        done.add(n)
        try:
            r = L.struct(n)
        except KeyError as e:
            skipped.append((n, str(e)))
            return
        for f in r['fields']:
            t = f['type'].replace('struct ', '').replace('union ', '').replace('const ', '').strip()
            for _ in range(10):
                if t in L.typedefs and t not in L.structs:
                    t = L.typedefs[t].replace('struct ', '').replace('union ', '').replace('const ', '').strip()
            if t in L.structs and not f['ptr']:
                dep(t)
        emit_one(n, r)

    def emit_one(n, r):
        ov = OVERRIDES.get(n, {})
        lines.append('// size 0x%X%s' % (r['size'], ' (GC: 0x%X)' % gcsize(n) if gcsize(n) is not None and gcsize(n) != r['size'] else ''))
        lines.append('%s %s {' % (r['kind'], n))
        if not r['fields']:
            lines.append('    char _empty; // +0x0 (leere Klasse)')
        pad = 0
        end = 0
        for f in r['fields']:
            if r['kind'] == 'struct' and f['off'] > end:
                lines.append('    char _pad%d[0x%X]; // +0x%X' % (pad, f['off'] - end, end))
                pad += 1
            dims = ''.join('[%d]' % d for d in f['dims'])
            t = f['type']
            if '(*' in t:
                lines.append('    %s; // +0x%X' % (t, f['off']))
            else:
                stars = '*' * f['ptr']
                note = f['label'] or ('anonymer Union/Struct, GC-Größe' if f.get('opaque') else '') or ('Bitfelder ' + ' '.join(f['bits']) if f.get('bits') else '')
                lines.append('    %s %s%s%s; // +0x%X%s' % (t, stars, f['name'], dims, f['off'], '  ' + note if note else ''))
            end = f['off'] + f['size']
        if r['kind'] == 'struct' and r['size'] > end and r['fields']:
            lines.append('    char _pad%d[0x%X]; // +0x%X' % (pad, r['size'] - end, end))
        lines.append('};')

    gcs = {}
    for m in re.finditer(r'^// total size: 0x([0-9A-Fa-f]+)\n(?:// .*\n)*(?:struct|union) (\w+)', gc, re.M):
        gcs[m.group(2)] = int(m.group(1), 16)

    def gcsize(n):
        return gcs.get(n)

    for n in names:
        dep(n)
    return re.sub(r' &', ' *', '\n'.join(lines)) + '\n', skipped, gcs


def main():
    L = Layout()
    text, skipped, gcs = emit(L)
    open(OUT, 'w', encoding='utf-8').write(text)
    n = len(L.structs)
    diff = sum(1 for k in L.cache if k in gcs and not L.cache[k].get('placeholder') and L.cache[k]['size'] != gcs[k])
    print('Strukturen %d, übersprungen %d, Größe weicht vom GC ab: %d' % (n, len(skipped), diff))
    for s in skipped[:10]:
        print('  übersprungen', s)


if __name__ == '__main__':
    main()
