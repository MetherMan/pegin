// LauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "decode.h"

#include <afxinet.h>
#include <io.h>
#include <Mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_ProcessLife = 1;

#define	WM_UPDATE_DATA		WM_USER+1
#define WM_UPDATE_FINISH	WM_USER+2


/////////////////////////////////////////////////////////////////////////////
// CLauncherDlg dialog

CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLauncherDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_bSkipPatch = FALSE;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLauncherDlg)
	DDX_Control(pDX, IDC_EXPLORER1, m_WebBrowser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
	//{{AFX_MSG_MAP(CLauncherDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE( WM_UPDATE_FINISH, OnUpdateFinish )
	ON_MESSAGE( WM_UPDATE_DATA, OnUpdateData )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static DWORD WINAPI AutoPatch_ThreadProc( CLauncherDlg *pDlg )
{
	return pDlg->ThreadProc();
}

/////////////////////////////////////////////////////////////////////////////
// CLauncherDlg message handlers

BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDirectory();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText("Deicide Online Auto Launcher");
	::SetWindowPos(GetSafeHwnd(), NULL, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL);

	m_MsgMgr.ReadScript( dFILE_LAUNCHER_MSG );
	
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

	m_bDone					= FALSE;
	m_nBtnState[BTN_START]	= BTN_NOTSUPPORT;
	m_nBtnState[BTN_CANCEL]	= BTN_RELEASE;
	m_nBtnState[BTN_SETUP]	= BTN_NOTSUPPORT;
	m_nBtnState[BTN_REG]	= BTN_RELEASE;

	m_Progress[0].Create( FILE_PRO_RIGHT - FILE_PRO_LEFT, FILE_PRO_BOTTOM - FILE_PRO_TOP );
	m_Progress[1].Create( TOTAL_PRO_RIGHT - TOTAL_PRO_LEFT, TOTAL_PRO_BOTTOM - TOTAL_PRO_TOP );
	m_Progress[0].SetRange( 1000 );
	m_Progress[1].SetRange( 1000 );

	m_WebBrowser.SetWindowPos( NULL, NOTICE_LEFT, NOTICE_TOP, NOTICE_RIGHT - NOTICE_LEFT, NOTICE_BOTTOM - NOTICE_TOP, NULL );
	m_WebBrowser.Navigate( g_szNoticeURL, NULL, NULL, NULL, NULL );
	if(m_WebBrowser.GetBusy())
	{
		Sleep( 200 );
	}

	m_szFileInfo.Format( m_MsgMgr.GetString( 0 ) );
	m_szTotalInfo.Format( "" );
	m_szServerName.Format( "" );
	m_bPatchInfo		= FALSE;

	m_dwCurrVersion		= 0;
	m_dwTotalSize		= 0;
	m_dwFileSize		= 0;
	m_nFileCount		= 0;
	m_nTotalFile		= 0;

	m_rtStart.left		= START_BTN_LEFT;
	m_rtStart.top		= START_BTN_TOP;
	m_rtStart.right		= START_BTN_RIGHT;
	m_rtStart.bottom	= START_BTN_BOTTOM;

	m_rtCancel.left		= CANCEL_BTN_LEFT;
	m_rtCancel.top		= CANCEL_BTN_TOP;
	m_rtCancel.right	= CANCEL_BTN_RIGHT;
	m_rtCancel.bottom	= CANCEL_BTN_BOTTOM;

	m_rtSetup.left		= SETUP_BTN_LEFT;
	m_rtSetup.top		= SETUP_BTN_TOP;
	m_rtSetup.right		= SETUP_BTN_RIGHT;
	m_rtSetup.bottom	= SETUP_BTN_BOTTOM;

	m_rtReg.left		= REG_BTN_LEFT;
	m_rtReg.top			= REG_BTN_TOP;
	m_rtReg.right		= REG_BTN_RIGHT;
	m_rtReg.bottom		= REG_BTN_BOTTOM;

	DWORD dwThreadID;
	m_hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)AutoPatch_ThreadProc, this, 0, &dwThreadID );
	if(!m_hThread)
	{
		return FALSE;
	}

	SetTimer(1, 300, NULL);		

	if( m_bSkipPatch )
	{
		PushStartButton();
	} //if

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLauncherDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		Render();
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLauncherDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	// CDialog::OnCancel();
}

void CLauncherDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CLauncherDlg::OnTimer(UINT nIDEvent) 
{
	if(1 == nIDEvent)
	{
		Render();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CLauncherDlg::Render()
{
	CDC*	pDC	= GetDC();
	HDC		hDC	= pDC->GetSafeHdc();

	HDC hBackDC	= CreateCompatibleDC( hDC );
	
	SelectObject( hBackDC, m_bmBack.GetBitmap() );	

	m_bmReg[m_nBtnState[BTN_REG]].Draw( REG_BTN_LEFT, REG_BTN_TOP, hBackDC );	
	m_bmStart[m_nBtnState[BTN_START]].Draw( START_BTN_LEFT, START_BTN_TOP, hBackDC );	
	m_bmCancel[m_nBtnState[BTN_CANCEL]].Draw( CANCEL_BTN_LEFT, CANCEL_BTN_TOP, hBackDC );
	m_bmSetup[m_nBtnState[BTN_SETUP]].Draw( SETUP_BTN_LEFT, SETUP_BTN_TOP, hBackDC );

	BitBlt(	hBackDC,
			NOTICE_LEFT,
			NOTICE_TOP,
			NOTICE_RIGHT - NOTICE_LEFT,
			NOTICE_BOTTOM - NOTICE_TOP,
			m_WebBrowser.GetDC()->GetSafeHdc(),
			0,
			0,
			SRCCOPY );
	
	BitBlt(	hDC,
			0,
			0,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			hBackDC,
			0,
			0,
			SRCCOPY );
	
	m_Progress[0].Draw( FILE_PRO_LEFT, FILE_PRO_TOP, hDC );
	m_Progress[1].Draw( TOTAL_PRO_LEFT, TOTAL_PRO_TOP, hDC );

	m_hFont = CreateFont(	12, 0, 0, 0, 
		FW_NORMAL, 
		FALSE, FALSE, FALSE, 
		IME_CMODE_ALPHANUMERIC, 
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, 
		ANTIALIASED_QUALITY, 
		VARIABLE_PITCH, 
		"Arial" );

	HFONT hOldFont = (HFONT)SelectObject( hDC, hFont );

	SetBkMode( hDC, TRANSPARENT );
	SetTextColor( hDC, RGB( 235, 231, 227 ) );

	TextOut( hDC, FILE_TXT_LEFT, FILE_TXT_TOP, m_szFileInfo, strlen( m_szFileInfo ) );
	TextOut( hDC, TOTAL_TXT_LEFT, TOTAL_TXT_TOP, m_szTotalInfo, strlen( m_szTotalInfo ) );

	SelectObject( hDC, hOldFont );
	DeleteObject( hFont );
	DeleteDC( hBackDC );
	ReleaseDC( pDC );
}

void CLauncherDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(CheckButton(BTN_CANCEL, point) && ( m_nBtnState[BTN_CANCEL] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_CANCEL] = BTN_PRESS;
		InvalidateRect(&m_rtCancel, FALSE);
	}
	else
	if(CheckButton(BTN_START, point) && ( m_nBtnState[BTN_START] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_START] = BTN_PRESS;
		InvalidateRect(&m_rtStart, FALSE);
	}
	else
	if(CheckButton(BTN_SETUP, point) && ( m_nBtnState[BTN_SETUP] == BTN_RELEASE ) )
	{
		m_nBtnState[BTN_SETUP] = BTN_PRESS;
		InvalidateRect(&m_rtSetup, FALSE);
	}
	else 
		if( CheckButton( BTN_REG, point ) && ( m_nBtnState[BTN_REG] == BTN_RELEASE ) )
		{
			m_nBtnState[BTN_REG] = BTN_PRESS;
			InvalidateRect( &m_rtReg, FALSE );
		}
	else
	{
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, (LPARAM)MAKELPARAM(point.x, point.y));	
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CLauncherDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(BTN_PRESS == m_nBtnState[BTN_CANCEL])
	{
		m_nBtnState[BTN_CANCEL] = BTN_RELEASE;
		InvalidateRect(&m_rtCancel, FALSE);
		
		PlaySound( "./sound/button.wav", NULL, SND_FILENAME );

		DWORD exitCode;
		GetExitCodeThread(m_hThread, &exitCode);
		TerminateThread(m_hThread, exitCode);

		remove( "*.patch" );
		
		//		업데이트가 취소되었습니다.
		m_szFileInfo.Format( m_MsgMgr.GetString( 1 ) );
		Render();
		Sleep( 1000 );
		
		EndDialog(IDCANCEL);
	}
	else
	if(BTN_PRESS == m_nBtnState[BTN_START])
	{
		m_nBtnState[BTN_START] = BTN_RELEASE;
		InvalidateRect(&m_rtStart, FALSE);

		PlaySound( "./sound/button.wav", NULL, SND_FILENAME );

		//		게임을 시작합니다.
		m_szFileInfo.Format( m_MsgMgr.GetString( 2 ) );

		Render();
		Sleep( 1000 );

		PushStartButton();
	}
	else
	if(BTN_PRESS == m_nBtnState[BTN_SETUP])
	{
		m_nBtnState[BTN_SETUP] = BTN_RELEASE;
		InvalidateRect(&m_rtSetup, FALSE);

		PlaySound( "./sound/button.wav", NULL, SND_FILENAME );

		m_szFileInfo.Format("Video Setup");
		Render();
		Sleep( 1000 );

		// Video Setup
		ShellExecute( NULL, NULL, "VideoSetup.exe", "", NULL, SW_SHOW );	
		EndDialog( IDCANCEL );
	}
	else
	if( BTN_PRESS == m_nBtnState[BTN_REG] )
	{
		//Explorer Start
		ShellExecute( GetSafeHwnd(), NULL, "C:\\Program Files\\Internet Explorer\\iexplore.exe", 
			"http://deicide.ongameport.com/account/01_Register_Step1.php", NULL, SW_SHOW );
		EndDialog( IDCANCEL );
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CLauncherDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( ( BTN_PRESS == m_nBtnState[BTN_CANCEL] ) && !CheckButton(BTN_CANCEL, point ) )
	{
		m_nBtnState[BTN_CANCEL] = BTN_RELEASE;
		InvalidateRect(&m_rtCancel, FALSE);
	}	
	else
	if( ( BTN_PRESS == m_nBtnState[BTN_START] ) && !CheckButton(BTN_START, point ) )
	{
		m_nBtnState[BTN_START] = BTN_RELEASE;
		InvalidateRect(&m_rtStart, FALSE);
	}	
	else
	if( ( BTN_PRESS == m_nBtnState[BTN_SETUP] ) && !CheckButton(BTN_SETUP, point ) )
	{
		m_nBtnState[BTN_SETUP] = BTN_RELEASE;
		InvalidateRect(&m_rtSetup, FALSE);
	}
	else
		if( ( BTN_PRESS == m_nBtnState[BTN_REG] ) && !CheckButton(BTN_REG, point ) )
		{
			m_nBtnState[BTN_REG] = BTN_REG;
			InvalidateRect(&m_rtSetup, FALSE);
		}

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CLauncherDlg::CheckButton(int type, CPoint point)
{
	switch(type)
	{
	case BTN_REG:
		{
			if( point.x < m_rtReg.right &&
				point.x > m_rtReg.left	&&
				point.y > m_rtReg.top	&&
				point.y < m_rtReg.bottom
				)
			{
				return TRUE;
			}
			return FALSE;
		}
	case BTN_CANCEL :
		{
			if(
				point.x < m_rtCancel.right	&&
				point.x > m_rtCancel.left	&&
				point.y > m_rtCancel.top	&&
				point.y < m_rtCancel.bottom )
			{
				return TRUE;
			}
			return FALSE;
		}
	case BTN_START: 
		{
			if(
				point.x < m_rtStart.right	&&
				point.x > m_rtStart.left	&&
				point.y > m_rtStart.top		&&
				point.y < m_rtStart.bottom )
			{
				return TRUE;
			}
			else return FALSE;
		}
		break;
	case BTN_SETUP :
		{
			if(
				point.x < m_rtSetup.right	&&
				point.x > m_rtSetup.left	&&
				point.y > m_rtSetup.top		&&
				point.y < m_rtSetup.bottom )
			{
				return TRUE;
			}
			else return FALSE;
		}
		break;
	default :
		break;
	}
	return TRUE;
}

DWORD CLauncherDlg::ThreadProc()
{
	if( UpdateProcess() )
	{
		//		업데이트가 완료되었습니다
		m_szFileInfo.Format( m_MsgMgr.GetString( 3 ) ); 

		SendMessage(WM_UPDATE_DATA);	
		Sleep( 1000 );		

		PlaySound( "./sound/notify.wav", NULL, SND_FILENAME );

		SendMessage(WM_UPDATE_FINISH);
	}
	else
	{
		//		업데이트를 실패하였습니다.
		m_szFileInfo.Format( m_MsgMgr.GetString( 4 ) );

		SendMessage(WM_UPDATE_DATA);	
		Sleep( 1000 );
	}
	return TRUE;
}

BOOL CLauncherDlg::GetServerName()
{
/*	CString szTarget;
	remove( "name.txt" );

	// -- Get Patch Server Name
	szTarget.Format( "%s", g_szServerName );
	if( !HttpFileTransfer( ( char* )szTarget.operator LPCTSTR() ) )	
	{
		//		Can not connect Server
		m_szFileInfo.Format( m_MsgMgr.GetString( 7 ) );

		SendMessage(WM_UPDATE_DATA);
		Sleep( 1000 );

		g_ProcessLife = 0;
		return FALSE;		
	}

	FILE* fp = fopen( "name.txt", "rt" );
	if( !fp )
	{
		//		Can not recognize SeverName cause NO exist file "name.txt"
		m_szFileInfo.Format( m_MsgMgr.GetString( 19 ) );

		SendMessage(WM_UPDATE_DATA);
		Sleep( 1000 );

		g_ProcessLife = 0;
		return FALSE;
	}
		
	char szServer[128] = "\0";
	fgets( szServer, 128, fp );

	m_szFileInfo.Format( m_MsgMgr.GetString( 0 ) );
	m_szServerName.Format( szServer );	
	m_szFileInfo	+=	m_szServerName;
	m_szFileInfo	+=	m_MsgMgr.GetString( 18 );

	fclose( fp );

	remove( "name.txt" );
*/
	return TRUE;
}

BOOL CLauncherDlg::UpdateProcess()
{
	CString szTarget;
	m_dwCurrVersion = GetCurrVersion();

	remove( "patch.txt" );
	remove( "*.patch" );	

	// Version.dat 파일에 문제가 발생할 경우
	if( !m_dwCurrVersion )
	{
		// Version.dat 파일이 없으므로 생성한다.
		//		Version.dat 파일을 생성합니다.
		m_szFileInfo.Format( m_MsgMgr.GetString( 5 ) );

		SendMessage(WM_UPDATE_DATA);
		Sleep( 1000 );

		FILE* fp = fopen("Version.dat", "wb");
		if( !fp )
		{
			//			Version.dat 파일 생성실패
			m_szFileInfo.Format( m_MsgMgr.GetString( 6 ) );

			SendMessage(WM_UPDATE_DATA);
			Sleep( 1000 );

			g_ProcessLife = 0;			
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
			//			Version.dat 파일 생성실패
			m_szFileInfo.Format( m_MsgMgr.GetString( 6 ) );
			SendMessage(WM_UPDATE_DATA);
			Sleep( 1000 );

			g_ProcessLife = 0;			
			return FALSE;
		}		
	}

	// -- patch1.txt file download
	szTarget.Format( "%spatch.txt", dPATCH_SERVER_ADDR );

	if( !HttpFileTransfer( (char*)szTarget.operator LPCTSTR() ) )
	{		
		//		업데이트 서버에 연결할수 없습니다.
		m_szFileInfo.Format( m_MsgMgr.GetString( 7 ) );

		SendMessage(WM_UPDATE_DATA);
		Sleep( 1000 );

		g_ProcessLife = 0;
		return FALSE;
	}

	if( !IsValidPatchFile() )
	{
		//		patch1.txt 화일 오류 - 정상적인 패치 화일이 아닙니다
		m_szFileInfo.Format( m_MsgMgr.GetString( 8 ) );

		SendMessage(WM_UPDATE_DATA);
		Sleep( 1000 );

		g_ProcessLife = 0;		
		return FALSE;
	}

	DWORD	dwVersion		= 0;
	char	szFileName[128] = "\0";

	int		result1, result2;

	// -- patch file total size 계산
	FILE *fp = fopen( "patch.txt", "rb" );
	if( !fp )
	{
		//		pathch1.txt 화일 오류 - 화일을 열 수 없습니다.
		m_szFileInfo.Format( m_MsgMgr.GetString( 9 ) );

		SendMessage(WM_UPDATE_DATA);
		Sleep( 1000 );

		g_ProcessLife = 0;			
		return FALSE;
	}

	while(!feof(fp))
	{
		result1 = fscanf( fp, "%d", &dwVersion );
		result2 = fscanf( fp, "%d", &m_dwFileSize );

		// EOF가 아닌 상태에서 result1, result2가 0일경우는
		// patch1.txt 화일안의 숫자를 읽어들이지 못한경우이다.
		if( !result1 || !result2 )
		{
			//			patch1.txt 화일 오류 - 매칭 실패
			m_szFileInfo.Format( m_MsgMgr.GetString( 10 ) );

			SendMessage(WM_UPDATE_DATA);
			Sleep( 1000 );

			g_ProcessLife = 0;
			return FALSE;
		}

		if( dwVersion > m_dwCurrVersion )
		{
			m_dwTotalSize += m_dwFileSize;
			m_nTotalFile++;
		}
	}
	
	fclose(fp);
	m_Progress[1].SetRange(m_dwTotalSize);	

	fp = fopen( "patch.txt", "rb" );
	if( !fp )
	{
		//		pathch1.txt 화일 오류 - 화일을 열 수 없습니다(2).
		m_szFileInfo.Format( m_MsgMgr.GetString( 11 ) );

		SendMessage(WM_UPDATE_DATA);
		Sleep( 1000 );
		
		g_ProcessLife = 0;		
		return FALSE;
	}		

	char	szVersion[128]	=	"\0";
	
	// -- patch file download
	while( !feof( fp ) )	
	{			
		if( !g_ProcessLife )
			break;

		result1 = fscanf( fp, "%s", szVersion );
		dwVersion = atol( szVersion );
		result2 = fscanf( fp, "%d", &m_dwFileSize );
		if( !result1 || !result2 )
		{
			//			patch1.txt 화일 오류 - 매칭 실패(2)
			m_szFileInfo.Format( m_MsgMgr.GetString( 12 ) );

			SendMessage(WM_UPDATE_DATA);
			Sleep( 1000 );

			g_ProcessLife = 0;
			return FALSE;
		}

		m_Progress[0].SetRange(m_dwFileSize);
		ZeroMemory( szFileName, sizeof(szFileName) );
		wsprintf( szFileName, "%s.patch", szVersion );

		if( dwVersion > m_dwCurrVersion )
		{
			szTarget.Format( "%s%s", dPATCH_SERVER_ADDR, szFileName );
				
			if( !HttpFileTransfer( (char*)szTarget.operator LPCTSTR() ))
			{
				//				파일 전송을 실패하였습니다.
				m_szFileInfo.Format( m_MsgMgr.GetString( 13 ) );

				SendMessage(WM_UPDATE_DATA);
				Sleep( 1000 );

				return FALSE;
			}

			//			압축을 푸는 중...
			m_szFileInfo.Format( m_MsgMgr.GetString( 14 ) );

			SendMessage(WM_UPDATE_DATA);
			Sleep( 1000 );

			// 패치 파일의 arj 헤더를 검사한다
			if(IsValidHeader( szFileName ) )
			{
				// 패치 파일의 압축을 푼다
				if(0 == UpdateDecode( szFileName, 'X' ))
				{
					//					압축 풀기 실패.
					m_szFileInfo.Format( m_MsgMgr.GetString( 15 ) );

					SendMessage(WM_UPDATE_DATA);
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
				m_szFileInfo.Format( m_MsgMgr.GetString( 16 ) );

				SendMessage(WM_UPDATE_DATA);
				Sleep( 1000 );

				fclose( fp );
				
				remove( szFileName );
				return FALSE;
			}					
		}
	}
	// 패치를 정상적으로 완료
	if(fp)	fclose( fp );
	return TRUE;
}

void CLauncherDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
}

void CLauncherDlg::OnUpdateFinish(WPARAM wParam, LPARAM lParam)
{
	if(BTN_NOTSUPPORT == m_nBtnState[BTN_START])
	{
		m_nBtnState[BTN_START] = BTN_RELEASE;		
	} //if
	
	if(BTN_NOTSUPPORT == m_nBtnState[BTN_SETUP])
	{
		m_nBtnState[BTN_SETUP] = BTN_RELEASE;
	} //if
}

DWORD CLauncherDlg::GetCurrVersion()
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
}

#define dHTTP_TRANS_ERROR					0
#define dHTTP_TRANS_DONE					1	

int CLauncherDlg::HttpFileTransfer( const char *url )
{
	#define HTTPBUFLEN			512 // Size of HTTP Buffer...
	char httpbuff[HTTPBUFLEN] = "\0";
	char filename[128] = "\0";
	static DWORD totalSize = 0;

	TRY
	{
		CInternetSession mysession;
		CStdioFile *remotefile = mysession.OpenURL(url,1,INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_DONT_CACHE |INTERNET_FLAG_NO_AUTO_REDIRECT);

		strcpy( filename, remotefile->GetFileName() );
		
		CFile myfile(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		
		int numbytes = 0;
		DWORD currSize = 0;

		char totalFile[128] = "\0";	

		if( strcmp(filename, "patch.txt") && strcmp( filename, "name.txt" ) )
			m_nFileCount++;
		
		while( ( numbytes = remotefile->Read( httpbuff, HTTPBUFLEN ) ) )
		{			
			myfile.Write( httpbuff, numbytes );
			currSize	+= numbytes;
			totalSize	+= numbytes;

			if( strcmp( filename, "patch.txt" ) && strcmp( filename, "name.txt" ) )
			{
				m_Progress[0].SetPos( currSize );
				m_Progress[1].SetPos( totalSize );
			}

			wsprintf( totalFile, "%d/%d", m_nFileCount, m_nTotalFile );

			m_szFileInfo.Format( "%s (%dkb/%dkb)", filename, (int)(currSize/1000), (int)(m_dwFileSize/1000));		
			m_szTotalInfo.Format("%s (%dkb/%dkb)", totalFile, (int)(totalSize/1000), (int)(m_dwTotalSize/1000) );

			SendMessage(WM_UPDATE_DATA);			
		}	

		myfile.Close();
		mysession.Close();
		remotefile->Close();

		if( ( currSize != m_dwFileSize )				&&
			( strcmp( filename, "patch.txt" ) )		&&
			( strcmp( filename, "name.txt" ) )
			)
		{
			//			파일의 크기가 맞지 않습니다.
			m_szFileInfo.Format( m_MsgMgr.GetString( 17 ) );

			SendMessage(WM_UPDATE_DATA);
			Sleep( 1000 );

			return dHTTP_TRANS_ERROR;
		}

		return dHTTP_TRANS_DONE;
	}	
	CATCH_ALL(error)
	return dHTTP_TRANS_ERROR;
	END_CATCH_ALL;
	
	return dHTTP_TRANS_ERROR;
}

BOOL CLauncherDlg::IsValidPatchFile()
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
}

DWORD CLauncherDlg::SetCurrVersion(DWORD currVersion)
{
	remove( "Version.dat" );
	FILE* fp = fopen( "Version.dat", "wb" );

	char buf[128] = "\0";
	sprintf(buf, "%d", currVersion);

	fwrite(buf, 1, sizeof(buf), fp);

	return currVersion;
}

void CLauncherDlg::SetDirectory()
{
	char	szModuleFileName[256];
	GetModuleFileName( NULL, szModuleFileName, 256 );
	
	char	szDirectoryName[256];
	char	szDriverName[256];
	char	szModuleName[256];
	char	szExt[256];
	_splitpath( szModuleFileName, szDriverName, szDirectoryName, szModuleName, szExt );
	
	sprintf( m_szFullName, "%s%s", szDriverName, szDirectoryName );
	
	SetCurrentDirectory( m_szFullName );
}

void CLauncherDlg::PushStartButton()
{
	char szArg[40] = "\0";
	int		nServerIdx = 0;
	strcpy( szArg, "노패치투" );

	if( _access( "config.ini", 0 ) == -1 )
	{ // "config.ini" 파일이 없을 시..
		ShellExecute( NULL, NULL, "VideoSetup.exe", "", NULL, SW_SHOW );
	}
	else
	{ // 게임 시작
		ShellExecute( NULL, NULL, "DeicideOnline.exe", szArg, NULL, SW_SHOW );
		
//#ifdef _DEBUG
		FILE* fp = fopen( "arg.txt", "wb" );
		fprintf( fp, "%s", szArg );
		fclose( fp );
		fp = fopen( "si.tmp", "wb" );
		fwrite( &nServerIdx, sizeof(int), 1, fp );
		fclose( fp );
		
//#endif

	} //if..else
	
	EndDialog( IDOK );
} //CLauncherDlg::PushStartButton