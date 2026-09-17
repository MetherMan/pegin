/**
 *	$Workfile: fight.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 51 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/fight.cpp $
 * 
 * 51    04-12-22 5:18p Sk8snow
 * 
 * 50    04-12-15 10:40a Sk8snow
 * 
 * 49    04-11-18 1:40a Sk8snow
 * 
 * 48    04-10-29 11:56p Sk8snow
 * 
 * 47    04-10-19 11:14p Sk8snow
 * 
 * 46    04-09-20 10:53p Sk8snow
 * 
 * 45    04-09-11 11:20p Sk8snow
 * 
 * 44    04-09-08 3:54p Sk8snow
 * 
 * 43    04-08-04 9:27a Sk8snow
 * 
 * 42    04-07-13 7:26a Sk8snow
 * 
 * 41    04-05-29 10:12p Sk8snow
 * 
 * 40    04-05-21 11:37a Sk8snow
 * 
 * 39    04-05-18 1:17a Sk8snow
 * 
 * 38    04-05-14 10:38p Sk8snow
 * 
 * 37    04-05-11 1:45a Sk8snow
 * 
 * 36    04-04-30 9:46a Sk8snow
 * 
 * 35    04-04-07 10:17p Sk8snow
 * 
 * 34    04-04-03 4:03p Sk8snow
 * 
 * 33    04-03-31 9:19a Sk8snow
 * CheckDeadMob - ITEM_EVENT_040321 아이템 드랍처리 삭제
 * 
 * 32    04-03-31 8:22a Sk8snow
 *         
 */

#include "def.h"
#include "structs.h"
#include "extern.h"
#include "message.h"

#include "world.h"
#include "player.h"
#include "monster.h"

#include "fight.h"
#include "skill.h"

#include "item.h"

#include "party.h"

#include "effect.h"
#include "pet.h"

#include "war.h"
#include "guild.h"

#include "billing.h"

sDAMAGE_TABLE g_DAMAGE_TABLE[dMAX_DAMAGE_TABLE];


//
// Function Name: LoadDamageTable
// Date:
// Description: 
//
BOOL LoadDamageTable()
{
	for( int i = 0; i < dMAX_DAMAGE_TABLE; i++ )
	{		
		g_DAMAGE_TABLE[i].minStrDamage = 0;			//공격력(힘)
		g_DAMAGE_TABLE[i].maxStrDamage = 0;
		g_DAMAGE_TABLE[i].minDexDamage = 0;			//공격력(민첩)
		g_DAMAGE_TABLE[i].maxDexDamage = 0;
		g_DAMAGE_TABLE[i].minIntDamage = 0;			//공격력(지능)
		g_DAMAGE_TABLE[i].maxIntDamage = 0;
		
		g_DAMAGE_TABLE[i].minSAttackDamage = 0;		//근거리(공)
		g_DAMAGE_TABLE[i].maxSAttackDamage = 0;	
		g_DAMAGE_TABLE[i].minLAttackDamage = 0;		//원거리(공)
		g_DAMAGE_TABLE[i].maxLAttackDamage = 0;	
		g_DAMAGE_TABLE[i].minBMagicDamage = 0;		//흑마법(공)
		g_DAMAGE_TABLE[i].maxBMagicDamage = 0;	
		g_DAMAGE_TABLE[i].minWMagicDamage = 0;		//백마법(공)
		g_DAMAGE_TABLE[i].maxWMagicDamage = 0;	

#ifdef _dUMENOKOJI_									//마방관련
		g_DAMAGE_TABLE[i].SAttackDefense = 0;
		g_DAMAGE_TABLE[i].LAttackDefense = 0;

		g_DAMAGE_TABLE[i].BMagicDefense = 0;
		g_DAMAGE_TABLE[i].WMagicDefense = 0;
#endif
		g_DAMAGE_TABLE[i].SAttackArmor = 0;			// 근거리 방어
		g_DAMAGE_TABLE[i].LAttackArmor = 0;			// 원거리 방어 
		g_DAMAGE_TABLE[i].BMagicArmor = 0;			// 흑마법 방어 
		g_DAMAGE_TABLE[i].WMagicArmor = 0;			// 백마법 방어
	}
	
	int		minStrDamage;		//공격력(힘)
	int		maxStrDamage;
	int		minDexDamage;		//공격력(민첩)
	int		maxDexDamage;
	int		minIntDamage;		//공격력(지능)
	int		maxIntDamage;
	
	int		minSAttackDamage;	//근거리(공)
	int		maxSAttackDamage;	
	int		minLAttackDamage;	//원거리(공)
	int		maxLAttackDamage;	
	int		minBMagicDamage;	//흑마법(공)
	int		maxBMagicDamage;	
	int		minWMagicDamage;	//백마법(공)
	int		maxWMagicDamage;	

	int		SAttackArmor;		// 근거리 방어
	int		LAttackArmor;		// 원거리 방어 
	int		BMagicArmor;		// 흑마법 방어 
	int		WMagicArmor;		// 백마법 방어

#ifdef _dUMENOKOJI_				//마방관련
	
	int		SAttackDefense;		//근거리 방어
	int		LAttackDefense;		//원거리 방어
	int		BMagicDefense;		//흑마법 방어
	int		WMagicDefense;		//백마법 방어

	FILE *fp = fopen( dDAMAGE_TABLE_FILE, "rb" );		//테스트서버용 damageTable
#else
	FILE *fp = fopen( dDAMAGE_TABLE_FILE, "rb" );	//본 서버용 damageTable 본섭에 적용시에는 DATA/폴더에있는 damageTable과 tdamageTable를 바꿔주어야함.
#endif

	if( !fp )
	{
		log( "LoadDamageTable : Load damage_table.txt failed\r\n" );
		return 0;
	}


#ifdef _dUMENOKOJI_
	char readBuff[1024];
	
	int level = 0;
	
	while( !feof( fp ) )
	{
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		if( sscanf( readBuff, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			&minStrDamage,		//공격력(힘)
			&maxStrDamage,
			&minDexDamage,		//공격력(민첩)
			&maxDexDamage,
			&minIntDamage,		//공격력(지능)
			&maxIntDamage,
			
			&minSAttackDamage,	//근거리(공)
			&maxSAttackDamage,	
			&minLAttackDamage,	//원거리(공)
			&maxLAttackDamage,	
			&SAttackDefense	,				//근거리(방)
			&LAttackDefense	,				//원거리(방)
			&minBMagicDamage,	//흑마법(공)
			&maxBMagicDamage,	
			&minWMagicDamage,	//백마법(공)
			&maxWMagicDamage,
			&BMagicDefense	,				//흑마법(방)
			&WMagicDefense	,				//백마법(방)
			
			&SAttackArmor,		// 근거리 방어 
			&LAttackArmor,		// 원거리 방어 
			&BMagicArmor,		// 흑마법 방어 
			&WMagicArmor		// 백마법 방어
			) != 22 )
		{
			log( "LoadDamageTable : Bad File Format\r\n" );
			fclose(fp);
			return 0;
		}
		
		
#else
		char readBuff[1024];
		
		int level = 0;
		
		while( !feof( fp ) )
		{
			fgets( readBuff, 1024, fp );
			
			if( *readBuff == ';' )
				continue;
			
			if( *readBuff == '\n' ||
				*readBuff == '\r' ||
				*readBuff == '\0' )
				break;
		if( sscanf( readBuff, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
					&minStrDamage,		//공격력(힘)
					&maxStrDamage,
					&minDexDamage,		//공격력(민첩)
					&maxDexDamage,
					&minIntDamage,		//공격력(지능)
					&maxIntDamage,
		
					&minSAttackDamage,	//근거리(공)
					&maxSAttackDamage,	
					&minLAttackDamage,	//원거리(공)
					&maxLAttackDamage,	
					&minBMagicDamage,	//흑마법(공)
					&maxBMagicDamage,	
					&minWMagicDamage,	//백마법(공)
					&maxWMagicDamage,

					&SAttackArmor,		// 근거리 방어 
					&LAttackArmor,		// 원거리 방어 
					&BMagicArmor,		// 흑마법 방어 
					&WMagicArmor		// 백마법 방어
					) != 18 )
		{
			log( "LoadDamageTable : Bad File Format\r\n" );
			fclose(fp);
			return 0;
		}
		
		
#endif

		if( level >= dMAX_DAMAGE_TABLE )
		{
			log( "LoadDamageTable : level >= dMAX_DAMAGE_TABLE \r\n" );
			fclose(fp);
			return 0;
		}
		
#ifdef _dUMENOKOJI_				//마방관련
		g_DAMAGE_TABLE[level].minStrDamage = minStrDamage;			//공격력(힘)
		g_DAMAGE_TABLE[level].maxStrDamage = maxStrDamage;
		g_DAMAGE_TABLE[level].minDexDamage = minDexDamage;			//공격력(민첩)
		g_DAMAGE_TABLE[level].maxDexDamage = maxDexDamage;
		g_DAMAGE_TABLE[level].minIntDamage = minIntDamage;			//공격력(지능)
		g_DAMAGE_TABLE[level].maxIntDamage = maxIntDamage;
		
		g_DAMAGE_TABLE[level].minSAttackDamage = minSAttackDamage;		//근거리(공)
		g_DAMAGE_TABLE[level].maxSAttackDamage = maxSAttackDamage;	
		g_DAMAGE_TABLE[level].minLAttackDamage = minLAttackDamage;		//원거리(공)
		g_DAMAGE_TABLE[level].maxLAttackDamage = maxLAttackDamage;
		g_DAMAGE_TABLE[level].SAttackDefense	= SAttackDefense;		//근거리(방)
		g_DAMAGE_TABLE[level].LAttackDefense	= LAttackDefense;		//원거리(방)
		g_DAMAGE_TABLE[level].minBMagicDamage = minBMagicDamage;		//흑마법(공)
		g_DAMAGE_TABLE[level].maxBMagicDamage = maxBMagicDamage;	
		g_DAMAGE_TABLE[level].minWMagicDamage = minWMagicDamage;		//백마법(공)
		g_DAMAGE_TABLE[level].maxWMagicDamage = maxWMagicDamage;	
		g_DAMAGE_TABLE[level].BMagicDefense	= BMagicDefense;			//흑마법(방)
		g_DAMAGE_TABLE[level].WMagicDefense	= WMagicDefense;			//백마법(방)

#else
		g_DAMAGE_TABLE[level].minStrDamage = minStrDamage;			//공격력(힘)
		g_DAMAGE_TABLE[level].maxStrDamage = maxStrDamage;
		g_DAMAGE_TABLE[level].minDexDamage = minDexDamage;			//공격력(민첩)
		g_DAMAGE_TABLE[level].maxDexDamage = maxDexDamage;
		g_DAMAGE_TABLE[level].minIntDamage = minIntDamage;			//공격력(지능)
		g_DAMAGE_TABLE[level].maxIntDamage = maxIntDamage;
		
		g_DAMAGE_TABLE[level].minSAttackDamage = minSAttackDamage;		//근거리(공)
		g_DAMAGE_TABLE[level].maxSAttackDamage = maxSAttackDamage;	
		g_DAMAGE_TABLE[level].minLAttackDamage = minLAttackDamage;		//원거리(공)
		g_DAMAGE_TABLE[level].maxLAttackDamage = maxLAttackDamage;	
		g_DAMAGE_TABLE[level].minBMagicDamage = minBMagicDamage;		//흑마법(공)
		g_DAMAGE_TABLE[level].maxBMagicDamage = maxBMagicDamage;	
		g_DAMAGE_TABLE[level].minWMagicDamage = minWMagicDamage;		//백마법(공)
		g_DAMAGE_TABLE[level].maxWMagicDamage = maxWMagicDamage;	
#endif
		
		g_DAMAGE_TABLE[level].SAttackArmor = SAttackArmor;
		g_DAMAGE_TABLE[level].LAttackArmor = LAttackArmor;
		g_DAMAGE_TABLE[level].BMagicArmor = BMagicArmor;
		g_DAMAGE_TABLE[level].WMagicArmor = WMagicArmor;


		if( level == (dMAX_DAMAGE_TABLE-1) )
			break;

		level++;
	}
	fclose( fp );
	return 1;
}

//
// Function Name: CheckAllowAttack
// Date: 2003-05-22
// Description: 
//
int CheckAllowAttack( sPDESC_DATA pPlayer )
{
	// TODO : 0 = 공격 불가능
	//        1 = 공격 가능
	//        2 = MP 부족
	//		  3 = LEV 부족

	if( !IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
		return 0;

	switch( GET_USE_SKILL( pPlayer ) )
	{
	case dSATTACK_SKILL_ITEM:
		if( !pPlayer->equip[dEQUIP_WEAPON] )
			return 0;
		else
			return 1;
		break;
	case dLATTACK_SKILL_ITEM:
		if( !pPlayer->equip[dEQUIP_BOW] )
			return 0;
		else
			return 1;
		break;
	default:
		return 0;
	}
	return 1;
}

//
// Function Name: set_fighting
// Date: 2003-05-22
// Description: 
//
void set_fighting( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return;

	if( pMob->isDead )
		return;

	if( !IsValidAttackArea( pPlayer, pMob ) )
		return;

	int checkValue = CheckAllowAttack( pPlayer );

	if( checkValue == 3 || checkValue == 0 )
	{
//		log( "set_fighting : checkValue == 3 || checkValue == 0\r\n" );
		return;
	}

	if( IsPlayerAttackRange( pPlayer, pMob ) )
	{
		if( pMob->enemy )
		{
			if( pMob->enemy->enemyMob != pMob )
			{
				SET_MOB_ENEMY( pMob, pPlayer );
				SendMobState( pMob );
			}
			pPlayer->enemyMob = pMob;
		}
		else
		{
			SET_MOB_ENEMY( pMob, pPlayer );
			SendMobState( pMob );
			pPlayer->enemyMob = pMob;
		}
		
		GET_STATE( pPlayer ) = dDESC_STATE_FIGHT;

		AttackToMob( pPlayer, pMob );
	}
//	if( IsPlayerAttackRange( pPlayer, pMob ) )
//		AttackToMob( pPlayer, pMob );
}

//
// Function Name: set_pvp_fight
// Date: 
// Description: 
//
void set_pvp_fight( sPDESC_DATA pAttacker, sPDESC_DATA pVict )
{
#ifndef dNON_PVP_MODE
	if( pAttacker == pVict )
		return;

#ifdef AUTO_ATTACK
	if( pAttacker->victPlayer == pVict )
		return;
#endif

//	if( GET_CHA( pAttacker ) <= -30000 )
//	{
//		SendSystemMsg( pAttacker, g_LANG_STR[285] );
//		return;
//	}

	if( !IsValidPvPAttackArea( pAttacker, pVict ) )
	{
		SendSystemMsg( pAttacker, g_LANG_STR[0] );
		return;
	}

	if( pAttacker->enemyMob )
		pAttacker->enemyMob = NULL;

	pAttacker->victPlayer = pVict;

	AttackPvP( pAttacker, pVict );
#endif
}

//
// Function Name: stop_fighting_player
// Date: 2003-03-20
// Description: 
//
void stop_fighting_player( sPDESC_DATA pPlayer )
{
	if( pPlayer->enemyMob )
	{
		// 맞짱 중이면
		if( pPlayer->enemyMob->enemy == pPlayer )
		{
			SET_MOB_ENEMY( pPlayer->enemyMob, NULL );
			SendMobState( pPlayer->enemyMob );
			pPlayer->enemyMob = NULL;			
		}
		else
			pPlayer->enemyMob = NULL;
	}

	if( pPlayer->victPlayer )
		pPlayer->victPlayer = NULL;

	GET_STATE( pPlayer ) = dDESC_STATE_PLAYING;

	if( pPlayer->ch2.waitSkill.skillNum )
	{
		pPlayer->ch2.waitSkill.skillNum = 0;
		pPlayer->ch2.waitSkill.targetIdx = 0;
		pPlayer->ch2.waitSkill.targetType = 0;
	}
}


//
// Function Name: stop_fighting_mob
// Date: 2003-03-20
// Description: 
//
void stop_fighting_mob( sPMOB_DATA pMob )
{
	if( pMob->enemy )
	{
		if( pMob->enemy->enemyMob == pMob )
		{
			pMob->enemy->enemyMob = NULL;
			if( GET_STATE( pMob->enemy ) == dDESC_STATE_FIGHT )
				GET_STATE( pMob->enemy ) = dDESC_STATE_PLAYING; 
		}
		SET_MOB_ENEMY( pMob, NULL );
		SendMobState( pMob );
	}
}


//
// Function Name: stop_fighting_enemy
// Date: 2003-03-20
// Description: 몬스터 데이터용 
//
void stop_fighting_enemy( sPMOB_DATA pMob )
{
	if( !IS_VALID_WORLD( pMob->mapNum ) )
		return;

	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);

	sPDESC_DATA d, next_d;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{				
				LIST_WHILE( gWORLD[pMob->mapNum]->player[y][x], d, next_d , w_next, WMgr );
					
				if( d->enemyMob == pMob )
					stop_fighting_player( d );

				LIST_WHILEEND( gWORLD[pMob->mapNum]->player[y][x], d, next_d );
			}
		}
	}
}


