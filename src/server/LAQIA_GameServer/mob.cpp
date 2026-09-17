#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "player.h"
#include "player_world.h"
#include "item.h"
#include "mob.h"
#include "mob_world.h"

#include "world.h"
#include "combat.h"

#include "message.h"
#include "data.h"

#include "quest.h"
#include "party.h"

/*
 *	G L O B A L   V A R I A B L E S
 */
sMOBS		gMOBS;

sPMOB_INFO	g_MOBINFO[dMAX_MOB_INFO];
sPMOB_RESET g_MOBRESET = NULL;


/*
 *	InitMobs
 */
BOOL InitMobs()
{
	if( !LoadMobInfo() )
		return 0;

	if( !LoadMobReset() )
	{
		log( "LoadMobReset : process failed\r\n" );
		return 0;
	}
	
	if( !LoadMobItems() )
	{
		log( "LoadMobReset : process failed\r\n" );
		return 0;
	}

	return 1;
}

/*
 *	LoadMobInfo
 */
BOOL LoadMobInfo()
{
	for( int i = 0; i < dMAX_MOB_INFO; i++ )
	{
		g_MOBINFO[i] = NULL;
	}

	CLoadDataFile cLoad;

	if( !cLoad.Load( dMOB_DATA_TABLE ) )
	{
		log( "LoadMobInfo : file not found\r\n" );
		return 0;
	}

	int		mobNum;			//몹번호	1	A
	char	name[128];				//이름		2	B
	char	ename[128];				//영문이름	3	C
	int		level;				//레벨		4	D
	int		damage;			//공격력	5	E
	int		armor;				//방어력	6	F
	int		hp;				//HP		7	G
	int		mp;				//MP		8	H
	WORD	moveSpeed;				//이동속도	9	I
	WORD	attackRange;				//공격가능거리	10	J
	WORD	attackSpeed;				//공격속도	11	K
	int		visionOfRange;		//시야		12	L
	int		attackType;			//공격성향	13	M
	int		exp;				//경험치	14	N
	int		money;				//돈		15	O
	int		moneyRnd;			//돈 확률	16	P
	WORD	linkNum;				//링크번호	17	Q
	int		attr;				//속성		18	R
	int		str;				//STR		19	S
	int		con;				//CON		20	T
	int		dex;				//DEX		21	U
	int		intelli;			//INT		22	V
	int		wiz;				//WIZ		23	W
	int		luk;				//LUK		24	X
	int		useSkill1;			//사용스킬1	25	Y
	int		useSkill2;			//사용스킬2	26	Z
	int		useSkill3;			//사용스킬3	27	AA
	int		criticalRnd;			//크리티컬확률	28	AB
	WORD	size;					//몬스터 크기	29	AC
	int		type;				//몬스터 타입	30	AD
	BYTE	actionType;				//공격타입	31	AE

	//12월 28일 추가
	int		country;			//국가		32	AF
	int		dropItemMin;			//드랍아이템 최소 개수	33	AG
	int		dropItemMax;			//드랍아이템 최대 개수	34	AH

	char tmp[128];

	while( 1 )
	{
		BYTE btRes = cLoad.GetLine();

		if( btRes == CLoadDataFile::IS_EOF )
			break;
		
		if( btRes == CLoadDataFile::IS_SKIP )
			continue;

		mobNum = cLoad.Get();					// 몹번호 
		cLoad.Get( name );						// 이름
		cLoad.Get( ename );						// 영문이름
		level = cLoad.Get();						// 레벨
		damage = cLoad.Get();					// 공격력
		armor = cLoad.Get();						// 방어력
		hp = cLoad.Get();						// HP
		mp = cLoad.Get();						// MP
		moveSpeed = cLoad.Get();					// 이동속도
		attackRange = cLoad.Get();					// 공격가능거리
		attackSpeed = cLoad.Get();					// 공격속도
		visionOfRange = cLoad.Get();				// 시야 
		attackType = cLoad.Get();					// 공격성향
		exp = cLoad.Get();						// 경험치 
		money = cLoad.Get();						// 돈 
		cLoad.Get( tmp );
		moneyRnd = (int)( atof( tmp ) * 20.0F );	// 돈 확률 
		linkNum = cLoad.Get();						// 링크번호
		attr = cLoad.Get();							// 속성 
		str = cLoad.Get();							// STR
		con = cLoad.Get();							// CON
		dex = cLoad.Get();							// DEX
		intelli = cLoad.Get();						// INT
		wiz = cLoad.Get();							// WIZ
		luk = cLoad.Get();							// LUK
		useSkill1 = cLoad.Get();					// 사용스킬1
		useSkill2 = cLoad.Get();					// 사용스킬2
		useSkill3 = cLoad.Get();					// 사용스킬3
		cLoad.Get( tmp );
		criticalRnd = (int)( atof( tmp ) * 20.0F );	// 크리티컬 확률 
		size = cLoad.Get();							// 몬스터 크기 
		type = cLoad.Get();							// 몬스터 타입
		actionType = cLoad.Get();					// 공격타입

		////
		country = cLoad.Get();						//국가
		dropItemMin = cLoad.Get();					//드랍최소
		dropItemMax = cLoad.Get();					//드랍최대

		if( size >= dMAX_MOBSIZE )
		{
			log( "LoadMobInfo : Bad Mob Size\r\n" );
			return 0;
		}

		if( !g_MOBINFO[mobNum] )
		{
			g_MOBINFO[mobNum] = (sPMOB_INFO)malloc(sizeof(sMOB_INFO));

			if( !g_MOBINFO[mobNum] )
			{
				log( "LoadMobInfo : !g_MOBINFO[mobNum]\r\n" );
				return 0;
			}
		}

		if( mobNum >= dMAX_MOB_INFO )
		{
			log( "LoadMobInfo : mobNum >= dMAX_MOB_INFO\r\n" );
			return 0;
		}

		if( strlen( name ) >= dMOB_NAME_LEN )
		{
			log( "LoadMobInfo : Mob name length over [%s]\r\n", name );
			return 0;
		}

		g_MOBINFO[mobNum]->mobNum = mobNum;				// 몹번호 
		strcpy( g_MOBINFO[mobNum]->name, name );				// 이름
		g_MOBINFO[mobNum]->level = level;					// 레벨
		g_MOBINFO[mobNum]->damage = damage;				// 공격력
		g_MOBINFO[mobNum]->armor = armor;					// 방어력
		g_MOBINFO[mobNum]->hp = hp;						// HP
		g_MOBINFO[mobNum]->mp = mp;						// MP
		g_MOBINFO[mobNum]->moveSpeed = moveSpeed;				// 이동속도
		g_MOBINFO[mobNum]->attackRange = attackRange;			// 공격가능거리
		g_MOBINFO[mobNum]->attackSpeed = attackSpeed;			// 공격속도
		g_MOBINFO[mobNum]->visionOfRange = visionOfRange;		// 시야 
		g_MOBINFO[mobNum]->attackType = attackType;			// 공격성향
		g_MOBINFO[mobNum]->exp = exp;					// 경험치 
		g_MOBINFO[mobNum]->money = money;					// 돈 
		g_MOBINFO[mobNum]->moneyRnd = moneyRnd;				// 돈 확률 
		g_MOBINFO[mobNum]->linkNum = linkNum;				// 링크번호
		g_MOBINFO[mobNum]->attr = attr;					// 속성 
		g_MOBINFO[mobNum]->str = str;					// STR
		g_MOBINFO[mobNum]->con = con;					// CON
		g_MOBINFO[mobNum]->dex = dex;					// DEX
		g_MOBINFO[mobNum]->intelli = intelli;				// INT
		g_MOBINFO[mobNum]->wiz = wiz;					// WIZ
		g_MOBINFO[mobNum]->luk = luk;					// LUK
		g_MOBINFO[mobNum]->useSkill1 = useSkill1;				// 사용스킬1
		g_MOBINFO[mobNum]->useSkill2 = useSkill2;				// 사용스킬2
		g_MOBINFO[mobNum]->useSkill3 = useSkill3;				// 사용스킬3
		g_MOBINFO[mobNum]->criticalRnd = criticalRnd;			// 크리티컬 확률 
		g_MOBINFO[mobNum]->size = size;					// 몬스터 크기 
		g_MOBINFO[mobNum]->type = type;					// 몬스터 타입
		g_MOBINFO[mobNum]->actionType = actionType;			// 공격타입

		g_MOBINFO[mobNum]->country = country;				//국가
		g_MOBINFO[mobNum]->dropItemMin = dropItemMin;			//드랍최소
		g_MOBINFO[mobNum]->dropItemMax = dropItemMax;			//드랍최대

		g_MOBINFO[mobNum]->dropItems = NULL;
		g_MOBINFO[mobNum]->currDropItem = NULL;
		g_MOBINFO[mobNum]->questDropItem = NULL;
	}

	return 1;
}

