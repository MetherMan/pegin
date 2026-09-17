/**
 * 유저 인터페이스용 스트링리스트 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-11-16 7:11p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIStringList.cpp $
 * 
 * 11    04-11-16 7:11p Sk8snow2
 * 
 * 10    04-04-10 12:50a Paran
 * 
 * 9     04-03-02 10:13p Paran
 * 
 * 8     04-02-10 3:19p Paran
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIStringList.h
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIStringList.h"
#include "ChattingIME.h"

	/**
	 * 생성자.
	 */
UIStringList::UIStringList()
{
	m_apFont[0]	  = NULL;
	m_apFont[1]	  = NULL;
	m_apFont[2]	  = NULL;
	m_apFont[3]	  = NULL;
	m_dwTxtColor  = 0xffffffff;
	m_nMaxCount	  = 0;
	m_nDisplayRow = 0;
	m_nScrollPos  = 0;
	m_dwAlign	  = DT_CENTER | DT_VCENTER;
} //UIStringList::UIStringList

//
// UIStringList::GetDisplayLineCount
//
int UIStringList::GetDisplayLineCount()
{
	return m_nDisplayRow;
} //UIStringList::GetDisplayLineCount

//
// UIStringList::SetCount
//
void UIStringList::SetCount( int nDisplayRow_ )
{
	m_nDisplayRow = nDisplayRow_;
	m_RowHeight = ( m_rcRegion.bottom - m_rcRegion.top ) / m_nDisplayRow;
}

//
// UIStringList::SetMax
//
void UIStringList::SetMax( int nMax_ )
{
	m_nMaxCount = nMax_;
}

//
// UIStringList::SetPos
//
void UIStringList::SetPos( int nPos_ )
{
	m_nScrollPos = nPos_;

	int		nLimit = m_listString.size() - m_nDisplayRow;
	if( nLimit < 0 )
	{
		nLimit = 0;
	} //if

	if( m_nScrollPos > nLimit )
	{
		m_nScrollPos = nLimit;
	}
	else if( m_nScrollPos < 0 ) 
	{
		m_nScrollPos = 0;
	} //if..else
	//m_nScrollPos = m_listString.size() - m_nScrollPos;
}

/**
 * 폰트 설정.
 */
void UIStringList::SetFont( IW3DFont* pFont, BYTE btIndex )
{
	m_apFont[btIndex] = pFont;
	if( btIndex == 0 )
	{
		m_apFont[1] = pFont;
		m_apFont[2] = pFont;
		m_apFont[3] = pFont;
	} //if
} //UIStringList::SetFont

/**
 * 정렬방식.
 */
void UIStringList::SetAlign( int nAlign_ )
{
	switch( nAlign_ )
	{
	case ALIGN_LEFT:		m_dwAlign	= DT_LEFT | DT_VCENTER;				break;
	case ALIGN_CENTER:		m_dwAlign	= DT_CENTER | DT_VCENTER;			break;
	case ALIGN_RIGHT:		m_dwAlign	= DT_RIGHT | DT_VCENTER;			break;
	} //switch
} //UIStringList::SetAlign

//
// UIStringList::GetListSize
//
int UIStringList::GetListSize()
{
	return (int)m_listString.size();
}

/**
 * 텍스트 설정.
 */
void UIStringList::SetText( const char* szContent, DWORD dwColor, BYTE btFontIndex )
{
	if( !szContent )
	{
		Assert( !"추가 내용 없당" );
		return;
	} //if

	int			nStrWidth, nStrHeight;
	int			nWindowWidth = m_rcRegion.right - m_rcRegion.left - 30;

	m_apFont[btFontIndex]->GetStringSize( &nStrWidth, &nStrHeight, szContent );
	if( nWindowWidth > nStrWidth )
	{ // 안에 다 들어간 문자열.
		AddString( szContent, dwColor );
		return;
	} //if

	char		szStrTemp[128];
	char*		pStartStr = const_cast<char*>( szContent );

	BOOL		bHan = IsDoubleByteCharset( pStartStr ) ? TRUE : FALSE;
	int			nCurrLength = bHan ? 2 : 1;
	int			nPos = 0;
	int			nLength = strlen( szContent );
	int			nLineNum = 0;

	while( nCurrLength <= nLength )
	{
		if( bHan )
		{
			memcpy( &szStrTemp[nPos], &pStartStr[nCurrLength-2], 2 );
			nPos += 2;
		}
		else
		{
			szStrTemp[nPos] = pStartStr[nCurrLength-1];
			nPos++;
		} //if..else
		szStrTemp[nPos] = '\0';

		m_apFont[btFontIndex]->GetStringSize( &nStrWidth, &nStrHeight, szStrTemp );
		if( nWindowWidth <= nStrWidth )
		{
			AddString( szStrTemp, dwColor );
			nLineNum++;
			nPos = 0;
		} //if

		if( IsDoubleByteCharset( &pStartStr[nCurrLength] ) )
		{
			bHan = TRUE;
			nCurrLength += 2;	
		}
		else
		{
			bHan = FALSE;
			nCurrLength++;
		} //if..else
	} //while

	if( nLineNum > 0 )		AddString( szStrTemp, dwColor );
} //UIStringList::SetText

//
// UIStringList::Push
//
void UIStringList::AddString( const char* szContent, DWORD dwColor, BYTE btFontIndex )
{
	if( !szContent )
	{
		Assert( !"추가 내용 없당" );
		return;
	} //if

	COLORTEXT		colorText;
	if( dwColor == 0 )		colorText.dwColor = m_dwTxtColor;
	else					colorText.dwColor = dwColor;

	colorText.btFontIndex = btFontIndex;

	if( (int)m_listString.size() >= m_nMaxCount )
	{
		m_listString.pop_back();
	} //if
	colorText.strText = szContent;
	m_listString.push_front( colorText );
}

//
// UIStringList::Cleanup
//
void UIStringList::Cleanup()
{
	UICComponent::Cleanup();
	m_listString.clear();
}

//
// UIStringList::Draw
//
void UIStringList::Draw()
{
	if( !IsShow() )				return;
	if( m_listString.empty() )	return;

	int			nCnt = 0;
	RECT		rcText;

	LIST_COLORSTRING::iterator	listIter_content;

	listIter_content =  m_listString.begin();
	std::advance( listIter_content, m_nScrollPos );

	COLORTEXT		colorText;

	for( nCnt = 1; nCnt <= m_nDisplayRow; nCnt++ )
	{
		if( listIter_content == m_listString.end() )	break;

		rcText.left		= m_rcRegion.left + 10;
		rcText.top		= ( m_rcRegion.bottom - 3 )- m_RowHeight * nCnt;
		rcText.right	= m_rcRegion.right - 10;
		rcText.bottom	= rcText.top + m_RowHeight;

		colorText = (*listIter_content);

		m_apFont[colorText.btFontIndex]->DrawText( 
			colorText.strText.c_str(), 
			&rcText, 
			m_dwAlign, 
			colorText.dwColor 
			);

		listIter_content++;
	}
}