//
// Function Name: stop_fighting_enemy
// Date: 2003-03-20
// Description: 플레이어 데이터용 
//
void stop_fighting_enemy( sPDESC_DATA pPlayer )
{
	if( !IS_VALID_WORLD( pPlayer->ch.mapNum ) )
		return;

	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);

	sPMOB_DATA m, next_m;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{				
				LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m , w_next, WMgr );
					
				if( m->enemy == pPlayer )
					stop_fighting_mob( m );

				LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m );
			}
		}
	}
}


//
// Function Name: SendMobDamage
// Date: 2003-03-20
// Description: 몹 -> 플레이어 타격 메시지
//
void SendMobDamage( sPMOB_DATA pMob, sPDESC_DATA pPlayer, WORD damage , BYTE isCritical )
{
	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_DAMAGE_MOB, g_nPos );
	PutWord( g_Packet, pMob->idxNum, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, damage, g_nPos );
	PutByte( g_Packet, isCritical, g_nPos );
	PutWord( g_Packet, 0, g_nPos );		// 마법 번호
	PutWord( g_Packet, 0, g_nPos );		// 마법 인덱스

	PutInteger( g_Packet, pMob->attackDelay - 300, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToRange( pPlayer , g_Packet, g_nPos, dATTACK_PACKET_RANGE );
}


//
// Function Name: SendBeginPlayerAttack
// Date: 2003-05-06
// Description: 
//
void SendBeginPlayerAttack( sPDESC_DATA pPlayer , int idxNum, BYTE targetKind )
{
	if( !IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
		return;

	int attackDelay = GetAttackDelay( pPlayer );

	pPlayer->ch2.actionDelay = g_CurrTime + attackDelay;

#ifndef AUTO_ATTACK
	attackDelay += 100;
#endif

	WORD skillNum;
	
	switch( GET_USE_SKILL( pPlayer ) )
	{
	case dSATTACK_SKILL_ITEM:
		skillNum = 0;
		break;
	case dLATTACK_SKILL_ITEM:
		skillNum = 1;
		break;
	default:
		return;		
	}
	
	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_BEGIN_PLAYER_ATTACK, g_nPos );
	PutInteger( g_Packet, idxNum, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, skillNum, g_nPos );
	PutByte( g_Packet, targetKind, g_nPos );
	PutInteger( g_Packet, attackDelay, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendToRange( pPlayer, g_Packet, g_nPos, dATTACK_PACKET_RANGE );
	
	if( GET_PARTY_MASTER( pPlayer ) )
		PARTY_SendHPMP( pPlayer );
}



//
// Function Name: SendMobDamage
// Date: 2003-03-20
// Description: 플레이어 -> 몹 타격 메시지
//
void SendPlayerDamage( sPDESC_DATA pPlayer , sPMOB_DATA pMob, WORD damage , BYTE isCritical )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_DAMAGE_USER, g_nPos );
	PutWord( g_Packet, pMob->idxNum, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, damage, g_nPos );
	PutByte( g_Packet, isCritical, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToRange( pPlayer, g_Packet, g_nPos, dATTACK_PACKET_RANGE );

	// HP Info
	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_MOB_HPINFO, g_nPos );
	PutWord( g_Packet, pMob->idxNum, g_nPos );
	PutInteger( g_Packet, pMob->hp, g_nPos );
	PutInteger( g_Packet, GET_MOB_HP( pMob ), g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: SendPvPDamage
// Date: 
// Description: 
//
void SendPvPDamage( sPDESC_DATA pAttacker, sPDESC_DATA pVict, WORD damage, BYTE isCritical )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_DAMAGE_PVP, g_nPos );
	PutInteger( g_Packet, GET_IDX( pVict ), g_nPos );
	PutInteger( g_Packet, GET_IDX( pAttacker ), g_nPos );
	PutWord( g_Packet, damage, g_nPos );
	PutByte( g_Packet, isCritical, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToRange( pAttacker, g_Packet, g_nPos, dATTACK_PACKET_RANGE );
}



//
// Function Name: AttackToPlayer
// Date: 2003-03-29
// Description: 
//
void AttackToPlayer( sPMOB_DATA pMob, sPDESC_DATA pPlayer, BYTE divid )
{
	if( !pMob || !pPlayer )
	{
		log( "AttackToPlayer : AttackToPlayer !pMob || !pPlayer\r\n" );
		return;
	}

	// Object Mob
	if( pMob->mobNum > 1000 )
		return;

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
	{
		stop_fighting_mob( pMob );
		return;
	}
	
	WORD MobDamage = 0;
	int playerDP = 0;

	// 버로우나 스텔스
	UnSetVisibleSkill( pPlayer );

	int rndValue = 0;

	if( GET_MOB_LEVEL( pMob ) >= GET_ALL_SKILL( pPlayer ) )
		rndValue = 0;
	else
		rndValue = GET_ALL_SKILL( pPlayer ) - GET_MOB_LEVEL( pMob );

	if( rndValue && number( 1, 1000 ) <= rndValue )
		MobDamage = 0;
	else
	{
#ifdef _dUMENOKOJI_		//마방관련
		int	tmpDamage = 0;
//		int	tmpGetSkillArmorDamage = 0; //스킬중에 방어력이 쓰이는지 확인할때 쓰인다.
		/*
		몹이 마법이고 플레이가 물리면 데미지는 마법방어
		몹이 마법이고 플레이거 마법이면 데미지는 마법
		몹이 물리고 플레이가 물리면 물리
		몹이 물리고 플레이가 마법이면 물리
		*/
		if( GET_MOB_ATTACK_ATTR( pMob ) == MOB_ATTR_PHYSICS ) //몹이 마법속성이 아니면...
		{
			//몬 얻어오기
			tmpDamage = number( GET_MOB_MINDAMAGE( pMob ), GET_MOB_MAXDAMAGE( pMob ) );
			BYTE	btClass = GetPlayerClass( pPlayer );
			//전사및궁수는 물리 방어력에 +
			if( btClass ==1 || btClass == 2 )
			{
				playerDP += (int)( GetPlayerItemDepense( pPlayer ) / 2 ); 
				playerDP += GetSkillAddArmor( pPlayer, playerDP );
			}
			//법사이면.. 마법 방어력에 +
			else if( btClass == 3 )
			{
				playerDP += (int)( GetPlayerItemDepense( pPlayer ) / 2 );
				playerDP += GetSkillAddArmor( pPlayer, playerDP );
			}

			if( GET_tDAMAGE( ) == TRUE || GET_tMDAMAGE( ) == TRUE )
			{
				playerDP += (int)( playerDP * 0.1 );
				INIT_tDAMGE(  );
			}			
		}
			
		else if( GET_MOB_ATTACK_ATTR( pMob ) == MOB_ATTR_MAGIC )
		{
			//몬스터의 물리 데미지 얻어오기
			tmpDamage = number( GET_MOB_MINMAGICDAMAGE( pMob ), GET_MOB_MAXMAGICDAMAGE( pMob ) );
			BYTE	btClass = GetPlayerClass( pPlayer );
			//전사및궁수는 물리 방어력에 +
			if( btClass ==1 || btClass == 2 )
			{
				playerDP += (int)( GetPlayerItemMagicDepense( pPlayer ) / 2 );
				playerDP += GetSkillAddArmor( pPlayer, playerDP );
			}
			//법사이면.. 마법 방어력에 +
			else if( btClass == 3 )
			{
				playerDP += (int)( GetPlayerItemMagicDepense( pPlayer ) / 2 );
				playerDP += GetSkillAddArmor( pPlayer, playerDP );
			}
	
			if( GET_tDAMAGE( ) == TRUE || GET_tMDAMAGE( ) == TRUE )
			{
				playerDP += (int)( playerDP * 0.1 );
				INIT_tDAMGE(  );
			}
		
			//마나 베리어 링을 장착하고 있다면
			if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_BARRIER ) )
			{
				playerDP += (int)( tmpDamage * 0.11 );
			}

		}	
#else		
		int tmpDamage = number( GET_MOB_MINDAMAGE( pMob ), GET_MOB_MAXDAMAGE( pMob ) ); 

#endif

#ifdef DAMAGE_TEST
		//playerDP = GetPlayerItemDepense( pPlayer );
#else
		//playerDP = (int)( GetPlayerItemDepense( pPlayer ) / 2 );  //바로 위에서서 작업함...
#endif	
		//playerDP += GetSkillAddArmor( pPlayer, playerDP );

		//버프 스킬 체크하고 적용
		//클라우드실드   3  6%, 브라이트 실드  14 10%, 프로텍션 실드  15 14%
		if( pPlayer->ch2.skillEffect[3].delay > 0 )
		{
			playerDP += (int)( tmpDamage * 0.06 );
		}
		if( pPlayer->ch2.skillEffect[14].delay > 0 )
		{
			playerDP += (int)( tmpDamage * 0.15 );
		}
		if( pPlayer->ch2.skillEffect[15].delay > 0 )
		{
			playerDP += (int)( tmpDamage * 0.25 );
		}

		
		if( GET_RIDE_STATE( pPlayer ) && playerDP )
		{
			switch( GET_RIDE_TYPE( pPlayer ) )
			{
			case dRIDE_TYPE1:		// 갈색말
				playerDP -= (int)(playerDP*0.2);		
				break;
			case dRIDE_TYPE2:		// 검정말
				playerDP -= (int)(playerDP*0.15);
				break;
			case dRIDE_TYPE3:		// 백색말
				playerDP -= (int)(playerDP*0.1);
				break;
			}
		}
		
		MobDamage = MIN( tmpDamage - playerDP, 0 );

		if( MobDamage < (WORD)( tmpDamage * 0.1 ) )
		{
			if( number( 1, 100 ) < 95 )
				MobDamage = (WORD)( tmpDamage * 0.1 );
			else
				MobDamage = 0;
		}
	}

	BYTE isCritical = dCRITICAL_NONE;

	if( MobDamage > 0 )
	{
#ifndef dNOT_USE_PET
		if( IS_USING_PET( pPlayer ) )
		{
			int damPerMax = (int)( GET_MAX_HP( pPlayer ) / 100 );
			
			int pluseValue = (int)( ( MobDamage / MIN( damPerMax, 1 ) ) * 10 );
			
			GET_PET_ANGERGAGE( pPlayer ) += MIN( pluseValue, 1 );
			
			PET_UpdatePetGage( pPlayer );
		}
#endif

		if( divid > 1 )
			MobDamage = MIN( MobDamage / divid, 1 );
		
		// 더스트 스킨
		if( pMob->AttrEffectTime[MOB_ATTREFFECT_DUSTSKIN] )
		{
			if( pMob->AttrEffectTime[MOB_ATTREFFECT_DUSTSKIN] + 60 > g_CurrRealTime )
				MobDamage -= (int)( MobDamage * 0.1 );
			else
				pMob->AttrEffectTime[MOB_ATTREFFECT_DUSTSKIN] = 0;
		}
		
		// 반지처리
		if( CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_CHANNELING ) )
		{
			if( number( 1, 100 ) <= 33 &&
				GET_MP( pPlayer ) >= MobDamage )
			{
				GET_MP( pPlayer ) = MIN( GET_MP( pPlayer ) - MobDamage, 0 );
				SendPlayerMP( pPlayer );
			}
			else
			{
				if( !( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_NOHIT ) && pPlayer->adminLevel ) )
				{
					pPlayer->ch.hp -= MobDamage;
					SendPlayerHP( pPlayer );
				}
			}
		}
		else
		{
			if( !( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_NOHIT ) && pPlayer->adminLevel ) )
			{
				pPlayer->ch.hp -= MobDamage;
				SendPlayerHP( pPlayer );
			}
		}			
	}

	SendMobDamage( pMob, pPlayer, MobDamage , isCritical );

	// range delay
	PluseRangeAttackDelay( pMob, pPlayer , 100 );

	// 죽을경우 
	if( pPlayer->ch.hp <= 0 )
	{
		for( int i = 0; i < dMAX_SAVE_MOB_EXP; i++ )
		{
			if( pMob->saveExp[i].pPlayer == pPlayer )
			{
				pMob->saveExp[i].pPlayer = NULL;

				pMob->saveExp[i].expDamage[0] = 0;
				pMob->saveExp[i].expDamage[1] = 0;
				pMob->saveExp[i].expDamage[2] = 0;
				pMob->saveExp[i].expDamage[3] = 0;
			}
		}

		// 레벨 5 이상일 경우 경험치 하강
		if( GET_ALL_SKILL( pPlayer ) >= 5 )
		{
			BYTE noExpDown = 0;
			
#ifdef dDAUM_BILLING
			// 오오라 오브 브릴리언스 ( 경험치 하강 없음 )
			if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM7 ) )
			{
				noExpDown = 1;
			}
			
			if( !noExpDown )
			{
				if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM8 ) )
				{
					noExpDown = 1;
				}
			}
			
			if( !noExpDown )
			{
				if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM9 ) )
				{
					noExpDown = 1;
				}
			}
