"""Verify independent native ice colour, exact legacy default and save migration."""
from pathlib import Path
import copy
import hashlib
import json
import io
import sys

R = Path(__file__).resolve().parents[1]
D = R / 'client-overlay'
H = D / 'Tools/SkillColors'
sys.dont_write_bytecode = True
sys.path.insert(0, str(H))
sys.path.insert(0, str(R / 'runtime/pylibs'))
from PIL import Image
from tuning import compile_resources, load_settings, resource_reader, validate_colors
from native_assets import read_wed, load_preview, png_from_wtm
from cocytus import DEFAULT_ICE_COLOR, is_ice_body_color
import edit_colors as editor


def sha(value):
    return hashlib.sha256(value).hexdigest()


before = json.loads((R / '.cache/ice-color-before.json').read_text(encoding='utf-8'))
saved_numbers = (H / 'tuning.json').read_bytes()
saved_colors = (H / 'colors.json').read_bytes()
saved = load_settings(H)
colors = validate_colors(json.loads(saved_colors))
read = resource_reader(D)
baseline, _ = compile_resources(read, before['settings'], before['colors'])
assert set(baseline) == set(before['compiled'])
assert all(sha(raw) == before['compiled'][key] for key, raw in baseline.items()
           if key != 'Tools/SkillColors/colors.json'), 'Default blue changed existing native resources'

original, meta = compile_resources(read, saved, colors)
body = {key for key in original if key.startswith('Effect/') and is_ice_body_color(key)}
assert body and all(key.endswith('.wed') for key in body)
assert body == {'Effect/' + name for name in meta['frost']['iceColorSurfaces']}
assert 'Effect/mf_cocytus_pillar_rim.wed' in body
cases = []
for chosen in ('#D82F4A', '#31D85A', '#884CDB', '#FFFFFF', '#000000', DEFAULT_ICE_COLOR):
    selection = dict(colors, frostIce=chosen)
    out, timing = compile_resources(read, saved, selection)
    assert set(original) <= set(out)
    neutral_textures = {'Texture/Effect/' + read_wed(out[key])['texture'] for key in body} if chosen != DEFAULT_ICE_COLOR else set()
    assert set(out) - set(original) <= neutral_textures, 'Custom colour added an unrelated resource'
    for texture in neutral_textures:
        pixels = Image.open(io.BytesIO(png_from_wtm(out[texture]))).convert('RGB')
        rgb_pixels = pixels.tobytes()
        assert rgb_pixels[::3] == rgb_pixels[1::3] == rgb_pixels[2::3], 'Texture still biases the selected body hue'
    changed, frame_count = [], 0
    for key, raw in out.items():
        if key == 'Tools/SkillColors/colors.json':
            continue
        if key not in original:
            assert key in neutral_textures
            continue
        if key not in body:
            assert raw == original[key], ('Ice colour changed an unrelated resource', key)
            continue
        source, target = read_wed(original[key]), read_wed(raw)
        assert {k: v for k, v in source.items() if k not in ('frames', 'texture')} == {k: v for k, v in target.items() if k not in ('frames', 'texture')}
        for a, b in zip(source['frames'], target['frames']):
            assert a[:11] == b[:11] and a[12:] == b[12:], ('Geometry/timing changed', key)
            assert (a[11] >> 24) == (b[11] >> 24), ('Opacity changed', key)
            frame_count += 1
        if raw != original[key]:
            changed.append(key)
    if chosen == colors['frostIce']:
        assert not changed
    else:
        assert set(changed) == body, (chosen, 'Some body layers did not change')
    preview = load_preview(H, D, generated=out, metadata=timing, compact=True, kinds=['frost'])
    assert preview['timing']['frost']['iceColor'] == chosen
    for key in body:
        assert preview['effects'][Path(key).name]['frames'] == read_wed(out[key])['frames']
    cases.append(dict(color=chosen, changedBodyResources=len(changed), checkedFrames=frame_count,
                      neutralTextures=len(neutral_textures),
                      opacityAndGeometryUnchanged=True, unrelatedNativeResourcesUnchanged=True,
                      compactPreviewMatchesNative=True))

