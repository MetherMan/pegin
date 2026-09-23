"""Stage a guarded ice-only alpha branch in client-overlay/Render.dll.

No installed/runtime DLL is ever written. Validation executes the real x86
Render body against a fake D3D8 device at the preferred and a relocated base.
The original additive path, mesh calls, WED format, and Z-write stay unchanged.
"""
from pathlib import Path
import argparse
import hashlib
import json
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path[:0] = [str(ROOT / 'runtime/pylibs'), str(ROOT / '.cache/weapon-build-deps'),
                str(ROOT / 'client-overlay/Tools/SkillColors')]
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *
from pe_hooks import Hooks
from ice_opacity import is_ice_surface

DLL = ROOT / 'client-overlay/Render.dll'
REPORT = ROOT / 'assets/skills140/ice-opacity-renderer-patch.json'
VALIDATION = ROOT / 'assets/skills140/ice-opacity-renderer-validation.json'
ORIGINAL_SHA = '1361ad946b6657635e24c9432017115e4d23fa22213af7caf672e0c98327ce50'
BASE = 0x10000000
DEVICE_RVA = 0x89328
RENDER_RVA = 0x24220


def sha(data):
    return hashlib.sha256(data).hexdigest()


def classifier_asm():
    """ECX=this; EAX=bool. VC6 std::string data/length are this+14h/+18h.

    ASCII folding is deliberate: generated filenames and policy tokens are ASCII.
    Neither prefix comparisons nor suffix comparisons read outside string length.
    """
    s = ['push ebx', 'push esi', 'push edi', 'mov esi, [ecx+0x14]',
         'test esi, esi', 'jz no', 'mov edx, [ecx+0x18]', 'add edx, esi',
         'mov edi, esi', 'scan:', 'cmp esi, edx', 'jae basename',
         'mov al, [esi]', 'inc esi', 'cmp al, 0x2f', 'je separator',
         'cmp al, 0x5c', 'jne scan', 'separator:', 'mov edi, esi',
         'jmp scan', 'basename:', 'mov ebx, edx', 'sub ebx, edi']

    def compare(token, offset, fail, label):
        for i, ch in enumerate(token):
            s.extend([f'mov al, byte ptr [edi+{offset+i}]', 'cmp al, 0x41',
                      f'jb {label}_{i}', 'cmp al, 0x5a', f'ja {label}_{i}',
                      'add al, 0x20', f'{label}_{i}:', f'cmp al, {ord(ch)}', f'jne {fail}'])

    s += ['cmp ebx, 4', 'jb no', 'sub edi, 4', 'add edi, ebx']
    compare('.wed', 0, 'no', 'suffix')
    s += ['sub edi, ebx', 'add edi, 4', f'cmp ebx, {len("mf_cocytus_pillar.wed")}', 'jne eruption']
    compare('mf_cocytus_pillar.wed', 0, 'eruption', 'pillar')
    s += ['jmp yes', 'eruption:', 'cmp ebx, 16', 'jb crown']
    compare('mf_eruption_', 0, 'crown', 'eruption_cmp')
    s += ['jmp yes', 'crown:', f'cmp ebx, {len("mf_crown_spike") + 4}', 'jb no']
    compare('mf_crown_spike', 0, 'no', 'crown_cmp')
    s += ['yes:', 'mov eax, 1', 'jmp done', 'no:', 'xor eax, eax',
          'done:', 'pop edi', 'pop esi', 'pop ebx', 'ret']
    return '; '.join(s)


def get_device_asm():
    # call/pop is position independent; no unregistered absolute relocation.
    return f'call device_pc; device_pc: pop edi; sub edi, device_pc; add edi, {hex(BASE + DEVICE_RVA)}; mov edi, [edi]'


