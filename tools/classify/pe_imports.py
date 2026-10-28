"""Importtabelle von Speed.exe lesen (ohne Fremdpakete).

Ghidra legt Importe als EXTERNAL:<n> an, n = laufende Nummer ab 1 in
Reihenfolge der Import-Deskriptoren und Thunks (geprüft: EXTERNAL:000000fe =
DINPUT8!DirectInputCreateA, letzter Import).
"""
import struct

EXE = 'game/pc/Speed.exe'


def read_imports(path=EXE):
    d = open(path, 'rb').read()
    pe = struct.unpack_from('<I', d, 0x3C)[0]
    nsec, = struct.unpack_from('<H', d, pe + 6)
    optsz, = struct.unpack_from('<H', d, pe + 20)
    opt = pe + 24
    imp_rva, = struct.unpack_from('<I', d, opt + 96 + 8)
    secs = []
    for i in range(nsec):
        _, vsize, va, rawsize, rawptr = struct.unpack_from('<8sIIII', d, opt + optsz + i * 40)
        secs.append((va, max(vsize, rawsize), rawptr))

    def off(rva):
        for va, sz, raw in secs:
            if va <= rva < va + sz:
                return rva - va + raw
        raise ValueError(hex(rva))

    def cstr(rva):
        o = off(rva)
        return d[o:d.index(b'\0', o)].decode()

    out = []          # (dll, name)
    p = imp_rva
    while True:
        oft, _, _, name_rva, ft = struct.unpack_from('<IIIII', d, off(p))
        if name_rva == 0:
            break
        dll = cstr(name_rva).upper()
        t = oft or ft
        while True:
            ent, = struct.unpack_from('<I', d, off(t))
            if ent == 0:
                break
            out.append((dll, f'#{ent & 0xFFFF}' if ent & 0x80000000 else cstr((ent & 0x7FFFFFFF) + 2)))
            t += 4
        p += 20
    return out


def external_names(path=EXE):
    """'EXTERNAL:000000xx' -> (dll, name)"""
    return {f'EXTERNAL:{i + 1:08x}': x for i, x in enumerate(read_imports(path))}


if __name__ == '__main__':
    e = external_names()
    print(len(e), e['EXTERNAL:000000fe'], e['EXTERNAL:00000001'])
