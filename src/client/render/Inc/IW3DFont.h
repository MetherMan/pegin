/**
 * 폰트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-07-16 11:53a $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DFont.h $
 * 
 * 10    03-07-16 11:53a Paran
 * 
 * 9     03-06-22 8:25p Paran
 * 
 * 8     03-04-09 11:43p Ahastudio
 * <HR>
 *
 * @file	IW3DFont.h
 */


#ifndef __Wind3D_Render_IW3DFont_Header__
#define __Wind3D_Render_IW3DFont_Header__


/**
 * 폰트 클래스의 인터페이스.
 * @ingroup		Render
 */
class IW3DFont : public IComponent
{
public:
	/**
	 * 폰트 초기화.
	 */
	virtual BOOL	Init(
		const char*		szName,		///< [IN] 폰트 이름
		const int		nSize,		///< [IN] 폰트 크기
		const long		lWeight,	///< [IN] 두께
		const BOOL		bULine,		///< [IN] 밑줄
		const BOOL		bItalic		///< [IN] 기울임
		) = 0;

	/**
	 * 클린업.
	 */
	virtual void	Cleanup() = 0;

	/**
	 * 폰트 다시 생성
	 */
	virtual BOOL	Restore() = 0;

	/**
	 * 문자열 사이즈.
	 */
	virtual BOOL	GetStringSize( int* pnWidth, int* pnHeight, const char* szText ) = 0;

	/**
	 * 문자열 출력.
	 */
	virtual void	DrawText(
		const char*		szText,		///< [IN] 문자열
		const RECT*		pRc,		///< [IN] 출력 영역
		const DWORD		dwAlign,	///< [IN] 정렬
		const DWORD		dwColor		///< [IN] 글자색
		) = 0;
}; //class IW3DFont


#endif // #ifndef __Wind3D_Render_IW3DFont_Header__
