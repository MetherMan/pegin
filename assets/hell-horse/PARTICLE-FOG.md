# Hoof ember size and fog correction

The shipped `Texture/Particle/pa_05.wtm` contains a 64-by-64, 24-bit RGB BMP.
It has no alpha channel. Its entire outer border, including all four corners,
is exactly black; the artwork itself does not contain a bright square. The
decoded originals used for inspection were written only under ignored
`runtime/performance`; no source texture was edited.

Native particles are always camera-facing billboards. `SetWorld` divides the
0.06-unit quad by `fSize` through homogeneous W. Thus the former .041-.048
values produce a 1.46-1.25-unit square. The horse model's four hoof groups are
only .154-.170 wide and .242-.247 long before animation. A .22-unit quad uses
inverse size .272727; .20-.24 corresponds to .30-.25. The texture's black
padding makes the visible flame smaller than that quad. There is no particle
header option for a horizontal ground plane: `bInverse` reverses movement age,
not orientation. Reusing pa_05 at hoof scale is appropriate for short embers;
pa_02 is a radial star and pa_08/pa_09 are round glows, not small flame shapes.

The native renderer enables fog in both particle passes. The light pass uses
ONE/ONE blending with alpha testing disabled. Fog blends the polygon RGB with
the fog color, including black texels, so a nonblack fog color can illuminate
the whole billboard before additive blending. The fixed-function formula
applies to DirectX 7 and later. See Microsoft's [fog formula documentation](https://learn.microsoft.com/en-us/windows/win32/direct3d9/fog-formulas)
and [blend-factor definitions](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dblend).
This establishes a mechanism for the square artifact; the staged patch still
requires an actual in-game visual check at the reported location.

`tools/patch_hell_particle_fog.py` appends a reversible `.hpfog` hook after the
existing `.icealp` patch. It wraps only each Draw call for the dedicated
`ParticleHead::nTick == 140041` marker, disables fog, draws, and immediately
restores the native TRUE state. It does this in both passes; otherwise even a
black secondary color could darken the fog-colored rectangle. Other effects
receive no additional device calls. Each marked visible particle receives four
additional render-state calls in total, two per pass. Texture, blend, size,
color, draw count, and the world's weather/fog remain unchanged.

The marker's native field is offset +4. The native AddParticle age gate was
executed with 1/16/33/80/200/1000 ms intervals: the marker starts with its full
385 ms lifetime at each interval instead of the ordinary random age reduction.
The existing external movement/contact emitter controls spawning; nTick itself
does not provide a rate limit. Lifetime 385 ms is divisible by 11 and therefore
safe for the native 12-key interpolation's final living millisecond.

The staged validation executes complete native Render, SetRenderState and
CVertexBuffer::Draw paths at two DLL load addresses, with empty, ordinary,
marked, mixed, invisible and adjacent-marker cases. Only the D3D device calls,
particle simulation and camera transform are substituted. Draw arguments,
texture/state-call order outside the scoped fog calls, stack, and nonvolatile
registers are checked against the original DLL. Each marked draw observes fog
FALSE; the following ordinary draw and the function exit observe TRUE. The
generator defaults to `runtime/performance` and does not deploy the DLL.
