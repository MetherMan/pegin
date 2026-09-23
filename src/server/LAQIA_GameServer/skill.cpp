/**
 *	$Workfile: skill.cpp $
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
 *	$Log: /GameServer/skill.cpp $
 * 
 * 37    04-12-22 5:18p Sk8snow
 * 
 * 36    04-12-15 10:40a Sk8snow
 * 
 * 35    04-11-18 1:40a Sk8snow
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
 * 28    04-05-21 11:37a Sk8snow
 * 
 * 27    04-05-18 1:17a Sk8snow
 * 
 * 26    04-05-14 10:38p Sk8snow
 * 
 * 25    04-05-11 1:45a Sk8snow
 * 
 * 24    04-04-30 9:46a Sk8snow
 * 
 * 23    04-04-03 4:03p Sk8snow
 * 
 * 22    04-03-31 8:22a Sk8snow
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
#include "local_level_limits.h"
#include "item.h"

#include "effect.h"

#include "party.h"

#include "pet.h"

int g_StateLevelExp[dMAX_LEVEL][3];
int g_SkillLevelExp[dMAX_LEVEL][4];

sPSKILL_DATA g_SKILL[dMAX_SKILLS];
sSPE_SKILL g_SPE_SKILL[dMAX_SPE_SKILL];

//
// Function Name: FindSkillIdx
// Date:
// Description: 
//
WORD FindSkillIdx( int itemNum )
{
	for( int i = 0; i < dMAX_SKILLS; i++ )
	{
		if( IS_VALID_SKILL( i ) )
		{
			if( g_SKILL[i]->itemNum == itemNum )
				return i;
		}
	}
	return 0;
}

//
// Function Name: LoadSkillData
// Date:
// Description: 
//
BOOL LoadSkillData()
{
	for( int i = 0; i < dMAX_SKILLS; i++ )
	{
		g_SKILL[i] = NULL;
	}
	
	FILE *fp = fopen( dSKILL_TABLE_FILE, "rb" );
	
	if( !fp )
		return 0;
		
	char readBuff[1024];
	char skillDesc[512];

	int nPos;
	
	int skillNum;				// 기술번호 
	char hName[50];				// 한글이름
	char eName[50];				// 영문이름
	int itemNum;				// 아이템 번호
	int skillKind;				// 기술종류
	int skillType;				// 기술형태
	int effectIndex;			// 이펙트 인덱스
	int reuse;					// 중복검색
	int skillAttr;				// 기술속성 
	int needSAttack;			// 요구근거리
	int needLAttack;			// 요구원거리
	int needBMagic;				// 요구흑마법
	int needWMagic;				// 요구백마법
	int needClass;				// 습득가능직업
	int minDamage;				// 최소데미지
	int maxDamage;				// 최대데미지
	int	effectType;				// 이펙트 종류
	int	effectValueType;		// 이펙트 값 종류
	int	effectValue;			// 이펙트 적용 값
	int needHP;					// 시전소모HP
	int needMP;					// 시전소모 MP
	int targetType;				// 시전대상
	int range;					// 시전거리
	int timer;					// 지속시간

	int	coolTime;			// 쿨타임 
	int	coolTimeIdx;		// 쿨타임 인덱스
	int	areaRange;			// 지역공격 범위

	int	tmp1;

	int			beforeSkill;		// 이전스킬
	
	int			sattackExp;			// 경험치 분배율
	int			lattackExp;
	int			bmagicExp;
	int			wmagicExp;



	int arrayNum = 0;

	while( !feof( fp ) )
	{
		nPos = 0;
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		skillNum = GetTabNumberic( readBuff, nPos );		// 스킬번호 
		GetTabString( readBuff, hName, nPos );				// 한글이름
		GetTabString( readBuff, eName, nPos );				// 영문이름
		itemNum = GetTabNumberic( readBuff, nPos );			// 아이템번호
		skillKind = GetTabNumberic( readBuff, nPos );		// 기술종류

		if( skillKind > 0 && skillKind != 4 )		// 클라이언트용 스킬 
			continue;

		skillType = GetTabNumberic( readBuff, nPos );		// 기술형태
		effectIndex = GetTabNumberic( readBuff, nPos );		// 이펙트 인덱스
		reuse = GetTabNumberic( readBuff, nPos );			// 중복검색
		skillAttr = GetTabNumberic( readBuff, nPos );		// 기술속성
		needSAttack = GetTabNumberic( readBuff, nPos );		// 요구근거리	 
		needLAttack = GetTabNumberic( readBuff, nPos );		// 요구원거리
		needBMagic = GetTabNumberic( readBuff, nPos );		// 요구흑마법
		needWMagic = GetTabNumberic( readBuff, nPos );		// 요구백마법 
		needClass = GetTabNumberic( readBuff, nPos );		// 필요직업 
		minDamage = GetTabNumberic( readBuff, nPos );		// 최소데미지
		maxDamage = GetTabNumberic( readBuff, nPos );		// 최대데미지 

		effectType = GetTabNumberic( readBuff, nPos );				// 이펙트 종류
		effectValueType = GetTabNumberic( readBuff, nPos );		// 이펙트 값 종류
		effectValue = GetTabNumberic( readBuff, nPos );			// 이펙트 적용 값
		
		needHP = GetTabNumberic( readBuff, nPos );			// 필요 HP
		needMP = GetTabNumberic( readBuff, nPos );			// 필요 MP
		targetType = GetTabNumberic( readBuff, nPos );		// 타겟 종류
		range = GetTabNumberic( readBuff, nPos );			// 시전거리 
		timer = GetTabNumberic( readBuff, nPos );			// 지속시간
	
		coolTime = GetTabNumberic( readBuff, nPos );			// 쿨타임 
		coolTimeIdx = GetTabNumberic( readBuff, nPos );		// 쿨타임 인덱스
		areaRange = GetTabNumberic( readBuff, nPos );		// 지역공격 범위

		tmp1 = GetTabNumberic( readBuff, nPos );;

		beforeSkill = GetTabNumberic( readBuff, nPos );		// 이전스킬

		GetTabString( readBuff, skillDesc, nPos );
		
		sattackExp = GetTabNumberic( readBuff, nPos );			// 경험치 분배율
		lattackExp = GetTabNumberic( readBuff, nPos );
		bmagicExp = GetTabNumberic( readBuff, nPos );
		wmagicExp = GetTabNumberic( readBuff, nPos );


		if( effectIndex >= dMAX_PLAYER_EFFECT )
		{
			log( "LoadSkillData : effectIndex >= dMAX_PLAYER_EFFECT\r\n" );
			fclose( fp );
			return 0;
		}

		if( coolTimeIdx >= dMAX_COOL_TIME )
		{
			log( "LoadSkillData : coolTimeIdx >= dMAX_COOL_TIME\r\n" );
			fclose( fp );
			return 0;
		}

		if( arrayNum >= dMAX_SKILLS )
		{
			log( "LoadSkillData : arrayNum >= dMAX_SKILLS\r\n" );
			fclose(fp);
			return 0;
		}

		if( !IS_VALID_SKILL( arrayNum ) )
		{
			g_SKILL[arrayNum] = (sPSKILL_DATA)malloc(sizeof(sSKILL_DATA));

			if( !IS_VALID_SKILL( arrayNum ) )
			{
				log( "LoadSkillData : malloc failed\r\n" );
				fclose(fp);
				return 0;
			}
		}
		
		strcpy( g_SKILL[arrayNum]->hName, hName );				// 한글이름 
		
		g_SKILL[arrayNum]->itemNum = itemNum;					// 아이템 번호 			
		g_SKILL[arrayNum]->skillType = skillType;				// 기술형태
		g_SKILL[arrayNum]->effectIndex = effectIndex;			// 몹타이머
		g_SKILL[arrayNum]->checkReuse = reuse;					// 중복검색
		g_SKILL[arrayNum]->skillAttr = skillAttr;				// 기술속성
		g_SKILL[arrayNum]->needSAttack = needSAttack;			// 요구근거리
		g_SKILL[arrayNum]->needLAttack = needLAttack;			// 요구원거리
		g_SKILL[arrayNum]->needBMagic = needBMagic;				// 요구흑마법
		g_SKILL[arrayNum]->needWMagic = needWMagic;				// 요구백마법
		g_SKILL[arrayNum]->needClass = needClass;				// 습득가능직업
		
		g_SKILL[arrayNum]->minDamage = minDamage;				// 최소데미지
		g_SKILL[arrayNum]->maxDamage = maxDamage;				// 최대데미지
				
		g_SKILL[arrayNum]->effectType = effectType;				// 이펙트 종류
		g_SKILL[arrayNum]->effectValueType = effectValueType;	// 이펙트 값 종류
		g_SKILL[arrayNum]->effectValue = effectValue;			// 이펙트 적용 값
		
		g_SKILL[arrayNum]->needHP = needHP;						// 시전소모 HP
		g_SKILL[arrayNum]->needMP = needMP;						// 시전소모 MP
		
		g_SKILL[arrayNum]->targetType = targetType;				// 시전대상
		g_SKILL[arrayNum]->range = range;						// 시전거리
		g_SKILL[arrayNum]->timer = timer;						// 지속시간 

		g_SKILL[arrayNum]->coolTime = coolTime;					// 쿨타임 
		g_SKILL[arrayNum]->coolTimeIdx = coolTimeIdx;			// 쿨타임 인덱스
		g_SKILL[arrayNum]->areaRange = areaRange;				// 지역공격범위
		g_SKILL[arrayNum]->tmp1 = tmp1;
		g_SKILL[arrayNum]->beforSkill = beforeSkill;			// 이전스킬

		g_SKILL[arrayNum]->sattackExp = sattackExp;				// 경험치 분배율
		g_SKILL[arrayNum]->lattackExp = lattackExp;
		g_SKILL[arrayNum]->bmagicExp = bmagicExp;
		g_SKILL[arrayNum]->wmagicExp = wmagicExp;

		g_SKILL[arrayNum]->func = SKILL_NullFunc;				// 스킬 함수

		arrayNum++;
	}
	
	fclose( fp );
	return 1;
}


//
// Function Name: get_skill_exp
// Date: 2003-05-22
// Description: 
//
int get_skill_exp( int lev , BYTE skill )
{
	if(skill>=4)return 1;
	int row=lev<0?1:(lev>=dMAX_LEVEL-1?dMAX_LEVEL-1:lev+1);
	int required=g_SkillLevelExp[row][skill];return required>0?required:1;
}

//
// Function Name: get_state_exp
// Date: 2003-05-22
// Description: 
//
int get_state_exp( int lev , BYTE state )
{
	return (g_StateLevelExp[MAX( lev+1, dMAX_LEVEL-1)][state]);
}


//
// Function Name: SendUpdatePointPacket
// Date: 2003-05-22
// Description: 
//
void SendUpdatePointPacket( sPDESC_DATA pPlayer, BYTE point )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_POINT, g_nPos );
	PutByte( g_Packet, point, g_nPos );

	switch( point )
	{
	case dPOINTUP_STR:			// STR
		PutWord( g_Packet, GET_STR_TOTAL( pPlayer ), g_nPos );
		break;
	case dPOINTUP_INT:			// INT
		PutWord( g_Packet, GET_INT_TOTAL( pPlayer ), g_nPos );
		break;
	case dPOINTUP_DEX:			// DEX
		PutWord( g_Packet, GET_DEX_TOTAL( pPlayer ), g_nPos );
		break;
	}

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer );
}


//
// Function Name: CheckLevelUp
// Date: 2003-05-22
// Description: 
//
void CheckLevelUp( sPDESC_DATA pPlayer , BYTE skill )
{
	if( !pPlayer )
	{
		log( "CheckLevelUp : !pPlayer \r\n" );
		return;
	}

	if( skill >= dMAX_PLAYER_SKILL )
	{
		log( "CheckLevelUp : skill >= dMAX_SKILL\r\n" );
		return;
	}

	int i;
	BYTE isUpdate = 0;
	BYTE stateUP = 0;

	for( i = 0; i < LOCAL_MAX_SKILL_LEVEL; i++ )
	{
        if(pPlayer->ch.skill[skill] >= LOCAL_MAX_SKILL_LEVEL) break;
		// 스킬 레벨 
		if( pPlayer->ch.skill_exp[skill] >=
			get_skill_exp( pPlayer->ch.skill[skill], skill ) )
		{
			pPlayer->ch.skill_exp[skill] = 
				MIN( pPlayer->ch.skill_exp[skill] - get_skill_exp( pPlayer->ch.skill[skill], skill ), 0 );
			pPlayer->ch.skill[skill]++;
			
			switch( skill )
			{
			case dSKILL_SHORT_ATTACK:
				GET_MAX_HP( pPlayer ) += g_STATE_MPHP_TABLE[0].SAttack;
				GET_MAX_MP( pPlayer ) += g_STATE_MPHP_TABLE[1].SAttack;
				
				GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
				GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer );

				sprintf( g_TMP_STR, g_LANG_STR[55], pPlayer->ch.skill[skill] );

				pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP1] = 1;
				break;
			case dSKILL_LONG_ATTACK:
				GET_MAX_HP( pPlayer ) += g_STATE_MPHP_TABLE[0].LAttack;
				GET_MAX_MP( pPlayer ) += g_STATE_MPHP_TABLE[1].LAttack;
				
				GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
				GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer );

				sprintf( g_TMP_STR, g_LANG_STR[56], pPlayer->ch.skill[skill] );

				pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP2] = 1;
				break;
			case dSKILL_BLACK_MAGIC:
				GET_MAX_HP( pPlayer ) += g_STATE_MPHP_TABLE[0].BMagic;
				GET_MAX_MP( pPlayer ) += g_STATE_MPHP_TABLE[1].BMagic;
				
				GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
				GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer );

				sprintf( g_TMP_STR, g_LANG_STR[57], pPlayer->ch.skill[skill] );

				pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP3] = 1;
				break;
			case dSKILL_WHITE_MAGIC:
				GET_MAX_HP( pPlayer ) += g_STATE_MPHP_TABLE[0].WMagic;
				GET_MAX_MP( pPlayer ) += g_STATE_MPHP_TABLE[1].WMagic;
				
				GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
				GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ); 

				sprintf( g_TMP_STR, g_LANG_STR[58], pPlayer->ch.skill[skill] );

				pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP4] = 1;
				break;
			}
			SendSystemMsg( pPlayer, g_TMP_STR );

			isUpdate = 1;
			stateUP = 1;
		}
		else
			break;
	}

	if( isUpdate )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_SKILL_UP, g_nPos );
		PutByte( g_Packet, skill, g_nPos );
		PutWord( g_Packet, pPlayer->ch.skill[skill], g_nPos );
		PutInteger( g_Packet, get_skill_exp( pPlayer->ch.skill[skill], skill ), g_nPos );
		PutInteger( g_Packet, pPlayer->ch.skill_exp[skill], g_nPos );
		PutSize( g_Packet, g_nPos );
		
		SendData( pPlayer, g_Packet, g_nPos );
		
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_SKILLUP_EVENT, g_nPos );
		PutInteger( g_Packet, pPlayer->sock, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOOTHER );
		
		DATASERV_SendUpdateSkill( pPlayer, skill );
		DATASERV_SendUpdateSkillExp( pPlayer, skill );

		SendSetExpMsg( pPlayer, skill, pPlayer->ch.skill_exp[skill] );

#ifndef dNOT_USE_PET
		if( GET_ALL_SKILL( pPlayer ) == 10 )
		{
			PET_SupplyPet( pPlayer );
		}
#endif
	}

	// init 
	isUpdate = 0;

	// INT 
	for( i = 0; i < 25; i++ )
	{
		if( pPlayer->ch.intelli_exp >=
			get_state_exp( GET_INT_TOTAL( pPlayer ), dSTATE_TABLE_INT ) )
		{		
			pPlayer->ch.intelli_exp = MIN( pPlayer->ch.intelli_exp - 
				get_state_exp( GET_INT_TOTAL( pPlayer ), dSTATE_TABLE_INT ), 0 );
			
			GET_INT( pPlayer )++;
			
			SendSystemMsg( pPlayer, g_LANG_STR[59], GET_INT_TOTAL( pPlayer ) );
			
			// HP++, MP++
			GET_MAX_MP( pPlayer ) += number( g_STATE_MPHP_TABLE[1].minInt, g_STATE_MPHP_TABLE[1].maxInt );
			GET_MAX_HP( pPlayer ) += number( g_STATE_MPHP_TABLE[0].minInt, g_STATE_MPHP_TABLE[0].maxInt );
			
			GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
			GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer );
			
			stateUP = 1;
			isUpdate = 1;
		}
		else
			break;
	}
	
	// INT Update
	if( isUpdate )
	{
		DATASERV_SendUpdateInt( pPlayer );
		DATASERV_SendUpdateIntExp( pPlayer );
		SendUpdatePointPacket( pPlayer, dPOINTUP_INT );
	}

	// init 
	isUpdate = 0;

	// STR
	for( i = 0; i < 25; i++ )
	{
		if( pPlayer->ch.str_exp >=
			get_state_exp( GET_STR_TOTAL( pPlayer ), dSTATE_TABLE_STR ) )
		{
			pPlayer->ch.str_exp = MIN( pPlayer->ch.str_exp -
				get_state_exp( GET_STR_TOTAL( pPlayer ), dSTATE_TABLE_STR ), 0 );
		
			GET_STR( pPlayer )++;
			
			SendSystemMsg( pPlayer, g_LANG_STR[60], GET_STR_TOTAL( pPlayer ) );
			
			// HP++, MP++
			GET_MAX_MP( pPlayer ) += number( g_STATE_MPHP_TABLE[1].minStr, g_STATE_MPHP_TABLE[1].maxStr );
			GET_MAX_HP( pPlayer ) += number( g_STATE_MPHP_TABLE[0].minStr, g_STATE_MPHP_TABLE[0].maxStr );
			
			GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
			GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer );
			
			stateUP = 1;
			isUpdate = 1;
		}
		else
			break;
	}

	// STR Update
	if( isUpdate )
	{
		DATASERV_SendUpdateStr( pPlayer );
		DATASERV_SendUpdateStrExp( pPlayer );
		SendUpdatePointPacket( pPlayer, dPOINTUP_STR );
	}
	
	// init
	isUpdate = 0;
	
	// DEX
	for( i = 0; i < 25; i++ )
	{
		if( pPlayer->ch.dex_exp >=
			get_state_exp( GET_DEX_TOTAL( pPlayer ), dSTATE_TABLE_DEX ) )
		{
			pPlayer->ch.dex_exp = MIN( pPlayer->ch.dex_exp - 
				get_state_exp( GET_DEX_TOTAL( pPlayer ), dSTATE_TABLE_DEX ), 0 );
		
			GET_DEX( pPlayer )++;
			
			SendSystemMsg( pPlayer, g_LANG_STR[61], GET_DEX_TOTAL( pPlayer ) );
			
			// HP++, MP++
			GET_MAX_MP( pPlayer ) += number( g_STATE_MPHP_TABLE[1].minDex, g_STATE_MPHP_TABLE[1].maxDex );
			GET_MAX_HP( pPlayer ) += number( g_STATE_MPHP_TABLE[0].minDex, g_STATE_MPHP_TABLE[0].maxDex );
			
			GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
			GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer );
			
			stateUP = 1;
			isUpdate = 1;
		}
		else
			break;
	}

	// DEX Update
	if( isUpdate )
	{
		DATASERV_SendUpdateDex( pPlayer );
		DATASERV_SendUpdateDexExp( pPlayer );
		SendUpdatePointPacket( pPlayer, dPOINTUP_DEX );
	}

	// HP, MP
	if( stateUP )
	{
		SendPlayerMaxHPMP( pPlayer );

		DATASERV_SendUpdateHp( pPlayer );
		DATASERV_SendUpdateMp( pPlayer );

		SendPlayerHP( pPlayer );
		SendPlayerMP( pPlayer );
	}
}


//
// Function Name: SendGetExpMsg
// Date: 2003-05-22
// Description: 
//
void SendGetExpMsg( sPDESC_DATA pPlayer, BYTE skill, WORD exp )
{
	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_GET_EXP, g_nPos );
	PutByte( g_Packet, skill, g_nPos );
	PutWord( g_Packet, exp, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: SendSetExpMsg
// Date: 2003-06-30
// Description: 
//
void SendSetExpMsg( sPDESC_DATA pPlayer, BYTE skill, int exp )
{
	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_SET_SKILLEXP, g_nPos );
	PutByte( g_Packet, skill, g_nPos );
	PutInteger( g_Packet, exp, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: MAGIC_GetNeedMp
// Date: 2003-06-12
// Description: 
//
int MAGIC_GetNeedMp( sPDESC_DATA pPlayer )
{	
	if( !IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
		return 10000;

	if( !IS_VALID_SKILL( g_ItemInfo[GET_USE_SKILL( pPlayer )]->skillIdx ) )
		return 10000;

	return GET_SKILL_DATA(pPlayer)->needMP;
}


//
// Function Name: MAGIC_GetDamage
// Date: 2003-06-12
// Description: 
//
int MAGIC_GetDamage( sPDESC_DATA pPlayer )
{
	if( GET_ITEM_TYPE2( GET_USE_SKILL( pPlayer ) ) != dITEMTYPE_MAGICSCROLL )
		return 0;

	sPSKILL_DATA pSkill = GET_SKILL_DATA( pPlayer );

	if( !pSkill )
		return 0;

	int damage = 0;
	int nInt = 0;
	int nLv = 0;
	int nSkill = 0;

	// 흑마법 
	if( IS_BMAGIC( pSkill ) )
	{
		nSkill = number( pSkill->minDamage, pSkill->maxDamage );
		nLv = number( g_DAMAGE_TABLE[GET_BMAGIC(pPlayer)].minBMagicDamage, g_DAMAGE_TABLE[GET_BMAGIC(pPlayer)].maxBMagicDamage );
		nInt = number( g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].minIntDamage, g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].maxIntDamage );

		damage = nSkill + nLv + nInt;
		//damage = number( pSkill->minDamage + g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].minIntDamage + g_DAMAGE_TABLE[GET_BMAGIC(pPlayer)].minBMagicDamage,
		//	pSkill->maxDamage + g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].maxIntDamage + g_DAMAGE_TABLE[GET_BMAGIC(pPlayer)].maxBMagicDamage );

		//GetPvPDamage()로 나가면 그곳에서 작업함...
		if( pPlayer->equip[dEQUIP_WEAPON] )
		{
			if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) == dITEMTYPE_STAFF )
			{
				damage += number( GET_ITEM_MINMANADMG( pPlayer->equip[dEQUIP_WEAPON] ),
						GET_ITEM_MAXMANADMG( pPlayer->equip[dEQUIP_WEAPON] ) );
				//damage += number( GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_WEAPON] ),
				//		GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_WEAPON] ) );
			}
		} //umenokoji
	}
	// 백마법shj1218
	else if( IS_WMAGIC( pSkill ) )
	{
		nSkill = number( pSkill->minDamage, pSkill->maxDamage );
		nLv = number( g_DAMAGE_TABLE[GET_WMAGIC(pPlayer)].minWMagicDamage, g_DAMAGE_TABLE[GET_WMAGIC(pPlayer)].maxWMagicDamage );
		nInt = number( g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].minIntDamage, g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].maxIntDamage );

		damage = nSkill + nLv + nInt;

//		damage = number( pSkill->minDamage + g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].minIntDamage + g_DAMAGE_TABLE[GET_WMAGIC(pPlayer)].minWMagicDamage, 
//			pSkill->maxDamage + g_DAMAGE_TABLE[GET_INT_TOTAL(pPlayer)].maxIntDamage + g_DAMAGE_TABLE[GET_WMAGIC(pPlayer)].maxWMagicDamage );

		if( pPlayer->equip[dEQUIP_WEAPON] ) //GetPvPDamage()로 나가면 그곳에서 작업함...
		{
			if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) == dITEMTYPE_STAFF )
			{
				damage += number( GET_ITEM_MINMANADMG( pPlayer->equip[dEQUIP_WEAPON] ),
						GET_ITEM_MAXMANADMG( pPlayer->equip[dEQUIP_WEAPON] ) );
			}

		} //umenokoji
	}
	//else
	//	damage = 0;

	return damage;
}


//
// Function Name: AddPlayerSkillDirect
// Date: 
// Description: 
//
void AddPlayerSkillDirect( sPDESC_DATA pPlayer, int itemNum )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "insert into UserSkills values( '', '%s', %d, %d )",
		escID, GET_CHAR_POS( pPlayer ), itemNum );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}


//
// Function Name: FindSpeSkill
// Date: 
// Description: 
//
sPSPE_SKILL FindSpeSkill( int skillNum )
{
	for( int i = 0; i < dMAX_SPE_SKILL; i++ )
	{
		if( g_SPE_SKILL[i].itemNum == 0 )
			break;

		if( g_SPE_SKILL[i].itemNum == skillNum )
			return &g_SPE_SKILL[i];
	}
	return NULL;
}

//
// Function Name: AddPlayerSkill
// Date: 
// Description: 
//
BOOL AddPlayerSkill( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	sPSKILL_DATA pSKill = g_SKILL[g_ItemInfo[pItem->itemNum]->skillIdx];


	if( !pSKill )
		return 0;
	
	// 근거리 
	if( pSKill->needSAttack > GET_SATTACK( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[63], pSKill->needSAttack );
		return 0;
	}
	
	// 원거리 
	if( pSKill->needLAttack > GET_LATTACK( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[64], pSKill->needLAttack );
		return 0;
	}
	
	// 흑마법
	if( pSKill->needBMagic > GET_BMAGIC( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[65], pSKill->needBMagic );
		return 0;
	}
	
	// 백마법
	if( pSKill->needWMagic > GET_WMAGIC( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[66], pSKill->needWMagic );
		return 0;
	}

	if( pSKill->beforSkill > 0 )
	{
		if( IS_VALID_ITEM( pSKill->beforSkill ) )
		{
			if( !IsHaveSkill( pPlayer, pSKill->beforSkill ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[266], GET_ITEM_HNAME2( pSKill->beforSkill ) );
				return 0;
			}
		}
		else
			return 0;
	}

	// 특화 스킬
	if( IS_SPESKILL( pSKill ) )
	{
		sPSPE_SKILL pSpeSkill = FindSpeSkill( pItem->itemNum );
		
		if( !pSpeSkill )
			return 0;

		GET_CLASS( pPlayer ) = pSpeSkill->classNum;
		DATASERV_UpdateClass( pPlayer );
		SendPlayerClass( pPlayer );
	}
	// 특화스킬 End

	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "insert into UserSkills values( '', '%s', %d, %d )",
		escID, GET_CHAR_POS( pPlayer ), pItem->itemNum );

	if( !mysql_query( g_MYSQL, query ) )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_ADD_SKILL, g_nPos );
		PutWord( g_Packet, pItem->itemNum, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );

		// 추가
		AddUserSkillData( pPlayer, pItem->itemNum );

		return 1;
	}
	else
		log( "%s",mysql_error( g_MYSQL ) );

	return 0;
}



//
// Function Name: SendPlayerSkillList
// Date: 
// Description: 
//
void SendPlayerSkillList( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "select skillNum from UserSkills where ownerID = '%s' and ownerPos = %d",
		escID, GET_CHAR_POS( pPlayer ) );

	MYSQL_RES *result;
	MYSQL_ROW row;
	BYTE cnt = 2;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SKILL_LIST, g_nPos );
	g_nPos += sizeof(BYTE);

	// 근거리 , 원거리
	PutWord( g_Packet, dSATTACK_SKILL_ITEM, g_nPos );	
	PutWord( g_Packet, dLATTACK_SKILL_ITEM, g_nPos );

	if( !mysql_query( g_MYSQL, query ) )
	{
		result = mysql_store_result( g_MYSQL );
		
		if( result )
		{
			while( ( row = mysql_fetch_row( result ) ) )
			{
				PutWord( g_Packet, atoi( row[0] ), g_nPos );

				// Add
				AddUserSkillData( pPlayer, atoi( row[0] ) );

				cnt++;
			}
			*(BYTE*)(g_Packet + 4) = cnt;
			PutSize( g_Packet, g_nPos );
			SendData( pPlayer );

			mysql_free_result( result );
		}
	}
	else
		log( "%s",mysql_error( g_MYSQL ) );


	// 소유 스킬 체크
	if( GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM &&
		GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		if( !IsHaveSkill( pPlayer , GET_USE_SKILL( pPlayer ) ) )
			GET_USE_SKILL( pPlayer ) = dSATTACK_SKILL_ITEM;
	}
}


//
// Function Name: MAGIC_SendMagicInfo
// Date: 2003-06-12
// Description: 
//
void MAGIC_SendMagicInfo( sPDESC_DATA pPlayer )
{
	/*
	#define dMAGIC_TYPE_ATTACK		0			// 공격형 마법 
	#define dMAGIC_TYPE_CAST		1			// 사용 마법

	#define dMAGIC_VICT_TARGET		0			// 타겟  지정 마법
	#define dMAGIC_VICT_BYSELF		1			// 자기 자신에게 만
	#define dMAGIC_VICT_BOTH		2			// 자기자신 또는 타겟 지정 가능
	*/

	if( !IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
		return;

	if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM ||
		GET_USE_SKILL( pPlayer ) == dLATTACK_SKILL_ITEM )
		return;

	sPSKILL_DATA pSkill = GET_SKILL_DATA( pPlayer );

	if( !pSkill )
		return;

	BYTE victType;

	switch( pSkill->targetType )
	{	
	case dMAGIC_VICT_TARGET:			// 타겟  지정 마법
	case dMAGIC_VICT_BYSELF:			// 자기 자신에게 만
	case dMAGIC_VICT_BOTH:				// 자기자신 또는 타겟 지정 가능
		victType = pSkill->targetType;
		break;
	default:
		victType = dMAGIC_VICT_TARGET;
		break;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MAGIC_INFO, g_nPos );
	PutByte( g_Packet, victType, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer );
}

