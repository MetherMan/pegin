"""Clone Taros as map 41; keep original spawn coordinates for future custom mobs."""
from pathlib import Path
import argparse, collections, hashlib, json, re, shutil, struct

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'assets/forgotten-taros'
MAP_ID = 41

def entities(raw):
    size = struct.unpack_from('<I', raw, 4)[0]
    at = 40 + 252 * size * size
    count = struct.unpack_from('<I', raw, at)[0]
    return at, [raw[at + 4 + i * 48:at + 4 + (i + 1) * 48] for i in range(count)]

def build(server, client):
    OUT.mkdir(parents=True, exist_ok=True)
    mapping_file=OUT/'custom-monsters.json'
    mapping={int(k):int(v) for k,v in json.loads(mapping_file.read_text(encoding='utf-8')).items()} if mapping_file.exists() else {}
    dest = ROOT / 'client-overlay/Map'
    dest.mkdir(exist_ok=True)
    client_raw = (client / 'Map/land0014.map').read_bytes()
    raw = (server / 'MAP/land0014.map').read_bytes()
    at, objects = entities(raw)
    props, spawns = [], []
    for obj in objects:
        name = obj[:32].split(b'\0')[0].decode('cp949')
        x, y, z, yaw = struct.unpack_from('<4f', obj, 32)
        mob = re.fullmatch(r'MOB_(\d+)\.mod', name, re.I)
        if mob:
            custom=mapping.get(int(mob[1]))
            spawns.append(dict(source_mob=int(mob[1]), x=x, y=y, z=z, yaw=yaw,
                               custom_mob=custom))
            if custom is not None:
                props.append(f'MOB_{custom}.mod'.encode().ljust(32,b'\0')+obj[32:])
        elif not name.lower().startswith('cn_'):
            # New zone has its own mobs and no duplicate original town NPCs.
            props.append(obj)
    # Retain both portal landmarks; both return to the original wreck entrance.
    new = raw[:at] + struct.pack('<I', len(props)) + b''.join(props) + raw[at + 4 + len(objects) * 48:]
    client_at, client_objects = entities(client_raw)
    client_props = [o for o in client_objects if not o[:32].lower().startswith((b'cn_',b'mob_'))]
    client_new = client_raw[:client_at]+struct.pack('<I',len(client_props))+b''.join(client_props)+client_raw[client_at+4+len(client_objects)*48:]
    (dest / 'land0041.map').write_bytes(client_new)
    (ROOT / 'game-data/DATA/LocalWorld_land0041.map').write_bytes(new)
    for src, target in [('title0014.wtm','title0041.wtm'), ('land0014.door','land0041.door'), ('land0014.wtm','land0041.wtm'),
                        ('landb0014.wtm','landb0041.wtm'), ('loading0014_0800.wtm','loading0041_0800.wtm'),
                        ('loading0014_1024.wtm','loading0041_1024.wtm')]:
        shutil.copy2(client / 'Map' / src, dest / target)
    ini = (client / 'Map/land0014.ini').read_bytes().decode('cp949')
    ini = re.sub(r'(?m)^map_name[^\r\n]*', 'map_name 잊혀진 타로스', ini)
    ini = re.sub(r'(?m)^area_warcastle[^\r\n]*[\r\n]*', '', ini)
    (dest / 'land0041.ini').write_bytes(ini.encode('cp949'))
    (dest / 'land0041.mds').write_bytes(b'; Forgotten Taros\r\nWeather: 3\r\nEnd\r\n')
    # Preserve the separate region's title and permanent-night presentation.
    from build_hell_horse_taros_details import build_map_presentation
    build_map_presentation(client)
    # Arrival is outside the trigger cross so one step cannot bounce the player back.
    portals = [(14,448,264,41,443,264,0), (41,448,264,14,443,264,0),
               (41,60,265,14,443,264,0)]
    size = struct.unpack_from('<I', raw, 4)[0]
    for x,y in [(448,264),(443,264),(60,265)]:
        assert client_raw[40+73*size*size+y*size*4+x] == 0, ('blocked client portal',x,y)
    for name in ['map_list.txt','map_name.txt','loadingpoint.txt']:
        original = (server / 'MAP' / name).read_bytes().decode('cp949')
        if name == 'map_list.txt':
            assert not re.search(r'^41\s*$', original, re.M)
            text = re.sub(r'(?m)^#', '41\r\n#', original, count=1)
            assert text != original
        elif name == 'map_name.txt':
            text = re.sub(r'(?m)^#', '41\t잊혀진 타로스\r\n#', original, count=1)
            assert text != original
        else:
            text = original.rstrip()+'\r\n'+'\r\n'.join('\t'.join(map(str,row)) for row in portals)+'\r\n'
        (ROOT / 'game-data/DATA' / ('LocalWorld_'+name)).write_bytes(text.encode('cp949'))
    report = dict(map_id=MAP_ID, name='잊혀진 타로스', original_map=14, portals=portals,
                  spawn_count=len(spawns), source_groups=dict(collections.Counter(s['source_mob'] for s in spawns)),
                  enabled_spawn_count=sum(s['custom_mob'] is not None for s in spawns), spawns=spawns, source_sha256=hashlib.sha256(raw).hexdigest(),
                  map_sha256=hashlib.sha256(new).hexdigest())
    (OUT / 'spawn-blueprint.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k!='spawns'},ensure_ascii=False))

if __name__ == '__main__':
    p=argparse.ArgumentParser();p.add_argument('--server',type=Path,required=True);p.add_argument('--client',type=Path,required=True)
    a=p.parse_args();build(a.server,a.client)
