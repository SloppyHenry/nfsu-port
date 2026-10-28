#!/usr/bin/env python3
"""Property-Tests: Originalfunktion (Unicorn) gegen Port (libnfsu_core.dylib).

Aufruf:  tools/emu/.venv/bin/python tools/emu/compare.py [--n 10000] [--seed 1] [name ...]
Neue Funktion: eine Funktion `case_<name>(emu, lib, rng)` schreiben, die `check(cond, info)` aufruft,
und sie in CASES eintragen. Die erste Abweichung wird mit Eingabe ausgegeben.
"""
import argparse
import ctypes
import os
import random
import struct
import sys
import time

sys.path.insert(0, os.path.dirname(__file__))
from nfsu_emu import Emu, REPO, f2i  # noqa: E402

DYLIB = os.environ.get("NFSU_CORE_DYLIB", os.path.join(REPO, "port", "build", "release", "libnfsu_core.dylib"))


class Mismatch(Exception):
    pass


def check(cond, info):
    if not cond:
        raise Mismatch(info)


# ------------------------------------------------------------------ name_hash
def case_name_hash(emu, lib, rng):
    n = rng.choice([0, 1, 2, 3, 8, 16, 33, 64])
    s = bytes(rng.randint(1, 255) for _ in range(n))
    addr = emu.alloc(s + b"\0")
    emu_h = emu.call(0x59AA50, "cdecl", [addr]).eax
    port_h = lib.t_name_hash(s + b"\0")
    check(emu_h == port_h, f"name_hash({s!r}): Original 0x{emu_h:08x}, Port 0x{port_h:08x}")


# ------------------------------------------------------------------ reset_identity_matrix
def case_reset_identity_matrix(emu, lib, rng):
    base = 0x77D7E0
    junk = bytes(rng.getrandbits(8) for _ in range(128))
    emu.write(base, junk)
    emu.call(0x40A7C0, "cdecl", [])
    orig = emu.read(base, 128)
    buf = ctypes.create_string_buffer(junk, 128)
    lib.t_reset_identity_matrix(ctypes.byref(buf, 0), ctypes.byref(buf, 64))
    check(orig == buf.raw, f"Original {orig.hex()} != Port {buf.raw.hex()}")


# ------------------------------------------------------------------ jdlz_decompress
def jdlz_encode(rng, out_size_hint):
    """Erzeugt (strom, erwartete_ausgabe) mit gültigem JDLZ-v2-Aufbau (Zufallsentscheidungen)."""
    out = bytearray()
    data = bytearray()          # Bytes ab +0x12 (inkl. Flag-Nachladebytes)
    a_bits, b_bits = [], []     # Flags des aktuellen Bytes
    head = bytearray(2)         # Flag-Bytes im Kopf (+0x10/+0x11)
    a_slot, b_slot = "headA", "headB"   # Ablageort des aktuellen Flag-Bytes (Kopf oder Index in data)

    def store(slot, bits):
        v = sum(b << i for i, b in enumerate(bits))
        if slot == "headA":
            head[0] = v
        elif slot == "headB":
            head[1] = v
        else:
            data[slot] = v

    while len(out) < out_size_hint:
        copy = len(out) > 0x20 and rng.random() < 0.5
        if copy:
            use_b = rng.random() < 0.5
            if use_b:                                    # Form B=1: Abstand 1..16
                dist = rng.randint(1, min(16, len(out)))
                ln = rng.choice([3, 4, 10, 0x22, 0x100, rng.randint(3, 0xFFF + 3)]) if rng.random() < 0.3 \
                    else rng.randint(3, 40)
                ln = min(ln, 0xFFF + 3)
                v = ln - 3
                b0 = ((v >> 4) & 0xF0) | (dist - 1)
                b1 = v & 0xFF
            else:                                        # Form B=0: Abstand 0x11..0x7FF+0x11
                if len(out) < 0x11:
                    continue
                dist = rng.randint(0x11, min(0x7FF + 0x11, len(out)))
                ln = rng.randint(3, 0x22)
                d = dist - 0x11
                b0 = ((d >> 3) & 0xE0) | (ln - 3)
                b1 = d & 0xFF
            for _ in range(ln):
                out.append(out[-dist])
            data += bytes([b0, b1])
            a_bits.append(1)
            b_bits.append(1 if use_b else 0)
        else:
            byte = rng.getrandbits(8)
            out.append(byte)
            data.append(byte)
            a_bits.append(0)
        # Nachladen genau wie der Dekoder (A zuerst, dann B)
        if len(a_bits) == 8:
            store(a_slot, a_bits)
            a_bits = []
            a_slot = len(data)
            data.append(0)
        if len(b_bits) == 8:
            store(b_slot, b_bits)
            b_bits = []
            b_slot = len(data)
            data.append(0)
    store(a_slot, a_bits)
    store(b_slot, b_bits)
    total = 0x12 + len(data)
    stream = b"JDLZ" + bytes([2, 0, 0, 0]) + struct.pack("<II", len(out), total) + bytes(head) + bytes(data)
    return stream, bytes(out)


PAD = 0x4000


def case_jdlz_decompress(emu, lib, rng):
    size = rng.choice([1, 5, 50, 300, 2000, 6000])
    stream, expect = jdlz_encode(rng, size)
    n = len(expect)
    src = emu.alloc(stream + b"\0" * 8)
    dst_region = emu.alloc(PAD + n + PAD)
    ret = emu.call(0x59DD70, "cdecl", [src, dst_region + PAD]).eax
    orig_out = emu.read(dst_region, PAD + n + PAD)
    hbuf = ctypes.create_string_buffer(PAD + n + PAD)
    sbuf = ctypes.create_string_buffer(stream + b"\0" * 8, len(stream) + 8)
    pret = lib.t_jdlz_decompress(sbuf, ctypes.byref(hbuf, PAD))
    check(ret == pret == n, f"Rückgabe Original {ret}, Port {pret}, erwartet {n}")
    check(orig_out[PAD:PAD + n] == expect, "Original weicht von Python-Referenz ab (Encoder-Fehler?)")
    check(hbuf.raw == orig_out, f"Port weicht vom Original ab (Strom {stream[:40].hex()}…)")


def case_jdlz_bad_header(emu, lib, rng):
    hdr = bytearray(rng.getrandbits(8) for _ in range(0x20))
    if rng.random() < 0.5:
        hdr[0:4] = b"JDLZ"          # Magic stimmt, Version meist nicht
    if hdr[4] == 2 and hdr[0:4] == b"JDLZ":
        return  # gültiger Kopf mit Zufallsdaten: Dekoder läuft unbegrenzt, nicht vergleichbar
    src = emu.alloc(bytes(hdr))
    dst = emu.alloc(0x100)
    ret = emu.call(0x59DD70, "cdecl", [src, dst]).eax
    sbuf = ctypes.create_string_buffer(bytes(hdr), 0x20)
    dbuf = ctypes.create_string_buffer(0x100)
    pret = lib.t_jdlz_decompress(sbuf, dbuf)
    check(ret == pret == 0, f"falscher Kopf: Original {ret}, Port {pret}")


# ------------------------------------------------------------------ WP-30.1: Timer, tCubic1D
def case_timer_hms(emu, lib, rng):
    packed = rng.choice([rng.randint(0, 4000 * 3600 * 99), rng.randint(0, 4000 * 100), 0, 3999, 4000])
    obj = emu.alloc(struct.pack("<i", packed))
    outs = emu.alloc(16)
    emu.call(0x562600, "cdecl", [outs, outs + 4, outs + 8, outs + 12], regs={"eax": obj})
    orig = struct.unpack("<4i", emu.read(outs, 16))
    buf = (ctypes.c_int * 4)()
    lib.t_timer_hms(packed, buf)
    check(tuple(buf) == orig, f"hms({packed}): Original {orig}, Port {tuple(buf)}")


def case_timer_round(emu, lib, rng):
    packed = rng.randint(0, 4000 * 3600)
    iota = rng.choice([0.01, 0.05, 0.1, 0.25, 0.5, 1.0, rng.uniform(0.001, 2.0)])
    obj = emu.alloc(struct.pack("<i", packed))
    r = emu.call(0x5627A0, "cdecl", [f2i(iota)], regs={"eax": obj})
    port = lib.t_timer_rounded(packed, iota)
    q = packed * 0.00025 / iota
    near_tie = abs((q - int(q)) - 0.5) < q * 3e-7 + 1e-5  # Rundung auf 0,5: float und x87 dürfen verschieden entscheiden
    # Emulator rechnet x87 mit 53 Bit (FPCW 0x27F), der Port mit float: bCeil kann eine Rasterstufe (1/4000 s) abweichen
    check(abs(r.st0 - port) <= 1.5 / 4000 + (iota * 1.01 if near_tie else 0), f"round({packed}, {iota}): Original {r.st0}, Port {port}")


