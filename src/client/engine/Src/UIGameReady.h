/**
 * 게임 준비 장면.
 * 맵 이름을 화면에 출력한다.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-05-03 11:07a $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Engine/Src/UIGameReady.h $
 * 
 * 5     03-05-03 11:07a Ahastudio
 * <HR>
 *
 * @file	UIGameReady.h
 */


#ifndef __Wind3D_Engine_UIGameReady_Header__
#define __Wind3D_Engine_UIGameReady_Header__


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

	BOOL			LoadImage( WORD wMap );
	void			ReleaseImage();
	void			Update( DWORD dwTick );
	void			Draw();

private:
	typedef map<WORD, string>		MAP_MAPNAME;

	MAP_MAPNAME		m_mapMapName;		///<
	IW3DSprite*		m_pMapName;			///<
	IW3DSprite*		m_pMapNameBack;		///<

	int				m_nState;			///<
	BYTE			m_btAlpha;			///<
	BYTE			m_btAlphaBack;		///<
	DWORD			m_dwTick;			///<
	DWORD			m_dwInterval;		///<
	BOOL			m_bShow;			///<
}; //class CUIGameReady


#endif // #ifndef __Wind3D_Engine_UIGameReady_Header__
