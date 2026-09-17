/**
 * Monster Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-09-20 2:23p $
 * @version	$Revision: 30 $
 *
 * <HR>
 * $Log: /GameClient/Src/MonsterAction.cpp $
 * 
 * 30    04-09-20 2:23p Sk8snow2
 * 
 * 29    04-09-17 12:10p Paran
 * 
 * 28    04-09-10 6:09p Sk8snow2
 * 
 * 27    04-05-25 2:53p Paran2
 * 
 * 26    04-05-25 12:59a Paran2
 * 
 * 25    04-01-06 9:51p Kjmgo
 * 동작쪽 버그 수정
 * 
 * 24    03-12-18 7:13p Paran
 * 
 * 23    03-12-16 5:05p Paran
 * 
 * 22    03-12-16 2:51a Paran
 * 
 * 21    03-12-11 11:17a Paran
 * 
 * 20    03-11-29 2:32p Paran
 * 
 * 19    03-11-29 2:30p Paran
 * FindAction 함수 추가
 * 
 * 18    03-10-23 10:04p Paran
 * 데미지 줄때 공격자 제공설정.
 * 
 * 17    03-10-18 12:04a Sckyj
 * 
 * 16    03-10-17 6:02p Sckyj
 * 
 * 15    03-10-17 11:22a Sckyj
 * sound file이 없을경우 처리 추가
 * 
 * 14    03-10-17 10:50a Sckyj
 * Monster sound 재생작업
 * 
 * 13    03-09-29 10:34p Kjmgo
 * Damage가 제대로 뜨지 않던것 수정( 서버쪽에서도 수정 요망 - 몬스터 )
 * 
 * 12    03-09-23 9:52a Kjmgo
 * 
 * 11    03-09-23 2:20a Kjmgo
 * 
 * 10    03-09-09 4:47p Kjmgo
 * 
 * 9     03-09-09 3:56p Kjmgo
 * 
 * 8     03-09-09 5:45a Kjmgo
 * 몬스터 속도 수정
 * 
 * 7     03-09-09 12:15a Kjmgo
 * 
 * 6     03-09-08 8:20p Kjmgo
 * 동작~ 작업
 * 
 * 5     03-09-07 12:57p Kjmgo
 * 
 * 4     03-09-02 8:30p Kjmgo
 * 몬스터 동작 수정
 * 
 * 3     03-09-02 5:54a Kjmgo
 * 
 * 2     03-09-02 5:49a Kjmgo
 * Monster 추가작업
 * 
 * 1     03-09-02 2:16a Kjmgo
 * <HR>
 *
 * @file	MonsterAction.cpp
 */

#include "ActionMgr.h"

/**
 *
 */
CMonsterAction::CMonsterAction()
{
	m_pMonster		= NULL;
	m_pModel		= NULL; 
	m_dwAttackLimit	= 0;
	m_dwTime		= 0;
	m_dwTick		= 0;
	m_dwPeaceTime	= 0;
	m_fActionSpeed	= 1.0F;
	m_wState		= MONSTER_STATE_STOP;
	m_wAdvanced		= MONSTER_STATE_STOP;
	m_bEndAction	= FALSE;
	m_bPeace		= FALSE;
	m_bShowDamage	= FALSE;
	m_pSound		= FALSE;
} //CMonsterAction::CMonsterAction

/**
 *
 */
CMonsterAction::~CMonsterAction()
{
	SAFE_DELETE( m_pAction );
} //CMonsterAction::~CMonsterAction

/**
 *	초기화 ( 인자는 애니메이션의 종료 여부를 확인하기 위한 더미 )
 */
void CMonsterAction::Init( IW3DModel* pModel, IW3DCreature* pCreature )
{
	Assert( pModel );
	Assert( pCreature );

	m_pCreature = pCreature;
	m_pModel = pModel;
	m_dwTick = 0;
	m_dwTime = 0;

	m_pAction = new SAction;
	m_pAction->pAnimation = NULL;
} //CMonsterAction::Init

/**
 *
 */
BOOL CMonsterAction::SetMonster( DWORD dwMonster )
{
	Assert( dwMonster );

	m_dwMonster = dwMonster;
	m_pMonster = g_pMonsterState->GetMonster( dwMonster );

	if( !m_pMonster )
	{
		Assert( !"몬스터가 없당" );
		return FALSE;
	} //if

	SetMode( TRUE );
	SetAction();
	ApplyAction();

	return TRUE;
} //CMonsterAction::SetMonster

/**
 *
 */
