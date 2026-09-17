/**
 * 네트웍 클라이언트 모듈 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-03-19 7:16p $
 * @version	$Revision: 28 $
 *
 * <HR>
 * $Log: /GameClient/Src/Client.h $
 * 
 * 28    04-03-19 7:16p Sk8snow2
 * 
 * 27    04-03-10 12:12p Sk8snow2
 * 
 * 26    03-10-06 7:19p Kjmgo
 * Send Fail. Bug. 
 * 
 * 25    03-05-24 2:58a Ahastudio
 * <HR>
 *
 * @file	Client.h
 */


#ifndef __Client_Header__
#define __Client_Header__

#include <winsock.h>


/**
 * 네트웍 클라이언트 클래스.
 * @ingroup		Game
 */
class CClient
{
public:
	UNDER_MEMORY_MGR( CClient );

	CClient();
	~CClient();

	BOOL	Connect( const char *szHost, int nPort );
	void	Disconnect();

	BOOL	IsConnect();

	void	Send( const void* pData, int nSize );
	BOOL	Recv();

	int				GetRecvSize();
	int				GetSendSize();

	void	Send();
	const char*		GetRecvData();
	void			RemoveRecvData( int nSize );
	
private:
	void	NonBlocking();
	void	SetBuffer( int iSize );

private:
	enum	{ BUFFER_SIZE = 1024 * 10 };	///< 버퍼 크기.

	BOOL		m_bConnect;					///< 연결상태
	SOCKET		m_hSocket;					///< 소켓

	char		m_RecvBuff[BUFFER_SIZE];	///< 받은 데이터
	int			m_nRecvSize;				///< 받은 데이터 크기

	char		m_SendBuffer[BUFFER_SIZE];	///< 보낼 데이터
	int			m_nSendSize;				///< 보낼 데이터 크기
}; //class CClient


#endif // #ifndef __Client_Header__
