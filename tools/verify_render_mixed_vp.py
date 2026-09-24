"""Render real game models through the original and patched Render.dll and compare.

Builds tools/render_engine_check.cpp (Visual Studio 2022, x86), links two test
copies of runtime/client (hard links; every file the game may write is a real
copy, and the two DLLs under test are fresh files), runs the select-screen
scene (stage, animated armour, no fog mode) plus world-style linear fog in both,
and compares the screenshots. runtime/client itself is never written.
"""
from pathlib import Path
import hashlib, json, os, shutil, subprocess, sys

ROOT = Path(__file__).resolve().parents[1]
sys.dont_write_bytecode = True
sys.path[:0] = [str(ROOT / 'runtime/pylibs'), str(ROOT / 'tools'), str(ROOT / 'tools/runtime')]
import numpy as np
import pefile
from PIL import Image
from portable_support import client_drive, release_drive
import patch_render_mixed_vp as patch

WORK = ROOT / '.cache/render-mixed-vp-verify'
BUILD = ROOT / '.cache/render-check'
SOURCE_CLIENT = ROOT / 'runtime/client'
RESULT = ROOT / 'assets/performance/render-mixed-vp-render-check.json'
TICKS = ('0', '400', '900', '1500', '2300', 'fog-900')
VCVARS = Path(os.environ.get('ProgramFiles', r'C:\Program Files')) / 'Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvars32.bat'


def build_checker():
    BUILD.mkdir(parents=True, exist_ok=True)
    (BUILD / 'stub').mkdir(exist_ok=True)
    (BUILD / 'stub/zlib.h').write_bytes(b'')  # WTM.h includes zlib; the checker never uses it
    exports = pefile.PE(str(ROOT / 'client-overlay/Common.dll')).DIRECTORY_ENTRY_EXPORT.symbols
    (BUILD / 'Common.def').write_text('LIBRARY Common.dll\nEXPORTS\n' + ''.join(
        '  ' + e.name.decode() + '\n' for e in exports if e.name), encoding='ascii')
    script = BUILD / 'build.cmd'
    script.write_text('\r\n'.join([
        '@echo off', 'call "%s" >nul' % VCVARS,
        r'lib /nologo /def:.cache\render-check\Common.def /machine:x86 /out:.cache\render-check\Common.lib || exit /b 1',
        r'cl /nologo /EHsc /O2 /MT /wd4819 /wd4996 /I.cache\render-check\stub /Isrc\client\render\Inc '
        r'tools\render_engine_check.cpp /Fo.cache\render-check\ /Fe.cache\render-check\render_engine_check.exe '
        r'/link .cache\render-check\Common.lib user32.lib gdi32.lib /NODEFAULTLIB:zlib.lib', '']), encoding='ascii')
    # Relative paths only: cmd.exe would misread the non-ASCII repository path in the script.
    subprocess.run(['cmd', '/c', str(script)], cwd=ROOT, check=True, capture_output=True)
    return BUILD / 'render_engine_check.exe'


def test_client(name, render, uinterface):
    client = WORK / name / 'client'
    if client.exists():
        shutil.rmtree(client)
    for dirpath, dirs, files in os.walk(SOURCE_CLIENT):
        rel = Path(dirpath).relative_to(SOURCE_CLIENT)
        (client / rel).mkdir(parents=True, exist_ok=True)
        for f in files:
            os.link(Path(dirpath) / f, client / rel / f)
    game = client / 'GameClient'
    for f in game.iterdir():
        # Logs/settings may be appended by the game: never write through a hard link.
        if f.is_file() and f.suffix.lower() not in ('.dll', '.exe'):
            data = f.read_bytes(); f.unlink(); f.write_bytes(data)
    for dll, data in (('Render.dll', render), ('UInterface.dll', uinterface)):
        (game / dll).unlink(); (game / dll).write_bytes(data)
    return client


def run(client, checker):
    shutil.copyfile(checker, client / 'GameClient/render_engine_check.exe')
    drive, created = client_drive(client)
    try:
        game = os.path.join(drive + os.sep, 'GameClient')
        p = subprocess.run([os.path.join(game, 'render_engine_check.exe'), 'check', '300'], cwd=game,
                           capture_output=True, text=True, timeout=600)
        if p.returncode:
            raise RuntimeError('render check failed: ' + p.stdout + p.stderr)
        return json.loads(p.stdout.strip().splitlines()[-1])
    finally:
        if created:
            release_drive(drive, client)


def main():
    patched = (ROOT / 'client-overlay/Render.dll').read_bytes()
    # The patch report restores the exact original bytes from the patched DLL.
    rebuilt, report, original = patch.build()
    assert rebuilt == patched, 'client-overlay/Render.dll is not the validated patch'
    uinterface = (ROOT / 'client-overlay/UInterface.dll').read_bytes()
    checker = build_checker()
    timings, images = {}, {}
    for name, render in (('original', original), ('patched', patched)):
        client = test_client(name, render, uinterface)
        timings[name] = run(client, checker)
        images[name] = {t: np.asarray(Image.open(client / 'GameClient' / f'check-{t}.bmp').convert('RGB'), int) for t in TICKS}
    comparison = {}
    for t in TICKS:
        a, b = images['original'][t], images['patched'][t]
        d = np.abs(a - b).max(axis=2)
        lit_a, lit_b = int((a.max(axis=2) > 8).sum()), int((b.max(axis=2) > 8).sum())
        comparison[t] = dict(lit_original=lit_a, lit_patched=lit_b, differing=int((d > 0).sum()),
                             over_24=int((d > 24).sum()), max=int(d.max()))
        assert abs(lit_a - lit_b) <= d.size // 1000, ('scene coverage changed', t, comparison[t])
        assert comparison[t]['over_24'] <= d.size // 10000, ('visible difference', t, comparison[t])
    poses = [images['original'][t] for t in TICKS[:5]]
    moved = [int((np.abs(poses[i] - poses[i + 1]).max(axis=2) > 24).sum()) for i in range(4)]
    assert all(m > 0 for m in moved), 'animation did not change the pose'
    result = dict(passed=True, render_dll_sha256=report['sha256'], scene='select-screen stage + default male armour, intro_m.ani',
                  timings=timings, comparison=comparison, pose_changes_between_ticks=moved,
                  gpu=subprocess.run(['powershell', '-NoProfile', '-Command',
                                      '(Get-CimInstance Win32_VideoController).Name'], capture_output=True, text=True).stdout.strip())
    RESULT.write_text(json.dumps(result, ensure_ascii=False, indent=2) + '\n', encoding='utf-8', newline='\n')
    print(json.dumps(result, ensure_ascii=False))


if __name__ == '__main__':
    main()