# Changing the old auxiliary colour cannot change a single ice-body byte.
aux, _ = compile_resources(read, saved, dict(colors, frost='#13A984'))
assert all(aux[key] == original[key] for key in body)
assert aux['Effect/mf_cocytus_seal1.wed'] != original['Effect/mf_cocytus_seal1.wed']

# The legacy schema is accepted and upgraded, but malformed/new unknown fields
# are rejected before compilation. Inputs are never mutated by migration.
legacy = {key: colors[key] for key in ('meteor', 'frost')}
legacy_copy = copy.deepcopy(legacy)
upgraded = validate_colors(legacy)
assert legacy == legacy_copy and upgraded == dict(legacy, frostIce=DEFAULT_ICE_COLOR)
assert validate_colors(dict(legacy, frostIce='#a1b2c3'))['frostIce'] == '#A1B2C3'
bad = [None, {}, dict(legacy, other='#ffffff')]
for invalid in ('', '#fff', '112233', '#GG0000', '#1234567', None, 123, True, '#12\n3456'):
    bad.append(dict(legacy, frostIce=invalid))
for value in bad:
    try:
        validate_colors(value)
    except ValueError:
        pass
    else:
        raise AssertionError(('Invalid colour accepted', value))

# Exercise the real save-preview writer under a bounded task-cache client root.
# Its resource reader points to the real overlay, but every write stays in cache.
cache_client = R / '.cache/ice-color-save-test/client-overlay'
cache_here = cache_client / 'Tools/SkillColors'
assert cache_client.resolve().is_relative_to((R / '.cache').resolve())
cache_here.mkdir(parents=True, exist_ok=True)
old_here, old_client, old_reader = editor.HERE, editor.CLIENT, editor.resource_reader
try:
    editor.HERE, editor.CLIENT = cache_here, cache_client
    editor.resource_reader = lambda *args, **kwargs: read
    (cache_here / 'colors.json').write_text(json.dumps(dict(legacy, frostIce='#31D85A')), encoding='utf-8')
    # Old open panels must preserve the last independently saved body colour.
    assert editor.validate(legacy)['frostIce'] == '#31D85A'
    result = editor.save_preview(legacy, saved)
    reloaded = json.loads((cache_here / 'colors.json').read_text())
    assert reloaded == dict(legacy, frostIce='#31D85A')
    assert load_settings(cache_here) == saved
    assert result['colors'] == reloaded
    assert result['timing']['frost']['iceColor'] == '#31D85A'
    new = editor.save_preview(dict(legacy, frostIce='#884CDB'), saved)
    assert editor.validate(legacy)['frostIce'] == '#884CDB'
    assert json.loads((cache_here / 'colors.json').read_text()) == new['colors']
    # A first migration of an old two-colour file keeps both saved spell colours.
    (cache_here / 'colors.json').write_text(json.dumps(legacy), encoding='utf-8')
    assert editor.validate(legacy) == dict(legacy, frostIce=DEFAULT_ICE_COLOR)
finally:
    editor.HERE, editor.CLIENT, editor.resource_reader = old_here, old_client, old_reader

assert (H / 'tuning.json').read_bytes() == saved_numbers, 'Test changed saved numeric values'
assert (H / 'colors.json').read_bytes() == saved_colors, 'Test changed saved colours'
assert sha((D / 'Render.dll').read_bytes()) == before['renderHash'], 'Ice colour must not repatch the renderer'
report = dict(passed=True, defaultBlueNativeByteIdentity=len(baseline)-1,
              defaultIceColor=DEFAULT_ICE_COLOR, independentBodySurfaces=len(body), cases=cases,
              auxiliaryColorIndependent=True, invalidInputsRejected=len(bad),
              savePreviewRoundTrip=True, legacyPanelPreservesSavedIceColor=True,
              savedNumericFieldsPreserved=sum(map(len, saved.values())),
              currentUserOpacity=saved['frost']['iceOpacity'], savedColorsUnchanged=True,
              rendererUnchanged=True, writesOnlyToTaskCache=True, inGameDirect3DTest=False)
path = R / 'assets/skills140/ice-color-validation.json'
path.write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
print(json.dumps(report, ensure_ascii=False))
