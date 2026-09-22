/**
 * Vehicle Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-28 3:20p $
 * @version	$Revision: 16 $
 *
 * <HR>
 * $Log: /GameClient/Src/VehicleAction.cpp $
 * 
 * 16    04-03-28 3:20p Sckyj
 * 
 * 15    04-03-12 6:41p Sckyj
 * 운영자 작업중
 * 
 * 14    04-01-07 8:12a Kjmgo
 * 
 * 13    04-01-06 9:51p Kjmgo
 * 동작쪽 버그 수정
 * 
 * 12    03-12-16 1:25a Sckyj
 * 물위에서 물발자국 소리 나도록 추가
 * 
 * 11    03-10-18 12:04a Sckyj
 * 
 * 10    03-10-17 6:02p Sckyj
 * 
 * 9     03-10-17 3:29p Sckyj
 * vehicle sound 재생 관련 코드 추가
 * 
 * 8     03-10-17 11:22a Sckyj
 * vehicle sound 관련 코드 추가
 * 
 * 7     03-10-16 4:41p Sckyj
 * sound재생관련 코드 수정
 * 
 * 6     03-10-07 9:27a Kjmgo
 * 활을 들고 말을 탔을때의 오류 수정( AddVehicle시,
 * VehicleAction->SetWeaopn();으로 하던것을 SetWeapon2()로 수정
 * 
 * 5     03-10-02 10:33p Kjmgo
 * 캐릭터 State 권한 이전 작업 : 1차 백업
 * 
 * 4     03-09-14 2:49a Kjmgo
 * 
 * 3     03-09-13 5:26p Kjmgo
 * 
 * 2     03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * 1     03-09-11 10:27p Kjmgo
 * 
 * <HR>
 *
 * @file	VehicleAction.cpp
 */

#include "ActionMgr.h"

/**
 *
 */
CVehicleAction::CVehicleAction()
{
	m_pWeapon			= NULL;
	m_pMode				= NULL;
	m_pAction			= NULL;
	m_pMotion			= NULL;
	m_pModel			= NULL;
	m_wWeapon			= WEAPON_NONE;		///< 무기 
	m_wMode				= MODE_PEACE;		///< 모드 ( 평화, 전쟁, 말 ... )
	m_wLevel			= 0;				///< 레벨 ( 상, 중, 하 )
	m_wState			= STATE_STOP;		///< 상태
	m_wAction			= MODE_STOP;
	m_wAdvanced			= MODE_STOP;

	m_bEndAction		= FALSE;
	m_bSameChange		= FALSE;
	m_bWithShield		= FALSE;
	m_dwTime			= 0;
	m_dwTick			= 0;
	m_btMagicKind		= 0;
	m_btMagicKind2		= 0;

	m_dwDeadLine		= 0;
	
} //CVehicleAction::CVehicleAction

/**
 *
 */
CVehicleAction::~CVehicleAction()
{

} //CVehicleAction::~CVehicleAction

/**
 *
 */
void CVehicleAction::Init( IW3DModel* pModel )
{
	Assert( pModel );

	m_pAction = new SAction;
	m_pModel = pModel;
} //CVehicleAction::Init

/**
 *
 */
void CVehicleAction::SetModel( IW3DModel* pModel )
{
	Assert( pModel );
	m_pModel = pModel;
} //CVehicleAction::SetModel

/**
 *
 */
void CVehicleAction::SetSex( UINT nSex )
{
	m_nSex		= nSex;

	/*
		TODO :
	*/

	m_pWeapon	= g_pVehicleState->GetWeapon( m_nSex, m_wWeapon );


	m_pMode		= &m_pWeapon->aMode[m_wMode];
	m_pAct		= &m_pMode->aAct[m_wState];
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	ApplyAction();
} //CVehicleAction::SetSex

/**
 *	Mode 설정
 */
void CVehicleAction::SetMode( WORD wMode )
{
	m_wMode		= GetVehicleType( wMode );
	m_pMode		= &m_pWeapon->aMode[m_wMode];
	m_pAct		= &m_pMode->aAct[m_wState];
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	ApplyAction();
} // CVehicleAction::SetMode

/**
 *	무기를 설정
 */
void CVehicleAction::SetWeapon( WORD wWeapon )
{
	m_wWeapon = GetWeaponId( wWeapon );

	if( m_wWeapon > 3000 )
	{
		return ;
	} //if

	/*
		TODO :
	*/
	if( m_wWeapon != WEAPON_SHORTBOW )
	{
		m_wWeapon = 0;
	} //if

	m_pWeapon	= g_pVehicleState->GetWeapon( m_nSex, m_wWeapon );
	m_pMode		= &m_pWeapon->aMode[m_wMode];
	m_pAct		= &m_pMode->aAct[m_wState];
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	ApplyAction();
} //CVehicleAction::SetWeapon

/**
 *	무기를 설정
 */
