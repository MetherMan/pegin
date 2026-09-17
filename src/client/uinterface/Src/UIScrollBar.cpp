/**
 * 유저 인터페이스용 스크롤 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-05-07 9:53a $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIScrollBar.cpp $
 * 
 * 10    04-05-07 9:53a Paran2
 * 
 * 9     04-02-10 3:19p Paran
 * 
 * 8     03-11-28 1:46p Paran
 * 
 * 7     03-10-09 12:25a Paran
 * 
 * 6     03-10-07 9:07p Paran
 * 
 * 2     03-05-17 4:20a Paran			각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIScrollBar.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIScrollBar.h"

//
// UIScrollBar::ReleaseAll
//
void UIScrollBar::ReleaseAll()
{
	SAFE_RELEASE( m_pimgArrow[0] );
	SAFE_RELEASE( m_pimgArrow[1] );
	SAFE_RELEASE( m_pimgThumb );
	SAFE_RELEASE( m_rectBox );
}

//
// UIScrollBar::Init
//
BOOL UIScrollBar::Init()
{
	int		nWidth, nHeight;
	nWidth  = m_rcRegion.right - m_rcRegion.left;
	nHeight = m_rcRegion.bottom - m_rcRegion.top;

	m_pimgArrow[0]	= MAKE_COMPONENT( Sprite );
	m_pimgArrow[1]	= MAKE_COMPONENT( Sprite );
	m_pimgThumb		= MAKE_COMPONENT( Sprite );
	m_rectBox		= MAKE_COMPONENT( Rectangle );
	if( !m_pimgArrow[0] || !m_pimgArrow[0] || !m_pimgThumb || !m_rectBox )
	{
		Assert( 0 );
		return FALSE;
	} //if

	int			nThumbW, nThumbH;
	char		szArrowName[2][64];
	char		szThumb[64];
	switch( m_bType )
	{
	case HORIZONTAL:	
		strcpy( szArrowName[0], "Interface/arrow-l-1.tga" );
		strcpy( szArrowName[1], "Interface/arrow-r-1.tga" );
		strcpy( szThumb, "Interface/button-opiton-s.bmp" );
		nThumbW = 20;
		nThumbH = 13;

		m_rcArrow[1].left		= m_rcRegion.right - 13;
		m_rcArrow[1].top		= m_rcRegion.top;
		m_rcArrow[1].right		= m_rcRegion.right;
		m_rcArrow[1].bottom		= m_rcRegion.top + 13;

		m_rcTrack.left			= m_rcRegion.left + 13;
		m_rcTrack.top			= m_rcRegion.top;
		m_rcTrack.right			= m_rcRegion.right - 13 - 20;
		m_rcTrack.bottom		= m_rcTrack.top + 13;

		m_rcThumb.left			= m_rcRegion.left + 13;
		m_rcThumb.top			= m_rcRegion.top;
		m_rcThumb.right			= m_rcThumb.left + 20;
		m_rcThumb.bottom		= m_rcThumb.top + 13;

		m_nTrackW = m_rcTrack.right - m_rcTrack.left + 20;
		m_nTrackH = m_rcTrack.bottom - m_rcTrack.top;
		break;

	case VERTICAL:
		strcpy( szArrowName[0], "Interface/u-arrow.tga" );
		strcpy( szArrowName[1], "Interface/d-arrow.tga" );
		strcpy( szThumb, "Interface/s-bar.tga" );
		nThumbW = 13;
		nThumbH = 20;

		m_rcArrow[1].left		= m_rcRegion.left;
		m_rcArrow[1].top		= m_rcRegion.bottom - 13;
		m_rcArrow[1].right		= m_rcArrow[1].left + 13;
		m_rcArrow[1].bottom		= m_rcRegion.bottom;

		m_rcTrack.left			= m_rcRegion.left;
		m_rcTrack.top			= m_rcRegion.top + 13;
		m_rcTrack.right			= m_rcTrack.left + 13;
		m_rcTrack.bottom		= m_rcRegion.bottom - 13 - 20;

		m_rcThumb.left			= m_rcRegion.left;
		m_rcThumb.top			= m_rcRegion.top + 13;
		m_rcThumb.right			= m_rcThumb.left + 13;
		m_rcThumb.bottom		= m_rcThumb.top + 20;

		m_nTrackW = m_rcTrack.right - m_rcTrack.left;
		m_nTrackH = m_rcTrack.bottom - m_rcTrack.top + 20;

		break;
	} //switch
	m_rcArrow[0].left		= m_rcRegion.left;
	m_rcArrow[0].top		= m_rcRegion.top;
	m_rcArrow[0].right		= m_rcArrow[0].left + 13;
	m_rcArrow[0].bottom		= m_rcArrow[0].top + 13;

	if( !m_pimgArrow[0]->Load( szArrowName[0], 13, 13 ) ||
		!m_pimgArrow[1]->Load( szArrowName[1], 13, 13 ) ||
		!m_pimgThumb->Load( szThumb, nThumbW, nThumbH ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	return TRUE;
}

void UIScrollBar::SetType( BOOL bType )
{
	m_bType = bType;
} //UIScrollBar::SetType

BOOL UIScrollBar::GetType()
{
	return m_bType;
} //UIScrollBar::GetType

void UIScrollBar::Move( int nOffsetX, int nOffsetY )
{
	UICComponent::Move( nOffsetX, nOffsetY );
	m_rcTrack.left		+= nOffsetX;
	m_rcTrack.top		+= nOffsetY;
	m_rcTrack.right		+= nOffsetX;
	m_rcTrack.bottom	+= nOffsetY;

	m_rcArrow[0].left	+= nOffsetX;
	m_rcArrow[0].top	+= nOffsetY;
	m_rcArrow[0].right	+= nOffsetX;
	m_rcArrow[0].bottom	+= nOffsetY;

	m_rcArrow[1].left	+= nOffsetX;
	m_rcArrow[1].top	+= nOffsetY;
	m_rcArrow[1].right	+= nOffsetX;
	m_rcArrow[1].bottom	+= nOffsetY;

	m_rcThumb.left		+= nOffsetX;
	m_rcThumb.top		+= nOffsetY;
	m_rcThumb.right		+= nOffsetX;
	m_rcThumb.bottom	+= nOffsetY;
}

/**
 * 위치, 사이즈 조정
 */