//
// Function Name: LoadStateExpTable
// Date: 
// Description: 
//
BOOL LoadStateExpTable()
{
	for( int i = 0; i < dMAX_LEVEL; i++ )
	{
		g_StateLevelExp[i][0] = 0;
		g_StateLevelExp[i][1] = 0;
		g_StateLevelExp[i][2] = 0;
	}

	FILE *fp = fopen( dSTATE_EXP_TABLE_FILE, "rb" );

	if( !fp )
		return 0;

	int exp[3];
	int level;
	
	char readBuff[1024];
	
	while( !feof( fp ) )
	{
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		if( sscanf( readBuff, "%d %d %d %d", &level, &exp[0], &exp[1], &exp[2] ) != 4 )
		{
			log( "LoadStateExpTable : Bad File Format\r\n" );
			fclose( fp );
			return 0;
		}
		
		if( level >= dMAX_LEVEL )
		{
			log( "LoadStateExpTable : level >= dMAX_LEVEL\r\n" );
			fclose ( fp );
			return 0;
		}
		
		g_StateLevelExp[level][0] = exp[0];
		g_StateLevelExp[level][1] = exp[1];
		g_StateLevelExp[level][2] = exp[2];
	}
	
	fclose( fp );
	return 1;
}

//
// Function Name: LoadSkillExpTable
// Date: 
// Description: 
//
BOOL LoadSkillExpTable()
{
	for( int i = 0; i < dMAX_LEVEL; i++ )
	{
		g_SkillLevelExp[i][0] = 0;
		g_SkillLevelExp[i][1] = 0;
		g_SkillLevelExp[i][2] = 0;
		g_SkillLevelExp[i][3] = 0;		
	}

	FILE *fp = fopen( dSKILL_EXP_TABLE_FILE, "rb" );

	if( !fp )
		return 0;

	int exp[4];
	int level;
	
	char readBuff[1024];
	
	while( !feof( fp ) )
	{
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		if( sscanf( readBuff, "%d %d %d %d %d", &level, &exp[0], &exp[1], &exp[2], &exp[3] ) != 5 )
		{
			log( "LoadSkillExpTable : Bad File Format\r\n" );
			fclose( fp );
			return 0;
		}
		
		if( level >= dMAX_LEVEL )
		{
			log( "LoadSkillExpTable : level >= dMAX_LEVEL\r\n" );
			fclose ( fp );
			return 0;
		}
		g_SkillLevelExp[level][0] = exp[0];
		g_SkillLevelExp[level][1] = exp[1];
		g_SkillLevelExp[level][2] = exp[2];
		g_SkillLevelExp[level][3] = exp[3];
	}
	
	fclose( fp );
	return 1;
}

