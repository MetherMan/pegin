import socket, struct, json
from pathlib import Path
MASK=0xffffffff
KEY=(0x780412,0x760626,0x790310,0x791212)
def crypt(data, decrypt=False):
    data+=b'\0'*(-len(data)%8)
    out=b''
    a,b,c,d=KEY
    for i in range(0,len(data),8):
        y,z=struct.unpack('<II',data[i:i+8]); s=(0x9e3779b9*8)&MASK if decrypt else 0
        for _ in range(8):
            if decrypt:
                z=(z-(((y<<4)+c ^ y+s ^ (y>>5)+d)&MASK))&MASK
                y=(y-(((z<<4)+a ^ z+s ^ (z>>5)+b)&MASK))&MASK
                s=(s-0x9e3779b9)&MASK
            else:
                s=(s+0x9e3779b9)&MASK
                y=(y+(((z<<4)+a ^ z+s ^ (z>>5)+b)&MASK))&MASK
                z=(z+(((y<<4)+c ^ y+s ^ (y>>5)+d)&MASK))&MASK
        out+=struct.pack('<II',y,z)
    return out
def string(s):
    s=s.encode(); return struct.pack('<H',len(s))+s
def send(s,tag,payload=b''):
    raw=struct.pack('<HH',len(payload)+4,tag)+payload
    enc=crypt(b'AL'+raw)
    s.sendall(struct.pack('<H',len(enc)+2)+enc)
def exact(s,n):
    b=b''
    while len(b)<n:
        v=s.recv(n-len(b))
        if not v: raise EOFError('Server closed connection')
        b+=v
    return b
def receive(s):
    n=struct.unpack('<H',exact(s,2))[0]
    raw=crypt(exact(s,n-2),True)
    size,tag=struct.unpack('<HH',raw[:4])
    return tag,raw[4:size]
if __name__=='__main__':
    import getpass
    password=getpass.getpass('Local developer game password: ')
    results=[]
    for label,test_password in [('invalid','incorrect-test-password'),('valid',password)]:
        try:
            with socket.create_connection(('127.0.0.1',8030),timeout=5) as s:
                send(s,1,string('developer')+string(test_password))
                tag,payload=receive(s)
                results.append({'case':label,'tag':tag,'result':payload[0] if payload else None})
        except Exception as e: results.append({'error':str(e)})
    print(json.dumps(results,indent=2))
    Path(__file__).with_name('login-probe-result.json').write_text(json.dumps(results,indent=2))
