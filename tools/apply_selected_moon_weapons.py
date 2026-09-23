"""Install the approved S4/H03/eclipse weapons into the two existing clients.

Only the twelve named weapon resources can change. No updater, account, server,
skill or launcher is installed. Backups precede all writes and failures roll back.
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
O = R / 'assets/twilight-set'
sys.dont_write_bytecode = True
sys.path.insert(0, str(D / 'Tools/SkillColors'))
from edit_colors import game_running


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    before = json.loads((R / '.cache/v13-before.json').read_text(encoding='utf-8'))
    for path, expected in before['protected'].items():
        assert sha(path) == expected, 'Protected file changed: ' + path

    specs = [('sword', 'mt_twilight', 's4'),
             ('longbow', 'mt_longbow', 'h03-horn-bow'),
             ('staff', 'mt_staff', 'eclipse-hook-staff')]
    resources = []
    for kind, prefix, folder in specs:
        validation = json.loads((O / folder / 'build-validation.json').read_text(encoding='utf-8'))
        assert validation['passed'], folder
        expected = {item['path']: item['sha256'] for item in validation['replacements']}
        paths = [f'Equip/{prefix}_{lod}.mod' for lod in (1, 2, 3)] + [f'Texture/Equip/{prefix}_atlas.wtm']
        assert set(expected) == set(paths), folder
        for relative in paths:
            assert sha(D / relative) == expected[relative], relative
            resources.append((kind, relative, expected[relative]))

    family_runtime = R.parent / 'https-laqia-co-kr-https-laqia/work/laqia-runtime'
    stamp = datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    roots = [('family', family_runtime / 'client/GameClient', family_runtime / 'backups' / ('selected-weapons-' + stamp)),
             ('workspace', R / 'runtime/client/GameClient', R / '.cache' / ('selected-weapons-' + stamp))]
    entries = []
    for label, root, backup in roots:
        assert root.resolve().is_relative_to(R.parent.resolve())
        assert backup.resolve().is_relative_to(R.parent.resolve())
        assert (root / 'DeicideOnline.exe').is_file()
        for kind, relative, digest in resources:
            destination = root / relative
            assert destination.resolve().is_relative_to(root.resolve()) and destination.is_file()
            current = sha(destination)
            assert current in (before['weapons'][str(destination)], digest), 'Installed weapon changed during preparation: ' + str(destination)
            entries.append(dict(root=label, kind=kind, path=relative, destination=str(destination),
                                before=current, sha256=digest, changed=current != digest,
                                backup=str(backup / relative) if current != digest else None))

    if not args.apply:
        print(json.dumps(dict(ready=True,verified=len(entries),changes=sum(e['changed'] for e in entries)),ensure_ascii=False))
        return
    if game_running():
        raise RuntimeError('Game is running; no installed resources were changed.')
    changed = [entry for entry in entries if entry['changed']]
    for entry in changed:
        backup = Path(entry['backup']); backup.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(entry['destination'], backup)
        assert sha(backup) == entry['before']
    if game_running():
        raise RuntimeError('Game started during backup; installed resources remain unchanged.')
    replaced = []
    try:
        for entry in changed:
            destination = Path(entry['destination'])
            assert sha(destination) == entry['before']
            staged = destination.with_name(destination.name + '.v13-install.tmp')
            shutil.copy2(D / entry['path'], staged)
            assert sha(staged) == entry['sha256']
            os.replace(staged, destination)
            replaced.append(entry)
        for entry in entries:
            assert sha(entry['destination']) == entry['sha256'], entry['destination']
        for path, digest in before['protected'].items():
            assert sha(path) == digest, path
    except BaseException as original_error:
        failures = []
        for entry in reversed(replaced):
            try:
                shutil.copy2(entry['backup'], entry['destination'])
                assert sha(entry['destination']) == entry['before']
            except Exception as rollback_error:
                failures.append(dict(destination=entry['destination'],backup=entry['backup'],error=str(rollback_error)))
        if failures:
            raise RuntimeError('Installation failed; these backups still need restoration: ' + json.dumps(failures,ensure_ascii=False)) from original_error
        raise

    report = dict(passed=True, timestamp=stamp, selection=dict(sword='S4',longbow='H03',staff='월식 갈고리'),
                  files=entries, verified_resources=len(entries), replaced_resources=len(changed),
                  protected_files_unchanged=len(before['protected']), existing_launch_paths_preserved=True,
                  item_ids_preserved=True, skill_installation_changed=False, in_game_visual_test=False)
    (O / 'selected-weapons-installation.json').write_text(json.dumps(report,ensure_ascii=False,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:v for k,v in report.items() if k!='files'},ensure_ascii=False))


if __name__ == '__main__':
    main()