//
// Function Name: LoadSpeSkillData
// Date: 
// Description: 
//
BOOL LoadSpeSkillData()
{
	for( int i = 0; i < dMAX_SPE_SKILL; i++ )
	{
		g_SPE_SKILL[i].itemNum = 0;
		g_SPE_SKILL[i].classNum = 0;
		g_SPE_SKILL[i].needSkill[0] = 0;
		g_SPE_SKILL[i].needSkill[1] = 0;
		g_SPE_SKILL[i].needSkill[2] = 0;
		g_SPE_SKILL[i].needSkill[3] = 0;
		g_SPE_SKILL[i].skillIdx[0] = 0;
		g_SPE_SKILL[i].skillIdx[1] = 0;
	}

	FILE *fp;

	fp = fopen( dSPE_SKILL_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadSpeSkillData : file open failed\r\n" );
		return 0;
	}

	char readBuff[1024];

	int arrayNum = 0;
	int nPos;

	int itemNum;
	char skillName[1024];
	int skill[4];
	int classNum;
	BYTE skillIdx[2];

	while( !feof( fp ) )
	{
		nPos = 0;

		fgets( readBuff, 1024, fp );

		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		itemNum = GetTabNumberic( readBuff, nPos );
		GetTabString( readBuff, skillName, nPos );
		skill[0] = GetTabNumberic( readBuff, nPos );
		skill[1] = GetTabNumberic( readBuff, nPos );
		skill[2] = GetTabNumberic( readBuff, nPos );
		skill[3] = GetTabNumberic( readBuff, nPos );
		classNum = GetTabNumberic( readBuff, nPos );
		skillIdx[0] = GetTabNumberic( readBuff, nPos );
		skillIdx[1] = GetTabNumberic( readBuff, nPos );

		g_SPE_SKILL[arrayNum].itemNum = itemNum;
		g_SPE_SKILL[arrayNum].classNum = classNum;
		g_SPE_SKILL[arrayNum].needSkill[0] = skill[0];
		g_SPE_SKILL[arrayNum].needSkill[1] = skill[1];
		g_SPE_SKILL[arrayNum].needSkill[2] = skill[2];
		g_SPE_SKILL[arrayNum].needSkill[3] = skill[3];
		g_SPE_SKILL[arrayNum].skillIdx[0] = skillIdx[0];
		g_SPE_SKILL[arrayNum].skillIdx[1] = skillIdx[1];

		arrayNum++;
	}

	fclose( fp );
	return 1;
}

