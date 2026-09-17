/*
	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	utils.cpp

	Build log...
*/
#include <stdarg.h>

#include "def.h"
#include "structs.h"
#include "extern.h"


//
// Function Name: timeGetTime
// Date:
// Description: Unix 기반에선 timeGetTime같은 ms단위의 CPU시간을 다루는 함수가
//				없는 경우가 많음으로 에뮬레이트 함수를 사용한다.
//
#ifndef WIN32
DWORD timeGetTime()
{
	struct timeval tv;
	gettimeofday( &tv, NULL );
	return ( ( tv.tv_sec * 1000 ) + ( tv.tv_usec / 1000 ) )-1000000000;
}
#endif


//
// Function Name: LoopDelay
// Date:
// Description: delayTime의 ms단위 시간만큼 프로세스 정지
//
void LoopDelay( DWORD delayTime )
{
#ifndef WIN32
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = delayTime * 1000;

	if( tv.tv_usec >= 1000000 )
	{
		tv.tv_sec = 1;
		tv.tv_usec = 0;
	}
	select( 0, NULL, NULL, NULL, &tv );
#else
	Sleep( delayTime );
#endif
}



//
// Function Name: log
// Date:
// Description: 내용을 화면과 화일로 출력한다
//
void log( const char *buf, ... )
{
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;
	FILE *fp;
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );

	fp = fopen( "errlog.txt", "a" );

	if( !fp )
		return;

	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );

	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s", currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf );
	strcpy( tmp_buf, tmp_buf2 );

	fprintf( fp, tmp_buf );

#ifndef WIN32 
	printf( tmp_buf );
#endif 

#ifdef _CONSOLE_MODE_
	DWORD nCharsWritten;
	WriteConsole(	GetStdHandle( STD_OUTPUT_HANDLE ),
		(void*)tmp_buf,
		strlen( tmp_buf ),
		&nCharsWritten,
		NULL
		);
#endif 

	fclose( fp );
}

void DebugMsg( char *buf, ... )
{
	char tmp_buf[1024];
	va_list args;
			
	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );

#if defined(WIN32)
	OutputDebugString( tmp_buf );
#else
	printf( tmp_buf );
#endif
}


//
// Function Name: PutByte, GetByte 
// Date: 2002-02-12
// Description: BYTE 형의 패킷 처리
//
void PutByte( char *pPacket, BYTE value, int &nPos )
{
	*(BYTE*)(pPacket + nPos) = value;
	nPos += sizeof(BYTE);
}

BYTE GetByte( char *pPacket, int &nPos )
{
	BYTE value = *(BYTE*)(pPacket + nPos );
	nPos += sizeof(BYTE);
	return value;
}


//
// Function Name: PutWord, GetWord
// Date: 2002-02-12
// Description: WORD 형의 패킷 처리
//
void PutWord( char *pPacket, WORD value, int &nPos )
{
	*(WORD*)(pPacket + nPos ) = value;
	nPos += sizeof(WORD);
}

WORD GetWord( char *pPacket, int &nPos )
{
	WORD value = *(WORD*)(pPacket + nPos );
	nPos += sizeof(WORD);
	return value;
}



//
// Function Name: PutDword, GetDword
// Date: 2002-02-12
// Description: DWORD 형의 패킷 처리
//
void PutDword( char *pPacket, DWORD value, int &nPos )
{
	*(DWORD*)(pPacket + nPos ) = value;
	nPos += sizeof(DWORD);
}

DWORD GetDword( char *pPacket, int &nPos )
{
	DWORD value = *(DWORD*)(pPacket + nPos );
	nPos += sizeof(DWORD);
	return value;
}


//
// Function Name: PutInteger, GetInteger
// Date: 2002-02-12
// Description: int 형의 패킷 처리 함수
//
void PutInteger( char *pPacket, int value, int &nPos )
{
	*(int*)(pPacket + nPos ) = value;
	nPos += sizeof(int);
}

int GetInteger( char *pPacket, int &nPos )
{
	int value = *(int*)(pPacket + nPos );
	nPos += sizeof(int);
	return value;
}

