"""Gold attack effects for the 천상의 날개 weapons (same approach as the Twilight blue effects).

- Bow: copies of the normal arrow (Magic/arw_d1.ms "단궁기본"): projectile arrow01.wed, impact
  ta_arrow01.wed and the arrow trail texture arw01, with only their RGB recoloured to gold.
  Motion, mesh, sound, blend, opacity, timing and frame counts stay byte-for-byte. Registered as
  magic "cw_arrow" with trail "cw_arrow_trace"; tools/patch_celestial_effects_client.py makes the
  bow 19290..19310 use it.
- Sword: trail "celestial_gold" = the paladin greatsword trail texture (검흔C, swd08) in gold,
  same frame count and rate as 검흔C. Equip/Trace.txt points cw_sword_{1,2,3}.mod at it.
Recolour: the sources are red (swd08) and cyan (arw01, arrow frames), so a plain multiply gives
red/green. Each colour's intensity (its brightest channel) is mapped onto a gold ramp instead;
these effects blend additively, so black stays invisible and the brightness pattern is unchanged.
"""
from pathlib import Path
import json, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
C = R/'runtime/client/GameClient'
D = R/'client-overlay'
# intensity -> colour (additive effects: black stays transparent, brightest becomes pale gold)
GOLD = [(0, (0, 0, 0)), (.35, (120, 70, 10)), (.65, (235, 170, 40)), (.85, (255, 215, 110)), (1, (255, 248, 215))]


def gold(r, g, b):
    v = max(r, g, b)/255
    for (a, ca), (z, cz) in zip(GOLD, GOLD[1:]):
        if v <= z:
            t = (v-a)/(z-a)
            return tuple(round(x+(y-x)*t) for x, y in zip(ca, cz))
    return GOLD[-1][1]


# The arrow effect frame colours are pure white and multiply the effect texture, so they are
# multiplied to gold directly (the Twilight bow multiplied the same white to blue).
WED_MULTIPLY = (1.0, .80, .32)
TRAIL = 'celestial_gold'


def overlay_or_game(rel):
    return (D/rel) if (D/rel).exists() else (C/rel)


def tint_wed(source, target):
    raw = (C/'Effect'/source).read_bytes()
    b = bytearray(raw)
    at = 31
    for _ in range(2):
        n = struct.unpack_from('<i', b, at)[0]
        at += 4+n
    start, last, count, row, col = struct.unpack_from('<5i', b, at)
    at += 20
    assert at+count*52 == len(b), (source, at, count, len(b))
    changed = set()
    for i in range(count):
        off = at+i*52+44
        old = struct.unpack_from('<I', b, off)[0]
        rgb = [round(((old >> sh) & 255)*f) for sh, f in zip((16, 8, 0), WED_MULTIPLY)]
        struct.pack_into('<I', b, off, (old & 0xff000000) | (rgb[0] << 16) | (rgb[1] << 8) | rgb[2])
        changed.update(range(off, off+3))
    assert all(i in changed for i, (x, y) in enumerate(zip(raw, b)) if x != y)
    (D/'Effect'/target).write_bytes(bytes(b))
    return dict(source=source, target=target, frames=count, only_rgb_changed=True)


