/**
 *	$Workfile: player.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 33 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/player.h $
 * 
 * 33    04-12-22 5:18p Sk8snow
 * 
 * 32    04-12-15 10:40a Sk8snow
 * 
 * 31    04-09-20 10:53p Sk8snow
 * 
 * 30    04-08-04 9:27a Sk8snow
 * 
 * 29    04-07-13 7:26a Sk8snow
 * 
 * 28    04-05-29 10:12p Sk8snow
 * 
 * 27    04-05-11 1:45a Sk8snow
 * 
 * 26    04-04-30 9:46a Sk8snow
 * 
 * 25    04-04-07 10:17p Sk8snow
 * 
 * 24    04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __PLAYER_H__
#define __PLAYER_H__

////////////////////////////////////////////////////
// D E F I N E S
///////////////////////////////////////////////////

typedef struct 
{
	BYTE	SAttack;
	BYTE	LAttack;
	BYTE	BMagic;
	BYTE	WMagic;
	BYTE	minStr;
	BYTE	maxStr;
	BYTE	minDex;
	BYTE	maxDex;
	BYTE	minInt;
	BYTE	maxInt;
} sSTATE_MPHP_TABLE, *sPSTATE_MPHP_TABLE;

typedef struct {
	BYTE r;
	BYTE g;
	BYTE b;
} sNAME_COLOR, *sPNAME_COLOR;

#define dPVPMODE_NONE			0			// PK모드 없음 
#define dPVPMODE_FULL			1			// 아무나 PK
#define dPVPMODE_GUILD			2			// 길드PK
#define dPVPMODE_PARTY			3			// 파티PK

#define dBADUSER_TYPE_NONE			0			// NONE
#define dBADUSER_TYPE_DISCON		1			// 접속불가 
#define dBADUSER_TYPE_NOCHAT		2			// 채팅불가
#define dBADUSER_TYPE_NOLOADING		3			// 로딩불가 
#define dBADUSER_TYPE_NOSHOP		4			// 상점불가
#define dBADUSER_TYPE_NODEAL		5			// 거래불가
#define dBADUSER_TYPE_NOATTACK		6			// 전투불가

#define dTOTAL_INVEN_SIZE			288

#define dDEF_PLAYER_HP			10

#define dDEF_PLAYER_ATTACKDELAY		4		// 0.3 초 단위 

#define dDEF_DEX	10
#define dDEF_INT	10
#define dDEF_STR	10

#define GET_IDX( p ) (p->sock)
#define GET_ID( p ) (p->ch.id)
#define GET_NAME( p ) (p->ch.name)
#define GET_STATE( p ) (p->status)

#define GET_CLASS( p ) (p->ch.charClass)

#define GET_HP( p ) (p->ch.hp)
#define GET_HPADD( p ) (p->ch2.addHP)

#define GET_MP( p ) (p->ch.mp)
#define GET_MPADD( p ) (p->ch2.addMP)

#define GET_MAX_HP( p ) (p->ch.max_hp)
#define GET_MAX_MP( p ) (p->ch.max_mp )

#define GET_STR( p ) (p->ch.str)
#define GET_STRADD( p ) (p->ch.str_add)
#define GET_STRADD_EX( p ) (p->ch2.addStr)

#define GET_DEX( p ) (p->ch.dex)
#define GET_DEXADD( p ) (p->ch.dex_add)
#define GET_DEXADD_EX( p ) (p->ch2.addDex)

#define GET_INT( p ) (p->ch.intelli)
#define GET_INTADD( p ) (p->ch.intelli_add)
#define GET_INTADD_EX( p ) (p->ch2.addInt)

#define GET_CHA( p ) (p->ch.cha)

#define GET_STR_EXP( p ) (p->ch.str_exp)
#define GET_DEX_EXP( p ) (p->ch.dex_exp)
#define GET_INT_EXP( p ) (p->ch.intelli_exp)

#define GET_STR_TOTAL( p ) (GET_STR(p)+GET_STRADD(p))
#define GET_INT_TOTAL( p ) (GET_INT(p)+GET_INTADD(p))
#define GET_DEX_TOTAL( p ) (GET_DEX(p)+GET_DEXADD(p))

#define GET_USER_ALL_LEVEL( p ) ( GET_SATTACK( p ) + GET_LATTACK( p ) + GET_BMAGIC( p ) + GET_WMAGIC( p ) )  // 모든레벨

#define GET_SATTACK( p ) (p->ch.skill[0])			// 근거리
#define GET_LATTACK( p ) (p->ch.skill[1])			// 원거리
#define GET_BMAGIC( p ) (p->ch.skill[2])			// 흑마법
#define GET_WMAGIC( p ) (p->ch.skill[3])			// 백마법 

#define GET_SATTACK_ADD( p ) (p->ch2.addSAttack)		// 근거리 ADD	
#define GET_LATTACK_ADD( p ) (p->ch2.addLAttack)		// 원거리 ADD
#define GET_BMAGIC_ADD( p ) (p->ch2.addBMagic)		// 흑마법 ADD
#define GET_WMAGIC_ADD( p ) (p->ch2.addWMagic)		// 백마법 ADD

#define GET_SATTACK_EXP( p ) (p->ch.skill_exp[0])	// 근거리
#define GET_LATTACK_EXP( p ) (p->ch.skill_exp[1])	// 원거리
#define GET_BMAGIC_EXP( p ) (p->ch.skill_exp[2])		// 흑마법
#define GET_WMAGIC_EXP( p ) (p->ch.skill_exp[3])		// 백마법 

#define GET_MAP_NUM( p ) (p->ch.mapNum)			// 맵번호 
#define GET_POSX( p ) (p->ch.posX)				// X좌표 
#define GET_POSY( p ) (p->ch.posY)				// Y좌표
#define GET_LAYER( p ) (p->ch.posLayer)			// 레이어 번호
#define GET_MAP_KIND( p ) (p->ch.posMapKind)		// 맵 종류

#define GET_GOTO_MAPNUM( p ) (p->ch2.gotoMapNum)		// 로딩할 맵번호 
#define GET_GOTO_POSX( p ) (p->ch2.gotoPosX)		// 로딩할 X좌표 
#define GET_GOTO_POSY( p ) (p->ch2.gotoPosY)		// 로딩할 Y좌표
#define GET_GOTO_MAPKIND( p ) (p->ch2.gotoPosMapKind)	// 로딩할 맵 종류

#define GET_SUMMON_MOB( p ) (p->ch2.sMob)			// 소환 몬스터
#define GET_SUMMON_MOBLIFE( p ) (p->ch2.sMobLife)	// 소환 몬스터 수명

#define GET_CHAR_POS( p ) (p->ch.charPos)

#define GET_USE_SKILL( p ) (p->ch.useSkill)

#define GET_MONEY( p ) (p->ch.money)
#define GET_CASHMONEY( p ) ( p->ch.cash )

#define GET_GUILD( p ) (p->ch.guildNo)
#define GET_GUILD_LEVEL( p ) (p->ch.guildLevel)
#define GET_TMP_GUILD( p ) (p->ch.tmpGuildNum)
//#define GET_GUILD_POINT( p ) ( p->ch.guildPoint )

#define GET_RIDE_TYPE( p ) (p->ch2.RideNum)
#define GET_RIDE_STATE( p ) (p->ch2.isRide)

#ifdef _dUMENOKOJI_GHOST_GUILD_		//유령 길드 관련 전쟁포인트 테스트
#define SET_PLAYER_TIME( p ) ( p->ch2.getTime = g_CurrRealTime )
#define GET_PLAYER_TIME( p ) ( p->ch2.getTime )
#endif

#ifdef dUSE_AUTO_SHOP
#define GET_CURR_OPENSHOP( p ) (p->ch2.currOpenShop)
#endif

#define GET_DEAD_STATE( p )  (p->isDead)
#define GET_GHOST_TIMER( p ) (p->ch2.ghostTimer)

#define GET_ALL_SKILL( p ) ( GET_SATTACK( p ) + GET_LATTACK( p ) + GET_BMAGIC( p ) + GET_WMAGIC( p ) )

#define GET_EFFECT( p ) (p->ch2.effect)
#define GET_EFFECT_TIME( p, num ) (p->ch2.effect_time[num])
#define GET_EFFECT_IDX( p ) (p->ch2.effectIdx)

#define GET_STATUS_DELAY( p, num ) (p->ch2.status_delay[num])

#define GET_VISIBLE( p ) (p->ch2.isVisible)

#define GET_FRIEND_CNT( p ) (p->ch2.friendCnt)

#define GET_CURRITEM_WEIGHT( p ) (p->currItemWeight)

// PET
#define GET_PET_TYPE( p ) (p->ch2.pet.type)
#define GET_PET_LEVEL( p ) (p->ch2.pet.level)
#define GET_PET_HP( p ) (p->ch2.pet.hp)
#define GET_PET_MP( p ) (p->ch2.pet.mp)
#define GET_PET_EXP( p ) (p->ch2.pet.exp)
#define GET_PET_STATE( p ) (p->ch2.pet.state)
#define GET_PET_ANGERGAGE( p ) (p->ch2.pet.angerGage)
#define GET_PET_NAME( p ) (p->ch2.pet.name)

#define GET_PVPMODE( p ) (p->ch2.btPkMode )
	


////////////////////////////////////////////////////
// D E F I N E S
///////////////////////////////////////////////////
#define dPLAYER_MOVE_RANGE		5		// 플레이어 이동 허용 거리

#define dPLAYER_FLG_NOHIT		(1<<1)		// 운영자용 
#define dPLAYER_FLG_FULLPOWER	(1<<2)		// 운영자용
#define dPLAYER_FLG_DENY_FRIEND	(1<<3)		// 친구거부
#define dPLAYER_FLG_DENY_CHAT	(1<<4)		// 채팅거부
#define dPLAYER_FLG_DENY_WHISPER	(1<<5)		// 귓말거부
#define dPLAYER_FLG_DENY_DEAL	(1<<6)		// 거래거부
#define dPLAYER_FLG_DENY_PARTY	(1<<7)		// 파티거부

#define dDEF_PLAYER_SPEED		40 //플레이어 이동속도

#define dSENDMODE_TOALL		0
#define dSENDMODE_TOOTHER		1
#define dSENDMODE_TOOWNER		2

#define dDEAD_STATE_NONE		0			// 죽은상태 아님 
#define dDEAD_STATE_CORPSE		1			// 시체
#define dDEAD_STATE_GHOST		2			// 유령

#define dGHOST_TIMER_VALUE		30000

#define dCLASS_SWORDMAN		0
#define dCLASS_BOWMAN		1
#define dCLASS_MAGICUSER		2

#define dMAX_PLAYER_ABL_TABLE	40
#define dMAX_PLAYER_ABL_CLASS	17

#define	dCLASS_TYPE_PHYSICS	8		// 캐릭터 물리 속성
#define dCLASS_TYPE_MAGICS		9		// 캐릭터 마법 속성

#define dCLASS_NONE			0		// 무직
#define dCLASS_WARRIOR		1		// 워리어 
#define dCLASS_BANDIT		2		// 밴디츠
#define dCLASS_SHADOWKNIGHT		3		// 샤도우나이츠
#define dCLASS_PALADIN		4		// 팔라딘
#define dCLASS_ASSASSIN		5		// 어쌔씬
#define dCLASS_ARCHER		6		// 아처
#define dCLASS_PORCHER		7		// 포처 
#define dCLASS_RANGER		8		// 레인저
#define dCLASS_BLADEMAGE		9		// 블레이드 메이지
#define dCLASS_WONDERER		10		// 원더러 
#define dCLASS_WIZZARD		11		// 위저드 
#define dCLASS_SUMMON		12		// 샤먼 
#define dCLASS_MONK			13		// 몽크
#define dCLASS_HARK			14		// 호크
#define dCLASS_DRUID			15		// 드루이드
#define dCLASS_CLERIC		16		// 클레릭

#define dMAX_CLASS			17		

#define dCLASS_TYPE_NONE		0		
#define dCLASS_TYPE_WARRIOR		1
#define dCLASS_TYPE_ARCHER		2
#define dCLASS_TYPE_MAGICUSER	3
#define dCLASS_TYPE_ANY		4

#define IS_WARRIOR( charClass ) \
	( ( charClass >= dCLASS_WARRIOR && charClass <= dCLASS_PALADIN ) )

#define IS_ARCHER( charClass ) \
	( ( charClass >= dCLASS_ASSASSIN && charClass <= dCLASS_RANGER ) )

#define IS_MAGICUSER( charClass ) \
	( ( charClass >= dCLASS_BLADEMAGE && charClass <= dCLASS_CLERIC ) )

extern const char g_CLASS_NAME[dMAX_CLASS][64];

#define dSTATE_TABLE_STR		0
#define dSTATE_TABLE_DEX		1
#define dSTATE_TABLE_INT		2		

#define dPOINTUP_STR			0		// 힘
#define dPOINTUP_INT			1		// 지력
#define dPOINTUP_DEX			2		// 민첩

#define dCHANGE_STATUS_STUN		1		// 기절 
#define dCHANGE_STATUS_STUNEND	2		// 기절해제

#define dNAMECOLOR_DEFAULT		0
#define dNAMECOLOR_EVILCHA		1
#define dNAMECOLOR_CAO		2

////////////////////////////////////////////////////
// G L O B A L
////////////////////////////////////////////////////
extern int g_PLAYER_ABL_TABLE[dMAX_PLAYER_ABL_TABLE][dMAX_PLAYER_ABL_CLASS];
extern sSTATE_MPHP_TABLE g_STATE_MPHP_TABLE[2];
extern BYTE g_BASEHIT_TABLE[6][6];
extern BYTE g_EXP_PROTECT[dMAX_EXP_PROTECT];

////////////////////////////////////////////////////
// F U N C S
///////////////////////////////////////////////////
void InitPlayerData( sPDESC_DATA pData, SOCKET sock, char *hostname );
sPDESC_DATA NewClient();
void DisconnectClient( sPDESC_DATA pData );

void MakeInPacket( sPDESC_DATA pPlayer , BYTE isConnect );
void MakeOutPacket( sPDESC_DATA pPlayer , BYTE isDisconnect );
void SendToSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, char *data, int size , BYTE mode );
void SendToArea( sPDESC_DATA pPlayer , char *data, int size , BYTE mode );
void PlayerIntoMap( sPDESC_DATA pPlayer , int mapNum , int posX, int posY , BYTE posMapKind = 0 );
void PlayerFromMap( sPDESC_DATA pPlayer );

void SendToRange( sPDESC_DATA pPlayer, char *data, int size, int range );
void SendToRangeSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, char *data, int size, int range );

BOOL MovePlayer( sPDESC_DATA pPlayer , int posX, int posY , BYTE layer );

void PlayerPointGain( sPDESC_DATA pPlayer );

int GetPlayerItemDepense( sPDESC_DATA pPlayer );				//물리 방어력
int	GetPlayerItemMagicDepense( sPDESC_DATA pPlayer );		//마법 방어력
int GetPlayerMainMap( sPDESC_DATA pPlayer );
sPDESC_DATA FindWaitAllPlayer( char *id );
sPDESC_DATA FindWaitCharPlayer( char *id );
sPDESC_DATA FindWaitDelPlayer( char *id );

sPDESC_DATA FindPlayerNameList( char *name );
sPDESC_DATA FindPlayerIdList( char *id );

void SendToAll( char *data, int size );

void UpdateMoney( sPDESC_DATA pPlayer );
void UpdateCash( sPDESC_DATA pPlayer );

void BroadCastingMsg( char *str );
void BroadCastSystemMsg( char *str );

void GetCurrAreaPlayer( sPDESC_DATA pPlayer );
void OutCurrAreaPlayer( sPDESC_DATA pPlayer );

#ifdef _DUMMY_MODE_
void DummyMove( sPDESC_DATA pPlayer );
void CreateDummy( int mapNum, int posX, int posY, char *name , int idxNum );
#endif

void CheckLoadingPoint( sPDESC_DATA pPlayer );

void SendPlayerHP( sPDESC_DATA pPlayer );
void SendPlayerMP( sPDESC_DATA pPlayer );
void SendPlayerMaxHPMP( sPDESC_DATA pPlayer );

// Find
sPDESC_DATA FindPlayerInTile( sPDESC_DATA list, int idxNum );
sPDESC_DATA FindPlayerInArea( int mapNum, int posX, int posY, int idxNum );
sPDESC_DATA FindPlayerInHouse( sPDESC_DATA pPlayer , int idxNum );

void InitDealData( sPDESC_DATA pPlayer );

void SendPlayerAttackRange( sPDESC_DATA pPlayer );
void SendPlayerMainSkill( sPDESC_DATA pPlayer );

void PARTY_SendMovePacket( sPDESC_DATA pPlayer );

void SendMapLoading( sPDESC_DATA pPlayer, BYTE btLayer = 0 );

void OutCurrHousePlayer( sPDESC_DATA pPlayer );
void GetCurrHousePlayer( sPDESC_DATA pPlayer );

void SendToHouse( sPDESC_DATA pPlayer, char *data, int size , BYTE mode );

void PlayerIntoIndoor( sPDESC_DATA pPlayer, int mapNum, int posX, int posY, BYTE posMapKind );
void PlayerFromIndoor( sPDESC_DATA pPlayer );

void SendRideInfo( sPDESC_DATA pPlayer );
BYTE GetRideSpeed( sPDESC_DATA pPlayer );

void PlayerToHomeTown( sPDESC_DATA pPlayer );
void PlayerToStartPoint( sPDESC_DATA pPlayer , int mapNum = 0 );

DWORD GetAttackDelay( sPDESC_DATA pPlayer );

void GetStartPoint( sPDESC_DATA pPlayer );

BOOL LoadPlayerAbilityTable();
BOOL LoadPlayerHPMPTable();
BOOL LoadBaseHitTable();

void NewCharacterProc( sPDESC_DATA pPlayer );

void SendUpdateCha( sPDESC_DATA pPlayer );

void SendPlayerClass( sPDESC_DATA pPlayer );

void TogglePlayerFlag( sPDESC_DATA pPlayer, int itemNum );

BOOL LoadExpProtectTable();

BOOL IsEquipOk( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

void SetVisible( sPDESC_DATA pPlayer, BYTE btVisible );

int GetCurrSkillLevel( sPDESC_DATA pPlayer );

void GetBadUserType( sPDESC_DATA pPlayer );

void RandomTrans( sPDESC_DATA pPlayer );

void SendPlayerEffect( sPDESC_DATA pPlayer, WORD effect );

void SendPlayerStatus( sPDESC_DATA pPlayer, BYTE status );

void SetPlayerStun( sPDESC_DATA pPlayer );
void UnSetPlayerStun( sPDESC_DATA pPlayer );

void DropItemByCHA( sPDESC_DATA pPlayer );

void AddCurrItemWeight( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void DelCurrItemWeight( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
int GetItemWeight( sPITEM_DATA pItem );

int CalsAllowTotalWeight( sPDESC_DATA pPlayer );
void SendWeightGageInfo( sPDESC_DATA pPlayer );
int GetDealItemsTotalWeight( sPDESC_DATA pPlayer );
BOOL IsValidClassItem( sPDESC_DATA pPlayer, int itemNum );

void SendChangeNameColor( sPDESC_DATA pPlayer, sPNAME_COLOR pColor );
void SendPlayerNameColor( sPDESC_DATA pPlayer );

void GetPlayerNameColor( sPDESC_DATA pPlayer, sPNAME_COLOR pColor );
void GetNameColor( BYTE color, sPNAME_COLOR pColor );

void SendPlayerPvPMode( sPDESC_DATA pPlayer );

BOOL IsPvPMode( sPDESC_DATA pPlayer );
BOOL IsValidPvPTarget( sPDESC_DATA pPlayer, sPDESC_DATA pVict );
BOOL IsPvPArea( sPDESC_DATA pPlayer );

BYTE GetPlayerClass( sPDESC_DATA pPlayer );

extern BOOL	g_tDamage;
extern BOOL	g_tMDamage;

#define SET_tDAMAGE( ) ( g_tDamage = TRUE )
#define GET_tDAMAGE( ) ( g_tDamage )
#define INIT_tDAMGE( ) ( g_tDamage = FALSE )

#define SET_tMDAMAGE(  ) ( g_tMDamage = TRUE )
#define GET_tMDAMAGE(  ) ( g_tMDamage )
#define INIT_tMDAMAGE( ) ( g_tMDamage = FALSE )

#endif

// EOF



