"""Build complete blue-moon weapon geometry from the new approved artwork.

Only sword and longbow IDs are emitted. The staff, item records, launch files,
and installed client are never written. Source artwork remains unchanged;
the image work here is source cropping and UV atlas packing only.
"""
from pathlib import Path
import argparse,hashlib,json,shutil,sys
R=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(R/'assets/twilight-set'))
from mesh_tools import *
from noble_moon_atlas import full_resolution_atlas
O=R/'assets/twilight-set';D=R/'client-overlay'
DEFAULT_SPEC=O/'noble-moon-geometry.json'

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()

def active_specs(path=DEFAULT_SPEC):
    """Resolve an approved per-weapon design without rewriting sibling specs."""
    config=json.loads(Path(path).read_text(encoding='utf-8'))
    active_path=O/'active-design.json'
    if Path(path).resolve()==DEFAULT_SPEC.resolve() and active_path.exists():
        active=json.loads(active_path.read_text(encoding='utf-8'))
        for i,spec in enumerate(config['weapons']):
            override=active.get('overrides',{}).get(spec['kind'])
            if override:
                choices=json.loads((O/override['geometry_spec']).read_text(encoding='utf-8'))['weapons']
                config['weapons'][i]=next(s for s in choices if s['kind']==spec['kind'])
    return config

def polygons_from_source(im,spec):
    pixels=np.asarray(im.convert('RGB'));h,w=pixels.shape[:2]
    yy,xx=np.indices((h,w));x0,y0,x1,y1=spec['box']
    zone=(xx>=x0)&(xx<x1)&(yy>=y0)&(yy<y1)
    rgba=np.asarray(im.convert('RGBA'))
    if (rgba[:,:,3]<240).any():
        mask=((rgba[:,:,3]>spec.get('alpha_threshold',160))&zone).astype('uint8')
    else:
        gray=pixels.max(2);gc=np.full((h,w),cv2.GC_BGD,dtype='uint8')
        gc[zone]=cv2.GC_PR_BGD
        gc[zone&(gray>spec.get('likely_threshold',55))]=cv2.GC_PR_FGD
        gc[zone&(gray>spec.get('solid_threshold',120))]=cv2.GC_FGD
        for rect in spec.get('background',[]):
            a,b,c,d=rect;gc[b:d,a:c]=cv2.GC_BGD
        cv2.grabCut(pixels,gc,None,np.zeros((1,65)),np.zeros((1,65)),7,cv2.GC_INIT_WITH_MASK)
        mask=((gc==cv2.GC_FGD)|(gc==cv2.GC_PR_FGD)).astype('uint8')
    # Joining only tiny image gaps keeps deliberately empty crescent openings.
    close=spec.get('close_pixels',3)
    if close>1:mask=cv2.morphologyEx(mask,cv2.MORPH_CLOSE,np.ones((close,close),np.uint8))
    for a,b,c,d in spec.get('exclude',[]):mask[b:d,a:c]=0
    contours,hier=cv2.findContours(mask,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_SIMPLE)
    result=[]
    for i,c in enumerate(contours):
        if hier[0,i,3]!=-1 or cv2.contourArea(c)<spec.get('minimum_area',160):continue
        outer=cv2.approxPolyDP(c,spec.get('outline_error',.75),True)[:,0,:]
        holes=[];child=hier[0,i,2]
        while child!=-1:
            if cv2.contourArea(contours[child])>spec.get('minimum_hole',90):
                holes.append(cv2.approxPolyDP(contours[child],.7,True)[:,0,:])
            child=hier[0,child,0]
        p=Polygon(outer,holes)
        if not p.is_valid:p=shapely.make_valid(p)
        for g in p.geoms if hasattr(p,'geoms') else [p]:
            if isinstance(g,Polygon) and g.area>spec.get('minimum_area',160):result.append(g)
    assert result,'No solid weapon silhouette found'
    return result

def surface_depth(kind,spec,xy,distance):
    y=xy[:,1]
    if kind=='sword':
        root,tip=spec['blade_root_y'],spec['tip_y']
        u=np.clip((root-y)/(root-tip),0,1)
        blade=y<root
        # A long nearly planar cutting bevel, never the former puffed plate.
        ridge=.023*(1-.77*u)
        cut=.00024+ridge*np.minimum(distance/spec.get('bevel_pixels',38),1)
        guard=.0015+.026*np.sin(np.minimum(distance/20,1)*math.pi/2)
        return np.where(blade,cut,guard)
    y0,y1=spec['box'][1],spec['box'][3];grip=spec['grip'][1]
    extent=np.where(y<grip,grip-y0,y1-grip)
    u=np.clip(np.abs(y-grip)/np.maximum(extent,1),0,1)
    return .001+.026*(1-.65*u)*np.sin(np.minimum(distance/18,1)*math.pi/2)

