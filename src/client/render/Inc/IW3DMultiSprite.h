/**
 * 2D 멀티 스프라이트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란
 * @date	$Date: 04-05-29 10:06p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DMultiSprite.h $
 * 
 * 3     04-05-29 10:06p Paran
 * 
 * 2     04-04-29 5:30p Paran
 * MultiSprite 추가
 * 
 * <HR>
 *
 * @file	IW3DMultiSprite.h
 */


#ifndef __Wind3D_Render_IW3DMultiSprite_Header__
#define __Wind3D_Render_IW3DMultiSprite_Header__


/**
 * 2D 멀티스프라이트 인터페이스.
 * @ingroup		Render
 */
class IW3DMultiSprite : public IComponent
{
public:
	/**
	 * 스프라이트 텍스처 읽기 및 가로 세로 길이 설정.
	 */
	virtual int		AddSprite(
		const char*		szFileName		///< [IN] 텍스처 파일명
		) = 0;

	/**
	 * 스프라이트 색상 설정.
	 */
	virtual void	SetColor( const DWORD dwColor ) = 0;

	/**
	 * 2D 스프라이트를 그린다.
	 */
	virtual void	Draw(
		const int	nX,					///< [IN] Drawing coord X
		const int	nY,					///< [IN] Drawing coord Y
		const int	nWidth,				///< [IN] Number X Couple Texture
		const int	nHeight,			///< [IN] Number Y Couple Texture
		const int	nBoxColor			///< [IN] Color Box
		) = 0;
}; //class IW3DMultiSprite


#endif // #ifndef __Wind3D_Render_IW3DMultiSprite_Header__
