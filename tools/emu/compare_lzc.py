"""WP-12: jdlz_decompress (0x59dd70) im Emulator gegen den Port auf allen echten .lzc-Dateien.

Aufruf: tools/emu/.venv/bin/python tools/emu/compare_lzc.py [datei ...]
Voraussetzung: cmake --build port/build/release --target nfsu_core_shared
"""
import ctypes
import glob
import os
import struct
import sys
import time

sys.path.insert(0, os.path.dirname(__file__))
from nfsu_emu import Emu  # noqa: E402

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
DYLIB = os.path.join(ROOT, "port", "build", "release", "libnfsu_core.dylib")
PAD = 64


def main():
    files = sys.argv[1:] or sorted(glob.glob(os.path.join(ROOT, "game", "pc", "**", "*.[lL][zZ][cC]"), recursive=True))
    lib = ctypes.CDLL(DYLIB)
    lib.t_jdlz_decompress.argtypes = [ctypes.c_void_p, ctypes.c_void_p]
    lib.t_jdlz_decompress.restype = ctypes.c_uint32
    emu = Emu()
    bad = 0
    for path in files:
        data = open(path, "rb").read()
        if data[:4] != b"JDLZ":
            print(f"[skip] {os.path.relpath(path, ROOT)}: kein JDLZ")
            continue
        out_size, total = struct.unpack_from("<II", data, 8)
        emu.reset_scratch()
        t0 = time.time()
        src = emu.alloc(data + b"\0" * 8)
        dst = emu.alloc(PAD + out_size + PAD)
        ret = emu.call(0x59DD70, "cdecl", [src, dst + PAD], max_insns=4_000_000_000).eax
        orig = emu.read(dst + PAD, out_size)
        t_emu = time.time() - t0
        sbuf = ctypes.create_string_buffer(data + b"\0" * 8, len(data) + 8)
        hbuf = ctypes.create_string_buffer(out_size)
        pret = lib.t_jdlz_decompress(sbuf, hbuf)
        ok = ret == pret == out_size and orig == hbuf.raw
        bad += not ok
        print(f"[{' ok ' if ok else 'FAIL'}] {os.path.relpath(path, ROOT)}: {len(data)} -> {out_size} B, Emulator {t_emu:.1f} s"
              + ("" if ok else f" (Rückgabe Original {ret}, Port {pret})"))
    print(f"{len(files) - bad} von {len(files)} bitgleich")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
