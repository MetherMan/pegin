/**
 *	$Workfile: monster.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 21 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/monster.h $
 * 
 * 21    04-12-15 10:40a Sk8snow
 * 
 * 20    04-09-20 10:53p Sk8snow
 * 
 * 19    04-09-08 3:54p Sk8snow
 * 
 * 18    04-07-13 7:26a Sk8snow
 * 
 * 17    04-05-29 10:12p Sk8snow
 * 
 * 16    04-05-18 1:17a Sk8snow
 * 
 * 15    04-05-11 1:45a Sk8snow
 * 
 * 14    04-04-30 9:46a Sk8snow
 * 
 * 13    04-04-07 10:17p Sk8snow
 * 
 * 12    04-04-03 4:03p Sk8snow
 * 
 * 11    04-03-31 8:22a Sk8snow
 *        
 */
 
#ifndef __MONSTER_H__
#define __MONSTER_H__

#ifdef dEVENT_MOB_ON

#define dEVENT_MOB_ITEM_MAX		300 //아이템스

typedef struct {
	BYTE isAlive;
	DWORD updateTime;
	int	prevMapNum;
} sEVENT_MOB, *sPEVENT_MOB;

#define dMAX_EVENT_MOB			3
extern sEVENT_MOB g_EVENT_MOB[dMAX_EVENT_MOB];

#endif

//#define __ONLY_ONE_MOB_IN_SECTOR__

typedef struct
{
	int posX;
	int posY;
} sATTACK_POSITION, *sPATTACK_POSITION;

typedef struct
{
	int x;
	int y;
} sMOB_MOVE_POINT, *sPMOB_MOVE_POINT;

#define dMOB_MOVE_POINT_CNT		5

typedef struct 
{
	BYTE	isFinish;
	BYTE	size;

	sMOB_MOVE_POINT		list[dMOB_MOVE_POINT_CNT];
} sMOB_MOVE_INFO, *sPMOB_MOVE_INFO;

///////////////////////////////////////////////////////////////////
// D E F I N E S 
///////////////////////////////////////////////////////////////////
#define dMOBATTACK_ATTR_NORMAL			(1<<1)		// 후공격 
#define dMOBATTACK_ATTR_AGRESSIVE			(1<<2)		// 무조건 선공격
#define dMOBATTACK_ATTR_EVILATTACK			(1<<3)		// 카오 선공격 
#define dMOBATTACK_ATTR_NORMALATTACK		(1<<4)		// 라우풀 선공격 
#define dMOBATTACK_ATTR_RANDOM			(1<<5)		// 선후 공격 랜덤

#define dMOBRESQUE_ATTR_MYAREA			(1<<1)		// 주위 몹 원조 
#define dMOBRESQUE_ATTR_CHOICE			(1<<2)		// 지정 몹 원조 
#define dMOBRESQUE_ATTR_EVILCHAR			(1<<3)		// 카오 케릭 원조 
#define dMOBRESQUE_ATTR_NORMALCHAR			(1<<4)		// 라우풀 케릭 원조

#define dDEF_MONSTER_HP				100

#define dMAX_MOB_IDX					65000

#define dMON_STATE_UNUSED				1		// 사용 않하고 있는 데이터
#define dMON_STATE_NORMAL				2		// 일반 상태
#define dMON_STATE_FIGHT				3		// 전투중

#define dMONSTER_CASTLEDOOR				1001	// 성문 
#define dMONSTER_WARCRISTAL				1002	// 성물

#define dMONSTER_HP_CASTLEDOOR			1000	// 성문 HP
#define dMONSTER_HP_WARCRISTAL			1000	// 성물 HP

#define dDEF_MOB_EXP					5
#define dDEF_MOB_ATTACKRANGE			2		// 최소 공격 거리

#define dDEF_MOB_AITICK				50
#define dMAX_MOB_BLOCKOVER				50

#define MOVABLE_OK					1
#define MOVABLE_BLOCKMOB				2
#define MOVABLE_BLOCKPLAYER				3
#define MOVABLE_BLOCKMAP				4
#define MOVABLE_BLOCKEXC				5

//_dUMENOKOJI_		//마방관리
#define MOB_ATTR_PHYSICS				0	
#define MOB_ATTR_MAGIC				1


#define GET_MOB_HNAME( mob ) (g_MOBINFO[mob->mobNum]->hName)
#define GET_MOB_MONEY( mob ) (g_MOBINFO[mob->mobNum]->money)
#define GET_MOB_ATTR( mob ) (g_MOBINFO[mob->mobNum]->mobAttr)
#define GET_MOB_ATTACKATTR( mob ) (g_MOBINFO[mob->mobNum]->attackAttr)
#define GET_MOB_HELPATTR( mob ) (g_MOBINFO[mob->mobNum]->helpAttr)
#define GET_MOB_MINDAMAGE( mob ) (g_MOBINFO[mob->mobNum]->minDamage)
#define GET_MOB_MAXDAMAGE( mob ) (g_MOBINFO[mob->mobNum]->maxDamage)

