/**
 *	$Workfile: utils.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 19 $
 *	$Date: 04-09-08 3:54p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/utils.cpp $
 * 
 * 19    04-09-08 3:54p Sk8snow
 * 
 * 18    04-07-13 7:26a Sk8snow
 * 
 * 17    04-04-30 9:46a Sk8snow
 * 
 * 16    04-03-31 8:22a Sk8snow
 *        
 */

#include <stdarg.h>
#include "def.h"
#include "structs.h"

#include "extern.h"

#ifndef WIN32
extern DWORD g_ProcessStartTime;
#endif


char g_GAMEDB_NAME[][128] =
{
	"EquipItems",
	"GuildData",
	"InvenItems",
	"UserFriend",
	"UserGarbage",
	"UserQuest",
	"UserSkills",
	"BADUser",
	"\n",
};

char g_LOGDB_NAME[][128] =
{
	"GarbageLog",
	"ItemLog",
	"MoneyLog",
	"\n"
};


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
	DWORD value = ( ( ( tv.tv_sec - g_ProcessStartTime ) * 1000 ) + ( tv.tv_usec / 1000 ) );
	return value;
}
#endif


//
// Function Name: Sleep
// Date:
// Description: Sleep 함수 에뮬레이트 
//
#ifndef WIN32
void Sleep( DWORD time )
{
	struct timeval tv;

	tv.tv_sec = 0;
	tv.tv_usec = time * 1000;

	if( tv.tv_usec >= 1000000 )
	{
		tv.tv_sec = 1;
		tv.tv_usec = 0;
	}
	select( 0, NULL, NULL, NULL, &tv );
}
#endif


//
// Function Name: log
// Date:
// Description: 내용을 화면과 화일로 출력한다
//
void log( char *buf, ... )
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

	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s", 
		currTM->tm_year+1900, 
		currTM->tm_mon+1, 
		currTM->tm_mday, 
		currTM->tm_hour, 
		currTM->tm_min, 
		currTM->tm_sec, 
		tmp_buf );
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

//
// Function Name: log_file
// Date:
// Description: 파일이름을 지정하여 로그를 남김
//
void log_file( char *filename, char *buf, ...  )
{
	char tmp_buf[1024], tmp_buf2[1024];
	va_list args;
	FILE *fp;
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );

	fp = fopen( filename, "a" );

	if( !fp )
		return;

	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );

	sprintf( tmp_buf2, "[%d/%02d/%02d %02d:%02d:%02d] %s", currTM->tm_year+1900, 
		currTM->tm_mon+1, currTM->tm_mday, currTM->tm_hour, currTM->tm_min, currTM->tm_sec, tmp_buf );
	strcpy( tmp_buf, tmp_buf2 );

	fprintf( fp, tmp_buf );

	fclose( fp );
}

//
// Function Name: WriteToFile
// Date:
// Description: 단순 파일 기록
//
void WriteToFile( char *filename, char *buf, ... )
{
	char tmp_buf[1024];
	va_list args;
	FILE *fp;
	
	fp = fopen( filename, "a" );

	if( !fp )
		return;

	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );

	fprintf( fp, tmp_buf );

	fclose( fp );
}

//
// Function Name: DebugMsg
// Date:
// Description: 윈도우의 Debug 콘솔을 사용하기 위한 로그함수
//
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
// Date: 2003-02-12
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
// Date: 2003-02-12
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
// Date: 2003-02-12
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
// Date: 2003-02-12
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
// Date: 2003-02-12
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
// Date: 2003-02-12
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
	*( pBuffer + BufferLength ) = NULL_STR;
	nPos += BufferLength;
}


