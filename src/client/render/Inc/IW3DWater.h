/**
 * 수면 모듈 인터페이스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-08-08 2:37p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DWater.h $
 * 
 * 6     03-08-08 2:37p Admin
 * 
 * 5     03-03-13 4:00p Ahastudio
 * comment 정리.
 * <HR>
 *
 * @file	IW3DWater.h
 */


#ifndef __Wind3D_Render_IW3DWater_Header__
#define __Wind3D_Render_IW3DWater_Header__


/**
 * 수면 효과 클래스.
 */
class IW3DWater : public IComponent
{
public:
	/**
	 * 수면 생성.
	 *
	 * @param	szTextureName		[IN] 수면 텍스처 이름
	 * @param	nSize				[IN] 수면 크기
	 * @param	fHeight				[IN] 수면 높이
	 *
	 * @return	생성 성공 여부
	 */
	virtual BOOL	Create( char* szTextureName, int nCount, int nSize, float fHeight ) = 0;

	/**
	 * 수면 렌더링
	 *
	 * @param	dwTick				[IN] 경과 tick
	 */
	virtual void	Render( DWORD dwTick ) = 0;

	/**
	 * 수면 높이를 얻는다.
	 *
	 * @return	수면 높이
	 */
	virtual float	GetHeight() = 0;

	/**
	 * 위치 정해준다.
	 */
	virtual void    SetPosition( const Vector3& vPos ) = 0;
}; //class IW3DWater


#endif // #ifndef __Wind3D_Render_IW3DWater_Header__
