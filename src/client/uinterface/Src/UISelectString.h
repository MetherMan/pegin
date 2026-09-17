/**
 * 유저 인터페이스용 셀렉트 스트링 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-05-07 9:53a $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /UInterface/Src/UISelectString.h $
 * 
 * 8     04-05-07 9:53a Paran2
 * 
 * 2     03-05-17 4:20a Paran			각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UISelectString.h
 */

#ifndef __UISelectString_Header__
#define __UISelectString_Header__

#include <list>

/**
 * 유저 인터페이스 문자열 선택.
 */
class UISelectString : public UICComponent
{
public:
	UNDER_MEMORY_MGR( UISelectString )

	enum { ALIGN_LEFT = 0, ALIGN_CENTER = 1, ALIGN_RIGHT = 2 };

	UISelectString();
	~UISelectString();

	BOOL				Init( const int nMaxCount );
	
	void				ReleaseAll();
	void				Cleanup();
	
	void				Move( int nOffsetX, int nOffsetY );
	void				Push( int bPush_, const int nX = 0, const int nY = 0 );
	BOOL				PointIn( int nX, int nY );

	int					GetSelectIndex();
	char*				GetText();
	int					GetListSize();
	void				AddString( const char* szContent, DWORD dwColor = 0 );
	void				DeleteString( const char* szContent );
	COLORTEXT*			FindString( const char* szContent );
	
	void				Previous();
	void				Next();
	
	void				SetDisplayCount( int nDisplayRow_ );
	void				SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }
	void				SetAlign( int nAlign_);
	void				SetMax( int nMax_ )						{ m_nMaxCount = nMax_; }
	void				SetLinePos( int nPos );
	
	int					GetScrollMax();
	void				SetScrollPos( int nPos );
	
	void				Draw();

private:
	LIST_COLORSTRING	m_listString;
	IW3DRectangle*		m_prectSelect;
	IW3DFont*			m_pFont;
	DWORD				m_dwTxtColor;
	RECT*				m_prcSelect;
	DWORD				m_dwAlign;
	int					m_nDisplayRow;
	int					m_nMaxCount;
	int					m_nLineIndex;
	int					m_nBeginIndex;
	int					m_nScrollPos;
}; //class UISelectString

#endif //__UISelectString_Header__