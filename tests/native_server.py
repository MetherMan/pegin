"""Minimal i386 ELF harness for offline server regression tests (Unicorn)."""
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EIP, UC_X86_REG_ESP


class Server:
    def __init__(self, path):
        self.raw = path.read_bytes()
        raw = self.raw
        assert raw[:7] == b'\x7fELF\x01\x01\x01'
        hdr = struct.unpack_from('<16sHHIIIIIHHHHHH', raw)
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        pages = set()
        segments = []
        for i in range(hdr[10]):
            seg = struct.unpack_from('<8I', raw, hdr[5] + i * hdr[9])
            kind, offset, address, _, size, memsize, _, _ = seg
            if kind != 1:
                continue
            pages.update(range(address & ~4095, (address + memsize + 4095) & ~4095, 4096))
            segments.append((address, raw[offset:offset + size]))
        ordered = sorted(pages)
        start = previous = ordered[0]
        for page in ordered[1:]:
            if page != previous + 4096:
                self.uc.mem_map(start, previous + 4096 - start)
                start = page
            previous = page
        self.uc.mem_map(start, previous + 4096 - start)
        for address, content in segments:
            self.uc.mem_write(address, content)
        sections = [struct.unpack_from('<10I', raw, hdr[6] + i * hdr[11]) for i in range(hdr[12])]
        self.symbols = {}
        for section in sections:
            if section[1] != 2:
                continue
            strings = sections[section[6]]
            names = raw[strings[4]:strings[4] + strings[5]]
            for offset in range(section[4], section[4] + section[5], section[9]):
                name, address, size, _, _, index = struct.unpack_from('<IIIBBH', raw, offset)
                if name and index:
                    label = names[name:names.index(b'\0', name)].decode()
                    self.symbols[label] = (address, size)
        self.stack = 0x70000000
        self.player = 0x60000000
        self.stop = 0x71000000
        self.uc.mem_map(self.stack, 0x100000)
        self.uc.mem_map(self.player, 0x100000)
        self.uc.mem_map(self.stop, 4096)

    def name(self, prefix):
        if prefix in self.symbols:
            return prefix
        matches = [s for s in self.symbols if s == prefix or s.startswith(prefix)]
        assert len(matches) == 1, (prefix, matches)
        return matches[0]

    def address(self, prefix):
        return self.symbols[self.name(prefix)][0]

    def stub(self, prefix):
        self.uc.mem_write(self.address(prefix), b'\x31\xc0\xc3')

    def call(self, prefix, *args):
        sp = self.stack + 0x80000
        self.uc.mem_write(sp, struct.pack('<' + 'I' * (len(args) + 1), self.stop, *args))
        self.uc.reg_write(UC_X86_REG_ESP, sp)
        self.uc.reg_write(UC_X86_REG_EAX, 0)
        self.uc.emu_start(self.address(prefix), self.stop, count=1000000)
        assert self.uc.reg_read(UC_X86_REG_EIP) == self.stop
        return self.uc.reg_read(UC_X86_REG_EAX)

