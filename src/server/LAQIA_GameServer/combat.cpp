#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "player.h"
#include "player_world.h"
#include "combat.h"
#include "mob.h"
#include "mob_world.h"
#include "item.h"

#include "world.h"
#include "message.h"

#include "deal.h"
#include "constants.h"

#include "party.h"
#include "data.h"

#include "skills.h"
#include "quest.h"

sWEAPON_DAMAGE_TABLE	g_WeaponDmgTable[dMAX_ITEM_TYPE];

/*
 *	AddAction
 */
void AddPlayerAction( sPDESC_DATA pPlayer, BYTE targetType, int targetIdx, int valueType, int value, void *owner, DWORD delayTime )
{
	sPACTION		newAction = NULL;

	GET_MEMORY( g_MEMORY.actionMemory, newAction, sACTION, prev, next );

	if( !newAction )
		return;

	newAction->targetType = targetType;
	newAction->targetIndex = targetIdx;
	newAction->valueType = valueType;
	newAction->value = value;
	newAction->delayTime = delayTime;
	newAction->owner = owner;
	newAction->prev = NULL;
	newAction->next = NULL;

	INSERT_TO_LIST( pPlayer->ch.pActionList, newAction, prev, next );
}

/*
 *	AddAction
 */
void AddMobAction( sPMOB_DATA pMob, BYTE targetType, int targetIdx, int valueType, int value, void *owner, DWORD delayTime )
{
	sPACTION		newAction = NULL;

	GET_MEMORY( g_MEMORY.actionMemory, newAction, sACTION, prev, next );

	if( !newAction )
		return;

	newAction->targetType = targetType;
	newAction->targetIndex = targetIdx;
	newAction->valueType = valueType;
	newAction->value = value;
	newAction->delayTime = delayTime;
	newAction->owner = owner;
	newAction->prev = NULL;
	newAction->next = NULL;

	INSERT_TO_LIST( pMob->pActionList, newAction, prev, next );
}


/*
 *	RemoveAllAction
 */
void RemoveAllAction( sPDESC_DATA pPlayer )
{
	sPACTION act, next_act;

	LIST_WHILE( pPlayer->ch.pActionList, act, next_act, next );
	REMOVE_FROM_LIST( pPlayer->ch.pActionList, act, prev, next );
	INSERT_TO_LIST( g_MEMORY.actionMemory, act, prev, next );
	LIST_WHILEEND();
}

/*
 *	RemoveAllAction
 */
void RemoveAllAction( sPMOB_DATA pMob )
{
	sPACTION act, next_act;

	LIST_WHILE( pMob->pActionList, act, next_act, next );
	REMOVE_FROM_LIST( pMob->pActionList, act, prev, next );
	INSERT_TO_LIST( g_MEMORY.actionMemory, act, prev, next );
	LIST_WHILEEND();
}

/*
 *	PlayerActionProc
 */
void PlayerActionProc( sPDESC_DATA pPlayer )
{
	sPACTION act, next_act;

	LIST_WHILE( pPlayer->ch.pActionList, act, next_act, next );
	if( act->delayTime <= g_CurrTime )
	{
		if( act->valueType == dACTION_TYPE_DAMAGE || act->valueType == dACTION_TYPE_CRIDAMAGE )
		{
			PutHeader( dPACKET_PLAYER_POPDAMAGE );
			PutInteger( GET_IDX( pPlayer ) );
			PutInteger( act->value );
			PutByte( act->valueType == dACTION_TYPE_CRIDAMAGE ? 1 : 0 );

			SendToRange( pPlayer, g_Packet, g_nPos, 40, dSENDMODE_TOALL );
//			SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );

			if( !IS_SET( pPlayer->ch.flag, dPLAYER_FLAG_NOHIT ) )
				GET_HP( pPlayer ) = MIN( GET_HP( pPlayer ) - act->value, 0 );

			PLAYER_SendHPInfo( pPlayer );
			PLAYER_UpdateHP( pPlayer );

			// 죽었다!
			if( GET_HP( pPlayer ) <= 0 )
			{
				DeleteAreaDamage( pPlayer );
				PlayerDIE( pPlayer );
				return;
			}

			PARTY_SendHPMP( pPlayer );
		}
		else if( act->valueType == dACTION_TYPE_EFFECT )
		{
			if( act->targetType == dACTION_TARGET_MOB )
				PLAYER_PopEffect( pPlayer, dMOB, act->targetIndex, act->value );
			else if( act->targetType == dACTION_TARGET_PLAYER )
				PLAYER_PopEffect( pPlayer, dPLAYER, act->targetIndex, act->value );
		}
		else if( act->valueType == dACTION_TYPE_HEALHP )
		{
			int maxHP = GET_MAXHP( pPlayer ) + 
				GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDHP ) - 
				GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELHP ) +
				GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDHP ) -
				GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELHP );

			GET_HP( pPlayer ) = MAX( GET_HP( pPlayer ) + act->value, maxHP  );

			PLAYER_UpdateHP( pPlayer );
			PLAYER_SendHPInfo( pPlayer );
		
			if( GET_PARTY_MASTER( pPlayer ) )
				PARTY_SendHPMP( pPlayer );
		}
		else if( act->valueType == dACTION_TYPE_HEALMP )
		{
			int maxMP = GET_MAXMP( pPlayer ) + 
				GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDMP ) - 
				GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELMP ) +
				GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDHP ) -
				GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELHP );
			
			GET_MP( pPlayer ) = MAX( GET_MP( pPlayer ) + act->value, maxMP );

			PLAYER_UpdateMP( pPlayer );
			PLAYER_SendMPInfo( pPlayer );

			if( GET_PARTY_MASTER( pPlayer ) )
				PARTY_SendHPMP( pPlayer );
		}

		// 빼기
		REMOVE_FROM_LIST( pPlayer->ch.pActionList, act, prev, next );
		INSERT_TO_LIST( g_MEMORY.actionMemory, act, prev, next );
		
		LIST_SKIP();
	}
	LIST_WHILEEND();	
}

/*
 *	CalculationMobExp /몬스터랩차이 경험치  Monster Lab Difference EXP
 */
int	CalculationMobExp( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	int levelGap = GET_LEVEL( pPlayer ) - GET_MOB_LEVEL( pMob );

	if( levelGap < 8 )
		return GET_MOB_EXP( pMob );
	if( levelGap >= 17 )
		return 0;
	return 0;
}

/*
 *	MonsterDieProc
 */

