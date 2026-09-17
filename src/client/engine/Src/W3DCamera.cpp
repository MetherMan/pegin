/**
 * 카메라.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-05-31 9:32p $
 * @version	$Revision: 47 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DCamera.cpp $
 * 
 * 47    04-05-31 9:32p Paran2
 * 
 * 46    04-05-31 6:57a Kjmgo
 * 
 * 45    04-04-07 4:29p Paran2
 * 
 * 44    04-03-10 6:17p Paran
 * 기능조물 추가
 * 
 * 43    04-03-03 5:28p Kjmgo			Min Pitch 적용
 *
 * 42    04-02-06 4:13p Kjmgo			카메라 적용
 * 
 * 37    03-12-03 5:27p Kjmgo			카메라 흔들기 수정
 * 
 * 36    03-12-03 1:47p Kjmgo			카메라 흔들기 약간 수정 ( 카메라 좌우로 흔들기로 )
 * 
 * 35    03-12-03 10:27a Kjmgo			카메라 흔들기 추가
 * 
 * 34    03-12-02 10:21p Kjmgo			카메라를 흔드는 기능 추가
 * 
 * 33    03-11-28 5:38p Kjmgo			절투체 위치를 카메라 위치로 세팅
 * 
 * 32    03-11-12 5:42p Kjmgo			Far Plane을 종정 65에서, 90으로 확장
 * 
 * 29    03-10-12 9:44p Kjmgo			시야관련 수정 
 * 
 * 28    03-10-08 2:20p Kjmgo			최대 맥스 줌이 35미터 였던것을 50미터로 수정 
 * 
 * 23    03-07-29 9:40p Kjmgo			시야를 내렸음
 * 
 * 21    03-07-05 5:10p Paran			카메라 지형 제한.
 * 
 * 15    03-05-22 11:07a Ahastudio		MinPitch 추가.
 *
 * 6     03-03-15 5:16a Ahastudio		Yaw 값의 범위를 0~360으로 제한, overflow로 인한 카메라 정지를 막음.
 *
 * 5     03-03-13 5:21p Ahastudio		GetRight() 버그 수정.
 *
 * 4     03-03-12 9:25p Ahastudio		SetFarPlane() 추가.
 *
 * 3     03-02-25 11:06a Ahastudio		SetMaxZoom() 추가.
 *
 * 2     03-02-22 10:07a Ahastudio		Camera 컴포넌트 추가.
 * <HR>
 *
 * @file	W3DCamera.cpp
 */


#include "Engine_Export.h"


/**
 * 카메라 클래스.
 * 카메라 이동, View Matrix 설정 등을 담당한다.
 * @ingroup		Engine
 */
class CW3DCamera : public IW3DCamera
{
public:
	enum { STATE_STOP = 0, STATE_FULLSHOT = 1, STATE_CLOSESHOT = 2, STATE_SHAKE = 3 };

	UNDER_MEMORY_MGR( CW3DCamera );

	/**
	 * 생성자.
	 * @todo	Scene 객체를 멋대로 얻는다. 반드시 안전하게 고칠 것.
	 */
	CW3DCamera()
	:	m_fDistance	( 100.0F ),
		m_fPitch	( 45.0F ),
		m_fYawRange	( 90.0F ),
		m_bFreeYaw	( TRUE	),
		m_fYaw		( 0.0F ),
	//	m_fFarPlane	( 60.0F ),
		m_fFarPlane	( 100.0F ),
		m_dwShakeTime( 0 ),
		m_fMaxZoom	( 35.0F ),
		m_fMinPitch	( 5.0F ),
		//m_fMinPitch	( 25.0F ),
		m_fMaxMapZoom( 35.0F ),
		m_wFovAngle	( 30 ),
		m_pWorld	( NULL ),
		m_nState	( STATE_FULLSHOT )
	{
		m_pScene	= MAKE_COMPONENT( Scene );

		m_vEye	= Vector3( 0.0F, m_fDistance, 0.0F );
		m_vAt	= Vector3( 0.0F, 0.0F, 0.0F );
		m_vUp	= Vector3( 0.0F, 0.0F, 1.0F );

		m_vEye.RotateX( ANGLE( m_fPitch ) );
		m_vEye.RotateZ( ANGLE( m_fYaw ) );
		m_vEye += m_vAt;

		m_fShake = 1.0F;
	} //CW3DCamera

