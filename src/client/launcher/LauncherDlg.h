// LauncherDlg.h : header file
//
//{{AFX_INCLUDES()
#include "webbrowser.h"
//}}AFX_INCLUDES

#if !defined(AFX_LAUNCHERDLG_H__B400501F_7820_46CC_8C5B_789775D031A2__INCLUDED_)
#define AFX_LAUNCHERDLG_H__B400501F_7820_46CC_8C5B_789775D031A2__INCLUDED_

#include "Bitmap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Bitmap.h"
#include "Progress.h"
#include "MsgMgr.h"
#include "Define.h"

/////////////////////////////////////////////////////////////////////////////
// CLauncherDlg dialog

class CLauncherDlg : public CDialog
{
// Construction
public:
	void			SkipPatch()		{ m_bSkipPatch = TRUE; }
	
	DWORD			SetCurrVersion(DWORD dwVersion);
	DWORD			GetCurrVersion();
	BOOL			GetServerName();
	void			SetDirectory();
	
	BOOL			IsValidPatchFile();
	int				HttpFileTransfer(const char *url);
	
	void			OnUpdateFinish(WPARAM wParam, LPARAM lParam);
	void			OnUpdateData(WPARAM wParam, LPARAM lParam);
	
	DWORD			ThreadProc();
	BOOL			UpdateProcess();
	
	BOOL			CheckButton(int type, CPoint point);
	void			PushStartButton();

	void			Render();

	HANDLE			m_hThread;
	DWORD			m_dwCurrVersion;
	int				m_nTotalFile;
	int				m_nFileCount;
	DWORD			m_dwTotalSize;
	DWORD			m_dwFileSize;	

	CMsgMgr			m_MsgMgr;

	my::CBitmap		m_bmBack;
	my::CBitmap		m_bmReg[3];
	my::CBitmap		m_bmStart[3];
	my::CBitmap		m_bmCancel[3];
	my::CBitmap		m_bmSetup[3];
	
	CProgress	    m_Progress[2];

	BOOL			m_bDone;
	int				m_nBtnState[4];
	char			m_szFullName[256];

	CString			m_szFileInfo;
	CString			m_szTotalInfo;
	CString			m_szServerName;
	BOOL			m_bPatchInfo;

	RECT			m_rtCancel;
	RECT			m_rtStart;
	RECT			m_rtSetup;
	RECT			m_rtReg;

	BOOL			m_bSkipPatch;
			
	CLauncherDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLauncherDlg)
	enum { IDD = IDD_LAUNCHER_DIALOG };
	CWebBrowser	m_WebBrowser;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLauncherDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLauncherDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHERDLG_H__B400501F_7820_46CC_8C5B_789775D031A2__INCLUDED_)
