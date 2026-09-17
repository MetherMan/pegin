/*
	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	net_init.cpp

	Build log...
*/
#include "def.h"

#include "structs.h"

#include "net_init.h"
#include "extern.h"

char g_AllowIP[3][50];

//
// Function Name: nonbolck
// Date:
// Description: for set nonblocking mode socket
//
#if defined ( WIN32 )
void nonblock( SOCKET s )
{
	u_long            ulOn = 1L;
	
	ioctlsocket(s, FIONBIO, (unsigned long*)&ulOn);
}
#else
void nonblock( SOCKET s )
{
	int flags;
	
	flags = fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	if (fcntl(s, F_SETFL, flags) < 0) 
		log( "* nonblock Error\r\n" );
}
#endif


//
// Function Name: GetMyIP
// Date: 2002-02-12
// Description: Get Local Machines IP
//
void GetMyIP(char* ip)
{
	char buf[100];
	gethostname (buf, 100);
	struct hostent *p;
	p=gethostbyname(buf);
	struct in_addr *ptr;
	char **listptr=p->h_addr_list;

	switch(p->h_addrtype)
	{
	case PF_INET:
		while((ptr=(struct in_addr *) *listptr++) != NULL)
			strcpy(ip, inet_ntoa(*ptr));
		break;
	}
}


//
// Function Name: InitServerSock
// Date: 2002-02-12
// Description: 서버 소켓 초기화 
//
SOCKET InitServerSock( int port, int backLog )
{
#if defined (WIN32)
	WSADATA WSAData;
	WSAStartup( 0x0101, &WSAData );
#endif

	struct sockaddr_in sa;
	SOCKET sock;

	sock = socket( PF_INET, SOCK_STREAM, 0 );
	
	if( sock < 0 )
	{
		log( "InitServerSock(), socket(..) failed.. \r\n" );
		return -1;
	}
	
#ifndef WIN32
	int opt = 1;

	if( setsockopt( sock , SOL_SOCKET , SO_REUSEADDR , (char *) &opt , sizeof(opt) ) < 0 )
	{
		log( "* Error: sersockopt REUSEADDR\r\n" );
		exit(1);
	}	
#endif

	struct linger ld;
	
	ld.l_onoff = 0;
	ld.l_linger = 0;

	if ( setsockopt( sock, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof( ld ) ) < 0)
		log( "* Error: sersockopt SO_LINGER...\r\n" );
	
	/* Clear the structure */
	memset( (char *)&sa, 0, sizeof(sa) );
	
	sa.sin_family = PF_INET;
	sa.sin_port	= htons( port );
	sa.sin_addr.s_addr = INADDR_ANY;
	
	// 주소지 연결
	if( bind( sock, (struct sockaddr *)&sa, sizeof( sa ) ) < 0 )
	{
		log( "InitServerSock(), bind(..) failed.. \r\n" );
		closesocket( sock );
		return -1;
	}
		
	// socket nonblocking
	nonblock( sock );
	listen( sock, backLog );

	return sock;
}

//
// Function Name: CloseMotherSock
// Date: 2002-02-12
// Description: 서버 소켓 종료 
//
void CloseMotherSock( SOCKET sock )
{
	closesocket( sock );
#if defined (WIN32)
	WSACleanup();
#endif
}


//
// Function Name: AcceptNewClient
// Date: 2002-02-12
// Description: 접속 처리 함수 
//
void AcceptNewClient( SOCKET MotherSock )
{
	SOCKET newDesc;
	struct sockaddr_in peer;
#if defined( WIN32 )
	int peersize;
#else
	socklen_t peersize;
#endif
	
	peersize = sizeof( peer );
	
	newDesc = accept( MotherSock, (struct sockaddr *) &peer, &peersize );
	
	if( newDesc < 0 )
	{
		log( "Accept New Client Failed!\r\n" );
		return;		
	}
	
	if( !IsAllowIP( (char*)inet_ntoa( peer.sin_addr ) ) )
	{
		closesocket( newDesc );
		log( "!IsAllowIp : %s\r\n", (char*)inet_ntoa( peer.sin_addr ) );
		return;
	}

	nonblock( newDesc );

	sPDESC_DATA newClient = (sPDESC_DATA)malloc(sizeof(sDESC_DATA));

	if( !newClient )
	{
		closesocket( newDesc );
		return;
	}

	g_ServerList.push( newClient );

	newClient->sock = newDesc;
	
	strcpy( newClient->ip, (char*)inet_ntoa( peer.sin_addr ) );

	*newClient->recvBuff = '\0';		// recv 데이터 버퍼 
	newClient->recvSize = 0;			// recv 버퍼 상의 총 데이터 량
	newClient->recvPos = 0;						

#ifdef dUSE_SEND_BUFF
	*newClient->sendBuff = '\0';		// send 대기중인 데이터 
	newClient->sendSize = 0;			// send 할 데이터량
#endif

	for( int i = 0; i < dHASHKEY_STR_MAX; i++ )
		newClient->DataList[i].Init();

#if defined (WIN32)
	char title[128];
	sprintf( title, "Data Server - Total %d[%d]", g_ServerList.GetSize(), g_ServerList.GetMemSize() );
	SetWindowText( g_hwnd, title );
#endif
}

