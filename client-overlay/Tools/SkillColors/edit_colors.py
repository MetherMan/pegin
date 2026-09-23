"""Loopback-only color editor for the local game's private level-140 effects."""
from pathlib import Path
from http.server import HTTPServer,BaseHTTPRequestHandler
import argparse,json,secrets,re,struct,webbrowser,hashlib,os,ctypes,socket,sys,time
sys.dont_write_bytecode=True
from ctypes import wintypes
from urllib.request import urlopen
HERE=Path(__file__).resolve().parent
CLIENT=HERE.parents[1]
if str(HERE) not in sys.path:sys.path.insert(0,str(HERE))
from native_assets import load_preview
from tuning import DEFAULTS,FIELDS,load_settings,validate_settings,validate_colors,resource_reader,compile_resources

class ColorServer(HTTPServer):
 allow_reuse_address=False
 def server_bind(self):
  if hasattr(socket,'SO_EXCLUSIVEADDRUSE'):
   self.socket.setsockopt(socket.SOL_SOCKET,socket.SO_EXCLUSIVEADDRUSE,1)
  super().server_bind()
def validate(value):
 colors=validate_colors(value)
 # A panel opened before frostIce existed still posts two keys. Preserve the
 # latest saved body colour instead of silently resetting it to default blue.
 if 'frostIce' not in value and (HERE/'colors.json').is_file():
  saved=validate_colors(json.loads((HERE/'colors.json').read_text()))
  colors['frostIce']=saved['frostIce']
 return colors
def recolor(raw,color):
 b=bytearray(raw);at=31
 for _ in range(2):n=struct.unpack_from('<i',b,at)[0];at+=4+n
 count=struct.unpack_from('<i',b,at+8)[0];at+=20;assert at+count*52==len(b)
 rgb=[int(color[i:i+2],16) for i in [1,3,5]]
 for j in range(count):
  off=at+j*52+44;v=struct.unpack_from('<I',b,off)[0];lum=v&255;c=[round(x*lum/255) for x in rgb]
  struct.pack_into('<I',b,off,(v&0xff000000)|(c[0]<<16)|(c[1]<<8)|c[2])
 return bytes(b)
def game_running():
 class Entry(ctypes.Structure):
  _fields_=[('size',wintypes.DWORD),('usage',wintypes.DWORD),('pid',wintypes.DWORD),('heap',ctypes.c_size_t),('module',wintypes.DWORD),('threads',wintypes.DWORD),('parent',wintypes.DWORD),('priority',wintypes.LONG),('flags',wintypes.DWORD),('exe',wintypes.WCHAR*260)]
 k=ctypes.WinDLL('kernel32',use_last_error=True);k.CreateToolhelp32Snapshot.restype=wintypes.HANDLE
 k.Process32FirstW.argtypes=[wintypes.HANDLE,ctypes.POINTER(Entry)];k.Process32NextW.argtypes=k.Process32FirstW.argtypes;k.CloseHandle.argtypes=[wintypes.HANDLE]
 h=k.CreateToolhelp32Snapshot(2,0)
 if h==ctypes.c_void_p(-1).value:raise ctypes.WinError(ctypes.get_last_error())
 try:
  e=Entry();e.size=ctypes.sizeof(e);ok=k.Process32FirstW(h,ctypes.byref(e))
  if not ok:raise ctypes.WinError(ctypes.get_last_error())
  while ok:
   if e.exe.lower()=='deicideonline.exe':return True
   ok=k.Process32NextW(h,ctypes.byref(e))
  return False
 finally:k.CloseHandle(h)
def apply(colors,mirror=None,settings=None):
 colors=validate(colors)
 if game_running():raise ValueError('게임을 완전히 종료한 뒤 다시 적용해 주세요.')
 roots=[CLIENT]
 if mirror:
  mirror=Path(mirror).resolve()
  if not (mirror/'DeicideOnline.exe').is_file():raise ValueError('기존 게임 설치 경로를 찾지 못했습니다.')
  if mirror!=CLIENT:roots.append(mirror)
 settings=load_settings(HERE) if settings is None else validate_settings(settings)
 prepared,meta=compile_resources(resource_reader(CLIENT,mirror),settings,colors)
 store_resources(prepared,roots,meta)
 return {'ok':True,'message':'색상·속도·거리·크기 적용 완료. 평소 게임 시작 버튼으로 다시 실행해 주세요.','colors':colors,'tuning':settings,'timing':meta}

