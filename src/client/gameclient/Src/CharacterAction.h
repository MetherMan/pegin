/**
 * Character Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-09-14 5:57p $
 * @version	$Revision: 43 $
 *
 * <HR>
 * $Log: /GameClient/Src/CharacterAction.h $
 * 
 * 43    04-09-14 5:57p Sk8snow2
 * 
 * 42    04-09-11 5:49p Sk8snow2
 * 
 * 41    04-09-10 10:28p Sk8snow2
 * 
 * 40    04-09-10 6:09p Sk8snow2
 * 
 * 39    04-06-04 4:29p Sckyj
 * 
 * 38    04-03-28 7:52p Sckyj
 * 
 * 37    04-03-28 3:20p Sckyj
 * 
 * 36    04-03-27 10:33a Sckyj
 * 
 * 35    04-03-12 6:41p Sckyj
 * 운영자 작업중
 * 
 * 34    04-02-02 4:47p Kjmgo
 * 
 * 33    04-01-30 5:46p Kjmgo
 * 
 * 32    04-01-06 9:51p Kjmgo
 * 동작쪽 버그 수정
 * 
 * 31    04-01-06 12:01a Kjmgo
 * 공속 추가 
 * 
 * 30    03-12-24 10:30a Kjmgo
 * 스킬적용
 * 
 * 29    03-12-22 5:00p Sckyj
 * 2D, 3D sound 분리
 * 
 * 28    03-12-16 1:25a Sckyj
 * 물위에서 물발자국 소리 나도록 추가
 * 
 * 27    03-12-06 2:35p Kjmgo
 * 평화모드인지 전투 모드인지 판단하는 함수 추가
 * 
 * 26    03-10-16 4:41p Sckyj
 * sound재생관련 코드 수정
 * 
 * 25    03-10-07 9:27a Kjmgo
 * 활을 들고 말을 탔을때의 오류 수정( AddVehicle시,
 * VehicleAction->SetWeaopn();으로 하던것을 SetWeapon2()로 수정
 * 
 * 24    03-10-04 2:02a Kjmgo
 * 동작 버그 잡는중 : 40%
 * 
 * 23    03-10-04 1:18a Kjmgo
 * 캐릭터 공격 부분 수정 및, 근거리 스킬이 아닐경우, 근거리 아이템을
 * 바꾸면 무기 동작이 안바뀌던것 수정( 원거리가 아닐경우 바꾸기로.)
 * 
 * 22    03-10-02 11:20p Kjmgo
 * 캐릭터 State 권한 이전 : 완료 
 * 
 * 21    03-10-02 2:41p Kjmgo
 * 몬스터 공격 거리를 몬스터의 Radius만큼의 여유를 줌
 * 
 * 20    03-10-01 6:40p Kjmgo
 * Skill적용
 * 
 * 19    03-09-23 9:54p Kjmgo
 * State중, Skill추가
 * 
 * 18    03-09-23 9:38p Kjmgo
 * Skill 관련 부분 작업
 * 
 * 17    03-09-23 10:48a Kjmgo
 * 
 * 16    03-09-23 2:57a Kjmgo
 * 
 * 15    03-09-23 2:31a Kjmgo
 * 
 * 14    03-09-23 2:20a Kjmgo
 * 
 * 13    03-09-18 10:58a Kjmgo
 * 데미지 처리 관련 수정
 * 
 * 12    03-09-13 1:26p Kjmgo
 * 활을 비롯한 몇가지 옵션 추가
 * 
 * 11    03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * 10    03-09-10 2:42p Kjmgo
 * 
 * 9     03-09-10 2:30a Kjmgo
 * 
 * 8     03-09-09 3:56p Kjmgo
 * 
 * 7     03-09-08 8:20p Kjmgo
 * 동작~ 작업
 * 
 * 6     03-09-07 6:03p Kjmgo
 * 동작 수정 및, Model Setting할수 있도록 수정
 * 
 * 5     03-09-05 2:14a Kjmgo
 * 방패 착용 
 * 
 * 4     03-09-02 5:49a Kjmgo
 * Monster 추가작업
 * 
 * 3     03-08-31 6:18p Kjmgo
 * Character Action 관련 추가
 * 
 * 2     03-08-30 11:07a Kjmgo
 * Character Animation Data 읽는 부분 추가
 * 
 * 1     03-08-29 11:44p Kjmgo
 * <HR>
 *
 * @file	CharacterAction.h
 */