	/**
	 * 소멸자.
	 */
	~CW3DCamera()
	{
	} //~CW3DCamera

	/**
	 * 카메라를 View Matrix에 적용한다.
	 */
	void	Execute()
	{
		if( m_pWorld )
		{
			Vector3	vCheck  = m_vEye - Vector3( 0.0F, 0.0F, 1.0F );
			float	fHeight = m_pWorld->GetHeight( vCheck );
			if( fHeight >= vCheck.z )
			{
				float	fAngle = (float)atan2( m_fDistance, fHeight - vCheck.z ) * 180.0F / MATH_PI;
				fAngle = 90 - fAngle;
				RotateX( -fAngle );
			} //if
		} //if

		m_pScene->GetFrustum().m_vPos = GetPosition();

		m_pScene->SetProjection( ANGLE( m_wFovAngle ), 0.1F, m_fFarPlane );
		m_pScene->LookAt( m_vEye, m_vAt, m_vUp );
	} //Execute

	/**
	 * 월드 
	 */
	void	SetWorldObject( const IW3DWorld* pWorld )
	{
		m_pWorld = const_cast<IW3DWorld*>( pWorld );
	} //SetWorld

	/**
	 * Fov 설정
	 */
	void	SetFov( const WORD wAngle )
	{
		m_wFovAngle = wAngle;
	} //SetFov

	/**
	 *
	 */
	void SetMinPitch( float fMinPitch )
	{
		m_fMinPitch = fMinPitch;
	} //SetMinPitch

	/**
	 * 카메라가 바라볼 위치 지정한다.
	 *
	 * @param	vPos		[IN] 위치
	 */
	void	SetPosition( const Vector3& vPos )
	{
		Vector3		vOffset = vPos - GetPosition();

		m_vAt  += vOffset;
		m_vEye += vOffset;		
	} //SetPosition

	/**
	 * 카메라가 바라보고 있는 위치를 얻는다.
	 *
	 * @return	위치
	 */
	Vector3		GetPosition()
	{
		return m_vAt - Vector3( 0.0F, 0.0F, 1.0F );
	} //GetPosition

	/**
	 * 카메라의 현재 위치를 얻는다.
	 *
	 * @return	위치
	 */
	Vector3		GetEye()
	{
		return m_vEye;
	} //GetEye

	/**
	 * 카메라가 바라보는 방향을 얻는다.
	 *
	 * @return	카메라가 바라보는 방향
	 */
	Vector3		GetFront()
	{
		return ( m_vAt - m_vEye ).Normal();
	} //GetFront

	/**
	 * 카메라의 오른쪽 방향 벡터를 얻는다.
	 *
	 * @return	카메라의 오른쪽 방향 벡터
	 */
	Vector3		GetRight()
	{
		Vector3 vDir = m_vAt - m_vEye;
		vDir.Normalize();

		return m_vUp.CrossProduct( vDir ).Normal();
	} //GetRight

	/**
	 * View Matrix를 얻는다.
	 *
	 * @return	View Matrix
	 */
	Matrix4		GetViewMatrix()
	{
		Matrix4		matView;
		matView.SetView( m_vEye, ( m_vAt - m_vEye ), m_vUp );
		return matView;
	} //GetViewMatrix

	/**
	 * 월드좌표 
	 */
	void		SetWorld( const Matrix4& matWorld )
	{
		Matrix4		matUp = matWorld;
		matUp._41 = 0.0F;
		matUp._42 = 0.0F;
		matUp._43 = 0.0F;
		m_vEye = Vector3( 0.0F, m_fDistance, 0.0F ) * matWorld;
		m_vAt  = Vector3( 0.0F, 0.0F, 0.0F ) * matWorld;
		m_vUp  = Vector3( 0.0F, 0.0F, 1.0F ) * matUp;
	} //SetWorld

