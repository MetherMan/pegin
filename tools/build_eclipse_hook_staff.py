"""Build only the approved P3 right-hand staff, retaining source pixels and grip."""
from pathlib import Path
import json, shutil, subprocess, sys
from build_noble_moon import R, O, D, digest, polygons_from_source
from mesh_tools import *
from noble_moon_atlas import full_resolution_atlas
from verify_closed_surfaces import audit

REVIEW = O / 'eclipse-hook-staff'
SPEC = REVIEW / 'geometry.json'


def source_polygons(image, spec):
    """Keep fine holes and every polygon after repairing narrow gold filigree."""
    pixels = np.asarray(image)
    h, w = pixels.shape[:2]
    x0, y0, x1, y1 = spec['box']
    zone = np.zeros((h, w), dtype=bool)
    zone[y0:y1, x0:x1] = True
    gray = pixels.max(2)
    gc = np.full((h, w), cv2.GC_BGD, dtype='uint8')
    gc[zone] = cv2.GC_PR_BGD
    gc[zone & (gray > spec['likely_threshold'])] = cv2.GC_PR_FGD
    gc[zone & (gray > spec['solid_threshold'])] = cv2.GC_FGD
    for a, b, c, d in spec.get('background', []):
        gc[b:d, a:c] = cv2.GC_BGD
    cv2.grabCut(pixels, gc, None, np.zeros((1, 65)), np.zeros((1, 65)), 7, cv2.GC_INIT_WITH_MASK)
    mask = ((gc == cv2.GC_FGD) | (gc == cv2.GC_PR_FGD)).astype('uint8')
    close = spec['close_pixels']
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, np.ones((close, close), np.uint8))
    for a, b, c, d in spec.get('exclude', []):
        mask[b:d, a:c] = 0
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE)

    def polygons(geometry):
        if isinstance(geometry, Polygon):
            if geometry.area > spec['minimum_area']:
                yield geometry
        elif hasattr(geometry, 'geoms'):
            for child in geometry.geoms:
                yield from polygons(child)

    result = []
    for index, contour in enumerate(contours):
        if hierarchy[0, index, 3] != -1 or cv2.contourArea(contour) < spec['minimum_area']:
            continue
        outer = cv2.approxPolyDP(contour, spec['outline_error'], True)[:, 0, :]
        holes = []
        child = hierarchy[0, index, 2]
        while child != -1:
            if cv2.contourArea(contours[child]) > spec['minimum_hole']:
                holes.append(cv2.approxPolyDP(contours[child], .45, True)[:, 0, :])
            child = hierarchy[0, child, 0]
        polygon = Polygon(outer, holes)
        # A subpixel inset separates point-touching ornament islands, which
        # otherwise weld into a nonmanifold native edge after vertex sharing.
        for valid in polygons(shapely.make_valid(polygon)):
            result.extend(polygons(valid.buffer(-.025, join_style=2)))
    assert result
    return result


def body_depth(spec, xy, distance):
    base = .0012 + .029 * np.sin(np.minimum(distance / 15, 1) * math.pi / 2)
    # The large moon is a rounded lens, rather than a decal or a flat plate.
    relative = (xy - np.asarray(spec['moon_center'])) / np.asarray(spec['moon_radii'])
    inside = 1 - (relative ** 2).sum(1)
    moon = .002 + spec['moon_half_depth'] * np.sqrt(np.maximum(inside, 0))
    return np.maximum(base, np.where(inside > 0, moon, 0))


def build_staff(spec):
    source = R / spec['source']
    image = Image.open(source).convert('RGB')
    w, h = image.size
    polys = source_polygons(image, spec)
    scale = spec['scale']
    grip = np.asarray(spec['grip'])

    def mapping(points):
        p = np.asarray(points)
        return np.column_stack(((p[:, 0] - grip[0]) * scale,
                                (grip[1] - p[:, 1]) * scale))

    def at(point):
        return [*mapping(np.asarray([point]))[0], 0]

    parts = []
    for index, poly in enumerate(polys):
        face, side = sculpt(poly, mapping, lambda xy, d: body_depth(spec, xy, d),
                            [w, h], step=spec['mesh_step'])
        parts.extend([(f'eclipse staff original face {index}', 0, face),
                      (f'eclipse staff closed blue edge {index}', 1, side)])
    a, b = spec['grip_ends']
    p, n, uv = cylinder(at(a), at(b), *spec['grip_radii'], sides=24, rings=42)
    original_pixels = np.column_stack((p[:, 0] / scale + grip[0],
                                      grip[1] - p[:, 1] / scale))
    parts.append(('round original blue staff shaft', 0,
                  (p, n, original_pixels / np.array([w, h]))))
    x0, y0, x1, y1 = spec['box']
    remapped = []
    for name, material, (p, n, uv) in parts:
        if material == 0:
            uv = (uv * [w, h] - [x0, y0]) / [x1 - x0, y1 - y0]
        remapped.append((name, material, (p, n, uv)))
    images = [image.crop(spec['box']), image.crop(spec['side_material_box'])]
    # Mechanical source extraction only; neither crop is painted or resampled.
    images[0].save(REVIEW / 'source-staff-crop.png')
    write_glb(REVIEW / 'eclipse-hook-staff.glb', remapped, images)
    result = export_native(REVIEW / 'payload', 'mt_staff', remapped, images,
                           R / 'runtime/client/GameClient/Equip/ww_0015_1.mod',
                           atlas_packer=full_resolution_atlas)
    for lod in (1, 2, 3):
        shutil.copy2(REVIEW / f'payload/mt_staff_{lod}.mod', D / f'Equip/mt_staff_{lod}.mod')
    shutil.copy2(REVIEW / 'payload/mt_staff_atlas.wtm', D / 'Texture/Equip/mt_staff_atlas.wtm')
    result.update(kind='staff', source=spec['source'], source_sha256=digest(source),
                  grip=spec['grip'], scale=scale, parts=len(polys),
                  holes=sum(len(p.interiors) for p in polys), source_repainted=False,
                  original_weapon_ids_preserved=True, in_game_visual_test=False)
    return result


