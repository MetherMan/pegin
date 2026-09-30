"""Export full warrior armour sets on the native characters for a side-by-side preview.

Sets are the ITEM_DATA torso/pants/gauntlet/boots rows one to three steps below
Temple Knight, with Temple Knight itself as the reference. Only reads game files;
the output is a browser preview under assets/blue-moon-armor.
"""
from pathlib import Path
import json, struct, sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
from native_actor import model, animation
from native_assets import png_from_wtm
from tuning import resource_reader

O = R/'assets/blue-moon-armor'
PARTS = [('tor', 19, '아머'), ('leg', 20, '팬츠'), ('gun', 31, '건틀렛'), ('boo', 34, '부츠')]
# Base names as listed in ITEM_DATA (enchant 0). step = tiers below Temple Knight.
SETS = [
    ('temple', 0, ['템플나이트 아머', '템플나이트 팬츠', '템플나이트 건틀렛', '템플나이트 부츠']),
    ('titan', 1, ['티탄의 갑옷', '티탄의 팬츠', '티탄의 건틀렛', '티탄의 부츠']),
    ('dwarf', 2, ['드워프의 갑옷', '드워프의 바지', '드워프의 건틀렛', '드워프의 부츠']),
    ('elf', 3, ['엘프의 갑옷', '엘프의 바지', '엘프의 건틀렛', '엘프의 부츠']),
    ('orichalcum', 4, ['오리하르콘 갑옷', '오리하르콘 팬츠', '오리하르콘 건틀렛', '오리하르콘 부츠']),
    ('mithril', 5, ['미스릴 갑옷', '미스릴 팬츠', '미스릴 건틀렛', '미스릴 부츠']),
    ('golden', 6, ['황금의 갑옷', '황금의 팬츠', '황금의 건틀렛', '황금의 부츠']),
    ('alloy', 7, ['합금 갑옷', '합금 팬츠', '합금 건틀렛', '합금 부츠']),
    ('steel', 8, ['강철 아머', '강철 팬츠', '강철 건틀렛', '강철 부츠']),
]


def item_rows():
    rows = {}
    for line in (R/'game-data/DATA/ITEM_DATA.txt').read_bytes().decode('cp949').splitlines():
        cols = line.split('\t')
        if len(cols) < 10 or line.startswith(';') or cols[1] != '0' or sum(c.lower().endswith('.mod') for c in cols) < 6:
            continue
        models = [c for c in cols if c.lower().endswith('.mod')]
        rows[(cols[2], int(cols[4]))] = dict(id=int(cols[0]), name=cols[2], type=int(cols[4]), tier=int(cols[5]),
                                             defense=int(cols[6]), male=models[0], female=models[3])
    return rows


def bare_hand_idle(raw):
    """Standing motion of the unarmed weapon slot (0), state 0, per sex, from character.wad."""
    assert struct.unpack_from('<i', raw)[0] == 100
    at, names = 8, {}
    for _ in range(struct.unpack_from('<i', raw, 4)[0]):
        key, n = struct.unpack_from('<2i', raw, at)
        names[key] = raw[at+8:at+8+n].split(b'\x00')[0].decode('cp949')
        at += 8+n
    result = []
    for _ in range(2):
        count = struct.unpack_from('<i', raw, at)[0]
        begin, end, ani = struct.unpack_from('<2IH', raw, at+4)
        result.append(dict(begin=begin, end=end, ani=names[ani]))
        at += 4+count*28800
    assert at == len(raw)
    return result


def main():
    read = resource_reader(R/'client-overlay')
    rows = item_rows()
    textures = {}
    (O/'textures').mkdir(parents=True, exist_ok=True)

    def load(path):
        chunks = model(read(path))
        for chunk in chunks:
            name = chunk['texture']
            if name not in textures:
                (O/'textures'/Path(name).with_suffix('.png')).write_bytes(png_from_wtm(read('Texture/Body/'+name)))
                textures[name] = 'textures/'+Path(name).with_suffix('.png').name
            chunk['texture'] = name
        return chunks

    table = bare_hand_idle(read('Body/character.wad'))
    people = {}
    for sex, base in ((0, 'mb'), (1, 'fb')):
        idle = table[sex]
        clip = animation(read('Body/Animation/'+idle['ani']), list(range(idle['begin'], idle['end']+1)))
        clip['duration'] = len(clip['sourceFrames'])*1000/30
        people[str(sex)] = dict(head=load(f'Body/High/{base}_hea_0001.mod')+load(f'Body/High/{base}_hir_0001.mod'),
                                idle=clip, idleSource=idle['ani'])

    sets = []
    for key, step, names in SETS:
        items, parts = [], {'0': [], '1': []}
        for (part, kind, _), name in zip(PARTS, names):
            row = rows[(name, kind)]
            items.append(row)
            parts['0'] += load('Body/High/'+row['male'])
            parts['1'] += load('Body/High/'+row['female'])
        sets.append(dict(key=key, step=step, items=items, parts=parts,
                         defense=sum(i['defense'] for i in items), tier=items[0]['tier']))
    data = dict(people=people, sets=sets, textures=textures)
    (O/'armor-sets.json').write_text(json.dumps(data, ensure_ascii=False, separators=(',', ':')), encoding='utf-8')
    for s in sets:
        print(s['key'], s['step'], s['defense'], [i['male'] for i in s['items']], [i['female'] for i in s['items']])


if __name__ == '__main__':
    main()
