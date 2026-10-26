"""Live check of the 천상의 날개 weapons (19260..19340) on disposable local players.

Live definitions match assets/golden-knight/item_definitions.json (63 rows, Rank 10); the warehouse
세트 page "천상의 날개" puts sword / longbow / staff in their weapon slots; each weapon is copied
from the shelf, equipped by a normal level-1 character, GM-enhanced to +1/+10/+20 and still
equipped at +20 after relog. Deletes only the fixture rows; the save tables must stay unchanged.
"""
from pathlib import Path
import argparse, json, struct, sys, time, uuid
ROOT = Path(__file__).resolve().parents[1]
MARK = struct.pack('<i', -1700000001)
WEAPONS = [(19260, '무기', (2, 0)), (19290, '활', (2, 1)), (19320, '무기', (4, 1))]


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
            with f.open('/tmp/celestial-check.gdb', 'w') as out:
                out.write('set pagination off\nset confirm off\n'+s+'\ndetach\nquit\n')
        result = cmd('gdb -q -batch -p '+pid+' -x /tmp/celestial-check.gdb 2>&1')
        assert 'Error in sourced command file' not in result, result
        return result

    def admin(account, on):
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin={int(on)}\nset $qa->adminLevel={3 if on else 0}')

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

    def equip(account):
        return sql(f"SELECT itemNum,equipPos FROM EquipItems WHERE ownerID='{account}' ORDER BY equipPos")

    assert not cmd("ss -Htn state established 'sport = :2560'").strip(), 'Close all clients first'
    digest = ('mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems '
              'UserSkills UserCashMoney | sha256sum')
    before = cmd(digest).split()[0]
    try:
        expected = json.loads((ROOT/'assets/golden-knight/item_definitions.json').read_text(encoding='utf-8'))
        script = ('python\nimport gdb,json\ndata=[]\nfor n in list(range(19260,19281))+list(range(19290,19311))+list(range(19320,19341)):\n'
                  ' p=gdb.parse_and_eval("g_ItemInfo")[n].dereference()\n'
                  ' data.append(dict(id=n,level=int(p["isUniq"]),rank=int(p["itemLevel"]),type=int(p["itemType"]),'
                  'min=int(p["minDamage"]),max=int(p["maxDamage"]),magic_min=int(p["minMana"]),magic_max=int(p["maxMana"])))\n'
                  'print("ITEMS="+json.dumps(data))\nend')
        memory = json.loads(next(l[6:] for l in gdb(script).splitlines() if l.startswith('ITEMS=')))
        assert len(memory) == len(expected) == 63
        for m, d in zip(memory, expected):
            assert all(m[k] == d[k] for k in m), (m, d)
        passed('live server: 63 천상의 날개 definitions, Rank 10, planned attack/magic per level')
        for item, slotname, (x, y) in WEAPONS:
            account = 'qc'+uuid.uuid4().hex[:10]
            assert sql(f"SELECT COUNT(*) FROM UserTable WHERE id='{account}'") == '0'
            sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill,isNewChar) "
                f"VALUES('{account}','{account}',3,248,264,500,100,500,100,1,10,1,0,201,0)")
            accounts.append(account)
            p = Player(account)
            players.append(p)
            admin(account, True)
            cells = shelf(p.command('/재뽕 창고 천상'))
            page = {i: (cx, cy) for i, cat, cx, cy in cells if cat == 0 and i in (19260, 19290, 19320)}
            assert page == {19260: (2, 0), 19290: (2, 1), 19320: (4, 1)}, cells
            events = p.packet(69, struct.pack('<HiH', 250, item, 1))
            ident = next((struct.unpack_from('<i', d)[0] for t, d in events if t == 43 and struct.unpack_from('<i', d, 4)[0] == item), None)
            assert ident, messages(events)
            admin(account, False)
            p.packet(111, struct.pack('<iB', ident, 0))
            assert equip(account).startswith(f'{item}\t'), (item, equip(account))
            admin(account, True)
            for level in (1, 10, 20):
                text = messages(p.command(f'/재뽕 {slotname} {level}'))
                assert any('[GM 완료]' in s for s in text), (item, level, text)
                assert equip(account).startswith(f'{item+level}\t'), (item, level, equip(account))
            saved = equip(account)
            p.close()
            players.remove(p)
            time.sleep(.8)
            p = Player(account)
            players.append(p)
            assert equip(account) == saved
            p.close()
            players.remove(p)
            passed(f'{item}: warehouse page slot, shelf copy, equipped by a level-1 character, GM +1/+10/+20, +20 kept after relog')
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
    (ROOT/'assets/golden-knight/live-validation.json').write_text(
        json.dumps(dict(passed=True, checks=checks, save_data_preserved=True, in_game_visual_test=False), ensure_ascii=False, indent=2)+'\n',
        encoding='utf-8')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--runtime', type=Path, required=True)
    main(parser.parse_args().runtime.resolve())