def build(path=DLL, report=REPORT):
    h = Hooks(path, report, section=b'.icealp')
    assert sha(h.original) == ORIGINAL_SHA, 'Unknown Render.dll: refuse to patch'
    classifier = h.base + h.rva
    h.code.extend(h.asm(classifier_asm(), classifier))
    entry = f'''
        push ebp; mov ebp, esp; sub esp, 16; push esi; push edi;
        mov esi, ecx; call {hex(classifier)}; test eax, eax; jz passthrough;
        cmp dword ptr [esi+0x0c], 0; je passthrough;
        cmp dword ptr [ebp+16], 0; jne capture;
        mov eax, [ebp+8]; cmp eax, [esi+0x34]; jge passthrough;
        capture: {get_device_asm()};
        lea eax, [ebp-4]; push eax; push 4; push 0; push edi;
        mov eax, [edi]; call dword ptr [eax+0xf8]; test eax, eax; js failed;
        lea eax, [ebp-8]; push eax; push 5; push 0; push edi;
        mov eax, [edi]; call dword ptr [eax+0xf8]; test eax, eax; js failed;
        lea eax, [ebp-12]; push eax; push 6; push 0; push edi;
        mov eax, [edi]; call dword ptr [eax+0xf8]; test eax, eax; js failed;
        push dword ptr [ebp+16]; push dword ptr [ebp+12]; push dword ptr [ebp+8];
        mov ecx, esi; call original; mov [ebp-16], eax;
        push dword ptr [ebp-4]; push 4; push 0; push edi;
        mov eax, [edi]; call dword ptr [eax+0xfc];
        push dword ptr [ebp-8]; push 5; push 0; push edi;
        mov eax, [edi]; call dword ptr [eax+0xfc];
        push dword ptr [ebp-12]; push 6; push 0; push edi;
        mov eax, [edi]; call dword ptr [eax+0xfc];
        push 2; push 0x13; push edi; mov eax, [edi]; call dword ptr [eax+0xc8];
        push 2; push 0x14; push edi; mov eax, [edi]; call dword ptr [eax+0xc8];
        mov eax, [ebp-16]; jmp returned;
        failed: xor eax, eax;
        returned: pop edi; pop esi; mov esp, ebp; pop ebp; ret 12;
        passthrough: mov ecx, esi; pop edi; pop esi; mov esp, ebp; pop ebp;
        original: push esi; mov esi, ecx; push edi; mov edi, [esp+0x14]
    '''
    h.hook(BASE + RENDER_RVA, '568bf1578b7c2414', entry, 'Capture and restore alpha states around selected ice Render')
    setup = f'''
        mov ecx, esi; call 0x100243d0;
        pushad; mov ecx, esi; call {hex(classifier)}; test eax, eax; jz finished;
        {get_device_asm()};
        push 5; push 0x13; push edi; mov eax, [edi]; call dword ptr [eax+0xc8];
        push 6; push 0x14; push edi; mov eax, [edi]; call dword ptr [eax+0xc8];
        push 2; push 4; push 0; push edi; mov eax, [edi]; call dword ptr [eax+0xfc];
        push 3; push 5; push 0; push edi; mov eax, [edi]; call dword ptr [eax+0xfc];
        finished: popad
    '''
    h.hook(0x100242d2, '8bcee8f7000000', setup, 'Ice only SRCALPHA/INVSRCALPHA and TFACTOR alpha before mesh draw')
    data, result = h.finish()
    result.update(classifier_va=classifier, protocol='ice-opacity-v1',
                  scope='client-overlay only; no installed/runtime writes',
                  policy='basename case-insensitive: exact mf_cocytus_pillar.wed; mf_eruption_*.wed; mf_crown_spike*.wed',
                  alpha='WED dwColor high byte, multiplied only by standard SRCALPHA/INVSRCALPHA blend',
                  depthWrite=False, alphaStageRestored=[4, 5, 6], sourceEncoding='cp949')
    return data, result, h.original


