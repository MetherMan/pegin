"""Cold-boot retries must wait for one VM, never create a second server."""
from pathlib import Path
import contextlib,importlib.util,io,sys,types,unittest
from unittest.mock import Mock,MagicMock,patch
R=Path(__file__).resolve().parents[1]

class Clock:
    def __init__(self):self.now=0
    def monotonic(self):return self.now
    def sleep(self,seconds):self.now+=seconds

class StartupTests(unittest.TestCase):
    def setUp(self):
        spec=importlib.util.spec_from_file_location('startup_test_control',R/'tools/runtime/local_control.py')
        self.ctl=importlib.util.module_from_spec(spec)
        ssh=types.ModuleType('ssh_vm');ssh.connect=MagicMock()
        portable=types.ModuleType('portable_support')
        for name in ('client_running','client_drive','release_drive','preflight'):setattr(portable,name,Mock())
        with patch.dict(sys.modules,ssh_vm=ssh,portable_support=portable),patch('logging.basicConfig'):spec.loader.exec_module(self.ctl)
    def test_already_booting_server_is_waited_for_without_launching_second_vm(self):
        c=self.ctl;clock=Clock()
        with patch.object(c,'online',return_value=False) as online,patch.object(c,'port_open',return_value=True),patch.object(c.subprocess,'run') as launch,patch.object(c,'time',clock),patch.object(c,'command',return_value='active'),patch.object(c.socket,'create_connection',return_value=MagicMock()):
            with patch.object(c,'vm_status',side_effect=[(False,'booting'),(False,'booting'),(True,'')]) as probe,contextlib.redirect_stdout(io.StringIO()):
                c.start()
            launch.assert_not_called()
            online.assert_called_once_with()
            self.assertEqual(probe.call_count,3)
            self.assertEqual(clock.now,4)
    def test_slow_boot_has_a_ten_minute_budget(self):
        c=self.ctl
        clock=Clock()
        with patch.object(c,'time',clock),patch.object(c,'vm_status',side_effect=lambda:(clock.now>=580,'still booting')):
            with patch.object(c,'boot_diagnostic') as diagnostic,contextlib.redirect_stdout(io.StringIO()):
                c.wait_for_vm()
            diagnostic.assert_not_called()
        self.assertEqual(clock.now,580)
        clock=Clock()
        with patch.object(c,'time',clock),patch.object(c,'vm_status',return_value=(False,'SSH handshake timed out')):
            with patch.object(c,'boot_diagnostic',side_effect=lambda reason:reason) as diagnostic,contextlib.redirect_stdout(io.StringIO()):
                with self.assertRaisesRegex(RuntimeError,'SSH handshake timed out'):c.wait_for_vm()
            diagnostic.assert_called_once_with('SSH handshake timed out')
        self.assertEqual(clock.now,600)
    def test_ssh_failure_closes_connection_and_bounds_all_handshake_timeouts(self):
        spec=importlib.util.spec_from_file_location('ssh_test',R/'tools/runtime/ssh_vm.py')
        module=importlib.util.module_from_spec(spec);paramiko=MagicMock();client=paramiko.SSHClient.return_value
        client.connect.side_effect=TimeoutError('cold boot')
        with patch.dict(sys.modules,paramiko=paramiko):spec.loader.exec_module(module)
        with self.assertRaises(TimeoutError):module.connect(timeout=2)
        client.close.assert_called_once()
        for key in ('timeout','banner_timeout','auth_timeout'):self.assertEqual(client.connect.call_args.kwargs[key],2)

if __name__=='__main__':unittest.main()
