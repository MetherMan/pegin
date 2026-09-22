/**
 * Character Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-09-14 5:57p $
 * @version	$Revision: 74 $
 *
 * <HR>
 * $Log: /GameClient/Src/CharacterAction.cpp $
 * 
 * 74    04-09-14 5:57p Sk8snow2
 * 
 * 73    04-09-10 10:28p Sk8snow2
 * 
 * 72    04-09-10 6:09p Sk8snow2
 * 
 * 71    04-06-11 5:33p Sckyj
 * Update_MagicCasting()
 * 후 동작 종료후 m_wAdvance = MODE_STOP으로 수정
 * 
 * 70    04-06-11 5:24p Sckyj
 * 
 * 69    04-06-04 4:42p Sckyj
 * 
 * 68    04-06-04 4:29p Sckyj
 * 
 * 67    04-03-28 7:52p Sckyj
 * 
 * 66    04-03-28 3:20p Sckyj
 * 
 * 65    04-03-27 10:33a Sckyj
 * 
 * 64    04-03-12 6:41p Sckyj
 * 운영자 작업중
 * 
 * 63    04-02-02 4:47p Kjmgo
 * 
 * 62    04-01-30 5:46p Kjmgo
 * 
 * 61    04-01-06 9:51p Kjmgo
 * 동작쪽 버그 수정
 * 
 * 60    04-01-06 9:59a Kjmgo
 * IsEndAction 에서 버그 수정( 동작이 깨질 염려가 있는 코드 )
 * 
 * 59    04-01-06 12:01a Kjmgo
 * 공속 추가 
 * 
 * 58    04-01-02 7:26p Kjmgo
 * 메인스킬이 마법일경우 특화스킬 사용 불가능 하도록 수정
 * 
 * 57    03-12-26 9:47a Paran
 * 
 * 56    03-12-24 10:30a Kjmgo
 * 스킬적용
 * 
 * 55    03-12-23 9:10p Kjmgo
 * 쓰지 않는 무기에 대한 애니메이션은 생성하지 아니하도록 수정
 * 
 * 54    03-12-20 2:38a Kjmgo
 * 
 * 53    03-12-19 9:28p Kjmgo
 * 
 * 52    03-12-19 8:55a Kjmgo
 * 동작 합체및, 분리( 창은 불리, 검류와 도끼류 합체, 단,장궁 합체 )
 * 
 * 51    03-12-19 6:23a Kjmgo
 * 석궁일때, 처리
 * 
 * 50    03-12-16 1:25a Sckyj
 * 물위에서 물발자국 소리 나도록 추가
 * 
 * 49    03-12-13 3:17p Kjmgo
 * 마법 동작 버그 수정( 전, 중, 후 중에 TIme을 그대로 누적하던것을 각
 * 파트가 끝날때 0으로 초기화 )
 * 
 * 48    03-10-23 10:04p Paran
 * 데미지 줄때 공격자 제공설정.
 * 
 * 47    03-10-18 12:04a Sckyj
 * 
 * 46    03-10-17 6:02p Sckyj
 * 
 * 45    03-10-17 3:30p Sckyj
 * character sound 재생 관련 코드 수정
 * 
 * 44    03-10-17 11:23a Sckyj
 * sound file이 없을경우의 처리 추가( wSoundFile 이 0일경우 무시 )
 * 
 * 43    03-10-16 4:41p Sckyj
 * sound재생관련 코드 수정
 * 
 * 42    03-10-11 12:08a Kjmgo
 * AddMagic수정 및, CCreature::~CCreature(); 쓰던거 삭제
 * 
 * 41    03-10-10 7:25p Kjmgo
 * 마법 관련해서 수정
 * 
 * 40    03-10-07 9:27a Kjmgo
 * 활을 들고 말을 탔을때의 오류 수정( AddVehicle시,
 * VehicleAction->SetWeaopn();으로 하던것을 SetWeapon2()로 수정
 * 
 * 39    03-10-04 2:02a Kjmgo
 * 동작 버그 잡는중 : 40%
 * 
 * 38    03-10-04 1:18a Kjmgo
 * 캐릭터 공격 부분 수정 및, 근거리 스킬이 아닐경우, 근거리 아이템을
 * 바꾸면 무기 동작이 안바뀌던것 수정( 원거리가 아닐경우 바꾸기로.)
 * 
 * 37    03-10-04 12:22a Kjmgo
 * 
 * 36    03-10-03 11:49p Kjmgo
 * 몬스터 피킹 거리 15미터에서 25미터로 수정, 창, 봉, 동작을 지팡이로
 * 대체, 몬스터 공격거리를 종전거리로 재수정
 * 
 * 35    03-10-03 2:38a Kjmgo
 * 
 * 34    03-10-02 11:20p Kjmgo
 * 캐릭터 State 권한 이전 : 완료 
 * 
 * 33    03-10-02 10:33p Kjmgo
 * 캐릭터 State 권한 이전 작업 : 1차 백업
 * 
 * 32    03-10-02 2:41p Kjmgo
 * 몬스터 공격 거리를 몬스터의 Radius만큼의 여유를 줌
 * 
 * 31    03-10-01 8:40p Kjmgo
 * 말탔을때 말은 보이지 않고 타는 동작만 나오던 버그 수정
 * 
 * 30    03-10-01 6:40p Kjmgo
 * Skill적용
 * 
 * 29    03-10-01 2:14p Kjmgo
 * 마법이 끝나고 정지하던것을 수정
 * 
 * 28    03-10-01 1:39a Kjmgo
 * 
 * 27    03-09-30 10:22p Kjmgo
 * 
 * 26    03-09-29 10:34p Kjmgo
 * Damage가 제대로 뜨지 않던것 수정( 서버쪽에서도 수정 요망 - 몬스터 )
 * 
 * 25    03-09-29 11:48a Kjmgo
 * 
 * 24    03-09-23 9:38p Kjmgo
 * Skill 관련 부분 작업
 * 
 * 23    03-09-23 3:59p Paran
 * 
 * 22    03-09-23 10:48a Kjmgo
 * 
 * 21    03-09-23 9:52a Kjmgo
 * 
 * 20    03-09-23 2:57a Kjmgo
 * 
 * 19    03-09-23 2:49a Paran
 * 로냐: Dead 처리 
 * 
 * 18    03-09-23 2:31a Kjmgo
 * 
 * 17    03-09-23 2:20a Kjmgo
 * 
 * 16    03-09-18 10:58a Kjmgo
 * 데미지 처리 관련 수정
 * 
 * 15    03-09-17 11:00a Kjmgo
 * 마법 시전 부분 약간 수정( 동작이 중복되서 나가는거 수정 )
 * 
 * 14    03-09-13 3:55p Kjmgo
 * 
 * 13    03-09-13 1:26p Kjmgo
 * 활을 비롯한 몇가지 옵션 추가
 * 
 * 12    03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * 11    03-09-10 2:30a Kjmgo
 * 
 * 10    03-09-09 5:19p Kjmgo
 * 
 * 9     03-09-09 3:56p Kjmgo
 * 
 * 8     03-09-09 12:15a Kjmgo
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
 * 4     03-08-31 10:19p Kjmgo
 * Chracter 이동과 공격 수정 
 * 
 * 3     03-08-31 6:18p Kjmgo
 * Character Action 관련 추가
 * <HR>
 *
 * @file	CharacterAction.cpp
 */