/*
 *	LoadMobReset
 */
BOOL LoadMobReset()
{
	CLoadDataFile cLoad;

	if( !cLoad.Load( dMOB_REET_TABLE ) )
		return 0;

	int mobNum;
	int posX;
	int posY;
	int responCnt;
	int responDelay;
	int mapNum;

	while( 1 )
	{
		BYTE res = cLoad.GetLine();

		if( res == CLoadDataFile::IS_EOF )
			break;

		if( res == CLoadDataFile::IS_SKIP )
			continue;

		mobNum = cLoad.Get();
		posX = cLoad.Get();
		posY = cLoad.Get();
		responCnt = cLoad.Get();
		responDelay = cLoad.Get();
		mapNum = cLoad.Get();

		if( !IS_VALID_WORLD( mapNum ) )
		{
			log( "LoadMobReset : bad mapNum\r\n" );
			return 0;
		}

		if( posX < 0 || posX >= dONE_MAP_SIZE || posY < 0 || posY >= dONE_MAP_SIZE )
		{
			log( "LoadMobReset : bad position[%d]\r\n", mobNum );
			continue;
		}

		if( gWORLD[mapNum]->mobAttr[posY][posX] != 0 )
		{
			log( "LoadMobReset : Bad point %d/%d/%d\r\n", mapNum, posX, posY );
			continue;
		}


		if( !IS_VALID_MOB( mobNum ) )
		{
			log( "LoadMobReset : bad mobNum\r\n" );
			return 0;
		}

		sPMOB_RESET pNewData = (sPMOB_RESET)malloc(sizeof( sMOB_RESET ));

		if( !pNewData )
		{
			log( "LoadMobReset : malloc failed\r\n" );
			return 0;
		}

		pNewData->mapNum = mapNum;				// 맵번호 
		pNewData->mobNum = mobNum;				// 몹번호 
		pNewData->posX = posX;				// X좌표 
		pNewData->posY = posY;				// Y좌표
		pNewData->maxCnt = responCnt;			// 리스폰수
		pNewData->currCnt = 0;				// 현재 몬스터수
		pNewData->responDelay = responDelay;	// 리스폰 타이밍
		pNewData->responTimer = g_CurrRealTime + pNewData->responDelay;		// 리스폰 처리 시간
		
		pNewData->origPosX = posX;
		pNewData->origPosY = posY;

		pNewData->moveCheckTime = g_CurrRealTime;

		pNewData->moveDir = number( 0, 7 );

		pNewData->prev = NULL;
		pNewData->next = NULL;

		INSERT_TO_LIST( g_MOBRESET, pNewData, prev, next );
	}
	
	return 1;
}