void CVehicleAction::SetWeapon2( WORD wWeapon )
{
	m_wWeapon = wWeapon;

	if( m_wWeapon > 3000 )
	{
		return ;
	} //if

	/*
		TODO :
	*/
	if( m_wWeapon != WEAPON_SHORTBOW )
	{
		m_wWeapon = 0;
	} //if


	m_pWeapon	= g_pVehicleState->GetWeapon( m_nSex, m_wWeapon );
	m_pMode		= &m_pWeapon->aMode[m_wMode];
	m_pAct		= &m_pMode->aAct[m_wState];
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	ApplyAction();
} //CVehicleAction::SetWeapon

/**
 *
 */
void CVehicleAction::SetLevel( BYTE btLevel )
{
	m_wLevel	= btLevel;
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	m_dwTick = 0;

	ApplyAction();
} //CVehicleAction::SetLevel

/**
 *	Action을 얻는다.
 */
SAction* CVehicleAction::GetAction()
{
	Assert( m_pAction );
	return m_pAction;
} //CVehicleAction::GetAction
 
/**
 * Mode에 맞게, Action 에 적용
 */
void CVehicleAction::ApplyAction()
{
	Assert( m_pMotion );

	m_pAction->nStart		= m_pMotion->dwBegin;
	m_pAction->nEnd			= m_pMotion->dwEnd;
	m_pAction->nFrames		= m_pMotion->dwEnd - m_pMotion->dwBegin + 1;
	m_pAction->pAnimation	= m_pMotion->pAnimation;
	
	// STATE_RUN의 "중"동작을 제외한 사운드에 대한 재생
	if( m_pSound && m_pMotion->wSoundFile )
	{
		if( m_wState != STATE_RUN )
		{
			char szWave[40] = "\0";
			strcpy( szWave, g_pVehicleState->GetFileName( m_pMotion->wSoundFile ) );
			if( strcmp( szWave, "" ) )
			{
				m_pSound->Play3D( szWave );
			}
		}	
		else
		if( m_wLevel != 1 )
		{
			char szWave[40] = "\0";
			strcpy( szWave, g_pVehicleState->GetFileName( m_pMotion->wSoundFile ) );
			if( strcmp( szWave, "" ) )
			{
				m_pSound->Play3D( szWave );
			}
		}
	}
} //CVehicleAction::ApplyAction

/**
 *	설정된 Option을 Action에 적용
 */
void CVehicleAction::ChangeAction()
{
	m_wLevel = 0;
	m_dwTick = 0;
	m_bSameChange = FALSE;

	m_bEndAction	= FALSE;
	m_pAct			= &m_pMode->aAct[m_wState];
	m_pMotion		= &m_pAct->aMotion[m_wLevel];

	ApplyAction();
} //CVehicleAction::ChangeAction

/**
 *	갱신
 */
void CVehicleAction::Update( DWORD dwTick )
{
	m_dwTick = dwTick;
	m_dwSoundTick += dwTick;

	if( m_wState != STATE_STOP	&&
		m_wState != STATE_WALK	&&
		m_wState != STATE_RUN )
	{
		if( IsEndAction() )
		{
			m_pAction->nStart		= m_pMotion->dwEnd;
			m_pAction->nEnd			= m_pMotion->dwEnd;
			m_pAction->nFrames		= 1;
			m_pAction->pAnimation	= m_pMotion->pAnimation;
		}
	}

	// STATE_RUN의 "중"동작 사운드 재생
	if(	m_pSound										&&
		m_dwSoundTick >= m_pModel[0].GetActionTicks()	&&
		( m_wLevel == 1 || ( m_wState == STATE_WALK && m_wLevel == 0 ) )
		) 
	{
		if( !m_bUnderWater )
		{
			if( m_pMotion->wSoundFile )
			{
				char szWave[40] = "\0";
				strcpy( szWave, g_pVehicleState->GetFileName( m_pMotion->wSoundFile ) );
				if( strcmp( szWave, "" ) )
				{
					m_pSound->Play3D( szWave );
				}
				m_dwSoundTick = 0;
			}
		}
		else
		{
			m_pSound->Play3D( "etc_nul_0012.wav" );
			m_dwSoundTick = 0;
		}
	}

//	m_dwTime += m_dwTick;
} //CVehicleAction::Update

/**
 *
 */
