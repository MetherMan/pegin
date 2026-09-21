"""Local integration test: real purchase/pickup/merge/enchant/storage/relogin.

Creates one uniquely named non-admin fixture and deletes only its DB rows.
Requires the matching 16-bit server/client build to have been installed.
"""
from pathlib import Path
import collections,hashlib,json,struct,sys,time,uuid
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'runtime'),str(ROOT/'runtime/pylibs')]
from ssh_vm import connect
from gm_probe_base import GMClient
from protocol_probe import send

class Player(GMClient):
    def __init__(self,name):
        super().__init__(name)
        self.items={}
        data=next(data for tag,data in self.login_packets if tag==60)
        count=struct.unpack_from('<H',data)[0]
        for i in range(count):
            ident,num,page,x,y,qty=struct.unpack_from('<iHBBBH',data,2+i*11)
            self.items[ident]=dict(num=num,page=page,x=x,y=y,qty=qty)
    def packet(self,tag,data=b''):
        send(self.sock,tag,data);events=self.drain(.4)
        for tag,data in events:
            if tag==43:
                ident,num,page,x,y,qty=struct.unpack('<iiBBBH',data)
                self.items[ident]=dict(num=num,page=page,x=x,y=y,qty=qty)
            elif tag==44:self.items.pop(struct.unpack('<i',data)[0],None)
            elif tag in (115,256):
                ident,qty=struct.unpack('<iH',data);assert ident in self.items
                self.items[ident]['qty']=qty
        return events
    def select(self,num,qty=None):
        return next((ident for ident,v in self.items.items() if v['num']==num and (qty is None or v['qty']==qty)),None)
    def quantities(self,num):return sorted(v['qty'] for v in self.items.values() if v['num']==num)
    def snapshot(self):return sorted((v['num'],v['page'],v['x'],v['y'],v['qty']) for v in self.items.values())

