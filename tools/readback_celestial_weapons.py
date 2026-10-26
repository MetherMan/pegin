"""Read the exported 천상의 날개 game files (Equip/cw_*_1.mod + Texture/Equip/cw_*_atlas.wtm) back into
assets/golden-knight/models/<kind>-native.glb, oriented like the Twilight native previews, so the
preview page can show exactly what the client loads."""
from pathlib import Path
from io import BytesIO
import sys, zlib
R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'runtime/pylibs'), str(R/'tools'), str(R/'client-overlay/Tools/SkillColors')]
from PIL import Image
from build_golden_knight_weapons import meshes, write_glb

D = R/'client-overlay'
for kind, prefix in (('sword', 'cw_sword'), ('longbow', 'cw_longbow'), ('staff', 'cw_staff')):
    chunks = meshes((D/f'Equip/{prefix}_1.mod').read_bytes())
    atlas = Image.open(BytesIO(zlib.decompress((D/f'Texture/Equip/{prefix}_atlas.wtm').read_bytes()[13:]))).convert('RGB')
    write_glb(R/f'assets/golden-knight/models/{kind}-native.glb', [(0, (p, n, u)) for _, p, n, u in chunks], [atlas])
    print(kind, sum(len(p) for _, p, _, _ in chunks)//3, 'triangles', atlas.size)
