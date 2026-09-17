/**
 * Monster Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-09-20 2:23p $
 * @version	$Revision: 18 $
 *
 * <HR>
 * $Log: /GameClient/Src/MonsterAction.h $
 * 
 * 18    04-09-20 2:23p Sk8snow2
 * 
 * 17    04-09-10 6:09p Sk8snow2
 * 
 * 16    03-12-22 5:00p Sckyj
 * 2D, 3D sound 분리
 * 
 * 15    03-12-18 7:13p Paran
 * 
 * 14    03-11-29 2:32p Paran
 * 
 * 13    03-11-29 2:30p Paran
 * FindAction 함수 추가
 * 
 * 12    03-10-23 10:04p Paran
 * 데미지 줄때 공격자 제공설정.
 * 
 * 11    03-10-17 10:50a Sckyj
 * Monster sound 재생작업
 * 
 * 10    03-10-01 11:38a Kjmgo
 * 최상위 Creature에 ShowDamageForTarget 함수 추가 및, 하위 클래스 함수
 * 추가
 * 
 * 9     03-09-29 10:34p Kjmgo
 * Damage가 제대로 뜨지 않던것 수정( 서버쪽에서도 수정 요망 - 몬스터 )
 * 
 * 8     03-09-23 2:20a Kjmgo
 * 
 * 7     03-09-10 2:42p Kjmgo
 * 
 * 6     03-09-09 4:47p Kjmgo
 * 
 * 5     03-09-09 3:56p Kjmgo
 * 
 * 4     03-09-09 12:15a Kjmgo
 * 
 * 3     03-09-02 8:30p Kjmgo
 * 몬스터 동작 수정
 * 
 * 2     03-09-02 5:49a Kjmgo
 * Monster 추가작업
 * 
 * 1     03-09-01 8:47p Kjmgo
 * 
 * <HR>
 *
 * @file	MonsterAction.h
 */

#ifndef __MonsterAction_Header__
#define __MonsterAction_Header__

#include "Global.h"


////////////////////////////////////////////////////////////////

#define		MONSTER_PEACE	0
#define		MONSTER_WAR		1
#define		MODECHANGETIME	5000

////////////////////////////////////////////////////////////////

#define		MONSTER_STATE_STOP			0		///< 정지
#define		MONSTER_STATE_MOVE			1		///< 이동 ( 평 : 걷기 , 전 : 달리기 )
#define		MONSTER_STATE_ACTION1		2		///< 행동 ( 평 : 이모티콘, 전 : 공격 )
#define		MONSTER_STATE_ACTION2		3		///< 행동 ( 평 : 이모티콘, 전 : 공격 )
#define		MONSTER_STATE_ACTION3		4		///< 행동 ( 평 : 이모티콘, 전 : 공격 )
#define		MONSTER_STATE_ACTION4		5		///< 행동 ( 평 : 이모티콘, 전 : 마공 )
#define		MONSTER_STATE_ACTION5		6		///< 행동 ( 평 : 이모티콘, 전 : 마공 )
#define		MONSTER_STATE_ACTION6		7		///< 행동 ( 평 : 이모티콘, 전 : 마공 )
#define		MONSTER_STATE_DEAD			8		///< 사망

#define		MONSTER_STATE_DAMAGE		4		///< 데미지

////////////////////////////////////////////////////////////////


namespace MonsterDummy
{
	/**
	 *
	 */
	struct  SMotion
	{
		DWORD	dwBegin;			///< 시작 프레임
		DWORD	dwEnd;				///< 끝 프레임

		WORD	wAniFile;
		WORD	wSoundFile;

		BOOL	bUse;

		void	Clear()
		{
			bUse		= FALSE;
			dwBegin		= 1;
			dwEnd		= 0;

			wAniFile	= 0;
			wSoundFile	= 0;
		} //Clear

	}; //struct SMotion

	/**
	 *
	 */
	struct SMode
	{
		int			nCountPart1;
		int			nCountPart2;
		SMotion		aAction[9];

		void		Clear()
		{
			nCountPart1 = nCountPart2 = 0;
			for( int i = 0 ; i < 9 ; i++ )
			{
				aAction[i].Clear();
			} //for
		} //if
	}; //struct SMode

	/**
	 *
	 */
	struct SMonster
	{
		int			nId;
		SMode		modePeace;
		SMode		modeWar;

		void		Clear()
		{
			nId = 0;
			modePeace.Clear();
			modeWar.Clear();
		} //Clear
	}; //struct SMonster
} //namespace

namespace Monster
{
	/**
	 *
	 */
	struct  SMotion
	{
		DWORD	dwBegin;			///< 시작 프레임
		DWORD	dwEnd;				///< 끝 프레임

		IW3DAnimation* pAnimation;
		
		WORD	wSoundFile;
		BOOL	bUse;

		void	Clear()
		{
			pAnimation	= NULL;
			bUse		= FALSE;
			dwBegin		= 1;
			dwEnd		= 0;
			wSoundFile	= 0;
		} //Clear

	}; //struct SMotion

	/**
	 *
	 */
	struct SMode
	{
		int			nCountPart1;
		int			nCountPart2;
		SMotion		aAction[9];

		void		Clear()
		{
			nCountPart1 = nCountPart2 = 0;
			for( int i = 0 ; i < 9 ; i++ )
			{
				aAction[i].Clear();
			} //for
		} //if
	}; //struct SMode

	/**
	 *
	 */
	struct SMonster
	{
		int			nId;
		SMode		modePeace;
		SMode		modeWar;

