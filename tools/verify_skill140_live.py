"""Real local server: 139/140 learning, old-card merges, multi-hit/AoE packets."""
from pathlib import Path
import sys,json,struct,time,uuid,collections,socket
R=Path(__file__).resolve().parents[1];O=R/'assets/skills140'
sys.path[:0]=[str(R/'runtime'),str(R/'runtime/pylibs'),str(R/'tools')]
from ssh_vm import connect
from local_control import command
from verify_stacks_live import Player
from protocol_probe import send
from build_skill140 import SPECS

def main():
 c=connect();account='q140'+uuid.uuid4().hex[:8];p=None;checks=[]
 def cmd(s):return command(c,s)
 def sql(s):
  i,o,e=c.exec_command('mariadb -N -B LAQIA_GAMEDB');i.write(s+';\n');i.channel.shutdown_write();out=o.read();err=e.read();assert o.channel.recv_exit_status()==0,err.decode();return out.decode().strip()
 def gdb(s):
  pid=cmd('systemctl show -p MainPID --value LAQIA_GameServer').strip()
  with c.open_sftp() as f:
   with f.open('/tmp/skill140-check.gdb','w') as out:out.write('set pagination off\nset confirm off\n'+s+'\ndetach\nquit\n')
  result=cmd('gdb -q -batch -p '+pid+' -x /tmp/skill140-check.gdb 2>&1')
  assert 'Error in sourced command file' not in result,result;return result
 def passed(s):checks.append(s);print('PASS '+s,flush=True)
 digest='mariadb-dump --no-create-info --skip-comments --compact LAQIA_GAMEDB UserTable InvenItems EquipItems UserSkills UserCashMoney | sha256sum'
 before=cmd(digest).split()[0]
 assert not cmd("ss -Htn state established 'sport = :2560'").strip()
 try:
  out=gdb('''python
import gdb,json
rows=[]
for n in range(19120,19124):
 p=gdb.parse_and_eval('g_SKILL[g_ItemInfo[%d]->skillIdx]'%n).dereference()
 rows.append(dict(item=n,reqr=int(p['needLAttack']),reqb=int(p['needBMagic']),area=int(p['areaRange']),attr=int(p['skillAttr']),cool=int(p['coolTime']),damage=[int(p['minDamage']),int(p['maxDamage'])],callback=str(p['func'])))
print('SKILL140='+json.dumps(rows))
end''')
  memory=json.loads(next(l.split('=',1)[1] for l in out.splitlines() if l.startswith('SKILL140=')))
  for i,v in enumerate(memory):
   assert v['reqr']==(140 if i<2 else 0) and v['reqb']==(140 if i>=2 else 0) and v['area']==[5,0,6,6][i] and 'SKILL_Level140Attack' in v['callback'],v
   assert v['damage']==SPECS[i]['damage'],v
  passed('all four live skill definitions, exact damage ranges, learn requirements, area and callbacks')
  sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill,isNewChar,skill_level1,skill_level2) "
   f"VALUES('{account}','{account}',3,266,272,30000,30000,30000,30000,100,100,100,0,10102,0,139,139)")
  seeds=[(10193,0),(10193,0),(10194,0),(10194,0),(10193,998),(10193,5)]+[(n,0) for n in range(19120,19124)]
  sql('INSERT INTO InvenItems(itemNum,invenPage,invenX,invenY,ownerID,ownerPos,life,exValue1) VALUES '+','.join(f"({n},0,{i%5},{i//5},'{account}',0,100,{q})" for i,(n,q) in enumerate(seeds)))
  p=Player(account)
  for n in [10193,10194]:
   ids=[k for k,v in p.items.items() if v['num']==n and v['qty'] in [0,1]];assert len(ids)==2,p.items
   dst=p.items[ids[1]];p.packet(45,struct.pack('<iBBB',ids[0],dst['page'],dst['x'],dst['y']))
   assert ids[0] not in p.items and p.items[ids[1]]['qty']==2
  passed('legacy zero-count weapon/armour singleton cards merge 1+1 into 2')
  src=p.select(10193,5);dst=p.items[p.select(10193,998)];p.packet(45,struct.pack('<iBBB',src,dst['page'],dst['x'],dst['y']))
  assert p.quantities(10193)==[2,4,999];passed('card 998+5 retains 999 and overflow 4')
  for n in range(19120,19124):
   item=p.select(n);events=p.packet(111,struct.pack('<iB',item,0));assert item in p.items and not any(t==112 for t,d in events)
  assert sql(f"SELECT COUNT(*) FROM UserSkills WHERE ownerID='{account}'")=='0';passed('139 cannot learn any of the four skills; scrolls retained')
  gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->ch.skill[1]=140\nset $qa->ch.skill[2]=140')
  for n in range(19120,19124):
   item=p.select(n);events=p.packet(111,struct.pack('<iB',item,0));assert item not in p.items and any(t==112 and struct.unpack('<H',d)[0]==n for t,d in events)
  passed('140 learns all four through real scroll-use packets')
  gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin=1\nset $qa->adminLevel=3')
  for branch,want in [('활',[19120,19121]),('흑마법',[19122,19123])]:
   events=p.command('/재뽕 창고 '+branch);data=next(d for t,d in events if t==67)
   entries=[struct.unpack_from('<iBBB',data,1+i*7) for i in range(data[0])]
   assert all(n in [r[0] for r in entries] for n in want),(branch,entries)
   slots=[r[1:] for r in entries if r[0]>0];assert len(slots)==len(set(slots)),entries
  gdb(f'set $qa=FindPlayerIdList("{account}")\nset $qa->isAdmin=0\nset $qa->adminLevel=0')
  passed('all four scrolls appear in the existing GM bow/black-magic shelves without overlapping slots')
  # Spawn disposable high-HP targets only while the debugger stops the AI.
  # Each cast goes through its real server callback and sends normal damage packets.
  results=[]
  for index,(skill,hits,area) in enumerate([(19120,3,5),(19121,6,0),(19122,7,6),(19123,3,6)]):
   p.drain(.2);script=f'set $qa=FindPlayerIdList("{account}")\nset $qa->ch.useSkill={10102 if index<2 else skill}\n'
   for j,x in enumerate([268,273,274,275]):
    script+=f'call CreateMob(1,3,{x},272,0,"skill140-fixture",1)\nset $m{j}=gMOBS.list\nset $m{j}->hp=1000000\n'
   script+='printf "MOBS=%d,%d,%d,%d\\n",$m0->idxNum,$m1->idxNum,$m2->idxNum,$m3->idxNum\n'
   script+=f'call SKILL_Level140Attack($qa,0,$m0,1,g_SKILL[g_ItemInfo[{skill}]->skillIdx])\n'
   script+='printf "HP=%d,%d,%d,%d\\n",$m0->hp,$m1->hp,$m2->hp,$m3->hp\ncall stop_fighting_player($qa)\n'
   for j in range(4):script+=f'call DelMobData($m{j},"skill140-fixture",1)\n'
   out=gdb(script);ids=list(map(int,next(l.split('=',1)[1] for l in out.splitlines() if l.startswith('MOBS=')).split(',')))
   hp=list(map(int,next(l.split('=',1)[1] for l in out.splitlines() if l.startswith('HP=')).split(',')))
   events=p.drain(.7);counts=collections.Counter(struct.unpack_from('<H',d)[0] for t,d in events if t==20)
   actual=[counts[n] for n in ids];want=[hits,hits if area>=5 else 0,hits if area>=6 else 0,0]
   assert actual==want,(skill,actual,want,out);assert all((h<1000000)==bool(w) for h,w in zip(hp,want)),(skill,hp,want)
   results.append(dict(skill=skill,target_and_radius_5_6_7_hits=actual));passed(f'{skill}: actual hit packets / radius 0,5,6,7 = {actual}')
  p.close();p=None;time.sleep(1);p=Player(account)
  learned=next(d for t,d in p.login_packets if t==113);ids=[struct.unpack_from('<H',learned,1+i*2)[0] for i in range(learned[0])]
  assert all(n in ids for n in range(19120,19124));assert p.quantities(10193)==[2,4,999] and p.quantities(10194)==[2]
  passed('learned skills and merged cards persist after relog')
 finally:
  if p:p.close();time.sleep(1)
  for table in ['InvenItems','EquipItems','UserSkills','UserCashMoney']:sql(f"DELETE FROM {table} WHERE ownerID='{account}'")
  sql(f"DELETE FROM UserTable WHERE id='{account}'")
  after=cmd(digest).split()[0];assert before==after,'Existing save data changed';c.close()
 passed('all five existing save tables preserved; fixture removed')
 (O/'live-validation.json').write_text(json.dumps(dict(passed=True,checks=checks,skills=memory,hit_tests=results,save_data_preserved=True),ensure_ascii=False,indent=2),encoding='utf-8')
if __name__=='__main__':
 import manage
 for port in [22222,2560,44444]:
  with socket.socket() as sock:
   sock.settimeout(.2)
   if sock.connect_ex(('127.0.0.1',port))==0:raise RuntimeError('A local server is already running')
 ctl=manage.control();ctl.start()
 try:main()
 finally:
  if ctl.online() and not ctl.client_running():ctl.stop()
