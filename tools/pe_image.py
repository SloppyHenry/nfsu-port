"""Liest Speed.exe ohne Emulator: virtuelle Adressen über die PE-Abschnittstabelle auf Dateiversätze abbilden.
Wird beim Bauen genutzt, damit erzeugte Tabellen nicht im Repository liegen müssen."""
import hashlib, struct

# SHA-256 der unterstützten Speed.exe (NFSU PC, 2003)
KNOWN_SHA256 = {"1f72abe1e5c527c0a161e382d489605f483dd36c4ce7a2f4d561a0964c5db235"}


class PEImage:
    def __init__(self, path, check=True):
        self.data = open(path, "rb").read()
        if check and hashlib.sha256(self.data).hexdigest() not in KNOWN_SHA256:
            raise SystemExit(f"{path}: unbekannte Speed.exe (SHA-256 passt nicht)")
        pe = struct.unpack_from("<I", self.data, 0x3C)[0]
        nsec, = struct.unpack_from("<H", self.data, pe + 6)
        optsz, = struct.unpack_from("<H", self.data, pe + 20)
        self.base, = struct.unpack_from("<I", self.data, pe + 24 + 28)
        self.sections = []
        for i in range(nsec):
            o = pe + 24 + optsz + 40 * i
            vsize, va, rawsize, raw = struct.unpack_from("<IIII", self.data, o + 8)
            self.sections.append((self.base + va, max(vsize, rawsize), raw, rawsize))

    def read(self, addr, n):
        for va, size, raw, rawsize in self.sections:
            if va <= addr and addr + n <= va + size:
                off = addr - va
                chunk = self.data[raw + off: raw + min(off + n, rawsize)] if off < rawsize else b""
                return chunk + b"\0" * (n - len(chunk))  # .bss-Anteil ist null
        raise ValueError(f"Adresse 0x{addr:x} liegt in keinem Abschnitt")

    def u32(self, addr):
        return struct.unpack("<I", self.read(addr, 4))[0]
