#!/usr/bin/env python3
"""DirectX-COM-Tabellen und Zuordnung der Aufrufe (WP-04, Schritt 2).

1. symbols/com_vtables.csv: Methoden-Offsets aller Schnittstellen aus com_defs.py.
2. Datenfluss im Maschinencode (je Funktion linear, Register und Stack-Argumente symbolisch):
   Startpunkte sind die Erzeuger (Direct3DCreate9-Rückgabe, Ausgabeparameter `IFoo **` von
   DirectSoundCreate, DirectInput8Create, CreateDevice, CreateTexture, QueryInterface mit bekannter IID …).
   Typen wandern über Speicherungen in Globals und Klassenfelder (`this` einer Methode `Klasse::M`).
3. symbols/com_calls.csv: jeder virtuelle Aufruf `call [reg+off]`, dessen Objekt einen bekannten
   Schnittstellentyp hat -> Schnittstelle::Methode. Typisierte Globals/Felder -> refs/types_cache/com_slots.csv
   (für types.h und data.csv).

  tools/emu/.venv/bin/python tools/types/com.py
"""
import collections
import csv
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from pe import Pe, ROOT
import disasm
from com_defs import INTERFACES, methods, method_name

OUT_VT = os.path.join(ROOT, 'symbols', 'com_vtables.csv')
OUT_CALLS = os.path.join(ROOT, 'symbols', 'com_calls.csv')
OUT_SLOTS = os.path.join(ROOT, 'symbols', 'com_globals.csv')
OUT_H = os.path.join(ROOT, 'symbols', 'com_types.h')

# Import -> (Rückgabetyp, {Argumentindex: Typ des Ausgabeparameters})
IMPORTS = {
    'd3d9.dll!Direct3DCreate9': ('IDirect3D9', {}),
    'DSOUND.dll!#1': (None, {1: 'IDirectSound'}),               # DirectSoundCreate(guid, ppDS, outer)
    'DINPUT8.dll!DirectInput8Create': (None, {3: 'IDirectInput8A'}),
    'DINPUT.dll!DirectInputCreateA': (None, {2: 'IDirectInputA'}),
    'DDRAW.dll!DirectDrawCreate': (None, {1: 'IDirectDraw'}),
}

# IIDs (Data1-Data2-Data3-Data4) für QueryInterface/CreateDevice-Erkennung
IIDS = {
    '279AFA86-4981-11CE-A521-0020AF0BE560': 'IDirectSound3DBuffer',
    '279AFA84-4981-11CE-A521-0020AF0BE560': 'IDirectSound3DListener',
    '279AFA85-4981-11CE-A521-0020AF0BE560': 'IDirectSoundBuffer',
    '279AFA83-4981-11CE-A521-0020AF0BE560': 'IDirectSound',
    'C50A7E93-F395-4834-9EF6-7FA99DE50966': 'IDirectSound8',
    '6825A449-7524-4D82-920F-50E36AB3AB1E': 'IDirectSoundBuffer8',
    'B0210783-89CD-11D0-AF08-00A0C925CD16': 'IDirectSoundNotify',
    '31EFAC30-515C-11D0-A9AA-00AA0061BE93': 'IKsPropertySet',
    'BF798030-483A-4DA2-AA99-5D64ED369700': 'IDirectInput8A',
    '54D41080-DC15-4833-A41B-748F73A38179': 'IDirectInputDevice8A',
    'B3A6F3E0-2B43-11CF-A2DE-00AA00B93356': 'IDirectDraw2',
    '15E65EC0-3B9C-11D2-B92F-00609797EA5B': 'IDirectDraw7',
    '6C14DB80-A733-11CE-A521-0020AF0BE560': 'IDirectDraw',
}

REG = r'e(?:ax|bx|cx|dx|si|di|bp|sp)'
MEM = re.compile(r'^(?:(?:byte|word|dword|qword) ptr )?\[([^\]]+)\]$')
IMM = re.compile(r'^(?:0x[0-9a-f]+|\d+)$')
VOLATILE = ('eax', 'ecx', 'edx')
# Argumentzahl bekannter stdcall-Importe; unbekannte Importe verbrauchen alle Pushes
NARGS = {'KERNEL32.dll!GetModuleHandleA': 1, 'DSOUND.dll!#1': 3, 'd3d9.dll!Direct3DCreate9': 1,
         'DINPUT8.dll!DirectInput8Create': 5, 'DINPUT.dll!DirectInputCreateA': 4, 'DDRAW.dll!DirectDrawCreate': 3}


