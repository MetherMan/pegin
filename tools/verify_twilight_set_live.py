"""New weapon integration on uniquely named disposable local characters."""
from pathlib import Path
import sys,json,struct,time,uuid,hashlib
ROOT=Path(__file__).resolve().parents[1];O=ROOT/'assets/twilight-set'
sys.path[:0]=[str(ROOT/'runtime'),str(ROOT/'runtime/pylibs')]
from ssh_vm import connect
from local_control import command
from gm_probe_base import GMClient
from protocol_probe import send

def main():
    conn=connect();player=None;account=None;checks=[]
    def cmd(s):return command(conn,s)
    def sql(s):
        i,o,e=conn.exec_command('mariadb -N -B LAQIA_GAMEDB');i.write(s+';\n');i.channel.shutdown_write();out=o.read();err=e.read();assert o.channel.recv_exit_status()==0,err.decode(errors='replace');return out.decode().strip()
    def gdb(s):
        pid=cmd('systemctl show -p MainPID --value LAQIA_GameServer').strip()
        with conn.open_sftp() as f:
            with f.open('/tmp/twilight-set-check.gdb','w') as out:out.write('set pagination off\nset confirm off\n'+s+'\ndetach\nquit\n')
        return cmd('gdb -q -batch -p '+pid+' -x /tmp/twilight-set-check.gdb 2>&1')
    def messages(events):return [d[2:2+struct.unpack_from('<H',d)[0]].decode('cp949') for t,d in events if t==150]
    def passed(name):checks.append(name);print('PASS '+name,flush=True)
    def equip():return sql(f"SELECT itemNum,equipPos,serial FROM EquipItems WHERE ownerID='{account}' ORDER BY equipPos")
    def admin(enabled):gdb(f'set $qa = FindPlayerIdList("{account}")\nset $qa->isAdmin={int(enabled)}\nset $qa->adminLevel={3 if enabled else 0}')
    assert not cmd("ss -Htn state established 'sport = :2560'").strip(),'Close all clients first'
    before=cmd('mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems | sha256sum').split()[0]
    try:
        script='''python
import gdb,json
data=[]
for n in list(range(19060,19081))+list(range(19090,19111)):
 p=gdb.parse_and_eval('g_ItemInfo')[n].dereference()
 data.append(dict(id=n,level=int(p['isUniq']),rank=int(p['itemLevel']),type=int(p['itemType']),min=int(p['minDamage']),max=int(p['maxDamage']),magic_min=int(p['minMana']),magic_max=int(p['maxMana']),weight=int(p['weight'])))
print('SET_MEMORY='+json.dumps(data))
end'''
        out=gdb(script);line=next(l for l in out.splitlines() if l.startswith('SET_MEMORY='));memory=json.loads(line.split('=',1)[1])
        expected=json.loads((O/'item_definitions.json').read_text(encoding='utf-8'))
        assert len(memory)==42
        for actual,want in zip(memory,expected):assert all(actual[k]==want[k] for k in actual),(actual,want)
        passed('all 42 physical/magic enhancement definitions loaded by live server')
        for alias,base,slotname in [(3,19060,'활'),(4,19090,'무기')]:
            account='qt'+uuid.uuid4().hex[:10]
            assert sql(f"SELECT COUNT(*) FROM UserTable WHERE id='{account}'")=='0'
            sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill,isNewChar) "
                f"VALUES('{account}','{account}',3,248,264,500,100,500,100,1,10,1,0,201,0)")
            player=GMClient(account)
            assert any('운영자 전용' in s for s in messages(player.command(f'/재뽕 테스트 {alias}')))
            admin(True);events=player.command(f'/재뽕 테스트 {alias}');ids=[struct.unpack_from('<i',d)[0] for tag,d in events if tag==43 and struct.unpack_from('<i',d,4)[0]==base]
            assert len(ids)==1,(alias,messages(events));passed(f'test {alias} grants exactly one item, admin-only')
            admin(False);send(player.sock,111,struct.pack('<iB',ids[0],0));player.drain(.7)
            assert equip().startswith(str(base)+'\t'),(base,equip());passed(f'{base} equips on a normal character without stat restrictions')
            admin(True)
            for level in [1,10,20,0,20]:
                text=messages(player.command(f'/재뽕 {slotname} {level}'));assert any('[GM 완료]' in s for s in text),(base,level,text)
                assert equip().startswith(str(base+level)+'\t'),(base,level,equip())
            saved=equip();player.close();player=None;time.sleep(.8);player=GMClient(account);assert equip()==saved
            data=next(d for tag,d in player.login_packets if tag==60);count=struct.unpack_from('<H',data)[0];at=2+count*11;eqcount=data[at];at+=1
            equipped=[struct.unpack_from('<iH',data,at+i*6)[1] for i in range(eqcount)];assert base+20 in equipped
            passed(f'{base} 0/1/10/20 enhancement and equipped +20 relog persistence')
            player.close();player=None;time.sleep(.8)
            for table in ['InvenItems','EquipItems','UserCashMoney','UserSkills']:sql(f"DELETE FROM {table} WHERE ownerID='{account}'")
            sql(f"DELETE FROM UserTable WHERE id='{account}'");account=None
    finally:
        if player:player.close();time.sleep(1)
        if account:
            for table in ['InvenItems','EquipItems','UserCashMoney','UserSkills']:sql(f"DELETE FROM {table} WHERE ownerID='{account}'")
            sql(f"DELETE FROM UserTable WHERE id='{account}'")
        after=cmd('mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems | sha256sum').split()[0]
        assert before==after,'Existing character or equipment records changed'
        conn.close()
    passed('temporary characters removed; existing character/inventory/equipment hash unchanged')
    report=dict(passed=True,checks=checks,definitions=memory,existing_player_data_preserved=True,server_sha256=hashlib.sha256((ROOT/'server-bin/LAQIA_GameServer').read_bytes()).hexdigest(),in_game_visual_test=False)
    (O/'live-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')

if __name__=='__main__':main()
