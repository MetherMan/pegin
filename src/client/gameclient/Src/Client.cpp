/**
 * 네트웍 클라이언트 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-04-13 9:06p $
 * @version	$Revision: 47 $
 *
 * <HR>
 * $Log: /GameClient/Src/Client.cpp $
 * 
 * 47    04-04-13 9:06p Paran2
 * 
 * 46    04-03-19 7:16p Sk8snow2
 * 
 * 45    04-03-10 11:30p Sk8snow2
 * 
 * 44    04-03-10 4:40p Sk8snow2
 * 
 * 43    04-03-10 4:24p Sk8snow2
 * 
 * 42    04-03-10 4:20p Sk8snow2
 * 
 * 41    04-03-10 4:16p Sk8snow2
 * 
 * 40    04-03-10 12:12p Sk8snow2
 * 
 * 39    04-01-07 3:31p Kjmgo
 * 
 * 38    03-11-17 6:22p Kjmgo
 * 주석 제거( 주석으로된 구문을 삭제 )
 * 
 * 37    03-10-06 7:19p Kjmgo
 * Send Fail. Bug. 
 * 
 * 36    03-06-27 5:30p Kjmgo
 * Assert처리
 * 
 * 35    03-05-24 2:58a Ahastudio
 * <HR>
 *
 * @file	Client.cpp
 */


#pragma comment( lib, "WSOCK32.LIB" )


#include "Global.h"
#include "Client.h"
#include "packet.h"
#include "protocol.h"


/**
 * 생성자.
 */
CClient::CClient()
:	m_bConnect	( FALSE ),
	m_hSocket	( INVALID_SOCKET )
{
	WSADATA		wsaData;

	if( WSAStartup( 0x0101, &wsaData ) )
	{
		throw "윈속 초기화 실패";
	} //if
} //CClient::CClient

/**
 * 소멸자.
 */
CClient::~CClient()
{
	Disconnect();
} //CClient::~CClient



/**
 * 서버에 접속한다.
 *
 * @param	szHost		[IN] 접속할 호스트 이름
 * @param	nPort		[IN] 접속할 포트 번호
 *
 * @return	접속 성공 여부
 */
BOOL CClient::Connect( const char* szHost, int nPort )
{
	m_hSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( INVALID_SOCKET == m_hSocket )
	{
		DEBUG_OUT( "소켓 생성 실패" );
		return FALSE;
	} //if

	SOCKADDR_IN		serverAddr;
	ZeroMemory( &serverAddr, sizeof( serverAddr ) );
	serverAddr.sin_family	= AF_INET;
	serverAddr.sin_port		= htons( nPort );

	DWORD		dwAddr = inet_addr( szHost );
	if( dwAddr == INADDR_NONE )
	{
		HOSTENT*	pHostent = gethostbyname( szHost );		// Get Host By Name.
		if( !pHostent )
		{
			DEBUG_OUT( "Get Host By Name 실패" );
			return FALSE;
		} //if

		memcpy( &serverAddr.sin_addr, pHostent->h_addr, pHostent->h_length );
	}
	else
	{
		serverAddr.sin_addr.s_addr = inet_addr( szHost );	
	} //if..else

	if( connect( m_hSocket, (sockaddr*)&serverAddr, sizeof( serverAddr ) ) < 0 )
	{
		DEBUG_OUT( "서버 접속 실패" );
		return FALSE;
	} //if

	NonBlocking();
	SetBuffer( 65536 );

	m_bConnect	= TRUE;
	m_nRecvSize	= 0;
	m_nSendSize	= 0;

	return TRUE;
} //CClient::Connect

/**
 * Non-blocking 모드로 만든다.
 */
void CClient::NonBlocking()
{
	u_long		ulOn = 1L;
	ioctlsocket( m_hSocket, FIONBIO, (unsigned long*)&ulOn );
} //CClient::NonBlocking

/**
 * Send/Receive Buffer 크기를 변경한다.
 *
 * @param	iSize	[IN] 버퍼 크기
 */
void CClient::SetBuffer( int iSize )
{
	setsockopt( m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&iSize, sizeof( iSize ) );
	setsockopt( m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&iSize, sizeof( iSize ) );
} //CClient::SetBuffer

/**
 * 서버와의 연결을 끊는다.
 */
void CClient::Disconnect()
{
	m_bConnect = FALSE;

	if( m_hSocket != INVALID_SOCKET )
	{
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	} //if
} //CClient::Disconnect

/**
 * 연결된 상태인지 돌려준다.
 *
 * @return	연결된 상태라면 TRUE
 */
BOOL CClient::IsConnect()
{
	return m_bConnect;
} //CClient::IsConnect

/**
 * 데이터를 서버로 보낸다.
 * 데이터 전송이 실패할 경우, 서버와의 연결을 끊는다.
 *
 * @param	pData	[IN] 보낼 데이터
 * @param	nSize	[IN] 보낼 데이터 길이
 */
