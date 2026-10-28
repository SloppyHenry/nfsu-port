#!/usr/bin/env python3
"""Abgleich PC <-> GC (WP-08): Anker über Strings, Ausbreitung über den Aufrufgraphen,
Ausrichtung in Linkreihenfolge (Needleman-Wunsch zwischen gesicherten Ankern).

  python3 tools/match/match.py            # voller Lauf -> symbols/pc_gc_map.csv
  python3 tools/match/match.py --cv       # Kreuzvalidierung: 25 % der String-Anker verbergen
"""
import argparse, bisect, collections, csv, math, os, random, re, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import features
import lits

ROOT = features.ROOT
GC_TEXT_LO, GC_LINKONCE_LO = 0x800034A0, 0x8025D5B0


def load_vtables(pc, gc):
    import struct
    sys.path.insert(0, os.path.join(ROOT, 'tools'))
    from gc_elf import Elf
    # PC: Läufe aus Funktionsanfängen in .rdata/.data
    data = open(os.path.join(ROOT, 'game/pc/Speed.exe'), 'rb').read()
    nsec = struct.unpack_from('<H', data, struct.unpack_from('<I', data, 0x3c)[0] + 6)[0]
    opt = struct.unpack_from('<I', data, 0x3c)[0] + 24
    osz = struct.unpack_from('<H', data, struct.unpack_from('<I', data, 0x3c)[0] + 20)[0]
    pcvt = {}
    for i in range(nsec):
        o = opt + osz + i * 40
        name = data[o:o + 8].rstrip(b'\0').decode()
        vsz, va, rsz, raw = struct.unpack_from('<IIII', data, o + 8)
        if name not in ('.rdata', '.data'):
            continue
        n = min(vsz, rsz) // 4
        words = struct.unpack_from('<%dI' % n, data, raw)
        i = 0
        while i < n:
            if words[i] in pc:
                j = i
                while j < n and words[j] in pc:
                    j += 1
                if j - i >= 2 or (j - i == 1 and False):
                    pcvt[0x400000 + va + i * 4] = list(words[i:j])
                i = j
            else:
                i += 1
    e = Elf()
    gcvt = {}
    for sy in e.symbols:
        if sy.name.startswith('_vt.') or sy.name.startswith('_vt$'):
            sec = e.sections[sy.shndx]
            o = sec.offset + sy.value - sec.addr
            w = struct.unpack('>%dI' % (sy.size // 4), e.data[o:o + sy.size])
            sl = [w[k] for k in range(3, len(w), 2)]
            while sl and sl[-1] == 0:
                sl.pop()
            gcvt[sy.name] = sl
    return pcvt, gcvt


class Matcher:
    def __init__(self, d):
        self.pc, self.gc = d['pc'], d['gc']
        self.pcs = sorted(self.pc)
        self.gcs = sorted(a for a in self.gc if a >= GC_TEXT_LO)
        self.pc_callers = collections.defaultdict(list)
        self.gc_callers = collections.defaultdict(list)
        for a, f in self.pc.items():
            for c in f.callees:
                self.pc_callers[c].append(a)
        for a, f in self.gc.items():
            for c in f.callees:
                self.gc_callers[c].append(a)
        self.M = {}        # pc -> gc
        self.R = {}        # gc -> pc
        self.beleg = {}    # pc -> beleg
        self.mu, self.sigma = 0.3, 0.5
        self.GM = {}       # PC-Global -> GC-Global
        self.gm_used = set()
        self.vt_pairs = []  # (pc_vtable, gc_vtable_name, gc_vtable, Treffer)
        self.pc_vt, self.gc_vt = load_vtables(self.pc, self.gc)
        # Konstanten-Häufigkeit
        self.pc_cc = collections.Counter(c for f in self.pc.values() for c in f.consts)
        self.gc_cc = collections.Counter(c for f in self.gc.values() for c in f.consts)
        self.lits = lits.load()
        df = collections.Counter()
        for side in ('pc', 'gc'):
            for c in self.lits[side].values():
                df.update(c.keys())
        self.lit_w = {k: math.log(1 + 1000 / n) for k, n in df.items()}
        self.pc_sc = collections.Counter(s for f in self.pc.values() for s in f.strings)
        self.gc_sc = collections.Counter(s for f in self.gc.values() for s in f.strings)

    # ---------- Zuordnung ----------
    def bind(self, p, g, beleg):
        if p in self.M or g in self.R:
            return False
        self.M[p], self.R[g] = g, p
        self.beleg[p] = beleg
        return True

    def unbind(self, p):
        g = self.M.pop(p)
        del self.R[g]
        self.beleg.pop(p, None)

    def lit_sim(self, p, g):
        """IDF-gewichtete Jaccard-Ähnlichkeit der Zahlenliterale (None, wenn eine Seite keine hat)."""
        a, b = self.lits['pc'].get(p), self.lits['gc'].get(g)
        if not a or not b:
            return None
        w = self.lit_w
        inter = sum(w[k] for k in a.keys() & b.keys())
        return inter / sum(w[k] for k in a.keys() | b.keys())

    # ---------- Bewertung ----------
    def score(self, p, g):
        a, b = self.pc[p], self.gc[g]
        s = 0.0
        # Strings
        sh = a.strings & b.strings
        if sh:
            for t in sh:
                n = max(self.pc_sc[t], self.gc_sc[t])
                s += (5.0 if n == 1 else 2.0 / n ** 0.5) * (1 if len(t) >= 6 else 0.4)
        elif a.strings and b.strings:
            s -= 1.0
        # Größe
        if a.size > 0 and b.size > 0:
            lr = math.log(b.size / a.size) - self.mu
            if abs(lr) > 1.6 and not sh and min(a.size, b.size) >= 24:
                return -10.0
            w = 1.0 if min(a.size, b.size) >= 24 else 0.5
            s += w * max(-3.0, 1.0 - (lr / self.sigma) ** 2 / 2)
        # Parameter
        if a.nparams == b.nparams:
            s += 0.3
        elif abs(a.nparams - b.nparams) >= 2:
            s -= 0.5
        # Aufrufziele / Aufrufer über bestehende Zuordnung
        cp = {self.M[c] for c in a.callees if c in self.M}
        cg = set(b.callees)
        if cp:
            ov = len(cp & cg)
            s += 2.0 * min(ov, 6)
            if cg:
                s -= 0.4 * min(len(cp - cg), 4)
        kp = {self.M[c] for c in self.pc_callers.get(p, ()) if c in self.M}
        kg = set(self.gc_callers.get(g, ()))
        if kp:
            s += 1.5 * min(len(kp & kg), 6)
        # globale Variablen über bestehende Zuordnung
        if self.GM and a.globs and b.globs:
            gp = {self.GM[x] for x in a.globs if x in self.GM}
            if gp:
                s += 1.2 * min(len(gp & b.globs), 5)
                if not (gp & b.globs) and len(gp) >= 2:
                    s -= 0.8
        # Konstanten
        sc = 0
        for c in a.consts & b.consts:
            if c >= 0x100 and self.pc_cc[c] <= 4 and self.gc_cc[c] <= 4:
                sc += 1
        s += min(sc, 4) * 0.8
        # Zahlenliterale (Zufallsniveau: 97 % der falschen Paare < 0,16)
        ls = self.lit_sim(p, g)
        if ls is not None:
            if ls >= 0.45:
                s += 3.0
            elif ls >= 0.25:
                s += 2.0
            elif ls >= 0.15:
                s += 1.0
            elif ls == 0 and len(self.lits['pc'][p]) >= 3 and len(self.lits['gc'][g]) >= 3:
                s -= 0.5
        return s

    # ---------- Anker über Strings ----------
    def string_anchors(self, exclude=()):
        ip, ig = collections.defaultdict(set), collections.defaultdict(set)
        for a, f in self.pc.items():
            for t in f.strings:
                if len(t) >= 4:
                    ip[t].add(a)
        for a, f in self.gc.items():
            for t in f.strings:
                if len(t) >= 4:
                    ig[t].add(a)
        votes = collections.defaultdict(collections.Counter)
        for t in set(ip) & set(ig):
            if len(ip[t]) == 1 and len(ig[t]) == 1:
                p, g = next(iter(ip[t])), next(iter(ig[t]))
                votes[p][g] += 1
        res = []
        for p, c in votes.items():
            if p in exclude:
                continue
            (g, n), *rest = c.most_common()
            if rest and rest[0][1] == n:
                continue
            res.append((p, g, n))
        # GC-Seite eindeutig
        cnt = collections.Counter(g for _, g, _ in res)
        return [(p, g, n) for p, g, n in res if cnt[g] == 1]

    def fit_size(self):
        xs = [math.log(self.gc[g].size / self.pc[p].size) for p, g in self.M.items()
              if self.pc[p].size >= 24 and self.gc[g].size >= 24]
        if len(xs) > 50:
            xs.sort()
            self.mu = xs[len(xs) // 2]
            dev = sorted(abs(x - self.mu) for x in xs)
            self.sigma = max(0.25, dev[len(dev) // 2] * 1.4826)

    # ---------- Kette (Linkreihenfolge) ----------
    def chain(self, conf_only=True):
        """Längste in beiden Adressräumen aufsteigende Teilfolge der Zuordnung (nur Spielbereich)."""
        pairs = sorted((p, g) for p, g in self.M.items() if g < GC_LINKONCE_LO)
        tails, idx, prev = [], [], [-1] * len(pairs)
        for i, (p, g) in enumerate(pairs):
            k = bisect.bisect_left(tails, g)
            if k == len(tails):
                tails.append(g); idx.append(i)
            else:
                tails[k] = g; idx[k] = i
            prev[i] = idx[k - 1] if k else -1
        out, i = [], idx[-1] if idx else -1
        while i >= 0:
            out.append(pairs[i]); i = prev[i]
        return out[::-1]

    # ---------- Ausrichtung ----------
    def align_gaps(self, gap_pen=-0.6, min_score=1.2, cap=900, tag='link'):
        chain = self.chain()
        ch_p = [(0, 0)] + chain + [(0x7FFFFFFF, 0xFFFFFFFF)]
        new = 0
        for (p0, g0), (p1, g1) in zip(ch_p, ch_p[1:]):
            P = [a for a in self.pcs[bisect.bisect_right(self.pcs, p0):bisect.bisect_left(self.pcs, p1)]
                 if a not in self.M]
            G = [a for a in self.gcs[bisect.bisect_right(self.gcs, g0):bisect.bisect_left(self.gcs, g1)]
                 if a not in self.R]
            if p1 == 0x7FFFFFFF or p0 == 0:     # offene Enden: nicht erzwingen
                continue
            if not P or not G or len(P) > cap or len(G) > cap:
                continue
            new += self._nw(P, G, gap_pen, min_score, tag)
        return new

    def _nw(self, P, G, gap_pen, min_score, tag='link'):
        n, m = len(P), len(G)
        band = max(25, int(0.35 * max(n, m)))
        NEG = -1e9
        S = [[NEG] * (m + 1) for _ in range(n + 1)]
        T = [[0] * (m + 1) for _ in range(n + 1)]
        S[0][0] = 0
        for i in range(n + 1):
            for j in range(m + 1):
                if i == 0 and j == 0:
                    continue
                if abs(i * m / n - j) > band + 0 and abs(i - j * n / m) > band:
                    continue
                best, t = NEG, 0
                if i and S[i - 1][j] > NEG:
                    v = S[i - 1][j] + gap_pen
                    if v > best: best, t = v, 1
                if j and S[i][j - 1] > NEG:
                    v = S[i][j - 1] + gap_pen
                    if v > best: best, t = v, 2
                if i and j and S[i - 1][j - 1] > NEG:
                    sc = self.score(P[i - 1], G[j - 1]) - min_score * 0.5
                    v = S[i - 1][j - 1] + sc
                    if v > best: best, t = v, 3
                S[i][j], T[i][j] = best, t
        i, j, new = n, m, 0
        pairs = []
        while i or j:
            t = T[i][j]
            if t == 3:
                pairs.append((P[i - 1], G[j - 1])); i -= 1; j -= 1
            elif t == 1:
                i -= 1
            elif t == 2:
                j -= 1
            else:
                break
        for p, g in pairs:
            if self.score(p, g) >= min_score:
                if self.bind(p, g, tag):
                    new += 1
        return new

    # ---------- Globale Variablen ----------
    def learn_globals(self, min_votes=2):
        co = collections.defaultdict(collections.Counter)
        tot = collections.Counter()
        for p, g in self.M.items():
            a, b = self.pc[p], self.gc[g]
            if not a.globs or not b.globs or len(a.globs) > 12 or len(b.globs) > 12:
                continue
            for x in a.globs:
                tot[x] += 1
                for y in b.globs:
                    co[x][y] += 1
        res = {}
        for x, c in co.items():
            (y, n), *rest = c.most_common()
            if n >= min_votes and n >= 0.75 * tot[x] and (not rest or rest[0][1] < n * 0.6):
                res[x] = y
        cnt = collections.Counter(res.values())
        self.GM = {x: y for x, y in res.items() if cnt[y] == 1}
        return len(self.GM)

    # ---------- Vtables ----------
    def vtables_pass(self, min_hits=2):
        new = 0
        pairs = []
        for pv, pslots in self.pc_vt.items():
            votes = collections.Counter()
            for i, f in enumerate(pslots):
                g = self.M.get(f)
                if g is None:
                    continue
                for gv, gslots in self.gc_vt_by_fn.get(g, ()):
                    votes[(gv, gslots.index(g) - i)] += 1
            if not votes:
                continue
            (gv, delta), n = votes.most_common(1)[0]
            if n < min_hits and not (n == 1 and len(pslots) <= 2):
                continue
            if len(votes) > 1 and votes.most_common(2)[1][1] * 2 > n:
                continue
            gslots = self.gc_vt[gv]
            pairs.append((pv, gv, n))
            for i, f in enumerate(pslots):
                j = i + delta
                if 0 <= j < len(gslots) and f not in self.M and gslots[j] in self.gc and gslots[j] not in self.R:
                    if abs(math.log(max(1, self.gc[gslots[j]].size) / max(1, self.pc[f].size)) - self.mu) < 1.6:
                        if self.bind(f, gslots[j], 'vtable'):
                            new += 1
        self.vt_pairs = pairs
        return new

    # ---------- Ausbreitung über Aufrufgraph ----------
    def propagate(self, min_score=3.0, margin=1.0):
        cand = collections.defaultdict(set)
        for p, g in list(self.M.items()):
            for pc_, gc_ in ((self.pc[p].callees, self.gc[g].callees),
                             (self.pc_callers.get(p, ()), self.gc_callers.get(g, ()))):
                for x in pc_:
                    if x in self.M:
                        continue
                    for y in gc_:
                        if y not in self.R:
                            cand[x].add(y)
        best = {}
        for x, ys in cand.items():
            sc = sorted(((self.score(x, y), y) for y in ys), reverse=True)
            if sc[0][0] >= min_score and (len(sc) == 1 or sc[0][0] - sc[1][0] >= margin):
                best[x] = sc[0]
        # gegenseitig eindeutig
        rev = collections.defaultdict(list)
        for x, (s, y) in best.items():
            rev[y].append((s, x))
        new = 0
        for y, lst in rev.items():
            lst.sort(reverse=True)
            if len(lst) == 1 or lst[0][0] - lst[1][0] >= margin:
                if self.bind(lst[0][1], y, 'graph'):
                    new += 1
        return new

    def run(self, anchors, rounds=5, log=print):
        for p, g, n in anchors:
            self.bind(p, g, 'string')
        log(f'Anker (Strings): {len(self.M)}')
        self.fit_size()
        self.gc_vt_by_fn = collections.defaultdict(list)
        for gv, sl in self.gc_vt.items():
            for f in set(sl):
                self.gc_vt_by_fn[f].append((gv, sl))
        for r in range(rounds):
            n1 = self.propagate()
            self.fit_size()
            ng = self.learn_globals()
            n3 = self.vtables_pass()
            n2 = self.align_gaps()
            log(f'Runde {r + 1}: graph +{n1}, vtable +{n3}, link +{n2}, Globale {ng}, gesamt {len(self.M)} (mu={self.mu:.2f} sigma={self.sigma:.2f})')
            if n1 + n2 + n3 == 0:
                break
        for r in range(4):     # schwächere Schwelle: Reihenfolge + Größe + Nachbarn
            n2 = self.align_gaps(min_score=0.5, tag='link-schwach')
            n1 = self.propagate()
            self.learn_globals()
            n3 = self.vtables_pass()
            self.fit_size()
            log(f'Schwach {r + 1}: link +{n2}, graph +{n1}, vtable +{n3}, gesamt {len(self.M)}')
            if n1 + n2 + n3 == 0:
                break
        for r in range(4):     # Ausbreitung mit niedrigerer Schwelle, jetzt mit Literalen und mehr Kontext
            n1 = self.propagate(min_score=2.0, margin=0.6)
            n2 = self.align_gaps(min_score=0.5, tag='link-schwach', cap=1500)
            self.learn_globals()
            n3 = self.vtables_pass()
            self.fit_size()
            log(f'Nachlauf {r + 1}: graph +{n1}, link +{n2}, vtable +{n3}, gesamt {len(self.M)}')
            if n1 + n2 + n3 == 0:
                break


def classify(m, p):
    g = m.M[p]
    sc = m.score(p, g)
    shared = [t for t in m.pc[p].strings & m.gc[g].strings
              if len(t) >= 6 and m.pc_sc[t] == 1 and m.gc_sc[t] == 1]
    if shared or sc >= 14:
        return 'sicher', sc
    if sc >= 4.0 or m.beleg[p] == 'vtable' and sc >= 1.0:
        return 'wahrscheinlich', sc
    return 'moeglich', sc


def gc_symbol_name(r, fallback=''):
    """Klasse::Methode aus gc_functions.csv, bereinigt für Ghidra (nur [A-Za-z0-9_~:])."""
    n = (r['klasse'] + '::' if r.get('klasse') else '') + (r.get('methode') or '')
    if not r.get('methode'):
        n = (r.get('demangled') or fallback).split('(')[0]
    n = re.sub(r'[^A-Za-z0-9_~:]+', '_', n).strip('_')
    return n or fallback


def lib_functions():
    """PC-Funktionen, die WP-01/02 als Bibliothek (FID, CRT, D3DX, zlib, Thunk) führen: nie zuordnen."""
    out = set()
    for r in csv.DictReader(open(os.path.join(ROOT, 'symbols/functions.csv'))):
        if r['source'] == 'fid' or r['kind'] in ('crt', 'thunk', 'd3dx') or r['module'].startswith(('lib', 'pc/utillib')):
            out.add(int(r['addr'], 16))
    return out


def write_map(m, path):
    gcrow = {int(r['gc_addr'], 16): r for r in csv.DictReader(open(os.path.join(ROOT, 'symbols/gc_functions.csv')))}
    with open(path, 'w', newline='') as fh:
        w = csv.writer(fh)
        w.writerow(['pc_addr', 'gc_addr', 'gc_name', 'quelldatei', 'beleg', 'confidence', 'score', 'gc_demangled'])
        libs = lib_functions()
        for p in sorted(m.M):
            if p in libs:
                continue
            g = m.M[p]
            r = gcrow.get(g, {})
            conf, sc = classify(m, p)
            src = r.get('quelldatei') or r.get('objekt', '')
            w.writerow([f'0x{p:x}', f'0x{g:X}', gc_symbol_name(r, m.gc[g].name), src, m.beleg[p], conf,
                        f'{sc:.1f}', r.get('demangled', '')])


def game_scope():
    """PC-Funktionen im Spielbereich: 0x401000-0x5a0000 ohne CRT/Thunks (functions.csv)."""
    out = set()
    for r in csv.DictReader(open(os.path.join(ROOT, 'symbols/functions.csv'))):
        a = int(r['addr'], 16)
        if 0x401000 <= a < 0x5a0000 and r['kind'] not in ('crt', 'thunk'):
            out.add(a)
    return out


def cross_validate(seed=1, frac=0.25):
    d = features.load()
    m0 = Matcher(d)
    anchors = m0.string_anchors()
    rnd = random.Random(seed)
    hidden = {p: g for p, g, n in anchors if rnd.random() < frac}
    keep = [(p, g, n) for p, g, n in anchors if p not in hidden]
    for p, g in hidden.items():     # versteckte Funktionen verlieren Strings: Test ohne den stärksten Beleg
        d['pc'][p].strings = set()
        d['gc'][g].strings = set()
    m = Matcher(d)
    m.run(keep)
    got = {p: m.M[p] for p in hidden if p in m.M}
    ok = sum(1 for p, g in got.items() if hidden[p] == g)
    print(f'verborgen {len(hidden)}, zugeordnet {len(got)}, richtig {ok}, falsch {len(got) - ok}')
    by = collections.Counter((m.beleg[p], hidden[p] == g) for p, g in got.items())
    print(dict(by))
    tier = collections.Counter((classify(m, p)[0], hidden[p] == g) for p, g in got.items())
    print('Stufen:', dict(tier))
    return tier


if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('--cv', action='store_true')
    a = ap.parse_args()
    if a.cv:
        tot = collections.Counter()
        for sd in range(1, 5):
            tot.update(cross_validate(seed=sd, frac=0.4))
        for t in ('sicher', 'wahrscheinlich', 'moeglich'):
            ok, bad = tot[(t, True)], tot[(t, False)]
            print(f'{t}: {ok}/{ok + bad} = {100 * ok / max(1, ok + bad):.0f} %')
    else:
        d = features.load()
        m = Matcher(d)
        m.run(m.string_anchors(), rounds=6)
        out = os.path.join(ROOT, 'symbols/pc_gc_map.csv')
        write_map(m, out)
        scope = game_scope() - lib_functions()
        got = [p for p in scope if p in m.M]
        print(f'Zuordnungen {len(m.M)}; Spielbereich {len(got)}/{len(scope)} = {100 * len(got) / len(scope):.1f} %')
        c = collections.Counter(classify(m, p)[0] for p in m.M)
        print(dict(c), dict(collections.Counter(m.beleg.values())))
