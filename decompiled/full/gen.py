#!/usr/bin/env python3
"""Erzeugt aus dem Ghidra-Export (out*/all_functions.c, functions.tsv, data.tsv)
ein C-Projekt unter build/ : ghidra.h, protos.h, data.h, src/part_NNNN.c

Datenzugriffe: Jedes Datensymbol wird als Makro auf die ORIGINALADRESSE abgebildet
(#define DAT_00732104 (*(undefined4*)0x00732104)). Das Zielprogramm muss das
Original-Datenabbild an dieser Adresse bereitstellen (siehe ../README_BUILD.md).

Aufruf: gen.py [exportdir] [builddir] [funcs_per_file]
"""
import re, sys, pathlib, collections

exp = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else 'out')
bld = pathlib.Path(sys.argv[2] if len(sys.argv) > 2 else 'build')
PER = int(sys.argv[3]) if len(sys.argv) > 3 else 150
(bld / 'src').mkdir(parents=True, exist_ok=True)

# ---------------------------------------------------------------- Funktionen
src = (exp / 'all_functions.c').read_text(errors='replace')
parts = re.split(r'^/\* ==== ([0-9a-f]+) (\S+) ==== \*/\n', src, flags=re.M)
funcs = []  # (addr, name, body)
for i in range(1, len(parts), 3):
    funcs.append((parts[i], parts[i + 1], parts[i + 2]))
import json
# Namen != FUN_<addr> (Bibliotheks-/C++-Namen aus Ghidra) auf FUN_<addr> normalisieren
ren = {}
for line in (exp / 'functions.tsv').read_text(errors='replace').splitlines():
    f = line.split('\t')
    if len(f) > 1 and f[1] != 'FUN_' + f[0]:
        ren['' + f[1]] = 'FUN_' + f[0]
CM = re.compile(r'/\*.*?\*/', re.S)
if ren:
    rx = re.compile('(?<![A-Za-z0-9_])(' + '|'.join(re.escape(k) for k in sorted(ren, key=len, reverse=True)) + ')(?![A-Za-z0-9_])')
funcs = [(a, 'FUN_' + a, (rx.sub(lambda m: ren[m.group(1)], CM.sub('', b)) if ren else CM.sub('', b))) for a, n, b in funcs]
json.dump(ren, open(bld / 'renamed.json', 'w'), indent=0)
fnames = {n for _, n, _ in funcs}

# ---------------------------------------------------------------- Daten
data_by_addr = {}
for line in (exp / 'data.tsv').read_text(errors='replace').splitlines():
    f = line.split('\t')
    if len(f) < 4 or ':' in f[0]:
        continue
    data_by_addr.setdefault(f[0], (f[1], f[2], int(f[3] or 0)))

TYPEMAP = {'undefined': 'undefined1', 'undefined1': 'undefined1', 'undefined2': 'undefined2',
           'undefined4': 'undefined4', 'undefined8': 'undefined8', 'byte': 'byte', 'uint': 'uint',
           'int': 'int', 'short': 'short', 'ushort': 'ushort', 'float': 'float', 'double': 'double',
           'char': 'char', 'pointer': 'undefined4', 'dword': 'uint', 'word': 'ushort', 'ulong': 'uint',
           'long': 'int'}

def ctype_for(addr):
    d = data_by_addr.get(addr)
    if not d:
        return 'undefined4', 4
    _, t, sz = d
    if t in TYPEMAP:
        return TYPEMAP[t], sz
    if t.endswith('*') or t.endswith('*32'):
        return 'undefined4', 4
    return None, sz  # Struktur/Feld -> Bytes

# ---------------------------------------------------------------- Bezeichner sammeln
ID = re.compile(r'\b((?:[A-Za-z]+_)*?)([0-9a-f]{8})\b')
sym_re = re.compile(r'\b((?:PTR_)?(?:_?DAT|s|u|PTR_DAT|PTR_s|PTR_u|PTR_LAB|PTR_FUN|DAT_EXT|FLOAT|DOUBLE|SUB|ptr|thunk_FUN|PTR_PTR)_[0-9a-f]{8})\b')
used = collections.OrderedDict()
for _, _, body in funcs:
    for m in sym_re.finditer(body):
        used[m.group(1)] = 1