#endif
			
			if( !noExpDown )
			{
				int topSkill = 0;
				int topSkillValue = GET_SATTACK( pPlayer );
				
				for( int j = 0; j < 4; j++ )
				{
					if( pPlayer->ch.skill[j] > topSkillValue )
					{
						topSkill = j;
						topSkillValue = pPlayer->ch.skill[j];
					}
				}

				pPlayer->ch.skill_exp[topSkill] -= (int)(pPlayer->ch.skill_exp[topSkill]*0.1);
				
				switch( topSkill )
				{
				case 0:
					pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP1] = 1;
					
					if( pPlayer->ch.str_exp )
					{
						pPlayer->ch.str_exp -= (int)(pPlayer->ch.str_exp*0.1);
						pPlayer->isUpdate[dDATA_UPDATE_STREXP] = 1;
					}
					break;
				case 1:
					pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP2] = 1;
					
					if( pPlayer->ch.dex_exp )
					{
						pPlayer->ch.dex_exp -= (int)(pPlayer->ch.dex_exp*0.1);
						pPlayer->isUpdate[dDATA_UPDATE_DEXEXP] = 1;
					}
					break;
				case 2:
				case 3:
					pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP3] = 1;
					
					if( pPlayer->ch.intelli_exp )
					{
						pPlayer->ch.intelli_exp -= (int)(pPlayer->ch.intelli_exp*0.1);
						pPlayer->isUpdate[dDATA_UPDATE_INTEXP] = 1;
					}
					break;
				}
				
				SendSetExpMsg( pPlayer, topSkill, pPlayer->ch.skill_exp[topSkill] );
			}
#ifndef dNON_PVP_MODE
			if( GET_CHA( pPlayer ) > - 59998 )
			{
				GET_CHA( pPlayer ) -= 2;
				pPlayer->isUpdate[dDATA_UPDATE_CHA] = 1;
				SendUpdateCha( pPlayer );
			}
#endif
		} // LEVEL > 5

#ifndef dNON_PVP_MODE
		DropItemByCHA( pPlayer );
#endif
		stop_fighting_player( pPlayer );
		stop_fighting_enemy( pPlayer );
		PlayerDie( pPlayer );
	}
}


//
// Function Name: CalculationMobExp
// Date: 2003-05-22
// Description: 
//
int CalculationMobExp( sPDESC_DATA pPlayer, sPMOB_DATA pMob, WORD damage )
{
	if( !damage )
		return 0;

	int exp = 0;

	exp = (int)( ( (float)damage / (float)GET_MOB_HP( pMob ) ) * (float)GET_MOB_EXP( pMob ) );

	return MAX( exp, GET_MOB_EXP( pMob ) );
}


//
// Function Name: PushPartyExp
// Date: 
// Description: 
//
BOOL PushPartyExp( sPDESC_DATA pPlayer, sPDESC_DATA pTmpPlayer, int splitExp )
{	
	if( !splitExp )
		return 1;

	if( GET_MAP_NUM( pPlayer ) != GET_MAP_NUM( pTmpPlayer ) )
		return 0;
	
	if( GET_DEAD_STATE( pTmpPlayer ) != dDEAD_STATE_NONE )
		return 0;
	
	// 30미터 거리제한
	int rangeX = abs( GET_POSX( pPlayer ) - GET_POSX( pTmpPlayer ) );
	int rangeY = abs( GET_POSY( pPlayer ) - GET_POSY( pTmpPlayer ) );
	
	int rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );
	
	if( rangeValue >= 60 )
		return 0;

	if( GET_USE_SKILL( pTmpPlayer ) == dSATTACK_SKILL_ITEM )
	{
		if( GET_SATTACK( pTmpPlayer ) < dMAX_SKILL_LEVEL )
		{
			pTmpPlayer->ch.skill_exp[0] += splitExp;
			SendGetExpMsg( pTmpPlayer, 0, splitExp );
			pTmpPlayer->ch.str_exp += (int)( splitExp * 0.98 );
			pTmpPlayer->ch.dex_exp += (int)( splitExp * 0.02 );
			
			pTmpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP1] = 1;
			pTmpPlayer->isUpdate[dDATA_UPDATE_STREXP] = 1;
			pTmpPlayer->isUpdate[dDATA_UPDATE_DEXEXP] = 1;
			
#ifndef dNOT_USE_PET
			PET_UpdateExp( pTmpPlayer, (int)(splitExp*0.05) );
#endif
			CheckLevelUp( pTmpPlayer, 0 );
		}
	}
	else if( GET_USE_SKILL( pTmpPlayer ) == dLATTACK_SKILL_ITEM )
	{
		if( GET_LATTACK( pTmpPlayer ) < dMAX_SKILL_LEVEL )
		{
			pTmpPlayer->ch.skill_exp[1] += splitExp;
			SendGetExpMsg( pTmpPlayer, 1, splitExp );
			pTmpPlayer->ch.str_exp += (int)( splitExp * 0.02 );
			pTmpPlayer->ch.dex_exp += (int)( splitExp * 0.98 );
			
			pTmpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP2] = 1;
			pTmpPlayer->isUpdate[dDATA_UPDATE_STREXP] = 1;
			pTmpPlayer->isUpdate[dDATA_UPDATE_DEXEXP] = 1;
			
#ifndef dNOT_USE_PET
			PET_UpdateExp( pTmpPlayer, (int)(splitExp*0.05) );
#endif
			CheckLevelUp( pTmpPlayer, 1 );
		}
	}
	else
	{
		if( GET_ITEM_TYPE2( GET_USE_SKILL( pTmpPlayer ) ) != dITEMTYPE_MAGICSCROLL )
			return 0;
		
		sPSKILL_DATA pSkill = GET_SKILL_DATA( pTmpPlayer );
		
		if( !pSkill )
			return 0;
		
		if( IS_BMAGIC( pSkill ) )
		{
			if( GET_BMAGIC( pTmpPlayer ) < dMAX_SKILL_LEVEL )
			{
				pTmpPlayer->ch.skill_exp[2] += splitExp;
				SendGetExpMsg( pTmpPlayer, 2, splitExp );
				pTmpPlayer->ch.intelli_exp += (int)( splitExp * 0.98 );
				pTmpPlayer->ch.dex_exp += (int)( splitExp * 0.02 );
				
				pTmpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP3] = 1;
				pTmpPlayer->isUpdate[dDATA_UPDATE_INTEXP] = 1;
				pTmpPlayer->isUpdate[dDATA_UPDATE_DEXEXP] = 1;
				
#ifndef dNOT_USE_PET
				PET_UpdateExp( pTmpPlayer, (int)(splitExp*0.05) );
#endif
				CheckLevelUp( pTmpPlayer, 2 );
			}
		}
		else if( IS_WMAGIC( pSkill ) )
		{
			if( GET_WMAGIC( pTmpPlayer ) < dMAX_SKILL_LEVEL )
			{
				pTmpPlayer->ch.skill_exp[3] += splitExp;
				SendGetExpMsg( pTmpPlayer, 3, splitExp );
				pTmpPlayer->ch.intelli_exp += (int)( splitExp * 0.98 );
				pTmpPlayer->ch.dex_exp += (int)( splitExp * 0.02 );
				
				pTmpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP4] = 1;
				pTmpPlayer->isUpdate[dDATA_UPDATE_INTEXP] = 1;
				pTmpPlayer->isUpdate[dDATA_UPDATE_DEXEXP] = 1;
				
#ifndef dNOT_USE_PET
				PET_UpdateExp( pTmpPlayer, (int)(splitExp*0.05) );
#endif
				CheckLevelUp( pTmpPlayer, 3 );
			}
		}
	}
	return 1;
}


//
// Function Name: SplitPartyExp
// Date: 
// Description: 
//
BYTE SplitPartyExp( sPDESC_DATA pPlayer, int exp )
{
	if( !GET_PARTY_MASTER( pPlayer ) ||
		( GET_PARTY_MASTER( pPlayer ) == pPlayer && pPlayer->ch2.party.memberCnt == 0 ) )
		return 0;
	
	int totalLev = 0;

	totalLev += GetCurrSkillLevel( GET_PARTY_MASTER( pPlayer ) );
	totalLev = MIN( totalLev, 1 );

	int i;

	for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
		{
			int tmpLev = GetCurrSkillLevel( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
			totalLev += MIN( tmpLev, 1 );
		}
	}

	int splitExp = 0;
	float seedValue = 0.0F;
	int totalMember = GET_PARTY_MEMBER_CNT( GET_PARTY_MASTER( pPlayer ) ) + 1;
	int realExp = exp + (int)( (float)exp * ( (float)totalMember / 100.0F ) );
	int mySkillLev = 0;

	// 파티마스터
	mySkillLev = MIN( GetCurrSkillLevel( GET_PARTY_MASTER( pPlayer ) ), 1 );
	seedValue = (float)( (float)mySkillLev / (float)totalLev );
	splitExp = (realExp+((int)( (float)seedValue * (float)realExp )));

	if( splitExp > 0 )
	{
		SendSystemMsg( GET_PARTY_MASTER( pPlayer ), "%d의 경험치 획득.", splitExp );
		PushPartyExp( pPlayer, GET_PARTY_MASTER( pPlayer ), splitExp  );
	}

	// 파티원들 
	for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
		{
			mySkillLev = MIN( GetCurrSkillLevel( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ), 1 );
			seedValue = (float)( (float)mySkillLev / (float)totalLev );
			splitExp = (realExp+((int)( (float)seedValue * (float)realExp )));
			
			if( splitExp > 0 )
			{
				SendSystemMsg( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i], "%d의 경험치 획득.", splitExp );
		    	PushPartyExp( pPlayer, GET_PARTY_MASTER( pPlayer )->ch2.party.member[i], splitExp );
			}
		}
	}

	return 1;
}

