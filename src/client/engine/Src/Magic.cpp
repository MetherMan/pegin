/**
 * 마법 처리
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-13 8:14p $
 * @version	$Revision: 60 $
 *
 * <HR>
 * $Log: /Engine/Src/Magic.cpp $
 * 
 * 60    04-01-13 8:14p Kjmgo
 * Creature및IW3DMagic을 포인터레퍼런스로 관리하도록 수정
 * 
 * 59    04-01-07 7:15a Kjmgo
 * 
 * 58    03-12-30 11:27p Kjmgo
 * 
 * 57    03-12-30 2:24p Sckyj
 * Sound관련 코드 제거 - 마법 관련 사운드는 MagicEffect에서 전담
 * 
 * 56    03-12-26 2:22p Kjmgo
 * 
 * 55    03-12-26 11:56a Kjmgo
 * Skill관련 추가 
 * 
 * 54    03-12-26 4:21a Kjmgo
 * 
 * 53    03-12-21 3:56a Sckyj
 * 사용하지 않는 사운드 재생코드 제거
 * 
 * 52    03-12-03 1:25p Kjmgo
 * IsSendAttack일경우에만, Update시, Creature 갱신
 * 
 * 51    03-12-03 10:27a Kjmgo
 *  카메라 흔들기 추가
 * 
 * 50    03-12-01 10:25a Kjmgo
 * 코드 정리
 * 
 * 49    03-11-01 3:42p Sckyj
 * 마법 이펙트 사운드 관련 코드 추가( 스크립트에서 사운드 재생 타입을
 * 읽어서 사운드 출력 )
 * 
 * 48    03-10-15 8:36p Kjmgo
 * 
 * 47    03-10-10 3:45a Kjmgo
 * 상대방에게도  Bone에 이펙트 붙이도록 수정
 * 
 * 46    03-10-08 3:03p Kjmgo
 * 상대방에게도 Bone에 이펙트를 달수 있도록 추가
 * 
 * 45    03-10-01 9:22p Kjmgo
 * Sound중 최상위 부분에서 위치 지정이 안되어  출력이 안되었던것을
 * Attacker위치로 출력되도록 수정
 * 
 * 43    03-10-01 11:38a Kjmgo
 * BlowTiming 작업
 * 
 * 42    03-09-18 5:15p Kjmgo
 * Magic에서 Bone을 사용할수 있게 수정
 * 
 * 41    03-09-13 1:25p Kjmgo
 * 마법 동작을 Creature Action에서 처리하도록 몇가지 옵션 추가
 * 
 * 40    03-09-11 10:45p Sckyj
 * 
 * 39    03-09-10 11:30a Sckyj
 * 
 * 38    03-08-22 11:11a Kjmgo
 * 
 * 37    03-08-21 11:29p Paran
 * 
 * 36    03-08-19 3:19a Kjmgo
 * 
 * 35    03-08-15 4:01a Kjmgo
 * 33    03-06-28 10:16a Kjmgo		Assert 추가
 * 32    03-06-26 5:21p Kjmgo
 * 23    03-06-13 3:02a Kjmgo		마법 체계 뒤엎음
 * 22    03-06-11 9:36p Kjmgo		Sound 추가
 * 13    03-03-23 9:17p Kjmgo		적 위치에서 출력하는 부분 버그 수정.
 * 9     03-03-21 6:23a Kjmgo		Creature를 포인터로 안가지게 고침.
 * <HR>
 *
 * @file	Magic.cpp
 */


#include "Engine_Export.h"
#include "Magic.h"

////////////////////////////////////
//
////////////////////////////////////

IW3DWorld*		g_pWorld;		///< World
IW3DCamera*		g_pCamera;		///< Camera

/**
 *
 */
void Magic::SetWorld( IW3DWorld* pWorld )
{
	g_pWorld = pWorld;
	g_pCamera = g_pWorld->GetCamera();


	Assert( g_pWorld );
	Assert( g_pCamera );
} //Magic::SetWorld

/**
 *
 */