def main():
    REVIEW.mkdir(parents=True, exist_ok=True)
    spec = json.loads(SPEC.read_text())['weapons'][0]
    assert spec['kind'] == 'staff'
    protected = []
    for prefix in ('mt_twilight', 'mt_longbow'):
        protected.extend(D / f'Equip/{prefix}_{lod}.mod' for lod in (1, 2, 3))
        protected.append(D / f'Texture/Equip/{prefix}_atlas.wtm')
    protected.extend([O / 'sword-native.glb', O / 'longbow-native.glb',
                      D / 'Item/ITEM.dat', D / 'Interface/item.dat', D / 'Equip/Trace.txt',
                      R / spec['source']])
    before = {str(p.relative_to(R)): digest(p) for p in protected}
    staff = D / 'Equip/mt_staff_1.mod'
    old = read_mod(staff)[0]
    if not (REVIEW / 'prior-staff.json').exists():
        prior = dict(sha256=digest(staff), triangles=len(old['corners']) // 3,
                     bounds=[old['points'].min(0).tolist(), old['points'].max(0).tolist()])
        (REVIEW / 'prior-staff.json').write_text(json.dumps(prior, indent=2) + '\n')
        shutil.copy2(O / 'staff-native.glb', REVIEW / 'prior-staff-native.glb')
    result = build_staff(spec)
    surface = audit(staff)
    assert surface['passed'], surface
    native_mod = read_mod(staff)
    assert len(native_mod) == 1
    raw = (D / 'Texture/Equip/mt_staff_atlas.wtm').read_bytes()
    native = np.asarray(Image.open(BytesIO(zlib.decompress(raw[13:]))).convert('RGB'))
    rect = result['atlas']['material_rects'][0]
    source = np.asarray(Image.open(R / spec['source']).convert('RGB').crop(spec['box']))
    assert np.array_equal(native[rect['y']:rect['y'] + rect['height'],
                                 rect['x']:rect['x'] + rect['width']], source)
    subprocess.run([sys.executable, '-B', '-X', 'utf8', str(O / 'preview_native.py'),
                    '--kind', 'staff'], cwd=R, check=True)
    after = {str(p.relative_to(R)): digest(p) for p in protected}
    assert before == after, 'Sword, bow, item tables and source must remain byte-identical'
    active = json.loads((O / 'active-design.json').read_text(encoding='utf-8'))
    active['staff'] = 'approved eclipse-hook staff from moon-pairs-10 P3'
    active.setdefault('overrides', {})['staff'] = dict(
        design='eclipse-hook-staff', source='../visual-refresh-20260923/moon-pairs-10/pair-03.png',
        geometry_spec='eclipse-hook-staff/geometry.json', builder='tools/build_eclipse_hook_staff.py',
        approved_by_user=True)
    (O / 'active-design.json').write_text(json.dumps(active, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    replacements = [D / f'Equip/mt_staff_{lod}.mod' for lod in (1, 2, 3)] + [D / 'Texture/Equip/mt_staff_atlas.wtm']
    prior = json.loads((REVIEW / 'prior-staff.json').read_text())
    height = result['bounds'][1][0] - result['bounds'][0][0]
    old_height = prior['bounds'][1][0] - prior['bounds'][0][0]
    assert abs(height / old_height - 1) < .02
    report = dict(passed=True, design='eclipse-hook-staff', model=result, surface=surface,
                  native_source_pixels_verified=int(source.shape[0] * source.shape[1]),
                  protected_sha256=after, sword_and_bow_not_regenerated=True,
                  installed_client_written=False, prior=prior,
                  height_ratio_to_prior=height / old_height,
                  replacements=[dict(path=str(p.relative_to(D)).replace('\\', '/'),
                                     sha256=digest(p), bytes=p.stat().st_size) for p in replacements])
    (REVIEW / 'build-validation.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(passed=True, triangles=result['triangles'], parts=result['parts'],
                         holes=result['holes'], height_ratio=report['height_ratio_to_prior'],
                         replacements=report['replacements'], protected_files=len(after))))


if __name__ == '__main__':
    main()
