"""Engine.dll (.shild): tooltip prices of the custom items (19000+) read 실드, not 캐쉬.

W3DItemInfoMgr (DII_SELLPRICE) labels every item id >= 12102 as 캐쉬, the range where the Power
cash items start. The ids from 19000 up (Blood Knight/Twilight weapons, Blue Moon armour, skill
books, horse certificates, extra-large potion 19132) are bought and sold for 실드 by the server
(SHOP_BuyItem/SHOP_SellItem always use money), so only 12102..18999 keep the 캐쉬 label. Both the
own-item (sell price) and shop (buy price) branches are hooked; emulated at two load addresses.
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
CASH_FIRST, CUSTOM_FIRST = 12102, 19000
# (cmp word [edi],12102 / jb shild), cash path, shild path
SITES = [(0x100250ef, 0x100250f6, 0x10025104, 'own item: sell price'),
         (0x1002512d, 0x10025134, 0x10025142, 'shop: buy price')]


def patch():
    h = Hooks(ROOT/'client-overlay/Engine.dll', OUT/'engine-price-label-hooks.json', b'.shild')
    for va, cash, shild, label in SITES:
        off = h.pe.get_offset_from_rva(va-h.base)
        old = bytes(h.data[off:off+7])
        assert old == b'\x66\x81\x3f'+struct.pack('<H', CASH_FIRST)+bytes([0x72, shild-(va+7)]), (hex(va), old.hex())
        h.hook(va, old.hex(), f'cmp word ptr [edi],{CASH_FIRST}; jb {hex(shild)}; cmp word ptr [edi],{CUSTOM_FIRST}; '
                              f'jae {hex(shild)}; jmp {hex(cash)}', f'{label}: 캐쉬 only for {CASH_FIRST}..{CUSTOM_FIRST-1}')
    data, report = h.finish()
    cases = 0
    for base in [0x10000000, 0x03500000]:
        shift = base-0x10000000
        for va, cash, shild, _ in SITES:
            for item in [1, 10097, 11996, 12101, 12102, 12165, 13530, 18999, 19000, 19030, 19131, 19132, 19140, 19250, 20479]:
                u, _ = machine(data, base)
                u.mem_write(0x2000100, struct.pack('<H', item))
                u.reg_write(UC_X86_REG_EDI, 0x2000100)
                reached = []

                def halt(mu, pc, size, unused):
                    if pc in (cash+shift, shild+shift):
                        reached.append(pc-shift)
                        mu.emu_stop()
                u.hook_add(UC_HOOK_CODE, halt)
                u.emu_start(va+shift, 0, count=40)
                want = cash if CASH_FIRST <= item < CUSTOM_FIRST else shild
                assert reached == [want], (hex(base), hex(va), item, reached)
                assert u.reg_read(UC_X86_REG_ESP) == 0x201f000
                cases += 1
    report['validation'] = dict(cases=cases, cash_range=[CASH_FIRST, CUSTOM_FIRST-1], relocated_bases=[0x10000000, 0x03500000])
    return data, report


def main():
    data, report = patch()
    (ROOT/'client-overlay/Engine.dll').write_bytes(data)
    (OUT/'engine-price-label-hooks.json').write_text(json.dumps(report, indent=2)+'\n', encoding='utf-8')
    p = ROOT/'src/client/engine/Src/W3DItemInfoMgr.cpp'
    s = p.read_bytes().decode('cp949')
    old = 'if( pItemInfoDat->wItemNum >= 12102 )'
    new = 'if( pItemInfoDat->wItemNum >= 12102 && pItemInfoDat->wItemNum < 19000 )'
    if old in s:
        p.write_bytes(s.replace(old, new).encode('cp949'))
    print(json.dumps(report['validation']))


if __name__ == '__main__':
    main()
