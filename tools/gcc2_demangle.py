#!/usr/bin/env python3
"""Demangler für GNU-v2-Namen (GCC 2.95, SN Systems), wie sie in der GC-Speed.elf stehen.

demangle(sym) -> Demangled | None
  .qualname   'Joystick::GenerateEventsFromScanners'
  .klass      'Joystick' ('' bei freien Funktionen)
  .method     'GenerateEventsFromScanners'
  .args       ['int', 'unsigned char *', ...]
  .const      True bei const-Methoden
  .full       vollständige Signatur

Aufruf: tools/gcc2_demangle.py <name> ...
"""
import re
import sys
from dataclasses import dataclass, field

BUILTIN = {'v': 'void', 'b': 'bool', 'c': 'char', 's': 'short', 'i': 'int', 'l': 'long',
           'x': 'long long', 'f': 'float', 'd': 'double', 'r': 'long double', 'w': 'wchar_t',
           'e': '...'}

OPS = {
    'nw': 'new', 'dl': 'delete', 'vn': 'new []', 'vd': 'delete []', 'as': '=', 'ne': '!=',
    'eq': '==', 'ge': '>=', 'gt': '>', 'le': '<=', 'lt': '<', 'pl': '+', 'apl': '+=', 'mi': '-',
    'ami': '-=', 'ml': '*', 'aml': '*=', 'dv': '/', 'adv': '/=', 'md': '%', 'amd': '%=',
    'ls': '<<', 'als': '<<=', 'rs': '>>', 'ars': '>>=', 'aa': '&&', 'oo': '||', 'nt': '!',
    'co': '~', 'ad': '&', 'aad': '&=', 'or': '|', 'aor': '|=', 'er': '^', 'aer': '^=',
    'pp': '++', 'mm': '--', 'rf': '->', 'rm': '->*', 'vc': '[]', 'cl': '()', 'cm': ',',
}


class Fail(Exception):
    pass


@dataclass
class Demangled:
    klass: str = ''
    method: str = ''
    args: list = field(default_factory=list)
    const: bool = False
    special: str = ''

    @property
    def qualname(self):
        return f'{self.klass}::{self.method}' if self.klass else self.method

    @property
    def full(self):
        if self.special:
            return self.special
        a = ', '.join(self.args) if self.args != ['void'] else 'void'
        return f'{self.qualname}({a})' + (' const' if self.const else '')


