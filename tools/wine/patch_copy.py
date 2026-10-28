#!/usr/bin/env python3
"""Patcht die KOPIE refs/wine/nfsu-copy/Speed.exe so, dass sie unter macOS-Wine (GPTK 7.7) startet. Original in game/pc bleibt unberührt.
1. 0x407ac0: Windowed=1 statt [0x72b194] (macOS kennt weder 800x600 noch 1024x768 im Vollbild; D3D9-CreateDevice scheiterte, Spiel prüft das nicht)
2. Auflösungstabelle .data 0x6f31bc/0x6f31d0 (5 Breiten, 5 Höhen) auf Modi, die macdrv anbietet, sonst läuft FUN_0040fa50 über die Tabelle hinaus
Weitere Wine-Einstellung: HKCU\\Software\\Wine\\Drivers Audio="" (sonst Absturz beim Öffnen des Audiogeräts in DirectSound)."""
import struct, sys
p = sys.argv[1] if len(sys.argv) > 1 else 'refs/wine/nfsu-copy/Speed.exe'
d = bytearray(open(p, 'rb').read())
assert d[0x7ac0:0x7ac6].hex() == '8b1594b17200'
d[0x7ac0:0x7ac6] = bytes.fromhex('ba0100000090')   # mov edx,1 ; nop
pe = struct.unpack_from('<I', d, 0x3c)[0]; n = struct.unpack_from('<H', d, pe + 6)[0]
opt = struct.unpack_from('<H', d, pe + 20)[0]; sec = pe + 24 + opt
for i in range(n):
    name, vs, rva, rs, ro = struct.unpack_from('<8sIIII', d, sec + 40 * i)
    if rva <= 0x2f31bc < rva + rs: o = ro + 0x2f31bc - rva
for k, (w, h) in enumerate([(960, 600), (1280, 800), (1440, 900), (1680, 1050), (1920, 1200)]):
    struct.pack_into('<I', d, o + 4 * k, w); struct.pack_into('<I', d, o + 0x14 + 4 * k, h)
open(p, 'wb').write(d)