		void		Clear()
		{
			nId = 0;
			modePeace.Clear();
			modeWar.Clear();
		} //Clear
	}; //struct SMonster
} //namespace


/**
 *
 */
class CMonsterAction
{
public:
	UNDER_MEMORY_MGR( CMonsterAction );

	/**
	 *
	 */
	CMonsterAction();

	/**
	 *
	 */
	~CMonsterAction();
	
	/**
	 *	초기화 ( 인자는 애니메이션의 종료 여부를 확인하기 위한 더미 )
	 */
	void	Init( IW3DModel* pModel, IW3DCreature* pCreature );

	/**
	 *	초기화 ( 인자는 애니메이션의 종료 여부를 확인하기 위한 더미 )
	 */
	void	SetSound( IW3DSoundObject3D* pSound )
	{
		Assert( pSound );
		m_pSound = pSound;
	}

	/**
	 *
	 */
	BOOL	SetMonster( DWORD dwMonster );

	/**
	 *
	 */
	void	SetMode( BOOL bPeace );

	/**
	 *
	 */
	BOOL	FindAction( SAction& sAction, BYTE btPeace, BYTE btAction );

	/**
	 *
	 */
	void	Update( DWORD dwTick );

	/**
	 *
	 */
	BYTE	GetMode()
	{
		return m_bPeace;
	} //GetMode

	/**
	 *
	 */
	SAction*	GetAction()
	{
		Assert( m_pAction );
		return m_pAction;
	} //GetAction

	/**
	 *
	 */
	BOOL IsEnd()
	{
		return m_bEndAction;
	} //IsEnd

	/** 
	 *	동작을 마지막으로 고정 시킨다.
	 */
	void PauseAction();

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

	/////////////////////////////////////////////////////
	//	행동
	/////////////////////////////////////////////////////

	void	Attack( BYTE btAttack = 0 )		
	{
		m_bShowDamage = TRUE;
		SetMode( FALSE ); 
		m_wAdvanced = MONSTER_STATE_ACTION1; 
		m_dwPeaceTime = 0; 
		
	} //Attack

	void	Move()							{	m_wAdvanced = MONSTER_STATE_MOVE;	}
	void	Stop()							{	m_wAdvanced = MONSTER_STATE_STOP;	}
	void	Dead()							{	m_wAdvanced = MONSTER_STATE_DEAD;	}

	/**
	 *
	 */
	DWORD			GetTime()
	{
		return	m_dwTime * m_fActionSpeed;
	} //GetTime

	void		ShowDamageOfTarget();
	/**
	 *
	 */
	void		SetAttackSpeed( DWORD dwTime )
	{
		m_dwAttackLimit = dwTime;
	} //SetAttackSpeed

	/**
	 *
	 */
	void			SetActionSpeed( DWORD dwTime )
	{
		if( dwTime == 0 || !m_pModel )
		{
			return ;
		} //if

		DWORD	dwTicks = 0;

		if( m_pMotion->pAnimation )
		{
			SAction action;
			action.nStart		= m_pMotion->dwBegin;
			action.nEnd			= m_pMotion->dwEnd;
			action.nFrames		= m_pMotion->dwEnd - m_pMotion->dwBegin + 1;
			action.pAnimation	= m_pMotion->pAnimation;

			m_pModel->SetAction( &action );
			dwTicks = m_pModel->GetActionTicks();
		} //if

	//	m_pMotion	= &m_pAct->aMotion[m_wLevel];
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

	DWORD					m_dwMonster;	///< 몬스터 아이디 
	IW3DModel*				m_pModel;		///< 애니메이션의 종료 여부를 확인하기 위한 더미
	Monster::SMonster*		m_pMonster;		///< 몬스터의 모션
	SAction*				m_pAction;		///< 동작 정보
	Monster::SMode*			m_pMode;		///< 모드
	Monster::SMotion*		m_pMotion;		///< 모션..데이타

	BOOL					m_bEndAction;	///< 동작이 끝났는가?
	BOOL					m_bPeace;		///< 평화 모드인가?
	BOOL					m_bShowDamage;

	WORD					m_wState;		///< 상태 
	WORD					m_wAdvanced;	///< 예약된. 상태
	DWORD					m_dwTime;		///< 누적 틱
	DWORD					m_dwTick;		///< 경과 틱
	DWORD					m_dwPeaceTime;	///< 누적 틱 ( 평화 모드 )
	DWORD					m_dwAttackLimit; ///< 이 안에 공격 동작을 끝내야만 한다.
	float					m_fActionSpeed;	///< 동작 속도
	
	IW3DSoundObject3D*		m_pSound;		///< monster Sound

	//--=-=-==-=-=-=-=-== Creature List -=-=-=--=-=-==-=-==//
	typedef std::map<DWORD, SW3DDamage>	CreatureMap;
	CreatureMap		m_mapTargets;

	IW3DCreature*	m_pCreature;			///< 액션 쪽의 주인공...

private:

	/**
	 *
	 */
	void		ApplyAction();

	/**
	 *
	 */
	void		SetAction();

	/**
	 *
	 */
	void		ChangeAction();

	/**
	 *
	 */
	BOOL		IsEndAction();

private:

	//////////////////////////////////////////////////////
	//	State
	//////////////////////////////////////////////////////
	void		StateStop();
	void		StateAction();	


}; //class CMonsterAction


#endif //#ifndef __MonsterAction_Header__