//
// Function Name: SendData
// Date: 2002-02-12
// Description: 플레이어의 send버퍼에 데이터를 넣는다
//
void SendData( sPDESC_DATA pClient , const char *data , int size )
{		
#ifdef dUSE_SEND_BUFF
	while( ( pClient->sendSize + size ) >= dMAX_SOCK_BUFF )
	{
		log( "SendData : sendBuff OverFlow!!! Flushing!![%s]\r\n", pClient->ip );
		FlushSendBuff( pClient );
	}

	memcpy( &pClient->sendBuff[pClient->sendSize], data, size );
	pClient->sendSize += size;
#else
	send( pClient->sock, data, size, 0 );
#endif
}

//
// Function Name: FlushSendBuff
// Date: 2002-02-12
// Description: send버퍼의 내용을 모두 보내기 
//
#ifdef dUSE_SEND_BUFF
void FlushSendBuff( sPDESC_DATA pClient )
{
	int sendSize;

	sendSize = send( pClient->sock, pClient->sendBuff, pClient->sendSize, 0 );

	if( sendSize < 0 )
	{
		log( "FlushSendBuff : sendSize < 0 [%s]\r\n", pClient->ip );
		return;
	}

	if( sendSize < pClient->sendSize )
	{
		memmove( &pClient->sendBuff[0], &pClient->sendBuff[sendSize], pClient->sendSize - sendSize );
		pClient->sendSize -= sendSize;
	}
	else
	{
		*pClient->sendBuff = '\0';
		pClient->sendSize = 0;
	}
}
#endif


//
// Function Name: DisconnectClient
// Date: 2002-02-12
// Description: 클라이언트의 접속 해제 처리
//
void DisconnectClient( sPDESC_DATA pData )
{
	// 해당 게임서버를 통해 접속한 플레이어 데이터들 
	// 전부 저장
	int i;
	for( i = 0; i < dHASHKEY_STR_MAX; i++ )
	{
		if( pData->DataList[i].GetSize() )
		{
			sPLIST_BLOCK block;
			sPCHAR_DATA charInfo;
			
			for( block = pData->DataList[i].GetFirstBlock(); block; block = block->next )
			{
				charInfo = (sPCHAR_DATA)block->data;
				
				UpdateCharData( charInfo );
			}
		}
	}
	for( i = 0; i < dHASHKEY_STR_MAX; i++ )
		pData->DataList[i].Destroy();
	
	g_ServerList.remove( pData );
	
	closesocket( pData->sock );

	free( pData );
	pData = NULL;

#if defined (WIN32)
	char title[128];
	sprintf( title, "Data Server - Total %d[%d] ", g_ServerList.GetSize(), g_ServerList.GetMemSize() );
	SetWindowText( g_hwnd, title );
#endif
}

BOOL LoadAllowIp()
{
	FILE *fp = fopen ( "ALLOW_IP.INI", "rb" );

	if( !fp )
		return 0;

	fscanf( fp, "%s", g_AllowIP[0] );
	fscanf( fp, "%s", g_AllowIP[1] );
	fscanf( fp, "%s", g_AllowIP[2] );

	fclose(fp);

	return 1;
}

BOOL IsAllowIP( char *ip )
{
	if( strcmp( ip, g_AllowIP[0] ) &&
		strcmp( ip, g_AllowIP[1] ) &&
		strcmp( ip, g_AllowIP[2] ) )
		return 0;
	return 1;
}


//EOF

