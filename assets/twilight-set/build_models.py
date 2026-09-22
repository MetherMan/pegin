from mesh_tools import *
import shutil
O=Path(__file__).resolve().parent
C=ROOT/'runtime/client/GameClient'

def silhouettes(im,kind):
    pix=np.asarray(im.convert('RGB')).astype(np.int16);h,w=pix.shape[:2]
    mask=((pix.max(2)>103)|((pix[:,:,2]-pix[:,:,0]>22)&(pix[:,:,2]>68))).astype('uint8')
    yy,xx=np.indices((h,w))
    # Explicit exclusion of the thin string; it gets its own round geometry.
    if kind=='longbow':
        line=747+(yy-137)*(540-747)/(1340-137)
        mask[(yy>145)&(yy<1305)&(xx>line-11)]=0
        mask[(yy>=742)&(yy<880)]=0
        for y in range(160,1310):
            xs=np.where(mask[y])[0]
            runs=np.split(xs,np.where(np.diff(xs)>1)[0]+1)
            if len(runs)>1 and len(runs[-1])<15 and runs[-1][0]-runs[-2][-1]>25:mask[y,runs[-1]]=0
    else:
        mask[(yy>552)&(yy<1240)]=0
    mask[:6]=0;mask[h-18:]=0
    mask=cv2.morphologyEx(mask,cv2.MORPH_CLOSE,np.ones((5,5),np.uint8))
    contours,hier=cv2.findContours(mask,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)
    polys=[]
    for i,c in enumerate(contours):
        if hier[0,i,3]!=-1 or cv2.contourArea(c)<350:continue
        outer=cv2.approxPolyDP(c,1.3,True)[:,0,:];holes=[];child=hier[0,i,2]
        while child!=-1:
            area=cv2.contourArea(contours[child]);center=contours[child][:,0,:].mean(0)
            actual_hole=(min(np.linalg.norm(center-q) for q in [(735,135),(540,1340)])<45) if kind=='longbow' else area>900
            if area>180 and actual_hole:holes.append(cv2.approxPolyDP(contours[child],1.1,True)[:,0,:])
            child=hier[0,child,0]
        p=Polygon(outer,holes)
        if not p.is_valid:p=shapely.make_valid(p)
        for g in (p.geoms if hasattr(p,'geoms') else [p]):
            if isinstance(g,Polygon) and g.area>350:polys.append(g)
    return sorted(polys,key=lambda p:p.bounds[1])

def build(kind):
    im=Image.open(O/(kind+'-concept.png')).convert('RGB');w,h=im.size
    steel=Image.open(ROOT/'assets/twilight/moonsteel_side_albedo.png').convert('RGB')
    prefix='mt_longbow' if kind=='longbow' else 'mt_staff'
    # Match the peer weapon's size and native grip origin. Slightly larger head
    # is intentional for the Rank 9 model; no change to attachment animation.
    scale=1.74/1480 if kind=='longbow' else 1.82/1490
    grip=(453,810) if kind=='longbow' else (490,790)
    xy_map=lambda xy:np.column_stack(((xy[:,0]-grip[0])*scale,(grip[1]-xy[:,1])*scale))
    parts=[];polys=silhouettes(im,kind)
    for i,p in enumerate(polys):
        def height(xy,d):
            # A narrow silver edge, round inlay body and thicker lunar relief.
            body=.030 if kind=='longbow' else .037
            return .004+body*np.sin(np.minimum(d/18,1)*math.pi/2)
        face,side=sculpt(p,xy_map,height,[w,h],step=20)
        parts.extend([(f'{kind} sculpted body {i}',0,face),(f'{kind} textured edge {i}',1,side)])
    def at(x,y):return [*(xy_map(np.array([[x,y]]))[0]),0]
    if kind=='longbow':
        parts.append(('round wrapped bow grip',0,cylinder(at(450,885),at(454,735),.022,.022,(431/w,748/h,475/w,878/h),rings=8)))
        # The side-profile string matches the approved full-length design.
        parts.append(('continuous blue bowstring',2,cylinder(at(540,1338),at(747,137),.0014,.0014,sides=8)))
    else:
        # Slightly tapered round shaft; the approved projected inlay wraps fully.
        shaft=cylinder(at(487,1290),at(513,525),.016,.019,rings=48)
        pix=np.asarray(im).astype(int);suv=shaft[2].copy()
        for i,(u,t) in enumerate(suv):
            y=int(round(1290+(525-1290)*t));line=pix[y];xs=np.arange(w)
            mask=((line[:,2]-line[:,0]>20)|(line.max(1)>105))&(xs>420)&(xs<565)
            runs=np.split(np.where(mask)[0],np.where(np.diff(np.where(mask)[0])>1)[0]+1)
            runs=[q for q in runs if len(q)>3]
            run=max(runs,key=len) if runs else np.arange(487,513)
            suv[i]=[(run[0]+(run[-1]-run[0])*(.2+.6*u))/w,y/h]
        parts.append(('round blue staff shaft',0,(shaft[0],shaft[1],suv)))
        for y,r in [(562,.025),(686,.028),(925,.023),(1150,.026),(1265,.029)]:
            x=487+(1290-y)/765*26
            parts.append((f'raised silver collar {y}',1,cylinder(at(x,y+6),at(x,y-6),r,r,(0,0,1,.25),sides=24)))
    images=[im,steel,Image.new('RGB',(8,8),(47,98,188))]
    write_glb(O/f'{prefix}.glb',parts,images)
    report=export_native(O/'payload',prefix,parts,images,C/'Equip'/('wb_0040_1.mod' if kind=='longbow' else 'ww_0015_1.mod'))
    report.update(kind=kind,body_half_thickness_max=.034 if kind=='longbow' else .041,round_grip=True,source_polygons=len(polys),genuine_holes=sum(len(p.interiors) for p in polys),visual_in_game_checked=False)
    for lod in [1,2,3]:shutil.copy2(O/f'payload/{prefix}_{lod}.mod',ROOT/f'client-overlay/Equip/{prefix}_{lod}.mod')
    shutil.copy2(O/f'payload/{prefix}_atlas.wtm',ROOT/f'client-overlay/Texture/Equip/{prefix}_atlas.wtm')
    (O/f'{kind}-geometry.json').write_text(json.dumps([dict(outline=list(p.exterior.coords),holes=[list(q.coords) for q in p.interiors]) for p in polys]))
    (O/f'{kind}-validation.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='atlas'}))

if __name__=='__main__':
    for kind in sys.argv[1:] or ['longbow','staff']:build(kind)
