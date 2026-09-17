/**
 * 유저 인터페이스용 탭 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-06-21 12:39p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UITabButton.h $
 * 
 * 3     03-06-21 12:39p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UITabButton.h
 */

#ifndef __UITabButton_Header__
#define __UITabButton_Header__

#include <string>
#include <vector>

/**
 * 유저 인터페이스 탭 버튼.
 */
class UITabButton : public UICComponent
{
public:
	UNDER_MEMORY_MGR( UITabButton );

	UITabButton();
	~UITabButton();

	void			ReleaseAll();
	BOOL			Load( const char* szFileName );
	void			AddTabInfo( const char* szName_, const RECT *pRect_ );
	void			SetCount( int nCount_ )					{ m_nTabCount = nCount_; }
	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }
	void			SetTxtColor( BYTE r, BYTE g, BYTE b )	{ m_dwTxtColor = 0xff000000 | r << 16 | g << 8 | b; }
	BOOL			SelectTab( int nX, int nY );
	BOOL			SelectTab( int nTab );
	int				GetTabIndex();
	void			Cleanup();
	void			Move( int nOffsetX, int nOffsetY );
	void			Draw();

	typedef std::vector<std::string>		VEC_STR;
	typedef std::vector<RECT>				VEC_RECT;
	typedef std::vector<IW3DSprite*>		VEC_ICON;

private:
	IW3DSprite*		m_pSelectTab;
	VEC_STR			m_vecName;
	VEC_RECT		m_vecRect;
	int				m_nTabIndex;
	int				m_nTabCount;
	IW3DFont*		m_pFont;
	DWORD			m_dwTxtColor;
}; //class UITabButton

#endif //__UITabButton_Header__