WORD CVehicleAction::GetWeaponId( WORD wWeapon )
{
/*	switch( wWeapon )
	{
	case dITEMTYPE_NONE:
		return WEAPON_NONE;
	case dITEMTYPE_ONEHANDSWORD:
		return WEAPON_ONEHANDSWORD;
	case dITEMTYPE_TWOHANDSWORD:
		return WEAPON_TWOHANDSWORD;
	case dITEMTYPE_DUALSWORD:				
		return WEAPON_DUALSWORD;
	case dITEMTYPE_ONEHANDAXE:	
		return WEAPON_ONEHANDAXE;
	case dITEMTYPE_TWOHANDAXE:	
		return WEAPON_TWOHANDAXE;
	case dITEMTYPE_DUALAXE:		
		return WEAPON_DUALAXE;
	case dITEMTYPE_ONEHANDBLUNT:
		return WEAPON_ONEHANDBLUNT;
	case dITEMTYPE_TWOHANDBLUNT:
		return WEAPON_TWOHANDBLUNT;
	case dITEMTYPE_DUALBLUNT:	
		return WEAPON_DUALBLUNT;
	case dITEMTYPE_DONPA:		
		return WEAPON_DONPA;
	case dITEMTYPE_SHORTBOW:				
		return WEAPON_SHORTBOW;
	case dITEMTYPE_LONGBOW:	
		return WEAPON_LONGBOW;
	case dITEMTYPE_DDABALBOW:
		return WEAPON_DDABALBOW;
	case dITEMTYPE_CROSSBOW:	
		return WEAPON_CROSSBOW;
	case dITEMTYPE_SPHERE:		
		return WEAPON_SPHERE;
	case dITEMTYPE_BONG:	
		return WEAPON_BONG;
	case dITEMTYPE_STAFF:
		return WEAPON_STAFF;
	}; //switch

	return WEAPON_NONE;*/

	switch( wWeapon )
	{
	case dITEMTYPE_NONE:
		return WEAPON_NONE;
	
	case dITEMTYPE_ONEHANDSWORD:
		return WEAPON_ONEHANDSWORD;
	case dITEMTYPE_TWOHANDSWORD:
		return WEAPON_TWOHANDSWORD;
	case dITEMTYPE_DUALSWORD:				
		return WEAPON_DUALSWORD;
	/*
		TODO : 추후에 다시 WEAPON_AXE로 바꾸어야 한다.
	*/
	case dITEMTYPE_ONEHANDAXE:	
		return WEAPON_ONEHANDSWORD;
	case dITEMTYPE_TWOHANDAXE:	
		return WEAPON_TWOHANDSWORD;

	case dITEMTYPE_DUALAXE:		
		return WEAPON_DUALSWORD;
	/*
		TODO : 추후에 다시 WEAPON_BLUNT로 바꾸어야 한다.
	*/
	case dITEMTYPE_ONEHANDBLUNT:
		return WEAPON_ONEHANDSWORD;
	case dITEMTYPE_TWOHANDBLUNT:
		return WEAPON_TWOHANDSWORD;
	case dITEMTYPE_DUALBLUNT:	
		return WEAPON_DUALSWORD;

	case dITEMTYPE_DONPA:		
		return WEAPON_DONPA;
	case dITEMTYPE_SHORTBOW:				
		return WEAPON_SHORTBOW;

	/*
		TODO : 추후에 다시 WEAPON_LONGBOW로 바꾸어야 한다.
	*/
	case dITEMTYPE_LONGBOW:	
		return WEAPON_SHORTBOW;


	case dITEMTYPE_DDABALBOW:
		return WEAPON_DDABALBOW;
	case dITEMTYPE_CROSSBOW:	
		return WEAPON_CROSSBOW;

	/*
		TODO : 추후에 다시 WEAPON_SPHERE로 바꾸어야 한다.
	*/
	case dITEMTYPE_SPHERE:	
		return WEAPON_SPHERE;
		//return WEAPON_STAFF;
	/*
		TODO : 추후에 다시 WEAOPN_BONG로 바꾸어야 한다.
	*/
	case dITEMTYPE_BONG:	
		return WEAPON_SPHERE;


	case dITEMTYPE_STAFF:
		return WEAPON_STAFF;

	case dITEMTYPE_FISHTACKLE :
		return WEAPON_FISHINGTACKLE;
	}; //switch

	return WEAPON_NONE;
} //CVehicleAction::GetWeaponId

/**
 *
 */
WORD CVehicleAction::GetVehicleType( WORD wId )
{
	switch( wId )
	{
	case 0:	case 1:	case 2:	case 3:
		return 1;
	case 4:
		return 1;
	} //switch

	return 1;
} //CVehicleAction::GetVehicleType

/**
 *
 */
void CVehicleAction::SetState( WORD wState )
{
	m_wLevel = 0;
	m_dwTick = 0;
	m_bSameChange = FALSE;

	m_wState = wState;

	m_bEndAction	= FALSE;
	m_pAct			= &m_pMode->aAct[m_wState];
	m_pMotion		= &m_pAct->aMotion[m_wLevel];

	ApplyAction();
} //CVehicleAction::SetState

/**
 *
 */
BOOL CVehicleAction::IsEndAction()
{
	Assert( m_pModel );
	m_pModel->SetAction( m_pAction );

	if( m_dwTime + m_dwTick >= m_pModel->GetActionTicks() )
	{
		return TRUE;
	} //if

	return FALSE;
} //CVehicleAction::IsEndAction