from pathlib import Path
import json,base64,shutil
O=Path(__file__).resolve().parent
V=O.parent/'blood-knight/openwork_3d'
shutil.copytree(V/'vendor',O/'vendor',dirs_exist_ok=True)
p=O/'build_model.mjs';s=p.read_text(encoding='utf-8').replace("'../블러드나이트_스피어/openwork_3d/vendor/three.module.js'","'./vendor/three.module.js'")
if 'const blobs=' not in s:
 old=(V/'build_model.mjs').read_text(encoding='utf-8')
 tail=old[old.index('const blobs='):]
 tail=tail.replace('Bloodfang_Openwork_Spear','Blue_Moon_Twilight_Greatsword').replace('Bloodfang reference-guided geometry builder','Twilight reference-guided geometry builder')
 a=tail.index('const info=');b=tail.index("fs.writeFileSync(path.join(OUT,'model_info.json')",a)
 tail=tail[:a]+"const info={name:'푸른달의 트와일라잇 양손검',rank:9,name_color:'#72CFFF',rank_color:'#72CFFF',type:2,triangles:stats.reduce((a,b)=>a+b.triangles,0),mesh_objects:stats.length,cloth_strips:4,source:'concept_texture.png',not_in_game:true,construction:'Real extruded blade and separate crescent fins, open negative spaces, round wrapped hilt, curved cloth ribbon meshes. Both blade faces use the concept projection; some painted lighting remains in the texture.',parts:stats};\n"+tail[b:]
 tail=tail.replace('chain_links:count','cloth_strips:4')
 s+=tail
p.write_text(s,encoding='utf-8')
spec={'name':'푸른달의 트와일라잇 양손검','rank':9,'weapon_type':2,'weapon_class':'양손검','name_color':'#72CFFF','rank_color':'#72CFFF','inventory_style':'black background, antique gold border','damage':None,'in_game_registered':False,'stage':'3D model and inventory preview','source_concept':'concept_texture.png','icon':'inventory_icon_imagegen.png'}
if (O/'design.json').exists():
 existing_spec=json.loads((O/'design.json').read_text(encoding='utf-8'))
 if existing_spec.get('in_game_registered'):spec.update(existing_spec)
(O/'design.json').write_text(json.dumps(spec,ensure_ascii=False,indent=2),encoding='utf-8')