BOOL MonsterDieProc( sPMOB_DATA pMob )
{
	if( pMob->hp <= 0 )
	{
		DeleteAreaDamage( pMob );

		// 경험치
		if( pMob->enemy )
		{
			GET_ENEMY_CNT( pMob->enemy ) = MIN( GET_ENEMY_CNT( pMob->enemy ) - 1, 0 );

			if( GET_LEVEL( pMob->enemy ) - GET_MOB_LEVEL( pMob ) <= dAGRO_LEVEL_GAP )
			{
				if( GET_PARTY_MASTER( pMob->enemy ) )
				{
					SplitPartyExp( pMob->enemy, GET_MOB_EXP( pMob ) );
				}
				else
				{
					SendSystemMsg( pMob->enemy, MakeString( "You have gained %d experience.", GET_MOB_EXP( pMob ) ) );
					GET_EXP( pMob->enemy ) += GET_MOB_EXP( pMob );
					PLAYER_UpdateExp( pMob->enemy );
					DATA_SendUpdateExp( pMob->enemy );
					CheckLevelUp( pMob->enemy );
				}
				GetMonsterItem( pMob );
			}

			GetMobQuestItem( pMob, pMob->enemy );
			
			if( pMob->enemy->ch.pQuestList )
			{
				if( !GET_PARTY_MASTER( pMob->enemy ) )
					QUEST_AddMonsterHunt( pMob->enemy, pMob );
				else
				{
					QUEST_AddMonsterHunt( GET_PARTY_MASTER( pMob->enemy ), pMob );
					
					for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
					{
						if( GET_PARTY_MASTER( pMob->enemy )->ch.party.member[i] )
							QUEST_AddMonsterHunt( GET_PARTY_MASTER( pMob->enemy )->ch.party.member[i], pMob );
					}
				}
			}
		}

		MonsterDIE( pMob );
		return 0;
	}
	return 1;
}

/*
 *	MonsterActionProc
 */
BOOL MonsterActionProc( sPMOB_DATA pMob )
{
	sPACTION act, next_act;

	LIST_WHILE( pMob->pActionList, act, next_act, next );

	if( act->delayTime <= g_CurrTime )
	{
		if( act->valueType == dACTION_TYPE_DAMAGE || act->valueType == dACTION_TYPE_CRIDAMAGE )
		{
			PutHeader( dPACKET_MOB_POPDAMAGE );
			PutInteger( pMob->idxNum );
			PutInteger( act->value );
			PutByte( act->valueType == dACTION_TYPE_CRIDAMAGE ? 1 : 0 );
//			SendToMobArea( pMob, g_Packet, g_nPos );
			SendToMobRange( pMob, g_Packet, g_nPos, 40 );

			pMob->hp = MIN( pMob->hp - act->value, 0 );

			MOB_SendHPInfo( pMob );

			// 죽었다!
			if( pMob->hp <= 0 )
			{
				DeleteAreaDamage( pMob );

				// 경험치
				if( pMob->enemy )
				{
					GET_ENEMY_CNT( pMob->enemy ) = MIN( GET_ENEMY_CNT( pMob->enemy ) - 1, 0 );

					if( GET_LEVEL( pMob->enemy ) - GET_MOB_LEVEL( pMob ) <= dAGRO_LEVEL_GAP )
					{
						if( GET_PARTY_MASTER( pMob->enemy ) )
						{
							SplitPartyExp( pMob->enemy, GET_MOB_EXP( pMob ) );
						}
						else
						{
							SendSystemMsg( pMob->enemy, MakeString( "You have gained %d experience.", GET_MOB_EXP( pMob ) ) );
							GET_EXP( pMob->enemy ) += GET_MOB_EXP( pMob );
							PLAYER_UpdateExp( pMob->enemy );
							DATA_SendUpdateExp( pMob->enemy );
							CheckLevelUp( pMob->enemy );
						}
						GetMonsterItem( pMob );
					}

					GetMobQuestItem( pMob, pMob->enemy );
					
					if( pMob->enemy->ch.pQuestList )
					{
						if( !GET_PARTY_MASTER( pMob->enemy ) )
							QUEST_AddMonsterHunt( pMob->enemy, pMob );
						else
						{
							QUEST_AddMonsterHunt( GET_PARTY_MASTER( pMob->enemy ), pMob );
							
							for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
							{
								if( GET_PARTY_MASTER( pMob->enemy )->ch.party.member[i] )
									QUEST_AddMonsterHunt( GET_PARTY_MASTER( pMob->enemy )->ch.party.member[i], pMob );
							}
						}
					}
				}

				MonsterDIE( pMob );
				return 0;
			}

			// 맞는 동작
			if( ( pMob->attackDelay - 400 ) <= g_CurrTime && pMob->hp > (int)( pMob->maxHp * 0.4 ) )
			{
				PutHeader( dPACKET_MONSTER_BLOW );
				PutInteger( pMob->idxNum );
//				SendToMobArea( pMob, g_Packet, g_nPos );
				SendToMobRange( pMob, g_Packet, g_nPos, 40 );
			}
		}
		else if( act->valueType == dACTION_TYPE_EFFECT )
		{
			if( act->targetType == dACTION_TARGET_MOB )
				MOB_PopEffect( pMob, dMOB, act->targetIndex, act->value );
			else if( act->targetType == dACTION_TARGET_PLAYER )
				MOB_PopEffect( pMob, dPLAYER, act->targetIndex, act->value );
		}
		
		// 뺴�?
		REMOVE_FROM_LIST( pMob->pActionList, act, prev, next );
		INSERT_TO_LIST( g_MEMORY.actionMemory, act, prev, next );

		LIST_SKIP();
	}

	LIST_WHILEEND();
	return 1;
}

/*
 *	PlayerDIE
 */
void PlayerDIE( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch.pDealVict )
		DEAL_Cancel( pPlayer );
	
	if( pPlayer->ch.myshopOpen )
		MYSHOP_Close( pPlayer );

	StopFightingEnemy( pPlayer );

	PutHeader( dPACKET_PLAYER_DIE );
	PutInteger( GET_IDX( pPlayer ) );
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );

//	GET_HP( pPlayer ) = GET_MAXHP( pPlayer );
//	GET_MP( pPlayer ) = GET_MAXMP( pPlayer );

	if( GET_LEVEL( pPlayer ) > 9 )
	{
		// 경험치 처리
		int exp = (int)( GetExp( GET_LEVEL( pPlayer ) ) * 0.03 );
		
		if( GET_EXP( pPlayer ) <= exp )
			exp = GET_EXP( pPlayer );
		
		if( exp > 0 )
		{
			GET_EXP( pPlayer ) = MIN( GET_EXP( pPlayer ) - exp, 0 );
			PLAYER_UpdateExp( pPlayer );
			DATA_SendUpdateExp( pPlayer );
			
			SendSystemMsg( pPlayer, MakeString( "You have lost %d experience.", exp ) );
		}
		//
	}

	RemoveAllAction( pPlayer );

	GET_DEAD_STATE( pPlayer ) = 1;

	GET_ENEMY_CNT( pPlayer ) = 0;

