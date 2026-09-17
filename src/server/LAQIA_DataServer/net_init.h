#ifndef __NET_INIT_H__
#define __NET_INIT_H__

extern char g_AllowIP[3][50];

void nonblock( SOCKET s );
void GetMyIP(char* ip);
SOCKET InitServerSock( int port, int backLog );
void CloseMotherSock( SOCKET sock );
void AcceptNewClient( SOCKET MotherSock );
void DisconnectClient( sPDESC_DATA pData );

#ifdef dUSE_SEND_BUFF
void FlushSendBuff( sPDESC_DATA pClient );
#endif

BOOL LoadAllowIp();
BOOL IsAllowIP( char *ip );

#endif