def build(spec):
    if spec.get('artifact_tag')=='twin-moon-greatbow':
        from build_twin_moon_greatbow import build_bow
        return build_bow(spec)
    if spec.get('artifact_tag')=='h03-horn-bow':
        from build_h03_horn_bow import build_bow
        return build_bow(spec)
    kind=spec['kind'];assert kind in ['sword','longbow']
    prefix='mt_twilight' if kind=='sword' else 'mt_longbow'
    source=R/spec['source'];im=Image.open(source);w,h=im.size
    rgb=im.convert('RGB');polys=polygons_from_source(im,spec)
    scale=spec['scale'];grip=np.asarray(spec['grip'],float)
    def xy_map(a):
        a=np.asarray(a);return np.column_stack(((a[:,0]-grip[0])*scale,(grip[1]-a[:,1])*scale))
    def at(point):return [*xy_map(np.asarray([point]))[0],0]
    parts=[]
    for i,poly in enumerate(polys):
        face,side=sculpt(poly,xy_map,lambda xy,d:surface_depth(kind,spec,xy,d),[w,h],step=spec.get('mesh_step',19))
        parts.extend([(f'{kind} blue-moon body {i}',0,face),(f'{kind} blue crystal side {i}',1,side)])
    if spec.get('grip_ends'):
        a,b=spec['grip_ends'];p,n,u=cylinder(at(a),at(b),*spec.get('grip_radii',[.020,.020]),rings=16)
        pixel=np.column_stack((p[:,0]/scale+grip[0],grip[1]-p[:,1]/scale))
        parts.append(('round blue moon grip',0,(p,n,pixel/np.array([w,h]))))
    if kind=='longbow':
        a,b=spec['string_ends'];parts.append(('single continuous moonlight bowstring',2,cylinder(at(a),at(b),.0012,.0012,sides=8)))
    x0,y0,x1,y1=spec['box'];remapped=[]
    for name,material,(p,n,u) in parts:
        if material==0:u=(u*np.array([w,h])-np.array([x0,y0]))/np.array([x1-x0,y1-y0])
        remapped.append((name,material,(p,n,u)))
    # Both the broad faces and the bevel sides come from this weapon's new
    # blue artwork. No grey steel material and no independent moon badge.
    images=[rgb.crop(spec['box']),rgb.crop(spec['side_material_box']),Image.new('RGB',(8,8),(110,218,255))]
    parts=remapped
    if spec.get('blade_refinement'):
        from refine_s4_blade import refine_preview_parts
        parts=refine_preview_parts(parts,spec)
    write_glb(O/f"{prefix}-{spec.get('artifact_tag','noble')}.glb",parts,images)
    ref='ws_0081_1.mod' if kind=='sword' else 'wb_0040_1.mod'
    result=export_native(O/'payload',prefix,parts,images,R/'runtime/client/GameClient/Equip'/ref,atlas_packer=full_resolution_atlas)
    if spec.get('blade_refinement'):
        from refine_s4_blade import verify_refined_native
        reference=json.loads((O/'s4/blade-refinement-validation.json').read_text(encoding='utf-8'))
        result['blade_refinement']=verify_refined_native((O/f'payload/{prefix}_1.mod').read_bytes(),spec,reference)
    for lod in (1,2,3):shutil.copy2(O/f'payload/{prefix}_{lod}.mod',D/f'Equip/{prefix}_{lod}.mod')
    shutil.copy2(O/f'payload/{prefix}_atlas.wtm',D/f'Texture/Equip/{prefix}_atlas.wtm')
    result.update(kind=kind,source=spec['source'],source_sha256=digest(source),grip=spec['grip'],scale=scale,
                  separate_moon_badge=False,parts=len(polys),holes=sum(len(p.interiors) for p in polys),in_game_visual_test=False)
    print(kind,result['vertices'],result['triangles'],result['parts'],result['holes'])
    return result

def main():
    parser=argparse.ArgumentParser();parser.add_argument('--spec',type=Path,default=DEFAULT_SPEC);parser.add_argument('--kind',action='append',choices=['sword','longbow']);args=parser.parse_args()
    config=active_specs(args.spec)
    if args.kind is None and args.spec.resolve()==DEFAULT_SPEC.resolve() and (O/'active-design.json').exists():
        args.kind=json.loads((O/'active-design.json').read_text(encoding='utf-8')).get('default_build_kinds')
    staff=[D/f'Equip/mt_staff_{lod}.mod' for lod in (1,2,3)]+[D/'Texture/Equip/mt_staff_atlas.wtm']
    before={str(p.relative_to(D)):digest(p) for p in staff}
    reports=[build(spec) for spec in config['weapons'] if not args.kind or spec['kind'] in args.kind]
    after={str(p.relative_to(D)):digest(p) for p in staff};assert before==after,'Staff files must remain unchanged'
    reportpath=O/'noble-moon-validation.json'
    if args.kind and reportpath.exists():
        previous=json.loads(reportpath.read_text());updated={r['kind']:r for r in reports}
        reports=[updated.get(r['kind'],r) for r in previous['weapons']]
    reportpath.write_text(json.dumps(dict(passed=True,design='noble-blue-moon',weapons=reports,staff_unchanged=True,staff_sha256=after),indent=2)+'\n')

if __name__=='__main__':main()
