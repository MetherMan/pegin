"""Read the installed Wind3D effects for the browser preview (stdlib only).

Layout/transform references: SpecialEffectMesh.h and W3DSpecialEffect.cpp.
No substitute meshes, painted textures, or generated animation curves.
"""
from pathlib import Path
import base64
import hashlib
import io
import json
import re
import struct
import wave
import zlib


def png_from_wtm(raw):
    bmp = zlib.decompress(raw[13:])
    tga=bmp[:2]!=b'BM'
    if tga:
        if bmp[1]!=0 or bmp[2]!=2:raise ValueError('Unsupported texture format')
        width,height=struct.unpack_from('<2H',bmp,12);bits=bmp[16];offset=18+bmp[0];compression=0
        if bmp[17]&32:height=-height
    else:
        offset = struct.unpack_from('<I', bmp, 10)[0]
        width, height = struct.unpack_from('<ii', bmp, 18)
        bits = struct.unpack_from('<H', bmp, 28)[0]
        compression = struct.unpack_from('<I', bmp, 30)[0]
    if bits not in (8,24,32) or compression or not 0 < width <= 4096 or not 0 < abs(height) <= 4096:
        raise ValueError('Unsupported effect bitmap format')
    stride = width*bits//8 if tga else ((width * bits + 31) // 32) * 4
    scanlines = bytearray()
    for row in range(abs(height)):
        y = abs(height) - 1 - row if height > 0 else row
        scanlines.append(0)
        for x in range(width):
            at = offset + y * stride + x * (bits // 8)
            if bits==8:
                palette=14+struct.unpack_from('<I',bmp,14)[0]+bmp[at]*4
                b,g,r=bmp[palette:palette+3]
            else:b, g, r = bmp[at:at + 3]
            # Native ONE/ONE blending uses RGB, not texture alpha.
            scanlines.extend((r, g, b))
    def chunk(kind, data):
        return struct.pack('>I', len(data)) + kind + data + struct.pack('>I', zlib.crc32(kind + data))
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>2I5B', width, abs(height), 8, 2, 0, 0, 0))
            + chunk(b'IDAT', zlib.compress(scanlines)) + chunk(b'IEND', b''))


def read_wed(raw):
    if raw[:31].rstrip(b'\0') != b'Wind3D Special Effect Data V1.1':
        raise ValueError('Unsupported WED header')
    at, names = 31, []
    for _ in range(2):
        size = struct.unpack_from('<i', raw, at)[0]
        if not 1 <= size <= 128:
            raise ValueError('Invalid WED resource name')
        at += 4
        name = raw[at:at + size].rstrip(b'\0').decode('cp949')
        if '/' in name or '\\' in name or '..' in name:
            raise ValueError('Invalid WED resource path')
        names.append(name)
        at += size
    start, end, count, cols, rows = struct.unpack_from('<5i', raw, at)
    at += 20
    if not (0 <= start < end < count <= 4096 and cols > 0 and rows > 0 and at + count * 52 == len(raw)):
        raise ValueError('Invalid WED frame table')
    return dict(mesh=names[0], texture=Path(names[1]).with_suffix('.wtm').name,
                start=start, end=end, cols=cols, rows=rows,
                frames=[list(struct.unpack_from('<11fIi', raw, at + i * 52)) for i in range(count)])


def read_wem(raw):
    has_mesh, faces, vertices = struct.unpack_from('<3i', raw, 100)
    if not has_mesh or not 0 < vertices <= 65535 or not 0 < faces <= 65535:
        raise ValueError('Invalid WEM geometry')
    at = 112 + vertices * 20
    count = struct.unpack_from('<i', raw, at)[0]
    indices = list(struct.unpack_from(f'<{count}H', raw, at + 4))
    if count != faces * 3 or max(indices) >= vertices or len(raw) - (at + 4 + count * 2) not in (0, 40):
        raise ValueError('Invalid WEM index table')
    # matTM at bytes 36..99 is deliberately unused by native CEffectMesh::Render.
    return dict(vertices=[list(struct.unpack_from('<5f', raw, 112 + i * 20)) for i in range(vertices)], indices=indices)


