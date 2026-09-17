#include "StdAfx.h"
#include "EditForm.h"

/**
 *
 */
CEditForm::CEditForm()
{
	static DWORD dwId = 4000;

	m_dwId = dwId;
	dwId += 5;
} //CEditForm::CEditForm

/**
 *
 */
CEditForm::~CEditForm()
{

} //CEditForm::~CEditForm

/**
 *
 */
BOOL CEditForm::Create( int nX, int nY, CWnd* pWnd, BOOL bShow )
{
	DWORD dwStyle = ES_AUTOHSCROLL | WS_TABSTOP;

	if( !bShow )
	{
		dwStyle = WS_DISABLED;
	} //if


	m_dwId;
	RECT rect;

	rect.left = nX += 2;
	rect.top = nY;

	rect.right = nX += 35;
	rect.bottom = nY += 18;
	
	m_BeginFrame.Create( dwStyle, rect, pWnd, m_dwId );
	m_BeginFrame.ShowWindow( TRUE );
	m_dwId++;

	rect.left = nX += 2;
	rect.right = nX += 35;
		
	m_EndFrame.Create( dwStyle, rect, pWnd, m_dwId );
	m_EndFrame.ShowWindow( TRUE );
	m_dwId++;

	rect.left = nX += 2;
	rect.right = nX += 90;
	
	m_AniFile.Create( dwStyle, rect, pWnd, m_dwId );
	m_AniFile.ShowWindow( TRUE );

	m_dwId++;

	rect.left = nX += 2;
	rect.right = nX += 60;	

	m_WeaponFile.Create( dwStyle, rect, pWnd, m_dwId );
	m_WeaponFile.ShowWindow( TRUE );

	m_dwId++;

	rect.left = nX += 2;
	rect.right = nX += 60;	

	m_SoundFile.Create( dwStyle, rect, pWnd, m_dwId );
	m_SoundFile.ShowWindow( TRUE );


	m_BeginFrame.SetWindowText( "1" );
	m_EndFrame.SetWindowText( "" );
	m_AniFile;
	m_SoundFile.SetWindowText( "¾øÀ½" );

	return TRUE;
} //CEditForm::Create

BOOL	CEditForm::IsUse()
{
	if( strlen( GetAniFile() ) <= 3 )
	{
		return FALSE;
	} //if

	if( strlen( GetSoundFile() ) <= 3 )
	{
		return FALSE;
	} //if

	if( GetBeginFrame() + GetEndFrame() <= 0 )
	{
		return FALSE;
	} //if

	return TRUE;
} //

/**
 *
 */
char*	CEditForm::GetAniFile()
{
	char szBuffer[80];
	m_AniFile.GetWindowText( szBuffer, 80 );

	return szBuffer;
}

/**
 *
 */
char*	CEditForm::GetSoundFile()
{
	char szBuffer[80];
	sprintf( szBuffer, "" );
	m_SoundFile.GetWindowText( szBuffer, 80 );

	return szBuffer;
}

/**
 *
 */
DWORD	CEditForm::GetBeginFrame()
{
	char szBuffer[80];
	m_BeginFrame.GetWindowText( szBuffer, 80 );

	DWORD dwFrame = atoi( szBuffer );
	return dwFrame;
}

/**
 *
 */
DWORD	CEditForm::GetEndFrame()
{
	char szBuffer[80];
	m_EndFrame.GetWindowText( szBuffer, 80 );

	DWORD dwFrame = atoi( szBuffer );
	return dwFrame;
}

/**
 *
 */
void CEditForm::Set( const SMotion& motion )
{
	char szBuffer[80];

	sprintf( szBuffer, "%d", motion.dwBegin );
	m_BeginFrame.SetWindowText( szBuffer );

	if( motion.dwEnd != 0 )
	{
		sprintf( szBuffer, "%d", motion.dwEnd );
		m_EndFrame.SetWindowText( szBuffer );
	}
	else
	{
		m_EndFrame.SetWindowText( "" );
	}

	m_SoundFile.SetWindowText( g_FileMgr.GetFile( motion.wSoundFile ) );
	m_AniFile.SetWindowText( g_FileMgr.GetFile( motion.wAniFile ) );
	m_WeaponFile.SetWindowText( g_FileMgr.GetFile( motion.wWeaponFile ) );
	//motion.
} //CEditForm::Set

/**
 *
 */
void CEditForm::Save( SMotion* motion )
{
	char szBuffer[80];
	m_BeginFrame.GetWindowText( szBuffer, 80 );
	motion->dwBegin = atoi( szBuffer );

	m_EndFrame.GetWindowText( szBuffer, 80 );
	motion->dwEnd = atoi( szBuffer );

	m_SoundFile.GetWindowText( szBuffer, 30 );
	motion->wSoundFile = g_FileMgr.Insert( szBuffer );
	m_AniFile.GetWindowText( szBuffer, 30 );
	motion->wAniFile = g_FileMgr.Insert( szBuffer );
	m_WeaponFile.GetWindowText( szBuffer, 30 );
	motion->wWeaponFile = g_FileMgr.Insert( szBuffer );
	
/*
	motion.

	sprintf( szBuffer, "%d", motion.dwEnd );
	m_EndFrame.SetWindowText( szBuffer );

	m_SoundFile.SetWindowText( motion.szSoundFile );
	m_AniFile.SetWindowText( motion.szAniFile );
	m_WeaponFile.SetWindowText( motion.szWeaponFile );
	//motion.
	*/
} //CEditForm::Save

