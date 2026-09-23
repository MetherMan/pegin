"""Exercise slow/retried VM startup without launching or killing a real VM."""
from pathlib import Path
import contextlib,importlib.util,io,sys,types,unittest
import tempfile
from unittest import mock

ROOT=Path(__file__).resolve().parents[1]
fake_modules={'ssh_vm':types.SimpleNamespace(connect=mock.Mock()),
              'portable_support':types.SimpleNamespace(client_running=mock.Mock(),client_drive=mock.Mock(),
                                                       release_drive=mock.Mock(),preflight=mock.Mock())}
spec=importlib.util.spec_from_file_location('vm_control_test',ROOT/'tools/runtime/local_control.py')
ctl=importlib.util.module_from_spec(spec)
with mock.patch.dict(sys.modules,fake_modules),mock.patch('logging.basicConfig'):
    spec.loader.exec_module(ctl)
spec=importlib.util.spec_from_file_location('vm_launcher_test',ROOT/'tools/runtime/start_vm.py')
launcher=importlib.util.module_from_spec(spec);spec.loader.exec_module(launcher)

class Clock:
    def __init__(self):self.now=0
    def monotonic(self):return self.now
    def sleep(self,seconds):self.now+=seconds

class StartupTests(unittest.TestCase):
    def test_reported_whpx_error_selects_tcg_without_hardware_attempt(self):
        with tempfile.TemporaryDirectory(dir=ROOT/'.cache') as d:
            root=Path(d)
            error='-accel whpx: WHPX: No accelerator found, hr=00000000\n-accel whpx: failed to initialize whpx: No space left on device'
            (root/'vm-stderr.log').write_text(error,encoding='utf-8')
            self.assertEqual(launcher.accelerator_options(root),['-accel','tcg,thread=multi'])
            self.assertEqual((root/'vm-whpx-error.log').read_text(encoding='utf-8'),error)

    def test_default_launch_includes_native_accelerator_fallback(self):
        with tempfile.TemporaryDirectory(dir=ROOT/'.cache') as d:
            self.assertEqual(launcher.accelerator_options(Path(d)),['-accel','whpx','-accel','tcg,thread=multi'])

    def test_missing_existing_disk_never_creates_or_launches_vm(self):
        with tempfile.TemporaryDirectory(dir=ROOT/'.cache') as d,mock.patch.object(launcher.subprocess,'Popen') as launch:
            with self.assertRaisesRegex(RuntimeError,'disk is missing'):launcher.launch(Path(d))
            launch.assert_not_called()

    def test_slow_boot_after_old_timeout_succeeds(self):
        clock=Clock()
        with mock.patch.object(ctl,'time',clock),mock.patch.object(ctl,'vm_status',side_effect=lambda:(clock.now>=150,'booting')):
            with contextlib.redirect_stdout(io.StringIO()):ctl.wait_for_vm()
        self.assertEqual(clock.now,150)

    def test_timeout_reports_actual_probe_error(self):
        clock=Clock()
        with mock.patch.object(ctl,'time',clock),mock.patch.object(ctl,'vm_status',return_value=(False,'SSH authentication failed')):
            with mock.patch.object(ctl,'boot_diagnostic',side_effect=lambda reason:reason) as diagnostic:
                with contextlib.redirect_stdout(io.StringIO()),self.assertRaisesRegex(RuntimeError,'authentication failed'):
                    ctl.wait_for_vm(timeout=10)
                diagnostic.assert_called_once_with('SSH authentication failed')
        self.assertEqual(clock.now,10)

    def test_existing_booting_vm_is_waited_for_without_second_process(self):
        connection=mock.MagicMock()
        with mock.patch.object(ctl,'online',return_value=False),mock.patch.object(ctl,'preflight'):
            with mock.patch.object(ctl,'port_open',return_value=True),mock.patch.object(ctl,'wait_for_vm') as wait:
                with mock.patch.object(ctl.subprocess,'run') as launch,mock.patch.object(ctl,'connect',return_value=connection):
                    with mock.patch.object(ctl,'command',return_value='active'),mock.patch.object(ctl.socket,'create_connection',return_value=mock.MagicMock()):
                        with contextlib.redirect_stdout(io.StringIO()):ctl.start()
                    wait.assert_called_once_with();launch.assert_not_called()

    def test_probe_retains_connection_failure_reason(self):
        with mock.patch.object(ctl,'connect',side_effect=OSError('connection refused')):
            self.assertEqual(ctl.vm_status(),(False,'OSError: connection refused'))

    def test_probe_rejects_unprepared_server(self):
        with mock.patch.object(ctl,'connect',return_value=mock.MagicMock()),mock.patch.object(ctl,'command',side_effect=RuntimeError('marker missing')):
            self.assertEqual(ctl.vm_status(),(False,'RuntimeError: marker missing'))

if __name__=='__main__':unittest.main()
