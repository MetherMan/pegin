"""Golden Knight (Celestial Wings) weapons as 3D preview models (no game files yet).

Same method as the shipped Twilight weapons (tools/build_noble_moon.py): the approved single-weapon
artwork is cut out, every silhouette polygon becomes a closed body with real thickness and rounded
edges, the art is projected on both faces, the grip (and the bow string) are round cylinders.
Lengths match the current Twilight sword / longbow / staff so they sit in the hand the same way.
Spec: assets/golden-knight/models/geometry.json. Output: assets/golden-knight/models/<kind>.glb.
"""
from pathlib import Path
import json, sys

R = Path(__file__).resolve().parents[1]
sys.path[:0] = [str(R/'.cache/weapon-build-deps'), str(R/'runtime/pylibs'), str(R/'assets/twilight-set'),
                str(R/'assets/twilight'), str(R/'tools')]
from build_noble_moon import polygons_from_source, surface_depth
from mesh_tools import sculpt, cylinder, write_glb, np, Image

O = R/'assets/golden-knight/models'
SPEC = O/'geometry.json'


def parts_for(spec):
    """(named parts with material 0 face / 1 side / 2 string, images, scale) for one weapon."""
    image = Image.open(R/spec['source'])
    w, h = image.size
    rgb = image.convert('RGB')
    polys = polygons_from_source(image, spec)
    x0, y0, x1, y1 = spec['box']
    scale = spec['length']/(spec['tip_bottom'][1]-spec['tip_bottom'][0])
    spec = dict(spec, scale=scale)
    grip = np.asarray(spec['grip'], float)

    def xy_map(a):
        a = np.asarray(a)
        return np.column_stack(((a[:, 0]-grip[0])*scale, (grip[1]-a[:, 1])*scale))

    def at(point):
        return [*xy_map(np.asarray([point]))[0], 0]
    depth_kind = 'sword' if spec['kind'] == 'sword' else 'longbow'
    parts = []
    for i, poly in enumerate(polys):
        face, side = sculpt(poly, xy_map, lambda xy, d: surface_depth(depth_kind, spec, xy, d)*spec.get('depth_factor', 1),
                            [w, h], step=spec.get('mesh_step', 16))
        parts.extend([(f'{spec["kind"]} face {i}', 0, face), (f'{spec["kind"]} side {i}', 1, side)])
    if spec.get('grip_ends'):
        a, b = spec['grip_ends']
        p, n, u = cylinder(at(a), at(b), *spec.get('grip_radii', [.02, .02]), rings=16)
        pixel = np.column_stack((p[:, 0]/scale+grip[0], grip[1]-p[:, 1]/scale))
        parts.append(('round grip', 0, (p, n, pixel/np.array([w, h]))))
    if spec.get('string_ends'):
        a, b = spec['string_ends']
        parts.append(('bow string', 2, cylinder(at(a), at(b), .0013, .0013, sides=8)))
    remapped = []
    for name, material, (p, n, u) in parts:
        if material == 0:
            u = (u*np.array([w, h])-np.array([x0, y0]))/np.array([x1-x0, y1-y0])
        remapped.append((name, material, (p, n, u)))
    images = [rgb.crop(spec['box']), Image.new('RGB', (8, 8), tuple(spec.get('side_rgb', [214, 178, 96]))),
              Image.new('RGB', (8, 8), (250, 236, 196))]
    return remapped, images, scale, polys


def build(spec):
    remapped, images, scale, polys = parts_for(spec)
    write_glb(O/f'{spec["kind"]}.glb', remapped, images)
    points = np.concatenate([p for _, _, (p, _, _) in remapped])
    return dict(kind=spec['kind'], polygons=len(polys), holes=sum(len(p.interiors) for p in polys),
                triangles=sum(len(p)//3 for _, _, (p, _, _) in remapped), size=np.ptp(points, 0).round(3).tolist())


def main():
    specs = json.loads(SPEC.read_text(encoding='utf-8'))['weapons']
    kinds = set(sys.argv[1:])
    report = [build(s) for s in specs if not kinds or s['kind'] in kinds]
    print(json.dumps(report))


if __name__ == '__main__':
    main()