def case_cubic1d_update(emu, lib, rng):
    state = rng.choice([0, 1, 2, 2, 2])
    f = [rng.uniform(-50, 50) for _ in range(4)]
    coeff = [rng.uniform(-20, 20) for _ in range(4)]
    time_ = rng.uniform(0, 1.2)
    duration = rng.choice([rng.uniform(0.1, 5.0), 0.0, 1e-6])
    thresh = rng.choice([1.0, 1.0, 0.8, 1.2])
    flags = rng.choice([0, 0, 1])
    blob = struct.pack("<4f4f3fhh", *f, *coeff, time_, duration, thresh, state, flags)
    sec, dcl, ddcl = rng.uniform(0, 0.5), rng.choice([0.0, rng.uniform(1, 60)]), rng.choice([0.0, rng.uniform(1, 200)])
    obj = emu.alloc(blob)
    emu.call(0x56EF60, "thiscall", [f2i(sec), f2i(dcl), f2i(ddcl)], this=obj)
    orig = struct.unpack("<4f4f3fhh", emu.read(obj, 48))
    buf = ctypes.create_string_buffer(blob, 48)
    lib.t_cubic1d_update(buf, ctypes.c_float(sec), ctypes.c_float(dcl), ctypes.c_float(ddcl))
    port = struct.unpack("<4f4f3fhh", buf.raw)
    ok = orig[-2:] == port[-2:] and all(
        abs(a - b) <= 2e-3 * max(1.0, abs(a), abs(b)) or (a != a and b != b) for a, b in zip(orig[:-2], port[:-2]))
    check(ok, f"Update({sec:.3f},{dcl:.2f},{ddcl:.2f}) Zustand {blob.hex()}:\n Original {orig}\n Port     {port}")


# ------------------------------------------------------------------ WP-30.1: Rumble
def _rand_env(rng):
    n = rng.randint(2, 6)
    xs = sorted(rng.sample(range(0, 40), n))
    return [(float(x) / 4, float(rng.randint(0, 255))) for x in xs]


def case_envelope_value(emu, lib, rng):
    pts = _rand_env(rng)
    flat = [v for p in pts for v in p]
    pbuf = emu.alloc(struct.pack(f"<{len(flat)}f", *flat))
    env = emu.alloc(struct.pack("<II", pbuf, len(pts)))
    x = rng.choice([rng.uniform(-1, 11), pts[0][0], pts[-1][0], rng.choice(pts)[0]])
    r = emu.call(0x554440, "thiscall", [f2i(x)], this=env)
    arr = (ctypes.c_float * len(flat))(*flat)
    port = lib.t_envelope_value(arr, len(pts), ctypes.c_float(x))
    check(abs(r.st0 - port) <= 1e-3 * max(1.0, abs(port)), f"GetValue({x}) pts={pts}: Original {r.st0}, Port {port}")


def case_rumble_start(emu, lib, rng):
    envs = [_rand_env(rng) for _ in range(3)]
    flat = [v for e in envs for p in e for v in p]
    ns = [len(e) for e in envs]
    pbufs, off = [], 0
    for e in envs:
        pbufs.append(emu.alloc(struct.pack(f"<{len(e) * 2}f", *[v for p in e for v in p])))
    eptr = [emu.alloc(struct.pack("<II", pbufs[i], ns[i])) for i in range(3)]
    st_env = [rng.choice([0, 1, 2, 3]) for _ in range(3)]
    st_time = [rng.uniform(0, 8) if st_env[i] else 0.0 for i in range(3)]
    st_dur = [rng.choice([0.5, 1.0, 2.0, 3.0, 0.25]) for _ in range(3)]
    st_prio = [rng.choice([-1, 0, 1, 2, 3]) for _ in range(3)]
    new_env, dur, prio = rng.randint(0, 2), rng.choice([0.5, 1.0, 2.0, 3.0]), rng.choice([0, 1, 2, 3])
    blob = struct.pack("<3I3f3f3b", *[eptr[e - 1] if e else 0 for e in st_env], *st_time, *st_dur, *st_prio)
    obj = emu.alloc(blob + b"\0" * 16)
    r = emu.call(0x5544F0, "stdcall", [eptr[new_env], f2i(dur), prio & 0xFF], regs={"edi": obj})
    o = struct.unpack("<3I3f3f3b", emu.read(obj, len(blob)))
    orig_env = [([0] + eptr).index(v) if v in eptr else 0 for v in o[0:3]]
    ok_orig = r.eax & 0xFF
    to, do, po, eo = (ctypes.c_float * 3)(), (ctypes.c_float * 3)(), (ctypes.c_byte * 3)(), (ctypes.c_int * 3)()
    fa = (ctypes.c_float * len(flat))(*flat)
    na = (ctypes.c_int * 3)(*ns)
    se = (ctypes.c_int * 3)(*st_env)
    stt = (ctypes.c_float * 3)(*st_time)
    sd = (ctypes.c_float * 3)(*st_dur)
    sp = (ctypes.c_byte * 3)(*st_prio)
    ok_port = lib.t_rumble_start(fa, na, 3, se, stt, sd, sp, new_env, ctypes.c_float(dur), prio, to, do, po, eo)
    same = (ok_orig != 0) == (ok_port != 0) and list(eo) == orig_env and list(po) == list(o[9:12]) \
        and all(abs(a - b) < 1e-6 for a, b in zip(to, o[3:6])) and all(abs(a - b) < 1e-6 for a, b in zip(do, o[6:9]))
    check(same, f"StartRumble Zustand env={st_env} prio={st_prio} dur={st_dur} neu=({new_env},{dur},{prio}): "
                f"Original ok={ok_orig} env={orig_env} prio={list(o[9:12])}, Port ok={ok_port} env={list(eo)} prio={list(po)}")


# ------------------------------------------------------------------ WP-30.1: Joystick-Scanner
SCANNERS = [0x553CE0, 0x553D20, 0x553D80, 0x553DD0, 0x553E60, 0x554080]


def _scan_setup(emu, rng, kind):
    a = [rng.getrandbits(32) for _ in range(8)]
    b = [rng.getrandbits(32) if rng.random() < 0.5 else a[i] for i in range(8)]
    xor1, xor2 = rng.randint(0, 1), rng.randint(0, 1)
    s1, s2 = rng.randint(1, 120), rng.choice([0, rng.randint(1, 120)])
    param = rng.choice([0, 255, 1]) if kind in (1, 2) else rng.choice([0, 1, 2])
    analog, valid = rng.randint(0, 3), 0
    if kind == 3:
        s2 = rng.randint(1, 120)
    if kind == 5:
        s2 = rng.choice([0, s2])
    event = rng.randint(1, 170)
    last = rng.choice([rng.getrandbits(16), rng.getrandbits(8), 0])
    fresh = rng.choice([0, 0, 0, 1])
    frame = rng.randint(100, 5000)
    repeat = frame + rng.randint(-10, 30)
    jtype = rng.randint(0, 6)
    pts = [0.0, 0.0, rng.uniform(100, 127), 128.0, rng.uniform(128, 160), 128.0, 255.0, 255.0]
    hyst = rng.choice([0, 5, 12])
    return dict(a=a, b=b, xor1=xor1, xor2=xor2, s1=s1, s2=s2, param=param, analog=analog, valid=valid, event=event,
                last=last, fresh=fresh, frame=frame, repeat=repeat, jtype=jtype, pts=pts, hyst=hyst)


def case_joy_scan(emu, lib, rng):
    kind = rng.randint(0, 5)
    c = _scan_setup(emu, rng, kind)
    node = bytearray(0x24)
    struct.pack_into("<IIII", node, 0x8, c["event"], 0, 0, 0)
    struct.pack_into("<III", node, 0x18, c["last"], c["fresh"], c["repeat"])
    naddr = emu.alloc(bytes(node))
    aaddr = emu.alloc(struct.pack("<8I", *c["a"]))
    baddr = emu.alloc(struct.pack("<8I", *c["b"]))
    cfg = bytearray(0x20)
    cfg[0x10:0x17] = bytes([c["xor1"], c["xor2"], c["s1"], c["s2"], c["param"], c["analog"], c["valid"]])
    caddr = emu.alloc(bytes(cfg))
    joy = bytearray(0x18C)
    jaddr = emu.alloc(bytes(joy))
    struct.pack_into("<i", joy, 0x17C, c["jtype"])
    for ax in range(4):
        base = 8 + 0x2C * ax
        struct.pack_into("<8f", joy, base, *c["pts"])
        struct.pack_into("<II", joy, base + 0x20, jaddr + base, 4)
        joy[base + 0x28] = c["hyst"]
    emu.write(jaddr, bytes(joy))
    emu.write(0x7295A4, struct.pack("<i", c["frame"]))
    r = emu.call(SCANNERS[kind], "cdecl", [naddr, aaddr, baddr, caddr, jaddr])
    orig = (r.eax & 0xFFFF, struct.unpack("<IIi", emu.read(naddr + 0x18, 12)))
    nd = (ctypes.c_int * 4)(c["event"], c["last"], c["fresh"], c["repeat"])
    cb = (ctypes.c_ubyte * 7)(c["xor1"], c["xor2"], c["s1"], c["s2"], c["param"], c["analog"], c["valid"])
    pa, pb = (ctypes.c_uint * 8)(*c["a"]), (ctypes.c_uint * 8)(*c["b"])
    pp = (ctypes.c_float * 8)(*c["pts"])
    res = lib.t_joy_scan(kind, nd, cb, pa, pb, c["jtype"], pp, c["hyst"], c["frame"]) & 0xFFFF
    port = (res, (nd[1] & 0xFFFFFFFF, nd[2], nd[3]))
    ok = res == orig[0] and port[1][1] == orig[1][1] and (port[1][0] == orig[1][0]) and (port[1][2] == orig[1][2] or orig[0] == 0 and kind != 4)
    check(ok, f"Scanner {kind} {c}: Original {orig}, Port {port}")


