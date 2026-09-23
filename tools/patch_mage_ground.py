"""Keep explicitly marked stationary spell layers level in the shipped engine.

SPEED is ignored by native [ENEMY] placement. A reserved value marks only our
stationary layers without changing movement types, sound styles, or height.
"""
from pathlib import Path
import json
import math
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
from pe_hooks import Hooks
from native_vfx import GROUND_SPEED
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import *

DLL = R/'client-overlay/Engine.dll'
REPORT = R/'assets/skills140/ground-engine-hook.json'


def machine(data, base):
    """Map actual Engine + actual Common math code, including PE relocations."""
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    engine = pefile.PE(data=data)
    common = pefile.PE(str(R/'client-overlay/Common.dll'))
    common_base = 0x22000000
    for p, address in ((engine, base), (common, common_base)):
        p.relocate_image(address)
        u.mem_map(address, (p.OPTIONAL_HEADER.SizeOfImage+4095)&~4095)
        u.mem_write(address, p.get_memory_mapped_image())
    exports = {e.name: common_base+e.address for e in common.DIRECTORY_ENTRY_EXPORT.symbols}
    for descriptor in engine.DIRECTORY_ENTRY_IMPORT:
        if descriptor.dll.lower() != b'common.dll':
            continue
        for entry in descriptor.imports:
            if entry.name in exports:
                u.mem_write(entry.address, struct.pack('<I', exports[entry.name]))
    # GetDir's static Matrix4 construction only registers an empty atexit callback.
    # Mark that registration done; all vector and matrix arithmetic runs natively.
    u.mem_write(base+0x45ba9, b'\x01')
    u.mem_write(base+0x45b44, b'\x0f')
    u.mem_write(common_base+0x2019a, b'\x01')
    u.mem_map(0x30000000, 0x10000)
    u.mem_map(0x40000000, 0x10000)
    return u


def native_ground(data, base, speed, origin, target):
    u = machine(data, base)
    obj, node, part = 0x30000100, 0x30000300, 0x30000400
    stack, stop = 0x40008000, 0x3000f000
    u.mem_write(obj+0x1c, struct.pack('<I', node))
    u.mem_write(node+8, struct.pack('<I', part))
    u.mem_write(part+0x54, struct.pack('<I', speed))
    u.mem_write(obj+0x30, struct.pack('<3f', *target))
    u.mem_write(obj+0x48, struct.pack('<3f', *origin))
    u.mem_write(stack, struct.pack('<II', stop, 0x30000800))
    saved = {UC_X86_REG_EBX:0x1357, UC_X86_REG_ESI:0x2468,
             UC_X86_REG_EDI:0x3579, UC_X86_REG_EBP:0x468a}
    for reg, val in saved.items(): u.reg_write(reg, val)
    u.reg_write(UC_X86_REG_ESP, stack)
    u.reg_write(UC_X86_REG_ECX, obj)
    u.emu_start(base+0x1af30, stop, count=20000)
    assert u.reg_read(UC_X86_REG_EIP) == stop
    assert u.reg_read(UC_X86_REG_ESP) == stack+8
    assert all(u.reg_read(reg) == val for reg,val in saved.items())
    assert bytes(u.mem_read(obj+0x24, 12)) == struct.pack('<3f', *target)
    quaternion = struct.unpack('<4f', u.mem_read(obj+0x54, 16))
    # GetDir's actual Matrix4 before conversion gives the local-Y (up) basis.
    matrix = struct.unpack('<16f', u.mem_read(base+0x45af0, 64))
    assert all(math.isfinite(v) for v in quaternion+matrix)
    return quaternion, matrix


def verify(original, patched):
    cases = []
    origins = [(0, -5, 0), (0, -2, 0), (4, -3, 2), (0, 0, 0)]
    targets = [(0, 0, .07), (1.4, .6, .55), (0, 0, 5), (0, 0, 0)]
    regression = None
    for base in (0x10000000, 0x16000000):
        for origin, target in zip(origins, targets):
            before = native_ground(original, base, GROUND_SPEED, origin, target)
            after = native_ground(patched, base, GROUND_SPEED, origin, target)
            # All explicitly level layers retain the requested world position.
            assert abs(after[1][4]) < 1e-5 and abs(after[1][5]) < 1e-5
            assert abs(after[1][6]-1) < 1e-5
            horizontal = native_ground(original, base, 30, origin, (*target[:2], origin[2]))
            assert all(abs(a-b) < 1e-5 for a,b in zip(after[0], horizontal[0]))
            for ordinary in (0, 30, 540, 600, 140030, 140032):
                assert native_ground(original, base, ordinary, origin, target) == native_ground(patched, base, ordinary, origin, target)
            cases.append(dict(base=hex(base), origin=origin, target=target, up=list(after[1][4:7])))
            if origin == (0,-5,0):
                # The 7.2-unit magma plane previously tilted under flat terrain.
                prior_floor_min = target[2]-7.2*abs(before[1][10])
                new_floor_min = target[2]-7.2*abs(after[1][10])
                assert prior_floor_min < 0 < new_floor_min
                regression = dict(previous_floor_min_z=prior_floor_min, fixed_floor_min_z=new_floor_min)
        # Flattening a purely vertical direction becomes a zero vector. Native
        # GetDir has its own (0,1,0) fallback; exercise that exact code as well.
        vertical = native_ground(patched, base, GROUND_SPEED, (0,0,0), (0,0,5))
        assert abs(vertical[1][6]-1) < 1e-5
    return dict(passed=True, native_function='CMagicEffect::UpdateEnemyPos',
                actual_common_math=True, relocated_bases=2, stationary_cases=cases,
                ordinary_comparisons=len(cases)*6, yaw_matches_horizontal_reference=True,
                zero_horizontal_distance_safe=True, ground_regression=regression,
                in_game_visual_test=False)


def main():
    h = Hooks(DLL, REPORT, b'.mgnd')
    # EAX contains vDir.z. ESI is CMagicEffect; +0x1c is current PART iterator.
    # No absolute pointer is embedded in the stub, so DLL rebasing stays safe.
    h.hook(0x1001afd0, '8d4c240457', f'''
        push ecx;
        mov ecx, dword ptr [esi+0x1c];
        mov ecx, dword ptr [ecx+8];
        cmp dword ptr [ecx+0x54], {GROUND_SPEED};
        jne ordinary;
        xor eax, eax;
    ordinary:
        pop ecx;
        lea ecx, [esp+4];
        push edi;
    ''', 'level explicitly marked stationary spell layers')
    data, report = h.finish()
    report['validation'] = verify(h.original, data)
    DLL.write_bytes(data)
    REPORT.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print('PASS: native stationary spell orientation, relocated DLLs, unchanged ordinary effects')


if __name__ == '__main__': main()
