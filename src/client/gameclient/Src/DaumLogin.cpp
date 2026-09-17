/**
 * 다음 인증처리 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	전홍
 * @date	$Date: 03-10-10 1:20p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /GameClient/Src/DaumLogin.cpp $
 * 
 * 4     03-10-10 1:20p Kjmgo
 * Login 에 관련하여, 몇가지 수정및, 로그가 남도록 수정 
 * 
 * 3     03-06-24 5:48p Paran
 * 
 * 2     03-06-24 11:16a Paran
 * 
 * <HR>
 *
 * @file	DaumLogin.cpp
 */

#include "Global.h"
#include "DaumLogin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define dDAUM_LOGIN_PORT		80

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDaumLogin::CDaumLogin()
:	m_sAccountHost( g_sDaumAccountHost )
{

}

CDaumLogin::~CDaumLogin()
{

}

/**
* 도메인으로 IP 정보 얻기
**/
unsigned long getaddrbydomain( char *addr )
{
	struct hostent *ph;
	struct in_addr in;

	ph = gethostbyname( addr );
	
	memcpy( (char**)&(in), ph->h_addr, ph->h_length );
	
	return (in.s_addr);
} //getaddrbydomain

/**
* 접속 함수
**/
BOOL CDaumLogin::Connect()
{
	struct sockaddr_in s_addr_in;
	
	m_Sock = socket( AF_INET, SOCK_STREAM, 0 );
	
	if( m_Sock == -1 )
	{
		Assert( !"소켓 생성 실패" );
		return 0;
	} 
	
	s_addr_in.sin_family = AF_INET;  
	s_addr_in.sin_port = htons( dDAUM_LOGIN_PORT );    
	s_addr_in.sin_addr.s_addr = getaddrbydomain( const_cast<char*>( m_sAccountHost.c_str() ) ); 
	
	int nResult = connect( m_Sock, (struct sockaddr*)&s_addr_in, sizeof(s_addr_in) );

	if( nResult < 0 )
	{
		Assert( !"Connect 실패" );
		return 0;
	} //if
	
	u_long		ulOn = 1L;
	ioctlsocket( m_Sock, FIONBIO, (unsigned long*)&ulOn );	

	return TRUE;
} //CDaumLogin::Connect

/**
* 소켓 종료 
**/
void CDaumLogin::Close()
{
	if( m_Sock != INVALID_SOCKET )
	{
		closesocket( m_Sock );
		m_Sock = INVALID_SOCKET;
	} //if
} //CDaumLogin::Close

/**
* 클래스 초기화 
**/
void CDaumLogin::Init()
{
	*m_recvBuff = NULL;
	*m_resultString = NULL;
	m_recvSize = 0;
	m_resultStringLen = 0;

	*m_HMCU_Buffer = NULL;
	*m_TS_Buffer = NULL;
	*m_HTS_Buffer = NULL;
	*m_PROF_Buffer = NULL;
} //CDaumLogin::Init

/**
* 다음 로그인 서버에 로그인 정보 전송
**/
void CDaumLogin::SendLoginInfo( const char *id, const char *pw )
{
	char httpMsg[1024];

//POST /Mail-bin/login.cgi HTTP/1.1
//Host: login.daum.net
//Accept: */*
//Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)
//Connection: Close
//Content-Length: 13 

//id=abc&pw=abc

	sprintf( httpMsg, 
		"POST /Mail-bin/login.cgi HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Accept: */*\r\n"
		"Agent: LAQIA_ONLINE\r\n"
		"Connection: Close\r\n"
		"Content-Length:%d\r\n"
		"\r\n"
		"id=%s&pw=%s",
		m_sAccountHost.c_str(),
		7+strlen(id)+strlen(pw), 
		id, pw );

	int nSize = strlen( httpMsg );
	int nSend = 0;

	int nCount = 0;
	
	while( nSize != 0 )
	{
		nCount++;
		nSend = send( m_Sock, httpMsg, nSize, 0 );
	
		if( nSend <= 0 )
		{
			Sleep( 100 );
			continue;
		} //if

		nSize -= nSend;
		
		if( nSize <= 0 || nCount >= 300 )
		{
			break;
		} //if
	} //while

} //CDaumLogin::SendLoginInfo