#include "ActionMgr.h"

/**
 *
 */
CCharacterAction::CCharacterAction()
{
	Assert( g_pCharacterState );

	m_pWeapon			= NULL;
	m_pMode				= NULL;
	m_pAction			= NULL;
	m_pTarget			= NULL;
	m_pMotion			= NULL;
	m_pModel			= NULL;
	m_pWorld			= NULL;
	m_pVehicleAction	= NULL;
	m_pLongWeaponEffect = NULL;
	m_pCreature			= NULL;
	m_pWeaponAction		= NULL;
	m_wWeapon			= WEAPON_NONE;		///< 무기 
	m_wMode				= MODE_PEACE;		///< 모드 ( 평화, 전쟁, 말 ... )
	m_wLevel			= 0;				///< 레벨 ( 상, 중, 하 )
	m_wState			= CHARACTER_STATE_STOP;		///< 상태
	m_wAction			= MODE_STOP;
	m_wAdvanced			= MODE_STOP;

	m_bEndAction		= FALSE;
	m_bSameChange		= FALSE;
	m_bWithShield		= FALSE;
	m_bLongAttack		= FALSE;
	m_bPeace			= TRUE;
	m_bAutoAction		= FALSE;
	m_bLock				= FALSE;
	m_dwTime			= 0;
	m_dwStopTicks		= 0;
	m_dwPeaceTime		= 0;
	m_dwAttackLimit		= 0;
	m_dwTick			= 0;
	m_btMagicKind		= 0;
	m_btMagicKind2		= 0;

	m_fActionSpeed		= 1.0F;
	m_dwDeadLine		= 0;
	
	sprintf( m_szLongWeapon, "기본화살" );
} //CCharacterAction::CCharacterAction

/**
 *
 */
CCharacterAction::~CCharacterAction()
{
	SAFE_DELETE( m_pAction );
	SAFE_DELETE( m_pWeaponAction );
} //CCharacterAction::~CCharacterAction

/**
 *
 */
void CCharacterAction::Init( IW3DModel** pModel, IW3DWorld* pWorld, IW3DCreature* pCreature )
{
	Assert( pModel );

	m_pAction = new SAction;
	m_pWeaponAction = new SAction;

	m_pAction->pAnimation = NULL;
	m_pWeaponAction->pAnimation = NULL;

	m_pModel	= pModel;
	m_pWorld	= pWorld;
	m_pCreature = pCreature;
	
	m_pSound	= pCreature->GetSound();

} //CCharacterAction::Init

/**
 *
 */
void CCharacterAction::SetModel( IW3DModel** pModel )
{
//	Assert( pModel );
	m_pModel = pModel;
} //CCharacterAction::SetModel

/**
 *
 */
