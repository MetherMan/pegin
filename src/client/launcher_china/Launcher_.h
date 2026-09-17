// Launcher_.h : main header file for the LAUNCHER_ application
//

#if !defined(AFX_LAUNCHER__H__2AA508D0_AFCE_4611_97A0_2AB254F15AD0__INCLUDED_)
#define AFX_LAUNCHER__H__2AA508D0_AFCE_4611_97A0_2AB254F15AD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CLauncher_App:
// See Launcher_.cpp for the implementation of this class
//

class CLauncher_App : public CWinApp
{
public:
	CLauncher_App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLauncher_App)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CLauncher_App)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHER__H__2AA508D0_AFCE_4611_97A0_2AB254F15AD0__INCLUDED_)
