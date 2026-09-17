/**
 *	$Workfile: effect.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 17 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/effect.h $
 * 
 * 17    04-12-15 10:40a Sk8snow
 * 
 * 16    04-11-18 1:40a Sk8snow
 * 
 * 15    04-09-20 10:53p Sk8snow
 * 
 * 14    04-09-08 3:54p Sk8snow
 * 
 * 13    04-08-04 9:27a Sk8snow
 * 
 * 12    04-07-13 7:26a Sk8snow
 * 
 * 11    04-05-14 10:38p Sk8snow
 * 
 * 10    04-05-11 1:45a Sk8snow
 * 
 * 9     04-04-30 9:46a Sk8snow
 * 
 * 8     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __EFFECT_H__
#define __EFFECT_H__

// 사운드 이펙트 인덱스
enum
{
	SOUND_EFFECT_HPPOTION_S		=	1,	// HP포션 (소) 마시는 사운드
	SOUND_EFFECT_HPPOTION_M		,		// HP포션 (중) 마시는 사운드
	SOUND_EFFECT_HPPOTION_L		,		// HP포션 (대) 마시는 사운드
	SOUND_EFFECT_MPPOTION_S		,		// MP포션 (소) 마시는 사운드
	SOUND_EFFECT_MPPOTION_M		,		// MP포션 (중) 마시는 사운드 
	SOUND_EFFECT_MPPOTION_L		,		// MP포션 (대) 마시는 사운드
};

// 플레이어 이벡트 
enum
{
	PLAYER_EFFECT_STUN		=	1,		// 스턴이팩트
	PLAYER_EFFECT_DONTMOVE,				// 이동 못함
	PLAYER_EFFECT_DONTATTACK,			// 공격 못함
	
	PLAYER_EFFECT_BILL1,				// 웜스 오브 아라임			1.3
	PLAYER_EFFECT_BILL2,				// 오오라 오브 브릴리언스
	PLAYER_EFFECT_BILL3,				// 팬텀게더러
	PLAYER_EFFECT_BILL4,				// 외침용알약
	PLAYER_EFFECT_BILL5,				// 아이템 오브 아라임		1.3
	PLAYER_EFFECT_BILL6,				// 부활의 두루마리 	
	PLAYER_EFFECT_BILL7,				// 길드공지의 알약 
	PLAYER_EFFECT_BILL8,				// 프리미엄 존 
	PLAYER_EFFECT_BILL9,				// 프리미엄 존 플러스 
	PLAYER_EFFECT_BILL10,				// 펫 푸드 
	PLAYER_EFFECT_BILL11,				// 신의축복 
	PLAYER_EFFECT_BILL12,				// 인챈트 체인지 
	PLAYER_EFFECT_BILL13,				// 인첸트 콜렉션 
	PLAYER_EFFECT_BILL14,				// 아이템룩 
	PLAYER_EFFECT_BILL15,				// 아이템 프로텍션 
	PLAYER_EFFECT_BILL16,				// 레벨룩 
	PLAYER_EFFECT_BILL17,				// 레벨 프로텍션 
	PLAYER_EFFECT_BILL18,				// 기동성의 물약 
	PLAYER_EFFECT_BILL19,				// 웨이트 드링크 
	PLAYER_EFFECT_BILL20,				// HP 드링크 
	PLAYER_EFFECT_BILL21,				// MP 드링크 
	PLAYER_EFFECT_BILL22,				// 그룹 웜쓰 오브 아라임 
	PLAYER_EFFECT_BILL23,				// 그룹 HP 드링크 
	PLAYER_EFFECT_BILL24,				// 그룹 MP 드링크 
	PLAYER_EFFECT_BILL25,				// 군단원 HP 드링크 
	PLAYER_EFFECT_BILL26,				// 군단원 MP 드링크
	PLAYER_EFFECT_BILL27,				// 웜스 오브 아라임			1.5
	PLAYER_EFFECT_BILL28,				// 아이템 오브 아라임		1.5
	PLAYER_EFFECT_BILL29,				// 단원 소환
	PLAYER_EFFECT_BILL30,				// 단원 필승의 외침
	PLAYER_EFFECT_BILL31,				// 단원 축복의 외침
	PLAYER_EFFECT_BILL32,				// 단원 기원의 외침
	PLAYER_EFFECT_BILL33,				// 단원 저주의 외침
	PLAYER_EFFECT_BILL34,				// 단원 도발의 외침 
	PLAYER_EFFECT_BILL35,				// 마이너스 
	PLAYER_EFFECT_BILL36,				// 하트 이모티콘
	PLAYER_EFFECT_BILL37,				// 기쁨 이모티콘
	PLAYER_EFFECT_BILL38,				// 슬픔 이모티콘
	PLAYER_EFFECT_BILL39,				// 분노 이모티콘
};

// 몹 이펙트 
enum
{
	MOB_EFFECT_STUN			=	1,		// 스턴 이팩트
	MOB_EFFECT_DONTMOVE,				// 이동 못함
	MOB_EFFECT_DONTATTACK,				// 공격 못함 	
};

// 몹 효과
enum
{
	MOB_ATTREFFECT_CHILTOUCH = 0,		// 칠터치
	MOB_ATTREFFECT_POISONWEAPON,		// 포이즌 웨폰 
	MOB_ATTREFFECT_POISONARROW,			// 포이즌 에로우
	MOB_ATTREFFECT_DUSTSKIN,			// 더스트스킨
	MOB_ATTREFFECT_DONTMOVE,			// 이동못함
	MOB_ATTREFFECT_CANTATTACK,			// 공격못함
//	MOB_ATTREFFECT_
};



//
#define dSTATUS_DELAY_STUN							0


// 기술


void EFFECT_CheckTime( sPDESC_DATA pPlayer );
void SendSoundEffect( sPDESC_DATA pPlayer, WORD soundIdx );
void SendEffectON( sPDESC_DATA pPlayer, WORD skillNum, int time );
void SendEffectOFF( sPDESC_DATA pPlayer, WORD skillNum );
void SetMovable( sPDESC_DATA pPlayer, BYTE flag );
void ClearPlayerSkillEffect( sPDESC_DATA pPlayer );
void SendAllEffect( sPDESC_DATA pPlayer );

#endif


//




