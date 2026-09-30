"""Exercise the Blue Moon horse on a disposable local player with real packets.

Mount/dismount all five certificates (70/75/80/100/120), relog while mounted, GM grant aliases and the
GM warehouse listing. Deletes only the fixture rows and checks the five save tables are unchanged.
"""
from pathlib import Path
import argparse, json, struct, sys, time, uuid
ROOT = Path(__file__).resolve().parents[1]
HORSES = [(10188, 1, 70), (10189, 2, 75), (10190, 3, 80), (19130, 4, 100), (19131, 5, 120)]


def main(runtime):
    sys.path[:0] = [str(runtime), str(runtime/'pylibs'), str(ROOT/'tools')]
    import local_control as ctl
    from ssh_vm import connect
    from verify_stacks_live import Player
    assert Path(ctl.__file__).resolve().parent == runtime.resolve()
    c = connect()
    account = 'qb'+uuid.uuid4().hex[:10]
    p = None
    checks = []
    cmd = lambda s: ctl.command(c, s)

    def sql(s):
        i, o, e = c.exec_command('mariadb -N -B LAQIA_GAMEDB')
        i.write(s+';\n')
        i.channel.shutdown_write()
        result = o.read()
        error = e.read()
        assert o.channel.recv_exit_status() == 0, error.decode()
        return result.decode().strip()

    def gdb(s):
        pid = cmd('systemctl show -p MainPID --value LAQIA_GameServer').strip()
        with c.open_sftp() as f:
            with f.open('/tmp/blue-moon-horse-check.gdb', 'w') as out:
                out.write('set pagination off\nset confirm off\n'+s+'\ndetach\nquit\n')
        result = cmd('gdb -q -batch -p '+pid+' -x /tmp/blue-moon-horse-check.gdb 2>&1')
        assert 'Error in sourced command file' not in result, result
        return result

    def passed(s):
        checks.append(s)
        print('PASS '+s, flush=True)

    def ride():
        out = gdb(f'set $qa=FindPlayerIdList("{account}")\n'
                  'printf "RIDE=%d,%d,%d\\n",$qa->ch2.RideNum,$qa->ch2.isRide,GetRideSpeed($qa)')
        return list(map(int, next(l[5:] for l in out.splitlines() if l.startswith('RIDE=')).split(',')))

    digest = ('mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems '
              'UserSkills UserCashMoney | sha256sum')
    assert not cmd("ss -Htn state established 'sport = :2560'").strip()
    before = cmd(digest).split()[0]
    grants = {}
    try:
        sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill,isNewChar) "
            f"VALUES('{account}','{account}',3,266,272,30000,30000,30000,30000,100,100,100,0,201,0)")
        sql('INSERT INTO InvenItems(itemNum,invenPage,invenX,invenY,ownerID,ownerPos,life,exValue1) VALUES '
            + ','.join(f"({n},0,{i},0,'{account}',0,100,0)" for i, (n, _, _) in enumerate(HORSES)))
        p = Player(account)
        for item, kind, speed in HORSES:
            serial = p.select(item)
            assert serial, item
            p.packet(111, struct.pack('<iB', serial, 0))
            assert ride() == [kind, 1, speed], (item, ride())
            p.packet(111, struct.pack('<iB', serial, 0))
            assert ride() == [0, 0, 40]
        passed('real item-use packets mount and dismount all five horses: 70 / 75 / 80 / 100 / 120; walking stays 40')
        p.packet(111, struct.pack('<iB', p.select(19131), 0))
        p.close()
        p = None
        time.sleep(1)
        p = Player(account)
        assert ride() == [5, 1, 120]
        passed('Blue Moon horse remains mounted at speed 120 after relog')
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin=1\nset $qa->adminLevel=3')
        for alias in ['/재뽕 말 푸른달', '/재뽕 말 5']:
            events = p.command(alias)
            got = [struct.unpack_from('<i', d, 4)[0] for t, d in events if t == 43]
            assert 19131 in got, (alias, got)
            grants[alias] = got
        events = p.command('/재뽕 말 지옥마')
        assert 19130 in [struct.unpack_from('<i', d, 4)[0] for t, d in events if t == 43]
        passed('GM aliases "푸른달" and "5" grant certificate 19131; "지옥마" still grants 19130')
        events = p.command('/재뽕 창고 푸른달의 말')
        listed = [d for t, d in events if struct.pack('<i', 19131) in d and struct.pack('<i', -1700000001) in d]
        assert listed, [(t, d[:24].hex()) for t, d in events]
        events = p.command('/재뽕 창고 소유증서')
        listed_all = [d for t, d in events if struct.pack('<i', -1700000001) in d]
        assert listed_all and all(struct.pack('<i', n) in listed_all[0] for n, _, _ in HORSES)
        passed('GM warehouse lists 푸른달의 말 소유증서 next to the other four horse certificates')
    finally:
        if p:
            p.close()
            time.sleep(1)
        for table in ['InvenItems', 'EquipItems', 'UserSkills', 'UserCashMoney']:
            sql(f"DELETE FROM {table} WHERE ownerID='{account}'")
        sql(f"DELETE FROM UserTable WHERE id='{account}'")
        assert before == cmd(digest).split()[0], 'Existing save data changed'
        c.close()
    passed('existing five save tables unchanged; disposable player removed')
    out = ROOT/'assets/blue-moon-horse/live-validation.json'
    out.write_text(json.dumps(dict(passed=True, checks=checks, horses=HORSES, save_data_preserved=True),
                              ensure_ascii=False, indent=2)+'\n', encoding='utf-8')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--runtime', type=Path, required=True)
    main(parser.parse_args().runtime.resolve())
