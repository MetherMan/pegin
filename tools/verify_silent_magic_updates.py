"""Count native active-update work omitted by the silent mage allocation patch.

This is an executed-x86 instruction/call comparison, not a wall-clock or FPS
benchmark. Engine, Common math and the normal-style Sound Update3D all run as
shipped native code. Rendering and audio-device work are outside this fixture.
"""
from pathlib import Path
from collections import Counter
import hashlib
import json
import re
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
import pefile
from patch_silent_magic import build
from patch_mage_ground import machine, GROUND_SPEED
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *


def batch(image, base, sound_base, has_sound, start=0, frames=120):
    u = machine(image, base)
    sound_pe = pefile.PE(str(R/'client-overlay/Sound.dll'))
    native = sound_pe.get_memory_mapped_image()
    # Exact native SND_NORMAL early-return path, and Update3D vtable slot.
    assert native[0x5f50:0x5f59] == bytes.fromhex('568bf1807e28647617')
    assert native[0x5f70:0x5f7b] == bytes.fromhex('8a462884c00f84a3000000')
    assert native[0x601e:0x6022] == bytes.fromhex('5ec20400')
    assert struct.unpack_from('<I', native, 0x12300)[0] == 0x10005f50
    assert native[0x5c1d:0x5c20] == bytes.fromhex('885e28')  # ctor style = BL = 0
    sound_pe.relocate_image(sound_base)
    u.mem_map(sound_base, (sound_pe.OPTIONAL_HEADER.SizeOfImage+4095)&~4095)
    u.mem_write(sound_base, sound_pe.get_memory_mapped_image())
    magic, data, effect = 0x30000100, 0x30000400, 0x30000800
    head, node, parthead, partnode, part = [0x30001000+i*0x100 for i in range(5)]
    stack, stop, particles, sound = 0x40008000, 0x3000f000, 0x30002000, 0x30005000
    def word(at, value): u.mem_write(at, struct.pack('<I', value))
    def vector(at, value): u.mem_write(at, struct.pack('<3f', *value))
    word(magic+0xb8, 1)
    word(magic+0xec, data); word(data+0x34, 1)
    word(magic+0xe4, head); word(magic+0xe8, 1)
    word(head, node); word(node, head); word(node+4, head); word(node+8, effect)
    word(magic+0x24, particles); vector(magic+0xc0, (0,0,0))
    word(effect+0x14, parthead); word(effect+0x18, 1); word(effect+0x1c, partnode)
    word(parthead, partnode); word(partnode, parthead); word(partnode+8, part)
    word(effect+0xd4, 0x30003000)
    vector(effect+0x48, (0,-5,0)); vector(effect+0x30, (0,0,0))
    word(effect+0xc0, start); word(effect+0xc4, GROUND_SPEED)
    vector(part+0xc, (0,0,.07))
    word(part+0x44, start); word(part+0x48, 100000)
    word(part+0x54, GROUND_SPEED); word(part+0x6c, 1)
    # Constructor-created silent SoundObject3D has normal style byte +0x28=0,
    # null sample handles and its real shipped vtable. No device/OS stubs run.
    word(sound, sound_base+0x122f4)
    word(effect+0x10c, sound if has_sound else 0)
    counts = dict(engine=0, common=0, sound=0, update3d_calls=0)
    def count(u, address, size, _):
        if base <= address < base+0x100000:
            counts['engine'] += 1
        elif 0x22000000 <= address < 0x22100000:
            counts['common'] += 1
        elif sound_base <= address < sound_base+0x100000:
            counts['sound'] += 1
        else:
            raise AssertionError(f'Unexpected external code: {address:#x}')
        if address == sound_base+0x5f50:
            counts['update3d_calls'] += 1
    u.hook_add(UC_HOOK_CODE, count)
    history, per_frame = [], []
    for _ in range(frames):
        previous = dict(counts)
        u.mem_write(stack, struct.pack('<II', stop, 33))
        saved = {UC_X86_REG_EBX:0x1357, UC_X86_REG_ESI:0x2468,
                 UC_X86_REG_EDI:0x3579, UC_X86_REG_EBP:0x468a}
        for reg, value in saved.items(): u.reg_write(reg, value)
        u.reg_write(UC_X86_REG_ESP, stack); u.reg_write(UC_X86_REG_ECX, magic)
        u.emu_start(base+0x19110, stop, count=50000)
        assert u.reg_read(UC_X86_REG_EIP) == stop
        assert u.reg_read(UC_X86_REG_ESP) == stack+8
        assert u.reg_read(UC_X86_REG_EAX) == 1
        assert all(u.reg_read(reg) == value for reg,value in saved.items())
        # Both variants must preserve every effect field except sound ownership.
        state = bytes(u.mem_read(effect, 0x10c))+bytes(u.mem_read(effect+0x110, 16))
        history.append(state)
        per_frame.append({key:counts[key]-previous[key] for key in counts})
    return dict(frames=frames, totals=counts, per_frame=per_frame), history