/*	GET_GOTO_MAPNUM( pPlayer ) = dDEF_PLAYER_MAPNUM;
	GET_GOTO_POSX( pPlayer ) = dDEF_PLAYER_POSX;
	GET_GOTO_POSY( pPlayer ) = dDEF_PLAYER_POSY;
	GET_GOTO_POSZ( pPlayer ) = dDEF_PLAYER_POSZ;

	PlayerFromMap( pPlayer );
	SendMapLoading( pPlayer );*/
}

/*
 *	MonsterDIE
 */
void MonsterDIE( sPMOB_DATA pMob )
{
	PutHeader( dPACKET_MONSTER_DIE );
	PutInteger( pMob->idxNum );
	SendToMobArea( pMob, g_Packet, g_nPos );

	DeleteMobData( pMob );
}

/*
 *	StopFightingEnemy
 */
void StopFightingEnemy( sPDESC_DATA pPlayer )
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
				LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m , w_next );
					
				if( m->enemy == pPlayer )
				{
					SET_MOB_ENEMY( m, NULL );
					ChangeMobState( m );

					m->hp = m->maxHp;
					m->mp = m->maxMp;
					MOB_SendHPInfo( m );
					MOB_SendMPInfo( m );
				}

				LIST_WHILEEND();
			}
		}
	}
}

/*
 *	GetPlayerArmor
 */
int GetPlayerArmor( sPDESC_DATA pPlayer )
{
	int statArmor = ( ( GET_PLAYER_STATUS( GET_CLASS( pPlayer ), dSTAT_CON ) + 
						GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDCON ) - GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELCON ) +
						GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDCON ) - GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELCON ) ) );

	MIN_CHECK( statArmor, 0 );

	int itemArmor = GET_PLAYER_ITEM_ARMOR( pPlayer ) + 
		GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDARMOR ) - GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELARMOR ) +
		GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDARMOR ) - GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELARMOR );

	MIN_CHECK( itemArmor, 0 );

	int point = ( GET_LEVEL( pPlayer ) * 2 ) + statArmor + itemArmor;
	int armorMastery = 0;

	if( IS_WARRIOR( GET_CLASS( pPlayer ) ) && GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_BODY ) )
	{
		armorMastery += GetSkillValue( pPlayer, dSKILL_HEAVYARMOR_MASTERY1 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_HEAVYARMOR_MASTERY2 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_HEAVYARMOR_MASTERY3 );
	}
	else if( IS_ARCHER( GET_CLASS( pPlayer ) ) && GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_BODY ) )
	{
		armorMastery += GetSkillValue( pPlayer, dSKILL_LIGHTARMOR_MASTERY1 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_LIGHTARMOR_MASTERY2 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_LIGHTARMOR_MASTERY3 );
	}
	else if( IS_TRADEMAN( GET_CLASS( pPlayer ) ) && GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_BODY ) )
	{
		armorMastery += GetSkillValue( pPlayer, dSKILL_FEEARMOR_MASTERY1 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_FEEARMOR_MASTERY2 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_FEEARMOR_MASTERY3 );
	}
	else if( IS_OFFICIAL( GET_CLASS( pPlayer ) ) && GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_BODY ) )
	{
		armorMastery += GetSkillValue( pPlayer, dSKILL_NORMALARMOR_MASTERY1 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_NORMALARMOR_MASTERY2 );
		armorMastery += GetSkillValue( pPlayer, dSKILL_NORMALARMOR_MASTERY3 );
	}

	int armorPoint = ( point + (int)( (float)point * (float)( armorMastery / 100.0F ) ) ) / 3;

	/*
	 	무사 90% >>> 105%
	궁수 100% >>> 110%
	문관 120% >>> 현상태 유지(120%)
	상인 85% >>> 105%
	 */
	if( IS_WARRIOR( GET_CLASS( pPlayer ) ) )
	{
		armorPoint = (int)( armorPoint * 1.05 );
	}
	else if( IS_ARCHER( GET_CLASS( pPlayer ) ) )
	{
		armorPoint = (int)( armorPoint * 1.1 );
	}
	else if( IS_OFFICIAL( GET_CLASS( pPlayer ) ) ) 
	{
		armorPoint = (int)( armorPoint * 1.2 );
	}
	else if( IS_TRADEMAN( GET_CLASS( pPlayer ) ) )
	{
		armorPoint = (int)( armorPoint * 1.05 );
	}

	return armorPoint;
}

/*
 *	GetMobArmor
 */
int GetMobArmor( sPMOB_DATA pMob )
{
	int point = ( GET_MOB_LEVEL( pMob ) * 2 ) 
		+ ( GET_MOB_CON( pMob ) + pMob->skillPoint[dEFF_VALUE_ADDCON] - pMob->skillPoint[dEFF_VALUE_DELCON] * 2 ) 
		+ GET_MOB_ARMOR( pMob ) + pMob->skillPoint[dEFF_VALUE_ADDARMOR] - pMob->skillPoint[dEFF_VALUE_DELARMOR];

	if( pMob->skillPoint[dEFF_VALUE_DELARMOR] )		// 방어력 감소
		point = MIN( point - pMob->skillPoint[dEFF_VALUE_DELARMOR], 0 );

	return ( point / 3 );
}

/*
 *	GetMobAttackDamage
 */
int GetMobAttackDamage( sPMOB_DATA pMob )
{
	int damage = ( GET_MOB_LEVEL( pMob ) * 2 ) 
		+ ( GET_MOB_STR( pMob ) + pMob->skillPoint[dEFF_VALUE_ADDSTR] - pMob->skillPoint[dEFF_VALUE_DELSTR] * 2 ) + GET_MOB_DAMAGE( pMob )
		+ pMob->skillPoint[dEFF_VALUE_ADDDAMAGE] - pMob->skillPoint[dEFF_VALUE_DELDAMAGE];

	if( pMob->skillPoint[dEFF_VALUE_DELDAMAGE] )		// 공격력 감소
		damage = MIN( damage - pMob->skillPoint[dEFF_VALUE_DELDAMAGE], 0 );

	return damage;
}



/*
 *	COMBAT_AttackToPlayer
 */