//
// Function Name: __skill_attack__
// Date: 
// Description: 
//
void UseSkillAttack( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target , sPSKILL_DATA pSkill )
{
	if( !pSkill )
		return;

	if( target == dPLAYER ) // Player
	{
	//	DamagePvP( pPlayer, pTargetPlayer, pSkill );
		
		sPDESC_DATA d, next_d;
		int rangeX, rangeY, rangeValue;
		int posX, posY;
		int x, y;
		int i;
		
		if( pSkill->skillAttr == dSKILL_AREA_NONE )
		{

			DamagePvP( pPlayer, pTargetPlayer, pSkill );

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
						LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d, w_next, WMgr );
						
						if( d == pPlayer )
						{
							LIST_SKIP( d, next_d );
						}
						
						rangeX = abs( d->ch.posX - pPlayer->ch.posX );
						rangeY = abs( d->ch.posY - pPlayer->ch.posY );
						
						rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );
						
						if( rangeValue <= pSkill->areaRange )
						{
							if( GET_PVPMODE( pPlayer ) == dPVPMODE_FULL )
							{
								DamagePvP( pPlayer, d, pSkill );
							//	DamagePvP( pPlayer, d, NULL );
							}
							else if( GET_PVPMODE( pPlayer ) == dPVPMODE_GUILD )
							{
								if( GET_GUILD( pPlayer ) && GET_GUILD( d ) != GET_GUILD( pPlayer ) )
								{

									DamagePvP( pPlayer, d, pSkill );				

								}
							}
							else if( GET_PVPMODE( pPlayer ) == dPVPMODE_PARTY )
							{
								if( GET_PARTY_MASTER( pPlayer ) && GET_PARTY_MASTER( d ) != GET_PARTY_MASTER( pPlayer ) )
								{

									DamagePvP( pPlayer, d, pSkill );									

								}
							}
						}
						
						LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d );
					}
				}
			}	
		}
		else if( pSkill->skillAttr == dSKILL_AREA_MOB_ZONE )
		{
			posX = (int)(GET_POSX( pTargetPlayer ) / dONE_TILE);
			posY = (int)(GET_POSY( pTargetPlayer ) / dONE_TILE);
			
			for( y = posY - 1; y <= posY + 1; y++ )
			{
				for( x = posX - 1; x <= posX + 1; x++ )
				{
					if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
					{
						LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d, w_next, WMgr );
						
						if( d == pPlayer )
						{
							LIST_SKIP( d, next_d );
						}
						
						rangeX = abs( d->ch.posX - pTargetPlayer->ch.posX );
						rangeY = abs( d->ch.posY - pTargetPlayer->ch.posY );
						
						rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );
						
						if( rangeValue <= pSkill->areaRange )
						{
							if( GET_PVPMODE( pPlayer ) == dPVPMODE_FULL )
							{

								DamagePvP( pPlayer, d, pSkill );
								

							}
							else if( GET_PVPMODE( pPlayer ) == dPVPMODE_GUILD )
							{
								if( GET_GUILD( pPlayer ) && GET_GUILD( d ) != GET_GUILD( pPlayer ) )
								{

									DamagePvP( pPlayer, d, pSkill );
								}
							}
							else if( GET_PVPMODE( pPlayer ) == dPVPMODE_PARTY )
							{
								if( GET_PARTY_MASTER( pPlayer ) && GET_PARTY_MASTER( d ) != GET_PARTY_MASTER( pPlayer ) )
								{

									DamagePvP( pPlayer, d, pSkill );
								}
							}
						}
						
						LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d );
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
			
			int desPosX = GET_POSX( pPlayer );
			int desPosY = GET_POSY( pPlayer );
			
			for( i = 0; i < 5; i++ )
			{
				if( desPosX < GET_POSX( pTargetPlayer ) )
					desPosX += 1;
				else if( desPosX > GET_POSX( pTargetPlayer ) )
					desPosX -= 1;
				
				if( desPosY < GET_POSY( pTargetPlayer ) )
					desPosY += 1;
				else if( desPosY > GET_POSY( pTargetPlayer ) )
					desPosY -= 1;
				
				checkPos[i][0] = desPosX;
				checkPos[i][1] = desPosY;
			}
			
			posX = (int)(GET_POSX( pPlayer ) / dONE_TILE);
			posY = (int)(GET_POSX( pPlayer ) / dONE_TILE);
			
			for( y = posY - 1; y <= posY + 1; y++ )
			{
				for( x = posX - 1; x <= posX + 1; x++ )
				{
					if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
					{
						LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d, w_next, WMgr );
						
						if( d == pTargetPlayer || d == pPlayer )
						{
							LIST_SKIP( d, next_d );
						}
						
						for( i = 0; i < 5; i++ )
						{
							if( checkPos[i][0] == -1 )
								break;
							else if( checkPos[i][0] == GET_POSX( d ) && checkPos[i][1] == GET_POSY( d ) )
							{
								if( GET_PVPMODE( pPlayer ) == dPVPMODE_FULL )
								{

									DamagePvP( pPlayer, d, pSkill );
								}
								else if( GET_PVPMODE( pPlayer ) == dPVPMODE_GUILD )
								{
									if( GET_GUILD( pPlayer ) && GET_GUILD( d ) != GET_GUILD( pPlayer ) )
									{

										DamagePvP( pPlayer, d, pSkill );
									}
								}
								else if( GET_PVPMODE( pPlayer ) == dPVPMODE_PARTY )
								{
									if( GET_PARTY_MASTER( pPlayer ) && GET_PARTY_MASTER( d ) != GET_PARTY_MASTER( pPlayer ) )
									{

										DamagePvP( pPlayer, d, pSkill );
									}
								}
							}
						}
						
						LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d );
					}
				}
			}

					DamagePvP( pPlayer, pTargetPlayer, pSkill );

		}
	}
	else if( target == dMOB )
	{
		sPMOB_DATA m, next_m;
		int rangeX, rangeY;
		int posX, posY;
		int	rangeValue;
		int x, y;
		int i;
		
		if( pSkill->skillAttr == dSKILL_AREA_NONE )
		{
			DamageToMob( pPlayer, pMob, pSkill );
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
							DamageToMob( pPlayer, m, pSkill );
						
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
							DamageToMob( pPlayer, m, pSkill );
						
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
									DamageToMob( pPlayer, m, pSkill );
								}
							}
						}
						
						LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m );
					}
				}
			}
			DamageToMob( pPlayer, pMob, pSkill );
		}
		else
			return;

	}
//	if( target == dMOB )
//		pMob->lastMoveTime = g_CurrTime;
}

int CheckSpeSkillDelay( sPDESC_DATA pPlayer, int delayIdx, int delay )
{
	if( pPlayer->ch2.speSkillDelay[delayIdx] + delay > g_CurrRealTime )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[149], ( pPlayer->ch2.speSkillDelay[delayIdx] + delay ) - g_CurrRealTime );
		return 0;
	}
	else
	{
		pPlayer->ch2.speSkillDelay[delayIdx] = g_CurrRealTime;
		return 1;
	}

	return 0;
}

BOOL SKILL_NullFunc( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill )
{
	return 0;
}

/*
 *	Energy drain (specialized)
 */
BOOL SKILL_EnergyDrain( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill )
{
	sPSKILL_DATA pStelth = g_SKILL[g_ItemInfo[dSPE_SKILL_STELTH]->skillIdx];
	
	if( pPlayer->ch2.skillEffect[pStelth->effectIndex].delay )
	{
		SendSystemMsg( pPlayer, "It cannot be used in stealth mode." );
		return 0;
	}
	
	if( GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[171] );
		return 0;
	}
	
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( !ISSHORTWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[172] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[172] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_ENERGYDRAIN, 20 ) )
		return 0;
	
	// 장착 아이템 제한 END 
	{
		int drainPoint = 0;
				
		if( target == dPLAYER ) // Player
		{
			if( (int)(GET_HP( pTargetPlayer ) * 0.05 ) > 0 )
			{
				//drainPoint = MIN( (int)( GET_HP( pTargetPlayer ) * 0.3 ), 0 );
				drainPoint = MIN( (int)( GET_HP( pTargetPlayer ) * 0.1 ), 0 );
				
				GET_HP( pTargetPlayer ) = MIN( GET_HP( pTargetPlayer ) - drainPoint, 0 );
				GET_HP( pPlayer ) = MAX( GET_HP( pPlayer ) + drainPoint, GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
				SendPlayerHP( pTargetPlayer );
				SendPlayerHP( pPlayer );
				
				//StrSendSysMsg( pPlayer, 93, GET_NAME( pTargetPlayer ), drainPoint );
				SendSystemMsg( pPlayer, g_LANG_STR[93], GET_NAME( pTargetPlayer ), drainPoint );
			}
			else
				return 0;
		}
		else
		{
			if( (int)( pMob->hp * 0.1 ) > 0 )
			{
				
				
#ifdef dEVENT_MOB_ON
				if( pMob->mobNum >= 101 && pMob->mobNum <= 103 )
				{
					//drainPoint = MIN( (int)( pMob->hp * 0.015 ), 0 );
					drainPoint = MIN( (int)( pMob->hp * 0.1 ), 0 );
				}
				else
#endif
					//drainPoint = MIN( (int)( pMob->hp * 0.15 ), 0 );
				{
					drainPoint = MIN( (int)( pMob->hp * 0.15 ), 0 );
					
				}
				pMob->hp = MIN( pMob->hp - drainPoint, 0 );
				GET_HP( pPlayer ) = MAX( GET_HP( pPlayer ) + drainPoint, GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
				
				SendPlayerHP( pPlayer );

				//StrSendSysMsg( pPlayer, 93, GET_MOB_HNAME( pMob ), drainPoint );
				SendSystemMsg( pPlayer, g_LANG_STR[93], GET_MOB_HNAME( pMob ), drainPoint );
				
				if( !pMob->enemy )
				{
					SET_MOB_ENEMY( pMob, pPlayer );
					SendMobState( pMob );
				}

				// 근거리 60, 흑마법 40 
				GetMobExp( pPlayer, pMob, (int)(drainPoint*0.6), dSKILL_ARRAY_SATTACK );
				GetMobExp( pPlayer, pMob, (int)(drainPoint*0.4), dSKILL_ARRAY_BMAGIC );
				
				// 공격 대상
				if( pMob->enemy )
				{
					if( pMob->enemy->enemyMob != pMob )
					{
						SET_MOB_ENEMY( pMob, pPlayer );
					}
				}
			}
			else        
				return 0;
		}
	}
	return 1;
}

/*
 *	크리티컬(특화) 
 */
BOOL SKILL_Critical( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill )
{
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[94] );
		return 0;
	}
	
	// 아이템 제한
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[173] );
		return 0;
	}
	if( pPlayer->equip[dEQUIP_BOW] )
	{
		if( !ISLONGWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_BOW] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[174] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[174] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_CRITICAL, 10 ) )
		return 0;
	
	UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSKill );

	return 1;
}

/*
 *	파이어 에로우
 */
BOOL SKILL_FireArrow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[94] );
		return 0;
	}
	
	// 아이템 제한
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM ) 
	{
		SendSystemMsg( pPlayer, g_LANG_STR[173] );
		return 0;
	}
	if( pPlayer->equip[dEQUIP_BOW] )
	{
		if( !ISLONGWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_BOW] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[174] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[174] );
		return 0;
	}	
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_FIREARROW, 7 ) ) // Cooldown
		return 0;
	
	UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
	
	if( target == dMOB && pMob )
	{
		sPMOB_DATA mob, next_mob;
		
		LIST_WHILE( gWORLD[pMob->mapNum]->mob[pMob->posY/dONE_TILE][pMob->posX/dONE_TILE], mob, next_mob, w_next, WMgr );
		
		if( abs( pMob->posX - mob->posX ) <= 2 &&
			abs( pMob->posY - mob->posY ) <= 2 )
		{
			DamageToMob( pPlayer, mob, pSkill );
		}
		
		LIST_WHILEEND( gWORLD[pMob->mapNum]->mob[pMob->posY/dONE_TILE][pMob->posX/dONE_TILE], mob, next_mob );
	}
	
	return 1;
}

/*
 *	헤븐리 에로우
 */
BOOL SKILL_HavenlyArrow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill )
{
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[94] );
		return 0;
	}
	
	// 아이템 제한
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[173] );
		return 0;
	}
	if( pPlayer->equip[dEQUIP_BOW] )
	{
		if( !ISLONGWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_BOW] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[174] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[174] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_HEAVENLYARROW, 7 ) ) // cooldown
		return 0;
	
	UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSKill );
	return 1;
}

/*
 *	포이즌 웨폰 
 */
BOOL SKILL_PoisonWeapon( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
	
	if( !pCurrSkill )
		return 0;
	
	if( !IS_BMAGIC( pCurrSkill ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[175] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[97] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[97] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_POISONWEAPON, 7 ) )
		return 0;
		
	if( target == dMOB )
		pMob->AttrEffectTime[MOB_ATTREFFECT_POISONWEAPON] = g_CurrRealTime;
	
	UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
	return 1;
}

/*
 *	포이즌 에로우	
 */
BOOL SKILL_PoisonArraow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[94] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_BOW] )
	{
		if( !ISLONGWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_BOW] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[174] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[174] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_POISONARROW, 10 ) )
		return 0;
	
	UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
	return 1;
}

/*
 *	헤븐리 해머
 */
BOOL SKILL_HavenlyHammer( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{	
	if( target == dMOB )
		return 0;
	
	sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
	
	if( !pCurrSkill )
		return 0;
	
	if( !IS_WMAGIC( pCurrSkill ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[176] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[97] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[97] );
		return 0;
	}
	
	GET_HP( pTargetPlayer ) = MAX( GET_HP( pTargetPlayer ) + (int)( GET_HP( pTargetPlayer ) * 0.2 ), GET_MAX_HP( pTargetPlayer ) + GET_HPADD( pTargetPlayer ) );
	SendPlayerHP( pTargetPlayer );
	
	return 1;
}

/*
 *	헤븐리 스테프	
 */
BOOL SKILL_HavenlyStaff( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{	
	if( target == dMOB )
		return 0;
	
	sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
	
	if( !pCurrSkill )
		return 0;
	
	if( !IS_WMAGIC( pCurrSkill ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[176] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[97] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[97] );
		return 0;
	}
				
	return 1;
}

/*
 *	워크라이
 */
BOOL SKILL_WarCry( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )
		return 0;
	
	if( GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[171] );
		return 0; 
	}
	// 무기체크
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( !ISTWOHANDWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[177] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[177] );
		return 0;
	}		
	return 1;
}

/*
 *	버로우	
 */
BOOL SKILL_Burrow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )
		return 0;
	
	// 무기 장착 체크 
	if( GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[173] );
		return 0;
	}
	if( pPlayer->equip[dEQUIP_BOW] )
	{
		if( !ISLONGWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_BOW] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[174] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[174] );
		return 0;
	}
	// 무기 장착 체크 END 
	
	if( IsHaveSkillEffect( pPlayer, dSPE_SKILL_STELTH ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[102] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_BURROW, 30 ) )
		return 0;

	if( IsHaveSkillEffect( pPlayer, dSPE_SKILL_BURROW ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[100] );
		return 0;
	}
		
	SetVisible( pTargetPlayer, 1 );
	SetMovable( pTargetPlayer, 1 );	
	return 1;
}

