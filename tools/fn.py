#!/usr/bin/env python3
"""Pseudocode und Querverweise einer Funktion aus dem neuesten Ghidra-Export (WP-01).

  fn.py 0x4448b0 | fn.py game_main      Pseudocode
  --brief    nur Signatur, Größe, Status und Aufrufe
  --callers  Aufrufer   --callees  Aufgerufene   --strings  referenzierte Strings
Namen kommen aus symbols/functions.csv. Export: decompiled/full/out3, sonst out2.

  --gc       GameCube-Export decompiled/gc/out (WP-07); Ziel: Adresse, Name, Klasse::Methode,
             gemangelter Name oder Teilstring. Metadaten aus symbols/gc_functions.csv.
"""
import argparse, csv, os, pickle, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
EXPORTS = [ROOT / "decompiled/full/out3", ROOT / "decompiled/full/out2"]
HDR = re.compile(rb"^/\* ==== ([0-9a-f]+) (\S+) ==== \*/$", re.M)

def norm(a):
    return "0x%06x" % int(a, 16)

def export_dir():
    for d in EXPORTS:
        if (d / "all_functions.c").exists():
            return d
    sys.exit("kein Export gefunden")

def load_index(d):
    src = d / "all_functions.c"
    idx_file = d / ".fnindex"
    if idx_file.exists() and idx_file.stat().st_mtime >= src.stat().st_mtime:
        with open(idx_file, "rb") as fh:
            return pickle.load(fh)
    data = src.read_bytes()
    starts = [(m.start(), norm(m.group(1).decode())) for m in HDR.finditer(data)]
    idx = {}
    for i, (off, a) in enumerate(starts):
        end = starts[i + 1][0] if i + 1 < len(starts) else len(data)
        idx[a] = (off, end)
    with open(idx_file, "wb") as fh:
        pickle.dump(idx, fh)
    return idx

def load_symbols():
    syms, byname = {}, {}
    with open(ROOT / "symbols/functions.csv", newline="") as fh:
        for r in csv.DictReader(fh):
            syms[r["addr"]] = r
            if r["name"]:
                byname.setdefault(r["name"], r["addr"])
    return syms, byname

def label(syms, a):
    r = syms.get(a)
    return f"{a} {r['name'] or 'FUN_%08x' % int(a, 16)}" if r else a

def callgraph(d):
    p = d / "callgraph.tsv"
    if not p.exists():
        return None
    edges = []
    for line in open(p):
        c, e, site = line.rstrip("\n").split("\t")
        edges.append((norm(c), e if e.startswith("EXTERNAL") else norm(e), site))
    return edges

def callees_from_code(code):
    return sorted({norm(m) for m in re.findall(r"FUN_([0-9a-f]{8})", code)})

GC_EXPORT = ROOT / "decompiled/gc/out"
GC_HDR = re.compile(rb"^/\* ==== ([0-9a-f]+) (.*?) ==== \*/$", re.M)

