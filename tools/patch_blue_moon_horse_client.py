"""Blue Moon horse (ride type 5) client hooks, layered on the existing hell-horse patches.

DeicideOnline.exe (.bmoon): ride type 5 loads Vehicle/mt_bluemoonhorse; rider mode 7 (vehicle 2 + type 5)
reuses the original horse posture. Other types fall through to the unchanged .hell code.
UInterface.dll (.bmsnd): certificate 19131 joins the native horse whinny branch.
Type 5 already uses the horse animation mode (0x408780) and the ordinary hoof dust (.hfire only
replaces type 4), which the tests below confirm without changing those paths.
"""
from pathlib import Path
import json, struct, sys
ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT/'runtime/pylibs'), str(ROOT/'.cache/weapon-build-deps'), str(ROOT/'tools')]
from pe_hooks import Hooks
from patch_stack_client import machine
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *

OUT = ROOT/'assets/blue-moon-horse'
MODELS = {0: 'horse_1', 1: 'horse_1', 2: 'horse_2', 3: 'horse_3', 4: 'mt_hellhorse', 5: 'mt_bluemoonhorse', 6: 'horse_1',
          0xffffffff: 'horse_1'}


def current_jump(h, va, size):
    off = h.pe.get_offset_from_rva(va-h.base)
    raw = bytes(h.data[off:off+size])
    assert raw[0] == 0xe9 and set(raw[5:]) <= {0x90}, (hex(va), raw.hex())
    return raw.hex(), va+5+struct.unpack_from('<i', raw, 1)[0]


def patch_exe():
    path = ROOT/'client-overlay/DeicideOnline.exe'
    h = Hooks(path, OUT/'client-hooks.json', b'.bmoon')
    model = h.base+h.rva+len(h.code)
    h.code.extend(b'mt_bluemoonhorse\0')
    old_model, hell_model = current_jump(h, 0x422220, 5)
    old_rider, hell_rider = current_jump(h, 0x4054e8, 6)
    h.hook(0x422220, old_model,
           f'mov eax,[esp+4]; cmp eax,5; jne previous; mov eax,{model}; ret 4; previous: jmp {hex(hell_model)}',
           'ride type 5 loads the Blue Moon horse model')
    h.hook(0x4054e8, old_rider,
           'mov edi,[esp+0xc]; cmp di,7; jne previous; mov edi,3; mov [esi],eax; jmp 0x4054ee; '
           f'previous: jmp {hex(hell_rider)}',
           'Blue Moon horse rider uses the original horse posture')
    data, report = h.finish()
    cases = []
    for kind, want in MODELS.items():
        u, _ = machine(data)
        sp, stop = 0x201f000, 0x201d000
        u.mem_write(sp, struct.pack('<II', stop, kind))
        u.emu_start(0x422220, stop, count=500)
        got = bytes(u.mem_read(u.reg_read(UC_X86_REG_EAX), 32)).split(b'\0')[0].decode()
        assert got == want, (kind, got, want)
        assert u.reg_read(UC_X86_REG_ESP) == sp+8
        cases.append(dict(kind=kind, model=got))
    for kind in range(7):
        u, _ = machine(data)
        u.mem_write(0x201f000, struct.pack('<II', 0x201d000, kind))
        u.emu_start(0x408780, 0x201d000, count=100)
        assert u.reg_read(UC_X86_REG_EAX) & 0xffff == 1, kind
    for mode in range(9):
        u, _ = machine(data)
        u.mem_write(0x201f00c, struct.pack('<I', mode))
        u.reg_write(UC_X86_REG_ESI, 0x2001000)
        u.reg_write(UC_X86_REG_EAX, 0x1234)
        u.emu_start(0x4054e8, 0x4054ee, count=100)
        assert u.reg_read(UC_X86_REG_EDI) == (3 if mode in (6, 7) else mode), mode
        assert bytes(u.mem_read(0x2001000, 4)) == struct.pack('<I', 0x1234)
    report['validation'] = dict(model_cases=cases, horse_animation_mode_types=list(range(7)), rider_modes=list(range(9)),
                                rider_modes_mapped_to_horse={6: 3, 7: 3})
    return path, data, report


def patch_ui():
    path = ROOT/'client-overlay/UInterface.dll'
    h = Hooks(path, OUT/'sound-ui-hooks.json', b'.bmsnd')
    old, previous = current_jump(h, 0x100257b8, 5)
    h.hook(0x100257b8, old, f'cmp eax,19131; je 0x100257cd; jmp {hex(previous)}',
           'Blue Moon horse certificate uses the native horse whinny and item handling')
    data, report = h.finish()
    horses = [10188, 10189, 10190, 19130, 19131]
    for base in [0x10000000, 0x13000000]:
        for item in horses+[10191, 19129, 19132, 0]:
            u, _ = machine(data, base)
            u.reg_write(UC_X86_REG_EAX, item)
            reached = []
            yes, no = base+0x257cd, base+0x257e3

            def halt(mu, pc, size, unused):
                if pc in [yes, no]:
                    reached.append(pc)
                    mu.emu_stop()
            u.hook_add(UC_HOOK_CODE, halt)
            u.emu_start(base+0x257b8, 0, count=100)
            assert reached == [yes if item in horses else no], (hex(base), item, reached)
    report['validation'] = dict(item_sound_cases=18, relocated_bases=[0x10000000, 0x13000000], horse_items=horses)
    return path, data, report


def main():
    exe = patch_exe()
    ui = patch_ui()
    # Publish only after every native instruction-path test passed.
    for (path, data, report), name in [(exe, 'client-hooks.json'), (ui, 'sound-ui-hooks.json')]:
        path.write_bytes(data)
        (OUT/name).write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(dict(exe=exe[2]['validation']['rider_modes_mapped_to_horse'], models=len(exe[2]['validation']['model_cases']),
                          ui=ui[2]['validation']['item_sound_cases'])))


if __name__ == '__main__':
    main()
