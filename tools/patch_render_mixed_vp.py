"""Restore the engine's designed mixed vertex processing in client-overlay/Render.dll.

CW3DScene::CreateDevice computed MIXED for T&L hardware, then forced
SOFTWARE_VERTEXPROCESSING (W3DScene.cpp:856). Every vertex of terrain, weapons,
effects and UI was then transformed on the CPU each frame, which is the
movement/camera-turn lag recorded in runtime/diagnostics/lag-analysis-20260924.md.

The engine already switches D3DRS_SOFTWAREVERTEXPROCESSING on around the two
paths that need it (skinned meshes in CW3DModel::Render, CBlockList::Draw), so
those stay on the CPU exactly as before. GPUs without vertex shader 1.0 (the
original engine rule) and any MIXED CreateDevice failure fall back to the old
SOFTWARE device, so no PC loses the ability to start the game.

Fog: screens before the world (character select) enable fog without ever
choosing a fog mode. The software pipeline then applies no fog, but hardware
vertex processing fogs everything to the fog colour, which blacked out the
select-screen stage. On a MIXED device only, after CreateDevice and after every
successful Reset, the default becomes linear fog starting beyond any far plane,
which is the same "no fog" result. CW3DWorld::Draw still sets its own fog every
frame, so world fog is unchanged.

Validation executes the patched native code in Unicorn against fake D3D8
objects at the preferred and a relocated base. tools/verify_render_mixed_vp.py
renders real game models through the original and patched Render.dll.
"""
from pathlib import Path
import argparse, hashlib, json, struct, sys

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path[:0] = [str(ROOT / 'runtime/pylibs'), str(ROOT / 'tools')]
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *
from pe_hooks import Hooks

DLL = ROOT / 'client-overlay/Render.dll'
REPORT = ROOT / 'assets/performance/render-mixed-vp-patch.json'
VALIDATION = ROOT / 'assets/performance/render-mixed-vp-validation.json'
ORIGINAL_SHA = '1aff9c55db7e490fb089a9d37921d22d3f85424036634d45eccbbf906712af29'
BASE = 0x10000000
CREATE_RVA = 0x1ecde
# mov ecx,[eax]; push 20h; push edx; push 1; push ebx; push eax; call [ecx+3Ch]
CREATE_BYTES = '8b086a20526a015350ff513c'
RESET_RVA = 0x1f43d
# push edx; push eax; mov ecx,[eax]; call [ecx+38h]   (CW3DScene::Restore)
RESET_BYTES = '52508b08ff5138'
SOFTWARE, MIXED = 0x20, 0x80
VS_1_0 = 0xfffe0100
CAPS_SIZE, CAPS_VS = 0xd4, 0xc4
RS_FOGSTART, RS_FOGEND, RS_FOGVERTEXMODE, FOG_LINEAR = 36, 37, 140, 3
FOG_START, FOG_END = 10000.0, 20000.0


def f32(value):
    return struct.unpack('<I', struct.pack('<f', value))[0]


# stdcall FogDefault(device): only for MIXED devices, set linear fog beyond any far plane.
FOG_DEFAULT = f'''
    push ebx
    mov ebx, [esp+8]
    sub esp, 16
    mov eax, [ebx]
    push esp
    push ebx
    call dword ptr [eax+0x24]
    test eax, eax
    jl fog_done
    test dword ptr [esp+12], {MIXED}
    jz fog_done
    mov eax, [ebx]
    push {FOG_LINEAR}
    push {RS_FOGVERTEXMODE}
    push ebx
    call dword ptr [eax+0xc8]
    mov eax, [ebx]
    push {f32(FOG_START)}
    push {RS_FOGSTART}
    push ebx
    call dword ptr [eax+0xc8]
    mov eax, [ebx]
    push {f32(FOG_END)}
    push {RS_FOGEND}
    push ebx
    call dword ptr [eax+0xc8]
fog_done:
    add esp, 16
    pop ebx
    ret 4
'''


