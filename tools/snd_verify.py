#!/usr/bin/env python3
"""WP-15: vergleicht den eigenen EA-Decoder (nfsu-tool snd-md5) bitgleich mit FFmpeg.
Ganze Datei; wo FFmpeg wegen wechselnder Kanalzahl im Strom abweicht (SpecialStream), je Strom einzeln."""
import glob, hashlib, re, subprocess, sys, tempfile, os
TOOL = sys.argv[1] if len(sys.argv) > 1 else "port/build/release/nfsu-tool"
files = sorted(glob.glob("game/pc/SNDSTREAMS/*.ast") + glob.glob("game/pc/SpeechStreams/*.ast"))

def md5_of(cmd):
    h = hashlib.md5(); p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    for blk in iter(lambda: p.stdout.read(1 << 20), b""): h.update(blk)
    p.wait(); return h.hexdigest()

ok = True
for f in files:
    a = md5_of([TOOL, "snd-md5", f]); b = md5_of(["ffmpeg", "-v", "quiet", "-i", f, "-f", "s16le", "-"])
    if a == b: print("gleich   ", f); continue
    # je Strom
    listing = subprocess.run([TOOL, "snd", f], capture_output=True, text=True).stdout.splitlines()
    data = open(f, "rb").read(); mine = subprocess.run([TOOL, "snd-md5", f], capture_output=True).stdout
    pos = 0; bad = 0
    for l in listing:
        m = re.match(r"\s*\d+\s+@(\d+)\s+(\d+) B\s+\d+ Hz\s+(\d+)ch.*?(\d+)/\d+ Samples", l)
        off, sz, ch, n = map(int, m.groups())
        with tempfile.NamedTemporaryFile(suffix=".ast", delete=False) as t: t.write(data[off:off + sz])
        ref = subprocess.run(["ffmpeg", "-v", "quiet", "-i", t.name, "-f", "s16le", "-"], capture_output=True).stdout
        os.unlink(t.name)
        if ref != mine[pos:pos + n * ch * 2]: bad += 1
        pos += n * ch * 2
    print("je Strom " + ("gleich" if not bad else f"{bad} Abweichungen"), f)
    ok &= bad == 0
sys.exit(0 if ok else 1)