def viewer():
 enc=lambda s:'data:text/javascript;base64,'+base64.b64encode(s.encode()).decode()
 imports={key:enc((O/'vendor'/name).read_text(encoding='utf-8').replace('../utils/BufferGeometryUtils.js','bufferutils')) for key,name in [('three','three.module.js'),('controls','OrbitControls.js'),('loader','GLTFLoader.js'),('room','RoomEnvironment.js'),('bufferutils','BufferGeometryUtils.js')]}
 html=(V/'viewer_template.html').read_text(encoding='utf-8')
 html=html.replace('Bloodfang · 3D 모델','푸른달의 트와일라잇 양손검 · Rank 9').replace('BLOODFANG / OPENWORK','푸른달의 트와일라잇 양손검')
 html=html.replace('길게 뚫린 창날 · 송곳니 문장 · 금속 사슬 · 피에 물든 짐승털','Rank 9 · 양손검 · 초승달 문양 · 흩날리는 붕대').replace('창날 확대','칼날 확대')
 html=html.replace('#4a2831','#163359').replace('#a0606f','#72cfff').replace('color:#ede8e4','color:#72cfff').replace('color:#a6a3a5','color:#8dacc6')
 html=html.replace('extent*2.30','extent*2.2').replace("dir.set(.72,.13,1)","dir.set(.38,.09,1)")
 html=html.replace('model=gltf.scene;scene.add(model);','model=gltf.scene;model.rotation.z=-.38;scene.add(model);')
 html=html.replace('const camera=new THREE.PerspectiveCamera(29','const camera=new THREE.PerspectiveCamera(32')
 html=html.replace('0xffe4d1,1.6','0xe8f2ff,1.5').replace("'#13161c'","'#0e1420'")
 html=html.replace('Bloodfang_Openwork_Spear.glb','Blue_Moon_Twilight_Greatsword.glb')
 icon=base64.b64encode((O/'inventory_icon_imagegen.png').read_bytes()).decode()
 ui='''<aside id="itemCard"><div class="eyebrow">인벤토리 · 표기 미리보기</div><img id="icon" src="data:image/png;base64,'''+icon+'''"><h2>푸른달의 트와일라잇 양손검</h2><b class="rank">Rank 9</b><p>양손검</p><p class="muted">짙은 청람색 문양 · 은빛 칼날<br>검은 배경 · 금색 테두리</p><div class="slots"><span>실제 크기</span><img src="data:image/png;base64,'''+icon+'''" width="28" height="28"><img src="data:image/png;base64,'''+icon+'''" width="56" height="56"></div><small>게임 등록 완료 · /재뽕 테스트 2</small></aside><style>#itemCard{position:fixed;right:26px;top:105px;width:290px;padding:23px;background:#101723ed;border:1px solid #29384f;border-radius:15px;box-shadow:0 12px 50px #0005}#icon{display:block;width:155px;height:155px;margin:16px auto 22px;background:black;border-radius:4px}.eyebrow{font-size:11px;color:#8093ac;letter-spacing:2px}#itemCard h2{font-size:19px;font-weight:600;line-height:1.55;color:#72cfff;margin:0 0 9px}.rank{color:#72cfff;font-size:17px}#itemCard p{font-size:13px;color:#d4e1f0;line-height:1.7}.muted{color:#8b9aaf!important}.slots{display:flex;align-items:center;gap:15px;margin:18px 0;color:#8b9aaf;font-size:11px}.slots img{background:black;image-rendering:pixelated}#itemCard small{font-size:10px;color:#73849b}body.clean #itemCard{display:none}header h1{letter-spacing:0;font-size:24px}#status{right:28px;bottom:25px}@media(max-width:950px){#itemCard{width:230px;right:12px;padding:15px}#icon{width:110px;height:110px}header h1{font-size:19px}}</style>'''
 html=html.replace('<script type="importmap">',ui+'<script type="importmap">')
 html=html.replace('__IMPORTMAP__',json.dumps({'imports':imports})).replace('__GLB__',base64.b64encode((O/'Blue_Moon_Twilight_Greatsword.glb').read_bytes()).decode())
 # Move the model's visual center left so the preview card does not obscure it.
 html=html.replace('renderer.render(scene,camera);window.currentView',"camera.setViewOffset(innerWidth,innerHeight,innerWidth*.09,0,innerWidth,innerHeight);renderer.render(scene,camera);window.currentView")
 html=html.replace('<script type="importmap">',(O/'color_controls.html').read_text(encoding='utf-8')+'<script type="importmap">')
 appearance=json.loads((O/'game_appearance.json').read_text(encoding='utf-8'))['settings'] if (O/'game_appearance.json').exists() else {}
 color_script=(O/'color_controls.js').read_text(encoding='utf-8').replace('__GAME_APPEARANCE__',json.dumps(appearance))
 html=html.replace('try{const bytes=',color_script+'\ntry{const bytes=')
 html=html.replace('window.viewerReady=true;','window.viewerReady=true;initAppearance(model,originals);')
 html=html.replace('>GLB 저장</a>','>원본 GLB 저장</a>')
 html=html.replace('<button data-view="back">','<button data-view="cloth">붕대 확대</button><button data-view="back">')
 html=html.replace('<button data-view="cloth">','<button data-view="junction">연결부 확대</button><button data-view="cloth">')
 html=html.replace('<button data-view="back">','<button data-view="side">옆면</button><button data-view="back">')
 html=html.replace("if(view==='back')", "if(view==='side'){dist=extent*2.1;dir.set(1,.02,.045);}if(view==='back')")
 html=html.replace("if(view==='back')", "if(view==='junction'){const part=(model.getObjectByName('01 | design 04 integrated irregular blades | face'));target.set(0,.85,0).applyMatrix4(model.matrixWorld);dist=extent*1.05;dir.set(.14,.03,1);}if(view==='back')")
 html=html.replace("if(view==='back')", "if(view==='cloth'){target.y=center.y-extent*.29;target.x=center.x-extent*.035;dist=extent*.92;dir.set(.16,.09,1);}if(view==='back')")
 html=html.replace('초승달 문양 · 흩날리는 붕대','불규칙 보조 칼날 · 원형 관통 장식').replace('<button data-view="cloth">붕대 확대</button>','')
 if (O/'size_comparison.js').exists():
  comparison=(O/'size_comparison.js').read_text(encoding='utf-8').replace('__SIZE_SPEC__',(O/'size_spec.json').read_text(encoding='utf-8')).replace('__BLACK_KNIGHT_REFERENCE__',(O/'black_knight_reference.json').read_text(encoding='utf-8'))
  html=html.replace('</script></html>','\nif(window.viewerReady){\n'+comparison+'\n}\n</script></html>')
 (O/'모델_돌려보기.html').write_text(html,encoding='utf-8')
 print('Offline 3D viewer created')
if __name__=='__main__':
 import sys
 if '--viewer' in sys.argv:viewer()