void UIScrollBar::SetPosition( int nX, int nY, int nLength )
{
	int		nThumbW = 20;
	int		nThumbH = 13;
	int		nWeight;

	switch( m_bType )
	{
	case HORIZONTAL:
		nWeight = m_rcRegion.bottom - m_rcRegion.top;
		m_rcRegion.left		= nX;
		m_rcRegion.top		= nY;
		m_rcRegion.right	= nX + nLength;
		m_rcRegion.bottom	= nY + nWeight;

		nThumbW = 20;
		nThumbH = 13;

		m_rcArrow[1].left		= m_rcRegion.right - 13;
		m_rcArrow[1].top		= m_rcRegion.top;
		m_rcArrow[1].right		= m_rcRegion.right;
		m_rcArrow[1].bottom		= m_rcRegion.top + 13;

		m_rcTrack.left			= m_rcRegion.left + 13;
		m_rcTrack.top			= m_rcRegion.top;
		m_rcTrack.right			= m_rcRegion.right - 13 - 20;
		m_rcTrack.bottom		= m_rcTrack.top + 13;

		m_nTrackW = m_rcTrack.right - m_rcTrack.left + 20;
		m_nTrackH = m_rcTrack.bottom - m_rcTrack.top;
		
		m_rcThumb.left	 = m_rcTrack.right;
		m_rcThumb.right = m_rcThumb.right + nThumbW;
		break;

	case VERTICAL:
		nWeight = m_rcRegion.right - m_rcRegion.left;
		m_rcRegion.left		= nX;
		m_rcRegion.top		= nY;
		m_rcRegion.right	= nX + nWeight;
		m_rcRegion.bottom	= nY + nLength;

		nThumbW = 13;
		nThumbH = 20;

		m_rcArrow[1].left		= m_rcRegion.left;
		m_rcArrow[1].top		= m_rcRegion.bottom - 13;
		m_rcArrow[1].right		= m_rcArrow[1].left + 13;
		m_rcArrow[1].bottom		= m_rcRegion.bottom;

		m_rcTrack.left			= m_rcRegion.left;
		m_rcTrack.top			= m_rcRegion.top + 13;
		m_rcTrack.right			= m_rcTrack.left + 13;
		m_rcTrack.bottom		= m_rcRegion.bottom - 13 - 20;

		m_nTrackW = m_rcTrack.right - m_rcTrack.left;
		m_nTrackH = m_rcTrack.bottom - m_rcTrack.top + 20;

		m_rcThumb.top	 = m_rcTrack.bottom;
		m_rcThumb.bottom = m_rcThumb.top + nThumbH; 
		break;
	} //switch

	m_nPos = 0;

	m_rcArrow[0].left		= m_rcRegion.left;
	m_rcArrow[0].top		= m_rcRegion.top;
	m_rcArrow[0].right		= m_rcArrow[0].left + 13;
	m_rcArrow[0].bottom		= m_rcArrow[0].top + 13;
} //UIScrollBar::SetPosition

//
// UIScrollBar::GetMax
//
int UIScrollBar::GetMax()
{
	return m_nMax;
} //UIScrollBar::GetMax

