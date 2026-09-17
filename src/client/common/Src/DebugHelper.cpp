/**
 * Wind3D 디버그 정보 출력 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-04-03 8:53p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /Common/Src/DebugHelper.cpp $
 * 
 * 7     03-04-03 8:53p Ahastudio
 * <HR>
 *
 * @file	DebugHelper.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"
#include <WINCON.H>
#include <stdio.h>


//#define DEBUG_OUT_TO_LOG_FILE
#define DEBUG_OUT_TO_CONSOLE


DebugHelper		GlobalDebugHelper;			///< 디버거 헬퍼 객체.
OutputStream	GlobalOutputStream;			///< 출력용 문자열 스트림 객체.


/**
 * 생성자.
 */
DebugHelper::DebugHelper()
{
#ifdef _DEBUG
	AllocConsole();		// Console을 연다.

	SetConsoleTitle( "DEBUG HELPER" );
#endif // #ifdef _DEBUG
} //DebugHelper::DebugHelper

/**
 * 소멸자.
 */
DebugHelper::~DebugHelper()
{
#ifdef _DEBUG

#ifdef DEBUG_OUT_TO_LOG_FILE
	// 로그 파일에 기록을 남긴다.
	FILE* fp = fopen( "debug.log", "at" );
	fprintf( fp, "\n\n" );
	fclose( fp );
#endif // #ifdef DEBUG_OUT_TO_LOG_FILE

	FreeConsole();	// Console을 닫는다.

	// _closeall returns the total number of streams closed...
	// 즉, 아직 열려 있는 Stream이 있으면 에러를 뱉어준다!! +_+
	Assert( 0 == _fcloseall() );

#endif // #ifdef _DEBUG
} //DebugHelper::~DebugHelper

/**
 * Debug 정보를 출력한다. (printf와 용법이 같다.)
 *
 * @param	fmt		[IN] 출력 포맷
 * @param	...		[IN] 출력 인자들
 */
void DebugHelper::Out( const char* fmt, ... )
{
#ifdef _DEBUG

	int		bufcnt;
	char	szMsg[1024];

	// Format에 맞게 인자들을 정리해서 문자열을 만듬.

	va_list ap;
	va_start( ap, fmt );
	bufcnt = vsprintf( szMsg, fmt, ap );
	va_end( ap );

#ifdef DEBUG_OUT_TO_MESSAGE_BOX
	// Message Box를 이용한다.
	// 주의할 점이... Direct Graphics를 쓸 경우엔
	// 어떤 식으로 반응이 일어날지 확언할 수 없다.

	MessageBox( NULL, szMsg, "DEBUG", MB_OK );
#endif // #ifdef DEBUG_OUT_TO_MESSAGE_BOX

	strcat( szMsg, "\n" );
	bufcnt++;

#ifdef DEBUG_OUT_TO_LOG_FILE
	// 로그 파일에 기록을 남긴다.

	FILE* fp = fopen( "debug.log", "at" );
	fprintf( fp, szMsg );
	fclose( fp );
#endif // #ifdef DEBUG_OUT_TO_LOG_FILE

#ifdef DEBUG_OUT_TO_CONSOLE
	// 콘솔에 메시지를 출력한다.

	DWORD nCharsWritten;
	WriteConsole(	GetStdHandle( STD_OUTPUT_HANDLE ),
					(void*)szMsg,
					strlen( szMsg ),
					&nCharsWritten,
					NULL
					);
#endif // #ifdef DEBUG_OUT_TO_CONSOLE

#endif // #ifdef _DEBUG
} //DebugHelper::Out

/**
 * 생성자.
 */
OutputStream::OutputStream()
{
	szText[0] = NULL;
} //OutputStream::OutputStream

/**
 * 소멸자.
 */
OutputStream::~OutputStream()
{
} //OutputStream::~OutputStream

/**
 * 문자 추가.
 *
 * @param	data		[IN] 인자
 *
 * @return	OutputStream
 */
OutputStream& OutputStream::operator + ( const char data )
{
	char	szTemp[80];
	sprintf( szTemp, "%c", data );
	strcat( szText, szTemp );
	return *this;
} //OutputStream::operator +

/**
 * 정수 추가.
 *
 * @param	data		[IN] 인자
 *
 * @return	OutputStream
 */
OutputStream& OutputStream::operator + ( const int data )
{
	char	szTemp[80];
	sprintf( szTemp, "%d", data );
	strcat( szText, szTemp );
	return *this;
} //OutputStream::operator +

/**
 * 실수 추가.
 *
 * @param	data		[IN] 인자
 *
 * @return	OutputStream
 */
OutputStream& OutputStream::operator + ( const float data )
{
	char	szTemp[80];
	sprintf( szTemp, "%f", data );
	strcat( szText, szTemp );
	return *this;
} //OutputStream::operator +

/**
 * 실수 추가.
 *
 * @param	data		[IN] 인자
 *
 * @return	OutputStream
 */
OutputStream& OutputStream::operator + ( const char* data )
{
	strcat( szText, data );
	return *this;
} //OutputStream::operator +
