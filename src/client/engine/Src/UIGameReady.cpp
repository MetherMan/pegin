/**
 * 게임 준비 장면.
 * 맵 이름을 화면에 출력한다.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-05-03 11:07a $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Engine/Src/UIGameReady.cpp $
 * 
 * 9     03-05-03 11:07a Ahastudio
 *
 * 7     03-03-17 8:43a Ahastudio		게임 화면이 보이도록 수정.
 * <HR>
 *
 * @file	UIGameReady.cpp
 */


#include "Engine_Export.h"
#include "UIGameReady.h"


#define MAX_FADEIN				3000
#define MAX_FADEOUT1			3000
#define MAX_FADEOUT2			1500
#define MAX_DELAY				1500


/**
 * 생성자.
 */
CUIGameReady::CUIGameReady()
:	m_pMapName		( NULL ),
	m_pMapNameBack	( NULL ),
	m_btAlpha		( 0 ),
	m_btAlphaBack	( 0 ),
	m_bShow			( FALSE ),
	m_nState		( 0 ),
	m_dwTick		( 0 ),
	m_dwInterval	( 0 )
{
} //CUIGameReady::CUIGameReady

/**
 * 소멸자.
 */
CUIGameReady::~CUIGameReady()
{
	ReleaseImage();
} //CUIGameReady::~CUIGameReady

/**
 * 이미지를 모두 해제한다.
 */
void CUIGameReady::ReleaseImage()
{
	SAFE_RELEASE( m_pMapName );
	SAFE_RELEASE( m_pMapNameBack );
} //CUIGameReady::ReleaseImage

/**
 * 이미지를 불러온다.
 */
BOOL CUIGameReady::LoadImage( WORD wMap )
{
	ReleaseImage();

	m_btAlpha		= 0;
	m_btAlphaBack	= 0;
	m_bShow			= TRUE;
	m_nState		= 0;
	m_dwTick		= 0;
	m_dwInterval	= 0;

	m_pMapNameBack	= MAKE_COMPONENT( Sprite );
	m_pMapName		= MAKE_COMPONENT( Sprite );

	CHECK_PTR_RETURN( m_pMapNameBack );
	CHECK_PTR_RETURN( m_pMapName );

	if( !m_pMapNameBack->Load( "interface/moros01_b.bmp", 800, 600 ) )
	{
		Assert( !"이미지읽기 실패" );
		return FALSE;
	} //if

	if( !m_pMapName->Load( "interface/moros01.tga", 334, 118 ) )
	{
		Assert( !"이미지읽기 실패" );
		return FALSE;
	} //if

	return TRUE;
} //CUIGameReady::LoadImage

/**
 * update.
 */
void CUIGameReady::Update( DWORD dwTick )
{
	if( !m_bShow )			return;

	m_dwInterval += dwTick;// - m_dwTick;
	//m_dwTick = dwTick;

	if( m_nState == 0 )
	{
		if( m_dwInterval > MAX_FADEIN )
		{
			m_dwInterval = 0;
			m_nState = 1;
			return;
		} //if
		m_btAlpha = (BYTE)( (float)m_dwInterval / (float)MAX_FADEIN * 255 );
		m_btAlphaBack = m_btAlpha;
	}
	else if( m_nState == 1 )
	{
		if( m_dwInterval > MAX_DELAY )
		{
			m_dwInterval = 0;
			m_nState = 2;
			return;
		} //if
	}
	else if( m_nState == 2 )
	{
		if( m_dwInterval > MAX_FADEOUT1 )
		{
			m_dwInterval = 0;
			m_nState = 3;
			return;
		} //if
		m_btAlphaBack = 255 - (BYTE)( (float)m_dwInterval / (float)MAX_FADEOUT1 * 255 );
	}
	else if( m_nState == 3 )
	{
		if( m_dwInterval > MAX_FADEOUT2 )
		{
			m_dwInterval = 0;
			m_nState	 = 0;
			m_bShow		 = FALSE;
			return;
		} //if
		m_btAlpha = 255 - (BYTE)( (float)m_dwInterval / (float)MAX_FADEOUT2 * 255 );
	} //if..else if..

	m_pMapName->SetColor( ( m_btAlpha << 24 ) | ( 255 << 16 ) | ( 255 << 8 ) | 255 );
	m_pMapNameBack->SetColor( 0x00000000  );
//	m_pMapNameBack->SetColor( ( m_btAlphaBack << 24 ) | ( 255 << 16 ) | ( 255 << 8 ) | 255  );
} //CUIGameReady::Update

/**
 * 화면에 그려준다.
 */
void CUIGameReady::Draw()
{
	if( !m_bShow )		return;

	m_pMapNameBack->Draw( 0, 0 );
	m_pMapName->Draw( 243, 150 );
} //CUIGameReady::Draw