#ifndef __CharacterAction_Header__
#define __CharacterAction_Header__

#include "Global.h"
#include "VehicleAction.h"

////////////////////////////////////////////////////////////////

#define SEX_MALE						0
#define SEX_FEMALE						1

////////////////////////////////////////////////////////////////

enum
{
	_MALE = 0,
	_FEMALE,
	_MASTER
};

/*0 맨손
1 맨손	+ 방패
2 한손검
3 한손검 + 방패
4 양손검
5 쌍칼
6 한손도끼
7 한손도끼 + 방패
8 양손도끼
9 쌍도끼
10 한손둔기
11 한손둔기 + 방패
12 양손둔기
13 쌍둔기
14 돈파
15 단궁 
16 장궁
17 연노
18 석궁
19 창
20 봉
21 지팡이
22 도
23 참마도*/
#define WEAPON_NONE						0		//"0.맨손",
#define WEAPON_NONEWITHSHIELD			1		//"0.맨손방패",
#define WEAPON_ONEHANDSWORD				2		//"1.한손검",
#define WEAPON_ONEHANDSWORDWITHSHIELD	3		//"1.한손검방패",
#define WEAPON_TWOHANDSWORD				4		//"2.양손검", WITHSHIELD
#define WEAPON_DUALSWORD				5		//"3.쌍칼",
#define WEAPON_ONEHANDAXE				6		//"4.한손도끼",
#define WEAPON_ONEHANDAXEWITHSHIELD		7		//"4.한손도끼방패",
#define WEAPON_TWOHANDAXE				8		//"5.양손도끼",
#define WEAPON_DUALAXE					9		//"6.쌍도끼",
#define WEAPON_ONEHANDBLUNT				10		//"7.한손둔기",
#define WEAPON_ONEHANDBLUNTWITHSHIELD	11		//"7.한손둔기방패",
#define WEAPON_TWOHANDBLUNT				12		//"8.양손둔기",
#define WEAPON_DUALBLUNT				13		//"9.쌍둔기",
#define WEAPON_DONPA					14		//"10.돈파",
#define WEAPON_SHORTBOW					15		//"11.단궁",
#define WEAPON_LONGBOW					16		//"12.장궁",
#define WEAPON_DDABALBOW				17		//"13.연노",
#define WEAPON_CROSSBOW					18		//"14.석궁",
#define WEAPON_SPHERE					19		//"16.창",
#define WEAPON_BONG						20		//"17.봉",
#define WEAPON_STAFF					21		//"18.지팡이",
#define WEAPON_DO						22		//"19.노" 
#define WEAPON_SLAYER					23		//"20.참마도"
// 이벤트용 무기
#define WEAPON_FISHINGTACKLE			24		//"21.낚시대"

////////////////////////////////////////////////////////////////

/*0 정지
1 걷기
2 뛰기
3 뛰기전동작
4 뛰고난후동작
5 사망
6 걷기전쟁모드변경
7 뛰기전쟁모드변경
8 공격1
9 공격2
10 공격3
11 공격4
12 공격5
13 뛰면서공격1
14 뛰면서공격2
15 뛰면서공격3
16 백마법시전
17 흑마법시전
18 이모티콘(피로)
*/

#define CHARACTER_STATE_STOP					0
#define CHARACTER_STATE_WALK					1
#define CHARACTER_STATE_RUN						2
#define CHARACTER_STATE_DIE						3
#define CHARACTER_STATE_WALK_MODECHANGE			4
#define CHARACTER_STATE_RUN_MODECHANGE			5
#define CHARACTER_STATE_ATTACK1					6
#define CHARACTER_STATE_ATTACK2					7
#define CHARACTER_STATE_ATTACK3					8
#define CHARACTER_STATE_ATTACK4					9
#define CHARACTER_STATE_ATTACK5					10
#define CHARACTER_STATE_RUN_ATTACK1				11
#define CHARACTER_STATE_RUN_ATTACK2				12
#define CHARACTER_STATE_RUN_ATTACK3				13
#define CHARACTER_STATE_FISHING					CHARACTER_STATE_WALK // 낚시 동작을 걷기에 넣어놨다

