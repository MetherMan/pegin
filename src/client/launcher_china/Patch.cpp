/////////////////////////////////////////////////////
//
// Patch.cpp
// Created By : sckyj
//
/////////////////////////////////////////////////////

// include 		
	#include "stdafx.h"
	#include "Global.h"		
	#include "StringTable.h"
	#include "Decode.h"
#include <afxinet.h>
#include <io.h>
#include <Mmsystem.h>
//~include 

#pragma warning( disable : 4786 )

static DWORD WINAPI AutoPatch_ThreadProc( CPatch *pPatch )
{
	return pPatch->ThreadProc();
} // AutoPatch_ThreadProc

CPatch::CPatch()
:	m_hThread			( NULL ),
	m_dwCurrVersion		( 0 ),
	m_dwFileSize		( 0 ),
	m_dwTotalSize		( 0 ),
	m_nTotalFile		( 0 ),
	m_nFileCount		( 0 ),
	m_hFont				( 0 ),
	m_bFinish			( FALSE )
{
} // CPatch::CPatch

CPatch::~CPatch()
{
	Clear();
} // CPatch::~CPatch

void CPatch::Clear()
{
	if( m_hFont )
	{
		DeleteObject( m_hFont );
	} // if
	
	DWORD exitCode;
	GetExitCodeThread( m_hThread, &exitCode );
	TerminateThread( m_hThread, exitCode );

	m_hThread = NULL;
} // CPatch::Clear

// Init
BOOL CPatch::Init()
{
/*	m_bmBack.Load( "Launcher/mainback.wtm" );
	m_bmWeb.Load( "Launcher/apback.wtm" );
	m_bmProgress.Load( "Launcher/patch.wtm" );*/
	
	DWORD dwThreadID;
	m_hThread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )AutoPatch_ThreadProc, this, 0, &dwThreadID );
	if( !m_hThread )
	{
		return FALSE;
	} // if

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

	m_pgProgress[0].Create( FILE_PRO_RIGHT - FILE_PRO_LEFT, FILE_PRO_BOTTOM - FILE_PRO_TOP );
	m_pgProgress[1].Create( TOTAL_PRO_RIGHT - TOTAL_PRO_LEFT, TOTAL_PRO_BOTTOM - TOTAL_PRO_TOP );
	m_pgProgress[0].SetRange( 1000 );
	m_pgProgress[1].SetRange( 1000 );

	m_bmBack.Load("Interface/apback.wtm");
	m_bmReg[BTN_RELEASE].Load( "Interface/reg0.wtm" );
	m_bmReg[BTN_PRESS].Load("Interface/reg1.wtm");
	m_bmReg[BTN_NOTSUPPORT].Load("Interface/reg2.wtm");
	m_bmStart[BTN_RELEASE].Load("Interface/start0.wtm");
	m_bmStart[BTN_PRESS].Load("Interface/start1.wtm");
	m_bmStart[BTN_NOTSUPPORT].Load("Interface/start2.wtm");	
	m_bmSetup[BTN_RELEASE].Load("Interface/setup0.wtm");
	m_bmSetup[BTN_PRESS].Load("Interface/setup1.wtm");
	m_bmSetup[BTN_NOTSUPPORT].Load("Interface/setup2.wtm");	
	m_bmCancel[BTN_RELEASE].Load("Interface/cancel0.wtm");
	m_bmCancel[BTN_PRESS].Load("Interface/cancel1.wtm");
	m_bmCancel[BTN_NOTSUPPORT].Load("Interface/cancel2.wtm");	

	m_nBtnState[BTN_START]	= BTN_NOTSUPPORT;
	m_nBtnState[BTN_CANCEL]	= BTN_RELEASE;
	m_nBtnState[BTN_SETUP]	= BTN_NOTSUPPORT;	
	m_nBtnState[BTN_REG]	= BTN_NOTSUPPORT;	

	SetRect( &PATCH_REG, REG_BTN_LEFT,	REG_BTN_TOP, REG_BTN_RIGHT,	REG_BTN_BOTTOM );
	SetRect( &PATCH_START, START_BTN_LEFT, START_BTN_TOP, START_BTN_RIGHT, START_BTN_BOTTOM );
	SetRect( &PATCH_OPTION, SETUP_BTN_LEFT, SETUP_BTN_TOP, SETUP_BTN_RIGHT, SETUP_BTN_BOTTOM );
	SetRect( &PATCH_CANCEL, CANCEL_BTN_LEFT, CANCEL_BTN_TOP, CANCEL_BTN_RIGHT, CANCEL_BTN_BOTTOM );
	
	m_sFileInfo.Format( STR_REQUEST_CONNECT );
	
	return TRUE;
} // CPatch::Init