class Machine:
    def __init__(self, data, base):
        p = pefile.PE(data=data)
        p.relocate_image(base)
        self.u = u = Uc(UC_ARCH_X86, UC_MODE_32)
        self.base = base
        u.mem_map(base, 0x200000)
        u.mem_write(base, p.get_memory_mapped_image())
        u.mem_map(0x2000000, 0x40000)
        self.obj, self.device, self.vtable = 0x2001000, 0x2002000, 0x2003000
        self.mesh, self.index, self.text = 0x2004000, 0x2005000, 0x2006000
        self.stop = 0x203f000
        self.logs, self.draws = [], []
        self.rs = {19: 9, 20: 10, 14: 1}
        self.ts = {(0, 4): 4, (0, 5): 0x12, (0, 6): 0x20}
        self.fail_get = None
        self.put(base + DEVICE_RVA, self.device)
        self.put(self.device, self.vtable)
        self.methods = {0xc8: ('RS', 3), 0xfc: ('TS', 4), 0xf8: ('GET', 4),
                        0xf4: ('TEXTURE', 3), 0x154: ('INDICES', 3),
                        0x130: ('SHADER', 2), 0x14c: ('STREAM', 4), 0x11c: ('DRAW', 6)}
        for offset, (_, argc) in self.methods.items():
            addr = 0x2030000 + offset * 8
            self.put(self.vtable + offset, addr)
            u.mem_write(addr, b'\xc2' + struct.pack('<H', argc * 4))
        u.hook_add(UC_HOOK_CODE, self.step)
        self.put(self.obj + 12, self.mesh)
        self.put(self.obj + 0x34, 10)
        self.put(self.obj + 0x40, 1)
        self.put(self.obj + 0x44, 1)
        self.put(self.mesh, 12)
        self.put(self.mesh + 4, 20)
        self.put(self.mesh + 0x50, self.index)
        self.put(self.mesh + 0x54, 0x2007000)
        self.put(self.index + 8, 0x2008000)

    def put(self, address, value):
        self.u.mem_write(address, struct.pack('<I', value & 0xffffffff))

    def get(self, address):
        return struct.unpack('<I', self.u.mem_read(address, 4))[0]

    def name(self, value):
        raw = value.encode('ascii')
        self.u.mem_write(self.text, raw + b'\0')
        self.put(self.obj + 0x14, self.text)
        self.put(self.obj + 0x18, len(raw))

    def step(self, u, address, size, unused):
        offset = (address - 0x2030000) // 8
        if address == 0x2030000 + offset * 8 and offset in self.methods:
            kind, argc = self.methods[offset]
            esp = u.reg_read(UC_X86_REG_ESP)
            args = [self.get(esp + 4 + i * 4) for i in range(argc)]
            assert args[0] == self.device
            self.logs.append((kind, tuple(args[1:])))
            result = 0
            if kind == 'RS':
                self.rs[args[1]] = args[2]
            elif kind == 'TS':
                self.ts[tuple(args[1:3])] = args[3]
            elif kind == 'GET':
                if args[2] == self.fail_get:
                    result = 0x8876086c
                else:
                    self.put(args[3], self.ts[tuple(args[1:3])])
            elif kind == 'DRAW':
                self.draws.append({'rs': dict(self.rs), 'ts': dict(self.ts), 'args': args[1:]})
            u.reg_write(UC_X86_REG_EAX, result)
            u.reg_write(UC_X86_REG_ECX, 0xa1a2a3a4)
            u.reg_write(UC_X86_REG_EDX, 0xb1b2b3b4)
        if address == self.stop:
            u.emu_stop()

    def call(self, address, args=()):
        esp = 0x202f000
        self.u.mem_write(esp, struct.pack('<' + 'I' * (len(args) + 1), self.stop, *args))
        self.u.reg_write(UC_X86_REG_ESP, esp)
        self.u.reg_write(UC_X86_REG_ECX, self.obj)
        preserved = {UC_X86_REG_EBX: 0x11113333, UC_X86_REG_ESI: 0x44446666,
                     UC_X86_REG_EDI: 0x77779999, UC_X86_REG_EBP: 0xaaaacccc}
        for reg, value in preserved.items():
            self.u.reg_write(reg, value)
        self.u.emu_start(address, self.stop + 1, count=30000)
        assert self.u.reg_read(UC_X86_REG_EIP) == self.stop, 'Execution did not return'
        assert self.u.reg_read(UC_X86_REG_ESP) == esp + 4 * (len(args) + 1), 'Calling convention/stack changed'
        assert all(self.u.reg_read(r) == v for r, v in preserved.items()), 'Callee-saved register corruption'
        return self.u.reg_read(UC_X86_REG_EAX)

    def render(self, name, alpha=217, frame=0, tool=1):
        self.name(name)
        self.put(self.obj + 0x20, (alpha << 24) | 0x17467f)
        return self.call(self.base + RENDER_RVA, (frame, 0x2009000, tool))


