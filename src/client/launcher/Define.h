#ifndef __DEFINE_HEADER__
#define __DEFINE_HEADER__



// - DEFINES
#define	WINDOW_NAME			"Launcher"

#define	WINDOW_WIDTH		505
#define	WINDOW_HEIGHT		431

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

//msgFile
#define dFILE_LAUNCHER_MSG	"Interface/LauncherMsg.txt"


#define STR_TEMPFILE		"si.tmp"

//#define dPATCH_SERVER_ADDR			"http://laqiap.daumgame.com/patch/"
//const char	g_szNoticeURL[]			= "http://laqiap.daumgame.com/patch/notice.html";
//#define 			"http://222.122.25.48/patch/"
//const char	g_szNoticeURL[]			= "http://222.122.25.48/patch/notice.html";

//Mike's Laqia Server
#define dPATCH_SERVER_ADDR			"http://ageofcignareonline.net/patch/"
const char	g_szNoticeURL[]			= "http://ageofcignareonline.net/patch/notice.html";

#endif // #ifndef __DEFINE_HEADER__




















