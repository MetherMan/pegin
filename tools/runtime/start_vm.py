"""Start the existing VM, with QEMU's native WHPX-to-TCG fallback."""
from pathlib import Path
import subprocess, time, json, sys
ROOT = Path(__file__).resolve().parent


def accelerator_options(root):
    try: previous=(root/'vm-stderr.log').read_text(encoding='utf-8',errors='replace')
    except OSError: previous=''
    if 'whpx: no accelerator found' in previous.lower() or 'failed to initialize whpx' in previous.lower():
        (root/'vm-whpx-error.log').write_text(previous,encoding='utf-8')
        return ['-accel','tcg,thread=multi']
    # QEMU itself tries TCG if WHPX initialization fails, regardless of timing.
    return ['-accel','whpx','-accel','tcg,thread=multi']


def launch(root=ROOT):
    if not (root/'laqia-vm.qcow2').is_file() or not (root/'debian-base.qcow2').is_file():
        raise RuntimeError('Server disk is missing. Extract the complete game folder. A new empty server was not created.')
    accelerators=accelerator_options(root)
    if accelerators==['-accel','tcg,thread=multi']:
        print('WHPX 초기화 실패 기록 확인: 소프트웨어 방식(TCG)으로 기존 서버를 시작합니다.',flush=True)
    args=[str(root/'qemu/qemu-system-x86_64.exe'),'-L','qemu/share','-name','LAQIA Local Server',
          '-machine','q35',*accelerators,'-m','4096','-smp','2',
          '-drive','file=laqia-vm.qcow2,if=virtio,format=qcow2',
          '-drive','file=seed.iso,if=virtio,format=raw,readonly=on',
          '-netdev','user,id=net0,hostfwd=tcp:127.0.0.1:22222-:22,hostfwd=tcp:127.0.0.1:8030-:8030,hostfwd=tcp:127.0.0.1:2560-:2560',
          '-device','virtio-net-pci,netdev=net0','-display','none','-serial','file:vm-console.log',
          '-monitor','tcp:127.0.0.1:44444,server=on,wait=off','-no-reboot']
    with (root/'vm-stderr.log').open('wb') as err,(root/'vm-stdout.log').open('wb') as out:
        process=subprocess.Popen(args,cwd=str(root),stdout=out,stderr=err,creationflags=subprocess.CREATE_NO_WINDOW)
    time.sleep(3)
    if process.poll() is not None:
        raise RuntimeError('QEMU 시작 실패:\n'+(root/'vm-stderr.log').read_text(encoding='utf-8',errors='replace')[-4000:])
    mode='tcg' if len(accelerators)==2 else 'whpx:tcg'
    (root/'vm-process.json').write_text(json.dumps({'pid':process.pid,'accel':mode,'args':args},indent=2),encoding='utf-8')
    print(json.dumps({'pid':process.pid,'accel':mode,'status':'started'}),flush=True)


if __name__=='__main__':
    if sys.stdout is not None:sys.stdout.reconfigure(encoding='utf-8')
    launch()
