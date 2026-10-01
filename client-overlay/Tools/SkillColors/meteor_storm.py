"""Eight tilted triple portals, native artillery, then a separate final impact."""
import math
from native_vfx import plane,flame_plume,flame_funnel,arcane_texture,mesh,frames,rgb,envelope,GROUND_SPEED
from skill_tuning_frames import shade_generated

def build(read,out,wed,part,effect,sound,blow,m,color):
    starts=[(-4.8,2.1,8.4),(3.7,-3.6,10.2),(-1.9,-4.7,7.8),(4.6,2.8,9.1),(-4.0,-2.4,11.0),(1.4,4.2,8.7),(3.0,.5,11.6),(-.8,1.8,9.9)]
    starts=[(x*m['portalSpread']+m['portalOffsetX'],y*m['portalSpread']+m['portalOffsetY'],z+m['portalHeight']) for x,y,z in starts]
    targets=[(2.7,-1.8),(-1.1,-3.3),(3.4,2.0),(-3.2,-1.7),(.8,3.4),(1.9,.6),(-.5,1.6),(-2.8,1.3)]
    first=7*m['portalGap']+m['readyHold'];shots=[first+round(v*m['shotGap']) for v in (0,1.4,2.9,4.5,6.4,8.1,10.6,13.0)]
    targetz=max(.08,m['impactHeight'])
    def travel(start,target,speed=None):return math.ceil(math.dist(start,(*target,targetz))/((speed or m['flightSpeed'])/1000))*1000/30
    barrage_end=round(max(t+travel(p,q) for t,p,q in zip(shots,starts,targets)))
    final_seal=barrage_end+m['finalSealDelay'];last=final_seal+m['finalFallDelay'];final_start=(.3+m['finalPortalOffsetX'],-.4+m['finalPortalOffsetY'],14.0+m['finalPortalHeight'])
    final_speed=max(1,round(m['flightSpeed']*m['finalFlightSpeed']))
    impact=round(last+travel(final_start,(0,0),final_speed));fade=impact+2100;end=impact+3800
    s=sound('mb_0004_A.wav')
    seal_textures=[arcane_texture(read,out,'mm',n) for n in ('jin03_2','jin02','jin05')]
    def anchor(x,y,z,at,life):return part(f'[ENEMY]\n[SPEED] {GROUND_SPEED}\n[TARGET] {x:.5f} {y:.5f} {z:.5f}\n[STARTTIME] {at}\n[TIMELIMIT] {life}')
    def seal(name,layer,center,radius,axis,tilt,at,life):
        normal=plane(read,out,name+'.WEM',axis,tilt)
        def transform(n,fs):
            n[0]=name+'.WEM'
            n[1]=seal_textures[layer]
            def update(f,t,i):
                f[:3]=[0,0,0];f[3:7]=[*normal,(1 if layer!=1 else -1)*i*.8+layer*29];f[7]=1
                f[8:11]=[radius*(.65+.35*min(1,t/300))]*3;f[11]=rgb('#FFAD57' if layer!=1 else color,envelope(t,life,170,320)*.9);f[12]=1
            return n,frames(fs,life,update)
        w=wed('hell02.wed',name+'.wed',color,transform=transform)
        x,y,z=center;offset=layer*.55
        return effect(w,anchor(x+normal[0]*offset,y+normal[2]*offset,z+normal[1]*offset,at,life))
    for i,start in enumerate(starts):
        tx,ty=targets[i];dx,dy=start[0]-tx,start[1]-ty
        length=math.hypot(dx,dy);axis=(dy/length,0,-dx/length) if length>1e-6 else (1,0,0)
        tilt=math.degrees(math.atan2(length,start[2]-targetz))
        for layer,radius in enumerate((2.05,1.65,1.25)):
            at=i*m['portalGap']+layer*45;life=shots[i]+320-at
            s+=seal(f'mm_battery_{i}_{layer}',layer,start,radius*(1+(i%3-1)*.12)*m['portalSize'],axis,tilt,at,life)
    for layer,radius in enumerate((6.7,5.2,3.7)):
        s+=seal(f'mm_final_seal{layer}',layer,(final_start[0],final_start[1],final_start[2]-1),radius*m['finalPortalSize'],(1,0,0),8,final_seal+layer*100,impact+750-final_seal-layer*100)
    # Preserve the original high-level fireball geometry, texture and rotation.
    cannon=wed('fireball01_2.wed','mm_cannon.wed',scale=1.25*m['projectileSize'],origin=(0,0,0))
    final=wed('fireball01_2.wed','mm_final_fireball.wed',scale=3.7*m['finalProjectileSize'],origin=(0,0,0))
    def explosion(n,fs,large=False):
        duration=1250 if large else 950
        def update(f,t,i):
            grow=.3+1.25*(1-(1-min(1,t/(110 if large else 90)))**2);size=(18 if large else 6.2)*grow*(m['finalImpactSize'] if large else m['impactSize'])
            f[:3]=[0,.12,0];f[3:7]=[0,1,0,i*3];f[7]=1;f[8:11]=[size]*3
            f[11]=rgb('#FFE0A3',envelope(t,duration,32,650)*.86);f[12]=min(16,1+int(t*16/duration))
        return n,frames(fs,duration,update)
    hit=wed('d_flame.wed','mm_cannon_hit.wed',transform=explosion)
    heavy=wed('d_flame.wed','mm_final_hit.wed',transform=lambda n,fs:explosion(n,fs,True))
    def expand(n,fs):
        def update(f,t,i):
            f[:3]=[0,0,0];f[7]=1;f[8:11]=[(1+54*(1-(1-min(1,t/620))**2))*m['finalImpactSize']]*3;f[11]=rgb('#FFB344',envelope(t,1250,35,700)*.8)
        return n,frames(fs,1250,update)
    shock=wed('pajang02.wed','mm_final_wave.wed','#FFB344',transform=expand)
    def flight(w,start,target,at,tail,damage=False,heavy_shot=False):
        x,y,z=start;tx,ty=target
        p=part(f'[NOSHOW]\n[ENEMY]\n[TARGET] {x} {y} {z}\n[STARTTIME] {at-80}\n[TIMELIMIT] 80')
        p+=part(f'[ATTACK]\n[TARGET] {tx} {ty} {targetz}\n[MOVETYPE] 1\n[TIMELIMIT] 0\n[SPEED] {final_speed if heavy_shot else m["flightSpeed"]}')
        p+=part(f'[NOSHOW]\n[ENEMY]\n[SPEED] {GROUND_SPEED}\n[TARGET] {tx} {ty} {m["impactHeight"]}\n[POS] 0 0 {m["impactHeight"]-targetz}\n'+(blow('mm_meteor_hit.wav')+'\n' if damage else '[SOUND] 0 mm_meteor_hit.wav 3\n')+f'[TAIL] {tail}\n[TIMELIMIT] {1250 if heavy_shot else 950}')
        return effect(w,p,True)
    for i,(start,target,at) in enumerate(zip(starts,targets,shots)):
        s+=sound('mm_meteor_launch.wav',at)+flight(cannon,start,target,at,hit,i<6)
    s+=sound('mm_meteor_launch.wav',last)+flight(final,final_start,(0,0),last,heavy,True,True)
    # Followers use the same native flight and trigger at actual arrival,
    # including when the victim moves; they carry no damage event.
    def follow(tail,life,offset=(0,0,.07),source=final_start,destination=(0,0),launched=last):
        x,y,z=source;dx,dy=destination;ox,oy,tz=offset;tx,ty=dx+ox,dy+oy
        p=part(f'[NOSHOW]\n[ENEMY]\n[TARGET] {x} {y} {z}\n[STARTTIME] {launched-80}\n[TIMELIMIT] 80')
        speed=final_speed if source==final_start and launched==last else m['flightSpeed']
        p+=part(f'[NOSHOW]\n[ATTACK]\n[TARGET] {dx} {dy} {targetz}\n[MOVETYPE] 1\n[TIMELIMIT] 0\n[SPEED] {speed}')
        p+=part(f'[NOSHOW]\n[ENEMY]\n[SPEED] {GROUND_SPEED}\n[TARGET] {tx} {ty} {tz}\n[POS] {ox} {oy} {tz-targetz}\n[TAIL] {tail}\n[TIMELIMIT] {life}')
        return effect(None,p)
    s+=follow(shock,1250)
    from meteor_blast import build as build_blast
    blast,blast_meta=build_blast(read,out,wed,follow,starts,targets,shots,m)
    s+=blast
    flame_plume(read,out,'mm_fire_funnel.WEM')
    flame_funnel(read,out,'mm_fire_sheath.WEM')
    for i in range(6):
        def vortex(n,fs,i=i,sheath=False):
            n[0]='mm_fire_sheath.WEM' if sheath else 'mm_fire_funnel.WEM'
            def update(f,t,j):
                a=i*math.tau/6+t*.0008;r=(2.8+2.4*min(1,t/1900))*m['finalImpactSize'];grow=(.2+.8*min(1,t/420))*m['finalImpactSize']
                sx,sy,sz=.95*grow,(.85+.08*(i%3))*grow,.95*grow;f[7]=1;f[8:11]=[sy,sx,sz]
                f[:3]=[math.cos(a)*r/sx,0,math.sin(a)*r/sz];f[3:7]=[0,1,0,-t*.5+i*47]
                f[11]=rgb('#FFD6A0',envelope(t,2400,250,700)*(.75 if sheath else .55));f[12]=1+(j//2+i)%(6 if sheath else 16)
            return n,frames(fs,2400,update)
        w=wed('s_magic03_1.wed',f'mm_fire_vortex{i}.wed',transform=vortex)
        s+=follow(w,2400)
        w=wed('firewall01.wed',f'mm_fire_vortex_sheath{i}.wed',transform=lambda n,fs,i=i:vortex(n,fs,i,True))
        s+=follow(w,2400)
    # Project-bound ImageGen additive floor decal, in a native horizontal mesh.
    read('Texture/Effect/mm_magma_surface.wtm')
    mesh(read,out,'mm_magma.WEM',[[-1,0,-1,0,0],[1,0,-1,1,0],[1,0,1,1,1],[-1,0,1,0,1]],[0,1,2,0,2,3,2,1,0,3,2,0])
    def magma(n,fs):
        n[0]='mm_magma.WEM';n[1]='mm_magma_surface.bmp'
        def update(f,t,i):
            f[:3]=[0,0,0];f[3:7]=[0,1,0,0];f[7]=1;f[8:11]=[7.2*min(1,.4+t/550)*m['finalImpactSize']]*3
            f[11]=rgb('#FFFFFF',envelope(t,3800,120,1700)*(.9+.08*math.sin(t*.012)));f[12]=1
        return n,frames(fs,3800,update)
    floor=wed('fireball01.wed','mm_magma.wed',transform=magma);s+=follow(floor,3800)
    shade_generated(out,set(),'mm_',m['brightness'],m['intensity'],core=('mm_cannon.wed','mm_final_fireball.wed','mm_cannon_hit.wed','mm_final_hit.wed'))
    return s,dict(first=first,last=last,fadeStart=fade,fadeEnd=end,motionEnd=1500,impactHeight=m['impactHeight'],finalFlightSpeed=final_speed,finalSealDelay=m['finalSealDelay'],finalFallDelay=m['finalFallDelay'],finalFlightDuration=impact-last,layout='artillery-cataclysm-stormblast',batteries=8,seals=24,finalSeals=3,finalSealAt=final_seal,finalImpact=impact,barrageEnd=barrage_end,majorMeteors=9,damageEvents=7,vortices=6,magmaDuration=3800,**blast_meta)
