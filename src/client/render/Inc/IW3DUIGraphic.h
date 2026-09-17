/**
 * UI 그래픽 인터페이스 헤더
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란
 * @date	$Date: 04-06-10 5:32p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DUIGraphic.h $
 * 
 * 3     04-06-10 5:32p Paran
 * 
 * 2     04-05-29 10:06p Paran
 * 
 * <HR>
 *
 * @file	IW3DUIGraphic.h
 */


struct tagHUIG{};
typedef CHandle<tagHUIG>	HUIG;

class IW3DUIGraphic : public IComponent
{
public:
	/**
	 * Initilize 
	 */
	virtual void		Init() = 0;

	/**
	 * Create Sprite
	 */
	virtual HUIG		CreateSprite( 
		const char* szTexFile, 
		const int nX, 
		const int nY, 
		const int nWidth, 
		const int nHeight,
		const int nSrcX,
		const int nSrcY,
		const int nSrcWidth,
		const int nSrcHeight
	) = 0;

	/**
	 * Create Sprite 
	 */
	virtual HUIG		CreateSprite( 
		const char* szTexFile, 
		const int nX, 
		const int nY, 
		const int nWidth, 
		const int nHeight
	) = 0;
	
	/**
	 * Release Sprite
	 */
	virtual void		ReleaseSprite( HUIG hUIG ) = 0;
	
	/**
	 * Move the UIG 
	 */
	virtual void		Move( HUIG hUIG, int nX, int nY ) = 0;

	/**
	 * Draw Sprite
	 */
	virtual void		DrawSprite( HUIG hUIG ) = 0;

	/**
	 * Render
	 */
	virtual void		Render() = 0;
}; //class IW3DUIGraphic
