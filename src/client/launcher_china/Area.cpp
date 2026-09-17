/////////////////////////////////////////////////////
//
// Area.cpp
// Created By : sckyj
//
/////////////////////////////////////////////////////

// include 	
	#include "stdafx.h"
	#include "Global.h"	
	#include <winnetwk.h>
	#include <shlobj.h>
//~include 

#pragma warning( disable : 4786 )

CArea::CArea()
:	m_hFont			( NULL ),
	m_bSelect		( FALSE ),
	m_pServer		( NULL ),
	MAX_AREA		( 0 ),
	m_pnBlockState	( NULL ),
	pAREA_BLOCK		( NULL ),
	m_nPage			( 1 ),
	m_nPageCount	( 0 )
{	
} // CArea::CArea

CArea::~CArea()
{
} // CArea::~CArea

BOOL CArea::Init()
{
	if( !g_pServerMgr )	return FALSE;	

	LPITEMIDLIST	pItemDList;
	char			szFontDir[1024];
	char			szCheckFile[1024];
	
	SHGetSpecialFolderLocation(NULL, CSIDL_FONTS, &pItemDList);
	SHGetPathFromIDList( pItemDList, szFontDir );
	
	sprintf( szCheckFile, "%s%s", szFontDir, "\\Gulim.ttc" );
	
	FILE		*fp = fopen( szCheckFile, "rb" );
	
	if( !fp )
	{
		char	szCom[1024];
		sprintf( szCom, "xcopy interface\\Gulim.ttc %s\\*.*", szFontDir );
		system( szCom );
		sprintf( szCom, "del interface\\Gulim.ttc" );
	}
	else
	{

	}
	
	m_bmBack.Load( "Launcher/mainback.wtm" );
	m_bmAreaBack.Load( "Launcher/serverlist.wtm" );	

	m_hFont = CreateFont(	12, 0, 0, 0, 
							FW_NORMAL, 
							FALSE, FALSE, FALSE, 
							0x0000, 
							OUT_DEFAULT_PRECIS, 
							CLIP_DEFAULT_PRECIS, 
							ANTIALIASED_QUALITY, 
							VARIABLE_PITCH, 
							"Arial" );
							//"NSimSun" );

/*	m_bmStart[BTN_RELEASE].Load( "Launcher/start0.wtm" );
	m_bmStart[BTN_PRESS].Load( "Launcher/start1.wtm" );
	m_bmStart[BTN_NOTSUPPORT].Load( "Launcher/start2.wtm" );*/
	m_bmCancel[BTN_RELEASE].Load( "Launcher/cancel0.wtm" );	
	m_bmCancel[BTN_PRESS].Load( "Launcher/cancel1.wtm" );
	m_bmCancel[BTN_NOTSUPPORT].Load( "Launcher/cancel2.wtm" );

	m_bmBlock[BLK_RELEASE].Load( "Launcher/but_l_1.wtm" );
	m_bmBlock[BLK_SELECT].Load( "Launcher/but_server_sel.wtm" );
	m_bmBlock[BLK_NOSERVICE].Load( "Launcher/but_server_cant.wtm" );

	m_bmPage[BTN_RELEASE].Load( "Launcher/tap-sc-1.wtm" );
	m_bmPage[BTN_PRESS].Load( "Launcher/tap-sc-2.wtm" );
	m_bmPage[BTN_NOTSUPPORT].Load( "Launcher/tap-sc-3.wtm" );

	m_nBtnState[BTN_START]	= BTN_NOTSUPPORT;
	m_nBtnState[BTN_CANCEL]	= BTN_RELEASE;
	
	m_nPageCount = g_pServerMgr->GetPageCount();
	for( int i=0 ; i<4 ; i++ )
	{
		if( i < m_nPageCount )
		{
			m_anPageState[i] = BTN_RELEASE;
		}
		else
		{
			m_anPageState[i] = BTN_NOTSUPPORT;
		} // if..else..
	} // for
	
	SetRect( &AREA_START, 216, 397, 290, 422 );
	SetRect( &AREA_CANCEL, 343, 397, 418, 422 );
	for( i=0 ; i<4 ; i++ )
	{
		SetRect( &aAREA_PAGE[i], 255 + i * 70, 454, 255 + i * 70 + 55, 475 );
	} // for

	m_pServer = new CServer();
	m_pServer->Init();
	m_pServer->SetArea( g_pServerMgr->GetArea( m_nPage, 0 ) );

	SetTimer( g_hWnd, 3, 100, NULL );

	SetPage( 0 );

	m_anPageState[0] = BTN_PRESS;

	return TRUE;
} // CArea::Init

