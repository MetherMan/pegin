#ifndef __EXTERNS_H__
#define __EXTERNS_H__

#include "CMySql.h"
#include "CLoadDataFile.h"

/*
 *	D E F I N E S
 */

#define dSERVER_PORT		2560 // 2560 ? 8999
#define dMAX_LISTEN			200

#define NULL_STR			'\0'

#define dMAP_LIST_FILE			"./MAP/MAPLIST.TXT"
#define dKEEP_ALIVE_TIME		9500

#define dSENDMODE_TOALL			0
#define dSENDMODE_TOOTHER		1
#define dSENDMODE_TOOWNER		2

#define dCHAT_NORMAL			0
#define dCHAT_CRY				1				// 외치기 	
#define dCHAT_WHISPER			2				// 귓말 
#define dCHAT_BUY				3				// 판매 
#define dCHAT_PARTY				4				// 파티
#define dCHAT_GUILD				5				// 길드
#define dCHAT_SYSTEM			6				// 시스템
#define dCHAT_NOTICE			7				// 공지 
#define dCHAT_DAMAGE			8				// 데미지


#define MAX( val, max_val ) (val > max_val ? max_val : val)
#define MAX_CHECK( val, max_val ) if( val > max_val ){ val = max_val;}
#define MIN( val, min_val ) (val < min_val ? min_val : val)
#define MIN_CHECK( val, min_val ) if( val < min_val ){ val = min_val;}

#define dPLAYER					0
#define dMOB					1

#define IS_SET(flag, bit)  ((flag) & (bit))
#define SET_BIT(var, bit)  ((var) |= (bit))
#define REMOVE_BIT(var, bit)  ((var) &= ~(bit))

/*
 *	E X T E R N   V A R I A B L E S
 */
extern BOOL			g_GameLife;
extern SOCKET		g_MainSock;

extern int			g_ServerPort;
extern int			g_MaxListen;
extern char			g_ServerName[128];


extern DWORD		g_CurrTime;
extern DWORD		g_CurrRealTime;

#ifndef WIN32
extern DWORD		g_ProcessStartTime;
#else
extern HINSTANCE	hInst;				// Current instance
extern HWND			g_hwnd;				// Window Handle
#endif

extern	int			i,j,k,l,x,y,z;

extern	sDB_INFO	g_DATADB;		// DATADB
extern  sDB_INFO	g_LOGDB;		// LOGDB
extern	CMySql		g_DataSql;	
extern  CMySql		g_ItemSql;
extern	CMySql		g_LogSql;

extern sGLOBAL_MEMORY			g_MEMORY;

extern char			escID[128];
extern char			escName[128];
extern char			escItemName[128];
extern char			escMaker[128];
extern char			escSerial[128];

extern int			g_EXPTABLE[dMAX_LEVEL];	
extern sPMOB_INFO	g_MOBINFO[dMAX_MOB_INFO];

extern DWORD		timePulse;


typedef struct 
{
	char	command[50];
	void	(*chat_func)( sPDESC_DATA pPlayer, char *command );
}sCHAT_COMMAND, *sPCHAT_COMMAND;

/*
 *	F U N C T I O N S	
 */
#ifndef WIN32
DWORD timeGetTime();
void Sleep( DWORD time );
#endif

void log( char *buf, ... );
void log_file( char *filename, char *buf, ...  );
void WriteToFile( char *filename, char *buf, ... );
void DebugMsg( char *buf, ... );

// Random Util
void e_srandom( unsigned long initial_seed );
unsigned long e_random(void);
int number( int from, int to );
int dice(int number, int size);

// Get string hash index
int GetStrHashIndex( char *name );

BOOL LoadDbInfo( char *filename, sPDB_INFO pInfo );

void InitGlobalMemory();
void DestroyGlobalMemory();

char *MakeString( char *buf, ... );
void SendSystemMsg( sPDESC_DATA pPlayer, char *msg, BYTE btType = dCHAT_SYSTEM );
void SendSystemMsg( sPDESC_DATA pPlayer, char *msg, char *buf, int &bufSize, BYTE btType = dCHAT_SYSTEM );
void MakeSystemMsg( char *msg );
void SendNoticeMsg( sPDESC_DATA pPlayer, char *msg, BYTE btColor );
void SendNoticeMsg( sPDESC_DATA pPlayer, char *msg, BYTE btColor, char *buf, int &bufSize );

BOOL CheckChatCommand( sPDESC_DATA pPlayer, char *command );
int GetDistance( int posX, int posY, int posX2, int posY2 );

void BroadCastSystemMsg( char *str );
void BroadCastNoticeMsg( char *str );

void ToUpper( char *str );
void ToLower( char *str );

#endif


//EOF