// Render
BOOL CPatch::Render()
{
	HDC		hDC			= ::GetDC( g_hWnd );
	HDC		hBackDC		= CreateCompatibleDC( hDC );

	SelectObject( hBackDC, m_bmBack.GetBitmap() );

	m_bmWeb.Draw( 148, 81, hBackDC );
	m_bmProgress.Draw( 148, 412, hBackDC );

	m_bmStart[m_nBtnState[BTN_START]].Draw( START_BTN_LEFT, START_BTN_TOP, hBackDC );	
	m_bmSetup[m_nBtnState[BTN_SETUP]].Draw( SETUP_BTN_LEFT, SETUP_BTN_TOP, hBackDC );
	m_bmCancel[m_nBtnState[BTN_CANCEL]].Draw( CANCEL_BTN_LEFT, CANCEL_BTN_TOP, hBackDC );
	m_bmReg[m_nBtnState[BTN_REG]].Draw( REG_BTN_LEFT, REG_BTN_TOP, hBackDC );	

	/*if( g_pWeb )
	{
		BitBlt(	hBackDC, NOTICE_LEFT,
			NOTICE_TOP,
			NOTICE_RIGHT - NOTICE_LEFT,
			NOTICE_BOTTOM - NOTICE_TOP,			
			g_pWeb->GetDC()->GetSafeHdc(), 0, 0,
			SRCCOPY );	
	} // if	*/

	BitBlt(	hBackDC, NOTICE_LEFT,
		NOTICE_TOP,
		NOTICE_RIGHT - NOTICE_LEFT,
		NOTICE_BOTTOM - NOTICE_TOP,			
		g_pWeb->GetDC()->GetSafeHdc(), 0, 0,
		SRCCOPY );	

	BitBlt( hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		hBackDC, 0, 0,
		SRCCOPY );	

	m_pgProgress[0].Draw( FILE_PRO_LEFT, FILE_PRO_TOP, hBackDC );
	m_pgProgress[1].Draw( TOTAL_PRO_LEFT, TOTAL_PRO_TOP, hBackDC );

	m_hFont = CreateFont(	12, 0, 0, 0, 
		FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		0x0000, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, 
		VARIABLE_PITCH, 
		"Arial" );

	HFONT hOldFont = ( HFONT )SelectObject( hBackDC, m_hFont );

	SetBkMode( hDC, TRANSPARENT );	
	SetTextColor( hDC, RGB( 235, 231, 227 ) );

	TextOut( hDC, FILE_TXT_LEFT, FILE_TXT_TOP, m_sFileInfo.GetBuffer( 0 ), m_sFileInfo.GetLength() );
	TextOut( hDC, TOTAL_TXT_LEFT, TOTAL_TXT_TOP, m_sTotalInfo.GetBuffer( 0 ), m_sTotalInfo.GetLength() );

/*	m_hFont = CreateFont(	12, 0, 0, 0, 
		FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		0x0000, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, 
		VARIABLE_PITCH, 
		"Arial" );
*/	
	
	SelectObject( hDC, hOldFont );
	DeleteObject( m_hFont );
	DeleteDC( hBackDC );	
	ReleaseDC( g_hWnd, hDC );

	return TRUE;
} // CPatch::Render

