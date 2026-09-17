/**
 * 용병.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-06-08 3:57p $
 * @version	$Revision: 16 $
 *
 * <HR>
 * $Log: /GameClient/Src/Mercenary.cpp $
 * 
 * 16    04-06-08 3:57p Paran2
 * 
 * 15    04-03-12 6:42p Sckyj
 * 운영자 작업중
 * 
 * 14    04-03-12 2:12p Paran
 * Collision & Picking
 * 
 * 13    03-12-22 5:00p Sckyj
 * 2D, 3D sound 분리
 * 
 * 12    03-10-13 5:49p Kjmgo
 * 
 * 11    03-10-11 12:08a Kjmgo
 * AddMagic수정 및, CCreature::~CCreature(); 쓰던거 삭제
 * 
 * 10    03-09-09 12:15a Kjmgo
 * 
 * 9     03-08-29 9:19a Kjmgo
 * Animation 바꿈
 * 
 * 8     03-08-23 8:10p Paran
 * 
 * 7     03-08-19 9:47p Paran
 * 
 * 6     03-08-14 4:18p Kjmgo
 * 
 * 5     03-06-27 5:30p Kjmgo
 * Assert처리
 * 
 * 4     03-06-23 5:53p Paran
 * 바운드 박스 처리.
 * 
 * 3     03-06-13 7:43p Kjmgo
 * 
 * 2     03-06-03 11:38p Paran
 * 조력자 맴버 추가
 * 
 * <HR>
 *
 * @file	Mercenary.cpp
 */


#include "Global.h"
#include "Creature.h"
#include <string>
#include <vector>
#include <map>


typedef std::map<DWORD, IW3DModel*>		ModelMap;

static ModelMap		gs_mapMercenaryModel;		///< 모델 데이터.


/**
 * 용병 클래스.
 * @ingroup		Game
 */
class CW3DMercenary : public CCreature
{
public:
	UNDER_MEMORY_MGR( CW3DMercenary );

	/**
	 * 생성자.
	 */
	CW3DMercenary()
	:	m_pModel		( NULL ),
		m_pShadow		( NULL ),
		m_pAction		( NULL ),
		m_bLighting		( TRUE ),
		m_bShadow		( FALSE ),
		m_dwCharacter	( 0 )
	{
		m_fOffset = 4.2F;
	} //CW3DMercenary

	/**
	 * 소멸자.
	 */
	~CW3DMercenary()
	{
		SAFE_RELEASE( m_pShadow );

		if( m_pModel )
		{
			if( m_pModel->GetRefCount() <= 1 )
			{
				ModelMap::iterator	itor = gs_mapMercenaryModel.find( m_dwType );
				if( itor != gs_mapMercenaryModel.end() )
				{
					gs_mapMercenaryModel.erase( itor );
				} //if
			} //if
		} //if

		SAFE_RELEASE( m_pModel );	
	} //~CW3DMercenary

	BOOL	Create( IW3DWorld* pWorld, const DWORD dwId, const void* pData );

	BOOL	Update( const DWORD dwTick );
	BOOL	UpdateModel( const WORD wItemType, const BOOL bOn );

	void	Draw();
	void	DrawShadow();

	/**
	 * 크리쳐 종류를 얻는다.
	 */
	int		GetType()
	{
		return MERCENARY;
	} //GetType

	void	SetLighting( const BOOL bLighting );

	/**
	 * 동작을 설정한다.
	 */
	void	SetAction( const string& sAction );

	/**
	 * 길찾기를 통해 목표점까지 이동한다.
	 */
	BOOL	MoveTo( const Vector3& vPos );

	/**
	 * 다른 크리쳐를 공격한다.
	 */
	void	Attack(
		IW3DCreature*	pTarget,		///< [IN] 공격 대상
		const int		nType			///< [IN] 공격 방법
		)
	{
		Assert( pTarget );

		if( !pTarget )
		{
			return ;
		} //if

		SetState( STATE_ATTACK );

		
		m_dwCharacter	= pTarget->GetId();
	} //Attack

	/**
	 * 공격 상태.
	 *
	 * @param	dwTick		[IN] 경과 틱
	 * @param	nIndex		[IN] 공격형태
	 */
	BOOL	StateAttack( DWORD dwTick, int nIndex )
	{
		Assert( m_pWorld );
		Assert( m_pModel );

		DWORD	dwCurTick = m_dwTick + dwTick;
/*
		if( ( m_dwTick  <= 500 ) &&
			( dwCurTick >  500 ) )
		{
			IW3DCreature*	pCharacter = m_pWorld->GetMonster( m_dwCharacter );
			if( pCharacter )
			{
				pCharacter->Damage();
			} //if
		} //if
*/
		if( IsEndAction( dwCurTick ) )
		{
			m_vPos.z   = m_pWorld->GetHeight( m_vPos );
			m_vGoalPos = m_vPos;
			Stop();
		} //if

		return TRUE;
	} //StateAttack