def case_button_pair(emu, lib, rng):
    c = _scan_setup(emu, rng, 1)
    cfg = bytearray(0x20)
    cfg[0x10:0x14] = bytes([c["xor1"], c["xor2"], c["s1"], c["s2"]])
    caddr = emu.alloc(bytes(cfg))
    aaddr = emu.alloc(struct.pack("<8I", *c["a"]))
    baddr = emu.alloc(struct.pack("<8I", *c["b"]))
    orig = emu.call(0x553C20, "cdecl", [caddr, aaddr, baddr]).eax
    cb = (ctypes.c_ubyte * 4)(c["xor1"], c["xor2"], c["s1"], c["s2"])
    port = lib.t_button_pair(cb, (ctypes.c_uint * 8)(*c["a"]), (ctypes.c_uint * 8)(*c["b"]))
    check(orig == port, f"ReadButtonPair {c}: Original {orig}, Port {port}")


# ------------------------------------------------------------------ WP-30.1: Zeitgeber
CLK_F = [0x72CA1C, 0x7295B0, 0x7295B4, 0x7295C0, 0x7295C4]
CLK_I = [0x6E30B8, 0x72CA20, 0x7295B8, 0x6E30CC, 0x72CA18, 0x6E30D0]


def case_clock(emu, lib, rng):
    which = rng.randint(0, 2)
    f = [rng.choice([0.0, rng.uniform(0, 0.2)]), rng.uniform(0, 0.0167), rng.uniform(-0.001, 0.001),
         rng.choice([0.0, rng.uniform(0, 0.2)]), rng.uniform(0, 0.0167)]
    iv = [rng.randint(1, 100000), rng.randint(0, 10 ** 7), rng.randint(0, 10 ** 5),
          rng.randint(1, 100000), rng.randint(0, 10 ** 7), rng.randint(0, 10 ** 5), rng.randint(0, 1)]
    arg = rng.choice([rng.uniform(0, 0.2), 0.0166, 0.0, 0.5])
    for a, v in zip(CLK_F, f):
        emu.write(a, struct.pack("<f", v))
    for a, v in zip(CLK_I, iv):
        emu.write(a, struct.pack("<i", v))
    emu.write(0x6E30B0, struct.pack("<i", iv[6]))
    emu.write(0x7295E0, b"\0" * 8)
    if which == 0:
        emu.call(0x561D30, "cdecl", [f2i(arg)])
    elif which == 1:
        emu.call(0x561E80, "cdecl", [])
    else:
        emu.call(0x561F10, "cdecl", [])
    of = [struct.unpack("<f", emu.read(a, 4))[0] for a in CLK_F]
    oi = [struct.unpack("<i", emu.read(a, 4))[0] for a in CLK_I]
    oe = [struct.unpack("<i", emu.read(a, 4))[0] for a in (0x6E30BC, 0x7295BC, 0x7295AC)]
    pf = (ctypes.c_float * 5)(*f)
    pi = (ctypes.c_int * 10)(*iv, 0, 0, 0)
    lib.t_clock(which, pf, pi, ctypes.c_float(arg))
    ok = all(abs(a - b) <= 1e-5 * max(1.0, abs(a)) for a, b in zip(of, pf)) and list(pi[:6]) == oi
    if which == 0:
        ok = ok and pi[9] == oe[2]
    check(ok, f"Zeitgeber {which} f={f} i={iv} arg={arg}: Original {of} {oi} {oe}, Port {list(pf)} {list(pi)}")


def _cubic_blob(rng):
    state = rng.choice([0, 1, 2, 2, 2])
    f = [rng.uniform(-50, 50) for _ in range(4)]
    coeff = [rng.uniform(-20, 20) for _ in range(4)]
    return struct.pack("<4f4f3fhh", *f, *coeff, rng.uniform(0, 1.2), rng.choice([rng.uniform(0.1, 5.0), 0.0, 1e-6]),
                       rng.choice([1.0, 1.0, 0.8, 1.2]), state, rng.choice([0, 0, 1]))


def _cubic_close(x, y):
    ux = struct.unpack("<4f4f3fhh", x)
    uy = struct.unpack("<4f4f3fhh", y)
    return ux[-2:] == uy[-2:] and all(a == b or abs(a - b) <= 2e-3 * max(1.0, abs(a), abs(b)) or (a != a and b != b)
                                      for a, b in zip(ux[:-2], uy[:-2]))


def case_splineseek1d(emu, lib, rng):
    blob = _cubic_blob(rng)
    sec, dcl, ddcl = rng.uniform(0, 0.5), rng.choice([0.0, rng.uniform(1, 60)]), rng.choice([0.0, rng.uniform(1, 200)])
    obj = emu.alloc(blob)
    emu.call(0x56F0E0, "thiscall", [f2i(sec), f2i(dcl), f2i(ddcl)], this=obj)
    orig = emu.read(obj, 48)
    buf = ctypes.create_string_buffer(blob, 48)
    lib.t_splineseek1d(buf, ctypes.c_float(sec), ctypes.c_float(dcl), ctypes.c_float(ddcl))
    check(_cubic_close(orig, buf.raw), f"SplineSeek({sec:.3f},{dcl:.2f},{ddcl:.2f}) {blob.hex()}:\n {struct.unpack('<4f4f3fhh', orig)}\n {struct.unpack('<4f4f3fhh', buf.raw)}")


def case_splineseek3d(emu, lib, rng):
    blob = b"".join(_cubic_blob(rng) for _ in range(3))
    sec = rng.uniform(0, 0.5)
    obj = emu.alloc(blob)
    emu.call(0x56F1E0, "cdecl", [f2i(sec)], regs={"esi": obj})
    orig = emu.read(obj, 144)
    buf = ctypes.create_string_buffer(blob, 144)
    lib.t_splineseek3d(buf, ctypes.c_float(sec))
    ok = all(_cubic_close(orig[i * 48:(i + 1) * 48], buf.raw[i * 48:(i + 1) * 48]) for i in range(3))
    check(ok, f"SplineSeek3D({sec:.3f}) {blob.hex()}")


# ------------------------------------------------------------------ WP-30.2: bWare
def _a16(x):
    return x & 0xFFFF


def _circ_close(a, b, tol=1):
    d = (a - b) & 0xFFFF
    return d <= tol or d >= 0x10000 - tol


def _rf(rng):
    return rng.choice([0.0, rng.uniform(-1, 1), rng.uniform(-100, 100), rng.uniform(-1e-3, 1e-3), 1.0, -1.0])


def case_bsin(emu, lib, rng):
    a = rng.randint(0, 0xFFFF)
    r = emu.call(0x546EA0, "cdecl", [a]).st0
    p = lib.t_bsin(a)
    check(abs(r - p) <= 3e-6, f"bSin({a}): Original {r}, Port {p}")


def case_bsincos(emu, lib, rng):
    a = rng.randint(0, 0xFFFF)
    out = emu.alloc(8)
    emu.call(0x546F60, "fastcall", [out, out + 4], regs={"eax": a})
    o = struct.unpack("<2f", emu.read(out, 8))
    buf = (ctypes.c_float * 2)()
    lib.t_bsincos(a, buf)
    check(all(abs(x - y) <= 3e-6 for x, y in zip(o, buf)), f"bSinCos({a}): Original {o}, Port {tuple(buf)}")


def case_basin(emu, lib, rng):
    v = rng.choice([rng.uniform(-1.2, 1.2), rng.uniform(-1, 1), rng.uniform(-0.01, 0.01), 0.0, 1.0, -1.0, 0.5, rng.uniform(0.9, 1)])
    r = emu.call(0x547060, "cdecl", [f2i(v)]).eax
    p = lib.t_basin(ctypes.c_float(v))
    check(_circ_close(_a16(r), _a16(p)), f"bASin({v}): Original {_a16(r)}, Port {_a16(p)}")


