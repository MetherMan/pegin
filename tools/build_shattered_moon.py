"""Build the approved shattered-moon concept into the three existing weapon IDs."""
from pathlib import Path
import sys,shutil,argparse
R=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(R/'assets/twilight-set'))
from mesh_tools import *
O=R/'assets/twilight-set';D=R/'client-overlay'

def blade_map(a):
    """Retain the serrated outline while letting the distal blade taper."""
    a=np.asarray(a,dtype=float).copy()
    u=np.clip((a[:,1]-610)/485,0,1)
    center=np.interp(a[:,1],[610,780,925,1095],[239,253,298,399])
    a[:,0]=center+(a[:,0]-center)*(1-.22*u)
    return a

def sword_thickness(xy,d):
    # The old d/14 rounded profile made almost the entire blade as thick as
    # the guard. A long flat bevel ends at a 0.6 mm rim and also thins toward
    # the point; the grip and sculpted guard retain their structural depth.
    y=xy[:,1];blade=np.clip((y-387)/78,0,1)
    length=np.clip((y-430)/660,0,1)
    rim=.00032
    ridge=.023*(1-.72*length)
    cutting=rim+ridge*np.clip(d/43,0,1)**1.05
    guard=.003+.032*np.sin(np.minimum(d/14,1)*math.pi/2)
    return guard*(1-blade)+cutting*blade

