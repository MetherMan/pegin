/* extern.h */

#ifndef __EXTERN_H__
#define __EXTERN_H__

#include <mysql.h>

#if defined( WIN32 ) // for Windows
extern HWND g_hWnd;
#endif

extern BOOL		g_TeammayTestMode;

extern DWORD	g_CurrentTime;	// 현재 시간 
extern DWORD	g_CurrRealTime;
extern sClients g_Clients;		// 전체 유저 리스트 

extern char	g_Packet[dMAX_SOCK_BUFF];
extern int	g_nPos;

#ifdef __CHINA_VER__
extern MYSQL	*g_pMYSQL;
extern sDB_INFO g_IDPWDB;
#endif

///////////////////////////////////////////////////
// 전역 함수
///////////////////////////////////////////////////

#ifndef WIN32
DWORD timeGetTime();
#endif

DWORD CheckTime();

void LoopDelay( DWORD delayTime );
void Log( char *buf, ... );

/*----------------------------------------------------*/
#define dENC_FIRST_HEADER		'A'
#define dENC_SECOND_HEADER		'L'

#define dMAX_SOCK_BUFF1					4096
#define dMAX_SEND_SOCKBUFF				(dMAX_SOCK_BUFF1*50)
#define dMAX_DATA_SOCK_BUFF				(dMAX_SOCK_BUFF1*1000)

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
/*-------------------------------------------------------*/


char*	GetLogTime();
void	GetMyIP(char* ip);
int		GetStrHashIndex( char *name );

BOOL LoadDbInfo( char *filename, sPDB_INFO pInfo );
void DBConnectionKeeper( char *tableName, char *idxName, MYSQL *pSQL );
MYSQL *DBConnect( char *host, int port, char *id, char *pw , char *dbName );

BOOL SimpleEncrypt( char *src, char *ret );
BOOL SimpleDecrypt( char *src, char *ret );

#endif //__EXTERN_H__





