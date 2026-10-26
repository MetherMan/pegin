"""DeicideOnline.exe (.cwfx): gold attack effects for the 천상의 날개 sword and bow.

1. Login trail (0x40ab29): character creation builds the sword trail with the hard-coded white
   '검흔'; assets/twilight/patch_sword_trail.py already keeps 'twilight_blue' when the equipped
   Trace.txt entry names it. This layer does the same for 'celestial_gold' and otherwise continues
   into that existing cave. Re-equipping already selects the stored trail natively.
2. Arrow (SetArrowInfo, 0x40fa59 / 0x40fad8): the Twilight .moon hook picks 'mt_arrow' for bows
   19060..19080; this layer picks the gold 'cw_arrow' for 19290..19310 first and otherwise
   continues into .moon (other bows keep '단궁기본').
Fixed-base EXE without relocations, so the new strings are addressed absolutely. Both paths are
emulated on the full original instructions.
"""
from pathlib import Path
import json, struct, sys
ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT/'runtime/pylibs'), str(ROOT/'.cache/weapon-build-deps'), str(ROOT/'tools')]
import pefile
from pe_hooks import Hooks
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import *

OUT = ROOT/'assets/golden-knight'
TRAIL = b'celestial_gold'
BOW = (19290, 19310)
WHITE = '검흔'.encode('cp949')
SHORTBOW = '단궁기본'.encode('cp949')


def current_jump(h, va):
    off = h.pe.get_offset_from_rva(va-h.base)
    raw = bytes(h.data[off:off+5])
    assert raw[0] == 0xe9, (hex(va), raw.hex())
    return raw.hex(), va+5+struct.unpack_from('<i', raw, 1)[0]


def patch():
    h = Hooks(ROOT/'client-overlay/DeicideOnline.exe', OUT/'exe-effect-hooks.json', b'.cwfx')
    assert h.base == 0x400000 and not hasattr(h.pe, 'DIRECTORY_ENTRY_BASERELOC')
    strings = h.base+h.rva+len(h.code)
    h.code.extend(TRAIL+b'\0')
    arrow = h.base+h.rva+len(h.code)
    h.code.extend(b'cw_arrow\0')
    old, previous = current_jump(h, 0x40ab29)
    words = [struct.unpack_from('<I', TRAIL, k)[0] for k in (0, 4, 8)]
    h.hook(0x40ab29, old,
           'pushfd; mov ecx,dword ptr [ebp+0x30c]; test ecx,ecx; jz other; mov ecx,dword ptr [ecx+0x18]; test ecx,ecx; jz other; '
           f'cmp dword ptr [ecx],{hex(words[0])}; jne other; cmp dword ptr [ecx+4],{hex(words[1])}; jne other; '
           f'cmp dword ptr [ecx+8],{hex(words[2])}; jne other; cmp word ptr [ecx+12],0x646c; jne other; '
           f'cmp byte ptr [ecx+14],0; jne other; popfd; push ecx; jmp 0x40ab2e; other: popfd; jmp {hex(previous)}',
           'login sword trail keeps celestial_gold from the equipped Trace.txt entry')
    for va, reg, offset in [(0x40fa59, 'edi', 20), (0x40fad8, 'esi', 24)]:
        old, previous = current_jump(h, va)
        h.hook(va, old,
               f'pushfd; cmp dword ptr [esp+{offset}],{BOW[0]}; jb other; cmp dword ptr [esp+{offset}],{BOW[1]}; ja other; '
               f'popfd; mov {reg},{hex(arrow)}; jmp {hex(va+5)}; other: popfd; jmp {hex(previous)}',
               f'gold arrow cw_arrow for bows {BOW[0]}..{BOW[1]} at {hex(va)}')
    data, report = h.finish()
    report['strings'] = dict(trail=hex(strings), arrow=hex(arrow))
    return data, report


def machine(data):
    image = pefile.PE(data=data)
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    u.mem_map(0x400000, 0x100000)
    u.mem_write(0x400000, image.get_memory_mapped_image())
    u.mem_map(0x2000000, 0x10000)
    return u


