/**
 * 평면.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-06-13 9:16p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DPlane.h $
 * 
 * 3     03-06-13 9:16p Kjmgo
 * 
 * 2     03-06-12 5:25p Kjmgo
 * Plane 추가
 * <HR>
 *
 * @file	IW3DPlane.h
 */

#ifndef __Wind3D_Render_IW3DPlane_Header__
#define __Wind3D_Render_IW3DPlane_Header__

/**
 * 평면 컴포넌트 인터페이스.
 * @ingroup		Render
 */
class IW3DPlane : public IComponent
{
public:

	/**
	 *
	 */
	virtual BOOL Create( const float fWidth, const float fHeight, const DWORD dwColor ) = 0;

	/**
	 *
	 */
	virtual void SetWorld( const Vector3& vPos ) = 0;

	/**
	 *
	 */
	virtual void SetWorld( const Vector3& vPos, const Quaternion& qRot ) = 0;

	/**
	 *
	 */
	virtual void SetWorld( const Matrix4& matWorld ) = 0;


	/**
	 *
	 */
	virtual void SetTexture( const char* szTexutre ) = 0;

	/**
	 *
	 */
	virtual Vector3 GetPosition() = 0;

	/**
	 *
	 */
	virtual BOOL IsVisible() = 0;

	/**
	 *
	 */
	virtual void Draw() = 0;

}; //


#endif //__Wind3D_Render_IW3DPlane_Header__
