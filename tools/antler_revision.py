"""Curved skin-rooted antlers and a volume reconstruction of the chosen cleaver."""
import math
from horn_surface_uv import horn_tube

def sub(a,b):return [x-y for x,y in zip(a,b)]
def add(a,b):return [x+y for x,y in zip(a,b)]
def mul(a,t):return [x*t for x in a]
def dot(a,b):return sum(x*y for x,y in zip(a,b))
def unit(a):return mul(a,1/max(1e-12,math.sqrt(dot(a,a))))
def cross(a,b):return [a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]]

def smooth(path,radii,steps=3):
    points=[];rs=[]
    for i in range(len(path)-1):
        a,b,c,d=path[max(0,i-1)],path[i],path[i+1],path[min(len(path)-1,i+2)]
        for j in range(steps):
            t=j/steps
            points.append([.5*(2*b[k]+(-a[k]+c[k])*t+(2*a[k]-5*b[k]+4*c[k]-d[k])*t*t+(-a[k]+3*b[k]-3*c[k]+d[k])*t*t*t) for k in range(3)])
            rs.append(radii[i]*(1-t)+radii[i+1]*t)
    return points+[path[-1]],rs+[radii[-1]]

def closest(p,a,b,c):
    ab,ac,ap=sub(b,a),sub(c,a),sub(p,a);d1,d2=dot(ab,ap),dot(ac,ap)
    if d1<=0 and d2<=0:return a
    bp=sub(p,b);d3,d4=dot(ab,bp),dot(ac,bp)
    if d3>=0 and d4<=d3:return b
    vc=d1*d4-d3*d2
    if vc<=0 and d1>=0 and d3<=0:return add(a,mul(ab,d1/(d1-d3)))
    cp=sub(p,c);d5,d6=dot(ab,cp),dot(ac,cp)
    if d6>=0 and d5<=d6:return c
    vb=d5*d2-d1*d6
    if vb<=0 and d2>=0 and d6<=0:return add(a,mul(ac,d2/(d2-d6)))
    va=d3*d6-d5*d4
    if va<=0 and d4-d3>=0 and d5-d6>=0:return add(b,mul(sub(c,b),(d4-d3)/((d4-d3)+(d5-d6))))
    den=va+vb+vc
    if abs(den)<1e-12:return a
    return add(a,add(mul(ab,vb/den),mul(ac,vc/den)))