def create_cave(fog):
    # Entry: EAX=IDirect3D8, EDX=hWnd, EBP=CW3DScene, [ESP]=&present, [ESP+4]=&g_pd3dDevice.
    # Exit (at the original return address): EAX=HRESULT, both pushed arguments consumed.
    return f'''
    push edi
    mov edi, esp
    sub esp, {CAPS_SIZE + 8}
    mov [esp+{CAPS_SIZE}], edx
    mov ecx, [eax]
    push esp
    push 1
    push 0
    push eax
    call dword ptr [ecx+0x34]
    test eax, eax
    jl software
    cmp dword ptr [esp+{CAPS_VS}], {VS_1_0}
    jb software
    mov eax, [ebp+0x1c]
    mov ecx, [eax]
    push dword ptr [edi+8]
    push dword ptr [edi+4]
    push {MIXED}
    push dword ptr [esp+{CAPS_SIZE + 12}]
    push 1
    push 0
    push eax
    call dword ptr [ecx+0x3c]
    test eax, eax
    jl software
    mov [esp+{CAPS_SIZE + 4}], eax
    mov ecx, [edi+8]
    push dword ptr [ecx]
    call {hex(fog)}
    mov eax, [esp+{CAPS_SIZE + 4}]
    jmp created
software:
    mov eax, [ebp+0x1c]
    mov ecx, [eax]
    push dword ptr [edi+8]
    push dword ptr [edi+4]
    push {SOFTWARE}
    push dword ptr [esp+{CAPS_SIZE + 12}]
    push 1
    push 0
    push eax
    call dword ptr [ecx+0x3c]
created:
    mov esp, edi
    pop edi
    add esp, 8
'''


def reset_cave(fog):
    # Entry: EAX=device, EDX=&present. Exit: EAX=Reset HRESULT.
    return f'''
    push ebx
    mov ebx, eax
    push edx
    push eax
    mov ecx, [eax]
    call dword ptr [ecx+0x38]
    test eax, eax
    jl reset_done
    push eax
    push ebx
    call {hex(fog)}
    pop eax
reset_done:
    pop ebx
'''


def sha(data):
    return hashlib.sha256(data).hexdigest()


def build(path=DLL, report=REPORT):
    h = Hooks(path, report, section=b'.mixvp')
    assert sha(h.original) == ORIGINAL_SHA, 'Unknown Render.dll: refuse to patch'
    fog = h.base + h.rva
    h.code.extend(h.asm(FOG_DEFAULT, fog))
    h.hook(BASE + CREATE_RVA, CREATE_BYTES, create_cave(fog), 'CreateDevice: MIXED on T&L hardware, SOFTWARE fallback')
    h.hook(BASE + RESET_RVA, RESET_BYTES, reset_cave(fog), 'Reset: restore the MIXED no-fog default')
    data, result = h.finish()
    result.update(protocol='render-mixed-vp-v2', fog_default_va=fog,
                  rule='VertexShaderVersion >= 1.0 -> MIXED, else or on failure SOFTWARE; '
                       'MIXED only: FOGVERTEXMODE=LINEAR, FOGSTART=10000, FOGEND=20000 after CreateDevice/Reset',
                  source='src/client/render/Src/W3DScene.cpp:846-856, 445-455, 1089-1112')
    return data, result, h.original