void CCharacterAction::SetSex( UINT nSex )
{
	m_nSex		= nSex;
	m_pWeapon	= g_pCharacterState->GetWeapon( m_nSex, m_wWeapon );
	Assert( m_pWeapon );

	if( !m_pWeapon )
	{
		return ;
	} //if

	switch( m_wMode )
	{
	case MODE_PEACE:
		m_pMode	= &m_pWeapon->modePeace;
		break;
	case MODE_WAR:
		m_pMode	= &m_pWeapon->modeWar;
		break;
	default:
		// 평화와 전쟁 모드 이외는 탈 것이다.
		// -2 를 해준다.
		m_pMode	= &m_pWeapon->amodeVehicle[m_wMode - 2];
	}; //switch

	m_pAct		= &m_pMode->aAct[m_wState];
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	if( m_pVehicleAction )
	{
		m_pVehicleAction->SetSex( nSex );
	} //if

	ApplyAction();
	
} //CCharacterAction::SetSex

/**
 *
 */
void CCharacterAction::SetMode( WORD wMode )
{
	if( wMode == MODE_VEHICLE + 4 ) wMode = MODE_VEHICLE + 1;
	/*
		TODO : 현재는 임시로, 탈것은 맨손 동작만 쓴다.
	*/

	m_pWeapon	= g_pCharacterState->GetWeapon( m_nSex, m_wWeapon );

	if( wMode > MODE_WAR )
	{
		m_pWeapon	= g_pCharacterState->GetWeapon( m_nSex, 0 );

		if( WEAPON_SHORTBOW == m_wWeapon )
		{
			m_pWeapon	= g_pCharacterState->GetWeapon( m_nSex, WEAPON_SHORTBOW );
		} //if
	} //if
//////////////////////////////////////////////////////////////

	Assert( m_pWeapon );

	m_wMode = wMode;
	switch( m_wMode )
	{
	case MODE_PEACE:
		m_pMode	= &m_pWeapon->modePeace;
		break;
	case MODE_WAR:
		m_pMode	= &m_pWeapon->modeWar;
		break;
	default:
		// 평화와 전쟁 모드 이외는 탈 것이다.
		// -2 를 해준다.
		m_pMode	= &m_pWeapon->amodeVehicle[m_wMode - 2];
	}; //switch

	m_pAct		= &m_pMode->aAct[m_wState];
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	ApplyAction();
} //CCharacterAction::SetMode

/**
 *
 */
void CCharacterAction::SetWeapon( WORD wWeapon )
{
	m_wWeapon = GetWeaponId( wWeapon );


	if( m_wWeapon > 3000 )
	{
		return ; 
	} //if

	/*
		TODO : 현재는 임시로, 탈것은 맨손 동작만 쓴다.
	*/

	m_pWeapon	= g_pCharacterState->GetWeapon( m_nSex, m_wWeapon );

	if( m_wMode > MODE_WAR )
	{
		m_pWeapon	= g_pCharacterState->GetWeapon( m_nSex, 0 );

		if( WEAPON_SHORTBOW == m_wWeapon )
		{
			m_pWeapon	= g_pCharacterState->GetWeapon( m_nSex, WEAPON_SHORTBOW );
		} //if
	} //if

	Assert( m_pWeapon );

	switch( m_wMode )
	{
	case MODE_PEACE:
		m_pMode	= &m_pWeapon->modePeace;
		break;
	case MODE_WAR:
		m_pMode	= &m_pWeapon->modeWar;
		break;
	default:
		// 평화와 전쟁 모드 이외는 탈 것이다.
		// -2 를 해준다.
		m_pMode	= &m_pWeapon->amodeVehicle[m_wMode - 2];
	}; //switch

	m_pAct		= &m_pMode->aAct[m_wState];
	m_pMotion	= &m_pAct->aMotion[m_wLevel];

	ApplyAction();

	if( m_pVehicleAction ) 
	{
		m_pVehicleAction->SetWeapon( wWeapon );
	} //if

} //CCharacterAction::SetWeapon

/**
 *
 */
SAction* CCharacterAction::GetAction()
{
	Assert( m_pAction );
	return m_pAction;
} //CCharacterAction::GetAction

/**
 *
 */
void CCharacterAction::ApplyAction()
{
	Assert( m_pMotion );
	Assert( m_pAction );

	m_pAction->nStart		= m_pMotion->dwBegin;
	m_pAction->nEnd			= m_pMotion->dwEnd;
	m_pAction->nFrames		= m_pMotion->dwEnd - m_pMotion->dwBegin + 1;
	m_pAction->pAnimation	= m_pMotion->pAnimation;

	*m_pWeaponAction		= *m_pAction;
	m_pWeaponAction->pAnimation	= m_pMotion->pWeapon;

	// STATE_RUN의 "중"동작을 제외한 나머지 동작에 대한 사운드 재생
	if( m_pSound && m_pMotion->wSoundFile )
	{
		if( m_wAction != STATE_RUN )
		{
			char szWave[40] = "\0";
			strcpy( szWave, g_pCharacterState->GetFileName( m_pMotion->wSoundFile ) );
			if( strcmp( szWave, "" ) )
			{
				m_pSound->Play3D( szWave );
			} //if
		}		
		else if( m_wLevel != 1)	// STATE_RUN의 "중"동작 제외
		{
			char szWave[40] = "\0";
			strcpy( szWave, g_pCharacterState->GetFileName( m_pMotion->wSoundFile ) );
			if( strcmp( szWave, "" ) )
			{
				m_pSound->Play3D( szWave );
			} //if 
		} //if..else..
	} //if
	
	if( m_pVehicleAction )
	{
		m_pVehicleAction->SetLevel( m_wLevel );
	} //if
} //CCharacterAction::ApplyAction