//
// Function Name: PutSize
// Date: 2003-02-12
// Description: 패킷의 총 사이즈 처리 함수
//
void PutSize( char *pPacket, WORD nPos )
{
	*(WORD*)pPacket = nPos;
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

///////////////////////////////////////////////////////////
// Random Util
///////////////////////////////////////////////////////////
#define	m  (unsigned long)2147483647
#define	q  (unsigned long)127773

#define	a (unsigned int)16807
#define	r (unsigned int)2836

static unsigned long seed;


//
// Function Name: e_srandom
// Date: 2003-02-12
// Description: random seed 값 초기화 함수
//
void e_srandom( unsigned long initial_seed )
{
    seed = initial_seed; 
}


//
// Function Name: e_random
// Date: 2003-02-12
// Description: random 함수 에뮬레이트 
//
unsigned long e_random(void)
{
	register int lo, hi, test;
	
    hi   = seed/q;
    lo   = seed%q;
	
    test = a*lo - r*hi;
	
    if (test > 0)
		seed = test;
    else
		seed = test+ m;
	
    return (seed);
}



//
// Function Name: number
// Date: 2003-02-12
// Description: from 에서 to 사이의 값을 리턴
//
int number( int from, int to )
{
	if( from == to )
		return from;

	if (from > to) {
		int tmp = from;
		from = to;
		to = tmp;
	}
	return (( e_random() % (to - from + 1)) + from);
}


//
// Function Name: dice 
// Date: 2003-02-12
// Description: 주사위 함수
//
// TODO: number = 주사위를 굴릴 횟수
//		 주사위의 면수 
int dice(int number, int size)
{
	int sum = 0;
	
	if (size <= 0 || number <= 0)
		return (0);
	
	while ( number-- > 0 )
		sum += ((e_random() % size) + 1);
	
	return (sum);
}



//
// Function Name: DBConnect
// Date: 2003-02-17
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
// Date: 2003-03-17
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

//
// Function Name: DBConnectionKeeper
// Date: 
// Description: 
//
void DBConnectionKeeper( char *tableName, char *idxName, MYSQL *pSQL )
{
	char query[dMAX_QUERY_SIZE];

	sprintf( query, "select %s from %s limit 1", idxName, tableName );

	mysql_query( pSQL, query );

	MYSQL_RES *result;
	MYSQL_ROW row;
	
	result = mysql_store_result( pSQL );
	
	if( result )
	{
		row = mysql_fetch_row( result );
		mysql_free_result( result );
	}
}

//
// Function Name: DBTableManagement
// Date: 
// Description: 
//
BYTE DBTableManagement( char *tableName, char *command, MYSQL *pSQL )
{
	char query[1024];

	sprintf( query, "%s TABLE %s", command, tableName );

	if( mysql_query( pSQL , query ) )
	{
		log( "DBTableManagement : [%s] Table Name [%s] check failed!! [%s]\r\n", command, tableName, mysql_error( pSQL ) );
		return 0;
	}
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
		
		result = mysql_store_result( pSQL );
		
		if( result )
		{
			while( ( row = mysql_fetch_row( result ) ) )
			{
				if( !strcmp( row[2], "status" ) )
				{
					if( strcmp( row[3], "OK" ) )
					{
						log( "DBTableManagement : %s\t%s\t%s\t%s\r\n", row[0], row[1], row[2], row[3] );
						mysql_free_result( result );
						return 0;
					}
				}
			}
			mysql_free_result( result );
		}
	}
	return 1;
}

