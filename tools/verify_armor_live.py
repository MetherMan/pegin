"""Exercise ordinary +18/+19 armour rolls on a disposable local character.

Uses the local SSH connection and existing protocol probe. Never grants GM to
the fixture, changes probability, or touches a user's character/items.
"""
from pathlib import Path
import hashlib,json,shlex,struct,sys,time,uuid
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'runtime'),str(ROOT/'runtime/pylibs')]
from ssh_vm import connect
from local_control import command
from gm_probe_base import GMClient
from protocol_probe import send

def main():
    account='qa'+uuid.uuid4().hex[:10]
    c=connect()
    def sql(text):
        stdin,out,err=c.exec_command('mariadb --batch --skip-column-names LAQIA_GAMEDB')
        stdin.write(text+';\n');stdin.channel.shutdown_write()
        result=out.read().decode();error=err.read().decode()
        if out.channel.recv_exit_status():raise RuntimeError(error)
        return result
    rows={int(p[0]):p for line in (ROOT/'game-data/DATA/ITEM_DATA.txt').read_text(encoding='cp949').splitlines()
          if line and line[0].isdigit() for p in [line.split('\t')]}
    transitions=[n for n,p in rows.items() if 19<=int(p[4])<=37 and int(p[1]) in (18,19)]
    for n in transitions:
        assert n+1 in rows and rows[n+1][4]==rows[n][4] and int(rows[n+1][1])==int(rows[n][1])+1
    cases=[]
    fixture=None
    try:
        sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill) "
            f"VALUES('{account}','{account}',3,248,264,500,100,500,100,20,20,20,0,201)")
        for kind in (19,21,23):
            for level in (18,19):
                item=next(n for n,p in rows.items() if int(p[4])==kind and int(p[1])==level)
                successes=attempts=0
                while not successes and attempts<200:
                    sql(f"DELETE FROM InvenItems WHERE ownerID='{account}'")
                    values=[]
                    for j in range(20):
                        for page,num in ((0,item),(1,10194)):
                            values.append(f"({num},{page},{j%5},{j//5},'{account}',0,100,{999 if page else 1})")
                    sql('INSERT INTO InvenItems(itemNum,invenPage,invenX,invenY,ownerID,ownerPos,life,exValue1) VALUES '+','.join(values))
                    fixture=GMClient(account)
                    denial=fixture.command('/gm help',.1)
                    assert any('운영자 전용' in data.decode('cp949',errors='ignore') for _,data in denial), 'Fixture must not be a GM'
                    inv=next(data for tag,data in fixture.login_packets if tag==60)
                    count=struct.unpack_from('<H',inv)[0]
                    entries=[struct.unpack_from('<iHBBBH',inv,2+i*11) for i in range(count)]
                    targets=[e[0] for e in entries if e[1]==item]
                    cards=[e[0] for e in entries if e[1]==10194]
                    assert len(targets)==len(cards)==20
                    for target,card in zip(targets,cards):
                        send(fixture.sock,145,struct.pack('<ii',target,card))
                        result=fixture.drain(.35)
                        assert any(tag==146 for tag,_ in result), ('No enchant result',target,card,[(t,d.hex()) for t,d in result])
                        assert any(tag==115 and struct.unpack('<iH',data)==(card,998) for tag,data in result), 'Exactly one card must be consumed'
                        attempts+=1
                        successes+=sum(tag==43 and struct.unpack_from('<i',data,4)[0]==item+1 for tag,data in result)
                    fixture.close();fixture=None;time.sleep(.7)
                case=dict(type=kind,item=item,name=rows[item][2],from_level=level,to_level=level+1,attempts=attempts,successes=successes)
                cases.append(case);print(json.dumps(case,ensure_ascii=False),flush=True)
                assert successes>0,case
        # The +20 boundary must reject without consuming either object.
        capped=next(n for n,p in rows.items() if int(p[4])==19 and int(p[1])==20)
        sql(f"DELETE FROM InvenItems WHERE ownerID='{account}'")
        sql(f"INSERT INTO InvenItems(itemNum,invenPage,invenX,invenY,ownerID,ownerPos,life,exValue1) VALUES ({capped},0,0,0,'{account}',0,100,1),(10194,0,1,0,'{account}',0,100,1)")
        fixture=GMClient(account)
        inv=next(data for tag,data in fixture.login_packets if tag==60)
        entries=[struct.unpack_from('<iHBBBH',inv,2+i*11) for i in range(struct.unpack_from('<H',inv)[0])]
        send(fixture.sock,145,struct.pack('<ii',next(e[0] for e in entries if e[1]==capped),next(e[0] for e in entries if e[1]==10194)))
        assert not any(tag in (43,44,146) for tag,_ in fixture.drain(.3))
        fixture.close();fixture=None;time.sleep(.7)
        assert int(sql(f"SELECT COUNT(*) FROM InvenItems WHERE ownerID='{account}'").strip())==2
        report=dict(passed=True,fixture_is_non_admin=True,no_probability_override=True,valid_high_armour_transitions=len(transitions),cases=cases,cap_preserves_item_and_card=True,server_sha256=hashlib.sha256((ROOT/'server-bin/LAQIA_GameServer').read_bytes()).hexdigest())
        out=ROOT/'tests/results';out.mkdir(exist_ok=True)
        (out/'armor-live.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    finally:
        if fixture:fixture.close();time.sleep(1)
        # Delete only the uniquely named fixture from tables that store accounts.
        for table in ('InvenItems','EquipItems','UserTable'):
            field='id' if table=='UserTable' else 'ownerID'
            sql(f"DELETE FROM {table} WHERE {field}='{account}'")
        c.close()

if __name__=='__main__':main()
