"""Native live-server checks, called within verify_skill140_live's disposable account."""
import json,re,inspect
from protocol_probe import crypt,KEY,MASK

def verify(gdb,account,passed):
    results=[]
    for skill in (19122,19123):
        script='''python
import gdb,json,struct
def run(s):return gdb.execute(s,to_string=True)
def val(s):return int(gdb.parse_and_eval(s))
run('set $qa=FindPlayerIdList("ACCOUNT")')
old_clock=val('g_CurrTime');old_drop=gdb.parse_and_eval('g_MOBINFO[1]->inven')
run('set $drop=g_MOBINFO[1]->inven')
mobid=None
def packets():
 size=val('$qa->sendSize');addr=val('&$qa->sendBuff[0]')
 data=bytes(gdb.selected_inferior().read_memory(addr,size));out=[];at=0
 while at<len(data):
  length=struct.unpack_from('<H',data,at)[0]
  assert length>=10 and at+length<=len(data)
  decoded=crypt(data[at+2:at+length],True)
  inner,tag=struct.unpack_from('<HH',decoded)
  payload=decoded[4:inner]
  if tag in (130,23) and struct.unpack_from('<H',payload)[0]==mobid:out.append([tag,payload.hex()])
  at+=length
 return out
try:
 run('set g_MOBINFO[1]->inven=0') # no loot for this disposable fixture
 run('set $qa->ch.useSkill=SKILL')
 run('call CreateMob(1,3,268,272,0,"mage-death-fixture",1)')
 run('set $m=gMOBS.list');mobid=val('$m->idxNum')
 run('set $m->hp=1');run('set $hp=$qa->ch.hp')
 run('set $m->enemy=$qa');run('set $qa->enemyMob=$m')
 run('call SKILL_Level140Attack($qa,0,$m,1,g_SKILL[g_ItemInfo[SKILL]->skillIdx])')
 assert val('$m->hp')<=0 and val('$m->isDead')==1
 assert val('$m->enemy')==0 and val('$qa->enemyMob')==0
 delay=val('$m->deadTimer')-old_clock
 immediate=packets();assert [130,struct.pack('<HB',mobid,2).hex()] in immediate,(immediate,delay,val('$qa->ch.posX'),val('$qa->ch.posY'),val('FindPlayerInArea(3,268/20,272/20,$qa->sock)'))
 assert not any(tag==23 for tag,_ in immediate)
 assert val('GetSkill140DeathHold()')==0
 # Even an explicit stale attack call must not deal damage after the kill.
 run('call AttackToPlayer($m,$qa,0)');assert val('$qa->ch.hp')==val('$hp')
 run('set g_CurrTime=$m->deadTimer-1');run('call MonsterMoveProc()')
 assert val('FindMobInArea(3,266/20,272/20,%d)'%mobid)!=0
 assert not any(tag==23 for tag,_ in packets())
 run('set g_CurrTime=g_CurrTime+2');run('call MonsterMoveProc()')
 assert val('FindMobInArea(3,266/20,272/20,%d)'%mobid)==0
 assert sum(tag==23 for tag,_ in packets())==1
 run('call MonsterMoveProc()');assert sum(tag==23 for tag,_ in packets())==1
 print('DEATH_HOLD='+json.dumps(dict(skill=SKILL,delay_ms=delay,immediate_kill=True,aggro_cleared=True,stale_attack_blocked=True,freeze_packet=True,early_death_packets=0,final_death_packets=1)))
finally:
 run('set g_MOBINFO[1]->inven=$drop')
 run('set g_CurrTime=%d'%old_clock)
 run('call stop_fighting_player($qa)')
 if mobid is not None:
  run('set $remaining=FindMobInArea(3,266/20,272/20,%d)'%mobid)
  if val('$remaining'):run('call DelMobData($remaining,"mage-death-fixture",1)')
end'''.replace('ACCOUNT',account)
        script=re.sub(r'\bSKILL\b',str(skill),script)
        script=script.replace('import gdb,json,struct','import gdb,json,struct\nMASK=%r\nKEY=%r\n'%(MASK,KEY)+inspect.getsource(crypt))
        output=gdb(script)
        record=json.loads(next(line.split('=',1)[1] for line in output.splitlines() if line.startswith('DEATH_HOLD=')))
        results.append(record)
        passed(f'{skill}: immediate kill and freeze, zero further damage, one death animation after {record["delay_ms"]}ms')
    return results