void CArea::SetPage( int nPage )
{
	if( m_pnBlockState )	delete[] m_pnBlockState;
	if( pAREA_BLOCK )		delete[] pAREA_BLOCK;

	m_nPage = nPage;
	MAX_AREA = g_pServerMgr->GetAreaCount( m_nPage );

	pAREA_BLOCK = new RECT[MAX_AREA];
	for( int i=0 ; i<MAX_AREA ; i++ )
	{
		SetRect( &pAREA_BLOCK[i], 246, 124 + 30 * i, 370, 135 + 30 * i + 23 );
	} // for

	m_pnBlockState = new int[MAX_AREA];
	memset( m_pnBlockState, 0, sizeof( int ) * MAX_AREA );

	m_bSelect		= FALSE;
	g_nServerIndex	= -1;
} // CArea::SetPage

void CArea::Clear()
{
	SAFE_DELETE( m_pServer );

	delete[] m_pnBlockState;
	delete[] pAREA_BLOCK;
} // CArea::Clear

// Render
BOOL CArea::Render()
{
	if( g_nServerIndex == 0 )
	{
		if( m_nBtnState[BTN_START] != BTN_PRESS )
		{
			m_nBtnState[BTN_START] = BTN_RELEASE;
		} // if		
	}
	else
	{
		m_nBtnState[BTN_START] = BTN_NOTSUPPORT;
	} // if..else..

	HDC		hDC			= ::GetDC( g_hWnd );
	HDC		hBackDC		= CreateCompatibleDC( hDC );

	SelectObject( hBackDC, m_bmBack.GetBitmap() );

	m_bmAreaBack.Draw( 152, 11, hBackDC );

	DrawBlock( hBackDC );
	DrawPage( hBackDC );

	m_bmStart[m_nBtnState[BTN_START]].Draw( AREA_START.left, AREA_START.top, hBackDC );
	m_bmCancel[m_nBtnState[BTN_CANCEL]].Draw( AREA_CANCEL.left, AREA_CANCEL.top, hBackDC );

	switch( m_bSelect )
	{
	case FALSE	:
		m_pServer->Render( hBackDC, FALSE );
		break;

	case TRUE	:
		m_pServer->Render( hBackDC, TRUE );
		break;
	} // switch

	BitBlt( hDC, 0, 0, 800, 600,
			hBackDC, 0, 0,
			SRCCOPY );

	DeleteDC( hBackDC );
	ReleaseDC( g_hWnd, hDC );
	
	return TRUE;
} // CArea::Render

void CArea::DrawBlock( HDC hDC )
{
	HFONT hOldFont = ( HFONT )SelectObject( hDC, m_hFont );
	CString sName;

	for( int i=0 ; i<MAX_AREA ; i++ )
	{
		m_bmBlock[m_pnBlockState[i]].Draw( pAREA_BLOCK[i].left, pAREA_BLOCK[i].top, hDC );

		SetBkMode( hDC, TRANSPARENT );
		SetTextColor( hDC, RGB( 235, 231, 227 ) );

		SArea* sArea =  g_pServerMgr->GetArea( m_nPage, i );
		if( sArea )
		{
			sName.Format( sArea->szAreaName );
		}
		else
		{
			sName.Format( "" );
		} // if..else..

		TextOut( hDC, 288, 129 + 30 * i, sName.GetBuffer( 0 ), sName.GetLength() );
	} // for

	SelectObject( hDC, hOldFont );
} // CArea::DrawBlock

void CArea::DrawPage( HDC hDC )
{
	HFONT hOldFont = ( HFONT )SelectObject( hDC, m_hFont );
	CString sName;

	for( int i=0 ; i<4 ; i++ )
	{
		m_bmPage[m_anPageState[i]].Draw( aAREA_PAGE[i].left, aAREA_PAGE[i].top, hDC );

		SetBkMode( hDC, TRANSPARENT );

		if( i < m_nPageCount )
		{
			SetTextColor( hDC, RGB( 235, 231, 227 ) );
		}
		else
		{
			SetTextColor( hDC, RGB( 104, 104, 104) );
		} // if..else..	

		char szPage[2] = "\0";
		itoa( i, szPage, 10 );		
		
		TextOut( hDC, aAREA_PAGE[i].left + 25, aAREA_PAGE[i].top + 5, szPage, 2 );
	} // for

	SelectObject( hDC, hOldFont );
} // CArea::DrawPage

