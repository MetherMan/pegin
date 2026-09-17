/**
 *	$Workfile: net_init.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 21 $
 *	$Date: 04-09-08 3:54p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/net_init.cpp $
 * 
 * 21    04-09-08 3:54p Sk8snow
 * 
 * 20    04-07-13 7:26a Sk8snow
 * 
 * 19    04-04-30 9:46a Sk8snow
 * 
 * 18    04-04-07 10:17p Sk8snow
 * 
 * 17    04-04-06 10:26a Sk8snow
 * 
 * 16    04-04-03 4:03p Sk8snow
 * 
 * 15    04-03-31 8:22a Sk8snow
 *        
 */

#include "def.h"

#include "structs.h"
#include "extern.h"
#include "player.h"
#include "world.h"

#include "dataserver.h"
#include "item.h"

#include "party.h"
#include "guild.h"

#include "net_init.h"
#include "billing.h"
#include "skill.h"

#include "pet.h"

#include "friends.h"
#include "encrypt.h"

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
// Date: 2003-02-12
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
// Date: 
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
// Date: 2003-02-12
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
// Date: 2003-02-12
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
// Date: 2003-02-12
// Description: 서버 소켓 종료 
//
void CloseMotherSock( SOCKET sock )
{
	closesocket( sock );
}


//
// Function Name: AcceptNewClient
// Date: 2003-02-12
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

	sPDESC_DATA newClient = NewClient();

	if( !newClient )
	{
		closesocket( newDesc );
		return;
	}
	
	InitPlayerData( newClient, newDesc, (char*)inet_ntoa( peer.sin_addr ) );
	
	// 링크드 리스트 연결
	INSERT_TO_LIST( gPLAYERS.list, newClient , prev, next );
	gPLAYERS.total++;

#if defined (WIN32)
	char title[128];
	sprintf( title, "Game Server - Total %d[%d] User", gPLAYERS.total, gPLAYERS.total_memory );
	SetWindowText( g_hwnd, title );
#endif
}

//
// Function Name: SendData
// Date: 2003-02-12
// Description: 플레이어의 send버퍼에 데이터를 넣는다
//
void SendData( sPDESC_DATA pClient , char *data , int size )
{		
#ifdef _DUMMY_MODE_
	if( pClient->isDummy )
		return;
#endif

/*#ifdef DEBUG_SENDBUFF
	WORD packetSize = *(WORD*)data;
	WORD packetTag = *(WORD*)( data + sizeof( WORD ) );

	if( packetSize != size )
	{
		printf( "SendData : packetSize != size [%s/%s] size : %d , tag : %d\r\n", GET_NAME( pClient ), GET_ID( pClient ), packetSize, packetTag );
		return;
	}
	printf( "DEBUG_SENDBUFF : [%s/%s] size : %d , tag : %d\r\n", GET_NAME( pClient ), GET_ID( pClient ), packetSize, packetTag ); 
#endif

//#ifdef _DDKQCNRQKDTLR_*/
	char g_EncPacket[dMAX_SEND_SOCKBUFF];//[40960];//[dMAX_SEND_SOCKBUFF];
	char realData[dMAX_SEND_SOCKBUFF];//[40960];//[dMAX_SEND_SOCKBUFF];

	//realData 및 g_EncPacket 초기화
	memset( &realData, 0, sizeof( realData ) );
	memset( &g_EncPacket, 0, sizeof( g_EncPacket ) );

	memcpy( &realData[0], &data[0], size );

	DWORD key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4 };
	DWORD encDataSize = TEA_EncryptData( realData, g_EncPacket, size, key );
	DWORD packetSize = ( 2 + (WORD)encDataSize );

	size = ( 2 + encDataSize );

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
// Date:
// Description: 
//
void SendUDPData( SOCKET sock, char *data, int size, struct sockaddr_in *addr )
{
	sendto( sock, data, size, 0, (struct sockaddr*)addr, sizeof(struct sockaddr_in) ); 
}



