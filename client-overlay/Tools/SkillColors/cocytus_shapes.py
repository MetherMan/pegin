"""Native geometry for broken ice ridges, freezing ground, and snow squalls."""
import math
import random
from native_vfx import mesh


def crag(read, out, name, seed):
    """One uneven shell, so additive faces do not stack into a white block."""
    rng = random.Random(seed)
    sides = 5 + seed % 3
    angles = [i * math.tau / sides + rng.uniform(-.10, .10) for i in range(sides)]
    radii = [rng.uniform(.66, 1.15) for _ in angles]
    lean = (rng.uniform(-2.3, 2.3), rng.uniform(-2.3, 2.3))
    shoulder = [rng.uniform(5.3, 7.8) for _ in angles]
    vertices, indices = [], []
    for i, a in enumerate(angles):
        j = (i + 1) % sides
        b = angles[j]
        p = (math.cos(a) * radii[i], 0, math.sin(a) * radii[i])
        q = (math.cos(b) * radii[j], 0, math.sin(b) * radii[j])
        p1 = (p[0] * .74 + lean[0] * .45, shoulder[i], p[2] * .74 + lean[1] * .45)
        q1 = (q[0] * .74 + lean[0] * .45, shoulder[j], q[2] * .74 + lean[1] * .45)
        tip = (lean[0], 10.2, lean[1])
        k = len(vertices)
        u = .07 + (i % 3) * .28
        for v, uv in [(p, (u, .98)), (p1, (u, .26)), (q1, (u + .24, .26)), (q, (u + .24, .98)), (tip, (u + .12, .02))]:
            vertices.append([*v, *uv])
        indices.extend(k + n for n in [0, 1, 2, 0, 2, 3, 1, 4, 2])
    mesh(read, out, name, vertices, indices)


def ring_crystal(read, out, name, seed):
    """Broken faceted shells have different lean, shoulders and broad/narrow faces."""
    rng = random.Random(seed)
    vertices, indices = [], []
    sides = 5 + seed % 3
    angles = [i * math.tau / sides + rng.uniform(-.17, .17) for i in range(sides)]
    radii = [rng.uniform(.58, 1.32) for _ in angles]
    lean = rng.uniform(1.1, 4.6)
    lean_angle = rng.uniform(0, math.tau)
    tip = (math.cos(lean_angle)*lean, 10.2, math.sin(lean_angle)*lean)
    shoulder = [rng.uniform(3.6, 8.8) for _ in angles]
    for i, a in enumerate(angles):
        j = (i + 1) % sides; b = angles[j]; k = len(vertices)
        p = (math.cos(a)*radii[i], 0, math.sin(a)*radii[i])
        q = (math.cos(b)*radii[j], 0, math.sin(b)*radii[j])
        pp = (p[0]*.84+tip[0]*.45, shoulder[i], p[2]*.84+tip[2]*.45)
        qq = (q[0]*.84+tip[0]*.45, shoulder[j], q[2]*.84+tip[2]*.45)
        u = .06 + (i % 3)*.27
        for v, uv in [(p,(u,.98)),(pp,(u,.25)),(qq,(u+.25,.25)),(q,(u+.25,.98)),(tip,(u+.12,.02))]:
            vertices.append([*v,*uv])
        indices.extend(k+n for n in [0,1,2,0,2,3,1,4,2])
    mesh(read,out,name,vertices,indices)


def snow_cloud(read, out, name, seed, extent, count=96):
    """Crossed, slanted ice flecks remain visible from the game's fixed view."""
    rng = random.Random(seed)
    vertices, indices = [], []
    for flake in range(count):
        a = rng.random() * math.tau
        r = extent * rng.uniform(.34, 1.10)
        x, z = math.cos(a) * r, math.sin(a) * r
        y = rng.uniform(.25, 8.0)
        # Short, broad flecks survive the actual camera distance. A minority of
        # longer tails supply motion without turning all the snow into hair.
        length = rng.uniform(.11, .24) if flake % 4 else rng.uniform(.29, .42)
        width = rng.uniform(.055, .115)
        for cross in (0, math.pi / 2):
            theta = a + cross
            dx, dz = math.cos(theta), math.sin(theta)
            k = len(vertices)
            for along, across, uv in [(-length, 0, (.12, .80)), (0, -width, (.37, .28)), (length, 0, (.48, .28)), (0, width, (.23, .80))]:
                vertices.append([x + dx * along, y + along * .32 + across, z + dz * along, *uv])
            indices.extend(k + n for n in [0, 1, 2, 0, 2, 3, 2, 1, 0, 3, 2, 0])
    mesh(read, out, name, vertices, indices)