/**
 *
 */
void CCharacterAction::PauseAction()
{
	Assert( m_pMotion );
	Assert( m_pAction );

	m_pAction->nStart		= m_pMotion->dwEnd;
	m_pAction->nEnd			= m_pMotion->dwEnd;
	m_pAction->nFrames		= 1;
	m_pAction->pAnimation	= m_pMotion->pAnimation;

} //CCharacterAction::PauseAction

/**
 *
 */
void CCharacterAction::ChangeAction()
{
	Assert( m_pCreature );
	
	m_wLevel = 0;
	m_dwTime = 0;
	m_dwTick = 0;
	m_dwStopTicks = 0;
	m_dwDeadLine = 0;
	m_bSameChange = FALSE;

	switch( m_wAction )
	{
	case MODE_STOP:
		m_wState	=	CHARACTER_STATE_STOP;
		m_pCreature->SetState( STATE_STOP );
		break;
	case MODE_WALK:
		m_wState	=	CHARACTER_STATE_WALK;
		m_pCreature->SetState( STATE_MOVE );
		break;
	case MODE_RUN:
		m_wState	=	CHARACTER_STATE_RUN;
		m_pCreature->SetState( STATE_MOVE );
		break;
	case MODE_DEAD:
		m_wState	=	CHARACTER_STATE_DIE;
		m_pCreature->SetState( STATE_DEAD );
		break;
	case MODE_SKILL:
		m_pCreature->SetState( STATE_SKILL );
		break;
	case MODE_ATTACK:
		{
			int nRandom = random( 4 );
			
			if( m_wMode == MODE_PEACE )
			{
				m_dwPeaceTime = 0;
				m_wMode = MODE_WAR;
				SetMode( MODE_WAR );
			} //if

			m_wState	=	CHARACTER_STATE_ATTACK1 + nRandom;
		} // case MODE_ATTACK
		m_pCreature->SetState( STATE_ATTACK );		
		break;
	case MODE_WALK_ATTACK:
		if( m_wMode == MODE_PEACE )
		{
			m_wState	=	CHARACTER_STATE_WALK_MODECHANGE;
		}
		else
		{
			m_wState	=	CHARACTER_STATE_WALK;
		} //if..else..
		m_pCreature->SetState( STATE_ATTACK );
		break;
	case MODE_RUN_ATTACK:
		if( m_wMode == MODE_PEACE )
		{
			m_wState	=	CHARACTER_STATE_RUN_MODECHANGE;
		}
		else
		{
			m_wState	=	CHARACTER_STATE_RUN;
		} //if..else..
		m_pCreature->SetState( STATE_ATTACK );
		break;
	case MODE_ATTACK_RUN:
		{
			int nRandom = random( 1 );
			
			if( m_wMode == MODE_PEACE )
			{
				m_dwPeaceTime = 0;
				m_wMode = MODE_WAR;
				SetMode( MODE_WAR );
			} //if

			ShowDamageOfTarget();
			ClearTargets();
			
			m_wState	= CHARACTER_STATE_RUN_ATTACK1 +  nRandom;
		} // case MODE_ATTACK_RUN
		m_pCreature->SetState( STATE_MOVE );
		break;
	case MODE_MAGIC_CASTING:
		{
			//if( m_wMode == MODE_PEACE )
			{
				m_dwPeaceTime = 0;
				m_wMode = MODE_WAR;
				SetMode( MODE_WAR );
			} //if
			m_wState	=	STATE_MAGIC_ATTACK_UPANDDOWN + ( m_btMagicKind * 4 + m_btMagicKind2 );
			m_pCreature->SetState( STATE_MAGIC );
		} //
		break;
	case MODE_FISHING :
		{
			m_wState	=	CHARACTER_STATE_FISHING;
			m_pCreature->SetState( STATE_FISHING );
		}
		break;
	}; //switch



	m_bEndAction	= FALSE;
	m_pAct			= &m_pMode->aAct[m_wState];
	m_pMotion		= &m_pAct->aMotion[m_wLevel];

	ApplyAction();

	if( m_pVehicleAction )
	{
		m_pVehicleAction->SetState( m_wState );
	} //if
	
	if( m_wAction != MODE_ATTACK )
	{
		m_fActionSpeed = 1.0F;
	} 
	else
	{
		SetActionSpeed( m_dwAttackLimit );
	} //if
	
	switch( m_wAction )
	{
	case MODE_ATTACK:
		SetActionSpeed( m_dwAttackLimit );
		break;
	case MODE_MAGIC_CASTING:
		SetActionSpeed( MAGIC_CAST_SPEED );
		break;
	case MODE_SKILL:
		SetSkillSpeed( SKILL_CAST_SPEED );
		break;
	}; //switch


} //CCharacterAction::ChangeAction