//
// Function Name: CheckDeadMob
// Date: 2003-03-29
// Description: 
//
BYTE CheckDeadMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	sPDESC_DATA pExpPlayer = NULL;
	int mobExp;
	int currDmg = 0;
	int maxDmg = 0;
	char ownerName[128];
	char masterName[128];

	*ownerName = NULL_STR;
	*masterName = NULL_STR;

	// 죽을경우 
	if( pMob->hp <= 0 )
	{			
		int tmpMobNum = pMob->mobNum;
		int tmpMapNum = pMob->mapNum;
		BYTE isObject = pMob->isObject;

		// 오브젝트 몹 
		if( isObject )
		{
			stop_fighting_mob( pMob );		
			stop_fighting_enemy( pMob );
			
			MonsterDie( pMob );
			DelMobData( pMob , __FILE__, __LINE__ );

			if( tmpMobNum == dMONSTER_WARCRISTAL )
			{
				WAR_CheckEnd( gWORLD[tmpMapNum], dWARCRASTAL_CRASH_POINT );
			}
			else if( tmpMobNum == dMONSTER_CASTLEDOOR )
			{
				WAR_CheckEnd( gWORLD[tmpMapNum], 5 );
			}

			if( gWORLD[tmpMapNum]->underAttack )
				WAR_PopWarCristal( tmpMapNum );
	
			return 1;
		}

		for( int i = 0; i < dMAX_SAVE_MOB_EXP; i++ )
		{
			if( pMob->saveExp[i].pPlayer != NULL )
			{
				pExpPlayer = pMob->saveExp[i].pPlayer;
				currDmg = 0;
				
				// 근거리 
				if( pMob->saveExp[i].expDamage[0] )
				{
					if( GET_SATTACK( pExpPlayer ) < dMAX_SKILL_LEVEL )
					{
						currDmg += pMob->saveExp[i].expDamage[0];
						
						mobExp = CalculationMobExp( pExpPlayer, pMob, 
							MAX( pMob->saveExp[i].expDamage[0], GET_MOB_HP( pMob ) ) ) * MIN( pExpPlayer->ch2.expJump, 1 );
						
						if( g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )] )
							mobExp = mobExp * (int)(g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )]/100);
						
						if( g_ExpJump > 1 )
							mobExp = mobExp * g_ExpJump;
						
						if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )	// 프리미엄 계정 ( 2배 )
						{
							mobExp = (int)( mobExp * 2 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ) // 1.3 배 
						{
							mobExp = (int)( mobExp * 1.3 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] ) // 1.5 배
						{
							mobExp = (int)( mobExp * 1.5 );
						}

#ifdef __CHINA_VER__		//경험치 1.3배로 설정 20050228
						mobExp = (int)( mobExp * 1.3 );						
#endif
						//구정 이벤트 경험치 2배
						mobExp = (int)( mobExp * 2 );

						if( !SplitPartyExp( pExpPlayer, mobExp ) )
						{
							pExpPlayer->ch.skill_exp[0] += mobExp;
							pExpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP1] = 1;
							SendGetExpMsg( pExpPlayer, 0, mobExp );
							
							pExpPlayer->ch.str_exp += (int)(mobExp*0.98);
							pExpPlayer->ch.dex_exp += (int)(mobExp*0.02);
							//pExpPlayer->ch.intelli_exp += (int)(mobExp*0.01);
#ifndef dNOT_USE_PET
							PET_UpdateExp( pExpPlayer, (int)(mobExp*0.05) );
#endif
						}
					}
				}
				// 원거리 
				if( pMob->saveExp[i].expDamage[1] )
				{
					if( GET_LATTACK( pExpPlayer ) < dMAX_SKILL_LEVEL )
					{
						currDmg += pMob->saveExp[i].expDamage[1];
						
						mobExp = CalculationMobExp( pExpPlayer, pMob, 
							MAX( pMob->saveExp[i].expDamage[1], GET_MOB_HP( pMob ) ) ) * MIN( pExpPlayer->ch2.expJump, 1 );
						
						if( g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )] )
							mobExp = mobExp * (int)(g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )]/100);
						
						if( g_ExpJump > 1 )
							mobExp = mobExp * g_ExpJump;

						if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )	// 프리미엄 계정 ( 2배 )
						{
							mobExp = (int)( mobExp * 2 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ) // 1.3 배 
						{
							mobExp = (int)( mobExp * 1.3 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] ) // 1.5 배
						{
							mobExp = (int)( mobExp * 1.5 );
						}

#ifdef __CHINA_VER__		//경험치 1.3배로 설정 20050228
						mobExp = (int)( mobExp * 1.3 );						
#endif

						//구정 이벤트 경험치 2배
						mobExp = (int)( mobExp * 2 );

						if( !SplitPartyExp( pExpPlayer, mobExp ) )
						{
							pExpPlayer->ch.skill_exp[1] += mobExp;
							pExpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP2] = 1;
							SendGetExpMsg( pExpPlayer, 1, mobExp );
							
							pExpPlayer->ch.str_exp += (int)(mobExp*0.02);
							pExpPlayer->ch.dex_exp += (int)(mobExp*0.98);
							//pExpPlayer->ch.intelli_exp += (int)(mobExp*0.01);
#ifndef dNOT_USE_PET
							PET_UpdateExp( pExpPlayer, (int)(mobExp*0.05) );
#endif
						}
					}
				}
				// 흑마법
				if( pMob->saveExp[i].expDamage[2] )
				{
					if( GET_BMAGIC( pExpPlayer ) < dMAX_SKILL_LEVEL )
					{
						currDmg += pMob->saveExp[i].expDamage[2];
						
						mobExp = CalculationMobExp( pExpPlayer, pMob, 
							MAX( pMob->saveExp[i].expDamage[2], GET_MOB_HP( pMob ) ) ) * MIN( pExpPlayer->ch2.expJump, 1 );
						
						if( g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )] )
							mobExp = mobExp * (int)(g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )]/100);
						
						if( g_ExpJump > 1 )
							mobExp = mobExp * g_ExpJump;

						if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )	// 프리미엄 계정 ( 2배 )
						{
							mobExp = (int)( mobExp * 2 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ) // 1.3 배 
						{
							mobExp = (int)( mobExp * 1.3 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] ) // 1.5 배
						{
							mobExp = (int)( mobExp * 1.5 );
						}

#ifdef __CHINA_VER__		//경험치 1.3배로 설정 20050228
						mobExp = (int)( mobExp * 1.3 );						
#endif

						//구정 이벤트 경험치 2배
						mobExp = (int)( mobExp * 2 );
						
						if( !SplitPartyExp( pExpPlayer, mobExp ) )
						{	
							pExpPlayer->ch.skill_exp[2] += mobExp;
							pExpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP3] = 1;
							SendGetExpMsg( pExpPlayer, 2, mobExp );
							
							//	pExpPlayer->ch.str_exp += (int)(mobExp*0.01);
							pExpPlayer->ch.dex_exp += (int)(mobExp*0.02);
							pExpPlayer->ch.intelli_exp += (int)(mobExp*0.98);
#ifndef dNOT_USE_PET
							PET_UpdateExp( pExpPlayer, (int)(mobExp*0.05) );
#endif
						}
					}
				}
				// 백마법
				if( pMob->saveExp[i].expDamage[3] )
				{
					if( GET_WMAGIC( pExpPlayer ) < dMAX_SKILL_LEVEL )
					{
						currDmg += pMob->saveExp[i].expDamage[3];
						
						mobExp = CalculationMobExp( pExpPlayer, pMob, 
							MAX( pMob->saveExp[i].expDamage[3], GET_MOB_HP( pMob ) ) ) * MIN( pExpPlayer->ch2.expJump, 1 );
						
						if( g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )] )
							mobExp = mobExp * (int)(g_EXP_PROTECT[GET_ALL_SKILL( pPlayer )]/100);
						
						if( g_ExpJump > 1 )
							mobExp = mobExp * g_ExpJump;

						if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )	// 프리미엄 계정 ( 2배 )
						{
							mobExp = (int)( mobExp * 2 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ) // 1.3 배 
						{
							mobExp = (int)( mobExp * 1.3 );
						}
						else if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] ) // 1.5 배
						{
							mobExp = (int)( mobExp * 1.5 );
						}

#ifdef __CHINA_VER__		//경험치 1.3배로 설정 20050228
						mobExp = (int)( mobExp * 1.3 );						
#endif
						//구정 이벤트 경험치 2배
						mobExp = (int)( mobExp * 2 );
						
						if( !SplitPartyExp( pExpPlayer, mobExp ) )
						{	
							pExpPlayer->ch.skill_exp[3] += mobExp;
							pExpPlayer->isUpdate[dDATA_UPDATE_SKILLEXP4] = 1;
							SendGetExpMsg( pExpPlayer, 3, mobExp );
							
							pExpPlayer->ch.str_exp += (int)(mobExp*0.01);
							pExpPlayer->ch.dex_exp += (int)(mobExp*0.04);
							pExpPlayer->ch.intelli_exp += (int)(mobExp*0.95);
#ifndef dNOT_USE_PET
							PET_UpdateExp( pExpPlayer, (int)(mobExp*0.05) );
#endif
						}
					}
				}

				if( currDmg >= maxDmg )
				{
					strcpy( ownerName, GET_NAME( pExpPlayer ) );
					
					if( GET_PARTY_MASTER( pExpPlayer ) )
						strcpy( masterName, GET_NAME( GET_PARTY_MASTER( pExpPlayer ) ) );

					maxDmg = currDmg;
				}				
				
				CheckLevelUp( pExpPlayer, 0 );
				CheckLevelUp( pExpPlayer, 1 );
				CheckLevelUp( pExpPlayer, 2 );
				CheckLevelUp( pExpPlayer, 3 );
				
				pExpPlayer->isUpdate[dDATA_UPDATE_STREXP] = 1;
				pExpPlayer->isUpdate[dDATA_UPDATE_INTEXP] = 1;
				pExpPlayer->isUpdate[dDATA_UPDATE_DEXEXP] = 1;
			} // end of for( int i = 0; i < dMAX_SAVE_MOB_EXP; i++ )
		} // end of if( pMob->hp <= 0 )
		
		pMob->isDead = 1;	
		pMob->deadTimer = g_CurrTime + GetAttackDelay( pPlayer ); //+ PluseRangeAttackDelay( pMob, pPlayer, 40 );

		// 돈인데 돈이 안나온경우
#ifdef IS_TEST_SERVER
		if( g_ExpJump > 1 && pMob->mobNum < 100 )
		{
			for( int i = 0; i < g_ExpJump; i++ )
			{
				GetMonsterItem( pMob, ownerName, masterName, pPlayer );
			}
		}
		else
		{
			GetMonsterItem( pMob, ownerName, masterName, pPlayer );
		}
#else
			GetMonsterItem( pMob, ownerName, masterName, pPlayer );
#endif
		
#ifdef _dUMENOKOJI_GHOST_GUILD_		//유령 길드 관련 전쟁포인트 테스트
			sPWORLD pWorld = gWORLD[ pMob->mapNum ]; //몬스터의 맵을 얻어온다.
			if( ! pWorld )
				return 0;
			//플레이어가 길드가 없거나 맵의 오너길드가 없을때는 들어가지 못한다.
			if( ( GET_GUILD( pPlayer ) > 0 ) && ( pWorld->ownerGuild > 0 ) && ( pWorld->ownerGuild != GET_GUILD( pPlayer ) ) )
			{
				sPWAR_DATA pWarData[2];	// 0 : 진지 , 1 : 성
				pWarData[0] = WAR_FindData( GET_GUILD( pPlayer ), pWorld->ownerGuild );
				//진지를 구축한 길드이고, 오너길드가 아니면 들어간다.
				if( pWarData[0] )
				{					
					pWarData[1] = WAR_FindData( pWorld->ownerGuild, GET_GUILD( pPlayer ) );
					//여기서 한시간에 한번 들어가게 한다.
					if( GET_PLAYER_TIME( pPlayer ) == 0 )
					{
						SET_PLAYER_TIME( pPlayer );
					}
					else
					{
						if( GET_PLAYER_TIME( pPlayer ) + 3600 <= g_CurrRealTime )	 //5분으로 세팅되어있는것 1시간으로 수정할것!!!!!!
						{		
							if( pWarData[0]->ownerPoint <=  pWarData[1]->ownerPoint )
							{
								BYTE	plusPoint=1;
								pWarData[0]->ownerPoint += plusPoint;
								
								if( pWarData[0]->lastUpdateTime + 180 <= g_CurrRealTime )
								{
									WAR_UpdateWarData( pWarData[0] );
									pPlayer->ch2.getTime = g_CurrRealTime;	
									
								}
							}
						}		
					}
				}					
			}
#endif

#ifndef dNON_PVP_MODE
		// 성향치 회복 
		int plusCha = (int)( g_MOBINFO[pMob->mobNum]->exp * 0.01 );

		if( g_MOBINFO[pMob->mobNum]->level >= GET_ALL_SKILL( pPlayer ) + 5 )
			plusCha += 3;

		if( plusCha <= 0 )
			plusCha = 1;

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] )
			plusCha = plusCha * 2;

		GET_CHA( pPlayer ) = MAX( GET_CHA( pPlayer ) + plusCha, 60000 );
		SendUpdateCha( pPlayer );

		pPlayer->isUpdate[dDATA_UPDATE_CHA] = 1;
#endif

		stop_fighting_mob( pMob );		
		stop_fighting_enemy( pMob );

		//몬스터 경험치 획득 메시지
		SendSystemMsg( pPlayer, "%d의 경험치 획득.", mobExp );
				
		return 1;
	}
	return 0;
}

//
// Function Name: GetAttackDamage
// Date: 2003-03-29
// Description: 
//
int	 DistanceDamage( int x, int y, int minDamage, int maxDamage, float caculValue )
{
	int		defDamage = 0;
	int		tmpvalue = 0, tmpminDamage=0, tmpmaxDamage=0;
	
	tmpvalue = maxDamage - minDamage;
	if( tmpvalue < 5 ) //최소 최대 사이값이 10보다 작으면....
	{
		//계산 방법
		if( x >= y )
		{
			if( x >= 0 && x <= 6 )
			{
				tmpminDamage = maxDamage;
				tmpmaxDamage = maxDamage+1;
			}
			else if( x >= 7 && x <= 14 )
			{
				tmpminDamage = minDamage;
				tmpmaxDamage = maxDamage;
			}
			else if( x >= 15 && x <= 18 )
			{
				tmpminDamage = minDamage - 1;
				tmpmaxDamage = minDamage;
			}
		}
		else if( y > x )
		{
			if( y >= 0 && y <= 6 )
			{
				tmpminDamage = maxDamage;
				tmpmaxDamage = maxDamage+1;
			}
			else if( y >= 7 && y <= 14 )
			{
				tmpminDamage = minDamage;
				tmpmaxDamage = maxDamage;
			}
			else if( y >= 15 && y <= 18 )
			{
				tmpminDamage = minDamage-1;
				tmpmaxDamage = minDamage;
			}
		}
	}
	else
	{
		int	testValue = (int)(tmpvalue * caculValue);
		//계산 방법
		if( x >= y )
		{
			if( x >= 0 && x <= 6 )
			{
				tmpminDamage = ( maxDamage - testValue );
				tmpmaxDamage = maxDamage;
			}
			else if( x >= 7 && x <= 14 )
			{
				tmpminDamage = ( ( minDamage + testValue ) + 1 );
				tmpmaxDamage = ( ( maxDamage - testValue ) - 1 );
			}
			else if( x >= 15 && x <= 18 )
			{
				tmpminDamage = minDamage;
				tmpmaxDamage = ( minDamage + testValue );
			}
		}
		else if( y > x )
		{
			if( y >= 0 && y <= 6 )
			{
				tmpminDamage = ( maxDamage - testValue );
				tmpmaxDamage = maxDamage;
			}
			else if( y >= 7 && y <= 14 )
			{
				tmpminDamage = ( ( minDamage + testValue ) + 1 );
				tmpmaxDamage = ( ( maxDamage - testValue ) - 1 );
			}
			else if( y >= 15 && y <= 18 )
			{
				tmpminDamage = minDamage;
				tmpmaxDamage = ( minDamage + testValue );
			}
		}
	}

	defDamage = number( tmpminDamage, tmpmaxDamage );

	return defDamage;
}

