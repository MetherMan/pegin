/**
 *	w
 *	$Workfile: monster.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 37 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/monster.cpp $
 * 
 * 37    04-12-22 5:18p Sk8snow
 * 
 * 36    04-12-15 10:40a Sk8snow
 * 
 * 35    04-10-19 11:14p Sk8snow
 * 
 * 34    04-09-20 10:53p Sk8snow
 * 
 * 33    04-09-11 11:20p Sk8snow
 * 
 * 32    04-09-08 3:54p Sk8snow
 * 
 * 31    04-08-04 9:27a Sk8snow
 * 
 * 30    04-07-13 7:26a Sk8snow
 * 
 * 29    04-05-29 10:12p Sk8snow
 * 
 * 28    04-05-18 1:17a Sk8snow
 * 
 * 27    04-05-11 1:45a Sk8snow
 * 
 * 26    04-04-30 9:46a Sk8snow
 * 
 * 25    04-04-07 10:17p Sk8snow
 * 
 * 24    04-04-03 4:03p Sk8snow
 * 
 * 23    04-03-31 8:22a Sk8snow
 *        
 */

#include "def.h"

#include "structs.h"
#include "extern.h"
#include "message.h"

#include "player.h"
#include "world.h"
#include "monster.h"

#include "fight.h"

#include "db_rows.h"
#include "skill.h"

#include "item.h"

#include "house.h"
#include "effect.h"

#include "npc.h"

sPMOB_DEF_INFO g_MOBINFO[dMAX_MOBINFO];
BYTE g_MOB_MAX_RESET[dMAX_MAP_CNT][dMAX_MOBINFO];

int	g_MobResetTime = 20;//몹리젠  Mob Regen

#ifdef dEVENT_MOB_ON

sEVENT_MOB g_EVENT_MOB[dMAX_EVENT_MOB];

#endif

//
// Function Name: InitMaxMobReset
// Date: 
// Description: 
//
void InitMaxMobReset( int  value )
{
	for( int y = 0; y < dMAX_MAP_CNT; y++ )
	{
		for( int x = 0; x < dMAX_MOBINFO; x++ )
			g_MOB_MAX_RESET[y][x] = MAX( value, 255 );
	}
}

//
// Function Name: LoadMobDropItemTable
// Date: 
// Description: 
//

BOOL LoadMobDropItemTable()
{
	FILE *fp;

	fp = fopen( dMOBINVEN_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadMobDropItemTable : !fp\r\n" );
		return 0;
	}
	
	char readBuff[1024];

	int mobNum;
	int itemNum;
	
	int percent;

	while( !feof( fp ) )
	{
		int nPos = 0;

		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' ||
			*readBuff == '#' )
			break;
		
		mobNum = GetTabNumberic( readBuff, nPos );
		itemNum = GetTabNumberic( readBuff, nPos );
		percent = GetTabNumberic( readBuff, nPos );

		if( itemNum == dMONEY_NUM ||
			!itemNum ||
			!percent )
			continue;

		if( !g_MOBINFO[mobNum] )
		{
			log( "LoadMobDropItemTable : bad Mob Num [ %d ] \r\n", mobNum );
			fclose( fp );
			return 0;
		}

		if( !IS_VALID_ITEM( itemNum ) )
		{
			log( "LoadMobDropItemTable : Bad item num mob:%d item:%d\r\n", mobNum, itemNum );
			fclose( fp );
			return 0;
		}

		sPMOB_INVEN pMobInven = (sPMOB_INVEN)malloc(sizeof(sMOB_INVEN));

		if( !pMobInven )
		{
			log( "LoadMobDropItemTable : !pMobInven malloc failed\r\n" );
			fclose( fp );
			return 0;
		}
		
		pMobInven->itemNum = itemNum;
		
		pMobInven->percent = percent;
								
		pMobInven->prev = NULL;
		pMobInven->next = NULL;

		g_MOBINFO[mobNum]->invenCnt++;
		
		INSERT_TO_LIST( g_MOBINFO[mobNum]->inven, pMobInven, prev, next );
	}

	fclose( fp );

	return 1;
}


//
// Function Name: MobResetPulse
// Date: 
// Description: 
//
void MobResetPulse()
{
	sPMOB_RESET reset, next_reset;
	sPLIST_BLOCK list, block, next_block;
	sPWORLD pWorld;

	list = gWORLDLIST.GetFirstBlock();

	LIST_WHILE( list, block, next_block, next, WMgr );
	
	pWorld = (sPWORLD)block->data;

	// 전쟁중인 맵이면 Skip
	if( pWorld->underAttack )
		LIST_SKIP( block, next_block ); 

	LIST_WHILE( pWorld->mobReset , reset, next_reset , next, WMgr2 );
		
#ifdef dEVENT_MOB_ON

	if( reset->mapNum >= 10 && reset->mapNum <= 12 )
	{	
		WORD eventMobNum = 103;
		char tmp[128];

		// 102번
		if( reset->mapNum == 12 && !g_EVENT_MOB[0].isAlive && g_EVENT_MOB[0].updateTime <= g_CurrRealTime )
		{
			eventMobNum = 102;

			CreateMob( eventMobNum, reset->mapNum, reset->posX, reset->posY, NULL, __FILE__, __LINE__ );

			//
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_SYSTEM_MSG, g_nPos );

			if( number( 1, 2 ) == 1 )
				//sprintf( tmp, "%s : 크르릉!!", g_MOBINFO[eventMobNum]->hName );//크르릉
				sprintf( tmp, "%s : ARRGGG!!!", g_MOBINFO[eventMobNum]->hName );//크르릉
			else
				//sprintf( tmp, "%s : 전부 죽여주마!!", g_MOBINFO[eventMobNum]->hName );
				sprintf( tmp, "%s : You all DIE!!", g_MOBINFO[eventMobNum]->hName );
			
			PutString( g_Packet, tmp, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendToMap( reset->mapNum, g_Packet, g_nPos );
			//

			WriteToFile( "BOSS_MOB_POP.TXT", "%d\t\t%s\t\t%d %d %d\r\n", eventMobNum, g_MOBINFO[eventMobNum]->hName, reset->mapNum, reset->posX, reset->posY ); 

			g_EVENT_MOB[0].isAlive = 1;
		}
		// 103 번
		else if( reset->mapNum == 10 && !g_EVENT_MOB[1].isAlive && g_EVENT_MOB[1].updateTime <= g_CurrRealTime )
		{
			eventMobNum = 103;

			CreateMob( eventMobNum, reset->mapNum, reset->posX, reset->posY, NULL, __FILE__, __LINE__ );

			//
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_SYSTEM_MSG, g_nPos );
			
			sprintf( tmp, "%s: All fly away!!", g_MOBINFO[eventMobNum]->hName );
			
			PutString( g_Packet, tmp, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendToMap( reset->mapNum, g_Packet, g_nPos );
			//

			WriteToFile( "BOSS_MOB_POP.TXT", "%d\t\t%s\t\t%d %d %d\r\n", eventMobNum, g_MOBINFO[eventMobNum]->hName, reset->mapNum, reset->posX, reset->posY ); 

			g_EVENT_MOB[1].isAlive = 1;
		}
		else if( reset->mapNum == 11 && !g_EVENT_MOB[2].isAlive && g_EVENT_MOB[2].updateTime <= g_CurrRealTime )
		{
			eventMobNum = 101;

			CreateMob( eventMobNum, reset->mapNum, reset->posX, reset->posY, NULL, __FILE__, __LINE__ );

			//
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_SYSTEM_MSG, g_nPos );

			sprintf( tmp, "%s: Watch my power! Or you will face death colder than ice!!", g_MOBINFO[eventMobNum]->hName );

			PutString( g_Packet, tmp, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendToMap( reset->mapNum, g_Packet, g_nPos );
			//

			WriteToFile( "BOSS_MOB_POP.TXT", "%d\t\t%s\t\t%d %d %d\r\n", eventMobNum, g_MOBINFO[eventMobNum]->hName, reset->mapNum, reset->posX, reset->posY ); 

			g_EVENT_MOB[2].isAlive = 1;
		}
	}

#endif

	if( reset->currCnt < g_MOB_MAX_RESET[reset->mapNum][reset->mobNum] )
	{
		if( number( 0, 3 ) == 1 )
			CreateMob( reset->mobNum, reset->mapNum, reset->posX, reset->posY , reset, __FILE__, __LINE__ ); 
	}
	LIST_WHILEEND( pWorld->mobReset, reset, next_reset );

	LIST_WHILEEND( list, block, next_block );
}



