"""Validate that this revision changes only horn UVs, surface normals and pixels."""
from pathlib import Path
import hashlib
import io
import json
import math
import sys
import zlib

R = Path(__file__).resolve().parents[1]
D = R / 'client-overlay'
O = R / 'assets/primordial-baphomet'
B = R / '.cache/horn-v5-baseline'
sys.path[:0] = [str(R / 'runtime/pylibs'), str(D / 'Tools/SkillColors')]
from native_actor import model
from PIL import Image
from horn_strength import apply_strength,load_strength

def main():
    old = model((B / 'Monster/mt_prime_baphomet.mod').read_bytes())
    new = model((D / 'Monster/mt_prime_baphomet.mod').read_bytes())
    assert len(old) == len(new) == 2
    changed_uv, changed_normals = [], []
    for index, (before, after) in enumerate(zip(old, new)):
        for key in before:
            if key != 'corners':
                assert before[key] == after[key], (index, key)
        assert len(before['corners']) == len(after['corners'])
        count = normal_count = 0
        bound = .5 if index == 0 else .75
        for a, b in zip(before['corners'], after['corners']):
            assert a[0] == b[0], 'Position index changed'
            if a[1:4] != b[1:4]:
                assert a[4] >= bound and b[4] >= bound, 'Non-horn normal changed'
                assert abs(math.sqrt(sum(v*v for v in b[1:4])) - 1) < 1e-5, 'Non-unit horn normal'
                normal_count += 1
            if a[4:6] != b[4:6]:
                assert a[4] >= bound and b[4] >= bound, 'Non-horn UV changed'
                assert bound < b[4] < 1 and 0 < b[5] < 1
                count += 1
        assert count > 0
        assert normal_count > 0
        changed_uv.append(count)
        changed_normals.append(normal_count)
    assert json.loads(json.dumps(new)) == json.loads((O / 'model.json').read_text(encoding='utf-8'))['primordial']
    pixels = []
    for native, preview, boundary in [('mt_prime_body.wtm', 'body-cyclops-atlas.png', 1024),
                                      ('mt_prime_cleaver.wtm', 'cleaver-atlas.png', 1536)]:
        before = Image.open(io.BytesIO(zlib.decompress((B / 'Texture/Monster' / native).read_bytes()[13:]))).convert('RGB')
        after = Image.open(io.BytesIO(zlib.decompress((D / 'Texture/Monster' / native).read_bytes()[13:]))).convert('RGB')
        assert before.size == after.size == (2048, 1024)
        assert before.crop((0, 0, boundary, 1024)).tobytes() == after.crop((0, 0, boundary, 1024)).tobytes()
        source = Image.open(O / 'horn-satin-keratin-v5-imagegen.png').convert('RGB').resize((2048-boundary, 1024), Image.Resampling.LANCZOS)
        source = apply_strength(source,load_strength())
        assert source.tobytes() == after.crop((boundary, 0, 2048, 1024)).tobytes()
        assert after.tobytes() == Image.open(O / preview).convert('RGB').tobytes()
        pixels.append(dict(native=native, nonHornPixelsUnchanged=boundary*1024,
                           sourceHornPixelsExact=True, nativePreviewPixelsIdentical=True))
    allowed = {'Monster/mt_prime_baphomet.mod', 'Texture/Monster/mt_prime_body.wtm', 'Texture/Monster/mt_prime_cleaver.wtm'}
    before = json.loads((B / 'overlay-before.json').read_text())
    changed = {rel for rel, digest in before.items() if hashlib.sha256((D / rel).read_bytes()).hexdigest() != digest}
    assert changed == allowed, changed
    sculpt = json.loads((O / 'sculpt-validation.json').read_text())
    assert sculpt['scale'] == 1.7 and sculpt['blade_roll_degrees'] == 180
    report = dict(passed=True, revision='black-keratin-v5', changedHornUvCorners=changed_uv,
                  changedHornNormalCorners=changed_normals, geometryBonesUnchanged=True,
                  nonHornNormalsUnchanged=True, nonHornUvUnchanged=True, textures=pixels,
                  changedResources=sorted(changed), protectedOverlayFilesUnchanged=len(before)-len(changed),
                  scale=1.7, bladeRollDegrees=180, animationsUnchanged=True, inGameVisualTest=False)
    (O / 'v5-horn-validation.json').write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(json.dumps(report, ensure_ascii=False))

if __name__ == '__main__':
    main()
