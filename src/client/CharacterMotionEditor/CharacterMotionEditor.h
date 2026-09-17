// CharacterMotionEditor.h : main header file for the CHARACTERMOTIONEDITOR application
//

#if !defined(AFX_CHARACTERMOTIONEDITOR_H__3A2C1713_FAE1_4251_B5AE_5CD051BF0D99__INCLUDED_)
#define AFX_CHARACTERMOTIONEDITOR_H__3A2C1713_FAE1_4251_B5AE_5CD051BF0D99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CApp:
// See CharacterMotionEditor.cpp for the implementation of this class
//

class CApp : public CWinApp
{
public:
	CApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTERMOTIONEDITOR_H__3A2C1713_FAE1_4251_B5AE_5CD051BF0D99__INCLUDED_)