/*
 *	블레싱 실드
 */
BOOL SKILL_BlessingShield( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )
		return 0;
	
	// 아이템 제한
	if( GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[171] );
		return 0;
	}
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( !ISSHORTWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[172] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[172] );
		return 0;
	}		
	return 1;
}

/*
 *	스텔스
 */
BOOL SKILL_Stelth( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )
		return 0;
	
	if( GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[171] );
		return 0;
	}
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( !ISONEHANEWEAPON( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[178] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[178] );
		return 0;
	}

	if( IsHaveSkillEffect( pPlayer, dSPE_SKILL_BURROW ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[102] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_STELTH, 30 ) )
		return 0;
			
	SetVisible( pTargetPlayer, 1 );	
	return 1;
}

/*
 *	더스트 스킨
 */
BOOL SKILL_DustSkin( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dPLAYER )
		return 0;
	
	sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
	
	if( !pCurrSkill )
		return 0;
	
	if( !IS_BMAGIC( pCurrSkill ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[175] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[97] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[97] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_DUSTSKIN, 10 ) )
		return 0;
	
	pMob->AttrEffectTime[MOB_ATTREFFECT_DUSTSKIN] = g_CurrRealTime;
	return 1;
}

/*
 *	리 제너레이션
 */
BOOL SKILL_Regeneration( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )
		return 0;
	
	sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
	
	if( !pCurrSkill )
		return 0;
	
	if( !IS_BMAGIC( pCurrSkill ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[175] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[97] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[97] );
		return 0;
	}
	
	if( !CheckSpeSkillDelay( pPlayer, dSPESKILL_DELAYIDX_REGENERATION, 10 ) )
		return 0;		
	return 1;
}

/*
 *	센스티브
 */
BOOL SKILL_Senstive( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	int posX,posY,rangeX,rangeY,rangeValue;
	sPDESC_DATA d,next_d;
	int x,y;
	if( target == dMOB||pPlayer!=pTargetPlayer )
		return 0;

	sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
	
	if( !pCurrSkill )
		return 0;
	
	if( !IS_WMAGIC( pCurrSkill ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[176] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[97] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[97] );
		return 0;
	}

	posX = (int)(GET_POSX( pPlayer ) / dONE_TILE);
	posY = (int)(GET_POSY( pPlayer ) / dONE_TILE);
				
	for( y = posY - 1; y <= posY + 1; y++ ){
		for( x = posX - 1; x <= posX + 1; x++ ){
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE ){
				LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d, w_next, WMgr );
						
					if( d == pPlayer ){
						LIST_SKIP( d, next_d );
					}
					
					rangeX = abs( d->ch.posX - pPlayer->ch.posX );
					rangeY = abs( d->ch.posY - pPlayer->ch.posY );
							
					rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );
							
					if( rangeValue <= pSkill->areaRange ){
						if( GET_PVPMODE( pPlayer ) == dPVPMODE_FULL ){
							UnSetVisibleSkill(d);
						}
						else if( GET_PVPMODE( pPlayer ) == dPVPMODE_GUILD ){
							if( GET_GUILD( pPlayer ) && GET_GUILD( d ) != GET_GUILD( pPlayer ) ){
								UnSetVisibleSkill(d);
							}
						}
						else if( GET_PVPMODE( pPlayer ) == dPVPMODE_PARTY ){
							if( GET_PARTY_MASTER( pPlayer ) && GET_PARTY_MASTER( d ) != GET_PARTY_MASTER( pPlayer ) ){
								UnSetVisibleSkill(d);
							}
						}
					}
						
				LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d );
			}
		}
	}	

	return 1;
}

/*
 *	리쥬버 네이션
 */
BOOL SKILL_Rejyubernation( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )
		return 0;
	
	sPSKILL_DATA pCurrSkill = GET_SKILL_DATA( pPlayer );
	
	if( !pCurrSkill )
		return 0;
	
	if( !IS_WMAGIC( pCurrSkill ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[176] );
		return 0;
	}
	
	// 아이템 제한
	if( pPlayer->equip[dEQUIP_WEAPON] )
	{
		if( GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[97] );
			return 0;
		}
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[97] );
		return 0;
	}		
	return 1;
}

/*
 *	일반 공격 스킬
 */
BOOL SKILL_NormalSkillAttack( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	//if( target == dPLAYER )
	//	return 1;


	if( !pMob )
	{
		if( GET_CHA( pPlayer ) <= -30000 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[285] );
			return 1;
		}
	}
	
	switch( pSkill->itemNum )
	{
	case dLATTACK_SKILL_DOUBLESHOT:		// 더블샷
		if( target == dPLAYER )
		{
			sPSKILL_DATA Skill = pSkill;
			int		n = number( 2, 3 );
			Skill->maxDamage = pSkill->maxDamage / n;

			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
		}

		else
		{
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
		}
		break;

	case dLATTACK_SKILL_TRIPLESHOT:		// 트리플 샷
		if( target == dPLAYER )
		{
			sPSKILL_DATA Skill = pSkill;
			int		n = number( 3, 4 );
			Skill->maxDamage = pSkill->maxDamage / n;

			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
		}
		else
		{
		UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
		UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
		UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
	  }
		break;
	case dLATTACK_SKILL_DOUBLEDOUBLESHOT: // 더블 더블 샷
		if( target == dPLAYER )
		{
			sPSKILL_DATA Skill = pSkill;
			int		n = number( 4, 5 );
			Skill->maxDamage = pSkill->maxDamage / n;

			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
			UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , Skill );
		}
		else
		{
		 UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
		 UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
		 UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
		 UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );
	  }
		break;
	default:
		UseSkillAttack( pPlayer, pTargetPlayer, pMob, target , pSkill );		
		break;
	}		

	return 1;
}

/*
 *	일반 힐링
 */
BOOL SKILL_NormalHealing( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )
		return 0;

	if( (!GET_PARTY_MASTER( pPlayer ) ) || ( pTargetPlayer != pPlayer ) )
	{
			if( ( GET_DEAD_STATE( pTargetPlayer ) == dDEAD_STATE_NONE) )
			{
				GET_HP( pTargetPlayer ) = MAX( GET_HP( pTargetPlayer ) + GET_HPADD( pTargetPlayer ) + number( pSkill->minDamage, pSkill->maxDamage ), GET_MAX_HP( pTargetPlayer ) + GET_HPADD( pTargetPlayer ) ); 
				SendPlayerHP( pTargetPlayer );
				return 1;
			}
	}
	else{

		if( (pSkill->itemNum == dMAGIC_NUM_CUREUNZ)||(pSkill->itemNum ==  dMAGIC_NUM_HEAL) )
		for( int i = 0; i < 8; i++ )
		{
			sPDESC_DATA pMember = NULL;
	
			// 파티 마스터 
			if( i == 7 )
			{
				pMember = GET_PARTY_MASTER( pPlayer );
			}
			else
			{
				pMember = GET_PARTY_MASTER( pPlayer )->ch2.party.member[i];
			}
	
			if( !pMember ||(GET_DEAD_STATE( pTargetPlayer )!= dDEAD_STATE_NONE))
				continue;
	
			if( GET_MAP_NUM( pPlayer ) == GET_MAP_NUM( pMember ) &&
				abs( GET_POSX( pPlayer ) - GET_POSX( pMember ) ) <= 50 &&
				abs( GET_POSY( pPlayer ) - GET_POSY( pMember ) ) <= 50 )
			{
				GET_HP( pMember ) = MAX( GET_HP( pMember ) + GET_HPADD( pMember ) + number( pSkill->minDamage, pSkill->maxDamage ), GET_MAX_HP( pMember ) + GET_HPADD( pMember ) ); 
				SendPlayerHP( pMember );
			}
		}
	}
	
		return 1;
}

/*
 *	일반 버프
 */
BOOL SKILL_NormalBuff( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
	if( target == dMOB )	
	{
		SendSystemMsg( pPlayer, g_LANG_STR[183] );
		return 0;
	}			
	return 1;
}



/*
 *	범용 공격 마법들
 */
BOOL SKILL_NormalMagicAttack( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{	
	// 칠터치
	if( !pMob )
	{
		if( GET_CHA( pPlayer ) <= -30000 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[285] );
			return 1;
		}
	}

	if( pSkill->itemNum == dMAGIC_NUM_CHILLTOUCH )
	{
		if( target == dMOB )
			pMob->AttrEffectTime[MOB_ATTREFFECT_CHILTOUCH] = g_CurrRealTime;
	}
	
	if( target == dPLAYER ) // Player
		MagicAttackPvP( pPlayer, pTargetPlayer, pSkill );
	else
		MagicAttackToMob( pPlayer, pMob, pSkill );
	
//	if( target == dMOB )
//		pMob->lastMoveTime = g_CurrTime;
	
	return 1;
}


#include "skill140_death_timing.h"

// Level-140 bursts use the ordinary authoritative hit/area/PvP rules per hit.
// Never mutate the shared skill definition (the older multi-shot PvP code did).
BOOL SKILL_Level140Attack( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
    int hits = 0;
    switch( pSkill->itemNum )
    {
    case 19120: hits = 3; break;
    case 19121: hits = 6; break;
    case 19122: hits = 7; break;
    case 19123: hits = 3; break;
    default: return 0;
    }
    DWORD priorDeathHold = g_Skill140DeathHold;
    g_Skill140DeathHold = pSkill->itemNum == 19122 ? g_Skill140MeteorDeath :
                         pSkill->itemNum == 19123 ? g_Skill140FrostDeath : 0;
    for( int hit = 0; hit < hits; ++hit )
    {
        if( pSkill->itemNum <= 19121 )
            SKILL_NormalSkillAttack( pPlayer, pTargetPlayer, pMob, target, pSkill );
        else
            SKILL_NormalMagicAttack( pPlayer, pTargetPlayer, pMob, target, pSkill );
    }
    g_Skill140DeathHold = priorDeathHold;
    return 1;
}