defs = []
for s in used:
    base = s.lstrip('_')
    addr = s[-8:]
    if s.startswith('thunk_FUN') or s.startswith('PTR_FUN') or s.startswith('PTR_LAB'):
        pass
    t, sz = ctype_for(addr)
    if t is None:
        defs.append(f'#define {s} (*(uchar(*)[{max(sz,1)}])0x{addr})')
    else:
        defs.append(f'#define {s} (*({t} *)0x{addr})')
(bld / 'data.h').write_text('/* auto-generated: Datensymbole an Originaladressen */\n' + '\n'.join(defs) + '\n')

# ---------------------------------------------------------------- Prototypen
protos = []
for addr, name, body in funcs:
    lines = body.split('\n')
    sig = []
    started = False
    for ln in lines:
        if ln.startswith('/*') or ln.strip() == '' and not started:
            continue
        if ln.strip() == '{':
            break
        started = True
        sig.append(ln.strip())
    s = ' '.join(sig)
    if s and '(' in s and not s.startswith('//'):
        protos.append(s + ';')
    else:
        protos.append(f'void {name}();')
(bld / 'protos.h').write_text('/* auto-generated: Funktionsprototypen aus dem Dekompilat */\n' + '\n'.join(protos) + '\n')

# ---------------------------------------------------------------- ghidra.h
def concat_macros():
    out = []
    ut = {1: 'byte', 2: 'ushort', 3: 'uint', 4: 'uint', 5: 'ulonglong', 6: 'ulonglong', 7: 'ulonglong', 8: 'ulonglong'}
    for n in range(1, 8):
        for m in range(1, 9 - n):
            T = ut[n + m]
            out.append(f'#define CONCAT{n}{m}(a,b) ((({T})(a) << {8*m}) | ((ulonglong)(b) & ((1ULL << {8*m}) - 1)))')
    for n in range(2, 9):
        for m in range(0, n):
            T = ut[n - m] if n - m > 0 else 'byte'
            out.append(f'#define SUB{n}{n-m if False else 1}_PLACEHOLDER')
    return '\n'.join(l for l in out if 'PLACEHOLDER' not in l)

