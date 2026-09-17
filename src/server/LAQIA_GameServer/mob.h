#ifndef __MOB_H__
#define __MOB_H__

/*
 *	D E F I N E S
 */
#define dMOB_DATA_TABLE			"./DATA/MOB_DATA.TXT"
#define dMOB_REET_TABLE			"./DATA/MOB_RESET.TXT"
#define dMOB_ITEMS_TABLE			"./DATA/MOB_ITEMS.TXT"

#define dDEF_MOB_AITICK				50
#define dMAX_MOB_BLOCKOVER				50

#define dMON_STATE_UNUSED				1		// 사용 않하고 있는 데이터
#define dMON_STATE_NORMAL				2		// 일반 상태
#define dMON_STATE_FIGHT				3		// 전투중

#define MOVABLE_OK					1
#define MOVABLE_BLOCKMOB				2
#define MOVABLE_BLOCKPLAYER				3
#define MOVABLE_BLOCKMAP				4
#define MOVABLE_BLOCKEXC				5

#define dMOB_ACTIONTYPE_CLOSERANGE			0		// 근거리
#define dMOB_ACTIONTYPE_LONGRANGE			1		// 원거리

#define dMAX_AGRO_ENEMY				15


#define SET_MOB_ENEMY( mob, value )  SetMobEnemy( mob, value )

#define IS_VALID_MOB( num ) (g_MOBINFO[num])

#define GET_MOB_IDX( p )			(p->idxNum)
#define GET_MOB_NUM( p )			(p->mobNum)
#define GET_MOB_HP(p)			(p->hp)
#define GET_MOB_MAXHP(p)			(p->maxHp)
#define GET_MOB_MP(p)			(p->mp)
#define GET_MOB_MAXMP(p)			(p->maxMp)

#define GET_MOB_LEVEL(p)			(g_MOBINFO[p->mobNum]->level)		// 레벨
#define GET_MOB_DAMAGE(p)			(g_MOBINFO[p->mobNum]->damage)		// 데미지
#define GET_MOB_ARMOR(p)			(g_MOBINFO[p->mobNum]->armor)		// 방어력
#define GET_MOB_MOVESPEED(p)		(g_MOBINFO[p->mobNum]->moveSpeed)		// 이동속력
#define GET_MOB_ATTACKRANGE(p)		(g_MOBINFO[p->mobNum]->attackRange)	// 공격가능거리
#define GET_MOB_ATTACKSPEED(p)		(g_MOBINFO[p->mobNum]->attackSpeed)	// 공속
#define GET_MOB_VISIONOFRANGE(p)		(g_MOBINFO[p->mobNum]->visionOfRange)	// 시야
#define GET_MOB_ATTACKTYPE(p)		(g_MOBINFO[p->mobNum]->attackType)		// 공격성향
#define GET_MOB_EXP(p)			(g_MOBINFO[p->mobNum]->exp)			// 경험치
#define GET_MOB_MONEY(p)			(g_MOBINFO[p->mobNum]->money)		// 소유금액
#define GET_MOB_MONEYRND(p)			(g_MOBINFO[p->mobNum]->moneyRnd)		// 돈 랜덤
#define GET_MOB_LINKNUM(p)			(g_MOBINFO[p->mobNum]->linkNum)		// 링크번호
#define GET_MOB_ATTR(p)			(g_MOBINFO[p->mobNum]->attr)		// 속성
#define GET_MOB_STR(p)			(g_MOBINFO[p->mobNum]->str)			// STR
#define GET_MOB_CON(p)			(g_MOBINFO[p->mobNum]->con)			// CON
#define GET_MOB_DEX(p)			(g_MOBINFO[p->mobNum]->dex)			// DEX
#define GET_MOB_INT(p)			(g_MOBINFO[p->mobNum]->intelli)		// INT
#define GET_MOB_WIZ(p)			(g_MOBINFO[p->mobNum]->wiz)			// WIZ
#define GET_MOB_LUK(p)			(g_MOBINFO[p->mobNum]->luk)			// LUK
#define GET_MOB_USESKILL1(p)		(g_MOBINFO[p->mobNum]->useSkill1)		// 사용스킬1
#define GET_MOB_USESKILL2(p)		(g_MOBINFO[p->mobNum]->useSkill2)		// 사용스킬2
#define GET_MOB_USESKILL3(p)		(g_MOBINFO[p->mobNum]->useSkill3)		// 사용스킬3
#define GET_MOB_CRITICALRND(p)		(g_MOBINFO[p->mobNum]->criticalRnd)	// 크리티컬 확률 
#define GET_MOB_SIZE(p)			(g_MOBINFO[p->mobNum]->size)		// 몬스터 크기 
#define GET_MOB_TYPE(p)			(g_MOBINFO[p->mobNum]->type)		// 몬스터 타입
#define GET_MOB_ACTIONTYPE(p)		(g_MOBINFO[p->mobNum]->actionType)		// 공격타입
#define GET_MOB_NAME(p)			(g_MOBINFO[p->mobNum]->name)		// 몹이름
#define GET_MOB_NAME2(num)			(g_MOBINFO[num]->name)		
#define GET_MOB_DROP_ITEMMIN(p)		(g_MOBINFO[p->mobNum]->dropItemMin)	//드랍최소
#define GET_MOB_DROP_ITEMMAX(p)		(g_MOBINFO[p->mobNum]->dropItemMax)	//드랍최대