void AsignSkillFunc()
{
    LoadSkill140DeathTiming();
#define SET_SKILL_FUNC( skillItemNum, skill_func ) \
	if( g_SKILL[g_ItemInfo[skillItemNum]->skillIdx] )	\
	{	\
		g_SKILL[g_ItemInfo[skillItemNum]->skillIdx]->func = skill_func;	\
	}	\

	SET_SKILL_FUNC( 19120, SKILL_Level140Attack );
	SET_SKILL_FUNC( 19121, SKILL_Level140Attack );
	SET_SKILL_FUNC( 19122, SKILL_Level140Attack );
	SET_SKILL_FUNC( 19123, SKILL_Level140Attack );

	SET_SKILL_FUNC( dSPE_SKILL_ENERGYDRAIN,			SKILL_EnergyDrain );		// 에너지 드레인  
	SET_SKILL_FUNC( dSPE_SKILL_CRITICAL,			SKILL_Critical );			// 크리티컬
	SET_SKILL_FUNC( dSPE_SKILL_FIREARROW,			SKILL_FireArrow );			// 파이어에로우
	SET_SKILL_FUNC( dSPE_SKILL_HEAVENLYARROW,		SKILL_HavenlyArrow );		// 헤븐리 에로우
	SET_SKILL_FUNC( dSPE_SKILL_POISONWEAPON,		SKILL_PoisonWeapon );		// 포이즌 웨폰
	SET_SKILL_FUNC( dSPE_SKILL_POISONARROW,			SKILL_PoisonArraow );		// 포이즌 에로우
	SET_SKILL_FUNC( dSPE_SKILL_HEAVENLYHAMMER,		SKILL_HavenlyHammer );		// 헤븐리해머
	SET_SKILL_FUNC( dSPE_SKILL_HEAVENLYSTAFF,		SKILL_HavenlyStaff );		// 헤븐리 스테프
	SET_SKILL_FUNC( dSPE_SKILL_WARCRY,				SKILL_WarCry );				// 워크라이
	SET_SKILL_FUNC( dSPE_SKILL_BURROW,				SKILL_Burrow );				// 버로우 
	SET_SKILL_FUNC( dSPE_SKILL_BLESSINGSHIELD,		SKILL_BlessingShield );		// 플레싱 실드
	SET_SKILL_FUNC( dSPE_SKILL_STELTH,				SKILL_Stelth );				// 스텔스
	SET_SKILL_FUNC( dSPE_SKILL_DUSTSKIN,			SKILL_DustSkin );			// 더스트 스킨
	SET_SKILL_FUNC( dSPE_SKILL_REGENERATION,		SKILL_Regeneration );		// 리제너레이션 
	SET_SKILL_FUNC( dSPE_SKILL_SENSTIVE,			SKILL_Senstive );			// 센스티브
	SET_SKILL_FUNC( dSPE_SKILL_REJYUVERNATION,		SKILL_Rejyubernation );		// 리쥬버네이션

	// 전사, 궁수 스킬
	SET_SKILL_FUNC( dSATTACK_SKILL_WOLFCLOW,		SKILL_NormalSkillAttack );	// 울프클로
	SET_SKILL_FUNC( dSATTACK_SKILL_STAFF,			SKILL_NormalSkillAttack );	// 스탭 
	SET_SKILL_FUNC( dSATTACK_SKILL_BLADEWIND,		SKILL_NormalSkillAttack );	// 블레이드윈드
	SET_SKILL_FUNC( dLATTACK_SKILL_SPIRITARROW,		SKILL_NormalSkillAttack );	// 스피릿 에로우
	SET_SKILL_FUNC( dLATTACK_SKILL_FIREHON,			SKILL_NormalSkillAttack );	// 파이어혼 
	SET_SKILL_FUNC( dLATTACK_SKILL_DOUBLESHOT,		SKILL_NormalSkillAttack );	// 더블샷
	// 2차
	SET_SKILL_FUNC( dSATTACK_SKILL_BEARCLOW,		SKILL_NormalSkillAttack );	//베어클로
	SET_SKILL_FUNC( dSATTACK_SKILL_ORGCLOW,			SKILL_NormalSkillAttack );	//오거클로
	SET_SKILL_FUNC( dSATTACK_SKILL_WILDSTAFF,		SKILL_NormalSkillAttack );	//와일드스텝
	SET_SKILL_FUNC( dSATTACK_SKILL_BUSTSTAFF,		SKILL_NormalSkillAttack );	//버스트스텝
	SET_SKILL_FUNC( dSATTACK_SKILL_BLADEVRIZE,		SKILL_NormalSkillAttack );	//블레이드브리즈
	SET_SKILL_FUNC( dSATTACK_SKILL_BLADEGAIL,		SKILL_NormalSkillAttack );	//블레이드게일
	SET_SKILL_FUNC( dSATTACK_SKILL_BORNBREAK,		SKILL_NormalSkillAttack );	//본브레이크
	SET_SKILL_FUNC( dSATTACK_SKILL_DOUBLEBREAK,		SKILL_NormalSkillAttack );	//더블브레이크
	SET_SKILL_FUNC( dSATTACK_SKILL_BREAKREVOLUTION,	SKILL_NormalSkillAttack );	//브레이크레볼루션
	SET_SKILL_FUNC( dLATTACK_SKILL_DARKSPRITARROW,	SKILL_NormalSkillAttack );	//다크스피릿애로우
	SET_SKILL_FUNC( dLATTACK_SKILL_BLUDSPRITARROW,	SKILL_NormalSkillAttack );	//블러드스피릿애로우
	SET_SKILL_FUNC( dLATTACK_SKILL_ICEHON,			SKILL_NormalSkillAttack );	//아이스혼
	SET_SKILL_FUNC( dLATTACK_SKILL_LIGHTININGHON,	SKILL_NormalSkillAttack );	//라이트닝혼
	SET_SKILL_FUNC( dLATTACK_SKILL_TRIPLESHOT,		SKILL_NormalSkillAttack );	//트리플샷
	SET_SKILL_FUNC( dLATTACK_SKILL_DOUBLEDOUBLESHOT,SKILL_NormalSkillAttack );	//더블더블샷
	SET_SKILL_FUNC( dLATTACK_SKILL_PANUTRATION,		SKILL_NormalSkillAttack );	//패너트레이션
	SET_SKILL_FUNC( dLATTACK_SKILL_POWERPANUTRATION,SKILL_NormalSkillAttack );	//파워패너트레이션
	SET_SKILL_FUNC( dLATTACK_SKILL_DEATHPANUTRATION,SKILL_NormalSkillAttack );	//데쓰패너트레이션

	// 일반 마법
	SET_SKILL_FUNC( dMAGIC_NUM_FLAMEARROW,			SKILL_NormalMagicAttack );	// 플래임 에로우
	SET_SKILL_FUNC( dMAGIC_NUM_FLAMMINGSPHERE,		SKILL_NormalMagicAttack	);	// 플래밍 스피어
	SET_SKILL_FUNC( dMAGIC_NUM_PRODUCEFLAME,		SKILL_NormalMagicAttack );	// 프로듀스플래임 
	SET_SKILL_FUNC( dMAGIC_NUM_FIREBALL,			SKILL_NormalMagicAttack );	// 파이어 볼
	SET_SKILL_FUNC( dMAGIC_NUM_FIREPIERCE,			SKILL_NormalMagicAttack );	// 파이어 피어스
	SET_SKILL_FUNC( dMAGIC_NUM_PRODUCEICE,			SKILL_NormalMagicAttack );	// 프로듀스 아이스
	SET_SKILL_FUNC( dMAGIC_NUM_ICESWORD,			SKILL_NormalMagicAttack );	// 아이스스워드
	SET_SKILL_FUNC( dMAGIC_NUM_FIREBUST,			SKILL_NormalBuff );			// 파이어버스트 
	SET_SKILL_FUNC( dMAGIC_NUM_SPIKEICE,			SKILL_NormalMagicAttack );	// 스파이크 아이스
	SET_SKILL_FUNC( dMAGIC_NUM_CHILLTOUCH,			SKILL_NormalMagicAttack );	// 칠터치
	SET_SKILL_FUNC( dMAGIC_NUM_CLOUDKILL,			SKILL_NormalMagicAttack );	// 클라우드킬 
	SET_SKILL_FUNC( dMAGIC_NUM_FIREWALL,			SKILL_NormalMagicAttack );	// 파이어월 
	SET_SKILL_FUNC( dMAGIC_NUM_LIGHTNINGARROW,		SKILL_NormalMagicAttack );	// 라이트닝 에로우 
	SET_SKILL_FUNC( dMAGIC_NUM_LIGHTNING,			SKILL_NormalMagicAttack );	// 라이트닝 
	SET_SKILL_FUNC( dMAGIC_NUM_CALLLIGHTNING,		SKILL_NormalMagicAttack );	// 콜라이트닝 
	SET_SKILL_FUNC( dMAGIC_NUM_MINUHEALLING,		SKILL_NormalHealing );		// 마이너힐링
	SET_SKILL_FUNC( dMAGIC_NUM_LIGHTNINGPIERCE,		SKILL_NormalMagicAttack );	// 라이트닝 피어스
	SET_SKILL_FUNC( dMAGIC_NUM_WINDSWORD,			SKILL_NormalMagicAttack );	// 윈드소드
	SET_SKILL_FUNC( dMAGIC_NUM_WINDCANON,			SKILL_NormalMagicAttack );	// 윈드 캐논
	SET_SKILL_FUNC( dMAGIC_NUM_CLOUDSHIELD,			SKILL_NormalBuff );			// 클라우드 실드
	SET_SKILL_FUNC( dMAGIC_NUM_WINDSTRIKE,			SKILL_NormalMagicAttack );	// 윈드 스트라이크
	SET_SKILL_FUNC( dMAGIC_NUM_CURELIGHT,			SKILL_NormalHealing );		// 큐어 라이트 
	SET_SKILL_FUNC( dMAGIC_NUM_EARTHSWORD,			SKILL_NormalMagicAttack );	// 어스스워드 
	SET_SKILL_FUNC( dMAGIC_NUM_GLASSIP,				SKILL_NormalMagicAttack );	// 글래십 
	// 2차
	SET_SKILL_FUNC( dMAGIC_NUM_LIGHTNINGARROWBLOT,		SKILL_NormalMagicAttack );	//라이트닝애로우볼트
	SET_SKILL_FUNC( dMAGIC_NUM_LIGHTNINGARROWWAVE,		SKILL_NormalMagicAttack );	//라이트닝애로우웨이브
	SET_SKILL_FUNC( dMAGIC_NUM_LIGHTNINGBOLT,			SKILL_NormalMagicAttack );	//라이트닝볼트
	SET_SKILL_FUNC( dMAGIC_NUM_LIGHTNINGWAVE,			SKILL_NormalMagicAttack );	//라이트닝웨이브
	SET_SKILL_FUNC( dMAGIC_NUM_CALLLIGHTNINGBOLT,		SKILL_NormalMagicAttack );	//콜라이트닝볼트
	SET_SKILL_FUNC( dMAGIC_NUM_CALLLIGHTNINGWAVE,		SKILL_NormalMagicAttack );	//콜라이트닝웨이브
	SET_SKILL_FUNC( dMAGIC_NUM_CHAINLIGNTNINGPIERCE,	SKILL_NormalMagicAttack );	//체인라이트닝피어스
	SET_SKILL_FUNC( dMAGIC_NUM_SHORKLIGHTNINGPIERCE	,	SKILL_NormalMagicAttack );	//쇼크라이트닝피어스
	SET_SKILL_FUNC( dMAGIC_NUM_ELETRIC,					SKILL_NormalMagicAttack );	//일렉트릭
	SET_SKILL_FUNC( dMAGIC_NUM_THUNDERBOLT,				SKILL_NormalMagicAttack );	//썬더볼트
	SET_SKILL_FUNC( dMAGIC_NUM_WINDSHIELD,				SKILL_NormalMagicAttack );	//윈드실드
	SET_SKILL_FUNC( dMAGIC_NUM_WINDARROW,				SKILL_NormalMagicAttack );	//윈드애로우
	SET_SKILL_FUNC( dMAGIC_NUM_WINDBALL,				SKILL_NormalMagicAttack );	//윈드볼
	SET_SKILL_FUNC( dMAGIC_NUM_WINDSPHERE,				SKILL_NormalMagicAttack );	//윈드스피어
	SET_SKILL_FUNC( dMAGIC_NUM_BRIGHTSHIELD,			SKILL_NormalBuff );			//브라이트실드
	SET_SKILL_FUNC( dMAGIC_NUM_PROTECTIONSHIELD,		SKILL_NormalBuff );			//프로텍션실드
	SET_SKILL_FUNC( dMAGIC_NUM_WINDSTORM,				SKILL_NormalMagicAttack );	//윈드스톰
	SET_SKILL_FUNC( dMAGIC_NUM_TONEIDO,					SKILL_NormalMagicAttack );	//토네이도
	SET_SKILL_FUNC( dMAGIC_NUM_RECOVERYHEALING,			SKILL_NormalHealing );		//리커버리힐링
	SET_SKILL_FUNC( dMAGIC_NUM_GREATEHEALING,			SKILL_NormalHealing );		//그레이트힐링
	SET_SKILL_FUNC( dMAGIC_NUM_CUREUNZ,					SKILL_NormalHealing );		//큐어운즈
	SET_SKILL_FUNC( dMAGIC_NUM_HEAL,					SKILL_NormalHealing );		//힐
	SET_SKILL_FUNC( dMAGIC_NUM_GROUNDSTON,				SKILL_NormalMagicAttack );	//그라운드스톤
	SET_SKILL_FUNC( dMAGIC_NUM_GROUNDCRAY,				SKILL_NormalMagicAttack );	//그라운드크레이
	SET_SKILL_FUNC( dMAGIC_NUM_DIALFLAMEARROW,			SKILL_NormalMagicAttack );	//듀얼플레임애로우
	SET_SKILL_FUNC( dMAGIC_NUM_TRIPLEFLAMEARROW,		SKILL_NormalMagicAttack );	//트리플플래임애로우
	SET_SKILL_FUNC( dMAGIC_NUM_FLAMINGSCORUM,			SKILL_NormalMagicAttack );	//플래밍스코룸
	SET_SKILL_FUNC( dMAGIC_NUM_STRIKECORUM,				SKILL_NormalMagicAttack );	//스트라이크코룸
	SET_SKILL_FUNC( dMAGIC_NUM_DARKPRODUCEFLAME,		SKILL_NormalMagicAttack );	//다크플래듀스플래임
	SET_SKILL_FUNC( dMAGIC_NUM_DEATHPRODUCEFLAME,		SKILL_NormalMagicAttack );	//데쓰플래듀스플래임
	SET_SKILL_FUNC( dMAGIC_NUM_DARKFIREBALL,			SKILL_NormalMagicAttack );	//다크파이어볼
	SET_SKILL_FUNC( dMAGIC_NUM_DEATHFIREBALL,			SKILL_NormalMagicAttack );	//데쓰파이어볼
	SET_SKILL_FUNC( dMAGIC_NUM_DARKFIREPIERCE,			SKILL_NormalMagicAttack );	//다크파이어피어스
	SET_SKILL_FUNC( dMAGIC_NUM_DEATHFIREPIERCE,			SKILL_NormalMagicAttack );	//데쓰파이어피어스
	SET_SKILL_FUNC( dMAGIC_NUM_DARKFIRECHANT,			SKILL_NormalBuff );			//다크파이어챈트
	SET_SKILL_FUNC( dMAGIC_NUM_DEATHFIREORR,			SKILL_NormalBuff );			//데쓰파이어오러
	SET_SKILL_FUNC( dMAGIC_NUM_DARKFIRECORUM,			SKILL_NormalMagicAttack );	//다크파이어코룸
	SET_SKILL_FUNC( dMAGIC_NUM_DEATHFIRECORUMHELL,		SKILL_NormalMagicAttack );	//데쓰파이어코룸헬
	SET_SKILL_FUNC( dMAGIC_NUM_PRODUCEICEARROW,			SKILL_NormalMagicAttack );	//프로듀스아이스애로우
	SET_SKILL_FUNC( dMAGIC_NUM_PRODUCEICEBUG,			SKILL_NormalMagicAttack );	//프로듀스아이스버그
	SET_SKILL_FUNC( dMAGIC_NUM_ICESWORDCORUM,			SKILL_NormalMagicAttack );	//아이스스워드코룸
	SET_SKILL_FUNC( dMAGIC_NUM_ICESWORDHELL,			SKILL_NormalMagicAttack );	//아이스스워드헬
	SET_SKILL_FUNC( dMAGIC_NUM_SPIKEICEWAVE,			SKILL_NormalMagicAttack );	//스파이크아이스웨이브
	SET_SKILL_FUNC( dMAGIC_NUM_SPIKEICEMETEO,			SKILL_NormalMagicAttack );	//스파이크아이스메테오
	SET_SKILL_FUNC( dMAGIC_NUM_HELLSCREAM,				SKILL_NormalMagicAttack );	//헬스크림
	SET_SKILL_FUNC( dMAGIC_NUM_CULLREPS,				SKILL_NormalMagicAttack );	//컬렙스
	SET_SKILL_FUNC( dMAGIC_NUM_DEADFORG,				SKILL_NormalMagicAttack );	//데드포그
	SET_SKILL_FUNC( dMAGIC_NUM_DEATHFORGKILL,			SKILL_NormalMagicAttack );	//데쓰포그킬
	
#undef SET_SKILL_FUNC
}

