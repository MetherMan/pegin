"""Add the Blue Moon horse (certificate 19131, ride type 5, speed 120) to the server sources.

Edits the UTF-8 build inputs and their CP949 originals together (they are kept identical), and
the CP949-only item.h / player.cpp. Idempotent: every edit is guarded by its original text.
"""
from pathlib import Path

R = Path(__file__).resolve().parents[1]
S = R/'src/server'
PAIRS = {'item.cpp': 'item.utf8.cpp', 'gm_commands.cpp': 'gm_commands.utf8.cpp', 'gm_parser.h': 'gm_parser.utf8.h',
         'gm_catalog_data.h': 'gm_catalog_data.utf8.h'}

EDITS = {
    'item.h': [
        ('#define dRIDE_TYPE4 4\n',
         '#define dRIDE_TYPE4 4\n#define dRIDE_ITEM5 19131 // Blue Moon horse ownership certificate\n#define dRIDE_TYPE5 5\n')],
    'player.cpp': [
        ('\t\tcase dRIDE_TYPE4:\n\t\t\tspeed = 100;\n\t\t\tbreak;\n',
         '\t\tcase dRIDE_TYPE4:\n\t\t\tspeed = 100;\n\t\t\tbreak;\n\t\tcase dRIDE_TYPE5:\t\t// Blue Moon horse\n\t\t\tspeed = 120;\n\t\t\tbreak;\n')],
    'item.cpp': [
        ('\tcase dRIDE_ITEM4:\n', '\tcase dRIDE_ITEM4:\n\tcase dRIDE_ITEM5:\n'),
        ('\t\t\telse if( pItem->itemNum == dRIDE_ITEM4 )\n\t\t\t\tGET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE4;\n',
         '\t\t\telse if( pItem->itemNum == dRIDE_ITEM4 )\n\t\t\t\tGET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE4;\n'
         '\t\t\telse if( pItem->itemNum == dRIDE_ITEM5 )\n\t\t\t\tGET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE5;\n')],
    'gm_commands.cpp': [
        ('1=갈색, 2=흑마, 3=백마, 4=지옥마. 예시', '1=갈색, 2=흑마, 3=백마, 4=지옥마, 5=푸른달. 예시'),
        ('(r.subtype==4?dRIDE_ITEM4:10187+r.subtype)', '(r.subtype==5?dRIDE_ITEM5:r.subtype==4?dRIDE_ITEM4:10187+r.subtype)')],
    'gm_parser.h': [
        ('  else if(gmEqual(t[2],"지옥마")||gmEqual(t[2],"hell"))r.subtype=4;\n  else if(!gmNumber(t[2],1,4,r.subtype))return false;',
         '  else if(gmEqual(t[2],"지옥마")||gmEqual(t[2],"hell"))r.subtype=4;\n'
         '  else if(gmEqual(t[2],"푸른달")||gmEqual(t[2],"푸른달말")||gmEqual(t[2],"bluemoon"))r.subtype=5;\n'
         '  else if(!gmNumber(t[2],1,5,r.subtype))return false;')],
    'gm_catalog_data.h': [
        ('{19130,"지옥마 소유증서",4,-1,-1},\n', '{19130,"지옥마 소유증서",4,-1,-1},\n{19131,"푸른달의 말 소유증서",4,-1,-1},\n')],
}
COUNTS = {'item.cpp': [2, 2]}  # both the use-item and login-restore switches


def apply(text, name):
    for k, (old, new) in enumerate(EDITS[name]):
        if new in text:
            continue
        want = COUNTS.get(name, [1]*len(EDITS[name]))[k]
        assert text.count(old) == want, (name, old[:40], text.count(old))
        text = text.replace(old, new)
    return text


def main():
    for name in EDITS:
        cp = S/'LAQIA_GameServer'/name
        raw = cp.read_bytes().decode('cp949')
        updated = apply(raw, name)
        cp.write_bytes(updated.encode('cp949'))
        if name in PAIRS:
            u = S/'utf8'/PAIRS[name]
            text = apply(u.read_text(encoding='utf-8-sig'), name)
            u.write_bytes(text.encode('utf-8'))
            assert text.replace('\r\n', '\n') == updated.replace('\r\n', '\n'), name
        print('updated', name)


if __name__ == '__main__':
    main()