/**
 *
 */
BOOL CCharacterAction::IsEndAction()
{
	if( !(*m_pModel) )
	{
		//Assert( m_pModel );
		return FALSE;
	} //if

	(*m_pModel)->SetAction( m_pAction );

	DWORD	dwTicks = (*m_pModel)->GetActionTicks();

	if( GetTime() + ( m_dwTick * m_fActionSpeed ) >= dwTicks )
	{
		return TRUE;
	} //if

	return FALSE;
} //CCharacterAction::IsEndAction

/**
 *
 */
void CCharacterAction::Update( DWORD dwTick )
{
	m_dwTick = dwTick;
	
	if( m_bLock )
	{		
		m_dwTime += dwTick;
		return ;
	} //uf

	BOOL bEndAction = FALSE;		// 동작이 끝났는지의 여부

	if( m_wMode == MODE_WAR && m_dwPeaceTime >= PEACETIME && m_wMode <= MODE_WAR )
	{
		m_dwPeaceTime = 0;
		SetMode( MODE_PEACE );
	} //if

	switch( m_wAction )
	{
	case MODE_STOP:
		m_wCharacterState = STATE_STOP;
		Update_Stop();		
	case MODE_WALK:
		bEndAction = TRUE;
		m_dwPeaceTime += dwTick;
		break;
	case MODE_RUN:
		bEndAction = Update_Run();
		m_wCharacterState = STATE_MOVE;
		break;
	case MODE_ATTACK:
		bEndAction = Update_Attack();
		m_wCharacterState = STATE_ATTACK;
		break;
	case MODE_WALK_ATTACK:
		bEndAction = Update_WalkAttack();
		m_wCharacterState = STATE_ATTACK;
		break;
	case MODE_RUN_ATTACK:
		bEndAction = Update_RunAttack();
		m_wCharacterState = STATE_ATTACK;
		break;
	case MODE_ATTACK_RUN:
		bEndAction = Update_BasicAction();
		m_wCharacterState = STATE_MOVE;
		break;
	case MODE_MAGIC_CASTING:
		m_wCharacterState = STATE_MAGIC;
		//bEndAction = Update_MagicCasting();
		m_bEndAction = Update_MagicCasting();
		break;
	case MODE_SKILL:
		bEndAction = Update_Skill();
		break;
	case MODE_DEAD:
		bEndAction = Update_Dead();
		m_wCharacterState = STATE_DEAD;
		break;
	case MODE_FISHING :
		bEndAction = Update_Fishing();
		m_wCharacterState = STATE_FISHING;
	}; //switch

	if( bEndAction )
	{
		//Stop();
		if( m_wAction != m_wAdvanced )
		{
			m_wAction		= m_wAdvanced;
			ChangeAction();
			m_bEndAction	= FALSE;
		} //if		
	} //if

	m_dwTime += dwTick;
	m_dwDeadLine += dwTick;
	m_dwSoundTick += dwTick;

} //CCharacterAction::Update

void CCharacterAction::Compulsion()
{
	if( m_wAction != m_wAdvanced )
	{
		m_wAction		= m_wAdvanced;
		ChangeAction();
		m_bEndAction	= FALSE;
	} //if		
} //CCharacterAction::Compulsion

/**
 *
 */
BOOL CCharacterAction::Update_Stop()
{
	m_dwStopTicks += m_dwTick;
/*
	if( m_dwStopTicks >= 15 * 1000 )
	{		
		if( m_bAutoAction )
		{
			if( IsEndAction() )
			{
				SetMode( m_wMode );
				m_wAction = MODE_STOP;
				ChangeAction();
				m_bAutoAction = FALSE;
				m_dwStopTicks = 0;
			} //if			
		}
		else
		{
			m_bAutoAction = TRUE;
		
			m_pWeapon	= g_pCharacterState->GetWeapon( m_bSex, 0 );
			m_pMode		= &m_pWeapon->modePeace;
			m_pAct		= &m_pMode->aAct[STATE_ETC];
			m_pMotion	= &m_pAct->aMotion[0];

			ApplyAction();
			
			m_dwTime = 0;
			m_dwTick = 0;
			m_dwDeadLine = 0;
			m_fActionSpeed = 1.0F;
		} //if..else..
	} //if*/
	return TRUE;
} //CCharacterAction::Update_Stop

/**
 *
 */
BOOL CCharacterAction::Update_BasicAction()
{
	if( IsEndAction() )
	{
		return TRUE;
	} //if

	return FALSE; 
} //CCharacterAction::Update_BasicAction

/**
 *
 */
