/**
 * 빌보드.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-18 11:02a $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DBillboard.h $
 * 
 * 11    03-12-18 11:02a Kjmgo
 * Local Matrix 적용
 * 
 * 10    03-10-10 1:09a Kjmgo
 * 외부에서 프레임을 넣을수 있도록 추가
 * 
 * 9     03-08-28 11:20a Kjmgo
 * 
 * 8     03-08-04 11:21a Admin
 * <HR>
 *
 * @file	IW3DBillboard.h
 */


#ifndef __Wind3D_Render_IW3DBillboard_Header__
#define __Wind3D_Render_IW3DBillboard_Header__


/**
 * 빌보드 클래스.
 * @ingroup		Render
 */
class IW3DBillboard : public IComponent
{
public:
	/**
	 * 빌보드 생성.
	 *
	 * @return	생성 성공 여부
	 */
	virtual BOOL	Load( const char* szFileName ) = 0;

	/**
	 *
	 *
	 */
	virtual BOOL	Load( const char* szFileName, float fWidth, float fHeight ) = 0;

	/**
	 * 업데이트 ( 텍스처 애니메이션 )
	 *
	 */
	virtual void	Update( const DWORD dwTick ) = 0;

	/**
	 * 빌보드를 그린다.
	 * 빌보드의 가운데 아래 좌표를 이용한다.
	 *
	 * @param	vPos		[IN] 출력할 좌표.
	 */
	virtual void	Draw( const Vector3& vPos ) = 0;

	/**
	 *
	 *
	 *
	 *
	 */
	virtual void	SetLocal( const Matrix4& matLocal ) = 0;
	
	/**
	 * 빌보드를 그린다.
	 * 빌보드의 가운데 아래 좌표를 이용한다.
	 *
	 * @param	vPos		[IN] 출력할 좌표.
	 * @param	dwTime		[IN] 시간
	 */
	virtual void	Draw( const Vector3& vPos, DWORD dwTime ) = 0;
}; //class IW3DBillboard


#endif // #ifndef __Wind3D_Render_IW3DBillboard_Header__
