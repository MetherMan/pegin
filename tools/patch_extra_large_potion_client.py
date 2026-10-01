"""UInterface.dll (.xlpot): the extra-large HP potion 19132 joins the client's six hard-coded potion ids.

The shop opens its quantity box (EDT_POSIONBUY_NUM) only for 10095..10100, and four inventory/UI
paths treat a potion count of 0 as 1. Each check ends with `cmp reg,0x2772; jne not_potion`; that
pair is hooked to also accept 19132. Layered on the existing .stk16 and .bmsnd sections.
"""
from pathlib import Path
import json, struct, sys
ROOT = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(ROOT/'runtime/pylibs'), str(ROOT/'.cache/weapon-build-deps'), str(ROOT/'tools')]
from pe_hooks import Hooks
from patch_stack_client import machine
from unicorn import UC_HOOK_CODE
from unicorn.x86_const import *

OUT = ROOT/'assets/potions'
ITEM = 19132
POTIONS = [10095, 10096, 10097, 10098, 10099, 10100, ITEM]
# (last cmp, register, potion path, other path, start of the whole id test, stack slot or None)
SITES = [
    (0x100233bf, 'ebx', 0x100233c7, 0x100233d0, 0x1007c050, 0x20, 'UI count normalisation 1'),
    (0x1002352f, 'edi', 0x10023537, 0x10023540, 0x1007c060, 0x20, 'UI count normalisation 2'),
    (0x10023775, 'ebx', 0x1002377d, 0x10023786, 0x1007c070, 0x18, 'UI count normalisation 3'),
    (0x100238d5, 'ebx', 0x100238dd, 0x100238e6, 0x1007c080, 0x18, 'UI count normalisation 4'),
    (0x1003eeec, 'eax', 0x1003eef7, 0x1003ef80, 0x1003eec9, None, 'shop quantity box (W3DUIShop)'),
]
REGS = {'eax': UC_X86_REG_EAX, 'ebx': UC_X86_REG_EBX, 'edi': UC_X86_REG_EDI}


def patch():
    path = ROOT/'client-overlay/UInterface.dll'
    h = Hooks(path, OUT/'ui-hooks.json', b'.xlpot')
    for va, reg, yes, no, _, _, label in SITES:
        off = h.pe.get_offset_from_rva(va-h.base)
        cmp = list(h.cs.disasm(bytes(h.data[off:off+6]), va, 1))[0]
        assert (cmp.mnemonic, cmp.op_str) == ('cmp', f'{reg}, 0x2772'), (hex(va), cmp.mnemonic, cmp.op_str)
        jne = list(h.cs.disasm(bytes(h.data[off+cmp.size:off+cmp.size+6]), va+cmp.size, 1))[0]
        assert jne.mnemonic == 'jne' and int(jne.op_str, 16) == no and va+cmp.size+jne.size == yes, (hex(va), jne.op_str)
        old = bytes(h.data[off:off+cmp.size+jne.size]).hex()
        h.hook(va, old, f'cmp {reg},0x2772; je {hex(yes)}; cmp {reg},{ITEM}; je {hex(yes)}; jmp {hex(no)}',
               f'{label}: also accept {ITEM}')
    data, report = h.finish()
    cases = 0
    for base in [0x10000000, 0x13000000]:
        shift = base-0x10000000
        for _, reg, yes, no, start, slot, label in SITES:
            for item in POTIONS+[0, 10094, 10101, 19131, 19133, 0x12771]:
                for count in ([0, 1, 9999] if slot else [1]):
                    u, _ = machine(data, base)
                    u.reg_write(REGS[reg], item)
                    if slot:
                        u.mem_write(0x201f000+slot, struct.pack('<I', count))
                    else:
                        u.reg_write(UC_X86_REG_ECX, 0x2000100)
                    reached = []

                    def halt(mu, pc, size, unused):
                        if pc in (yes+shift, no+shift):
                            reached.append(pc-shift)
                            mu.emu_stop()
                    u.hook_add(UC_HOOK_CODE, halt)
                    u.emu_start(start+shift, 0, count=60)
                    assert reached == [yes if item in POTIONS else no], (hex(base), label, item, reached)
                    assert u.reg_read(REGS[reg]) == item and u.reg_read(UC_X86_REG_ESP) == 0x201f000
                    if slot:
                        assert u.reg_read(UC_X86_REG_ECX) & 0xffff == count & 0xffff
                    cases += 1
    report['validation'] = dict(cases=cases, relocated_bases=[0x10000000, 0x13000000], potion_ids=POTIONS,
                                sites=[s[-1] for s in SITES])
    return path, data, report


def main():
    path, data, report = patch()
    path.write_bytes(data)  # only after every emulated path passed
    (OUT/'ui-hooks.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(report['validation']))


if __name__ == '__main__':
    main()
