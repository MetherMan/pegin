"""Engine.dll (.bmarm): Rank-9 #72CFFF name/rank colour for the Blue Moon armour 19140..19250.

assets/twilight/patch_tooltip.py gave the Twilight family 19030..19110 its colour with three cave
snippets that start with the same range test. Each test is hooked to accept a second range, so the
items in between (skill books 19120.., horse certificates 19130/19131, potion 19132) keep their
normal colours. Emulates the rank-colour selection and both name-draw loads at two load addresses.
"""
from pathlib import Path
import json, struct, sys
ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT/'runtime/pylibs'), str(ROOT/'.cache/weapon-build-deps'), str(ROOT/'tools')]
import pefile
from pe_hooks import Hooks, align
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import *

OUT = ROOT/'assets/blue-moon-armor'
RANGES = [(19030, 19110), (19140, 19250)]
SNIPPETS = [0x1003cc00, 0x1003cc47, 0x1003cc82]
TEST = b'\x66\x81\x3f'+struct.pack('<H', 19030)+b'\x72\x07\x66\x81\x3f'+struct.pack('<H', 19110)+b'\x76'
BLUE = 0xff72cfff


def patch():
    h = Hooks(ROOT/'client-overlay/Engine.dll', OUT/'engine-color-hooks.json', b'.bmarm')
    for va in SNIPPETS:
        off = h.pe.get_offset_from_rva(va-h.base)
        old = bytes(h.data[off:off+14])
        assert old[:13] == TEST, (hex(va), old.hex())
        yes, no = va+14+old[13], va+14
        (a, b), (c, d) = RANGES
        h.hook(va, old.hex(), f'cmp word ptr [edi],{a}; jb {hex(no)}; cmp word ptr [edi],{b}; jbe {hex(yes)}; '
                              f'cmp word ptr [edi],{c}; jb {hex(no)}; cmp word ptr [edi],{d}; jbe {hex(yes)}; jmp {hex(no)}',
               f'Twilight colour range test at {hex(va)}: also {c}..{d}')
    return h.finish()


def test(data):
    cases = 0
    for base in [0x10000000, 0x03500000]:
        image = pefile.PE(data=data)
        image.relocate_image(base)
        mapped = image.get_memory_mapped_image()
        size = align(len(mapped), 0x1000)
        for rank in [1, 2, 5, 6, 9]:
            for item in [11424, 19000, 19029, 19030, 19110, 19111, 19120, 19131, 19132, 19139, 19140, 19160, 19200,
                         19250, 19251, 20000]:
                default = [0xffc2c2c2, 0xfffcff00, 0xff30ff00, 0xff00ffea, 0xfffe7e7e][rank-1] if rank <= 5 else 0xffff0000
                want = BLUE if any(a <= item <= b for a, b in RANGES) else default
                u = Uc(UC_ARCH_X86, UC_MODE_32)
                u.mem_map(base, size)
                u.mem_write(base, mapped)
                u.mem_map(0x2000000, 0x2000)
                u.reg_write(UC_X86_REG_EDI, 0x2000000)
                u.reg_write(UC_X86_REG_ESP, 0x2001f00)
                u.mem_write(0x2000000, struct.pack('<H', item))
                u.mem_write(0x2000084, struct.pack('<H', rank))
                u.emu_start(base+0x23ccc, base+0x23d28, count=200)
                assert struct.unpack('<I', u.mem_read(base+0x45e60, 4))[0] == want, (hex(base), rank, item)
                for start, end, reg in [(0x23e5e, 0x23e63, UC_X86_REG_EAX), (0x23f54, 0x23f5a, UC_X86_REG_ECX)]:
                    stack, flags = u.reg_read(UC_X86_REG_ESP), u.reg_read(UC_X86_REG_EFLAGS)
                    u.emu_start(base+start, base+end, count=200)
                    assert u.reg_read(reg) == want, (hex(base), rank, item, hex(start))
                    assert u.reg_read(UC_X86_REG_ESP) == stack and u.reg_read(UC_X86_REG_EFLAGS) == flags
                cases += 1
    return cases


def main():
    data, report = patch()
    report['validation'] = dict(cases=test(data), ranges=RANGES, colour='#72CFFF', relocated_bases=[0x10000000, 0x03500000])
    (ROOT/'client-overlay/Engine.dll').write_bytes(data)
    (OUT/'engine-color-hooks.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    for p in [ROOT/'src/client/engine/Src/W3DItemInfoMgr.cpp']:
        s = p.read_bytes().decode('cp949')
        old = 'pItemInfoDat->wItemNum >= 19030 && pItemInfoDat->wItemNum <= 19110;'
        new = ('( pItemInfoDat->wItemNum >= 19030 && pItemInfoDat->wItemNum <= 19110 ) ||'
               ' ( pItemInfoDat->wItemNum >= 19140 && pItemInfoDat->wItemNum <= 19250 );')
        if old in s:
            p.write_bytes(s.replace(old, new).encode('cp949'))
    print(json.dumps(report['validation']))


if __name__ == '__main__':
    main()
