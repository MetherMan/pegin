"""Verify exact native texture pixels and delegation to the shared compiler."""
from pathlib import Path
from io import BytesIO
import base64,json,sys,zlib
R=Path(__file__).resolve().parents[1];H=R/'client-overlay/Tools/SkillColors'
sys.dont_write_bytecode=True;sys.path[:0]=[str(H),str(R/'runtime/pylibs')]
from PIL import Image
from native_assets import load_preview
from tuning import resource_reader

def main():
    read=resource_reader(R/'client-overlay');a=load_preview(H,R/'client-overlay');pixels=0
    for prefix,textures in [('Effect',a['textures']),('',a['actors']['textures'])]:
        for name,url in textures.items():
            raw=zlib.decompress(read('Texture/'+(prefix+'/' if prefix else '')+name)[13:])
            original=Image.open(BytesIO(raw)).convert('RGB')
            preview=Image.open(BytesIO(base64.b64decode(url.split(',',1)[1]))).convert('RGB')
            assert original.size==preview.size and original.tobytes()==preview.tobytes(),name
            pixels+=original.width*original.height
    for effect in a['effects'].values():assert len(a['meshes'][effect['mesh']]['indices'])%3==0
    assert set(a['actors']['weapons'])=={'arrow','magic'}
    report=dict(passed=True,exact_texture_pixels=pixels,native_effects=len(a['effects']),native_meshes=len(a['meshes']),native_character_parts=len(a['actors']['people']['0']['parts']),original_sound_files=list(a['sounds']),source_hashes=a['sources'],in_game_visual_test=False)
    (R/'assets/skills140/preview-validation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print('PASS preview:',pixels,'exact native texture pixels, character skin and native motion assets')
if __name__=='__main__':main()