def case_batan(emu, lib, rng):
    y, x = _rf(rng), _rf(rng)
    r = emu.call(0x547140, "cdecl", [f2i(y), f2i(x)]).eax
    p = lib.t_batan(ctypes.c_float(y), ctypes.c_float(x))
    check(_circ_close(_a16(r), _a16(p), 2), f"bATan({y}, {x}): Original {_a16(r)}, Port {_a16(p)}")


def _ri(rng):
    return rng.choice([0, rng.randint(-0x2000000, 0x2000000), rng.randint(-0x40000, 0x40000), rng.randint(-0x300000, 0x300000),
                       rng.randint(-0x7FFFFFFF, 0x7FFFFFFF), 1, -1, 0x200000, 0x40000, 0x1000000])


def case_bfixatan1(emu, lib, rng):
    v = _ri(rng)
    r = emu.call(0x547290, "cdecl", [], regs={"eax": v}).eax
    p = lib.t_bfixatan1(v) & 0xFFFFFFFF
    check(r == p, f"bFixATan({v}): Original {r:#x}, Port {p:#x}")


def case_bfixatan2(emu, lib, rng):
    x, y = _ri(rng), _ri(rng)
    r = emu.call(0x547330, "cdecl", [], regs={"eax": x, "ecx": y}).eax
    p = lib.t_bfixatan2(x, y) & 0xFFFFFFFF
    check(r == p, f"bFixATan({x}, {y}): Original {r:#x}, Port {p:#x}")


def case_brandom(emu, lib, rng):
    st = rng.getrandbits(32)
    n = rng.choice([1, 2, 7, 100, 0x7FFFFFFF, rng.randint(1, 0xFFFFFFFF)])
    emu.write(0x6E46B4, struct.pack("<I", st))
    r = emu.call(0x546DE0, "cdecl", [n]).eax
    after = emu.u32(0x6E46B4)
    cs = ctypes.c_uint(st)
    p = lib.t_brandom(ctypes.byref(cs), n)
    check(r == p and after == cs.value, f"bRandom({n}) Zustand {st:#x}: Original {r},{after:#x}, Port {p},{cs.value:#x}")


def case_brandomf(emu, lib, rng):
    st = rng.getrandbits(32)
    sc = rng.choice([1.0, rng.uniform(0, 100), 360.0, rng.uniform(-10, 10)])
    emu.write(0x6E46B4, struct.pack("<I", st))
    r = emu.call(0x546E10, "cdecl", [f2i(sc)]).st0
    after = emu.u32(0x6E46B4)
    cs = ctypes.c_uint(st)
    p = lib.t_brandomf(ctypes.byref(cs), ctypes.c_float(sc))
    check(abs(r - p) <= 1e-6 * max(1, abs(r)) and after == cs.value, f"bRandomf({sc}) {st:#x}: Original {r},{after:#x}, Port {p},{cs.value:#x}")


def _rstr(rng, alpha=b"abcXYZ019 _-"):
    n = rng.choice([0, 1, 2, 3, 5, 8])
    return bytes(rng.choice(alpha) if rng.random() < 0.85 else rng.randint(1, 255) for _ in range(n))


def _str_pair(rng):
    a = _rstr(rng)
    b = a if rng.random() < 0.4 else _rstr(rng)
    if rng.random() < 0.3:
        b = a.swapcase()
    if rng.random() < 0.2:
        b = a + _rstr(rng)
    return a, b


def case_strcmp(emu, lib, rng):
    a, b = _str_pair(rng)
    pa, pb = emu.alloc(a + b"\0"), emu.alloc(b + b"\0")
    for which, addr, args in ((0, 0x59AB30, ()), (2, 0x59AC20, ())):
        r = emu.call(addr, "cdecl", [pa, pb]).eax
        r = r - (1 << 32) if r & 0x80000000 else r
        p = lib.t_strfn(which, a + b"\0", b + b"\0", 0)
        check(r == p, f"strfn{which}({a!r}, {b!r}): Original {r}, Port {p}")
    n = rng.randint(0, 6)
    if not a and not b and n:
        n = 0  # Original liest a[-1] bei zwei leeren Zeichenketten (Undefiniertes)
    r = emu.call(0x59AB60, "cdecl", [pa, pb, n]).eax
    r = r - (1 << 32) if r & 0x80000000 else r
    p = lib.t_strfn(1, a + b"\0", b + b"\0", n)
    check(r == p, f"bStrNCmp({a!r}, {b!r}, {n}): Original {r}, Port {p}")
    seed = rng.choice([0, -1, rng.getrandbits(32)])
    r = emu.call(0x59AA80, "cdecl", [pa, seed]).eax
    p = lib.t_strfn(3, a + b"\0", b"", seed) & 0xFFFFFFFF
    check(r == p, f"bStringHash({a!r}, {seed}): Original {r:#x}, Port {p:#x}")
    out = emu.alloc(64)
    emu.call(0x59ABC0, "cdecl", [out, pa, pb])
    buf = ctypes.create_string_buffer(64)
    lib.t_strcat(buf, a + b"\0", b + b"\0")
    check(emu.read(out, len(a) + len(b) + 1) == buf.raw[:len(a) + len(b) + 1], f"bStrCat({a!r}, {b!r})")


def _vec(rng, n):
    return [_rf(rng) for _ in range(n)]


def _fbuf(vals):
    return (ctypes.c_float * len(vals))(*vals)


def case_vecop(emu, lib, rng):
    b, c, s = _vec(rng, 4), _vec(rng, 4), rng.choice([0.0, rng.uniform(0.1, 5), 1.0])
    if rng.random() < 0.15:
        b = [0.0] * 4
    pb, pc = emu.alloc(struct.pack("<4f", *b)), emu.alloc(struct.pack("<4f", *c))
    po = emu.alloc(struct.pack("<4f", 7, 8, 9, 10))
    pa = (ctypes.c_float * 4)(7, 8, 9, 10)

    def run(which, addr, args, nout):
        pa[0], pa[1], pa[2], pa[3] = 7, 8, 9, 10
        emu.write(po, struct.pack("<4f", 7, 8, 9, 10))
        r = emu.call(addr, "cdecl", args(po))
        lib.t_vecop(which, pa, _fbuf(b), _fbuf(c), ctypes.c_float(s))
        o = struct.unpack("<4f", emu.read(po, 16))
        ok = all((x == y) or (x != x and y != y) or abs(x - y) <= 1e-5 * max(1, abs(x)) for x, y in zip(o[:nout], pa[:nout])) and \
            o[nout:] == tuple(pa[nout:])
        check(ok, f"vecop {which} b={b} c={c} s={s}: Original {o}, Port {tuple(pa)}")
        return r

    run(1, 0x599D50, lambda o: [o, pb], 2)
    run(2, 0x599DB0, lambda o: [o, pb, f2i(s)], 2)
    run(3, 0x599E90, lambda o: [o, pb, f2i(s)], 3)
    if abs(b[0] ** 2 + b[1] ** 2 + b[2] ** 2 + 2 * b[3]) > 1e-2:  # sonst entscheidet die x87-Genauigkeit über Null
        run(4, 0x599F10, lambda o: [o, pb], 3)
    run(5, 0x599F90, lambda o: [o, pb, pc, f2i(s)], 2)
    run(6, 0x59A000, lambda o: [o, pb, pc, f2i(s)], 4)
    run(9, 0x59A0E0, lambda o: [o, pb, pc], 3)
    r = emu.call(0x599D10, "cdecl", [pb, pc]).st0
    lib.t_vecop(0, pa, _fbuf(b), _fbuf(c), ctypes.c_float(0))
    check(abs(r - pa[0]) <= 1e-5 * max(1, r), f"dist {b} {c}: Original {r}, Port {pa[0]}")
    eps = rng.choice([0.0, 0.1, 1.0])
    c2 = [x + rng.choice([0, 0, rng.uniform(-0.2, 0.2)]) for x in b]
    pc2 = emu.alloc(struct.pack("<4f", *c2))
    for which, addr in ((7, 0x59A050), (8, 0x59A090)):
        r = emu.call(addr, "cdecl", [pb, pc2, f2i(eps)]).eax & 0xFF
        p = lib.t_vecop(which, pa, _fbuf(b), _fbuf(c2), ctypes.c_float(eps)) & 0xFF
        check(r == p, f"bEqual{which} {b} {c2} {eps}: Original {r}, Port {p}")