/*
 *	몬스터 크기 
 */
#define dMOB_SIZE_NONE		0
#define dMOB_SIZE_SMALL		1		// 소형
#define dMOB_SIZE_NORMAL		2		// 중형
#define dMOB_SIZE_BIG		3		// 대형

#define dMAX_MOBSIZE			4

/*
 *	몬스터 타입
 */
#define dMOB_TYPE_NONE		0				
#define dMOB_TYPE_2LEG		1		// 2족보행 
#define dMOB_TYPE_4LEG		2		// 4족보행 
#define dMOB_TYPE_FLY		3		// 공중부양

/*
 *	공격 타입
 */
#define dMOB_ATTACKTYPE_NONE	0
#define dMOB_ATTACKTYPE_PEACE	1	// 비선공
#define dMOB_ATTACKTYPE_AGRO	2	// 선공


// 채집용 몬스터 
#define dCOLLECTION_MOB_01		47		// 청대나무
#define dCOLLECTION_MOB_02		48		// 광석 
#define dCOLLECTION_MOB_03		49		// 오동나무 
#define dCOLLECTION_MOB_04		50		// 참나무 
#define dCOLLECTION_MOB_05		51		// 황대나무
#define dCOLLECTION_MOB_06		105		// 
#define dCOLLECTION_MOB_07		106		//
#define dCOLLECTION_MOB_08		107		//
#define dCOLLECTION_MOB_09		108		//
//몬스터 번호 105, 106, 107, 108번 광석 추가.

#define dCOLLECTION_ITEM_WOOD	1146	// 채집용도끼 
#define dCOLLECTION_ITEM_STONE	1147	// 곡괭이

#define dCOLLECTION_SKILL_WOOD	100		// 벌목
#define dCOLLECTION_SKILL_STONE	101		// 채광

#define IS_SRC_MOB( mob ) ( ( mob->mobNum >= 47 && mob->mobNum <= 51 ) || ( mob->mobNum >= 105 && mob->mobNum <= 108 ) )
#define IS_SRC_MOB2( num ) ( ( num >= 47 && num <= 51 ) || ( num >= 105 && num <= 108 ) )

#define IS_SRC_WOOD( mob ) ( mob->mobNum >= 47 && mob->mobNum <= 51 && mob->mobNum != 48 )
#define IS_SRC_STONE( mob ) ( mob->mobNum == 48 || ( mob->mobNum >= 105 && mob->mobNum <= 108 ) )

/*
 *	E X T E R N   V A R I A B L E S
 */
extern sMOBS		gMOBS;

/*
 *	E X T E R N   F U N C T I O N S
 */
BOOL InitMobs();
BOOL LoadMobInfo();
void DeleteMobInfo();
void DestroyMobs();
void InitGlobalMob();

BOOL CreateMob( int mobNum, int mapNum, int posX, int posY, sPMOB_RESET pCreater );
void InitMobData( sPMOB_DATA pMob );

void DeleteMobData( sPMOB_DATA pMob );

BYTE IsMobAttackRange( sPMOB_DATA pMob , sPDESC_DATA pPlayer );

void MOB_SendHPInfo( sPMOB_DATA pMob );
void MOB_SendMPInfo( sPMOB_DATA pMob );

void ChangeMobState( sPMOB_DATA pMob );
BOOL LoadMobReset();
void MobResetPulse();
BOOL LoadMobItems();

void GetMonsterItem( sPMOB_DATA pMob );
void MOB_PopEffect( sPMOB_DATA pMob, BYTE targetType, int targetIdx, int effectNum );

void MobPointGain( sPMOB_DATA pMob );
void MoveMobResetPulse();

void MOB_SetAgro( sPMOB_DATA pMob, sPDESC_DATA pVict );
void GetMobQuestItem( sPMOB_DATA pMob, sPDESC_DATA pPlayer );

void MobItemCollection( sPDESC_DATA pPlayer, sPMOB_DATA pMob );

void MobResetDirect( int level );
void SetMobEnemy( sPMOB_DATA pMob, sPDESC_DATA pVict );

#endif

//EOF


