/**
 * 유저 인터페이스용 문자열 입력모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-24 6:09p $
 * @version	$Revision: 16 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIEditText.cpp $
 * 
 * 16    03-12-24 6:09p Paran
 * 
 * 15    03-12-22 1:08a Paran
 * 
 * 14    03-12-17 6:27a Paran
 * 
 * 13    03-12-15 10:52p Paran
 * 
 * 12    03-12-14 3:23a Paran
 * 
 * 11    03-12-13 3:24p Sckyj
 * 한글버젼에서는 EditOn, EditOff시에 SetConversion, SetContext를 호출하지
 * 않도록( 한글, 영문이 자동으로 변환하지 않도록 ) 수정
 * 
 * 10    03-12-04 9:14p Sckyj
 * EditOn, EditOff() 수정 - SetConversion()
 * 
 * 9     03-12-03 2:35p Sckyj
 * EditOn(), EditOff()함수에서 g_ChatIME.SetContext()함수 호출하도록 추가
 * 
 * 8     03-11-28 6:08p Paran
 * 
 * 7     03-11-28 1:46p Paran
 * 
 * 6     03-11-25 10:25p Paran
 * IME 기능수정 및 추가
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
#include "UIEditText.h"
#include "Chattingime.h"
#include <string>

//
// UIEditText::UIEditText
//
UIEditText::UIEditText()
{
	m_dwTxtColor	= 0xffffffff;
	m_dwAlign		= DT_CENTER | DT_VCENTER;
	m_pText			= NULL;
	m_pResult		= NULL;
	m_pFont			= NULL;
	m_bEditing		= FALSE;
	m_bPw			= FALSE;
	m_nShowBegin	= 0;
	m_nShowEnd		= 0;
	m_nLimit		= 0;
	memset( m_szComp, 0, 3 );
} //UIEditText::UIEditText

/**
 * 소멸자
 */
UIEditText::~UIEditText()
{
	SAFE_FREE( m_pText );
	SAFE_FREE( m_pResult );
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

/**
 * 사용용량 할당
 */
void UIEditText::SetLimit( UINT nLimit_ )
{
	SAFE_FREE( m_pText );
	SAFE_FREE( m_pResult );

	m_pText		= (char*)_MemAlloc( nLimit_+4 );
	m_pResult	= (char*)_MemAlloc( nLimit_+4 );
	if( !m_pText || !m_pResult )
	{
		SAFE_FREE( m_pText );
		SAFE_FREE( m_pResult );
		Assert( !"할당실패" );
		return;
	} //if

	memset( m_pText, 0, nLimit_+4 );
	memset( m_pResult, 0, nLimit_+4 );

	m_nLimit = nLimit_;
} //UIEditText::SetLimit
			
/**
 * 편집시작.
 */
void UIEditText::EditOn()
{
	m_bEditing = TRUE;

	g_ChatIME.SetLimit( m_nLimit );
	g_ChatIME.SetString( GetText() );

/*#ifdef _CHINAVERSION
	g_ChatIME.SetContext( TRUE );
	g_ChatIME.SetConversion( TRUE );	
#endif*/

	if( GetMaskMode() )
	{
		g_ChatIME.SetCharMode();
	} //if

	Reflesh();
} //UIEditText::EditOn

/**
 * 편집끝.
 */
void UIEditText::EditOff()
{
	m_bEditing = FALSE;

	if( strlen( m_pText ) > m_nLimit )
	{
		Assert( !"범위이상" );
		return;
	} //if

	UINT		nLength = strlen( m_pResult );
	if( m_bPw && nLength )
	{
		int		nInsertPos = g_ChatIME.GetInsertPos();

		SetText( g_ChatIME.GetString() );
		SetCompositionText( g_ChatIME.GetCompositionString() );
		ResetShowCharPos();

		strcpy( m_pResult, m_pText + m_nShowBegin );
		m_pResult[m_nShowEnd - m_nShowBegin] = '\0';
		if( strlen( m_szComp ) > 0 )
		{
			m_szComp[2] = '\0';
			char*		pInsert = m_pResult + nInsertPos;
			strcpy( pInsert, m_szComp ); 
			strcat( m_pResult, &m_pText[nInsertPos] );
		} //if

		strcpy( m_pText, m_pResult );
		for( UINT nCnt = 0; nCnt < nLength; nCnt++ )
		{
			m_pResult[nCnt] = '*';
		} //for
	} //if
	
	g_ChatIME.EndData();
	memset( m_szComp, 0, 3 );
	g_ChatIME.ClearData();

/*#ifdef _CHINAVERSION
	g_ChatIME.SetContext( FALSE );
	g_ChatIME.SetConversion( FALSE );
#endif*/

} //UIEditText::EditOn

//
// UIEditText::Cleanup
//
void UIEditText::Cleanup()
{
	UICComponent::Cleanup();

	m_nShowBegin = 0;
	m_nShowEnd   = 0;
	memset( m_pText,	0, m_nLimit+1 );
	memset( m_pResult,	0, m_nLimit+1 );
	memset( m_szComp,	0, 3 );
	g_ChatIME.ClearData();
} //UIEditText::Cleanup

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
	if( strlen( szText_ ) > m_nLimit - 2 )
	{
		return;
	} //if

	strcpy( m_pText, szText_ );
	strcpy( m_pResult, m_pText );

	ResetShowCharPos();
} //UIEditText::SetText

