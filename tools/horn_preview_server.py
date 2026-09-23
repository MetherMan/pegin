"""Loopback horn-strength editor; saves textures and updates existing clients."""
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from datetime import datetime
from pathlib import Path
from urllib.parse import urlsplit
import contextlib, io, json, shutil, subprocess, sys, threading

sys.dont_write_bytecode = True
from horn_strength import R, O, D, SETTINGS, load_strength, validate_strength, base_tile, png_bytes, prepared_files, atomic_write
sys.path.insert(0, str(D / 'Tools/SkillColors'))
from edit_colors import game_running
import apply_final_visuals as installer

LOCK = threading.Lock()
ORIGIN = 'http://127.0.0.1:8879'
MANIFEST = R / 'distribution/update-manifest.json'


def save_strength(strength):
    strength = validate_strength(strength)
    with LOCK:
        if game_running():
            raise ValueError('게임을 완전히 종료한 뒤 저장해 주세요. 변경된 파일은 없습니다.')
        files = prepared_files(strength)
        snapshots = {path: path.read_bytes() if path.exists() else None
                     for path in [*files, MANIFEST, installer.PLAN, installer.REPORT]}
        version = json.loads(snapshots[MANIFEST])['version']
        backup = R / '.cache/horn-strength-backups' / datetime.now().strftime('%Y%m%d-%H%M%S-%f')
        if game_running():
            raise ValueError('게임이 실행되어 저장하지 않았습니다.')
        for path, content in snapshots.items():
            if content is not None:
                dest = backup / path.relative_to(R)
                dest.parent.mkdir(parents=True, exist_ok=True)
                dest.write_bytes(content)
        plan = None
        try:
            for path, content in files.items():
                atomic_write(path, content)
            subprocess.run([sys.executable, '-B', '-X', 'utf8', str(R / 'tools/package_father_update.py'),
                            '--manifest-only', '--version', version], cwd=R, capture_output=True, check=True)
            with contextlib.redirect_stdout(io.StringIO()):
                installer.prepare()
                plan = json.loads(installer.PLAN.read_text(encoding='utf-8'))
                installer.apply()
        except BaseException as original:
            failures = []
            # apply() has its own rollback. Also cover a failure while writing
            # its final report after the client files were already replaced.
            if plan:
                for entry in reversed(plan['files']):
                    if not entry['changed']:
                        continue
                    dest = Path(entry['destination'])
                    current = installer.sha(dest)
                    if current == entry['before']:
                        continue
                    try:
                        assert current == entry['sha256'], 'Installed file changed concurrently'
                        if entry['backup']:
                            prior = Path(entry['backup'])
                            assert installer.sha(prior) == entry['before']
                            atomic_write(dest, prior.read_bytes())
                        elif entry['before'] is None:
                            dest.unlink()
                    except Exception as error:
                        failures.append(str(dest) + ': ' + str(error))
            for path, content in snapshots.items():
                try:
                    if content is None:
                        if path.exists():
                            path.unlink()
                    else:
                        atomic_write(path, content)
                except Exception as error:
                    failures.append(str(path) + ': ' + str(error))
            if failures:
                raise RuntimeError('백업 복구 확인 필요: ' + '; '.join(failures)) from original
            raise
        return {'strength': strength, 'saved': True}


class Handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=str(R), **kwargs)

    def log_message(self, *args):
        pass

    def end_headers(self):
        self.send_header('Cache-Control', 'no-store')
        self.send_header('X-Content-Type-Options', 'nosniff')
        super().end_headers()

    def reply(self, status, value):
        data = json.dumps(value, ensure_ascii=False).encode('utf-8')
        self.send_response(status)
        self.send_header('Content-Type', 'application/json; charset=utf-8')
        self.send_header('Content-Length', str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        if self.headers.get('Host') != '127.0.0.1:8879':
            return self.reply(403, {'error': 'Loopback host required'})
        path = urlsplit(self.path).path
        if path == '/api/horn-strength':
            return self.reply(200, {'strength': load_strength()})
        if path in ('/api/horn-base/0.png', '/api/horn-base/1.png'):
            data = png_bytes(base_tile(int(path[-5])))
            self.send_response(200)
            self.send_header('Content-Type', 'image/png')
            self.send_header('Content-Length', str(len(data)))
            self.end_headers()
            return self.wfile.write(data)
        return super().do_GET()

    def do_POST(self):
        if self.headers.get('Host') != '127.0.0.1:8879' or self.headers.get('Origin') != ORIGIN:
            return self.reply(403, {'error': 'Same-origin loopback request required'})
        if urlsplit(self.path).path != '/api/horn-strength':
            return self.reply(404, {'error': 'Unknown endpoint'})
        try:
            size = int(self.headers.get('Content-Length', '0'))
            if not 0 < size <= 1024:
                raise ValueError('Invalid request size')
            if self.headers.get('Content-Type', '').split(';')[0].strip() != 'application/json':
                raise ValueError('JSON required')
            value = json.loads(self.rfile.read(size))
            if not isinstance(value, dict) or set(value) != {'strength'}:
                raise ValueError('Expected strength only')
            result = save_strength(value['strength'])
        except Exception as error:
            return self.reply(400, {'error': str(error)})
        self.reply(200, result)


if __name__ == '__main__':
    ThreadingHTTPServer(('127.0.0.1', 8879), Handler).serve_forever()