/*
 *	LoadMobItems
 */
BOOL LoadMobItems()
{
	CLoadDataFile cLoad;

	if( !cLoad.Load( dMOB_ITEMS_TABLE ) )
		return 0;
	
	int		mobNum;			// 몹 번호 
	int		itemNum;			// 아이템 번호 
	int		itemAttr;			// 오행속성
	int		optionAttr;			// 옵션속성 
	int		exAttr[5];			// 추가속성 
	int		exValue[5];			// 추가속성값 
	int		dropRnd;			// 드랍확률 
	BYTE	isMajor;			// 주력사용여부 
	WORD	setNum;				// 셋트 번호 
	int		optionRnd;			// 옵션붙을 확률 
	char	tmp[128];

	while( 1 )
	{
		BYTE res = cLoad.GetLine();

		if( res == CLoadDataFile::IS_EOF )
			break;

		if( res == CLoadDataFile::IS_SKIP )
			continue;

		mobNum = cLoad.Get();			
		itemNum = cLoad.Get();
		itemAttr = cLoad.Get();
		optionAttr = cLoad.Get();
		exAttr[0] = cLoad.Get();
		exValue[0] = cLoad.Get();
		exAttr[1] = cLoad.Get();
		exValue[1] = cLoad.Get();
		exAttr[2] = cLoad.Get();
		exValue[2] = cLoad.Get();
		exAttr[3] = cLoad.Get();
		exValue[3] = cLoad.Get();
		exAttr[4] = cLoad.Get();
		exValue[4] = cLoad.Get();
		cLoad.Get( tmp );
		dropRnd = (int)( atof( tmp ) * 20.0F );
		isMajor = cLoad.Get();
		setNum = cLoad.Get();
		cLoad.Get( tmp );
		optionRnd = (int)( atof( tmp ) * 20.0F );

		if( !IS_VALID_MOB( mobNum ) )
			continue;

		if( !IS_VALID_ITEM( itemNum ) )
			continue;

		sPMOB_ITEMS pNewData = (sPMOB_ITEMS)malloc( sizeof( sMOB_ITEMS) );

		if( !pNewData )
		{
			log( "LoadMobItems : malloc failed\r\n" );
			return 0;
		}

		pNewData->mobNum = mobNum;
		pNewData->itemNum = itemNum;
		pNewData->itemAttr = itemAttr;
		pNewData->optionAttr = optionAttr;
		pNewData->exAttr[0] = exAttr[0];
		pNewData->exValue[0] = exValue[0];
		pNewData->exAttr[1] = exAttr[1];
		pNewData->exValue[1] = exValue[1];
		pNewData->exAttr[2] = exAttr[2];
		pNewData->exValue[2] = exValue[2];
		pNewData->exAttr[3] = exAttr[3];
		pNewData->exValue[3] = exValue[3];
		pNewData->exAttr[4] = exAttr[4];
		pNewData->exValue[4] = exValue[4];
		pNewData->dropRnd = dropRnd;
		pNewData->isMajor = isMajor;
		pNewData->setNum = setNum;
		pNewData->optionRnd = optionRnd;

		pNewData->prev = NULL;
		pNewData->next = NULL;

		INSERT_TO_LIST( g_MOBINFO[mobNum]->dropItems, pNewData, prev, next );
		g_MOBINFO[mobNum]->currDropItem = pNewData;
	}
	return 1;
}

/*
 *	MoveMobResetPulse
 */
void MoveMobResetPulse()
{
	sPMOB_RESET reset, next_reset;

	LIST_WHILE( g_MOBRESET, reset, next_reset, next );

	if( reset->moveCheckTime + 180 <= g_CurrRealTime )
	{
		reset->moveCheckTime = g_CurrRealTime;
		reset->posX = reset->origPosX;
		reset->posY = reset->origPosY;
		LIST_SKIP();
	}

	if( number( 1, 100 ) == 1 )
		reset->moveDir = number( 0, 7 );
	
	int posX = reset->posX;
	int posY = reset->posY;
	
	switch( reset->moveDir )
	{
	case 0:
		posY+=1;
		break;
	case 1:
		posY+=1;
		posX+=1;
		break;
	case 2:
		posX+=1;
		break;
	case 3:
		posY-=1;
		posX+=1;
		break;
	case 4:
		posY-=1;
		break;
	case 5:
		posY-=1;
		posX-=1;
		break;
	case 6:
		posX-=1;
		break;
	case 7:
		posY+=1;
		posX-=1;
		break;
	}

	if( posX >= 0 && posX < dONE_MAP_SIZE && posY >= 0 && posY < dONE_MAP_SIZE )
	{
		if( !IS_SET( gWORLD[reset->mapNum]->mobAttr[posY][posX], dMOB_MAP_BLOCK ) )
		{
			reset->posX = posX;
			reset->posY = posY;
		}
		else
		{
			reset->moveDir = number( 0, 7 );
		}
	}

	LIST_WHILEEND();
}