	/**
	 * 카메라의 X축을 중심으로 회전한다.
	 * World 상의 X축이 아니라, 카메라 로컬 좌표계의 X축이라는 점에 주의하자.
	 *
	 * @param	fAngle		[IN] 각도
	 */
	void	RotateX( float fAngle )
	{
		m_fPitch -= fAngle;
		if( m_fPitch < m_fMinPitch )	m_fPitch = m_fMinPitch;
		if( m_fPitch > 85.0F )			m_fPitch = 85.0F;
	
		m_vEye	= Vector3( 0.0F, m_fDistance, 0.0F );

		m_vEye.RotateX( ANGLE( m_fPitch ) );
		m_vEye.RotateZ( ANGLE( m_fYaw ) );
		m_vEye += m_vAt;

	} //RotateX

	/**
	 * Z축을 중심으로 회전한다.
	 *
	 * @param	fAngle		[IN] 각도
	 */
	void	RotateZ( float fAngle )
	{
		m_fYaw += fAngle;

		if( m_bFreeYaw )
		{
			while( m_fYaw < 0.0F )
			{
				m_fYaw += 360.0F;
			} //while
			while( m_fYaw > 360.0F )
			{
				m_fYaw -= 360.0F;
			} //while
		}
		else
		{
			if( m_fYaw < m_fYawRange - 10.0F )
			{
				m_fYaw = m_fYawRange - 10.0F;
			} //

			if( m_fYaw > m_fYawRange + 10.0F )
			{
				m_fYaw = m_fYawRange + 10.0F ;
			} //
		} //if .. else .. 

		m_vEye	= Vector3( 0.0F, m_fDistance, 0.0F );

		m_vEye.RotateX( ANGLE( m_fPitch ) );
		m_vEye.RotateZ( ANGLE( m_fYaw ) );
		m_vEye += m_vAt;
	} //RotateZ

	/**
	 * 줌 인/아웃.
	 *
	 * @param	fOffset		[IN] 줌 offset
	 */
	void	Zoom( float fOffset )
	{
		if( STATE_CLOSESHOT == m_nState )
		{
			return;
		} //if

		if( fOffset == 0.0F )
		{
			return;
		} //if

		m_fDistance -= fOffset;

		if( m_fDistance < 2.0F )		m_fDistance = 2.0F;
		if( m_fDistance > m_fMaxZoom )	m_fDistance = m_fMaxZoom;

	/*	m_fMinPitch = 10.0F + m_fDistance / 3;

		if( m_fPitch < m_fMinPitch )	m_fPitch = m_fMinPitch;
		if( m_fPitch > 85.0F )			m_fPitch = 85.0F;*/

		m_vEye	= Vector3( 0.0F, m_fDistance, 0.0F );

		m_vEye.RotateX( ANGLE( m_fPitch ) );
		m_vEye.RotateZ( ANGLE( m_fYaw ) );
		m_vEye += m_vAt;
	} //Zoom

	/**
	 * Zoom 거리를 얻어 온다.
	 *
	 */
	float		GetZoom()
	{
		return m_fDistance;
	} //GetZoom

	/**
	 * Zoom 거리를 설정.
	 */
	void		SetZoom( const float fDist )
	{
		m_fDistance = fDist;
	} //SetZoom

	/**
	 * 시점자로부터 Far Plane까지의 거리를 설정한다.
	 *
	 * @param	fFarPlane	[IN] far plane.
	 */
	void	SetFarPlane( float fFarPlane )
	{
		m_fFarPlane = fFarPlane;
	} //SetFarPlane
	
	/**
	 * 시점자로부터 Far Plane까지의 거리를 얻는다.
	 */
	const float GetFarPlane()
	{
		return m_fFarPlane;
	}

