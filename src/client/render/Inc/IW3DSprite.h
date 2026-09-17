/**
 * 2D 스프라이트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-09-10 11:30p $
 * @version	$Revision: 14 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DSprite.h $
 * 
 * 14    04-09-10 11:30p Paran
 * 
 * 13    04-03-25 5:20p Kjmgo
 * 
 * 12    04-03-25 4:33p Kjmgo
 * 
 * 11    03-09-22 11:54a Kjmgo
 * 
 * 10    03-05-11 7:41p Kjmgo
 * 
 * 9     03-04-11 4:44a Ahastudio
 * <HR>
 *
 * @file	IW3DSprite.h
 */


#ifndef __Wind3D_Render_IW3DSprite_Header__
#define __Wind3D_Render_IW3DSprite_Header__


/**
 * 2D 스프라이트 인터페이스.
 * @ingroup		Render
 */
class IW3DSprite : public IComponent
{
public:
	/**
	 * 스프라이트 텍스처 읽기 및 가로 세로 길이 설정.
	 */
	virtual BOOL	Load(
		const char*		szFileName,		///< [IN] 텍스처 파일명
		const int		nWidth,			///< [IN] 가로 길이
		const int		nHeight,		///< [IN] 세로 길이
		const BOOL		bAutoSize = FALSE
		) = 0;

	/**
	 * 2D 스프라이트 크기 설정.
	 */
	virtual void	SetSize(
		const int	nWidth,
		const int	nHeight
		) = 0;

	/**
	 *
	 */
	virtual void	SetZoom( float fZoom ) = 0;

	/**
	 * 출력 보정 좌표 설정.
	 */
	virtual void	SetCenter(
		const int	nX,
		const int	nY
		) = 0;

	/**
	 *
	 */
	virtual void	SetAnimation(
		const int	nRow,
		const int	nCol,
		const DWORD	dwDelay
		) = 0;

	/**
	 *
	 */
	virtual void	Update(
		const DWORD	dwTick
		) = 0;

	/**
	 *
	 */
	virtual void	Stop() = 0;

	/**
	 *
	 */
	virtual void	Play() = 0;

	/**
	 * 스프라이트 색상 설정.
	 */
	virtual void	SetColor( const DWORD dwColor ) = 0;

	/**
	 * 2D 스프라이트를 그린다.
	 */
	virtual void	Draw(
		const int	nX,
		const int	nY
		) = 0;

	/**
	 *
	 */
	virtual void	DrawEffect(
		const int	nX,
		const int	nY
		) = 0;

	/**
	 * 2D 스프라이트를 그린다.
	 */
	virtual void	Draw(
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nSrcX,			///< [IN] 출력할 부분의 X 시작 좌표
		const int	nSrcY,			///< [IN] 출력할 부분의 Y 시작 좌표
		const int	nSrcWidth,		///< [IN] 출력할 부분의 가로 길이
		const int	nSrcHeight		///< [IN] 출력할 부분의 세로 길이
		) = 0;

	/**
	 * 2D 스프라이트를 그린다.
	 */
	virtual void	Draw(
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nWidth,			///< [IN] 가로 길이
		const int	nHeight,			///< [IN] 세로 길이
		const int	nSrcX,			///< [IN] 출력할 부분의 X 시작 좌표
		const int	nSrcY,			///< [IN] 출력할 부분의 Y 시작 좌표
		const int	nSrcWidth,		///< [IN] 출력할 부분의 가로 길이
		const int	nSrcHeight		///< [IN] 출력할 부분의 세로 길이
		) = 0;
	
	/**
	 * 2D 스프라이트 프레임을 그린다. 
	 */
	virtual void	DrawFrame(
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nFrame			///< [IN] 출력할 프레임
		) = 0;


	virtual int		GetWidth() = 0;
	virtual	int		GetHeight() = 0;
}; //class IW3DSprite


#endif // #ifndef __Wind3D_Render_IW3DSprite_Header__