/**
* 데이터 recv 
**/
BOOL CDaumLogin::RecvData()
{
	m_recvSize = recv( m_Sock, m_recvBuff, 4096, 0 );

	if( m_recvSize <= 0 )
	{
		if( WSAGetLastError() != WSAEWOULDBLOCK )
		{
			GetAuthBuffer();
			return TRUE;
		}
		return FALSE;
	}

	memcpy( m_resultString+m_resultStringLen, m_recvBuff, m_recvSize );
	m_resultStringLen += m_recvSize;
	
	return FALSE;
} //CDaumLogin::RecvData


/**
* 결과값 처리
**/
// 200 OK
// 404 Auth Error 존재하지 않는 아이디 
// 601 Auth Error 존재하지 않는 아이디
// 403 Auth Error 다음 서비스 사용 중지
// 602 Auth Error 다음 서비스 사용 중지
// 203 Auth Error 아이디 , 비번이 일치하지 않을경우
// 412 Auth Error 부모의 동의가 필요한 사무실인데 아직 동의하지 않은경우
// 415 Auth Error 법인 사용자인데 법인 확인이 되지 않은경우
// 423 Auth Error 서버 작업중인 아이디 
// 500 System Error 서버 내부 에러
int CDaumLogin::GetAuthResult()
{
	char *pointStr = strstr( m_resultString, "X-DaumLogin-Error:" );

	if( !pointStr )
		return 0;

	// 200 OK
	if( !strncmp( pointStr, "X-DaumLogin-Error: 200 OK", strlen( "X-DaumLogin-Error: 200 OK" ) ) )
		return 200;

	// 404 Auth Error 존재하지 않는 아이디 
	if( !strncmp( pointStr, "X-DaumLogin-Error: 404 Auth Error", strlen( "X-DaumLogin-Error: 404 Auth Error" ) ) )
		return 404;

	// 601 Auth Error 존재하지 않는 아이디
	if( !strncmp( pointStr, "X-DaumLogin-Error: 601 Auth Error", strlen( "X-DaumLogin-Error: 601 Auth Error" ) ) )
		return 601;

	// 403 Auth Error 다음 서비스 사용 중지
	if( !strncmp( pointStr, "X-DaumLogin-Error: 403 Auth Error", strlen( "X-DaumLogin-Error: 403 Auth Error" ) ) )
		return 403;

	// 602 Auth Error 다음 서비스 사용 중지
	if( !strncmp( pointStr, "X-DaumLogin-Error: 602 Auth Error", strlen( "X-DaumLogin-Error: 602 Auth Error" ) ) )
		return 602;

	// 400 Auth Error 아이디 또는 패스워드를 입력하지 않은경우
	if( !strncmp( pointStr, "X-DaumLogin-Error: 400 Auth Error", strlen( "X-DaumLogin-Error: 400 Auth Error" ) ) )
		return 400;

	// 203 Auth Error 아이디 , 비번이 일치하지 않을경우
	if( !strncmp( pointStr, "X-DaumLogin-Error: 203 Auth Error", strlen( "X-DaumLogin-Error: 203 Auth Error" ) ) )
		return 203;

	// 412 Auth Error 부모의 동의가 필요한 사무실인데 아직 동의하지 않은경우
	if( !strncmp( pointStr, "X-DaumLogin-Error: 412 Auth Error", strlen( "X-DaumLogin-Error: 412 Auth Error" ) ) )
		return 412;

	// 415 Auth Error 법인 사용자인데 법인 확인이 되지 않은경우
	if( !strncmp( pointStr, "X-DaumLogin-Error: 415 Auth Error", strlen( "X-DaumLogin-Error: 415 Auth Error" ) ) )
		return 415;

	// 423 Auth Error 서버 작업중인 아이디 
	if( !strncmp( pointStr, "X-DaumLogin-Error: 423 Auth Error", strlen( "X-DaumLogin-Error: 423 Auth Error" ) ) )
		return 423;

	// 603 Auth Error 서버 내부 에러
	if( !strncmp( pointStr, "X-DaumLogin-Error: 603 Auth Error", strlen( "X-DaumLogin-Error: 603 OK" ) ) )
		return 603;	

	// 604 Wwwn Connection Fail 해당 Mail 서버 에러
	if( !strncmp( pointStr, "X-DaumLogin-Error: 604 Wwwn Connection Fail", strlen( "X-DaumLogin-Error: 604 Wwwn Connection Fail" ) ) )
		return 604;

	// 605 Wwwn Server Error 해당 mail 서버 에러
	if( !strncmp( pointStr, "X-DaumLogin-Error: 605 Wwwn Server Error", strlen( "X-DaumLogin-Error: 605 Wwwn Server Error" ) ) )
		return 605;

	// 606 Need Relogin 사용자 timeout
	if( !strncmp( pointStr, "X-DaumLogin-Error: 606 Need Relogin", strlen( "X-DaumLogin-Error: 606 Need Relogin" ) ) )
		return 606;

	// 500 System Error 서버 내부 에러
	if( !strncmp( pointStr, "X-DaumLogin-Error: 500 Auth Error", strlen( "X-DaumLogin-Error: 500 OK" ) ) )
		return 500;

	return 0;
}

