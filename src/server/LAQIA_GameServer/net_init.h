/**
 *	$Workfile: net_init.h $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 7 $
 *	$Date: 04-07-13 7:26a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/net_init.h $
 * 
 * 7     04-07-13 7:26a Sk8snow
 * 
 * 6     04-04-06 10:26a Sk8snow
 * 
 * 5     04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __NET_INIT_H__
#define __NET_INIT_H__


void nonblock( SOCKET s );
void GetMyIP(char* ip);
SOCKET InitServerSock( int port, int backLog );
void CloseMotherSock( SOCKET sock );
void AcceptNewClient( SOCKET MotherSock );
void DisconnectClient( sPDESC_DATA pData );
SOCKET InitUDPServerSock( int port );
SOCKET ConnectToServer( char *host, int port , BYTE useDNS = 0 );
unsigned long getaddrbydomain( char *addr );

int FlushSendBuff( sPDESC_DATA pClient );


#endif


//EOF