	/**
	 * 최대 줌 길이를 설정한다.
	 *
	 * @param	fOffset		[IN] 최대 줌 길이.
	 */
	void	SetMaxZoom( float fOffset )
	{
		m_fMaxZoom = fOffset;
	} //SetMaxZoom

	/**
	 *
	 */
	void	SetYawRange( float fRange )
	{
		m_fYawRange = fRange;
	} //SetYawRange

	/**
	 *
	 */
	void	SetFreeYaw( BOOL bFree )
	{	
		m_bFreeYaw = bFree;
	} //if

	/**
	 * 초기샷 (각만)..
	 */
	void	DefaultShot( const Vector3& vPos )
	{
		m_nState	= STATE_FULLSHOT;

		m_fYaw		= 0.0F;

		m_vEye		= Vector3( 0.0F, m_fDistance, 0.0F );
		m_vEye.RotateX( ANGLE( m_fPitch ) );
		m_vEye.RotateZ( ANGLE( m_fYaw ) );
		m_vEye += m_vAt;
	} //DefaultShot

	/**
	 * 클로즈 샷.
	 */
	void	CloseShot( const Vector3& vPos, const Vector3& vDir, BOOL bLock )
	{
		m_nState	 = STATE_CLOSESHOT;
		m_bCloseShotLock = bLock;
		m_vGoalPos	 = vPos;
		m_fGoalAngle = vDir.x;
	} //SetGoalPos

	/**
	 * 풀 샷.
	 */
	void	FullShot( const Vector3& vPos )
	{
		if( m_nState != STATE_FULLSHOT )
		{
			return;
		} //if

		SetPosition( vPos );
	} //FullShot

	/**
	 *
	 */
	void	Shake( const DWORD dwTime )
	{
		m_dwShakeTime = dwTime;
//		m_nState = STATE_SHAKE;
		m_fShake = 1.0F;
		m_vAtBackup = GetPosition();
	} //Shake

	/** 
	 * 업데이트.
	 */
	void	Update( const DWORD dwTick )
	{
		
		if( m_dwShakeTime > 0 )
		{
			ShakeCamera( dwTick );
		} //if

		switch( m_nState )
		{
		case STATE_CLOSESHOT:		Movement( dwTick );			break;
		case STATE_FULLSHOT:									break;
		case STATE_STOP:										break;
		//case STATE_SHAKE:			ShakeCamera( dwTick );		break;
		} //switch

		
	} //Update

	/**
	 * 이동 중.
	 */
	void	Movement( const DWORD dwTick )
	{
		Vector3		vCurrentPos	  = GetPosition();
		Vector3		vGoalDistance = m_vGoalPos - vCurrentPos;
		if( vGoalDistance.Length() <= MATH_EPSILON )
		{
			if( m_bCloseShotLock )		m_nState = STATE_STOP;
			else						m_nState = STATE_FULLSHOT;

			return;
		} //if

		float		fOffset		  = 10.0F * dwTick / 1000;
		Vector3		vDir		  = vGoalDistance.Normal();
		float		fLength		  = vGoalDistance.Length();
		if( fOffset > fLength )
		{
			fOffset = fLength;
		} //if
		float		fNumOffset = fLength / fOffset;

		Vector3		vNextPos = vCurrentPos + vDir * fOffset;

		Vector3		vDistance = vNextPos - vCurrentPos;
		vDistance.z = 0.0F;
		float		fDistance = vDistance.Length();
		if( fDistance > MATH_EPSILON )
		{
			SetPosition( vNextPos );
		} //if

		if( m_bCloseShotLock )
		{
			if( m_fDistance > 2.0F )
			{
				float fZoomDist = m_fDistance - 2.0F;			///< 최대 근접거리.
				m_fDistance -= fZoomDist / fNumOffset;
				if( m_fDistance < 2.0F )
				{
					m_fDistance = 2.0F;
				} //if

				m_vEye	= Vector3( 0.0F, m_fDistance, 0.0F );
				m_vEye.RotateX( ANGLE( m_fPitch ) );
				m_vEye.RotateZ( ANGLE( m_fYaw ) );
				m_vEye += m_vAt;
			} //if
		}
		else
		{
			if( m_fDistance < 10.0F )
			{
				float fZoomDist = 10.0F;					///< 기본 근접거리.
				m_fDistance += fZoomDist / fNumOffset;
				if( m_fDistance > 10.0F )
				{
					m_fDistance = 10.0F;
				} //if

				m_vEye	= Vector3( 0.0F, m_fDistance, 0.0F );
				m_vEye.RotateX( ANGLE( m_fPitch ) );
				m_vEye.RotateZ( ANGLE( m_fYaw ) );
				m_vEye += m_vAt;
			} //if
		} //if..else
	} //Movement

