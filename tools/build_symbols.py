#!/usr/bin/env python3
"""Einmaliger Aufbau von symbols/functions.csv und symbols/data.csv (WP-01).

Quellen: decompiled/full/out2/functions.tsv (alle Funktionen, Ghidra-FID-Namen),
Kommentare `/* name @0xADDR (Sicherheit ...` in decompiled/speed/src/*.c,
Tabellen in decompiled/speed/NOTES.md, Globals aus decompiled/speed/src/game.h.
Danach ist symbols/ die Quelle der Wahrheit; dieses Skript überschreibt nur mit --force.
"""
import csv, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SPEED = ROOT / "decompiled/speed"
OUT = ROOT / "symbols"
COLS = "addr,name,module,kind,status,confidence,source,size,convention,prototype,note".split(",")

def a(x): return "0x%06x" % int(x, 16)

def main():
    if (OUT / "functions.csv").exists() and "--force" not in sys.argv:
        sys.exit("symbols/functions.csv existiert schon (Quelle der Wahrheit). --force zum Überschreiben.")
    rows = {}
    for line in open(ROOT / "decompiled/full/out2/functions.tsv"):
        addr, name, size, thunk, conv, proto = line.rstrip("\n").split("\t")
        r = dict.fromkeys(COLS, "")
        r.update(addr=a(addr), name=name, size=size, convention=conv, prototype=proto,
                 kind="unbekannt", status="unbearbeitet")
        if thunk == "true":
            r["kind"] = "thunk"
        if not re.match(r"(FUN_|thunk_FUN_|caseD_|switchD_|LAB_)", name):
            r.update(source="fid", confidence="wahrscheinlich", status="benannt")
            if thunk != "true":
                r["kind"] = "crt"
        else:
            r["name"] = ""  # Ghidra-Platzhalter nicht in die DB
            if name.startswith("caseD_"):
                r["note"] = "Ghidra: " + name
        rows[r["addr"]] = r

    prog = {}
    for line in open(SPEED / "inventar/ebene3_progress.txt"):
        p = line.split()
        if len(p) >= 3: prog[a(p[0])] = p[2]

    conflicts, legacy = [], {}
    def put(addr, name, module, status, conf, note):
        if addr in legacy and legacy[addr][0] != name:
            conflicts.append((addr, legacy[addr][0], name))
            return
        legacy.setdefault(addr, (name, module, status, conf, note))

    pat = re.compile(r"/\*\s*([A-Za-z_][A-Za-z0-9_:]*)\s*@0x([0-9a-fA-F]+)\s*(?:\(([^)\n]*)\)?)?([^\n]*)")
    for f in sorted((SPEED / "src").glob("*.c")):
        for m in pat.finditer(f.read_text(errors="replace")):
            name, addr, tag, rest = m.group(1), a(m.group(2)), (m.group(3) or ""), m.group(4)
            t = tag.lower()
            st = prog.get(addr, "")
            if t.startswith("exakt") or st == "DONE-exakt":
                status = "rekonstruiert"
            else:
                status = "skizze"
            conf = ("sicher" if t.startswith("sicher") else
                    "vermutet" if t.startswith("unklar") else "wahrscheinlich")
            note = ("alt: " + tag + rest).strip().rstrip("*/ ").replace("\n", " ")[:200]
            put(addr, name, f.stem, status, conf, note)

    # NOTES.md, Ebene 1 und 2 (falls ein Name nicht in src/ kommentiert ist)
    notes = (SPEED / "NOTES.md").read_text()
    for m in re.finditer(r"\|\s*0x([0-9a-f]+)\s*\|\s*([a-z_][a-z0-9_]*)", notes):
        addr = a(m.group(1))
        if addr not in legacy: put(addr, m.group(2), "", "benannt", "wahrscheinlich", "alt: NOTES Ebene 1")
    for m in re.finditer(r"([a-z_][a-z0-9_]+) 0x([0-9a-f]{6})", notes):
        addr = a(m.group(2))
        if addr != "0x400000" and addr not in legacy: put(addr, m.group(1), "", "benannt", "wahrscheinlich", "alt: NOTES")

    missing, fid_conf = [], []
    for addr, (name, module, status, conf, note) in legacy.items():
        r = rows.get(addr)
        if r is None:
            missing.append((addr, name)); continue
        if r["source"] == "fid" and r["name"] != "entry":
            fid_conf.append((addr, r["name"], name))
            r["note"] = (r["note"] + " alt: " + name).strip()
            continue
        r.update(name=name, module="speed/" + module if module else "", status=status,
                 confidence=conf, source="notes", note=note)

    OUT.mkdir(exist_ok=True)
    with open(OUT / "functions.csv", "w", newline="") as fh:
        w = csv.DictWriter(fh, COLS); w.writeheader()
        for k in sorted(rows, key=lambda x: int(x, 16)): w.writerow(rows[k])

    # Globals aus game.h
    data = {}
    dpat = re.compile(r"extern\s+([^;]+?)\s*\b(g_[A-Za-z0-9_]+)\s*(\[[^\]]*\])?\s*;\s*/\*\s*_?DAT_([0-9a-fA-F]{8})\s*([^*]*)")
    for f in [SPEED / "src/game.h"] + sorted((SPEED / "src").glob("*.c")):
        for m in dpat.finditer(f.read_text(errors="replace")):
            typ, name, arr, addr, note = m.groups()
            data.setdefault(a(addr), dict(addr=a(addr), name=name, type=(typ + (arr or "")).strip(),
                                          size="", module="speed/" + f.stem,
                                          note=note.strip(" ,…").strip()[:200]))
    with open(OUT / "data.csv", "w", newline="") as fh:
        w = csv.DictWriter(fh, "addr,name,type,size,module,note".split(",")); w.writeheader()
        for k in sorted(data, key=lambda x: int(x, 16)): w.writerow(data[k])

    print(f"functions: {len(rows)}  altnamen: {len(legacy)}  data: {len(data)}")
    print(f"KONFLIKT alt/alt: {conflicts}")
    print(f"KONFLIKT fid/alt: {fid_conf}")
    print(f"FEHLT in out2: {missing}")

if __name__ == "__main__":
    main()