def sculpt_body(body,tube):
    horn_ids=set(range(145,217));assert all(body['bones'][i]==5 for i in horn_ids)
    body['corners']=[v for i in range(0,len(body['corners']),3) for v in (body['corners'][i:i+3] if not any(q[0] in horn_ids for q in body['corners'][i:i+3]) else [])]
    body['corners']=[list(v) for v in body['corners']]
    for v in body['corners']:v[4]*=.5
    skin=[]
    for i in range(0,len(body['corners']),3):
        cc=body['corners'][i:i+3];bones={body['bones'][v[0]] for v in cc}
        pts=[body['points'][v[0]] for v in cc];normal=unit([sum(v[k] for v in cc) for k in (1,2,3)])
        skin.append((pts,bones,normal))
    # A shared density and a distinct crop per branch keep grain in proportion.
    roots=[];uv_reports=[];uv=(.52,.96,.98,.06)
    def horn(path,radii,bone,anchored=False):
        if anchored:
            candidates=[]
            for tri,bones,n in skin:
                if bone not in bones:continue
                q=closest(path[0],*tri);candidates.append((dot(sub(q,path[0]),sub(q,path[0])),q,n))
            dist,q,n=min(candidates,key=lambda x:x[0]);radius=radii[0]
            # Hidden root cap, a skin-level collar, then a tapered emergence.
            path=[add(q,mul(n,-radius*1.5)),add(q,mul(n,-.035)),add(q,mul(n,.085))]+path[1:]
            radii=[radius*.88,radius,radius*.86]+radii[1:]
            roots.append(dict(bone=bone,surface=q,root=path[0],inset=radius*1.5,requested_surface_distance=round(math.sqrt(dist),5)))
        p,r=smooth(path,radii)
        uv_reports.append(horn_tube(body,tube,p,r,bone,uv,8,'body-horn-'+str(len(uv_reports))))
    # Each antler bends back, rises, and curls forward; the two sides differ.
    right=[(.27,4.61,.19),(.58,4.82,.33),(.90,4.86,.62),(1.30,5.00,.76),(1.59,5.31,.61),(1.56,5.62,.28),(1.30,5.76,-.14)]
    left=[(-.27,4.59,.19),(-.54,4.78,.26),(-.90,4.92,.54),(-1.31,5.17,.57),(-1.48,5.48,.30),(-1.35,5.66,-.04),(-1.10,5.62,-.35)]
    horn(right,[.16,.145,.12,.093,.058,.031,.002],5,True)
    horn(left,[.15,.135,.112,.085,.056,.027,.002],5,True)
    branches=[
        ([(.60,4.83,.35),(.66,5.12,.29),(.89,5.39,.06),(1.13,5.42,-.15)],[.086,.072,.036,.002]),
        ([(1.18,4.96,.72),(1.54,4.90,.91),(1.88,5.08,.79),(2.04,5.32,.51)],[.082,.065,.032,.002]),
        ([(.47,4.75,.29),(.64,4.68,-.04),(.94,4.73,-.31),(1.05,4.91,-.44)],[.075,.058,.03,.002]),
        ([(-.60,4.82,.33),(-.68,5.10,.34),(-.90,5.26,.13),(-1.06,5.22,-.10)],[.087,.065,.037,.002]),
        ([(-1.03,5.0,.57),(-1.36,4.93,.86),(-1.72,5.05,.91),(-1.95,5.24,.71)],[.08,.062,.031,.002]),
        ([(-1.35,5.26,.50),(-1.61,5.42,.59),(-1.79,5.73,.38),(-1.76,5.90,.12)],[.067,.045,.02,.002]),
        ([(-.48,4.72,.23),(-.53,4.82,-.16),(-.74,5.04,-.37),(-.91,5.09,-.34)],[.07,.052,.025,.002]),
    ]
    for p,r in branches:horn(p,r,5)
    growths=[
        (7,[(.90,3.83,.36),(1.02,4.13,.53),(1.30,4.31,.46),(1.53,4.25,.27)], [.18,.125,.065,.002]),
        (7,[(1.54,3.65,-.015),(1.68,3.87,-.20),(1.82,3.96,-.22)],[.105,.058,.002]),
        (12,[(-.92,3.86,.36),(-1.05,4.19,.61),(-.97,4.43,.72),(-.73,4.46,.65)],[.17,.12,.058,.002]),
        (12,[(-1.41,3.59,.44),(-1.66,3.64,.62),(-1.82,3.85,.63)],[.10,.065,.002]),
        (8,[(2.06,3.58,.28),(2.12,3.77,.50),(2.38,3.83,.67),(2.52,3.73,.66)],[.125,.088,.04,.002]),
        (13,[(-2.30,3.44,-.02),(-2.33,3.56,-.23),(-2.14,3.78,-.35)],[.12,.075,.002]),
        (13,[(-1.88,3.61,.28),(-1.93,3.77,.42),(-2.08,3.81,.46)],[.07,.042,.002]),
        (18,[(.61,.90,.47),(.78,1.02,.66),(.96,1.06,.69)],[.125,.065,.002]),
        (23,[(-.66,.61,-.06),(-.85,.68,-.20),(-.93,.91,-.27)],[.11,.066,.002]),
        (22,[(-.64,1.34,.40),(-.83,1.51,.56),(-.76,1.70,.57)],[.12,.058,.002]),
        (3,[(.14,4.02,.52),(.24,4.25,.80),(.44,4.33,1.04)],[.125,.08,.002]),
        (3,[(-.25,3.72,.62),(-.45,3.84,.84),(-.61,3.78,1.00)],[.135,.075,.002]),
        (3,[(.37,3.27,.57),(.57,3.38,.76),(.66,3.57,.86)],[.105,.067,.002]),
    ]
    for bone,p,r in growths:horn(p,r,bone,True)
    return dict(anchored_roots=roots,branched_antlers=2,antler_branches=len(branches),body_horn_growths=len(growths),asymmetric_growths=True,curved_centerlines=True,original_ram_horns_removed=True,crown_horns=0,cavity_is_through_hole=False,dripping_strands=0,horn_uv=uv_reports)

