/**
 *	$Workfile: fight.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 15 $
 *	$Date: 04-09-20 10:53p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/fight.h $
 * 
 * 15    04-09-20 10:53p Sk8snow
 * 
 * 14    04-09-11 11:20p Sk8snow
 * 
 * 13    04-07-13 7:26a Sk8snow
 * 
 * 12    04-04-30 9:46a Sk8snow
 * 
 * 11    04-04-07 10:17p Sk8snow
 * 
 * 10    04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __FIGHT_H__
#define __FIGHT_H__

#define dDEF_PLAYER_ATTACK_RANGE		2

#define dCRITICAL_NONE					0
#define dCRITICAL_GOOD					1
#define dCRITICAL_GREATE				2
#define dCRITICAL_CRITICAL				3

#define dATTACK_TYPE_NONE				0		// 전투중 아님
#define dATTACK_TYPE_SATTACK			1		// 근거리 
#define dATTACK_TYPE_LATTACK			2		// 원거리 (활)
#define dATTACK_TYPE_MAGIC				3		// 흑마법 

#define dLATTACK_DEF_DELAY				400

#define dMAX_DAMAGE_TABLE				256

#define dEXP_MODE_NONE					0
#define dEXP_MODE_SAVE					1

//

typedef struct
{
	WORD		minStrDamage;		//공격력(힘)
	WORD		maxStrDamage;
	WORD		minDexDamage;		//공격력(민첩)
	WORD		maxDexDamage;
	WORD		minIntDamage;		//공격력(지능)
	WORD		maxIntDamage;
	
	WORD		minSAttackDamage;	//근거리(공)
	WORD		maxSAttackDamage;	
	WORD		minLAttackDamage;	//원거리(공)
	WORD		maxLAttackDamage;	
	WORD		minBMagicDamage;	//흑마법(공)
	WORD		maxBMagicDamage;	
	WORD		minWMagicDamage;	//백마법(공)
	WORD		maxWMagicDamage;	
#ifdef _dUMENOKOJI_ 
	WORD		SAttackDefense;		//_dUMENOKOJI_  마방 관련
	WORD		LAttackDefense;
	WORD		BMagicDefense;		//_dUMENOKOJI_  마방 관련
	WORD		WMagicDefense;
#endif

	WORD		SAttackArmor;		// 근거리 방어
	WORD		LAttackArmor;		// 원거리 방어 
	WORD		BMagicArmor;		// 흑마법 방어 
	WORD		WMagicArmor;		// 백마법 방어
} sDAMAGE_TABLE, *sPDAMAGE_TABLE;

//

extern sDAMAGE_TABLE g_DAMAGE_TABLE[dMAX_DAMAGE_TABLE];

void PlayerDie( sPDESC_DATA pPlayer );
void MonsterDie( sPMOB_DATA pMob );

void set_fighting( sPDESC_DATA pPlayer, sPMOB_DATA pMob );
void stop_fighting_player( sPDESC_DATA pPlayer );
void stop_fighting_mob( sPMOB_DATA pMob );

void AttackToPlayer( sPMOB_DATA pMob, sPDESC_DATA pPlayer, BYTE divid = 1 );
void AttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob );

void SendMobDamage( sPMOB_DATA pMob, sPDESC_DATA pPlayer, WORD damage , BYTE isCritical );
void SendPlayerDamage( sPDESC_DATA pPlayer , sPMOB_DATA pMob, WORD damage , BYTE isCritical );

int GetAttackRange( sPDESC_DATA pPlayer );

BYTE IsPlayerAttackRange( sPDESC_DATA pPlayer, sPMOB_DATA pMob );
BYTE IsMobAttackRange( sPMOB_DATA pMob , sPDESC_DATA pPlayer );

WORD GetAttackDamage( sPDESC_DATA pPlayer, sPMOB_DATA pMob );
BYTE CheckDeadMob( sPDESC_DATA pPlayer , sPMOB_DATA pMob );

void stop_fighting_enemy( sPMOB_DATA pMob );
void stop_fighting_enemy( sPDESC_DATA pPlayer );

void SendBeginPlayerAttack( sPDESC_DATA pPlayer , int idxNum, BYTE targetKind );

int CalculationMobExp( sPDESC_DATA pPlayer, sPMOB_DATA pMob, WORD damage );

int CheckAllowAttack( sPDESC_DATA pPlayer );

void DamageToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob, sPSKILL_DATA pSkill );

void MagicAttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob, sPSKILL_DATA pSkill );

BOOL IsValidAttackArea( sPDESC_DATA pPlayer, sPMOB_DATA pMob );

BOOL LoadDamageTable();

void set_pvp_fight( sPDESC_DATA pAttacker, sPDESC_DATA pVict );
WORD GetPvPDamage( sPDESC_DATA pAttacker, sPDESC_DATA pVict );
void DamagePvP( sPDESC_DATA pAttacker, sPDESC_DATA pVict, sPSKILL_DATA pSkill );
void AttackPvP( sPDESC_DATA pAttacker, sPDESC_DATA pVict );

void SendPvPDamage( sPDESC_DATA pAttacker, sPDESC_DATA pVict, WORD damage, BYTE isCritical );
void MagicAttackPvP( sPDESC_DATA pAttacker, sPDESC_DATA pVict, sPSKILL_DATA pSkill );

BOOL IsValidPvPAttackArea( sPDESC_DATA pAttacker, sPDESC_DATA pVict );

BYTE IsPlayerPvPAttackRange( sPDESC_DATA pAttacker, sPDESC_DATA pVict );

void GetMobExp( sPDESC_DATA pPlayer, sPMOB_DATA pMob, WORD damage, BYTE skill );

int PluseRangeAttackDelay( sPDESC_DATA pPlayer, sPMOB_DATA pMob, BYTE delay );
int PluseRangeAttackDelay( sPMOB_DATA pMob, sPDESC_DATA pPlayer, BYTE delay );
int PluseRangeAttackDelay( sPDESC_DATA pAttacker, sPDESC_DATA pVict, BYTE delay );

BOOL CheckMyTeam( sPDESC_DATA pAttacker, sPDESC_DATA pVict );

BOOL IsCanAttackWeight( sPDESC_DATA pPlayer );

int	 DistanceDamage( int x, int y, int cacul, int minDamage, int maxDamage, float caculValue );

#endif