// Check Button
BOOL CArea::LDown( POINT pt )
{	
	if( AreaBtnLDown( pt )		||
		AreaPageLDown( pt )		||
		AreaBlockLDown( pt )	||
		m_pServer->LDown( pt ) )
	{
		return TRUE;
	} // if

	return FALSE;
} // CArea::LDown

BOOL CArea::AreaBtnLDown( POINT pt )
{
	if( PtInRect( &AREA_CANCEL, pt ) && ( m_nBtnState[BTN_CANCEL] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_CANCEL] = BTN_PRESS;
		InvalidateRect( g_hWnd, &AREA_CANCEL, FALSE );

		return TRUE;
	}
	else if( PtInRect( &AREA_START, pt ) && 
		( m_nBtnState[BTN_START] == BTN_RELEASE ) )
	{
		if( g_nServerIndex >= 0 )
		{
			m_nBtnState[BTN_START] = BTN_PRESS;
			InvalidateRect( g_hWnd, &AREA_START, FALSE );

			return TRUE;
		}
	} // if..else..

	return FALSE;
} // CArea::AreaBtnLDown

BOOL CArea::AreaPageLDown( POINT pt )
{
	for( int i=0 ; i<4 ; i++ )
	{
		if( PtInRect( &aAREA_PAGE[i], pt ) && ( m_anPageState[i] == BTN_RELEASE ) )
		{
			for( int j=0 ; j<m_nPageCount ; j++ )
			{
				m_anPageState[j] = BTN_RELEASE;
			} // for

			m_anPageState[i] = BTN_PRESS;
			InvalidateRect( g_hWnd, &aAREA_PAGE[i], FALSE );
			
			SetPage( i );

			return TRUE;
		} // if
	} // for	

	return FALSE;
} // CArea::AreaPageLDown

BOOL CArea::AreaBlockLDown( POINT pt )
{
	for( int i=0 ; i<MAX_AREA ; i++ )
	{
		if( PtInRect( &pAREA_BLOCK[i], pt ) && m_pnBlockState[i] == BLK_RELEASE )
		{
			for( int j=0 ; j<MAX_AREA ;j++ )
			{
				m_pnBlockState[j] = BLK_RELEASE;
			} // for

			m_pnBlockState[i] = BLK_SELECT;
			InvalidateRect( g_hWnd, &pAREA_BLOCK[i], FALSE );

			g_nServerIndex = i;
			
		//	m_pServer->LDown( pt );

			return TRUE;
		} // if
	} // for

	return FALSE;
} // CArea::AreaLDown

BYTE CArea::LUp( POINT pt )
{
	if( BTN_PRESS == m_nBtnState[BTN_CANCEL] )
	{
		if( PtInRect( &AREA_CANCEL, pt ) )
		{
			m_nBtnState[BTN_CANCEL] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &AREA_CANCEL, FALSE );
			
			return 0;
		}
		else
		{
			m_nBtnState[BTN_CANCEL] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &AREA_CANCEL, FALSE );
		}
	} // if..else..

	if( BTN_PRESS == m_nBtnState[BTN_START] )		
	{
		if( PtInRect( &AREA_START, pt ) )
		{
			m_nBtnState[BTN_START] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &AREA_START, FALSE );

			return 1;
		}
		else
		{
			m_nBtnState[BTN_START] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &AREA_START, FALSE );
		}		
	} // if..else..

	for( int i=0 ; i<MAX_AREA ; i++ )
	{
		if( BLK_SELECT == m_pnBlockState[i] && PtInRect( &pAREA_BLOCK[i], pt ) )
		{
			m_bSelect = TRUE;
			SArea* pArea = g_pServerMgr->GetArea( m_nPage, i );
			m_pServer->SetArea( pArea );			
		} // if
	} // for

//	m_pServer->LUp( pt );
	
	return -1;
} // CArea::LUp