void COMBAT_AttackToPlayer( sPMOB_DATA pMob, sPDESC_DATA pVict )
{
	if( GET_DEAD_STATE( pVict ) )
		return;

	if( MobSkillAttack( pMob, pVict ) )
		return;

	BYTE aniType = number( 0, 1 );
	int attackSpeed = GET_MOB_ATTACKSPEED( pMob );

	if( pMob->skillPoint[dEFF_VALUE_DELATTACKSPEED] )		// 공속감소
		attackSpeed -= (int)( attackSpeed * 0.2 );

	int actionDelay = (int)( attackSpeed / 2 );

	if( GET_MOB_ACTIONTYPE( pMob ) == dMOB_ACTIONTYPE_LONGRANGE )
		actionDelay = attackSpeed;

	int mobDamage = GetMobAttackDamage( pMob );
	int damage = 0;
	int playerArmor = GetPlayerArmor( pVict );

	sPITEM_DATA pShield = GET_EQUIP_ITEM( pVict, dEQUIP_POS_SHIELD );

	// 방패 처리 
	if( pShield )
	{
		if( GET_ITEM_TYPE( pShield ) == dITEM_TYPE_SHIELD )
		{
			int plusePoint = 0;

			for( int i = 0; i < dITEM_EXVALUE; i++ )
			{
				if( pShield->exAttr[i] == dEFF_VALUE_ADDARMOR )
					plusePoint += pShield->exValue[i];
			}

			if( number( 1, 1000 ) <= GET_ITEM_DAMAGE( pShield ) + plusePoint )
				damage = 0;
			else
				damage = MIN( mobDamage - playerArmor, 1 );
		}
		else
			damage = MIN( mobDamage - playerArmor, 1 );
	}
	else
		damage = MIN( mobDamage - playerArmor, 1 );

	// 공격 패킷
	PutHeader( dPACKET_MOB_ATTACK );
	PutInteger( pMob->idxNum );
	
	PutInteger( GET_IDX( pMob->enemy ) );
	PutByte( dPLAYER );
	PutByte( aniType );
	PutInteger( attackSpeed );
//	SendToMobArea( pMob, g_Packet, g_nPos );
	SendToMobRange( pMob, g_Packet, g_nPos, 40 );
	
	int damageType = dACTION_TYPE_DAMAGE;

	if( number( 1, 10 ) == 1 )
	{
		damageType = dACTION_TYPE_CRIDAMAGE;
//		damage = mobDamage;
	}

	AddPlayerAction( pVict,
		dACTION_TARGET_SELF, 
		pMob->idxNum, 
		damageType, 
		damage, 
		(void*)pMob,
		g_CurrTime + actionDelay );

	// 원거리
	if( GET_MOB_ACTIONTYPE( pMob ) == dMOB_ACTIONTYPE_LONGRANGE )
	{
		AddMobAction( pMob, 
			dACTION_TARGET_PLAYER, 
			GET_IDX( pVict ), 
			dACTION_TYPE_EFFECT, 
			8, 
			NULL,
			g_CurrTime + ( actionDelay - (int)( actionDelay * 0.2 ) ) );
	}

	pMob->attackDelay = attackSpeed + 1000;
	pMob->lastAttackTime = g_CurrTime;
	pMob->lastMoveTime = g_CurrTime + 1000;

	if( pMob->agroLastVict == GET_IDX( pVict ) )
		pMob->agroTimer = g_CurrTime;
}

/*
 *	GetAttackSpeed
 */
DWORD GetAttackSpeed( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch.attackSpeed )
		return pPlayer->ch.attackSpeed;


	sPITEM_DATA pWeapon = GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON );
	sPITEM_DATA pBow = GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD );

	if( !pWeapon && !pBow )
		return 900;
	else
	{
		if( pWeapon )
			return GET_ITEM_ATTACKSPEED( pWeapon );
		else if( pBow )
		{
			if( GET_ITEM_TYPE( pBow ) == dITEM_TYPE_BOW )
				return GET_ITEM_ATTACKSPEED( pBow );
		}
		return 900;
	}
	return 900;
}

/*
 *	GetAttackDamage
 */
