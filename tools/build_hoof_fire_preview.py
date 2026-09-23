"""Export existing native textures and particle parameters for the hoof preview."""
from pathlib import Path
import hashlib
import json
import struct
import sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'tools'), str(R/'client-overlay/Tools/SkillColors')]
from native_assets import png_from_wtm
from build_hell_horse_sparks import build_table, records, name, PARTICLE_NAME

out = R/'assets/hell-horse/hoof-fire-preview'
out.mkdir(exist_ok=True)
texture_hashes = {}
for source, dest in [('Texture/Vehicle/mt_hellhorse.wtm', 'horse.png'),
                     ('Texture/particle/pa_05.wtm', 'flame.png')]:
    p = R/'client-overlay'/source
    if not p.exists():
        p = R/'runtime/client/GameClient'/source
    raw = p.read_bytes()
    (out/dest).write_bytes(png_from_wtm(raw))
    texture_hashes[source] = hashlib.sha256(raw).hexdigest()
raw = build_table((R/'client-overlay/Effect/particle.ptc').read_bytes())
row = next(r for r in records(raw) if name(r) == PARTICLE_NAME)
params = dict(lifetimeMs=struct.unpack_from('<i', row, 137)[0],
              keys=[list(struct.unpack_from('<7f', row, 425+i*28)) for i in range(12)],
              landingHeight=.16, liftHeight=.22, groundCenterHeight=.20,
              speedWorldPerSecond=10, textureHashes=texture_hashes,
              note='Browser reconstruction from native mesh, run clip and particle record; not an in-game capture.')
(out/'particle.json').write_text(json.dumps(params, indent=2)+'\n', encoding='utf-8')
print(json.dumps(dict(output=str(out), **params)))
