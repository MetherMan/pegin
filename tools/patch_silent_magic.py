"""Stage sound-object elision for explicitly marked, purely visual mage layers.

Default output is an ignored staging directory. Never changes deployed DLLs
unless the caller explicitly selects them as --output after reviewing results.
"""
from pathlib import Path
import argparse
import hashlib
import json
import re
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
from pe_hooks import Hooks
from patch_mage_ground import machine, native_ground, GROUND_SPEED
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *

DLL = R/'client-overlay/Engine.dll'
REPORT = R/'assets/skills140/silent-magic-hook.json'


def build(path=DLL, report=REPORT):
    h = Hooks(path, report, b'.msilent')
    ground = json.loads((R/'assets/skills140/ground-engine-hook.json').read_text())
    assert hashlib.sha256(h.original).hexdigest() == ground['sha256'], 'Expected validated ground patch as input'
    # Current entry contains no moved absolute relocation. Keep all existing
    # factory/IAT instructions in place; the silent branch skips over them.
    h.hook(0x10019830, '53568b74240c', f'''
        push ebx;
        push esi;
        mov esi, dword ptr [esp+0x0c];
        test esi, esi;
        jz original;
        push edi;
        push ebp;
        mov edi, dword ptr [esi+0x4c];
        mov ebp, dword ptr [edi];
        xor edx, edx;
    scan:
        cmp ebp, edi;
        je scanned;
        mov eax, dword ptr [ebp+8];
        test eax, eax;
        jz next;
        cmp dword ptr [eax+0x74], 0;
        jne needs_sound;
        cmp dword ptr [eax+0x78], 0;
        jne needs_sound;
        cmp dword ptr [eax+0x6c], 0;
        je next;
        cmp dword ptr [eax+0x54], {GROUND_SPEED};
        jne next;
        mov edx, 1;
    next:
        mov ebp, dword ptr [ebp];
        jmp scan;
    scanned:
        test edx, edx;
        jz needs_sound;
        pop ebp;
        pop edi;
        mov ebx, ecx;
        mov dword ptr [ebx+0x10c], 0;
        jmp 0x10019889;
    needs_sound:
        pop ebp;
        pop edi;
    original:
    ''', 'Omit unused sound objects only for marked silent private layers')
    data, result = h.finish()
    result.update(protocol='silent-private-mage-layers-v1', ground_patch_sha256=ground['sha256'],
                  preserved='All unmarked effects, every SOUND/BLOWTIMING part, visual resources and durations')
    return data, result, h.original


def native_case(data, base, parts):
    u = machine(data, base)
    u.mem_map(0, 0x1000)  # Exception registration used by the native destructor.
    obj, definition, own_head, input_head = [0x30000100+i*0x200 for i in range(4)]
    factory, sound, vtable, release = [0x30008000+i*0x100 for i in range(4)]
    stop, stack = 0x3000f000, 0x40008000
    calls = dict(factory=0, released=0, allocated=0, freed=0)
    def put(at, val): u.mem_write(at, struct.pack('<I', val))
    def get(at): return struct.unpack('<I', u.mem_read(at, 4))[0]
    put(obj+0x14, own_head); put(own_head, own_head); put(own_head+4, own_head)
    put(definition+0x4c, input_head)
    nodes = [0x30001000+i*0x20 for i in range(len(parts))]
    pointers = []
    put(input_head, nodes[0] if nodes else input_head)
    for i, (node, spec) in enumerate(zip(nodes, parts)):
        part = 0x30002000+i*0x200 if spec is not None else 0
        put(node, nodes[i+1] if i+1 < len(nodes) else input_head)
        put(node+8, part)
        if spec is not None:
            pointers.append(part)
            put(part+0x54, spec.get('speed', 30)); put(part+0x6c, int(spec.get('enemy', False)))
            put(part+0x74, int(spec.get('sound', False))); put(part+0x78, int(spec.get('blow', False)))
    put(base+0x3d02c, factory)
    put(sound, vtable); put(vtable, release)
    def ret(u, value=0):
        sp = u.reg_read(UC_X86_REG_ESP)
        u.reg_write(UC_X86_REG_EAX, value)
        u.reg_write(UC_X86_REG_EIP, get(sp)); u.reg_write(UC_X86_REG_ESP, sp+4)
    def service(u, address, size, _):
        if address == factory:
            calls['factory'] += 1; ret(u, sound)
        elif address == base+0x2fa33:
            ptr = 0x30005000+calls['allocated']*0x20
            calls['allocated'] += 1; ret(u, ptr)
        elif address == base+0x2f620:
            calls['freed'] += 1; ret(u)
        elif address == release:
            calls['released'] += 1; ret(u)
    u.hook_add(UC_HOOK_CODE, service)
    saved = {UC_X86_REG_EBX:0x1357, UC_X86_REG_ESI:0x2468,
             UC_X86_REG_EDI:0x3579, UC_X86_REG_EBP:0x468a}
    def invoke(rva, args):
        u.mem_write(stack, struct.pack('<'+'I'*(len(args)+1), stop, *args))
        for reg, val in saved.items(): u.reg_write(reg, val)
        u.reg_write(UC_X86_REG_ESP, stack); u.reg_write(UC_X86_REG_ECX, obj)
        u.emu_start(base+rva, stop, count=100000)
        assert u.reg_read(UC_X86_REG_EIP) == stop
        assert u.reg_read(UC_X86_REG_ESP) == stack+4*(len(args)+1)
        assert all(u.reg_read(reg) == val for reg,val in saved.items())
    invoke(0x19830, [definition])
    assert u.reg_read(UC_X86_REG_EAX) == 1
    assert get(obj+0xd4) == definition
    assert get(obj+0x18) == len(pointers)
    copied = []; node = get(own_head)
    while node != own_head:
        copied.append(get(node+8)); node = get(node)
    assert copied == pointers
    assert get(obj+0x10c) == (sound if calls['factory'] else 0)
    invoke(0x19750, [])
    assert calls['released'] == calls['factory']
    assert calls['freed'] == len(pointers)+1
    assert get(obj+0x14) == get(obj+0x18) == 0
    return calls


