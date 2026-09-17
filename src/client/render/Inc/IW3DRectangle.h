/**
 * 2D 사각.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-08-04 5:15p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DRectangle.h $
 * 
 * 2     03-08-04 5:15p Paran
 * W3DRectangle  추가
 * 
 * <HR>
 *
 * @file	IW3DRectangle.h
 */


#ifndef __Wind3D_Render_IW3DRectangle_Header__
#define __Wind3D_Render_IW3DRectangle_Header__


/**
 * 2D 사각 인터페이스.
 * @ingroup		Render
 */
class IW3DRectangle : public IComponent
{
public:
	/**
	 * 2D 사각을 그린다.
	 */
	virtual void			Draw( 
		const int nX, 
		const int nY, 
		const int nWidth, 
		const int nHeight, 
		const DWORD dwColor = 0xFFFFFFFF
		) = 0;
}; //IW3Rectangle

#endif //__Wind3D_Render_IW3DRectangle_Header__