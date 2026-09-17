// Launcher_View.cpp : implementation of the CLauncher_View class
//

#include "stdafx.h"
#include "Launcher_.h"

#include "Global.h"
#include "MainFrm.h"
#include <atlbase.h>
#include <afxinet.h>
#include <io.h>
#include <Mmsystem.h>

#include "Launcher_Doc.h"
#include "Launcher_View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_WEBSTATIC	4000

/////////////////////////////////////////////////////////////////////////////
// CLauncher_View

IMPLEMENT_DYNCREATE(CLauncher_View, CView)

BEGIN_MESSAGE_MAP(CLauncher_View, CView)
	//{{AFX_MSG_MAP(CLauncher_View)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_MESSAGE( WM_UPDATE_FINISH, OnUpdateFinish )
	ON_MESSAGE( WM_UPDATE_DATA, OnUpdateData )
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLauncher_View construction/destruction

CLauncher_View::CLauncher_View()
:	m_pWebStatic	( NULL )
{
	
}

CLauncher_View::~CLauncher_View()
{
}

BOOL CLauncher_View::PreCreateWindow(CREATESTRUCT& cs)
{	
	return CView::PreCreateWindow(cs);
}

void CLauncher_View::SetDirectory()
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

void CLauncher_View::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	SetDirectory();
	
	// Initilize
	g_hWnd		= GetSafeHwnd();
	g_hInstance	= AfxGetInstanceHandle();	

	RECT rt;
	SetRect( &rt, 0, 0, 10, 10 );
	m_pWebStatic = new CStatic;	
	if( !m_pWebStatic || 
		!m_pWebStatic->Create( "WEB", WS_CHILD | WS_VISIBLE, rt, this, IDC_WEBSTATIC ) )
	{
		MSG2( "Could NOT Create WEB STATIC Control" );
		return;
	} // if	

	CreateWeb();

	// Read Server Name Data
	g_pServerMgr = new CServerMgr;
	g_pServerMgr->Init();

	// Create Render Class
	g_pRender = new CRender;	
	if( !g_pRender || !g_pRender->Init() )
	{
		MSG2( "Could NOT Start Render" );
		return;
	} // if

	SetTimer(1, 300, NULL);
	
} // if

void CLauncher_View::CreateWeb()
{
	SAFE_DELETE( g_pWeb );
	
	RECT rt;
	SetRect( &rt, 0, 0, 469, 284 );	
	g_pWeb = new CWebBrowser;

	if( !g_pWeb || !g_pWeb->Create( "WebBrowser", "Web", WS_CHILD | WS_VISIBLE, rt, this, IDC_WEBSTATIC ) )
	{
		MSG2( "Could NOT Create WEB BROWSER Control" );
		return;
	} // if	

	g_pWeb->SetWindowPos( NULL, NOTICE_LEFT, NOTICE_TOP, NOTICE_RIGHT - NOTICE_LEFT, NOTICE_BOTTOM - NOTICE_TOP, NULL );
	g_pWeb->Navigate( NOTICE, NULL, NULL, NULL, NULL );
	if( g_pWeb->GetBusy() )
	{
		Sleep( 200 );
	} // if
} // CLauncher_View::CreateWeb

void CLauncher_View::DestroyWeb()
{
	g_pWeb->DestroyWindow();
	SAFE_DELETE( g_pWeb );
} // CLauncher_View::DestroyWeb

void CLauncher_View::PostNcDestroy() 
{	
	CView::PostNcDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CLauncher_View drawing

void CLauncher_View::OnDraw(CDC* pDC)
{	
	CLauncher_Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Render();
}

/////////////////////////////////////////////////////////////////////////////
// CLauncher_View diagnostics

#ifdef _DEBUG
void CLauncher_View::AssertValid() const
{
	CView::AssertValid();
}

void CLauncher_View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLauncher_Doc* CLauncher_View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLauncher_Doc)));
	return (CLauncher_Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLauncher_View message handlers
BOOL CLauncher_View::Render()
{
	if( g_pRender && 
		!g_pRender->Render() )
	{
		return FALSE;
	} // if

	return TRUE;
} // CLauncher_View::Render

/**
 *	Msg Handler
 */
void CLauncher_View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

	if( !g_pRender->LDown( point ) )
	{
		pFrm->SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, ( LPARAM )MAKELPARAM( point.x, point.y ) );
	} // if	
	
	CView::OnLButtonDown(nFlags, point);
}

void CLauncher_View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	BYTE btResult = g_pRender->LUp( point );

	if( g_pRender->GetMode() == CRender::MODE_PATCH )
	{
		OnPatchModeResult( btResult );
	} 
	else
	{
		OnSelectModeResult( btResult );
	} // if..else..
	
	CView::OnLButtonUp(nFlags, point);
}

void CLauncher_View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if( g_pRender->GetMode() == CRender::MODE_SERVERSELECT )
	{
		if( g_nServerIndex >= 0 )
		{
			OnSelectModeResult( 1 );
		} // if		
	} // if
	
	CView::OnLButtonDblClk(nFlags, point);
}

void CLauncher_View::OnPatchModeResult( BYTE btResult )
{
	switch( btResult )
	{
	case 0 : 
		g_pRender->ChangeMode();

		DestroyWeb();
		break;
			
	case 1 :
		// Video Setup
		ShellExecute( NULL, NULL, "VideoSetup.exe", "", NULL, SW_SHOW );

		PostQuitMessage( 0 );
		break;

	case 2 :
		PostQuitMessage( 0 );
		break;		
	case 3:
		ShellExecute( GetSafeHwnd(), NULL, "C:\\Program Files\\Internet Explorer\\iexplore.exe", 
			"http://deicide.ongameport.com/account/01_Register_Step1.php", NULL, SW_SHOW );
		PostQuitMessage( 0 );
	}

} // CLauncher_View::OnPatchModeResult

void CLauncher_View::OnSelectModeResult( BYTE btResult )
{
	FILE*	pFile;

	switch( btResult )
	{
	case 0 :
		g_pRender->ChangeMode();
		CreateWeb();
		break;

	case 1 :

		pFile = fopen( "si.tmp", "wb" );
		if( pFile )
		{
			fwrite( &g_nServerIndex, 1, sizeof( int ), pFile );
			fclose( pFile );
			ShellExecute( NULL, NULL, "DeicideOnline.exe", "노패치투", NULL, SW_SHOW );
		}
		else
		{
			MessageBox( "Server Index Error" );
		} //if..else

		PostQuitMessage( 0 );
		break;
	}
} // CLauncher_View::OnSelectModeResult

void CLauncher_View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CLauncher_View::OnUpdateFinish(WPARAM wParam, LPARAM lParam)
{	
} // CLauncher_View::OnUpdateFinish

void CLauncher_View::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
} // CLauncher_View::OnUpdateData

void CLauncher_View::OnTimer(UINT nIDEvent) 
{	
	Render();

	CView::OnTimer(nIDEvent);
}

void CLauncher_View::OnDestroy() 
{
	CView::OnDestroy();
	
	g_pRender->Clear();
	SAFE_DELETE( g_pRender );	
	SAFE_DELETE( g_pServerMgr );

	KillTimer( 1 );
}