def squall_arc(read, out, name, seed, extent):
    """An incomplete curved gust, not a stack of full cylindrical rings."""
    rng = random.Random(seed)
    vertices, indices = [], []
    sweep = rng.uniform(1.5, 2.6)
    radius = extent * rng.uniform(.61, .95)
    height = rng.uniform(1.1, 2.5)
    slope = rng.uniform(1.8, 2.6)
    for i in range(33):
        u = i / 32
        a = u * sweep
        r = radius * (1 + .08 * math.sin(u * 7))
        h = height + u * slope
        for dy, v in ((0, 1), (1.65 * math.sin(math.pi * u) ** .8 + .025, 0)):
            vertices.append([math.cos(a) * r, h + dy, math.sin(a) * r, u, v])
        if i:
            k = (i - 1) * 2
            indices.extend(k + n for n in [0, 2, 3, 0, 3, 1, 3, 2, 0, 1, 3, 0])
    mesh(read, out, name, vertices, indices)


def frozen_band(read, out, name, band, bands, extent):
    """Uneven angular ice patches avoid concentric freeze rings."""
    vertices, indices = [], []
    start = band * math.tau / bands + .12 * math.sin(band * math.tau / bands * 2)
    stop = (band + 1) * math.tau / bands + .12 * math.sin((band + 1) * math.tau / bands * 2)
    for i in range(25):
        a = start + (stop - start) * i / 24
        wobble = 1 + .065 * math.sin(a * 5 + .7) + .045 * math.sin(a * 11 + 1.3)
        for fraction in (0, 1):
            r = extent * fraction * wobble
            x, z = math.cos(a) * r, math.sin(a) * r
            # Keep the original black rim at the irregular geometric boundary;
            # scaling UVs by the larger wobble extent clips into bright ice.
            vertices.append([x, 0, z, .5 + math.cos(a) * fraction / 2, .5 + math.sin(a) * fraction / 2])
        if i:
            k = (i - 1) * 2
            indices.extend(k + n for n in [0, 3, 1, 1, 3, 0])
    mesh(read, out, name, vertices, indices)


def mist_planes(read, out, name):
    """Three offset smoke sheets use the unchanged native bsmoke texture."""
    vertices,indices=[],[]
    for angle,height,offset in [(0,1.2,-.15),(math.pi/2,.9,.12),(.65,.65,.32)]:
        dx,dz=math.cos(angle),math.sin(angle);k=len(vertices)
        for x,y,z,u,v in [(-dx,0,-dz,0,1),(dx,0,dz,1,1),(dx,height,dz,1,0),(-dx,height,-dz,0,0)]:
            vertices.append([x,y+.05,z+offset,u,v])
        indices.extend(k+j for j in [0,1,2,0,2,3,2,1,0,3,2,0])
    mesh(read,out,name,vertices,indices)


def monolith(read, out, name, rim_name):
    """Broad dark-blue faces with a separate narrow luminous edge shell."""
    vertices,indices=[],[];edges=[];sides=5
    radii=[1.15,.92,1.08,.86,1.03];heights=[7.6,6.8,7.3,8.0,6.6]
    base=[(math.cos(i*math.tau/sides)*radii[i],0,math.sin(i*math.tau/sides)*radii[i]) for i in range(sides)]
    shoulder=[(p[0]*.84+.12,heights[i],p[2]*.84-.08) for i,p in enumerate(base)]
    tip=(.32,10.2,-.23)
    for i in range(sides):
        j=(i+1)%sides;k=len(vertices);u=.06+(i%3)*.27
        for p,uv in [(base[i],(u,.98)),(shoulder[i],(u,.25)),(shoulder[j],(u+.25,.25)),(base[j],(u+.25,.98)),(tip,(u+.12,.02))]:vertices.append([*p,*uv])
        indices.extend(k+n for n in [0,1,2,0,2,3,1,4,2]);edges.extend([(base[i],shoulder[i]),(shoulder[i],tip)])
    mesh(read,out,name,vertices,indices)
    vertices,indices=[],[]
    for p,q in edges:
        for axis in [(1,0,0),(0,0,1)]:
            k=len(vertices);w=.012
            for v,sign in [(p,-1),(p,1),(q,1),(q,-1)]:vertices.append([v[j]+axis[j]*w*sign for j in range(3)]+[.48,.38])
            indices.extend(k+n for n in [0,1,2,0,2,3,2,1,0,3,2,0])
    mesh(read,out,rim_name,vertices,indices)