class Machine:
    """Runs the patched call sites with fake IDirect3D8 / IDirect3DDevice8 objects."""
    def __init__(self, data, base, vs=0xfffe0101, caps_hr=0, results=(0,), device_flags=MIXED, reset_hr=0):
        p = pefile.PE(data=data)
        p.relocate_image(base)
        self.u = u = Uc(UC_ARCH_X86, UC_MODE_32)
        self.base = base
        u.mem_map(base, 0x200000)
        u.mem_write(base, p.get_memory_mapped_image())
        u.mem_map(0x2000000, 0x40000)
        self.d3d, self.d3d_vtable, self.scene = 0x2001000, 0x2002000, 0x2003000
        self.present, self.device_slot, self.hwnd = 0x2004000, 0x2005000, 0x00c0ffee
        self.device, self.device_vtable = 0x2006000, 0x2007000
        self.stubs = 0x2030000
        self.vs, self.caps_hr, self.results = vs, caps_hr, list(results)
        self.device_flags, self.reset_hr = device_flags, reset_hr
        self.calls, self.states = [], {}
        self.put(self.d3d, self.d3d_vtable)
        self.put(self.device, self.device_vtable)
        self.put(self.scene + 0x1c, self.d3d)
        self.methods = {}
        for table, offset, argc, kind in ((self.d3d_vtable, 0x34, 4, 'caps'), (self.d3d_vtable, 0x3c, 7, 'create'),
                                          (self.device_vtable, 0x24, 2, 'params'), (self.device_vtable, 0x38, 2, 'reset'),
                                          (self.device_vtable, 0xc8, 3, 'rs')):
            stub = self.stubs + len(self.methods) * 16
            self.methods[stub] = (kind, argc)
            self.put(table + offset, stub)
            u.mem_write(stub, b'\xc2' + struct.pack('<H', argc * 4))
        u.hook_add(UC_HOOK_CODE, self.step)

    def put(self, address, value):
        self.u.mem_write(address, struct.pack('<I', value & 0xffffffff))

    def get(self, address):
        return struct.unpack('<I', self.u.mem_read(address, 4))[0]

    def step(self, u, address, size, unused):
        if address not in self.methods:
            return
        kind, argc = self.methods[address]
        esp = u.reg_read(UC_X86_REG_ESP)
        args = tuple(self.get(esp + 4 + 4 * i) for i in range(argc))
        result = 0
        if kind == 'caps':
            assert args[:3] == (self.d3d, 0, 1)
            u.mem_write(args[3], bytes(CAPS_SIZE))
            self.put(args[3] + CAPS_VS, self.vs)
            self.calls.append(('caps',))
            result = self.caps_hr
        elif kind == 'create':
            assert args[0] == self.d3d and args[1:4] == (0, 1, self.hwnd), args
            assert args[5:] == (self.present, self.device_slot), args
            self.calls.append(('create', args[4]))
            result = self.results.pop(0)
            self.put(self.device_slot, self.device if result >= 0 and result < 0x80000000 else 0)
            if result < 0x80000000:
                self.device_flags = args[4]
        elif kind == 'params':
            assert args[0] == self.device
            u.mem_write(args[1], struct.pack('<4I', 0, 1, self.hwnd, self.device_flags))
            self.calls.append(('params',))
        elif kind == 'reset':
            assert args == (self.device, self.present)
            self.calls.append(('reset',))
            self.states.clear()
            result = self.reset_hr
        elif kind == 'rs':
            assert args[0] == self.device
            self.states[args[1]] = args[2]
            self.calls.append(('rs', args[1], args[2]))
        u.reg_write(UC_X86_REG_EAX, result)
        u.reg_write(UC_X86_REG_ECX, 0xa1a2a3a4)
        u.reg_write(UC_X86_REG_EDX, 0xb1b2b3b4)

    def run(self, rva, old_bytes, regs, pushed=()):
        u = self.u
        esp = 0x202f000
        if pushed:
            u.mem_write(esp - 4 * len(pushed), struct.pack('<%dI' % len(pushed), *pushed))
        full = {UC_X86_REG_EBX: 0, UC_X86_REG_ESI: 0x44446666, UC_X86_REG_EDI: 0x77779999,
                UC_X86_REG_EBP: self.scene, UC_X86_REG_ESP: esp - 4 * len(pushed)}
        full.update(regs)
        for reg, value in full.items():
            u.reg_write(reg, value)
        resume = self.base + rva + len(bytes.fromhex(old_bytes))
        u.emu_start(self.base + rva, resume, count=5000)
        assert u.reg_read(UC_X86_REG_EIP) == resume, 'Did not resume after the original call'
        assert u.reg_read(UC_X86_REG_ESP) == esp, 'Stack differs from the original stdcall'
        for reg in (UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP):
            assert u.reg_read(reg) == full[reg], 'Callee-saved register changed'
        return u.reg_read(UC_X86_REG_EAX)

    def create(self):
        return self.run(CREATE_RVA, CREATE_BYTES, {UC_X86_REG_EAX: self.d3d, UC_X86_REG_EDX: self.hwnd,
                                                    UC_X86_REG_ESI: self.present},
                        (self.present, self.device_slot))

    def reset(self):
        return self.run(RESET_RVA, RESET_BYTES, {UC_X86_REG_EAX: self.device, UC_X86_REG_EDX: self.present,
                                                  UC_X86_REG_EDI: 0})