//
// Function Name: GetAttackDamage
// Date: 2003-03-29
// Description: 
//
WORD GetAttackDamage( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	WORD damage = 0;
	int defDamage;

	// 근거리 
	if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM ) 
	{
		if( pPlayer->equip[dEQUIP_WEAPON] )
		{
			#ifndef DAMAGE_TEST
			defDamage = number( g_DAMAGE_TABLE[GET_STR_TOTAL( pPlayer )].minStrDamage + g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].minSAttackDamage + GET_ITEM_MINDAMAGE(pPlayer->equip[dEQUIP_WEAPON]),
				g_DAMAGE_TABLE[GET_STR_TOTAL( pPlayer )].maxStrDamage + g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].maxSAttackDamage + GET_ITEM_MAXDAMAGE(pPlayer->equip[dEQUIP_WEAPON]) );
			#else
			defDamage = number( g_DAMAGE_TABLE[GET_STR_TOTAL( pPlayer )].minStrDamage + g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].minSAttackDamage + GET_ITEM_MINDAMAGE(pPlayer->equip[dEQUIP_WEAPON]),
				g_DAMAGE_TABLE[GET_STR_TOTAL( pPlayer )].maxStrDamage + g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].maxSAttackDamage + GET_ITEM_MAXDAMAGE(pPlayer->equip[dEQUIP_WEAPON]) );

			#endif
		}
		else
			return 0;
	}
	// 원거리
	else if( GET_USE_SKILL( pPlayer ) == dLATTACK_SKILL_ITEM ) 
	{
		if( pPlayer->equip[dEQUIP_BOW] )
		{
#ifdef IS_ARCHER_RANGE_TEST
			int minDamage, maxDamage;

			minDamage = g_DAMAGE_TABLE[GET_DEX_TOTAL( pPlayer )].minDexDamage + g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].minLAttackDamage + GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_BOW] );
			maxDamage = g_DAMAGE_TABLE[GET_DEX_TOTAL( pPlayer )].maxDexDamage + g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].maxLAttackDamage + GET_ITEM_MAXDAMAGE( pPlayer->equip[dEQUIP_BOW] );

			//몬스터와 플레이어와의 거리 계산
			int rangeX, rangeY;
			rangeX = abs( pMob->posX - GET_POSX( pPlayer ) );
			rangeY = abs( pMob->posY - GET_POSY( pPlayer ) );

			defDamage = DistanceDamage( rangeX, rangeY, minDamage, maxDamage, 0.2f );
#else
			defDamage = number( g_DAMAGE_TABLE[GET_DEX_TOTAL( pPlayer )].minDexDamage + g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].minLAttackDamage + GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_BOW] ),
				g_DAMAGE_TABLE[GET_DEX_TOTAL( pPlayer )].maxDexDamage + g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].maxLAttackDamage + GET_ITEM_MAXDAMAGE( pPlayer->equip[dEQUIP_BOW] ) );
#endif
		}
		else
			return 0;
	}
	else		
	{// SKILL
		defDamage = MAGIC_GetDamage( pPlayer );
		
		if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_WIZARDRY ) )
		{
			defDamage += (int)(defDamage * 0.15 );
		}
	}

	/*defDamage = MIN( defDamage - GET_MOB_DP( pMob ), 0 );  //순수대미지를 돌려주기위해서.....주석처리
	damage = MIN( defDamage, 0 ); */

	damage = defDamage;

	// 풀파워 
	if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_FULLPOWER ) && pPlayer->adminLevel )
	{
		damage += 60000;
	}

	return damage;
}

//
// Function Name: GetPvPDamage
// Date: 
// Description: 
//
WORD GetPvPDamage( sPDESC_DATA pAttacker, sPDESC_DATA pVict )
{

	if( !pAttacker || !pVict )
		return 0;

	WORD damage = 0;
	int defDamage = 0;

	int playerDP = GetPlayerItemDepense( pVict );


	//버프 스킬 체크하고 적용
	//클라우드실드   3  6%, 브라이트 실드  14 10%, 프로텍션 실드  15 14%
	if( pVict->ch2.skillEffect[3].delay > 0 )
	{
		playerDP += (int)( playerDP * 0.06 );
	}
	if( pVict->ch2.skillEffect[14].delay > 0 )
	{
		playerDP += (int)( playerDP * 0.15 );
	}
	if( pVict->ch2.skillEffect[15].delay > 0 )
	{
		playerDP += (int)( playerDP * 0.25 );
	}

	if( GET_RIDE_STATE( pVict ) && playerDP )
	{
		switch( GET_RIDE_TYPE( pVict ) )
		{
		case dRIDE_TYPE1:		// 갈색말
			playerDP -= (int)(playerDP*0.2);		
			break;
		case dRIDE_TYPE2:		// 검정말
			playerDP -= (int)(playerDP*0.15);
			break;
		case dRIDE_TYPE3:		// 백색말
			playerDP -= (int)(playerDP*0.1);
			break;
		}
	}

	// 근거리 
	if( GET_USE_SKILL( pAttacker ) == dSATTACK_SKILL_ITEM ) 
	{
		if( pAttacker->equip[dEQUIP_WEAPON] )
		{
			int strDamage = number( g_DAMAGE_TABLE[GET_STR_TOTAL( pAttacker )].minStrDamage, g_DAMAGE_TABLE[GET_STR_TOTAL( pAttacker )].maxStrDamage );
			int sattackDamage = number( g_DAMAGE_TABLE[GET_SATTACK( pAttacker )].minSAttackDamage, g_DAMAGE_TABLE[GET_SATTACK( pAttacker )].maxSAttackDamage );
			int weaponDamage = number( GET_ITEM_MINDAMAGE(pAttacker->equip[dEQUIP_WEAPON]), GET_ITEM_MAXDAMAGE(pAttacker->equip[dEQUIP_WEAPON]) );
			int calsValue = (int)( 1 - ( GET_ALL_SKILL( pVict ) - GET_ALL_SKILL( pAttacker ) ) * 0.005 );

			defDamage = (int)( ( ( strDamage + sattackDamage ) * 1.6 + weaponDamage * 1.8 ) * MIN( calsValue, 1 ) );
	//		defDamage = (int)( ( ( strDamage + sattackDamage + weaponDamage ) + ( sattackDamage * 1.2 ) ) ); 

	//		defDamage = (int)( ( strDamage + sattackDamage + weaponDamage ) * 1.5 ) + ( strDamage + sattackDamage );
			
		}
		else
			return 0;
	}
	// 원거리
	else if( GET_USE_SKILL( pAttacker ) == dLATTACK_SKILL_ITEM ) 
	{
		if( pAttacker->equip[dEQUIP_BOW] )
		{
#ifdef IS_ARCHER_RANGE_TEST
			int minDamage, maxDamage;

			minDamage = g_DAMAGE_TABLE[GET_DEX_TOTAL( pPlayer )].minDexDamage + g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].minLAttackDamage + GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_BOW] );
			maxDamage = g_DAMAGE_TABLE[GET_DEX_TOTAL( pPlayer )].maxDexDamage + g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].maxLAttackDamage + GET_ITEM_MAXDAMAGE( pPlayer->equip[dEQUIP_BOW] );

			//몬스터와 플레이어와의 거리 계산
			int rangeX, rangeY;
			rangeX = abs( GET_POSX( pVict ) - GET_POSX( pPlayer ) );
			rangeY = abs( GET_POSY( pVict ) - GET_POSY( pPlayer ) );

			defDamage = DistanceDamage( rangeX, rangeY, minDamage, maxDamage, 0.2f );
#else
			// 기존의 경우 DEX 계열 데미지가 아니라 STR로 사용중이었음
			int dexDamage = number( g_DAMAGE_TABLE[GET_DEX_TOTAL( pAttacker )].minDexDamage, g_DAMAGE_TABLE[GET_DEX_TOTAL( pAttacker )].maxDexDamage );
			int lattackDamage = number( g_DAMAGE_TABLE[GET_LATTACK( pAttacker )].minLAttackDamage, g_DAMAGE_TABLE[GET_LATTACK( pAttacker )].maxLAttackDamage );
			int weaponDamage = number( GET_ITEM_MINDAMAGE( pAttacker->equip[dEQUIP_BOW] ), GET_ITEM_MAXDAMAGE( pAttacker->equip[dEQUIP_BOW] ) );
			int calsValue =  (int)( 1 - ( GET_ALL_SKILL( pVict ) - GET_ALL_SKILL( pAttacker ) ) * 0.005 );

			defDamage = (int)( ( ( dexDamage + lattackDamage ) * 1.6 + weaponDamage * 1.8 ) * 1.26 * MIN( calsValue, 1 ) );
			//defDamage = (int)( ( ( dexDamage + lattackDamage + weaponDamage ) + ( lattackDamage * 1.2 ) ) ); 
			
			//defDamage = (int)( ( dexDamage + lattackDamage + weaponDamage ) * 1.5 ) + ( dexDamage + lattackDamage );
#endif
			
		}
		else
			return 0;
	} //
	else
	{
		int magicDamage = MAGIC_GetDamage( pAttacker );
		int intDamage = number( g_DAMAGE_TABLE[GET_INT_TOTAL( pAttacker )].minIntDamage, g_DAMAGE_TABLE[GET_INT_TOTAL( pAttacker )].maxIntDamage );
		int weaponDamage = 0;

		if( pAttacker->equip[dEQUIP_WEAPON] )
		{
			if( GET_ITEM_TYPE( pAttacker->equip[dEQUIP_WEAPON] ) == dITEMTYPE_STAFF )
			{
				weaponDamage = number( GET_ITEM_MINMANADMG( pAttacker->equip[dEQUIP_WEAPON] ), GET_ITEM_MAXMANADMG( pAttacker->equip[dEQUIP_WEAPON] ) );
			}
		}

		int calsValue =  (int)( 1 - ( GET_ALL_SKILL( pVict ) - GET_ALL_SKILL( pAttacker ) ) * 0.005 );
		defDamage = (int)( ( ( intDamage + magicDamage ) * 1.4 * 1.8 ) * MIN( calsValue, 1 ) );
//		defDamage = (int)( ( ( intDamage + magicDamage ) + ( magicDamage * 1.2 ) ) ); 

//		defDamage = (int)( ( magicDamage + intDamage + weaponDamage ) * 1.5 ) + weaponDamage;
//		defDamage = (int)( ( magicDamage + intDamage + weaponDamage ) );
	}

	// 임시 
	playerDP = (int)( playerDP / 2 ); //umenokoji
	
	// 데미지 적용
	defDamage = MIN( defDamage - playerDP, 0 ); //umenokoji
	damage = defDamage;

	// 풀파워(운영자)
	if( IS_SET( pAttacker->ch2.flag, dPLAYER_FLG_FULLPOWER ) && pAttacker->adminLevel )
		damage += 60000;

	return damage;

}

//
// Function Name: GetMobExp
// Date: 2003-05-22
// Description: 
//
void GetMobExp( sPDESC_DATA pPlayer, sPMOB_DATA pMob, WORD damage, BYTE skill )
{
	if( pMob->isObject )
		return;

	if( skill > dMAX_PLAYER_SKILL )
	{
		log( "GetMobExp : skill > dMAX_SKILL\r\n" );
		return;
	}

	if( damage > 0 )
	{
		int arrayNum = -1;
		int emptyArray = -1;
		
		for( int i = 0; i < dMAX_SAVE_MOB_EXP; i++ )
		{
			if( pMob->saveExp[i].pPlayer == NULL )
				emptyArray = i;
			else if( pMob->saveExp[i].pPlayer == pPlayer )
				arrayNum = i;
		}
		
		if( arrayNum == -1 )
		{
			if( emptyArray == -1 )
				return;
			
			pMob->saveExp[emptyArray].pPlayer = pPlayer;
			pMob->saveExp[emptyArray].expDamage[skill] += MAX( damage, pMob->hp );
		}
		else
		{
			pMob->saveExp[arrayNum].expDamage[skill] += MAX( damage, pMob->hp );
		}
	}
}



