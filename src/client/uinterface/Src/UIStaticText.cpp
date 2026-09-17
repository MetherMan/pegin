/**
 * 유저 인터페이스용 문자열 입력모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-08-15 9:17p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIStaticText.cpp $
 * 
 * 4     03-08-15 9:17p Paran
 * 
 * 3     03-08-14 2:40p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIStaticText.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIStaticText.h"
#include <string>

//
// UIStaticText::UIStaticText
//
UIStaticText::UIStaticText()
{
	m_dwTxtColor = 0xffffffff;
	m_dwAlign	 = DT_CENTER | DT_VCENTER;
	m_strText	 = "";
	m_pFont		 = NULL;
}

//
// UIStaticText::Cleanup
//
void UIStaticText::Cleanup()
{
	UICComponent::Cleanup();
}

/**
 * 정렬방식.
 */
void UIStaticText::SetAlign( int nAlign_ )
{
	switch( nAlign_ )
	{
	case ALIGN_LEFT:		m_dwAlign	= DT_LEFT | DT_VCENTER;				break;
	case ALIGN_CENTER:		m_dwAlign	= DT_CENTER | DT_VCENTER;			break;
	case ALIGN_RIGHT:		m_dwAlign	= DT_RIGHT | DT_VCENTER;			break;
	} //switch
} //UIStaticText::SetAlign

//
// UIStaticText::SetText
//
void UIStaticText::SetText( const char* szText )
{
	if( !szText )	return;

	char		aBuff[256];
	char*		pCurr = aBuff;
	m_strText = "";

	for( int nCnt = 0 ; nCnt < (int)strlen( szText ) ; nCnt++ )
	{
		if( szText[nCnt] == '\\' && szText[nCnt + 1] == 'n' )
		{
			*pCurr = '\n';
			pCurr++;
			nCnt++;
			continue;
		}
		*pCurr = szText[nCnt];
		pCurr++;
	}
	*pCurr = '\0';
	m_strText = aBuff;
}

//
// UIStaticText::Draw
//
void UIStaticText::Draw()
{
	Assert( m_pFont );
	
	if( !IsShow() || !m_strText.length() )		return;

	m_pFont->DrawText( m_strText.c_str(), &m_rcRegion, m_dwAlign, m_dwTxtColor );
}