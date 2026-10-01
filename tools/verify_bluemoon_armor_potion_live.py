"""Live check of the extra-large potion and the Blue Moon armour on disposable local players.

Potion 19132: live definition, potion-merchant list, buying several (price x count, merges into one
stack), drinking heals 600 and keeps the stack. Armour 19140..19250: live definitions match
assets/blue-moon-armor/item_definitions.json, the warehouse 세트 page shows the Twilight set in
its slots, shelf copies, equip without stat limits, card enchant +0 -> +1, GM enhance to +20 on all
four parts and relog persistence. The warehouse 기타 page still lists 19131 and now 19132.
Deletes only the fixture rows and checks the existing save tables are unchanged.
"""
from pathlib import Path
import argparse, json, struct, sys, time, uuid
ROOT = Path(__file__).resolve().parents[1]
MARK = struct.pack('<i', -1700000001)
SET = {19030: (2, 0), 19060: (2, 1), 19090: (4, 1), 19140: (0, 2), 19170: (1, 2), 19200: (2, 2), 19230: (3, 2)}
PARTS = [(19140, '갑옷', 5), (19170, '바지', 7), (19200, '장갑', 2), (19230, '신발', 11)]


def main(runtime):
    sys.path[:0] = [str(runtime), str(runtime/'pylibs'), str(ROOT/'tools')]
    import local_control as ctl
    from ssh_vm import connect
    from verify_stacks_live import Player
    c = connect()
    checks, players, accounts = [], [], []
    cmd = lambda s: ctl.command(c, s)

    def sql(s):
        i, o, e = c.exec_command('mariadb -N -B LAQIA_GAMEDB')
        i.write(s+';\n')
        i.channel.shutdown_write()
        result, error = o.read(), e.read()
        assert o.channel.recv_exit_status() == 0, error.decode(errors='replace')
        return result.decode().strip()

    def gdb(s):
        pid = cmd('systemctl show -p MainPID --value LAQIA_GameServer').strip()
        with c.open_sftp() as f:
            with f.open('/tmp/bluemoon-armor-check.gdb', 'w') as out:
                out.write('set pagination off\nset confirm off\n'+s+'\ndetach\nquit\n')
        result = cmd('gdb -q -batch -p '+pid+' -x /tmp/bluemoon-armor-check.gdb 2>&1')
        assert 'Error in sourced command file' not in result, result
        return result

    def value(account, expr):
        out = gdb(f'set $qa=FindPlayerIdList("{account}")\nprintf "VALUE=%d\\n",{expr}')
        return int(next(l[6:] for l in out.splitlines() if l.startswith('VALUE=')))

    def passed(s):
        checks.append(s)
        print('PASS '+s, flush=True)

    def messages(events):
        return [d[2:2+struct.unpack_from('<H', d)[0]].decode('cp949', 'replace') for t, d in events if t == 150]

    def shelf(events):
        data = next(d for t, d in events if MARK in d)
        at = data.index(MARK)
        total = data[at-1]
        at += 7
        cells = []
        for _ in range(total-1):  # the count includes the header entry
            item, cat, x, y = struct.unpack_from('<iBBB', data, at)
            at += 7
            cells.append((item, cat, x, y))
        return cells

    def fixture(prefix, money=0, stats=(1, 10, 1)):
        account = prefix+uuid.uuid4().hex[:10]
        assert sql(f"SELECT COUNT(*) FROM UserTable WHERE id='{account}'") == '0'
        sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,money,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill,isNewChar) "
            f"VALUES('{account}','{account}',3,266,272,{money},500,100,500,100,{stats[0]},{stats[1]},{stats[2]},0,201,0)")
        accounts.append(account)
        return account

    def equip(account):
        return sql(f"SELECT itemNum,equipPos FROM EquipItems WHERE ownerID='{account}' ORDER BY equipPos")

    assert not cmd("ss -Htn state established 'sport = :2560'").strip(), 'Close all clients first'
    digest = ('mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems '
              'UserSkills UserCashMoney | sha256sum')
    before = cmd(digest).split()[0]
    try:
        # Definitions loaded by the running server.
        expected = json.loads((ROOT/'assets/blue-moon-armor/item_definitions.json').read_text(encoding='utf-8'))
        script = 'python\nimport gdb,json\ndata=[]\nfor n in [19132]+list(range(19140,19251)):\n p=gdb.parse_and_eval("g_ItemInfo")[n]\n' \
                 ' if int(p)==0:\n  data.append(dict(id=n));continue\n p=p.dereference()\n' \
                 ' data.append(dict(id=n,level=int(p["isUniq"]),rank=int(p["itemLevel"]),type=int(p["itemType"]),' \
                 'min=int(p["minDamage"]),price=int(p["Price"]),cls=int(p["needClass"])))\nprint("ITEMS="+json.dumps(data))\nend'
        memory = {d['id']: d for d in json.loads(next(l[6:] for l in gdb(script).splitlines() if l.startswith('ITEMS=')))}
        assert memory[19132] == dict(id=19132, level=0, rank=1, type=44, min=600, price=720, cls=memory[19132]['cls'])
        for d in expected:
            m = memory[d['id']]
            assert (m['level'], m['rank'], m['type'], m['min'], m['price']) == (d['level'], 9, d['type'], d['defense'], d['price']), (m, d)
        assert all(len(memory[n]) == 1 for n in range(19140, 19251) if n not in {d['id'] for d in expected})
        passed('live server: 19132 heal 600 / price 720 type 44; 84 armour rows Rank 9 with the planned defense and price')

        # Potion: merchant list, buy several, merge, drink.
        account = fixture('qp', 2000000)
        p = Player(account)
        players.append(p)
        listing = next(d for t, d in p.packet(66, struct.pack('<H', 9)) if t == 67)
        assert struct.pack('<i', 19132) in listing and struct.pack('<i', 10097) in listing
        money = value(account, '$qa->ch.money')
        p.packet(69, struct.pack('<HiH', 9, 19132, 10))
        assert p.quantities(19132) == [10] and value(account, '$qa->ch.money') == money-7200, p.quantities(19132)
        p.packet(69, struct.pack('<HiH', 9, 10097, 10))
        assert p.quantities(10097) == [10] and value(account, '$qa->ch.money') == money-7200-6000
        passed('potion merchant 9 sells 19132; 10 cost 7,200 vs 6,000 for 10 large potions (+20%)')
        p.packet(69, struct.pack('<HiH', 9, 19132, 5))
        assert p.quantities(19132) == [15]
        passed('a second purchase merges into the same stack (15)')
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->ch.max_hp=5000\nset $qa->ch.hp=10')
        state = [value(account, e) for e in ('$qa->ch.hp', '$qa->ch.max_hp', '$qa->ch2.addHP')]
        potion = p.select(19132)
        events = p.packet(111, struct.pack('<iB', potion, 0))
        after = [value(account, e) for e in ('$qa->ch.hp', '$qa->ch.max_hp', '$qa->ch2.addHP')]
        assert after[0] == 610 and p.items[potion]['qty'] == 14, (state, after, p.items[potion], [(t, d.hex()) for t, d in events])
        large = p.select(10097)
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->ch.hp=10')
        p.packet(111, struct.pack('<iB', large, 0))
        assert value(account, '$qa->ch.hp') == 130
        passed('drinking heals 600 (large potion still 120) and leaves 14 in the stack')
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->ch.hp=10')
        p.packet(114, struct.pack('<H', 19132))
        assert value(account, '$qa->ch.hp') == 610 and p.items[potion]['qty'] == 13, (value(account, '$qa->ch.hp'), p.items[potion])
        passed('quick-slot use (packet 114) also heals 600 and leaves 13')
        p.close()
        players.remove(p)
        time.sleep(.8)
        p = Player(account)
        players.append(p)
        assert p.quantities(19132) == [13]
        passed('potion stack of 13 survives relog')
        p.close()
        players.remove(p)

        # Armour: warehouse page, copies, equip, card enchant, GM enhance, relog.
        account = fixture('qa', stats=(20, 1, 1))  # warrior: STR highest, as Black Knight armour requires
        p = Player(account)
        players.append(p)
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin=1\nset $qa->adminLevel=3')
        cells = shelf(p.command('/재뽕 창고 트와일라잇'))
        page = {item: (x, y) for item, cat, x, y in cells if cat == 0 and item in SET}
        assert page == SET, cells
        passed('warehouse 세트 page "푸른달 트와일라잇": sword/longbow/staff in the weapon rows, 4 armour parts in the warrior row')
        for base, _, _ in PARTS+[(19140, '', 0)]:
            events = p.packet(69, struct.pack('<HiH', 250, base, 1))
            assert any(t == 43 and struct.unpack_from('<i', d, 4)[0] == base for t, d in events), messages(events)
        assert sorted(v['num'] for v in p.items.values() if v['num'] >= 19140) == [19140, 19140, 19170, 19200, 19230]
        passed('shelf double-click copies each armour part')
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin=0\nset $qa->adminLevel=0')
        for base, _, _ in PARTS:
            p.packet(111, struct.pack('<iB', p.select(base), 0))
        assert equip(account).split() == ['19200', '2', '19140', '5', '19170', '7', '19230', '11'], equip(account)
        passed('all four parts equip on a level-1 warrior (same class rule as Black Knight armour)')
        sql(f"INSERT INTO InvenItems(itemNum,invenPage,invenX,invenY,ownerID,ownerPos,life,exValue1) VALUES (10194,1,0,0,'{account}',0,100,3)")
        p.close()
        players.remove(p)
        time.sleep(.8)
        p = Player(account)
        players.append(p)
        events = p.packet(145, struct.pack('<ii', p.select(19140), p.select(10194)))
        assert any(t == 146 and d == b'\1' for t, d in events) and p.select(19141) and p.quantities(10194) == [2]
        passed('armour card enchant +0 -> +1 (19140 -> 19141) uses one card, like other armour')
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin=1\nset $qa->adminLevel=3')
        for base, slot, pos in PARTS:
            for level in (10, 20):
                text = messages(p.command(f'/재뽕 {slot} {level}'))
                assert any('[GM 완료]' in s for s in text), (slot, level, text)
        assert equip(account).split() == ['19220', '2', '19160', '5', '19190', '7', '19250', '11'], equip(account)
        saved = equip(account)
        p.close()
        players.remove(p)
        time.sleep(.8)
        p = Player(account)
        players.append(p)
        assert equip(account) == saved
        passed('GM enhance +10/+20 on armour, pants, gauntlets and boots; +20 set survives relog')
        events = p.command('/재뽕 창고 초대형')
        assert any(i == 19132 and cat == 4 for i, cat, _, _ in shelf(events))
        events = p.command('/재뽕 창고 소유증서')
        assert any(i == 19131 and cat == 4 for i, cat, _, _ in shelf(events))
        passed('warehouse 기타 lists 초대형 물약 19132 and 푸른달의 말 소유증서 19131')
    finally:
        for p in players:
            p.close()
        time.sleep(1)
        for account in accounts:
            for table in ['InvenItems', 'EquipItems', 'UserSkills', 'UserCashMoney']:
                sql(f"DELETE FROM {table} WHERE ownerID='{account}'")
            sql(f"DELETE FROM UserTable WHERE id='{account}'")
        assert before == cmd(digest).split()[0], 'Existing save data changed'
        c.close()
    passed('existing five save tables unchanged; disposable players removed')
    out = ROOT/'assets/blue-moon-armor/live-validation.json'
    out.write_text(json.dumps(dict(passed=True, checks=checks, save_data_preserved=True, in_game_visual_test=False),
                              ensure_ascii=False, indent=2)+'\n', encoding='utf-8')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--runtime', type=Path, required=True)
    main(parser.parse_args().runtime.resolve())
