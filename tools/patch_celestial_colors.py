"""Engine.dll (.cgold): gold #FFD966 name and rank colour for the 천상의 날개 weapons 19260..19340.

Layered on .bmarm (Twilight / Blue Moon armour #72CFFF) and .shild. The three colour sites of the
Twilight cave (rank colour selection and the two name-draw loads) each start with a jump into the
.bmarm range test; the new hook checks the Celestial range first and otherwise continues to it.
The rank path writes the colour through the same call/pop position-independent address as the
original cave. Emulated at two load addresses for gold, blue and default items.
"""
from pathlib import Path
import json, struct, sys
ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT/'runtime/pylibs'), str(ROOT/'.cache/weapon-build-deps'), str(ROOT/'tools')]
import pefile
from pe_hooks import Hooks, align
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import *

OUT = ROOT/'assets/golden-knight'
LO, HI = 19260, 19340
GOLD, BLUE = 0xffffd966, 0xff72cfff
BLUE_RANGES = [(19030, 19110), (19140, 19250)]
COLOUR_VAR = 0x10045e60


def patch():
    h = Hooks(ROOT/'client-overlay/Engine.dll', OUT/'engine-color-hooks.json', b'.cgold')
    for va, kind in [(0x1003cc00, 'rank'), (0x1003cc47, 'eax'), (0x1003cc82, 'ecx')]:
        off = h.pe.get_offset_from_rva(va-h.base)
        old = bytes(h.data[off:off+14])
        assert old[0] == 0xe9 and set(old[5:]) == {0x90}, (hex(va), old.hex())
        previous = va+5+struct.unpack_from('<i', old, 1)[0]
        test = f'cmp word ptr [edi],{LO}; jb {hex(previous)}; cmp word ptr [edi],{HI}; ja {hex(previous)}; '
        if kind == 'rank':
            target = h.base+h.rva+len(h.code)+(-len(h.code) % 16)
            prefix = h.asm(test+'call next; next: pop eax', target)
            here = target+len(prefix)-1  # address of the pop, i.e. the call's return address
            body = (f'call next; next: pop eax; mov dword ptr [eax+{hex(COLOUR_VAR-here)}],{hex(GOLD)}; '
                    'mov eax,8; jmp 0x10023d28')
        elif kind == 'eax':
            body = f'mov eax,{hex(GOLD)}; jmp 0x1003cc6b'
        else:
            body = f'mov ecx,{hex(GOLD)}; jmp 0x1003cca6'
        h.hook(va, old.hex(), test+body, f'천상의 날개 {LO}..{HI} gold at {hex(va)} ({kind})')
    return h.finish()


def test(data):
    cases = 0
    for base in [0x10000000, 0x03500000]:
        image = pefile.PE(data=data)
        image.relocate_image(base)
        mapped = image.get_memory_mapped_image()
        for rank in [1, 6, 9, 10]:
            for item in [11424, 19029, 19030, 19110, 19131, 19140, 19250, 19259, 19260, 19280, 19300, 19340, 19341, 20000]:
                default = [0xffc2c2c2, 0xfffcff00, 0xff30ff00, 0xff00ffea, 0xfffe7e7e][rank-1] if rank <= 5 else 0xffff0000
                want = GOLD if LO <= item <= HI else BLUE if any(a <= item <= b for a, b in BLUE_RANGES) else default
                u = Uc(UC_ARCH_X86, UC_MODE_32)
                u.mem_map(base, align(len(mapped), 0x1000))
                u.mem_write(base, mapped)
                u.mem_map(0x2000000, 0x2000)
                u.reg_write(UC_X86_REG_EDI, 0x2000000)
                u.reg_write(UC_X86_REG_ESP, 0x2001f00)
                u.mem_write(0x2000000, struct.pack('<H', item))
                u.mem_write(0x2000084, struct.pack('<H', rank))
                u.emu_start(base+0x23ccc, base+0x23d28, count=300)
                assert struct.unpack('<I', u.mem_read(base+0x45e60, 4))[0] == want, (hex(base), rank, item)
                for start, end, reg in [(0x23e5e, 0x23e63, UC_X86_REG_EAX), (0x23f54, 0x23f5a, UC_X86_REG_ECX)]:
                    stack, flags = u.reg_read(UC_X86_REG_ESP), u.reg_read(UC_X86_REG_EFLAGS)
                    u.emu_start(base+start, base+end, count=300)
                    assert u.reg_read(reg) == want, (hex(base), rank, item, hex(start))
                    assert u.reg_read(UC_X86_REG_ESP) == stack and u.reg_read(UC_X86_REG_EFLAGS) == flags
                cases += 1
    return cases


def main():
    data, report = patch()
    report['validation'] = dict(cases=test(data), gold=[LO, HI], blue=BLUE_RANGES, colour='#FFD966',
                                relocated_bases=[0x10000000, 0x03500000])
    (ROOT/'client-overlay/Engine.dll').write_bytes(data)
    (OUT/'engine-color-hooks.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    p = ROOT/'src/client/engine/Src/W3DItemInfoMgr.cpp'
    s = p.read_bytes().decode('cp949')
    if 'bCelestialItem' not in s:
        old = 'const bool bTwilightItem ='
        assert old in s
        s = s.replace(old, f'const bool bCelestialItem = pItemInfoDat->wItemNum >= {LO} && pItemInfoDat->wItemNum <= {HI}; // gold #FFD966\n\t\t'+old, 1)
        p.write_bytes(s.encode('cp949'))
    print(json.dumps(report['validation']))


if __name__ == '__main__':
    main()