def verify(original, patched):
    marked = dict(speed=GROUND_SPEED, enemy=True)
    cases = [[], [None], [{}], [dict(speed=GROUND_SPEED)],
             [marked], [None,marked,None], [{},marked],
             [marked,dict(sound=True)], [dict(blow=True),marked],
             [dict(marked,sound=True)], [dict(marked,blow=True)]]
    checked = []
    for base in (0x10000000, 0x16000000):
        for parts in cases:
            sound = any(p and (p.get('sound') or p.get('blow')) for p in parts)
            private = any(p and p.get('enemy') and p.get('speed') == GROUND_SPEED for p in parts)
            old = native_case(original, base, parts)
            new = native_case(patched, base, parts)
            assert old['factory'] == 1
            assert new['factory'] == int(not private or sound)
            assert old['allocated'] == new['allocated'] and old['freed'] == new['freed']
            checked.append(dict(base=hex(base), parts=parts, before=old, after=new))
    import verify_mage_lifetime as lifetime
    class StagedImage:
        @staticmethod
        def read_bytes(): return patched
    previous = lifetime.DLL
    try:
        lifetime.DLL = StagedImage()
        for base in (0x10000000, 0x16000000):
            for origin,target in [((0,-5,0),(0,0,.07)), ((0,0,0),(0,0,5))]:
                _,matrix = native_ground(patched, base, GROUND_SPEED, origin, target)
                assert abs(matrix[6]-1) < 1e-5
            for start,duration in ((3596,3800), (853,4292)):
                assert lifetime.run(base, True, start, duration)['retained']
    finally:
        lifetime.DLL = previous
    counts = {}
    for kind in ('meteor', 'frost'):
        text = (R/f'client-overlay/Magic/mt_{kind}B.ms').read_text(encoding='cp949')
        blocks = re.findall(r'\[EFFECT\](.*?)\[/EFFECT\]', text, re.S)
        silent = [b for b in blocks if f'[SPEED] {GROUND_SPEED}' in b and '[ENEMY]' in b
                  and '[SOUND]' not in b and '[BLOWTIMING]' not in b]
        counts[kind] = dict(before=len(blocks), after=len(blocks)-len(silent), removed=len(silent))
    return dict(passed=True, native_create_and_destructor_cases=checked,
                external_services_stubbed=['component factory','list-node allocator/free','sound Release'],
                unchanged_part_copy_and_cleanup=True, stack_and_nonvolatile_registers=True,
                saved_resource_sound_objects=counts, ground_and_lifetime_native_regressions_passed=True,
                in_game_fps_measured=False)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', type=Path, default=DLL)
    parser.add_argument('--output', type=Path, default=R/'runtime/performance/Engine-silent-magic.dll')
    parser.add_argument('--report', type=Path, default=R/'runtime/performance/silent-magic-hook.json')
    args = parser.parse_args()
    data, report, original = build(args.input, REPORT)
    report['validation'] = verify(original, data)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(data)
    args.report.write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print('PASS: staged silent mage allocation optimization; native Create/destructor at two DLL bases')
    print(str(args.output))


if __name__ == '__main__': main()
