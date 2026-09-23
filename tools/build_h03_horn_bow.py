"""Build only the approved H03 bow with original pixels and closed native volume.

No installed client, sibling weapon, shared viewer or manifest is written.
The only image operations are segmentation, original source crops and lossless
UV atlas packing; the approved illustration itself is never repainted.
"""
from pathlib import Path
import json, shutil, sys
from build_noble_moon import R, O, D, digest
from build_eclipse_hook_staff import source_polygons
from mesh_tools import *
from noble_moon_atlas import full_resolution_atlas
from verify_closed_surfaces import audit

REVIEW = O / 'h03-horn-bow'
SPEC = REVIEW / 'geometry.json'


def smooth(mesh):
    """Area-weighted normals keep the curved crystal skin visually continuous."""
    p, _, uv = mesh
    t = p.reshape(-1, 3, 3)
    weighted = np.repeat(np.cross(t[:, 1] - t[:, 0], t[:, 2] - t[:, 0]), 3, axis=0)
    _, ids = np.unique(np.round(p, 9), axis=0, return_inverse=True)
    sums = np.zeros((ids.max() + 1, 3))
    np.add.at(sums, ids, weighted)
    n = sums[ids]
    n /= np.maximum(np.linalg.norm(n, axis=1)[:, None], 1e-12)
    return p, n, uv


def rounded_depth(poly, image_size, spec):
    """Give each narrowing horn a rounded cross-section, not a flat plate."""
    w, h = image_size
    mask = np.zeros((h, w), np.uint8)
    cv2.fillPoly(mask, [np.asarray(poly.exterior.coords, np.int32)], 1)
    for ring in poly.interiors:
        cv2.fillPoly(mask, [np.asarray(ring.coords, np.int32)], 0)
    distance = cv2.distanceTransform(mask, cv2.DIST_L2, cv2.DIST_MASK_PRECISE)
    radius = cv2.dilate(distance, np.ones((45, 45), np.uint8))

    def depth(xy, d):
        px = np.clip(np.rint(xy[:, 0]).astype(int), 0, w - 1)
        py = np.clip(np.rint(xy[:, 1]).astype(int), 0, h - 1)
        r = np.maximum(radius[py, px], d)
        half = .0011 + .9 * spec['scale'] * np.sqrt(np.maximum(d * (2 * r - d), 0))
        # A small solid lunar lens remains faithful to the original open C.
        q = (xy - spec['moon_center']) / spec['moon_radii']
        inside = 1 - (q * q).sum(1)
        lens = .0011 + spec['moon_half_depth'] * np.sin(np.minimum(d / 10, 1) * math.pi / 2)
        return np.maximum(half, np.where(inside > 0, lens, 0))
    return depth


def native_preview(mod, texture, target):
    """Read back actual MOD/WTM bytes independently of the authoring meshes."""
    parts, images = [], []
    for part in read_mod(mod):
        p = part['points'][part['corners']['index']]
        n, uv = part['corners']['normal'], part['corners']['uv']
        p = np.column_stack((-p[:, 1], p[:, 0], p[:, 2]))
        n = np.column_stack((-n[:, 1], n[:, 0], n[:, 2]))
        tri = p.reshape(-1, 3, 3)
        dots = (np.cross(tri[:, 1] - tri[:, 0], tri[:, 2] - tri[:, 0]) * n.reshape(-1, 3, 3).mean(1)).sum(1)
        if np.median(dots) < 0:
            p = p.reshape(-1, 3, 3)[:, [0, 2, 1]].reshape(-1, 3)
            n = n.reshape(-1, 3, 3)[:, [0, 2, 1]].reshape(-1, 3)
            uv = uv.reshape(-1, 3, 2)[:, [0, 2, 1]].reshape(-1, 2)
        raw = texture.read_bytes()
        images.append(Image.open(BytesIO(zlib.decompress(raw[13:]))).convert('RGB'))
        parts.append(('H03 from native MOD and WTM', len(images) - 1, (p, n, uv)))
    write_glb(target, parts, images)