//
// UIScrollBar::SetMax
//
void UIScrollBar::SetMax( int nMax )
{ 
	m_nMax = nMax + 1;				// 하나 더..
	if( m_nMax < 1 )
	{
		m_nMax = 1;
	} //if
} //UIScrollBar::SetMax

//
// UIScrollBar::MoveThumb
//
void UIScrollBar::MoveThumb( int nOffset )
{
	int		nThumbLength;
	int		nTrackLength;

	switch( m_bType )
	{
	case HORIZONTAL:
		nThumbLength = m_rcThumb.right - m_rcThumb.left;
		m_rcThumb.left += nOffset;

		if( m_rcThumb.left > m_rcTrack.right )
		{
			m_rcThumb.left = m_rcTrack.right;
		}
		else if( m_rcThumb.left < m_rcTrack.left )
		{
			m_rcThumb.left = m_rcTrack.left;
		} //if..else
		m_rcThumb.right = m_rcThumb.left + nThumbLength;

		nTrackLength = m_rcTrack.right - m_rcTrack.left;
		m_nPos = ( (float)m_nMax / (float)nTrackLength ) * (float)( m_rcThumb.left - m_rcTrack.left );
		break;

	case VERTICAL:
		nThumbLength = m_rcThumb.bottom - m_rcThumb.top;
		m_rcThumb.top += nOffset;

		if( m_rcThumb.top > m_rcTrack.bottom )
		{
			m_rcThumb.top = m_rcTrack.bottom;
		}
		else if( m_rcThumb.top < m_rcTrack.top )
		{
			m_rcThumb.top = m_rcTrack.top;
		} //if..else
		m_rcThumb.bottom = m_rcThumb.top + nThumbLength;

		nTrackLength = m_rcTrack.bottom - m_rcTrack.top;
		m_nPos = ( (float)m_nMax / (float)nTrackLength ) * (float)( m_rcThumb.top - m_rcTrack.top );
	} //switch
} //UIScrollBar::MoveThumb

/**
 * 스크롤 바 클릭.
 */
void UIScrollBar::ClickBar( const int nX, const int nY )
{
	POINT	ptPos = { nX, nY };
	if( PtInRect( &m_rcArrow[0], ptPos ) )
	{ // 위에
		SetScrollPos( m_nPos-1 );
	}
	else if( PtInRect( &m_rcArrow[1], ptPos ) )
	{ // 아래
		SetScrollPos( m_nPos+1 );
	}
} //UIScrollBar::ClickBar

//
// UIScrollBar::GetScrollPos
//
int UIScrollBar::GetScrollPos()
{
	if( m_nPos >= m_nMax )
	{
		m_nPos = m_nMax;
	} //if
	
	return m_nPos;
} //UIScrollBar::GetScrollPos

//
// UIScrollBar::SetScrollPos
//
void UIScrollBar::SetScrollPos( int nPos )
{
	m_nPos = nPos;
	if( m_nPos > m_nMax )
	{
		m_nPos = m_nMax;
	}
	else if( m_nPos < 0 )
	{
		m_nPos = 0;
	}

	int		nThumbLength;
	int		nTrackLength;

	switch( m_bType )
	{
	case HORIZONTAL:
		nThumbLength = m_rcThumb.right - m_rcThumb.left;
		nTrackLength = m_rcTrack.right - m_rcTrack.left;
		m_rcThumb.left		= m_rcTrack.left + ( ( (float)nTrackLength / (float)m_nMax ) * (float)m_nPos );
		m_rcThumb.right		= m_rcThumb.left + nThumbLength;
		break;

	case VERTICAL:
		nThumbLength = m_rcThumb.bottom - m_rcThumb.top;
		nTrackLength = m_rcTrack.bottom - m_rcTrack.top;
		m_rcThumb.top		 = m_rcTrack.top + ( ( (float)nTrackLength / (float)m_nMax ) * (float)m_nPos );
		m_rcThumb.bottom	 = m_rcThumb.top + nThumbLength;
		break;
	} //switch
} //UIScrollBar::SetScrollPos

//
// UIScrollBar::Draw
//
void UIScrollBar::Draw()
{
	if( !IsShow() )		return;

	if( !m_pimgArrow[0] ||
		!m_pimgArrow[1] ||
		!m_pimgThumb )
	{
		Assert( !"스크롤 이미지가 없습니다." );
		return;
	} //if

	m_rectBox->Draw( m_rcTrack.left, m_rcTrack.top, m_nTrackW, m_nTrackH, 0xC8000000 );
	m_pimgArrow[0]->Draw( m_rcArrow[0].left, m_rcArrow[0].top );
	m_pimgArrow[1]->Draw( m_rcArrow[1].left, m_rcArrow[1].top );
	m_pimgThumb->Draw( m_rcThumb.left, m_rcThumb.top );
} //UIScrollBar::Draw