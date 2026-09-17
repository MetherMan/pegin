/**
 * 흔적 효과 인터페이스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-05-14 2:07a $
 * @version	$Revision: 16 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DTraceEffect.h $
 * 
 * 16    03-05-14 2:07a Kjmgo
 * 
 * 15    03-03-12 3:21p Ahastudio
 * Texture 부분 수정 및 주석 정리.
 * <HR>
 *
 * @file	IW3DTraceEffect.h
 */


#ifndef __Wind3D_Render_IW3DTraceEffect_Header__
#define __Wind3D_Render_IW3DTraceEffect_Header__


/**
 * 흔적 효과 컴포넌트 클래스 인터페이스.
 *
 * @author	로냐
 * @date	2003-01-02 오후 7:22
 */
class IW3DTraceEffect : public IComponent
{
public:
	/**
	 *
	 */
	virtual BOOL	Init()	= 0;

	/**
	 *
	 */
	virtual BOOL	Load( const char* szFileName ) = 0;

	/**
	 *
	 */
	virtual void	Update( DWORD dwTick ) = 0; 

	/**
	 *
	 */
	virtual void	Render() = 0;

	/**
	 *
	 */
	virtual BOOL	CreateEffect( const char* szTypeName, const DWORD dwKey ) = 0;

	/**
	 *
	 */
	virtual BOOL	ChangeType( const DWORD dwKey, const char* szTypeName ) = 0;

	/**
	 *
	 */
	virtual void	PauseEffect( const DWORD dwKey ) = 0;

	/**
	 *
	 */
	virtual void	DeleteEffect( const DWORD dwKey ) = 0;

	/**
	 *
	 */
	virtual void	InsertPoint(
									const DWORD dwKey,			///< 키 이름.
									const Vector3& vStart,		///< 포인트 #1
									const Vector3& vEnd			///< 포인트 #2
								) = 0;
}; //class IW3DTraceEffect


#endif // #ifndef __Wind3D_Render_IW3DTraceEffect_Header__
