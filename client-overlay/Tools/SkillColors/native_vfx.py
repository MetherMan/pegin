"""Small native mesh/frame helpers shared by the two private spell builders."""
import math,struct

# Stationary [ENEMY] layers use a reserved, otherwise unused SPEED marker.
# Engine.dll keeps these layers level; flight PARTs retain their real speed.
GROUND_SPEED = 140031

def mesh(read,out,name,vertices,indices):
    out['Effect/'+name]=read('Effect/pl_06.WEM')[:100]+struct.pack('<3i',1,len(indices)//3,len(vertices))+b''.join(struct.pack('<5f',*v) for v in vertices)+struct.pack('<i',len(indices))+struct.pack('<'+'H'*len(indices),*indices)+bytes(40)

def rotate(p,axis,degrees):
    length=math.sqrt(sum(x*x for x in axis));a=[x/length for x in axis];t=math.radians(degrees);c,s=math.cos(t),math.sin(t);dot=sum(x*y for x,y in zip(a,p))
    cross=[a[1]*p[2]-a[2]*p[1],a[2]*p[0]-a[0]*p[2],a[0]*p[1]-a[1]*p[0]]
    return [p[k]*c+cross[k]*s+a[k]*dot*(1-c) for k in range(3)]

def glyph(read,out,name,layer,axis=(1,0,0),tilt=0):
    vertices=[];indices=[]
    def point(r,a):return (r*math.cos(a),r*math.sin(a))
    def line(a,b,width):
        width*=1.45
        dx,dz=b[0]-a[0],b[1]-a[1];length=math.hypot(dx,dz)
        if length<1e-9:return
        nx,nz=-dz/length*width,dx/length*width;start=len(vertices)
        for x,z in [(a[0]+nx,a[1]+nz),(a[0]-nx,a[1]-nz),(b[0]-nx,b[1]-nz),(b[0]+nx,b[1]+nz)]:vertices.append([*rotate((x,0,z),axis,tilt),.5,.5])
        indices.extend(start+i for i in [0,1,2,0,2,3,2,1,0,3,2,0])
    for r in ([1,.88] if layer==0 else [1]):
        for i in range(72):line(point(r,i*math.tau/72),point(r,(i+1)*math.tau/72),.008)
    if layer==0:
        for i in range(24):
            a=i*math.tau/24;line(point(.9,a),point(.98,a+.03),.013);line(point(.93,a),point(.98,a+.08),.009)
    else:
        count,skip=(5,2) if layer==1 else (3,1)
        for i in range(count):line(point(.92,i*math.tau/count),point(.92,(i+skip)*math.tau/count),.016)
        for i in range(12):line(point(.92,i*math.tau/12),point(1.07,i*math.tau/12),.012)
    mesh(read,out,name,vertices,indices)
    return rotate((0,1,0),axis,tilt)

def rgb(color,gain=1):
    c=[max(0,min(255,round(int(color[i:i+2],16)*gain))) for i in (1,3,5)]
    return 0xff000000|(c[0]<<16)|(c[1]<<8)|c[2]

def frames(source,life,update):
    result=[]
    for i in range(math.ceil(life*.03)+2):
        f=source[i%max(1,len(source)-1)].copy();update(f,i*1000/30,i);result.append(f)
    return result

def envelope(t,life,attack=180,release=500):return max(0,min(1,t/attack,(life-t)/release))

def plane(read,out,name,axis=(1,0,0),tilt=0):
    vertices=[[*rotate((x,0,z),axis,tilt),u,v] for x,z,u,v in [(-1,-1,0,0),(1,-1,1,0),(1,1,1,1),(-1,1,0,1)]]
    mesh(read,out,name,vertices,[0,1,2,0,2,3,2,1,0,3,2,0])
    return rotate((0,1,0),axis,tilt)

def ice_crystal(read,out,name):
    # A single shell preserves facets instead of overlapping additive planes.
    vertices=[];indices=[];sides=7
    for i in range(sides):
        a=i*math.tau/sides;b=(i+1)*math.tau/sides
        a0=(math.cos(a),0,math.sin(a));b0=(math.cos(b),0,math.sin(b))
        a1=(math.cos(a)*.83,7.2+.35*math.sin(a*2),math.sin(a)*.83)
        b1=(math.cos(b)*.83,7.2+.35*math.sin(b*2),math.sin(b)*.83)
        tip=(.25,10.2,-.18);k=len(vertices);u=.08+(i%3)*.27
        for p,uv in [(a0,(u,.98)),(a1,(u,.22)),(b1,(u+.23,.22)),(b0,(u+.23,.98)),(tip,(u+.115,.02))]:vertices.append([*p,*uv])
        indices.extend(k+j for j in [0,1,2,0,2,3,1,4,2])
    mesh(read,out,name,vertices,indices)

def annulus(read,out,name,outer,inner,extent):
    vertices=[];indices=[]
    for i in range(65):
        a=i*math.tau/64
        for r in (inner,outer):
            x,z=r*math.cos(a),r*math.sin(a);vertices.append([x,0,z,.5+x/(2*extent),.5+z/(2*extent)])
        if i:
            k=(i-1)*2;indices.extend(k+j for j in [0,2,3,0,3,1,3,2,0,1,3,0])
    mesh(read,out,name,vertices,indices)

def flame_funnel(read,out,name):
    # Twisted tapered surface carrying the original six-frame firewall atlas.
    vertices=[];indices=[];rings=14;sectors=36
    for j in range(rings+1):
        h=j/rings;r=.26+1.2*h**1.35
        for i in range(sectors+1):
            a=i*math.tau/sectors+h*3.8;wobble=1+.08*math.sin(a*3+h*9)
            vertices.append([r*math.cos(a)*wobble,h*5.5,r*math.sin(a)*wobble,i/sectors*2,1-h])
            if j and i:
                k=(j-1)*(sectors+1)+i-1
                indices.extend(k+q for q in [0,sectors+1,sectors+2,0,sectors+2,1,sectors+2,sectors+1,0,1,sectors+2,0])
    mesh(read,out,name,vertices,indices)

def arcane_texture(read,out,prefix,source):
    """Reuse a native rune atlas, retaining luminance for selectable tint."""
    import zlib
    name=prefix+'_'+source;raw=read('Texture/Effect/'+source+'.wtm')
    bmp=bytearray(zlib.decompress(raw[13:]));off=struct.unpack_from('<I',bmp,10)[0]
    width,height=struct.unpack_from('<ii',bmp,18);bits=struct.unpack_from('<H',bmp,28)[0]
    if bits==8:
        begin=14+struct.unpack_from('<I',bmp,14)[0]
        for at in range(begin,off,4):bmp[at:at+3]=bytes([max(bmp[at:at+3])])*3
    else:
        assert bits in (24,32);stride=((width*bits+31)//32)*4
        for y in range(abs(height)):
            for x in range(width):
                at=off+y*stride+x*(bits//8);bmp[at:at+3]=bytes([max(bmp[at:at+3])])*3
    out['Texture/Effect/'+name+'.wtm']=raw[:9]+struct.pack('<I',len(bmp))+zlib.compress(bmp,9)
    return name+'.bmp'

def flame_plume(read,out,name):
    # Native Hell fire atlas on crossed planes distributed along a helix.
    vertices=[];indices=[]
    for j in range(18):
        h=j/17;a=h*math.tau*1.1;r=.18+.9*h
        center=(math.cos(a)*r,h*4.8,math.sin(a)*r);width=.65+.58*h;height=2.4
        for plane_angle in (a,a+math.pi/2):
            k=len(vertices);dx,dz=math.cos(plane_angle)*width,math.sin(plane_angle)*width
            for x,y,z,u,v in [(-dx,0,-dz,0,1),(dx,0,dz,1,1),(dx,height,dz,1,0),(-dx,height,-dz,0,0)]:
                vertices.append([center[0]+x,center[1]+y,center[2]+z,u,v])
            indices.extend(k+i for i in [0,1,2,0,2,3,2,1,0,3,2,0])
    mesh(read,out,name,vertices,indices)
