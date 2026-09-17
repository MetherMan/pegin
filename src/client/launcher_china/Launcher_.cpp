// Launcher_.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Launcher_.h"

#include "MainFrm.h"
#include "Launcher_Doc.h"
#include "Launcher_View.h"
//{{AFX_INCLUDES()
#include "webbrowser.h"
//}}AFX_INCLUDES

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLauncher_App

BEGIN_MESSAGE_MAP(CLauncher_App, CWinApp)
	//{{AFX_MSG_MAP(CLauncher_App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLauncher_App construction

CLauncher_App::CLauncher_App()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLauncher_App object

CLauncher_App theApp;

/////////////////////////////////////////////////////////////////////////////
// CLauncher_App initialization

BOOL CLauncher_App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	/*
		TODO : Something u want to do
	*/
	HANDLE hInstanceMutex;
	hInstanceMutex = CreateMutex( NULL, FALSE, "AutoPatch" );
	if( WaitForSingleObject( hInstanceMutex, 0 ) != WAIT_OBJECT_0 )
	{
		MessageBox( NULL, "Already Working Patch Program", "Decide Online", 0 );
		return FALSE;
	} // if

	// Register document templates

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CLauncher_Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLauncher_View));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CWebBrowser	m_pBrowser;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_BROWSER, m_pBrowser);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CLauncher_App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CLauncher_App message handlers


BOOL CLauncher_App::OnIdle(LONG lCount) 
{
	return CWinApp::OnIdle(lCount);
}

void CLauncher_App::OnFinalRelease() 
{	
	CWinApp::OnFinalRelease();
}