/*
 *	MobResetPulse
 */
void MobResetPulse()
{
	sPMOB_RESET reset, next_reset;

	LIST_WHILE( g_MOBRESET, reset, next_reset, next );

	if( reset->responTimer <= g_CurrRealTime )
	{
		reset->responTimer = g_CurrRealTime + reset->responDelay;

		for( int i = reset->currCnt; i <= MAX( reset->currCnt + 2, reset->maxCnt ); i++ )
		{
			if( reset->currCnt < reset->maxCnt )
			{
				int posX = reset->posX + number( -2, 2 );
				int posY = reset->posY + number( -2, 2 );

				if( /*!gWORLD[reset->mapNum]->player[reset->posY/dONE_TILE][reset->posX/dONE_TILE] &&*/
					!IS_SET( gWORLD[reset->mapNum]->mobAttr[posY][posX], dMOB_MAP_BLOCK ) &&
					!IS_SET( gWORLD[reset->mapNum]->mobAttr[posY][posX], dMOB_MOB_BLOCK ) )
					CreateMob( reset->mobNum, reset->mapNum, posX, posY, reset );
			}
		}
	}

	LIST_WHILEEND();
}

/*
 *	MobResetDirect
 */
void MobResetDirect( int level )
{
	sPMOB_RESET reset, next_reset;

	LIST_WHILE( g_MOBRESET, reset, next_reset, next );

	if( IS_SRC_MOB2( reset->mobNum ) )
		LIST_SKIP();

	if( g_MOBINFO[reset->mobNum]->level > level )
		LIST_SKIP();

	if( /*!gWORLD[reset->mapNum]->player[reset->posY/dONE_TILE][reset->posX/dONE_TILE] &&*/
		!IS_SET( gWORLD[reset->mapNum]->mobAttr[reset->posY][reset->posX], dMOB_MAP_BLOCK ) &&
		!IS_SET( gWORLD[reset->mapNum]->mobAttr[reset->posY][reset->posX], dMOB_MOB_BLOCK ) )
		CreateMob( reset->mobNum, reset->mapNum, reset->posX, reset->posY, reset );

	LIST_WHILEEND();
}

/*
 *	DeleteMobInfo
 */
void DeleteMobInfo()
{
	sPMOB_ITEMS item, next_item;
	sPMOB_QUEST_ITEMS qitem, next_qitem;

	for( int i = 0; i < dMAX_MOB_INFO; i++ )
	{
		if( g_MOBINFO[i] )
		{
			LIST_WHILE( g_MOBINFO[i]->questDropItem, qitem, next_qitem, next );
			REMOVE_FROM_LIST( g_MOBINFO[i]->questDropItem, qitem, prev, next );
			SAFE_FREE( qitem );
			LIST_WHILEEND();

			LIST_WHILE( g_MOBINFO[i]->dropItems, item, next_item, next );
			REMOVE_FROM_LIST( g_MOBINFO[i]->dropItems, item, prev, next );
			SAFE_FREE( item );
			LIST_WHILEEND();

			SAFE_FREE( g_MOBINFO[i] );
		}
	}
}

/*
 *	DestroyMobs
 */
void DestroyMobs()
{
	DeleteMobInfo();
	
	sPMOB_RESET reset, next_reset;
	
	LIST_WHILE( g_MOBRESET, reset, next_reset, next );
	REMOVE_FROM_LIST( g_MOBRESET, reset, prev, next );
	SAFE_FREE( reset );
	LIST_WHILEEND();
}

/*
 *	InitGlobalMob
 */
void InitGlobalMob()
{
	gMOBS.list = NULL;
	gMOBS.memory = NULL;
	gMOBS.total = 0;
	gMOBS.total_memory = 0;
}

/*
 *	InitMobData
 */
void InitMobData( sPMOB_DATA pMob )
{
	pMob->idxNum = 0;				// 몹 고유번호 
	pMob->mobNum = 0;				// 몹 번호 
	
	pMob->mapNum = 0;				// 맵번호 
	pMob->posX = 0;
	pMob->posY = 0;					// 좌표 
	
	pMob->state = 0;
	
	pMob->lastMoveTime = 0;			// 최후 이동시간 
	pMob->lastAttackTime = 0;		// 최후 공격시간
	pMob->attackDelay = 0;			// 공격 딜레이 
	pMob->AICheckTime = 0;			// AI 체크 시간
	pMob->lastAttackDamage = 0;		

	pMob->isDead = 0;
	
	pMob->isAIMob = 0;
	pMob->AiTick = 0;
	
	pMob->deadTimer = 0;
	
	pMob->enemy = NULL;
	
	pMob->moveSpeed = 0;

	pMob->pActionList = NULL;


	int i;

	for( i = 0; i < dMAX_EFFECT_IDX; i++ )
	{
		pMob->skillEffect[i].skillNum = 0;
		pMob->skillEffect[i].duration = 0;
		pMob->skillEffect[i].value = 0;
	}

	for( i = 0; i < dEFF_VALUE_MAX; i++ )
	{
		pMob->skillPoint[i] = 0;
	}
	
	pMob->agroLastVict = 0;
	pMob->agroTimer = 0;
	
	// 전체 리스트용 ( AI )
	pMob->prev = NULL;
	pMob->next = NULL;
	
	// 월드 섹터 리스트용
	pMob->w_prev = NULL;
	pMob->w_next = NULL;
}


