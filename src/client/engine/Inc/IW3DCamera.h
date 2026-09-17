/**
 * 카메라 인터페이스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-03-03 5:28p $
 * @version	$Revision: 20 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DCamera.h $
 * 
 * 20    04-03-03 5:28p Kjmgo
 * Min Pitch 적용
 * 
 * 19    04-02-06 4:51p Kjmgo
 * MaxMapZoom에 관련한 함수 추가
 * 
 * 18    04-02-06 12:13p Kjmgo
 * 
 * 17    03-12-02 10:21p Kjmgo
 * 카메라를 흔드는 기능 추가
 * 
 * 16    03-09-09 11:14p Paran
 * 
 * 15    03-09-07 4:39a Paran
 * 
 * 14    03-09-01 7:19p Paran
 * 
 * 13    03-08-29 11:42p Paran
 * 
 * 12    03-07-05 5:10p Paran			카메라 지형 제한.
 * 
 * 4     03-03-12 9:25p Ahastudio		SetFarPlane() 추가.
 *
 * 3     03-02-25 11:06a Ahastudio		SetMaxZoom() 추가
 *
 * 2     03-02-22 10:07a Ahastudio		Camera 컴포넌트 추가.
 * <HR>
 *
 * @file	IW3DCamera.h
 */


#ifndef __Wind3D_Engine_Camera_Header__
#define __Wind3D_Engine_Camera_Header__


/**
 * 카메라 컴포넌트 인터페이스.
 * 카메라 이동, View Matrix 설정 등을 담당한다.
 * @ingroup		Engine
 */
class IW3DCamera : public IComponent
{
public:
	/**
	 * 카메라 세팅.
	 */
	virtual void		Execute() = 0;

	/**
	 * Fov 설정
	 */
	virtual void		SetFov( const WORD wAngle ) = 0;
	
	/**
	 * 월그객체 설정.
	 */
	virtual void		SetWorldObject( const IW3DWorld* pWorld ) = 0;

	/**
	 * 카메라가 바라볼 위치 지정한다.
	 */
	virtual void		SetPosition( const Vector3& vPos ) = 0;

	/**
	 * 카메라가 바라보고 있는 위치를 얻어옴.
	 */
	virtual Vector3		GetPosition() = 0;

	/**
	 * 카메라의 현재 위치를 얻어옴.
	 */
	virtual Vector3		GetEye() = 0;

	/**
	 * 카메라가 바라보는 방향을 얻어옴.
	 */
	virtual Vector3		GetFront() = 0;

	/**
	 * 카메라의 오른쪽 방향 벡터를 얻음.
	 */
	virtual Vector3		GetRight() = 0;

	/**
	 * 카메라의 View Matrix를 얻어옴.
	 */
	virtual Matrix4		GetViewMatrix() = 0;
	
	virtual void		SetWorld( const Matrix4& matWorld ) = 0;

	/**
	 * X축을 중심으로 회전한다.
	 * World 상의 X축이 아니라, 카메라 로컬 좌표계의 X축이라는 점에 주의하자.
	 */
	virtual void		RotateX( float fAngle ) = 0;

	/**
	 * Z축을 중심으로 회전.
	 */
	virtual void		RotateZ( float fAngle ) = 0;

	/**
	 * 줌 인/아웃.
	 */
	virtual void		Zoom( float fOffset ) = 0;

	/**
	 * 줌 의 거리를 얻는다.
	 */
	virtual float		GetZoom() = 0;

	/**
	 * 줌 의 거리설정.
	 */
	virtual void		SetZoom( const float fDist ) = 0;

	/**
	 * 초기샷 (각만)..
	 */
	virtual void		DefaultShot( const Vector3& vPos ) = 0;

	/**
	 * 클로즈 샷.
	 */
	virtual void		CloseShot( const Vector3& vPos, const Vector3& vDir, BOOL bLock ) = 0;

	/**
	 * 풀 샷.
	 */
	virtual void		FullShot( const Vector3& vPos ) = 0;

	/** 
	 * 업데이트.
	 */
	virtual void		Update( const DWORD dwTick ) = 0;	

	/**
	 * Far Plane 설정.
	 */
	virtual void		SetFarPlane( float fFarPlane ) = 0;

	/**
	 * Far Plane을 얻는다.
	 */
	virtual const float	GetFarPlane() = 0;

	/**
	 * 최대 줌 길이 설정.
	 */
	virtual void		SetMaxZoom( float fOffset ) = 0;

	/**
	 * 최대 줌 길이 설정.
	 */
	virtual void		SetMaxMapZoom( float fOffset ) = 0;

	/**
	 * 최대 줌 길이 설정.
	 */
	virtual void		SetMaxMapZoom() = 0;

	/**
	 *
	 */
	virtual const float	GetMaxMapZoom() = 0;

	/**
	 *
	 */
	virtual void		SetYawRange( float fRange ) = 0;

	/**
	 *
	 */
	virtual void		SetFreeYaw( BOOL bFree = TRUE ) = 0;

	/**
	 *
	 */
	virtual void		Shake( const DWORD dwTime ) = 0;

	/**
	 *
	 */
	virtual void		SetMinPitch( float fMinPitch ) = 0;
}; //class IW3DCamera


#endif // #ifndef __Wind3D_Engine_Camera_Header__