void Magic::Shake( DWORD dwTime )
{
	Assert( g_pCamera );

	if( g_pCamera )
	{
		g_pCamera->Shake( dwTime );
	} //if
} //Magic::Shake

/**
 *
 */
class CMagic : public IW3DMagic
{
public:
	UNDER_MEMORY_MGR( CMagic );

	CMagic();
	~CMagic();

	//-=-=-=-=-=-= Create -=-=-=-=-=-=//
	BOOL Create( IW3DCreature& pAttacker, IW3DCreature& pTarget, IW3DMagicMgr*	pMagicMgr, MAGIC* pMagic, BOOL bSendEndAttack = TRUE );
	
	//-=-=-=-=-=-= Render -=-=-=-=-=-=//
	BOOL Update( const DWORD dwTick );
	BOOL Draw();

	//-=-=-=-=-=-= Option -=-=-=-=-=-=//
	void Play();
	void Pause();
	void Stop();

	//-=-=-=-=-=-= State -=-=-=-=-=-=//
	BOOL IsEnd();

	//-=-=-=-=-=-= Get -=-=-=-=-=-=//
	IW3DCreature* GetTarget();
	IW3DCreature* GetAttacker();

	//-=-=-=-=-=-= Die Creature -=-=-=-=-=-=//
	void DieAttacker()
	{
		m_bAttacker = TRUE;
		m_pAttacker = NULL;
	} //DieAttacker

	void DieTarget()
	{
		m_bTarget = TRUE;
		m_pTarget = NULL;
	} //DieTarget

	//-=-=-=-=-=-= ID -=-=-=-=-=//
	void  SetID( const DWORD dwID )
	{
		m_dwID = dwID;
	} //SetID

	DWORD GetID()
	{
		return m_dwID;
	} //GetID

	BYTE GetMagicKind()
	{
		return m_pMagicData->btKind;
	} //GetMagicKind
	
	BYTE GetMagicType()
	{
		return m_pMagicData->btType;
	} //GetMagicType

	BOOL IsSendEndAttack()
	{
		if( m_bSendEndAttack || m_bSkill )
		{
			return TRUE;
		} //if
		return FALSE;
	} //IsSendEndAttack

	void SetSkill()
	{
		m_bSkill = TRUE;
	} //SetSkill

private:
	DWORD				m_dwID;			///< 마법 고유 ID
	BOOL				m_bAttacker;	///< 공격자 크리처가 있는가? 
	BOOL				m_bTarget;		///< 피하재 크리처가 있는가?
	IW3DCreature*		m_pAttacker;	///< 공격자 크리처
	IW3DCreature*		m_pTarget;		///< 피해자 크리처

	IW3DMagicMgr*		m_pMagicMgr;	///< 마법 매니저
	IW3DTraceEffect*	m_pTrace;		///< 트레이스 이펙트
	IW3DParticles*		m_pParticles;	///< 파티클...

	Matrix4				m_matWorld;
	Matrix4				m_matTargetWorld;

	BYTE				m_btKind;		///< 종류
	BYTE				m_btType;		///< 타입
	
	BOOL				m_bQuit;		///< 종료 Flag
	BOOL				m_bPause;		///< 정지
	BOOL				m_bSound;
	BOOL				m_bSendEndAttack;
	BOOL				m_bSkill;

	Vector3				m_vTarget;
	Vector3				m_vPos;

	//-=-=-=-=-=-=-=-= Time -=-=-=-=-=-=-=-=-=//
	DWORD				m_dwTick;		///< 경과 시간
	DWORD				m_dwTime;		///< 누적 시간

	typedef std::list<CMagicEffect*>		EffectList;
	EffectList			m_listEffect;

	MAGIC*				m_pMagicData;


	////////////////////////////////////////////

	void	MakeMagic();

};

IMPLEMENT_COMPONENT( CMagic, Magic );

/**
 * 생성자
 */
