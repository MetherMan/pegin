/**
 * 마법 처리
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-05-31 6:57a $
 * @version	$Revision: 31 $
 *
 * <HR>
 * $Log: /Engine/Src/MagicEffect.cpp $
 * 
 * 31    04-05-31 6:57a Kjmgo
 * 
 * 30    04-01-10 12:57p Kjmgo
 * 
 * 29    04-01-10 10:39a Kjmgo
 * 
 * 28    04-01-07 7:15a Kjmgo
 * 
 * 27    04-01-06 12:01a Kjmgo
 * 버그 수정및, 코드 정리
 * 
 * 26    03-12-30 5:28p Sckyj
 * MagicEffect Sound를 두개에서 한개로 줄임
 * 
 * 25    03-12-22 4:59p Sckyj
 * 2D, 3D sound 분리
 * 
 * 24    03-12-14 4:17a Kjmgo
 * 화살 버그 수정
 * 
 * 23    03-12-04 4:58p Kjmgo
 * Model 관련 추가
 * 
 * 22    03-12-03 10:27a Kjmgo
 *  카메라 흔들기 추가
 * 
 * 21    03-11-28 10:22a Sckyj
 * 마법 사운드 재생시 SetReverb() 호출하던것 변경
 * 
 * 20    03-11-04 10:43a Sckyj
 * SoundPlay 함수 수정( Reverb control을 IW3DSoundObject 안으로 넣음 )
 * 
 * 19    03-11-01 3:42p Sckyj
 * 마법 이펙트 사운드 관련 코드 추가( 스크립트에서 사운드 재생 타입을
 * 읽어서 사운드 출력 )
 * 
 * 18    03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 17    03-10-10 3:45a Kjmgo
 * 상대방에게도  Bone에 이펙트 붙이도록 수정
 * 
 * 16    03-10-10 1:10a Kjmgo
 * 빌보드 프레임을 이쪽에서 관리하도록 수정
 * 
 * 15    03-10-08 3:03p Kjmgo
 * 상대방에게도 Bone에 이펙트를 달수 있도록 추가
 * 
 * 14    03-10-01 1:22p Kjmgo
 * 타격 이펙트 버그 수정
 * 
 * 13    03-10-01 11:38a Kjmgo
 * BlowTiming 작업
 * 
 * 12    03-09-18 5:15p Kjmgo
 * Magic에서 Bone을 사용할수 있게 수정
 * 
 * 11    03-09-10 11:30a Sckyj
 * 
 * 10    03-08-20 12:48p Kjmgo
 * 
 * 9     03-08-19 10:59a Kjmgo
 * 
 * 8     03-08-19 3:19a Kjmgo
 * 
 * 7     03-08-15 4:01a Kjmgo
 * 
 * 6     03-07-31 1:36a Kjmgo
 * 
 * 5     03-06-28 10:16a Kjmgo
 * Assert 추가
 * 
 * 4     03-06-26 12:02p Kjmgo
 * 
 * 3     03-06-15 7:29p Kjmgo
 * 
 * 2     03-06-13 3:02a Kjmgo
 * 마법 체계 뒤엎음
 * 
 * <HR>
 *
 * @file	MagicEffect.cpp
 */

#include "Engine_Export.h"
#include "Magic.h"
 
/**
 * 생성자
 */
CMagicEffect::CMagicEffect()
{
	m_pTrace			= NULL;				///< Trace Effect.
	m_pEffectData		= NULL;
	m_pModel			= NULL;
	m_pSound			= NULL;
	m_pAttacker			= NULL;
	m_pTargetModel		= NULL;

	m_bBlowTiming		= TRUE;
	m_dwSpeed			= 1000;
	m_dwTimeForModel	= 0;

	m_matWorld.SetIdentity();
} //CMagicEffect::CMagicEffect

/**
 * 소멸자
 */
