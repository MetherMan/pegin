"""Export native actor and equipped weapons for a screen-size review, not a game capture."""
from pathlib import Path
import sys,json,base64
R=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(R/'client-overlay/Tools/SkillColors'))
from native_actor import load_actors,model
from native_assets import png_from_wtm
from tuning import resource_reader

def main():
    read=resource_reader(R/'client-overlay');data=load_actors(read,png_from_wtm)
    parts=model(read('Equip/mt_twilight_1.mod'))
    for part in parts:
        part['texture']='Equip/'+part['texture']
        data['textures'][part['texture']]='data:image/png;base64,'+base64.b64encode(png_from_wtm(read('Texture/'+part['texture']))).decode()
    data['weapons']['sword']=dict(parts=parts,bone='Bip01 R Finger0')
    data['people']['0']['sword']=data['people']['0']['magic']
    (R/'assets/twilight-set/readability-actors.json').write_text(json.dumps(data,separators=(',',':')))
    print('Native character and three weapon models exported for screen-size review')

if __name__=='__main__':main()
