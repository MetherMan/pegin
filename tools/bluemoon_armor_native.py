"""Native game files for the confirmed Blue Moon armour (Elf set + Black Knight pauldrons).

The look is the one approved in assets/blue-moon-armor/index.html?data=elf-bluemoon (texture LOOK,
Black Knight pauldrons, crescent emblem on chest and back). Models are written as Wind3D MOD files
with the original chunk matrices/materials, so the game skins them exactly like the source sets:

- Body/High/<sex>_<part>_b050_1.mod: Elf model; the torso also carries the Black Knight
  pauldron triangles (all three vertices on Clavicle/UpperArm), pushed 4% out from their own joint.
- Female torso/legs show skin, so the client loads race variants b150/b250 (it swaps the first
  model digit for the face's race). They differ only in the Girl_*_<race> skin texture, as in the
  original fe_*_b111/b211 files.
- Body/Sub/<sex>_<part>_b050_2.mod: the Elf ground (dropped item) model with the new textures.
"""
from pathlib import Path
from io import BytesIO
import struct, zlib
import numpy as np
from PIL import Image

R = Path(__file__).resolve().parents[1]
GAME = R/'runtime/client/GameClient'
TEX = R/'assets/blue-moon-armor/elf'
# Applied look from tools/build_bluemoon_golden_armor.py ELF_LOOKS: 'prev' = A (first release),
# 'bright' = D, more bright plate area (user pick 2026-10-01 after A read like a blue bodysuit).
LOOK = 'bright'
NUM = 'b050'
PARTS = ('tor', 'leg', 'gun', 'boo')
SEXES = {'0': ('ma', 'b008', 'b032'), '1': ('fe', 'b011', 'b034')}
SKIN = {'girl_torso_0-1.bmp': 'Girl_torso_{race}-1.bmp', 'girl_leg_0-.bmp': 'Girl_leg_{race}-.bmp'}
SCALE = 1.04  # same push-out as tools/bluemoon_armor_kitbash.py (approved preview)


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


def pauldrons(donor, bone_list):
    """Native twin of bluemoon_armor_kitbash.shoulder_parts (keeps matrix, material and kind)."""
    names = [b['name'] for b in bone_list]
    rest = [np.array(b['rest'][12:15], float) for b in bone_list]
    keep = {i for i, n in enumerate(names) if 'UpperArm' in n or 'Clavicle' in n}
    out = []
    for c in donor:
        cs = c['corners']
        faces = [f for f in range(0, len(cs), 3) if all(c['bones'][cs[f+k][0]] in keep for k in range(3))]
        if not faces:
            continue
        used = sorted({cs[f+k][0] for f in faces for k in range(3)})
        index = {old: new for new, old in enumerate(used)}
        points = []
        for old in used:
            centre = rest[c['bones'][old]]
            points.append(list(map(float, centre+(np.array(c['points'][old])-centre)*SCALE)))
        corners = [[index[k[0]], *k[1:]] for f in faces for k in cs[f:f+3]]
        out.append(dict(c, points=points, corners=corners, bones=[c['bones'][o] for o in used]))
    return out


def texture_names(prefix, part, donor_textures=()):
    """Source texture (lower case) -> (new texture name, approved preview PNG)."""
    elf = next(e for p, e, _ in SEXES.values() if p == prefix)
    names = {}
    for ext in ('bmp', 'BMP'):
        names[f'{prefix}_{part}_{elf}.{ext}'.lower()] = (f'{prefix}_{part}_{NUM}.bmp', TEX/f'{LOOK}-{prefix}_{part}_{elf}.png')
    for k, source in enumerate(sorted(donor_textures)):
        stem = Path(source).stem
        names[source.lower()] = (f'{prefix}_{part}_{NUM}{"abc"[k]}.bmp', TEX/f'{LOOK}-{stem}.png')
    return names


def wtm(image):
    stream = BytesIO()
    image.convert('RGB').save(stream, format='BMP')
    raw = stream.getvalue()
    out = b'TEAMMAY\0\0'+struct.pack('<I', len(raw))+zlib.compress(raw, 9)
    back = Image.open(BytesIO(zlib.decompress(out[13:])))
    assert back.size == image.size and back.convert('RGB').tobytes() == image.convert('RGB').tobytes()
    return out


def build(bones_by_sex):
    """Returns ({relative client path: bytes}, summary). Reads only the original game files."""
    files, summary = {}, {}
    for sex, (prefix, elf, black) in SEXES.items():
        for part in PARTS:
            header, chunks = parse((GAME/f'Body/High/{prefix}_{part}_{elf}_1.mod').read_bytes())
            assert write(header, chunks) == (GAME/f'Body/High/{prefix}_{part}_{elf}_1.mod').read_bytes()
            extra = []
            if part == 'tor':
                _, donor = parse((GAME/f'Body/High/{prefix}_tor_{black}_1.mod').read_bytes())
                extra = pauldrons(donor, bones_by_sex[sex])
            rename = texture_names(prefix, part, {c['texture'] for c in extra})
            skin = {c['texture'].lower() for c in chunks} & set(SKIN)
            races = ['0', '1', '2'] if skin else ['0']
            for race in races:
                out = []
                for c in chunks+extra:
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
            header2, ground = parse((GAME/f'Body/Sub/{prefix}_{part}_{elf}_2.mod').read_bytes())
            for c in ground:
                key = c['texture'].lower()
                c['texture'] = SKIN[key].format(race='0') if key in SKIN else rename[key][0]
            files[f'Body/Sub/{prefix}_{part}_{NUM}_2.mod'] = write(header2, ground)
            used = {c['texture'].lower() for c in chunks+extra}-set(SKIN)
            for key in sorted(used):
                name, png = rename[key]
                files['Texture/Body/'+Path(name).with_suffix('.wtm').name] = wtm(Image.open(png))
            summary[f'{prefix}_{part}'] = dict(
                elf_triangles=sum(len(c['corners'])//3 for c in chunks),
                pauldron_triangles=sum(len(c['corners'])//3 for c in extra),
                race_variants=races, textures=sorted(rename[k][0] for k in used))
    return files, summary