// 공격형 마법
#define STATE_MAGIC_ATTACK_UPANDDOWN	14
#define STATE_MAGIC_ATTACK_DOWNANDUP	15
#define STATE_MAGIC_ATTACK_GO			16
#define STATE_MAGIC_ATTACK_AROUND		17

// 축복
#define STATE_MAGIC_BLESS_UPANDDOWN		18
#define STATE_MAGIC_BLESS_DOWNANDUP		19
#define STATE_MAGIC_BLESS_GO			20
#define STATE_MAGIC_BLESS_AROUND		21

// 소환
#define STATE_MAGIC_RECALL_UPANDDOWN	22
#define STATE_MAGIC_RECALL_DOWNANDUP	23
#define STATE_MAGIC_RECALL_GO			24
#define STATE_MAGIC_RECALL_AROUND		25

// 저
#define STATE_MAGIC_CURSE_UPANDDOWN		26
#define STATE_MAGIC_CURSE_DOWNANDUP		27
#define STATE_MAGIC_CURSE_GO			28
#define STATE_MAGIC_CURSE_AROUND		29

// 기타 동작
#define STATE_ETC						31

//////////////////////////////////////////////////////

#define MODE_STOP			0
#define MODE_WALK			1
#define MODE_RUN			2
#define MODE_WALK_ATTACK	3
#define MODE_RUN_ATTACK		4
#define MODE_ATTACK			5
#define MODE_DEAD			6
#define MODE_ATTACK_RUN		7
#define MODE_MAGIC_CASTING	8
#define MODE_SKILL			9
#define MODE_FISHING		10

//////////////////////////////////////////////////////

#define MAX_WEAPON		25
#define MAX_STATE		12

#define PEACETIME			5000

//////////////////////////////////////////////////////

#define MODE_PEACE			0
#define MODE_WAR			1
#define MODE_VEHICLE_HORSE	2
#define MODE_VEHICLE		2


const DWORD		MAGIC_CAST_SPEED		= 1500;
const DWORD		SKILL_CAST_SPEED		= 2500;

//////////////////////////////////////////////////////

namespace Character
{

	/**
	 *
	 */
	struct  SMotion
	{
		BOOL	bUse;				///< 애니메이션이 있는지의 여부
		DWORD	dwBegin;			///< 시작 프레임
		DWORD	dwEnd;				///< 끝 프레임

		IW3DAnimation* pAnimation;	///< 애니메이션
		IW3DAnimation* pWeapon;		///< 무기용 애니메이션

		WORD	wSoundFile;
		
		void	Clear()
		{
			dwBegin = 1;
			dwEnd = 0;
			pAnimation	= NULL;
			pWeapon		= NULL;
			wSoundFile	= 0;
			bUse		= FALSE;
		} //Clear

	}; //struct SMotion

	/**
	 *
	 */
	struct SAct
	{
		int			nCount;
		int			nSelect;
		SMotion		aMotion[3];

		void		Clear()
		{
			nCount = 0;
			nSelect = 0;

			for( int i = 0; i < 3 ; i++ )
			{
				aMotion[i].Clear();
			} //for
		} //if
	}; // struct SAction

	/**
	 *
	 */
	struct SMode
	{
		int			nCount;
		SAct		aAct[50];

		void		Clear()
		{
			nCount = MAX_STATE;
			for( int i = 0 ; i < 50 ; i++ )
			{
				aAct[i].Clear();
			} //for
		} //if
	}; //struct SMode

	/**
	 *
	 */
	struct SWeapon
	{
		BOOL		bUse;
		int			nCount;
		SMode		modePeace;
		SMode		modeWar;
		SMode		amodeVehicle[10];