BOOL IsHaveSkillEffectDelay( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPSKILL_DATA pSkill, BYTE target )
{
	if( !pTargetPlayer || !pPlayer || !pSkill )
		return 0;
	
	if( target == dPLAYER  && pSkill->timer )
	{
	/*	if( pSkill->itemNum == dMAGIC_NUM_FIREBUST ||
			pSkill->itemNum == dMAGIC_NUM_DARKFIRECHANT ||
			pSkill->itemNum == dMAGIC_NUM_DEATHFIREORR ||
			pSkill->itemNum == dMAGIC_NUM_CLOUDSHIELD ||
			pSkill->itemNum == dMAGIC_NUM_BRIGHTSHIELD ||
			pSkill->itemNum == dMAGIC_NUM_PROTECTIONSHIELD )
		{
			return 0;
		}*/

		if( pTargetPlayer->ch2.skillEffect[pSkill->effectIndex].delay )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[267], GET_ITEM_HNAME2( pSkill->itemNum ) );
			return 1;
		}
	}
	return 0;
}

void AddSkillEffect( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPSKILL_DATA pSkill, BYTE target )
{
	if( !pTargetPlayer || !pPlayer || !pSkill )
		return;

	if( target == dPLAYER && pSkill->timer )
	{
		pTargetPlayer->ch2.skillEffect[pSkill->effectIndex].skillNum = pSkill->itemNum;
		pTargetPlayer->ch2.skillEffect[pSkill->effectIndex].delay = g_CurrTime + pSkill->timer;
		pTargetPlayer->ch2.skillEffect[pSkill->effectIndex].effectType = pSkill->effectType;
		pTargetPlayer->ch2.skillEffect[pSkill->effectIndex].value = pSkill->effectValue;
		pTargetPlayer->ch2.skillEffect[pSkill->effectIndex].valueType = pSkill->effectValueType;

		SetBuffSkill( pTargetPlayer, pSkill->itemNum );
		SendEffectON( pTargetPlayer, pSkill->itemNum, pSkill->timer );
	}
}

//
// Function Name: SKILL_UseSkill
// Date: 
// Description: 
//
void SKILL_UseSkill( sPDESC_DATA pPlayer, int idxNum, BYTE target, WORD skillNum )
{
	if( !IS_VALID_ITEM( skillNum ) )
		return;

	if( GET_ITEM_TYPE2( skillNum ) != dITEMTYPE_MAGICSCROLL )
		return;

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return;
	
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[86] );
		return;
	}
	
	if( GET_RIDE_STATE( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[87] );
		return;
	}

	// 소유여부 체크
	if( !IsHaveSkill( pPlayer, skillNum ) )
		return;

	if( pPlayer->ch2.actionDelay > g_CurrTime )
	{
		if( !pPlayer->ch2.waitSkill.skillNum )
		{
			pPlayer->ch2.waitSkill.skillNum = skillNum;
			pPlayer->ch2.waitSkill.targetIdx = idxNum;
			pPlayer->ch2.waitSkill.targetType = target;
		}
		return;
	}

	sPSKILL_DATA pSkill = g_SKILL[g_ItemInfo[skillNum]->skillIdx];
	
	if( !pSkill )
		return;

	// 레벨 체크 
	if( GET_SATTACK( pPlayer ) < pSkill->needSAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[268], pSkill->needSAttack );
		return;
	}
	if( GET_LATTACK( pPlayer ) < pSkill->needLAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[269], pSkill->needLAttack );
		return;
	}
	if( GET_BMAGIC( pPlayer ) < pSkill->needBMagic )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[270], pSkill->needBMagic );
		return;
	}
	if( GET_WMAGIC( pPlayer ) < pSkill->needWMagic )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[271], pSkill->needWMagic );
		return;
	}
	// 레벨체크 END

	if( pSkill->coolTimeIdx )
	{
		if( pPlayer->ch2.skillCoolTime[pSkill->coolTimeIdx] > g_CurrTime )
			return;
	}
	
	// 흑마법이나 백마법 이면
	if( IS_BMAGIC( pSkill ) || IS_WMAGIC( pSkill ) )
	{
			pPlayer->ch2.lastCastMagic = 0;	
	}
	else if( IS_SATTACK( pSkill ) && GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM )	// 근거리 스킬
	{
		SendSystemMsg( pPlayer, g_LANG_STR[147] );
		return;
	}
	else if( IS_LATTACK( pSkill ) && GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )	// 원거리 스킬
	{
		SendSystemMsg( pPlayer, g_LANG_STR[148] );
		return;
	}

	WORD needMP;
	WORD needHP;
	char szPacket[dMAX_SOCK_BUFF];
	int packetPos;
	
	if( IS_ATTACK_SKILL( pSkill ) && !IsCanAttackWeight( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[179] );
		return;
	}

	needMP = pSkill->needMP;
	needHP = pSkill->needHP;

	if( GET_MP( pPlayer ) < needMP )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[1] );
		return;
	}       
	
	sPDESC_DATA pTargetPlayer = NULL;
	sPMOB_DATA pMob = NULL;
	
	int targetIdx;
	
	// 플레이어 
	if( target == dPLAYER ) 
	{
		if( idxNum == GET_IDX( pPlayer ) )
			pTargetPlayer = pPlayer;
		else
		{
			pTargetPlayer = FindPlayerInArea( pPlayer->ch.mapNum, pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
		}
		
		if( !pTargetPlayer )
			return;

		targetIdx = GET_IDX( pTargetPlayer );

		if( IS_ATTACK_SKILL( pSkill ) )
		{
			if( !IsPvPMode( pPlayer ) )
				return;

			if( CheckMyTeam( pPlayer, pTargetPlayer ) )
				return;

#ifndef dNON_PVP_MODE
			if( GET_ALL_SKILL( pPlayer ) <= 25 )
			{
				if( !gWORLD[GET_MAP_NUM( pPlayer )]->underAttack )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[134] );
					return;
				}
			}
			
			if(	GET_ALL_SKILL( pTargetPlayer ) <= 25 )
			{
				if( !gWORLD[GET_MAP_NUM( pTargetPlayer )]->underAttack )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[135] );
					return;
				}
			}

			if( !pPlayer->victPlayer && pTargetPlayer != pPlayer && pTargetPlayer->victPlayer != pPlayer )			
			{
				pPlayer->ch2.isEvilCha = 1;
				pPlayer->ch2.evilChaTime = g_CurrRealTime;
				
				sNAME_COLOR nameColor;

				GetNameColor( dNAMECOLOR_EVILCHA, &nameColor );
				SendChangeNameColor( pPlayer, &nameColor );
			}
#endif
		}
	}
	else if( target == dMOB ) // 몹
	{
		pMob = FindMobInArea( pPlayer->ch.mapNum, pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
		
		if( !pMob )
			return;

		if( pMob->isDead )
			return;
		
#ifdef USE_GUILD_WAR
		// 공성관련 오브젝트 공격권한 처리 
		if( pMob->mobNum == dMONSTER_CASTLEDOOR ||
			pMob->mobNum == dMONSTER_WARCRISTAL )
		{
			sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];
			
			if( !pWorld )
				return;
			
			if( pWorld->underAttack )
			{
				if( GET_ALL_SKILL( pPlayer ) <= 25 )
					return;

				if( GET_GUILD( pPlayer ) != pWorld->warCristal.allowGuild[1] )
					return;
			}
		}
#endif
		
		targetIdx = pMob->idxNum;
	}
	else 
		return;

	// 타겟 속성 검사
	if( pSkill->targetType == dMAGIC_VICT_BYSELF )
	{
		if( target == dMOB || ( target == dPLAYER && pPlayer != pTargetPlayer ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[88] );
			return;
		}
	}
	else if( pSkill->targetType == dMAGIC_VICT_TARGET )
	{
		if( target == dPLAYER && pTargetPlayer == pPlayer )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[89] );
			return;
		}
	}
	else if( pSkill->targetType == dMAGIC_VICT_BOTH )
	{
		if( target == dPLAYER && pTargetPlayer == pPlayer && pSkill->skillType == dSKILL_TYPE_SPEATTACK )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[89] );
			return;
		}
	}

	// 거리 계산 
	int rangeX, rangeY;

	if( target == dPLAYER )
	{
		rangeX = abs( GET_POSX( pTargetPlayer ) - GET_POSX( pPlayer ) );
		rangeY = abs( GET_POSY( pTargetPlayer ) - GET_POSY( pPlayer ) );

		if( rangeX > pSkill->range || rangeY > pSkill->range )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[91] );
			return;
		}
	}
	else if( target == dMOB )
	{
		rangeX = abs( pMob->posX - GET_POSX( pPlayer ) );
		rangeY = abs( pMob->posY - GET_POSY( pPlayer ) );
		


			if( rangeX > pSkill->range || rangeY > pSkill->range )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[91] );
				return;
			}
		
	}
	else
		return;

	// Packet 헤더 
	packetPos = 2;
	
	if( IS_BMAGIC( pSkill ) || IS_WMAGIC( pSkill ) )
		PutWord( szPacket, dPACKET_MAGIC_CAST_USER, packetPos );
	else
		PutWord( szPacket, dPACKET_BEGIN_SKILL, packetPos );
	PutWord( szPacket, skillNum, packetPos );
	PutInteger( szPacket, GET_IDX( pPlayer ), packetPos );
	PutInteger( szPacket, targetIdx, packetPos );
	
	// 공격 마법일 경우 
	if( IS_ATTACK_SKILL( pSkill ) )
	{
		// 현재 PK 모드 지원안됨
		//		if( !target || !pMob )
		//			return;
#ifndef dNON_PVP_MODE
		if( target == dMOB && !pMob )
			return;
		else if( target == dPLAYER && !pTargetPlayer )
			return;
		else if( target == dPLAYER && ( pTargetPlayer == pPlayer ) )
			return;
#else
		if( target == dMOB && !pMob )
			return;

		if( target == dPLAYER )
			return;
#endif
		
		// 대상이 몹이면 
		if( target == dMOB )
		{
			if( !IsValidAttackArea( pPlayer, pMob ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[0] );
				return;
			}
		}
		else if( target == dPLAYER )
		{
			if( !IsPvPArea( pPlayer ) )
				return;
			
			if( !IsValidPvPTarget( pPlayer, pTargetPlayer ) )
				return;

			if( !IsValidPvPAttackArea( pPlayer, pTargetPlayer ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[0] );
				return;
			}
		}
		else 
			return;

		if( IsHaveSkillEffectDelay( pPlayer, pTargetPlayer, pSkill, target ) )
			return;
		
		if( !pSkill->func( pPlayer, pTargetPlayer, pMob, target, pSkill ) )
			return;

		AddSkillEffect( pPlayer, pTargetPlayer, pSkill, target );
	}
	else
	{
		if( IsHaveSkillEffectDelay( pPlayer, pTargetPlayer, pSkill, target ) )
			return;

		if( !pSkill->func( pPlayer, pTargetPlayer, pMob, target, pSkill ) )
			return;

		AddSkillEffect( pPlayer, pTargetPlayer, pSkill, target );
	}

	// 쿨타임
	if( pSkill->coolTimeIdx )
	{
		pPlayer->ch2.skillCoolTime[pSkill->coolTimeIdx] = g_CurrTime + ( pSkill->coolTime * 1000 );
		SendItemTimer( pPlayer, pSkill->itemNum, pSkill->coolTime * 1000 );
	}

	// MP 소모
	GET_MP( pPlayer ) = MIN( GET_MP( pPlayer ) - needMP, 0 );	
	GET_HP( pPlayer ) = MIN( GET_HP( pPlayer ) - needHP, 0 );
	SendPlayerMP( pPlayer );
	SendPlayerHP( pPlayer );
	
	PutByte( szPacket, target, packetPos );

	if( IS_BMAGIC( pSkill ) || IS_WMAGIC( pSkill ) )
	{
		if( target == dPLAYER )
		{
			if( GET_EFFECT_IDX( pTargetPlayer ) > 65000 )
				GET_EFFECT_IDX( pTargetPlayer ) = dMAGIC_IDX_SEED;
			else
				GET_EFFECT_IDX( pTargetPlayer )++;

			PutWord( szPacket, GET_EFFECT_IDX( pTargetPlayer ), packetPos );
		}
		else if( target == dMOB )
		{
			if( pMob->effectIdx > 65000 )
				pMob->effectIdx = dMAGIC_IDX_SEED;
			else
				pMob->effectIdx++;

			PutWord( szPacket, pMob->effectIdx, packetPos );
		}
		
		pPlayer->ch2.actionDelay = g_CurrTime + 1500;
	}
	else
	{
		pPlayer->ch2.actionDelay = g_CurrTime + 2500;
	}

	PutSize( szPacket, packetPos );
	
	SendToRange( pPlayer, szPacket, packetPos, dATTACK_PACKET_RANGE );
}