CMagic::CMagic()
:		m_pAttacker		( NULL ),
		m_pTarget		( NULL ),
		m_pMagicMgr		( NULL ),
		m_pTrace		( NULL ),
		m_pParticles	( NULL ),
		m_bSkill		( FALSE ),
		m_bSound		( TRUE ),
		m_bQuit			( FALSE ),
		m_bSendEndAttack( TRUE ),
		m_bPause		( FALSE ),
		m_bAttacker		( FALSE ),
		m_bTarget		( FALSE )
{
	static DWORD dwID = 0;
	m_dwID = dwID++;

	m_matWorld.SetIdentity();
	m_matTargetWorld.SetIdentity();
} //CMagic::CMagic

/**
 * 소멸자
 */
CMagic::~CMagic()
{
	EffectList::iterator	itor = m_listEffect.begin();

	while( itor != m_listEffect.end() )
	{
		SAFE_DELETE( *itor );
		itor = m_listEffect.erase( itor );
	} //while
} //CMagic::~CMagic

/**
 *
 */
BOOL CMagic::Create( IW3DCreature& pAttacker, IW3DCreature& pTarget, IW3DMagicMgr*	pMagicMgr, MAGIC* pMagic, BOOL bSendEndAttack )
{
	Assert( pMagicMgr );
	Assert( pMagic );

	if( !pMagicMgr || !pMagic )
	{
		return FALSE;
	} //if

	//-=-=-=-=-=-=-=-= Get Effect -=-=-=-=-=-=-=-=//
	m_pMagicMgr		= pMagicMgr;
	m_pTrace		= pMagicMgr->GetTraceEffect();
	m_pParticles	= pMagicMgr->GetParticles();
	m_pMagicData	= pMagic;

	m_bSendEndAttack= bSendEndAttack;

	//-=-=-=-=-=-=-=-= Set Creature -=-=-=-=-=-=-=-=//
	m_pAttacker = &pAttacker;
	m_pTarget	= &pTarget;

	if( !m_pTrace || !m_pParticles )
	{
		Assert( !"Effect를 얻어오는데 실패했습니다." );
		return FALSE;
	} //if

	m_matWorld.SetWorld( m_pAttacker->GetPosition(), m_pAttacker->GetDirection().GetQuaternion() );
	m_matTargetWorld.SetWorld( m_pTarget->GetPosition(), m_pTarget->GetDirection().GetQuaternion() );
	m_vPos = m_pAttacker->GetPosition();
	MakeMagic();

	return TRUE;
} //CMagic::Init

/**
 *
 */
BOOL CMagic::Update( const DWORD dwTick )
{
	if( m_bQuit )
	{
		return FALSE;
	} //if

	if( m_bPause )
	{
		return TRUE;
	} //if

	//-=-=-=-=-=-=-=-= If Target die -=-=-=-=-=-=-=-=//

	if( m_bSendEndAttack || m_bSkill )
	{
		if( !m_pTarget ) 
		{
			Assert( m_pMagicData );
			switch( m_pMagicData->nScriptType )
			{
			case TARGET_DIE_STOP:
				return FALSE;
				break;
			case TARGET_DIE_LASTTARGET:
				break;
			} //switch
		} 
		else
		{
			m_vTarget = m_pTarget->GetPosition();
		} //if..else..
	} //if

	DWORD dwTime = 0;
	DWORD dwTargetTime = 0;
	

	if( m_bSendEndAttack || m_bSkill )
	{
		if( m_pAttacker )
		{
			m_vPos = m_pAttacker->GetPosition();
			m_matWorld.SetWorld( m_vPos, m_pAttacker->GetQuaternion() );
			dwTime = m_pAttacker->GetTime();
		} //if

		if( m_pTarget )
		{
			m_vPos = m_pTarget->GetPosition();
			m_matTargetWorld.SetWorld( m_vPos, m_pTarget->GetQuaternion() );
			dwTargetTime = m_pTarget->GetTime();
		} //if
	} //if

	//-=-=-=-=-=-=-=-= Update Effect In a list -=-=-=-=-=-=-=-=//

	CMagicEffect*			pEffect	= NULL;
	EffectList::iterator	itor	= m_listEffect.begin();

	while( itor != m_listEffect.end() )
	{
		pEffect = *itor;

		if( pEffect && ( m_bSendEndAttack || m_bSkill ) )
		{
			pEffect->SetAttacker( m_pAttacker );

			if( m_pAttacker )
			{
				pEffect->SetModel( m_pAttacker->GetModel( 0 ) );
			}
			else
			{
				pEffect->SetModel( NULL );
			} //if..else..

			if( m_pTarget )
			{
				pEffect->SetTargetModel( m_pTarget->GetModel( 0 ) );
			}
			else
			{
				pEffect->SetTargetModel( NULL );
			} //if..else..
		} //if


		if( !pEffect || 
			!pEffect->Update( dwTick, m_vTarget, m_pParticles, 
							m_matWorld, m_matTargetWorld, dwTime, dwTargetTime ) )
		{
			SAFE_DELETE( pEffect );
			itor = m_listEffect.erase( itor );
			continue;
		} //if

		itor++;
	} //while

	//-=-=-=-=-=-=-=-= If List Empty... -=-=-=-=-=-=-=-=//
	if( m_listEffect.empty() )
	{
		return FALSE;
	} //if

	m_dwTime += dwTick;
	return TRUE;
} //CMagic::Update