void CPatch::Send( BOOL bFinish, char* szMsg )
{	
	if( !bFinish )
	{
		SendMessage( g_hWnd, WM_UPDATE_DATA, 0, 0 );
		m_sFileInfo.Format( szMsg );
	}
	else
	{
		m_bFinish = TRUE;

		SendMessage( g_hWnd, WM_UPDATE_FINISH, 0, 0 );
		
		if( BTN_NOTSUPPORT == m_nBtnState[BTN_START] )
		{
			m_nBtnState[BTN_START] = BTN_RELEASE;		
		} //if
		
		if( BTN_NOTSUPPORT == m_nBtnState[BTN_SETUP] )
		{
			m_nBtnState[BTN_SETUP] = BTN_RELEASE;
		} //if

		if( BTN_NOTSUPPORT == m_nBtnState[BTN_REG] )
		{
			m_nBtnState[BTN_REG] = BTN_RELEASE;
		} //if
	} // if..else..	
} // CPatch::Send

DWORD CPatch::ThreadProc()
{	
	if( UpdateProcess() )
	{
		//	업데이트가 완료되었습니다		
		Send( FALSE, STR_SUCCEED_UPDATE );
		Sleep( 1000 );

		PlaySound( "./sound/notify.wav", NULL, SND_FILENAME );

		Send( TRUE );
	}
	else
	{
		//	업데이트를 실패하였습니다.		
		Send( FALSE, STR_FAILED_UPDATE );	
		Sleep( 1000 );
	}

	return TRUE;
} // CPatch::ThreadProc

BOOL CPatch::UpdateProcess()
{	
	m_dwCurrVersion = GetCurrVersion();

	remove( "patch.txt" );
	remove( "*.patch" );	

	if( !Process_VersionCheck() )	return FALSE;
	if( !Process_PatchInfoDown() )	return FALSE;
	if( !Process_Download() )		return FALSE;

	return TRUE;
} // CPatch::UpdateProcess

// 버전 데이터 검사
BOOL CPatch::Process_VersionCheck()
{
	// Version.dat 파일에 문제가 발생할 경우
	if( !m_dwCurrVersion )
	{
		// Version.dat 파일이 없으므로 생성한다.
		Send( FALSE, STR_CREATE_VERSIONFILE );		
		Sleep( 1000 );

		FILE* fp = fopen("Version.dat", "wb");
		if( !fp )
		{
			// Version.dat 파일 생성실패
			Send( FALSE, STR_FAILED_CREATE_VERSIONFILE );
			Sleep( 1000 );
			return FALSE;
		}		

		char buf[128] = "\0";
		sprintf(buf, "%d", 1);
		fwrite(buf, 1, sizeof(buf), fp);
		fclose(fp);

		// Version.dat 파일을 생성한후 재시도
		m_dwCurrVersion = GetCurrVersion();
		if( !m_dwCurrVersion )
		{
			// Version.dat 파일 생성실패			
			Send( FALSE, STR_FAILED_CREATE_VERSIONFILE );
			Sleep( 1000 );
			return FALSE;
		} // if
	} // if

	return TRUE;
} // CPatch::Process_VersionCheck

// 패치 정보 받기
BOOL CPatch::Process_PatchInfoDown()
{
	CString sTarget;
	DWORD	dwVersion		= 0;	
	int		result1, result2;

	// -- patch.txt file download
	sTarget.Format( "%spatch.txt", SERVER );

	if( !HttpFileTransfer( (char*)sTarget.operator LPCTSTR() ) )
	{		
		//		업데이트 서버에 연결할수 없습니다.
		Send( FALSE, STR_CANNOT_CONNECT );
		Sleep( 1000 );
		return FALSE;
	} // if

	if( !IsValidPatchFile() )
	{
		//		patch.txt 화일 오류 - 정상적인 패치 화일이 아닙니다
		Send( FALSE, STR_PATCH1ERR_ABNORMALFILE );
		Sleep( 1000 );
		return FALSE;
	}	

	// -- patch file total size 계산
	FILE *fp = fopen( "patch.txt", "rb" );
	if( !fp )
	{
		//		pathch1.txt 화일 오류 - 화일을 열 수 없습니다.
		Send( FALSE, STR_PATCH1ERR_CANNOTOPENFILE );
		Sleep( 1000 );
		return FALSE;
	}

	while(!feof(fp))
	{
		result1 = fscanf( fp, "%d", &dwVersion );
		result2 = fscanf( fp, "%d", &m_dwFileSize );

		// EOF가 아닌 상태에서 result1, result2가 0일경우는
		// patch.txt 화일안의 숫자를 읽어들이지 못한경우이다.
		if( !result1 || !result2 )
		{
			//			patch.txt 화일 오류 - 매칭 실패
			Send( FALSE, STR_PATCH1ERR_MISMATCHING );
			Sleep( 1000 );
			return FALSE;
		} // if

		if( dwVersion > m_dwCurrVersion )
		{
			m_dwTotalSize += m_dwFileSize;
			m_nTotalFile++;
		} // if
	} // while
	
	fclose(fp);

	m_pgProgress[1].SetRange(m_dwTotalSize);

	return TRUE;

} // CPatch::Process_PatchInfoDown