//
// Function Name: MoveResetPoint
// Date: 
// Description: 
//
void MoveResetPoint()
{
	sPMOB_RESET reset, next_reset;
	sPLIST_BLOCK list, block, next_block;
	sPWORLD pWorld;
	
	BYTE tmpRnd;
	int posX, posY;
	
	list = gWORLDLIST.GetFirstBlock();
	
	LIST_WHILE( list, block, next_block, next, WMgr );

	pWorld = (sPWORLD)block->data;
	
	LIST_WHILE( pWorld->mobReset , reset, next_reset , next, WMgr2 );
	
	// 5분 기준
	if( reset->moveStartTime + 300000 <= g_CurrTime )
	{
		reset->posX = reset->origPosX;
		reset->posY = reset->origPosY;
		reset->moveStartTime = g_CurrTime;
		LIST_SKIP( reset, next_reset );
	}
	
	tmpRnd = number( 1, 10 );
	
	if( tmpRnd == 1 )
		reset->moveDir = number( 0, 7 );	//number( 0, 3 );
	
	posX = reset->posX;
	posY = reset->posY;
	
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
	
	if( !IS_MAP_RANGE( posX, posY ) )
	{
		reset->moveDir = number( 0, 7 );
		LIST_SKIP( reset, next_reset );
	}
	
	if( IS_MOB_MOVABLE( reset->mapNum, posX, posY ) )
	{
		reset->moveDir = number( 0, 7 );
		LIST_SKIP( reset, next_reset );
	}
	
	reset->posX = posX;
	reset->posY = posY;
	
	LIST_WHILEEND( pWorld->mobReset, reset, next_reset );
	
	LIST_WHILEEND( list, block, next_block );
}



//
// Function Name: 
// Date: 
// Description: 
//
BOOL LoadMobReset( int mapNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return 0;

	FILE *fp;
	char filename[128];

	sprintf( filename, "./MAP/land%04d.map", mapNum );

	fp = fopen( filename, "rb" );

	if( !fp )
	{
		log( "LoadMobReset : %s file not found\r\n", filename );
		return 0;
	}
	
	int totalMob = 0;
	DWORD dwSize;
	DWORD m_dwSize;
	DWORD dwVersion;
	long skip_size = 0;

	fread( &dwVersion, sizeof( DWORD ), 1, fp );
	fread( &dwSize, sizeof( DWORD ), 1, fp );
	
	m_dwSize = dwSize * 4;
	
	skip_size += 32; // 텍스춰 이름 	
	skip_size += ( dwSize * dwSize * sizeof( float ) );	// 높이 맵
	skip_size += ( dwSize * dwSize * sizeof( BYTE ) );	// 텍스춰 맵
	skip_size += ( dwSize * dwSize * sizeof( DWORD ) );	// 컬러

/*
	for( int i = 0 ; i < 3 ; i++ )
	{
		pTerrain->SetLayer(i);
		pWorld->SetLayer(i);
		ar.Serialize( pTerrain->GetObjectHeightMap(),		dwSize * dwSize * 4 * sizeof( float ) );
		ar.Serialize( pWorld->GetMovable(),			dwSize * 2 * dwSize * 2 * sizeof( BYTE ) );
		ar.Serialize( pTemp,						dwSize * dwSize * sizeof( BYTE ) );
	} //for
*/
	// Layer ( 현재 총 3개 )
	int i;
	
	for( i = 0; i < dMAX_MAP_LAYER; i++ )
	{
		skip_size += ( m_dwSize * m_dwSize * sizeof( float ) );
		skip_size += ( m_dwSize * m_dwSize * sizeof( BYTE ) );
		skip_size += ( dwSize * dwSize * sizeof( BYTE ) );
	}

	fseek( fp, skip_size, SEEK_CUR );

	int		numEntities;

	fread( &numEntities, sizeof( int ), 1, fp );

	char	szName[32];
	Vector3	vPos;
	float	fYaw;
	int		mobNum;

	for( i = 0; i < numEntities; i++ )
	{	
		fread( &szName, 32, 1, fp );
		fread( &vPos, sizeof( Vector3 ), 1, fp );
		fread( &fYaw, sizeof( float ), 1, fp );

		//log( "mapNum : %d modName : %s\r\n", mapNum, szName );
		int j=0;
		for( j = 0; j < 2; j++  )
		{
			if( !strncmp( szName, "MOB_", 4 ) || !strncmp( szName, "mob_", 4 ) )
			{
				if( sscanf( szName, "MOB_%d.mod", &mobNum ) != 1 )
				{
					if( sscanf( szName, "mob_%d.mod", &mobNum ) != 1 )
					{
						fclose( fp );
						return 0;
					}
				}

				int rnd = number( 0, 5 );

				switch( rnd )
				{
				case 1:
					mobNum = mobNum + 200;
					break;
				case 2:
					mobNum = mobNum + 300;
					break;
				case 3:
					mobNum = mobNum + 400;
					break;
				case 4:
					mobNum = mobNum + 500;
					break;
				case 5:
					mobNum = mobNum + 600;
					break;
				}

				if( mobNum <= 0 || mobNum >= dMAX_MOBINFO )
					continue;

				if( !g_MOBINFO[mobNum] )
					continue;

				// 이동 불가 지역인 경우
				if( gWORLD[mapNum]->mobMapAttr[(int)vPos.y][(int)vPos.x] )
					continue;

				//
				sPMOB_RESET newData = (sPMOB_RESET)malloc(sizeof(sMOB_RESET));

				if( !newData )
				{
					fclose(fp);
					return 0;
				}

				//			if( mapNum == 1003 )
				//				log( "mob : %d\r\n", mobNum );

				newData->mapNum = mapNum;

				newData->mobNum = mobNum;

				newData->posX = (int)vPos.x;

				newData->origPosX = newData->posX;

				newData->posY = (int)vPos.y;

				newData->origPosY = newData->posY;

				newData->moveDir = number( 0, 7 );

				newData->next = NULL;

				newData->prev = NULL;

				newData->currCnt = 0;

				INSERT_TO_LIST( gWORLD[newData->mapNum]->mobReset, newData, prev, next );
				gWORLD[newData->mapNum]->mobResetCnt++;

				totalMob++;
			}
		} 
	}

	fclose(fp);

	log( "LoadMobReset : Total Monster Count [%d/%d]\r\n", mapNum, totalMob );

	return 1;
}



