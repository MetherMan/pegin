// Launcher_Doc.h : interface of the CLauncher_Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAUNCHER_DOC_H__ACD128F8_4447_42F1_AB62_3835ADDB4D8E__INCLUDED_)
#define AFX_LAUNCHER_DOC_H__ACD128F8_4447_42F1_AB62_3835ADDB4D8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLauncher_Doc : public CDocument
{
protected: // create from serialization only
	CLauncher_Doc();
	DECLARE_DYNCREATE(CLauncher_Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLauncher_Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLauncher_Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLauncher_Doc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAUNCHER_DOC_H__ACD128F8_4447_42F1_AB62_3835ADDB4D8E__INCLUDED_)
