"""Verify tint/native texture equivalence and transactional writes in an isolated fixture."""
from pathlib import Path
from io import BytesIO
from unittest.mock import patch
import base64, hashlib, importlib.util, json, struct, subprocess, sys, tempfile, zlib

ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'runtime/pylibs'))
from PIL import Image
spec=importlib.util.spec_from_file_location('baphomet_editor',ROOT/'client-overlay/Tools/BaphometColors/edit_skin.py')
editor=importlib.util.module_from_spec(spec);spec.loader.exec_module(editor)
tool=ROOT/'client-overlay/Tools/BaphometColors'
settings=json.loads((tool/'settings.json').read_text())
raw=(ROOT/'client-overlay/Texture/Monster/mt_prime_body.wtm').read_bytes()
assert raw[:9]==b'TEAMMAY\0\0'
bmp=zlib.decompress(raw[13:]);assert len(bmp)==struct.unpack_from('<I',raw,9)[0]
wtm,png=editor.encode(bmp)
atlas=Image.open(ROOT/'assets/primordial-baphomet/body-atlas.png').convert('RGBA')
assert Image.open(BytesIO(bmp)).convert('RGBA').tobytes()==atlas.tobytes()
assert Image.open(BytesIO(png)).convert('RGBA').tobytes()==atlas.tobytes()
checks=['native BMP/WTM and saved PNG pixel equivalence']

with tempfile.TemporaryDirectory(prefix='baphomet-test-') as temporary:
    folder=Path(temporary)
    (folder/'source.rgba').write_bytes(Image.open(tool/'base-body.png').convert('RGBA').tobytes())
    (folder/'settings.json').write_text(json.dumps(settings))
    subprocess.run([str(ROOT/'runtime/node/node.exe'),str(ROOT/'tests/baphomet_colors_test.mjs'),str(folder)],check=True)
    assert (folder/'tinted.rgba').read_bytes()==atlas.tobytes(), 'saved game pixels must match the editor tint'
    checks.append('browser saved texture matches tint of immutable baseline and final settings')

    repo=folder/'fixture';client=repo/'client-overlay';here=client/'Tools/BaphometColors'
    (repo/'assets/primordial-baphomet').mkdir(parents=True);(repo/'distribution').mkdir()
    paths=[client/'Texture/Monster/mt_prime_body.wtm',here/'settings.json',repo/'assets/primordial-baphomet/body-atlas.png',repo/'assets/primordial-baphomet/skin-settings.json']
    for p in paths:p.parent.mkdir(parents=True,exist_ok=True);p.write_bytes(b'original')
    manifest_path=repo/'distribution/update-manifest.json'
    manifest_path.write_text(json.dumps(dict(files=[dict(source=p.relative_to(repo).as_posix()) for p in paths[:2]])))
    previous={p:p.read_bytes() for p in paths+[manifest_path]}
    payload=dict(settings=settings,bmp=base64.b64encode(bmp).decode())
    with patch.object(editor,'CLIENT',client),patch.object(editor,'HERE',here),patch.object(editor,'game_running',return_value=True):
        try:editor.apply(payload);raise AssertionError('running game accepted')
        except ValueError:pass
    assert all(p.read_bytes()==old for p,old in previous.items())
    checks.append('running game blocked without writes')
    real_replace=editor.os.replace;calls=[0]
    def fail_once(source,target):
        calls[0]+=1
        if calls[0]==3:raise OSError('injected save failure')
        return real_replace(source,target)
    with patch.object(editor,'CLIENT',client),patch.object(editor,'HERE',here),patch.object(editor,'game_running',return_value=False):
        with patch.object(editor.os,'replace',side_effect=fail_once):
            try:editor.apply(payload);raise AssertionError('failure not raised')
            except OSError:pass
        assert all(p.read_bytes()==old for p,old in previous.items())
        checks.append('mid-save failure rolls back every original file')
        assert editor.apply(payload)['ok']
    for entry in json.loads(manifest_path.read_text())['files']:
        data=(repo/entry['source']).read_bytes()
        assert entry['bytes']==len(data) and entry['sha256']==hashlib.sha256(data).hexdigest()
    assert not (here/'backups').exists()
    checks.append('successful save updates texture/settings/manifest; backups outside payload')

for value in [dict(settings,brightness=float('nan')),dict(settings,strength=2),dict(settings,color='../x'),dict(settings,brightness=True)]:
    try:editor.validate(value);raise AssertionError('invalid setting accepted')
    except ValueError:pass
for malformed in [bmp[:-1],b'XX'+bmp[2:],bmp[:18]+struct.pack('<i',512)+bmp[22:]]:
    try:editor.encode(malformed);raise AssertionError('invalid BMP accepted')
    except ValueError:pass
checks.append('invalid settings and malformed native texture rejected')
result=dict(checks=checks,settings=settings,texture_sha256=hashlib.sha256(raw).hexdigest(),actual_game_run=False)
(ROOT/'assets/primordial-baphomet/skin-validation.json').write_text(json.dumps(result,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
print(json.dumps(result,ensure_ascii=False,indent=2))