	/**
	 * 조력자 설정.
	 */
	void SetHelper( const IW3DCreature* pCreature )
	{
		//--NULL--
	} //SetHelper

	/**
	 * 조력자 제거.
	 */
	void RemoveHelper()
	{
		//--NULL--
	} //RemoveHelper

	/**
	 * 성별을 얻는다.
	 */
	UINT GetSex()
	{
		return 0;
	} // GetSex

private:
	IW3DModel*		m_pModel;		///< 모델
	IW3DShadow*		m_pShadow;		///< 그림자
	IW3DCreature*	m_pOwnerChar;	///< 주인 캐릭터

	SAction*		m_pAction;

	BOOL			m_bLighting;	///< 빛 적용 여부
	BOOL			m_bShadow;		///< 그림자 적용 여부

	DWORD			m_dwCharacter;	///< 공격 중인 캐릭터

	IW3DMagic*		m_pMagic;	///< 마법


	IW3DModel*	LoadModel( DWORD dwType );

	char*		TypeName( DWORD dwType );
	char*		GetModelName( DWORD dwType );

	char*		GetActionName();
	BOOL		IsEndAction( DWORD dwTick );

	BOOL		StateDead( DWORD dwTick );
	BOOL		StateMovement( DWORD dwTick );
}; //class CW3DMercenary


IMPLEMENT_COMPONENT( CW3DMercenary, Mercenary );




/**
 * 용병 생성.
 *
 * @return	성공 여부
 */
BOOL CW3DMercenary::Create(
	IW3DWorld*		pWorld,		///< [IN] 월드 객체
	const DWORD		dwId,		///< [IN] 크리쳐 ID
	const void*		pData		///< [IN] 크리쳐 생성 정보
	)
{
	m_pWorld = pWorld;

	Assert( pData );
	Assert( m_pWorld );
	Assert( NULL == m_pSound );
	Assert( NULL == m_pModel );
	Assert( NULL == m_pShadow );

	SW3DMercenaryInfo&	infoMercenary = *(SW3DMercenaryInfo*)pData;

	m_dwType	= infoMercenary.wType;

	m_dwId			= dwId;
	m_pOwnerChar	= m_pWorld->GetCharacter( m_dwId );
	if( !m_pOwnerChar )
	{
		Assert( !"주인이 없다" );
		return FALSE;
	} //if

	m_sName		= TypeName( m_dwType );
	m_vPos.x	= (float)infoMercenary.nX + 0.5F;
	m_vPos.y	= (float)infoMercenary.nY + 0.5F;
	m_vPos.z	= m_pWorld->GetHeight( m_vPos );
	m_vGoalPos	= m_vPos;
	m_qRot		= Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();

	m_pSound = MAKE_COMPONENT( SoundObject3D );
	CHECK_PTR_RETURN( m_pSound );

	// Load Model

	m_pModel = LoadModel( m_dwType );
	if( !m_pModel )
	{
		Assert( !"모델 읽기 실패" );
		return FALSE;
	} //if

	// Create Shadow

	m_pShadow = MAKE_COMPONENT( Shadow );
	CHECK_PTR_RETURN( m_pShadow ); 

	m_pShadow->SetSize( 1 );
	if( !m_pShadow->Init( 1 ) )
	{
		Assert( !"그림자 초기화 실패" );
		SAFE_RELEASE( m_pShadow );
		return FALSE;
	} //if

	m_bShadow = TRUE;

	m_pOwnerChar->SetHelper( this );
	return TRUE;
} //CW3DMercenary::Create

/**
 * 용병모델을 읽어옴.
 *
 * @param	dwType		[IN] 용병 종류
 *
 * @return 용병 모델
 */
