"""Run the shipped CMagic/CMagicEffect lifetime paths after target removal."""
from pathlib import Path
import json
import re
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(R/'runtime/pylibs'))
from patch_mage_ground import machine, DLL, GROUND_SPEED
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *


def run(base, retained, start, duration):
    u = machine(DLL.read_bytes(), base)
    magic, data, effect = 0x30000100, 0x30000400, 0x30000800
    head, node, parthead, partnode, part = [0x30001000+i*0x100 for i in range(5)]
    stack, stop, particles = 0x40008000, 0x3000f000, 0x30002000
    def word(at, value): u.mem_write(at, struct.pack('<I', value))
    def vector(at, value): u.mem_write(at, struct.pack('<3f', *value))
    word(magic+0xb8, 1)  # SendEndAttack, with both creature pointers removed.
    word(magic+0xec, data); word(data+0x34, int(retained))
    word(magic+0xe4, head); word(magic+0xe8, 1)
    word(head, node); word(node, head); word(node+4, head); word(node+8, effect)
    word(magic+0x24, particles)
    vector(magic+0xc0, (0,0,0))  # Last target position survives deletion.
    word(effect+0x14, parthead); word(effect+0x18, 1); word(effect+0x1c, partnode)
    word(parthead, partnode); word(partnode, parthead); word(partnode+8, part)
    word(effect+0xd4, 0x30003000)
    vector(effect+0x48, (0,-5,0)); vector(effect+0x30, (0,0,0))
    word(effect+0xc0, start); word(effect+0xc4, GROUND_SPEED)
    vector(part+0x0c, (0,0,.07))
    word(part+0x44, start); word(part+0x48, duration)
    word(part+0x54, GROUND_SPEED); word(part+0x6c, 1)
    expired = []
    def stop_before_allocator(u, address, size, _):
        if address == base+0x19335:
            expired.append(True); u.emu_stop()
    u.hook_add(UC_HOOK_CODE, stop_before_allocator)
    elapsed = 0
    while elapsed < start+duration+100:
        u.mem_write(stack, struct.pack('<II', stop, 33))
        u.reg_write(UC_X86_REG_ESP, stack); u.reg_write(UC_X86_REG_ECX, magic)
        u.emu_start(base+0x19110, stop, count=50000)
        elapsed += 33
        if expired:
            assert retained and start+duration <= elapsed <= start+duration+33
            return dict(retained=True, ended_ms=elapsed, start=start, duration=duration)
        assert u.reg_read(UC_X86_REG_EIP) == stop
        assert u.reg_read(UC_X86_REG_ESP) == stack+8
        value = u.reg_read(UC_X86_REG_EAX)
        if not retained:
            assert value == 0 and elapsed == 33
            return dict(retained=False, ended_ms=elapsed)
        assert value == 1
        assert bytes(u.mem_read(magic+0xc0, 12)) == bytes(12)
    raise AssertionError('native effect did not expire at its authored lifetime')


def main():
    results = {}
    meta = json.loads((R/'assets/skills140/effect-timing.json').read_text())
    for kind in ('meteor', 'frost'):
        raw = (R/f'client-overlay/Magic/mt_{kind}B.ms').read_text(encoding='cp949')
        assert raw.splitlines()[1] == '[ATTACKMAGIC]'
        if kind == 'meteor':
            start, duration = meta['meteor']['finalImpact'], meta['meteor']['magmaDuration']
        else:
            block = next(b for b in re.findall(r'\[EFFECT\](.*?)\[/EFFECT\]', raw, re.S) if 'mf_frozen_ground5.wed' in b)
            start = int(re.search(r'\[STARTTIME\] (\d+)',block)[1])
            duration = int(re.search(r'\[TIMELIMIT\] (\d+)',block)[1])
        results[kind] = []
        for base in (0x10000000, 0x16000000):
            old = run(base, False, start, duration)
            fixed = run(base, True, start, duration)
            results[kind].append(dict(base=hex(base), previous=old, fixed=fixed))
    report = dict(passed=True, native_functions=['CMagic::Update', 'CMagicEffect::Update', 'CMagicEffect::NextStep'],
                  target_removed_before_first_tick=True, tests=results,
                  limit='Terminal-layer lifecycle exercised; no renderer or live in-game visual test.')
    (R/'assets/skills140/lifetime-validation.json').write_text(json.dumps(report, indent=2)+'\n')
    print('PASS: native target-death lifetime regression and both terminal layers at two DLL bases')


if __name__ == '__main__': main()