BOOL CCharacterAction::Update_Run()
{
	if( m_wLevel != 2 )
	{
		if( m_wAdvanced != m_wAction )
		{// 뭔가 다른 일거리가 있따냥...
			m_dwTime = 0;
			m_dwTick = 0;

			switch( m_wAdvanced )
			{
			case MODE_STOP:
				m_wLevel	= 2;
				m_pMotion	= &m_pAct->aMotion[m_wLevel];
				ApplyAction();

				m_wAdvanced = MODE_RUN;
				m_wAction	= MODE_RUN;
				m_bEndAction = FALSE;
				m_pCreature->SetState( STATE_STOP );
				return FALSE;
				break;
			case MODE_RUN_ATTACK:
			case MODE_ATTACK:
				m_wLevel	= 2;
				m_pMotion	= &m_pAct->aMotion[m_wLevel];
				ApplyAction();
				
				m_wAction	= MODE_RUN;
				m_bEndAction = FALSE;
				m_pCreature->SetState( STATE_STOP );
				return FALSE;			
				break;
			default:
				return TRUE;
				break;
			}; //switch
		} //if..else..
	} //if

	m_dwPeaceTime = 0;
	if( IsEndAction() )
	{
		if( m_wLevel == 2 )
		{
			//if( m_wAdvanced != 
			//m_wAdvanced = MODE_STOP;
			if( m_wAdvanced == m_wAction )
			{
				m_wAdvanced = MODE_STOP;
			} //if

			m_pCreature->SetState( STATE_STOP );
			m_bEndAction = TRUE;
			return TRUE;
		} //if

		if( m_wLevel == 0 )
		{
			m_wLevel = 1;
			m_dwTime = 0;
			m_dwTick = 0;

			m_pMotion	= &m_pAct->aMotion[m_wLevel];
			ApplyAction();
			m_bEndAction = TRUE;
		} //if

		// STATE_RUN의 "중"동작 재생		
		if( *m_pModel									&&
			!m_pVehicleAction							&& 
			m_dwSoundTick >= (*m_pModel)->GetActionTicks()	&& 
			m_pSound 
			)
		{			
			if( !m_bUnderWater ) 
			{
				if( m_pMotion->wSoundFile )
				{
					char szWave[40] = "\0";
					strcpy( szWave, g_pCharacterState->GetFileName( m_pMotion->wSoundFile ) );
					if( strcmp( szWave, "" ) )
					{
						m_pSound->Play3D( szWave );
					} //if
					m_dwSoundTick = 0;
				} //if			
			}			
			else
			{
				m_pSound->Play3D( "etc_nul_0012.wav" );
				m_dwSoundTick = 0;
			} //if..else..
		}
	} //if

	m_bPeace = FALSE;

	return FALSE;
} //CCharacterAction::Update_Run

/**
 *
 */
BOOL CCharacterAction::Update_Attack()
{
	m_dwPeaceTime = 0;


	if( m_dwDeadLine >= 5000 )
	{

	} //if

	if( IsEndAction() )
	{
	//	Update_TargetState();

		if( m_wLevel == 1 )
		{
			PauseAction();
			m_bEndAction = TRUE;

		/*	if( m_wAdvanced == MODE_ATTACK )
			{	
				m_wAdvanced = MODE_STOP;
			} //if*/

			if( !m_bLongAttack )
			{
				ClearTargets();
			}
			else
			{
				m_pLongWeaponEffect = NULL;
			} //if..else..

			return TRUE;
		} //if

		m_wLevel = 1;
		m_dwTime = 0;
		m_dwTick = 0;

		m_pMotion	= &m_pAct->aMotion[m_wLevel];
		ApplyAction();
		
		if( !m_bLongAttack )
		{
			ShowDamageOfTarget();
			ClearTargets();
		}
		else if( m_pTarget )
		{
			m_pLongWeaponEffect = NULL;
			m_pWorld->AddMagic( m_szLongWeapon, m_pCreature, m_pTarget );
		} //if..else..
	} //if

	m_bPeace = FALSE;

	return FALSE;
} //CCharacterAction::Update_Attack

/**
 *
 */
BOOL CCharacterAction::Update_WalkAttack()
{
	if( IsEndAction() )
	{		
		m_wAction = MODE_WALK;
		ChangeAction();
	} //if

	return FALSE;
} //CCharacterAction::Update_WalkAttack

/**
 *
 */
BOOL CCharacterAction::Update_RunAttack()
{
	if( IsEndAction() )
	{
		if( m_wMode == MODE_PEACE )
		{
			m_dwPeaceTime = 0;
			m_wMode = MODE_WAR;
			SetMode( MODE_WAR );
		} //if

		m_wAction	= MODE_RUN;
		m_wAdvanced = MODE_RUN;
		ChangeAction();
		m_wLevel	= 1;
		m_pMotion	= &m_pAct->aMotion[m_wLevel];
		ApplyAction();
	} //if

	return FALSE;
} //CCharacterAction::Update_RunAttack

/**
 *
 */
BOOL CCharacterAction::Update_MagicCasting()
{
	if( IsEndAction() )
	{
		if( m_wLevel == 1 )
		{
			m_nMagicRepeat--;

			if( m_nMagicRepeat <= 0 )
			{
				m_wLevel = 2;
				m_dwTime = 0;
				m_pMotion	= &m_pAct->aMotion[m_wLevel];
				ApplyAction();
			} //if

			return FALSE;
		}
		else if( m_wLevel >= 2 )
		{	// 끝~~~~
	//		m_wAction = MODE_STOP;
			m_wAdvanced = MODE_STOP;
	//		ChangeAction();
			m_bEndAction = TRUE;

		///	m_pCreature->Stop();
			return TRUE;
		}
		else
		{	
			m_dwTime = 0;
			m_wLevel = 1;
			m_pMotion	= &m_pAct->aMotion[m_wLevel];
			ApplyAction();
		} //if..else..else..
	} //if
	return FALSE;
} //CCharacterAction::Update_MagicCasting