/**
* 인증 결과 버퍼 재구성
**/
char * CDaumLogin::GetAuthBuffer()
{
	if( !m_resultStringLen )
		return NULL;

	char *pointStr = strstr( m_resultString, "Connection: close" );

	if( !pointStr )
		return NULL;

	int result = pointStr - m_resultString + strlen( "Connection: Close" );

	m_resultString[result] = '\0';
	
	return m_resultString;
}

/**
* HMCU 암호화 데이터 얻기
**/
char * CDaumLogin::GetHMCU()
{
	strcpy( m_HMCU_Buffer, m_resultString );

	char *beginStr = strstr( m_HMCU_Buffer, "HM_CU=" );

	if( !beginStr )
		return NULL;

	char *endStr = strstr( beginStr, ";" );

	if( !endStr )
		return NULL;

	*endStr = '\0';

	strcpy( m_HMCU_Buffer, beginStr + strlen( "HM_CU=" ) );

	return m_HMCU_Buffer;
}


/**
* HTS 암호화 데이터 얻기
**/
char * CDaumLogin::GetHTS()
{
	strcpy( m_HTS_Buffer, m_resultString );

	char *beginStr = strstr( m_HTS_Buffer, "HTS=" );

	if( !beginStr )
		return NULL;

	char *endStr = strstr( beginStr, ";" );

	if( !endStr )
		return NULL;

	*endStr = '\0';

	strcpy( m_HTS_Buffer, beginStr + strlen( "HTS=" ) );

	return m_HTS_Buffer;
}


/**
* PROF 암호화 데이터 얻기 
**/
char * CDaumLogin::GetPROF()
{
	strcpy( m_PROF_Buffer, m_resultString );

	char *beginStr = strstr( m_PROF_Buffer, "PROF=" );

	if( !beginStr )
		return NULL;

	char *endStr = strstr( beginStr, ";" );

	if( !endStr )
		return NULL;

	*endStr = '\0';

	strcpy( m_PROF_Buffer, beginStr + strlen( "PROF=" ) );

	return m_PROF_Buffer;
}


/**
* TS 암호화 데이터 얻기
**/
char * CDaumLogin::GetTS()
{
	strcpy( m_TS_Buffer, m_resultString );

	char *beginStr = strstr( m_TS_Buffer, "TS=" );

	if( !beginStr )
		return NULL;

	char *endStr = strstr( beginStr, ";" );

	if( !endStr )
		return NULL;

	*endStr = '\0';

	strcpy( m_TS_Buffer, beginStr + strlen( "TS=" ) );

	return m_TS_Buffer;
}