def triangulate(poly):
    def orient(a,b,c):return (b[0]-a[0])*(c[1]-a[1])-(b[1]-a[1])*(c[0]-a[0])
    area=sum(a[0]*b[1]-a[1]*b[0] for a,b in zip(poly,poly[1:]+poly[:1]));ids=list(range(len(poly)))
    if area<0:ids.reverse()
    triangles=[]
    while len(ids)>3:
        for i,b in enumerate(ids):
            a,c=ids[i-1],ids[(i+1)%len(ids)]
            if orient(poly[a],poly[b],poly[c])<=1e-9:continue
            if any(min(orient(poly[a],poly[b],poly[k]),orient(poly[b],poly[c],poly[k]),orient(poly[c],poly[a],poly[k]))>=-1e-7 for k in ids if k not in (a,b,c)):continue
            triangles.append((a,b,c));ids.pop(i);break
        else:raise AssertionError('Cleaver outline is not a simple polygon')
    return triangles+[tuple(ids)]

def build_cleaver(c,face,tube,roll_degrees=180):
    c['points']=[];c['bones']=[];c['corners']=[];c['texture']='mt_prime_cleaver.wtm'
    uv_reports=[]
    scale=.0033;x0,y0=-2.92578,3.26140
    def pos(p,depth=0):return [x0+(p[1]-418)*scale,y0+depth,(p[0]-1200)*scale]
    def uv(p):return [p[0]/2048,p[1]/1024]
    outline=[(34,340),(156,359),(197,376),(274,379),(344,392),(423,400),(505,407),(591,420),(651,405),(722,398),(782,379),(836,356),(890,352),(944,378),(995,421),(1006,514),(984,597),(1001,646),(975,647),(960,630),(925,625),(864,622),(809,630),(754,637),(721,648),(695,641),(685,619),(644,626),(607,655),(568,647),(521,644),(488,620),(444,628),(393,641),(346,660),(297,645),(290,622),(258,625),(229,596),(202,590),(176,560),(157,558),(143,538),(113,526),(79,500),(57,491),(47,450)]
    tri=triangulate(outline);center=[sum(p[k] for p in outline)/len(outline) for k in (0,1)]
    inner=[(center[0]+(p[0]-center[0])*.975,center[1]+(p[1]-center[1])*.945) for p in outline]
    for side in (-1,1):
        for ids in tri:
            pts=[pos(inner[i],side*.077) for i in ids];tex=[uv(inner[i]) for i in ids]
            # Pixel x/y -> native z/x has a +Y surface normal.
            if side<0:pts.reverse();tex.reverse()
            face(c,pts,14,tex)
        for i,a in enumerate(outline):
            j=(i+1)%len(outline)
            for coords in ([a,outline[j],inner[j]],[a,inner[j],inner[i]]):
                pts=[pos(p,side*(.012 if p in (a,outline[j]) else .077)) for p in coords]
                tex=[uv(p) for p in coords]
                if side<0:pts.reverse();tex.reverse()
                face(c,pts,14,tex)
    for i,a in enumerate(outline):
        b=outline[(i+1)%len(outline)]
        for pts in ([pos(a,-.012),pos(b,-.012),pos(b,.012)],[pos(a,-.012),pos(b,.012),pos(a,.012)]):face(c,pts,14,[uv(a),uv(b),uv(b)])
    def ornament(path,radii,depth=0,sides=8,keratin=True):
        # Reconstruct the approved image's curved horn masses in real volume;
        # project the same artwork onto both outward sides of the geometry.
        paths=[pos(p,depth) for p in path];points,rs=smooth(paths,[r*scale for r in radii],4)
        at=len(c['corners'])
        if keratin:
            uv_reports.append(horn_tube(c,tube,points,rs,14,(.758,.91,.992,.045),sides,'cleaver-horn-'+str(len(uv_reports))))
        else:
            tube(c,points,rs,14,(.758,.91,.992,.045),sides)
        if not keratin:
            for corner in c['corners'][at:]:
                p=c['points'][corner[0]];corner[4:6]=uv((p[2]/scale+1200,(p[0]-x0)/scale+418))
    # Reinforcing horn spine, unequal tines and a sweeping branched guard.
    ornament([(1014,441),(972,394),(926,369),(870,374),(804,392),(741,409),(652,422),(556,403),(453,390),(351,380),(256,372),(171,368)],[31,32,28,23,21,19,15,13,12,11,9,1],-.028)
    for p,r in [
        ([(908,374),(950,321),(982,263),(1001,185),(991,112),(963,58),(944,30)],[22,23,21,17,12,7,.3]),
        ([(966,312),(916,278),(849,272),(800,254),(776,203)],[17,16,13,9,.3]),
        ([(957,345),(1022,324),(1066,279),(1100,219),(1121,187)],[16,14,11,7,.3]),
        ([(998,438),(1030,491),(1060,540),(1104,579),(1124,554)],[20,16,12,8,.3]),
        ([(1003,496),(1018,568),(1036,638),(1066,711),(1058,656)],[15,13,9,6,.3]),
        ([(784,400),(751,376),(735,351),(698,345),(664,324),(637,303)],[14,13,10,10,8,.3]),
        ([(602,416),(571,385),(547,355),(516,307)],[12,11,8,.3]),
        ([(463,392),(438,368),(422,328),(383,301)],[11,9,6,.3]),
        ([(359,382),(330,351),(295,300),(278,268)],[11,10,6,.3]),
        ([(257,374),(233,356),(215,324),(198,313)],[9,7,5,.3]),
        ([(177,368),(168,349),(164,335)],[7,4,.3]),
    ]:ornament(p,r,-.025)
    # Entire grip is replaced so the old red gradient/guard is not retained.
    ornament([(981,419),(1035,416),(1108,414),(1175,416),(1240,419),(1310,423),(1382,427),(1426,426)],[19,23,27,27,26,25,24,21],0,14,False)
    ornament([(1392,428),(1440,411),(1470,373),(1475,337),(1450,292)],[20,22,15,9,.3])
    ornament([(1412,431),(1448,456),(1438,491),(1404,518)],[17,14,9,.3])
    # Roll about the existing handle's longitudinal Z axis, never turn the
    # sword end-for-end. The gripped center and blade/pommel direction stay put.
    assert roll_degrees in (0,180)
    if roll_degrees:
        for p in c['points']:
            p[0]=2*x0-p[0];p[1]=2*y0-p[1]
        for corner in c['corners']:
            corner[1]*=-1;corner[2]*=-1
    return dict(approved_concept_reconstructed=True,blade_outline_points=len(outline),raised_horn_spine=True,curved_guard=True,original_red_blade_removed=True,weapon_concept_pending=False,weapon_hand_bone=14,
                blade_roll_degrees=roll_degrees,blade_roll_axis=[0,0,1],grip_pivot_baseline=[x0,y0,0],blade_long_axis_preserved=True,cleaver_horn_uv=uv_reports)