void CClient::Send( const void* pData, int nSize )
{
	Assert( pData );

	if( !pData )
	{
		return ; 
	} //if

	if( m_nSendSize > 0 )
	{
		memcpy( m_SendBuffer + m_nSendSize, (char*)pData, nSize );
		m_nSendSize += nSize;

		DEBUG_OUT( "Send Fail : " + m_nSendSize );
		return ;
	} //if


	if( !IsConnect() )		return;

	//여기서 패킷 압축 하기
	char	g_EncPacket[dMAX_SEND_SOCKBUFF];
	char	EncPacket[dMAX_SEND_SOCKBUFF];
	int		nEncPacket = 0;
	char	realData[dMAX_SEND_SOCKBUFF];

	memset( &realData, 0, sizeof(realData) );
	memset( &g_EncPacket, 0, sizeof(g_EncPacket) );

	realData[0] = 'A';
	realData[1] = 'L';

	memcpy( &realData[2], pData, nSize );

	DWORD key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4	};
	DWORD encDataSize = TEA_EncryptData( realData, g_EncPacket, nSize + 2, key );

	WORD  packetSize = ( 2 + (WORD)encDataSize );

	memcpy( &EncPacket[nEncPacket], &packetSize, sizeof(WORD) );
	nEncPacket += sizeof(WORD);

	memcpy( &EncPacket[nEncPacket], g_EncPacket, encDataSize );
	nEncPacket += encDataSize;
	nSize = nEncPacket;

	//int nSendSize = send( m_hSocket, (char*)pData, nSize, 0 );
	int nSendSize = send( m_hSocket, EncPacket, nEncPacket, 0 );
	
	  //원본
	//int nSendSize = send( m_hSocket, (char*)pData, nSize, 0 );//
	char	szTemp[1024];
	sprintf( szTemp, "Send( EncPacket, nEncPacket = %d )", nEncPacket );
	DEBUG_OUT( szTemp );
	
	//if( nSendSize != nSize )
	if( nSendSize != nEncPacket )
	{
		if( nSendSize >= 0 )
		{
			memcpy( m_SendBuffer + m_nSendSize, (char*)pData + nSendSize,  nSize - nSendSize );
			m_nSendSize += ( nSize - nSendSize );
		}
		else
		{
			memcpy( m_SendBuffer + m_nSendSize, (char*)pData, nSize );
			m_nSendSize += nSize;
		} //if..else..
		DEBUG_OUT( "Send 실패" );
		Disconnect();
	} //if


/*

	int nSendSize = send( m_hSocket, (char*)pData, nSize, 0 );
	DEBUG_OUT( "Send( const void* pData, int nSize )" );

	if( nSendSize != nSize )
	{
		if( nSendSize >= 0 )
		{
			memcpy( m_SendBuffer + m_nSendSize, (char*)pData + nSendSize,  nSize - nSendSize );
			m_nSendSize += ( nSize - nSendSize );
		}
		else
		{
			memcpy( m_SendBuffer + m_nSendSize, (char*)pData, nSize );
			m_nSendSize += nSize;
		} //if..else..
		DEBUG_OUT( "Send 실패" );
		Disconnect();
	} //if*/

} //CClient::Send

/**
 *
 *
 *
 */
void CClient::Send()
{
	if( m_nSendSize <= 0 )
	{
		m_nSendSize = 0;
		return ;
	} //if

	int nSendSize = send( m_hSocket, m_SendBuffer, m_nSendSize, 0 );
	DEBUG_OUT( "Send(  )" );

	if( nSendSize <= 0 )
	{
		return;
	} //if..else..

	m_nSendSize -= nSendSize;

	if( nSendSize != m_nSendSize )
	{
		memcpy( m_SendBuffer, m_SendBuffer + nSendSize, m_nSendSize );
	} //if

} //CClient::Send
 /**
 * 서버에서 데이터를 받는다.
 * Receive 에러가 발생할 경우, 서버와의 연결을 끊는다.
 * TRUE를 돌려준다고 해도 데이터를 받았음을 보장하지는 않는다.
 * 받은 데이터는 반드시 GetRecvSize() 함수를 이용해서 확인 후, 처리를 해야 한다.
 * @see		GetRecvSize
 *
 * @return	TRUE - Receive 성공 or 받은 데이터 없음 / FALSE - Receive 에러
 */
BOOL CClient::Recv()
{
	if( !IsConnect() )		return FALSE;

	int nRecvSize = BUFFER_SIZE - m_nRecvSize;

	if( nRecvSize <= 0 )
	{
		Log( "Receive 에러 : 받을수 있는 범위를 넘어 섰습니다." );
		DEBUG_OUT( "Receive 에러 : 받을수 있는 범위를 넘어 섰습니다." );
		return TRUE;
	} //if

	int		iRecv = recv( m_hSocket, m_RecvBuff + m_nRecvSize, BUFFER_SIZE - m_nRecvSize, 0 );

	if( iRecv <= 0 )
	{	// recv가 이뤄지지 않은 경우 : 에러 or Non-blocking
		if( WSAGetLastError() != WSAEWOULDBLOCK )
		{	// Receive 에러.
			DEBUG_OUT( "Receive 에러 - " + WSAGetLastError() );
			Disconnect();
			return FALSE;
		}
		else
		{	// Non-blocking (에러 아님)
			return TRUE;
		} //if..else..
	} //if

	m_nRecvSize += iRecv;

	return TRUE;
} //CClient::Recv

/**
 * 받은 데이터 버퍼에 쌓인 데이터 사이즈를 얻는다.
 */
int CClient::GetRecvSize()
{
	return m_nRecvSize;
} //CClient::GetRecvSize

/**
 * 받은 데이터를 얻는다.
 */
const char* CClient::GetRecvData()
{
	return m_RecvBuff;
} //CClient::GetRecvData

/**
 *
 */
int CClient::GetSendSize()
{
	return m_nSendSize;
} //CClient::GetSendSize


/**
 * 받은 데이터 버퍼에서 데이터를 제거한다.
 * 패킷 등을 처리한 후, 이 함수를 이용해 처리한 데이터를 제거한다.
 *
 * @param	nSize		[IN] 제거할 데이터 크기.
 */
void CClient::RemoveRecvData( int nSize )
{
	Assert( nSize <= m_nRecvSize  );

	memcpy( m_RecvBuff, m_RecvBuff + nSize, m_nRecvSize - nSize );
	m_nRecvSize -= nSize;
} //CClient::RemoveRecvData

