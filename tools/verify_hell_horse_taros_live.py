"""Exercise mounts and both portal directions on a disposable local player."""
from pathlib import Path
import argparse, json, struct, sys, time, uuid
ROOT=Path(__file__).resolve().parents[1]

def main(runtime):
    sys.path[:0]=[str(runtime),str(runtime/'pylibs'),str(ROOT/'tools')]
    import local_control as ctl
    from ssh_vm import connect
    from verify_stacks_live import Player
    from protocol_probe import send
    assert Path(ctl.__file__).resolve().parent==runtime.resolve()
    c=connect();account='qh'+uuid.uuid4().hex[:10];p=None;checks=[]
    cmd=lambda s:ctl.command(c,s)
    def sql(s):
        i,o,e=c.exec_command('mariadb -N -B LAQIA_GAMEDB');i.write(s+';\n');i.channel.shutdown_write()
        result=o.read();error=e.read();assert o.channel.recv_exit_status()==0,error.decode();return result.decode().strip()
    def gdb(s):
        pid=cmd('systemctl show -p MainPID --value LAQIA_GameServer').strip()
        with c.open_sftp() as f:
            with f.open('/tmp/hell-horse-taros-check.gdb','w') as out:out.write('set pagination off\nset confirm off\n'+s+'\ndetach\nquit\n')
        result=cmd('gdb -q -batch -p '+pid+' -x /tmp/hell-horse-taros-check.gdb 2>&1')
        assert 'Error in sourced command file' not in result,result;return result
    def passed(s):checks.append(s);print('PASS '+s,flush=True)
    def ride():
        out=gdb(f'set $qa=FindPlayerIdList("{account}")\nprintf "RIDE=%d,%d,%d\\n",$qa->ch2.RideNum,$qa->ch2.isRide,GetRideSpeed($qa)')
        return list(map(int,next(l[5:] for l in out.splitlines() if l.startswith('RIDE=')).split(',')))
    def location():
        out=gdb(f'set $qa=FindPlayerIdList("{account}")\nprintf "POS=%d,%d,%d\\n",$qa->ch.mapNum,$qa->ch.posX,$qa->ch.posY')
        return list(map(int,next(l[4:] for l in out.splitlines() if l.startswith('POS=')).split(',')))
    def warp(mapnum,x,y):
        p.drain(.1)
        gdb(f'set $qa=FindPlayerIdList("{account}")\ncall PlayerFromMap($qa)\nset $qa->ch2.gotoMapNum={mapnum}\nset $qa->ch2.gotoPosX={x}\nset $qa->ch2.gotoPosY={y}\nset $qa->ch2.gotoPosMapKind=0\ncall SendMapLoading($qa,0)')
        events=p.drain(.4);assert any(t==5 and struct.unpack_from('<H',d)[0]==mapnum for t,d in events),events
        p.packet(6);assert location()==[mapnum,x,y]
    digest='mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems UserSkills UserCashMoney | sha256sum'
    assert not cmd("ss -Htn state established 'sport = :2560'").strip()
    before=cmd(digest).split()[0]
    try:
        out=gdb('''python
import gdb,json
rows=[]
for i in range(1700):
 p=gdb.parse_and_eval('gWORLD[%d]'%i)
 if int(p):
  w=p.dereference();name=bytes(gdb.selected_inferior().read_memory(w['mapName'].address,64)).split(bytes([0]))[0].decode('cp949')
  rows.append(dict(id=i,reset_count=int(w['mobResetCnt']),name=name))
print('WORLDS='+json.dumps(rows))
end''')
        worlds=json.loads(next(l[7:] for l in out.splitlines() if l.startswith('WORLDS=')))
        baseline=json.loads((ROOT/'assets/world-audit/live-validation.json').read_text())['worlds']
        for old in baseline:
            new=next(w for w in worlds if w['id']==old['id']);assert new['reset_count']==old['reset_count'],(old,new)
        fresh=next(w for w in worlds if w['id']==41)
        blueprint=json.loads((ROOT/'assets/forgotten-taros/spawn-blueprint.json').read_text(encoding='utf-8'))
        assert fresh['name']=='잊혀진 타로스' and fresh['reset_count']==2*blueprint['enabled_spawn_count'],fresh
        passed('all 32 previous maps retain native spawn counts; independent map 41 has the full name and the configured custom spawns')
        sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill,isNewChar) "
            f"VALUES('{account}','{account}',3,266,272,30000,30000,30000,30000,100,100,100,0,201,0)")
        sql('INSERT INTO InvenItems(itemNum,invenPage,invenX,invenY,ownerID,ownerPos,life,exValue1) VALUES '+','.join(f"({n},0,{i},0,'{account}',0,100,0)" for i,n in enumerate([10188,10189,10190,19130])))
        p=Player(account)
        for item,kind,speed in [(10188,1,70),(10189,2,75),(10190,3,80),(19130,4,95)]:
            serial=p.select(item);assert serial
            p.packet(111,struct.pack('<iB',serial,0));assert ride()==[kind,1,speed],(item,ride())
            p.packet(111,struct.pack('<iB',serial,0));assert ride()==[0,0,40]
        passed('real item-use packets mount and dismount all horses: 70 / 75 / 80 / 95; walking speed remains 40')
        p.packet(111,struct.pack('<iB',p.select(19130),0));p.close();p=None;time.sleep(1)
        p=Player(account);assert ride()==[4,1,95]
        passed('hell horse remains mounted at speed 95 after relog')
        transitions=[]
        for src,x,y,dst in [(14,448,264,41),(41,448,264,14),(41,60,265,14)]:
            warp(src,x,y);p.drain(.1)
            gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->ch2.endMapLoading=0\ncall CheckLoadingPoint($qa)')
            events=p.drain(.4);data=next(d for t,d in events if t==5)
            assert struct.unpack_from('<Hii',data)==(dst,443,264),(src,data.hex())
            p.packet(6);assert location()==[dst,443,264];assert ride()==[4,1,95]
            # The landing coordinate lies outside the return trigger.
            gdb(f'set $qa=FindPlayerIdList("{account}")\ncall CheckLoadingPoint($qa)')
            assert not any(t==5 for t,d in p.drain(.3))
            transitions.append([src,x,y,dst,443,264])
        passed('wreck entrance and both return landmarks emit real map-load packets; no immediate portal bounce; mount retained')
        warp(41,443,264);p.close();p=None;time.sleep(1);p=Player(account)
        assert location()==[41,443,264] and ride()==[4,1,95]
        passed('relogin inside Forgotten Taros restores map, position and hell horse')
        gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin=1\nset $qa->adminLevel=3')
        events=p.command('/재뽕 말 지옥마');assert any(t==43 and struct.unpack_from('<i',d,4)[0]==19130 for t,d in events)
        passed('GM hell-horse alias grants the new certificate')
    finally:
        if p:p.close();time.sleep(1)
        for table in ['InvenItems','EquipItems','UserSkills','UserCashMoney']:sql(f"DELETE FROM {table} WHERE ownerID='{account}'")
        sql(f"DELETE FROM UserTable WHERE id='{account}'")
        assert before==cmd(digest).split()[0],'Existing save data changed'
        c.close()
    passed('existing five save tables unchanged; disposable player removed')
    out=ROOT/'assets/forgotten-taros/live-validation.json'
    out.write_text(json.dumps(dict(passed=True,checks=checks,worlds=worlds,transitions=transitions,save_data_preserved=True),ensure_ascii=False,indent=2)+'\n',encoding='utf-8')

if __name__=='__main__':
    parser=argparse.ArgumentParser();parser.add_argument('--runtime',type=Path,required=True)
    main(parser.parse_args().runtime.resolve())