// 패치 다운 로드
BOOL CPatch::Process_Download()
{
	FILE* fp = fopen( "patch.txt", "rb" );
	if( !fp )
	{
		//		pathch1.txt 화일 오류 - 화일을 열 수 없습니다(2).
		Send( FALSE, STR_PATCH1ERR_CANNOTOPENFILE );
		Sleep( 1000 );
		return FALSE;
	}		

	CString sTarget;
	DWORD	dwVersion		= 0;
	char	szVersion[128]	= "\0";
	char	szFileName[128] = "\0";
	int		result1, result2;		
	
	// -- patch file download
	while( !feof( fp ) )	
	{			
		result1 = fscanf( fp, "%s", szVersion );
		dwVersion = atol( szVersion );
		result2 = fscanf( fp, "%d", &m_dwFileSize );
		if( !result1 || !result2 )
		{
			//			patch.txt 화일 오류 - 매칭 실패(2)
			Send( FALSE, STR_PATCH1ERR_MISMATCHING );
			Sleep( 1000 );
			return FALSE;
		} // if

		m_pgProgress[0].SetRange( m_dwFileSize );
		ZeroMemory( szFileName, sizeof(szFileName) );
		wsprintf( szFileName, "%s.patch", szVersion );

		if( dwVersion > m_dwCurrVersion )
		{
			sTarget.Format( "%s%s", SERVER, szFileName );
				
			if( !HttpFileTransfer( (char*)sTarget.operator LPCTSTR() ))
			{
				//	파일 전송을 실패하였습니다.
				Send( FALSE, STR_FAILED_TRANSFILE );
				Sleep( 1000 );
				return FALSE;
			} // if

			//	압축을 푸는 중...
			Send( FALSE, STR_EXTRACTING_PACKAGE );			
			Sleep( 1000 );

			// 패치 파일의 arj 헤더를 검사한다
			if( IsValidHeader( szFileName ) )
			{
				// 패치 파일의 압축을 푼다
				if( 0 == UpdateDecode( szFileName, 'X' ) )
				{
					//					압축 풀기 실패.
					Send( FALSE, STR_FAILED_EXTRACT );				
					Sleep( 1000 );
					fclose( fp );

					remove( szFileName );

					return FALSE;
				}					
				
				remove( szFileName );
				
				// 정상적으로 압축이 풀렸을경우 버젼정보를 갱신한다.
				SetCurrVersion( dwVersion );
			}				
			else
			{
				// 패치파일의 헤더정보가 오류가 날경우 에러
				// (웹에 패치가 업뎃이 않됐경우도 오류로 나온다)
				//				패치파일이 존재하지 않거나 헤더오류입니다.
				Send( FALSE, STR_WRONGINFO_PACKAGE );				
				Sleep( 1000 );

				fclose( fp );
				
				remove( szFileName );
				return FALSE;
			}					
		} // if
	} // while

	// 패치를 정상적으로 완료
	if(fp)	
		fclose( fp );

	return TRUE;
} // CPatch::Process_Download

#define dHTTP_TRANS_ERROR					0
#define dHTTP_TRANS_DONE					1	

