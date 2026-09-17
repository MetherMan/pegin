/**
 *	$Workfile: extern.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 46 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/extern.h $
 * 
 * 46    04-12-22 5:18p Sk8snow
 * 
 * 45    04-12-15 10:40a Sk8snow
 * 
 * 44    04-09-20 10:53p Sk8snow
 * 
 * 43    04-09-08 3:54p Sk8snow
 * 
 * 42    04-08-04 9:27a Sk8snow
 * 
 * 41    04-07-13 7:26a Sk8snow
 * 
 * 40    04-05-29 10:12p Sk8snow
 * 
 * 39    04-04-30 9:46a Sk8snow
 * 
 * 38    04-04-07 10:17p Sk8snow
 * 
 * 37    04-04-03 4:03p Sk8snow
 * 
 * 36    04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __EXTERN_H__
#define __EXTERN_H__

#define dUSE_DIRECT_ITEM_DB_ACCESS

#ifndef __CHINA_VER__
//#define dNON_PVP_MODE			// PK 불허
//#define dNOT_USE_PET			// 애완동물
//#define dNOT_USE_GUILD		// 길드 사용 않함 
//#define dNOT_USE_FRIEND		// 친구 사용 않함
#endif

#define dEVENT_MOB_ON			// 이벤트 몹 사용 여부 
 
#define dMAX_LANGUAGE_STR		512
#define dMAX_LANGUAGE_STR_LEN	128

extern char g_LANG_STR[dMAX_LANGUAGE_STR][dMAX_LANGUAGE_STR_LEN];

///////////////////////////////////////////////////
// FILE DIR
///////////////////////////////////////////////////
#define dDAMAGE_TABLE_FILE			"./DATA/damage_table.txt"
#define dITEM_TABLE_FILE			"./DATA/ITEM_DATA.txt"
#define dMOB_TABLE_FILE			"./DATA/MOB_DATA.txt"
#define dLEVEL_HPMP_TABLE_FILE		"./DATA/StateLevelHPMP.txt"
#define dCHARPOINT_TABLE_FILE		"./DATA/char_def_point.txt"
#define dSTATE_EXP_TABLE_FILE		"./DATA/CharacterAbilityExp.txt"
#define dSKILL_EXP_TABLE_FILE		"./DATA/SkillLevelExp.txt"
#define dBASEHIT_TABLE_FILE			"./DATA/AttributeTable_Base_Hit.txt"
#define dSKILL_TABLE_FILE			"./DATA/skill_data.txt"
#define dMOBINVEN_TABLE_FILE		"./DATA/MobItemLoseTable.txt"
#define dSHOPINFO_TABLE_FILE		"./DATA/SHOP_INFO.txt"
#define dNPCINFO_TABLE_FILE			"./DATA/NPC_INFO.txt"
#define dADMININFO_TABLE_FILE		"./DATA/ADMIN_INFO.txt"
#define dSPE_SKILL_TABLE_FILE		"./DATA/spe_skill_class.txt"
#define dEXP_PROTECT_TABLE_FILE		"./DATA/exp_protect_table.txt"
#define dLANG_LIST_FILE			"./DATA/lang_table.txt"
#define dNEED_ITEMLOG_LIST_FILE		"./DATA/log_item.txt"
#define dPET_INFO_TABLE_FILE		"./DATA/PET_INFO.txt"
#define dWAR_INFO_TABLE_FILE		"./DATA/WAR_INFO.txt"
#define dWARCRISTAL_INFO_FILE		"./DATA/WAR_CRISTAL.txt"
#define dCASTLE_DOOR_INFO_FILE		"./DATA/CASTLE_DOOR.txt"
#define dQUEST_TABLE_FILE			"./DATA/Quest_data.txt"

#ifdef dEVENT_MOB_ON
#define dEVMOB_ARMOR_TABLE_FILE		"./DATA/EVMOB_ARMOR.txt"
#define dEVMOB_WEAPON_TABLE_FILE		"./DATA/EVMOB_WEAPON.txt"
#endif

#define dMAP_LIST_FILE			"./MAP/map_list.txt"
#define dMAP_LOADING_POINT_FILE		"./MAP/loadingpoint.txt"
#define dMAP_NAME_FILE			"./MAP/map_name.txt"

#define dBILLING_TABLE_FILE			"./DATA/BILLING_DATA.txt" 

///////////////////////////////////////////////////
// TEST FILE DIR
///////////////////////////////////////////////////
#define dTEST_MOB_TABLE_FILE		"./DATA/TestData/MOB_DATA.txt"		//테스트서버 몬스터 테이블 파일
#define dTEST_MOB_DAMAGE_FILE		"./DATA/TestData/damage_table.txt"		//테스트서버 데미지 테이블 파일
#define dTEST_MOB_NPCINFO_TABLE_FILE	"./DATA/TestData/NPC_INFO.txt"


#define dMTL		"./DATA/MOB_DATA.txt"
#define dDTF		"./DATA/damage_table.txt"
#define dSTF		"./DATA/skill_data.txt"
#define dNTF		"./DATA/NPC_INFO.txt"



///////////////////////////////////////////////////
// 전역 변수
///////////////////////////////////////////////////
extern sPLAYERS gPLAYERS;
extern sMOBS gMOBS;
extern sITEMS gITEMS;
extern MYSQL *g_MYSQL;
extern MYSQL *g_LOGSQL;
extern MYSQL *g_CASHMYSQL;

#if defined (WIN32)
extern HWND			g_hwnd;		// Window Handle
#endif

extern char g_Packet[dMAX_DATA_SOCK_BUFF];
extern int g_nPos;

#ifdef USE_ENCRYPT_PACKET
extern char g_EncPacket[dMAX_DATA_SOCK_BUFF+512];
#endif

extern DWORD g_CurrTime;
extern DWORD g_CurrRealTime;

extern char	g_TMP_STR[1024];

extern sDB_INFO g_ITEMDBINFO;
extern sDB_INFO g_DATASERVERINFO;
extern sDB_INFO g_CASHINFO;

extern BYTE g_itemRemoverOn;
extern WORD g_itemRemoverNum;

extern BYTE g_ExpJump;
extern BYTE g_EventExp;


extern char g_GAMEDB_NAME[][128];
extern char g_LOGDB_NAME[][128];

extern int g_nWeaponP[21];
extern int g_nArmorP[21];
extern int g_nAccessoriP[21];

extern int	g_ServerNumber;
extern int  g_nEnchantTotal;

////////////////////////////////////////////////////
// D E F I N E S
////////////////////////////////////////////////////

#define dKEEP_ALIVE_TIME		9500
#define dSPEED_CHECK_LIMIT		10000

#define dATTACK_PACKET_RANGE	25

#define dCHAT_DELAY_TIME		500		// 0.3 초 정도 ?

#define dBILL_BEGIN_TIMESTAMP		1093964400		// 2002-09-01 00:00:00

///////////////////////////////////////////////////
// 전역 함수
///////////////////////////////////////////////////
void log( char *buf, ... );
void log_file( char *filename, char *buf, ...  );
void WriteToFile( char *filename, char *buf, ... );
void SendData( sPDESC_DATA pClient , char *data = g_Packet, int size = g_nPos );
void SendUDPData( SOCKET sock, char *data, int size, struct sockaddr_in *addr );

#define dENC_FIRST_HEADER		'A'
#define dENC_SECOND_HEADER		'L'

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

// Random Util
int number( int from, int to );
int dice( int number, int size );


#ifndef WIN32
DWORD timeGetTime();
#endif

void DebugMsg( char *buf, ... );

BOOL LoadDbInfo( char *filename, sPDB_INFO pInfo );
BOOL LoadBillingIP( char *filename, sPBILL_INFO pInfo );

///////////////////////////////////////////////////
// 비트 연산 매크로
///////////////////////////////////////////////////
#define IS_SET(flag, bit)  ((flag) & (bit))
#define SET_BIT(var, bit)  ((var) |= (bit))
#define REMOVE_BIT(var, bit)  ((var) &= ~(bit))

#define SAFE_FREE( mem )	{ free(mem); mem = NULL; }


////////////////////////////////////////////////////
// D E F I N E S
///////////////////////////////////////////////////
#define MAX( val, max_val ) (val > max_val ? max_val : val)
#define MIN( val, min_val ) (val < min_val ? min_val : val)
///////////////////////////////////////////////////
// Send Update Info
///////////////////////////////////////////////////

void DATASERV_SendUpdateSkill( sPDESC_DATA pPlayer , BYTE skillNum );
void DATASERV_SendUpdateSkillExp( sPDESC_DATA pPlayer , BYTE skillNum );
void DATASERV_SendUpdateHp( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateMp( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateMapInfo( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateMoney( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateCash( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateStr( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateStrExp( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateInt( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateIntExp( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateDex( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateDexExp( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateChEquip( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateMercInfo( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateGuildInfo( sPDESC_DATA pPlayer );
void DATASERV_SendUpdateCha( sPDESC_DATA pPlayer );
void DATASERV_CrashGuild( int guildNo );
void DATASERV_UpdateClass( sPDESC_DATA pPlayer );
void DATASERV_SAVECHA( sPDESC_DATA pPlayer );

MYSQL *DBConnect( char *host, int port, char *id, char *pw , char *dbName );

int GetStrHashIndex( char *name );

void MakeChatMsg( sPDESC_DATA pClient , char *str );

void DBConnectionKeeper( char *tableName, char *idxName, MYSQL *pSQL );
BYTE DBTableManagement( char *tableName, char *command, MYSQL *pSQL );

void GetTabString( char *src, char *str, int &nPos );
int GetTabNumberic( char *src, int &nPos );
DWORD GetTabLNumberic( char *src, int &nPos );

void SendSystemMsg( sPDESC_DATA pPlayer, char *str, ... );
void SendSystemMsg( sPDESC_DATA pPlayer, const char *str );
void SendFriendMsg( sPDESC_DATA pPlayer, char *str, ... );

BOOL SimpleEncrypt( char *src , char *ret );
BOOL SimpleDecrypt( char *src, char *ret );

DWORD GetSecondTime( int time );
DWORD GetSecondDay( int day );
DWORD GetSecondMin( int min );
char *GetRemindTimeStr( DWORD value );
extern BOOL		g_bGameStar;

void LoadCountItem();

extern int	g_nWeapon;
extern int  g_nArmor;
extern int  g_nAccessori;

#endif