def main():
    patched, patch_report, original = build()
    tests = []
    for base, sound_base in ((0x10000000,0x24000000),(0x16000000,0x25000000)):
        for start in (0, 8000):
            before, old_history = batch(original, base, sound_base, True, start)
            after, new_history = batch(patched, base, sound_base, False, start)
            assert old_history == new_history
            expected_calls = 120 if start == 0 else 0
            assert before['totals']['update3d_calls'] == expected_calls
            assert after['totals']['update3d_calls'] == 0
            delta = {key:before['totals'][key]-after['totals'][key] for key in before['totals']}
            assert delta == dict(engine=4*expected_calls, common=0,
                                 sound=9*expected_calls, update3d_calls=expected_calls)
            for old_frame,new_frame in zip(before['per_frame'], after['per_frame']):
                expected = dict(engine=4,common=0,sound=9,update3d_calls=1) if start==0 else dict.fromkeys(delta,0)
                assert {key:old_frame[key]-new_frame[key] for key in old_frame} == expected
            for variant in (before,after):
                profiles = Counter(tuple(frame.items()) for frame in variant.pop('per_frame'))
                variant['per_frame_profiles'] = [dict(frames=n,counts=dict(profile)) for profile,n in profiles.items()]
            tests.append(dict(engine_base=hex(base),sound_base=hex(sound_base),
                              waiting_for_start=start!=0,before=before,after=after,
                              omitted=delta,state_equal_every_frame=True))
    bounds = {}
    for kind in ('meteor','frost'):
        raw = (R/f'client-overlay/Magic/mt_{kind}B.ms').read_text(encoding='cp949')
        blocks = re.findall(r'\[EFFECT\](.*?)\[/EFFECT\]',raw,re.S)
        count = sum(f'[SPEED] {GROUND_SPEED}' in block and '[ENEMY]' in block
                    and '[SOUND]' not in block and '[BLOWTIMING]' not in block for block in blocks)
        bounds[kind] = dict(silent_layers=count,instructions_omitted=count*13,virtual_calls_omitted=count)
    report = dict(passed=True, patched_engine_sha256=patch_report['sha256'],
                  sound_sha256=hashlib.sha256((R/'client-overlay/Sound.dll').read_bytes()).hexdigest(),
                  scope='CMagic::Update through native CMagicEffect::Update and native SoundObject3D::Update3D',
                  frames_per_comparison=120,tests=tests,
                  per_active_silent_layer=dict(engine_instructions_omitted=4,sound_instructions_omitted=9,
                                               total_instructions_omitted=13,virtual_calls_omitted=1),
                  per_waiting_silent_layer=dict(instructions_omitted=0,virtual_calls_omitted=0),
                  conservative_per_cast_per_frame_upper_bounds=bounds,
                  in_game_fps_measured=False,
                  limits='Instruction counts are not CPU cycles or FPS. Fixture omits SPE rendering and particles; unchanged rendering cost cancels between variants. Bounds assume all silent layers active, so delayed/expired layers reduce actual savings. No idle cost is saved when there is no active magic.')
    destination = R/'assets/skills140/silent-magic-update-validation.json'
    destination.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print('PASS: 480 paired native update frames at two DLL bases; 13 instructions and one no-op call omitted per active silent layer')


if __name__ == '__main__': main()