CMagicEffect::~CMagicEffect()
{
	if( m_dwTrace && m_pTrace )
	{
		m_pTrace->DeleteEffect( m_dwTrace );
	} //if

	SAFE_RELEASE( m_pSound );
	
	MagicDataList::iterator itor = m_listMagic.begin();

	while( itor != m_listMagic.end() )
	{
		PART* pMagic = (*itor);
		itor = m_listMagic.erase( itor );
	} //while
} //CMagicEffect::~CMagicEffect

/**
 *
 */
BOOL CMagicEffect::Create( EFFECT* pEffect )
{
	Assert( pEffect );

	if( !pEffect )
	{
		return FALSE;
	} //if

	// Only explicitly marked private visual layers may omit the unused sound
	// object. Any sound or damage timing in any PART preserves the normal path.
	BOOL bPrivateVisual = FALSE;
	BOOL bNeedsSound = FALSE;
	EFFECT::MagicDataList::iterator soundPart = pEffect->listMagic.begin();
	for( ; soundPart != pEffect->listMagic.end(); ++soundPart )
	{
		PART* p = *soundPart;
		if( !p ) continue;
		if( p->bEnemyPos && p->dwSpeed == 140031 ) bPrivateVisual = TRUE;
		if( p->bSound || p->bBlowTiming ) bNeedsSound = TRUE;
	}
	if( !bPrivateVisual || bNeedsSound )
	{
		m_pSound = MAKE_COMPONENT( SoundObject3D );
		Assert( m_pSound );
	}
	
	m_pEffectData = pEffect;

	EFFECT::MagicDataList::iterator itor = m_pEffectData->listMagic.begin();

	while( itor != m_pEffectData->listMagic.end() )
	{
		PART* p = *itor;

		if( p )
		{
			m_listMagic.push_back( p ); 
		} //if

		itor++;
	} //while

	static DWORD dwId = 3000;
	
	m_dwTrace = 0;

	if( m_pEffectData->bTrace )
	{
		m_dwTrace = dwId++;
	} //if

	return TRUE;
} //CMagicEffect::Create

/**
 * 처음 시작할 단계를 설정해 준다.
 *
 * @return	제대로된 설정인지 리턴.
 */
BOOL CMagicEffect::Start(
	const Vector3&		vPos,		///< [IN] 처음 시작할 위치
	const Vector3&		vTarget,	///< [IN] 목표 지점
	IW3DTraceEffect*	pTrace		///< [IN] Trace Effecct
	)
{
	Assert( pTrace );
	Assert( m_pEffectData );

	if( !pTrace )
	{
		Assert( !"Trace 오류!" );
		return FALSE;
	} //if

	if( m_listMagic.empty() )
	{
		return FALSE;
	} //if

	m_itorList = m_listMagic.begin();
	PART* pMagic = (*m_itorList);

	if( !pMagic )
	{
		return FALSE;
	} //if

	if( m_pEffectData->bTrace )
	{
		if( !pTrace->CreateEffect( m_pEffectData->szTrace, m_dwTrace ) )
		{
			m_dwTrace = 0;
		} //if
	} //if

	// x의 경우는 양옆을 뜻하고 y의 경우에는 앞뒤를..z의 경우에는 위아래
	m_vTarget = vTarget;
	m_vEnemyPos = vTarget;

	{
	//	pMagic->bBone = FALSE;

		// 본 애니메이션의 영향을 받지 않느 ㄴ거라
		if( vPos == vTarget )
		{
			m_vPos	= RelativeCoordinates( vPos, vPos + Vector3( 0.0F, 0.2F, 0.0F ), Vector3( 0.0F, -0.2F, 0.0F ) );	
		} 
		else
		{
			m_vPos	= vPos;
		} //if	

		m_vMove	= RelativeCoordinates( vPos, m_vTarget, pMagic->vPos ); 
	} //if..else..

	if( pMagic->bBezier )
	{
		m_bezier.Init();
		m_bezier.v1 = m_vMove;
		m_bezier.v2 = RelativeCoordinates( m_vMove, m_vTarget, pMagic->vBezier2 );
		m_bezier.v3 = RelativeCoordinates( m_vMove, m_vTarget, pMagic->vBezier3 );
	} //if

	if( pMagic->dwShake > 0 )
	{
		Magic::Shake( pMagic->dwShake );
	} //if

	m_dwTime			= 0;
	m_dwTimeForDraw		= 0;
	m_dwTimeParticle	= 0;
	m_dwPartTime		= 0;
	m_dwTimeForModel	= 0;
	m_bBlowTiming		= TRUE;

	m_bDraw = FALSE;

	m_pTrace = pTrace;
	m_btType = pMagic->btType;

	m_dwStartTime	= pMagic->dwStartTime;
	m_bSound		= pMagic->bSound;
	m_dwSpeed		= pMagic->dwSpeed;

	m_pAttacker		= NULL;

	return TRUE;
} //CMagicEffect::Start

