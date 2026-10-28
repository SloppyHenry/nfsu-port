#!/usr/bin/env python3
"""Liest die bWare-Winkeltabellen aus Speed.exe und schreibt port/src/game/base/bware_tables.cpp (WP-30.2).
bASin: 0x6e46b8, Eintrag 8 Byte {int16 winkel, int16 pad, float steigung}; bFixATan: 0x6e5148 und 0x6e5250 (uint16)."""
import os, struct, sys
sys.path.insert(0, os.path.dirname(__file__))
from pe_image import open_image
# Aufruf: gen_….py [Speed.exe] [Ausgabe.cpp] (Standard: game/pc/Speed.exe, Quellbaum)
REPO = os.path.join(os.path.dirname(__file__), "..")
EXE = sys.argv[1] if len(sys.argv) > 1 else os.path.join(REPO, "game", "pc", "Speed.exe")
OUT = sys.argv[2] if len(sys.argv) > 2 else os.path.join(REPO, "port", "build", "generated", "game", "base", "bware_tables.cpp")
e = open_image(EXE)
N_ASIN = 208  # Stufe 11 reicht bis Index 11*16+31
asin = [struct.unpack("<hhf", e.read(0x6E46B8 + 8 * i, 8)) for i in range(N_ASIN)]
fine = struct.unpack("<132H", e.read(0x6E5148, 264))
coarse = struct.unpack("<132H", e.read(0x6E5250, 264))
o = ["// Erzeugt von tools/gen_bware_tables.py aus Speed.exe. Nicht von Hand ändern.",
     '#include "game/base/bware.hpp"', "", "namespace nfsu {", "",
     f"// orig: 0x6e46b8, {N_ASIN} Einträge", f"const BAsinEntry kBAsinTable[{N_ASIN}] = {{"]
for a, p, s in asin:
    o.append(f"    {{{a}, {p}, 0x{struct.unpack('<I', struct.pack('<f', s))[0]:08x}u}},")
o.append("};")
for nm, adr, t in (("kBFixATanFine", 0x6E5148, fine), ("kBFixATanCoarse", 0x6E5250, coarse)):
    o.append(f"\n// orig: 0x{adr:x}\nconst uint16_t {nm}[132] = {{")
    for i in range(0, 132, 12):
        o.append("    " + ", ".join(str(x) for x in t[i:i + 12]) + ",")
    o.append("};")
o += ["", "}  // namespace nfsu", ""]
open(OUT, "w").write("\n".join(o))
