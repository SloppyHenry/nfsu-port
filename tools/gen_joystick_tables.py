#!/usr/bin/env python3
"""Liest die Joystick-Tabellen aus Speed.exe (Scanner-Konfigurationen, gültige Konfigurationen) und schreibt
port/src/game/misc/joystick_tables.cpp. Liest die Abschnitte direkt aus der PE-Datei (WP-30.1)."""
import os, struct, sys
sys.path.insert(0, os.path.dirname(__file__))
from pe_image import PEImage
# Aufruf: gen_….py [Speed.exe] [Ausgabe.cpp] (Standard: game/pc/Speed.exe, Quellbaum)
REPO = os.path.join(os.path.dirname(__file__), "..")
EXE = sys.argv[1] if len(sys.argv) > 1 else os.path.join(REPO, "game", "pc", "Speed.exe")
OUT = sys.argv[2] if len(sys.argv) > 2 else os.path.join(REPO, "port", "build", "generated", "game", "misc", "joystick_tables.cpp")

KINDS = {0x553CE0: "SK_TypeChanged", 0x553D20: "SK_ButtonPress", 0x553D80: "SK_ButtonChange",
         0x553DD0: "SK_TwoButtonAxis", 0x553E60: "SK_AnalogRepeat", 0x554080: "SK_AnalogGraph"}
e = PEImage(EXE)

def cstr(addr):
    if addr == 0: return None
    b = bytearray()
    while len(b) < 64:
        c = e.read(addr + len(b), 1)[0]
        if c == 0: break
        b.append(c)
    return bytes(b).decode("latin1")

n = e.u32(0x6F62F0)
out = ["// Erzeugt von tools/gen_joystick_tables.py aus Speed.exe (Daten ab 0x6f5a28). Nicht von Hand ändern.",
       '#include "game/misc/joystick.hpp"', "", "namespace nfsu {", ""]
thr = list(e.read(0x6F5A28, 3))
out.append(f"const unsigned char kAnalogThresholds[3] = {{{thr[0]}, {thr[1]}, {thr[2]}}};  // 0x6f5a28..2a: Modus 1, 2, 0")
out.append(f"const int kNumScannerConfigs = {n};")
out.append("const ScannerConfig kScannerConfigs[] = {")
for i in range(n):
    b = e.read(0x6F5A30 + 0x20 * i, 0x20)
    cfgs = struct.unpack("<8b", b[:8])
    ev, fn = struct.unpack("<II", b[8:16])
    x1, x2, s1, s2, param, analog, valid = b[16], b[17], b[18], b[19], b[20], b[21], b[22]
    tex, nameptr = struct.unpack("<II", b[24:32])
    name = cstr(nameptr)
    nm = "nullptr" if name is None else '"' + name.replace("\\", "\\\\").replace('"', '\\"') + '"'
    out.append(f"    {{{{{', '.join(map(str, cfgs))}}}, {ev}, {KINDS[fn]}, {x1}, {x2}, {s1}, {s2}, {param}, {analog}, {valid}, 0x{tex:08x}u, {nm}}},")
out.append("};")
m = e.u32(0x6F6370)
out.append(f"\nconst int kNumValidConfigs = {m};")
out.append("const ValidJoystickConfiguration kValidConfigs[] = {")
for i in range(m):
    b = e.read(0x6F62F8 + 0x14 * i, 0x14)
    t = struct.unpack("<I", b[:4])[0]
    out.append(f"    {{{t}, {{{', '.join(str(x) for x in b[4:14])}}}, {{{', '.join(str(x) for x in b[14:20])}}}}},")
out.append("};")
out += ["", "}  // namespace nfsu", ""]
open(OUT, "w").write("\n".join(out))
print(n, m)
