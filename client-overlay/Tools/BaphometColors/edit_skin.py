"""Loopback skin editor; writes only Baphomet textures/settings, never a game launcher or DB."""
from pathlib import Path
from http.server import BaseHTTPRequestHandler
from urllib.request import urlopen
import argparse,base64,hashlib,json,math,os,re,secrets,struct,sys,time,webbrowser,zlib
sys.dont_write_bytecode=True
HERE=Path(__file__).resolve().parent
CLIENT=HERE.parents[1]
sys.path.insert(0,str(HERE.parent/'SkillColors'))
from edit_colors import game_running,ColorServer
DEFAULTS=dict(color='#8A958D',strength=0,brightness=1)

def validate(value):
 if not isinstance(value,dict) or set(value)!=set(DEFAULTS):raise ValueError('피부색 설정이 올바르지 않습니다.')
 if not isinstance(value['color'],str) or not re.fullmatch(r'#[0-9A-Fa-f]{6}',value['color']):raise ValueError('색상은 #RRGGBB 형식이어야 합니다.')
 for key,low,high in [('strength',0,1),('brightness',.25,3)]:
  n=value[key]
  if type(n) not in (int,float) or not math.isfinite(n) or not low<=n<=high:raise ValueError('조절값 범위를 벗어났습니다.')
 return dict(color=value['color'].upper(),strength=value['strength'],brightness=value['brightness'])

def encode(bmp):
 if len(bmp)!=54+1024*1024*3 or bmp[:2]!=b'BM':raise ValueError('잘못된 텍스처입니다.')
 if struct.unpack_from('<I',bmp,2)[0]!=len(bmp) or struct.unpack_from('<I',bmp,10)[0]!=54:raise ValueError('잘못된 BMP 헤더입니다.')
 if struct.unpack_from('<IiiHHI',bmp,14)!=(40,1024,1024,1,24,0):raise ValueError('1024×1024 RGB 텍스처만 저장할 수 있습니다.')
 def chunk(tag,data):return struct.pack('>I',len(data))+tag+data+struct.pack('>I',zlib.crc32(tag+data)&0xffffffff)
 scan=bytearray()
 for y in range(1023,-1,-1):
  row=bmp[54+y*3072:54+(y+1)*3072];rgb=bytearray(3072)
  rgb[0::3]=row[2::3];rgb[1::3]=row[1::3];rgb[2::3]=row[0::3];scan+=b'\0'+rgb
 png=b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',1024,1024,8,2,0,0,0))+chunk(b'IDAT',zlib.compress(scan,9))+chunk(b'IEND',b'')
 return b'TEAMMAY\0\0'+struct.pack('<I',len(bmp))+zlib.compress(bmp,9),png

def texture_size(path):
 """(width, height) of an existing WTM texture, or None when it is absent or not a WTM."""
 try:
  raw=Path(path).read_bytes()
  if raw[:9]!=b'TEAMMAY\0\0':return None
  bmp=zlib.decompressobj().decompress(raw[13:],54)
  return struct.unpack_from('<ii',bmp,18) if bmp[:2]==b'BM' else None
 except (OSError,zlib.error,struct.error):return None