	/**
	 *
	 */
	void ShakeCamera( const DWORD dwTick )
	{
		/*
			Written Kjmgo
		*/
		static DWORD s_dwTime = 0;
		static DWORD s_dwTick = 0;
		static int nType = 1;
	//	static float f = 1.0F;

		/*
			천천히 흔들리게 하는 가중치 입니다.
		*/
		m_fShake = 3.25F - ( s_dwTime / 360.0F );//0.1F;
		
		// 가중치를 곱합니다.
		s_dwTime += dwTick * m_fShake;

		SetPosition( GetPosition() + ( Vector3( 0.4F, 0.4F, 0.1F ) * sinf( ANGLE( s_dwTime * 2 ) ) * -0.15F ) );
	
		// 360ms, 즉, 360도 라든지,
		// 가중치가 0.1 이하면, 중단 합니다.
		if( s_dwTime >= 360 ||
			m_fShake <= 0.1F 
			)
		{
			m_fShake = 1.0F;
			s_dwTime = 0;
			m_dwShakeTime = MAX( 0 , m_dwShakeTime - 1 );
			//SetPosition( m_vAtBackup );
			//m_nState = STATE_FULLSHOT;
		} //if
	} //Shake

	void SetMaxMapZoom( float fMaxZoom )
	{
		m_fMaxMapZoom = fMaxZoom;
	} //SetMaxMapZoom

	void SetMaxMapZoom()
	{
		m_fMaxZoom = m_fMaxMapZoom;
	} //SetMaxMapZoom

	const float GetMaxMapZoom()
	{
		return m_fMaxMapZoom;
	} //GetMaxMapZoom

private:
	IW3DScene*	m_pScene;		///< Scene 객체
	IW3DWorld*	m_pWorld;		///< World 객체

	Vector3		m_vEye;			///< 시점자의 위치
	Vector3		m_vAt;			///< 바라보는 곳
	Vector3		m_vUp;			///< 시점자의 위 (방향)
	Vector3		m_vAtBackup;
	WORD		m_wFovAngle;	///< Fov 각

	float		m_fDistance;	///< 카메라의 거리
	float		m_fPitch;		///< 피치
	float		m_fYaw;			///< Yaw

	BOOL		m_bFreeYaw;		///< Is Free Yaw?
	
	float		m_fYawRange;				///< Yaw Range
	float		m_fFarPlane;				///< Far Plane
	float		m_fMaxZoom;					///< 최대 줌 길이
	float		m_fMaxMapZoom;				///< 최대 맵 줌 
	float		m_fMinPitch;				///< 최소 피치
	float		m_fShake;

	int			m_nState;					///< 현재 카메라 동작상태.
	BOOL		m_bCloseShotLock;			///< 완료 후 클로즈 샷 모드 해제 여부
	Vector3		m_vGoalPos;					///< 이동 완료 점.
	float		m_fGoalAngle;				///< 피사체각

	DWORD		m_dwShakeTime;				///< 흔들려는 시간
}; //class CW3DCamera


IMPLEMENT_COMPONENT( CW3DCamera, Camera );