int CPatch::HttpFileTransfer( const char *url )
{
	#define HTTPBUFLEN			512 // Size of HTTP Buffer...
	char httpbuff[HTTPBUFLEN] = "\0";
	char filename[128] = "\0";
	static DWORD totalSize = 0;

	TRY
	{
		CInternetSession mysession;
		CStdioFile *remotefile = mysession.OpenURL( url, 1, INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_AUTO_REDIRECT );

		strcpy( filename, remotefile->GetFileName() );
		
		CFile myfile(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		
		int		numbytes		= 0;
		DWORD	currSize		= 0;
		char	totalFile[128]	= "\0";	

		if( strcmp( filename, "patch.txt" ) && 
			strcmp( filename, "name.txt" ) )
		{
			m_nFileCount++;
		} // if			
		
		while( ( numbytes = remotefile->Read( httpbuff, HTTPBUFLEN ) ) )
		{			
			myfile.Write( httpbuff, numbytes );
			currSize	+= numbytes;
			totalSize	+= numbytes;

			if( strcmp( filename, "patch.txt" ) && strcmp( filename, "name.txt" ) )
			{
				m_pgProgress[0].SetPos( currSize );
				m_pgProgress[1].SetPos( totalSize );
			} // if

			wsprintf( totalFile, "%d/%d", m_nFileCount, m_nTotalFile );

			m_sFileInfo.Format( "%s ( %dkb / %dkb )", filename, ( int )( currSize / 1000 ), ( int )( m_dwFileSize / 1000 ) );		
			m_sTotalInfo.Format("%s ( %dkb / %dkb )", totalFile, ( int )( totalSize / 1000 ), ( int )( m_dwTotalSize / 1000 ) );

			Send( FALSE, m_sFileInfo.GetBuffer( 0 ) );
		} // while

		myfile.Close();
		mysession.Close();
		remotefile->Close();

		if( ( currSize != m_dwFileSize )				&&
			( strcmp( filename, "patch.txt" ) )		&&
			( strcmp( filename, "name.txt" ) )
			)
		{
			//			파일의 크기가 맞지 않습니다.
			Send( FALSE, STR_WRONGSIZE_PACKAGE );
			Sleep( 1000 );
			return dHTTP_TRANS_ERROR;
		} // if

		return dHTTP_TRANS_DONE;
	}	
	CATCH_ALL(error)
	return dHTTP_TRANS_ERROR;
	END_CATCH_ALL;
	
	return dHTTP_TRANS_ERROR;
	
} // int CPatch::HttpFileTransfer

BOOL CPatch::IsValidPatchFile()
{
	FILE* fp = fopen( "patch.txt", "rb" );
	if( !fp )
		return 0;

	DWORD	nVersion = 0, nSize = 0;
	int		result1 = 0, result2 = 0;

	while( !feof(fp) )
	{
		result1 = fscanf(fp, "%d", &nVersion);
		result2 = fscanf(fp, "%d", &nSize);
		
		if( !result1 || !result2 )
		{
			fclose( fp );
			return	FALSE;
		}		
	}		

	return TRUE;
} // BOOL CPatch::IsValidPatchFile

DWORD CPatch::GetCurrVersion()
{
	DWORD	currVersion;

	FILE *fp = fopen( "Version.dat", "rb" );

	if( !fp )
		return 0;

	char str[128];

	fgets( str, 128, fp );

	currVersion = atol( str );

	fclose(fp);

	return currVersion;
} // CPatch::GetCurrVersion

DWORD CPatch::SetCurrVersion( DWORD currVersion )
{
	remove("Version.dat");
	FILE* fp = fopen("Version.dat", "wb");

	char buf[128] = "\0";
	sprintf(buf, "%d", currVersion);

	fwrite(buf, 1, sizeof(buf), fp);

	fclose( fp );

	return currVersion;
} // CPatch::SetCurrVersion

BOOL CPatch::LDown( POINT pt )
{
	if( PtInRect( &PATCH_CANCEL, pt ) && ( m_nBtnState[BTN_CANCEL] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_CANCEL] = BTN_PRESS;
		InvalidateRect( g_hWnd, &PATCH_CANCEL, FALSE );

		return TRUE;
	}
	else if( PtInRect( &PATCH_START, pt ) && ( m_nBtnState[BTN_START] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_START] = BTN_PRESS;
		InvalidateRect( g_hWnd, &PATCH_START, FALSE );

		return TRUE;
	}
	else if( PtInRect( &PATCH_OPTION, pt ) && ( m_nBtnState[BTN_SETUP] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_SETUP] = BTN_PRESS;
		InvalidateRect( g_hWnd, &PATCH_OPTION, FALSE );

		return TRUE;
	} // if..else..

	else if( PtInRect( &PATCH_REG, pt ) && ( m_nBtnState[BTN_REG] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_REG] = BTN_PRESS;
		InvalidateRect( g_hWnd, &PATCH_REG, FALSE );
		
		return TRUE;
	} // if..else..

	return FALSE;
} // CPatch::LDown

BYTE CPatch::LUp( POINT pt )
{
	if( BTN_PRESS == m_nBtnState[BTN_CANCEL] )
	{
		if( PtInRect( &PATCH_CANCEL, pt ) )
		{
			//		업데이트가 취소되었습니다.
			m_sFileInfo.Format( STR_CANCLE_UPDATE );		
			Send( FALSE, m_sFileInfo.GetBuffer( 0 ) );
			Sleep( 1000 );
			
			PlaySound( "./sound/button.wav", NULL, SND_FILENAME );
			
			DWORD exitCode;
			GetExitCodeThread( m_hThread, &exitCode );
			TerminateThread( m_hThread, exitCode );
			
			remove( "*.patch" );
			
			m_nBtnState[BTN_CANCEL] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &PATCH_CANCEL, FALSE );
			
			return 2;
		}
		else
		{
			m_nBtnState[BTN_CANCEL] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &PATCH_CANCEL, FALSE );
		} // if..else..		
	}
	else if( BTN_PRESS == m_nBtnState[BTN_START] )
	{
		if( PtInRect( &PATCH_START, pt ) )
		{
			//		게임을 시작합니다.
			m_sFileInfo.Format( STR_START_GAME );
			Send( FALSE, m_sFileInfo.GetBuffer( 0 ) );
			Sleep( 1000 );
			
			PlaySound( "./sound/button.wav", NULL, SND_FILENAME );
			
			m_nBtnState[BTN_START] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &PATCH_START, FALSE );
			
			return 0;
		}
		else
		{
			if( m_bFinish )
			{
				m_nBtnState[BTN_START] = BTN_RELEASE;
				InvalidateRect( g_hWnd, &PATCH_START, FALSE );
			} // if
		} // if..else..		
	}
	else if( BTN_PRESS == m_nBtnState[BTN_SETUP] ) 
	{
		if( PtInRect( &PATCH_OPTION, pt ) )
		{
			m_sFileInfo.Format( "Video Setup" );
			Send( FALSE, m_sFileInfo.GetBuffer( 0 ) );
			Sleep( 1000 );
			
			PlaySound( "./sound/button.wav", NULL, SND_FILENAME );	
			
			m_nBtnState[BTN_SETUP] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &PATCH_OPTION, FALSE );
			
			return 1;
		}
		else
		{
			if( m_bFinish )
			{
				m_nBtnState[BTN_SETUP] = BTN_RELEASE;
				InvalidateRect( g_hWnd, &PATCH_OPTION, FALSE );
			} // if		
		} // if..else..
		
	} // if
	else if( BTN_PRESS == m_nBtnState[BTN_REG] ) 
	{
		if( PtInRect( &PATCH_REG, pt ) )
		{
			PlaySound( "./sound/button.wav", NULL, SND_FILENAME );	
			
			m_nBtnState[BTN_REG] = BTN_RELEASE;
			InvalidateRect( g_hWnd, &PATCH_REG, FALSE );
			
			return 3;
		}
		else
		{
			if( m_bFinish )
			{
				m_nBtnState[BTN_REG] = BTN_RELEASE;
				InvalidateRect( g_hWnd, &PATCH_REG, FALSE );
			} // if		
		} // if..else..
		
	} // if

	return -1;
} // CPatch::LUp