//
// Function Name: MagicAttackPvP
// Date:
// Description: 
//
void MagicAttackPvP( sPDESC_DATA pAttacker, sPDESC_DATA pVict, sPSKILL_DATA pSkill )
{

	if( !pAttacker || !pVict )
	{
		log( "MagicAttackPvp : if( !pAttacker || !pVict )\r\n" ); 
		return;
	}
	
	if( pAttacker == pVict )
		return;

	if( !IS_VALID_WORLD( GET_MAP_NUM( pAttacker ) ) )
		return;
			
	if( !pSkill )
		return;
	
	if( !IS_ATTACK_MAGIC( pSkill ) )
		return;		
	
	sPDESC_DATA d, next_d;
	int rangeX, rangeY, rangeValue;
	int posX, posY;
	int x, y;
	int i;

	if( pSkill->skillAttr == dSKILL_AREA_NONE )
	{

		DamagePvP( pAttacker, pVict, pSkill );

		return;
	}
	else if( pSkill->skillAttr == dSKILL_AREA_PLAYER_ZONE )
	{
		posX = (int)(GET_POSX( pAttacker ) / dONE_TILE);
		posY = (int)(GET_POSY( pAttacker ) / dONE_TILE);
				
		for( y = posY - 1; y <= posY + 1; y++ )
		{
			for( x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[GET_MAP_NUM( pAttacker )]->player[y][x], d, next_d, w_next, WMgr );
					
					if( d == pAttacker )
					{
						LIST_SKIP( d, next_d );
					}

					rangeX = abs( d->ch.posX - pAttacker->ch.posX );
					rangeY = abs( d->ch.posY - pAttacker->ch.posY );
					
					rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );

					if( rangeValue <= pSkill->areaRange )
					{
						if( GET_PVPMODE( pAttacker ) == dPVPMODE_FULL )
						{

							DamagePvP( pAttacker, d, pSkill );

						}
						else if( GET_PVPMODE( pAttacker ) == dPVPMODE_GUILD )
						{
							if( GET_GUILD( pAttacker ) && GET_GUILD( d ) != GET_GUILD( pAttacker ) )
							{

								DamagePvP( pAttacker, d, pSkill );

							}
						}
						else if( GET_PVPMODE( pAttacker ) == dPVPMODE_PARTY )
						{
							if( GET_PARTY_MASTER( pAttacker ) && GET_PARTY_MASTER( d ) != GET_PARTY_MASTER( pAttacker ) )
							{

								DamagePvP( pAttacker, d, pSkill );

							}
						}
					}
					
					LIST_WHILEEND( gWORLD[GET_MAP_NUM( pAttacker )]->player[y][x], d, next_d );
				}
			}
		}	
	}
	else if( pSkill->skillAttr == dSKILL_AREA_MOB_ZONE )
	{
		posX = (int)(GET_POSX( pVict ) / dONE_TILE);
		posY = (int)(GET_POSY( pVict ) / dONE_TILE);
		
		for( y = posY - 1; y <= posY + 1; y++ )
		{
			for( x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[GET_MAP_NUM( pAttacker )]->player[y][x], d, next_d, w_next, WMgr );
					
					if( d == pAttacker )
					{
						LIST_SKIP( d, next_d );
					}
					
					rangeX = abs( d->ch.posX - pVict->ch.posX );
					rangeY = abs( d->ch.posY - pVict->ch.posY );
					
					rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );
					
					if( rangeValue <= pSkill->areaRange )
					{
						if( GET_PVPMODE( pAttacker ) == dPVPMODE_FULL )
						{

							DamagePvP( pAttacker, d, pSkill );

						}
						else if( GET_PVPMODE( pAttacker ) == dPVPMODE_GUILD )
						{
							if( GET_GUILD( pAttacker ) && GET_GUILD( d ) != GET_GUILD( pAttacker ) )
							{

								DamagePvP( pAttacker, d, pSkill );

							}
						}
						else if( GET_PVPMODE( pAttacker ) == dPVPMODE_PARTY )
						{
							if( GET_PARTY_MASTER( pAttacker ) && GET_PARTY_MASTER( d ) != GET_PARTY_MASTER( pAttacker ) )
							{

								DamagePvP( pAttacker, d, pSkill );

							}
						}
					}
					
					LIST_WHILEEND( gWORLD[GET_MAP_NUM( pAttacker )]->player[y][x], d, next_d );
				}
			}
		}	
	} 
	else if( pSkill->skillAttr == dSKILL_AREA_STRAIGHT )
	{
		int	checkPos[5][2] =
		{
			{ -1, -1 },	{ -1, -1 },	{ -1, -1 },	{ -1, -1 },	{ -1, -1 },
		};

		int desPosX = GET_POSX( pAttacker );
		int desPosY = GET_POSY( pAttacker );

		for( i = 0; i < 5; i++ )
		{
			if( desPosX < GET_POSX( pVict ) )
				desPosX += 1;
			else if( desPosX > GET_POSX( pVict ) )
				desPosX -= 1;

			if( desPosY < GET_POSY( pVict ) )
				desPosY += 1;
			else if( desPosY > GET_POSY( pVict ) )
				desPosY -= 1;

			checkPos[i][0] = desPosX;
			checkPos[i][1] = desPosY;
		}

		posX = (int)(GET_POSX( pAttacker ) / dONE_TILE);
		posY = (int)(GET_POSX( pAttacker ) / dONE_TILE);

		for( y = posY - 1; y <= posY + 1; y++ )
		{
			for( x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[GET_MAP_NUM( pAttacker )]->player[y][x], d, next_d, w_next, WMgr );
					
					if( d == pVict || d == pAttacker )
					{
						LIST_SKIP( d, next_d );
					}

					for( i = 0; i < 5; i++ )
					{
						if( checkPos[i][0] == -1 )
							break;
						else if( checkPos[i][0] == GET_POSX( d ) && checkPos[i][1] == GET_POSY( d ) )
						{
							if( GET_PVPMODE( pAttacker ) == dPVPMODE_FULL )
							{

								DamagePvP( pAttacker, d, pSkill );

							}
							else if( GET_PVPMODE( pAttacker ) == dPVPMODE_GUILD )
							{
								if( GET_GUILD( pAttacker ) && GET_GUILD( d ) != GET_GUILD( pAttacker ) )
								{

									DamagePvP( pAttacker, d, pSkill );

								}
							}
							else if( GET_PVPMODE( pAttacker ) == dPVPMODE_PARTY )
							{
								if( GET_PARTY_MASTER( pAttacker ) && GET_PARTY_MASTER( d ) != GET_PARTY_MASTER( pAttacker ) )
								{

									DamagePvP( pAttacker, d, pSkill );

								}
							}
						}
					}
					
					LIST_WHILEEND( gWORLD[GET_MAP_NUM( pAttacker )]->player[y][x], d, next_d );
				}
			}
		}

		DamagePvP( pAttacker, pVict, pSkill );


	}

}

//
// Function Name: MagicAttackToMob
// Date: 
// Description: 
//
void MagicAttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob, sPSKILL_DATA pSkill )
{
	if( !pPlayer || !pMob )
	{
		log( "MagicAttackToMob : !pPlayer || !pMob\r\n" );
		return;
	}

	if( !IS_VALID_WORLD( GET_MAP_NUM( pPlayer ) ) )
		return;
			
	if( !pSkill )
		return;

	if( !IS_ATTACK_MAGIC( pSkill ) )
		return;		

	//오토방지돌
	if( pMob->mobNum == 1003 )
		return;
	 
#ifndef dNOT_USE_PET
	if( IS_USING_PET( pPlayer ) )
	{
		if( GET_PET_ANGERGAGE( pPlayer ) >= dMAX_PET_ANGER )
		{
			PET_AttackToMob( pPlayer, pMob );
			GET_PET_ANGERGAGE( pPlayer ) = 0;
			PET_UpdatePetGage( pPlayer );
		}
	}
#endif

	sPMOB_DATA m, next_m;
	int rangeX, rangeY;
	int posX, posY;
	int	rangeValue;
	int x, y;
	int i;

	if( pSkill->skillAttr == dSKILL_AREA_NONE )
	{
#ifdef _dDTM_
		DamageToMob( pPlayer, pMob, pSkill );
#else
		DamageToMob( pPlayer, pMob, NULL );
#endif
		return;
	}
	else if( pSkill->skillAttr == dSKILL_AREA_PLAYER_ZONE )
	{		
		posX = (int)(GET_POSX( pPlayer ) / dONE_TILE);
		posY = (int)(GET_POSY( pPlayer ) / dONE_TILE);
		
		for( y = posY - 1; y <= posY + 1; y++ )
		{
			for( x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m, w_next, WMgr );
					
					rangeX = abs( m->posX - pPlayer->ch.posX );
					rangeY = abs( m->posY - pPlayer->ch.posY );
					
					rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );
					
					if( rangeValue <= pSkill->areaRange )
#ifdef _dDTM_
						DamageToMob( pPlayer, m, pSkill );
#else
						DamageToMob( pPlayer, m, NULL );
#endif
					
					LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m );
				}
			}
		}	
	}
	else if( pSkill->skillAttr == dSKILL_AREA_MOB_ZONE )
	{
		posX = (int)(pMob->posX / dONE_TILE);
		posY = (int)(pMob->posY / dONE_TILE);
				
		for( y = posY - 1; y <= posY + 1; y++ )
		{
			for( x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m, w_next, WMgr );
					
					rangeX = abs( m->posX - pMob->posX );
					rangeY = abs( m->posY - pMob->posY );
					
					rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );

					if( rangeValue <= pSkill->areaRange )
						
#ifdef _dDTM_
						DamageToMob( pPlayer, m, pSkill );
#else
					DamageToMob( pPlayer, m, NULL );
#endif
					
					LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m );
				}
			}
		}	
	}
	else if( pSkill->skillAttr == dSKILL_AREA_STRAIGHT )
	{
		int checkPos[5][2] = 
		{
			{ -1, -1 },	{ -1, -1 },	{ -1, -1 },	{ -1, -1 },	{ -1, -1 },
			//{ -1, -1 },	{ -1, -1 },	{ -1, -1 },	{ -1, -1 },	{ -1, -1 },
		};

		int desPosX = GET_POSX( pPlayer );
		int desPosY = GET_POSY( pPlayer );

		for( i = 0; i < 5; i++ )
		{
			if( desPosX < pMob->posX )
				desPosX += 1;
			else if( desPosX > pMob->posX )
				desPosX -= 1;
			
			if( desPosY < pMob->posY )
				desPosY += 1;
			else if( desPosY > pMob->posY )
				desPosY -= 1;

			checkPos[i][0] = desPosX;
			checkPos[i][1] = desPosY;
		}

		posX = (int)(GET_POSX( pPlayer ) / dONE_TILE);
		posY = (int)(GET_POSY( pPlayer ) / dONE_TILE);

		for( y = posY - 1; y <= posY + 1; y++ )
		{
			for( x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m, w_next, WMgr );
					
					if( m == pMob )
					{
						LIST_SKIP( m, next_m );
					}

					for( i = 0; i < 5; i++ )
					{
						if( checkPos[i][0] == -1 )
							break;
						else
						{
							if( checkPos[i][0] == m->posX &&
								checkPos[i][1] == m->posY )
							{
#ifdef _dDTM_
								DamageToMob( pPlayer, m, pSkill );
#else
								DamageToMob( pPlayer, m, NULL );
#endif
							}
						}
					}
										
					LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m );
				}
			}
		}
#ifdef _dDTM_
		DamageToMob( pPlayer, pMob, pSkill );
#else
		DamageToMob( pPlayer, pMob, NULL );
#endif		
	}
	else
		return;
}

BOOL IsCanAttackWeight( sPDESC_DATA pPlayer )
{
#ifdef USE_ITEM_WEIGHT
	if( GET_CURRITEM_WEIGHT( pPlayer ) > (int)( CalsAllowTotalWeight( pPlayer ) * 0.9 ) )
		return 0;
	return 1;
#else
	return 1;
#endif
}

//
// Function Name: AttackToMob
// Date: 2003-05-22
// Description: 
//
void AttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	if( !IS_VALID_WORLD( GET_MAP_NUM( pPlayer ) ) )
		return;

	if( !IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
		return;

	// 공격 가능거리가 아니면
	if( !IsPlayerAttackRange( pPlayer, pMob ) )
		return;

	if( !IsCanAttackWeight( pPlayer ) )
		return;

	if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM ||
		GET_USE_SKILL( pPlayer ) == dLATTACK_SKILL_ITEM )
	{		
		if( pPlayer->enemyMob )
		{
			SendBeginPlayerAttack( pPlayer, pPlayer->enemyMob->idxNum, dMOB );
			
			// 거리만큼 딜레이 추가
//			if( GET_USE_SKILL( pPlayer ) == dLATTACK_SKILL_ITEM )
//				PluseRangeAttackDelay( pPlayer, pPlayer->enemyMob , 30 );

#ifndef dNOT_USE_PET
			if( IS_USING_PET( pPlayer ) )
			{
				if( GET_PET_ANGERGAGE( pPlayer ) >= dMAX_PET_ANGER )
				{
					PET_AttackToMob( pPlayer, pPlayer->enemyMob );
					GET_PET_ANGERGAGE( pPlayer ) = 0;
					PET_UpdatePetGage( pPlayer );
				}
			}
#endif
			DamageToMob( pPlayer, pMob , NULL );			

		}
		return;
	}
}

//
// Function Name: AttackPvP
// Date: 
// Description: 
//
void AttackPvP( sPDESC_DATA pAttacker, sPDESC_DATA pVict )
{

	if( !IS_VALID_WORLD( GET_MAP_NUM( pAttacker ) ) )
		return;

	if( !IS_VALID_ITEM( GET_USE_SKILL( pAttacker ) ) )
		return;

	// 거리 체크
	if( !IsPlayerPvPAttackRange( pAttacker, pVict ) )
		return;

	if( GET_USE_SKILL( pAttacker ) == dSATTACK_SKILL_ITEM ||
		GET_USE_SKILL( pAttacker ) == dLATTACK_SKILL_ITEM )
	{		
		if( pAttacker->victPlayer )
		{
			SendBeginPlayerAttack( pAttacker, GET_IDX( pVict ), dPLAYER );
			DamagePvP( pAttacker, pVict, NULL );
		}
		return;
	}

}