def moon_parts(kind,xy_map,image_size):
    """A raised, blue bevel around the approved crescent, not a new emblem.

    Its face samples the existing artwork. Only the geometric bevel uses the
    shared blue material, so the approved crater/inlay texture stays intact.
    """
    if kind=='sword':
        outline=[(175,305),(195,302),(215,305),(236,316),(250,334),(257,353),(253,370),(243,384),(228,390),(207,389),(188,380),(175,364),(166,347),(178,359),(191,368),(204,371),(217,364),(229,352),(232,338),(227,325),(216,315),(195,308)]
        center=np.array([210,346]);grow=1.15
    else:
        outline=[(631,480),(612,479),(593,485),(579,499),(573,517),(575,537),(584,554),(600,566),(619,570),(639,567),(624,560),(614,549),(610,535),(611,520),(618,504),(628,491),(643,483)]
        center=np.array([611,526]);grow=1.2
    original=Polygon(outline)
    if not original.is_valid:original=shapely.make_valid(original)
    def mapped(a):return xy_map(center+(a-center)*grow)
    def level(a,d):return .040+.012*np.minimum(d/13,1)
    face,edge=sculpt(original,mapped,level,image_size,step=9)
    # A raised blue lip is wide enough to survive the game's small view. It
    # follows the crescent silhouette instead of outlining the whole weapon.
    border=original.buffer(3.4,join_style=1).difference(original.buffer(-1.8,join_style=1))
    if hasattr(border,'geoms'):border=max(border.geoms,key=lambda p:p.area)
    lip,wall=sculpt(border,mapped,lambda a,d:.047+.012*np.minimum(d/2.4,1),image_size,step=5)
    return [('raised original moon inlay',0,face),('silver moon seat',1,edge),('blue moon outline',2,lip),('silver moon bevel edge',1,wall)]

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--kind',choices=['sword','longbow','staff'],action='append');args=parser.parse_args()
    im=Image.open(R/'assets/visual-refresh-20260923/twilight-shattered-moon-concept.png').convert('RGB');w,h=im.size
    steel=Image.open(R/'assets/twilight/moonsteel_side_albedo.png').convert('RGB')
    pixels=np.asarray(im).astype(np.int16);gray=pixels.max(2);yy,xx=np.indices((h,w))
    reports=[]
    specs=[('sword','mt_twilight',(65,18,452,1102),(251,185),1.605/910,True,'ws_0081_1.mod'),
           ('longbow','mt_longbow',(505,102,880,1090),(674,570),1.74/974,False,'wb_0040_1.mod'),
           ('staff','mt_staff',(940,16,1255,1085),(1103,600),1.82/1060,False,'ww_0015_1.mod')]
    for kind,prefix,box,grip,scale,reverse,ref in specs:
        if args.kind and kind not in args.kind:continue
        x0,y0,x1,y1=box
        # Segment material against the neutral concept background, retaining
        # dark metal interiors instead of keeping only bright silver strokes.
        zone=(xx>=x0)&(xx<x1)&(yy>=y0)&(yy<y1)
        if kind=='sword':zone&=~((xx>325)&(yy<160))
        gc=np.zeros((h,w),dtype='uint8');gc[zone]=cv2.GC_PR_BGD
        gc[zone&(gray>53)]=cv2.GC_PR_FGD
        gc[zone&(gray>120)]=cv2.GC_FGD
        cv2.grabCut(np.asarray(im),gc,None,np.zeros((1,65)),np.zeros((1,65)),7,cv2.GC_INIT_WITH_MASK)
        mask=((gc==cv2.GC_FGD)|(gc==cv2.GC_PR_FGD)).astype('uint8')
        mask=cv2.morphologyEx(mask,cv2.MORPH_CLOSE,np.ones((9,9),np.uint8))
        if kind=='sword':mask[(yy>118)&(yy<239)&(xx>237)&(xx<269)]=0
        if kind=='longbow':
            mask[(xx>789)&(yy>196)&(yy<918)]=0
            mask[(yy>507)&(yy<630)&(xx>655)&(xx<700)]=0
        if kind=='staff':mask[(yy>507)&(yy<844)&(xx>1070)&(xx<1141)]=0
        contours,hier=cv2.findContours(mask,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)
        polys=[]
        for i,c in enumerate(contours):
            if hier[0,i,3]!=-1 or cv2.contourArea(c)<130:continue
            outer=cv2.approxPolyDP(c,1.05,True)[:,0,:];holes=[];child=hier[0,i,2]
            while child!=-1:
                if cv2.contourArea(contours[child])>230:holes.append(cv2.approxPolyDP(contours[child],1,True)[:,0,:])
                child=hier[0,child,0]
            p=Polygon(outer,holes)
            if not p.is_valid:p=shapely.make_valid(p)
            polys.extend(g.buffer(.15,join_style=2) for g in (p.geoms if hasattr(p,'geoms') else [p]) if isinstance(g,Polygon) and g.area>130)
        sign=-1 if reverse else 1
        def xy_map(a):
            a=blade_map(a) if kind=='sword' else np.asarray(a)
            return np.column_stack(((a[:,0]-grip[0])*scale*sign,(grip[1]-a[:,1])*scale*sign))
        def at(x,y):return [*xy_map(np.array([[x,y]]))[0],0]
        parts=[]
        for i,p in enumerate(polys):
            def thickness(xy,d):
                if kind=='sword':return sword_thickness(xy,d)
                body=.032 if kind=='sword' else (.027 if kind=='longbow' else .035)
                return .003+body*np.sin(np.minimum(d/14,1)*math.pi/2)
            face,side=sculpt(p,xy_map,thickness,[w,h],step=19)
            parts.extend([(f'{kind} moon face {i}',0,face),(f'{kind} silver edge {i}',1,side)])
        if kind=='sword':
            gripmesh=cylinder(at(251,246),at(251,109),.021,.021,rings=18)
        elif kind=='longbow':
            gripmesh=cylinder(at(674,643),at(674,494),.022,.020,rings=18)
            parts.append(('continuous lunar bowstring',2,cylinder(at(803,160),at(802,960),.0014,.0014,sides=8)))
        else:gripmesh=cylinder(at(1103,860),at(1103,484),.017,.022,rings=40)
        points,normal,uv=gripmesh
        projected=np.column_stack((points[:,0]/(scale*sign)+grip[0],grip[1]-points[:,1]/(scale*sign)))
        # Same artwork is wrapped around round grips, not painted anew.
        parts.append(('round original-grip attachment',0,(points,normal,projected/np.array([w,h]))))
        if kind in ['sword','longbow']:
            parts.extend(moon_parts(kind,xy_map,[w,h]))
            # Packing only this weapon into the atlas avoids spending three
            # quarters of its face texture on the other weapons and title.
            crop=im.crop(box);remapped=[]
            for name,mat,(p,n,u) in parts:
                if mat==0:u=(u*np.array([w,h])-np.array([x0,y0]))/np.array([x1-x0,y1-y0])
                remapped.append((name,mat,(p,n,u)))
            parts=remapped
            images=[crop,steel,Image.new('RGB',(8,8),(45,137,215))]
        else:images=[im,steel,Image.new('RGB',(8,8),(95,165,225))]
        write_glb(O/f'{prefix}-shattered.glb',parts,images)
        report=export_native(O/'payload',prefix,parts,images,R/'runtime/client/GameClient/Equip'/ref)
        for lod in (1,2,3):shutil.copy2(O/f'payload/{prefix}_{lod}.mod',D/f'Equip/{prefix}_{lod}.mod')
        shutil.copy2(O/f'payload/{prefix}_atlas.wtm',D/f'Texture/Equip/{prefix}_atlas.wtm')
        report.update(kind=kind,source='twilight-shattered-moon-concept.png',grip=grip,scale=scale,holes=sum(len(p.interiors) for p in polys),parts=len(polys),visual_in_game_checked=False,
                      refinement='raised-blue-crescent-and-tapered-blade' if kind!='staff' else None)
        reports.append(report);print(kind,report['vertices'],report['triangles'],len(polys),report['holes'])
    reportpath=O/'shattered-moon-validation.json'
    if args.kind and reportpath.exists():
        previous=json.loads(reportpath.read_text());updated={r['kind']:r for r in reports}
        reports=[updated.get(r['kind'],r) for r in previous]
    reportpath.write_text(json.dumps(reports,indent=2)+'\n')
if __name__=='__main__':main()