hdr = r'''/* auto-generated: Ghidra-Kompatibilitaet */
#ifndef GHIDRA_H
#define GHIDRA_H
#include <stddef.h>
typedef unsigned char undefined, undefined1, byte, uchar, bool8;
typedef unsigned short undefined2, ushort;
typedef unsigned int undefined3, undefined4, uint, ulong, dword;
typedef unsigned long long undefined5, undefined6, undefined7, undefined8, ulonglong;
typedef long long longlong;
typedef long double float10;
#include <stdbool.h>
typedef unsigned int DWORD, UINT, ULONG, ULONG_PTR, DWORD_PTR, SIZE_T, WPARAM, COLORREF, HRESULT, LCID, LANGID, ATOM;
typedef int LONG, BOOL, INT, LRESULT, LPARAM, INT_PTR, LONG_PTR;
typedef unsigned short WORD, USHORT, WCHAR;
typedef unsigned char BYTE, UCHAR;
typedef void *PVOID, *LPVOID, *HANDLE, *HWND, *HINSTANCE, *HMODULE, *HDC, *HGLRC, *HICON, *HCURSOR, *HBRUSH, *HMENU, *HKEY, *HGDIOBJ, *HBITMAP, *HPALETTE, *HFONT, *HRGN, *HMONITOR, *HHOOK, *HLOCAL, *HGLOBAL, *HRSRC, *HKL, *HWINSTA, *HDESK, *HINTERNET, *HLINE, *HLINEAPP, *HPHONE, *HPHONEAPP, *HACCEL, *HDWP, *HENHMETAFILE, *HIMC, *HDROP, *HDEVNOTIFY, *HSZ, *HCONV, *HCONVLIST, *HDDEDATA, *HRASCONN, *HFILE, *HTASK, *HWAVEOUT, *HMIXER, *HMIDIOUT, *HMMIO, *HWINEVENTHOOK, *HTHEME, *HHEAP, *HEVENT, *HUPDATE;
typedef const char *LPCSTR; typedef char *LPSTR, *PSTR, *LPTSTR; typedef const char *LPCTSTR;
typedef unsigned short *LPWSTR, *PWSTR; typedef const unsigned short *LPCWSTR;
typedef unsigned int *LPDWORD, *PDWORD; typedef int *LPLONG, *PLONG; typedef unsigned char *LPBYTE, *PBYTE; typedef unsigned short *LPWORD;
typedef BOOL *LPBOOL; typedef HANDLE *LPHANDLE;
typedef struct _EHRegistrationNode EHRegistrationNode;
typedef struct _EHExceptionRecord EHExceptionRecord;
typedef struct _s_FuncInfo _s_FuncInfo;
extern void *ExceptionList;  /* FS:[0], SEH-Kette */

typedef unsigned short word;
typedef void code();
typedef unsigned int pointer32;
#ifndef __cplusplus
#ifndef bool
typedef unsigned char bool;
#endif
#endif
typedef struct { unsigned char b[16]; } undefined16;
#define SUB41(x,i) ((byte)((uint)(x) >> (8*(i))))
#define SUB42(x,i) ((ushort)((uint)(x) >> (8*(i))))
#define SUB43(x,i) ((uint)(x) >> (8*(i)))
#define SUB81(x,i) ((byte)((ulonglong)(x) >> (8*(i))))
#define SUB82(x,i) ((ushort)((ulonglong)(x) >> (8*(i))))
#define SUB84(x,i) ((uint)((ulonglong)(x) >> (8*(i))))
#define SUB21(x,i) ((byte)((ushort)(x) >> (8*(i))))
#define SUB164(x,i) ((uint)0)
#define ZEXT14(x) ((uint)(byte)(x))
#define ZEXT24(x) ((uint)(ushort)(x))
#define ZEXT48(x) ((ulonglong)(uint)(x))
#define ZEXT12(x) ((ushort)(byte)(x))
#define SEXT14(x) ((int)(signed char)(x))
#define SEXT24(x) ((int)(short)(x))
#define SEXT48(x) ((longlong)(int)(x))
#define CARRY4(a,b) ((uint)(a) + (uint)(b) < (uint)(a))
#define CARRY2(a,b) ((ushort)((ushort)(a) + (ushort)(b)) < (ushort)(a))
#define SCARRY4(a,b) ((((int)(a) >= 0) == ((int)(b) >= 0)) && (((int)(a) + (int)(b) >= 0) != ((int)(a) >= 0)))
#define SBORROW4(a,b) ((((int)(a) >= 0) != ((int)(b) >= 0)) && (((int)(a) - (int)(b) >= 0) != ((int)(a) >= 0)))
#define POPCOUNT(x) __builtin_popcount((uint)(x))
#define halt_baddata() __builtin_trap()
#define halt_unimplemented() __builtin_trap()
#define swi(n) ((void)(n))
#define ROUND(x) ((int)((x) + 0.5))
#define LZCOUNT(x) __builtin_clz((uint)(x))
''' + concat_macros() + '\n#endif\n'
(bld / 'ghidra.h').write_text(hdr)

# ---------------------------------------------------------------- Quellen
n = 0
for i in range(0, len(funcs), PER):
    chunk = funcs[i:i + PER]
    out = ['#include "../ghidra.h"', '#include "../data.h"', '#include "../protos.h"', '']
    for addr, name, body in chunk:
        out.append(f'/* ==== {addr} {name} ==== */')
        out.append(body)
    (bld / 'src' / f'part_{n:04d}.c').write_text('\n'.join(out))
    n += 1
print(f'{len(funcs)} Funktionen, {len(used)} Datensymbole, {n} Quelldateien')