		void		Clear()
		{
			nCount = 0;

			modePeace.Clear();
			modeWar.Clear();
			
			for( int i = 0 ; i < 10 ; i++ )
			{
				amodeVehicle[i].Clear();
			} //for
		} //if

	}; //struct SWeapon
} //namespace 

/**
 *
 */
class CCharacterAction
{
public:
	UNDER_MEMORY_MGR( CCharacterAction );

	/**
	 *
	 */
	CCharacterAction();

	/**
	 *
	 */
	~CCharacterAction();

	/**
	 *	초기화 ( 인자는 애니메이션의 종료 여부를 확인하기 위한 더미 )
	 */
	void			Init( IW3DModel** pModel, IW3DWorld* pWorld, IW3DCreature* pCreature );

	/**
	 *
	 */
	void			SetModel( IW3DModel** pModel );

	/**
	 *	성별 설정
	 */
	void			SetSex( UINT nSex );

	/**
	 *	Mode 설정
	 */
	void			SetMode( WORD wMode );

	/**
	 *
	 */
	void			SetWithShield( BOOL bUse )
	{
		m_bWithShield = bUse;
	} //SetWithShield

	/**
	 *	무기를 설정
	 */
	void			SetWeapon( WORD wWeapon );
	
	/**
	 *	Action을 얻는다.
	 */
	SAction*		GetAction();

	/**
	 * Mode에 맞게, Action 에 적용
	 */
	void			ApplyAction();

	/**
	 *	설정된 Option을 Action에 적용
	 */
	void			ChangeAction();

	/**
	 *	갱신
	 */
	void			Update( DWORD dwTick );

	/**
	 *	동작이 끝났는지의 여부
	 */
	BOOL			IsEnd()
	{
		 return m_bEndAction;
	} //IsEnd

	/** 
	 *	동작을 마지막으로 고정 시킨다.
	 */
	void			PauseAction();

	/** 
	 *	캐릭터의 시간은 이제 이곳에서 관장한다.
	 */
	DWORD			GetTime()
	{
		return	m_dwTime * m_fActionSpeed;
	} //GetTime

	
	/////////////////////////////////////////////
	// State
	/////////////////////////////////////////////

	BOOL			CanChangeState()
	{
		switch( m_wAdvanced )
		{
		case MODE_ATTACK:
		case MODE_SKILL:
			return FALSE;
		default:
			return TRUE;
		}; //switch

		return TRUE;
	} //CanChangeState
	void			Stop()			{	if( CanChangeState() ) m_wAdvanced = MODE_STOP;	}	// 정지
	void			CompulsionStop();
	void			Walk()			{	m_wAdvanced = MODE_WALK;							}	// 걷기
	void			Run()			{	if( CanChangeState() ) m_wAdvanced = MODE_RUN;			}	// 뛰기
	void			AttackWalk()	{	if( CanChangeState() ) m_wAdvanced = MODE_WALK_ATTACK;		}	// 공격하기 위해 걸어가기
	
	void			Compulsion();


	void			AttackRun()		
	{
		if( m_wAction != MODE_RUN &&  m_wAction != MODE_RUN_ATTACK ) 
		{
			m_wAdvanced = MODE_RUN_ATTACK;
		} //if
	}	// 공격하기 위해 달려가기

	void			Attack();//		{	m_wAdvanced = MODE_ATTACK;	}	// 공격	
	void			Dead();
	void			MagicCasting( BYTE	btKind, BYTE btKind2, int nRepeat = 1 );	// 마법
	void			Resurrect();
	BOOL			Skill( SAction* pAction );
	void			Hook();

	void			Lock()
	{
		m_bEndAction = FALSE;
		m_bLock	 = TRUE;
	}//	잠근다.				( 아무것도 하지 못한다. )

	void			Unlock()
	{
		m_bLock	 = FALSE;
	}//	잠금을 해제 한다.

	
	/**
	 *
	 */
	void AddTarget( IW3DCreature* pTarget );

	/**
	 *
	 */
	void RemoveTarget( IW3DCreature* pTarget );

	/**
	 *
	 */
	void ClearTargets();

	/**
	 *	타겟들의 데미지들을 출력한다.
	 */
	void ShowDamageOfTarget();
	