//
// Function Name: LoadMobInfo
// Date: 
// Description: 
//
BOOL LoadMobInfo()
{
	InitMaxMobReset( 1 );

#ifdef _dUMENOKOJI_ //마방관련
	FILE	*fp = fopen( dMTL , "rb" );		//테스트섭 Test server
#else
	FILE *fp = fopen( dMOB_TABLE_FILE , "rb" );		//본섭		//나중에 본섭에 적용할때는 
#endif
	
	if( !fp )
	{
		log( "Can't Find MOB_DATA.TXT\r\n" );
		return 0;
	}
	
	int mobNum;				// 몹번호 
	char hName[64];			// 한글이름
	char eName[64];			// 영문이름 
	int level;				// 레벨 
	int hp;					// 체력
	int mp;					// 마나
	int minDamage;			// 물리 최소 공격력
	int maxDamage;			// 물리 최대 공격력
	int dp;					// 물리 방어력  Physical defense

#ifdef _dUMENOKOJI_			//마방관련
	int	minMagicDamage;		//마법 최소 공격력
	int	maxMagicDamage;		//마법 최대 공격력
	int	Magicdp;			//마법 방어력 Magical Defense
	int	mobAttackAttr;		//몬스터의 공격 속성 Monster attack properties
#endif

	int exp;				// 경험치 
	int money;				// 돈 
	int dropPercent;		// 전리확률 
	int dropLevelLimit;		// 전리Level제한 
	int bonusExp;			// 보너스 경험치 
	int attackRange;		// 공격거리 
	int defenseRange;		// 방어 범위 
	int helpRange;			// 원호 범위 
	int moveRange;			// 이동 범위 
	int attackAttr;			// 공격 성향 
	int helpAttr;			// 원호 성향 
	int attackType;			// 공격 방법
	int waitDelay;			// 대기 딜레이 
	int moveSpeed;			// 이동 속도
	int traceSpeed;			// 추적 속도
	int attackSpeed;		// 공격 속도
	int resetTimer;			// 리셋 주기 
	int minCnt;				// 최소 개체수 
	int mobAttr;			// 몬스터 속성
	int minTurn;			// 최소 턴
	int maxTurn;			// 최대 턴 
	int speAttackType;		// 특수공격종류 
	int speAttackLevel;		// 특수공격레벨 
	int createEffect;		// 생성 Effect 종류 
	int	deadEffect;			// 죽음 Effect 종류 

	char readBuff[1024];
	int nPos;

	int totalMobCnt = 0;

	while( !feof( fp ) )
	{
		nPos = 0;
		
		fgets( readBuff, 1000, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		mobNum = GetTabNumberic( readBuff, nPos ); 
		GetTabString( readBuff, hName, nPos );
		GetTabString( readBuff, eName, nPos );
		level = GetTabNumberic( readBuff, nPos ); 
		hp = GetTabNumberic( readBuff, nPos );  
		mp = GetTabNumberic( readBuff, nPos );  
		minDamage = GetTabNumberic( readBuff, nPos );  
		maxDamage = GetTabNumberic( readBuff, nPos ); 
		dp = GetTabNumberic( readBuff, nPos ); 

#ifdef _dUMENOKOJI_ //마방관련
		minMagicDamage = GetTabNumberic( readBuff, nPos );
		maxMagicDamage = GetTabNumberic( readBuff, nPos );
		Magicdp	= GetTabNumberic( readBuff, nPos );
#endif
		exp = GetTabNumberic( readBuff, nPos ); 
		money = GetTabNumberic( readBuff, nPos ); 
		dropPercent = GetTabNumberic( readBuff, nPos ); 
		dropLevelLimit = GetTabNumberic( readBuff, nPos ); 
		bonusExp = GetTabNumberic( readBuff, nPos ); 
		attackRange = GetTabNumberic( readBuff, nPos ); 
		defenseRange = GetTabNumberic( readBuff, nPos ); 
		helpRange = GetTabNumberic( readBuff, nPos ); 
		moveRange = GetTabNumberic( readBuff, nPos ); 
		attackAttr = GetTabNumberic( readBuff, nPos ); 
		helpAttr = GetTabNumberic( readBuff, nPos ); 
		attackType = GetTabNumberic( readBuff, nPos ); 
		waitDelay = GetTabNumberic( readBuff, nPos ); 
		moveSpeed = GetTabNumberic( readBuff, nPos ); 
		traceSpeed = GetTabNumberic( readBuff, nPos ); 
		attackSpeed = GetTabNumberic( readBuff, nPos ); 
		resetTimer = GetTabNumberic( readBuff, nPos ); 
		minCnt = GetTabNumberic( readBuff, nPos ); 
		mobAttr = GetTabNumberic( readBuff, nPos ); 
		minTurn = GetTabNumberic( readBuff, nPos ); 
		maxTurn = GetTabNumberic( readBuff, nPos ); 
		speAttackType = GetTabNumberic( readBuff, nPos ); 
		speAttackLevel = GetTabNumberic( readBuff, nPos ); 
		createEffect = GetTabNumberic( readBuff, nPos ); 
		deadEffect  = GetTabNumberic( readBuff, nPos ); 
#ifdef _dUMENOKOJI_ //마방관련
		mobAttackAttr = GetTabNumberic( readBuff, nPos );
#endif
		
		if( mobNum >= dMAX_MOBINFO )
		{
			log( "LoadMobInfo : invalid mobNum\r\n" );
			fclose( fp );
			return 0;
		}
		
		if( !g_MOBINFO[mobNum] )
		{
			g_MOBINFO[mobNum] = (sPMOB_DEF_INFO)malloc(sizeof(sMOB_DEF_INFO));
			
			if( !g_MOBINFO[mobNum] )
			{
				log( "LoadMobInfo : malloc failed\r\n" );
				fclose( fp );
				return 0;
			}
		}
		
		g_MOBINFO[mobNum]->mobNum = mobNum;					// 몹번호 
		
		strcpy( g_MOBINFO[mobNum]->hName, hName );			// 한글이름
		strcpy( g_MOBINFO[mobNum]->eName, eName );			// 영문이름 
		
		g_MOBINFO[mobNum]->level = level;					// 레벨 
		
		g_MOBINFO[mobNum]->hp = hp;							// 체력
		g_MOBINFO[mobNum]->mp = mp;							// 마나
		
		g_MOBINFO[mobNum]->minDamage = minDamage;			// 물리 최소공격력
		g_MOBINFO[mobNum]->maxDamage = maxDamage;			// 물리 최대공격력
		g_MOBINFO[mobNum]->dp = dp;							// 물리 방어력 
		
#ifdef _dUMENOKOJI_											//마방관련
		g_MOBINFO[mobNum]->minMagicDamage = minMagicDamage;	// 마법 최소 공격력
		g_MOBINFO[mobNum]->maxMagicDamage = maxMagicDamage;	// 마법 최대 공격력
		g_MOBINFO[mobNum]->Magicdp = Magicdp;				// 마법 방어력
		g_MOBINFO[mobNum]->mobAttackAttr = mobAttackAttr;	// 몹의 공격 속성
#endif

		g_MOBINFO[mobNum]->exp = exp;						// 경험치 
		g_MOBINFO[mobNum]->bonusExp = bonusExp;				// 보너스 경험치 
		
		g_MOBINFO[mobNum]->money = money;						// 돈 
		
		g_MOBINFO[mobNum]->dropPercent = dropPercent;			// 전리확률 
		g_MOBINFO[mobNum]->dropLevelLimit = dropLevelLimit;		// 전리Level제한 
		
		g_MOBINFO[mobNum]->attackRange = attackRange;			// 공격거리 
		g_MOBINFO[mobNum]->defenseRange = defenseRange;			// 방어 범위 
		g_MOBINFO[mobNum]->helpRange = helpRange;				// 원호 범위 
		g_MOBINFO[mobNum]->moveRange = moveRange;				// 이동 범위 
		
		g_MOBINFO[mobNum]->attackAttr = attackAttr;			// 공격 성향 
		g_MOBINFO[mobNum]->helpAttr = helpAttr;				// 원호 성향 
		
		g_MOBINFO[mobNum]->attackType = attackType;			// 공격 방법
		
		g_MOBINFO[mobNum]->waitDelay = waitDelay;			// 대기 딜레이 
		
		g_MOBINFO[mobNum]->moveSpeed = moveSpeed;			// 이동 속도
		g_MOBINFO[mobNum]->traceSpeed = traceSpeed;			// 추적 속도
		g_MOBINFO[mobNum]->attackSpeed = attackSpeed;		// 공격 속도
		
		g_MOBINFO[mobNum]->resetTimer = resetTimer;			// 리셋 주기 
		
		g_MOBINFO[mobNum]->minCnt = minCnt;				// 최소 개체수 
		
		g_MOBINFO[mobNum]->mobAttr = mobAttr;			// 몬스터 속성
		
		g_MOBINFO[mobNum]->minTurn = minTurn;			// 최소 턴
		g_MOBINFO[mobNum]->maxTurn = maxTurn;			// 최대 턴 
		
		g_MOBINFO[mobNum]->speAttackType = speAttackType;		// 특수공격종류 
		g_MOBINFO[mobNum]->speAttackLevel = speAttackLevel;		// 특수공격레벨 
		
		g_MOBINFO[mobNum]->createEffect = createEffect;		// 생성 Effect 종류 
		g_MOBINFO[mobNum]->deadEffect = deadEffect;			// 죽음 Effect 종류 
		
		g_MOBINFO[mobNum]->inven = NULL;					// 몹 인벤토리
		g_MOBINFO[mobNum]->invenCnt = 0;
		g_MOBINFO[mobNum]->currDropItem = NULL;				// 드랍할 아이템 
	//	log_file( "MonsterName_Attr.txt","몬스터이름[%s]\t\t공격속성[%d]\r\n", g_MOBINFO[mobNum]->hName, g_MOBINFO[mobNum]->mobAttackAttr );
		totalMobCnt++;
	}

	fclose(fp);

	log( "LoadMobInfo : total Monster Count : %d / %d\r\n", totalMobCnt , dMAX_MOBINFO );

	return 1;
}



//
// Function Name: DeleteDefaultMobInfo
// Date: 
// Description: 
//
void DeleteDefaultMobInfo()
{
	for( int i = 0; i < dMAX_MOBINFO; i++ )
	{
		if( g_MOBINFO[i] )
		{
			sPMOB_INVEN inven, next_inven;
			
			LIST_WHILE( g_MOBINFO[i]->inven, inven, next_inven, next, WMgr );
			REMOVE_FROM_LIST( g_MOBINFO[i]->inven, inven, prev, next );
			SAFE_FREE( inven );
			LIST_WHILEEND( g_MOBINFO[i]->inven, inven, next_inven );
			
			SAFE_FREE( g_MOBINFO[i] );
		}
	}
}

//
// Function Name: MakeMobInPacket
// Date: 
// Description: 
//
void MakeMobInPacket( sPMOB_DATA pMob, BYTE isCreate )
{
	if( pMob->mobNum > 1000 )
	{
		g_nPos = 2;
		
		PutWord( g_Packet, dPACKET_INSERT_OBJMOB, g_nPos );
		PutWord( g_Packet, pMob->idxNum, g_nPos );
		PutWord( g_Packet, pMob->mobNum, g_nPos );
		PutInteger( g_Packet, pMob->posX, g_nPos );
		PutInteger( g_Packet, pMob->posY, g_nPos );	
		PutInteger( g_Packet, pMob->objAngle, g_nPos );
		PutInteger( g_Packet, pMob->hp, g_nPos );

		switch( pMob->mobNum )
		{
		case dMONSTER_WARCRISTAL:
			PutInteger( g_Packet, dMONSTER_HP_WARCRISTAL, g_nPos );
			break;
		case dMONSTER_CASTLEDOOR:
			PutInteger( g_Packet, dMONSTER_HP_CASTLEDOOR, g_nPos );
			break;
		}
		
		PutSize( g_Packet, g_nPos );
	}
	else
	{
		g_nPos = 2;
		
		PutWord( g_Packet, dPACKET_MONSTER_INSERT, g_nPos );
		PutWord( g_Packet, pMob->idxNum, g_nPos );
		PutWord( g_Packet, pMob->mobNum, g_nPos );
		PutInteger( g_Packet, pMob->posX, g_nPos );
		PutInteger( g_Packet, pMob->posY, g_nPos );
		//
		PutByte( g_Packet, pMob->move_range, g_nPos );
		PutByte( g_Packet, pMob->move_range2, g_nPos );
		PutByte( g_Packet, pMob->enemy ? 1 : 0, g_nPos );
		
		PutInteger( g_Packet, pMob->hp, g_nPos );
		PutInteger( g_Packet, GET_MOB_HP( pMob ), g_nPos );
		
		PutByte( g_Packet, isCreate, g_nPos );
		
		PutSize( g_Packet, g_nPos );
	}
}

//
// Function Name: MakeMobOutPacket
// Date: 2003-02-12
// Description: 
//
void MakeMobOutPacket( sPMOB_DATA pMob )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_MONSTER_REMOVE, g_nPos );
	PutWord( g_Packet, pMob->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );
}


