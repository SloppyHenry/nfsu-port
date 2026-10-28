"""Emulations-Harness (WP-05): einzelne Funktionen aus game/pc/Speed.exe in Unicorn (x86-32) ausführen.

Die Exe wird nicht gestartet. Nur der Abschnittsinhalt wird an die Originaladressen gemappt,
Importe sind Stubs, die laut abbrechen (außer gezielt nachgebildeten).
"""
import os
import struct
from unicorn import (Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_PROT_ALL,
                     UC_PROT_READ, UC_PROT_WRITE)
from unicorn import x86_const as X

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
EXE = os.path.join(REPO, "game", "pc", "Speed.exe")

STACK_TOP = 0x00200000          # Stack: 0x1F0000..0x200000 (unterhalb des Image bei 0x400000)
STACK_SIZE = 0x10000
TEB_BASE = 0x00100000
RET_ADDR = 0x7FFE0000           # Rücksprungziel von call()
IMPORT_STUB_BASE = 0x7FFD0000
HEAP_BASE = 0x20000000
HEAP_SIZE = 0x04000000
SCRATCH_BASE = 0x30000000       # Pufferbereich für Eingaben/Ausgaben (alloc)
SCRATCH_SIZE = 0x04000000
PAGE = 0x1000


class ImportCalled(Exception):
    """Die Funktion hat einen nicht nachgebildeten Import aufgerufen."""


class EmuFault(Exception):
    pass


def _align(v, a=PAGE):
    return (v + a - 1) & ~(a - 1)


