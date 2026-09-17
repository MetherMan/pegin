/**
 * 유저 인터페이스용 셀렉트 스트링 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-06-08 3:56p $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /UInterface/Src/UISelectString.cpp $
 * 
 * 13    04-06-08 3:56p Paran2
 * 
 * 12    04-05-21 8:36p Paran2
 * 
 * 11    04-05-08 2:34a Paran2
 * 
 * 10    04-05-07 9:53a Paran2
 * 
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UISelectString.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UISelectString.h"

#define SIZE_FONT		12

//
// UISelectString::UISelectString
//
UISelectString::UISelectString()
{
	m_prectSelect		= NULL;
	m_prcSelect			= NULL;
	m_nDisplayRow		= 0;
	m_nMaxCount			= 0;
	m_nLineIndex		= -1;
	m_dwTxtColor		= 0xffc4c4c4;
	m_nScrollPos		= 0;
}

//
// UISelectString::~UISelectString
//
UISelectString::~UISelectString()
{
	ReleaseAll();
}

void UISelectString::ReleaseAll()
{
	m_nScrollPos = 0;
	SAFE_RELEASE( m_prectSelect );
	SAFE_DELETE_ARRAY( m_prcSelect );
}

//
// UISelectString::Load
//
BOOL UISelectString::Init( const int nMaxCount )
{
	m_prectSelect = MAKE_COMPONENT( Rectangle );
	if( !m_prectSelect )
	{
		Assert( !"Rectangle 생성 실패" );
		return FALSE;
	} //if

	m_nMaxCount = nMaxCount;

	int		nHeight = m_rcRegion.bottom - m_rcRegion.top;
	m_nDisplayRow = nHeight / 15;
	if( m_nDisplayRow > nMaxCount )
	{
		m_nDisplayRow = nMaxCount;
	} //if

	SAFE_DELETE_ARRAY( m_prcSelect );
	m_prcSelect = new RECT[m_nDisplayRow];
	if( !m_prcSelect )
	{
		Assert( !"Allocate Not Memory" );
		return FALSE;
	} //if

	for( int nCnt = 0; nCnt < m_nDisplayRow; nCnt++)
	{
		m_prcSelect[nCnt].left		= m_rcRegion.left;
		m_prcSelect[nCnt].right		= m_rcRegion.right;
		m_prcSelect[nCnt].top		= m_rcRegion.top + ( nCnt * 15 );
		m_prcSelect[nCnt].bottom	= m_prcSelect[nCnt].top + 13;
	} //for
	
	return TRUE;
} //UISelectString::Init

/**
 * 문자 정렬 방식 설정.
 */
void UISelectString::SetAlign( int nAlign_ )
{
	switch( nAlign_ )
	{
	case ALIGN_CENTER:			m_dwAlign = DT_CENTER | DT_VCENTER;		break;
	case ALIGN_LEFT:			m_dwAlign = DT_LEFT | DT_VCENTER;		break;
	case ALIGN_RIGHT:			m_dwAlign = DT_RIGHT | DT_VCENTER;		break;		
	} //switch
} //UISelectString::SetAlign


/**
 *
 */
void UISelectString::Cleanup()
{
	m_listString.clear();
	m_nLineIndex  = -1;
	m_nScrollPos  = 0;
} //UISelectString::Cleanup

/**
 * 죄표 이동.
 */
void UISelectString::Move( int nOffsetX, int nOffsetY )
{
	UICComponent::Move( nOffsetX, nOffsetY );
	for( int nCnt = 0 ; nCnt < m_nDisplayRow ; nCnt++ )
	{
		m_prcSelect[nCnt].left		+= nOffsetX;
		m_prcSelect[nCnt].top		+= nOffsetY;
		m_prcSelect[nCnt].right		+= nOffsetX;
		m_prcSelect[nCnt].bottom	+= nOffsetY;
	}
}

/**
 * 
 */
BOOL UISelectString::PointIn( int nX, int nY )
{
	if( !IsShow() )
	{
		return FALSE;
	} //if

	return UICComponent::PointIn( nX, nY );
} //UISelectString::PointIn

/**
 *
 */
void UISelectString::Push( int bPush_, const int nX, const int nY )
{ 
	if( !bPush_ )
	{
		return;
	} //if

	POINT	ptMouse = { nX, nY };
	for( int nCnt = 0 ; nCnt < m_nDisplayRow ; nCnt++ )
	{
		if( PtInRect( &m_prcSelect[nCnt], ptMouse ) )
		{
			m_nLineIndex = nCnt;
			return;
		} //if
	} //for
	m_nLineIndex = -1;
} //UISelectString::Push

/**
 *
 */
int UISelectString::GetSelectIndex()
{
	if( m_nLineIndex < 0 )
	{
		return m_nLineIndex;
	} //if

	return m_nScrollPos + m_nLineIndex;
} //UISelectString::GetSelectIndex

/**
 * 현재 선택 되어진 텍스트를 얻는다.
 */
char* UISelectString::GetText()
{
	if( m_nLineIndex < 0 )
	{
		m_nLineIndex = 0;
	} //if

	int		nIdx = m_nScrollPos + m_nLineIndex;
	if( nIdx >= (int)m_listString.size() || m_listString.size() <= 0 ) 
	{
		return NULL;
	} //if

	COLORTEXT*						pColorText;
	LIST_COLORSTRING::iterator		iterList_String;

	iterList_String =  m_listString.begin();
	std::advance( iterList_String, nIdx );
	pColorText = &(*iterList_String);
	char*		szText = const_cast<char*>( pColorText->strText.c_str() );
	
	return szText;
} //UISelectString::GetText