NO_FOG = {RS_FOGVERTEXMODE: FOG_LINEAR, RS_FOGSTART: f32(FOG_START), RS_FOGEND: f32(FOG_END)}


def validate(data, original):
    fail = 0x8876086c
    cases = [
        # vs, caps hr, CreateDevice results, expected flags tried, expected hr
        (0xfffe0101, 0, (0,), [MIXED], 0),
        (0xfffe0300, 0, (0,), [MIXED], 0),
        (VS_1_0, 0, (0,), [MIXED], 0),
        (0xfffe0101, 0, (fail, 0), [MIXED, SOFTWARE], 0),
        (0xfffe0101, 0, (fail, fail), [MIXED, SOFTWARE], fail),
        (0, 0, (0,), [SOFTWARE], 0),
        (0xfffe0000, 0, (0,), [SOFTWARE], 0),
        (0xfffe0101, fail, (0,), [SOFTWARE], 0),
    ]
    count = 0
    for base in (BASE, 0x03500000):
        for vs, caps_hr, results, flags, hr in cases:
            m = Machine(data, base, vs, caps_hr, results)
            assert m.create() == hr, (hex(vs), results)
            assert [c[1] for c in m.calls if c[0] == 'create'] == flags, (hex(vs), m.calls)
            assert m.calls[0] == ('caps',)
            mixed = hr == 0 and flags[-1] == MIXED
            assert m.states == (NO_FOG if mixed else {}), (hex(vs), m.states)
            count += 1
            if hr == 0:
                # A later device reset keeps the same default for MIXED and leaves SOFTWARE untouched.
                m.calls.clear()
                assert m.reset() == 0
                assert m.calls[0] == ('reset',)
                assert m.states == (NO_FOG if mixed else {}), m.states
                count += 1
        for flags in (MIXED, SOFTWARE):
            m = Machine(data, base, device_flags=flags, reset_hr=fail)
            assert m.reset() == fail and m.calls == [('reset',)] and not m.states
            count += 1
    # Everything outside the two hooks and the new section is byte-identical.
    p, q = pefile.PE(data=data), pefile.PE(data=original)
    hooked = []
    for rva, old in ((CREATE_RVA, CREATE_BYTES), (RESET_RVA, RESET_BYTES)):
        start = p.get_offset_from_rva(rva)
        hooked.append(range(start, start + len(bytes.fromhex(old))))
    header_end = q.OPTIONAL_HEADER.SizeOfHeaders
    diff = [i for i in range(len(original)) if data[i] != original[i] and not any(i in r for r in hooked)]
    assert all(i < header_end for i in diff), 'Unexpected byte change outside headers and the hooks'
    return dict(passed=True, cases=count, bases=[hex(BASE), '0x03500000'],
                changedOutsideHooks='PE headers only (new .mixvp section)',
                limitation='Unicorn checks the native call sites; tools/verify_render_mixed_vp.py renders real models')


def build_from(data, report):
    cache = ROOT / '.cache/render-mixed-vp'
    cache.mkdir(parents=True, exist_ok=True)
    (cache / 'Render.dll').write_bytes(data)
    (cache / 'patch.json').write_text(json.dumps(report), encoding='utf-8')
    return build(cache / 'Render.dll', cache / 'patch.json')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply-overlay', action='store_true', help='Write the validated client-overlay/Render.dll')
    args = parser.parse_args()
    data, report, original = build()
    validation = validate(data, original)
    again, repeated, restored = build_from(data, report)
    assert again == data and restored == original and repeated == report, 'Patch is not reproducible'
    validation['idempotent'] = True
    if args.apply_overlay:
        REPORT.parent.mkdir(parents=True, exist_ok=True)
        DLL.write_bytes(data)
        REPORT.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    validation.update(overlayApplied=args.apply_overlay, sha256=report['sha256'])
    VALIDATION.parent.mkdir(parents=True, exist_ok=True)
    VALIDATION.write_text(json.dumps(validation, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(validation, ensure_ascii=False))


if __name__ == '__main__':
    main()