#ifdef _dUMENOKOJI_										// 마방관련
#define GET_MOB_ATTACK_ATTR( mob ) ( g_MOBINFO[mob->mobNum]->mobAttackAttr )		//몹의 공격 속성
#define GET_MOB_MINMAGICDAMAGE( mob ) ( g_MOBINFO[mob->mobNum]->minMagicDamage )	//마법 최소 공격력 
#define GET_MOB_MAXMAGICDAMAGE( mob ) ( g_MOBINFO[mob->mobNum]->maxMagicDamage )	//마법 최대 공격력
#define GET_MOB_MDP( mob ) ( g_MOBINFO[mob->mobNum]->Magicdp )				//마법방어력
#endif

#define GET_MOB_ATTACKSPEED( mob ) (g_MOBINFO[mob->mobNum]->attackSpeed)
#define GET_MOB_VISIONRANGE( mob ) (g_MOBINFO[mob->mobNum]->defenseRange)
#define GET_MOB_DEFENSERANGE( mob ) (g_MOBINFO[mob->mobNum]->defenseRange)
#define GET_MOB_ATTACKRANGE( mob ) (g_MOBINFO[mob->mobNum]->attackRange)
#define GET_MOB_AP( mob ) (g_MOBINFO[mob->mobNum]->ap)
#define GET_MOB_DP( mob ) (g_MOBINFO[mob->mobNum]->dp)
#define GET_MOB_HP( mob ) (g_MOBINFO[mob->mobNum]->hp)
#define GET_MOB_MP( mob ) (g_MOBINFO[mob->mobNum]->mp)
#define GET_MOB_EXP( mob ) (g_MOBINFO[mob->mobNum]->exp)
#define GET_MOB_LEVEL( mob ) (g_MOBINFO[mob->mobNum]->level)
#define GET_MOB_HIT( mob )(g_MOBINFO[mob->mobNum]->nHit)

#define GET_MOB_DROPDICE( mob ) (g_MOBINFO[mob->mobNum]->dropPercent)

#define SET_MOB_ENEMY( mob, value )  { mob->enemy = value; mob->lastMoveTime = g_CurrTime; }

///////////////////////////////////////////////////////////////////
// V A R I A B L E S
///////////////////////////////////////////////////////////////////
extern sPMOB_DEF_INFO g_MOBINFO[dMAX_MOBINFO];
extern int	g_MobResetTime;
extern BYTE g_MOB_MAX_RESET[dMAX_MAP_CNT][dMAX_MOBINFO];


///////////////////////////////////////////////////////////////////
// F U N C 
///////////////////////////////////////////////////////////////////
void MakeMobInPacket( sPMOB_DATA pMob , BYTE isCreate );
void MakeMobOutPacket( sPMOB_DATA pMob );

// In
void GetMobSector( sPDESC_DATA pPlayer, int x, int y );
void GetMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void GetMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void OutMobSector( sPDESC_DATA pPlayer, int x, int y );
void OutMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void OutMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void SendToMobArea( sPMOB_DATA pMob, char *data, int size );
void GetCurrAreaMob( sPDESC_DATA pMob );
void OutCurrAreaMob( sPDESC_DATA pPlayer );
BOOL CreateMob( int mobNum, int mapNum, int posX, int posY , sPMOB_RESET pCreater, char *filename, int line );
void DelMobData( sPMOB_DATA pMob , char *file, int line );
void MonsterMoveProc();
sPMOB_DATA FindMobInTile( sPMOB_DATA list, int mobNum );
sPMOB_DATA FindMobInArea( int mapNum, int posX, int posY, int mobNum );
void MoveMonster( sPMOB_DATA pMob, int posX, int posY );
void CheckMobEnemyArea( sPMOB_DATA pMob );

void SendMobState( sPMOB_DATA pMob );

BOOL LoadMobInfo();

BOOL LoadMobReset( int mapNum );

void DeleteDefaultMobInfo();

void MobResetPulse();

void MoveResetPoint();

void OutCurrHouseMob( sPDESC_DATA pPlayer );
void GetCurrHouseMob( sPDESC_DATA pPlayer );

BOOL LoadMobDropItemTable();

void InitMaxMobReset( BYTE value );

void SendMobEffect( sPMOB_DATA pMob, WORD effect );

BOOL CreateObjectMob( int mobNum, int mapNum, int posX, int posY, int angle );

#endif
