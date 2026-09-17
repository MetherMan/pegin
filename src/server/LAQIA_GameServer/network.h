/*
 *	Filename : 			network.h
 *
 *	Author:				sk8snow
 *
 *	Description :		네트웍처리 헤더	
 */

#ifndef __NETWORK_H__
#define __NETWORK_H__


/*
 *	Function
 */
void nonblock( SOCKET s );
void GetMyIP(char* ip);
unsigned long getaddrbydomain( char *addr );
SOCKET InitServerSock( int port, int backLog );
SOCKET InitUDPServerSock( int port );
void CloseMotherSock( SOCKET sock );
void AcceptNewClient( SOCKET MotherSock );
void SendData( sPDESC_DATA pClient , const char *data, int size );
void SendUDPData( SOCKET sock, const char *data, int size, struct sockaddr_in *addr );
int FlushSendBuff( sPDESC_DATA pClient );
void DisconnectClient( sPDESC_DATA pData );
SOCKET ConnectToServer( char *host, int port , BYTE useDNS );

#endif

//EOF


