// Launcher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLauncherApp

BEGIN_MESSAGE_MAP(CLauncherApp, CWinApp)
	//{{AFX_MSG_MAP(CLauncherApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLauncherApp construction

CLauncherApp::CLauncherApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLauncherApp object

CLauncherApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLauncherApp initialization

BOOL CLauncherApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	HANDLE hInstanceMutex;
	hInstanceMutex = CreateMutex(NULL, FALSE, "laqiaautopatch");
	if (WaitForSingleObject(hInstanceMutex, 0) != WAIT_OBJECT_0)
	{
		MessageBox( NULL, "이미 오토패치가 실행중입니다.", "Laqia AutoPatch", 0 );
		return FALSE;
	}

	CLauncherDlg LauncherDlg;

	char*	pCmdLine = GetCommandLine();
	if( pCmdLine && strlen( pCmdLine ) > 0 )	
	{
		if( pCmdLine[strlen( pCmdLine )-1] == '1' )
		{
			LauncherDlg.SkipPatch();
		} //if
	} //if


	m_pMainWnd = &LauncherDlg;
	int nResponse = LauncherDlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{	
	}	

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