//
// Function Name: DamageToMob
// Date: 
// Description: 
//
void DamageToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob, sPSKILL_DATA pSkill )
{
	if( !pPlayer )
		return;

	if( !pMob )
		return;

	if( pMob->isDead )
	{
		stop_fighting_player( pPlayer );
		return;
	}

	if( !pMob->enemy )
	{
		SET_MOB_ENEMY( pMob, pPlayer );
		SendMobState( pMob );
	}

	if( !pPlayer->enemyMob )
		pPlayer->enemyMob = pMob;

	WORD		damage = 0;
	int			defDamage = 0;
	int			attackDamage = 0;

	BYTE btCritical = dCRITICAL_NONE;
	
	int rndCheckValue = 0;

	// 스킬이 아닐때만
	if( !pSkill )
	{
		if( GET_ALL_SKILL( pPlayer ) >= GET_MOB_LEVEL( pMob ) )
			rndCheckValue = 0;
		
		else
			rndCheckValue = GET_MOB_LEVEL( pMob ) - GET_ALL_SKILL( pPlayer );
	}
	
	if( rndCheckValue && number( 1, 1000 ) <= rndCheckValue )
	{	
		damage = 0;
	}
	else
	{
		attackDamage = GetAttackDamage( pPlayer, pMob ); 

		defDamage = attackDamage;
		//버프 스킬 체크하고 적용
		//파이어 버스트 1, 다크파이어챈트 16, 데스파이어오라 17
		if( pPlayer->ch2.skillEffect[1].delay > 0 )
		{
			defDamage += (int)( attackDamage * 0.05 );
		}
		if( pPlayer->ch2.skillEffect[16].delay > 0 )
		{
			defDamage += (int)( attackDamage * 0.15 );
		}
		if( pPlayer->ch2.skillEffect[17].delay > 0 )
		{
			defDamage += (int)( attackDamage * 0.25 );
		}
		
		//스킬 변경
		//근거리 기본이거나 원거리 기본일때(즉, 물리공격일때만)만 이펙트 스킬 적용
		if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM || GET_USE_SKILL( pPlayer ) == dLATTACK_SKILL_ITEM )
		{
			defDamage += GetSkillAddDamage( pSkill, attackDamage );
			defDamage += GetSkillEffectAddDamage( pPlayer, attackDamage );	
		}
		//흑마법이거나 백마법일때(즉, 마법공격일때만)만 이펙트 스킬 적용
		else
		{
			defDamage += GetSkillAddDamage( pSkill, attackDamage );
			defDamage += GetSkillEffectAddDamage( pPlayer, attackDamage );	
		}
		
		// 칠터치
		if( pMob->AttrEffectTime[MOB_ATTREFFECT_CHILTOUCH] )
		{
			if( pMob->AttrEffectTime[MOB_ATTREFFECT_CHILTOUCH] + 5 > g_CurrRealTime )
				defDamage += (int)( attackDamage * 0.1 );
			
			else
				pMob->AttrEffectTime[MOB_ATTREFFECT_CHILTOUCH] = 0;
		}
		
		// 포이즌 웨폰
		if( pMob->AttrEffectTime[MOB_ATTREFFECT_POISONWEAPON] )
		{
			if( pMob->AttrEffectTime[MOB_ATTREFFECT_POISONWEAPON] + 10 > g_CurrRealTime )
				defDamage += (int)( attackDamage * 0.5 );
			else
				pMob->AttrEffectTime[MOB_ATTREFFECT_POISONWEAPON] = 0;
		}
		
		// 크리티컬 계산 
		float criDiceN = (float)( ( 0.07 * ( GET_DEX( pPlayer ) * 5 / 6 + GET_LATTACK( pPlayer ) ) ) + 1 );
		int M3 = GET_LATTACK( pPlayer ) / 20 + 5;
		int M2 = (int)( GET_LATTACK( pPlayer ) / 0.15 + 15 );
		int M1 = 100 - ( M3 + M2 );
		
		if( number( 1, 10000 ) <= (int)( criDiceN * 100 ) )
		{ 
			BYTE criRnd = number( 1, 100 );
			
			if( criRnd <= M1 )
			{
				btCritical = dCRITICAL_GOOD;
				defDamage += (int)( attackDamage * 0.5 );
			}
			else if( criRnd <= M2 )
			{
				btCritical = dCRITICAL_GREATE;
				defDamage += (int)( attackDamage * 0.75 );
			}
			else
			{
				btCritical = dCRITICAL_CRITICAL;
				defDamage += attackDamage;
			}
		}
		
		if( defDamage < (int)( attackDamage * 0.1 ) ) 
		{
			if( number( 1, 100 ) < 95 )
				defDamage = (int)( attackDamage * 0.1 );
			else 
				defDamage = 0;
		}
		damage = MAX( defDamage, 6550 );
	}

	

#ifdef _dUMENOKOJI_
	// 스킬공격이라면 몬스터의 마법방어값으로 방어
	if( pSkill )
	{
		int	MOB_MDP = GET_MOB_MDP( pMob );
		// 흑마법이나 백마법 이냐?
		if( IS_BMAGIC( pSkill ) || IS_WMAGIC( pSkill ) )
		{
			//스킬방어값이 데미지값보다 크면 그냥 0을 보낸다.
			if( MOB_MDP >= damage )
				damage = 0;
			
			else
				damage -= GET_MOB_MDP( pMob );			
			
		}
		
		else
		{
			if( MOB_MDP >= damage )
				damage = 0;
			else
				damage -= GET_MOB_MDP( pMob );
			
		}
	}

	// 일반공격이라면 물리방어값으로 방어
	else
	{
		//물리방어값이 데미지값보다 크면 그냥 0을 보낸다.
		if( GET_MOB_DP( pMob ) >= damage )
		{
			damage = 0;
		}
		else
		{
			damage -= GET_MOB_DP( pMob );	 
		}
	}

	if( GET_tDAMAGE( ) == TRUE || GET_tMDAMAGE( ) == TRUE )
	{
		damage += (int)( damage * 0.15 );
		INIT_tMDAMAGE(  );
	}
#endif

	// 버로우, 스텔스
	UnSetVisibleSkill( pPlayer );
	if( damage > 0 )
	{	
		// 경험치 저장 
		// 일반 공격
		if( !pSkill )
		{
			if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM )
				GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_SATTACK );
			else if( GET_USE_SKILL( pPlayer ) == dLATTACK_SKILL_ITEM )
				GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_LATTACK );
			else
			{
				if( GET_ITEM_TYPE2( GET_USE_SKILL( pPlayer ) ) == dITEMTYPE_MAGICSCROLL )
				{	
					sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
					
					if( pCurrSkill )
					{
						
						if( IS_BMAGIC( pCurrSkill ) )
							GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_BMAGIC );
						else if( IS_WMAGIC( pCurrSkill ) )
							GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_WMAGIC );
					}
				}
			}
		}
		// 마법이나 스킬의 경우
		else
		{
			if( pSkill->sattackExp > 0 )
			{
				if( pSkill->sattackExp >= 100 )
					GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_SATTACK );
				
				else
				{
					WORD exp = (WORD)( (float)damage * (float)( (float)pSkill->sattackExp/100.0F ) );

					GetMobExp( pPlayer, pMob, exp, dSKILL_ARRAY_SATTACK );
				}
			}

			if( pSkill->lattackExp > 0 )
			{
				if( pSkill->lattackExp >= 100 )
					GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_LATTACK );
				else
				{
					WORD exp = (WORD)( (float)damage * (float)( (float)pSkill->lattackExp/100.0F ) );

					GetMobExp( pPlayer, pMob, exp, dSKILL_ARRAY_LATTACK );
					
				}
			}

			if( pSkill->bmagicExp > 0 )
			{
				if( pSkill->bmagicExp >= 100 )
					GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_BMAGIC );
				else
				{
					WORD exp = (WORD)( (float)damage * (float)( (float)pSkill->bmagicExp/100.0F ) );

					GetMobExp( pPlayer, pMob, exp, dSKILL_ARRAY_BMAGIC );
					}
			}

			if( pSkill->wmagicExp > 0 )
			{
				if( pSkill->wmagicExp >= 100 )
					GetMobExp( pPlayer, pMob, damage, dSKILL_ARRAY_WMAGIC );
				else
				{
					WORD exp = (WORD)( (float)damage * (float)( (float)pSkill->wmagicExp/100.0F ) );

					GetMobExp( pPlayer, pMob, exp, dSKILL_ARRAY_WMAGIC );
					
				}
			}
		}

		BYTE isStun = 0, isDontMove = 0, isDontAttack = 0;

		// 목걸이 
		if( CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_TRAMPING ) )		// 9% 확률 스턴
		{
			if( number( 1, 100 ) <= 9 )
				isStun = 1;
		}
		
		// 반지 
		if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_DISTURBANCE_1 ) )	// 3% 이동 정지
		{
			if( number( 1, 100 ) <= 3 )
				isDontMove = 1;
		}
		else if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_DISTURBANCE_2 ) )	// 6% 이동정지 
		{
			if( number( 1, 100 ) <= 6 )
				isDontMove = 1;
		}
		else if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_DISTURBANCE_3 ) )	// 9% 이동정지
		{
			if( number( 1, 100 ) <= 9 )
				isDontMove = 1;
		}
		else if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_FEAR_1 ) )			// 3% 공격정지
		{
			if( number( 1, 100 ) <= 3 )
				isDontAttack = 1;
		}
		else if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_FEAR_2 ) )			// 6% 공격정지
		{
			if( number( 1, 100 ) <= 6 )
				isDontAttack = 1;	
		}
		else if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_FEAR_3 ) ||		// 9% 공격정지
			CheckEquipItem( pPlayer, dEQUIP_RING, dRING_FEAR_4 ) )
		{
			if( number( 1, 100 ) <= 9 )
				isDontAttack = 1;
		}
				
		// 스턴 
		if( isStun )
		{
			SendMobEffect( pMob, MOB_EFFECT_STUN );
			pMob->AttrEffectTime[MOB_ATTREFFECT_DONTMOVE] = g_CurrTime + 5000;
			pMob->AttrEffectTime[MOB_ATTREFFECT_CANTATTACK] = g_CurrTime + 5000;
		}

		// 이동 못함
		if( isDontMove )
		{
			SendMobEffect( pMob, MOB_EFFECT_DONTMOVE );
			
			pMob->AttrEffectTime[MOB_ATTREFFECT_DONTMOVE] = g_CurrTime + 5000;
		}

		// 공격 못함
		if( isDontAttack )
		{
			SendMobEffect( pMob, MOB_EFFECT_DONTATTACK );
			pMob->AttrEffectTime[MOB_ATTREFFECT_CANTATTACK] = g_CurrTime + 5000;
		}

		pMob->hp -= damage;		

		SendPlayerDamage( pPlayer, pMob, damage, btCritical );
	}
	else
		SendPlayerDamage( pPlayer, pMob, 0 , btCritical );

	CheckDeadMob( pPlayer, pMob );
}


//
// Function Name: DamagePvP
// Date: 
// Description: 
//
void DamagePvP( sPDESC_DATA pAttacker, sPDESC_DATA pVict, sPSKILL_DATA pSkill )
{

	if( !pAttacker || !pVict )
		return;

	if( pAttacker == pVict )
		return;

	if( pVict->isDead )
	{
		stop_fighting_player( pAttacker );
		return;	
	}

	if( !pAttacker->victPlayer )
		pAttacker->victPlayer = pVict;

	// 버로우나 스텔스
	UnSetVisibleSkill( pAttacker );
	UnSetVisibleSkill( pVict );

	WORD damage = 0;
	
	int attackDamage = 0;
	int defDamage = 0;

	attackDamage = GetPvPDamage( pAttacker, pVict );
	defDamage = attackDamage;

	//스킬 변경 
	//근거리 기본이거나 원거리 기본일때(즉, 물리공격일때만)만 이펙트 스킬 적용
	if( GET_USE_SKILL( pAttacker ) == dSATTACK_SKILL_ITEM || GET_USE_SKILL( pAttacker ) == dLATTACK_SKILL_ITEM )
	{
		defDamage += GetSkillAddDamage( pSkill, attackDamage );
		defDamage += GetSkillEffectAddDamage( pAttacker, attackDamage );
	}
	else
	{
		defDamage += GetSkillAddDamage( pSkill, attackDamage );
		defDamage += GetSkillEffectAddDamage( pAttacker, attackDamage );

		//마나 베리어 링을 장착하고 있다면
#ifdef _dUMENOKOJI_
		if( CheckEquipItem( pVict, dEQUIP_RING, dRING_BARRIER ) )
		{
			defDamage -= (int)( defDamage * 0.11 )	;
		}
#endif	
	}
	//버프 스킬 체크하고 적용
	//파이어 버스트 1, 다크파이어챈트 16, 데스파이어오라 17
	if( pAttacker->ch2.skillEffect[1].delay > 0 )
	{
		defDamage += (int)( attackDamage * 0.05 );
	}
	if( pAttacker->ch2.skillEffect[16].delay > 0 )
	{
		defDamage += (int)( attackDamage * 0.15 );
	}
	if( pAttacker->ch2.skillEffect[17].delay > 0 )
	{
		defDamage += (int)( attackDamage * 0.25 );
	}

	damage = MAX( defDamage, 65500 );
	
	if( damage > 0 )
	{
		GET_HP( pVict ) = MIN( GET_HP( pVict ) - damage, 0 );

		SendPlayerHP( pVict );
		
		SendPvPDamage( pAttacker, pVict, damage, dCRITICAL_NONE );
	}
	else
		SendPvPDamage( pAttacker, pVict, damage, dCRITICAL_NONE );

	// 반지 처리 
	if( CheckEquipItem( pAttacker, dEQUIP_NECK, dAMULET_TRAMPING ) )
	{
		if( number( 1, 100 ) <= 9 )
		{
			SetPlayerStun( pVict );
		}
	}

	if( GET_HP( pVict ) <= 0 )
	{
		BYTE nonChaCheck = 0;

#ifdef __CHINA_VER__
		StrSendSysMsg( pVict, 133, GET_NAME( pAttacker ) );
//		SendSystemMsg( pVict, g_LANG_STR[133], GET_NAME( pAttacker ) );
#endif

		// 전쟁 포인트 증가
#ifdef USE_GUILD_WAR
		sPWAR_DATA pWarData = NULL;

		sPWORLD pWorld = gWORLD[GET_MAP_NUM( pAttacker )];

		if( !pWorld )
			return;

		if( !pWorld->underAttack )
		{
			if( GET_GUILD( pAttacker ) && GET_GUILD( pVict ) )
			{
				pWarData = WAR_FindData( GET_GUILD( pAttacker ), GET_GUILD( pVict ) );
				
				if( pWarData )
				{
					if( IsPointTime() )
					{
						BYTE plusPoint = 0;
						
						if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_MEMBER )
							plusPoint = 1;
						else if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_SECONDMASTER )
							plusPoint = 3;
						else if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_MASTER )
							plusPoint = 5;
						
						if( GET_GUILD_LEVEL( pAttacker ) == dGUILD_LEVEL_SECONDMASTER )
							plusPoint += 1;
						else if( GET_GUILD_LEVEL( pAttacker ) == dGUILD_LEVEL_MASTER )
							plusPoint += 2;
						
						pWarData->ownerPoint += plusPoint;
						
						if( pWarData->lastUpdateTime + 180 <= g_CurrRealTime )
							WAR_UpdateWarData( pWarData );
					}
					nonChaCheck = 1;
				}
				else
				{
					pWarData = WAR_FindData( GET_GUILD( pVict ), GET_GUILD( pAttacker ) );
					
					if( pWarData )
						nonChaCheck = 1;
				}
			}
		}
		else
		{
			if( GET_GUILD( pAttacker ) && GET_GUILD( pVict ) )
			{
				if( GET_GUILD( pVict ) == pWorld->ownerGuild &&
					GET_GUILD( pAttacker ) == pWorld->warCristal.allowGuild[1] )
				{
					BYTE plusPoint = 0;
					
					if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_MEMBER )
						plusPoint = 1;
					else if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_SECONDMASTER )
						plusPoint = 3;
					else if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_MASTER )
						plusPoint = 5;
					
					if( GET_GUILD_LEVEL( pAttacker ) == dGUILD_LEVEL_SECONDMASTER )
						plusPoint += 1;
					else if( GET_GUILD_LEVEL( pAttacker ) == dGUILD_LEVEL_MASTER )
						plusPoint += 2;

					WAR_CheckEnd( pWorld, plusPoint );
				}
				else if( GET_GUILD( pVict ) == pWorld->warCristal.allowGuild[1] &&
					GET_GUILD( pAttacker ) == pWorld->ownerGuild )
				{
					BYTE plusPoint = 0;
					
					if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_MEMBER )
						plusPoint = 1;
					else if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_SECONDMASTER )
						plusPoint = 3;
					else if( GET_GUILD_LEVEL( pVict ) == dGUILD_LEVEL_MASTER )
						plusPoint = 5;
					
					if( GET_GUILD_LEVEL( pAttacker ) == dGUILD_LEVEL_SECONDMASTER )
						plusPoint += 1;
					else if( GET_GUILD_LEVEL( pAttacker ) == dGUILD_LEVEL_MASTER )
						plusPoint += 2;

					pWorld->warPoint = MIN( pWorld->warPoint - plusPoint, 0 );
				}
			}
		}

		if( pWorld->underAttack )
			nonChaCheck = 1;