def guid_at(pe, va):
    try:
        b = pe.read(va, 16)
    except KeyError:
        return None
    d1, d2, d3 = struct.unpack('<IHH', b[:8])
    return '%08X-%04X-%04X-%s-%s' % (d1, d2, d3, b[8:10].hex().upper(), b[10:].hex().upper())


def parse_mem(s):
    """'eax + ecx*4 + 0x10' -> (basis, index, disp)."""
    base = idx = None
    disp = 0
    for t in s.replace(' - ', ' + -').split(' + '):
        t = t.strip()
        if '*' in t:
            idx = t
        elif re.fullmatch(REG, t):
            if base is None:
                base = t
            else:
                idx = t
        else:
            disp += int(t, 0)
    return base, idx, disp


def class_of(name):
    m = re.fullmatch(r'(.+)::~?\w+', name or '')
    return m.group(1) if m else None


def scan_function(f, ins, klass, pe, thunks):
    """Symbolischer Lauf: liefert Ereignisse (art, ...)."""
    regs = {}
    if klass:
        regs['ecx'] = ('THIS', klass)
    pushes = []
    ev = []

    def val(op):
        op = op.strip()
        if re.fullmatch(REG, op):
            return regs.get(op)
        if IMM.match(op):
            v = int(op, 0)
            return ('ADDR', v) if pe.section_of(v) in ('.data', '.rdata') else None
        m = MEM.match(op)
        if m:
            base, idx, disp = parse_mem(m.group(1))
            if idx:
                return None
            if base is None:
                return ('G', disp)
            if base == 'esp':
                return None
            b = regs.get(base)
            if b is None:
                return None
            if disp == 0 and b[0] in ('G', 'F', 'RET', 'THISF'):
                return ('V', b)
            if b[0] == 'THIS':
                return ('F', ('THIS', b[1]), disp)
            if b[0] in ('G', 'F', 'RET'):
                return ('F', b, disp)
            return None
        return None

    def lea(op):
        m = MEM.match(op)
        if not m:
            return None
        base, idx, disp = parse_mem(m.group(1))
        if idx or base is None:
            return ('ADDR', disp) if base is None and not idx else None
        b = regs.get(base)
        if b and b[0] == 'THIS':
            return ('FADDR', b[1], disp)
        if b and b[0] in ('G', 'F'):
            return ('FADDR2', b, disp)
        return None

    for a, mn, ops in ins:
        parts = [p.strip() for p in ops.split(', ')] if ops else []
        if mn == 'mov' and len(parts) == 2:
            dst, src = parts
            if re.fullmatch(REG, dst):
                regs[dst] = val(src)
                if regs[dst] and regs[dst][0] == 'F' and regs[dst][1][0] == 'THIS':
                    pass
            else:
                m = MEM.match(dst)
                v = val(src)
                if m and v is not None:
                    base, idx, disp = parse_mem(m.group(1))
                    if not idx:
                        if base is None:
                            ev.append(('store', ('G', disp), v))
                        elif base != 'esp' and regs.get(base) and regs[base][0] == 'THIS':
                            ev.append(('store', ('F', ('THIS', regs[base][1]), disp), v))
                        elif base != 'esp' and regs.get(base) and regs[base][0] in ('G', 'F'):
                            ev.append(('store', ('F', regs[base], disp), v))
        elif mn == 'lea' and len(parts) == 2:
            regs[parts[0]] = lea(parts[1])
        elif mn == 'push':
            pushes.append(val(parts[0]) if parts else None)
        elif mn == 'call':
            tgt = parts[0] if parts else ''
            args = list(reversed(pushes))
            if IMM.match(tgt) and int(tgt, 0) in thunks:
                ev.append(('import', a, thunks[int(tgt, 0)], args))
                regs = {k: v for k, v in regs.items() if k not in VOLATILE}
                regs['eax'] = ('RET', a)
                n = NARGS.get(thunks[int(tgt, 0)])
                pushes = pushes[:len(pushes) - n] if n is not None else []
                continue
            m = MEM.match(tgt)
            if m:
                base, idx, disp = parse_mem(m.group(1))
                if base is None and not idx and disp in pe.imports:
                    ev.append(('import', a, pe.imports[disp], args))
                    regs = {k: v for k, v in regs.items() if k not in VOLATILE}
                    regs['eax'] = ('RET', a)
                    n = NARGS.get(pe.imports[disp])
                    pushes = pushes[:len(pushes) - n] if n is not None else []
                    continue
                if base and not idx and regs.get(base) and regs[base][0] == 'V':
                    ev.append(('vcall', a, regs[base][1], disp, args))
                    regs = {k: v for k, v in regs.items() if k not in VOLATILE}
                    regs['eax'] = ('RET', a)
                    pushes = []
                    continue
            regs = {k: v for k, v in regs.items() if k not in VOLATILE}
            pushes = []
        elif mn in ('ret', 'jmp'):
            pushes = []
        elif mn in ('add', 'sub') and parts and parts[0] == 'esp':
            if mn == 'add':
                pushes = []
        elif parts and re.fullmatch(REG, parts[0]) and mn not in ('cmp', 'test', 'push'):
            regs[parts[0]] = None
    return ev


