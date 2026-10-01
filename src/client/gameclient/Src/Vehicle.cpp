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
	Assert( dwType < 4 );

	if( dwType >= 4 )
	{
		return "horse_1";
	} //if

	static char	szVehicle[4][256] =
	{
		"horse_1",
		"horse_2",
		"horse_3",
		"mt_hellhorse"
	}; //static char	szMonster[1][256] =
	
	return szVehicle[dwType];
} //CW3DMonster::GetModelName


/**
 * Update
 */
// State is external so CVehicle's shipped 32-bit object ABI stays unchanged.
// Query actual animated sole centers; body-center spacing is not a hoof trail.
static void EmitHellHorseHooves( const void* pVehicle, DWORD dwId,
                                const SAction* pAction, IW3DModel* pModel,
                                IW3DWorld* pWorld, const Vector3& vPos,
                                const Quaternion& qRot, DWORD animationTick )
{
    struct HoofState
    {
        const void* vehicle;
        DWORD id, seenAt;
        float sampleX, sampleY;
        DWORD armed;
        const void* action;
        DWORD reserved;
        float previousY[4];
        int actionStart, actionEnd;
        IW3DAnimation* animation;
        DWORD animationTick;
    };
    static HoofState states[64] = {0};
    static const char* names[4] = {
        "Bip02 L Finger01", "Bip02 R Finger01", "Bip02 L Toe0", "Bip02 R Toe0"
    };
    // Centroid of the four sole vertices, transformed by inverse bind matrix.
    static const Vector3 soleLocal[4] = {
        Vector3(0.128036912464F, -0.007704655553F, 0.017997010834F),
        Vector3(0.128054758531F,  0.007704499051F, 0.018100216139F),
        Vector3(0.123759327335F,  0.003451034783F, 0.040027760792F),
        Vector3(0.123718520184F, -0.003541369337F, 0.040048668851F)
    };
    const DWORD now = GetTickCount();
    DWORD index = (reinterpret_cast<DWORD>(pVehicle) >> 4) & 63;
    for( DWORD probe = 0; probe < 64; ++probe )
    {
        HoofState& slot = states[index];
        if( slot.vehicle == pVehicle || !slot.vehicle || now - slot.seenAt >= 5000 ) break;
        index = (index + 1) & 63;
    }
    HoofState& state = states[index];
    const float dx = vPos.x - state.sampleX, dy = vPos.y - state.sampleY;
    const float step = dx * dx + dy * dy;
    const int actionStart = pAction ? pAction->nStart : 0;
    const int actionEnd = pAction ? pAction->nEnd : 0;
    IW3DAnimation* animation = pAction ? pAction->pAnimation : NULL;
    const bool reset = state.vehicle != pVehicle || state.id != dwId ||
        now - state.seenAt >= 5000 || state.action != pAction ||
        state.actionStart != actionStart || state.actionEnd != actionEnd ||
        state.animation != animation || animationTick < state.animationTick ||
        !(step > 0.000001F) || !(step < 64.0F) || !pAction || !pModel || !pWorld;
    state.vehicle = pVehicle; state.id = dwId; state.seenAt = now;
    state.sampleX = vPos.x; state.sampleY = vPos.y; state.action = pAction;
    state.actionStart = actionStart; state.actionEnd = actionEnd;
    state.animation = animation; state.animationTick = animationTick;
    if( reset )
    {
        state.armed = 0;
        for( int foot = 0; foot < 4; ++foot ) state.previousY[foot] = 0;
        return;
    }
    Matrix4 world;
    world.SetWorld( vPos, qRot );
    for( int foot = 0; foot < 4; ++foot )
    {
        const Matrix4 bone = pModel->GetBoneMatrix( names[foot], animationTick );
        const Vector3 sole = soleLocal[foot] * bone;
        const float previousY = state.previousY[foot];
        state.previousY[foot] = sole.y;
        const DWORD bit = 1 << foot;
        if( sole.y > 0.22F ) state.armed |= bit;
        else if( (state.armed & bit) && sole.y <= 0.16F && sole.y <= previousY )
        {
            state.armed &= ~bit;
            Vector3 flame = sole * world;
            flame.z = pWorld->GetHeight( flame ) + 0.20F;
            pWorld->AddParticle( "mt_hell_hoof", flame );
        }
    }
}

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

	if( m_dwType == 4 )
	{
		EmitHellHorseHooves( this, m_dwId, m_pAction, m_pModel, m_pWorld,
		                      m_vPos, m_qRot, m_pActionMgr->GetTime() );
	}
	else switch( m_nState )
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