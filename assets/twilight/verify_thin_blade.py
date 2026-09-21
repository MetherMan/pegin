"""Check the shipped MOD, not just the editable model, for thin textured edges."""
from pathlib import Path
from io import BytesIO
import hashlib, json, struct, zlib
import numpy as np
from PIL import Image
from verify_native_animation import read_mod

ROOT = Path(__file__).resolve().parents[2]
OUT = Path(__file__).resolve().parent
CLIENT = ROOT / 'runtime/client/GameClient'

def blade_thickness(path):
    points = np.concatenate([p['points'] for p in read_mod(path)])
    # Exclude the grip and pommel; original weapons point along native +X.
    blade = points[points[:, 0] > .45]
    assert len(blade)
    return float(np.ptp(blade[:, 2]))

references = {name: blade_thickness(CLIENT / 'Equip' / model) for name, model in [
    ('red_knight_greatsword', 'ws_0076_1.mod'),
    ('black_knight_greatsword', 'ws_0081_1.mod'),
]}
thickness = blade_thickness(ROOT / 'client-overlay/Equip/mt_twilight_1.mod')
assert thickness < .005 and thickness < min(references.values())
parts = read_mod(ROOT / 'client-overlay/Equip/mt_twilight_1.mod')
assert len(parts) == 1 and parts[0]['texture'] == 'mt_twilight_atlas.bmp'
part = parts[0]
tri = part['points'][part['corners']['index']].reshape(-1, 3, 3)
uv = part['corners']['uv'].reshape(-1, 3, 2)
normal = part['corners']['normal'].reshape(-1, 3, 3)
side = (tri[:, :, 0].mean(axis=1) > .45) & (np.abs(normal[:, :, 2]).mean(axis=1) < .9)
assert side.sum() > 0
du, dv = uv[:, 1] - uv[:, 0], uv[:, 2] - uv[:, 0]
area = np.abs(du[:, 0] * dv[:, 1] - du[:, 1] * dv[:, 0]) / 2
assert (area[side] > 1e-13).all(), 'A side face has collapsed UVs'
encoded = (ROOT / 'client-overlay/Texture/Equip/mt_twilight_atlas.wtm').read_bytes()
pixels = np.asarray(Image.open(BytesIO(zlib.decompress(encoded[13:]))).convert('RGB'))
samples = uv[side].mean(axis=1)
xy = np.clip((samples * pixels.shape[0]).astype(int), 0, pixels.shape[0]-1)
colors = pixels[xy[:, 1], xy[:, 0]]
assert float(colors.std()) > 5, 'Side texture has no visible variation'
assert float(colors.mean()) < 200, 'Side texture washed out to white'
for lod in (2, 3):
    assert (ROOT / f'client-overlay/Equip/mt_twilight_{lod}.mod').read_bytes() == (ROOT / 'client-overlay/Equip/mt_twilight_1.mod').read_bytes()
result = dict(passed=True, native_blade_thickness=thickness, reference_blade_thickness=references,
              textured_side_triangles=int(side.sum()), minimum_side_uv_area=float(area[side].min()),
              side_texture_mean=float(colors.mean()), side_texture_std=float(colors.std()),
              native_sha256=hashlib.sha256((ROOT/'client-overlay/Equip/mt_twilight_1.mod').read_bytes()).hexdigest(),
              preview_front_and_side_reviewed=True, in_game_visual_test=False)
(OUT / 'thin_blade_validation.json').write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
print(json.dumps(result))
