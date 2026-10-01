# Hell-horse hoof fire

The former center emitter produced a 1.25-1.46 world-unit billboard every
80 ms and left it alive for 1.76 seconds. Actual sole widths are only
.154-.156 units. This made a chain of oversized fire cards rather than
small marks from the feet. The old render path could also tint black sprite
edges with fog. The shipped pa_05 texture itself has a black border.

## Animated contacts

tools/patch_hell_horse_hoof.py supersedes the center emitter for ride kind 4.
It reads the four actual bone matrices through the native model interface,
transforms a sole center calculated from each hoof mesh into world space,
and queries the ground height at that XY. Each foot is armed only after
its sole rises above .22; it emits once when it descends to .16 or below.
The .22/.16 gap prevents repeated emission while the hoof stays down.
New mounts, stopped movement, changed action, and teleports clear that state.

The real RUN clip is hor_w_r_16.ani (or the bow variant), frames 1-16 at
35 fps. The old preview range 151-167 describes WALK. The hook uses the
model current action and animation, so it does not assume one clip or a
fixed time-based left/right alternation. Ordinary horses retain their
original dust behavior. Previous .htrail/.htdata sections remain for
reversible undo but their center emitter is bypassed.

## Small stationary flames

Only the mt_hell_hoof row is replaced; all other 59 rows are byte-identical.
One flame is .264 world units wide at birth, shrinks to .24, and expires
at 385 ms. This is exactly 1.2 times the previous .22 to .20 width. Its center
is ground height plus .20, leaving its lowest edge at least .068 above
level ground. Its speed, acceleration,
force and secondary tint are zero. The 12 color/size keys safely interpolate
at every living millisecond (385 is divisible by 11).

The renderer uses a camera-facing billboard; this is a tiny upright flame,
not a flat decal. The private nTick=140041 marker identifies only this
particle for the scoped Render.dll fog correction. Fog is disabled during
its two draws and immediately restored; weather and other particles are
unchanged. See PARTICLE-FOG.md for the native render evidence.

## Build and checks

Run tools/patch_hell_horse_hoof.py --apply for the guarded executable patch.
Run tools/patch_hell_particle_fog.py --output client-overlay/Render.dll
--report assets/hell-horse/particle-fog-hook.json for the guarded renderer.
Run tools/build_hell_horse_sparks.py to rebuild the single particle row.
Run tools/build_hoof_fire_preview.py to export existing native textures
and the same particle parameters for the browser preview.

The hoof hook validation covers native bone/matrix calls, contact phases,
stopped movement and ABI preservation. hoof-motion.json records the actual
mesh-derived sole points and general/bow run/walk contact phases. The fog
validation checks complete native Render call sequences at two load bases,
including marked/unmarked particles and immediate state restoration.

The preview at assets/hell-horse/hoof-fire-preview uses the actual horse
mesh, texture, 35 fps run animation, sole coordinates and particle keys.
It is a browser reconstruction, not an in-game capture. Final appearance
on game terrain must still be checked in the running game.
