from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT/'pylibs'))
import paramiko

def connect():
    c=paramiko.SSHClient()
    known=ROOT/'vm-known-hosts'
    if known.exists(): c.load_host_keys(str(known))
    c.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    c.connect('127.0.0.1',port=22222,username='root',key_filename=str(ROOT/'vm-ssh-key'),timeout=8,allow_agent=False,look_for_keys=False)
    c.save_host_keys(str(known))
    return c

if __name__=='__main__':
    with connect() as c:
        if sys.argv[1]=='put':
            with c.open_sftp() as s: s.put(sys.argv[2],sys.argv[3])
            print('Uploaded',Path(sys.argv[2]).name)
        elif sys.argv[1]=='file':
            inp,out,err=c.exec_command('bash -s',timeout=55)
            inp.write(Path(sys.argv[2]).read_text(encoding='utf-8'))
            inp.channel.shutdown_write()
            sys.stdout.buffer.write(out.read())
            sys.stderr.buffer.write(err.read())
            sys.exit(out.channel.recv_exit_status())
        else:
            _,out,err=c.exec_command(sys.argv[1],timeout=55)
            sys.stdout.buffer.write(out.read())
            sys.stderr.buffer.write(err.read())
            sys.exit(out.channel.recv_exit_status())
