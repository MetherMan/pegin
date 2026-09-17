/**
 * 게임 준비 장면.
 * 맵 이름을 화면에 출력한다.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-08-13 10:33p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIGameReady.h $
 * 
 * 3     03-08-13 10:33p Paran
 * 
 * 2     03-08-12 11:56p Paran
 * 
 * 1     03-05-17 12:00a Paran
 * 
 * 5     03-05-03 11:07a Ahastudio
 * <HR>
 *
 * @file	UIGameReady.h
 */


#ifndef __Wind3D_UIGameReady_Header__
#define __Wind3D_UIGameReady_Header__


#include <string>
#include <map>


using std::string;
using std::map;


/**
 * 게임 준비 장면 클래스.
 * @ingroup		Engine
 */
class CUIGameReady
{
public:
	CUIGameReady();
	~CUIGameReady();

	void			Cleanup();
	BOOL			LoadImage( WORD wMap );
	void			ReleaseImage();
	void			Update( DWORD dwTick );
	void			Draw();

private:
	typedef map<WORD, string>		MAP_MAPNAME;

	IW3DSprite*		m_pMapName;			///<

	int				m_nWidth;			///< 이미지 가로.
	int				m_nHeight;			///< 이미지 세로.

	int				m_nState;			///<
	BYTE			m_btAlpha;			///<
	BYTE			m_btAlphaBack;		///<
	DWORD			m_dwTick;			///<
	DWORD			m_dwInterval;		///<
	BOOL			m_bShow;			///<
}; //class CUIGameReady


#endif // #ifndef __Wind3D_UIGameReady_Header__