def case_bbox(emu, lib, rng):
    def box():
        lo = [rng.uniform(-5, 5) for _ in range(4)]
        return lo, [x + rng.choice([0, rng.uniform(0, 6)]) for x in lo]

    (mn, mx), (lo, hi) = box(), box()
    p, q = _vec(rng, 4), _vec(rng, 4)
    r = rng.choice([0.0, 0.5, 2.0])
    if rng.random() < 0.3:
        p = [mn[i] if rng.random() < 0.5 else mx[i] for i in range(4)]

    def setup():
        return [emu.alloc(struct.pack("<4f", *v)) for v in (mn, mx, p, q)]

    # (which, addr, args(pmn, pmx, pp, pq), 'modify' = Ergebnis in mn/mx)
    mod = [(0, 0x59A120, lambda a: [a[0], a[1]]), (1, 0x59A410, lambda a: [a[0], a[1]]),
           (2, 0x59A150, lambda a: [a[0], a[1], a[2]]), (3, 0x59A440, lambda a: [a[0], a[1], a[2]]),
           (4, 0x59A170, lambda a: [a[0], a[1], a[2]]), (5, 0x59A470, lambda a: [a[0], a[1], a[2]]),
           (6, 0x59A1F0, lambda a: [a[0], a[1], a[2], f2i(r)]), (7, 0x59A510, lambda a: [a[0], a[1], a[2], f2i(r)]),
           (8, 0x59A280, lambda a: [a[0], a[1], a[2], a[3]]), (9, 0x59A5E0, lambda a: [a[0], a[1], a[2], a[3]])]
    for which, addr, args in mod:
        a = setup()
        emu.call(addr, "cdecl", args(a))
        omn, omx = emu.read(a[0], 16), emu.read(a[1], 16)
        bmn, bmx = _fbuf(mn), _fbuf(mx)
        lib.t_bbox(which, bmn, bmx, _fbuf(p), _fbuf(q), ctypes.c_float(r))
        used = 2 if which in (0, 2, 4, 6, 8) else 3
        pm, px = struct.unpack("<4f", omn), struct.unpack("<4f", omx)
        f32 = lambda v: struct.unpack("<4f", struct.pack("<4f", *v))
        ok = list(pm[:used]) == list(bmn)[:used] and list(px[:used]) == list(bmx)[:used] and pm[used:] == f32(mn)[used:] and px[used:] == f32(mx)[used:]
        check(ok, f"bbox {which}: mn={mn} mx={mx} p={p} q={q} r={r}: Original {pm}{px}, Port {list(bmn)}{list(bmx)}")
    tests = [(10, 0x59A6A0, lambda a: [a[0], a[1], a[2], f2i(r)]), (11, 0x59A310, lambda a: [a[0], a[1], a[2], f2i(r)]),
             (12, 0x59A370, lambda a: [a[0], a[1], a[2], a[3]]), (13, 0x59A3C0, lambda a: [a[0], a[1], a[2], a[3]]),
             (14, 0x59A720, lambda a: [a[0], a[1], a[2], a[3]])]
    p2, q2 = ([x for x in mn], [x for x in mx]) if rng.random() < 0.3 else (lo, hi)
    for which, addr, args in tests:
        a = [emu.alloc(struct.pack("<4f", *v)) for v in (mn, mx, p2 if which >= 12 else p, q2 if which >= 12 else q)]
        o = emu.call(addr, "cdecl", args(a)).eax & 0xFF
        pr = lib.t_bbox(which, _fbuf(mn), _fbuf(mx), _fbuf(p2 if which >= 12 else p), _fbuf(q2 if which >= 12 else q), ctypes.c_float(r)) & 0xFF
        check(o == pr, f"bboxtest {which}: mn={mn} mx={mx} p={p} q={q} r={r}: Original {o}, Port {pr}")


def case_poly(emu, lib, rng):
    n = rng.randint(3, 11)
    verts = [(rng.uniform(-10, 10), rng.uniform(-10, 10)) for _ in range(n)]
    pt = (rng.uniform(-10, 10), rng.uniform(-10, 10))
    if rng.random() < 0.2:
        pt = (pt[0], verts[rng.randrange(n)][1])  # genau auf Eckenhöhe
    pv = emu.alloc(struct.pack(f"<{2 * n}f", *[c for v in verts for c in v]))
    pp = emu.alloc(struct.pack("<2f", *pt))
    o = emu.call(0x59A790, "cdecl", [pp, pv, n]).eax & 0xFF
    flat = _fbuf([c for v in verts for c in v])
    p = lib.t_poly(_fbuf(pt), flat, n) & 0xFF
    check(o == p, f"poly n={n} pt={pt} verts={verts}: Original {o}, Port {p}")


def _rot_matrix(rng):
    import math
    ax = [rng.uniform(-1, 1) for _ in range(3)]
    l = math.sqrt(sum(x * x for x in ax)) or 1
    ax = [x / l for x in ax]
    th = rng.uniform(-3.2, 3.2)
    c, s = math.cos(th), math.sin(th)
    x, y, z = ax
    m = [c + x * x * (1 - c), x * y * (1 - c) + z * s, x * z * (1 - c) - y * s, 0,
         x * y * (1 - c) - z * s, c + y * y * (1 - c), y * z * (1 - c) + x * s, 0,
         x * z * (1 - c) + y * s, y * z * (1 - c) - x * s, c + z * z * (1 - c), 0,
         rng.uniform(-50, 50), rng.uniform(-50, 50), rng.uniform(-50, 50), 1]
    return m


def _close(a, b, tol=1e-4):
    return all(x == y or abs(x - y) <= tol * max(1, abs(x), abs(y)) or (x != x and y != y) for x, y in zip(a, b))


def case_matop(emu, lib, rng):
    m = _rot_matrix(rng)
    if rng.random() < 0.2:
        m = [rng.uniform(-3, 3) for _ in range(16)]
    pin = emu.alloc(struct.pack("<16f", *m))
    pout = emu.alloc(struct.pack("<16f", *[9.0] * 16))
    for which, addr, args in ((0, 0x598300, [pout]), (1, 0x598340, [pout, pin]), (2, 0x598550, [pout, pin])):
        emu.write(pout, struct.pack("<16f", *[9.0] * 16))
        emu.call(addr, "cdecl", args)
        o = struct.unpack("<16f", emu.read(pout, 64))
        out = (ctypes.c_float * 16)(*[9.0] * 16)
        lib.t_matop(which, out, _fbuf(m))
        check(_close(o, list(out), 1e-3), f"mat {which} {m}: Original {o}, Port {list(out)}")
    q = [rng.uniform(-1, 1) for _ in range(4)]
    l = sum(x * x for x in q) ** 0.5
    q = [x / l for x in q]
    pq = emu.alloc(struct.pack("<4f", *q))
    emu.write(pout, struct.pack("<16f", *[9.0] * 16))
    emu.call(0x599300, "thiscall", [pout], this=pq)
    o = struct.unpack("<16f", emu.read(pout, 64))
    out = (ctypes.c_float * 16)(*[9.0] * 16)
    lib.t_matop(3, out, _fbuf(q))
    check(_close(o, list(out), 1e-5), f"quat->mat {q}: Original {o}, Port {list(out)}")
    emu.write(pout, struct.pack("<16f", *[9.0] * 16))
    emu.call(0x599400, "cdecl", [pout, pq])
    o = struct.unpack("<16f", emu.read(pout, 64))
    lib.t_matop(4, out, _fbuf(q))
    check(_close(o, list(out), 1e-5), f"quat->mat2 {q}: Original {o}, Port {list(out)}")
    rm = _rot_matrix(rng)
    pm = emu.alloc(struct.pack("<16f", *rm))
    emu.write(pout, struct.pack("<4f", 9, 9, 9, 9))
    emu.call(0x5994E0, "cdecl", [pout, pm])
    o = struct.unpack("<4f", emu.read(pout, 16))
    outq = (ctypes.c_float * 16)()
    lib.t_matop(5, outq, _fbuf(rm))
    check(_close(o, list(outq)[:4], 1e-4), f"mat->quat {rm}: Original {o}, Port {list(outq)[:4]}")


def _setup_bware(lib):
    F, I, U, P = ctypes.c_float, ctypes.c_int, ctypes.c_uint, ctypes.c_void_p
    lib.t_bsin.argtypes = [U]; lib.t_bsin.restype = F
    lib.t_bsincos.argtypes = [U, P]
    lib.t_basin.argtypes = [F]; lib.t_basin.restype = I
    lib.t_batan.argtypes = [F, F]; lib.t_batan.restype = I
    lib.t_bfixatan1.argtypes = [I]; lib.t_bfixatan1.restype = I
    lib.t_bfixatan2.argtypes = [I, I]; lib.t_bfixatan2.restype = I
    lib.t_brandom.argtypes = [P, U]; lib.t_brandom.restype = U
    lib.t_brandomf.argtypes = [P, F]; lib.t_brandomf.restype = F
    lib.t_strfn.argtypes = [I, ctypes.c_char_p, ctypes.c_char_p, I]; lib.t_strfn.restype = I
    lib.t_strcat.argtypes = [P, ctypes.c_char_p, ctypes.c_char_p]
    lib.t_vecop.argtypes = [I, P, P, P, F]; lib.t_vecop.restype = I
    lib.t_bbox.argtypes = [I, P, P, P, P, F]; lib.t_bbox.restype = I
    lib.t_poly.argtypes = [P, P, I]; lib.t_poly.restype = I
    lib.t_matop.argtypes = [I, P, P]
    lib.t_table_value.argtypes = [I, F, F, P, F]; lib.t_table_value.restype = F
    lib.t_graph_inverse.argtypes = [P, I, F]; lib.t_graph_inverse.restype = F
    lib.t_avg.argtypes = [I, I, P, P, P, P, F, F]
    lib.t_linear.argtypes = [I, P, F, F, F, F]
    lib.t_blend.argtypes = [P, P, P, F]
    lib.t_lz.argtypes = [P, P]; lib.t_lz.restype = I
    lib.t_lz_valid.argtypes = [P]; lib.t_lz_valid.restype = I
    lib.t_qs.argtypes = [I, P, U, P, P]; lib.t_qs.restype = I