//
// Function Name: GetMobSector
// Date: 2003-02-12
// Description: 
//
void GetMobSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPMOB_DATA m, next_m;
	
	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m , w_next, WMgr ); 
	// AI 몹이 아닐경우 사용자가 시야범위안에 들어오면 
	// 다시 AI 리스트로 들어가서 AI 처리 시작
#ifdef dUSE_NONAIMOB
	if( !m->isAIMob )
	{
		INSERT_TO_LIST( gMOBS.list, m, prev, next );
		m->isAIMob = 1;
	}
#endif
	
	MakeMobInPacket( m , 0 );
	SendData( pPlayer, g_Packet, g_nPos );
	LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m );
}


//
// Function Name: GetMobVSector
// Date: 2003-02-12
// Description: 
//
void GetMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				GetMobSector( pPlayer,  x, y );
		}
	}
}

//
// Function Name: GetMobHSector
// Date: 2003-02-12
// Description: 
//
void GetMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				GetMobSector( pPlayer, x, y );
		}
	}
}

//
// Function Name: OutMobSector
// Date: 2003-02-12
// Description: 
//
void OutMobSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPMOB_DATA m, next_m;
	
	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m , w_next, WMgr ); 

	if( m->enemy == pPlayer )
	{
		SET_MOB_ENEMY( m, NULL );
		SendMobState( m );
	}

	for( int i = 0; i < dMAX_SAVE_MOB_EXP; i++ )
	{
		if( m->saveExp[i].pPlayer == pPlayer )
		{
			m->saveExp[i].pPlayer = NULL;
			
			m->saveExp[i].expDamage[0] = 0;
			m->saveExp[i].expDamage[1] = 0;
			m->saveExp[i].expDamage[2] = 0;
			m->saveExp[i].expDamage[3] = 0;
		}
	}

	MakeMobOutPacket( m );
	SendData( pPlayer, g_Packet, g_nPos );

	LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m );
}