def read_magic(text):
    effects = []
    for block in re.findall(r'\[EFFECT\](.*?)\[/EFFECT\]', text, re.S):
        header = block.split('[PART]', 1)[0]
        spe = re.search(r'\[SPE\]\s+(\S+)', header)
        trace = re.search(r'\[TRACE\]\s+(\S+)', header)
        parts = []
        for body in re.findall(r'\[PART\](.*?)\[/PART\]', block, re.S):
            blow=re.search(r'\[BLOWTIMING\](.*?)\[/BLOWTIMING\]',body,re.S)
            blow_sound=re.search(r'\[SOUND\]\s+(\S+)',blow[1]) if blow else None
            body = re.sub(r'\[BLOWTIMING\].*?\[/BLOWTIMING\]', '', body, flags=re.S)
            part = {}
            if blow_sound:part['HITSOUND']=blow_sound[1]
            for tag, value in re.findall(r'\[([A-Z]+)\]([^\[\r\n]*)', body):
                value = value.strip()
                if tag in ('POS', 'TARGET'):
                    part[tag] = [float(v) for v in value.split()]
                elif tag in ('STARTTIME', 'TIMELIMIT', 'SPEED', 'MOVETYPE','BONE'):
                    part[tag] = int(value)
                elif tag == 'TRACEDIST':
                    part[tag] = float(value)
                elif tag in ('ATTACK', 'NOSHOW', 'LOOP', 'PAUSE', 'ENEMY'):
                    part[tag] = True
                elif tag == 'TAIL':
                    part[tag] = value
                elif tag == 'SOUND':
                    delay,name,style=value.split();part['SOUND']=dict(delay=int(delay),name=name,style=int(style))
            parts.append(part)
        effects.append(dict(wed=spe[1] if spe else None, trace=trace[1] if trace else None, loop='[LOOP]' in header, parts=parts))
    return effects


def read_traces(raw):
    version, count = struct.unpack_from('<2i', raw)
    if version != 100 or not 0 < count < 1000:
        raise ValueError('Invalid trace table')
    at, traces = 8, {}
    for _ in range(count):
        names = []
        for _ in range(2):
            size = struct.unpack_from('<i', raw, at)[0]
            at += 4
            names.append(raw[at:at+size].rstrip(b'\0').decode('cp949'))
            at += size
        level, rate = struct.unpack_from('<2i', raw, at)
        at += 8
        traces[names[0]] = dict(texture=Path(names[1]).with_suffix('.wtm').name, level=level, rate=rate)
    if at != len(raw):
        raise ValueError('Invalid trace table size')
    return traces


def load_preview(here, client, mirror=None,generated=None,metadata=None,compact=False):
    roots = [client]
    if mirror:
        roots.append(Path(mirror).resolve())
    if client.name == 'client-overlay':
        roots.append(client.parent / 'runtime/client/GameClient')
    sources = {}
    def read(relative):
        if generated and relative in generated:
            raw=generated[relative];sources[relative]=hashlib.sha256(raw).hexdigest();return raw
        for root in roots:
            path = root / relative
            if path.is_file():
                raw = path.read_bytes()
                sources[relative] = hashlib.sha256(raw).hexdigest()
                return raw
        raise FileNotFoundError('게임 원본 파일을 찾지 못했습니다: ' + relative)
    result = dict(effects={}, meshes={}, textures={}, scripts={}, traces={}, sounds={}, soundDurations={}, sources=sources, fps=30)
    if generated is None:
        from tuning import compile_resources,load_settings
        generated,metadata=compile_resources(read,load_settings(here),json.loads((here/'colors.json').read_text()))
    result['timing']=metadata
    scripts = dict(meteor=dict(A='mt_meteorA',B='mt_meteorB'), frost=dict(A='mt_frostA',B='mt_frostB'),
                   heaven=dict(B='mt_heaven'), six=dict(B='mt_six_blue',N='mt_six'))
    required, trace_names = set(), set()
    for kind, phases in scripts.items():
        result['scripts'][kind] = {}
        for phase, script in phases.items():
            effects = read_magic(read('Magic/' + script + '.ms').decode('cp949'))
            result['scripts'][kind][phase] = effects
            for effect in effects:
                if effect['wed']:required.add(effect['wed'])
                if effect['trace']:
                    trace_names.add(effect['trace'])
                required.update(part['TAIL'] for part in effect['parts'] if 'TAIL' in part)
                for part in effect['parts']:
                    for name in ([part['SOUND']['name']] if 'SOUND' in part else [])+([part['HITSOUND']] if 'HITSOUND' in part else []):
                        if not compact and name not in result['sounds']:
                            raw=read('Sound/'+name)
                            result['sounds'][name]='data:audio/wav;base64,'+base64.b64encode(raw).decode()
                            with wave.open(io.BytesIO(raw),'rb') as sound:
                                result['soundDurations'][name]=sound.getnframes()/sound.getframerate()*1000
    def texture(name):
        if name not in result['textures']:
            png = png_from_wtm(read('Texture/Effect/' + name))
            result['textures'][name] = 'data:image/png;base64,' + base64.b64encode(png).decode('ascii')
    for name in sorted(required):
        raw = read('Effect/' + name)
        effect = dict(read_wed(raw), tintable=False)
        result['effects'][name] = effect
        if not compact and effect['mesh'] not in result['meshes']:
            result['meshes'][effect['mesh']] = read_wem(read('Effect/' + effect['mesh']))
        if not compact:texture(effect['texture'])
    traces = read_traces(read('Effect/trace.tdf'))
    for name in sorted(trace_names):
        result['traces'][name] = traces[name]
        if not compact:texture(traces[name]['texture'])
    if not compact:
        from native_actor import load_actors
        result['actors']=load_actors(read,png_from_wtm)
    return result
