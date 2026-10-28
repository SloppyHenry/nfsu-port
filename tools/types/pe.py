#!/usr/bin/env python3
"""Minimal-Leser für game/pc/Speed.exe (nur lesen): Abschnitte als Abbild, Importe (IAT -> Name).

Import:  from pe import Pe;  p = Pe();  p.u32(va), p.read(va, n), p.section_of(va), p.imports[iat_va]
"""
import os
import struct

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
EXE = os.path.join(ROOT, 'game', 'pc', 'Speed.exe')


class Pe:
    def __init__(self, path=EXE):
        self.data = d = open(path, 'rb').read()
        pe = struct.unpack_from('<I', d, 0x3C)[0]
        nsec, = struct.unpack_from('<H', d, pe + 6)
        optsz, = struct.unpack_from('<H', d, pe + 20)
        opt = pe + 24
        self.base, = struct.unpack_from('<I', d, opt + 28)
        imp_rva, = struct.unpack_from('<I', d, opt + 96 + 8)
        self.sections = []   # (name, va, vsize, bytes)
        for i in range(nsec):
            o = opt + optsz + i * 40
            name = d[o:o + 8].rstrip(b'\0').decode()
            vsz, va, rsz, raw = struct.unpack_from('<IIII', d, o + 8)
            img = d[raw:raw + min(vsz, rsz)].ljust(vsz, b'\0')
            self.sections.append((name, self.base + va, vsz, img))
        self.imports = {}    # IAT-Adresse -> "DLL!Name"
        o = imp_rva
        while True:
            oft, _, _, name_rva, ft = struct.unpack_from('<IIIII', self.read(self.base + o, 20))
            if not ft:
                break
            dll = self.cstr(self.base + name_rva)
            k = 0
            while True:
                thunk = self.u32(self.base + (oft or ft) + k * 4)
                if not thunk:
                    break
                nm = '#%d' % (thunk & 0xffff) if thunk & 0x80000000 else self.cstr(self.base + thunk + 2)
                self.imports[self.base + ft + k * 4] = dll + '!' + nm
                k += 1
            o += 20

    def section_of(self, va):
        for name, sva, vsz, _ in self.sections:
            if sva <= va < sva + vsz:
                return name
        return None

    def read(self, va, n):
        for _, sva, vsz, img in self.sections:
            if sva <= va < sva + vsz:
                return img[va - sva:va - sva + n]
        raise KeyError(hex(va))

    def u32(self, va):
        return struct.unpack('<I', self.read(va, 4))[0]

    def cstr(self, va):
        b = self.read(va, 256)
        return b[:b.index(b'\0')].decode('latin1')

    def words(self, name):
        """(Startadresse, Tupel aller Dwords) eines Abschnitts."""
        for n, sva, vsz, img in self.sections:
            if n == name:
                return sva, struct.unpack('<%dI' % (vsz // 4), img[:vsz // 4 * 4])
        raise KeyError(name)