def trail_case(data, name, has_record=True):
    """Same harness as assets/twilight/patch_sword_trail.py: run the login trail push."""
    u = machine(data)
    char, record, string, stack = 0x2000100, 0x2001000, 0x2002000, 0x200f000
    u.mem_write(char+0x30c, struct.pack('<I', record if has_record else 0))
    u.mem_write(record+0x18, struct.pack('<I', string if name is not None else 0))
    if name is not None:
        u.mem_write(string, name+b'\0')
    u.mem_write(stack, struct.pack('<I', 12345))
    for reg, value in [(UC_X86_REG_EBP, char), (UC_X86_REG_ESP, stack), (UC_X86_REG_EAX, 0x76543210),
                       (UC_X86_REG_EDX, 0x12345678), (UC_X86_REG_ECX, 12345), (UC_X86_REG_EFLAGS, 0x246)]:
        u.reg_write(reg, value)
    u.emu_start(0x40ab29, 0x40ab30, count=200)
    address = struct.unpack('<I', u.mem_read(stack-4, 4))[0]
    result = bytes(u.mem_read(address, 32)).split(b'\0')[0]
    assert u.reg_read(UC_X86_REG_ESP) == stack-4 and struct.unpack('<I', u.mem_read(stack, 4))[0] == 12345
    assert u.reg_read(UC_X86_REG_EAX) == u.reg_read(UC_X86_REG_ECX) == 0x76543210
    assert u.reg_read(UC_X86_REG_EDX) == 0x12345678 and u.reg_read(UC_X86_REG_EFLAGS) == 0x246
    return result


def arrow_case(data, item):
    """Same harness as assets/twilight-set/patch_client.py: the whole SetArrowInfo function."""
    u = machine(data)
    char, buf, sp, stop = 0x2000100, 0x2002000, 0x200f000, 0x400010
    u.mem_write(char+0x300, struct.pack('<3I', buf, 0, 31))
    u.mem_write(buf-1, b'\0')
    u.mem_write(sp, struct.pack('<2I', stop, item))
    regs = [(UC_X86_REG_EBX, 0x1111), (UC_X86_REG_EBP, 0x2222), (UC_X86_REG_ESI, 0x3333), (UC_X86_REG_EDI, 0x4444)]
    for reg, value in regs:
        u.reg_write(reg, value)
    u.reg_write(UC_X86_REG_ECX, char)
    u.reg_write(UC_X86_REG_ESP, sp)
    u.emu_start(0x40fa50, stop, count=300)
    text = bytes(u.mem_read(buf, 20)).split(b'\0')[0]
    assert u.reg_read(UC_X86_REG_ESP) == sp+8
    for reg, value in regs:
        assert u.reg_read(reg) == value
    return text


def main():
    data, report = patch()
    trail = []
    for name, has_record in [(None, False), (None, True), (b'other', True), (WHITE, True), (b'twilight_blue', True),
                             (b'celestial_gol', True), (b'celestial_gold_x', True), (TRAIL, True)]:
        want = name if has_record and name in (b'twilight_blue', TRAIL) else WHITE
        got = trail_case(data, name, has_record)
        assert got == want, (name, got, want)
        trail.append(name.decode('cp949') if name else None)
    arrows = 0
    for item in [0, 11739, 19059, 19060, 19080, 19081, 19289, *range(19290, 19311), 19311, 19320, 65535]:
        want = b'cw_arrow' if BOW[0] <= item <= BOW[1] else b'mt_arrow' if 19060 <= item <= 19080 else SHORTBOW
        got = arrow_case(data, item)
        assert got == want, (item, got, want)
        arrows += 1
    report['validation'] = dict(trail_cases=len(trail), arrow_cases=arrows, trail=TRAIL.decode(), arrow='cw_arrow', bows=list(BOW))
    (ROOT/'client-overlay/DeicideOnline.exe').write_bytes(data)
    (OUT/'exe-effect-hooks.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    source = ROOT/'src/client/gameclient/Src/Character.cpp'
    text = source.read_bytes().decode('cp949')
    old = 'm_sArrow = (nBowIndex >= 19060 && nBowIndex <= 19080) ? "mt_arrow" : "단궁기본";'
    new = ('m_sArrow = (nBowIndex >= 19290 && nBowIndex <= 19310) ? "cw_arrow" : '
           '(nBowIndex >= 19060 && nBowIndex <= 19080) ? "mt_arrow" : "단궁기본";')
    if old in text:
        source.write_bytes(text.replace(old, new).encode('cp949'))
    print(json.dumps(report['validation']))


if __name__ == '__main__':
    main()
