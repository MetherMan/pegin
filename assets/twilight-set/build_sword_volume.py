"""Preserve the approved silhouette while separating edge and body thickness."""
from mesh_tools import *
O=ROOT/'assets/twilight'
design=json.loads((O/'design04_geometry.json').read_text());W,H=design['image_size'];S=.00165;CX=design['center_x'];CY=1112
xy_map=lambda xy:np.column_stack(((xy[:,0]-CX)*S,(CY-xy[:,1])*S))
result={}
for part in design['parts']:
    poly=Polygon(part['outline'],part['holes']);is_blade=part['name'].startswith('main')
    def height(xy,d):
        if is_blade:
            # 0.004 total cutting rim, ~0.035 body. Near the hilt, the guard
            # is a structural object with its own ~0.06 total depth.
            guard=np.clip((xy[:,1]-880)/110,0,1)
            rim=.002+.010*guard;center=.019+.014*guard
            return rim+(center-rim)*np.sin(np.minimum(d/18,1)*math.pi/2)
        return .013+.014*np.sin(np.minimum(d/12,1)*math.pi/2)
    face,side=sculpt(poly,xy_map,height,[W,H],step=25)
    result[part['name']]={name:{'position':g[0].ravel().tolist(),'normal':g[1].ravel().tolist(),'uv':g[2].ravel().tolist()} for name,g in [('face',face),('side',side)]}
(O/'volume_geometry.json').write_text(json.dumps(result,separators=(',',':')))
print('Sculpted sword body, thin cutting rims, thick guard/pommel, explicit side UVs.')