/**
 * 다음 파트로 넘어 간다.
 *
 * @return		다음 단계가 없다면 False
 */
BOOL CMagicEffect::NextStep()
{
	PART* pMagic = *m_itorList;
	Assert( pMagic );

	if( !pMagic->bShow )
	{
		m_dwTimeForDraw = 0;
	} //if

	m_dwTimeForTail = 0;

	//m_vMove = RelativeCoordinates( m_vTarget, m_vPos, pMagic->vPos ); 
	if( pMagic->bBone )
	{
		if( m_pModel )
		{
			Matrix4 matBone = m_pModel->GetBoneMatrix( pMagic->szBone, m_pModel->GetTime() );
			Matrix4 matWorld = matBone * m_matWorld;

			m_vMove = matWorld.GetPosition();
			m_qRot = matWorld.GetQuaternion();
		} //if
	} //if

	m_itorList++;

	// 마지막 파트까지 왔다면...
	if( m_itorList == m_listMagic.end() || !(*m_itorList) )
	{
		return FALSE;
	} //if

	pMagic = (*m_itorList);
	Assert( pMagic );
	if( !pMagic )	return FALSE;

	m_dwTimeParticle = pMagic->dwParticleDelay;

	if( !pMagic->bAttack )
	{
		/*
		*/
	//	pMagic->vTarget += m_vMove;
	} //if

	if( pMagic->bEnemyPos )
	{
		m_vMove = RelativeCoordinates( m_vTarget, m_vPos, pMagic->vPos ); 
	//	pMagic->vTarget = m_vPos;
	} //if

	if( pMagic->bBezier )
	{
		m_bezier.Init();
		m_bezier.v1		= m_vMove;
		m_bezier.v2 = RelativeCoordinates( m_vMove, m_vTarget, pMagic->vBezier2 );
		m_bezier.v3 = RelativeCoordinates( m_vMove, m_vTarget, pMagic->vBezier3 );
	} //if

	if( pMagic->dwShake > 0 )
	{
		Magic::Shake( pMagic->dwShake );
	} //if

	m_btType = pMagic->btType;

	m_dwPartTime	= 0;
	m_bBlowTiming	= TRUE;
	m_dwStartTime	= pMagic->dwStartTime;
	m_bSound		= pMagic->bSound;
	m_dwSpeed		= pMagic->dwSpeed;

	m_pAttacker		= NULL;

	return TRUE;
} //CMagicEffect::NextStep

/**
 * 좌표 갱신 및 단계 이동을 한다.
 *
 * @return	이펙트가 완료 되지 않았는지의 여부 ( FALSE가 완료 )
 */