def save_preview(colors,settings):
 # Preview saving is deliberately limited to the development overlay. It must
 # never silently become a write to a playable installation or mirror.
 if CLIENT.name!='client-overlay':raise ValueError('이 설치에서는 미리보기 설정 저장을 지원하지 않습니다.')
 colors=validate(colors);settings=validate_settings(settings)
 prepared,meta=compile_resources(resource_reader(CLIENT),settings,colors)
 store_resources(prepared,[CLIENT],meta)
 return {'ok':True,'message':'미리보기 설정을 저장했습니다. 새로고침해도 유지되며 설치된 게임에는 반영되지 않습니다.','colors':colors,'tuning':settings,'timing':meta}

def store_resources(prepared,roots,metadata=None):
 originals={}
 try:
  for root in roots:
   for rel,data in prepared.items():
    p=root/rel;p.parent.mkdir(parents=True,exist_ok=True);originals[p]=p.read_bytes() if p.exists() else None
    temp=p.with_suffix(p.suffix+'.new');temp.write_bytes(data);os.replace(temp,p)
  # In the development workspace, keep the published overlay/manifest consistent.
  repo=CLIENT.parent
  if CLIENT.name=='client-overlay' and (repo/'distribution/update-manifest.json').is_file():
   for name in ('colors.json','tuning.json'):
    cfg=repo/'assets/skills140'/name;originals[cfg]=cfg.read_bytes() if cfg.exists() else None;cfg.write_bytes(prepared['Tools/SkillColors/'+name])
   if metadata:
    timing=repo/'game-data/DATA/SKILL140_DEATH.txt'
    originals[timing]=timing.read_bytes() if timing.exists() else None
    timing.write_bytes(('19122 %d\n19123 %d\n'%(metadata['meteor']['finalImpact'],metadata['frost']['pillarAt'])).encode('ascii'))
    report=repo/'assets/skills140/effect-timing.json';originals[report]=report.read_bytes() if report.exists() else None
    report.write_text(json.dumps(metadata,indent=2)+'\n',encoding='utf-8')
   p=repo/'distribution/update-manifest.json';originals[p]=p.read_bytes();manifest=json.loads(originals[p])
   for entry in manifest['files']:
    if entry['kind']=='client' and entry['path'] in prepared:
     data=prepared[entry['path']];entry.update(bytes=len(data),sha256=hashlib.sha256(data).hexdigest())
    if metadata and entry['kind']=='server' and entry['path']=='DATA/SKILL140_DEATH.txt':
     data=timing.read_bytes();entry.update(bytes=len(data),sha256=hashlib.sha256(data).hexdigest())
   p.write_text(json.dumps(manifest,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
 except BaseException:
  for p,data in originals.items():
   if data is None:p.unlink(missing_ok=True)
   else:p.write_bytes(data)
  raise
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--mirror');parser.add_argument('--no-browser',action='store_true');parser.add_argument('--preview-only',action='store_true');parser.add_argument('--port',type=int,default=8874);args=parser.parse_args()
 token=secrets.token_urlsafe(32)
 url=f'http://127.0.0.1:{args.port}/'
 instance=hashlib.sha256((str(CLIENT)+'|'+str(Path(args.mirror).resolve() if args.mirror else '')+('|preview-only' if args.preview_only else '')).encode()).hexdigest()
 class Handler(BaseHTTPRequestHandler):
  def log_message(self,*args):pass
  def reply(self,code,data,kind='application/json; charset=utf-8'):
   self.send_response(code);self.send_header('Content-Type',kind);self.send_header('Cache-Control','no-store');self.send_header('X-Content-Type-Options','nosniff');self.end_headers();self.wfile.write(data)
  def do_GET(self):
   if self.headers.get('Host')!=f'127.0.0.1:{args.port}':return self.reply(403,b'{}')
   if self.path=='/health':return self.reply(200,json.dumps({'app':'laqia-skill-colors','instance':instance}).encode())
   if self.path=='/preview.json':
    try:return self.reply(200,json.dumps(load_preview(HERE,CLIENT,args.mirror),separators=(',',':')).encode())
    except Exception as e:return self.reply(500,json.dumps({'error':str(e)},ensure_ascii=False).encode())
   static={'/preview.js':'text/javascript; charset=utf-8','/actor.js':'text/javascript; charset=utf-8','/controls.js':'text/javascript; charset=utf-8','/live_preview_queue.js':'text/javascript; charset=utf-8','/style.css':'text/css; charset=utf-8','/vendor/three.module.js':'text/javascript; charset=utf-8','/vendor/OrbitControls.js':'text/javascript; charset=utf-8'}
   if self.path in static:return self.reply(200,(HERE/self.path[1:]).read_bytes(),static[self.path])
   if self.path!='/':return self.reply(404,b'{}')
   cfg=validate(json.loads((HERE/'colors.json').read_text()));s=(HERE/'index.html').read_text(encoding='utf-8').replace('__TOKEN__',token).replace('__COLORS__',json.dumps(cfg)).replace('__TUNING__',json.dumps(load_settings(HERE))).replace('__FIELDS__',json.dumps(FIELDS,ensure_ascii=False)).replace('__DEFAULTS__',json.dumps(DEFAULTS))
   if args.preview_only:s=s.replace('<body>','<body data-preview-only="true">').replace('<h1>스킬 연출 조절</h1>','<h1>스킬 연출 미리보기</h1>').replace('<button id="apply" disabled>','<button id="apply" disabled style="display:none">').replace('게임을 종료한 상태에서 적용해 주세요.','미리보기 시안 · 이 창의 조절은 설치된 게임에 반영되지 않습니다.')
   self.reply(200,s.encode('utf-8'),'text/html; charset=utf-8')
  def do_POST(self):
   if self.headers.get('Host')!=f'127.0.0.1:{args.port}' or self.headers.get('Origin',url[:-1])!=url[:-1]:return self.reply(403,b'{}')
   if self.path not in ('/apply','/preview','/save-preview') or self.headers.get('X-Local-Token')!=token:return self.reply(403,b'{}')
   if self.path=='/apply' and args.preview_only:return self.reply(403,json.dumps({'ok':False,'message':'미리보기 전용 창입니다.'},ensure_ascii=False).encode())
   if self.path=='/save-preview' and not args.preview_only:return self.reply(403,b'{}')
   try:
    n=int(self.headers.get('Content-Length','0'))
    if not 0<n<131072:raise ValueError('잘못된 요청 크기입니다.')
    payload=json.loads(self.rfile.read(n))
    if not isinstance(payload,dict) or not {'colors','tuning'}<=set(payload) or not set(payload)<={'colors','tuning','kinds','knownSources'}:raise ValueError('잘못된 설정 요청입니다.')
    colors=validate(payload['colors']);settings=validate_settings(payload['tuning'])
    if self.path=='/preview':
     started=time.perf_counter();kinds=payload.get('kinds');known=payload.get('knownSources',{})
     if kinds is not None and (not isinstance(kinds,list) or not 1<=len(kinds)<=4 or not all(isinstance(k,str) and k in DEFAULTS for k in kinds)):raise ValueError('잘못된 미리보기 스킬입니다.')
     if not isinstance(known,dict) or len(known)>1000 or not all(isinstance(k,str) and isinstance(v,str) and re.fullmatch(r'[0-9a-f]{64}',v) for k,v in known.items()):raise ValueError('잘못된 미리보기 자산 정보입니다.')
     generated,meta=compile_resources(resource_reader(CLIENT,args.mirror),settings,colors,only=kinds)
     result=load_preview(HERE,CLIENT,args.mirror,generated=generated,metadata=meta,compact=True,kinds=kinds,known_sources=known)
     result['previewMetrics']={'serverMilliseconds':round((time.perf_counter()-started)*1000,1),'kinds':list(meta)}
    elif self.path=='/save-preview':result=save_preview(colors,settings)
    else:result=apply(colors,args.mirror,settings)
    self.reply(200,json.dumps(result,ensure_ascii=False,separators=(',',':')).encode())
   except Exception as e:self.reply(400,json.dumps({'ok':False,'message':str(e)},ensure_ascii=False).encode())
 try:server=ColorServer(('127.0.0.1',args.port),Handler)
 except OSError:
  with urlopen(url+'health',timeout=2) as response:existing=json.load(response)
  if existing!={'app':'laqia-skill-colors','instance':instance}:raise RuntimeError('다른 설치본의 색상 설정 창이 열려 있습니다. 기존 설정 창의 실행 콘솔을 닫고 다시 실행해 주세요.')
  if not args.no_browser:webbrowser.open(url)
  print('이미 열린 색상 설정 창을 사용합니다.',flush=True);return
 print(url,flush=True)
 if not args.no_browser:webbrowser.open(url)
 try:server.serve_forever()
 except KeyboardInterrupt:pass
 finally:server.server_close()
if __name__=='__main__':main()