/**
 *
 */
BOOL CMagic::Draw()
{
	//-=-=-=-=-=-=-=-= Draw Effect In a List -=-=-=-=-=-=-=-=//
	CMagicEffect*			pEffect = NULL;
	EffectList::iterator	itor	= m_listEffect.begin();

	while( itor != m_listEffect.end() )
	{
		pEffect = *itor;
		if( !pEffect || !pEffect->Draw() )
		{
			SAFE_DELETE( pEffect );
			itor = m_listEffect.erase( itor );
			continue;
		} //if

		itor++;
	} //while

	//-=-=-=-=-=-=-=-= If List Empty -=-=-=-=-=-=-=-=//
	if( m_listEffect.empty() )
	{
		return FALSE;
	} //if

	return TRUE;
} //CMagic::Draw

/**
 *
 */
void CMagic::Play()
{
	m_bPause = FALSE;
} //CMagic::Pause

/**
 *
 */
void CMagic::Pause()
{
	m_bPause = TRUE;
} //CMagic::Pause

/**
 *
 */
void CMagic::Stop()
{
	m_bQuit = TRUE;
} //CMagic::Stop

/**
 *
 */
BOOL CMagic::IsEnd()
{
	return m_bQuit;
} //CMagic::IsEnd

/**
 *
 */
IW3DCreature* CMagic::GetTarget()
{
	return m_pTarget;
} //CMagic::GetTarget

/**
 *
 */
IW3DCreature* CMagic::GetAttacker()
{
	return m_pAttacker;
} //CMagic::GetAttacker

/**
 *
 */
void CMagic::MakeMagic()
{
	Assert( m_pMagicData );

	if( !m_pMagicData )
		return ;

	MAGIC::EffectList::iterator itorEffect = m_pMagicData->listEffect.begin();
	
	while( itorEffect != m_pMagicData->listEffect.end() )
	{
		EFFECT* p = *itorEffect;
		Assert( p );

		CMagicEffect* pEffect = NULL;
		pEffect = new CMagicEffect;

		if( !pEffect || !pEffect->Create( p ) )
		{
			SAFE_DELETE( pEffect );
			itorEffect++;
			continue;
		} //if

		if( m_pAttacker )
		{
			pEffect->SetModel( m_pAttacker->GetModel( 0 ) );
		} //if

		if( m_pTarget )
		{
			pEffect->SetTargetModel( m_pTarget->GetModel( 0 ) );
		} //if

		pEffect->Start( m_pAttacker->GetPosition(),
						  m_pTarget->GetPosition(),		// 피해자의 위치
						  m_pTrace );						// Trace Effect

		m_listEffect.push_back( pEffect );

		itorEffect++;
	} //while
} //CMagic::MakeMagic
