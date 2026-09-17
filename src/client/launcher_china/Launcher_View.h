// Launcher_View.h : interface of the CLauncher_View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAUNCHER_VIEW_H__8BF8E783_30D6_4A07_8E09_5C43B8CFE2A2__INCLUDED_)
#define AFX_LAUNCHER_VIEW_H__8BF8E783_30D6_4A07_8E09_5C43B8CFE2A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLauncher_View : public CView
{
protected: // create from serialization only
	CLauncher_View();
	DECLARE_DYNCREATE(CLauncher_View)
 
// Attributes
public:
	CLauncher_Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLauncher_View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLauncher_View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLauncher_View)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public :	

	BOOL		Render();

	void		CreateWeb();
	void		DestroyWeb();

private :

	void		OnUpdateFinish( WPARAM wParam, LPARAM lParam );
	void		OnUpdateData( WPARAM wParam, LPARAM lParam );

	void		OnPatchModeResult( BYTE btResult );
	void		OnSelectModeResult( BYTE btResult );
	void		SetDirectory();

private :

	CStatic*		m_pWebStatic;
	HFONT			m_hFont;

	char			m_szFullName[256];
	
	CString			m_szFileInfo;
	CString			m_szTotalInfo;
	CString			m_szServerName;
	BOOL			m_bPatchInfo;
};

#ifndef _DEBUG  // debug version in Launcher_View.cpp
inline CLauncher_Doc* CLauncher_View::GetDocument()
   { return (CLauncher_Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHER_VIEW_H__8BF8E783_30D6_4A07_8E09_5C43B8CFE2A2__INCLUDED_)
