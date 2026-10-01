"""Server readiness inside the VM, game-file self repair, and updater housekeeping."""
from pathlib import Path
from unittest import mock
import contextlib, hashlib, importlib.util, io, json, os, shutil, sys, time, types, unittest, uuid

ROOT = Path(__file__).resolve().parents[1]
TEMPROOT = ROOT / '.cache/update-maintenance-tests'
TEMPROOT.mkdir(parents=True, exist_ok=True)

fake_modules = {'ssh_vm': types.SimpleNamespace(connect=mock.Mock()),
                'portable_support': types.SimpleNamespace(client_running=mock.Mock(), client_drive=mock.Mock(),
                                                          release_drive=mock.Mock(), preflight=mock.Mock())}
spec = importlib.util.spec_from_file_location('maintenance_control', ROOT / 'tools/runtime/local_control.py')
ctl = importlib.util.module_from_spec(spec)
with mock.patch.dict(sys.modules, fake_modules), mock.patch('logging.basicConfig'):
    spec.loader.exec_module(ctl)
spec = importlib.util.spec_from_file_location('maintenance_updater', ROOT / 'distribution/father_update.py')
u = importlib.util.module_from_spec(spec)
spec.loader.exec_module(u)


@contextlib.contextmanager
def folder():
    p = (TEMPROOT / uuid.uuid4().hex).resolve()
    p.mkdir()
    try:
        yield p
    finally:
        assert p.is_relative_to(TEMPROOT.resolve())
        shutil.rmtree(p)


class Clock:
    def __init__(self): self.now = 0
    def monotonic(self): return self.now
    def sleep(self, seconds): self.now += seconds


LISTEN = 'LISTEN 0 128 0.0.0.0:{} 0.0.0.0:*\n'


class ServiceReadinessTests(unittest.TestCase):
    def test_waits_until_both_guest_ports_listen(self):
        clock = Clock()
        outputs = iter(['', LISTEN.format(8030), LISTEN.format(8030) + LISTEN.format(2560)])
        with mock.patch.object(ctl, 'time', clock), mock.patch.object(ctl, 'command', side_effect=lambda c, t: next(outputs)):
            with contextlib.redirect_stdout(io.StringIO()):
                ctl.wait_for_services(object())
        self.assertEqual(clock.now, 4)

    def test_host_forward_alone_is_not_ready(self):
        # QEMU accepts on the host side even when nothing listens in the VM.
        clock = Clock()
        with mock.patch.object(ctl, 'time', clock), mock.patch.object(ctl, 'port_open', return_value=True):
            with mock.patch.object(ctl, 'command', return_value=LISTEN.format(22)):
                with contextlib.redirect_stdout(io.StringIO()), self.assertRaisesRegex(RuntimeError, '접속 준비'):
                    ctl.wait_for_services(object(), timeout=20)
        self.assertEqual(clock.now, 20)

    def test_ipv6_and_wildcard_listeners_count(self):
        with mock.patch.object(ctl, 'command', return_value='LISTEN 0 5 *:8030 *:*\nLISTEN 0 5 [::]:2560 [::]:*\n'):
            self.assertEqual(ctl.listening_ports(object()), {8030, 2560})


class UpdatedServerTests(unittest.TestCase):
    def run_states(self, states, ports):
        clock, calls = Clock(), []
        states, ports = iter(states), iter(ports)
        def cmd(text):
            calls.append(text)
            return next(states) if text.startswith('systemctl') else next(ports)
        u.wait_game_server(cmd, timeout=30, clock=clock)
        return clock, calls

    def test_ready_only_after_listening_and_still_running(self):
        clock, calls = self.run_states(['activating', 'active', 'active', 'active'], ['', LISTEN.format(2560)])
        self.assertTrue(any('ss -Hltn' in c for c in calls))
        self.assertEqual(clock.now, 2 + 2 + 5)

    def test_crash_after_binding_is_a_failure(self):
        with self.assertRaisesRegex(RuntimeError, '종료'):
            self.run_states(['active', 'failed'], [LISTEN.format(2560)])

    def test_exit_during_load_is_a_failure(self):
        with self.assertRaisesRegex(RuntimeError, '종료'):
            self.run_states(['activating', 'inactive'], [])

    def test_never_listening_times_out(self):
        with self.assertRaisesRegex(RuntimeError, '포트'):
            self.run_states(['active'] * 40, [''] * 40)


