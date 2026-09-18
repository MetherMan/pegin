"""Keep every rigid weapon surface on animation mesh zero.

The legacy weapon animation supplies one mesh transform. Material chunks after
mesh zero otherwise fall back to the character origin. Bake all materials into
one texture atlas; split repeated UVs at tile boundaries before remapping them.
"""
import math
import numpy as np
from PIL import Image

def single_mesh_atlas(groups, images, pack_texture, destination):
    # The two steel materials have the same baked pixels; reuse that tile.
    tiles=[];material_tile={}
    for material in sorted(groups):
        im=images[material].convert('RGB')
        key=(im.size,im.tobytes())
        index=next((i for i,t in enumerate(tiles) if t[0]==key),None)
        if index is None:index=len(tiles);tiles.append((key,im))
        material_tile[material]=index
    columns=math.ceil(math.sqrt(len(tiles)))
    side=1
    while side<columns*1024:side*=2
    cell=side//columns;pad=8;inner=cell-2*pad
    atlas=Image.new('RGB',(side,side))
    for i,(_,im) in enumerate(tiles):
        pixels=np.asarray(im.resize((inner,inner),Image.Resampling.LANCZOS))
        pixels=np.pad(pixels,((pad,pad),(pad,pad),(0,0)),mode='edge')
        atlas.paste(Image.fromarray(pixels),(i%columns*cell,i//columns*cell))
    pack_texture(atlas,destination)
    positions=[];normals=[];uvs=[];counts={};areas={}
    def clip(poly,axis,bound,keep_above):
        result=[]
        for i,a in enumerate(poly):
            b=poly[(i+1)%len(poly)];da=a[6+axis]-bound;db=b[6+axis]-bound
            inside=da>=-1e-9 if keep_above else da<=1e-9
            other=db>=-1e-9 if keep_above else db<=1e-9
            if inside:result.append(a)
            if inside!=other:
                t=da/(da-db);v=a+(b-a)*t;v[6+axis]=bound;result.append(v)
        return result
    for material,parts in sorted(groups.items()):
        tile=material_tile[material];offset=np.array([tile%columns*cell+pad,tile//columns*cell+pad])
        count=0;source_area=0;atlas_area=0
        for pos,normal,uv in parts:
            data=np.column_stack((pos,normal,uv)).astype(np.float64).reshape(-1,3,8)
            for triangle in data:
                source_area+=np.linalg.norm(np.cross(triangle[1,:3]-triangle[0,:3],triangle[2,:3]-triangle[0,:3]))/2
                low=np.floor(triangle[:,6:].min(axis=0)).astype(int)
                high=np.ceil(triangle[:,6:].max(axis=0)).astype(int)-1
                high=np.maximum(low,high)
                if np.any(high-low>100):raise ValueError('Unexpected UV span')
                for x in range(low[0],high[0]+1):
                    for y in range(low[1],high[1]+1):
                        poly=list(triangle)
                        for axis,lo in [(0,x),(1,y)]:
                            poly=clip(poly,axis,lo,True)
                            if poly:poly=clip(poly,axis,lo+1,False)
                            if not poly:break
                        for i in range(1,len(poly)-1):
                            t=np.array([poly[0],poly[i],poly[i+1]])
                            area=np.linalg.norm(np.cross(t[1,:3]-t[0,:3],t[2,:3]-t[0,:3]))/2
                            if area<5e-15:continue
                            atlas_area+=area
                            positions.extend(t[:,:3]);normals.extend(t[:,3:6])
                            uvs.extend((np.clip(t[:,6:]-[x,y],0,1)*inner+offset)/side);count+=1
        counts[str(material)]=count
        assert abs(source_area-atlas_area)<max(1e-8,source_area*1e-6),(material,source_area,atlas_area)
        areas[str(material)]={'source':float(source_area),'atlas':float(atlas_area)}
    return np.asarray(positions,dtype='<f4'),np.asarray(normals,dtype='<f4'),np.asarray(uvs,dtype='<f4'),dict(atlas_size=side,unique_tiles=len(tiles),material_tiles=material_tile,triangles_by_material=counts,surface_area_by_material=areas)
