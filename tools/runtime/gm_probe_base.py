from pathlib import Path
from protocol_probe import socket,struct,send,receive,string
import time,select,json
class GMClient:
 def __init__(self,user):
  self.sock=socket.create_connection(('127.0.0.1',2560),timeout=5);self.sock.settimeout(5)
  send(self.sock,1,string(user)+string(user)+struct.pack('<i',2020092801));tag,data=receive(self.sock);assert tag==2 and data==b'\x02',(user,tag,data)
  send(self.sock,33);tag,data=receive(self.sock);assert tag==34
  send(self.sock,35,b'\x00');self.login_packets=[]
  for _ in range(100):
   tag,data=receive(self.sock)
   self.login_packets.append((tag,data))
   if tag==5:self.map_loading=data;send(self.sock,6);break
  else:raise AssertionError('No map loading packet')
  self.drain(1)
 def drain(self,seconds=.6):
  out=[];deadline=time.monotonic()+seconds
  while time.monotonic()<deadline:
   if not select.select([self.sock],[],[],min(.1,max(0,deadline-time.monotonic())))[0]:continue
   out.append(receive(self.sock))
  return out
 def command(self,cmd,seconds=.6):
  self.drain(.1);b=cmd.encode('cp949');send(self.sock,16,struct.pack('<H',len(b))+b);return self.drain(seconds)
 def close(self):self.sock.close()
if __name__=='__main__':
 import sys;sys.stdout.reconfigure(encoding='utf-8')
 c=GMClient('developer')
 try:
  for tag,data in c.command('/gm help'):
   if len(data)>=2:
    n=struct.unpack_from('<H',data)[0]
    if n<=len(data)-2:print(tag,data[2:2+n].decode('cp949',errors='replace'))
 finally:c.close()