def apply(payload,mirror=None):
 if set(payload)!={'settings','bmp'}:raise ValueError('잘못된 저장 요청입니다.')
 settings=validate(payload['settings']);bmp=base64.b64decode(payload['bmp'],validate=True);wtm,png=encode(bmp)
 # This editor tints the earlier 1024x1024 skin. The current primordial model uses a
 # different atlas, and saving here would replace it with the old skin layout.
 current=texture_size(CLIENT/'Texture/Monster/mt_prime_body.wtm')
 if current is not None and current!=(1024,1024):
  raise ValueError('이 피부색 편집기는 이전 바포메트 모델용입니다. 현재 모델의 텍스처는 바꾸지 않았습니다.')
 if game_running():raise ValueError('게임을 완전히 종료한 뒤 저장해 주세요.')
 cfg=(json.dumps(settings,indent=2)+'\n').encode()
 roots=[CLIENT]
 if mirror:
  mirror=Path(mirror).resolve()
  if not (mirror/'DeicideOnline.exe').is_file():raise ValueError('기존 게임 클라이언트 경로를 찾지 못했습니다.')
  if mirror!=CLIENT:roots.append(mirror)
 prepared={}
 for root in roots:
  prepared[root/'Texture/Monster/mt_prime_body.wtm']=wtm
  prepared[root/'Tools/BaphometColors/settings.json']=cfg
 repo=CLIENT.parent if CLIENT.name=='client-overlay' else None
 if repo and (repo/'assets/primordial-baphomet').is_dir():
  prepared[repo/'assets/primordial-baphomet/body-atlas.png']=png
  prepared[repo/'assets/primordial-baphomet/skin-settings.json']=cfg
  manifest_path=repo/'distribution/update-manifest.json';manifest=json.loads(manifest_path.read_text(encoding='utf-8'))
  for e in manifest['files']:
   p=repo/e['source']
   if p in prepared:e.update(bytes=len(prepared[p]),sha256=hashlib.sha256(prepared[p]).hexdigest())
  prepared[manifest_path]=(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n').encode()
 previous={p:p.read_bytes() if p.exists() else None for p in prepared}
 backup_root=repo/'.local/baphomet-color-backups' if repo else HERE/'backups'
 backup=backup_root/(time.strftime('%Y%m%d-%H%M%S')+'-'+secrets.token_hex(3))
 backup.mkdir(parents=True,exist_ok=True)
 for n,(p,old) in enumerate(previous.items()):
  if old is not None:(backup/f'{n}-{p.name}').write_bytes(old)
 try:
  for p,raw in prepared.items():
   p.parent.mkdir(parents=True,exist_ok=True);temporary=p.with_name(p.name+'.new');temporary.write_bytes(raw);os.replace(temporary,p)
 except BaseException:
  for p,old in previous.items():
   if old is None:p.unlink(missing_ok=True)
   else:p.write_bytes(old)
  raise
 message='배포용 피부색 저장 완료. 커밋·푸시 후 아버지 PC에서 자동업데이트를 실행하면 반영됩니다.' if repo else '피부색 저장 완료. 평소 outputs의 혼자 게임 시작으로 실행해 주세요.'
 return dict(ok=True,message=message,sha256=hashlib.sha256(wtm).hexdigest(),settings=settings)

def main():
 parser=argparse.ArgumentParser();parser.add_argument('--mirror');parser.add_argument('--port',type=int,default=8878);parser.add_argument('--no-browser',action='store_true');args=parser.parse_args()
 token=secrets.token_urlsafe(32);host=f'127.0.0.1:{args.port}';origin='http://'+host
 instance=hashlib.sha256((str(CLIENT)+'|'+str(Path(args.mirror).resolve() if args.mirror else '')).encode()).hexdigest()
 try:
  with urlopen(origin+'/health',timeout=1) as response:existing=json.load(response)
 except (OSError,ValueError):existing={}
 if existing.get('instance')==instance:
  print(origin,flush=True)
  if not args.no_browser:webbrowser.open(origin)
  return
 static={n:'text/javascript' for n in ['viewer.js','skin.js']}
 static.update({n:'image/png' for n in ['base-body.png','blade-atlas.png','cm_0018-original.png','cm_0018_it-original.png']})
 static['model.json']='application/json'
 class Handler(BaseHTTPRequestHandler):
  def log_message(self,*args):pass
  def reply(self,code,data,kind='application/json; charset=utf-8'):
   self.send_response(code);self.send_header('Content-Type',kind);self.send_header('Cache-Control','no-store');self.send_header('X-Content-Type-Options','nosniff');self.end_headers();self.wfile.write(data)
  def do_GET(self):
   if self.headers.get('Host')!=host:return self.reply(403,b'{}')
   path=self.path.split('?')[0].removeprefix('/')
   if path=='health':return self.reply(200,json.dumps(dict(instance=instance)).encode())
   if path=='':
    note='저장 후 커밋·푸시하면, 아버지 PC의 자동업데이트를 통해 적용됩니다.' if CLIENT.name=='client-overlay' else '게임을 종료한 뒤 저장하고, 평소 outputs의 혼자 게임 시작으로 실행하세요.'
    return self.reply(200,(HERE/'index.html').read_text(encoding='utf-8').replace('__TOKEN__',token).replace('__SAVE_NOTE__',note).encode(),'text/html; charset=utf-8')
   if path=='settings.json':return self.reply(200,(HERE/'settings.json').read_bytes())
   if path in static:return self.reply(200,(HERE/path).read_bytes(),static[path])
   if path in ['vendor/three.module.js','vendor/OrbitControls.js']:return self.reply(200,(HERE.parent/'SkillColors'/path).read_bytes(),'text/javascript')
   self.reply(404,b'{}')
  def do_POST(self):
   if self.path!='/apply' or self.headers.get('Host')!=host or self.headers.get('Origin')!=origin or self.headers.get('X-Local-Token')!=token:return self.reply(403,b'{}')
   try:
    n=int(self.headers.get('Content-Length','0'))
    if not 0<n<5*1024*1024:raise ValueError('잘못된 요청 크기입니다.')
    result=apply(json.loads(self.rfile.read(n)),args.mirror);self.reply(200,json.dumps(result,ensure_ascii=False).encode())
   except Exception as e:self.reply(400,json.dumps(dict(ok=False,message=str(e)),ensure_ascii=False).encode())
 server=ColorServer(('127.0.0.1',args.port),Handler)
 print(origin,flush=True)
 if not args.no_browser:webbrowser.open(origin)
 try:server.serve_forever()
 except KeyboardInterrupt:pass
 finally:server.server_close()
if __name__=='__main__':main()
