"""Install the reviewed client payload into the two existing local clients.

No server, database, updater, launcher or private connection settings are written.
Prepare an immutable hash plan first; --apply uses that plan and backs up every
existing changed file before replacing anything.
"""
from pathlib import Path
from datetime import datetime
import argparse
import hashlib
import json
import os
import shutil
import sys

R = Path(__file__).resolve().parents[1]
D = R / 'client-overlay'
F = R.parent / 'https-laqia-co-kr-https-laqia'
PLAN = R / '.cache/final-visual-install-plan.json'
REPORT = R / 'assets/skills140/final-local-installation.json'
sys.dont_write_bytecode = True
sys.path.insert(0, str(D / 'Tools/SkillColors'))
from edit_colors import game_running
from tuning import compile_resources, load_settings, resource_reader


def sha(path):
    p = Path(path)
    return hashlib.sha256(p.read_bytes()).hexdigest() if p.is_file() else None


def roots():
    return {'family': F / 'work/laqia-runtime/client/GameClient',
            'workspace': R / 'runtime/client/GameClient'}


def verify_location(path, root):
    assert path.resolve().is_relative_to(root.resolve()), str(path)
    assert not path.is_symlink(), str(path)


def prepare():
    cfg = load_settings(D / 'Tools/SkillColors')
    colors = json.loads((D / 'Tools/SkillColors/colors.json').read_text())
    compiled, timing = compile_resources(resource_reader(D), cfg, colors)
    for rel, raw in compiled.items():
        assert (D / rel).read_bytes() == raw, 'Unsaved compiled resource: ' + rel
    renderer = json.loads((R / 'assets/skills140/ice-opacity-renderer-patch.json').read_text())
    assert sha(D / 'Render.dll') == renderer['sha256']
    manifest = json.loads((R / 'distribution/update-manifest.json').read_text(encoding='utf-8'))
    payload = [e for e in manifest['files'] if e['kind'] == 'client']
    permitted = {'Effect', 'Texture', 'Tools', 'Equip', 'Monster', 'Item', 'Magic',
                 'Map', 'Interface', 'Sound', 'Vehicle', 'Skill'}
    permitted_single = {'Common.dll', 'DeicideOnline.exe', 'Engine.dll', 'LaqiaCrash.dll',
                        'Media.dll', 'Render.dll', 'Sound.dll', 'start.lua', 'UInterface.dll'}
    stamp = datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    entries = []
    for label, root in roots().items():
        verify_location(root, R.parent)
        assert (root / 'DeicideOnline.exe').is_file()
        backup = ((F / 'work/laqia-runtime/backups') if label == 'family' else R / '.cache') / ('final-visuals-' + stamp)
        for e in payload:
            rel = e['path']
            assert rel.split('/')[0] in permitted or rel in permitted_single, rel
            assert Path(rel).name.lower() not in {'config.ini', 'server.ini', 'arg.txt', 'lang.ini'}
            source, dest = D / rel, root / rel
            verify_location(source, D)
            verify_location(dest, root)
            assert e['source'] == 'client-overlay/' + rel and sha(source) == e['sha256'], rel
            old = sha(dest)
            entries.append(dict(root=label, path=rel, destination=str(dest), before=old,
                                sha256=e['sha256'], changed=old != e['sha256'],
                                backup=str(backup / rel) if old != e['sha256'] and old else None))
    changed_paths = {e['destination'] for e in entries if e['changed']}
    protected_paths = set()
    old_baseline = R / '.cache/v13-before.json'
    if old_baseline.exists():
        protected_paths.update(json.loads(old_baseline.read_text(encoding='utf-8'))['protected'])
    for folder in (R.parent, F, F / 'outputs', R / 'outputs'):
        if folder.is_dir():
            protected_paths.update(str(p) for p in folder.glob('*.cmd'))
    for folder in (R / 'runtime', F / 'work/laqia-runtime'):
        for pattern in ('*.py', '*.json'):
            protected_paths.update(str(p) for p in folder.glob(pattern))
    for root in roots().values():
        for name in ('config.ini', 'server.ini', 'arg.txt', 'lang.ini'):
            if (root / name).is_file():
                protected_paths.add(str(root / name))
    protected_paths.update(str(p) for p in (R / 'game-data/DATA').glob('*') if p.is_file())
    protected_paths.add(str(R / 'server-bin/LAQIA_GameServer'))
    protected = {p: sha(p) for p in sorted(protected_paths - changed_paths) if Path(p).is_file()}
    sculpt = json.loads((R / 'assets/primordial-baphomet/sculpt-validation.json').read_text())
    baphomet = dict(revision=sculpt.get('revision', 'dark-antler-v3'),
                    scale=sculpt.get('scale', 1.4), modelSha256=sha(D / 'Monster/mt_prime_baphomet.mod'))
    plan = dict(timestamp=stamp, files=entries, protected=protected, colors=colors,
                tuning=cfg, compiledResources=len(compiled), timing=timing, baphomet=baphomet)
    PLAN.write_text(json.dumps(plan, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(dict(ready=True, resources=len(entries), changes=len(changed_paths),
                          protected=len(protected), colors=colors), ensure_ascii=False))


def apply():
    plan = json.loads(PLAN.read_text(encoding='utf-8'))
    for e in plan['files']:
        expected = roots()[e['root']] / e['path']
        assert Path(e['destination']) == expected
        verify_location(expected, roots()[e['root']])
        assert sha(D / e['path']) == e['sha256'], 'Source changed: ' + e['path']
        assert sha(expected) == e['before'], 'Destination changed: ' + str(expected)
    for p, digest in plan['protected'].items():
        assert sha(p) == digest, 'Protected file changed: ' + p
    if game_running():
        raise RuntimeError('Game is running; no installed resources were changed.')
    changed = [e for e in plan['files'] if e['changed']]
    for e in changed:
        if e['backup']:
            backup = Path(e['backup'])
            verify_location(backup, R.parent)
            assert not backup.exists(), 'Backup already exists: ' + str(backup)
            backup.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(e['destination'], backup)
            assert sha(backup) == e['before']
    if game_running():
        raise RuntimeError('Game started during backup; installed resources remain unchanged.')
    replaced = []
    try:
        for e in changed:
            dest = Path(e['destination'])
            assert sha(dest) == e['before']
            dest.parent.mkdir(parents=True, exist_ok=True)
            staged = dest.with_name(dest.name + '.final-install.tmp')
            assert not staged.exists(), str(staged)
            try:
                shutil.copy2(D / e['path'], staged)
                assert sha(staged) == e['sha256']
                os.replace(staged, dest)
                replaced.append(e)
            finally:
                if staged.exists():
                    verify_location(staged, roots()[e['root']])
                    staged.unlink()
        for e in plan['files']:
            assert sha(e['destination']) == e['sha256'], e['destination']
        for p, digest in plan['protected'].items():
            assert sha(p) == digest, p
    except BaseException as original_error:
        failures = []
        for e in reversed(replaced):
            try:
                dest = Path(e['destination'])
                if e['backup']:
                    shutil.copy2(e['backup'], dest)
                else:
                    verify_location(dest, roots()[e['root']])
                    dest.unlink()
                assert sha(dest) == e['before']
            except Exception as rollback_error:
                failures.append(dict(destination=e['destination'], backup=e['backup'], error=str(rollback_error)))
        if failures:
            raise RuntimeError('Restore required: ' + json.dumps(failures, ensure_ascii=False)) from original_error
        raise
    report = dict(passed=True, timestamp=plan['timestamp'], files=plan['files'],
                  verifiedResources=len(plan['files']), changedResources=len(changed),
                  protectedFilesUnchanged=len(plan['protected']), colors=plan['colors'],
                  tuning=plan['tuning'], compiledResources=plan['compiledResources'],
                  selection=dict(sword='S4', longbow='H03', staff='월식 갈고리'),
                  baphomet=plan['baphomet'], rendererOpacityInstalled=True,
                  existingLaunchPathsPreserved=True, serverAndDatabasesWritten=False,
                  fatherRemotePcApplied=False, inGameDirect3DTest=False)
    REPORT.write_text(json.dumps(report, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: v for k, v in report.items() if k not in ('files', 'tuning')}, ensure_ascii=False))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    apply() if args.apply else prepare()
