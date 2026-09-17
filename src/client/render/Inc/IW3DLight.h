/**
 * 광원.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-10-17 1:59p $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DLight.h $
 * 
 * 8     03-10-17 1:59p Kjmgo
 * Update함수 추가
 * 
 * 7     03-10-17 10:50a Kjmgo
 * Fade ( 보간 적용 ) 효과 추가
 * 
 * 6     03-10-17 10:02a Kjmgo
 * Light를 키고 끄는 함수 추가
 * 
 * 5     03-10-15 6:42p Kjmgo
 * 라이트 관리자 추가 ^-^ 
 * 
 * 4     03-04-23 2:11p Ahastudio
 * <HR>
 *
 * @file	IW3DLight.h
 */


#ifndef __Wind3D_Render_IW3DLight_Header__
#define __Wind3D_Render_IW3DLight_Header__

/**
 *
 */
class IW3DLightMgr : public IComponent
{
public:
	/**
	 *
	 */
	virtual void Init( int nCount ) = 0;

	/**
	 *
	 */
	virtual void Apply() = 0;

	/**
	 *
	 */
	virtual IW3DLight* GetLight( int nIndex ) = 0;

	/**
	 *
	 */
	virtual int	GetLightCount() = 0;

	/**
	 *
	 */
	virtual void Turn( int nIndex, BOOL bOn ) = 0;

	
}; //class IW3DLightMgr


/**
 * 광원 인터페이스 클래스.
 * @ingroup		Render
 */
class IW3DLight : public IComponent
{
public:
	/**
	 * 광원을 설정한다.
	 *
	 * @param	nIndex		[IN] 광원 인덱스. 0~7의 범위로 최대 8개까지 사용 가능하다.
	 */
	virtual void SetLight( int nIndex ) = 0;

	/**
	 * 점광원으로 설정한다.
	 */
	virtual void SetPoint() = 0;

	/**
	 * 집중 광원으로 설정한다.
	 */
	virtual void SetSpot() = 0;

	/**
	 * 방향성 관원으로 설정한다.
	 */
	virtual void SetDirectional() = 0;

	/**
	 * 분산색을 설정한다.
	 */
	virtual void SetDiffuse( DWORD dwColor ) = 0;

	/**
	 * 주변색을 설정한다.
	 */
	virtual void SetAmbient( DWORD dwColor ) = 0;

	/**
	 * 반사색을 설정한다.
	 */
	virtual void SetSpecular( DWORD dwColor ) = 0;

	/**
	 * 광원의 위치를 설정한다.
	 * 점 광원과 집중 광원에서 사용된다.
	 */
	virtual void SetPosition( const Vector3& vPos ) = 0;

	/**
	 * 광원의 방향을 설정한다.
	 * 방향성 광원과 집중 광원에서 사용된다.
	 */
	virtual void SetDirection( const Vector3& vDir ) = 0;

	/**
	 * 빛의 영향 거리를 설정한다.
	 * 점 광원과 집중 광원에서 사용된다.
	 */
	virtual void SetRange( float fRange ) = 0;

	/**
	 * 감쇠율을 설정한다.
	 */
	virtual void SetConstantAttenuation( float fAttenuation ) = 0;

	/**
	 * 감쇠율을 설정한다.
	 */
	virtual void SetLinearAttenuation( float fAttenuation ) = 0;

	/**
	 * 감쇠율을 설정한다.
	 */
	virtual void SetQuadraticAttenuation( float fAttenuation ) = 0;

	/**
	 * 페이드 이펙트를 사용할지의 여부
	 */
	virtual void SetFade( BOOL bOn ) = 0;

	/**
	 * 이펙트를 업데이트 한다.
	 */
	virtual void Update() = 0;
}; //class IW3DLight


#endif // #ifndef __Wind3D_Render_IW3DLight_Header__