//
// Function Name: OutMobVSector
// Date: 2003-02-12
// Description: 
//
void OutMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				OutMobSector( pPlayer,  x, y );
		}
	}
}

//
// Function Name: OutMobHSector
// Date: 2003-02-12
// Description: 
//
void OutMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				OutMobSector( pPlayer, x, y );
		}
	}
}


//
// Function Name: SendToMobVSector
// Date: 2003-02-12
// Description: 
//
void SendToMobVSector( int x, int startY, int endY, sPMOB_DATA pMob, char *data, int size )
{
	int mapNum = pMob->mapNum;

	if( x >= 0 && x < dONE_TILE_SIZE )
	{
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[mapNum]->player[y][x], data, size , dSENDMODE_TOALL );
		}
	}
}


//
// Function Name: SendToMobHSector
// Date: 2003-02-12
// Description: 
//
void SendToMobHSector( int y, int startX, int endX, sPMOB_DATA pMob, char *data, int size )
{
	int mapNum = pMob->mapNum;

	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
		}
	}
}


//
// Function Name: SendToMobArea
// Date: 2003-02-12
// Description: 
//
void SendToMobArea( sPMOB_DATA pMob, char *data, int size )
{
	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);
	
	BYTE findUser = 0;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				if( gWORLD[pMob->mapNum]->player[y][x] )
				{
					SendToSector( NULL, gWORLD[pMob->mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
					findUser = 1;
				}
			}
		}
	}
#ifdef dUSE_NONAIMOB
	if( !pMob->isObject )
	{
		if( !findUser )
		{
			if( pMob->AiTick++ >= dDEF_MOB_AITICK )
			{
				REMOVE_FROM_LIST( gMOBS.list, pMob, prev, next );
				pMob->isAIMob = 0;
			}
		}
		else 
		{
			if( pMob->AiTick )
				pMob->AiTick = 0;
		}
	}
#endif
}

//
// Function Name: GetCurrAreaMob
// Date: 2003-02-12
// Description: 
//
void GetCurrAreaMob( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				GetMobSector( pPlayer, x, y );
}

//
// Function Name: GetCurrHouseMob
// Date: 2003-02-12
// Description: 
//
void GetCurrHouseMob( sPDESC_DATA pPlayer )
{
	sPMOB_DATA mob, next_mob;

	LIST_WHILE( gHOUSE[GET_MAP_NUM(pPlayer)]->in_mob, mob, next_mob, w_next, WMgr );

	MakeMobInPacket( mob , 0 );
	SendData( pPlayer );

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM(pPlayer)]->in_mob, mob, next_mob );
}

//
// Function Name: CheckMobSectorEnemy
// Date: 2003-03-01
// Description: 
//
void CheckMobSectorEnemy( sPMOB_DATA pMob, int x, int y )
{
	sPDESC_DATA d, next_d;

	int rangeX, rangeY;
	
	LIST_WHILE( gWORLD[pMob->mapNum]->player[y][x], d, next_d , w_next, WMgr );
	rangeX = abs( d->ch.posX - pMob->posX );
	rangeY = abs( d->ch.posY - pMob->posY );
	
	// 선제 공격이 가능한 몹이면
	if( IS_SET( GET_MOB_ATTACKATTR( pMob ) , dMOBATTACK_ATTR_AGRESSIVE ) )
	{
		// 감지거리 안에 들어올 경우
		if( rangeX <= GET_MOB_VISIONRANGE( pMob ) && rangeY <= GET_MOB_VISIONRANGE( pMob ) )
		{
			if( CheckEquipItem( d, dEQUIP_NECK, dAMULET_INVISIBILITY ) )
			{
				return;
			}
			if( GET_DEAD_STATE( d ) == dDEAD_STATE_NONE && 
#ifdef _DUMMY_MODE_
				!d->isDummy &&
#endif
				!IsSetVisibleSkill( d ) //&&
				//!CheckEquipItem( d, dEQUIP_NECK, dAMULET_INVISIBILITY )
				//(!CheckEquipItem( d, dEQUIP_NECK, dAMULET_INVISIBILITY )||
				//((CheckEquipItem( d, dEQUIP_NECK, dAMULET_INVISIBILITY )||IsSetVisibleSkill( d ))&&(pMob->mobNum>100))
				)
			{
				if( IsValidAttackArea( d, pMob ) )
				{
					SET_MOB_ENEMY( pMob, d );
					SendMobState( pMob );
					return;
				}
			}
		}
	}
	LIST_WHILEEND( gWORLD[pMob->mapNum]->player[y][x], d, next_d );
}

//
// Function Name: CheckMobEnemyArea
// Date: 2003-03-01
// Description: 
//
void CheckMobEnemyArea( sPMOB_DATA pMob )
{
	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				CheckMobSectorEnemy( pMob, x, y );
}


//
// Function Name: OutCurrAreaMob
// Date: 2003-02-28
// Description: 
//
void OutCurrAreaMob( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				OutMobSector( pPlayer, x, y );
}

//
// Function Name: OutCurrHouseMob
// Date: 
// Description: 
//
void OutCurrHouseMob( sPDESC_DATA pPlayer )
{
	sPMOB_DATA mob, next_mob;

	LIST_WHILE( gHOUSE[GET_MAP_NUM(pPlayer)]->in_mob, mob, next_mob, w_next, WMgr );

	MakeMobOutPacket( mob );
	SendData( pPlayer );

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM(pPlayer)]->in_mob, mob, next_mob );
}


//
// Function Name: NewMobData
// Date: 2003-02-12
// Description: 
//
sPMOB_DATA NewMobData()
{
	sPMOB_DATA newMob;

	if( gMOBS.memory )
	{
		newMob = gMOBS.memory;
		REMOVE_FROM_LIST( gMOBS.memory, newMob, prev, next );
		gMOBS.total_memory--;
		return newMob;
	}
	else
	{
		newMob = (sPMOB_DATA)malloc(sizeof(sMOB_DATA));

		if( !newMob )
			return NULL;
		return newMob;
	}
	return NULL;
}



