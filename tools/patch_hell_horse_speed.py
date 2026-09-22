"""Raise only ride type 4 to 100, validating the shipped GetRideSpeed code."""
from io import BytesIO
from pathlib import Path
import hashlib
import json
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'runtime/pylibs'))
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP

ORIGINAL = bytes.fromhex(
    '5589e583ec10c645ff288b45080fb7803e2303006685c0740e8b45080fb68040230300'
    '84c07506c645ff28eb448b45080fb7803e2303000fb7c083f804742d83f8047f2d83f8'
    '03741d83f8037f2383f801740783f8027408eb17c645ff46eb11c645ff4beb0bc645ff'
    '50eb05c645ff5f908b45080fb7803e2303006685c075118b45088b802c24030085c074'
    '048045ff0f0fb645ffc9c3'
)
UPDATED = ORIGINAL.replace(bytes.fromhex('c645ff5f'), bytes.fromhex('c645ff64'))


def verify(code, address, hell_speed):
    cases = []
    for kind in [0, 1, 2, 3, 4, 5, 255, 65535]:
        for mounted in [0, 1]:
            for walking_bonus in [0, 1]:
                u = Uc(UC_ARCH_X86, UC_MODE_32)
                base = address & ~4095
                u.mem_map(base, 8192)
                u.mem_write(address, code)
                player, stack, stop = 0x10000000, 0x20000000, 0x30000000
                u.mem_map(player, 0x33000)
                u.mem_map(stack, 0x10000)
                u.mem_map(stop, 4096)
                u.mem_write(player + 0x3233e, struct.pack('<HB', kind, mounted))
                u.mem_write(player + 0x3242c, struct.pack('<I', walking_bonus))
                sp = stack + 0x8000
                u.mem_write(sp, struct.pack('<II', stop, player))
                u.reg_write(UC_X86_REG_ESP, sp)
                u.emu_start(address, stop, count=100)
                expected = {1: 70, 2: 75, 3: 80, 4: hell_speed}.get(kind, 40) if mounted else 40
                if kind == 0 and walking_bonus:
                    expected += 15
                actual = u.reg_read(UC_X86_REG_EAX) & 255
                assert actual == expected, (kind, mounted, walking_bonus, actual, expected)
                assert u.reg_read(UC_X86_REG_ESP) == sp + 4
                cases.append(dict(kind=kind, mounted=mounted, walking_bonus=walking_bonus, speed=actual))
    return cases


def main():
    path = ROOT / 'server-bin/LAQIA_GameServer'
    before = path.read_bytes()
    elf = ELFFile(BytesIO(before))
    symbol = elf.get_section_by_name('.symtab').get_symbol_by_name('_Z12GetRideSpeedP15descriptor_data')[0]
    section = elf.get_section(symbol['st_shndx'])
    address = symbol['st_value']
    offset = section['sh_offset'] + address - section['sh_addr']
    code = before[offset:offset + symbol['st_size']]
    assert code in (ORIGINAL, UPDATED), 'Unexpected server build: inspect before changing it.'
    verify(code, address, 95 if code == ORIGINAL else 100)
    cases = verify(UPDATED, address, 100)
    after = before[:offset] + UPDATED + before[offset + len(UPDATED):]
    changed = [i for i, (a, b) in enumerate(zip(before, after)) if a != b]
    assert len(before) == len(after) and len(changed) in (0, 1)
    path.write_bytes(after)
    report = dict(speed=100, original_horse_speeds=[70, 75, 80],
                  symbol_address=hex(address), changed_byte_offsets=changed,
                  sha256=hashlib.sha256(after).hexdigest(), native_cases=cases,
                  actual_game_installation_tested=False)
    (ROOT / 'assets/hell-horse/speed100-validation.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8', newline='\n')
    print('PASS: 64 native ride-speed cases; hell horse 100; original horses and walking unchanged')


if __name__ == '__main__':
    main()
