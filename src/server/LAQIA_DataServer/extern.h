#ifndef __EXTERN_H__
#define __EXTERN_H__


#if defined (WIN32)
extern HWND			g_hwnd;		// Window Handle
#endif

extern MYSQL *g_USER_DB;

extern CList	g_ServerList;		// 연결된 서버 리스트


extern char g_Packet[dMAX_SOCK_BUFF];
extern int g_nPos;

#define dENC_FIRST_HEADER		'A'
#define dENC_SECOND_HEADER		'L'

#define dMAX_SOCK_BUFF1					4096
#define dMAX_SEND_SOCKBUFF				(dMAX_SOCK_BUFF1*50)
#define dMAX_DATA_SOCK_BUFF				(dMAX_SOCK_BUFF1*1000)

////////////////////////////////////////////////////
// D E F I N E S
///////////////////////////////////////////////////
#define MAX( val, max_val ) (val > max_val ? max_val : val)
#define MIN( val, min_val ) (val < min_val ? min_val : val)

///////////////////////////////////////////////////
// 전역 함수
///////////////////////////////////////////////////
void log( const char *buf, ... );
void SendData( sPDESC_DATA pClient , const char *data , int size );

#ifndef WIN32
DWORD timeGetTime();
#endif

// BYTE
void PutByte( char *pPacket, BYTE value, int &nPos );
BYTE GetByte( char *pPacket, int &nPos );

// WORD
void PutWord( char *pPacket, WORD value, int &nPos );
WORD GetWord( char *pPacket, int &nPos );

//DWORD
void PutDword( char *pPacket, DWORD value, int &nPos );
DWORD GetDword( char *pPacket, int &nPos );

// int
void PutInteger( char *pPacket, int value, int &nPos );
int GetInteger( char *pPacket, int &nPos );

//short
void PutShort( char *pPacket, short value, int &nPos );
short GetShort( char *pPacket, int &nPos );

// String
void PutString( char *pPacket, char *str, int &nPos );
void GetString( char *pPacket, char *pBuffer, int &nPos );

void PutSize( char *pPacket, WORD nPos );

//
// Function Name: PutSize
// Date: 2004/04/23
// Description: 패킷의 총 사이즈 처리 함수
//
void PutHeader( char *pPacket, WORD value, int &nPos );
void PutHeader( WORD value );




MYSQL *DBConnect( char *host, int port, char *id, char *pw , char *dbName );
void UpdateCharData( sPCHAR_DATA pChar );
void SaveCharData( sPCHAR_DATA pChar );
void UpdateCashData( sPCHAR_DATA pChar );

int GetStrHashIndex( char *name );

BOOL LoadDbInfo( char *filename, sPDB_INFO pInfo );

#endif 



