"""Export higher-tier warrior armour models (original textures) to pick parts for the Blue Moon set.

Writes assets/blue-moon-armor/donor-sets.json for index.html?data=donor-sets. Read-only on game files.
"""
from pathlib import Path
import json, sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
from native_actor import model
from native_assets import png_from_wtm
from tuning import resource_reader

O = R/'assets/blue-moon-armor'
PARTS = ('tor', 'leg', 'gun', 'boo')
DONORS = [
    ('temple', '템플나이트', 'b002', 'b002'),
    ('redknight', '레드나이트', 'b033', 'b035'),
    ('blackknight', '블랙나이트', 'b032', 'b034'),
    ('b034m', '미등록 남 b034 / 여 b032', 'b034', 'b032'),
    ('b035m', '미등록 남 b035 / 여 b033', 'b035', 'b033'),
]


def main():
    read = resource_reader(R/'client-overlay')
    base = json.loads((O/'armor-sets.json').read_text(encoding='utf-8'))
    textures = dict(base['textures'])
    (O/'textures').mkdir(exist_ok=True)

    def load(path):
        chunks = model(read(path))
        for c in chunks:
            name = c['texture']
            if name not in textures:
                (O/'textures'/Path(name).with_suffix('.png')).write_bytes(png_from_wtm(read('Texture/Body/'+name)))
                textures[name] = 'textures/'+Path(name).with_suffix('.png').name
        return chunks
    sets = []
    for key, title, male, female in DONORS:
        parts = {'0': [], '1': []}
        for part in PARTS:
            for sex, prefix, num in (('0', 'ma', male), ('1', 'fe', female)):
                try:
                    parts[sex] += load(f'Body/High/{prefix}_{part}_{num}_1.mod')
                except FileNotFoundError:
                    pass
        sets.append(dict(key=key, title=title, note=f'남 {male} · 여 {female} 원본', items=[], parts=parts, defense=0, tier='-'))
    page = dict(title='상위 갑옷 원본 비교 (부품 후보)',
                lead='푸른달 갑옷에 옮겨 붙일 부품 후보를 고르기 위한 상위 전사 갑옷 원본 모델입니다. 색은 원본 그대로입니다.')
    (O/'donor-sets.json').write_text(json.dumps(dict(people=base['people'], sets=sets, textures=textures, page=page),
                                                ensure_ascii=False, separators=(',', ':')), encoding='utf-8')
    print(json.dumps([(s['key'], len(s['parts']['0']), len(s['parts']['1'])) for s in sets], ensure_ascii=False))


if __name__ == '__main__':
    main()
