/**
 * 스카이 박스
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-25 5:20p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DSky.h $
 * 
 * 3     04-03-25 5:20p Kjmgo
 * 
 * 2     04-03-25 3:49p Kjmgo
 * 
 * 1     04-03-25 3:25p Kjmgo
 * <HR>
 *
 * @file	IW3DSky.h
 */

#ifndef __Wind3D_Sky_Header__
#define __Wind3D_Sky_Header__

/**
 *
 */
class IW3DSky : public IComponent 
{
public:

	/**
	 *
	 */
	virtual BOOL Init( const char* szCloudTex ) = 0;

	/**
	 *
	 */
	virtual void Render( const DWORD dwTick, const Matrix4& matView ) = 0;

	/**
	 *
	 */
	virtual void RenderLens( const DWORD dwTick ) = 0;

}; //class IW3DSky


#endif //#ifndef __Wind3D_Sky_Header__
