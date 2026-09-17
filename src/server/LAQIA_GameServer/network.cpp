/*
 *	Filename : 			network.cpp
 *
 *	Author:				sk8snow
 *
 *	Description :		네트웍 처리	
 */

#include "def.h"

#include "structs.h"
#include "externs.h"
#include "network.h"

#include "encrypt.h"

#include "player.h"
#include "player_world.h"

#include "combat.h"
#include "party.h"
#include "deal.h"
#include "quest.h"
#include "friends.h"

#include "data.h"
#include "log.h"

//
// Function Name: nonbolck
// Date: 2004/04/23
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
// Date: 2004/04/23
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
// Function Name: getaddrbydomain
// Date: 2004/04/23
// Description: 
//
unsigned long getaddrbydomain( char *addr )
{
	struct hostent *ph;
	struct in_addr in;

	ph = gethostbyname( addr );

	memcpy( (char**)&(in), ph->h_addr, ph->h_length );

	return (in.s_addr);
}


//
// Function Name: InitServerSock
// Date: 2004/04/23
// Description: 서버 소켓 초기화 
//
SOCKET InitServerSock( int port, int backLog )
{
	struct sockaddr_in sa;
	SOCKET sock;

	sock = socket( PF_INET, SOCK_STREAM, 0 );
	
	if( sock < 0 )
	{
		log( "InitServerSock(), socket(..) failed [PORT:%d].. \r\n" , port );
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
		log( "InitServerSock(), bind(..) failed.. [PORT:%d]\r\n" , port );
		closesocket( sock );
		return -1;
	}
		
	// socket nonblocking
	nonblock( sock );
	listen( sock, backLog );

	return sock;
}


//
// Function Name: InitUDPServerSock
// Date: 2004/04/23
// Description: 서버 소켓 초기화 
//
SOCKET InitUDPServerSock( int port )
{
	struct sockaddr_in sa;
	SOCKET sock;

	sock = socket( PF_INET, SOCK_DGRAM, 0 );
	
	if( sock < 0 )
	{
		log( "InitUDPServerSock(), socket(..) failed [PORT:%d].. \r\n" , port );
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
		log( "InitUDPServerSock(), bind(..) failed.. [PORT:%d]\r\n" , port );
		closesocket( sock );
		return -1;
	}
		
	// socket nonblocking
	nonblock( sock );

	return sock;
}

//
// Function Name: CloseMotherSock
// Date: 2004/04/23
// Description: 서버 소켓 종료 
//
void CloseMotherSock( SOCKET sock )
{
	closesocket( sock );
}


//
// Function Name: AcceptNewClient
// Date: 2004/04/23
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
	
	nonblock( newDesc );

	sPDESC_DATA newClient = NULL;

	GET_MEMORY( gPLAYERS.memory, newClient, sDESC_DATA, prev, next );

	if( !newClient )
	{
		log( "AcceptNewClient : malloc failed\r\n" );
		closesocket( newDesc );
		return;
	}
	
	InitDescData( newClient, newDesc, (char*)inet_ntoa(peer.sin_addr) );
	InitPlayerData( newClient );
		
	// 링크드 리스트 연결
	INSERT_TO_LIST( gPLAYERS.list, newClient , prev, next );
	gPLAYERS.total++;
//	gPLAYERS.total_memory--;

#if defined (WIN32)
	char title[128];
	sprintf( title, "Game Server - Total %d[%d] User", gPLAYERS.total, gPLAYERS.total_memory );
	SetWindowText( g_hwnd, title );
#endif
}

//
// Function Name: SendData
// Date: 2004/04/23
// Description: 플레이어의 send버퍼에 데이터를 넣는다
//
void SendData( sPDESC_DATA pClient , const char *data , int size )
{		
#ifdef _DUMMY_MODE_
	if( pClient->isDummy )
		return;
#endif
	
#ifdef DEBUG_SENDBUFF
	WORD packetSize = *(WORD*)data;
	WORD packetTag = *(WORD*)( data + sizeof( WORD ) );

	if( packetSize != size )
	{
		printf( "SendData : packetSize != size [%s/%s] size : %d/%d , tag : %d\r\n", GET_NAME( pClient ), GET_ID( pClient ), size, packetSize, packetTag );
		return;
	}
	printf( "DEBUG_SENDBUFF : [%s/%s] size : %d , tag : %d\r\n", GET_NAME( pClient ), GET_ID( pClient ), packetSize, packetTag ); 
#endif

	char g_EncPacket[dMAX_SEND_SOCKBUFF];
	char realData[dMAX_SEND_SOCKBUFF];

	memcpy( &realData[0], &data[0], size );
	
	DWORD key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4 };

	DWORD encDataSize = TEA_EncryptData( realData, g_EncPacket, size, key );

	WORD packetSize = ( 2 + (WORD)encDataSize );

	int loopCnt = 0;
	
	while( ( pClient->sendSize + packetSize ) >= dMAX_SEND_SOCKBUFF )
	{
		if( loopCnt >= 3 )
		{
			return;
		}
		
		if( FlushSendBuff( pClient ) < 0 )
			return;
		
		loopCnt++;
	}
	memcpy( &pClient->sendBuff[pClient->sendSize], (char*)&packetSize, sizeof( WORD ) );
	pClient->sendSize += sizeof(WORD);
	memcpy( &pClient->sendBuff[pClient->sendSize], g_EncPacket, encDataSize );
	pClient->sendSize += encDataSize;
}