//
// Function Name: MoveMonster
// Date: 2003-02-28
// Description: 몬스터 이동 함수 
//
void MoveMonster( sPMOB_DATA pMob, int posX, int posY )
{
	int newX, newY, oldX, oldY, mapNum = pMob->mapNum;

	newX = (int)(posX / dONE_TILE);
	newY = (int)(posY / dONE_TILE);
	oldX = (int)(pMob->posX / dONE_TILE);
	oldY = (int)(pMob->posY / dONE_TILE);

#ifdef __ONLY_ONE_MOB_IN_SECTOR__
	if( newX != oldX || newY != oldY )
	{
		if( gWORLD[pMob->mapNum].mob[newY][newX] )
		{
			pMob->moveDir = number( 0, 7 );
			return;
		}
	}
#endif

	pMob->posX = posX;
	pMob->posY = posY;
	
	if( newX != oldX || newY != oldY )
	{
		REMOVE_FROM_LIST( gWORLD[mapNum]->mob[oldY][oldX], pMob, w_prev, w_next );
		INSERT_TO_LIST( gWORLD[mapNum]->mob[newY][newX], pMob, w_prev, w_next );
		
		if( newY < oldY && newX == oldX ) // Y 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY+1, oldX-1, oldX+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobHSector( newY-1, newX-1, newX+1, pMob, g_Packet, g_nPos );			
		}
		else if( newX > oldX && newY < oldY ) // X증가 Y 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY+1, oldX, oldX+1, pMob, g_Packet, g_nPos );
			SendToMobVSector( oldX-1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobHSector( newY-1, newX-1, newX, pMob, g_Packet, g_nPos );
			SendToMobVSector( newX+1, newY-1, newY+1, pMob, g_Packet, g_nPos );
		}
		else if( newX > oldX && newY == oldY ) // X 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX-1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobVSector( newX+1, newY-1, newY+1, pMob, g_Packet, g_nPos );			
		}
		else if( newX > oldX && newY > oldY ) // X 증가 Y 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX-1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( oldY-1, oldX, oldX+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobVSector( newX+1, newY-1, newY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( newY+1, newX-1, newX, pMob, g_Packet, g_nPos );			
		}
		else if( newY > oldY && newX == oldX ) // Y 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY-1, oldX-1, oldX+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobHSector( newY+1, newX-1, newX+1, pMob, g_Packet, g_nPos );			
		}
		else if( newX < oldX && newY > oldY ) // X 감소 Y 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY-1, oldX-1, oldX+1, pMob, g_Packet, g_nPos );
			SendToMobVSector( oldX+1, oldY, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobHSector( newY+1, newX-1, newX+1, pMob, g_Packet, g_nPos );
			SendToMobVSector( newX-1, newY-1, newY, pMob, g_Packet, g_nPos );			
		}
		else if( newX < oldX && newY == oldY ) // X 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX+1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobVSector( newX-1, newY-1, newY+1, pMob, g_Packet, g_nPos );
		}
		else if( newX < oldX && newY < oldY ) // X감소 Y 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX+1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( oldY+1, oldX-1, oldX, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob , 0 );
			SendToMobVSector( newX-1, newY-1, newY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( newY-1, newX, newX+1, pMob, g_Packet, g_nPos );			
		}
	}
	
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_MONSTER_MOVE, g_nPos );
	PutWord( g_Packet, pMob->idxNum, g_nPos );
	PutInteger( g_Packet, pMob->posX, g_nPos );
	PutInteger( g_Packet, pMob->posY, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToMobArea( pMob, g_Packet, g_nPos );
}

//
// Function Name: IsMovable
// Date: 
// Description: 
//
int IsMovable( sPMOB_DATA pMob, int destX, int destY )
{
	if( !IS_MAP_RANGE( destX, destY ) )
		return MOVABLE_BLOCKEXC;

	if( gWORLD[pMob->mapNum]->playerMoveAttr[destY][destX] )
		return MOVABLE_BLOCKPLAYER;

	if( IS_MOB_MOVABLE( pMob->mapNum, destX, destY ) )
		return MOVABLE_BLOCKMAP;

	return MOVABLE_OK;
}



//
// Function Name: MonsterMoveProc
// Date: 2003-03-01
// Description: 일정 시간단위로 몬스터 이동
//
void MonsterMoveProc()
{
	sPMOB_DATA m, next_m;

	int posX, posY;
	BYTE moveRange = 1;
	int movable;
	WORD moveSpeed;

	LIST_WHILE( gMOBS.list, m, next_m , next, WMgr );

	// Obj Monster Skip
	if( m->mobNum == dMONSTER_WARCRISTAL ||
		m->mobNum == dMONSTER_CASTLEDOOR )
	{
		LIST_SKIP( m, next_m );
	}

	if( m->isDead )
	{
		if( m->deadTimer < g_CurrTime )
		{
			MonsterDie( m );
			DelMobData( m, __FILE__, __LINE__ );
		}
		LIST_SKIP( m, next_m );
	}

	if( m->enemy )
		moveSpeed = m->move_speed2;
	else
		moveSpeed = m->move_speed;

	switch( m->moveDir )
	{
	case 1:
	case 3:
	case 5:
	case 7:
		moveSpeed += (int)( (float)moveSpeed * 0.4444444F );
		break;
	}
	
	if( m->enemy )
	{
		if( IsMobAttackRange( m, m->enemy ) )
		{
			if( m->AttrEffectTime[MOB_ATTREFFECT_CANTATTACK] &&
				m->AttrEffectTime[MOB_ATTREFFECT_CANTATTACK] > g_CurrTime )
			{
				LIST_SKIP( m, next_m );
			}
			else
				m->AttrEffectTime[MOB_ATTREFFECT_CANTATTACK] = 0;

			if( GET_DEAD_STATE( m->enemy ) != dDEAD_STATE_NONE )
			{
				SET_MOB_ENEMY( m, NULL );
				SendMobState( m );
			}
			else
			{
				if( m->lastAttackTime + m->attackDelay <= g_CurrTime )
				{
#ifdef dEVENT_MOB_ON 
					if( m->mobNum == 102 ) // 스턴
					{
						if( !GET_STATUS_DELAY( m->enemy, dSTATUS_DELAY_STUN ) && dice( 10, 3 ) <= 17 )
							SetPlayerStun( m->enemy );
					}
					else if( m->mobNum == 103 )	// 랜덤 떨구기 
					{
						if( dice( 10, 3 ) <= 17 )
						{
							SendSystemMsg( m->enemy, g_LANG_STR[262], GET_MOB_HNAME( m ) );
							RandomTrans( m->enemy );
							LIST_SKIP( m, next_m );
						}
					}
#endif

					AttackToPlayer( m, m->enemy );
#ifdef dEVENT_MOB_ON 
					if( m->enemy )
					{
						if( m->mobNum >= 101 && m->mobNum <= 103 ) // 지역공격
						{
							sPDESC_DATA vict, next_vict;
							
							LIST_WHILE( gWORLD[m->mapNum]->player[m->posY/dONE_TILE][m->posX/dONE_TILE], vict, next_vict, w_next, WMgr );
							
							if( m->enemy )
							{
								if( vict != m->enemy && GET_DEAD_STATE( vict ) == dDEAD_STATE_NONE )
								{
									if( abs( GET_POSX( vict ) - GET_POSX( m->enemy ) ) <= 3 &&
										abs( GET_POSY( vict ) - GET_POSY( m->enemy ) ) <= 3 )
									{
										AttackToPlayer( m, vict, 2 );
									}
								}
							}
							else
								break;

							LIST_WHILEEND( gWORLD[m->mapNum]->player[m->posY/dONE_TILE][m->posX/dONE_TILE], vict, next_vict );
						}
					}
#endif

					m->lastAttackTime = g_CurrTime;
				}
			}
			LIST_SKIP( m, next_m );
		}
	}
	else
	{
		if( m->AICheckTIme + 1000 <= g_CurrTime )
		{
			CheckMobEnemyArea( m );
			m->AICheckTIme = g_CurrTime;
		}
	}

	if( m->lastMoveTime + moveSpeed < g_CurrTime && m->lastAttackTime + m->attackDelay <= g_CurrTime )
	{
		if( m->AttrEffectTime[MOB_ATTREFFECT_DONTMOVE] &&
			m->AttrEffectTime[MOB_ATTREFFECT_DONTMOVE] > g_CurrTime )
		{
			LIST_SKIP( m, next_m );
		}
		else
			m->AttrEffectTime[MOB_ATTREFFECT_DONTMOVE] = 0;

		m->lastMoveTime = g_CurrTime;

		// 이동처리 
		if( !m->enemy ) // 일반 이동
		{
			if( dice( 10, 3 ) <= 16 )
			{
				m->lastMoveTime += ( moveSpeed * 15 );
				LIST_SKIP( m, next_m );
			}

			BYTE tmpRnd = number( 1, 10 );
			
			if( tmpRnd == 1 )
				m->moveDir = number( 0, 7 );	//number( 0, 3 );
			
			posX = m->posX;
			posY = m->posY;
			
			switch( m->moveDir )
			{
			case 0:
				posY+=moveRange;
				break;
			case 1:
				posY+=moveRange;
				posX+=moveRange;
				break;
			case 2:
				posX+=moveRange;
				break;
			case 3:
				posY-=moveRange;
				posX+=moveRange;
				break;
			case 4:
				posY-=moveRange;
				break;
			case 5:
				posY-=moveRange;
				posX-=moveRange;
				break;
			case 6:
				posX-=moveRange;
				break;
			case 7:
				posY+=moveRange;
				posX-=moveRange;
				break;
			}
			
			if( IsMovable( m, posX, posY ) != MOVABLE_OK )
			{
				m->moveDir = number( 0, 7 ); 
				LIST_SKIP( m, next_m );
			}
			
			MoveMonster( m, posX, posY );	
		}
		else
		{
			// 공격 대상이 있을경우
			posX = m->posX;
			posY = m->posY;

			if( posX < m->enemy->ch.posX )
				posX+= moveRange;
			else if( posX > m->enemy->ch.posX )
				posX-= moveRange;
			
			if( posY < m->enemy->ch.posY )
				posY+= moveRange;
			else if( posY > m->enemy->ch.posY )
				posY-= moveRange;
			
			movable = IsMovable( m, posX, posY );

			if( movable == MOVABLE_BLOCKMAP )
			{
				m->moveDir = number( 0, 7 );
				stop_fighting_mob( m );
				LIST_SKIP( m, next_m );
			}

			MoveMonster( m, posX, posY );
		}
	}

	LIST_WHILEEND( gMOBS.list, m, next_m );
}




