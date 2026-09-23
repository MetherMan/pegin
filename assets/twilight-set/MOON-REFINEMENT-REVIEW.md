# Moon emblem and sword bevel refinement

The approved shattered-moon artwork remains the source. No image generation or
raster repainting was used for this refinement. Changes are native geometry,
material assignments, and lossless source-region UV repacking.

## What changed

- Sword: the central crescent is 15% larger, raised above its silver mounting,
  and follows a narrow blue bevel. Its face still samples the approved artwork.
- Bow: the central crescent is 20% larger with the same blue bevel treatment.
- Sword blade: the former short, rounded bevel (`distance / 14`) made almost
  all of the blade share the guard's thickness. The new long bevel has a thin
  cutting rim, decreases depth toward the point, and reduces the distal outline
  width by up to 22% while preserving the irregular teeth and blue cutting edge.
- The sword and bow artwork now each occupies its own face atlas tile. Previously
  the entire three-weapon concept, including the title, occupied each face tile.
- Staff geometry and texture bytes are unchanged.

## Validation

`tools/verify_moon_refinement.py` reads the exported MOD rather than the design
parameters. The main blade has full depth 0.04431236 model units, decreasing to
0.01395191 near the point. 272 blade vertices belong to a cutting rim thinner
than 0.0007 model units. These measurements exclude the grip and raised emblem.

`verify_closed_surfaces.py` passed: no open, nonmanifold, or reversed seams.
`verify_assets.py` passed: native UV/material checks, one animation mesh,
identical LOD copies, and all 42 item records remain correctly linked.

The native model readback contains 11,452 sword triangles and 7,964 bow triangles.
The staff remains 5,988 triangles and MOD SHA-256
`0b7519a444d1cbe695df29aabe27bd6194c6726994c38daa90eed08fca2be323`.

## Visual evidence

- `../visual-refresh-20260923/moon-refined-sword-native.png`
- `../visual-refresh-20260923/moon-refined-sword-bevel-native.png`
- `../visual-refresh-20260923/moon-refined-bow-native.png`

These are browser renders of actual MOD/WTM readback. They are not Direct3D
in-game captures. The normal-distance actor preview was regenerated from the
current overlay and checked at 140px character height on dark and bright
backgrounds. Its scale is for comparison, not an actual game camera setting.

## Local installation

After checking that the game was closed, the parent task backed up and installed
the eight sword/bow resources in both the existing family client and repository
runtime mirror (16 files total). All 36 protected launcher, settings, executable,
table and staff files remained identical. The installation is recorded in
`moon-refinement-installation.json`; its backup directory is
`work/laqia-runtime/backups/moon-refinement-20260923-133842` under the existing
family installation. Skills remain preview-only. No account, DB or VM changes
were made, and actual game rendering/performance remains unverified.

Rebuild only the requested weapons:

```powershell
.\runtime\python\python.exe -B -X utf8 tools/build_shattered_moon.py --kind sword --kind longbow
.\runtime\python\python.exe -B -X utf8 assets/twilight-set/preview_native.py
```