/*
 *	CreateMob
 */
BOOL CreateMob( int mobNum, int mapNum, int posX, int posY, sPMOB_RESET pCreater )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return 0;

	if( !IS_VALID_MOB( mobNum ) )
		return 0;

	sPMOB_DATA pNewMob = NULL;

	GET_MEMORY( gMOBS.memory, pNewMob, sMOB_DATA, prev, next );

	if( !pNewMob )
		return 0;

	InitMobData( pNewMob );

	gWORLD[mapNum]->mobIdxCnt++;
	pNewMob->idxNum = gWORLD[mapNum]->mobIdxCnt;

	if( gWORLD[mapNum]->mobIdxCnt > 999999999 )
		gWORLD[mapNum]->mobIdxCnt = 1;

	//
	pNewMob->mobNum = mobNum;
	pNewMob->mapNum = mapNum;											// 맵 번호
	
	pNewMob->hp = g_MOBINFO[mobNum]->hp; 
	pNewMob->maxHp = g_MOBINFO[mobNum]->hp;
	pNewMob->mp = g_MOBINFO[mobNum]->mp;
	pNewMob->maxMp = g_MOBINFO[mobNum]->mp;

	pNewMob->posX = posX;
	pNewMob->posY = posY;												// x , y 
	
	pNewMob->moveDir = number( 0 , 7 );									// 이동 방향
					
	pNewMob->state = dMON_STATE_NORMAL;									// 몹 상태

	pNewMob->AICheckTime = g_CurrTime;

	pNewMob->moveSpeed = (int)( 600 / MIN( GET_MOB_MOVESPEED( pNewMob ), 1 ) );
	pNewMob->moveSpeed2 = (int)( 600 / ( MIN( GET_MOB_MOVESPEED( pNewMob ), 1 ) + 2 ) );

	pNewMob->lastMoveTime = g_CurrTime;
	pNewMob->lastAttackTime = g_CurrTime;
	
	pNewMob->attackDelay = 2000;

	pNewMob->blockCnt++;		

	pNewMob->isDead = 0;										// 죽언니 사란니

	pNewMob->isAIMob = 1;										// AI 몬스터 인지
	pNewMob->AiTick = 0;										// AI Tick

	//
	pNewMob->next = NULL;
	pNewMob->prev = NULL;

	pNewMob->w_next = NULL;
	pNewMob->w_prev = NULL;

	pNewMob->enemy = NULL;

	pNewMob->deadTimer = 0;

	pNewMob->attackRange = 5;

	if( pCreater )
	{
		pCreater->currCnt++;
		pNewMob->pCreater = pCreater;
	}
	else
		pNewMob->pCreater = NULL;
	
	INSERT_TO_LIST( gMOBS.list, pNewMob, prev, next );

	gMOBS.total++;
	INSERT_TO_LIST( gWORLD[mapNum]->mob[posY/dONE_TILE][posX/dONE_TILE], pNewMob, w_prev, w_next );
	
	//
	if( !IS_SET( gWORLD[mapNum]->mobAttr[posY][posX], dMOB_MOB_BLOCK ) )
		SET_BIT( gWORLD[mapNum]->mobAttr[posY][posX], dMOB_MOB_BLOCK );

	gWORLD[mapNum]->currMobCnt++;
//	gWORLD[mapNum]->MobCnt[mobNum]++;

	MakeMobInPacket( pNewMob );
	SendToMobArea( pNewMob, g_Packet, g_nPos );
	
	return 1;
}

/*
 *	DeleteMobData
 */
void DeleteMobData( sPMOB_DATA pMob )
{
	if( pMob->state == dMON_STATE_UNUSED )
	{
		log( "DeleteMobData : pMob->state == dMON_STATE_UNUSED\r\n" );
		return;
	}

	RemoveAllAction( pMob );

	if( pMob->pCreater )
	{
		pMob->pCreater->currCnt--;
		pMob->pCreater = NULL;
	}

	gWORLD[pMob->mapNum]->currMobCnt--;
	
	REMOVE_FROM_LIST( gWORLD[pMob->mapNum]->mob[pMob->posY/dONE_TILE][pMob->posX/dONE_TILE], pMob, w_prev, w_next );

	if( IS_SET( gWORLD[pMob->mapNum]->mobAttr[pMob->posY][pMob->posX], dMOB_MOB_BLOCK ) )
		REMOVE_BIT( gWORLD[pMob->mapNum]->mobAttr[pMob->posY][pMob->posX], dMOB_MOB_BLOCK );

	if( pMob->isAIMob )
	{
		REMOVE_FROM_LIST( gMOBS.list, pMob, prev, next );
	}

	pMob->state = dMON_STATE_UNUSED;
	pMob->isDead = 1;

	INSERT_TO_LIST( gMOBS.memory, pMob,prev, next );

	gMOBS.total--;
	gMOBS.total_memory++;

}