DWORD GetAttackDamage( sPDESC_DATA pPlayer, BYTE targetSize )
{
/*
 물리 순수 데미지 = 레벨별 기본 공격력 + STR 데미지 + 무기 데미지

                 = ( 레벨 x 2 ) + ( STR x 2 ) + ( 무기공격력 + 무기인첸 공격력 )

공격데미지 = ( 물리 순수 데미지 + 데미지 증가 버프 ) x (최소 무기 데미지 속성 ~ 최대 무기 데미지 속성 사이의 랜덤 값 + 숙련도(마스터리)) 

           ※ 무기 데미지 속성표는 테이블 존재(최대 0.75 ), 숙련도(마스터리) 값은 최소 0 ~ 최대 0.25 사이 값으로 고정

최종 데미지 = ( 공격 데미지 x 오행 속성효과(%) ) X 크기페널티 (%, 몹의 크기에 따라 페널티 부여)

            ※ 속성효과, 크기 페널티는 테이블 존재

크리티컬 데미지 = 크리티컬 데미지 + 숙련도(마스터리)

방패방어 발동 시 최소 DMG는 0

방패방어가 없을 시 최소 DMG는 1
Physical Physical Damage = Basic Attack by Level + STR Damage + Weapon Damage

                 = (Level x 2) + (STR x 2) + (Weapon Attack + Weapon Inchen Attack)

Attack Damage = (Pure Physical Damage + Damage Increase Buff) x (Random Value Between Min Weapon Damage Property ~ Max Weapon Damage Property + Proficiency (Mastery))

           ※ There is a table in the weapon damage attribute table (maximum 0.75), and the mastery (mastery) value is fixed at a value between minimum 0 and maximum 0.25

Final Damage = (Attack Damage x Five Elements Property Effect (%)) X Size Penalty (%, penalty given according to the size of the mob)

            ※ There is a table for attribute effects and size penalty

Critical Damage = Critical Damage + Mastery (Mastery)

When shield defense is activated, the minimum DMG is 0

If there is no shield defense, the minimum DMG is 1
 */
	//	 무기 속성 

	int	masteryVal = 0;
	int	weaponType = 0;
	int	statDamage = 0;
	
	sPITEM_DATA pShield = GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD );
	sPITEM_DATA pWeapon = GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON );
	
	if( pWeapon )
	{
		if( GET_ITEM_TYPE( pWeapon ) == dITEM_TYPE_CROSSBOW || 
			GET_ITEM_TYPE( pWeapon ) == dITEM_TYPE_BIWAN ||
			GET_ITEM_TYPE( pWeapon ) == dITEM_TYPE_TOO )
		{
			statDamage = ( GET_PLAYER_STATUS( GET_CLASS( pPlayer ), dSTAT_DEX ) + 
				GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDDEX ) - GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELDEX ) +
				GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDDEX ) - GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELDEX ) ) * 2;
			
			MIN_CHECK( statDamage, 0 );
		}
		
		weaponType = GET_ITEM_TYPE( pWeapon );
		
		// 무기 숙련도 처리
		switch( GET_ITEM_TYPE( pWeapon ) )
		{
		case dITEM_TYPE_ONEHANDDO:					// 한손도 
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDDO_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDDO_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDDO_MASTERY3 );
			break;
		case dITEM_TYPE_ONEHANDSWORD:				// 한손검 
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDSWORD_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDSWORD_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDSWORD_MASTERY3 );
			break;
		case dITEM_TYPE_SHORTSWORD:					// 단검 
			masteryVal += GetSkillValue( pPlayer, dSKILL_SHORTSWORD_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_SHORTSWORD_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_SHORTSWORD_MASTERY3 );
			break;
		case dITEM_TYPE_ONEHANDBLUNT:				// 한손 둔기
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDBLUNT_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDBLUNT_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDBLUNT_MASTERY3 );
			break;
		case dITEM_TYPE_ONEHANDAXE:					// 한손 도끼
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDAXE_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDAXE_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_ONEHANDAXE_MASTERY3 );
			break;
		case dITEM_TYPE_TWOHANDDO:					// 양손도
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDDO_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDDO_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDDO_MASTERY3 );
			break;
		case dITEM_TYPE_TWOHANDSWORD:				// 양손검 
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDSWORD_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDSWORD_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDSWORD_MASTERY3 );
			break;
		case dITEM_TYPE_SPEAR:						// 창 
			masteryVal += GetSkillValue( pPlayer, dSKILL_SPERE_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_SPERE_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_SPERE_MASTERY3 );
			break;
		case dITEM_TYPE_TWOHANDBLUNT:				// 양손둔기 
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDBLUNT_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDBLUNT_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDBLUNT_MASTERY3 );
			break;
		case dITEM_TYPE_TWOHANDAXE:					// 양손도끼 
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDAXE_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDAXE_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_TWOHANDAXE_MASTERY3 );
			break;
		case dITEM_TYPE_CROSSBOW:					// 석궁 
			masteryVal += GetSkillValue( pPlayer, dSKILL_CROSSBOW_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_CROSSBOW_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_CROSSBOW_MASTERY3 );
			break;
		case dITEM_TYPE_BIWAN:						// 비수
			masteryVal += GetSkillValue( pPlayer, dSKILL_BIWAN_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_BIWAN_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_BIWAN_MASTERY3 );
			break;			
		case dITEM_TYPE_TOO:
			masteryVal += GetSkillValue( pPlayer, dSKILL_TOO_MASTERY1 );			
			masteryVal += GetSkillValue( pPlayer, dSKILL_TOO_MASTERY2 );			
			masteryVal += GetSkillValue( pPlayer, dSKILL_TOO_MASTERY3 );			
			break;
		case dITEM_TYPE_STAFF:
			masteryVal += GetSkillValue( pPlayer, dSKILL_STAFF_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_STAFF_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_STAFF_MASTERY3 );
			break;
		case dITEM_TYPE_FAN:
			masteryVal += GetSkillValue( pPlayer, dSKILL_FAN_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_FAN_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_FAN_MASTERY3 );
			break;
		}
	}
	
	if( pShield )
	{
		if( GET_ITEM_TYPE( pShield ) == dITEM_TYPE_BOW )
		{
			weaponType = GET_ITEM_TYPE( pShield );

			statDamage = ( GET_PLAYER_STATUS( GET_CLASS( pPlayer ), dSTAT_DEX ) + 
				GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDDEX ) - GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELDEX ) +
				GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDDEX ) - GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELDEX ) ) * 2;
			
			MIN_CHECK( statDamage, 0 );
			
			// 궁 숙련도
			masteryVal += GetSkillValue( pPlayer, dSKILL_BOW_MASTERY1 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_BOW_MASTERY2 );
			masteryVal += GetSkillValue( pPlayer, dSKILL_BOW_MASTERY3 );			
		}
	}

	//

	int levelDefDmg = GET_LEVEL( pPlayer );// * 2;
	
	if( statDamage == 0 )
	{
		statDamage = ( GET_PLAYER_STATUS( GET_CLASS( pPlayer ), dSTAT_STR ) + 
			GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDSTR ) - GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELSTR ) +
			GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDSTR ) - GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELSTR ) ) * 2;
	}

	int weaponDamage = GET_PLAYER_ITEM_DAMAGE( pPlayer ) + 
					   GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDDAMAGE ) - GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELDAMAGE ) +
					   GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDDAMAGE ) - GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELDAMAGE );

	
	int defDamage = levelDefDmg + statDamage + weaponDamage;

	int attackDamage = (int)( defDamage * (float)( (float)( number( g_WeaponDmgTable[weaponType].minDmgRnd, g_WeaponDmgTable[weaponType].maxDmgRnd ) + masteryVal ) / 100.0F ) );

	float sizeRnd;

	switch( targetSize )
	{
	case 1:		// 소형
		sizeRnd = (float)g_WeaponDmgTable[weaponType].smallSizeDmg / 100.0F;
		break;
	case 2:		// 중형 
		sizeRnd = (float)g_WeaponDmgTable[weaponType].mediumSizeDmg / 100.0F;
		break;
	case 3:		// 대형 
		sizeRnd = (float)g_WeaponDmgTable[weaponType].largeSizeDmg / 100.0F;
		break;
	default:
		sizeRnd = 1.0F;
		break;
	}

	int finalDamage = (int)( (float)attackDamage * sizeRnd );

	return finalDamage;
}


/*
 *	GetAttackType
 */
BYTE GetAttackType( sPDESC_DATA pPlayer )
{
	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON ) )
	{
		if( GET_ITEM_TYPE( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON ) ) == dITEM_TYPE_CROSSBOW )
			return dATTACK_TYPE_LONGRANGE;
		else
			return dATTACK_TYPE_CLOSERANGE;
	}
	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD ) )
	{
		if( GET_ITEM_TYPE( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD ) ) == dITEM_TYPE_BOW )
			return dATTACK_TYPE_LONGRANGE;
	}
	return dATTACK_TYPE_CLOSERANGE;
}

/*
 *	COMBAT_TargetCircleDamage
 */