def validate(data, report, original):
    generated = [p.name for p in (ROOT / 'client-overlay/Effect').glob('*.wed')]
    candidates = generated + ['mf_eruption_.wed', 'mf_eruption_0.wed', 'mf_eruption_foo.wed',
        'mf_crown_spike.wed', 'mf_crown_spike24.wed', 'mf_cocytus_pillar.wed',
        'mf_cocytus_pillar_rim.wed', 'mf_cocytus_pillar.wed.x', 'xmf_eruption_1.wed',
        'mf_crown_spike24.wem', 'eruption.wed', '', '.wed', 'a.wed', 'mf_eruption_1xwed']
    candidates = sorted(set(candidates))
    counts = {'classification': 0, 'draws': 0, 'nativeColorTransfer': 0,
              'unchanged': 0, 'failureAndEarlyReturn': 0}
    for base in (BASE, 0x03500000):
        m = Machine(data, base)
        for name in candidates:
            for value in (name, 'Effect/' + name, 'Effect\\' + name.upper(), 'nested/other/' + name):
                m.name(value)
                actual = m.call(report['classifier_va'] - BASE + base)
                assert bool(actual) == is_ice_surface(value), (value, actual)
                counts['classification'] += 1
        for name in ('Effect/mf_eruption_0.wed', 'Effect/mf_cocytus_pillar.wed',
                     'Effect/mf_crown_spike24.wed', 'Effect\\MF_ERUPTION_123.WED'):
            for alpha in (0, 64, 128, 217, 255):
                m = Machine(data, base)
                alpha_before = dict(m.ts)
                argb = (alpha << 24) | 0x17467f
                # The original native SetBright implementation preserves all 32
                # WED dwColor bits; no guessed alpha channel in texture/scale.
                m.call(base + 0x24520, (argb,))
                assert m.get(m.obj + 0x20) == m.rs[60] == argb
                counts['nativeColorTransfer'] += 1
                assert m.render(name, alpha) == 1
                assert len(m.draws) == 1
                draw = m.draws[0]
                assert draw['rs'][19] == 5 and draw['rs'][20] == 6
                assert draw['rs'][14] == 0, 'Z-write changed'
                assert draw['rs'][60] == (alpha << 24) | 0x17467f
                assert draw['ts'][(0, 4)] == 2 and draw['ts'][(0, 5)] == 3
                assert draw['ts'][(0, 6)] == alpha_before[(0, 6)]
                assert all(m.ts[k] == v for k, v in alpha_before.items()), 'Alpha stage leaked'
                assert m.rs[19] == m.rs[20] == 2
                # The immediately following additive effect sees the original alpha state.
                m.render('Effect/mf_cocytus_pillar_rim.wed', alpha)
                assert m.draws[-1]['rs'][19] == m.draws[-1]['rs'][20] == 2
                assert all(m.draws[-1]['ts'][k] == v for k, v in alpha_before.items())
                counts['draws'] += 2
        for name in ('Effect/mf_cocytus_pillar_rim.wed', 'Effect/meteor_blast.wed',
                     'Effect/mh_pierce_core.wed', 'Effect/ms_arrow.wed',
                     'Effect/mf_ice_fog.wed', 'Effect/mf_cocytus_seal.wed', 'Effect/normal.wed'):
            patched, clean = Machine(data, base), Machine(original, base)
            assert patched.render(name) == clean.render(name)
            assert patched.logs == clean.logs, ('Other effect calls changed', name)
            assert patched.draws == clean.draws and patched.rs == clean.rs and patched.ts == clean.ts
            counts['unchanged'] += 1
        for failure in (4, 5, 6):
            m = Machine(data, base)
            m.fail_get = failure
            before_rs, before_ts = dict(m.rs), dict(m.ts)
            assert m.render('Effect/mf_eruption_0.wed') == 0
            assert not m.draws and m.rs == before_rs and m.ts == before_ts
            counts['failureAndEarlyReturn'] += 1
        for empty_mesh, frame in ((True, 0), (False, 10)):
            patched, clean = Machine(data, base), Machine(original, base)
            if empty_mesh:
                patched.put(patched.obj + 12, 0)
                clean.put(clean.obj + 12, 0)
            assert patched.render('Effect/mf_eruption_0.wed', frame=frame, tool=0) == 0
            assert clean.render('Effect/mf_eruption_0.wed', frame=frame, tool=0) == 0
            assert patched.logs == clean.logs and not patched.draws
            counts['failureAndEarlyReturn'] += 1
    # Patch must reject unexpected bytes, rather than accidentally changing an unknown DLL.
    assert sha(original) == ORIGINAL_SHA
    source = (ROOT / 'src/client/render/Src/W3DSpecialEffect.cpp').read_bytes().decode('cp949')
    assert '_strnicmp( name, "mf_eruption_", 12 )' in source
    assert '_strnicmp( name, "mf_crown_spike", 14 )' in source
    return dict(passed=True, cases=counts, bases=[hex(BASE), '0x03500000'],
                nativeFunction='CW3DSpecialEffect::Render, complete x86 body; fake D3D8 device',
                classificationMatchesPython=True, unrelatedRenderCallsByteForByteEquivalent=True,
                nativeWEDColorPreservesARGB=True, sourceEncoding='cp949',
                restoresIncomingAlphaStage=True, restoresAdditiveBlend=True, depthWrite=False,
                installedRuntimeTouched=False, inGameDirect3DTest=False,
                limitation='Unicorn checks native instructions/device calls, not an actual D3D8 GPU scene.')