//
// Function Name: PutShort, GetShort
// Date: 2002-02-12
// Description: short 형의 패킷 처리 함수
//
void PutShort( char *pPacket, short value, int &nPos )
{
	*(short*)(pPacket + nPos ) = value;
	nPos += sizeof(short);
}

short GetShort( char *pPacket, int &nPos )
{
	short value = *(short*)(pPacket + nPos );
	nPos += sizeof(short);
	return value;
}


//
// Function Name: PutString, GetString
// Date: 2002-02-12
// Description: 문자열 데이터 패킷 처리 함수 ( 사이즈가 헤더로 붙는다 )
//
void PutString( char *pPacket, char *str, int &nPos )
{
	*(WORD*)(pPacket + nPos ) = strlen( str );
	nPos += sizeof(WORD);
	memcpy( pPacket + nPos, str, strlen( str ) );
	nPos += strlen(str); 
}


void GetString( char *pPacket, char *pBuffer, int &nPos )
{
	WORD BufferLength;

	BufferLength = *(WORD*)(pPacket + nPos );
	nPos += sizeof(WORD);
	memcpy( pBuffer, pPacket + nPos, BufferLength);
	*( pBuffer + BufferLength ) = '\0';
	nPos += BufferLength;
}


//
// Function Name: PutSize
// Date: 2004/04/23
// Description: 패킷의 총 사이즈 처리 함수
//
void PutHeader( char *pPacket, WORD value, int &nPos )
{
	nPos = 0;

	pPacket[0] = dENC_FIRST_HEADER;
	pPacket[1] = dENC_SECOND_HEADER;

	nPos += 2;

	*(WORD*)(pPacket + nPos ) = value;
	nPos += sizeof(WORD);
}

void PutHeader( WORD value )
{
	g_nPos = 0;

	g_Packet[0] = dENC_FIRST_HEADER;
	g_Packet[1] = dENC_SECOND_HEADER;

	g_nPos += 2;

	*(WORD*)( g_Packet + g_nPos ) = value;
	g_nPos += sizeof(WORD);
}



//
// Function Name: PutSize
// Date: 2002-02-12
// Description: 패킷의 총 사이즈 처리 함수
//
void PutSize( char *pPacket, WORD nPos )
{
	*(WORD*)pPacket = nPos;
}

//
// Function Name: DBConnect
// Date: 2002-02-17
// Description: 
//
MYSQL *DBConnect( char *host, int port, char *id, char *pw , char *dbName )
{
	// MYSQL 구조체
	MYSQL *db;

	db = mysql_init( (MYSQL*)NULL );		// 초기화 함수

	if( db )
	{
		if( mysql_real_connect( db, host, id, pw, NULL, port, (char*)NULL, 0 ) ) // DB 접속 
		{
			if( mysql_select_db( db, dbName ) != 0 ) // DB 선택
			{
				mysql_close( db );
				return NULL;
			}
		}
		else
		{
			mysql_close( db );
			return NULL;
		}
	}
	else
		return NULL;
	return db;
}




//
// Function Name: GetStrHashIndex
// Date: 2002-03-17
// Description: 
//
int GetStrHashIndex( char *name )
{
	int hashIdx;

	hashIdx = abs( name[0] );

	if( hashIdx < 0 || hashIdx > dHASHKEY_STR_ETC )
		hashIdx = dHASHKEY_STR_ETC;
	
	return hashIdx;
}

BOOL LoadDbInfo( char *filename, sPDB_INFO pInfo )
{
	FILE *fp;

	fp = fopen( filename, "rb" );

	if( !fp )
	{
		log( "LoadDbInfo : [%s] open failed!\r\n" );
		return 0;
	}

	fscanf( fp, "HOST %s\n", pInfo->host );
	fscanf( fp, "PORT %d\n", &pInfo->port );
	fscanf( fp, "ID %s\n", pInfo->id );
	fscanf( fp, "PW %s\n", pInfo->pw );
	fscanf( fp, "DBNAME %s\n", pInfo->dbName );

	fclose( fp );
	return 1;
}
