// Launcher_Doc.cpp : implementation of the CLauncher_Doc class
//

#include "stdafx.h"
#include "Launcher_.h"

#include "Launcher_Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLauncher_Doc

IMPLEMENT_DYNCREATE(CLauncher_Doc, CDocument)

BEGIN_MESSAGE_MAP(CLauncher_Doc, CDocument)
	//{{AFX_MSG_MAP(CLauncher_Doc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLauncher_Doc construction/destruction

CLauncher_Doc::CLauncher_Doc()
{
}

CLauncher_Doc::~CLauncher_Doc()
{
}

BOOL CLauncher_Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLauncher_Doc serialization

void CLauncher_Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CLauncher_Doc diagnostics

#ifdef _DEBUG
void CLauncher_Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLauncher_Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLauncher_Doc commands