//
// UISelectString::SetDisplayCount
//
void UISelectString::SetDisplayCount( int nDisplayRow_ )
{
	m_nDisplayRow = nDisplayRow_;

	SAFE_DELETE_ARRAY( m_prcSelect );
	m_prcSelect = new RECT[m_nDisplayRow];
	if( !m_prcSelect )
	{
		Assert( 0 );
		return;
	}

	for( int nCnt = 0; nCnt < m_nDisplayRow; nCnt++ )
	{
		m_prcSelect[nCnt].left		= m_rcRegion.left   + 5;
		m_prcSelect[nCnt].top		= m_rcRegion.top + ( SIZE_FONT * nCnt );
		m_prcSelect[nCnt].right		= m_rcRegion.right  - 5;
		m_prcSelect[nCnt].bottom	= m_prcSelect[nCnt].top + SIZE_FONT;
	}
}

//
// UISelectString::GetListSize
//
int UISelectString::GetListSize()
{
	return (int)m_listString.size();
}

/**
 * 스크롤 최대 값을 얻는다.
 */
int UISelectString::GetScrollMax()
{
	return m_listString.size() - m_nDisplayRow;
} //UISelectString::GetScrollMax

/**
 * 스크롤 위치 설정.
 */
void UISelectString::SetScrollPos( int nPos )
{
	if( GetScrollMax() < nPos )
	{
		return;
	} //if

	m_nScrollPos = nPos;
	m_nLineIndex = -1;
} //UISelectString::SetScrollPos

//
// UISelectString::Push
//
void UISelectString::AddString( const char* szContent, DWORD dwColor )
{
	if( (int)m_listString.size() >= m_nMaxCount )
	{
		m_listString.pop_front();
	}

	COLORTEXT		colorText;
	if( dwColor == 0 )		colorText.dwColor = m_dwTxtColor;
	else					colorText.dwColor = dwColor;
	
	colorText.strText = szContent;
	m_listString.push_back( colorText );
}

//
// UISelectString::DeleteString
//
void UISelectString::DeleteString( const char* szContent )
{
	COLORTEXT*		pColorText;

	LIST_COLORSTRING::iterator	iter = m_listString.begin();
	while( iter != m_listString.end() )
	{
		pColorText = &(*iter);
		if( pColorText->strText.find( szContent ) >= 0 )
		{
			m_listString.erase( iter );
			m_nLineIndex = -1;
			return;
		} //if
		iter++;
	} //while
} //UISelectString::DeleteString

//
// UISelectString::FindString
//
COLORTEXT* UISelectString::FindString( const char* szContent )
{
	COLORTEXT*		pColorText;
	int				nCount = 0;

	LIST_COLORSTRING::iterator	iter = m_listString.begin();
	while( iter != m_listString.end() )
	{
		pColorText = &(*iter);
		if( !pColorText->strText.compare( szContent ) )
		{
			return pColorText;
		} //if
		iter++;
		nCount++;
	} //while

	return NULL;
} //UISelectString::FindString


//
// UISelectString::Previous
//
void UISelectString::Previous()
{
	if( m_nLineIndex > 0 )
	{
		m_nLineIndex--;
	}
	else if( m_nLineIndex == 0 )
	{
		if( --m_nScrollPos < 0 )
		{
			m_nScrollPos = 0;
		}
	}
}

//
// UISelectString::Next
//
void UISelectString::Next()
{
	if( m_nLineIndex < m_nDisplayRow - 1 )
	{
		m_nLineIndex++;
	}
	else if( m_nLineIndex == m_nDisplayRow - 1 )
	{
		if( ++m_nScrollPos > (int)m_listString.size() - 1 )
		{
			m_nScrollPos = m_listString.size() - 1;
		}
	}
}

//
// UISelectString::SetLinePos
//
void UISelectString::SetLinePos( int nPos )
{
	m_nLineIndex = -1;
	m_nScrollPos = nPos;
	if( m_nScrollPos < 0 )
	{
		m_nScrollPos = 0;
	}
	else if( m_nScrollPos > (int)m_listString.size() - 1 )
	{
		m_nScrollPos = m_listString.size() - 1;
	}
}

//
// UISelectString::Draw
//
void UISelectString::Draw()
{
	if( !IsShow() )				return;
	if( m_listString.empty() )	return;

	if( m_nLineIndex >= 0 ) 
	{
		int		nX		= m_prcSelect[m_nLineIndex].left;
		int		nY		= m_prcSelect[m_nLineIndex].top;
		int		nWidth  = m_prcSelect[m_nLineIndex].right - m_prcSelect[m_nLineIndex].left;

		m_prectSelect->Draw( nX, nY, nWidth, SIZE_FONT, 0x640000AA );
	} //if

	COLORTEXT						colorText;
	LIST_COLORSTRING::iterator		iterList_String;

	iterList_String =  m_listString.begin();
	std::advance( iterList_String, m_nScrollPos );
	for( int nCnt = 0; nCnt < m_nDisplayRow; nCnt++ )
	{
		if( iterList_String == m_listString.end() )		break;

		colorText = (*iterList_String);
		m_pFont->DrawText( colorText.strText.c_str(), &m_prcSelect[nCnt], m_dwAlign, colorText.dwColor );

		iterList_String++;
	} //for
} //UISelectString::Draw