def build(spec):
    image = Image.open(R / spec['source']).convert('RGB')
    w, h = image.size
    polys = source_polygons(image, spec)
    grip, scale = np.asarray(spec['grip']), spec['scale']

    def mapping(points):
        p = np.asarray(points)
        return np.column_stack(((p[:, 0] - grip[0]) * scale, (grip[1] - p[:, 1]) * scale))

    def at(point):
        return [*mapping(np.asarray([point]))[0], 0]

    parts = []
    contours = []
    for i, poly in enumerate(polys):
        face, side = sculpt(poly, mapping, rounded_depth(poly, (w, h), spec), [w, h], step=spec['mesh_step'])
        parts.extend([(f'H03 original crystal horn face {i}', 0, smooth(face)),
                      (f'H03 closed blue side {i}', 1, smooth(side))])
        contours.append(dict(exterior=np.asarray(poly.exterior.coords).tolist(),
                             holes=[np.asarray(r.coords).tolist() for r in poly.interiors]))
    a, b = spec['grip_ends']
    p, n, uv = cylinder(at(a), at(b), *spec['grip_radii'], sides=32, rings=24)
    pixels = np.column_stack((p[:, 0] / scale + grip[0], grip[1] - p[:, 1] / scale))
    parts.append(('H03 round original blue handgrip', 0, smooth((p, n, pixels / [w, h]))))
    a, b = spec['string_ends']
    parts.append(('H03 single continuous moonlight string', 2, cylinder(at(a), at(b), .00125, .00125, sides=10)))
    x0, y0, x1, y1 = spec['box']
    remapped = []
    for name, material, (p, n, uv) in parts:
        if material == 0:
            uv = (uv * [w, h] - [x0, y0]) / [x1 - x0, y1 - y0]
        remapped.append((name, material, (p, n, uv)))
    images = [image.crop(spec['box']), image.crop(spec['side_material_box']), image.crop(spec['string_material_box'])]
    images[0].save(REVIEW / 'source-bow-crop.png')
    write_glb(REVIEW / 'h03-horn-bow.glb', remapped, images)
    result = export_native(REVIEW / 'payload', 'mt_longbow', remapped, images,
                           R / 'runtime/client/GameClient/Equip/wb_0040_1.mod',
                           atlas_packer=full_resolution_atlas)
    (REVIEW / 'source-contours.json').write_text(json.dumps(contours, separators=(',', ':')) + '\n')
    result.update(kind='longbow', source=spec['source'], source_sha256=digest(R / spec['source']),
                  grip=spec['grip'], scale=scale, parts=len(polys),
                  holes=sum(len(p.interiors) for p in polys), source_repainted=False,
                  original_weapon_id_preserved=True, in_game_visual_test=False)
    return result, images


def build_bow(spec=None):
    REVIEW.mkdir(parents=True, exist_ok=True)
    spec = spec or json.loads(SPEC.read_text(encoding='utf-8'))['weapons'][0]
    assert spec['kind'] == 'longbow'
    protected = [R / spec['source'], O / 'sword-native.glb', O / 'staff-native.glb',
                 D / 'Item/ITEM.dat', D / 'Interface/item.dat', D / 'Equip/Trace.txt']
    for prefix in ('mt_twilight', 'mt_staff'):
        protected.extend(D / f'Equip/{prefix}_{lod}.mod' for lod in (1, 2, 3))
        protected.append(D / f'Texture/Equip/{prefix}_atlas.wtm')
    before = {str(p.relative_to(R)): digest(p) for p in protected}
    oldpath = D / 'Equip/mt_longbow_1.mod'
    old = read_mod(oldpath)[0]
    if not (REVIEW / 'prior-bow.json').exists():
        prior = dict(sha256=digest(oldpath), triangles=len(old['corners']) // 3,
                     bounds=[old['points'].min(0).tolist(), old['points'].max(0).tolist()])
        (REVIEW / 'prior-bow.json').write_text(json.dumps(prior, indent=2) + '\n')
        shutil.copy2(O / 'longbow-native.glb', REVIEW / 'prior-bow-native.glb')
    result, images = build(spec)
    payload = REVIEW / 'payload'
    mod, tex = payload / 'mt_longbow_1.mod', payload / 'mt_longbow_atlas.wtm'
    surface = audit(mod)
    assert surface['passed'], surface
    native = np.asarray(Image.open(BytesIO(zlib.decompress(tex.read_bytes()[13:]))).convert('RGB'))
    for material, image in enumerate(images):
        rect = result['atlas']['material_rects'][material]
        actual = native[rect['y']:rect['y'] + rect['height'], rect['x']:rect['x'] + rect['width']]
        assert np.array_equal(actual, np.asarray(image)), material
    assert len(read_mod(mod)) == 1
    assert len({digest(payload / f'mt_longbow_{lod}.mod') for lod in (1, 2, 3)}) == 1
    prior = json.loads((REVIEW / 'prior-bow.json').read_text())
    height = result['bounds'][1][0] - result['bounds'][0][0]
    ratio = height / (prior['bounds'][1][0] - prior['bounds'][0][0])
    assert abs(ratio - 1) < .02, ratio
    native_preview(mod, tex, REVIEW / 'h03-horn-bow-native.glb')
    # Ship only after topology, pixel preservation and native readback pass.
    replacements = []
    for lod in (1, 2, 3):
        dest = D / f'Equip/mt_longbow_{lod}.mod'
        shutil.copy2(payload / dest.name, dest)
        replacements.append(dest)
    dest = D / 'Texture/Equip/mt_longbow_atlas.wtm'
    shutil.copy2(tex, dest)
    replacements.append(dest)
    shutil.copy2(REVIEW / 'h03-horn-bow-native.glb', O / 'longbow-native.glb')
    after = {str(p.relative_to(R)): digest(p) for p in protected}
    assert before == after, 'Sibling weapons, source, item bindings and shared configuration must remain unchanged'
    report = dict(passed=True, design='approved-h03-horn-bow', model=result, surface=surface,
                  source_pixels_verified=images[0].width * images[0].height,
                  all_three_native_lods_identical=True, protected_sha256=after,
                  grip_origin_preserved=True, installed_client_written=False,
                  previous=prior, height_ratio_to_prior=ratio,
                  replacements=[dict(path=str(p.relative_to(D)).replace('\\', '/'),
                                     sha256=digest(p), bytes=p.stat().st_size) for p in replacements])
    (REVIEW / 'build-validation.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(passed=True, triangles=result['triangles'], parts=result['parts'],
                         holes=result['holes'], height_ratio=ratio, replacements=report['replacements'])))
    return result


if __name__ == '__main__':
    build_bow()
