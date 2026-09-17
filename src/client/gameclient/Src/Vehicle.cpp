/**
 * 탈것에 대한 처리 ( 말이나 기타 등등  )
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-12 6:42p $
 * @version	$Revision: 22 $
 *
 * <HR>
 * $Log: /GameClient/Src/Vehicle.cpp $
 * 
 * 22    04-03-12 6:42p Sckyj
 * 운영자 작업중
 * 
 * 21    03-12-22 5:00p Sckyj
 * 2D, 3D sound 분리
 * 
 * 20    03-12-21 3:55a Sckyj
 * 사용하지 않는 사운드 재생함수 제거2
 * 
 * 19    03-12-21 1:41a Sckyj
 * 사용하지 않는 사운드 재생코드 삭제
 * 
 * 18    03-12-16 1:25a Sckyj
 * 물위에서 물발자국 소리 나도록 추가
 * 
 * 17    03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 16    03-10-17 11:22a Sckyj
 * vehicle sound관련 코드 추가
 * 
 * 15    03-10-08 4:02p Sckyj
 * 
 * 14    03-10-08 3:13p Sckyj
 * 
 * 13    03-10-02 10:33p Kjmgo
 * 캐릭터 State 권한 이전 작업 : 1차 백업
 * 
 * 12    03-09-15 2:32a Paran
 * 
 * 11    03-09-14 2:49a Kjmgo
 * 
 * 10    03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * <HR>
 *
 * @file	Vehicle.h
 */

#include "Global.h"
#include "Vehicle.h"
#include <string>
#include <vector>
#include <map>

typedef std::map<DWORD, IW3DModel*>		ModelMap;
static ModelMap							gs_mapVehicleModel;		///< 모델 데이터.

const int	HORSE	= 1;	///<
const int	CAR		= 2;	///<	현재 안씀

/**
 *
 */
CVehicle::CVehicle()
{
	m_pModel		= NULL;
	m_pAction		= NULL;
	m_pActionMgr	= NULL;
	m_nSex			= _MALE;

	m_pWorld		= NULL;	
	m_pBoundBox		= NULL;	
	m_pSound		= NULL;	
} //CVehicle::CVehicle

/**
 *
 */
CVehicle::~CVehicle()
{
//	SAFE_RELEASE( m_pModel );
	if( m_pModel )
	{
		if( m_pModel->GetRefCount() <= 1 )
		{
			ModelMap::iterator	itor = gs_mapVehicleModel.find( m_dwType );
			if( itor != gs_mapVehicleModel.end() )
			{
				gs_mapVehicleModel.erase( itor );
			} //if
		} //if
	} //if

	SAFE_RELEASE( m_pModel );
	SAFE_RELEASE( m_pSound );
	SAFE_DELETE( m_pActionMgr );

} //CVehicle::CVehicle

/**
 *
 */
BOOL CVehicle::Create( IW3DWorld* pWorld, const DWORD dwId, const void* pData )
{
	m_pWorld = pWorld;

	Assert( pData );
	Assert( m_pWorld );

	if( !pData || !m_pWorld )
	{
		return FALSE;
	} //if
	
	SW3DVehicle&	infoVehicle = *(SW3DVehicle*)pData;

	m_dwId		= dwId;
	m_dwType	= infoVehicle.wType;
	m_sName		= "";
	m_vPos.x	= (float)infoVehicle.nX + 0.5F;
	m_vPos.y	= (float)infoVehicle.nY + 0.5F;
	m_vPos.z	= m_pWorld->GetHeight( m_vPos );
	m_nSex		= infoVehicle.nSex;
	
	m_vGoalPos	= m_vPos;
	m_qRot		= Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();

	SAFE_RELEASE( m_pSound );
	m_pSound = MAKE_COMPONENT( SoundObject3D );
	CHECK_PTR_RETURN( m_pSound );
	m_pSound->SetPosition( m_vPos );

	m_pModel = LoadModel( infoVehicle.wType );
	
	if( !m_pModel )
	{
		Assert( !"Vehicle 모델을 읽어 올수 없습니다." );
		return FALSE;
	} //if

	m_pActionMgr = new CVehicleAction;
	m_pActionMgr->Init( m_pModel );

	m_pActionMgr->SetSex( m_nSex );
	m_pActionMgr->SetMode( m_dwType );
	m_pAction = m_pActionMgr->GetAction();
	
	m_pActionMgr->SetSound( m_pSound );

	return TRUE;
} //CVehicle::Create