/**
 * 텍스트 붙이기
 */
void UIEditText::InsertText( const char* szText, int nInsertPos )
{
	Assert( szText );

	int		nLength = strlen( m_pText ) + strlen( szText ) + nInsertPos;
	if( nLength > m_nLimit - 2 )
	{
		return;
	} //if

	if( strlen( m_pText ) < nInsertPos )
	{
		strcat( m_pText, szText );
	} //if
	strcpy( m_pResult, &m_pText[nInsertPos] );
	strcpy( &m_pText[nInsertPos], szText );
	strcat( &m_pText[nInsertPos], m_pResult );
	strcpy( m_pResult, m_pText );
} //UIEditText::SetInsertText

/**
 * 텍스트 부분 삭제
 */
void UIEditText::DeleteText( int nDeletePos, int nDeleteCnt )
{
	int			nTextLength  = strlen( m_pText );
	if( nTextLength == 0 )
	{
		return;
	} //if

	if( nDeletePos >= nTextLength )
	{
		Assert( !"영역이상" );
		return;
	} //if

	int			nTailTextPos = nDeletePos + nDeleteCnt;
	if( nTextLength < nTailTextPos )
	{
		nTailTextPos = nTextLength;
	} //if

	strcpy( m_pResult, &m_pText[nTailTextPos] );
	m_pText[nDeletePos] = '\0';
	strcat( m_pText, m_pResult );
	strcpy( m_pResult, m_pText );
} //UIEditText::SetDeleteText

/**
 * 보여지는 문자위치 다시
 */
void UIEditText::ResetShowCharPos()
{
	int			nStrWidth, nStrHeight;
	int			nWindowWidth = m_rcRegion.right - m_rcRegion.left - 13;

	char		*pCurrStr;
	char		*pStartStr = m_pText;

	int			nLength = 0;
	int			nTotalLength = strlen( m_pText );
	while( nLength < nTotalLength )
	{
		pCurrStr = pStartStr + nLength;
		m_pFont->GetStringSize( &nStrWidth, &nStrHeight, pCurrStr );

		if( nWindowWidth > nStrWidth )
		{
			m_nShowBegin = nLength;
			m_nShowEnd	 = nTotalLength;
			break;
		} //if

		if( IsDoubleByteCharset( pCurrStr ) )
		{
			nLength += 2;	
		}
		else
		{
			nLength++;
		} //if..else
	} //while

	int		nInsertPos = g_ChatIME.GetInsertPos();
	if( nInsertPos < m_nShowBegin )
	{
		m_nShowBegin = nInsertPos;
	} //if
} //UIEditText::ResetShowCharPos

/**
 * 완성한 문자열.
 */
char* UIEditText::GetText()
{
	return m_pText;
} //UIEditText::GetText

/**
 * 업뎃.
 */
/*
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
*/

/**
 * 내부 텍스트 다시 처리
 */
void UIEditText::Reflesh()
{
	if( !m_bEditing )
	{
		return;
	} //if

	char*	pText;
	if( GetLoginMode() == TRUE )
		pText = strlwr( g_ChatIME.GetString() );
	else
		pText = g_ChatIME.GetString();

	if( strlen( pText ) > m_nLimit - 2 )
	{
		return;
	} //if

	int		nInsertPos = g_ChatIME.GetInsertPos();

	strcpy( m_pText, pText );
	strcat( m_pText, g_ChatIME.GetCompositionString() );
	ResetShowCharPos();

	strcpy( m_pResult, m_pText + m_nShowBegin );
	m_pResult[m_nShowEnd - m_nShowBegin] = '\0';
	if( strlen( m_szComp ) > 0 )
	{
		m_szComp[2] = '\0';
		char*		pInsert = m_pResult + nInsertPos;
		strcpy( pInsert, m_szComp ); 
		strcat( m_pResult, &m_pText[nInsertPos] );
	} //if

	UINT		nLength = strlen( m_pResult );
	if( m_bPw && nLength )
	{
		for( UINT nCnt = 0; nCnt < nLength; nCnt++ )
		{
			m_pResult[nCnt] = '*';
		} //for
	} //if

	char	chTemp = m_pText[nInsertPos];
	m_pText[nInsertPos] = '\0';

	int		nWidth, nHeight;
	m_pFont->GetStringSize( &nWidth, &nHeight, m_pText + m_nShowBegin );
	m_pText[nInsertPos] = chTemp;

	int		nX = m_rcRegion.left + nWidth; 
	g_ChatIME.SetCaretPos( nX+1, m_rcRegion.top );
} //UIEditText::Reflesh

//
// UIEditText::Draw
//
void UIEditText::Draw()
{
	if( !IsShow() )		return;

	m_pFont->DrawText( m_pResult, &m_rcRegion, m_dwAlign, m_dwTxtColor );
}