void COMBAT_TargetCircleDamage( sPDESC_DATA pPlayer, sPMOB_DATA pMob, int damage, int actionDelay, int range )
{
	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);
	
	sPMOB_DATA m, next_m;

	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				LIST_WHILE( gWORLD[pMob->mapNum]->mob[y][x], m, next_m, w_next );
				
				if( IS_SRC_MOB( m ) )
					LIST_SKIP();

				if( m == pMob )
					LIST_SKIP();

				if( GetDistance( pMob->posX, pMob->posY, m->posX, m->posY ) <= range )
				{
					AddMobAction( m, 
						dACTION_TARGET_SELF, 
						GET_IDX( pPlayer ),
						dACTION_TYPE_DAMAGE, 
						damage, 
						(void*)pPlayer,
						g_CurrTime + actionDelay );			
					
					if( !m->enemy )
					{
						SET_MOB_ENEMY( m, pPlayer );
						ChangeMobState( m );
						MOB_SetAgro( m, pPlayer );
					}
				}

				LIST_WHILEEND();
			}
		}
	}
}

/*
 *	COMBAT_AttackToMob
 */
void COMBAT_AttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	if( GET_DEAD_STATE( pPlayer ) )
	{
		SendAttackFailed( pPlayer );
		return;
	}

	if( GetDistance( GET_POSX( pPlayer ), GET_POSY( pPlayer ), pMob->posX, pMob->posY ) > GET_ATTACKRANGE( pPlayer ) + 4 )
	{
		SendAttackFailed( pPlayer );
		return;
	}

	if( !CheckAttackSourceItem( pPlayer ) )
	{
		SendAttackFailed( pPlayer );
		return;
	}

	BYTE aniType; //= number( 0, 4 );
	BYTE btIsAreaAttack = 0;

	if( number( 1, 1000 ) <= 100 )
		btIsAreaAttack = 1;

	if( pPlayer->ch.attackMotion < 3 )
		pPlayer->ch.attackMotion++;
	else
		pPlayer->ch.attackMotion = 0;
	
	aniType = pPlayer->ch.attackMotion;

	int attackSpeed = GetAttackSpeed( pPlayer );

	if( aniType == 4 )
		attackSpeed = attackSpeed * 3;

	// Bow effect when riding a horse
	int arrowEffect;

	if( GET_VEHICLE_NUM( pPlayer ) )
		arrowEffect = 9;
	else
		arrowEffect = 8;
	// 

	BYTE attackType = GetAttackType( pPlayer );
	int actionDelay = (int)( attackSpeed / 2 );
	
	if( attackType == dATTACK_TYPE_LONGRANGE )
		actionDelay = attackSpeed;

	int damage = GetAttackDamage( pPlayer, (BYTE)GET_MOB_SIZE( pMob ) );
	int mobArmor = GetMobArmor( pMob );

	damage = MIN( damage - mobArmor, 1 );

	if( IS_SET( pPlayer->ch.flag, dPLAYER_FLAG_FULL ) )
		damage = GET_MOB_HP( pMob );

	// 명중률 계산 
//	( GET_PLAYER_STATUS( GET_CLASS( pPlayer ), dSTAT_DEX ) * 4 ) 

	PutHeader( dPACKET_PLAYER_ATTACK );
	PutInteger( GET_IDX( pPlayer ) );
	PutInteger( pMob->idxNum );
	PutByte( dMOB );
	PutByte( aniType );							// 동작 번호
	PutInteger( attackSpeed );					// 동작 시간
//	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
	SendToRange( pPlayer, g_Packet, g_nPos, 40, dSENDMODE_TOALL );
	
	if( !pMob->enemy )
	{
		SET_MOB_ENEMY( pMob, pPlayer );
		ChangeMobState( pMob );
		MOB_SetAgro( pMob, pPlayer );
	}

	int damageType = dACTION_TYPE_DAMAGE;

	if( number( 1, 10 ) == 1 )
		damageType = dACTION_TYPE_CRIDAMAGE;

	// 일반 타격
//	if( aniType <= 3 )
	{
		AddMobAction( pMob, 
			dACTION_TARGET_SELF, 
			GET_IDX( pPlayer ),
			damageType, 
			damage, 
			(void*)pPlayer,
			g_CurrTime + actionDelay );

		// 원거리시
		if( attackType == dATTACK_TYPE_LONGRANGE )
		{
			AddPlayerAction( pPlayer,
				dACTION_TARGET_MOB,
				pMob->idxNum,
				dACTION_TYPE_EFFECT,
				arrowEffect,
				NULL,
				g_CurrTime + ( actionDelay - (int)( actionDelay * 0.2 ) ) );
		}

		// 지역 공격 
		if( btIsAreaAttack )
			COMBAT_TargetCircleDamage( pPlayer, pMob, MIN( (int)( damage * 0.3 ), 1 ), actionDelay, 1 );
	}
/*	else if( aniType == 3 )	// 3번 연타
	{
		int firstDelay = (int)( attackSpeed * 0.33 );
		int secondDelay = (int)( attackSpeed * 0.66 );

		AddMobAction( pMob, 
			dACTION_TARGET_SELF, 
			GET_IDX( pPlayer ),
			damageType, 
			damage, 
			(void*)pPlayer,
			g_CurrTime + firstDelay );
		AddMobAction( pMob, 
			dACTION_TARGET_SELF, 
			GET_IDX( pPlayer ),
			damageType, 
			damage, 
			(void*)pPlayer,
			g_CurrTime + secondDelay );

		// 원거리시
		if( attackType == dATTACK_TYPE_LONGRANGE )
		{				
			AddPlayerAction( pPlayer,
				dACTION_TARGET_MOB,
				pMob->idxNum,
				dACTION_TYPE_EFFECT,
				arrowEffect,
				NULL,
				g_CurrTime + ( firstDelay - (int)( firstDelay * 0.2 ) ) );
			
			AddPlayerAction( pPlayer,
				dACTION_TARGET_MOB,
				pMob->idxNum,
				dACTION_TYPE_EFFECT,
				arrowEffect,
				NULL,
				g_CurrTime + ( secondDelay - (int)( secondDelay * 0.2 ) ) );
		}
	}*/
/*	else if( aniType == 4 )		// 4번 크리티컬
	{
		AddMobAction( pMob, 
			dACTION_TARGET_SELF, 
			GET_IDX( pPlayer ),
			damageType, 
			damage, 
			(void*)pPlayer,
			g_CurrTime + actionDelay );

		// 원거리시
		if( attackType == dATTACK_TYPE_LONGRANGE )
		{
			AddPlayerAction( pPlayer,
				dACTION_TARGET_MOB,
				pMob->idxNum,
				dACTION_TYPE_EFFECT,
				arrowEffect,
				NULL,
				g_CurrTime + ( actionDelay - (int)( actionDelay * 0.2 ) ) );
		}

		// 지역 공격 
		COMBAT_TargetCircleDamage( pPlayer, pMob, MIN( (int)( damage * 0.3 ), 1 ), actionDelay, 1 );
	}*/
	
	GET_ACT_DELAY( pPlayer ) = g_CurrTime + attackSpeed;
	
	pPlayer->ch.lastAttackLink = GET_MOB_LINKNUM( pMob );

	MOB_SendHPInfo( pMob );

	if( pMob->enemy == pPlayer )
		pMob->lastAttackDamage += damage;

	if( pMob->enemy )
	{
		if( damage > pMob->lastAttackDamage && pMob->enemy != pPlayer )
		{
			SET_MOB_ENEMY( pMob, pPlayer );
			pMob->lastAttackDamage = damage;
			
			if( GET_PARTY_MASTER( pMob->enemy ) && GET_PARTY_MASTER( pPlayer ) ) 
			{
				if( GET_PARTY_MASTER( pMob->enemy ) != GET_PARTY_MASTER( pPlayer ) )
				{
					pMob->hp = pMob->maxHp;
					pMob->mp = pMob->maxMp;
				}
			}
			else
			{
				pMob->hp = pMob->maxHp;
				pMob->mp = pMob->maxMp;
			}
		}
	}
	else
	{
		SET_MOB_ENEMY( pMob, pPlayer );
	}

}

