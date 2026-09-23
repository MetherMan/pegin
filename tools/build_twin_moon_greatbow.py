"""Build the selected swept-back 06 longbow with bounded native vertex cost.

Only the approved source's geometry extraction, lossless crops and UV packing
are used. The hand pivot and item bindings remain those of the existing bow.
"""
import json
import shutil
from build_noble_moon import R, O, D, digest
from build_eclipse_hook_staff import source_polygons
from build_h03_horn_bow import smooth, rounded_depth, native_preview
from mesh_tools import *
from noble_moon_atlas import full_resolution_atlas
from verify_closed_surfaces import audit

REVIEW = O / 'twin-moon-greatbow'
SPEC = REVIEW / 'geometry.json'


def build_bow(spec=None):
    REVIEW.mkdir(parents=True, exist_ok=True)
    spec = spec or json.loads(SPEC.read_text(encoding='utf-8'))['weapons'][0]
    assert spec['kind'] == 'longbow'
    source = R / spec['source']
    protected = [source, O / 'sword-native.glb', O / 'staff-native.glb',
                 D / 'Item/ITEM.dat', D / 'Interface/item.dat', D / 'Equip/Trace.txt']
    for prefix in ('mt_twilight', 'mt_staff'):
        protected += [D / f'Equip/{prefix}_{lod}.mod' for lod in (1, 2, 3)]
        protected += [D / f'Texture/Equip/{prefix}_atlas.wtm']
    before = {str(p.relative_to(R)): digest(p) for p in protected}
    oldpath = D / 'Equip/mt_longbow_1.mod'
    if not (REVIEW / 'prior-bow.json').exists():
        old = read_mod(oldpath)[0]
        prior = dict(sha256=digest(oldpath), triangles=len(old['corners']) // 3,
                     bounds=[old['points'].min(0).tolist(), old['points'].max(0).tolist()])
        (REVIEW / 'prior-bow.json').write_text(json.dumps(prior, indent=2) + '\n')
        shutil.copy2(O / 'longbow-native.glb', REVIEW / 'prior-bow-native.glb')
    prior = json.loads((REVIEW / 'prior-bow.json').read_text())
    image = Image.open(source).convert('RGB')
    w, h = image.size
    # Stable segmentation makes rebuilds independent of earlier OpenCV calls.
    cv2.setRNGSeed(0)
    polygons = source_polygons(image, spec)
    grip, scale = np.asarray(spec['grip']), spec['scale']

    def mapping(points):
        p = np.asarray(points)
        y = (grip[1] - p[:, 1]) * scale
        length = np.abs(y)
        cutoff = spec['grip_half_length']
        factor = np.where(y >= 0, spec['upper_length_factor'], spec['lower_length_factor'])
        y = np.sign(y) * (np.minimum(length, cutoff) + np.maximum(length - cutoff, 0) * factor)
        return np.column_stack(((p[:, 0] - grip[0]) * scale, y))

    def image_pixels(points):
        y = points[:, 1]
        cutoff = spec['grip_half_length']
        factor = np.where(y >= 0, spec['upper_length_factor'], spec['lower_length_factor'])
        y = np.sign(y) * (np.minimum(np.abs(y), cutoff) + np.maximum(np.abs(y) - cutoff, 0) / factor)
        return np.column_stack((points[:, 0] / scale + grip[0], grip[1] - y / scale))

    def at(point):
        return [*mapping(np.asarray([point]))[0], 0]

    parts, contours = [], []
    for index, poly in enumerate(polygons):
        depth = rounded_depth(poly, (w, h), spec)
        face, side = sculpt(poly, mapping, lambda xy, d: np.minimum(depth(xy, d), spec['maximum_half_depth']),
                            [w, h], step=spec['mesh_step'])
        parts += [(f'06 swept crescent face {index}', 0, smooth(face)),
                  (f'06 closed crystal edge {index}', 1, smooth(side))]
        contours.append(dict(exterior=np.asarray(poly.exterior.coords).tolist(),
                             holes=[np.asarray(r.coords).tolist() for r in poly.interiors]))
    a, b = spec['grip_ends']
    p, n, uv = cylinder(at(a), at(b), *spec['grip_radii'],
                        sides=spec['grip_sides'], rings=spec['grip_rings'])
    pixels = image_pixels(p)
    parts.append(('round blue hand grip', 0, smooth((p, n, pixels / [w, h]))))
    a, b = spec['string_ends']
    parts.append(('continuous moonlight string', 2,
                  cylinder(at(a), at(b), .00125, .00125, sides=6)))
    x0, y0, x1, y1 = spec['box']
    remapped = []
    for name, material, (p, n, uv) in parts:
        # Scale about the existing hand pivot. Thin the sculpted body only;
        # the grip and string retain their circular cross-sections.
        axes = np.full(3, spec.get('overall_scale', 1.0))
        if name.startswith('06 '):
            axes[2] *= spec.get('body_side_scale', 1.0)
        p = p * axes
        n = n / axes
        n /= np.maximum(np.linalg.norm(n, axis=1)[:, None], 1e-12)
        if material == 0:
            uv = (uv * [w, h] - [x0, y0]) / [x1 - x0, y1 - y0]
        remapped.append((name, material, (p, n, uv)))
    images = [image.crop(spec['box']), image.crop(spec['side_material_box']),
              image.crop(spec['string_material_box'])]
    images[0].save(REVIEW / 'source-bow-crop.png')
    (REVIEW / 'source-contours.json').write_text(json.dumps(contours, separators=(',', ':')) + '\n')
    write_glb(REVIEW / 'twin-moon-greatbow.glb', remapped, images)
    payload = REVIEW / 'payload'
    result = export_native(payload, 'mt_longbow', remapped, images,
                           R / 'runtime/client/GameClient/Equip/wb_0040_1.mod',
                           atlas_packer=full_resolution_atlas)
    mod, texture = payload / 'mt_longbow_1.mod', payload / 'mt_longbow_atlas.wtm'
    surface = audit(mod)
    assert surface['passed'], surface
    assert result['triangles'] <= spec['triangle_budget'], result['triangles']
    native = np.asarray(Image.open(BytesIO(zlib.decompress(texture.read_bytes()[13:]))).convert('RGB'))
    for material, im in enumerate(images):
        rect = result['atlas']['material_rects'][material]
        assert np.array_equal(native[rect['y']:rect['y'] + rect['height'],
                                     rect['x']:rect['x'] + rect['width']], np.asarray(im))
    assert len(read_mod(mod)) == 1
    height = result['bounds'][1][0] - result['bounds'][0][0]
    prior_height = prior['bounds'][1][0] - prior['bounds'][0][0]
    ratio = height / prior_height
    assert spec['height_ratio_min'] <= ratio <= spec['height_ratio_max'], ratio
    assert np.allclose(mapping([grip]), [[0, 0]])
    native_preview(mod, texture, REVIEW / 'twin-moon-greatbow-native.glb')
    result.update(kind='longbow', source=spec['source'], source_sha256=digest(source), grip=spec['grip'],
                  scale=scale, parts=len(polygons), holes=sum(len(p.interiors) for p in polygons),
                  upper_length_factor=spec['upper_length_factor'], lower_length_factor=spec['lower_length_factor'],
                  overall_scale=spec.get('overall_scale', 1.0), body_side_scale=spec.get('body_side_scale', 1.0),
                  source_repainted=False, in_game_visual_test=False)
    replacements = []
    for lod in (1, 2, 3):
        dest = D / f'Equip/mt_longbow_{lod}.mod'
        shutil.copy2(payload / dest.name, dest)
        replacements.append(dest)
    dest = D / 'Texture/Equip/mt_longbow_atlas.wtm'
    shutil.copy2(texture, dest)
    replacements.append(dest)
    shutil.copy2(REVIEW / 'twin-moon-greatbow-native.glb', O / 'longbow-native.glb')
    after = {str(p.relative_to(R)): digest(p) for p in protected}
    assert before == after, 'Protected weapon files or item bindings changed'
    report = dict(passed=True, design='twin-moon-greatbow-06', model=result, surface=surface,
                  prior=prior, height_ratio_to_prior=ratio, grip_origin_preserved=True,
                  protected_sha256=after, installed_client_written=False,
                  native_corners_before=prior['triangles'] * 3,
                  native_corners_after=result['triangles'] * 3,
                  native_corner_reduction=1 - result['triangles'] / prior['triangles'],
                  source_pixels_verified=images[0].width * images[0].height,
                  replacements=[dict(path=str(p.relative_to(D)).replace('\\', '/'),
                                     sha256=digest(p), bytes=p.stat().st_size) for p in replacements])
    (REVIEW / 'build-validation.json').write_text(json.dumps(report, indent=2) + '\n')
    active = json.loads((O / 'active-design.json').read_text(encoding='utf-8'))
    active['sources'][1] = '../' + spec['source'].removeprefix('assets/')
    active['overrides']['longbow'] = dict(design='twin-moon-greatbow-06',
        source=active['sources'][1], geometry_spec='twin-moon-greatbow/geometry.json',
        builder='tools/build_twin_moon_greatbow.py', approved_by_user=True)
    (O / 'active-design.json').write_text(json.dumps(active, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ('passed', 'height_ratio_to_prior',
          'native_corners_before', 'native_corners_after', 'native_corner_reduction')}))
    return result


if __name__ == '__main__':
    build_bow()
