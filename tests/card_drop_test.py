"""Exercise the shipped ELF's real drop selection, card rates and RNG offline.

Requires Unicorn (or --deps PATH containing it). Item creation/map placement
are captured at their boundaries; no server, database or save is opened.
"""
import argparse
from collections import defaultdict
import hashlib
import json
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
CARDS = {10193, 10194}


def table():
    result = defaultdict(list)
    for line in (ROOT / 'game-data/DATA/MobItemLoseTable.txt').read_text(encoding='cp949').splitlines():
        if line and line[0].isdigit():
            mob, item, chance = map(int, line.split())
            if item and chance:
                result[mob].insert(0, (item, chance))  # Native INSERT_TO_LIST.
    return result


class DropServer:
    def __init__(self, binary, rows, rate=30, controlled=False, mob_id=34):
        from native_server import Server
        from unicorn import UC_HOOK_CODE
        from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ESP
        self.server = s = Server(binary)
        self.uc = u = s.uc
        self.mob = s.player
        self.info = s.player + 0x1000
        self.nodes = s.player + 0x2000
        self.item = s.player + 0x4000
        self.empty = s.player + 0x5000
        self.killer = s.player + 0x10000
        self.created = []
        self.placed = []
        self.index = 0
        self.roll = 1
        self.number_calls = []
        self.hooks = []
        self.write(self.mob + 4, mob_id)
        self.write(self.mob + 8, 60)
        self.write(s.address('g_MOBINFO') + mob_id * 4, self.info)
        # Legacy i386 ABI: inven, invenCnt, currDropItem; verified in native code.
        self.write(self.info + 0xe4, self.nodes if rows else 0)
        self.write(self.info + 0xe8, len(rows))
        for i, (item, chance) in enumerate(rows):
            u.mem_write(self.nodes + i * 16, struct.pack('<4I', item, chance, 0,
                        self.nodes + (i + 1) * 16 if i + 1 < len(rows) else 0))
        u.mem_write(s.address('_ZL6loaded'), b'\x01')
        self.write(s.address('_ZL8cardRate'), rate)

        def capture_create(uc, address, size, data):
            sp = uc.reg_read(UC_X86_REG_ESP)
            self.created.append(self.read(sp + 4))

        def capture_map(uc, address, size, data):
            self.placed.append(self.created[-1] if self.created else None)

        a = s.address('_Z10CreateItem')
        u.mem_write(a, b'\xb8' + struct.pack('<I', self.item) + b'\xc3')
        self.hooks.append(u.hook_add(UC_HOOK_CODE, capture_create, begin=a, end=a))
        a = s.address('_Z9ItemToMap')
        s.stub('_Z9ItemToMap')
        self.hooks.append(u.hook_add(UC_HOOK_CODE, capture_map, begin=a, end=a))
        s.stub('_Z11CreateMoney')
        if controlled:
            a = s.address('_Z6numberii')
            u.mem_write(a, b'\xc3')

            def number(uc, address, size, data):
                sp = uc.reg_read(UC_X86_REG_ESP)
                lo, hi = struct.unpack('<2i', uc.mem_read(sp + 4, 8))
                self.number_calls.append((lo, hi))
                value = self.index if lo == 0 else self.roll if hi == 1000 else hi
                assert lo <= value <= hi, (lo, hi, value)
                uc.reg_write(UC_X86_REG_EAX, value)

            self.hooks.append(u.hook_add(UC_HOOK_CODE, number, begin=a, end=a))

    def close(self):
        # Break ctypes callback cycles before Python's shutdown finalizers.
        for hook in self.hooks:
            self.uc.hook_del(hook)
        self.hooks.clear()
        self.uc = None
        self.server = None

    def write(self, address, value):
        self.uc.mem_write(address, struct.pack('<I', value))

    def read(self, address):
        return struct.unpack('<I', self.uc.mem_read(address, 4))[0]

    def kill(self, killer=0):
        self.created.clear()
        self.placed.clear()
        self.number_calls.clear()
        self.server.call('_Z14GetMonsterItem', self.mob, self.empty, self.empty, killer)
        assert self.created == self.placed, (self.created, self.placed)
        assert len(self.created) <= 1
        return self.created[0] if self.created else None