class P:
    def __init__(self, s):
        self.s, self.i = s, 0
        self.types = []
        self.tparams = []

    def peek(self):
        return self.s[self.i] if self.i < len(self.s) else ''

    def take(self, n=1):
        if self.i + n > len(self.s):
            raise Fail
        r = self.s[self.i:self.i + n]
        self.i += n
        return r

    def num(self):
        m = re.match(r'\d+', self.s[self.i:])
        if not m:
            raise Fail
        self.i += len(m.group())
        return int(m.group())

    def count(self):
        """Ein Ziffer oder mehrere Ziffern mit abschließendem '_'."""
        if self.peek() == '_':
            self.take()
            n = self.num()
            if self.peek() == '_':
                self.take()
            return n
        m = re.match(r'\d+', self.s[self.i:])
        if not m:
            raise Fail
        if len(m.group()) > 1 and self.s[self.i + len(m.group()):self.i + len(m.group()) + 1] == '_':
            self.i += len(m.group()) + 1
            return int(m.group())
        self.i += 1
        return int(m.group()[0])

    def name(self):
        n = self.num()
        if self.i + n > len(self.s):
            raise Fail
        return self.take(n)

    def class_name(self):
        c = self.peek()
        if c == 'Q':
            self.take()
            if self.peek() == '_':
                self.take()
                k = self.num()
                if self.peek() == '_':
                    self.take()
            else:
                k = int(self.take())
            return '::'.join(self.class_name() for _ in range(k))
        if c == 't':
            return self.template()
        if c.isdigit():
            return self.name()
        if c == 'K':           # squangling, hier nicht erwartet
            raise Fail
        raise Fail

    def template(self):
        self.take()  # 't'
        base = self.name()
        n = self.count()
        args = []
        for _ in range(n):
            if self.peek() == 'Z':
                self.take()
                args.append(self.type())
            else:
                t = self.type()
                args.append(self.literal(t))
        return f'{base}<{", ".join(args)}>'

    def literal(self, t):
        neg = ''
        if self.peek() == 'm':
            self.take()
            neg = '-'
        if t == 'bool':
            return 'true' if self.take() == '1' else 'false'
        if t.endswith('*') or t.endswith('&'):
            # Zeiger-Templateargument: Länge + Name
            return '&' + self.name()
        m = re.match(r'\d+', self.s[self.i:])
        if not m:
            raise Fail
        self.i += len(m.group())
        if self.peek() == '_' and len(m.group()) > 1:
            self.take()
        return neg + m.group()

    def type(self):
        quals = []
        while self.peek() in ('C', 'V', 'U', 'S'):
            c = self.take()
            quals.append({'C': 'const', 'V': 'volatile', 'U': 'unsigned', 'S': 'signed'}[c])
        c = self.peek()
        if c == 'P':
            self.take()
            inner = self.type()
            return self._q(quals, self._ptr(inner, '*'))
        if c == 'R':
            self.take()
            return self._q(quals, self._ptr(self.type(), '&'))
        if c == 'A':
            self.take()
            n = self.num()
            if self.take() != '_':
                raise Fail
            return self._q(quals, f'{self.type()}[{n}]')
        if c == 'F':
            self.take()
            args = self.arglist(stop='_')
            self.take()
            ret = self.type()
            return self._q(quals, f'{ret} (*)({", ".join(args)})')
        if c == 'M':
            self.take()
            cls = self.class_name()
            quals2 = []
            while self.peek() in ('C', 'V'):
                quals2.append(self.take())
            if self.peek() != 'F':
                t = self.type()
                return self._q(quals, f'{t} {cls}::*')
            self.take()
            args = self.arglist(stop='_')
            self.take()
            ret = self.type()
            return self._q(quals, f'{ret} ({cls}::*)({", ".join(args)})' + (' const' if 'C' in quals2 else ''))
        if c == 'G':          # Klasse als Wert übergeben
            self.take()
            return self._q(quals, self.class_name())
        if c == 'X':          # Template-Parameter X<index><ebene>
            self.take()
            k = int(self.take())
            self.take()
            if k >= len(self.tparams):
                raise Fail
            return self._q(quals, self.tparams[k])
        if c == 'T':
            self.take()
            k = self.count()
            if k >= len(self.types):
                raise Fail
            return self._q(quals, self.types[k])
        if c in BUILTIN:
            self.take()
            return self._q(quals, BUILTIN[c])
        if c and (c.isdigit() or c in 'Qt'):
            return self._q(quals, self.class_name())
        raise Fail

    @staticmethod
    def _ptr(inner, op):
        if '(*)' in inner and op == '*':
            return inner.replace('(*)', '(**)', 1)
        if '(*)' in inner:
            return inner.replace('(*)', '(*&)', 1)
        return f'{inner} {op}' if not inner.endswith(('*', '&')) else f'{inner}{op}'

    @staticmethod
    def _q(quals, t):
        if not quals:
            return t
        # const vor Zeigern gehört zum Zeiger-Ziel; in v2 steht C vor dem gesamten Typ
        return ' '.join(quals + [t])

    def arglist(self, stop=''):
        args = []
        while self.i < len(self.s) and self.peek() != stop:
            if self.peek() == 'N':
                self.take()
                # N<anzahl><index>; einstellige Ziffern haben Vorrang (N32_ = 3x Typ 2)
                if re.match(r'\d\d', self.s[self.i:]):
                    rep, idx = int(self.take()), int(self.take())
                else:
                    rep = self.count()
                    idx = self.count()
                if idx >= len(self.types):
                    raise Fail
                for _ in range(rep):
                    args.append(self.types[idx])
                    self.types.append(self.types[idx])
                continue
            t = self.type()
            args.append(t)
            self.types.append(t)
        return args


