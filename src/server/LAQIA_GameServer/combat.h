/*
 *	Filename : 			combat.h
 *
 *	Author:				sk8snow
 *
 *	Description :		전투처리헤더	
 */

#ifndef __COMBAT_H__
#define __COMBAT_H__

/*
 *	D E F I N E S
 */
#define dACTION_TYPE_DAMAGE				1			// 데미지 
#define dACTION_TYPE_CRIDAMAGE			2			// 크리티컬 데미지
#define dACTION_TYPE_EFFECT				3			// 이펙트		
#define dACTION_TYPE_HEALHP				4			// HP 회복 
#define dACTION_TYPE_HEALMP				5			// MP 회복

#define dACTION_TARGET_SELF				0			// 자기자신 
#define dACTION_TARGET_MOB				1			// 몬스터 
#define dACTION_TARGET_PLAYER			2			// 플레이어

#define dATTACK_TYPE_CLOSERANGE			0			// 근거리
#define dATTACK_TYPE_LONGRANGE			1			// 원거리


typedef struct 
{
	int		criRnd;						// 크리티컬 확률 
	int		hitRoll;						// 명중률 
	int		minDmgRnd;						// 최소타격데미지(%)
	int		maxDmgRnd;						// 최대타격데미지(%)
	int		criDmgRnd;						// 크리티컬데미지(%)
	int		smallSizeDmg;						// 소 
	int		mediumSizeDmg;					// 중 
	int		largeSizeDmg;						// 대
} sWEAPON_DAMAGE_TABLE, *sPWEAPON_DAMAGE_TABLE;

extern sWEAPON_DAMAGE_TABLE	g_WeaponDmgTable[dMAX_ITEM_TYPE];

#define dWEAPON_DMG_TABLE_FILE		"./DATA/WEAPON_DMG_ATTR.TXT"


/*
 *	E X T E R N		F U N C T I O N S
 */
void AddPlayerAction( sPDESC_DATA pPlayer, BYTE targetType, int targetIdx, int valueType, int value, void *owner, DWORD delayTime );
void AddMobAction( sPMOB_DATA pMob, BYTE targetType, int targetIdx, int valueType, int value, void *owner, DWORD delayTime );
void RemoveAllAction( sPDESC_DATA pPlayer );
void RemoveAllAction( sPMOB_DATA pMob );
void PlayerDIE( sPDESC_DATA pPlayer );
void MonsterDIE( sPMOB_DATA pMob );


BOOL MonsterDieProc( sPMOB_DATA pMob );
void PlayerActionProc( sPDESC_DATA pPlayer );
BOOL MonsterActionProc( sPMOB_DATA pMob );

void StopFightingEnemy( sPDESC_DATA pPlayer );

void COMBAT_AttackToPlayer( sPMOB_DATA pMob, sPDESC_DATA pVict );
void COMBAT_AttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob );
void COMBAT_AttackPvP( sPDESC_DATA pPlayer, sPDESC_DATA pVict );

DWORD GetAttackSpeed( sPDESC_DATA pPlayer );
int	GetAttackRange( sPDESC_DATA pPlayer );

void DeleteDamageAction( sPDESC_DATA pPlayer, sPMOB_DATA pOwner );
void DeleteDamageAction( sPMOB_DATA pMob, sPDESC_DATA pOwner );
void DeleteDamageAction( sPDESC_DATA pPlayer, sPDESC_DATA pOwner );

void DeleteAreaDamage( sPDESC_DATA pPlayer );
void DeleteAreaDamage( sPMOB_DATA pMob );

DWORD GetAttackDamage( sPDESC_DATA pPlayer, BYTE targetSize );
DWORD GetAttackSpeed( sPDESC_DATA pPlayer );
BYTE GetAttackType( sPDESC_DATA pPlayer );

int GetMobArmor( sPMOB_DATA pMob );
int GetMobAttackDamage( sPMOB_DATA pMob );

BOOL LoadWeaponDmgAttr();
int GetPlayerArmor( sPDESC_DATA pPlayer );

void COMBAT_TargetCircleDamage( sPDESC_DATA pPlayer, sPMOB_DATA pMob, int damage, int actionDelay, int range );
void SendAttackFailed( sPDESC_DATA pPlayer );

#endif

//EOF