/**
 * 낚시 상태 조절
 */
BOOL CCharacterAction::Update_Fishing()
{
	if( m_wAction != m_wAdvanced )
	{
		return TRUE;
	}
	return FALSE;
}

/**
 *
 */
WORD CCharacterAction::GetWeaponId( WORD wWeapon )
{
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
	
	//	TODO : 추후에 다시 WEAPON_AXE로 바꾸어야 한다.	
	case dITEMTYPE_ONEHANDAXE:	
	//	return WEAPON_ONEHANDSWORD;
		return WEAPON_ONEHANDAXE;

	// TODO : 현재 Item.dat에 양손도끼 들의 성질이 전부 한손도끼로 잘못 들어가 있기 때문에, 양손도끼는 사실상 없다.
	case dITEMTYPE_TWOHANDAXE:	
	//	return WEAPON_TWOHANDSWORD;	
		return WEAPON_ONEHANDAXE;

	case dITEMTYPE_DUALAXE:
	//	return WEAPON_DUALSWORD;
		return WEAPON_DUALAXE;
	
	//	TODO : 추후에 다시 WEAPON_BLUNT로 바꾸어야 한다.	
	case dITEMTYPE_ONEHANDBLUNT:
	//	return WEAPON_ONEHANDSWORD;
		return WEAPON_ONEHANDBLUNT;

	case dITEMTYPE_TWOHANDBLUNT:
	//	return WEAPON_TWOHANDSWORD;
		return WEAPON_TWOHANDBLUNT;

	// TODO : 양손둔기는 없다
	case dITEMTYPE_DUALBLUNT:	
		return WEAPON_DUALSWORD;

	case dITEMTYPE_DONPA:		
		return WEAPON_DONPA;

	case dITEMTYPE_SHORTBOW:				
		return WEAPON_SHORTBOW;

	//	TODO : 추후에 다시 WEAPON_LONGBOW로 바꾸어야 한다.	
	case dITEMTYPE_LONGBOW:	
		return WEAPON_SHORTBOW;

	case dITEMTYPE_DDABALBOW:
		return WEAPON_DDABALBOW;

	case dITEMTYPE_CROSSBOW:	
		return WEAPON_CROSSBOW;

	//	TODO : 추후에 다시 WEAPON_SPHERE로 바꾸어야 한다.	
	case dITEMTYPE_SPHERE:	
	//	return WEAPON_SPHERE;
		return WEAPON_SPHERE;
	
	//	TODO : 추후에 다시 WEAOPN_BONG로 바꾸어야 한다.	
	case dITEMTYPE_BONG:	
	//	return WEAPON_SPHERE;
		return WEAPON_BONG;

	case dITEMTYPE_STAFF:
		return WEAPON_STAFF;
	
	case dITEMTYPE_FISHTACKLE:
		return WEAPON_FISHINGTACKLE;
		
	}; //switch

	return WEAPON_NONE;
} //CCharacterAction::GetWeaponId

/**
 *
 */
BYTE CCharacterAction::GetShieldValue()
{
	BYTE btValue = 0;
	
	if( m_bWithShield )
	{
		btValue = 1;
	} //if
	switch( m_wWeapon )
	{
	case dITEMTYPE_NONE:
	case dITEMTYPE_ONEHANDSWORD:
	case dITEMTYPE_ONEHANDAXE:
	case dITEMTYPE_ONEHANDBLUNT:
		return btValue;
		break;
	}; //switch

	return 0;
} //CCharacterAction::GetShieldValue

/**
 *
 */
void CCharacterAction::MagicCasting( BYTE btKind, BYTE btKind2, int nRepeat )
{
	Assert( btKind <= 4 && btKind2 <= 4 );

	if( nRepeat > 100 || nRepeat <= 0 )
	{
		nRepeat = 1;
	} //if

	m_btMagicKind	= btKind;
	m_btMagicKind2	= btKind2;
	m_nMagicRepeat	= nRepeat;

	if( m_wMode < MODE_VEHICLE )
	{
		SetMode( MODE_WAR );
	} //if

	m_wAdvanced = MODE_MAGIC_CASTING;
} //CCharacterAction::MagicCasting

/**
 *
 */
void CCharacterAction::AddTarget( IW3DCreature* pTarget )
{
	Assert( pTarget );

	if( pTarget )
	{
		m_pTarget = pTarget;

		SW3DDamage* pDamage = pTarget->GetDamage();
		SW3DDamage	damage = *pDamage;

		if( !m_mapTargets.empty() )
		{
			CreatureMap::iterator itor = m_mapTargets.find( pTarget->GetId() );

			if( itor != m_mapTargets.end() )
			{
				// 있다.!!!
				m_mapTargets.erase( itor );
			} //if
		} //if

		m_mapTargets.insert( std::make_pair( pTarget->GetId(), damage ) );
	} //if
} //CCharacterAction::AddTarget

/**
 *
 */