	/**
	 *
	 */
	void SetLongAttack( BOOL bFlag )
	{
		m_bLongAttack = bFlag;
	} //SetLongAttack

	/**
	 *
	 */
	BOOL IsLongAttack()
	{
		return m_bLongAttack;
	} //IsLongAttack

	/**
	 *
	 */
	void SetVehicleActionMgr( CVehicleAction* pActionMgr )
	{
		m_pVehicleAction = pActionMgr;

		if( m_pVehicleAction )
		{
			m_pVehicleAction->SetSex( m_nSex );
			m_pVehicleAction->SetWeapon2( m_wWeapon );
		} //if
	} //SetVehicleActionMgr

	/**
	 *
	 */
	void SetLongWeaponEffect( const char* szEffect );

	/**
	 *
	 */
	void SetTime( DWORD dwTime )
	{
		m_dwTime = dwTime;
	} //SetTime

	/**
	 *
	 */
	void SetAction( SAction*	pAction )
	{
		Assert( pAction );

		if( pAction )
		{
			m_pAction->nStart		= pAction->nStart;
			m_pAction->nEnd			= pAction->nEnd;
			m_pAction->nFrames		= pAction->nFrames;
			m_pAction->pAnimation	= pAction->pAnimation;
		} //if
	} //SetAction

	/**
	 * 한 동작이 끝났는지의 여부
	 */
	BOOL			IsEndAction();
	
	/**
	 *
	 */
	WORD			GetWeaponId( WORD wWeapon );

	/**
	 *
	 */

	WORD			GetState()
	{
			return m_wCharacterState;
	} //GetState

	BOOL			IsWeaponAction()
	{
		if( m_pMotion && m_pMotion->pWeapon )
		{
			return TRUE;
		} //if

		return FALSE;
	} //IsWeaponAction
	
	SAction*		GetWeaponAction()
	{
		return	m_pWeaponAction;
	} //GetWeaponAction

	/**
	 *
	 */
	BOOL			IsPeaceMode()
	{
		return m_bPeace;
	} //IsPeaceMode
	
	/**
	 *
	 */
	void			SetUnderWater( BOOL bUnder )
	{
		m_bUnderWater = bUnder;
	} //SetUnderWater

	/**
	 *
	 */
	void		SetAttackSpeed( DWORD dwTime )
	{
		m_dwAttackLimit = dwTime;
	} //SetAttackSpeed

	void		SetSkillSpeed( DWORD dwTime )
	{
		(*m_pModel)->SetAction( m_pAction );
		DWORD	dwTick = (*m_pModel)->GetActionTicks();
	
		m_fActionSpeed = (float)( dwTick ) / (float)( dwTime );
	} //SetSkillSpeed
	
	/**
	 *
	 */
	void			SetActionSpeed( DWORD dwTime )
	{
		if( dwTime == 0 || !(*m_pModel) )
		{
			return ;
		} //if

		DWORD	dwTicks = 0;
		//m_pMotion	= &m_pAct->aMotion[m_wLevel];
		for( int i = 0 ; i < 3 ; i++ )
		{
			m_pMotion	= &m_pAct->aMotion[i];

			if( m_pMotion->pAnimation )
			{
				SAction action;
				action.nStart		= m_pMotion->dwBegin;
				action.nEnd			= m_pMotion->dwEnd;
				action.nFrames		= m_pMotion->dwEnd - m_pMotion->dwBegin + 1;
				action.pAnimation	= m_pMotion->pAnimation;

				(*m_pModel)->SetAction( &action );
				DWORD	dwTick = (*m_pModel)->GetActionTicks();

				dwTicks += dwTick;
			} //if
		} //for

		m_pMotion	= &m_pAct->aMotion[m_wLevel];
		m_fActionSpeed = (float)( dwTicks ) / (float)( dwTime );

	/*	if( m_pModel )
		{
			m_pMotion	= &m_pAct->aMotion[m_wLevel];
			m_pMotion->
			m_pModel->SetAction( m_pAction );
			DWORD	dwTicks = m_pModel->GetActionTicks();

			m_fActionSpeed = (float)( dwTicks ) / (float)( dwTime );
		} //if*/
	} //SetActionSpeed

private:

	Character::SWeapon*		m_pWeapon;		///< 무기
	Character::SMode*		m_pMode;		///< 모드
	Character::SAct*		m_pAct;			///< 
	Character::SMotion*		m_pMotion;		///< 모션 
	SAction*				m_pAction;		///< 액션 ( 모션 데이타 )
	SAction*				m_pWeaponAction;		///< 액션 ( 모션 데이타 )

	DWORD			m_dwTick;		///< 경과 틱
	DWORD			m_dwTime;		///< 누적 틱
	DWORD			m_dwPeaceTime;	///< 전쟁인지 평화인지에 대한 모드 변경 타임
	DWORD			m_dwDeadLine;	///< 한 동작 당 이 시간을 넘길수 없다.
	DWORD			m_dwAttackLimit; ///< 이 안에 공격 동작을 끝내야만 한다.
	float			m_fActionSpeed;	///< 동작 속도

	IW3DMagic*		m_pLongWeaponEffect;	///< 장거리용 이펙트
	IW3DModel**		m_pModel;		///< 애니메이션 확용 Model
	IW3DWorld*		m_pWorld;		///< World

//	BOOL			m_bSex;			///< 성별( 0 = 남자, 1 = 여자 )
	UINT			m_nSex;			///< 0 : 남자, 1 : 여자, 2 : 운영자 
	WORD			m_wWeapon;		///< 무기 
	WORD			m_wMode;		///< 모드 ( 평화, 전쟁, 말 ... )
	BOOL			m_bPeace;		///< 평화모드인지의 여부
	WORD			m_wState;		///< 상태
	WORD			m_wLevel;		///< 레벨 ( 상, 중, 하 )
	WORD			m_wCharacterState;

	WORD			m_wAction;		///< 캐릭터 행동
	WORD			m_wAdvanced;	///< 예비 행동
	BOOL			m_bEndAction;	///< 모든 행동이 끝났는가?
	BOOL			m_bSameChange;  ///< 같은 행동을 지시 받았을때, 새로 갱신할지의 여부
	BOOL			m_bWithShield;	///< 방패 착용 여부

	BOOL			m_bLongAttack;	///< 원거리 공격인지의 여부

	BOOL			m_bLock;		///< 잠겼는지의 여부

	BYTE			m_btMagicKind;	///< 마법 종류
	BYTE			m_btMagicKind2; ///< 마법 소분

	int				m_nMagicRepeat; ///< 마법 중간 반복

	CVehicleAction*	m_pVehicleAction;	///< 탈것용 동작 관리자

	char			m_szLongWeapon[80];	///< 장거리 공격 이펙트 이름
	
	IW3DSoundObject3D* m_pSound;		///< 캐릭터 사운드	
	DWORD			m_dwSoundTick;		///< 반복동작의 제어를 위함(뛰기등)

	DWORD			m_dwStopTicks;
	BOOL			m_bAutoAction;
	BOOL			m_bUnderWater;		///< 물을 걸어갈때의 사운드 재생을 위한 boolean

	//--=-=-==-=-=-=-=-== Creature List -=-=-=--=-=-==-=-==//
//	typedef std::map<DWORD, IW3DCreature*> CreatureMap;
	typedef std::map<DWORD, SW3DDamage>	CreatureMap;
	CreatureMap		m_mapTargets;

	IW3DCreature*	m_pTarget;		///< 공격의 타겟 
	IW3DCreature*	m_pCreature;	///< 액션 쪽의 주인공...


private:

	BOOL			Update_Run();
	BOOL			Update_Stop();	
	BOOL			Update_BasicAction();
	BOOL			Update_Attack();
	BOOL			Update_AttackRun();
	BOOL			Update_RunAttack();
	BOOL			Update_WalkAttack();
	BOOL			Update_MagicCasting();
	BOOL			Update_TargetState();
	BOOL			Update_Dead();
	BOOL			Update_Skill();
	BOOL			Update_Fishing();

	BYTE			GetShieldValue();



}; //class CCharacterAction

#endif //#ifndef __CharacterAction_Header__