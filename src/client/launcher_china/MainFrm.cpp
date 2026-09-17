// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Launcher_.h"

#include "MainFrm.h"

#define	WINDOW_WIDTH		507
#define	WINDOW_HEIGHT		433

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{ 
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	// Window 타이틀 바 없애기
	cs.style = WS_POPUP;
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    cs.cx = WINDOW_WIDTH;									// 윈도우의 가로크기
    cs.cy = WINDOW_HEIGHT;									// 윈도우의 세로크기
    cs.x = ( cx - WINDOW_WIDTH ) / 2;						// 윈도우의 좌상단 x 좌표 
    cs.y = ( cy - WINDOW_HEIGHT ) / 2;						// 윈도우의 좌상단 y 좌표
    CMenu *pMenu = new CMenu;						// 새로운 메뉴를 만든다.
    pMenu->Attach(cs.hMenu);						// 메인프레임에 설정된 메뉴와 연결
    pMenu->DestroyMenu();							// 메뉴삭제
    cs.hMenu = NULL;
    delete pMenu;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText("Deicide Online Auto Launcher");
		
	return 0;
}
