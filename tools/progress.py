#!/usr/bin/env python3
"""Fortschritt aus symbols/functions.csv als Markdown-Tabellen (je status, kind, module)."""
import csv
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
rows = list(csv.DictReader(open(ROOT / "symbols/functions.csv", newline="")))
total = len(rows)
size = sum(int(r["size"] or 0) for r in rows)
print(f"Funktionen: {total}, Bytes: {size}\n")
for col in ("status", "kind", "module"):
    c, b = Counter(), Counter()
    for r in rows:
        k = r[col] or "–"
        c[k] += 1
        b[k] += int(r["size"] or 0)
    print(f"| {col} | Anzahl | % | Bytes % |\n|---|---:|---:|---:|")
    for k, n in c.most_common():
        print(f"| {k} | {n} | {100*n/total:.1f} | {100*b[k]/size:.1f} |")
    print()