void CMonsterAction::SetMode( BOOL bPeace )
{
	if( !m_pMonster )
	{
		Assert( m_pMonster );
		return;
	} //if
	
	m_bPeace = bPeace;
	m_dwPeaceTime = 0;

	if( bPeace )
	{
		m_pMode = &m_pMonster->modePeace;	
	}
	else
	{
		m_pMode = &m_pMonster->modeWar;	
	} //if..else..

} //CMonsterAction::SetMode

/**
 *
 */
void CMonsterAction::SetAction()
{
	Assert( m_pMode );
	m_pMotion = &m_pMode->aAction[m_wState];
} //CMonsterAction::SetAction

/**
 * 액션 찾는다.
 */
BOOL CMonsterAction::FindAction( SAction& sAction, BYTE btPeace, BYTE btAction )
{
	if( !m_pMonster )
	{
		return FALSE;
	} //if
	if( btAction >= 9 || btAction < 0 )
	{
		Assert( !"없는 액션" );
		return FALSE;
	} //if

	Monster::SMode*	pMode = NULL;
	if( btPeace )	pMode = &m_pMonster->modePeace;	
	else			pMode = &m_pMonster->modeWar;	

	if( !pMode )
	{
		Assert( !"이상" );
		return FALSE;
	} //if
	
	Monster::SMotion*	pMotion = &pMode->aAction[btAction];
	if( !pMotion )
	{
		Assert( !"없는 모션" );
		return FALSE;
	} //if

	sAction.nStart		= pMotion->dwBegin;
	sAction.nEnd		= pMotion->dwEnd;
	sAction.nFrames		= sAction.nEnd - sAction.nStart + 1;
	sAction.pAnimation	= pMotion->pAnimation;

	return TRUE;
} //CMonsterAction::FindAction

/**
 *
 */
void CMonsterAction::ApplyAction()
{
	if( !m_pMotion || !m_pMotion->pAnimation || !m_pAction )
	{
		Assert( m_pMotion );
		Assert( m_pAction );
		return;
	} //if

	m_pAction->nStart		= m_pMotion->dwBegin;
	m_pAction->nEnd			= m_pMotion->dwEnd;
	m_pAction->nFrames		= m_pAction->nEnd - m_pAction->nStart + 1;
	m_pAction->pAnimation	= m_pMotion->pAnimation;

	if( m_pSound && m_pMotion->wSoundFile )
	{
		char szWave[40] = "\0";
		strcpy( szWave, g_pMonsterState->GetFileName( m_pMotion->wSoundFile ) );
		if( strcmp( szWave, "" ) )
		{
			m_pSound->Play3D( szWave );
		} //if
	} //if
} //CMonsterAction::ApplyAction

/**
 *
 */
void CMonsterAction::Update( DWORD dwTick )
{
	m_dwTick = dwTick;
	
	if( !m_bPeace && m_dwPeaceTime >= PEACETIME )
	{
		m_dwPeaceTime = 0;
	//	SetMode( TRUE );
	} //if

	BOOL	bEndAction = FALSE;

	switch( m_wState )
	{
	case MONSTER_STATE_STOP:
		m_dwPeaceTime += dwTick;
		StateStop();
	case MONSTER_STATE_MOVE:
		bEndAction = TRUE;
		break;
	case MONSTER_STATE_ACTION1:
	case MONSTER_STATE_ACTION2:
	case MONSTER_STATE_ACTION3:
	case MONSTER_STATE_ACTION4:
	case MONSTER_STATE_ACTION5:
	case MONSTER_STATE_ACTION6:
		StateAction();
		bEndAction = IsEndAction();
		break;
	case MONSTER_STATE_DEAD:
		bEndAction = IsEndAction();
		break;
	default:
		bEndAction = IsEndAction();
		break;
	} //switch

	if( bEndAction )
	{
		m_bShowDamage = TRUE;
		m_bEndAction = TRUE;

		if( !m_bPeace && ( m_wState >= MONSTER_STATE_ACTION1 && m_wState <= MONSTER_STATE_ACTION6 ) )
		{
			ClearTargets();
		} //if

		if( m_wState != m_wAdvanced )
		{
			m_wState		= m_wAdvanced;
			ChangeAction();
			m_bEndAction	= FALSE;
		}  //if
		else	if( MONSTER_STATE_MOVE != m_wState && MONSTER_STATE_STOP != m_wState )
		{
			PauseAction();
			m_bEndAction	= TRUE;
		} //if
	} //if

	m_dwTime += dwTick;

} //CMonsterAction::Update

/**
 *
 */
void CMonsterAction::ChangeAction()
{
	m_wState		= m_wAdvanced;

	m_dwTime		= 0;
	m_dwTick		= 0;
	m_dwPeaceTime	= 0;
	m_bEndAction	= FALSE;

	SetAction();
	ApplyAction();

	m_fActionSpeed = 1.0F;

	if( m_wState >= MONSTER_STATE_ACTION1 &&
		m_wState <= MONSTER_STATE_ACTION6
		)
	{
		SetActionSpeed( m_dwAttackLimit );		
	} //if\
	
} //CMonsterAction::ChangeAction