class Emu:
    def __init__(self, exe=EXE):
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.imports = {}      # stub-Adresse -> (dll, name)
        self.stubs = {}        # name -> python callable(emu) -> eax
        self.sections = []     # (name, va, vsize, characteristics)
        self.image_base = 0
        self._scratch_top = SCRATCH_BASE
        self._heap_top = HEAP_BASE
        self._load_pe(exe)
        self._setup_cpu()
        self.uc.hook_add(UC_HOOK_CODE, self._on_code, begin=IMPORT_STUB_BASE,
                         end=IMPORT_STUB_BASE + 0x10000)

    # ---------------- Laden ----------------
    def _load_pe(self, path):
        with open(path, "rb") as f:
            d = f.read()
        pe = struct.unpack_from("<I", d, 0x3C)[0]
        assert d[pe:pe + 4] == b"PE\0\0"
        nsec, = struct.unpack_from("<H", d, pe + 6)
        optsz, = struct.unpack_from("<H", d, pe + 20)
        opt = pe + 24
        self.image_base, = struct.unpack_from("<I", d, opt + 28)
        self.entry_rva, = struct.unpack_from("<I", d, opt + 16)
        size_of_image, = struct.unpack_from("<I", d, opt + 56)
        imp_rva, imp_size = struct.unpack_from("<II", d, opt + 96 + 8)
        self.uc.mem_map(self.image_base, _align(size_of_image), UC_PROT_ALL)
        # Header
        hdr_size, = struct.unpack_from("<I", d, opt + 60)
        self.uc.mem_write(self.image_base, d[:hdr_size])
        sec = opt + optsz
        for i in range(nsec):
            name, vsize, va, rawsize, rawptr = struct.unpack_from("<8sIIII", d, sec + i * 40)
            chars, = struct.unpack_from("<I", d, sec + i * 40 + 36)
            name = name.rstrip(b"\0").decode()
            self.sections.append((name, self.image_base + va, vsize, chars))
            raw = d[rawptr:rawptr + min(rawsize, vsize)]
            self.uc.mem_write(self.image_base + va, raw)
        self._bind_imports(imp_rva)

    def _bind_imports(self, imp_rva):
        rd = lambda a, n: bytes(self.uc.mem_read(self.image_base + a, n))
        self.uc.mem_map(IMPORT_STUB_BASE, 0x10000, UC_PROT_ALL)
        self.uc.mem_write(IMPORT_STUB_BASE, b"\xCC" * 0x10000)
        idx = 0
        p = imp_rva
        while True:
            oft, _, _, name_rva, ft = struct.unpack("<IIIII", rd(p, 20))
            if name_rva == 0:
                break
            dll = rd(name_rva, 64).split(b"\0")[0].decode()
            t = oft or ft
            slot = ft
            while True:
                ent, = struct.unpack("<I", rd(t, 4))
                if ent == 0:
                    break
                if ent & 0x80000000:
                    fname = f"#{ent & 0xFFFF}"
                else:
                    fname = rd((ent & 0x7FFFFFFF) + 2, 80).split(b"\0")[0].decode()
                stub = IMPORT_STUB_BASE + idx * 16
                self.imports[stub] = (dll, fname)
                self.uc.mem_write(self.image_base + slot, struct.pack("<I", stub))
                idx += 1
                t += 4
                slot += 4
            p += 20

    def _setup_cpu(self):
        uc = self.uc
        uc.mem_map(STACK_TOP - STACK_SIZE, STACK_SIZE, UC_PROT_READ | UC_PROT_WRITE)
        uc.mem_map(TEB_BASE, PAGE, UC_PROT_READ | UC_PROT_WRITE)
        # TIB: [0]=SEH-Kette (-1), [4]=Stack-Basis, [8]=Stack-Limit, [0x18]=Selbstzeiger
        uc.mem_write(TEB_BASE, struct.pack("<IIII", 0xFFFFFFFF, STACK_TOP, STACK_TOP - STACK_SIZE, 0))
        uc.mem_write(TEB_BASE + 0x18, struct.pack("<I", TEB_BASE))
        uc.mem_map(RET_ADDR, PAGE, UC_PROT_ALL)
        # RET_ADDR: hlt (Rücksprungziel). +16: fninit. +32: fst qword [FST_SLOT] (ST0 sichern).
        uc.mem_write(RET_ADDR, b"\xF4")
        uc.mem_write(RET_ADDR + 16, b"\xDB\xE3")
        uc.mem_write(RET_ADDR + 32, b"\xDD\x15" + struct.pack("<I", TEB_BASE + 0x800))
        uc.mem_map(HEAP_BASE, HEAP_SIZE, UC_PROT_READ | UC_PROT_WRITE)
        uc.mem_map(SCRATCH_BASE, SCRATCH_SIZE, UC_PROT_READ | UC_PROT_WRITE)
        self._setup_fs()
        # SSE/FPU einschalten: CR0.EM=0, CR0.MP=1, CR4.OSFXSR/OSXMMEXCPT
        uc.reg_write(X.UC_X86_REG_CR0, (uc.reg_read(X.UC_X86_REG_CR0) & ~0x4) | 0x2)
        uc.reg_write(X.UC_X86_REG_CR4, uc.reg_read(X.UC_X86_REG_CR4) | 0x600)
        self._fninit()

    def _setup_fs(self):
        """FS über einen GDT-Eintrag (Basis = TEB) setzen; Unicorn 2.x rät von FS_BASE ab."""
        uc = self.uc
        gdt = 0x00110000
        uc.mem_map(gdt, PAGE, UC_PROT_READ | UC_PROT_WRITE)
        def desc(base, limit, access, flags):
            return struct.pack("<HHBBBB", limit & 0xFFFF, base & 0xFFFF, (base >> 16) & 0xFF, access,
                               ((limit >> 16) & 0xF) | (flags << 4), (base >> 24) & 0xFF)
        entries = [b"\0" * 8,
                   desc(0, 0xFFFFF, 0x9A, 0xC),       # 1: Code 32-Bit flach
                   desc(0, 0xFFFFF, 0x92, 0xC),       # 2: Daten flach
                   desc(TEB_BASE, 0xFFF, 0x92, 0x4)]  # 3: FS -> TEB
        uc.mem_write(gdt, b"".join(entries))
        uc.reg_write(X.UC_X86_REG_GDTR, (0, gdt, len(entries) * 8 - 1, 0))
        uc.reg_write(X.UC_X86_REG_CS, 1 << 3)
        for r in (X.UC_X86_REG_DS, X.UC_X86_REG_ES, X.UC_X86_REG_SS):
            uc.reg_write(r, 2 << 3)
        uc.reg_write(X.UC_X86_REG_FS, 3 << 3)

    def _fninit(self):
        self.uc.emu_start(RET_ADDR + 16, RET_ADDR + 18)
        self.uc.reg_write(X.UC_X86_REG_FPCW, 0x27F)  # 53-Bit-Genauigkeit wie Windows-Prozess

    def _on_code(self, uc, address, size, user):
        if address not in self.imports:
            return
        dll, name = self.imports[address]
        fn = self.stubs.get(name)
        if fn is None:
            raise ImportCalled(f"Import {dll}!{name} aufgerufen (nicht nachgebildet)")
        eax = fn(self) & 0xFFFFFFFF
        esp = uc.reg_read(X.UC_X86_REG_ESP)
        ret, = struct.unpack("<I", uc.mem_read(esp, 4))
        # stdcall-Bereinigung: Stub meldet pop-Bytes über fn.pop (Standard 0)
        pop = getattr(fn, "pop", 0)
        uc.reg_write(X.UC_X86_REG_EAX, eax)
        uc.reg_write(X.UC_X86_REG_ESP, esp + 4 + pop)
        uc.reg_write(X.UC_X86_REG_EIP, ret)

    # ---------------- Speicher ----------------
    def read(self, addr, n):
        return bytes(self.uc.mem_read(addr, n))

    def write(self, addr, data):
        self.uc.mem_write(addr, bytes(data))

    def u32(self, addr):
        return struct.unpack("<I", self.read(addr, 4))[0]

    def f32(self, addr):
        return struct.unpack("<f", self.read(addr, 4))[0]

    def alloc(self, size_or_data, align=16):
        """Puffer im Scratch-Bereich anlegen (Größe oder Initialdaten). Gibt die Adresse zurück."""
        data = size_or_data if isinstance(size_or_data, (bytes, bytearray)) else bytes(size_or_data)
        addr = (self._scratch_top + align - 1) & ~(align - 1)
        self._scratch_top = addr + len(data)
        if self._scratch_top > SCRATCH_BASE + SCRATCH_SIZE:
            raise MemoryError("Scratch voll; Emu.reset_scratch() nutzen")
        if data:
            self.uc.mem_write(addr, data)
        return addr

    def arg(self, n):
        """n-tes Stack-Argument (1-basiert) innerhalb eines Import-Stubs."""
        return self.u32(self.uc.reg_read(X.UC_X86_REG_ESP) + 4 * n)

    def reset_scratch(self):
        self.uc.mem_write(SCRATCH_BASE, b"\0" * (self._scratch_top - SCRATCH_BASE))
        self._scratch_top = SCRATCH_BASE

    def heap_alloc(self, size, zero=False):
        addr = (self._heap_top + 15) & ~15
        self._heap_top = addr + size
        if zero:
            self.uc.mem_write(addr, b"\0" * size)
        return addr

    def snapshot(self):
        """Veränderliche Bereiche: Schreibabschnitte des Images + benutzter Scratch/Heap."""
        snap = {}
        for name, va, vsize, ch in self.sections:
            if ch & 0x80000000:  # IMAGE_SCN_MEM_WRITE
                snap[va] = self.read(va, vsize)
        if self._heap_top > HEAP_BASE:
            snap[HEAP_BASE] = self.read(HEAP_BASE, self._heap_top - HEAP_BASE)
        return snap

    @staticmethod
    def diff(before, after, gap=8):
        """Liste (addr, alt_bytes, neu_bytes) der geänderten Bereiche (Lücken < gap werden verbunden)."""
        out = []
        for base, a in before.items():
            b = after[base]
            if a == b:
                continue
            idx = [i for i in range(len(a)) if a[i] != b[i]]
            s = e = idx[0]
            for i in idx[1:] + [None]:
                if i is not None and i - e <= gap:
                    e = i
                    continue
                out.append((base + s, a[s:e + 1], b[s:e + 1]))
                if i is not None:
                    s = e = i
        return out

    # ---------------- Aufruf ----------------
    def call(self, addr, conv="cdecl", args=(), this=None, regs=None, max_insns=50_000_000, track=False):
        """Funktion ausführen. conv: cdecl | stdcall | fastcall | thiscall | usercall.

        args: Ganzzahlen (32 Bit). Fließkomma-Argumente als Bitmuster übergeben (f2i).
        regs: zusätzliche Register, z. B. {"eax": 5} (usercall, Ghidra-`in_EAX`).
        track=True: Speicher vor/nach vergleichen (langsam, ~30 ms); .diff = geänderte Bereiche
        (addr, alt, neu), sonst leer.
        Rückgabe: Result mit .eax .edx .st0 (float oder None) und .diff.
        """
        uc = self.uc
        for r in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"):
            uc.reg_write(getattr(X, f"UC_X86_REG_{r.upper()}"), 0)
        args = [a & 0xFFFFFFFF for a in args]
        reg_args = []
        if conv == "thiscall":
            uc.reg_write(X.UC_X86_REG_ECX, (this or 0) & 0xFFFFFFFF)
        elif conv == "fastcall":
            reg_args, args = args[:2], args[2:]
            for r, v in zip((X.UC_X86_REG_ECX, X.UC_X86_REG_EDX), reg_args):
                uc.reg_write(r, v)
        for k, v in (regs or {}).items():
            uc.reg_write(getattr(X, f"UC_X86_REG_{k.upper()}"), v & 0xFFFFFFFF)
        sp = STACK_TOP - 0x100
        sp -= 4 * len(args)
        uc.mem_write(sp, struct.pack(f"<{len(args)}I", *args))
        sp -= 4
        uc.mem_write(sp, struct.pack("<I", RET_ADDR))
        uc.reg_write(X.UC_X86_REG_ESP, sp)
        uc.reg_write(X.UC_X86_REG_EFLAGS, 0x202)
        self._fninit()
        before = self.snapshot() if track else None
        try:
            uc.emu_start(addr, RET_ADDR, count=max_insns)
        except UcError as e:
            raise EmuFault(f"{e} bei EIP=0x{uc.reg_read(X.UC_X86_REG_EIP):08x}") from e
        if uc.reg_read(X.UC_X86_REG_EIP) != RET_ADDR:
            raise EmuFault(f"Abbruch vor Rückkehr (EIP=0x{uc.reg_read(X.UC_X86_REG_EIP):08x}, "
                           f"Instruktionslimit {max_insns}?)")
        res = Result()
        for r in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"):
            setattr(res, r, uc.reg_read(getattr(X, f"UC_X86_REG_{r.upper()}")))
        res.st0 = self._st0()
        res.diff = self.diff(before, self.snapshot()) if track else []
        return res

    def _st0(self):
        """ST(0) als Python-float, wenn der x87-Stack nicht leer ist (TOP != 0 nach fninit)."""
        top = (self.uc.reg_read(X.UC_X86_REG_FPSW) >> 11) & 7
        if top == 0:
            return None
        self.uc.emu_start(RET_ADDR + 32, RET_ADDR + 38)
        return struct.unpack("<d", self.read(TEB_BASE + 0x800, 8))[0]


class Result:
    def __repr__(self):
        return f"Result(eax=0x{self.eax:08x} edx=0x{self.edx:08x} st0={self.st0} diff={len(self.diff)} Bereiche)"


def f2i(x):
    return struct.unpack("<I", struct.pack("<f", x))[0]


def i2f(x):
    return struct.unpack("<f", struct.pack("<I", x & 0xFFFFFFFF))[0]


def stub(name, pop=0):
    """Decorator-Helfer: Emu.stubs[name] = fn; fn.pop = stdcall-Bytes."""
    def deco(fn):
        fn.pop = pop
        return fn
    return deco