BOOL CMagicEffect::Update(
	const DWORD		dwTick,		///< [IN] 경과 tick
	const Vector3&	vTarget,	///< [IN] 상대방 크리쳐
	IW3DParticles*	pParticles,	///< [IN] 파티클 시스템
	Matrix4			matWorld,
	Matrix4			matTargetWorld,	///< [IN] 타겟의 월드 매트릭스
	DWORD			dwTime,
	DWORD			dwTargetTime
	)
{
	Assert( pParticles );
	// 각각의 Tick을 누적한다.
	m_matWorld = matWorld;

	m_dwTick = dwTick;
	m_dwTime += dwTick;
	m_dwPartTime += dwTick;

	static PART* pMagic = NULL;
	pMagic = (*m_itorList);
	Assert( pMagic );

	if( !pMagic )
	{
		return FALSE;
	} //if

	if( m_dwTime < m_dwStartTime )
	{	// 시작 타임이 지나지 않았다.
		m_bDraw = FALSE;
		return TRUE;
	} //if

	m_dwTime -= m_dwStartTime;
	m_dwStartTime = 0;

	if( m_dwTime >= pMagic->dwLimit && pMagic->dwLimit != 0 )
	{
		if( !NextStep() )
		{
			return FALSE;
		} //if

		m_dwTime -=  pMagic->dwLimit;
		pMagic = (*m_itorList);
	} //if

	// Play Sound
	if( m_bSound )
	{
		if( pMagic->dwSoundDelay <= m_dwPartTime )
		{	// 사운드 재생
			SoundPlay( pMagic->btType, pMagic->szSound );
			m_bSound = FALSE;
		} //if
	} //if

	if( m_pSound )
	{
		m_pSound->Update3D( dwTick );
	}

	if( pMagic->bBlowTiming && m_bBlowTiming )
	{ // 타격 타이밍이 존재 한다면
		if( pMagic->blowTiming.btType == 0 && 
			pMagic->blowTiming.dwTime <= m_dwPartTime )
		{ // 타입이 정적이고, 정해진 시간안이라면, 
			Assert( m_pSound );
			m_pSound->SetPosition( m_vPos );
			m_pSound->Play3D( pMagic->blowTiming.szSound );

			m_bBlowTiming = FALSE;

			if( m_pAttacker )
			{
				m_pAttacker->ShowDamageForTarget();
			} //if
		} //if	
	} //if

	m_dwTimeForDraw += dwTick;

	if( pMagic->bTail )
	{
		m_dwTimeForTail += dwTick;
	} //if

	if( pMagic->bParticle )
	{
		m_dwTimeParticle += dwTick;
	} //if

	m_bDraw = TRUE;

	m_vEnemyPos = vTarget;

	if( pMagic->bEnemyPos )
	{
		if( pMagic->bBone && m_pTargetModel )
		{
			Matrix4 matBone = m_pTargetModel->GetBoneMatrix( pMagic->szBone, dwTargetTime );
			matTargetWorld = matBone * matTargetWorld;

			m_vMove = matTargetWorld.GetPosition();
			m_qRot = matTargetWorld.GetQuaternion();
			return TRUE;
		} //if

		m_vTarget = RelativeCoordinates( vTarget, m_vPos, pMagic->vTarget );
		UpdateEnemyPos( pParticles );

		return TRUE;
	} //if

	if( pMagic->dwLimit == 0 )
	{
		if( !Move( pMagic, vTarget ) )
		{
			return FALSE;
		} //if
	}
	else
	{
		if( pMagic->bBone )
		{
			if( m_pModel )
			{
				Matrix4 matBone = m_pModel->GetBoneMatrix( pMagic->szBone, dwTime );
				matWorld = matBone * matWorld;

				m_vMove = matWorld.GetPosition();
				m_qRot = matWorld.GetQuaternion();
			} //if
		}
		else
		{
			Vector3	vDir;

			if( m_vTarget == m_vPos )
			{
				vDir = Vector3( 0.0F, 1.0F, 0.0F );
			}
			else
			{
				vDir = m_vTarget - m_vPos;
			} //if..else..

			m_qRot = GetDir( vDir );
		} 
		
	} //if..else

	EtcEffect( pMagic, pParticles );

	m_dwTime += m_dwStartTime;


	return TRUE;
} //CMagicEffect::Update

/**
 *
 */