//
// Function Name: DelMobData
// Date: 2003-02-12
// Description: 
//
void DelMobData( sPMOB_DATA pMob , char *file, int line )
{
	if( pMob->state == dMON_STATE_UNUSED )
	{
		log( "DelMobData : pMob is UNUSED Data [%s/%d]\r\n", file, line );
		return;
	}

	gWORLD[pMob->mapNum]->currMobCnt--;
	gWORLD[pMob->mapNum]->MobCnt[pMob->mobNum]--;

	if( pMob->pCreater )
		pMob->pCreater->currCnt--;
	
	REMOVE_FROM_LIST( gWORLD[pMob->mapNum]->mob[pMob->posY/dONE_TILE][pMob->posX/dONE_TILE],
		pMob, w_prev, w_next );
	
#ifdef dUSE_NONAIMOB
	if( pMob->isAIMob )
		REMOVE_FROM_LIST( gMOBS.list, pMob, prev, next );
#endif

	INSERT_TO_LIST( gMOBS.memory, pMob, prev, next );

	pMob->state = dMON_STATE_UNUSED;

	gMOBS.total--;
	gMOBS.total_memory++;
}


//
// Function Name: CreateObjectMob
// Date: 2003-02-12
// Description: 
//
BOOL CreateObjectMob( int mobNum, int mapNum, int posX, int posY, int angle )
{
	if( mobNum != dMONSTER_WARCRISTAL &&
		mobNum != dMONSTER_CASTLEDOOR )
		return 0;
		
	sPMOB_DATA newMob = NewMobData();
	
	if( !newMob )
		return 0;
	
	gWORLD[mapNum]->mobIdxCnt++;
	newMob->idxNum = gWORLD[mapNum]->mobIdxCnt;
	
	if( gWORLD[mapNum]->mobIdxCnt > dMAX_MOB_IDX )
		gWORLD[mapNum]->mobIdxCnt = 1;
	
	//
	newMob->mobNum = mobNum;	
	newMob->mapNum = mapNum;											// 맵 번호
	
	newMob->hp = g_MOBINFO[mobNum]->hp;
	newMob->mp = 0;
	
	newMob->posX = posX;
	newMob->posY = posY;												// x , y 
	newMob->destPosX = posX;
	newMob->destPosY = posY;

	newMob->moveDir = 0;												// 이동 방향
	
	newMob->state = dMON_STATE_NORMAL;									// 몹 상태
	
	newMob->AICheckTIme = g_CurrTime;
	newMob->isObject = 1;
	newMob->objAngle = angle;
	
	newMob->move_speed = 0;
	newMob->move_range = 0;
	newMob->move_speed2 = 0;
	newMob->move_range2 = 0;
			
	newMob->lastMoveTime = g_CurrTime;
	newMob->lastAttackTime = g_CurrTime;
	
	newMob->attackDelay = 0;
	newMob->blockCnt++;		
	
	newMob->isDead = 0;										// 죽언니 사란니
	
	newMob->effectIdx = dMAGIC_IDX_SEED;					// 마법 IDX
	
#ifdef dUSE_NONAIMOB
	newMob->isAIMob = 1;									// AI 몬스터 인지
	newMob->AiTick = 0;										// AI Tick
#endif
	
	int i;
	
	// Save experience
	for( i = 0; i < dMAX_SAVE_MOB_EXP; i++ )
	{
		newMob->saveExp[i].expDamage[0] = 0;
		newMob->saveExp[i].expDamage[1] = 0;
		newMob->saveExp[i].expDamage[2] = 0;
		newMob->saveExp[i].expDamage[3] = 0;
		newMob->saveExp[i].pPlayer = NULL;
	}
	
	// 이펙트
	for( i = 0; i < dMAX_MOB_EFFECT; i++ )
		newMob->AttrEffectTime[i] = 0;
	
	//
	newMob->next = NULL;
	newMob->prev = NULL;
	
	newMob->w_next = NULL;
	newMob->w_prev = NULL;
	
	newMob->enemy = NULL;
	newMob->pCreater = NULL;
	
	INSERT_TO_LIST( gMOBS.list, newMob, prev, next );
	
	gMOBS.total++;
	INSERT_TO_LIST( gWORLD[mapNum]->mob[posY/dONE_TILE][posX/dONE_TILE], newMob, w_prev, w_next );
	
	gWORLD[mapNum]->currMobCnt++;
	gWORLD[mapNum]->MobCnt[mobNum]++;
		
	MakeMobInPacket( newMob, 0 );
	SendToMobArea( newMob, g_Packet, g_nPos );
	return 1;
}