def tint_texture(source, target):
    """Recolour the BMP colour bytes inside a WTM to the gold ramp; header and pattern stay as they are."""
    raw = (C/'Texture/Effect'/source).read_bytes()
    bmp = bytearray(zlib.decompress(raw[13:]))
    pixoff = struct.unpack_from('<I', bmp, 10)[0]
    width, height = struct.unpack_from('<2i', bmp, 18)
    bits = struct.unpack_from('<H', bmp, 28)[0]
    assert bits in (24, 32) and struct.unpack_from('<I', bmp, 30)[0] == 0
    stride = ((width*bits+31)//32)*4
    step = bits//8
    before = bytes(bmp)
    for y in range(abs(height)):
        for x in range(width):
            off = pixoff+y*stride+x*step
            r, g, b = gold(bmp[off+2], bmp[off+1], bmp[off])  # BMP stores B, G, R
            bmp[off], bmp[off+1], bmp[off+2] = b, g, r
    assert bmp[:pixoff] == before[:pixoff]
    (D/'Texture/Effect'/target).write_bytes(raw[:9]+struct.pack('<I', len(bmp))+zlib.compress(bytes(bmp), 9))
    return dict(source=source, target=target, size=[width, abs(height)], bits=bits)


def trace_entries(add):
    """Effect/trace.tdf: (name -> (texture, peer name)) replaced or appended, other entries kept."""
    path = overlay_or_game('Effect/trace.tdf')
    data = path.read_bytes()
    version, count = struct.unpack_from('<2i', data)
    at, entries, peers = 8, [], {}
    for _ in range(count):
        begin = at
        n = struct.unpack_from('<i', data, at)[0]
        at += 4
        name = data[at:at+n].rstrip(b'\0').decode('cp949')
        at += n
        n = struct.unpack_from('<i', data, at)[0]
        at += 4+n
        peers[name] = struct.unpack_from('<2i', data, at)
        at += 8
        if name not in add:
            entries.append(data[begin:at])
    assert at == len(data)

    def string(s):
        b = s.encode('ascii')+b'\0'
        return struct.pack('<i', len(b))+b
    for name, (texture, peer) in add.items():
        entries.append(string(name)+string(texture)+struct.pack('<2i', *peers[peer]))
    (D/'Effect/trace.tdf').write_bytes(struct.pack('<2i', version, len(entries))+b''.join(entries))
    return {name: dict(texture=t, frames_and_rate=list(peers[p]), copied_from=p) for name, (t, p) in add.items()}


def main():
    report = dict(gold_ramp=GOLD, effect_colour_multiply=WED_MULTIPLY)
    report['wed'] = [tint_wed('arrow01.wed', 'cw_arrow.wed'), tint_wed('ta_arrow01.wed', 'cw_arrow_hit.wed')]
    report['textures'] = [tint_texture('arw01.wtm', 'cw_arrow_trace.wtm'), tint_texture('swd08.wtm', 'cw_sword_trail.wtm')]
    report['trace'] = trace_entries({'cw_arrow_trace': ('cw_arrow_trace.bmp', '활'), TRAIL: ('cw_sword_trail.bmp', '검흔C')})
    magic = (C/'Magic/arw_d1.ms').read_bytes().decode('cp949')
    for old, new in [('[NAME] 단궁기본', '[NAME] cw_arrow'), ('[TRACE] 활', '[TRACE] cw_arrow_trace'),
                     ('[SPE] arrow01.wed', '[SPE] cw_arrow.wed'), ('[TAIL] ta_arrow01.wed', '[TAIL] cw_arrow_hit.wed')]:
        assert magic.count(old) == 1, old
        magic = magic.replace(old, new)
    (D/'Magic/cw_arrow.ms').write_bytes(magic.encode('cp949'))
    ml = overlay_or_game('Magic/MagicList.ml').read_bytes()
    lines = [l for l in ml.split(b'\r\n') if not l.startswith(b'[MAGIC] cw_arrow')]
    while lines and not lines[-1].strip():
        lines.pop()
    lines.append(b'[MAGIC] cw_arrow\t\tcw_arrow.ms')
    (D/'Magic/MagicList.ml').write_bytes(b'\r\n'.join(lines)+b'\r\n')
    trace = D/'Equip/Trace.txt'
    text = trace.read_bytes().decode('cp949')
    nl = '\r\n' if '\r\n' in text else '\n'
    lines = text.split(nl)
    hits = 0
    for i, line in enumerate(lines):
        if line.startswith('cw_sword_'):
            parts = line.split()
            parts[1] = TRAIL
            lines[i] = ' '.join(parts)
            hits += 1
    assert hits == 3
    trace.write_bytes(nl.join(lines).encode('cp949'))
    report['sword_trail'] = TRAIL
    (R/'assets/golden-knight/effects.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(report, ensure_ascii=False)[:600])


if __name__ == '__main__':
    main()
