"""Stop default IME composition handling outside chat in client-overlay/UInterface.dll.

CW3DUInterface::IMEProc returns "unhandled" when no edit box is active, so the
game window passes WM_IME_START/END/COMPOSITION to DefWindowProc. Windows then
creates and destroys its composition window for every key typed in Hangul mode
while moving (runtime/diagnostics/lag-analysis-20260924.md: 39 of 40 default
IME calls). The original developers blocked these (W3DGUIInput.cpp rev 77/79)
but the shipped build has the block commented out.

The patch reports only those three composition messages as handled when no
editor is active. Chat input (editor present), WM_IME_SETCONTEXT/NOTIFY and all
other messages keep the original code path.
"""
from pathlib import Path
import argparse, hashlib, json, struct, sys

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path[:0] = [str(ROOT / 'runtime/pylibs'), str(ROOT / 'tools')]
import pefile
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import *
from pe_hooks import Hooks

DLL = ROOT / 'client-overlay/UInterface.dll'
REPORT = ROOT / 'assets/performance/uinterface-ime-idle-patch.json'
VALIDATION = ROOT / 'assets/performance/uinterface-ime-idle-validation.json'
ORIGINAL_SHA = '42b35656203734794acc3b067ca7a0d0e7391001968715f16c28f8e2e0dd4352'
BASE = 0x10000000
IMEPROC_RVA = 0x216d0
SITE_RVA = 0x21744          # editor-absent branch: mov dword ptr [esi+1D4h], 0
SITE_BYTES = 'c786d401000000000000'
EDITOR = 0x18c
WM_IME_STARTCOMPOSITION, WM_IME_COMPOSITION = 0x10d, 0x10f

# EDI=message, EBX=0 (the return value) on this branch.
CAVE = f'''
    mov dword ptr [esi+0x1d4], 0
    cmp edi, {WM_IME_STARTCOMPOSITION}
    jb keep
    cmp edi, {WM_IME_COMPOSITION}
    ja keep
    mov ebx, 1
keep:
'''


def sha(data):
    return hashlib.sha256(data).hexdigest()


def build(path=DLL, report=REPORT):
    h = Hooks(path, report, section=b'.imeidle')
    assert sha(h.original) == ORIGINAL_SHA, 'Unknown UInterface.dll: refuse to patch'
    h.hook(BASE + SITE_RVA, SITE_BYTES, CAVE, 'IMEProc without editor: handle composition messages')
    data, result = h.finish()
    result.update(protocol='uinterface-ime-idle-v1', handled_without_editor=['0x10d', '0x10e', '0x10f'],
                  source='src/client/uinterface/Src/W3DGUIInput.cpp:3290-3329')
    return data, result, h.original


def ime_proc(data, base, msg, wparam, editor=0):
    p = pefile.PE(data=data)
    p.relocate_image(base)
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    u.mem_map(base, 0x200000)
    u.mem_write(base, p.get_memory_mapped_image())
    u.mem_map(0x2000000, 0x40000)
    this, stop, esp = 0x2001000, 0x203f000, 0x202f000
    u.mem_write(this + EDITOR, struct.pack('<I', editor))
    u.mem_write(this + 0x1d4, struct.pack('<I', 0x55))
    u.mem_write(esp, struct.pack('<5I', stop, 0x1234, msg, wparam, 0x5678))
    preserved = {UC_X86_REG_EBX: 0x11113333, UC_X86_REG_ESI: 0x44446666,
                 UC_X86_REG_EDI: 0x77779999, UC_X86_REG_EBP: 0xaaaacccc}
    for reg, value in preserved.items():
        u.reg_write(reg, value)
    u.reg_write(UC_X86_REG_ESP, esp)
    u.reg_write(UC_X86_REG_ECX, this)
    u.emu_start(base + IMEPROC_RVA, stop, count=2000)
    assert u.reg_read(UC_X86_REG_EIP) == stop and u.reg_read(UC_X86_REG_ESP) == esp + 20
    assert all(u.reg_read(r) == v for r, v in preserved.items()), 'Callee-saved register changed'
    ime_flag = struct.unpack('<I', u.mem_read(this + 0x1d4, 4))[0]
    return u.reg_read(UC_X86_REG_EAX), ime_flag


def validate(data, original):
    count = 0
    for base in (BASE, 0x03500000):
        for msg in list(range(0x100, 0x120)) + [0x0f, 0x102, 0x104, 0x201, 0x281, 0x282, 0x283, 0x284, 0x285, 0x286, 0x290, 0x291]:
            wparam = 0x41  # never VK_RETURN, which calls the chat ReturnKey helper
            patched = ime_proc(data, base, msg, wparam)
            clean = ime_proc(original, base, msg, wparam)
            expected = (1 if WM_IME_STARTCOMPOSITION <= msg <= WM_IME_COMPOSITION else clean[0], 0)
            assert clean == (0, 0), (hex(msg), clean)
            assert patched == expected, (hex(msg), patched)
            count += 1
    p = pefile.PE(data=data)
    site = p.get_offset_from_rva(SITE_RVA)
    header_end = pefile.PE(data=original).OPTIONAL_HEADER.SizeOfHeaders
    diff = [i for i in range(len(original)) if data[i] != original[i] and not site <= i < site + len(bytes.fromhex(SITE_BYTES))]
    assert all(i < header_end for i in diff), 'Unexpected byte change outside headers and the hook'
    return dict(passed=True, messages=count, bases=[hex(BASE), '0x03500000'],
                editorPresentPath='byte-identical (hook is only on the editor-absent branch)',
                limitation='Unicorn runs the native IMEProc; Korean chat typing must still be checked in game')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply-overlay', action='store_true', help='Write the validated client-overlay/UInterface.dll')
    args = parser.parse_args()
    data, report, original = build()
    validation = validate(data, original)
    cache = ROOT / '.cache/uinterface-ime-idle'
    cache.mkdir(parents=True, exist_ok=True)
    (cache / 'UInterface.dll').write_bytes(data)
    (cache / 'patch.json').write_text(json.dumps(report), encoding='utf-8')
    again, repeated, restored = build(cache / 'UInterface.dll', cache / 'patch.json')
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
