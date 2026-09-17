/////////////////////////////////////////////////////
//
// Area.cpp
// Created By : sckyj
//
/////////////////////////////////////////////////////

// include 	
	#include "stdafx.h"
	#include "Global.h"	
//~include 

#pragma warning( disable : 4786 )

CServer::CServer()
:	m_pArea		( NULL ),
	m_hFont		( NULL ),
	m_bSelect	( NULL )
{	
} // CServer::CServer

CServer::~CServer()
{	
} // CServer::~CServer

void CServer::Init()
{
	m_bmServerBack.Load( "Launcher/serverlist2.wtm" );

	m_bmBlock[BLK_RELEASE].Load( "Launcher/but_l_1.wtm" );
	m_bmBlock[BLK_SELECT].Load( "Launcher/but_server_sel.wtm" );
	m_bmBlock[BLK_NOSERVICE].Load( "Launcher/but_server_cant.wtm" );

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

	for( int i=0 ; i<MAX_SERVER ; i++ )
	{
		SetRect( &SERVER_BLOCK[i], 246, 124 + 30 * i, 370, 135 + 30 * i + 23 );//544, 192 + 30 * i, 670, 192 + 30 * i + 23 );
	} // for
} // CServer::Init

void CServer::SetArea( SArea* pArea )
{
	if( !pArea )	return;

	m_pArea		= pArea;
	m_nCount	= m_pArea->serverMap.size();

	memset( m_nBlockState, 0, sizeof( int ) * MAX_SERVER );
} // CServer::SetArea

BOOL CServer::Render( HDC hDC, BOOL bTrue )
{
/*	HFONT hOldFont = ( HFONT )SelectObject( hDC, m_hFont );
	CString sName;

	m_bmServerBack.Draw( 533, 175, hDC );

	m_bSelect = bTrue;
	if( m_bSelect )
	{
		int i=0;
		ServerMap::iterator itor = m_pArea->serverMap.begin();
		SServer* pServer = NULL;
		while( itor != m_pArea->serverMap.end() )
		{
			m_bmBlock[m_nBlockState[i]].Draw( SERVER_BLOCK[i].left, SERVER_BLOCK[i].top, hDC );
			
			pServer = itor->second;
			if( pServer )
			{
				sName.Format( pServer->szServerName );
				
				SetBkMode( hDC, TRANSPARENT );
				SetTextColor( hDC, RGB( 235, 231, 227 ) );		
				
				TextOut( hDC, 583, 197 + 30 * i, sName.GetBuffer( 0 ), sName.GetLength() );
			} // if		
			
			i++;
			itor++;
		} // while	
	} // if

	SelectObject( hDC, hOldFont );	
*/
	return TRUE;	
} // CServer::Render

BOOL CServer::LDown( POINT pt )
{
//	if( !m_bSelect ) return FALSE;

	for( int i=0 ; i<m_nCount ; i++ )
	{
		if( PtInRect( &SERVER_BLOCK[i], pt ) && m_nBlockState[i] == BLK_RELEASE )
		{
			for( int j=0 ; j<m_nCount ; j++ )
			{
				m_nBlockState[j] = BLK_RELEASE;
			} // for

			m_nBlockState[i] = BLK_SELECT;
			
			
		//	memset( &m_nBlockState[i], BLK_SELECT, sizeof(int) );
			InvalidateRect( g_hWnd, &SERVER_BLOCK[i], FALSE );

			return TRUE;
		} // if
	} // for

	return FALSE;
} // CServer::LDown

BYTE CServer::LUp( POINT pt )
{
//	if( !m_bSelect ) return FALSE;

	for( int i=0 ; i<m_nCount ; i++ )
	{
		if( BLK_SELECT == m_nBlockState[i] && PtInRect( &SERVER_BLOCK[i], pt ) )
		{			
			ServerMap::iterator itor = m_pArea->serverMap.find( i );
			if( itor != m_pArea->serverMap.end() )
			{
				SServer* pServer = itor->second;
				if( pServer )
				{
					g_nServerIndex = pServer->nServerIndex;
				}
				else
				{
					g_nServerIndex = -1;
				} // if..else..
			} // if
		} // if
	} // for

	return TRUE;
} // CServer::LUp