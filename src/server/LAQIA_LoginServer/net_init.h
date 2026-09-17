/* net_init.h */

#ifndef __NET_INIT__
#define __NET_INIT__

void	nonblock( SOCKET s );
SOCKET	InitListenServerSock( int port, int backLog );
SOCKET	InitConnectServerSock( char* TargetAddress, int port );
SOCKET InitUDPServerSock( int port );
#endif //__NET_INIT__