/*
 *	IsMobAttackRange
 */
BYTE IsMobAttackRange( sPMOB_DATA pMob , sPDESC_DATA pPlayer )
{
	if( GetDistance( pMob->posX, pMob->posY, GET_POSX( pPlayer ), GET_POSY( pPlayer ) ) <= MIN( GET_MOB_ATTACKRANGE( pMob ), 1 ) )
		return 1;

	return 0;
}

/*
 *	MOB_SendHPInfo
 */
void MOB_SendHPInfo( sPMOB_DATA pMob )
{
	PutHeader( dPACKET_MOB_HPINFO );
	PutInteger( pMob->idxNum );
	PutInteger( GET_MOB_HP( pMob ) );
	PutInteger( GET_MOB_MAXHP( pMob ) );
	
	SendToMobRange( pMob, g_Packet, g_nPos, 40 );
}

/*
 *	MOB_SendMPInfo
 */
void MOB_SendMPInfo( sPMOB_DATA pMob )
{
	PutHeader( dPACKET_MOB_MPINFO );
	PutInteger( pMob->idxNum );
	PutInteger( GET_MOB_MP( pMob ) );
	PutInteger( GET_MOB_MAXMP( pMob ) );
	
	SendToMobRange( pMob, g_Packet, g_nPos, 40 );
}

/*
 *	ChangeMobState
 */
void ChangeMobState( sPMOB_DATA pMob )
{
	PutHeader( dPACKET_MOB_STATE );
	PutInteger( pMob->idxNum );

	if( pMob->enemy )
	{
		PutByte( 1 );
		PutInteger( GET_MOB_MOVESPEED( pMob ) + 2 );
	}
	else
	{
		PutByte( 0 );
		PutInteger( GET_MOB_MOVESPEED( pMob ) );
	}

	SendToMobArea( pMob, g_Packet, g_nPos );
}


/*
 *	GetMonsterItem
 */
void GetMonsterItem( sPMOB_DATA pMob )
{
	if( !pMob->enemy )
		return;

	if( !g_MOBINFO[pMob->mobNum]->currDropItem )
		return;

	int ncount = number(GET_MOB_DROP_ITEMMIN(pMob), GET_MOB_DROP_ITEMMAX(pMob));

	//for(int i = GET_MOB_DROP_ITEMMIN(pMob); i< GET_MOB_DROP_ITEMMAX(pMob); i++)
	int randX = 0;
	int randY = 0;
	int count = 0;
	for(int i= 0; i< ncount; i++)
	{
		sPMOB_ITEMS pDropInfo = g_MOBINFO[pMob->mobNum]->currDropItem;

		BYTE	isDrop = 0;

		// 확률 안에 들경우
		if( number( 1, 1000 ) <= pDropInfo->dropRnd )
		{	
			randX = number(randX,count);
			randY = number(randY,count);
			count++;
			sPITEM_DATA pItem = CreateItem( pDropInfo->itemNum );

			if( pItem )
			{
				// 옵션 확률이 붙은 경우
				if( number( 1, 1000 ) <= pDropInfo->optionRnd )
				{
					pItem->attr = pDropInfo->itemAttr;
					pItem->exAttr[0] = pDropInfo->exAttr[0];
					pItem->exValue[0] = pDropInfo->exValue[0];
					pItem->exAttr[1] = pDropInfo->exAttr[1];
					pItem->exValue[1] = pDropInfo->exValue[1];
					pItem->exAttr[2] = pDropInfo->exAttr[2];
					pItem->exValue[2] = pDropInfo->exValue[2];
					pItem->exAttr[3] = pDropInfo->exAttr[3];
					pItem->exValue[3] = pDropInfo->exValue[3];
					pItem->exAttr[4] = pDropInfo->exAttr[4];
					pItem->exValue[4] = pDropInfo->exValue[4];
				}
				pItem->isMajor = pDropInfo->isMajor;
				pItem->setNum = pDropInfo->setNum;

				strcpy( pItem->ownerName, GET_NAME( pMob->enemy ) );
				pItem->ownerCheckTick = g_CurrTime + 10000;

				ItemToMap( pItem, pMob->mapNum, pMob->posX+randX, pMob->posY+randY );

				isDrop = 1;
			}
		}

		// 갱신
		if( !pDropInfo->next )
			g_MOBINFO[pMob->mobNum]->currDropItem = g_MOBINFO[pMob->mobNum]->dropItems;
		else
			g_MOBINFO[pMob->mobNum]->currDropItem = pDropInfo->next;
	}

	//if( !isDrop )
	//{
		if( GET_MOB_MONEY( pMob ) > 0 )
		{
			if( number( 1, 1000 ) <= GET_MOB_MONEYRND( pMob ) )
			{
				if( !GET_PARTY_MASTER( pMob->enemy ) )
				{
					if( GET_MONEY( pMob->enemy ) + GET_MOB_MONEY( pMob ) < dMAX_HAVE_MONEY )
					{
						GET_MONEY( pMob->enemy ) += GET_MOB_MONEY( pMob );
						SendSystemMsg( pMob->enemy, MakeString( "%d냥 습득!", GET_MOB_MONEY( pMob ) ) );
						
						PLAYER_UpdateMoney( pMob->enemy );
						DATA_SendUpdateMoney( pMob->enemy );
					}
				}
				else
				{
					SplitPartyMoney( pMob->enemy, GET_MOB_MONEY( pMob ) );
				}
			}
		}
	//}
}


