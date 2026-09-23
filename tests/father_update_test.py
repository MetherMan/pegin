from pathlib import Path
from unittest.mock import patch
import importlib.util,json,unittest,uuid,contextlib,shutil,hashlib,types,sys
from unittest import mock
ROOT=Path(__file__).resolve().parents[1]
TEMPROOT=ROOT/'.cache/updater-tests'
TEMPROOT.mkdir(parents=True,exist_ok=True)
@contextlib.contextmanager
def test_directory():
    p=(TEMPROOT/uuid.uuid4().hex).resolve();p.mkdir()
    try:yield str(p)
    finally:
        assert p.is_relative_to(TEMPROOT.resolve())
        shutil.rmtree(p)
spec=importlib.util.spec_from_file_location('updater',ROOT/'distribution/father_update.py')
u=importlib.util.module_from_spec(spec);spec.loader.exec_module(u)

class UpdaterTests(unittest.TestCase):
    def test_confined_paths(self):
        with test_directory() as d:
            for name in ['../save.db','C:/save.db','/save.db','DATA/../../key','a\\b']:
                with self.assertRaises(ValueError):u.safe_path(d,name)
    def test_rollback_restores_existing_and_removes_new(self):
        with test_directory() as d:
            p=Path(d);old=p/'client/Engine.dll';old.parent.mkdir();old.write_bytes(b'old')
            new=p/'new';new.write_bytes(b'updated')
            untouched=p/'character.db';untouched.write_bytes(b'personal-save')
            tx=u.LocalTransaction(p/'backup')
            tx.put(new,old);tx.put(new,p/'client/new-model.mod')
            tx.rollback()
            self.assertEqual(old.read_bytes(),b'old')
            self.assertFalse((p/'client/new-model.mod').exists())
            self.assertEqual(untouched.read_bytes(),b'personal-save')
    def test_manifest_matches_every_payload_and_excludes_personal_files(self):
        m=u.read_manifest(ROOT)
        self.assertTrue(m['files'])
        for e in u.payload_entries(m):
            p=ROOT/e['source']
            self.assertEqual(u.sha(p),e['sha256'])
            self.assertEqual(p.stat().st_size,e['bytes'])
            self.assertNotIn(e['path'].lower(),['config.ini','server.ini'])
            self.assertFalse(any(s in e['path'].lower() for s in ['.qcow2','.sql','vm-ssh','setup-private']))
    def test_manifest_rejects_database_target(self):
        with test_directory() as d:
            p=Path(d);(p/'distribution').mkdir()
            m={'format':1,'repository':u.REPOSITORY,'files':[{'kind':'server','path':'database.sql','source':'game-data/database.sql','sha256':'0'*64,'bytes':1}]}
            (p/u.MANIFEST).write_text(json.dumps(m))
            with self.assertRaises(ValueError):u.read_manifest(p)
    def test_manifest_excludes_local_administrator_config(self):
        self.assertFalse(any(e['path']=='DATA/ADMIN_INFO.txt' for e in u.read_manifest(ROOT)['files']))
    def test_download_failures_do_not_write_installed_files(self):
        # Downloads and validation happen entirely in staging, before apply().
        with test_directory() as d:
            p=Path(d);live=p/'Engine.dll';live.write_bytes(b'live')
            bad=p/'broken';bad.write_bytes(b'wrong')
            with self.assertRaises(RuntimeError):u.download(bad.as_uri(),p/'staging/file','0'*64,5)
            self.assertEqual(live.read_bytes(),b'live')
    def test_runtime_repairs_are_verified_before_install_and_confined(self):
        with test_directory() as d:
            p=Path(d);source=p/'release';runtime=p/'existing/work/laqia-runtime'
            helper=source/'tools/runtime/local_control.py';helper.parent.mkdir(parents=True);helper.write_bytes(b'new startup')
            runtime.mkdir(parents=True);(runtime/'local_control.py').write_bytes(b'old startup')
            protected={'laqia-vm.qcow2':b'father disk','vm-ssh-key':b'father key','family-local.json':b'father settings'}
            for name,data in protected.items():(runtime/name).write_bytes(data)
            entry=dict(kind='runtime',path='local_control.py',source='tools/runtime/local_control.py',sha256=u.sha(helper),bytes=helper.stat().st_size)
            m=dict(format=1,repository=u.REPOSITORY,files=[],runtime_files=[entry]);(source/'distribution').mkdir();(source/u.MANIFEST).write_text(json.dumps(m))
            loaded=u.read_manifest(source);u.validate_payloads(source,loaded)
            u.install_runtime_support(runtime,source,loaded,p/'backup')
            self.assertEqual((runtime/'local_control.py').read_bytes(),helper.read_bytes())
            for name,data in protected.items():self.assertEqual((runtime/name).read_bytes(),data)
            helper.write_bytes(b'tampered')
            with self.assertRaises(RuntimeError):u.validate_payloads(source,loaded)
            for forbidden in ['laqia-vm.qcow2','vm-ssh-key','family-local.json','../start_vm.py']:
                m['runtime_files'][0]['path']=forbidden;(source/u.MANIFEST).write_text(json.dumps(m))
                with self.assertRaises(ValueError):u.read_manifest(source)
    def test_repair_zip_handoff_preserves_installation_and_failure_status(self):
        with test_directory() as d:
            p=Path(d);(p/'distribution').mkdir();script=p/'distribution/father_update.py';script.write_bytes(b'new verified updater')
            e=dict(kind='updater',path='update/father_update.py',source='distribution/father_update.py',sha256=u.sha(script),bytes=script.stat().st_size)
            (p/u.MANIFEST).write_text(json.dumps(dict(format=1,repository=u.REPOSITORY,files=[e])))
            root=p/'father installation';runtime=root/'work/laqia-runtime'
            with patch.object(u.subprocess,'run') as run:
                run.return_value.returncode=7
                self.assertEqual(u.run_latest_updater(root,runtime,p),7)
                args=run.call_args.args[0]
                self.assertEqual(args[args.index('--root')+1],str(root))
                self.assertEqual(args[args.index('--runtime')+1],str(runtime))
    def test_payload_entries_include_bootstrap_repairs(self):
        manifest=u.read_manifest(ROOT)
        self.assertEqual({e['path'] for e in manifest['runtime_files']},u.RUNTIME_SUPPORT)
        u.validate_payloads(ROOT,manifest)

    def fixture(self,p,runtime_name='local_control.py'):
        (p/'distribution').mkdir()
        entries=[]
        for kind,name,source,data in [('client','Engine.dll','client-overlay/Engine.dll',b'new-game'),
                                     ('updater','update/father_update.py','distribution/father_update.py',b'print("new updater")\n')]:
            f=p/source;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(data)
            entries.append(dict(kind=kind,path=name,source=source,bytes=len(data),sha256=hashlib.sha256(data).hexdigest()))
        data=b'# repaired startup helper\n';source='tools/runtime/'+runtime_name
        f=p/source;f.parent.mkdir(parents=True,exist_ok=True);f.write_bytes(data)
        r=dict(kind='runtime',path=runtime_name,source=source,bytes=len(data),sha256=hashlib.sha256(data).hexdigest())
        m=dict(format=1,repository=u.REPOSITORY,files=entries,runtime_files=[r])
        (p/u.MANIFEST).write_text(json.dumps(m),encoding='utf-8')
        return m

    def test_runtime_allowlist_excludes_disk_key_and_settings(self):
        for name in ['laqia-vm.qcow2','vm-ssh-key','server.ini','../local_control.py']:
            with test_directory() as d:
                p=Path(d);self.fixture(p,name)
                with self.assertRaises(ValueError):u.read_manifest(p)

    def test_new_updater_executes_before_vm_and_propagates_failure(self):
        with test_directory() as d:
            p=Path(d);self.fixture(p)
            with mock.patch.object(u.subprocess,'run',return_value=types.SimpleNamespace(returncode=7)) as run:
                self.assertEqual(u.run_latest_updater(p,p/'runtime',p),7)
                args=run.call_args.args[0]
                self.assertIn(str(p/'distribution/father_update.py'),args)
                self.assertEqual(args[-2:],['--source',str(p)])

    def test_corrupt_helper_blocks_bootstrap_and_handoff(self):
        with test_directory() as d:
            p=Path(d);self.fixture(p);(p/'tools/runtime/local_control.py').write_bytes(b'corrupt')
            with mock.patch.object(u.subprocess,'run') as run:
                with self.assertRaises(RuntimeError):u.run_latest_updater(p,p/'runtime',p)
                run.assert_not_called()

    def test_boot_failure_preserves_game_but_keeps_bootstrap_fix(self):
        with test_directory() as d:
            p=Path(d);self.fixture(p);runtime=p/'runtime';runtime.mkdir()
            helper=runtime/'local_control.py';helper.write_bytes(b'old helper')
            game=runtime/'client/GameClient/Engine.dll';game.parent.mkdir(parents=True);game.write_bytes(b'old-game')
            def failed_start():
                self.assertEqual(helper.read_bytes(),b'# repaired startup helper\n')
                raise RuntimeError('test VM unavailable')
            ctl=types.SimpleNamespace(online=lambda:False,start=failed_start)
            connect=mock.Mock()
            modules={'family_network':types.SimpleNamespace(SessionLock=contextlib.nullcontext),
                     'portable_support':types.SimpleNamespace(client_running=lambda:False),
                     'local_control':ctl,'ssh_vm':types.SimpleNamespace(connect=connect)}
            with mock.patch.dict(sys.modules,modules),mock.patch.object(sys,'path',sys.path.copy()):
                with self.assertRaisesRegex(RuntimeError,'test VM unavailable'):u.apply(p,runtime,p)
            self.assertEqual(game.read_bytes(),b'old-game')
            self.assertFalse((p/'update/installed.json').exists())
            connect.assert_not_called()

if __name__=='__main__':unittest.main()