/**
 *
 */
BOOL CMonsterAction::IsEndAction()
{
	Assert( m_pModel );

	m_pModel->SetAction( m_pAction );

	DWORD dwTicks = m_pModel->GetActionTicks();

	if( GetTime() + ( m_dwTick * m_fActionSpeed ) >= dwTicks )
	{
		return TRUE;
	} //if

	return FALSE;
} //CMonsterAction::IsEndAction

/**
 *
 */
void CMonsterAction::StateStop()
{
	if( m_dwPeaceTime >= 5100 && m_bPeace )
	{
		m_dwPeaceTime = 0;

		/*
			현재 서버에서 정지 상태에서의 그 무언가를 보내지 않으므로,
			일단 보류...

			( 여기선... 이모티콘을 하는데 서버에서 움직이라고 하면-_-;;; )
	
		if( m_pMode->nCountPart1 )
		{
			int nRandom = random( m_pMode->nCountPart1 );
			m_wAdvanced = STATE_ACTION1 + nRandom;
		} //if
	*/
		return ;
	} //if
	
} //CMonsterAction::StateStop

/**
 *
 */
void CMonsterAction::StateAction()
{	
	if( m_bShowDamage && !m_bEndAction )
	{
		Assert( m_pModel );
		m_pModel->SetAction( m_pAction );

		DWORD dwTicks = m_pModel->GetActionTicks();
		
		if( GetTime() + ( m_dwTick * m_fActionSpeed )
			>= m_pModel->GetActionTicks() / 2 )
		{
			DEBUG_OUT( "Show Damage" );
			ShowDamageOfTarget();	
			m_bShowDamage = FALSE;
		} //if
	} //if

	if( IsEndAction() )
	{
		PauseAction();
	} //if

	// 전쟁 모드
} //CMonsterAction::StateAction

/**
 *
 */
void CMonsterAction::AddTarget( IW3DCreature* pTarget )
{
	Assert( pTarget );

	if( pTarget )
	{
		SW3DDamage* pDamage = pTarget->GetDamage();
		Assert( pDamage );
		
		SW3DDamage	damage = *pDamage;

		if( !m_mapTargets.empty() )
		{
			CreatureMap::iterator itor = m_mapTargets.find( pTarget->GetId() );

			if( itor != m_mapTargets.end() )
			{ // 있다.!!!
				m_mapTargets.erase( itor );
			} //if
		} //if

		m_mapTargets.insert( std::make_pair( pTarget->GetId(), damage ) );
	} //if
} //CMonsterAction::AddTarget

/**
 *
 */
void CMonsterAction::RemoveTarget( IW3DCreature* pTarget )
{
	if( !pTarget )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapTargets.find( pTarget->GetId() );

	if( itor != m_mapTargets.end() )
	{
		// 있다.!!!
		m_mapTargets.erase( itor );
	} //if

} //CMonsterAction::RemoveTarget

/**
 *
 */
void CMonsterAction::ClearTargets()
{
	if( m_mapTargets.empty() )
	{
		return ;
	} //if

	CreatureMap::iterator itor = m_mapTargets.begin();
	CreatureMap::iterator itorNext;

	while( itor != m_mapTargets.end() )
	{
		itorNext = itor;
		itorNext++;

		m_mapTargets.erase( itor );
		itor = itorNext;
	} //while
} //CMonsterAction::ClearTargets

/**
 *
 */
void CMonsterAction::ShowDamageOfTarget()
{
	if( m_mapTargets.empty() )
	{
		return ;
	} //if

	Assert( m_pCreature );

	CreatureMap::iterator itor = m_mapTargets.begin();

	while( itor != m_mapTargets.end() )
	{
		SW3DDamage*		pDamage = &itor->second;
		Assert( pDamage );
		IW3DCreature*	pCreature = pDamage->pCreature;

		if( pCreature )
		{
			pCreature->Damage( pDamage->wDamage, pDamage->btCritical );
			pCreature->Damage( FALSE, m_pCreature );
		} //if
		
		itor++;
	} //while
} //CMonsterAction::ShowDamageOfTarget

/**
 *
 */
void CMonsterAction::PauseAction()
{
	Assert( m_pMotion );
	Assert( m_pAction );

	m_pAction->nStart		= m_pMotion->dwEnd;
	m_pAction->nEnd			= m_pMotion->dwEnd;
	m_pAction->nFrames		= 1;
	m_pAction->pAnimation	= m_pMotion->pAnimation;

} //CMonsterAction::PauseAction