/*
 *	MOB_PopEffect
 */
void MOB_PopEffect( sPMOB_DATA pMob, BYTE targetType, int targetIdx, int effectNum )
{
	PutHeader( dPACKET_POP_MOBEFFECT );
	PutInteger( pMob->idxNum );
	PutByte( targetType );
	PutInteger( targetIdx );
	PutInteger( effectNum );
//	SendToMobArea( pMob, g_Packet, g_nPos ); 
	SendToMobRange( pMob, g_Packet, g_nPos, 40 );
}


/*
 *	MobPointGain
 */
void MobPointGain( sPMOB_DATA pMob )
{
	if( !pMob )
		return;

	BYTE isUpdateHP = 0;
	BYTE isUpdateMP = 0;

	if( pMob->hp < pMob->maxHp )
	{
		int value = MIN( (int)( pMob->maxHp * 0.03 ), 3 );
		pMob->hp = MAX( pMob->hp + value, pMob->maxHp );
		isUpdateHP = 1;
	}

	if( pMob->mp < pMob->maxMp )
	{
		int value = MIN( (int)( pMob->maxMp * 0.03 ), 3 );
		pMob->mp = MAX( pMob->mp + value, pMob->maxMp );
		isUpdateMP = 1;
	}

	if( pMob->skillPoint[dEFF_VALUE_DELGAINHP] )		// HP 회복력 감소 
	{
		pMob->hp = MIN( pMob->hp - pMob->skillPoint[dEFF_VALUE_DELGAINHP], 0 );
		isUpdateHP = 1;
	}
	if( pMob->skillPoint[dEFF_VALUE_DELUNGAINHP] )		// HP 감소 
	{
		pMob->hp = MIN( pMob->hp - pMob->skillPoint[dEFF_VALUE_DELUNGAINHP], 0 );
		isUpdateHP = 1;
	}

	if( pMob->skillPoint[dEFF_VALUE_DELGAINMP] )		// MP 회복력 감소
	{
		pMob->mp = MIN( pMob->mp - pMob->skillPoint[dEFF_VALUE_DELGAINMP], 0 );
		isUpdateMP = 1;
	}
	if( pMob->skillPoint[dEFF_VALUE_DELUNGAINMP] )		// MP 감소
	{
		pMob->mp = MIN( pMob->mp - pMob->skillPoint[dEFF_VALUE_DELUNGAINMP], 0 );
		isUpdateMP = 1;
	}

	if( isUpdateHP )
		MOB_SendHPInfo( pMob );

	if( isUpdateMP )
		MOB_SendMPInfo( pMob );
}

/*
 *	MOB_SetAgro
 */
void MOB_SetAgro( sPMOB_DATA pMob, sPDESC_DATA pVict )
{
	if( !pMob || !pVict )
		return;

	pMob->agroLastVict = GET_IDX( pVict );
	pMob->agroTimer = g_CurrTime;
}

/*
 *	LoadMobQuestItems
 */
void LoadMobQuestItems()
{
	CLoadDataFile	cLoad;

	if( !cLoad.Load( "./DATA/QUEST_DROP_ITEM.TXT" ) )
		return;

	int     questNum;
	int		mobNum;
	int		itemNum;
	int		itemCnt;
	int		rand;

	while( 1 )
	{
		BYTE res = cLoad.GetLine();

		if( res == CLoadDataFile::IS_EOF )
			break;

		if( res == CLoadDataFile::IS_SKIP )
			continue;

		questNum = cLoad.Get();
		mobNum = cLoad.Get();
		itemNum = cLoad.Get();
		itemCnt = cLoad.Get();
		rand = cLoad.Get();

		if( !IS_VALID_MOB( mobNum ) )
			continue;

		if( !IS_VALID_ITEM( itemNum ) )
			continue;

		sPMOB_QUEST_ITEMS pData = (sPMOB_QUEST_ITEMS)malloc(sizeof(sMOB_QUEST_ITEMS));

		if( !pData )
		{
			log( "LoadMobQuestItems : malloc failed\r\n" );
			continue;
		}

		pData->itemNum = itemNum;
		pData->questNum = questNum;
		pData->rand = rand;
		
		pData->prev = NULL;
		pData->next = NULL;

		INSERT_TO_LIST( g_MOBINFO[mobNum]->questDropItem, pData, prev, next );
	}
}

/*
 *	GetMobQuestItem	
 */
void GetMobQuestItem( sPMOB_DATA pMob, sPDESC_DATA pPlayer )
{
	if( !pMob || !pPlayer )
		return;
	
	sPMOB_QUEST_ITEMS qitem, next_qitem;

	LIST_WHILE( g_MOBINFO[pMob->mobNum]->questDropItem, qitem, next_qitem, next );
	if( GET_QUEST_STATE( pPlayer, qitem->questNum ) == dQUEST_STATE_BEING )
	{
		if( number( 1, 100 ) <= qitem->rand )
		{
			sCHECK_INVEN cInven;

			GetEmptyInventory( pPlayer, &cInven );

			if( cInven.nArray == -1 )
				return;

			sPITEM_DATA pItem = CreateItem( qitem->itemNum );

			if( pItem )
			{
				ItemToInventory( pPlayer, pItem, cInven.btInvenPos, cInven.nArray );
				ItemToDB( pPlayer, pItem );
				SendSystemMsg( pPlayer, MakeString( "'%s' 아이템 습득!", GET_ITEM_HNAME( pItem ) ) );
				return;
			}
		}
	}
	LIST_WHILEEND();
}

