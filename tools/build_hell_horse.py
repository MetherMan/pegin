"""Create a separate native horse model and texture using the original rig."""
from pathlib import Path
from io import BytesIO
import hashlib, json, struct, sys, zlib

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'client-overlay/Tools/SkillColors')]
from PIL import Image
from native_actor import model, animation

O = R/'assets/hell-horse'
C = R/'runtime/client/GameClient'


def main():
    source = (C/'Vehicle/horse_1.mod').read_bytes()
    original = model(source)
    raw = bytearray(source)
    at = 36
    changed = []
    for chunk_index, chunk in enumerate(original):
        points = chunk['points']
        at += 64
        count = struct.unpack_from('<i', raw, at)[0]
        at += 4
        if chunk_index == 0:
            # Broaden the existing four angular hooves, preserving their topology,
            # height, UVs and one-bone skin weights. No disconnected added shells.
            for bone in (14, 20, 24, 28):
                ids = [i for i, k in enumerate(chunk['bones']) if k == bone]
                cx = sum(points[i][0] for i in ids)/len(ids)
                cz = sum(points[i][2] for i in ids)/len(ids)
                for i in ids:
                    x, y, z = points[i]
                    struct.pack_into('<3f', raw, at+i*12, cx+(x-cx)*1.20, y, cz+(z-cz)*1.13)
                    changed.append(i)
        at += count*12
        faces = struct.unpack_from('<i', raw, at)[0]
        at += 4+faces*72+16
        raw[at:at+32] = b'mt_hellhorse.bmp'.ljust(32, b'\0')
        at += 32
        kind = struct.unpack_from('<i', raw, at)[0]
        at += 4 + (count if kind == 2 else 0)
    assert at == len(raw)
    modified = model(bytes(raw))
    for a, b in zip(original, modified):
        assert a['bones'] == b['bones'] and a['corners'] == b['corners'] and a['matrix'] == b['matrix']
    payload = O/'payload'
    (payload/'Vehicle').mkdir(parents=True, exist_ok=True)
    (payload/'Texture/Vehicle').mkdir(parents=True, exist_ok=True)
    target = payload/'Vehicle/mt_hellhorse.mod'
    target.write_bytes(raw)
    texture = Image.open(O/'hellhorse-atlas-imagegen.png').convert('RGB').resize((1024,1024), Image.Resampling.LANCZOS)
    texture.save(O/'hellhorse-atlas.png')
    stream = BytesIO()
    texture.save(stream, format='BMP')
    bmp = stream.getvalue()
    wtm = b'TEAMMAY\0\0'+struct.pack('<I', len(bmp))+zlib.compress(bmp, 9)
    (payload/'Texture/Vehicle/mt_hellhorse.wtm').write_bytes(wtm)
    assert Image.open(BytesIO(zlib.decompress(wtm[13:]))).tobytes() == texture.tobytes()
    ani_raw = (C/'Vehicle/Animation/horse_1.ani').read_bytes()
    n = struct.unpack_from('<i', ani_raw, 32)[0]
    clip = animation(ani_raw, list(range(n)))
    assert len(clip['bones']) == 32
    assert all(max(c['bones']) < len(clip['bones']) for c in modified)
    # horse_1.act uses native idle frames 1..101 and locomotion frames 151..167.
    data = dict(original=original, hellhorse=modified, animation=clip,
                textures={'original':'horse21-original.png', 'hellhorse':'hellhorse-atlas.png'},
                motions={'idle':[1,101], 'run':[151,167]})
    (O/'model.json').write_text(json.dumps(data, separators=(',',':')), encoding='utf-8')
    report = dict(passed=True, source='Vehicle/horse_1.mod', source_texture='Texture/Vehicle/horse21.wtm',
                  source_sha256=hashlib.sha256(source).hexdigest(), model_sha256=hashlib.sha256(raw).hexdigest(),
                  source_bones=32, source_frames=n, vertices=sum(len(c['points']) for c in modified),
                  triangles=sum(len(c['corners'])//3 for c in modified), hoof_vertices_modified=len(changed),
                  original_skin_weights_uvs_topology_preserved=True, texture_size=[1024,1024],
                  eye_emission=False, registered_as_ride_item=False,
                  note='Separate native model asset and animated preview. Existing horse definitions are unchanged.')
    (O/'validation.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(report, ensure_ascii=False))


if __name__ == '__main__':
    main()