def protected_snapshot():
    paths = [ROOT / 'runtime/GameClient/Render.dll']
    baseline = ROOT / '.cache/v12-before.json'
    if baseline.exists():
        paths.extend(Path(p) for p in json.loads(baseline.read_text(encoding='utf-8'))['protected'])
    return {str(p): sha(p.read_bytes()) for p in paths if p.is_file()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply-overlay', action='store_true', help='Write only guarded client-overlay/Render.dll after native validation')
    args = parser.parse_args()
    before = protected_snapshot()
    data, report, original = build()
    validation = validate(data, report, original)
    # Demonstrate deterministic reruns from the exact reversible report, in task cache.
    cache = ROOT / '.cache/ice-opacity-patch'
    cache.mkdir(parents=True, exist_ok=True)
    test_dll, test_report = cache / 'Render.dll', cache / 'patch.json'
    test_dll.write_bytes(data)
    test_report.write_text(json.dumps(report), encoding='utf-8')
    again, repeated, restored = build(test_dll, test_report)
    assert again == data and restored == original and repeated == report
    # An altered original must fail before any write.
    damaged = bytearray(original); damaged[0x24220] ^= 1
    test_dll.write_bytes(damaged)
    try:
        build(test_dll, test_report)
    except AssertionError:
        pass
    else:
        raise AssertionError('Unknown original byte guard failed')
    test_dll.write_bytes(data)
    validation.update(idempotent=True, unexpectedBytesRejected=True, protectedFiles=len(before))
    assert protected_snapshot() == before, 'Protected runtime/install files changed during validation'
    validation['protectedHashesUnchanged'] = before
    if args.apply_overlay:
        # No parameter permits an alternate write target or installation directory.
        assert DLL.resolve() == (ROOT / 'client-overlay/Render.dll').resolve()
        DLL.write_bytes(data)
        REPORT.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
        assert DLL.read_bytes() == data and protected_snapshot() == before
    validation['overlayApplied'] = args.apply_overlay
    validation['sha256'] = report['sha256']
    VALIDATION.write_text(json.dumps(validation, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: v for k, v in validation.items() if k != 'protectedHashesUnchanged'}, ensure_ascii=False))


if __name__ == '__main__':
    main()
