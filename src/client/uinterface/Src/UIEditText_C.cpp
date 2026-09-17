/**
 * 유저 인터페이스용 문자열 입력모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-11-19 8:08p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIEditText_C.cpp $
 * 
 * 2     03-11-19 8:08p Sckyj
 * 중국어 IME관련 작업 진행중
 * 
 * 5     03-08-19 11:14a Paran
 * 
 * 4     03-08-15 8:14p Paran
 * 
 * 3     03-08-14 3:59a Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIEditText.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIEditText_C.h"
#include <string>

//
// UIEditText::UIEditText
//
UIEditText::UIEditText()
{
	m_dwTxtColor	= 0xffffffff;
	m_dwAlign		= DT_CENTER | DT_VCENTER;
	m_strText		= "";
	m_pFont			= NULL;
	m_bPw			= FALSE;
	m_nShowLimit	= 0;
	memset( m_szComp, 0, 3 );
} //UIEditText::UIEditText

/**
 * 정렬설정.
 */
void UIEditText::SetAlign( int nAlign_ )
{
	switch( nAlign_ )
	{
	case ALIGN_CENTER:		m_dwAlign = DT_CENTER | DT_VCENTER;			return;
	case ALIGN_LEFT:		m_dwAlign =	DT_LEFT | DT_VCENTER;			return;
	case ALIGN_RIGHT:		m_dwAlign =	DT_RIGHT | DT_VCENTER;			return;
	} //switch
} //UIEditText::SetAlign

//
// UIEditText::LeaveFocus
//
void UIEditText::LeaveFocus()
{
	if( m_szComp[0] != '_' )
	{
		m_szComp[2] = '\0';
		m_strText.append( m_szComp );
	}
	memset( m_szComp, 0, 3 );
}

//
// UIEditText::Cleanup
//
void UIEditText::Cleanup()
{
	UICComponent::Cleanup();

	m_nShowLimit = 0;
	m_strText	= "";
	m_strResult	= "";
	memset( m_szComp, 0, 3 );
}

/** 
 * 조합중인 문자 설정.
 */
void UIEditText::SetCompositionText( const char* szComp_ ) 
{ 
	memcpy( m_szComp, szComp_, 3 );
} //UIEditText::SetCompositionText

/**
 * 텍스트 설정.
 */
void UIEditText::SetText( const char* szText_ )		
{ 
	m_strText = szText_; 

	int			nStrWidth, nStrHeight;
	int			nWindowWidth = m_rcRegion.right - m_rcRegion.left - 13;

	char		*pCurrStr;
	char		*pStartStr = const_cast<char*>( m_strText.c_str() );

	int nLength = 0;
	while( nLength < m_strText.length() )
	{
		pCurrStr = pStartStr + nLength;
		m_pFont->GetStringSize( &nStrWidth, &nStrHeight, pCurrStr );

		if( nWindowWidth > nStrWidth )
		{
			m_nShowLimit = nLength;
			break;
		} //if

		if( IsHangul( pCurrStr ) )		nLength += 2;	
		else							nLength++;
	} //for
} //UIEditText::SetText

/**
 * 한글인지 체크.
 */
BOOL UIEditText::IsHangul( char* pBuff )
{
	if( ( pBuff[0] & 0x80 ) && ( pBuff[1] & 0x80 ) )		return TRUE;
	else													return FALSE;
} //UIEditText::IsHangul

/**
 * 완성한 문자열.
 */
const char* UIEditText::GetText()
{
	return m_strText.c_str(); 
} //UIEditText::GetText

/**
 * 업뎃.
 */
void UIEditText::Update( const DWORD dwTick )
{
	if( !IsShow() )		return;

	m_strResult = m_strText;
	if( m_szComp[0] != '\0' )
	{
		m_strResult.append( m_szComp );
	} //if

	if( m_bPw && m_strResult.length() )
	{
		UINT		nLength = m_strResult.length();
		for( UINT nCnt = 0; nCnt < nLength; nCnt++ )
		{
			if( m_strResult[nCnt] != '_' )
			{
				m_strResult[nCnt] = '*';
			} //if
		} //for
	} //if
} //UIEditText::Update

//
// UIEditText::Draw
//
void UIEditText::Draw()
{
	if( !IsShow() )		return;

	char		*pBeginStr = const_cast<char*>( m_strResult.c_str() ) + m_nShowLimit;
	m_pFont->DrawText( pBeginStr, &m_rcRegion, m_dwAlign, m_dwTxtColor );
}