def run(args):
    drops = table()
    taros = [base + variant for base in (34, 37, 38, 44, 80)
             for variant in (0, 200, 300, 400, 500, 600)]
    assert all(len(drops[m]) == 26 and [i for i, x in enumerate(drops[m]) if x[0] in CARDS] == [0, 1]
               for m in taros)
    report = {'binary_sha256': hashlib.sha256(args.binary.read_bytes()).hexdigest(),
              'taros_monster_types_checked': taros, 'cards_per_cycle_before': 2,
              'fixed_noncard_kills_before': 24}
    if args.before:
        old = DropServer(args.before, drops[34], controlled=True)
        sequence = [old.kill() for _ in range(52)]
        assert sequence == [x[0] for x in drops[34]] * 2
        old.close()
        old = DropServer(args.before, drops[34], controlled=True)
        old.kill(); old.kill()
        old.write(old.mob + 8, 61)
        assert old.kill() == drops[34][2][0]
        old.close()
        report['before_cards_at_kills'] = [i + 1 for i, x in enumerate(sequence) if x in CARDS]
        report['changing_map_does_not_reset_old_cursor'] = True

    # Every original entry remains selectable, independent of the legacy cursor.
    for mob in taros:
        f = DropServer(args.binary, drops[mob], controlled=True, mob_id=mob)
        f.write(f.info + 0xec, f.nodes + 24 * 16)
        for index, (item, chance) in enumerate(drops[mob]):
            f.index = index
            assert f.kill() == item
            assert f.number_calls[0] == (0, len(drops[mob]) - 1)
        assert f.read(f.info + 0xec) == f.nodes + 24 * 16
        f.close()

    # Card multiplier boundaries, cap, and normal-item probability are unchanged.
    for rate in (0, 1, 30, 1000):
        f = DropServer(args.binary, drops[34], rate=rate, controlled=True)
        for index in (0, 1, 2):
            f.index = index
            item, base = drops[34][index]
            chance = min(base * rate, 1000) if item in CARDS else base
            for roll in sorted({1, max(1, chance), min(1000, chance + 1), 1000}):
                f.roll = roll
                assert f.kill() == (item if roll <= chance else None), (rate, index, roll)
        f.close()
    # Card blessing must inspect the selected entry, even with an absent cursor.
    f = DropServer(args.binary, drops[34], rate=1, controlled=True)
    f.write(f.killer + 0x32428, 1)  # ch2.billingFlag[dBILL_ITEM_EFFECT7]
    f.roll = 99
    assert f.kill(f.killer) == 10194
    f.roll = 100
    assert f.kill(f.killer) is None
    f.index = 2
    f.roll = 201
    assert f.kill(f.killer) is None
    f.close()
    for rows in ([], [(10194, 33)], [(10196, 200)]):
        f = DropServer(args.binary, rows, controlled=True)
        assert f.kill() == (rows[0][0] if rows else None)
        f.close()

    # Actual native PRNG, uninterrupted hunting and identical-seed map/cursor changes.
    report['soak'] = []
    for seed in (1, 20260923, 123456789):
        f = DropServer(args.binary, drops[34])
        f.server.call('_Z9e_srandomm', seed)
        windows = []
        trace = []
        for window in range(4):
            outcomes = [f.kill() for _ in range(2600)]
            trace.extend(outcomes)
            count = sum(x in CARDS for x in outcomes)
            assert 140 <= count <= 260, (seed, window, count)
            windows.append(count)
        g = DropServer(args.binary, drops[34])
        g.server.call('_Z9e_srandomm', seed)
        for i, expected in enumerate(trace):
            if i % 31 == 0:
                g.write(g.mob + 8, 60 + i % 7)
                g.write(g.info + 0xec, g.nodes + (i % 26) * 16)
            assert g.kill() == expected
        report['soak'].append({'seed': seed, 'kills': len(trace), 'cards_per_2600_kills': windows})
        f.close()
        g.close()
    report['passed'] = True
    report['native_soak_kills'] = 62400
    report['scope'] = 'Native selection/rate/RNG; item allocation and world placement captured, no live player session.'
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--binary', type=Path, default=ROOT / 'server-bin/LAQIA_GameServer')
    p.add_argument('--before', type=Path)
    p.add_argument('--deps', type=Path)
    p.add_argument('--report', type=Path, default=ROOT / 'tests/results/card-drop.json')
    args = p.parse_args()
    if args.deps:
        sys.path.insert(0, str(args.deps))
    run(args)
