/**
 * Effect가 있는 오브젝트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-06-02 8:36p $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DEffectObject.h $
 * 
 * 8     03-06-02 8:36p Kjmgo
 * 
 * 7     03-06-02 8:31p Kjmgo
 * 
 * 6     03-05-20 4:58p Kjmgo
 * 시간에 따른 이펙트 적용
 * 
 * 5     03-05-09 7:42a Ahastudio
 * <HR>
 *
 * @file	IW3DEffectObject.h
 */


#ifndef __Wind3D_Engine_EffectObject_Header__
#define __Wind3D_Engine_EffectObject_Header__


/**
 * Effect Object 인터페이스.
 * @ingroup		Engine
 */
class IW3DEffectObject : public IComponent
{
public:
	/**
	 * 오브젝트를 생성한다.
	 */
	virtual BOOL	Create(
		IW3DWorld*		pWorld,		///< [IN] 월드 객체
		IW3DModel*		pModel,		///< [IN] 모델
		const char*		szFile		///< [IN] 파일 이름
		) = 0;

	/**
	 * World Transform을 세팅해준다.
	 */
	virtual void	SetWorld(
		const Vector3&		vPos,		///< [IN] 위치
		const Quaternion&	qRot		///< [IN] 방향
		) = 0;

	/**
	 *
	 */
	virtual void	SetFront( BOOL bFlag ) = 0;

	/**
	 *
	 */
	virtual BOOL	GetFront() = 0;

	/**
	 * Time 을 설정한다.
	 */
	virtual void	SetTime( const int nTime ) = 0;

	/**
	 * 프레임 갱신을 한다.
	 *
	 * @param	dwTick		[IN] 경과 tick
	 *
	 * @return	...
	 */
	virtual BOOL	Update( const DWORD dwTick ) = 0;

	/**
	 * 오브젝트를 그려준다.
	 */
	virtual void	Draw() = 0;	

	/**
	 *
	 */
	virtual Vector3& GetPosition() = 0;

	/**
	 * 모델을 얻는다.
	 */
	virtual IW3DModel*	GetModel() = 0;
}; //class IW3DEffectObject


#endif //__Wind3D_Engine_EffectObject_Header__
