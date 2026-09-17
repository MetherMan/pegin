/**
 * 유저 인터페이스용 체크버튼 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-08-14 2:40p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UICheckButton.cpp $
 * 
 * 3     03-08-14 2:40p Paran
 * 
 * 2     03-07-16 7:30p Paran
 * 
 * <HR>
 *
 * @file	UICheckButton.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UICheckButton.h"
#include <string>

/**
 * 생성자.
 */
UICheckButton::UICheckButton()
{
	m_dwTxtColor	= 0xffffffff;
	m_pSpriteUP		= NULL;
	m_pSpriteDN		= NULL;
	m_bPush			= FALSE;
	m_bCheck		= FALSE;
	m_dwAlign		= DT_CENTER | DT_VCENTER;
} //UICheckButton::UICheckButton

/**
 * 소멸자.
 */
UICheckButton::~UICheckButton()
{
	ReleaseAll();
} //UICheckButton::~UICheckButton

/**
 * 릴리즈.
 */
void UICheckButton::ReleaseAll()
{
	SAFE_RELEASE( m_pSpriteUP );
	SAFE_RELEASE( m_pSpriteDN );
} //UICheckButton::ReleaseAll

/**
 * 클린업.
 */
void UICheckButton::Cleanup()
{
	UICComponent::Cleanup();
	Push( FALSE );
} //UICheckButton::Cleanup

/**
 * 로드.
 */
BOOL UICheckButton::Load( const char* szFileNameDN, const char* szFileNameUP )
{
	int		nWidth, nHeight;
	nWidth  = m_rcRegion.right - m_rcRegion.left;
	nHeight = m_rcRegion.bottom - m_rcRegion.top;

	m_pSpriteUP = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pSpriteUP );

	if( !m_pSpriteUP->Load( szFileNameUP, nWidth, nHeight ) )
	{
		Assert( 0 );
		return FALSE;
	}

	m_pSpriteDN = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pSpriteDN );

	if( !m_pSpriteDN->Load( szFileNameDN, nWidth, nHeight ) )
	{
		Assert( 0 );
		return FALSE;
	}

	return TRUE;
} //UICheckButton::Load

/**
 * 푸쉬
 */
void UICheckButton::Push( int bPush_ )
{
	if( !m_bEnable )
	{
		return;
	}

	m_bPush = bPush_; 
} //UICheckButton::Push

/**
 * 정렬방식.
 */
void UICheckButton::SetAlign( int nAlign_ )
{
	switch( nAlign_ )
	{
	case ALIGN_LEFT:		m_dwAlign	= DT_LEFT | DT_VCENTER;				break;
	case ALIGN_CENTER:		m_dwAlign	= DT_CENTER | DT_VCENTER;			break;
	case ALIGN_RIGHT:		m_dwAlign	= DT_RIGHT | DT_VCENTER;			break;
	} //switch
} //UICheckButton::SetAlign

/**
 * 체크.
 */
void UICheckButton::Check( BOOL bCheck )
{
	if( !m_bEnable )
	{
		return;
	}

	m_bCheck = bCheck;
} //UICheckButton::Check

/**
 * 그리기.
 */
void UICheckButton::Draw()
{
	Assert( m_pSpriteDN );
	Assert( m_pSpriteUP );

	if( !IsShow() )		return;
	
	if( m_bEnable )
	{
		if( m_bCheck )	m_pSpriteDN->Draw( m_rcRegion.left, m_rcRegion.top );
		else			m_pSpriteUP->Draw( m_rcRegion.left, m_rcRegion.top );
	} //if

	if( m_strText.length() > 0 )
	{
		m_pFont->DrawText( m_strText.c_str(), &m_rcRegion, m_dwAlign, m_dwTxtColor ); 
	} //if
} //UICheckButton::Draw