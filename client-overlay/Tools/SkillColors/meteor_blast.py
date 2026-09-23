"""Arrival-linked impact blooms and a fast, outward-moving cataclysm front."""
import math,random
from native_vfx import mesh,frames,rgb,envelope

def build(read,out,wed,follow,starts,targets,shots,m):
    rng=random.Random(140602);s=''
    # Multiple crossed flame sheets form an uneven blast flower, not a cylinder.
    vertices=[];indices=[]
    for i in range(11):
        a=i*math.tau/11+rng.uniform(-.1,.1);r=rng.uniform(.35,.7);height=rng.uniform(.4,1.0)
        cx,cz=math.cos(a)*r,math.sin(a)*r
        for turn in (a,a+math.pi/2):
            k=len(vertices);dx,dz=math.cos(turn)*.38,math.sin(turn)*.38
            for x,y,z,u,v in [(-dx,0,-dz,0,1),(dx,0,dz,1,1),(dx,height,dz,1,0),(-dx,height,-dz,0,0)]:vertices.append([cx+x,y,cz+z,u,v])
            indices.extend(k+j for j in [0,1,2,0,2,3,2,1,0,3,2,0])
    mesh(read,out,'mm_shell_bloom.WEM',vertices,indices)
    def bloom(n,fs):
        n[0]='mm_shell_bloom.WEM'
        def update(f,t,j):
            u=min(1,t/450);radius=.3+2.5*(1-(1-u)**2)
            f[:3]=[0,0,0];f[3:7]=[0,1,0,-t*.035];f[7]=1;f[8:11]=[v*m['impactSize'] for v in (1.3+1.5*u,radius,radius)]
            f[11]=rgb('#FFD6AE',envelope(t,1050,35,650)*.68);f[12]=min(16,1+int(t/70))
        return n,frames(fs,1050,update)
    flower=wed('s_magic03_1.wed','mm_shell_bloom.wed',transform=bloom)
    def shell_wave(n,fs):
        def update(f,t,j):
            size=(.12+1.45*(1-(1-min(1,t/380))**2))*m['impactSize']
            f[:3]=[0,0,0];f[3:7]=[1,0,0,270];f[7]=1;f[8:11]=[size]*3
            f[11]=rgb('#FFE1AA',envelope(t,800,35,450)*.8);f[12]=min(9,1+int(t/85))
        return n,frames(fs,800,update)
    ripple=wed('ta_fire.wed','mm_shell_wave.wed',transform=shell_wave)
    for source,target,shot in zip(starts,targets,shots):
        s+=follow(flower,1050,source=source,destination=target,launched=shot)
        s+=follow(ripple,800,source=source,destination=target,launched=shot)
    def pressure(n,fs):
        def update(f,t,j):
            size=(.15+4.7*(1-(1-min(1,t/650))**2))*m['finalImpactSize']
            f[:3]=[0,0,0];f[3:7]=[1,0,0,270];f[7]=1;f[8:11]=[size]*3
            f[11]=rgb('#FFECB8',envelope(t,1150,33,680)*.8);f[12]=min(9,1+int(t/125))
        return n,frames(fs,1150,update)
    pressure_wave=wed('ta_fire.wed','mm_storm_pressure.wed',transform=pressure);s+=follow(pressure_wave,1150)
    for layer,source in enumerate(('firebust03_2.wed','firebust03_3.wed')):
        def rim(n,fs,layer=layer):
            def update(f,t,j):
                age=max(0,t-layer*90);size=(2+48*(1-(1-min(1,age/820))**2))*m['finalImpactSize']
                f[:3]=[0,0,0];f[3:7]=[0,1,0,t*(.045 if layer==0 else -.065)];f[7]=1;f[8:11]=[size]*3
                f[11]=rgb('#FFB965',envelope(age,1550-layer*90,50,800)*.72);f[12]=1
            return n,frames(fs,1550,update)
        name=wed(source,f'mm_storm_rim{layer}.wed',transform=rim);s+=follow(name,1550)
    # A broad rolling wall of fire accelerates out from the final impact.
    # Each lobe follows a curved radial path with independent speed and height.
    fronts=[]
    for i in range(18):
        a=i*math.tau/18+rng.uniform(-.08,.08);reach=rng.uniform(10.8,14.0)*m['finalImpactSize'];delay=rng.randrange(0,110)
        turn=rng.uniform(.19,.44);height=rng.uniform(1.3,2.6)*m['finalImpactSize'];duration=1550
        def front(n,fs,a=a,reach=reach,delay=delay,turn=turn,height=height):
            def update(f,t,j):
                age=max(0,t-delay);u=min(1,age/1000);r=.7*m['finalImpactSize']+reach*(1-(1-u)**2);angle=a+turn*u
                size=(3.7+5.4*min(1,age/320))*m['finalImpactSize'];sy=size*(.85+.32*math.sin(u*math.pi))
                f[7]=1;f[8:11]=[sy,size,size];f[:3]=[math.cos(angle)*r/size,(.35+height*math.sin(u*math.pi))/sy,math.sin(angle)*r/size]
                f[3:7]=[0,1,0,-angle*180/math.pi];f[11]=rgb('#FFD2A0',envelope(age,1550-delay,45,780)*.7);f[12]=min(16,1+int(age/95))
            return n,frames(fs,duration,update)
        name=wed('d_flame.wed',f'mm_stormfront{i}.wed',transform=front);s+=follow(name,duration)
        fronts.append(dict(angle=a,reach=reach,delay=delay))
    # Small hot ejecta travel farther and above the main fire front.
    for i in range(8):
        a=i*math.tau/8+.2;radius=(10.2+(i%3)*1.3)*m['finalImpactSize']
        def ejecta(n,fs,a=a,radius=radius,i=i):
            def update(f,t,j):
                u=min(1,t/1350);r=radius*u;sx=.8*(1-.5*u)*m['finalImpactSize'];sy=sx
                f[7]=1;f[8:11]=[sy,sx,sx];f[:3]=[math.cos(a)*r/sx,(.5+5.5*math.sin(math.pi*u))/sy,math.sin(a)*r/sx]
                f[3:7]=[0,1,0,j*14+i*31];f[11]=rgb('#FFC76B',envelope(t,1450,30,600));f[12]=1
            return n,frames(fs,1450,update)
        w=wed('firebust02.wed',f'mm_ejecta{i}.wed',transform=ejecta);s+=follow(w,1450)
    return s,dict(shellImpactLayers=3,stormFronts=18,stormReach=max(x['reach'] for x in fronts),stormDuration=1550,ejecta=8,pressureWaves=3)