//
// Function Name: SendUDPData
// Date: 2004/04/23
// Description: 
//
void SendUDPData( SOCKET sock, const char *data, int size, struct sockaddr_in *addr )
{
	sendto( sock, data, size, 0, (struct sockaddr*)addr, sizeof(struct sockaddr_in) ); 
}



//
// Function Name: FlushSendBuff
// Date: 2004/04/23
// Description: send버퍼의 내용을 모두 보내기 
//
int FlushSendBuff( sPDESC_DATA pClient )
{	
#ifdef _DUMMY_MODE_
	if( pClient->isDummy )
		return 1;
#endif

	int sendSize;

	sendSize = send( pClient->sock, pClient->sendBuff, pClient->sendSize, 0 );
	
	if( sendSize > 0 )
	{
		if( sendSize == pClient->sendSize )
		{
			*pClient->sendBuff = NULL_STR;
			pClient->sendSize = 0;
		}
		else
		{
			memmove( &pClient->sendBuff[0], &pClient->sendBuff[sendSize], pClient->sendSize - sendSize );
			pClient->sendSize -= sendSize;
		}
	}
	else // send Error !
	{
		// This should never happen! 
		if( sendSize == 0 )
		{
			pClient->disconnectFlag = 1;
			return -1;
		}
		
#ifdef WIN32
		if( WSAGetLastError() != WSAEWOULDBLOCK &&
			WSAGetLastError() != WSAEINTR )
		{
			pClient->disconnectFlag = 1;
			return -1;
		}
		else
			return 0;
#else
		if( errno != EWOULDBLOCK &&
			errno != EINTR )
		{
			pClient->disconnectFlag = 1;
			return -1;
		}
		else
			return 0;
#endif
	}
	return sendSize;
}
   


//
// Function Name: DisconnectClient
// Date: 2004/04/23
// Description: 클라이언트의 접속 해제 처리
//
void DisconnectClient( sPDESC_DATA pData )
{
	// 플레이 중이면 맵에서 빼기
	if( GET_STATE( pData ) == dDESC_STATE_PLAYING )
	{
		PlayerFromMap( pData );
	}

	if( pData->isDataLoaded )
	{
		LOG_LoginLog( pData, dLOGINLOG_LOGOUT );

		//
		if( GET_VEHICLE_STATE( pData ) )
		{
			GET_MAP_NUM( pData ) = GET_PRE_MAPNUM( pData );
			GET_POSX( pData ) = GET_PRE_POSX( pData );
			GET_POSY( pData ) = GET_PRE_POSY( pData );
			GET_POSZ( pData ) = 0;
		}

		RemoveAllAction( pData );
		ClearPlayerItems( pData );
		SavePlayerData( pData );
		ClearPlayerQuest( pData );
		ClearPlayerFriend( pData );

		// 
		DATA_SendLogout( pData );
	}

	if( GET_PARTY_MASTER( pData ) == pData )
		PARTY_Destroy( pData );
	else if( GET_PARTY_MASTER( pData ) )
	{
		sPDESC_DATA pMaster = GET_PARTY_MASTER( pData );
		PARTY_OutMember( pData );
		PARTY_CheckDeatroy( pMaster );
	}

	PARTY_ClearMember( pData );

	// 거래시 거래 종료
	if( pData->ch.pDealVict )
		DEAL_Cancel( pData );

	if( pData->ch.myshopOpen )
		MYSHOP_Close( pData );

	//////////////////////////////////////////////
	// REMOVE FROM NAME LIST
	//////////////////////////////////////////////
	int hashIdx;
	
	if( pData->isInNameList )
	{
		hashIdx = GetStrHashIndex( GET_NAME( pData ) );
		REMOVE_FROM_LIST( gPLAYERS.name_list[hashIdx], pData, name_prev, name_next );
	}

	if( pData->isInIdList )
	{
		hashIdx = GetStrHashIndex( GET_ID( pData ) );
		REMOVE_FROM_LIST( gPLAYERS.id_list[hashIdx], pData, id_prev, id_next );
	}

	closesocket( pData->sock );
	
	GET_STATE( pData ) = dDESC_STATE_DISCON;

	REMOVE_FROM_LIST( gPLAYERS.list, pData , prev, next );
	INSERT_TO_LIST( gPLAYERS.memory, pData , prev, next );

	gPLAYERS.total--;
	gPLAYERS.total_memory++;
#if defined (WIN32)
	char title[128];
	sprintf( title, "Game Server - Total %d[%d] User", gPLAYERS.total, gPLAYERS.total_memory );
	SetWindowText( g_hwnd, title );
#endif
}

//
// Function Name: ConnectToServer
// Date: 2004/04/23
// Description: 
//
SOCKET ConnectToServer( char *host, int port , BYTE useDNS )
{
	struct sockaddr_in s_addr_in;
	SOCKET sock;

	sock = socket( AF_INET, SOCK_STREAM, 0 );
	
	if( sock == -1 )
		return -1;

	s_addr_in.sin_family = AF_INET;  
	s_addr_in.sin_port = htons(port);  
	if( useDNS )
		s_addr_in.sin_addr.s_addr = getaddrbydomain( host );
	else
		s_addr_in.sin_addr.s_addr = inet_addr( host ); 
	
	if( connect( sock, (struct sockaddr*)&s_addr_in, sizeof(s_addr_in) ) != 0 )
		return -1;
	
	nonblock( sock );
	return sock;
}


// EOF