//
// Function Name: CreateMob
// Date: 2003-02-12
// Description: 
//
BOOL CreateMob( int mobNum, int mapNum, int posX, int posY , sPMOB_RESET pCreater , char *filename , int line )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return 0;

	if( mobNum < 1 || mobNum >= dMAX_MOBINFO )
		return 0;

	if( !g_MOBINFO[mobNum] )
	{
		log( "CreateMob : bad mobNum [%s/%d]\r\n", filename, line );
		return 0;
	}

	sPMOB_DATA newMob = NewMobData();

	if( !newMob )
		return 0;

	gWORLD[mapNum]->mobIdxCnt++;
	newMob->idxNum = gWORLD[mapNum]->mobIdxCnt;

	if( gWORLD[mapNum]->mobIdxCnt > dMAX_MOB_IDX )
		gWORLD[mapNum]->mobIdxCnt = 1;

	//
	newMob->mobNum = mobNum;
	
	newMob->mapNum = mapNum;											// 맵 번호
	
	newMob->hp = g_MOBINFO[mobNum]->hp; 
	newMob->mp = g_MOBINFO[mobNum]->mp;

	newMob->posX = posX;
	newMob->posY = posY;												// x , y 
	newMob->destPosX = posX;
	newMob->destPosY = posY;
	
	newMob->moveDir = number( 0 , 7 );									// 이동 방향
					
	newMob->state = dMON_STATE_NORMAL;									// 몹 상태

	newMob->AICheckTIme = g_CurrTime;
	newMob->isObject = 0;
	newMob->objAngle = 0;

	switch( g_MOBINFO[mobNum]->moveSpeed )
	{
	case 0:							// 매우느림  
		newMob->move_speed = 500;	// 2 m/s
		newMob->move_range = 12;
		break;
	case 1:							// 느림
		newMob->move_speed = 357;   // 3 m/s
		newMob->move_range = 20;
		break;
	case 2:							// 보통 
		newMob->move_speed = 232;   // 4 m/s
		newMob->move_range = 28;
		break;		
	case 3:							// 빠름 
		newMob->move_speed = 172;   // 5.5 m/s
		newMob->move_range = 43;
		break;
	case 4:							// 매우빠름
		newMob->move_speed = 122;	// 7 m/s
		newMob->move_range = 58;
		break;
	}

//	newMob->move_speed -= 100;
	
	switch( g_MOBINFO[mobNum]->traceSpeed )
	{
	case 0:							// 매우느림  Very slow
		newMob->move_speed2 = 500;	// 2 m/s
		newMob->move_range2 = 12;
		break;
	case 1:							// 느림 Slow
		newMob->move_speed2 = 357;   // 3 m/s
		newMob->move_range2 = 20;
		break;
	case 2:							// 보통 usually
		newMob->move_speed2 = 232;   // 4 m/s
		newMob->move_range2 = 28;
		break;		
	case 3:							// 빠름 Speed
		newMob->move_speed2 = 172;   // 5.5 m/s
		newMob->move_range2 = 43;
		break;
	case 4:							// 매우빠름 Very Fast
		newMob->move_speed2 = 122;	// 7 m/s
		newMob->move_range2 = 58;
		break;
	}
	
//	newMob->move_speed2 -= 100;

	newMob->lastMoveTime = g_CurrTime;
	newMob->lastAttackTime = g_CurrTime;

	switch( g_MOBINFO[mobNum]->attackSpeed )
	{
	case 0:								// 매우느림 
		newMob->attackDelay = 2100;
		break;
	case 1:								// 느림
		newMob->attackDelay = 1800;
		break;
	case 2:								// 보통
		newMob->attackDelay = 1500;
		break;
	case 3:								// 빠름
		newMob->attackDelay = 1200;
		break;
	case 4:								// 매우빠름
		newMob->attackDelay = 1000;
		break;
	}

	newMob->blockCnt++;		

	newMob->isDead = 0;										// 죽언니 사란니
	newMob->nHit = 0;										// 파티사냥시 맞은 횟수

	newMob->effectIdx = dMAGIC_IDX_SEED;					// 마법 IDX

#ifdef dUSE_NONAIMOB
	newMob->isAIMob = 1;									// AI 몬스터 인지
	newMob->AiTick = 0;										// AI Tick
#endif

	int i;

	// 경험치 저장
	for( i = 0; i < dMAX_SAVE_MOB_EXP; i++ )
	{
		newMob->saveExp[i].expDamage[0] = 0;
		newMob->saveExp[i].expDamage[1] = 0;
		newMob->saveExp[i].expDamage[2] = 0;
		newMob->saveExp[i].expDamage[3] = 0;
		newMob->saveExp[i].pPlayer = NULL;
	}

	// 이펙트
	for( i = 0; i < dMAX_MOB_EFFECT; i++ )
		newMob->AttrEffectTime[i] = 0;

	//
	newMob->next = NULL;
	newMob->prev = NULL;

	newMob->w_next = NULL;
	newMob->w_prev = NULL;

	newMob->enemy = NULL;

	newMob->deadTimer = 0;

	if( pCreater )
	{
		newMob->pCreater = pCreater;
		pCreater->currCnt++;
	}
	else
		newMob->pCreater = NULL;
	
	INSERT_TO_LIST( gMOBS.list, newMob, prev, next );

	gMOBS.total++;
	INSERT_TO_LIST( gWORLD[mapNum]->mob[posY/dONE_TILE][posX/dONE_TILE], newMob, w_prev, w_next );
		
	gWORLD[mapNum]->currMobCnt++;
	gWORLD[mapNum]->MobCnt[mobNum]++;

	MakeMobInPacket( newMob , 1 );
	SendToMobArea( newMob, g_Packet, g_nPos );
	
	return 1;
}

//
// Function Name: FindMoInTile
// Date: 2003-02-12
// Description: 
//
sPMOB_DATA FindMobInTile( sPMOB_DATA list, int mobNum )
{
	sPMOB_DATA m, next_m;
	
	LIST_WHILE( list, m, next_m , w_next, WMgr );
	if( m->idxNum == mobNum )
		return m;
	LIST_WHILEEND( list, m, next_m );

	return NULL;
}

//
// Function Name: FindMobInArea
// Date: 2003-02-28
// Description: 
//
sPMOB_DATA FindMobInArea( int mapNum, int posX, int posY, int mobNum )
{
	sPMOB_DATA m;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				m = FindMobInTile( gWORLD[mapNum]->mob[y][x], mobNum );

				if( m )
					return m;
			}
		}
	}
	return NULL;
}


//
// Function Name: SendMobState
// Date: 2003-02-28
// Description: 
//
void SendMobState( sPMOB_DATA pMob )
{
	if( !pMob )
		return;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MOB_STATE, g_nPos );
	PutWord( g_Packet, pMob->idxNum, g_nPos );
	PutByte( g_Packet, pMob->enemy ? 1 : 0, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToMobArea( pMob, g_Packet, g_nPos );
}



//
// Function Name: RandomTrans
// Date: 
// Description: 
//
void SendMobEffect( sPMOB_DATA pMob, WORD effect )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_MOB_POPEFFECT, g_nPos );
	PutInteger( g_Packet, pMob->idxNum, g_nPos );
	PutWord( g_Packet, effect, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToMobArea( pMob, g_Packet, g_nPos );
}


//EOF