BOOL CMagicEffect::Move( PART* pMagic, const Vector3& vTarget )
{
	Assert( pMagic );
	m_vTarget = RelativeCoordinates( m_vPos, vTarget, pMagic->vTarget );

	if( pMagic->bAttack )
	{
		// 공격이다.
		m_vTarget = RelativeCoordinates( vTarget, m_vPos, pMagic->vTarget );
	} //if

	// 추적용이다..+_+
	if( m_btType == 1 )
	{
		// 직선 이동
		if( Tracking() )
		{	
			if( pMagic->bBlowTiming &&
				pMagic->blowTiming.btType == 1 )
			{ // 타격 타이밍이 존재 하고, 그 타입이 동적일때
				m_pSound->SetPosition( m_vPos );
				m_pSound->Play3D( pMagic->blowTiming.szSound );

				if( m_pAttacker )
					m_pAttacker->ShowDamageForTarget();
			} //if

			// 추적이 끝났다면..
			if( !NextStep() )
			{
				return FALSE;
			} //if
			
			m_dwTime = 0;
			pMagic = (*m_itorList);
			Assert( pMagic );
		} //if
	}
	else if( m_btType == 2 )
	{
		// 매개 변수 f를 구한다.. 그래봤자..0에서 1사이값을 결정해주는 것일뿐
		float f = (float)( pMagic->dwSpeed ) / 10000;
				
		if( !m_bezier.Move( f ) || ( m_vTarget - m_vMove ).Length() <= 0.5F )
		{
			m_btType = 1;
			m_dwSpeed = 1000;
			return TRUE;
		} //if
		
		static Vector3	vDir;
		vDir = m_vMove;

		if( vDir == Vector3( 0.0F, 0.0F, 0.0F ) )
		{
			vDir.y = 1.0F;
		} //if

		m_bezier.v4 = m_vTarget;
		m_vMove = m_bezier.GetBezier();

		vDir -= m_vMove;

		m_qRot = GetDir( vDir * -1 );
	}
	else if( m_btType == 3 )
	{

	}//if..else if..

	return TRUE;
} //CMagicEffect::Move

/**
 *	기타 Effect.
 */
void CMagicEffect::EtcEffect( PART* pMagic, IW3DParticles* pParticles )
{
	Assert( pMagic );
	Assert( pParticles );
	Assert( m_pTrace );

	if( m_dwTrace && m_pTrace )
	{
		// 꼬리를 남긴다면
		Vector3 v1 = m_vMove;
		Vector3 v2 = m_vMove;

		v1.y -= pMagic->fTraceDist;
		v2.y += pMagic->fTraceDist;
		
		m_pTrace->InsertPoint( m_dwTrace, v1, v2 );
	} //if

	if( pMagic->bParticle )
	{
		if( m_dwTimeParticle >= pMagic->dwParticleDelay )
		{
			Vector3 vParticlePos = m_vMove;

			pParticles->AddParticle( pMagic->szParticle, vParticlePos );
			m_dwTimeParticle -= pMagic->dwParticleDelay;
		} //if
	} //if

/*	if( pMagic->pBillboard )
	{
		pMagic->pBillboard->Update( m_dwTick );
	} //if*/
} //CMagicEffect::EtcEffect

/**
 * 좌표들을 갱신한다. 
 */
void CMagicEffect::UpdateEnemyPos( IW3DParticles* pParticles )
{
	static PART*	pMagic = NULL;
	pMagic = *m_itorList;

	if( !pMagic )
	{
		return ; 
	}//if

	Vector3	vDir;
	
	if( m_vTarget == m_vPos )
	{
		vDir = Vector3( 0.0F, 1.0F, 0.0F );
	}
	else
	{
		vDir = m_vTarget - m_vPos;
	} //if..else..


	// Private stationary spell layers stay level instead of tilting into ground.
	// SPEED is otherwise unused by the [ENEMY] branch; moving parts are unchanged.
	if( pMagic->dwSpeed == 140031 ) vDir.z = 0.0F;
	m_qRot = GetDir( vDir );
	m_vMove = m_vTarget;

	if( pMagic->bParticle )
	{
		static Vector3 vParticle;
		vParticle = m_vMove;

		pParticles->AddParticle( pMagic->szParticle, vParticle );
	} //if

	m_dwTime += m_dwStartTime;
} //CMagicEffect::UpdateEnemyPos

/**
 * 그려준다.
 *
 * @return	완료 되었는지의 여부 ( FALSE가 완료 )
 */
