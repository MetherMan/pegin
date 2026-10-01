"""Native game files for the confirmed Blue Moon armour (Black Knight set in Blue Moon colours).

History: the first release (2026-10-01) was the Elf set with kitbashed Black Knight pauldrons. In
game it looked cheap and its chest crescent stretched, so the user switched to the whole Black Knight
set (male ma_*_b032, female fe_*_b034) recoloured by tools/build_bluemoon_bk_armor.py, look LOOK:
the applied D tone, a crescent on the outer chest plate, and the male back leather kept dark so the
V-shaped back plate no longer reads as an Iron Man mask. Models keep the original chunk matrices,
materials and skinning; only texture names change:

- Body/High/<sex>_<part>_b050_1.mod: the Black Knight model.
- Female torso/legs show skin, so the client loads race variants b150/b250 (it swaps the first
  model digit for the face's race). They differ only in the Girl_*_<race> skin texture, as in the
  original fe_*_b134/b234 files. Male Black Knight parts have no skin flag.
- Body/Sub/<sex>_<part>_b050_2.mod: the Black Knight ground (dropped item) model, new textures.
"""
from pathlib import Path
from io import BytesIO
import struct, zlib
from PIL import Image

R = Path(__file__).resolve().parents[1]
GAME = R/'runtime/client/GameClient'
TEX = R/'assets/blue-moon-armor/bk'
LOOK = 'd-moon-calm'  # tools/build_bluemoon_bk_armor.py LOOKS key (user pick 2026-10-01)
NUM = 'b050'
PARTS = ('tor', 'leg', 'gun', 'boo')
SEXES = {'0': ('ma', 'b032'), '1': ('fe', 'b034')}
SKIN = {'girl_torso_0-1.bmp': 'Girl_torso_{race}-1.bmp', 'girl_leg_0-.bmp': 'Girl_leg_{race}-.bmp'}
# Texture files of the previous (Elf-based) release that the Black Knight look no longer uses.
RETIRED = ['Texture/Body/ma_tor_b050c.wtm', 'Texture/Body/fe_tor_b050a.wtm']


def parse(raw):
    count = struct.unpack_from('<i', raw, 32)[0]
    at, chunks = 36, []
    for _ in range(count):
        c = dict(matrix=raw[at:at+64])
        at += 64
        n = struct.unpack_from('<i', raw, at)[0]
        at += 4
        c['points'] = [list(p) for p in struct.iter_unpack('<3f', raw[at:at+n*12])]
        at += n*12
        faces = struct.unpack_from('<i', raw, at)[0]
        at += 4
        c['corners'] = [list(k) for k in struct.iter_unpack('<i5f', raw[at:at+faces*72])]
        at += faces*72
        c['material'] = raw[at:at+16]
        at += 16
        c['texture_raw'] = raw[at:at+32]
        c['texture'] = raw[at:at+32].split(b'\0')[0].decode('cp949')
        at += 32
        c['kind'] = struct.unpack_from('<i', raw, at)[0]
        at += 4
        c['bones'] = list(raw[at:at+n]) if c['kind'] == 2 else None
        at += n if c['kind'] == 2 else 0
        chunks.append(c)
    assert at == len(raw), (at, len(raw))
    return raw[:32], chunks


def write(header, chunks):
    out = bytearray(header)+struct.pack('<i', len(chunks))
    for c in chunks:
        assert len(c['corners']) % 3 == 0 and all(0 <= k[0] < len(c['points']) for k in c['corners'])
        out += c['matrix']+struct.pack('<i', len(c['points']))
        out += b''.join(struct.pack('<3f', *p) for p in c['points'])
        out += struct.pack('<i', len(c['corners'])//3)+b''.join(struct.pack('<i5f', *k) for k in c['corners'])
        name = c['texture'].encode('cp949')
        raw = c['texture_raw'] if c['texture_raw'].split(b'\0')[0] == name else name.ljust(32, b'\0')
        assert len(raw) == 32
        out += c['material']+raw+struct.pack('<i', c['kind'])
        if c['kind'] == 2:
            assert len(c['bones']) == len(c['points'])
            out += bytes(c['bones'])
    return bytes(out)


def texture_names(prefix, part, sources):
    """Source texture (lower case) -> (new texture name, approved preview PNG). The part's own
    texture (e.g. ma_tor_b032) keeps the plain name, any others get a, b, ... in name order."""
    base = f'{prefix}_{part}_{SEXES["0" if prefix == "ma" else "1"][1]}'
    order = sorted((s for s in sources if s.lower() not in SKIN), key=lambda s: (Path(s).stem.lower() != base, s.lower()))
    names = {}
    for k, source in enumerate(order):
        suffix = '' if k == 0 else 'abcdefgh'[k-1]
        names[source.lower()] = (f'{prefix}_{part}_{NUM}{suffix}.bmp', TEX/f'{LOOK}-{Path(source).stem}.png')
    return names


def wtm(image):
    stream = BytesIO()
    image.convert('RGB').save(stream, format='BMP')
    raw = stream.getvalue()
    out = b'TEAMMAY\0\0'+struct.pack('<I', len(raw))+zlib.compress(raw, 9)
    back = Image.open(BytesIO(zlib.decompress(out[13:])))
    assert back.size == image.size and back.convert('RGB').tobytes() == image.convert('RGB').tobytes()
    return out


def build(bones_by_sex=None):
    """Returns ({relative client path: bytes}, summary). Reads only the original game files."""
    files, summary = {}, {}
    for sex, (prefix, black) in SEXES.items():
        for part in PARTS:
            source = (GAME/f'Body/High/{prefix}_{part}_{black}_1.mod').read_bytes()
            header, chunks = parse(source)
            assert write(header, chunks) == source
            rename = texture_names(prefix, part, {c['texture'] for c in chunks})
            skin = {c['texture'].lower() for c in chunks} & set(SKIN)
            races = ['0', '1', '2'] if skin else ['0']
            for race in races:
                out = []
                for c in chunks:
                    key = c['texture'].lower()
                    if key in SKIN:
                        name = SKIN[key].format(race=race)
                        assert (GAME/'Texture/Body'/Path(name).with_suffix('.wtm').name).exists(), name
                    else:
                        name = rename[key][0]
                    out.append(dict(c, texture=name))
                model = f'Body/High/{prefix}_{part}_b{race}{NUM[2:]}_1.mod'
                files[model] = write(header, out)
                assert [len(c['points']) for c in parse(files[model])[1]] == [len(c['points']) for c in out]
            header2, ground = parse((GAME/f'Body/Sub/{prefix}_{part}_{black}_2.mod').read_bytes())
            for c in ground:
                key = c['texture'].lower()
                c['texture'] = SKIN[key].format(race='0') if key in SKIN else rename[key][0]
            files[f'Body/Sub/{prefix}_{part}_{NUM}_2.mod'] = write(header2, ground)
            for key, (name, png) in sorted(rename.items()):
                files['Texture/Body/'+Path(name).with_suffix('.wtm').name] = wtm(Image.open(png))
            summary[f'{prefix}_{part}'] = dict(source=f'{prefix}_{part}_{black}', triangles=sum(len(c['corners'])//3 for c in chunks),
                                              race_variants=races, textures=sorted(n for n, _ in rename.values()))
    return files, summary
