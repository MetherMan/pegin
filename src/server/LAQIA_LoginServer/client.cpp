/* client.cpp client 관리*/

#include "def.h"
#include "clist.h"
#include "structs.h"
#include "extern.h"
#include "net_init.h"
#include "protocol.h"
#include "message.h"
#include "client.h"
#include "encrypt.h"

//
// Function Name: NewClient
// Date: 2002-02-17
// Description: 
//
pClient_Data NewClient()
{
	pClient_Data newClient;

	if( g_Clients.memory )
	{
		newClient = g_Clients.memory;
		
		REMOVE_FROM_LIST( g_Clients.memory, newClient , prev, next );

		g_Clients.total_memory--;
		
		return newClient;
	}
	else
	{
		newClient = (pClient_Data)malloc(sizeof(sClient_Data));

		if( !newClient )
		{
			Log( "NewClient malloc failed!\r\n" );
			return NULL;
		}
		
		return newClient;
	}
	return NULL;
}

//
// Function Name: AcceptNewClient
// Date: 2002-02-17
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
		Log( "Accept New Client Failed!\r\n" );
		return;		
	}
	
	nonblock( newDesc );

	pClient_Data newClient = NewClient();

	if( !newClient )
	{
		closesocket( newDesc );
		return;
	}
	
	InitClientData( newClient, newDesc, (char*)inet_ntoa( peer.sin_addr ) );
	
	// 링크드 리스트 연결
	INSERT_TO_LIST( g_Clients.list, newClient , prev, next );
	g_Clients.total++;

#if defined (WIN32)
	char title[128];
	sprintf( title, "Login Server - Total %d[%d] User", g_Clients.total, g_Clients.total_memory );
	SetWindowText( g_hWnd, title );
#endif
}

//
// Function Name: InitClientData
// Date: 2002-02-17
// Description: 플레이어 데이터 초기화 
//
void InitClientData( pClient_Data pClient, SOCKET sock, char *hostname )
{
	pClient->sock = sock;
	strcpy( pClient->ip, hostname );

	pClient->id[0] = '\0';

	pClient->lastAliveCheck = g_CurrentTime;
	pClient->timeLimit = 0;

	*pClient->recvBuff = '\0';
	pClient->recvSize = 0;	
	pClient->recvPos = 0;
	
	*pClient->sendBuff = '\0';		
	pClient->sendSize = 0;		
	
	pClient->IsAccepted = 0;
	pClient->disconnectFlag = 0;

	pClient->prev = NULL;
	pClient->next = NULL;

	pClient->id_prev = NULL;
	pClient->id_next = NULL;

	pClient->lastRecvTime = g_CurrentTime;
}

//
// Function Name: RecvFromClient
// Date: 2002-02-12
// Description: 클라이언트에서 전송한 데이터를 버퍼에 담아온다
//
BOOL RecvFromClient( pClient_Data pClient )
{
	int recvSize;
	char recvBuff[dMAX_SOCK_BUFF];

	recvSize = recv( pClient->sock, recvBuff, 1024, 0 );

	if( recvSize <= 0 )
	{
#if defined(WIN32)
		if( WSAGetLastError() != WSAEWOULDBLOCK )
			return 0;
#else
		if( errno != EWOULDBLOCK )
			return 0;
#endif
		else
			return 1;
	}

	if( ( pClient->recvSize + recvSize ) >= dMAX_SOCK_BUFF )
	{
		Log( "%s's recvBuff Overflow\r\n", pClient->ip );
		return 0;
	}
	
	memcpy( &pClient->recvBuff[pClient->recvSize], recvBuff, recvSize );
	pClient->recvSize += recvSize;

	pClient->lastRecvTime = g_CurrentTime;

	return 1;
}

//
// Function Name: ReadRecvBuff
// Date: 2002-02-12
// Description: recv buffer에 있는 데이터들을 처리함
//