BOOL CMagicEffect::Draw()
{
	PART* pMagic = *m_itorList;
	Assert( pMagic );

	if( !pMagic )	return TRUE;


	DrawTail();

	if( pMagic->bShowModel && m_pEffectData->pModel )
	{
		m_dwTimeForModel += m_dwTick;
		m_pEffectData->pModel->SetWorld( m_vPos, m_qRot );
		m_pEffectData->pModel->Render( m_dwTimeForModel );
	} //if
	
	if( m_bDraw && m_pEffectData->bSpecialEffect && pMagic->bShow )
	{
		if( !m_pEffectData->pEffect )
		{
			return TRUE;
		} //if

		IW3DSpecialEffect* pEffect = m_pEffectData->pEffect;

		static Matrix4		matWorld;
		matWorld.SetWorld( m_vMove, m_qRot );

		DWORD dwFrame = m_dwTimeForDraw * FRAME_RATE / 1000;

		if( !pEffect->Render( dwFrame, matWorld ) )
		{
			if( m_pEffectData->bLoop || pMagic->bLoop )
			{
				m_dwTimeForDraw = 0;
				pEffect->Render( m_dwTimeForDraw, matWorld );
			} //if
			
			if( pMagic->bPause )
			{
				dwFrame = pEffect->GetMaxFrame() - 1;
				pEffect->Render( dwFrame, matWorld );
				return TRUE;
			} //if
		} //if
	} //if

	if( pMagic->pBillboard )
	{
		pMagic->pBillboard->Draw( m_vMove, m_dwTime - m_dwStartTime );
	} //if

	return TRUE;	// 성공적으로 그렸음.
} //CMagicEffect::Draw

/**
 * 꼬리를 그린다.
 */
void CMagicEffect::DrawTail()
{
	PART*	pMagic = *m_itorList;
	Assert( pMagic );

	if( !pMagic || !pMagic->bTail || !m_bDraw )		return;

	static Matrix4		matWorld;
	matWorld.SetWorld( m_vMove, m_qRot );

	if( !pMagic->pTailEffect->Render( m_dwTimeForTail * FRAME_RATE / 1000, matWorld ) )
	{
		if( m_pEffectData->bLoop || pMagic->bLoop )
		{
			m_dwTimeForTail = 0;
			pMagic->pTailEffect->Render( m_dwTimeForTail * FRAME_RATE / 1000, matWorld );
		} //if
	} //if
} //CMagicEffect::DrawTail

/**
 * 타겟까지 이동한다.
 *
 * @return	이동이 완료 되었는지의 여부 ( TRUE가 완료 )
 */
BOOL CMagicEffect::Tracking()
{
	static PART* pMagic = NULL;
	pMagic = *m_itorList;

	Assert( pMagic );
	
	static Vector3	vDist;
	vDist = m_vTarget - m_vPos;

	float	fDistance = vDist.Length();
	float	fOffset = (float)( m_dwSpeed ) / 1000;

	vDist = m_vTarget - m_vMove;
	fDistance = vDist.Length();

	if( m_vTarget == m_vMove || fDistance <= fOffset )
	{
		m_vMove = m_vTarget;
		return TRUE;
	} //if

//	static Vector3 vDir;
//	vDir = ( vDist ).Normal();
	vDist.Normalize();

	if( vDist == Vector3( 0.0F, 0.0F, 0.0F ) )
	{
		return TRUE;
	} //if

/*	if( fDistance < fOffset )
	{
		fOffset = fDistance + 0.2F;
	} //if*/

	m_vMove += vDist * fOffset;
	m_qRot = GetDir( ( m_vTarget - m_vMove ) );

	return FALSE;
} //CMagicEffect::Tracking

/**
 * Part를 추가 한다.
 */
void CMagicEffect::AddPart( PART* pPart )
{
	Assert( pPart );
	m_listMagic.push_back( pPart );
} //CMagicEffect::

/**
 * Sound Play
 */
void CMagicEffect::SoundPlay( BYTE btType, char* szSound )
{	
	m_pSound->SetPosition( m_vPos );
	m_pSound->Set3DStyle( btType );
	m_pSound->Play3D( szSound );
} // CMagicEffect::SoundPlay