//
// Function Name: LoadDbInfo
// Date:
// Description: 
//
BOOL LoadDbInfo( char *filename, sPDB_INFO pInfo )
{
	FILE *fp;

	fp = fopen( filename, "rb" );

	if( !fp )
	{
		log( "LoadDbInfo : [%s] open failed!\r\n", filename );
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

/*---------------------------------------------------------
	Name : LoadBillingIP
	Desc : 빌링서버 IP를 읽는다.
 ---------------------------------------------------------*/
BOOL LoadBillingIP( char *filename, sPBILL_INFO pInfo )
{
	FILE	*fp;
	fp = fopen( filename, "rb" );

	if( !fp )
	{
		log( "LoadDbInfo : [%s] open failed!\r\n", filename );
		return 0;
	}

	fscanf( fp, "HOST %s\n", pInfo->host );
	fscanf( fp, "PORT %d\n", &pInfo->port );
	fscanf( fp, "SERVERNUM %d\n", &pInfo->ServerNum ); //포트에 서버넘버 집어 넣어습니다.

	return 1;
}

//
// Function Name: GetTabString
// Date:
// Description: 
//
void GetTabString( char *src, char *str, int &nPos )
{
	int tmpPos = nPos;

	for( int i = 0; i < 128; i++ )
	{
		if( src[tmpPos] == '\t' ||
			src[tmpPos] == '\r' ||
			src[tmpPos] == '\n' )
		{
			strncpy( str, &src[nPos], tmpPos - nPos );
			str[tmpPos-nPos] = NULL_STR;
			nPos = tmpPos+1;
			return;
		}
		tmpPos++;
	}
}

//
// Function Name: GetTabNumberic
// Date:
// Description: 
//
int GetTabNumberic( char *src, int &nPos )
{
	int tmpPos = nPos;
	char str[128];

	for( int i = 0; i < 128; i++ )
	{
		if( src[tmpPos] == '\t' ||
			src[tmpPos] == '\r' ||
			src[tmpPos] == '\n' )
		{
			strncpy( str, &src[nPos], tmpPos - nPos );
			str[tmpPos-nPos] = NULL_STR;
			nPos = tmpPos+1;
			return atoi( str );
		}
		tmpPos++;
	}
	return 0;
}


DWORD GetTabLNumberic( char *src, int &nPos )
{
	int tmpPos = nPos;
	char str[128];

	for( int i = 0; i < 128; i++ )
	{
		if( src[tmpPos] == '\t' ||
			src[tmpPos] == '\r' ||
			src[tmpPos] == '\n' )
		{
			strncpy( str, &src[nPos], tmpPos - nPos );
			str[tmpPos-nPos] = NULL_STR;
			nPos = tmpPos+1;
			return atol( str );
		}
		tmpPos++;
	}
	return 0;
}

/////////////////////////////////////////////
// Encrypt
/////////////////////////////////////////////
//
// Function Name: SimpleEncrypt
// Date:
// Description: 
//
const char *g_ENCRYPT_KEY = "S!K@8#S$N%O^W&1*2(3)4!5@6#7$8%9^0&A*B(C)D!E@F#G$H%I^J7K*L(M!N@O#P$Q%R^S&T*U(V)W!X@Y#Z$TEAMMAY!@#LAQIA#@$JEON-HONGONLINEMMORPGGAMER#G$H%I^J7K*L(M!N@O#P$Q%R^S&T*U(V)W!X@Y#Z$TEAMMAY";

BOOL SimpleEncrypt( char *src , char *ret )
{
	char retStr[512];
	memset( retStr, 0, 512 );
	int nPos = 0;

	BYTE offset = 3 * number( 0, 3 );

	if( strlen( src ) > strlen( g_ENCRYPT_KEY ) - offset )
		return 0;

	// offset 설정
	*(BYTE*)retStr = offset;
	nPos += sizeof(BYTE);

	for( int i = 0; i < (int)strlen( src ); i++ )
	{
		*(retStr+nPos) = src[i]^0xFFFFFFF;
		nPos++;
		*(retStr+nPos) = g_ENCRYPT_KEY[offset+i]^0xFFFFFFF;
		nPos++;
	}

	strcpy( ret, retStr );

	return 1;
}

//
// Function Name: SimpleDecrypt
// Date:
// Description: 
//
BOOL SimpleDecrypt( char *src, char *ret )
{
	char retStr[512];
	memset( retStr, 0, 512 );
	char key[512];
	memset( key, 0, 512 );

	int nPos = 0;
	int keyPos = 0;

	// offset 읽기
	BYTE offset;

	offset = *(BYTE*)src;

	if( ( offset + strlen( src ) ) >= strlen( g_ENCRYPT_KEY ) )
		return 0;

	for( int i = 1; i < (int)strlen( src ); i++ )
	{
		if( (i%2) )
		{
			*(retStr+nPos) = src[i]^0xFFFFFFF;
			nPos++;
		}
		else
		{
			*(key+keyPos) = src[i]^0xFFFFFFF;
			keyPos++;
		}
	}

	if( strncmp( &g_ENCRYPT_KEY[offset], key , strlen( key ) ) )
		return 0;

	strcpy( ret, retStr );

	return 1;
}

DWORD GetSecondTime( int time )
{
#ifdef USE_LITTLE_SECOND
	return 180;
#endif
	return (DWORD)( 60 * 60 * time );
}

DWORD GetSecondDay( int day )
{
#ifdef USE_LITTLE_SECOND
	return 180;
#endif
	return (DWORD)( 60 * 60 * 24 * day );
}

DWORD GetSecondMin( int min )
{
#ifdef USE_LITTLE_SECOND
	return 180;
#endif
	return (DWORD)( 60 * min );
}

char *GetRemindTimeStr( DWORD value )
{
	static char retStr[128];

	long diff_in_days = (long)( value / 86400 ); 
	long rest_hours = value % 86400; 
	long diff_in_hours = (long)( rest_hours / 3600 ); 
	long rest_mins = rest_hours % 3600; 
	long diff_in_mins = (long)( rest_mins / 60 ); 
	long diff_in_secs = (long)( rest_mins % 60 ); 

	sprintf( retStr, "%ld일 %ld시간 %ld분 %ld초", diff_in_days, diff_in_hours, diff_in_mins, diff_in_secs );
	//sprintf( retStr, "%ld Date %ld Hour %ld Minute %ld Second", diff_in_days, diff_in_hours, diff_in_mins, diff_in_secs );
	return retStr;
}

/////////////////////////////////////////////////////////////
// T i n y    E n c r y p t i o n   A l g o r i t h m 
/////////////////////////////////////////////////////////////






//