//
// Function Name: FlushSendBuff
// Date: 2003-02-12
// Description: send버퍼의 내용을 모두 보내기 
//
int FlushSendBuff( sPDESC_DATA pClient )
{	
#ifdef _DUMMY_MODE_
	if( pClient->isDummy )
		return 1;
#endif

	int sendSize;

	if( pClient->sendSize > 0 )
	{
		sendSize = send( pClient->sock, pClient->sendBuff, pClient->sendSize, 0 );
	}
	else
	{
		sendSize = 0;
	}
	
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
// Date: 2003-02-12
// Description: 클라이언트의 접속 해제 처리
//
void DisconnectClient( sPDESC_DATA pData )
{
	if( pData->isWaitAllData )
		gPLAYERS.WaitAllData.remove( pData );
	if( pData->isWaitCharData )
		gPLAYERS.WaitCharData.remove( pData );
	if( pData->isWaitDelData )
		gPLAYERS.WaitDelData.remove( pData );

	if( GET_PARTY_MASTER( pData ) == pData )
		PARTY_Destroy( pData );
	else if( GET_PARTY_MASTER( pData ) )
		PARTY_OutMember( pData );

	PARTY_ClearMember( pData );

	// 플레이 중이면 맵에서 빼기
	if( GET_STATE( pData ) == dDESC_STATE_PLAYING ||
		GET_STATE( pData ) == dDESC_STATE_FIGHT )
	{
		if( GET_MAP_KIND( pData ) == dMAP_KIND_INDOOR )
			PlayerFromIndoor( pData );
		else
			PlayerFromMap( pData );
	}

	//////////////////////////////////////////////
	// REMOVE FROM NAME LIST
	//////////////////////////////////////////////
	int hashIdx;
	
	if( pData->isInNameList )
	{
		hashIdx = GetStrHashIndex( pData->ch.name );
		REMOVE_FROM_LIST( gPLAYERS.name_list[hashIdx], pData, name_prev, name_next );
	}

	if( pData->isInIdList )
	{
		hashIdx = GetStrHashIndex( pData->ch.id );
		REMOVE_FROM_LIST( gPLAYERS.id_list[hashIdx], pData, id_prev, id_next );
	}

	// GUILD
	if( GET_GUILD( pData ) )
	{
		if( !IS_VALID_GUILD( GET_GUILD( pData ) ) )
		{
			log( "DisconnectClient : !IS_VALID_GUILD( GET_GUILD( pData ) ) [%s/%s]\r\n",
				GET_ID( pData ), GET_NAME( pData ) );
		}
		else
		{
				REMOVE_FROM_LIST( gGUILDS.array[GET_GUILD( pData )]->memberList,
					pData, guild_prev, guild_next );
		}
	}

	if( GET_TMP_GUILD( pData ) )
	{
		if( !IS_VALID_GUILD( GET_TMP_GUILD( pData ) ) )
		{
			log( "DisconnectClient : !IS_VALID_GUILD( GET_TMP_GUILD( pData ) ) [%s/%s]\r\n",
				GET_ID( pData ), GET_NAME( pData ) );
		}
		else
		{
			REMOVE_FROM_LIST( gGUILDS.array[GET_TMP_GUILD( pData )]->mercenaryList,
				pData, merc_prev, merc_next );
		}
	}

	// LogOut 데이터 서버로 보내기 
	// TODO : 데이터 처리의 맨 마지막에 존재해야 함
	if( pData->chDataLoaded )
	{	
		DATASERV_SendUpdateChEquip( pData );
		DATASERV_SendUpdateHp( pData );
		DATASERV_SendUpdateMp( pData );

		// 근거리 
		if( pData->isUpdate[dDATA_UPDATE_SKILLEXP1] )
			DATASERV_SendUpdateSkillExp( pData, 0 );

		// 원거리 
		if( pData->isUpdate[dDATA_UPDATE_SKILLEXP2] )
			DATASERV_SendUpdateSkillExp( pData, 1 );
		
		// 흑마법
		if( pData->isUpdate[dDATA_UPDATE_SKILLEXP3] )
			DATASERV_SendUpdateSkillExp( pData, 2 );
		
		// 백마법 
		if( pData->isUpdate[dDATA_UPDATE_SKILLEXP4] )
			DATASERV_SendUpdateSkillExp( pData, 3 );
		 
		// 힘 (STR)
		if( pData->isUpdate[dDATA_UPDATE_STREXP] )
			DATASERV_SendUpdateStrExp( pData );
		
		// 민첩 (DEX)
		if( pData->isUpdate[dDATA_UPDATE_DEXEXP] )
			DATASERV_SendUpdateDexExp( pData );
		
		// 지력 (INT)
		if( pData->isUpdate[dDATA_UPDATE_INTEXP] )
			DATASERV_SendUpdateIntExp( pData );

		if( pData->isUpdate[dDATA_UPDATE_CHA] )
			DATASERV_SendUpdateCha( pData );

		/////////////////////////////////////////
		// Item Data Clear
		/////////////////////////////////////////
		ClearPlayerItem( pData );

#ifdef dDAUM_BILLING
		SaveUserBillEffect( pData );
#endif

#ifndef dNOT_USE_PET
		// Update Pet Info
		if( GET_PET_TYPE( pData ) )
			PET_SaveUserPet( pData );
#endif

		/////////////////////////////////////////
		// SKILL DATA Clear
		/////////////////////////////////////////
		ClearAllUserSKill( pData );

		// Friend Data Clear
		FRIEND_ClearFriendList( pData );

		g_nPos = 2;
		PutWord( g_Packet, dDATA_PACKET_CHAR_LOGOUT, g_nPos );
		PutString( g_Packet, pData->ch.id, g_nPos );
		PutWord( g_Packet, GET_USE_SKILL( pData ), g_nPos );		// 최후 사용 기술 
		PutSize( g_Packet, g_nPos );
		
		SendToDataServer( g_Packet, g_nPos );
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
// Date: 
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




