#!/usr/bin/env python3
"""Zahlenliterale je Funktion aus dem Pseudocode (WP-08, Fortsetzung): Multimenge ganzer Zahlen
und Gleitkommazahlen. Cache: refs/match_lits.pkl. Dient als weiterer Beleg (Jaccard mit IDF)."""
import os, pickle, re, sys, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import features
ROOT = features.ROOT
CACHE = os.path.join(ROOT, 'refs/match_lits.pkl')
NUM = re.compile(r'(?<![\w.])(?:0x[0-9a-fA-F]+|\d+\.\d+(?:e[+-]?\d+)?f?|\d+)(?![\w.])')
HDR = features.HDR
SKIP = {0, 1, 2, 0xff, 0xffffffff, 0x80000000}


def parse(path):
    text = open(path, errors='replace').read()
    hs = list(HDR.finditer(text))
    out = {}
    for i, h in enumerate(hs):
        a = int(h.group(1), 16)
        body = text[h.end(): hs[i + 1].start() if i + 1 < len(hs) else len(text)]
        k = body.find('\n{')
        code = body[k:] if k >= 0 else body
        code = re.sub(r'"(?:[^"\\\n]|\\.)*"', '', code)
        c = collections.Counter()
        for m in NUM.findall(code):
            t = m.lower().rstrip('f')
            try:
                if t.startswith('0x'):
                    v = int(t, 16)
                    if v in SKIP or v >= 0x400000:    # Adressen
                        continue
                    key = ('i', v)
                elif '.' in t:
                    key = ('f', round(float(t), 4))
                else:
                    v = int(t)
                    if v in SKIP:
                        continue
                    key = ('i', v)
            except ValueError:
                continue
            c[key] += 1
        out[a] = c
    return out


def load(force=False):
    if not force and os.path.exists(CACHE):
        return pickle.load(open(CACHE, 'rb'))
    d = {'pc': parse(os.path.join(ROOT, 'decompiled/full/out3/all_functions.c')),
         'gc': parse(os.path.join(ROOT, 'decompiled/gc/out/all_functions.c'))}
    pickle.dump(d, open(CACHE, 'wb'))
    return d


if __name__ == '__main__':
    d = load(True)
    print({k: len(v) for k, v in d.items()})
