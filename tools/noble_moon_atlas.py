"""Pack tall weapon artwork without reducing its native vertical resolution."""
import numpy as np
from PIL import Image

def full_resolution_atlas(groups,images,pack_texture,destination):
    side=2048;pad=8;atlas=Image.new('RGB',(side,side));rects={};x=0
    # The face keeps every pixel of its tall crop. Small side/string materials
    # occupy separate columns, instead of forcing the face into a square tile.
    for material in sorted(groups):
        im=images[material].convert('RGB');w,h=im.size
        assert x+w+pad*2<=side and h+pad*2<=side,(material,im.size,x)
        pixels=np.asarray(im);padded=np.pad(pixels,((pad,pad),(pad,pad),(0,0)),mode='edge')
        atlas.paste(Image.fromarray(padded),(x,0))
        rects[material]=dict(x=x+pad,y=pad,width=w,height=h,source_size=[w,h],resampled=False)
        x+=w+pad*2
    decoded=np.asarray(atlas)
    for material,r in rects.items():
        assert np.array_equal(decoded[r['y']:r['y']+r['height'],r['x']:r['x']+r['width']],np.asarray(images[material].convert('RGB')))
    pack_texture(atlas,destination)
    positions=[];normals=[];uvs=[];counts={};areas={}
    for material,parts in sorted(groups.items()):
        r=rects[material];count=0;area=0.
        for pos,normal,uv in parts:
            assert uv.min()>-1e-6 and uv.max()<1+1e-6,'Tall atlas requires non-repeating source UVs'
            positions.extend(pos);normals.extend(normal)
            uvs.extend((np.clip(uv,0,1)*[r['width'],r['height']]+[r['x'],r['y']])/side)
            tri=np.asarray(pos).reshape(-1,3,3)
            area+=float(np.linalg.norm(np.cross(tri[:,1]-tri[:,0],tri[:,2]-tri[:,0]),axis=1).sum()/2)
            count+=len(tri)
        counts[str(material)]=count;areas[str(material)]={'source':area,'atlas':area}
    return (np.asarray(positions,dtype='<f4'),np.asarray(normals,dtype='<f4'),np.asarray(uvs,dtype='<f4'),
            dict(atlas_size=side,layout='unscaled-tall-source-crops',unique_tiles=len(rects),material_rects=rects,
                 source_pixels_preserved=True,triangles_by_material=counts,surface_area_by_material=areas))
