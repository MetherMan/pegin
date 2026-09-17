
/////////////////////////////////////////////////////
//
// Global.h
// Created By : sckyj
//
/////////////////////////////////////////////////////

class IMode
{
public :
	virtual BOOL Init() = 0;
	virtual BOOL Render() = 0;
	virtual	BOOL LDown( POINT pt ) = 0;
	virtual BYTE LUp( POINT pt ) = 0;
	virtual void Clear() = 0;
};

// include 
//	#include "Offset.h"
	#include "Bitmap.h"
	#include "Progress.h"
	#include "Patch.h"
	#include "Server.h"
	#include "Area.h"
	#include "Render.h"	
	#include "Webbrowser.h"
	#include "ServerMgr.h"
//~include 

// define	
	#define	MSG( t )			MessageBox( NULL, t, "Error", MB_OK )
	#define MSG2( t )			MessageBox( t )

//Mike's Laqia Server
	#define SERVER			"http://154.38.165.170/patch/"
	#define NOTICE			"http://154.38.165.170/patch/notice.html"
	
	//#define NOTICE				"http://222.122.25.48/patch/notice.html"//"http://laqia.gameking.com.cn/update.htm"	"http://deicideupdate.ongameport.com/patch/notice.html"
	//#define SERVER				"http://222.122.25.48/patch/"// "http://patchc.teammay.co.kr/~patch/"

	#define SAFE_DELETE( t )		{ if( t ) { delete t; t = NULL; } }
	#define SAFE_DELETE_ARRAY( t )	{ if( t ) { delete[] t; t = NULL; } }
//~define

// define
#define	WINDOW_NAME			"Launcher"

#define	WINDOW_WIDTH		800//505
#define	WINDOW_HEIGHT		600//431

#define	REG_BTN_LEFT		415		// start button
#define	REG_BTN_TOP			306
#define	REG_BTN_RIGHT		490
#define	REG_BTN_BOTTOM		330

#define	START_BTN_LEFT		415		// start button
#define	START_BTN_TOP		336
#define	START_BTN_RIGHT		490
#define	START_BTN_BOTTOM	360

#define	SETUP_BTN_LEFT		415		// setup button
#define	SETUP_BTN_TOP		366
#define	SETUP_BTN_RIGHT		490
#define	SETUP_BTN_BOTTOM	390

#define	CANCEL_BTN_LEFT		415		// cancel button
#define	CANCEL_BTN_TOP		396
#define	CANCEL_BTN_RIGHT	490
#define	CANCEL_BTN_BOTTOM	420

#ifdef  DAUM_AUTO_UPDATE

#define	NOTICE_LEFT			18		// html notice window
#define	NOTICE_TOP			29
#define	NOTICE_RIGHT		486
#define	NOTICE_BOTTOM		312

#define	FILE_PRO_LEFT		134		// 파일 전송 프로그래스
#define	FILE_PRO_TOP		361
#define	FILE_PRO_RIGHT		404
#define	FILE_PRO_BOTTOM		363

#define FILE_TXT_LEFT		139		// 파일 전송 텍스트
#define FILE_TXT_TOP		341
#define FILE_TXT_RIGHT		403
#define FILE_TXT_BOTTOM		353

#define	TOTAL_PRO_LEFT		134		// 총 전송 프로그래스
#define	TOTAL_PRO_TOP		379
#define	TOTAL_PRO_RIGHT		404
#define	TOTAL_PRO_BOTTOM	383

#define TOTAL_TXT_LEFT		140		// 총 전송 텍스트
#define TOTAL_TXT_TOP		375
#define TOTAL_TXT_RIGHT		403
#define TOTAL_TXT_BOTTOM	388

#else

#define	NOTICE_LEFT			135		// html notice window
#define	NOTICE_TOP			40
#define	NOTICE_RIGHT		485
#define	NOTICE_BOTTOM		267

#define	FILE_PRO_LEFT		135		// 파일 전송 프로그래스
#define	FILE_PRO_TOP		334
#define	FILE_PRO_RIGHT		405
#define	FILE_PRO_BOTTOM		337

#define FILE_TXT_LEFT		134		// 파일 전송 텍스트
#define FILE_TXT_TOP		346
#define FILE_TXT_RIGHT		404
#define FILE_TXT_BOTTOM		358

#define	TOTAL_PRO_LEFT		135		// 총 전송 프로그래스
#define	TOTAL_PRO_TOP		380
#define	TOTAL_PRO_RIGHT		405
#define	TOTAL_PRO_BOTTOM	383

#define TOTAL_TXT_LEFT		134		// 총 전송 텍스트
#define TOTAL_TXT_TOP		390
#define TOTAL_TXT_RIGHT		404
#define TOTAL_TXT_BOTTOM	402


#endif

// button status
#define BTN_RELEASE			0
#define	BTN_PRESS			1
#define BTN_NOTSUPPORT		2
//#define BTN_REG				3

// button type
#define BTN_START			0
#define BTN_CANCEL			1
#define BTN_SETUP			2
#define BTN_REG				3

#define	RENDER_TIMER_ID		1


/*******************************

  ServerDlg

********************************/

#define	SERVER_WIDTH		174
#define	SERVER_HEIGHT		301

#define LIST_X				11
#define LIST_Y				20
#define LIST_DX				152	
#define LIST_DY				215

#define SELECT_X			8 + SERVER_WIDTH
#define SELECT_Y			255
#define SELECT_DX			75
#define SELECT_DY			24
#define CANCEL_X			91 + SERVER_WIDTH
#define CANCEL_Y			255
#define CANCEL_DX			75
#define CANCEL_DY			24

// button status
#define BTN_RELEASE			0
#define	BTN_PRESS			1
#define BTN_NULL			2

// button type
#define BTN_SELECT			0
#define BTN_CANCEL			1
	// button status
/*	#define BTN_RELEASE			0
	#define	BTN_PRESS			1
	#define BTN_NOTSUPPORT		2*/

	#define BLK_RELEASE			0
	#define BLK_SELECT			1
	#define BLK_NOSERVICE		2

	// button type
	#define BTN_START			0
	#define BTN_CANCEL			1
	#define BTN_SETUP			2

	// Button Up Result
	#define BT_UP_CHANGEMODE	0
	#define BT_UP_NORMAL		1
	#define BT_UP_CANCEL		2
//~define

// extern
	extern CRender*			g_pRender;
	extern CWebBrowser*		g_pWeb;
	extern CServerMgr*		g_pServerMgr;

	extern int				g_nServerIndex;
//~extern





