def gc_main(o):
    """--gc: Pseudocode aus dem GC-Export, Namen/Quelle aus symbols/gc_functions.csv."""
    src = GC_EXPORT / "all_functions.c"
    if not src.exists():
        sys.exit(f"kein GC-Export: {src}")
    data = src.read_bytes()
    heads = [(m.start(), int(m.group(1), 16), m.group(2).decode("latin1")) for m in GC_HDR.finditer(data)]
    meta = {}
    with open(ROOT / "symbols/gc_functions.csv", newline="") as fh:
        for r in csv.DictReader(fh):
            meta[int(r["gc_addr"], 16)] = r
    t = o.target
    if re.fullmatch(r"(0x)?[0-9a-fA-F]{8}", t):
        a = int(t, 16)
        hits = [h for h in heads if h[1] == a] or [h for h in heads if h[1] <= a][-1:]
    else:
        hits = [h for h in heads if h[2] == t or h[2].endswith("::" + t)]
        if not hits:
            addrs = {a for a, r in meta.items() if t in (r["mangled"], r["demangled"])}
            hits = [h for h in heads if h[1] in addrs]
        if not hits:
            hits = [h for h in heads if t.lower() in h[2].lower()]
    if not hits:
        sys.exit(f"unbekannt: {t}")
    if len(hits) > 5:
        for h in hits[:200]:
            print(f"0x{h[1]:08x}  {h[2]}")
        sys.exit(0 if len(hits) <= 200 else f"… {len(hits)} Treffer")
    pos = {h[0]: i for i, h in enumerate(heads)}
    for h in hits:
        i = pos[h[0]]
        end = heads[i + 1][0] if i + 1 < len(heads) else len(data)
        code = data[h[0]:end].decode("latin1").rstrip()
        r = meta.get(h[1], {})
        print(f"0x{h[1]:08x} {h[2]}  size={r.get('size','')}  export=gc/out")
        if r:
            print(f"  {r['demangled']}")
            loc = r["quelldatei"] + (f":{r['zeilen']}" if r["zeilen"] else "")
            print(f"  Quelle: {loc}  (Objekt {r['objekt']}, sicher: {r['objekt_sicher']})")
            if r["rueckgabe"] or r["parameter"]:
                print(f"  DWARF: {r['rueckgabe']} ({r['parameter']})  statisch={r['statisch']} virtuell={r['virtuell']}")
        if o.brief:
            sig = next((l for l in code.splitlines()[1:] if l.strip()), "")
            print("  " + sig.strip())
        else:
            print(code)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("target")
    ap.add_argument("--brief", action="store_true")
    ap.add_argument("--callers", action="store_true")
    ap.add_argument("--callees", action="store_true")
    ap.add_argument("--strings", action="store_true")
    ap.add_argument("--gc", action="store_true", help="GameCube-Export (WP-07)")
    o = ap.parse_args()
    if o.gc:
        return gc_main(o)

    d = export_dir()
    syms, byname = load_symbols()
    t = o.target
    if re.fullmatch(r"(0x)?[0-9a-fA-F]{5,8}", t):
        a = norm(t)
    elif t in byname:
        a = byname[t]
    elif t.startswith("FUN_"):
        a = norm(t[4:])
    else:
        hits = [n for n in byname if t in n]
        sys.exit(f"unbekannt: {t}" + (f"; ähnlich: {', '.join(hits[:15])}" if hits else ""))

    idx = load_index(d)
    code = ""
    if a in idx:
        off, end = idx[a]
        with open(d / "all_functions.c", "rb") as fh:
            fh.seek(off)
            code = fh.read(end - off).decode(errors="replace")
    r = syms.get(a, {})
    edges = callgraph(d)
    extra = o.callers or o.callees or o.strings

    if o.brief or extra:
        sig = next((l for l in code.splitlines()[1:] if l.strip() and not l.lstrip().startswith("/*")), r.get("prototype", ""))
        print(f"{label(syms, a)}  [{r.get('module','')}|{r.get('kind','')}|{r.get('status','')}|"
              f"{r.get('confidence','')}] size={r.get('size','')}  export={d.name}")
        print("  " + sig.strip())
        if r.get("note"):
            print("  note: " + r["note"])
    if o.brief or o.callees:
        if edges is not None:
            out = sorted({e for c, e, _ in edges if c == a})
        else:
            out = [x for x in callees_from_code(code) if x != a]
        print(f"  callees ({len(out)}): " + ", ".join(label(syms, x) if x.startswith("0x") else x for x in out))
    if o.brief or o.callers:
        if edges is not None:
            inc = sorted({c for c, e, _ in edges if e == a})
            print(f"  callers ({len(inc)}): " + ", ".join(label(syms, x) for x in inc))
        else:
            print("  callers: erst mit out3/callgraph.tsv")
    if o.strings:
        p = d / "strrefs.tsv"
        if p.exists():
            for line in open(p):
                f, sa, s = line.rstrip("\n").split("\t", 2)
                if norm(f) == a:
                    print(f"  str {sa}: {s}")
        else:
            print("  strings: erst mit out3/strrefs.tsv")
    if not (o.brief or extra):
        print(code if code else f"{a}: kein Pseudocode im Export {d.name}")

if __name__ == "__main__":
    main()