def _sig(rest, klass_first):
    """rest nach '__': [C]<klasse><args> oder F<args>. Gibt (klasse, const, args)."""
    p = P(rest)
    const = False
    if p.peek() == 'H':       # Template-Funktion: H<n><args>_<parameter>_<rückgabe>
        p.take()
        n = p.count()
        for _ in range(n):
            if p.peek() == 'Z':
                p.take()
                p.tparams.append(p.type())
            else:
                t = p.type()
                p.tparams.append(p.literal(t))
        if p.take() != '_':
            raise Fail
        args = p.arglist(stop='_')
        p.take()
        p.type()  # Rückgabetyp
        if p.i != len(p.s):
            raise Fail
        return '', False, (args or ['void']), p.tparams
    if klass_first:
        if p.peek() == 'C' and p.s[p.i + 1:p.i + 2] in tuple('0123456789Qt'):
            p.take()
            const = True
        klass = p.class_name()
        p.types.append(klass)
    else:
        if p.take() != 'F':
            raise Fail
        klass = ''
    args = p.arglist()
    if p.i != len(p.s):
        raise Fail
    return klass, const, args or ['void'], []


def demangle(sym):
    s = sym
    # Sonderformen
    m = re.match(r'_GLOBAL_\.([ID])\.(.+)$', s)
    if m:
        inner = demangle(m.group(2))
        what = 'statische Initialisierung' if m.group(1) == 'I' else 'statische Zerstörung'
        d = Demangled(special=f'[{what}] ' + (inner.full if inner else m.group(2)))
        return d
    for pre in ('_vt$', '_vt.', '__vt_'):
        if s.startswith(pre):
            try:
                p = P(s[len(pre):].replace('$', '').replace('.', ''))
                cls = p.class_name() if s[len(pre)].isdigit() or s[len(pre)] in 'Qt' else s[len(pre):]
            except Fail:
                cls = s[len(pre):]
            return Demangled(klass=cls, method='__vtable', special=f'vtable für {cls}')
    for pre in ('_$_', '_._'):
        if s.startswith(pre):
            try:
                p = P(s[len(pre):])
                cls = p.class_name()
                if p.i != len(p.s):
                    raise Fail
            except Fail:
                return None
            last = cls.split('::')[-1].split('<')[0]
            return Demangled(klass=cls, method='~' + last, args=['void'])
    # Konstruktor: __<klasse><args>
    if s.startswith('__') and len(s) > 2 and (s[2].isdigit() or s[2] in 'Qt'):
        try:
            k, c, a, _ = _sig(s[2:], True)
            last = k.split('::')[-1].split('<')[0]
            return Demangled(klass=k, method=last, args=a, const=c)
        except Fail:
            pass
    # Operatoren: __<op>__<klasse>... oder __op<typ>__...
    start = 0
    if s.startswith('__'):
        start = 2
    # Trennstelle "__" suchen, an der die Signatur parsebar ist
    pos = s.find('__', max(start, 1))
    while pos != -1:
        name, rest = s[:pos], s[pos + 2:]
        if rest:
            for klass_first in ((True,) if rest[0] != 'F' else (False, True)):
                try:
                    k, c, a, tp = _sig(rest, klass_first)
                except Fail:
                    continue
                meth = _opname(name) + (f'<{", ".join(tp)}>' if tp else '')
                return Demangled(klass=k, method=meth, args=a, const=c)
        pos = s.find('__', pos + 1)
    return None


def _opname(name):
    if name.startswith('__'):
        op = name[2:]
        if op in OPS:
            return 'operator' + ('' if OPS[op][0].isalpha() is False else ' ') + OPS[op]
        if op.startswith('op'):
            try:
                t = P(op[2:]).type()
                return f'operator {t}'
            except Fail:
                pass
    return name


if __name__ == '__main__':
    for a in sys.argv[1:]:
        d = demangle(a)
        print(a, '->', d.full if d else '?')