def key(v):
    """Symbolischer Wert -> Ablageort, der einen Typ tragen kann."""
    if v is None:
        return None
    if v[0] == 'G':
        return ('G', v[1])
    if v[0] == 'F' and v[1][0] == 'THIS':
        return ('CF', v[1][1], v[2])
    if v[0] == 'F' and v[1][0] == 'G':
        return ('GF', v[1][1], v[2])
    if v[0] == 'RET':
        return v
    return None


def out_key(v):
    """Ausgabeparameter (Adresse eines Ablageorts) -> Ablageort."""
    if v is None:
        return None
    if v[0] == 'ADDR':
        return ('G', v[1])
    if v[0] == 'FADDR':
        return ('CF', v[1], v[2])
    if v[0] == 'FADDR2' and v[1][0] == 'G':
        return ('GF', v[1][1], v[2])
    return None


def main():
    pe = Pe()
    code = disasm.load()
    funcs = {}
    with open(os.path.join(ROOT, 'symbols', 'functions.csv'), newline='') as fh:
        for r in csv.DictReader(fh):
            funcs[int(r['addr'], 16)] = r

    # 1. Tabelle der Methoden-Offsets
    with open(OUT_VT, 'w', newline='') as fh:
        w = csv.writer(fh)
        w.writerow(['interface', 'slot', 'offset', 'method', 'deklariert_in', 'signatur'])
        for iface in INTERFACES:
            for slot, sig, decl in methods(iface):
                w.writerow([iface, slot, '0x%x' % (slot * 4), method_name(sig), decl, sig])

    # 2. Ereignisse; Import-Thunks (jmp [IAT]) wie direkte Importaufrufe behandeln
    thunks = {}
    for f, ins in code.items():
        if ins and ins[0][1] == 'jmp':
            m = MEM.match(ins[0][2])
            if m and parse_mem(m.group(1))[0] is None and parse_mem(m.group(1))[2] in pe.imports:
                thunks[f] = pe.imports[parse_mem(m.group(1))[2]]
    events = {}
    for f, ins in code.items():
        nm = funcs[f]['name'] if f in funcs else ''
        conv = funcs[f]['convention'] if f in funcs else ''
        klass = class_of(nm) if conv in ('__thiscall', '__fastcall', '') else None
        events[f] = scan_function(f, ins, klass, pe, thunks)

    T = {}
    src_of = collections.defaultdict(set)  # Typquelle je Ort (für das Protokoll)

    def settype(k, t, why):
        if k is None or t is None:
            return False
        if k in T:
            if T[k] != t:
                src_of[k].add('KONFLIKT %s/%s' % (T[k], t))
            return False
        T[k] = t
        src_of[k].add(why)
        return True

    for it in range(20):
        new = 0
        for f, ev in events.items():
            for e in ev:
                if e[0] == 'import':
                    _, a, name, args = e
                    ret, outs = IMPORTS.get(name, (None, {}))
                    if ret:
                        new += settype(('RET', a), ret, name)
                    for i, t in outs.items():
                        if i < len(args):
                            new += settype(out_key(args[i]), t, name)
                elif e[0] == 'vcall':
                    _, a, obj, off, args = e
                    t = T.get(key(obj))
                    if not t or off % 4:
                        continue
                    ms = methods(t)
                    if off // 4 >= len(ms):
                        continue
                    sig = ms[off // 4][1]
                    mname = method_name(sig)
                    params = [p.strip() for p in sig[sig.index('(') + 1:-1].split(',') if p.strip()]
                    for i, p in enumerate(params):
                        m = re.match(r'(?:const )?(I\w+) \*\*', p)
                        if m and i + 1 < len(args):
                            new += settype(out_key(args[i + 1]), m.group(1), '%s::%s' % (t, mname))
                    if mname == 'QueryInterface' and len(args) >= 3 and args[1] and args[1][0] == 'ADDR':
                        q = IIDS.get(guid_at(pe, args[1][1]))
                        new += settype(out_key(args[2]), q, 'QueryInterface')
                elif e[0] == 'store':
                    _, dst, v = e
                    kv = key(v)
                    if kv in T:
                        new += settype(key(dst), T[kv], 'Kopie')
        # Rückwärts: Ort wird in einen typisierten Ort kopiert -> gleicher Typ
        for f, ev in events.items():
            for e in ev:
                if e[0] == 'store':
                    kd, kv = key(e[1]), key(e[2])
                    if kd in T and kv and kv[0] != 'RET':
                        new += settype(kv, T[kd], 'Kopie')
        if not new:
            break

    # 3. Aufrufe
    calls, per_if = [], collections.Counter()
    vtot = 0
    for f, ev in events.items():
        for e in ev:
            if e[0] != 'vcall':
                continue
            vtot += 1
            _, a, obj, off, args = e
            t = T.get(key(obj))
            if not t or off % 4 or off // 4 >= len(methods(t)):
                continue
            m = method_name(methods(t)[off // 4][1])
            calls.append(dict(funktion='0x%06x' % f, funktion_name=funcs[f]['name'] if f in funcs else '',
                              aufruf='0x%06x' % a, objekt=fmt(key(obj)), interface=t, offset='0x%x' % off, methode=m))
            per_if[t] += 1
    calls.sort(key=lambda r: r['aufruf'])
    with open(OUT_CALLS, 'w', newline='') as fh:
        w = csv.DictWriter(fh, fieldnames=list(calls[0].keys()))
        w.writeheader()
        w.writerows(calls)
    os.makedirs(os.path.dirname(OUT_SLOTS), exist_ok=True)
    with open(OUT_SLOTS, 'w', newline='') as fh:
        w = csv.writer(fh)
        w.writerow(['ort', 'interface', 'quelle'])
        for k in sorted(T, key=fmt):
            if k[0] != 'RET':
                w.writerow([fmt(k), T[k], ' '.join(sorted(src_of[k]))])
    write_header()
    kinds = collections.Counter(k[0] for k in T if k[0] != 'RET')
    print('com_vtables: %d Schnittstellen, %d Methoden' % (len(INTERFACES), sum(len(methods(i)) for i in INTERFACES)))
    print('typisierte Orte: %s; Konflikte: %d' % (dict(kinds), sum(1 for s in src_of.values() if any('KONFLIKT' in x for x in s))))
    print('virtuelle Aufrufe gesamt %d, davon COM zugeordnet %d' % (vtot, len(calls)))
    for t, n in per_if.most_common():
        print('  %-26s %5d' % (t, n))


def write_header():
    L = ['// Automatisch erzeugt von tools/types/com.py (WP-04): DirectX-COM-Schnittstellen als C-Strukturen.',
         '// Jede Schnittstelle: Objekt { lpVtbl } und Vtable-Struktur mit Methodennamen in Slot-Reihenfolge.',
         '#pragma once', '']
    for n in INTERFACES:
        L.append('typedef struct %s %s;' % (n, n))
    L.append('')
    for n in INTERFACES:
        L.append('typedef struct %sVtbl {' % n)
        for slot, sig, decl in methods(n):
            L.append('    void * %s; // +0x%X %s' % (method_name(sig), slot * 4, decl if decl != n else ''))
        L.append('} %sVtbl;' % n)
        L.append('struct %s { const %sVtbl * lpVtbl; };' % (n, n))
    open(OUT_H, 'w').write('\n'.join(L) + '\n')


def fmt(k):
    if k is None:
        return ''
    if k[0] == 'G':
        return 'G:0x%06x' % k[1]
    if k[0] == 'CF':
        return 'F:%s+0x%x' % (k[1], k[2])
    if k[0] == 'GF':
        return 'F:[0x%06x]+0x%x' % (k[1], k[2])
    if k[0] == 'RET':
        return 'RET:0x%06x' % k[1]
    return str(k)


if __name__ == '__main__':
    main()