IW3DModel* CW3DMercenary::LoadModel( DWORD dwType )
{
	ModelMap::iterator	itor = gs_mapMercenaryModel.find( dwType );
	if( itor != gs_mapMercenaryModel.end() )
	{
		IW3DModel*	pModel = itor->second;
		Assert( pModel );

		if( !pModel )
		{
			return NULL;
		} //if

		pModel->AddRef();
		return pModel;
	} //if

	std::string		sName = GetModelName( m_dwType );

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	if( !pModel )
	{
		Assert( !"메모리 할당 실패" );
		return NULL;
	} //if

	if( !pModel->Load( "Mercenary", ( sName + ".mod" ).c_str() ) )
	{
		Assert( !"모델 읽기 실패" );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if
	pModel->LoadAnimation( "Mercenary", ( sName + ".ani" ).c_str() );

	gs_mapMercenaryModel.insert( std::make_pair( dwType, pModel ) );

	DEBUG_OUT( "Load MERCENARY Model - " + sName.c_str() );

	return pModel;
} //CW3DMercenary::LoadModel

/**
 * 용병 이름을 얻는다.
 *
 * @param	dwType		[IN] 용병 종류
 *
 * @return	용병 이름
 */
char* CW3DMercenary::TypeName( DWORD dwType )
{
	static char	szMercenary[3][256] =
	{
		"사오정",
		"저팔계",
		"손오공"
	}; //static char	szMercenary[3][256] =

	return szMercenary[dwType % 3];
} //CW3DMercenary::TypeName

/**
 * 모델파일이름을 얻는다.
 *
 * @param	dwType		[IN] 몬스터 종류
 *
 * @return	모델파일이름.
 */
char* CW3DMercenary::GetModelName( DWORD dwType )
{
	static char	szMercenary[3][256] =
	{
		"globefish",
		"oger",
		"orc_archer"
	}; //static char	szMercenary[3][256] =

	return szMercenary[dwType % 3];
} //CW3DMercenary::GetModelName

/**
 * 용병를 그려준다.
 */
void CW3DMercenary::Draw()
{
	IW3DScene*		pScene = MAKE_COMPONENT( Scene );

	Assert( pScene );
	Assert( m_pWorld );
	Assert( m_pModel );

	m_pModel->SetWorld( m_vPos, m_qRot );

	if( !m_bShow )
	{
		return;
	} //if

	m_pModel->SetAction( m_pAction );

	m_pModel->Render( m_dwTick, m_bLighting );
} //CW3DMercenary::Draw

/**
 * 용병의 그림자를 그려준다.
 */
void CW3DMercenary::DrawShadow()
{
	IW3DScene*		pScene = MAKE_COMPONENT( Scene );

	Assert( pScene );
	Assert( m_pWorld );
	Assert( m_pShadow );

	if( !m_bShadow || !pScene->GetFrustum().CheckSphere( m_vPos + Vector3( 0.0F, 0.0F, 1.0F ), 1.0F ) )
	{
		return;
	} //if

	if( m_pShadow->IsProjection() )
	{
		m_pWorld->GetTerrain()->InsertShadow( m_pShadow, m_vPos );

		m_pShadow->Render( pScene->GetFrustum().m_matView );
	}
	else
	{
		Matrix4		matWorld;
		matWorld.SetWorld( m_vPos + Vector3( 0.0F, 0.0F, 0.1F ), m_qRot );
		m_pShadow->Render( matWorld );
	} //if..else..
} //CW3DMercenary::DrawShadow

/**
 * 빛 적용 여부를 설정한다.
 * 빛을 적용하지 않으면 원색 그대로 출력된다.
 * 즉, 주변의 오브젝트에 비해 상대적으로 밝게 보인다.
 * 크리쳐를 선택하거나 하는 경우에 사용하면 효과적이다.
 *
 * @param	bLighting	[IN] 빛 적용 여부.
 */
void CW3DMercenary::SetLighting( const BOOL bLighting )
{
	m_bLighting = bLighting;
} //CW3DMercenary::SetLighting

/**
 * 죽은 상태.
 *
 * @param	dwTick		[IN]	경과 tick
 */
BOOL CW3DMercenary::StateDead( DWORD dwTick )
{
	Assert( m_pWorld );
	Assert( m_pModel );

	DWORD	dwCurTick = m_dwTick + dwTick;

	m_bShadow = FALSE;

	switch( m_dwEvent )
	{
	case 0:
		if( dwCurTick > 0 )
		{
//			m_pSound->Play( "골렘죽을때.wav" );
			m_dwEvent++;
		} //if
		break;

	case 1:
		if( dwCurTick > 500 )
		{
			m_pWorld->AddEffect( "die.wed", m_vPos, m_qRot );
			m_dwEvent++;
		} //if
		break;
	} //switch

	if( IsEndAction( dwCurTick ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DMercenary::StateDead

/**
 * 이동하는 상태
 *
 * @param	dwTick		[IN]	경과 틱
 */
BOOL CW3DMercenary::StateMovement( DWORD dwTick )
{
	Assert( m_pWorld );

	m_vGoalPos.z = m_pWorld->GetHeight( m_vGoalPos );

	if( !IsGoal() )
	{
		float		fOffset = m_fOffset * dwTick / 1000;
		Vector3		vDir = ( m_vGoalPos - m_vPos ).Normal();

		if( ( m_vGoalPos - m_vPos ).Length() < fOffset )
		{
			fOffset = ( m_vGoalPos - m_vPos ).Length();
		}

		m_vPos += vDir * fOffset;
		m_qRot = vDir.GetQuaternion();

		m_vPos.z = m_pWorld->GetHeight( m_vPos );

		if( fOffset > 5.0F )
		{
			m_vPos = m_vGoalPos;
			m_qRot = vDir.GetQuaternion();
		} //if

		Move();
	}
	else
	{
		m_vPos.z = m_pWorld->GetHeight( m_vPos );

		Stop();
	} //if..else..

	return TRUE;
} //CW3DMercenary::StateMovement

/**
 * 현재 Action의 이름을 알려준다.
 *
 * @return	현재 Action의 이름 / NULL
 */
char* CW3DMercenary::GetActionName()
{
	switch( GetState() )
	{
	case STATE_DEAD:	return "사망";
	case STATE_ATTACK:	return "공격";
	case STATE_MOVE:	return "이동";
	case STATE_STOP:	return "정지";
	default: 			break;
	} //switch
	return NULL;
} //CW3DMercenary::GetActionName

/**
 * 액션이 끝났는지 알려준다.
 */
BOOL CW3DMercenary::IsEndAction( DWORD dwTick )
{
	if( dwTick >= m_pModel->GetActionTicks() )
	{
		return TRUE;
	} //if

	return FALSE;
} //CW3DMercenary::IsEndAction


/**
 * ...
 */
BOOL CW3DMercenary::Update( const DWORD dwTick )
{
	IW3DScene*		pScene = MAKE_COMPONENT( Scene );

	Assert( pScene );
	Assert( m_pWorld );
	Assert( m_pShadow );
	Assert( m_pOwnerChar );

	Frustum		frustum = pScene->GetFrustum();
	Vector3		vPos    = m_vPos + Vector3( 0.0F, 0.0F, 1.0F );

	m_bShow = FALSE;
	if( frustum.CheckSphere( vPos, 1.0F ) )
	{
		m_bShow = TRUE;
		if( m_bShadow && m_pShadow->IsProjection() )
		{
			m_pModel->SetAction( m_pAction );
			m_pModel->SetWorld( m_vPos, m_qRot );
			m_pShadow->SetLight( m_vPos );
			m_pShadow->InsertBlocker( m_pModel );
			m_pShadow->ShadowMatrix( m_vPos );

			m_pShadow->MakeShadowMap( m_dwTick + dwTick );
		} //if
	} //if

	Vector3		vOwner = m_pOwnerChar->GetPosition();
	if( ( vOwner - GetPosition() ).Length() > 4.0F )//if( ( vOwner - GetPosition() ).Length() > 3.0F )
	{ // 임자와 3미터 이상 멀어지면 쫓아간다.
		MoveTo( vOwner );
	} //if
	
	BOOL	bLive = TRUE;

	switch( GetState() )
	{
	case STATE_DEAD:
		bLive = StateDead( dwTick ); 
		break; 

	case STATE_ATTACK:
		m_bShadow = TRUE;
		StateAttack( dwTick, 1 );
		break;

	default:
		m_bShadow = TRUE;
		StateMovement( dwTick );
		break;
	} //switch

	m_dwTick += dwTick;

	return bLive;
} //CW3DMercenary::Update

/**
 * 이동.
 */
BOOL CW3DMercenary::MoveTo( const Vector3& vPos )
{
	SetState( STATE_MOVE );
	SetAction( "이동" );
	SetGoalPosition( vPos );

	return TRUE;
} //CW3DMercenary::MoveTo

/**
 * 동작을 설정한다.
 */
void CW3DMercenary::SetAction( const string& sAction )
{
} //CW3DMercenary::SetAction

/**
 * 몬스터 모델을 업데이트 한다.
 */
BOOL CW3DMercenary::UpdateModel( const WORD wItemType, BOOL bOn )
{
	return TRUE;
} //CW3DMercenary::UpdateModel