void CCharacterAction::RemoveTarget( IW3DCreature* pTarget )
{
	if( !pTarget )
	{
		return ;
	} //if

	if( pTarget == m_pTarget )
	{	
		m_pTarget = NULL;
	} //if


	CreatureMap::iterator itor = m_mapTargets.find( pTarget->GetId() );

	if( itor != m_mapTargets.end() )
	{
		// 있다.!!!
		m_mapTargets.erase( itor );
	} //if

} //CCharacterAction::RemoveTarget

/**
 *
 */
void CCharacterAction::ClearTargets()
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

	m_mapTargets.clear();

} //CCharacterAction::ClearTargets

/**
 *
 */
void CCharacterAction::ShowDamageOfTarget()
{
	if( m_mapTargets.empty() )
	{
		return ;
	} //if


	int nSize = m_mapTargets.size();

	CreatureMap::iterator itor = m_mapTargets.begin();

	while( itor != m_mapTargets.end() )
	{
		SW3DDamage*		pDamage = &itor->second;
		IW3DCreature*	pCreature = pDamage->pCreature;

		//------------------------
		// pCreatre Is Target.
		// So, m_pCreatre Is This Class's Master,
		// And, Attaker.
		//------------------------
		if( pCreature )
		{
			pCreature->Damage( pDamage->wDamage, pDamage->btCritical );
			pCreature->Damage( FALSE, m_pCreature );
		} //if
		
		itor++;
	} //while
} //CCharacterAction::ShowDamageOfTarget

/**
 *
 */
BOOL CCharacterAction::Update_TargetState()
{
	if( m_mapTargets.empty() )
	{
		return TRUE;
	} //if

	CreatureMap::iterator itor = m_mapTargets.begin();

	while( itor != m_mapTargets.end() )
	{
		SW3DDamage*		pDamage		= &itor->second;
		IW3DCreature*	pCreature	= pDamage->pCreature;

		if( pCreature )
		{
			if( pCreature->IsDead() )
			{
				pCreature->Damage( FALSE, m_pCreature );
			} //if
		} //if
		
		itor++;
	} //while

	return TRUE;
} //CCharacterAction::Update_TargetState

/**
 *
 */
BOOL CCharacterAction::Update_Dead()
{
	if( IsEndAction() )
	{
		PauseAction();
		return FALSE;
	} //if

	return FALSE;
} //CCharacterAction::Update_Dead

/**
 *
 */
void CCharacterAction::SetLongWeaponEffect( const char* szEffect )
{
	Assert( szEffect );

	sprintf( m_szLongWeapon, "%s", szEffect );
} //CCharacterAction::SetLongWeaponEffect

/**
 *
 */
void CCharacterAction::Resurrect()
{	
	Stop();

	m_wAction = m_wAdvanced;
	ChangeAction();
	m_bEndAction	= FALSE;
} //CCharacterAction::Resurrect


/**
 *
 */
void CCharacterAction::Attack()
{	
	if( m_wAction == MODE_RUN )
	{
		m_wAdvanced = MODE_ATTACK;
		return ;
	} //if
	m_wAdvanced = MODE_ATTACK;
	m_wAction = m_wAdvanced;
	ChangeAction();
	m_bEndAction	= FALSE;
} //CCharacterAction::Attack


/**
 *
 */
BOOL CCharacterAction::Skill( SAction* pAction )
{
	if( m_wAction == MODE_SKILL )
	{
		return FALSE;
	} //if

	SetAction( pAction );

	m_wAdvanced = MODE_SKILL;
	m_wAction = m_wAdvanced;
//	ChangeAction();
	m_pCreature->SetState( STATE_SKILL );
	m_bEndAction	= FALSE;	
	m_wLevel = 0;
	m_dwTime = 0;
	m_dwTick = 0;
	m_dwDeadLine = 0;
	m_bSameChange = FALSE;

	SetSkillSpeed( SKILL_CAST_SPEED );

	return TRUE;

} //Skill

/**
 * 낚시 게임에서 낚시대를 드리운다
 */
void CCharacterAction::Hook()
{
	// 걷기 동작에 낚시대를 드리우는 에니메이션을 넣었음
	m_wAction = MODE_FISHING;
	m_wAdvanced = MODE_FISHING;
	
	ChangeAction();

	m_bEndAction	= FALSE;
} // CCharacterAction::Hook()


/**
 *
 */
void CCharacterAction::CompulsionStop()
{
	m_wAdvanced = MODE_STOP;
	m_wAction = m_wAdvanced;
	ChangeAction();
	m_bEndAction	= FALSE;
} //CCharacterAction::CompulsionStop

/**
 *
 */
void CCharacterAction::Dead()
{
	m_wAdvanced = MODE_DEAD;
	m_wAction = MODE_DEAD;
	ChangeAction();
	m_bEndAction	= FALSE;
} //CCharacterAction::Dead

/**
 *
 */
BOOL CCharacterAction::Update_Skill()
{
	if( IsEndAction() )
	{
		m_wAdvanced = MODE_STOP;
		m_bEndAction = TRUE;

		return TRUE;
	} //if

	m_bPeace = FALSE;

	return FALSE;	
} //CCharacterAction::Update_Skill