# ------------------------------------------------------------------ WP-30.2: QuickSpline
_D3DX_HOOKED = []


def _hook_d3dx(emu):
    """0x40a8a0 (eMulVector -> D3DXVec4Transform(out, v, M), nicht im Emulator verfügbar): Zeilenvektor mal Matrix."""
    if _D3DX_HOOKED:
        return
    from unicorn import UC_HOOK_CODE
    from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP

    def cb(uc, address, size, user):
        esp = uc.reg_read(UC_X86_REG_ESP)
        ret, out, m, v = struct.unpack("<4I", bytes(uc.mem_read(esp, 16)))
        M = struct.unpack("<16f", bytes(uc.mem_read(m, 64)))
        V = struct.unpack("<4f", bytes(uc.mem_read(v, 16)))
        r = [V[0] * M[j] + V[1] * M[4 + j] + V[2] * M[8 + j] + V[3] * M[12 + j] for j in range(4)]
        uc.mem_write(out, struct.pack("<4f", *r))
        uc.reg_write(UC_X86_REG_ESP, esp + 4)
        uc.reg_write(UC_X86_REG_EIP, ret)
    emu.uc.hook_add(UC_HOOK_CODE, cb, begin=0x40A8A0, end=0x40A8A0)
    _D3DX_HOOKED.append(1)


def _qs_blob(rng, base, with_n=None, dirty=None):
    """Objekt (0x2c) + Stützpunkte ab 0x40 (Puffer, Punkt 0 bei 0x50); Zeiger absolut (base) oder Arena-Offset (base=0)."""
    n = with_n or rng.randint(3, 9)
    et = rng.choice([0, 1, 2])
    pts = []
    x, y, z, w = 0.0, 0.0, 0.0, 0.0
    for _ in range(n):
        nx, ny, nz = x + rng.uniform(-8, 8), y + rng.uniform(-8, 8), z + rng.uniform(-2, 2)
        w += ((nx - x) ** 2 + (ny - y) ** 2 + (nz - z) ** 2) ** 0.5
        x, y, z = nx, ny, nz
        pts.append((x, y, z, w))
    length = rng.choice([0.0, w, w + rng.uniform(0, 5)])
    buf = [(0.0, 0.0, 0.0, 0.0)] + pts + [(0.0, 0.0, 0.0, 0.0)] * 2   # n + 3
    maxp, minp = {0: (float(n), 0.0), 1: (float(n - 1), 0.0), 2: (9999999.0, -9999999.0)}[et]
    obj = struct.pack("<IIIfffbbbbHHIII", rng.getrandbits(32), et, 0, maxp, minp, length, rng.choice([0, 1]) if dirty is None else dirty, 0, 0, 0, n, n,
                      base + 0x40, base + 0x50, 0)
    data = obj.ljust(0x40, b"\0") + b"".join(struct.pack("<4f", *p) for p in buf)
    return n, et, data, length


def _qs_setup(emu, rng, with_n=None, dirty=1):
    n, et, port_blob, length = _qs_blob(rng, 0, with_n, dirty)
    # Zeigerfelder: Emulator absolut, Port als Arena-Offset (Basis = Blob-Anfang)
    addr = emu.alloc(bytes(len(port_blob) + 16))
    emu_bytes = bytearray(port_blob)
    struct.pack_into("<I", emu_bytes, 0x20, addr + 0x40)
    struct.pack_into("<I", emu_bytes, 0x24, addr + 0x50)
    struct.pack_into("<I", emu_bytes, 0x28, 0x72C4B0)  # Basismatrizen des Originals
    emu.write(addr, bytes(emu_bytes))
    return n, et, addr, bytearray(port_blob)


def _qs_read(emu, addr, size):
    b = bytearray(emu.read(addr, size))
    return b


def _qs_norm(b):
    b = bytearray(b)
    struct.pack_into("<III", b, 0x20, 0, 0, 0)  # Zeigerfelder unterscheiden sich absichtlich
    return bytes(b)


def _qs_port(lib, which, blob, ins, outn=16):
    buf = ctypes.create_string_buffer(bytes(blob), len(blob))
    fin = (ctypes.c_float * 8)(*ins, *[0.0] * (8 - len(ins)))
    fout = (ctypes.c_float * 16)()
    r = lib.t_qs(which, buf, len(blob), fin, fout)
    return r, list(fout), buf.raw


def _qs_close(a, b, tol=2e-3):
    return all(abs(x - y) <= tol * max(1.0, abs(x), abs(y)) or (x != x and y != y) for x, y in zip(a, b))


def case_qs_state(emu, lib, rng):
    _hook_d3dx(emu)
    emu.call(0x5775E0, "cdecl", [])
    n, et, addr, blob = _qs_setup(emu, rng, dirty=None)
    size = len(blob)
    # SetNumControlPoints (fastcall: ecx = n, eax = Objekt)
    m = rng.randint(0, 12)
    emu.call(0x5778D0, "fastcall", [m], regs={"eax": addr})
    r, _, pb = _qs_port(lib, 0, blob, [m])
    check(_qs_norm(emu.read(addr, size)) == _qs_norm(pb), f"SetNumControlPoints({m}) et={et}: {emu.read(addr, 0x2c).hex()} vs {pb[:0x2c].hex()}")
    # Fixup
    n, et, addr, blob = _qs_setup(emu, rng)
    size = len(blob)
    emu.call(0x577940, "cdecl", [], regs={"edi": addr})
    r, _, pb = _qs_port(lib, 1, blob, [])
    check(_qs_close(struct.unpack(f"<{(size - 0x40) // 4}f", emu.read(addr + 0x40, size - 0x40)),
                    struct.unpack(f"<{(size - 0x40) // 4}f", pb[0x40:]), 1e-6), f"Fixup et={et} n={n}")
    # ClampParam
    p = rng.choice([rng.uniform(-3, 12), rng.uniform(-30, 30), 0.0, float(n)])
    n, et, addr, blob = _qs_setup(emu, rng)
    o = emu.call(0x577BD0, "thiscall", [f2i(p)], this=addr).st0
    r, out, _ = _qs_port(lib, 2, blob, [p])
    check(abs(o - out[0]) <= 1e-4 * max(1, abs(o)), f"ClampParam({p}) et={et} max={struct.unpack('<f', blob[12:16])[0]}: Original {o}, Port {out[0]}")
    # HasKink
    o = emu.call(0x578550, "cdecl", [], regs={"eax": addr}).eax & 0xFF
    r, _, _ = _qs_port(lib, 10, blob, [])
    check(o == (r & 0xFF), f"HasKink: Original {o}, Port {r}")


def case_qs_points(emu, lib, rng):
    _hook_d3dx(emu)
    n, et, addr, blob = _qs_setup(emu, rng)
    size = len(blob)
    emu.call(0x5775E0, "cdecl", [])
    first = rng.randint(-4, n + 2)
    outp = emu.alloc(64)
    emu.call(0x577C60, "thiscall", [outp], this=addr, regs={"eax": first})
    o = struct.unpack("<16f", emu.read(outp, 64))
    r, out, _ = _qs_port(lib, 3, blob, [first])
    check(_qs_close(o, out, 1e-5), f"GenerateExtrapolated({first}) n={n} et={et}")
    lo = -2.0 if et == 2 else -1.0
    t = rng.uniform(lo, n + 2.0 if et != 0 else n * 2.5)
    deriv = rng.choice([0, 1, 2])
    pq = emu.alloc(16)
    emu.call(0x577D90, "stdcall", [pq, f2i(t), deriv], regs={"eax": addr})
    o = struct.unpack("<4f", emu.read(pq, 16))
    r, out, _ = _qs_port(lib, 4, blob, [t, deriv])
    check(_qs_close(o, out[:4], 2e-4), f"GetPoint({t}, {deriv}) n={n} et={et}: Original {o}, Port {out[:4]}")
    # FindClosestControlPoint
    P = [rng.uniform(-20, 20) for _ in range(3)] + [float(rng.randint(0, n - 1))]
    pp = emu.alloc(struct.pack("<4f", *P[:3], 0))
    o = emu.call(0x577E70, "cdecl", [pp, int(P[3])], regs={"edi": addr}).eax
    r, _, _ = _qs_port(lib, 5, blob, P)
    check(o == r, f"FindClosestControlPoint({P}) n={n}: Original {o}, Port {r}")