/*
 *	COMBAT_AttackPvP
 */
void COMBAT_AttackPvP( sPDESC_DATA pPlayer, sPDESC_DATA pVict )
{
	if( pPlayer == pVict )
		return;

	if( GET_DEAD_STATE( pPlayer ) )
	{
		SendAttackFailed( pPlayer );
		return;
	}

	if( GetDistance( GET_POSX( pPlayer ), GET_POSY( pPlayer ), GET_POSX( pVict ), GET_POSY( pVict ) ) > GET_ATTACKRANGE( pPlayer ) + 4 )
	{
		SendAttackFailed( pPlayer );
		return;
	}

	if( !CheckAttackSourceItem( pPlayer ) )
	{
		SendAttackFailed( pPlayer );
		return;
	}

	BYTE aniType; //= number( 0, 4 );
	BYTE btIsAreaAttack = 0;

	if( number( 1, 1000 ) <= 100 )
		btIsAreaAttack = 1;

	if( pPlayer->ch.attackMotion < 3 )
		pPlayer->ch.attackMotion++;
	else
		pPlayer->ch.attackMotion = 0;
	
	aniType = pPlayer->ch.attackMotion;

	int attackSpeed = GetAttackSpeed( pPlayer );

	if( aniType == 4 )
		attackSpeed = attackSpeed * 3;

	// 말을 탔을시에 활 이펙트
	int arrowEffect;

	if( GET_VEHICLE_NUM( pPlayer ) )
		arrowEffect = 9;
	else
		arrowEffect = 8;
	// 

	BYTE attackType = GetAttackType( pPlayer );
	int actionDelay = (int)( attackSpeed / 2 );
	
	if( attackType == dATTACK_TYPE_LONGRANGE )
		actionDelay = attackSpeed;

	int damage = GetAttackDamage( pPlayer, dMOB_SIZE_NONE );
	int victArmor = GetPlayerArmor( pVict );

	damage = MIN( damage - victArmor, 1 );

	PutHeader( dPACKET_PLAYER_ATTACK );
	PutInteger( GET_IDX( pPlayer ) );
	PutInteger( GET_IDX( pVict ) );
	PutByte( dPLAYER );
	PutByte( aniType );							// 동작 번호
	PutInteger( attackSpeed );					// 동작 시간
//	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
	SendToRange( pPlayer, g_Packet, g_nPos, 40, dSENDMODE_TOALL );
	
	int damageType = dACTION_TYPE_DAMAGE;

	if( number( 1, 10 ) == 1 )
		damageType = dACTION_TYPE_CRIDAMAGE;

	// 일반 타격
//	if( aniType <= 3 )
	{
		AddPlayerAction( pVict,
			dACTION_TARGET_SELF,
			GET_IDX( pPlayer ),
			damageType,
			damage,
			(void*)pPlayer,
			g_CurrTime + actionDelay );

		// 원거리시
		if( attackType == dATTACK_TYPE_LONGRANGE )
		{
			AddPlayerAction( pPlayer,
				dACTION_TARGET_PLAYER,
				GET_IDX( pVict ),
				dACTION_TYPE_EFFECT,
				arrowEffect,
				NULL,
				g_CurrTime + ( actionDelay - (int)( actionDelay * 0.2 ) ) );
		}

		// 지역 공격 
//		if( btIsAreaAttack )
//			COMBAT_TargetCircleDamage( pPlayer, pMob, MIN( (int)( damage * 0.3 ), 1 ), actionDelay, 1 );
	}
	
	GET_ACT_DELAY( pPlayer ) = g_CurrTime + attackSpeed;
	
}



/*
 *	GetItemAttackRange
 */
int GetItemAttackRange( sPITEM_DATA pItem )
{
	if( !IS_WEAPON( GET_ITEM_TYPE( pItem ) ) )
		return 2;

	switch( GET_ITEM_TYPE( pItem ) )
	{
	case dITEM_TYPE_ONEHANDSWORD:			//11 : 한손검
	case dITEM_TYPE_ONEHANDDO:				//12 : 한손도
	case dITEM_TYPE_ONEHANDBLUNT:			//13 : 한손둔기
	case dITEM_TYPE_ONEHANDAXE:				//14 : 한손도끼
	case dITEM_TYPE_SHORTSWORD:				//15 : 단검
		return 2;	
	case dITEM_TYPE_TWOHANDSWORD:			//16 : 양손검
	case dITEM_TYPE_TWOHANDDO:				//17 : 양손도
	case dITEM_TYPE_TWOHANDBLUNT:			//18 : 양손둔기
	case dITEM_TYPE_TWOHANDAXE:				//19 : 양손도끼
	case dITEM_TYPE_SPEAR:					//20 : 창
		return 3;							
	case dITEM_TYPE_BOW:					//21 : 활
		return 23;	
	case dITEM_TYPE_CROSSBOW:				//22 : 석궁
		return 18;	
	case dITEM_TYPE_FAN:					//23 : 부채
	case dITEM_TYPE_STAFF:					//24 : 지팡이
		return 2;
	case dITEM_TYPE_BIWAN:					//25 : 비완 
	case dITEM_TYPE_TOO:					//26 : 투(투척용무기)
		return 10;
	default:
		return 2;
	}
	return 2;
}


/*
 *	GetAttackRange	
 */
int	GetAttackRange( sPDESC_DATA pPlayer )
{
	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON ) )
	{
		return GetItemAttackRange( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON ) );
	}
	
	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD ) )
	{
		if( GET_ITEM_TYPE( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD ) ) == dITEM_TYPE_BOW )
		{
			return GetItemAttackRange( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD ) );
		}
	}
	return 2;
}


/*
 *	DeleteDamageAction
 */