def main():
    account='qs'+uuid.uuid4().hex[:10];c=connect();player=None;checks=[]
    def sql(text):
        stdin,out,err=c.exec_command('mariadb --batch --skip-column-names LAQIA_GAMEDB')
        stdin.write(text+';\n');stdin.channel.shutdown_write()
        result=out.read().decode();error=err.read().decode()
        if out.channel.recv_exit_status():raise RuntimeError(error)
        return result
    def passed(name):checks.append(name);print('PASS '+name,flush=True)
    try:
        sql("INSERT INTO UserTable(id,name,mapNum,posX,posY,money,hp,mp,max_hp,max_mp,str_point,int_point,dex_point,charPos,lastSkill) "
            f"VALUES('{account}','{account}',3,266,272,2000000000,100,50,500,100,20,20,20,0,201)")
        seeds=[(10193,998),(10193,1),(10194,998),(10194,5),(1,0),(3970,0),(10095,9990),(10095,20),(10194,1),(10193,1),(1,0)]
        values=[f"({num},0,{i%5},{i//5},'{account}',0,100,{qty})" for i,(num,qty) in enumerate(seeds)]
        sql('INSERT INTO InvenItems(itemNum,invenPage,invenX,invenY,ownerID,ownerPos,life,exValue1) VALUES '+','.join(values))
        player=Player(account)
        assert player.quantities(10193)==[1,1,998] and player.quantities(10095)==[20,9990]
        passed('initial inventory preserves 16-bit potion/card quantities')
        assert any(tag==67 for tag,_ in player.packet(66,struct.pack('<H',9)))
        events=player.packet(69,struct.pack('<HiH',9,10096,9999))
        assert player.quantities(10096)==[9999],[(t,d.hex()) for t,d in events]
        passed('NPC shop purchase 9999')
        before=player.snapshot();player.packet(69,struct.pack('<HiH',9,10096,10000));assert before==player.snapshot()
        passed('NPC rejects purchase above 9999')
        # A normal single dropped weapon card joins an existing stack at 999.
        singleton=player.select(10193,1)
        events=player.packet(46,struct.pack('<iB',singleton,0))
        dropped=next(struct.unpack_from('<i',data)[0] for tag,data in events if tag==41)
        player.packet(47,struct.pack('<i',dropped));assert player.quantities(10193)==[1,999]
        passed('dropped weapon card pickup stacks to 999')
        singleton=player.select(10194,1)
        events=player.packet(46,struct.pack('<iB',singleton,0))
        dropped=next(struct.unpack_from('<i',data)[0] for tag,data in events if tag==41)
        player.packet(47,struct.pack('<i',dropped));assert player.quantities(10194)==[5,999]
        passed('dropped armour card pickup stacks to 999')
        # Dropping and collecting a whole stack must also keep its entire amount.
        small=player.select(10194,5)
        events=player.packet(46,struct.pack('<iB',small,0))
        dropped=next(struct.unpack_from('<i',data)[0] for tag,data in events if tag==41)
        player.packet(47,struct.pack('<i',dropped));assert player.quantities(10194)==[5,999]
        passed('pickup at card cap preserves the excess in a separate stack')
        # Consume the only weapon card in its stack and remove exactly that object.
        singleton=player.select(10193,1);item=player.select(1)
        player.packet(145,struct.pack('<ii',item,singleton))
        assert singleton not in player.items and player.quantities(10193)==[999]
        passed('consuming last card removes only that stack')
        # Dragging onto an almost-full stack keeps every excess item.
        for num,expected in [(10194,[5,999]),(10095,[11,9999])]:
            source=player.select(num,min(player.quantities(num)));target=player.items[player.select(num,max(player.quantities(num)))]
            player.packet(45,struct.pack('<iBBB',source,target['page'],target['x'],target['y']))
            assert player.quantities(num)==expected,(num,player.quantities(num))
        passed('card 999 and potion 9999 merge caps retain overflow')
        for num,card in [(1,10193),(3970,10194)]:
            card_id=player.select(card,999);item=player.select(num)
            events=player.packet(145,struct.pack('<ii',item,card_id))
            assert any(tag==146 and data==b'\1' for tag,data in events)
            assert player.items[card_id]['qty']==998 and player.select(num+1) is not None
        passed('weapon and armour enchanting consume exactly one card')
        # Potion use decrements a four-digit stack and keeps the same item.
        potion=player.select(10096,9999);player.packet(111,struct.pack('<iB',potion,0))
        assert player.items[potion]['qty']==9998,player.items[potion]
        passed('potion use preserves remaining 9998')
        # The village shop and warehouse are within interaction range.
        for num in [10095,10193]:
            ident=player.select(num,max(player.quantities(num)));qty=player.items[ident]['qty']
            events=player.packet(124,struct.pack('<i',ident))
            entry=next(data for tag,data in events if tag==122)
            item,stored,quantity=struct.unpack('<HiH',entry)
            assert item==num and quantity==qty and ident not in player.items
            events=player.packet(120)
            data=next(data for tag,data in events if tag==121)
            records=[struct.unpack_from('<HiH',data,2+i*8) for i in range(struct.unpack_from('<H',data)[0])]
            assert (num,stored,qty) in records
            player.packet(125,struct.pack('<i',stored));assert qty in player.quantities(num)
        passed('warehouse deposit/list/withdraw preserve large stacks')
        before=player.snapshot();player.close();player=None;time.sleep(1)
        player=Player(account);assert player.snapshot()==before,(before,player.snapshot())
        passed('relogin preserves all quantities, types and positions')
        rows=[r.split('\t') for r in (ROOT/'game-data/DATA/ITEM_DATA.txt').read_text(encoding='cp949').splitlines() if r and r[0].isdigit()]
        potions=[r for r in rows if int(r[4])==44];assert potions and all(float(r[19])==0 for r in potions)
        passed('all potion data weights are zero')
        result=dict(passed=True,checks=checks,potion_limit=9999,card_limit=999,potion_rows=len(potions),server_sha256=hashlib.sha256((ROOT/'server-bin/LAQIA_GameServer').read_bytes()).hexdigest())
        out=ROOT/'tests/results';out.mkdir(exist_ok=True);(out/'stacks-live.json').write_text(json.dumps(result,indent=2)+'\n')
    finally:
        if player:player.close()
        time.sleep(1)
        for table in ['InvenItems','EquipItems','UserGarbage','UserTable']:
            field='id' if table=='UserTable' else 'ownerID'
            sql(f"DELETE FROM {table} WHERE {field}='{account}'")
        c.close()

if __name__=='__main__':main()
