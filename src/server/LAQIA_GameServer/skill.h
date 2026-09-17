/**
 *	$Workfile: skill.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 벨
 *
 *	$Revision: 21 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/skill.h $
 * 
 * 21    04-12-15 10:40a Sk8snow
 * 
 * 20    04-11-18 1:40a Sk8snow
 * 
 * 19    04-09-20 10:53p Sk8snow
 * 
 * 18    04-09-11 11:20p Sk8snow
 * 
 * 17    04-09-08 3:54p Sk8snow
 * 
 * 16    04-07-13 7:26a Sk8snow
 * 
 * 15    04-05-29 10:12p Sk8snow
 * 
 * 14    04-05-11 1:45a Sk8snow
 * 
 * 13    04-04-30 9:46a Sk8snow
 * 
 * 12    04-04-07 10:17p Sk8snow
 * 
 * 11    04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __SKILL_H__
#define __SKILL_H__

#define dMAX_LEVEL						1600 //총레
#define dMAX_SKILL_LEVEL					400 //캐릭터 스킬레벨

#define dMAGIC_TYPE_NUNE					0
#define dMAGIC_TYPE_ATTACK					1			// 공격형 마법 
#define dMAGIC_TYPE_CAST					2			// 사용 마법

#define dMAGIC_VICT_TARGET					0			// 타겟  지정 마법
#define dMAGIC_VICT_BYSELF					1			// 자기 자신에게 만
#define dMAGIC_VICT_BOTH					2			// 자기자신 또는 타겟 지정 가능

#define dSKILL_AREA_NONE					0			// 영역 없음
#define dSKILL_AREA_PLAYER_ZONE				1			// 플레이어 기준 지역
#define dSKILL_AREA_MOB_ZONE				2			// 몹 기준 지역   
#define dSKILL_AREA_STRAIGHT				3			// 직선 

#define dMAX_SKILLS							512	
#define dMAX_SPE_SKILL						512		
#define IS_VALID_SKILL( num ) (g_SKILL[num])

#define dSKILL_TYPE_SATTACK					0			// 근거리 공격기술
#define dSKILL_TYPE_SASSIST					1			// 근거리 보조기술
#define dSKILL_TYPE_LATTACK					2			// 원거리 공격기술
#define dSKILL_TYPE_LASSIST					3			// 원거리 보조기술 
#define dSKILL_TYPE_BMAGIC					4			// 공격계 흑마법 
#define dSKILL_TYPE_BASSIST					5			// 보조계 흑마법 
#define dSKILL_TYPE_WMAGIC					6			// 공격계 백마법
#define dSKILL_TYPE_WASSIST					7			// 보조계 백마법
#define dSKILL_TYPE_SPEATTACK				8			// 특화 공격 스킬
#define dSKILL_TYPE_SPEASSIST				9			// 특화 보조 스킬

#define IS_ATTACK_MAGIC( skill ) ( skill->skillType == dSKILL_TYPE_BMAGIC || skill->skillType == dSKILL_TYPE_WMAGIC )
#define IS_SATTACK( skill ) ( skill->skillType == dSKILL_TYPE_SATTACK || skill->skillType == dSKILL_TYPE_SASSIST )
#define IS_LATTACK( skill ) ( skill->skillType == dSKILL_TYPE_LATTACK || skill->skillType == dSKILL_TYPE_LASSIST )
#define IS_BMAGIC( skill ) ( skill->skillType == dSKILL_TYPE_BMAGIC || skill->skillType == dSKILL_TYPE_BASSIST )
#define IS_WMAGIC( skill ) ( skill->skillType == dSKILL_TYPE_WMAGIC || skill->skillType == dSKILL_TYPE_WASSIST )
#define IS_SPESKILL( skill ) ( skill->skillType == dSKILL_TYPE_SPEATTACK || skill->skillType == dSKILL_TYPE_SPEASSIST )
#define IS_ATTACK_SKILL( skill ) ( skill->skillType == dSKILL_TYPE_SATTACK || \
								   skill->skillType == dSKILL_TYPE_LATTACK || \
								   skill->skillType == dSKILL_TYPE_BMAGIC || \
								   skill->skillType == dSKILL_TYPE_WMAGIC || \
								   skill->skillType == dSKILL_TYPE_SPEATTACK )

#define GET_SKILL_DATA( p ) (g_SKILL[g_ItemInfo[GET_USE_SKILL(p)]->skillIdx])
#define GET_ITEM_SKILL( item ) (g_SKILL[g_ItemInfo[item->itemNum]->skillIdx])

#define dSATTACK_SKILL_ITEM					10101		// 근거리 기본 
#define dLATTACK_SKILL_ITEM					10102		// 원거리 기본

///////////////////////////////////////////////////////
// 특화스킬
///////////////////////////////////////////////////////
#define dSPE_SKILL_WARCRY					10129			// 워크라이(워리어)
#define dSPE_SKILL_BURROW					10130			// 버로우(밴디츠)
#define dSPE_SKILL_ENERGYDRAIN				10121			// 에너지드레인(샤도우나이츠)
#define dSPE_SKILL_BLESSINGSHIELD				10131			// 블레싱 실드(팔라딘)
#define dSPE_SKILL_STELTH					10132			// 스텔스(어쌔신)
#define dSPE_SKILL_CRITICAL					10122			// 크리티컬(아처)
#define dSPE_SKILL_FIREARROW				10123			// 파이어 에로우(포처)
#define dSPE_SKILL_HEAVENLYARROW				10124			// 해븐리 에로우(레인저)
#define dSPE_SKILL_POISONWEAPON				10125			// 포이즌웨폰(블레이드 메이지)l
#define dSPE_SKILL_POISONARROW				10126			// 포이즌에로우(원더러)
#define dSPE_SKILL_DUSTSKIN					10133			// 더스트스킨(위저드)
#define dSPE_SKILL_REGENERATION				10134			// 리제너레이션(샤먼)
#define dSPE_SKILL_HEAVENLYSTAFF				10127			// 해븐리 스테프(몽크)
#define dSPE_SKILL_SENSTIVE					10135			// 센스티브(호크)
#define dSPE_SKILL_REJYUVERNATION				10136			// 리쥬버네이션(드루이드)
#define dSPE_SKILL_HEAVENLYHAMMER				10128			// 해븐리해머(클레릭)

#define dSPESKILL_DELAYIDX_STELTH			0				// 스텔스 
#define dSPESKILL_DELAYIDX_ENERGYDRAIN		1				// 에너지드레인
#define dSPESKILL_DELAYIDX_BURROW			2				// 버로우
#define dSPESKILL_DELAYIDX_CRITICAL		3				// 크리티컬 
#define dSPESKILL_DELAYIDX_FIREARROW		4				// 파이어에로우
#define dSPESKILL_DELAYIDX_HEAVENLYARROW		5				// 헤븐리에로우
#define dSPESKILL_DELAYIDX_POISONWEAPON		6				// 포이즌웨폰
#define dSPESKILL_DELAYIDX_POISONARROW		7				// 포이즌에로우
#define dSPESKILL_DELAYIDX_DUSTSKIN		8				// 더스트스킨
#define dSPESKILL_DELAYIDX_REGENERATION		9				// 리제너레이션 
#define dSPESKILL_DELAYIDX_HOMEWARDRING		10				// 홈워드링 ( 스킬 아님! )
#define dSPESKILL_DELAYIDX_HEAVENLYSTAFF		11				// 헤븐리 스테프 
#define dSPESKILL_DELAYIDX_HEAVENLYHAMMER		12				// 헤븐리 헤머

#define dMAGIC_NUM_FLAMEARROW				10103			// 플래임에로우
#define dMAGIC_NUM_FLAMMINGSPHERE				10104			// 플래밍스피어 
#define dMAGIC_NUM_PRODUCEFLAME				10105			// 프로듀스 플래임
#define dMAGIC_NUM_FIREBALL					10106			// 파이어볼
#define dMAGIC_NUM_FIREPIERCE				10107			// 파이어피어스 
#define dMAGIC_NUM_PRODUCEICE				10108			// 프로듀스아이스
#define dMAGIC_NUM_ICESWORD					10109			// 아이스스워드
#define dMAGIC_NUM_FIREBUST					10110			// 파이어버스트
#define dMAGIC_NUM_SPIKEICE					10111			// 스파이크아이스 
#define dMAGIC_NUM_LIGHTNINGARROW				10112			// 라이트닝에로우
#define dMAGIC_NUM_LIGHTNING				10113			// 라이트닝 
#define dMAGIC_NUM_CALLLIGHTNING				10114			// 콜라이트닝 
#define dMAGIC_NUM_MINUHEALLING				10115			// 마이너힐링
#define dMAGIC_NUM_LIGHTNINGPIERCE				10116			// 라이트닝피어스
#define dMAGIC_NUM_WINDSWORD				10117			// 윈드소드 
#define dMAGIC_NUM_WINDCANON				10118			// 윈드캐논 
#define dMAGIC_NUM_CLOUDSHIELD				10119			// 클라우드실드 
#define dMAGIC_NUM_WINDSTRIKE				10120			// 윈드스트라이크

#define dMAGIC_NUM_CHILLTOUCH				10386			// 칠터치 
#define dMAGIC_NUM_CLOUDKILL				10387			// 클라우드킬
#define dMAGIC_NUM_FIREWALL					10388			// 파이어월
#define dMAGIC_NUM_CURELIGHT				10389			// 큐어라이트 
#define dMAGIC_NUM_EARTHSWORD				10390			// 어스스워드
#define dMAGIC_NUM_GLASSIP					10391			// 글래십

// 2차
#define dMAGIC_NUM_LIGHTNINGARROWBLOT			12049			//라이트닝애로우볼트
#define dMAGIC_NUM_LIGHTNINGARROWWAVE			12050			//라이트닝애로우웨이브
#define dMAGIC_NUM_LIGHTNINGBOLT				12051			//라이트닝볼트
#define dMAGIC_NUM_LIGHTNINGWAVE				12052			//라이트닝웨이브
#define dMAGIC_NUM_CALLLIGHTNINGBOLT			12053			//콜라이트닝볼트
#define dMAGIC_NUM_CALLLIGHTNINGWAVE			12054			//콜라이트닝웨이브
#define dMAGIC_NUM_CHAINLIGNTNINGPIERCE			12055			//체인라이트닝피어스
#define dMAGIC_NUM_SHORKLIGHTNINGPIERCE			12056			//쇼크라이트닝피어스
#define dMAGIC_NUM_ELETRIC					12057			//일렉트릭
#define dMAGIC_NUM_THUNDERBOLT				12058			//썬더볼트
#define dMAGIC_NUM_WINDSHIELD				12059			//윈드실드
#define dMAGIC_NUM_WINDARROW				12060			//윈드애로우
#define dMAGIC_NUM_WINDBALL					12061			//윈드볼
#define dMAGIC_NUM_WINDSPHERE				12062			//윈드스피어
#define dMAGIC_NUM_BRIGHTSHIELD				12063			//브라이트실드
#define dMAGIC_NUM_PROTECTIONSHIELD			12064			//프로텍션실드
#define dMAGIC_NUM_WINDSTORM				12065			//윈드스톰
#define dMAGIC_NUM_TONEIDO					12066			//토네이도
#define dMAGIC_NUM_RECOVERYHEALING				12067			//리커버리힐링
#define dMAGIC_NUM_GREATEHEALING				12068			//그레이트힐링
#define dMAGIC_NUM_CUREUNZ					12069			//큐어운즈
#define dMAGIC_NUM_HEAL					12070			//힐
#define dMAGIC_NUM_GROUNDSTON				12071			//그라운드스톤
#define dMAGIC_NUM_GROUNDCRAY				12072			//그라운드크레이
#define dMAGIC_NUM_DIALFLAMEARROW				12073			//듀얼플레임애로우
#define dMAGIC_NUM_TRIPLEFLAMEARROW			12074			//트리플플래임애로우
#define dMAGIC_NUM_FLAMINGSCORUM				12075			//플래밍스코룸
#define dMAGIC_NUM_STRIKECORUM				12076			//스트라이크코룸
#define dMAGIC_NUM_DARKPRODUCEFLAME			12077			//다크플래듀스플래임
#define dMAGIC_NUM_DEATHPRODUCEFLAME			12078			//데쓰플래듀스플래임
#define dMAGIC_NUM_DARKFIREBALL				12079			//다크파이어볼
#define dMAGIC_NUM_DEATHFIREBALL				12080			//데쓰파이어볼
#define dMAGIC_NUM_DARKFIREPIERCE				12081			//다크파이어피어스
#define dMAGIC_NUM_DEATHFIREPIERCE				12082			//데쓰파이어피어스
#define dMAGIC_NUM_DARKFIRECHANT				12083			//다크파이어챈트
#define dMAGIC_NUM_DEATHFIREORR				12084			//데쓰파이어오러
#define dMAGIC_NUM_DARKFIRECORUM				12085			//다크파이어코룸
#define dMAGIC_NUM_DEATHFIRECORUMHELL			12086			//데쓰파이어코룸헬
#define dMAGIC_NUM_PRODUCEICEARROW				12087			//프로듀스아이스애로우
#define dMAGIC_NUM_PRODUCEICEBUG				12088			//프로듀스아이스버그
#define dMAGIC_NUM_ICESWORDCORUM				12089			//아이스스워드코룸
#define dMAGIC_NUM_ICESWORDHELL				12090			//아이스스워드헬
#define dMAGIC_NUM_SPIKEICEWAVE				12091			//스파이크아이스웨이브
#define dMAGIC_NUM_SPIKEICEMETEO				12092			//스파이크아이스메테오
#define dMAGIC_NUM_HELLSCREAM				12093			//헬스크림
#define dMAGIC_NUM_CULLREPS					12094			//컬렙스
#define dMAGIC_NUM_DEADFORG					12095			//데드포그
#define dMAGIC_NUM_DEATHFORGKILL				12096			//데쓰포그킬

#define dMAGIC_IDX_FIREBUST					1				// 파이어버스트
#define dMAGIC_IDX_CLOUDSHIELD				2				// 클라우드실드

#define dMAGIC_IDX_SEED					300				//

// 전사 스킬 
#define dSATTACK_SKILL_WOLFCLOW			10392			// 울프클로
#define dSATTACK_SKILL_STAFF			10393			// 스탭
#define dSATTACK_SKILL_BLADEWIND			10394			// 블레이드윈드 

// 2차
#define dSATTACK_SKILL_BEARCLOW			12031			//베어클로
#define dSATTACK_SKILL_ORGCLOW			12032			//오거클로
#define dSATTACK_SKILL_WILDSTAFF			12033			//와일드스텝
#define dSATTACK_SKILL_BUSTSTAFF			12034			//버스트스텝
#define dSATTACK_SKILL_BLADEVRIZE			12035			//블레이드브리즈
#define dSATTACK_SKILL_BLADEGAIL			12036			//블레이드게일
#define dSATTACK_SKILL_BORNBREAK			12037			//본브레이크
#define dSATTACK_SKILL_DOUBLEBREAK			12038			//더블브레이크
#define dSATTACK_SKILL_BREAKREVOLUTION		12039			//브레이크레볼루션

// 궁수 스킬 
#define dLATTACK_SKILL_SPIRITARROW			10395			// 스피릿애로우
#define dLATTACK_SKILL_FIREHON			10396			// 파이어혼
#define dLATTACK_SKILL_DOUBLESHOT			10397			// 더블샷

// 2차
#define dLATTACK_SKILL_DARKSPRITARROW		12040			//다크스피릿애로우
#define dLATTACK_SKILL_BLUDSPRITARROW		12041			//블러드스피릿애로우
#define dLATTACK_SKILL_ICEHON			12042			//아이스혼
#define dLATTACK_SKILL_LIGHTININGHON		12043			//라이트닝혼
#define dLATTACK_SKILL_TRIPLESHOT			12044			//트리플샷
#define dLATTACK_SKILL_DOUBLEDOUBLESHOT		12045			//더블더블샷
#define dLATTACK_SKILL_PANUTRATION			12046			//패너트레이션
#define dLATTACK_SKILL_POWERPANUTRATION		12047			//파워패너트레이션
#define dLATTACK_SKILL_DEATHPANUTRATION		12048			//데쓰패너트레이션


#define dSKILL_ARRAY_SATTACK				0
#define dSKILL_ARRAY_LATTACK				1
#define dSKILL_ARRAY_BMAGIC					2
#define dSKILL_ARRAY_WMAGIC					3

#define dSKILL_NORMAL_DELAY					100  // 스킬딜레이
#define dATTACK_NORMAL_DELAY				200  // 어텍딜레

// 경험치 테이블 
extern int g_StateLevelExp[dMAX_LEVEL][3];
extern int g_SkillLevelExp[dMAX_LEVEL][4];

extern sPSKILL_DATA g_SKILL[dMAX_SKILLS];

BOOL LoadStateExpTable();
BOOL LoadSkillExpTable();

////////////////////////////////////////////////////////
// F U N C S
////////////////////////////////////////////////////////
int get_skill_exp( int lev , BYTE skill );
int get_state_exp( int lev , BYTE state );

void CheckLevelUp( sPDESC_DATA pPlayer , BYTE skill );
void SendGetExpMsg( sPDESC_DATA pPlayer, BYTE skill, WORD exp );
void SendGetExpMsg(sPDESC_DATA pPlayer, BYTE skill, int exp);

int MAGIC_GetNeedMp( sPDESC_DATA pPlayer );
int MAGIC_GetDamage( sPDESC_DATA pPlayer );
void MAGIC_SendMagicInfo( sPDESC_DATA pPlayer );

void MAGIC_CastMagic( sPDESC_DATA pPlayer, int idxNum, BYTE target );

void AddPlayerSkillDirect( sPDESC_DATA pPlayer, int itemNum );
BOOL AddPlayerSkill( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void SendPlayerSkillList( sPDESC_DATA pPlayer );

WORD FindSkillIdx( int itemNum );
BOOL LoadSkillData();

BOOL LoadSpeSkillData();

void SendSetExpMsg( sPDESC_DATA pPlayer, BYTE skill, int exp );

void SKILL_UseSkill( sPDESC_DATA pPlayer, int idxNum, BYTE target, WORD skillNum );

void AddUserSkillData( sPDESC_DATA pPlayer, WORD skillNum );
void DestroyUserSkillMemory();
void ClearAllUserSKill( sPDESC_DATA pPlayer );
BOOL IsHaveSkill( sPDESC_DATA pPlayer, WORD skillNum );

void ClearEffect( sPDESC_DATA pPlayer, int effectIndex );
void UnSetVisibleSkill( sPDESC_DATA pPlayer );
BOOL IsSetVisibleSkill( sPDESC_DATA pPlayer );
int GetSkillAddDamage( sPSKILL_DATA pSkill , int baseDamage );
int GetSkillEffectAddDamage( sPDESC_DATA pPlayer , int baseDamage );
int GetSkillAddArmor( sPDESC_DATA pPlayer, int baseArmor  );
BOOL IsHaveSkillEffect( sPDESC_DATA pPlayer, int skillNum );
BOOL CheckWaitSkill( sPDESC_DATA pPlayer );

// 스킬 함수들 
BOOL SKILL_NullFunc( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill );

BOOL SKILL_EnergyDrain( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill );
BOOL SKILL_Critical( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill );
BOOL SKILL_FireArrow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_HavenlyArrow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSKill );
BOOL SKILL_PoisonWeapon( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_PoisonArraow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_HavenlyHammer( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_HavenlyStaff( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_WarCry( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_Burrow( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_BlessingShield( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_Stelth( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_DustSkin( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_Regeneration( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_Senstive( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );
BOOL SKILL_Rejyubernation( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill );

void AsignSkillFunc();

#ifdef _dUMENOKOJI_	//파이어버스트관련등등등
#define dSKILLDATA_NUM_DARKFIRE			1
#define dSKILLDATA_NUM_FIREBUST			16
#define dSKILLDATA_NUM_CLOUDSHIELD			3
#define dSKILLDATA_NUM_BRIGHTSHIELD		14

void	SetBuffSkill( sPDESC_DATA pPlayer, int skillNum );
BOOL	GetAttackBuffSkill( sPDESC_DATA pPlayer );

#endif

#endif 