class ClientRepairTests(unittest.TestCase):
    def install(self, p, files):
        runtime = p / 'work/laqia-runtime'
        client = runtime / 'client/GameClient'
        client.mkdir(parents=True)
        entries = []
        for name, data in files.items():
            entries.append(dict(kind='client', path=name, source='client-overlay/' + name,
                                bytes=len(data), sha256=hashlib.sha256(data).hexdigest()))
        manifest = dict(format=1, repository=u.REPOSITORY, version='t', revision='a' * 40, files=entries)
        (runtime / 'installed-manifest.json').write_text(json.dumps(manifest), encoding='utf-8')
        return runtime, client

    def test_missing_and_truncated_files_are_restored_verified(self):
        good, lost, cut = b'good-dll', b'quarantined-dll', b'complete-effect-file'
        with folder() as p:
            runtime, client = self.install(p, {'Render.dll': good, 'UInterface.dll': lost, 'Effect/a.wed': cut})
            (client / 'Render.dll').write_bytes(good)
            (client / 'Effect').mkdir()
            (client / 'Effect/a.wed').write_bytes(cut[:5])
            cache = p / 'update/cache'
            cache.mkdir(parents=True)
            (cache / hashlib.sha256(lost).hexdigest()).write_bytes(lost)
            response = mock.MagicMock()
            response.__enter__.return_value.read.return_value = cut
            with mock.patch.object(ctl, 'ROOT', runtime), mock.patch.object(ctl.urllib.request, 'urlopen', return_value=response) as get:
                with contextlib.redirect_stdout(io.StringIO()):
                    self.assertEqual(ctl.repair_client_files(), 2)
            self.assertEqual((client / 'UInterface.dll').read_bytes(), lost)
            self.assertEqual((client / 'Effect/a.wed').read_bytes(), cut)
            url = get.call_args.args[0].full_url
            self.assertEqual(url, 'https://raw.githubusercontent.com/MetherMan/pegin/' + 'a' * 40 + '/client-overlay/Effect/a.wed')

    def test_wrong_download_is_never_written(self):
        with folder() as p:
            runtime, client = self.install(p, {'Engine.dll': b'expected-bytes'})
            response = mock.MagicMock()
            response.__enter__.return_value.read.return_value = b'tampered-bytes'
            with mock.patch.object(ctl, 'ROOT', runtime), mock.patch.object(ctl.urllib.request, 'urlopen', return_value=response):
                with contextlib.redirect_stdout(io.StringIO()), mock.patch.object(ctl.logging, 'warning'):
                    self.assertEqual(ctl.repair_client_files(), 0)
            self.assertFalse((client / 'Engine.dll').exists())
            self.assertEqual(list(client.iterdir()), [])

    def test_same_size_local_edit_and_unsafe_paths_are_left_alone(self):
        with folder() as p:
            runtime, client = self.install(p, {'Effect/color.wed': b'AAAA'})
            manifest = json.loads((runtime / 'installed-manifest.json').read_text(encoding='utf-8'))
            manifest['files'].append(dict(kind='client', path='../../escape.txt', source='client-overlay/../../escape.txt',
                                          bytes=1, sha256='0' * 64))
            (runtime / 'installed-manifest.json').write_text(json.dumps(manifest), encoding='utf-8')
            (client / 'Effect').mkdir()
            (client / 'Effect/color.wed').write_bytes(b'BBBB')
            with mock.patch.object(ctl, 'ROOT', runtime), mock.patch.object(ctl.urllib.request, 'urlopen') as get:
                self.assertEqual(ctl.repair_client_files(), 0)
            get.assert_not_called()
            self.assertEqual((client / 'Effect/color.wed').read_bytes(), b'BBBB')
            self.assertFalse((p / 'work/escape.txt').exists())

    def test_without_installed_manifest_nothing_happens(self):
        with folder() as p, mock.patch.object(ctl, 'ROOT', p), mock.patch.object(ctl.urllib.request, 'urlopen') as get:
            self.assertEqual(ctl.repair_client_files(), 0)
            get.assert_not_called()


