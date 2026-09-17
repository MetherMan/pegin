/* utils.cpp 내부 사용 유틸 */

#include <stdarg.h>

#include "def.h"
#include "structs.h"
#include "extern.h"


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
// Description: random seed . ... ..
//
void e_srandom( unsigned long initial_seed )
{
    seed = initial_seed; 
}


//
// Function Name: e_random
// Date: 2003-02-12
// Description: random .. ..... 
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
// Description: from .. to ... .. ..
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
// Description: ... ..
//
// TODO: number = .... .. ..
//		 .... .. 
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
	DWORD value = ( ( tv.tv_sec * 1000 ) + ( tv.tv_usec / 1000 ) ) -1000000000;
	return value;
}
#endif


//
// Function Name: CheckTime
// Date:
// Description: 
//
DWORD CheckTime()
{
	DWORD time_now;
	time_now = time(NULL);
	return time_now;
}

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
// Function Name: Log
// Date: 2003-02-13
// Description: 내용을 화면과 화일로 출력한다
//
void Log( char* buf, ... )
{
	char tmp_buf[1024];
	va_list args;
	FILE *fp;

	fp = fopen( "errlog.txt", "a" );

	if( !fp )
		return;

	va_start( args, buf );
	vsprintf( tmp_buf, buf, args );
	va_end( args );

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

/* ---------------------- 패킷 관련 함수 ------------------------------------------*/

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
// Date: 2002-02-12
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



/*-------------- 패킷 관련 함수 --------------------------------------------------*/



//
// Function Name: GetLogTime
// Date: 2002-02-17
// Description: 로그에 시간을 남길 필요가 있을때 -_- 쓴다! 
//
char* GetLogTime()
{
	struct tm *t;
	time_t tmp;

	tmp = time( NULL );

	t = localtime( &tmp );

	static char data[128];
	sprintf( data, "%d/%d/%d %d시 %d분 %d초",
	t->tm_year+1900, t->tm_mon+1, t->tm_mday,t->tm_hour, t->tm_min, t->tm_sec );

	return data;
}

/* GetLogTime 사용예제 ************************************

	Log( "%s-This is LogMessage.\n", GetLogTime() );
	
	--- 출력 ---
	03/06/16/ 11시 52분 17초-This is LogMessage 

********************************************************* */

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
// Function Name: GetStrHashIndex
// Date: 2003-03-17
// Description: dHASHKEY_STR_MAX 범위안에서 name의 첫글자에 해당하는 hashindex 값을 리턴 
//
int GetStrHashIndex( char *name )
{
	int hashIdx;

	hashIdx = abs( name[0] / 3 );

	if( hashIdx < 0 || hashIdx > dHASHKEY_STR_ETC )
	{
		hashIdx = dHASHKEY_STR_ETC;
	}
	
	return hashIdx;
}

//
// Function Name: DBConnectionKeeper
// Date: 
// Description: 
//
void DBConnectionKeeper( char *tableName, char *idxName, MYSQL *pSQL )
{
	char query[1024];

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

BOOL LoadDbInfo( char *filename, sPDB_INFO pInfo )
{
	FILE *fp;

	fp = fopen( filename, "rb" );

	if( !fp )
	{
		Log( "LoadDbInfo : [%s] open failed!\r\n" );
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

     	int nNum = number( 0, 3 );

	BYTE offset = (BYTE)( 3 * nNum);

	if( strlen( src ) > strlen( g_ENCRYPT_KEY ) - offset )
		return 0;

	// offset 설정
	*(BYTE*)retStr = offset;
	nPos += sizeof(BYTE);

	int i=0;

	for( i = 0; i < strlen( src ); i++ )
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

	int i=0;
	for( i = 1; i < strlen( src ); i++ )
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

// EOF