//
// Function Name: NewUserSkillData
// Date: 
// Description: 
//
sPUSER_SKILL NewUserSkillData()
{
	sPUSER_SKILL pNewData;

	if( gPLAYERS.userSkill_memory )
	{
		pNewData = gPLAYERS.userSkill_memory;

		REMOVE_FROM_LIST( gPLAYERS.userSkill_memory, pNewData, prev, next );

		return pNewData;
	}
	else
	{
		pNewData = (sPUSER_SKILL)malloc(sizeof(sUSER_SKILL));

		if( !pNewData )
		{
			log( "NewUserSkillData : malloc failed\r\n" );
			return NULL;
		}
		return pNewData;
	}
}

//
// Function Name: AddUserSkillData
// Date: 
// Description: 
//
void AddUserSkillData( sPDESC_DATA pPlayer, WORD skillNum )
{
	sPUSER_SKILL newSkill;

	newSkill = NewUserSkillData();

	if( !newSkill )
		return;

	newSkill->skillNum = skillNum;
	newSkill->prev = NULL;
	newSkill->next = NULL;

	INSERT_TO_LIST( pPlayer->ch2.haveSkill, newSkill, prev, next );
}

//
// Function Name: DestroyUserSkillMemory
// Date: 
// Description: 
//
void DestroyUserSkillMemory()
{
	sPUSER_SKILL data, next_data;

	LIST_WHILE( gPLAYERS.userSkill_memory, data, next_data, next, WMgr );
	REMOVE_FROM_LIST( gPLAYERS.userSkill_memory, data, prev, next );
	SAFE_FREE( data );
	LIST_WHILEEND( gPLAYERS.userSkill_memory, data, next_data );
}

//
// Function Name: ClearAllUserSKill
// Date: 
// Description: 
//
void ClearAllUserSKill( sPDESC_DATA pPlayer )
{
	sPUSER_SKILL data, next_data;

	LIST_WHILE( pPlayer->ch2.haveSkill, data, next_data, next, WMgr );
	REMOVE_FROM_LIST( pPlayer->ch2.haveSkill, data, prev, next );
	INSERT_TO_LIST( gPLAYERS.userSkill_memory, data, prev, next );
	LIST_WHILEEND( pPlayer->ch2.haveSkill, data, next_data );
}

//
// Function Name: IsHaveSkill
// Date: 
// Description: 
//
BOOL IsHaveSkill( sPDESC_DATA pPlayer, WORD skillNum )
{
	sPUSER_SKILL data, next_data;

	LIST_WHILE( pPlayer->ch2.haveSkill, data, next_data, next, WMgr );
	if( data->skillNum == skillNum )
		return 1;
	LIST_WHILEEND( pPlayer->ch2.haveSkill, data, next_data );
	return 0;
}

void ClearEffect( sPDESC_DATA pPlayer, int effectIndex )
{
	pPlayer->ch2.skillEffect[effectIndex].delay = 0;
	pPlayer->ch2.skillEffect[effectIndex].effectType = 0;
	pPlayer->ch2.skillEffect[effectIndex].value = 0;
	pPlayer->ch2.skillEffect[effectIndex].valueType = 0;
}

void UnSetVisibleSkill( sPDESC_DATA pPlayer )
{
	sPSKILL_DATA pBurrow = g_SKILL[g_ItemInfo[dSPE_SKILL_BURROW]->skillIdx];
	sPSKILL_DATA pStelth = g_SKILL[g_ItemInfo[dSPE_SKILL_STELTH]->skillIdx];

	// 버로우 
	if( pPlayer->ch2.skillEffect[pBurrow->effectIndex].delay )
	{
		ClearEffect( pPlayer, pBurrow->effectIndex );
		SetVisible( pPlayer, 0 );
		SetMovable( pPlayer, 0 );
		SendEffectOFF( pPlayer, dSPE_SKILL_BURROW );
	}
	// 스텔스
	if( pPlayer->ch2.skillEffect[pStelth->effectIndex].delay )
	{
		ClearEffect( pPlayer, pStelth->effectIndex );
		SetVisible( pPlayer, 0 );
		SendEffectOFF( pPlayer, dSPE_SKILL_STELTH );
	}
}

BOOL IsSetVisibleSkill( sPDESC_DATA pPlayer )
{
	sPSKILL_DATA pBurrow = g_SKILL[g_ItemInfo[dSPE_SKILL_BURROW]->skillIdx];
	sPSKILL_DATA pStelth = g_SKILL[g_ItemInfo[dSPE_SKILL_STELTH]->skillIdx];

	// 버로우 
	if( pBurrow )
	{
		if( pPlayer->ch2.skillEffect[pBurrow->effectIndex].delay )
		{
			return 1;
		}
	}
	// 스텔스
	if( pStelth )
	{
		if( pPlayer->ch2.skillEffect[pStelth->effectIndex].delay )
		{
			return 1;
		}
	}
	return 0;
}

int GetSkillAddDamage( sPSKILL_DATA pSkill , int baseDamage )
{
	if( !pSkill )
		return 0;

	int damage = 0;
	
	if( pSkill->effectType == dPLAYER_EFFECT_TYPE_ADDDAMAGE && pSkill->timer == 0 )
	{
		if( pSkill->effectValueType == dEFFECT_VALUE_TYPE_PERCENT )
		{
			if( pSkill->effectValue )
				damage += (int)( (float)baseDamage * (float)( (float)pSkill->effectValue / 100.0F ) );
		}
		else if( pSkill->effectValueType == dEFFECT_VALUE_TYPE_POINT )
			damage += pSkill->effectValue;
	}
	else
		damage += number( pSkill->minDamage, pSkill->maxDamage );

	return damage;
}

int GetSkillEffectAddDamage( sPDESC_DATA pPlayer, int baseDamage )
{
	if( !pPlayer )
		return 0;

	int damage = 0;
	
	for( int i = 0; i < dMAX_PLAYER_EFFECT; i++ )
	{
		if( pPlayer->ch2.skillEffect[i].delay && pPlayer->ch2.skillEffect[i].effectType == dPLAYER_EFFECT_TYPE_ADDDAMAGE )
		{
			if( pPlayer->ch2.skillEffect[i].valueType == dEFFECT_VALUE_TYPE_PERCENT )
			{
				if( pPlayer->ch2.skillEffect[i].value )
				{
					damage += (int)( (float)baseDamage * (float)( (float)pPlayer->ch2.skillEffect[i].value / 100.0F ) );
				}
			}
			else if( pPlayer->ch2.skillEffect[i].valueType == dEFFECT_VALUE_TYPE_POINT )
			{
				damage += pPlayer->ch2.skillEffect[i].value;
			}	
		}
	}

	return damage;
}

int GetSkillAddArmor( sPDESC_DATA pPlayer, int baseArmor  )
{
	if( !pPlayer )
		return 0;
	
	int armor = 0;
	
	for( int i = 0; i < dMAX_PLAYER_EFFECT; i++ )
	{
		if( pPlayer->ch2.skillEffect[i].delay && pPlayer->ch2.skillEffect[i].effectType == dPLAYER_EFFECT_TYPE_ADDARMOR )
		{
			if( pPlayer->ch2.skillEffect[i].valueType == dEFFECT_VALUE_TYPE_PERCENT )
			{
				if( pPlayer->ch2.skillEffect[i].value )
				{
					armor += (int)( (float)baseArmor * (float)( (float)pPlayer->ch2.skillEffect[i].value / 100.0F ) );
				}
			}
			else if( pPlayer->ch2.skillEffect[i].valueType == dEFFECT_VALUE_TYPE_POINT )
			{
				armor += pPlayer->ch2.skillEffect[i].value;
			}	
		}
	}
	return armor;
}

BOOL IsHaveSkillEffect( sPDESC_DATA pPlayer, int skillNum )
{
	if( !IS_VALID_ITEM( skillNum ) )
		return 0;

	sPSKILL_DATA pSkill = g_SKILL[g_ItemInfo[skillNum]->skillIdx];

	if( !pSkill )
		return 0;

	if( pPlayer->ch2.skillEffect[pSkill->effectIndex].delay )
		return 1;

	return 0;
}

BOOL CheckWaitSkill( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.waitSkill.skillNum )
	{
		SKILL_UseSkill( pPlayer, 
			pPlayer->ch2.waitSkill.targetIdx, 
			pPlayer->ch2.waitSkill.targetType,
			pPlayer->ch2.waitSkill.skillNum );
		
		pPlayer->ch2.waitSkill.skillNum = 0;
		pPlayer->ch2.waitSkill.targetIdx = 0;
		pPlayer->ch2.waitSkill.targetType = 0;
		
		return 1;
	}
	return 0;
}

void SetBuffSkill( sPDESC_DATA pPlayer, int skillNum )
{
	switch( skillNum ) 
	{
	case dMAGIC_NUM_FIREBUST:		//... ...
		{
			SendSystemMsg( pPlayer, g_LANG_STR[290], 3 );
		}
		break;

	case dMAGIC_NUM_DARKFIRECHANT:	//.......
		{
			SendSystemMsg( pPlayer, g_LANG_STR[290], 5 );//Attack rate raised.", 15 );
		}
		break;

	case dMAGIC_NUM_DEATHFIREORR:	//.......
		{
			SendSystemMsg( pPlayer, g_LANG_STR[290], 10 );
		}
		break;

	case dMAGIC_NUM_CLOUDSHIELD:	//...... 
		{
			SendSystemMsg( pPlayer, g_LANG_STR[291], 3 );	//5
		}
		break;

	case dMAGIC_NUM_BRIGHTSHIELD:	//......
		{
			SendSystemMsg( pPlayer, g_LANG_STR[291], 5 );	//10
		}
		break;

	case dMAGIC_NUM_PROTECTIONSHIELD:	//......
		{
			SendSystemMsg( pPlayer, g_LANG_STR[291], 10 ); //15
		}
		break;
	}
}

//


void SendGetExpMsg(sPDESC_DATA pPlayer, BYTE skill, int exp){
 if(exp<=0)return;
 SendGetExpMsg(pPlayer,skill,(WORD)(exp>65535?65535:exp));
 if(exp>65535){SendSetExpMsg(pPlayer,skill,pPlayer->ch.skill_exp[skill]);SendSystemMsg(pPlayer,(char*)"[EXP] +%d",exp);}
}