BOOL ReadRecvBuff( pClient_Data pClient )
{
	WORD msgSize;
	WORD tag;

	while( pClient->recvSize )
	{
		pClient->recvPos = 0;

		msgSize = GetWord( pClient->recvBuff, pClient->recvPos );

		char	szDecData[dMAX_SOCK_BUFF];
		DWORD	key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4 };
		int		decSize;
		int		nReadPos = 0;
		
		decSize = TEA_DecryptData( &pClient->recvBuff[2], szDecData, msgSize - 2, key );

		if( szDecData[0] != dENC_FIRST_HEADER ||
			szDecData[1] != dENC_SECOND_HEADER )
				return 0;

		nReadPos = 2;

		WORD omsgSize = GetWord( szDecData, nReadPos );
		tag = GetWord( szDecData, nReadPos );

		if( tag > dMAX_TAG  )
			return 0;//(TODO) return 0 으로 바꾸기. test를 위한 코드
		
		if( tag == dFIRST_PACKET )
			pClient->IsAccepted = 1;
		
		if( !pClient->IsAccepted && tag != dFIRST_PACKET )
			return 0;

		if( pClient->recvSize < msgSize )
			return 1;

		if( !MessageFuncsList[tag]( pClient, szDecData, nReadPos ) )
			return 0;
		
		memmove( &pClient->recvBuff[0], &pClient->recvBuff[msgSize], pClient->recvSize - msgSize );
		pClient->recvSize -= msgSize;
	}
	return 1;
}


//
// Function Name: SendData
// Date: 2002-02-12
// Description: 플레이어의 send버퍼에 데이터를 넣는다
//
void SendData( pClient_Data pClient , const char *data , int size )
{	
	char g_EncPacket[dMAX_SEND_SOCKBUFF];
	char realData[dMAX_SEND_SOCKBUFF];

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
// Function Name: FlushSendBuff
// Date: 2002-02-12
// Description: send버퍼의 내용을 모두 보내기 
//
int FlushSendBuff( pClient_Data pClient )
{
	int sendSize;

	sendSize = send( pClient->sock, pClient->sendBuff, pClient->sendSize, 0 );

	if( sendSize > 0 )
	{
		if( sendSize == pClient->sendSize )
		{
			*pClient->sendBuff = '\0';
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
// Date: 2002-02-12
// Description: 클라이언트의 접속 해제 처리
//
void DisconnectClient( pClient_Data pClient )
{
	closesocket( pClient->sock );

	if( pClient->id[0] != '\0' )
	{
		int nHashIdx = GetStrHashIndex( pClient->id );
		REMOVE_FROM_LIST( g_Clients.id_list[ nHashIdx ], pClient, id_prev, id_next );
	}

	REMOVE_FROM_LIST( g_Clients.list, pClient , prev, next );
	INSERT_TO_LIST( g_Clients.memory, pClient , prev, next );
	g_Clients.total--;
	g_Clients.total_memory++;
	
#if defined (WIN32)
	char title[128];
	sprintf( title, "LoginServer - Total %d[%d] User", g_Clients.total, g_Clients.total_memory );
	SetWindowText( g_hWnd, title );
#endif
}

//
// Function Name: FindClientIdList
// Date: 2003-06-02
// Description: ID 를 이용 Client 를 찾는다. 존재하지 않을경우 NULL 리턴. 
//
pClient_Data FindClientIdList( char* id )
{
	int hashIdx;
	hashIdx = GetStrHashIndex( id );

	pClient_Data d, next_d;
	for( d = g_Clients.id_list[hashIdx]; d; d = next_d )
	{
		next_d = d->id_next;

		if( !strcmp( d->id, id ) )
		{
			return d;
		}
	}
	return NULL;
}

//
// Function Name: SaveUserData
// Date: 2003-06-11
// Description: 유저 정보 DB입력 
//
void SaveUserData( pClient_Data pClient )
{	
	return ;
}

