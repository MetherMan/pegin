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
 * $Log: /UInterface/Src/UITabButton.cpp $
 * 
 * 3     03-06-21 12:39p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UITabButton.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UITabButton.h"

//
// UITabButton::UITabButton
//
UITabButton::UITabButton()
{
	m_dwTxtColor	= 0xffffffff;
	m_pSelectTab	= NULL;
	m_pFont			= NULL;
	m_nTabIndex		= 0;
}

//
// UITabButton::~UITabButton
//
UITabButton::~UITabButton()
{
	ReleaseAll();
}

//
// UITabButton::ReleaseAll
//
void UITabButton::ReleaseAll()
{
	SAFE_RELEASE( m_pSelectTab );
}

//
// UITabButton::Cleanup
//
void UITabButton::Cleanup()
{
	UICComponent::Cleanup();
	m_nTabIndex		= 0;
}

//
// UITabButton::Load
//
BOOL UITabButton::Load( const char* szFileName )
{
	Assert( szFileName );

	int		nWidth, nHeight;
	nWidth  = m_vecRect[0].right - m_vecRect[0].left;
	nHeight = m_vecRect[0].bottom - m_vecRect[0].top;

	m_pSelectTab = MAKE_COMPONENT( Sprite );
	if( !m_pSelectTab )
	{
		Assert( 0 );
		return FALSE;
	} //if

	if( !m_pSelectTab->Load( szFileName, nWidth, nHeight ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	return TRUE;
} //UITabButton::Load

void UITabButton::Move( int nOffsetX, int nOffsetY )
{
	UICComponent::Move( nOffsetX, nOffsetY );
	
	VEC_RECT::iterator		iterVec_Rect = m_vecRect.begin();
	RECT		*prcRect;
	while( iterVec_Rect != m_vecRect.end() )
	{
		prcRect = &(*iterVec_Rect);
		prcRect->left		+= nOffsetX;
		prcRect->top		+= nOffsetY;
		prcRect->right		+= nOffsetX;
		prcRect->bottom		+= nOffsetY;

		iterVec_Rect++;
	}
}

//
// UITabButton::AddTabInfo
//
void UITabButton::AddTabInfo( const char* szName_, const RECT *pRect_ )	
{ 
	m_vecName.push_back( szName_ ); 
	m_vecRect.push_back( *pRect_ ); 
}

//
// UITabButton::SelectTab
//
BOOL UITabButton::SelectTab( int nX, int nY )
{
	POINT		pt = { nX, nY };
	for( UINT cnt = 0; cnt < m_vecRect.size(); cnt++ )
	{
		if( PtInRect( &m_vecRect[cnt], pt ) )
		{
			m_nTabIndex = cnt;
			return TRUE;
		}
	}
	return FALSE;
}

//
// UITabButton::SelectTab
//
BOOL UITabButton::SelectTab( int nTab )
{
	if( m_vecRect.size() <= nTab || 0 > nTab )
	{
		return FALSE;
	}

	m_nTabIndex = nTab;
	return TRUE;
}

//
// UITabButton::GetTabIndex
//
int	UITabButton::GetTabIndex()
{
	return m_nTabIndex;
}

//
// UITabButton::Draw
//
void UITabButton::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	for( int i = 0 ; i < (int)m_vecRect.size() ; i++ )
	{
		if( i != m_nTabIndex )
		{
			m_pSelectTab->Draw( m_vecRect[i].left, m_vecRect[i].top );
		} //if
	} //for
} //UITabButton::Draw