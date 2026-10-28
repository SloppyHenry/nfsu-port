#!/usr/bin/env python3
"""Minimal-Leser für refs/gc/Speed.elf (PowerPC BE, GCC 2.95 SN): Abschnitte und Symbole.

Import:  from gc_elf import Elf;  e = Elf();  e.sections, e.symbols
"""
import os
import struct
from collections import namedtuple

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ELF_PATH = os.path.join(ROOT, 'refs', 'gc', 'Speed.elf')

Section = namedtuple('Section', 'idx name type addr offset size')
# bind: 0 local, 1 global, 2 weak · type: 0 notype, 1 object, 2 func, 3 section, 4 file
Symbol = namedtuple('Symbol', 'idx name value size bind type shndx')


class Elf:
    def __init__(self, path=ELF_PATH):
        self.data = d = open(path, 'rb').read()
        shoff, = struct.unpack('>I', d[0x20:0x24])
        shnum, shstrndx = struct.unpack('>HH', d[0x30:0x34])
        raw = [struct.unpack('>10I', d[shoff + i * 40:shoff + i * 40 + 40]) for i in range(shnum)]
        names = raw[shstrndx][4]
        self.sections = [Section(i, self._cstr(names + r[0]), r[1], r[3], r[4], r[5])
                         for i, r in enumerate(raw)]
        self.by_name = {s.name: s for s in self.sections}
        symtab = next(s for s in self.sections if s.type == 2)
        strtab = self.sections[raw[symtab.idx][6]].offset
        self.symbols = []
        for i in range(symtab.size // 16):
            o = symtab.offset + i * 16
            n, v, sz, info, _, sh = struct.unpack('>IIIBBH', d[o:o + 16])
            self.symbols.append(Symbol(i, self._cstr(strtab + n), v, sz, info >> 4, info & 15, sh))

    def _cstr(self, o):
        return self.data[o:self.data.index(b'\0', o)].decode('latin1')

    def section_bytes(self, name):
        s = self.by_name[name]
        return self.data[s.offset:s.offset + s.size]

    def section_of(self, sym):
        return self.sections[sym.shndx].name if 0 < sym.shndx < len(self.sections) else ''