void DeleteDamageAction( sPDESC_DATA pPlayer, sPMOB_DATA pOwner )
{
	sPACTION act, next_act;

	LIST_WHILE( pPlayer->ch.pActionList, act, next_act, next );
	if( ( act->valueType == dACTION_TYPE_DAMAGE || act->valueType == dACTION_TYPE_CRIDAMAGE ) && (sPMOB_DATA)act->owner == pOwner )
	{
		// 빼기
		REMOVE_FROM_LIST( pPlayer->ch.pActionList, act, prev, next );
		INSERT_TO_LIST( g_MEMORY.actionMemory, act, prev, next );
		
		LIST_SKIP();
	}
	LIST_WHILEEND();
}

void DeleteDamageAction( sPMOB_DATA pMob, sPDESC_DATA pOwner )
{
	sPACTION act, next_act;

	LIST_WHILE( pMob->pActionList, act, next_act, next );
	if( ( act->valueType == dACTION_TYPE_DAMAGE || act->valueType == dACTION_TYPE_CRIDAMAGE ) && (sPDESC_DATA)act->owner == pOwner )
	{
		// 빼기
		REMOVE_FROM_LIST( pMob->pActionList, act, prev, next );
		INSERT_TO_LIST( g_MEMORY.actionMemory, act, prev, next );
		
		LIST_SKIP();
	}
	LIST_WHILEEND();
}

void DeleteDamageAction( sPDESC_DATA pPlayer, sPDESC_DATA pOwner )
{
	sPACTION act, next_act;

	LIST_WHILE( pPlayer->ch.pActionList, act, next_act, next );
	if( ( act->valueType == dACTION_TYPE_DAMAGE || act->valueType == dACTION_TYPE_CRIDAMAGE ) && (sPDESC_DATA)act->owner == pOwner )
	{
		// 빼기
		REMOVE_FROM_LIST( pPlayer->ch.pActionList, act, prev, next );
		INSERT_TO_LIST( g_MEMORY.actionMemory, act, prev, next );
		
		LIST_SKIP();
	}
	LIST_WHILEEND();
}

/*
 *	DeleteAreaDamage
 */
void DeleteAreaDamage( sPDESC_DATA pPlayer )
{
	int posX = (int)(GET_POSX( pPlayer )/dONE_TILE);
	int posY = (int)(GET_POSY( pPlayer )/dONE_TILE);

	sPDESC_DATA d, next_d;
	sPMOB_DATA m, next_m;

	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d, w_next );
				DeleteDamageAction( d, pPlayer );
				LIST_WHILEEND();

				LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m, w_next );
				DeleteDamageAction( m, pPlayer );
				LIST_WHILEEND();
			}	
		}
	}
}

void DeleteAreaDamage( sPMOB_DATA pMob )
{
	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);

	sPDESC_DATA d, next_d;

	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				LIST_WHILE( gWORLD[pMob->mapNum]->player[y][x], d, next_d, w_next );
				DeleteDamageAction( d, pMob );
				LIST_WHILEEND();
			}	
		}
	}	
}


/*
 *	LoadWeaponDmgAttr
 */
BOOL LoadWeaponDmgAttr()
{
	for( int i = 0; i < dMAX_ITEM_TYPE; i++ )
	{
		g_WeaponDmgTable[i].criRnd = 0;						// 크리티컬 확률 
		g_WeaponDmgTable[i].hitRoll = 0;					// 명중률 
		g_WeaponDmgTable[i].minDmgRnd = 0;					// 최소타격데미지(%)
		g_WeaponDmgTable[i].maxDmgRnd = 0;					// 최대타격데미지(%)
		g_WeaponDmgTable[i].criDmgRnd = 0;					// 크리티컬데미지(%)
		g_WeaponDmgTable[i].smallSizeDmg = 0;				// 소 
		g_WeaponDmgTable[i].mediumSizeDmg = 0;				// 중 
		g_WeaponDmgTable[i].largeSizeDmg = 0;				// 대
	}

	CLoadDataFile cLoad;

	if( !cLoad.Load( dWEAPON_DMG_TABLE_FILE ) )
	{
		log( "LoadWeaponDmgAttr : file not found\r\n" );
		return 0;
	}
	
	char	name[128];
	int		type;
	int		criRnd;						// 크리티컬 확률 
	int		hitRoll;					// 명중률 
	int		minDmgRnd;					// 최소타격데미지(%)
	int		maxDmgRnd;					// 최대타격데미지(%)
	int		criDmgRnd;					// 크리티컬데미지(%)
	int		smallSizeDmg;				// 소 
	int		mediumSizeDmg;				// 중 
	int		largeSizeDmg;				// 대

	while( 1 )
	{
		BYTE res = cLoad.GetLine();

		if( res == CLoadDataFile::IS_EOF )
			break;

		if( res == CLoadDataFile::IS_SKIP )
			continue;

		cLoad.Get( name );
		type = cLoad.Get();		
		criRnd = cLoad.Get();				// 크리티컬 확률 
		hitRoll = cLoad.Get();				// 명중률 
		minDmgRnd = cLoad.Get();			// 최소타격데미지(%)
		maxDmgRnd = cLoad.Get();			// 최대타격데미지(%)
		criDmgRnd = cLoad.Get();			// 크리티컬데미지(%)
		smallSizeDmg = cLoad.Get();			// 소 
		mediumSizeDmg = cLoad.Get();		// 중 
		largeSizeDmg = cLoad.Get();			// 대

		if( type < 0 || type >= dMAX_ITEM_TYPE )
		{
			log( "LoadWeaponDmgAttr : bad item type\r\n" );
			return 0;
		}
		
		g_WeaponDmgTable[type].criRnd = criRnd;					// 크리티컬 확률 
		g_WeaponDmgTable[type].hitRoll = hitRoll;				// 명중률 
		g_WeaponDmgTable[type].minDmgRnd = minDmgRnd;			// 최소타격데미지(%)
		g_WeaponDmgTable[type].maxDmgRnd = maxDmgRnd;			// 최대타격데미지(%)
		g_WeaponDmgTable[type].criDmgRnd = criDmgRnd;			// 크리티컬데미지(%)
		g_WeaponDmgTable[type].smallSizeDmg = smallSizeDmg;		// 소 
		g_WeaponDmgTable[type].mediumSizeDmg = mediumSizeDmg;	// 중 
		g_WeaponDmgTable[type].largeSizeDmg = largeSizeDmg;		// 대

	}

	return 1;
}

/*
 *	SendAttackFailed
 */
void SendAttackFailed( sPDESC_DATA pPlayer )
{
	PutHeader( dPACKET_ATTACK_FAILED );
	SendData( pPlayer, g_Packet, g_nPos );
}

//EOF




