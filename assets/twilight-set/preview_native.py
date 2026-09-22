from mesh_tools import *
O=Path(__file__).resolve().parent
for key,prefix,folder in [('longbow','mt_longbow',ROOT/'client-overlay'),('staff','mt_staff',ROOT/'client-overlay'),('sword','mt_twilight',ROOT/'client-overlay')]:
    parts=[];images=[]
    for p in read_mod(folder/f'Equip/{prefix}_1.mod'):
        pos=p['points'][p['corners']['index']];n=p['corners']['normal'];uv=p['corners']['uv']
        pos=np.column_stack((-pos[:,1],pos[:,0],pos[:,2]));n=np.column_stack((-n[:,1],n[:,0],n[:,2]))
        t=pos.reshape(-1,3,3);dots=(np.cross(t[:,1]-t[:,0],t[:,2]-t[:,0])*n.reshape(-1,3,3).mean(1)).sum(1)
        if np.median(dots)<0:
            pos=pos.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);n=n.reshape(-1,3,3)[:,[0,2,1]].reshape(-1,3);uv=uv.reshape(-1,3,2)[:,[0,2,1]].reshape(-1,2)
        raw=(folder/'Texture/Equip'/Path(p['texture']).with_suffix('.wtm')).read_bytes();im=Image.open(BytesIO(zlib.decompress(raw[13:]))).convert('RGB')
        parts.append((key,len(images),(pos,n,uv)));images.append(im)
    write_glb(O/f'{key}-native.glb',parts,images)
print('Native MOD/WTM readback previews created.')