def case_qs_search(emu, lib, rng):
    _hook_d3dx(emu)
    n, et, addr, blob = _qs_setup(emu, rng)
    emu.call(0x5775E0, "cdecl", [])
    # Punkt nahe der Kurve: Stützpunkt mit Versatz
    pts = [struct.unpack("<4f", blob[0x50 + 16 * i:0x60 + 16 * i]) for i in range(n)]
    base = rng.choice(pts)
    P = [base[0] + rng.uniform(-3, 3), base[1] + rng.uniform(-3, 3), base[2] + rng.uniform(-1, 1)]
    init = rng.uniform(0, max(0.0, n - 1.0))
    tol = rng.choice([0.01, 0.001])
    pp = emu.alloc(struct.pack("<4f", *P, 0))
    # FindClosestParam / Lateral stehen im Original auf sehr x87-empfindlichen Iterationen: nur wenn beide konvergieren
    o = emu.call(0x577F30, "cdecl", [pp, f2i(init), f2i(tol)], regs={"eax": addr}).st0
    r, out, _ = _qs_port(lib, 6, blob, P + [init, tol])
    ok = abs(o - out[0]) <= 5e-3 * max(1, abs(o))
    check(ok, f"FindClosestParam({P}, {init}, {tol}) n={n} et={et}: Original {o}, Port {out[0]}")
    po = emu.alloc(4)
    o = emu.call(0x578130, "cdecl", [f2i(init), po, f2i(tol)], regs={"edi": addr, "esi": pp, "eax": 0}).st0
    r, out, _ = _qs_port(lib, 7, blob, P + [init, tol])
    check(abs(o - out[0]) <= 5e-2 * max(1, abs(o)) and abs(emu.f32(po) - out[1]) <= 5e-3 * max(1, abs(out[1])),
          f"FindClosestLateralOffset({P}, {init}, {tol}) n={n} et={et}: Original {o}, {emu.f32(po)}, Port {out[:2]}")


def case_qs_length(emu, lib, rng):
    _hook_d3dx(emu)
    n, et, addr, blob = _qs_setup(emu, rng)
    size = len(blob)
    emu.call(0x5775E0, "cdecl", [])
    steps = rng.randint(1, 6)
    emu.call(0x5783F0, "cdecl", [steps], regs={"eax": addr})
    r, _, pb = _qs_port(lib, 9, blob, [steps])
    o = struct.unpack(f"<{(size - 0x40) // 4}f", emu.read(addr + 0x40, size - 0x40))
    pf = struct.unpack(f"<{(size - 0x40) // 4}f", pb[0x40:])
    check(_qs_close(o, pf, 2e-3) and abs(emu.f32(addr + 0x14) - struct.unpack("<f", pb[0x14:0x18])[0]) < 2e-3 * max(1, emu.f32(addr + 0x14)),
          f"CalibrateLength({steps}) n={n} et={et}: Länge Original {emu.f32(addr + 0x14)}, Port {struct.unpack('<f', pb[0x14:0x18])[0]}")
    # GetParam mit kalibrierter Strecke
    L = emu.f32(addr + 0x14)
    d = rng.uniform(-1, 1.1) * max(L, 1.0)
    o = emu.call(0x578230, "cdecl", [f2i(d)], regs={"edi": addr}).st0
    r2, out, _ = _qs_port(lib, 8, pb, [d])
    check(abs(o - out[0]) <= 5e-3 * max(1, abs(o)), f"GetParam({d}) n={n} et={et} L={L}: Original {o}, Port {out[0]}")


def case_qs_basis(emu, lib, rng):
    emu.call(0x5775E0, "cdecl", [])
    o = struct.unpack("<48f", emu.read(0x72C4B0, 192))
    fout = (ctypes.c_float * 48)()
    lib.t_qs(11, ctypes.create_string_buffer(0x80), 0x80, (ctypes.c_float * 8)(), fout)
    check(all(abs(a - b) < 1e-6 for a, b in zip(o, fout)), f"Basismatrizen: {o} vs {list(fout)}")


# ------------------------------------------------------------------ WP-30.3: Table, Graph, Average, Linear, LZ
def case_table(emu, lib, rng):
    n = rng.randint(2, 12)
    tab = [rng.uniform(-50, 50) for _ in range(n)]
    mn = rng.uniform(-10, 10)
    mul = rng.choice([0.5, 1.0, 2.0, rng.uniform(0.1, 3)])
    x = rng.uniform(mn - 3, mn + (n + 2) / mul)
    ptab = emu.alloc(struct.pack(f"<{n}f", *tab))
    obj = emu.alloc(struct.pack("<ifffI", n, mn, 0.0, mul, ptab))
    o = emu.call(0x59CEB0, "thiscall", [f2i(x)], this=obj).st0
    p = lib.t_table_value(n, ctypes.c_float(mn), ctypes.c_float(mul), _fbuf(tab), ctypes.c_float(x))
    check(abs(o - p) <= 1e-4 * max(1, abs(o)), f"Table({n},{mn},{mul},{x}): Original {o}, Port {p}")


def case_graph_inverse(emu, lib, rng):
    n = rng.randint(2, 8)
    xs = sorted(rng.sample(range(-20, 20), n))
    ys = sorted(rng.uniform(-30, 30) for _ in range(n))
    if rng.random() < 0.2:
        ys[1] = ys[0]
    pts = [c for xy in zip(xs, ys) for c in xy]
    pp = emu.alloc(struct.pack(f"<{2 * n}f", *pts))
    obj = emu.alloc(struct.pack("<II", pp, n))
    y = rng.uniform(-35, 35)
    o = emu.call(0x59D040, "thiscall", [f2i(y)], this=obj).st0
    p = lib.t_graph_inverse(_fbuf(pts), n, ctypes.c_float(y))
    check(abs(o - p) <= 1e-4 * max(1, abs(o)), f"GraphInverse({pts},{y}): Original {o}, Port {p}")


def case_average(emu, lib, rng):
    n = rng.randint(1, 8)
    win = rng.random() < 0.5
    ns, cur = rng.randint(0, n), rng.randint(0, n - 1)
    data = [rng.uniform(-5, 5) for _ in range(n)]
    times = sorted(rng.uniform(0, 3) for _ in range(n))
    total, avg = sum(data), rng.uniform(-5, 5)
    window = rng.choice([0.5, 1.0, 2.0])
    oldest = rng.randint(0, n - 1)
    a, b = rng.uniform(-5, 5), rng.uniform(0, 4)
    which = rng.choice([3, 3, 3, 4]) if win else rng.choice([0, 0, 1, 2])
    pd = emu.alloc(struct.pack(f"<{n}f", *data))
    pt = emu.alloc(struct.pack(f"<{n}f", *times))
    obj = emu.alloc(struct.pack("<IBBBBffI5ffII", 0, 4, n, ns, cur, total, avg, pd, 0, 0, 0, 0, 0, window, oldest, pt))
    addr, args = {0: (0x59D320, [f2i(a)]), 1: (0x59D380, [f2i(a)]), 2: (0x59D3C0, [f2i(a)]),
                  3: (0x59D5B0, [f2i(a), f2i(b)]), 4: (0x59D560, [f2i(a)])}[which]
    emu.call(addr, "thiscall", args, this=obj)
    raw = emu.read(obj, 0x34)
    _, _, _, ons, ocur, ototal, oavg, _ = struct.unpack("<IBBBBffI", raw[:20])
    ooldest = struct.unpack("<I", raw[0x2C:0x30])[0]
    odata = struct.unpack(f"<{n}f", emu.read(pd, 4 * n))
    otimes = struct.unpack(f"<{n}f", emu.read(pt, 4 * n))
    st = (ctypes.c_int * 3)(ns, cur, oldest)
    f = (ctypes.c_float * 3)(total, avg, window)
    pdat, ptim = _fbuf(data), _fbuf(times)
    lib.t_avg(which, n, st, f, pdat, ptim, ctypes.c_float(a), ctypes.c_float(b))
    ok = (ons, ocur) == (st[0], st[1]) and (not win or ooldest == st[2]) and _close([ototal, oavg], [f[0], f[1]], 1e-4) and \
        _close(odata, list(pdat), 1e-5) and (not win or _close(otimes, list(ptim), 1e-5))
    check(ok, f"Average which={which} n={n} ns={ns} cur={cur} data={data} times={times} a={a} b={b}: Original {ons},{ocur},{ooldest},{ototal},{oavg},{odata}; Port {list(st)},{list(f)},{list(pdat)}")