/*
 *	MobItemCollection
 */
void MobItemCollection( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	if( GET_DEAD_STATE( pPlayer ) )
		return;
	
	if( GetDistance( GET_POSX( pPlayer ), GET_POSY( pPlayer ), pMob->posX, pMob->posY ) > GET_ATTACKRANGE( pPlayer ) + 4 )
		return;
	
	if( pMob->isDead )
		return;

	sPITEM_DATA pItem = GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON );

	if( !pItem )
		return;

	int skillNum = 101;

	if( pItem->itemNum == dCOLLECTION_ITEM_STONE )
		skillNum = 101;
	else if( pItem->itemNum == dCOLLECTION_ITEM_WOOD )
		skillNum = 100;
	else 
		return;

	PutHeader( dPACKET_SKILL_MOTION );
	PutInteger( GET_IDX( pPlayer ) );
	PutInteger( skillNum );
	PutByte( dMOB );
	PutInteger( pMob->idxNum );
	PutInteger( 1000 );
//	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
	SendToRange( pPlayer, g_Packet, g_nPos, 40, dSENDMODE_TOALL );

	/*
	 *	아이템 얻기 
	 */


	
	sPMOB_ITEMS item, next_item;
	bool bcontainCheck = false;
	
	LIST_WHILE( g_MOBINFO[pMob->mobNum]->dropItems, item, next_item, next );
	if( number( 1, 1000 ) <= item->dropRnd )
	{	
		sPITEM_DATA pItem = CreateItem( item->itemNum );

		if( pItem )
		{
			if( IS_STACK_ITEM( pItem->itemNum ) )
			{
				sPITEM_DATA pOldItem = FindStackItem(pPlayer,pItem);

				if(pOldItem)
				{
					pOldItem->containCnt += MIN( pItem->containCnt, 1 );
					ITEM_UpdateItemCnt( pPlayer, pOldItem );
					SendSystemMsg( pPlayer, MakeString( "%s 아이템 습득!", pItem->name ) );
					ItemToDB( pPlayer, pItem );
					bcontainCheck = true;
				}
			}
			break;
		}
	}
	else bcontainCheck = true;
	LIST_WHILEEND();
		

	sCHECK_INVEN cInven;

	GetEmptyInventory( pPlayer, &cInven );


	// 인벤토리 공간이 있으면
	if( cInven.nArray != -1)
	{
		sPMOB_ITEMS item, next_item;

		LIST_WHILE( g_MOBINFO[pMob->mobNum]->dropItems, item, next_item, next );
		if( number( 1, 1000 ) <= item->dropRnd )
		{	
			sPITEM_DATA pItem = CreateItem( item->itemNum );

			if( pItem )
			{
				ItemToInventory( pPlayer, pItem, cInven.btInvenPos, cInven.nArray );
				ItemToDB( pPlayer, pItem );

				SendSystemMsg( pPlayer, MakeString( "Acquire %s items!", pItem->name ) );
				bcontainCheck = true;
				break;
			}
		}
		else bcontainCheck = true;
		LIST_WHILEEND();
	}

	if(bcontainCheck == false)
	{
		sPMOB_ITEMS item, next_item;

		LIST_WHILE( g_MOBINFO[pMob->mobNum]->dropItems, item, next_item, next );
		if( number( 1, 1000 ) <= item->dropRnd )
		{	
			sPITEM_DATA pItem = CreateItem( item->itemNum );

			if( pItem )
			{
				ItemToMap( pItem, pMob->mapNum, pMob->posX, pMob->posY );
				SendSystemMsg( pPlayer, "Insufficient inventory space." );
			}
		}
		LIST_WHILEEND();
	}
	/**/

//	GetMonsterItem( pMob );

	// 몬스터 없애기
	pMob->hp--;

	if( pMob->hp <= 0 )
	{
		SendSystemMsg( pPlayer, MakeString( "You have gained %d experience.", GET_MOB_EXP( pMob ) ) );
		GET_EXP( pPlayer ) += GET_MOB_EXP( pMob );
		PLAYER_UpdateExp( pPlayer );
		DATA_SendUpdateExp( pPlayer );
		CheckLevelUp( pPlayer );
		
		MonsterDIE( pMob );
	}
}

/*
 *	
 */
void SetMobEnemy( sPMOB_DATA pMob, sPDESC_DATA pVict )
{
	if( pMob->enemy )
		GET_ENEMY_CNT( pMob->enemy ) = MIN( GET_ENEMY_CNT( pMob->enemy ) - 1, 0 );
	if( pVict != NULL )
		GET_ENEMY_CNT( pVict ) = MAX( GET_ENEMY_CNT( pVict ) + 1, dMAX_AGRO_ENEMY + 1 );
	pMob->enemy = pVict;
	pMob->lastMoveTime = g_CurrTime;
	pMob->lastAttackDamage = 0;
}

//EOF


