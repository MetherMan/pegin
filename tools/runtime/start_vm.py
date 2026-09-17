from pathlib import Path
import subprocess, time, json, sys
# The desktop launcher starts this file with pythonw.exe, which has no stdout.
if sys.stdout is not None:
    sys.stdout.reconfigure(encoding='utf-8')
ROOT = Path(__file__).resolve().parent
qemu = ROOT/'qemu'
disk = ROOT/'laqia-vm.qcow2'
if not disk.exists() or not (ROOT/'debian-base.qcow2').exists():
    raise RuntimeError('Server disk is missing. Extract the complete game folder, including both qcow2 files. A new empty server was not created.')
for accel in ['whpx','tcg,thread=multi']:
    args=[str(qemu/'qemu-system-x86_64.exe'),'-L','qemu/share','-name','LAQIA Local Server','-machine','q35','-accel',accel,'-m','4096','-smp','2',
          '-drive','file=laqia-vm.qcow2,if=virtio,format=qcow2','-drive','file=seed.iso,if=virtio,format=raw,readonly=on',
          '-netdev','user,id=net0,hostfwd=tcp:127.0.0.1:22222-:22,hostfwd=tcp:127.0.0.1:8030-:8030,hostfwd=tcp:127.0.0.1:2560-:2560',
          '-device','virtio-net-pci,netdev=net0','-display','none','-serial','file:vm-console.log',
          '-monitor','tcp:127.0.0.1:44444,server=on,wait=off','-no-reboot']
    with (ROOT/'vm-stderr.log').open('wb') as err, (ROOT/'vm-stdout.log').open('wb') as out:
        p=subprocess.Popen(args,cwd=str(ROOT),stdout=out,stderr=err,creationflags=subprocess.CREATE_NO_WINDOW)
    time.sleep(3)
    if p.poll() is None:
        (ROOT/'vm-process.json').write_text(json.dumps({'pid':p.pid,'accel':accel,'args':args},indent=2))
        print(json.dumps({'pid':p.pid,'accel':accel,'status':'started'}))
        break
    print((ROOT/'vm-stderr.log').read_text(errors='replace'))
else:
    raise RuntimeError('Could not start QEMU')