def case_linear(emu, lib, rng):
    x1, y1 = rng.uniform(-10, 10), rng.uniform(-10, 10)
    x2, y2 = x1 + rng.choice([rng.uniform(0.1, 5), -rng.uniform(0.1, 5)]), rng.uniform(-10, 10)
    obj = emu.alloc(8)
    emu.call(0x59D770, "thiscall", [f2i(x1), f2i(y1), f2i(x2), f2i(y2)], this=obj)
    om, ob = struct.unpack("<2f", emu.read(obj, 8))
    mb = (ctypes.c_float * 3)()
    lib.t_linear(0, mb, *[ctypes.c_float(v) for v in (x1, y1, x2, y2)])
    check(_close([om, ob], [mb[0], mb[1]], 1e-5), f"Linear.Init: {om},{ob} vs {mb[0]},{mb[1]}")
    for m in (om, rng.uniform(-0.002, 0.002)):
        emu.write(obj, struct.pack("<2f", m, ob))
        mb[0], mb[1] = m, ob
        v = rng.uniform(-10, 10)
        o = emu.call(0x59D7A0, "thiscall", [f2i(v)], this=obj).st0
        lib.t_linear(1, mb, ctypes.c_float(v), 0, 0, 0)
        check(abs(o - mb[2]) <= 1e-4 * max(1, abs(o)), f"Linear.GetValue({v}) m={m}")
        o = emu.call(0x59D7B0, "thiscall", [f2i(v)], this=obj).st0
        lib.t_linear(2, mb, ctypes.c_float(v), 0, 0, 0)
        check(abs(o - mb[2]) <= 1e-4 * max(1, abs(o)), f"Linear.GetInverse({v}) m={m}")
    a, b, t = rng.uniform(-5, 5), rng.uniform(-5, 5), rng.uniform(0, 1)
    pa, pb, po = emu.alloc(struct.pack("<f", a)), emu.alloc(struct.pack("<f", b)), emu.alloc(4)
    emu.call(0x59CF30, "cdecl", [po, pa, pb, f2i(t)])
    out = (ctypes.c_float * 1)()
    lib.t_blend(out, _fbuf([a]), _fbuf([b]), ctypes.c_float(t))
    check(abs(emu.f32(po) - out[0]) <= 1e-5, "bBlend")


def _comp_encode(rng, n_tokens):
    out = bytearray()
    body = bytearray()
    toks = []
    for _ in range(n_tokens):
        if len(out) >= 3 and rng.random() < 0.5:
            off = rng.randint(1, min(len(out), 0xFFF))
            ln = rng.randint(3, 18)
            for _k in range(ln):
                out.append(out[-off])
            toks.append((1, bytes([((off >> 4) & 0xF0) | (ln - 3), off & 0xFF])))
        else:
            c = rng.getrandbits(8)
            out.append(c)
            toks.append((0, bytes([c])))
    for g in range(0, len(toks), 16):
        grp = toks[g:g + 16]
        bits = sum(1 << i for i, (f, _) in enumerate(grp) if f)
        body += bytes([bits & 0xFF, bits >> 8])
        for _, b in grp:
            body += b
    return bytes(out), bytes(body)


def case_lz(emu, lib, rng):
    kind = rng.choice(["comp", "comp", "stored", "bad"])
    if kind == "comp":
        expect, body = _comp_encode(rng, rng.choice([1, 5, 16, 17, 40, 300]))
        flag = 0
    elif kind == "stored":
        expect = bytes(rng.getrandbits(8) for _ in range(rng.randint(0, 40)))
        body, flag = expect, 1
    else:
        expect = b""
        body, flag = b"x" * 8, 0
    total = 16 + len(body)
    magic = b"COMP" if kind != "bad" else b"ZZZZ"
    hdr = magic + struct.pack("<BBhII", 1, 0, flag, len(expect), total)[:12]
    hdr = magic + bytes([1, 0]) + struct.pack("<h", flag) + struct.pack("<II", len(expect), total)
    pkt = hdr + body
    PADL = 0x2000
    src = emu.alloc(pkt + b"\0" * 8)
    reg = emu.alloc(PADL + len(expect) + 64)
    o = emu.call(0x59DFE0, "cdecl", [src, reg + PADL]).eax
    o = o - (1 << 32) if o & 0x80000000 else o
    odst = emu.read(reg + PADL, len(expect))
    pbuf = ctypes.create_string_buffer(PADL + len(expect) + 64)
    sbuf = ctypes.create_string_buffer(pkt + b"\0" * 8, len(pkt) + 8)
    p = lib.t_lz(sbuf, ctypes.byref(pbuf, PADL))
    valid_o = emu.call(0x59DFB0, "cdecl", [src]).eax & 0xFF
    valid_p = lib.t_lz_valid(sbuf) & 0xFF
    check(o == p and valid_o == valid_p, f"LZ {kind}: Rückgabe Original {o}, Port {p}, gültig {valid_o}/{valid_p}")
    if kind != "bad":
        check(odst == expect == pbuf.raw[PADL:PADL + len(expect)], f"LZ {kind}: Ausgabe weicht ab")


CASES = {
    "name_hash": case_name_hash,
    "reset_identity_matrix": case_reset_identity_matrix,
    "jdlz_decompress": case_jdlz_decompress,
    "jdlz_bad_header": case_jdlz_bad_header,
    "timer_hms": case_timer_hms,
    "timer_round": case_timer_round,
    "cubic1d_update": case_cubic1d_update,
    "envelope_value": case_envelope_value,
    "rumble_start": case_rumble_start,
    "button_pair": case_button_pair,
    "joy_scan": case_joy_scan,
    "clock": case_clock,
    "splineseek1d": case_splineseek1d,
    "splineseek3d": case_splineseek3d,
    "bsin": case_bsin, "bsincos": case_bsincos, "basin": case_basin, "batan": case_batan,
    "bfixatan1": case_bfixatan1, "bfixatan2": case_bfixatan2, "brandom": case_brandom, "brandomf": case_brandomf,
    "qs_state": case_qs_state, "qs_points": case_qs_points, "qs_search": case_qs_search, "qs_length": case_qs_length, "qs_basis": case_qs_basis,
    "table": case_table, "graph_inverse": case_graph_inverse, "average": case_average, "linear": case_linear, "lz": case_lz,
    "strings": case_strcmp, "vecop": case_vecop, "bbox": case_bbox, "poly": case_poly, "matop": case_matop,
}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--n", type=int, default=10000)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("names", nargs="*")
    args = ap.parse_args()

    lib = ctypes.CDLL(DYLIB)
    lib.t_name_hash.argtypes = [ctypes.c_char_p]
    lib.t_name_hash.restype = ctypes.c_uint32
    lib.t_reset_identity_matrix.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
    lib.t_jdlz_decompress.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
    lib.t_jdlz_decompress.restype = ctypes.c_uint32

    lib.t_timer_hms.argtypes = [ctypes.c_int32, ctypes.c_void_p]
    lib.t_timer_rounded.argtypes = [ctypes.c_int32, ctypes.c_float]
    lib.t_timer_rounded.restype = ctypes.c_float
    lib.t_cubic1d_update.argtypes = [ctypes.c_void_p, ctypes.c_float, ctypes.c_float, ctypes.c_float]

    lib.t_envelope_value.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_float]
    lib.t_envelope_value.restype = ctypes.c_float
    lib.t_rumble_start.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_int, ctypes.c_void_p, ctypes.c_void_p,
                                   ctypes.c_void_p, ctypes.c_void_p, ctypes.c_int, ctypes.c_float, ctypes.c_byte,
                                   ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]
    lib.t_rumble_start.restype = ctypes.c_int

    lib.t_joy_scan.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_int,
                               ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
    lib.t_joy_scan.restype = ctypes.c_int
    lib.t_button_pair.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p]
    lib.t_button_pair.restype = ctypes.c_uint

    lib.t_splineseek1d.argtypes = [ctypes.c_void_p, ctypes.c_float, ctypes.c_float, ctypes.c_float]
    lib.t_splineseek3d.argtypes = [ctypes.c_void_p, ctypes.c_float]
    lib.t_clock.argtypes = [ctypes.c_int, ctypes.c_void_p, ctypes.c_void_p, ctypes.c_float]

    _setup_bware(lib)
    emu = Emu()
    rng = random.Random(args.seed)
    failed = 0
    for name in args.names or CASES:
        fn = CASES[name]
        t0 = time.time()
        try:
            for i in range(args.n):
                if i % 500 == 0:
                    emu.reset_scratch()
                fn(emu, lib, rng)
            print(f"[ ok ] {name}: {args.n} Eingaben bitgleich ({time.time() - t0:.1f} s)")
        except Mismatch as e:
            failed += 1
            print(f"[FAIL] {name}: Eingabe {i}: {e}")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