class HousekeepingTests(unittest.TestCase):
    def test_prune_local_keeps_newest_backups_and_current_cache(self):
        with folder() as p:
            runtime = p / 'work/laqia-runtime'
            names = ['auto-update-20260901-000000-aaaaaaaa', 'auto-update-20260902-000000-bbbbbbbb',
                     'auto-update-20260903-000000-cccccccc', 'auto-update-20260904-000000-dddddddd', 'local-keep']
            for n in names:
                (runtime / 'backups' / n).mkdir(parents=True)
            (runtime / 'backups/local-20260901.sql').write_bytes(b'db')
            keep, old = hashlib.sha256(b'k').hexdigest(), hashlib.sha256(b'o').hexdigest()
            cache = p / 'update/cache'
            cache.mkdir(parents=True)
            for h in (keep, old):
                (cache / h).write_bytes(b'x')
            (cache / 'notes.txt').write_bytes(b'x')
            manifest = dict(files=[dict(sha256=keep)], runtime_files=[])
            u.prune_local(p, runtime, manifest)
            left = sorted(x.name for x in (runtime / 'backups').iterdir())
            self.assertEqual(left, sorted(names[1:] + ['local-20260901.sql']))
            self.assertEqual(sorted(x.name for x in cache.iterdir()), sorted([keep, 'notes.txt']))

    def test_prune_downloads_removes_only_old_staging(self):
        with folder() as p:
            old, fresh, other = p / ('a' * 32), p / ('b' * 32), p / 'keep-me'
            for d in (old, fresh, other):
                d.mkdir()
            past = time.time() - 7200
            os.utime(old, (past, past))
            os.utime(other, (past, past))
            u.prune_downloads(p)
            self.assertEqual(sorted(x.name for x in p.iterdir()), sorted([fresh.name, other.name]))

    def test_remote_prune_targets_only_stamped_folders(self):
        commands = []
        u.prune_remote(commands.append)
        self.assertEqual(len(commands), 2)
        self.assertIn('/opt/laqia/update-backups', commands[0])
        self.assertIn('head -n -3', commands[0])
        self.assertIn('head -n -0', commands[1])
        for c in commands:
            self.assertIn(u.REMOTE_STAMP_GLOB, c)

    def test_revision_accepts_plain_sha_and_legacy_json(self):
        for body in ('f' * 40 + '\n', json.dumps({'sha': 'f' * 40})):
            with folder() as p:
                def fake(url, target, expected=None, length=None, accept=None):
                    target.parent.mkdir(parents=True, exist_ok=True)
                    if url.endswith('/commits/main'):
                        self.assertEqual(accept, 'application/vnd.github.sha')
                        target.write_text(body, encoding='utf-8')
                    else:
                        raise RuntimeError('stop after revision')
                with mock.patch.object(u, 'download', side_effect=fake) as d, self.assertRaisesRegex(RuntimeError, 'stop'):
                    u.fetch(p, p / 'work/laqia-runtime')
                self.assertIn('/' + 'f' * 40 + '/', d.call_args.args[0])
                # A failed fetch leaves no staging copy behind.
                self.assertEqual(list((p / 'update/downloads').iterdir()), [])

    def test_rate_limit_is_explained(self):
        error = u.urllib.error.HTTPError('https://api.github.com', 403, 'rate limited', {'X-RateLimit-Remaining': '0'}, None)
        with folder() as p, mock.patch.object(u.urllib.request, 'urlopen', side_effect=error):
            with self.assertRaisesRegex(RuntimeError, '횟수 제한'):
                u.download('https://api.github.com/x', p / 'x')

    def test_offline_is_explained(self):
        with folder() as p, mock.patch.object(u.urllib.request, 'urlopen', side_effect=u.urllib.error.URLError('offline')):
            with self.assertRaisesRegex(RuntimeError, '인터넷'):
                u.download('https://api.github.com/x', p / 'x')

    def test_main_removes_its_staging_after_success_and_failure(self):
        for outcome in (None, RuntimeError('apply failed')):
            with folder() as p:
                runtime = p / 'work/laqia-runtime'
                (runtime / 'python').mkdir(parents=True)
                (runtime / 'python/python.exe').write_bytes(b'x')
                (runtime / 'local_control.py').write_bytes(b'x')
                (p / 'outputs').mkdir()
                staging = p / 'update/downloads' / ('c' * 32)
                staging.mkdir(parents=True)
                with mock.patch.object(u, 'fetch', return_value=staging), mock.patch.object(u, 'run_latest_updater', return_value=None):
                    with mock.patch.object(u, 'apply', side_effect=outcome), mock.patch.object(sys, 'argv', ['x', '--root', str(p)]):
                        if outcome:
                            with self.assertRaises(RuntimeError):
                                u.main()
                        else:
                            self.assertEqual(u.main(), 0)
                self.assertFalse(staging.exists())

    def test_old_updaters_accept_the_published_manifest(self):
        # A father's installed updater validates the new manifest before handing off to the new one.
        import subprocess, zipfile
        released = subprocess.run(['git', 'show', 'b446ef5:distribution/father_update.py'], cwd=ROOT,
                                  capture_output=True, check=True).stdout
        with zipfile.ZipFile(ROOT / 'distribution/father-updater-repair.zip') as z:
            repair = z.read('라키아/update/father_update.py')
        for n, source in enumerate((released, repair)):
            with folder() as p:
                (p / 'old_update.py').write_bytes(source)
                spec = importlib.util.spec_from_file_location('old_updater_%d' % n, p / 'old_update.py')
                legacy = importlib.util.module_from_spec(spec)
                spec.loader.exec_module(legacy)
                manifest = legacy.read_manifest(ROOT)
                legacy.validate_payloads(ROOT, manifest)


if __name__ == '__main__':
    unittest.main()