#endif

#ifdef __CHINA_VER__
		if( GET_MAP_NUM( pAttacker ) == 15 )
		{
			nonChaCheck = 1;
		}
#endif

#ifndef dNON_PVP_MODE
		if( pVict->ch2.isEvilCha )
			nonChaCheck = 1;
#endif

		// 성향치 감소
		if( !nonChaCheck )
		{
			if( GET_CHA( pVict ) > -25000 )
			{
				GET_CHA( pAttacker ) = MIN( GET_CHA( pAttacker ) - 12000, -60000 );
				SendUpdateCha( pAttacker );
				pAttacker->isUpdate[dDATA_UPDATE_CHA] = 1;
			}
		}
		
#ifdef dUSE_DEAD_GHOST
		pVict->ch2.isDeadGhost = 1;
#endif

#ifdef __CHINA_VER__
		if( GET_MAP_NUM( pVict ) != 15 )
		{
			DropItemByCHA( pVict );
		}
#endif
		stop_fighting_player( pVict );
		stop_fighting_player( pAttacker );
		PlayerDie( pVict );
	}

}

//
// Function Name: MonsterDie
// Date: 2003-02-14
// Description: 몬스터 죽음 처리
//
void MonsterDie( sPMOB_DATA pMob )
{
#ifdef dEVENT_MOB_ON
	
	switch( pMob->mobNum )
	{
	case 101:
		g_EVENT_MOB[2].isAlive = 0;
		g_EVENT_MOB[2].updateTime = g_CurrRealTime + 10800;
		break;
	case 102:
		g_EVENT_MOB[0].isAlive = 0;
		g_EVENT_MOB[0].updateTime = g_CurrRealTime + 10800;
		break;
	case 103:
		g_EVENT_MOB[1].isAlive = 0;
		g_EVENT_MOB[1].updateTime = g_CurrRealTime + 10800;
		break;
	}
	
#endif

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DIE_MOB, g_nPos );
	PutWord( g_Packet, pMob->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToMobArea( pMob, g_Packet, g_nPos );
}

//
// Function Name: PlayerDie
// Date: 2003-02-14
// Description: 플레이어 죽음 처리
//
void PlayerDie( sPDESC_DATA pPlayer )
{
	if( !pPlayer )
		return;

#ifndef dNON_PVP_MODE
	if( pPlayer->ch2.isEvilCha )
	{
		pPlayer->ch2.isEvilCha = 0;
		SendPlayerNameColor( pPlayer );
	}
#endif

	GET_HP( pPlayer ) = 0;
	SendPlayerHP( pPlayer );

	// 스턴
	if( GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) )
		UnSetPlayerStun( pPlayer );

	// 거래 중이면
	if( pPlayer->ch2.dealVict )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_DEAL_FINISH, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer->ch2.dealVict );
		SendData( pPlayer );
		InitDealData( pPlayer->ch2.dealVict );
		InitDealData( pPlayer );
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DIE_USER, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOALL );
	GET_DEAD_STATE( pPlayer) = dDEAD_STATE_CORPSE;
}


//
// Function Name: GetAttackRange
// Date: 2003-03-21
// Description: 
//
int GetAttackRange( sPDESC_DATA pPlayer )
{
	if( !IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
		return dDEF_PLAYER_ATTACK_RANGE;

	switch( GET_USE_SKILL( pPlayer ) )
	{
	case dSATTACK_SKILL_ITEM:
		if( !pPlayer->equip[dEQUIP_WEAPON] )
			return dDEF_PLAYER_ATTACK_RANGE;
		else
			return GET_ITEM_ATTACKRANGE( pPlayer->equip[dEQUIP_WEAPON] );
		break;
	case dLATTACK_SKILL_ITEM:
		if( !pPlayer->equip[dEQUIP_BOW] )
			return dDEF_PLAYER_ATTACK_RANGE;
		else
			return GET_ITEM_ATTACKRANGE( pPlayer->equip[dEQUIP_BOW] );
		break;
	default:
		{
			sPSKILL_DATA pSkill = GET_SKILL_DATA( pPlayer );

			if( !pSkill )
				return dDEF_PLAYER_ATTACK_RANGE;

			return pSkill->range;
		}
	}
	return dDEF_PLAYER_ATTACK_RANGE;
}



//
// Function Name: IsPlayerAttackRange
// Date: 2003-03-21
// Description: 
//
BYTE IsPlayerAttackRange( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	int rangeX = abs( pPlayer->ch.posX - pMob->posX );
	int rangeY = abs( pPlayer->ch.posY - pMob->posY );
	
	int AttackRange = GetAttackRange( pPlayer );

	if( ( rangeX <= AttackRange ) && ( rangeY <= AttackRange ) )
		return 1;
	return 0;
}

//
// Function Name: IsPlayerPvPAttackRange
// Date: 
// Description: 
//
BYTE IsPlayerPvPAttackRange( sPDESC_DATA pAttacker, sPDESC_DATA pVict )
{
	int rangeX = abs( pAttacker->ch.posX - pVict->ch.posX );
	int rangeY = abs( pAttacker->ch.posY - pVict->ch.posY );

	int AttackRange = GetAttackRange( pAttacker );

	int rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );

	if( rangeValue <= AttackRange )
		return 1;

	return 0;
}

//
// Function Name: IsMobAttackRange
// Date: 2003-03-21
// Description: 
//
BYTE IsMobAttackRange( sPMOB_DATA pMob , sPDESC_DATA pPlayer )
{
	int rangeX = abs( pPlayer->ch.posX - pMob->posX );
	int rangeY = abs( pPlayer->ch.posY - pMob->posY );
	
	int AttackRange = GET_MOB_ATTACKRANGE( pMob ); 

	int rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );

	if( rangeValue <= AttackRange )
		return 1;

	return 0;
}

//
// Function Name: GetAttackDelay
// Date: 2003-03-21
// Description: 
//
int PluseRangeAttackDelay( sPDESC_DATA pPlayer, sPMOB_DATA pMob , BYTE delay )
{
	int rangeX = abs( GET_POSX( pPlayer ) - pMob->posX );
	int rangeY = abs( GET_POSY( pPlayer ) - pMob->posY );

	int rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );

	pPlayer->ch2.actionDelay += (int)(rangeValue * delay);

	return (int)(rangeValue * delay);
}

//
// Function Name: PluseRangeAttackDelay
// Date: 
// Description: 
//
int PluseRangeAttackDelay( sPDESC_DATA pAttacker, sPDESC_DATA pVict , BYTE delay )
{
	int rangeX = abs( GET_POSX( pAttacker ) - GET_POSX( pVict ) );
	int rangeY = abs( GET_POSY( pAttacker ) - GET_POSY( pVict ) );

	int rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );

	pAttacker->ch2.actionDelay += (int)(rangeValue * delay);

	return (int)(rangeValue * delay);
}

//
// Function Name: PluseRangeAttackDelay
// Date: 
// Description: 
//
int PluseRangeAttackDelay( sPMOB_DATA pMob, sPDESC_DATA pPlayer , BYTE delay )
{
	int rangeX = abs( GET_POSX( pPlayer ) - pMob->posX );
	int rangeY = abs( GET_POSY( pPlayer ) - pMob->posY );

	int rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );

	if( rangeValue > 2 )
		pMob->lastAttackTime += (int)( rangeValue * delay );

	return (int)( rangeValue * delay );
}

//
// Function Name: GetAttackDelay
// Date: 2003-03-21
// Description: 
//
DWORD GetAttackDelay( sPDESC_DATA pPlayer )
{
	// 근거리 
	if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM )
	{
		if( !pPlayer->equip[dEQUIP_WEAPON] )
			return 700;
		else
		{
			switch( g_ItemInfo[pPlayer->equip[dEQUIP_WEAPON]->itemNum]->attackSpeed )
			{
			case 0:		// 매우느림
				return 1900;
			case 1:		// 느림 
				return 1750;
			case 2:		// 보통
				return 1600;
			case 3:		// 빠름	
				return 1450;
			case 4:		// 매우빠름
				return 1300;
			}
		}
	}
	// 원거리
	else if( GET_USE_SKILL( pPlayer ) == dLATTACK_SKILL_ITEM )
	{
		if( !pPlayer->equip[dEQUIP_BOW] )
			return 5000;
		else
		{
			switch( g_ItemInfo[pPlayer->equip[dEQUIP_BOW]->itemNum]->attackSpeed )
			{
			case 0:		// 매우느림
				return 3000;
			case 1:		// 느림 
				return 2700;
			case 2:		// 보통
				return 2400;
			case 3:		// 빠름	
				return 2100;
			case 4:		// 매우빠름
				return 1800;
			}
		}
		return 1600;
	}

	return 1600;
}

//
// Function Name: IsValidAttackArea
// Date: 
// Description: 
//
BOOL IsValidAttackArea( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	if( pMob->mobNum == dMONSTER_CASTLEDOOR ||
		pMob->mobNum == dMONSTER_WARCRISTAL )
		return 1;
	
	int playerX, playerY;
	int mobX, mobY;

	playerX = GET_POSX( pPlayer );
	playerY = GET_POSY( pPlayer );

	mobX = pMob->posX;
	mobY = pMob->posY;

	if( IS_MOB_MOVABLE( GET_MAP_NUM( pPlayer ) , playerX, playerY ) )
		return 0;

	for( int i = 0; i < dONE_MAP_SIZE; i++ )
	{
		if( mobX < playerX )
			mobX++;
		else if( mobX > playerX )
			mobX--;

		if( mobY < playerY )
			mobY++;
		else if( mobY > playerY )
			mobY--;

		if( IS_MOB_MOVABLE( GET_MAP_NUM( pPlayer ), mobX, mobY ) )
			return 0;

		if( mobX == playerX && mobY == playerY )
			return 1;
	}
	return 1;
}

//
// Function Name: IsValidPvPAttackArea
// Date: 
// Description: 
//
BOOL IsValidPvPAttackArea( sPDESC_DATA pAttacker, sPDESC_DATA pVict )
{
	if( GET_MAP_KIND( pAttacker ) == dMAP_KIND_INDOOR )
		return 1;

	int attackerX, attackerY;
	int victX, victY;

	attackerX = GET_POSX( pAttacker );
	attackerY = GET_POSY( pAttacker );

	victX = GET_POSX( pVict );
	victY = GET_POSY( pVict );

	if( IS_MOB_MOVABLE( GET_MAP_NUM( pAttacker ) , attackerX, attackerY ) )
		return 0;

	for( int i = 0; i < dONE_MAP_SIZE; i++ )
	{
		if( victX < attackerX )
			victX++;
		else if( victX > attackerX )
			victX--;

		if( victY < attackerY )
			victY++;
		else if( victY > attackerY )
			victY--;

		if( IS_MOB_MOVABLE( GET_MAP_NUM( pAttacker ), victX, victY ) )
			return 0;

		if( victX == attackerX && victY == attackerY )
			return 1;
	}
	return 1;
}

//
// Function Name: CheckMyTeam
// Date: 
// Description: 
//
BOOL CheckMyTeam( sPDESC_DATA pAttacker, sPDESC_DATA pVict )
{
	if( gWORLD[GET_MAP_NUM( pAttacker )]->underAttack )
	{
		if( GET_GUILD( pAttacker ) == GET_GUILD( pVict ) )
			return 1;
	}

	return 0;
}


//







//EOF





















































