/**
 *
 */
IW3DModel* CVehicle::LoadModel( DWORD dwType )
{
	ModelMap::iterator	itor = gs_mapVehicleModel.find( dwType );
	if( itor != gs_mapVehicleModel.end() )
	{
		IW3DModel*	pModel = itor->second;
		Assert( pModel );

		if( pModel )
		{
			pModel->AddRef();
			return pModel;
		} //if
	} //if

	std::string		sName = GetModelName( dwType );

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	if( !pModel )
	{
		Assert( !"메모리 할당 실패" );
		return NULL;
	} //if

	if( !pModel->Load( "Vehicle", ( sName + ".mod" ).c_str() ) )
	{
		Assert( !"모델 읽기 실패" );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if

	gs_mapVehicleModel.insert( std::make_pair( dwType, pModel ) );

	DEBUG_OUT( "Load Vehicle Model - " + sName.c_str() );

	return pModel;
} //CVehicle::LoadModel

/**
 * 모델파일이름을 얻는다.
 *
 * @param	dwType		[IN] 몬스터 종류
 *
 * @return	모델파일이름.
 */
char* CVehicle::GetModelName( DWORD dwType )
{
	dwType--;
	Assert( dwType < 6 && dwType >= 0 );

	if( dwType < 0 || dwType > 6 )
	{
		return "horse_1";
	} //if

	static char	szVehicle[3][256] =
	{
		"horse_1",
		"horse_2",
		"horse_3"
	}; //static char	szMonster[1][256] =
	
	return szVehicle[dwType];
} //CW3DMonster::GetModelName


/**
 * Update
 */
BOOL CVehicle::Update( const DWORD dwTick )
{
	Assert( m_pWorld );
	Assert( m_pModel );
	Assert( m_pActionMgr );

	m_pSound->SetPosition( m_vPos );
	m_pModel->SetAction( m_pAction );
	
	if( m_vPos.z < 0 )
	{
		m_pActionMgr->SetUnderWater( TRUE );
	}
	else
	{
		m_pActionMgr->SetUnderWater( FALSE );
	}
	m_pActionMgr->Update( dwTick );

	static Matrix4	matBone;
	static Matrix4	matWorld;

	matBone = m_pModel->GetBoneMatrix( "Bip01", m_dwTick );
	matWorld.SetWorld( m_vPos, m_qRot );
	matWorld = matBone * matWorld;

	BOOL	bLive = TRUE;

	switch( m_nState )
	{
	case STATE_MOVE:
		m_pWorld->AddParticle( "연기", m_vPos );
		break;
	}; //switch

	m_dwTick = m_pActionMgr->GetTime();

	return bLive;
} //CVehicle::Update

/**
 * Draw
 */
void CVehicle::Draw()
{
	Assert( m_pWorld );
	Assert( m_pModel );

#ifdef SHOW_BOUNDBOX
	m_pBoundBox->Render( m_dwTick );
#endif
	m_pModel->SetWorld( m_vPos, m_qRot );
	m_pModel->SetAction( m_pAction );
	m_pModel->Render( m_dwTick, TRUE );
		
} //CVehicle::Draw

/**
 * 그림자
 */
void CVehicle::DrawShadow()
{
} //CVehicle::DrawShadow

/**
 *
 */
void CVehicle::SetLighting( const BOOL bLighting )
{

} 

/**
 * 캐릭터 상태를 설정한다.
 */
void CVehicle::SetState( const int nState )
{
	switch( nState )
	{
	case STATE_STOP:	
		if( m_nState == STATE_STOP )	break;		
		//m_pActionMgr->
		break;

	case STATE_MOVE:
		if( m_nState == STATE_MOVE )	break;
		m_pWorld->AddParticle( "연기", m_vPos );
		
		break;

	case STATE_DEAD:
//		m_pSound->Stop();
//		m_pSound->Stop3D();
		break;
	} //if
} //CVehicle::SetState