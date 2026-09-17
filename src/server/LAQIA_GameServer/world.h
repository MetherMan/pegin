/**
 *	$Workfile: world.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 20 $
 *	$Date: 04-09-08 3:54p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/world.h $
 * 
 * 20    04-09-08 3:54p Sk8snow
 * 
 * 19    04-08-04 9:27a Sk8snow
 * 
 * 18    04-07-13 7:26a Sk8snow
 * 
 * 17    04-05-29 10:12p Sk8snow
 * 
 * 16    04-04-30 9:46a Sk8snow
 * 
 * 15    04-04-07 10:17p Sk8snow
 * 
 * 14    04-04-01 5:34p Sk8snow
 * 
 * 13    04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __WORLD_H__
#define __WORDL_H__

//#define dONE_TILE_SIZE				33+1	// 배열 갯수 
//#define dONE_TILE					15		// 배열 당 거리 

#define dONE_TILE_SIZE				25+1	// 배열 갯수 
#define dONE_TILE					20		// 배열 당 거리 

//#define dONE_TILE_SIZE				12+1
//#define dONE_TILE					40

//#define dONE_TILE_SIZE			10+1	
//#define dONE_TILE					50

//#define dONE_TILE_SIZE			29+1	// 배열 갯수 
//#define dONE_TILE					17		// 배열 당 거리 

#define dMAX_DONGEON_CNT			10	// 최대 던전

#define dMAP_KIND_FIELD				0	// 일반 필드 
#define dMAP_KIND_DUNGEON			1	// 던전 
#define dMAP_KIND_INDOOR			2	// 집안 
#define dMAP_KIND_CASTLE			3	// 성 내부

#define dMAX_MAP_LAYER				3

#define dMAX_LOADING_POINT			512

#define dWORLD_OBJ_LOADPOINT		1		// 로딩 포인트 
#define dWORLD_OBJ_HPHEAL			2		// HP 힐링 
#define dWORLD_OBJ_MPHEAL			3		// MP 힐링

#define IS_MAP_RANGE( x, y ) ( x >= 0 && x < dONE_MAP_SIZE && y >= 0 && y < dONE_MAP_SIZE )
#define IS_MOB_MOVABLE( mapNum, x, y ) gWORLD[mapNum]->mobMapAttr[y][x]

#define IS_VALID_WORLD( mapNum ) ( mapNum < dMAX_MAP_CNT && gWORLD[mapNum] )

#define dMAX_WORLD_PARTY_BOARD		250

#define dDEAL_MAP_NUM				20
#define dDEAL_MAP_POSX				100
#define dDEAL_MAP_POSY				150

///////////////////////////////////////////////
// for World Time
///////////////////////////////////////////////
typedef struct 
{
	BYTE time;
	DWORD delay;
} sTIME_SET, *sPTIME_SET;

extern sTIME_SET gWorldTimeTable[7];

//////////////////////////////////////////////
// Party Board
//////////////////////////////////////////////
struct party_board
{
	DWORD		time;
	sPDESC_DATA pPlayer;

	struct party_board *prev;
	struct party_board *next;
};
typedef struct party_board sWORLD_PARTY_BOARD, *sPWORLD_PARTY_BOARD;

/////////////////////////////////////////////
// for Check Loading Point
/////////////////////////////////////////////
typedef struct
{
	int mapNum;
	int posX, posY;
	BYTE posMapKind;
} sLOAD_POINT;


typedef struct
{
	int srcMapNum;
	int	srcPosX, srcPosY;

	int destMapNum;
	int destPosX, destPosY;

	BYTE posMapKind;
} sLOADPOINT_INFO, *sPLOADPOINT_INFO;


////////////////////////////////////////
// N P C   D A T A
////////////////////////////////////////
struct npc_data
{
	int npcNum;
	WORD shopNum;
	WORD questNum;

	BYTE isGarbage;
	BYTE isGuildMaster;

	int mapNum;
	int posX;
	int posY;

	struct npc_data *prev;
	struct npc_data *next;
};

typedef struct npc_data sNPC_DATA, *sPNPC_DATA;


////////////////////////////////////////
// W O R L D    D A T A
////////////////////////////////////////
typedef struct
{
	int mapNum;
	char mapName[64];
	
	/////////////////////////////////////////////
	// M O B
	/////////////////////////////////////////////
	sPMOB_DATA mob[dONE_TILE_SIZE][dONE_TILE_SIZE];
	BYTE mobMapAttr[dONE_MAP_SIZE][dONE_MAP_SIZE];
	sPMOB_RESET mobReset;
	int			mobResetCnt;

	/////////.///////////////////////////////////
	// P L A Y E R
	/////////////////////////////////////////////
	sPDESC_DATA player[dONE_TILE_SIZE][dONE_TILE_SIZE];
	BYTE playerMoveAttr[dONE_MAP_SIZE][dONE_MAP_SIZE];

	/////////////////////////////////////////////
	// I T E M 
	/////////////////////////////////////////////
	sPITEM_DATA item[dONE_TILE_SIZE][dONE_TILE_SIZE];

	/////////////////////////////////////////////
	// Castle and War
	/////////////////////////////////////////////
	sCASTLE_DOOR castleDoor[dMAX_CASTLE_DOOR];			
	sWAR_CRYSTAL warCristal;			

	BYTE						hasCastle;
	BYTE						underAttack;
	int							campTarget;
	int							linkedCastle[dMAX_LINKED_CASTLE];
	int							ownerGuild;	
	int							warPoint;

	// OBJECT
	BYTE objectMap[dONE_MAP_SIZE][dONE_MAP_SIZE];

	// NPC
	sPNPC_DATA npcList;

	int		currPlayerCnt;
	int		currMobCnt;
	int		mobIdxCnt;
	int		itemIdxCnt;
	int		itemTax;						// 세율
	DWORD	TaxBank;						// 세금저장할 곳

	short MobCnt[dMAX_MOBINFO];

	sPWORLD_PARTY_BOARD			partyBoard;

	sPDESC_DATA					pChatList;

	// Q u e s t
	sPQUEST_DEF_INFO			questList;


} sWORLD, *sPWORLD;


////////////////////////////////////////////////////
// H O U S E  D A T A
////////////////////////////////////////////////////
typedef struct
{
	DWORD dwID;					// 문 ID
	DWORD dwMoveMap;			// 이동할 맵의 ID
	BYTE  btType;				// 맵타입
	float fX;					// 이동할 맵에서의 위치
	float fY;					//		""
	char  szBuffer[128];
	
	Vector3 vPos;
	float fYaw;
} sDOOR, *sPDOOR;
// 내부->필드 = 0, 필드->집안 = 1, 필드->성내부 = 2

#define dINDOOR_TO_FIELD	0					// 내부->필드
#define dFIELD_TO_INDOOR	1					// 필드->내부 
#define dFIELD_TO_CASTLE	2					// 필드->성내부 

#define dDOOR_TO_OUT		0					// 필드로 
#define dDOOR_TO_OTHER		1					// 다른 집으로

#define dWEATHER_NORMAL		0					// 기본
#define dWEATHER_SNOW		1					// 눈
#define dWEATHER_RAIN		2					// 비

struct house_data
{
	int mapNum;									// 소속 맵
	int houseNum;								// 집번호 

	char ownerID[50];							// 주인 ID
	BYTE ownerPos;								// 케릭터 번호 

	int	currPlayerCnt;
	int currMobCnt;
	int	mobIdxCnt;
	int itemIdxCnt;

	//
	int doorNum;								// 집에 들어가기 위한 문번호
	int houseType;								// 집 모델 번호

	int targetX;
	int	targetY;

	int linkHouseDoor;
	struct house_data *linkHouse;
	int linkMapDoor;
	sPWORLD linkMap;
	//

	struct descriptor_data *in_player;
	struct item_data *in_items;
	struct monster_data *in_mob;
}; 

typedef struct house_data sHOUSE, *sPHOUSE;
//

extern sPWORLD gWORLD[dMAX_MAP_CNT];
extern BYTE		gWorldUnderAttack;
extern CList gWORLDLIST;
extern BYTE gWorldTime;
extern DWORD gWorldTimeTick;

extern sPWORLD_PARTY_BOARD gPARTY_BOARD_MEMORY;



/////////////////////////////////////////////////
// F U N C S 
/////////////////////////////////////////////////
BOOL InitWorldData();

sPDESC_DATA FindPlayerInWorld( char *name );
BOOL IsLoadPoint( sLOAD_POINT *point, int mapNum, int posX, int posY );
BOOL LoadMobMapAttr( int mapNum );
void PrintAsciiMap( int mapNum );
BOOL IsOpenMap( int mapNum );

void SendWorldTime( sPDESC_DATA pPlayer );

void DestroyWorldData();

BOOL LoadLoadingPointInfo();

sPWORLD_PARTY_BOARD NewPartyBoardArticle();
sPWORLD_PARTY_BOARD FindPartyBoardArticle( sPDESC_DATA pPlayer );

void SendChangeWeather( sPDESC_DATA pPlayer, BYTE value );

void SendToMap( int mapNum , char *data, int size );

void LoadMapName();

void UpdateCastleTaxValue( int mapNum, int taxValue );
void UpdateCastleTaxBank( int mapNum, int value );

#endif



