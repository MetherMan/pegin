/**
 * 메인 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 179 $
 *
 * <HR>
 * $Log: /GameClient/Src/WinMain.cpp $
 * 
 * 179   05-02-18 7:38p Sk8snow2
 * 
 * 178   04-12-08 3:06a Sk8snow2
 * 
 * 177   04-08-23 8:39p Paran
 * 
 * 176   04-08-03 9:53p Paran2
 * 
 * 168   04-02-27 9:43p Kjmgo			펫추가 ^.^/
 * 
 * 151   04-01-07 10:54a Sckyj			중국 lpcmdLine 한개 더 추가 - 서버 인덱스
 * 
 * 150   04-01-02 2:20p Paran			메세지 define 처리
 * 
 * 148   03-12-31 13:09 Sckyj			IsQuote() : Add Chinese Double Quote In Chinaversion
 * 
 * 146   03-12-26 5:20p Sckyj			ChangeIcon() 제거
 * 
 * 144   03-12-26 10:45a Sckyj			메칸더를 NOPATCH로 변경
 * 
 * 135   03-12-20 3:33a Sckyj			CheckIcon() 함수 수정
 * 
 * 134   03-12-20 1:37a Kjmgo			메모리 검사를 종전 3백메가에서 6백메가로 증가
 * 
 * 131   03-12-19 10:24p Sckyj			ChangeIcon() 함수 추가
 * 
 * 126   03-12-17 7:50a Kjmgo			선택 유지 구현 ( 選擇[せんたく] 維持[いじ] )
 * 
 * 123   03-11-22 4:53p Sckyj			중국어버젼일경우 사용자의 해상도를 변경하도록 수정 
 * 
 * 122   03-11-22 11:42a Kjmgo			디버그 모드일때, 루프에 Sleep 추가
 * 
 * 121   03-11-17 6:21p Kjmgo			Loop중 변수 선언을 외부로 뺌
 * 
 * 118   03-10-10 5:24p Kjmgo			메모리 주의를 200메가에서 300메가로 수정
 * 
 * 117   03-10-10 1:56p Kjmgo			Log로 게임 시작및, 종료를 남기게 됨
 * 
 * 116   03-10-10 10:32a Kjmgo			예외 처리 모두 해제하고, Err_Msg와 Caution_Msg로, 생성 초기화 오류 세팅
 * 
 * 115   03-10-10 9:50a Kjmgo			메모리 부족 경고를 좀더 여유롭게 잡도록
 * 
 * 114   03-10-08 12:16p Kjmgo			메모리가 40메가 일떄는 아예 실행 자체를 하지 않도록 함
 * 
 * 113   03-10-08 12:13p Kjmgo			메모리를 검사하여 200메가 이하로 남았을떄 경고 메시지 출력
 * 
 * 112   03-10-03 7:51p Kjmgo			프레임 넘길떄마다 Randomize함수 호출
 * 
 * 109   03-09-02 5:49a Kjmgo			Monster 추가작업
 * 
 * 108   03-08-30 11:07a Kjmgo			Character Animation Data 읽는 부분 추가
 * 
 * 98    03-05-29 2:28p Paran			아이템 손바꾸기 패킷 수정.
 *
 * 90    03-05-10 12:54p Ahastudio		Engine에서 Creature Implement 관련 클래스를 Game으로 이동함.
 *
 * 88    03-05-06 11:51p Ahastudio		Lua 추가 및 초기화 변수 설정 부분 변경.
 * <HR>
 *
 * @file	WinMain.cpp
 */

#include "Global.h"
#include "resource.h"
#include "Client.h"
#include <io.h>
#include <string>
#include <vector>
#include <fcntl.h>

using std::string;
using std::vector;


const char	WND_CLASS_NAME[]	= "Wind3D Window Class";	///< 메인 윈도우 클래스 이름

#ifdef _CHINAVERSION
const char  SKIP_COMMAND[]		= "노패치투";
const char	WND_TITLE[]			= "Deicide Online";			///< 윈도우 타이틀
#define STR_TEMPFILE			"si.tmp"
#else
const char  SKIP_COMMAND[]		= "NOPATCH";
const char	WND_TITLE[]			= "Laqia Online";			///< 윈도우 타이틀
#endif //_CHINAVERSION

#define			ERR_MSG( s )		MessageBox( g_hWnd, s, STR_SYSERR_ERRORMESSAGE, MB_OK )
#define			CAUTION_MSG( s )	MessageBox( g_hWnd, s, STR_SYSERR_WARNINGMESSAGE, MB_OK )
#define			WINDOWCURSOR	TRUE


HWND					g_hWnd		= NULL;			///< 메인 윈도우 핸들
BOOL					g_bActive	= FALSE;		///< 메인 윈도우가 활성화됐는지 여부


static vector<string>	g_astrEnginePackage;		///< 게임 엔진 패키지들

static void		Err_Msg( char* szMsg );
static void		Caution_Msg( char* szMsg );
static void		Shutdown();
static void		LoadEngine();
static void		UnloadEngine();
static void		CloseLua();
static void		SetDirectory();


static BOOL		InitLua();
static BOOL		CreateMainWindow( HINSTANCE hInstance, int nCmdShow, int nWidth, int nHeight, BOOL bFullscreen );
static BOOL		ChangeDisplay( BOOL bChange, int nWidth = 0, int nHeight = 0, int nDepth = 0 );
static BOOL		Initialize( HINSTANCE hInstance, int nCmdShow );
static BOOL		InitIni();
BOOL		LoadGlobalData();

static IFrame*	InitFirstFrame( HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, string& sFrameName );
static LRESULT CALLBACK		MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
BOOL			CheckFolderAndFiles();
BOOL			CheckMonsterFiles();


/*
 *	Hack Detect
 */
#define dMAX_HACK_DETECT		128
#define dHACK_NAME_LEN			32			

class CHackDetect
{
public:
	char m_HackName[dMAX_HACK_DETECT][dHACK_NAME_LEN];

	CHackDetect()
	{
		for( int i = 0; i < dMAX_HACK_DETECT; i++ )
		{
			*m_HackName[i] = NULL;
		}
	};

	void SetDetect( char *szName )
	{
		for( int i = 0; i < dMAX_HACK_DETECT; i++ )
		{
			if( *m_HackName[i] == NULL )
			{
				strcpy( m_HackName[i], szName );
				break;
			}
		}
	};

	BOOL LoadList()
	{
		FILE *fp = fopen( "./Interface/hdl.wtm", "rb" );

		if( !fp )
			return FALSE;

		char szName[128];

		while( !feof( fp ) )
		{
			fscanf( fp, "%s", szName );
			SetDetect( szName );
		}

		fclose( fp );
		return TRUE;
	};

	BOOL HackDetect()
	{
		for( int i = 0; i < dMAX_HACK_DETECT; i++ )
		{
			if( *m_HackName[i] != NULL )
			{
				if( FindWindow( m_HackName[i], NULL ) != NULL ||
					FindWindow( NULL, m_HackName[i] ) != NULL )
					return TRUE;
			}
			else
				return FALSE;
		}
		return FALSE;
	};

};

CHackDetect			g_HackDetect;

/**
 * Windows Application 메인 함수.
 */
int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow
	)
{
	MEMORYSTATUS status;
	GlobalMemoryStatus( &status ); 

	if( status.dwAvailPageFile <= 94003200 )
	{
		MessageBox( g_hWnd, STR_SYSERR_NOTENOUGHMEMORY1, STR_SYSERR_ERRORMESSAGE, MB_OK );
		return 0;
	}

	BOOL		bQuit = FALSE;
	MSG			msg;

	IFrame*		pFrame = NULL;
	string		sFrameName;

	SetDirectory();		// 게임이 존재하는 디렉토리로 이동한다.

	if( _access( "_AutoPatch.exe", 0 ) != -1 )
	{	// AutoPatch의 새로운 버전을 받은 경우, 교체해서 실행한다.
		Sleep( 1000 );
		DeleteFile( "AutoPatch.exe" );
		rename( "_AutoPatch.exe", "AutoPatch.exe" );
		ShellExecute( NULL, NULL, "AutoPatch.exe", "", NULL, SW_SHOW );
		return 0;
	} //if

/*	if( string( lpCmdLine ) != SKIP_COMMAND )
	{	// AutoPatch에서 실행하지 않은 경우, AutoPatch를 실행한다.
		ShellExecute( NULL, NULL, "AutoPatch.exe", "", NULL, SW_SHOW );
		return 0;
	} //if*/
/*
#ifdef _CHINAVERSION
	if( !g_HackDetect.LoadList() )
	{
		MessageBox( g_hWnd, NULL, "can not find hardware auto dect information list", MB_OK );
		return 0;
	}

	if( g_HackDetect.HackDetect() )
		return 0;
#endif*/
/*	if( _access( "gulim.ttf", 0 ) != -1 )
	{

	}*/
	

	pFrame = InitFirstFrame( hInstance, lpCmdLine, nCmdShow, sFrameName );
	if( !pFrame )
	{
		ChangeDisplay( FALSE );
		return 0;
	} //if

	int		dwTick		= 0;
	DWORD	dwOldTick	= GetTickCount();
	DWORD	dwCurTick	= 0;

#ifdef _CHINAVERSION
	DWORD	dwHackCheckTime = 0;
#endif

	while( !bQuit )
	{

#ifdef _CHINAVERSION
		if( dwHackCheckTime + 60000 <= timeGetTime() )
		{
			dwHackCheckTime = timeGetTime();
			if( g_HackDetect.HackDetect() )
			{
				bQuit = TRUE;
			}
		}
#endif

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( WM_QUIT == msg.message )
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			} //if..else..
		}
		else
		{				
			dwCurTick = GetTickCount();
			
			dwTick    = MIN( dwCurTick - dwOldTick, 200 );	// 최대 경과 Tick 설정 ( 최소한의 애니메이션 처리를 보장 )
			dwOldTick = dwCurTick;
			
			static DWORD dwTicks = 0;
			dwTicks += dwTick;
			
			if( !pFrame->Update( dwTick, &sFrameName ) )
			{
				bQuit = TRUE;
				break;
			} //if
			
			if( !sFrameName.empty() )
			{	// 다른 상태 프레임으로 넘김.
				SAFE_DELETE( pFrame );
				
				randomize();
				
				pFrame = MakeFrame( sFrameName );
				pFrame->Init();
				
				sFrameName = "";
			} //if
		} //if..else..
	} //while..

	SAFE_DELETE( pFrame );

	Shutdown();

	return msg.wParam;
} //WinMain

/**
 * 애플리케이션 초기화.
 */
BOOL Initialize( HINSTANCE hInstance, int nCmdShow )
{
/*	if( CheckFolderAndFiles() == FALSE )
	{
		return FALSE;
	}*/
	LoadEngine();

	if( CheckMonsterFiles() == FALSE )
	{
		Err_Msg( "YOU ARE MOD PLAYER!!!!!" );
		return FALSE;
	}

	//ChangeDisplay( TRUE, 0, 0, 32 );
	g_pGUI = MAKE_COMPONENT( UInterface );
	if( !g_pGUI ||
		!g_pGUI->Init( g_nWidth, g_nHeight, WINDOWCURSOR ) )
	{
		Assert( !"GUI생성 실패" );
		Err_Msg( "GUI생성 실패입니다." );
		return FALSE;
	} //if

#ifdef _CHINAVERSION

#ifndef _DEBUG
	if( g_bFullscreen == TRUE )
		ChangeDisplay( TRUE, g_nWidth, g_nHeight, g_nColorDepth );
#endif

#endif

	if( !CreateMainWindow( hInstance, nCmdShow, g_nWidth, g_nHeight, g_bFullscreen ) )
	{
		return FALSE;
	} //if

	g_pInput = MAKE_COMPONENT( Input );
	if( !g_pInput||
		!g_pInput->Init( hInstance, g_hWnd, TRUE ) )
	{
		Assert( !"다이렉트 인풋 초기화 실패" );
		Err_Msg( "Direct Input 초기화 실패입니다." );
		return FALSE;
	} //if

	g_pInput->SetViewport( 0, 0, g_nWidth, g_nHeight );

	g_pScene = MAKE_COMPONENT( Scene );	

	if( !g_pScene ||
		!g_pScene->Init( g_hWnd, g_nWidth, g_nHeight, g_nColorDepth, g_bFullscreen ) )
	{
		Assert( !"다이렉트 인풋 초기화 실패" );
		Err_Msg( "Scene 초기화 실패입니다." );
		return FALSE;
	} //if
	
	g_pScene->SetTexLevel( g_nTextureLevel );

	g_pScene->SetGammaControl( 0.8F, 0.8F, 0.8F );
	
	g_pSoundMgr = MAKE_COMPONENT( SoundMgr );
	
	if( !g_pSoundMgr ||
		!g_pSoundMgr->Init() )
	{
		Assert( !"사운드 매니저 생성 실패" );
		Caution_Msg( "사운드 생성 실패입니다." );
	} //if

	g_pSoundMgr->SetPosition( Vector3( 0.0F, 0.0F, 0.0F ), Vector3( 1.0F, 1.0F, 0.0F ) );

	g_pMediaPlayer = MAKE_COMPONENT( MediaPlayer );

	if( !g_pMediaPlayer->Init( g_hWnd ) )
	{
		Assert( !"미디어 매니저 생성 실패" );
		Caution_Msg( "미디어 생성 실패입니다." );
	} //if

	g_pItemInfoMgr	= MAKE_COMPONENT( ItemInfoMgr );
	
	if( !g_pItemInfoMgr ||
		!g_pItemInfoMgr->Init() )
	{
		Assert( !"Item정보 생성 실패" );
		Err_Msg( "Item정보 생성 실패입니다." );
		return FALSE;
	} //if

	if( !g_pGUI->SetWinDevice( g_hWnd, g_pInput ) )
	{
		Assert( !"Filed Setting Device UInterface" );
		return FALSE;
	} //if

	g_pTrace = new CSwordTrace;
	if( !g_pTrace )
	{
		Assert( !"할당실패" );
	} //if


//	ShowCursor( FALSE );

//#ifdef _DEBUG 
	g_pScene->ShowFrameRate( g_bFPS_On );
//#endif

	return TRUE;
} //Initialize

BOOL InitIni()
{
	WSADATA	wsadata;
	WSAStartup( MAKEWORD(1,1), &wsadata );
	if( !InitLua() )
	{
		return FALSE;
	} //if
		
	if( lua_dofile( g_pLuaVM, "./config.ini" ) ||
		lua_dofile( g_pLuaVM, "./server.ini" ) ||
		lua_dofile( g_pLuaVM, "./start.lua" ) )
	{
		Assert( !"스크립트 에러" );
		Err_Msg( "스크립트 오류입니다." );
		return FALSE;
	} //if

	g_pClient = new CClient;
	if( !g_pClient )
	{
		Assert( !"할당실패" );
		return FALSE;
	} //if

	return TRUE;
}

IFrame* InitFirstFrame( HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow, string& sFrameName )
{
	sFrameName	= "";
	IFrame*		pFrame = NULL;
#ifdef _CHINAVERSION
	if( _access( "server_c.ini", 0 ) != -1 )
	{
		rename( "server.ini", "server_k.ini" );
		rename( "server_c.ini", "server.ini" );
	} //if

	InitIni();
 
	/*FILE	*pFile = fopen( STR_TEMPFILE, "rb" );
	if( !pFile )
	{	
		if( string( lpCmdLine ) != SKIP_COMMAND )
		{	// AutoPatch에서 실행하지 않은 경우, AutoPatch를 실행한다.
			ShellExecute( NULL, NULL, "AutoPatch.exe", "", NULL, SW_SHOW );
			return NULL;
		} //if
	}
	else
	{
		int			nServeIdx;
		fread( &nServeIdx, 1, sizeof( int ), pFile );
		fclose( pFile );
		remove( STR_TEMPFILE );

		if( nServeIdx >= NUM_GAMESERVER ||
			nServeIdx < 0 )
		{
			Assert( !"없는 서버인덱스" );
			return NULL;
		} //if
		g_btServer = nServeIdx;
		char		szTitleName[125];
		sprintf( szTitleName, "%s - %s", WND_TITLE, g_asGameServerHost[g_btServer].c_str() );
	} //if..else
*/
	if( !Initialize( hInstance, nCmdShow ) )
	{
		Shutdown();
		return NULL;
	} //if

	pFrame = MakeFrame( "Intro" );		// Intro -> Login -> SelectCharacter -> Game
#else
	if( _access( "server_k.ini", 0 ) != -1 )
	{
		rename( "server.ini", "server_c.ini" );
		rename( "server_k.ini", "server.ini" );
	} //if

	InitIni();

	pFrame = MakeFrame( "Login" );		// Login -> Intro -> SelectCharacter -> Game
	if( !pFrame->Init() )
	{
		SAFE_DELETE( g_pClient );
		SAFE_DELETE( pFrame );	
		return NULL;
	} //if
	
	if( !Initialize( hInstance, nCmdShow ) )
	{
		Shutdown();
		return NULL;
	} //if

	SAFE_DELETE( pFrame );	
	pFrame = MakeFrame( "Intro" );
#endif //_CHINAVERSION

	pFrame->Init();

	return pFrame;
} //InitFirstFrame

/**
 * 전역 데이터를 로드한다.
 */
BOOL LoadGlobalData()
{
	if( !g_pTrace || 
		!g_pTrace->Load( "Equip/Trace.txt" ) )
	{
		Assert( !"Trace 정보 로드 실패" );
		Caution_Msg( "Trace Effect 정도 로드 실패입니다." );
	} //if

	g_pCharacterState = new CCharacterState;
	g_pMonsterState	  = new CMonsterState;
	g_pVehicleState   = new CVehicleState;

	if( !g_pCharacterState->Load( "Body/Character.wad" ) ||
		!g_pMonsterState->Load( "Monster/monster.wad" )	||
		!g_pVehicleState->Load( "Vehicle/vehicle.wad" ) )
	{
		Assert( !"Creature State Load 실패" );
		Err_Msg( "Creature의 정보 로드 실패입니다." );
		return FALSE;
	} //if

	return TRUE;
} //LoadGlobalData

/**
 * 애플리케이션 종료시 처리.
 */
void Shutdown()
{
	ShowCursor( TRUE );

	ChangeDisplay( FALSE );

	SAFE_DELETE( g_pMonsterState );
	SAFE_DELETE( g_pCharacterState );
	SAFE_DELETE( g_pVehicleState );
	SAFE_DELETE( g_pTrace );
	SAFE_RELEASE( g_pGUI );
	SAFE_RELEASE( g_pItemInfoMgr );
	SAFE_RELEASE( g_pMediaPlayer );
	SAFE_RELEASE( g_pSoundMgr );
	SAFE_RELEASE( g_pScene );
	SAFE_RELEASE( g_pInput );
	SAFE_DELETE( g_pClient );

	DestroyWindow( g_hWnd );

	UnloadEngine();

	CloseLua();

	Log( "== Exit Game ==" );
} //Shutdown

/**
 * Engine DLL을 로드한다.
 */
void LoadEngine()
{
	for( int i = 0 ; i < g_astrEnginePackage.size() ; i++ )
	{
		LoadComponentPackage( g_astrEnginePackage[i].c_str() );
	} //for

	DECLARE_COMPONENT( Character );
	DECLARE_COMPONENT( Monster );
	DECLARE_COMPONENT( NPC );
	DECLARE_COMPONENT( Mercenary );

	static ComponentInfo*	apComponentInfo[] =
	{
		COMPONENT( Character )
		COMPONENT( Monster )
		COMPONENT( NPC )
		COMPONENT( Mercenary )
		0
	}; //static ComponentInfo*	apComponentInfo[] =

	AddComponent( apComponentInfo );
} //LoadEngine

/**
 * Engine DLL을 해제한다.
 */
void UnloadEngine()
{
	for( int i = 0 ; i < g_astrEnginePackage.size() ; i++ )
	{
		UnloadComponentPackage( g_astrEnginePackage[i].c_str() );
	} //for
} //UnloadEngine

/**
 * Lua "l_SetVideo" Binding Function.
 */
int l_SetVideo( lua_State* pLuaVM )
{
	g_nWidth		= lua_tonumber( pLuaVM, 1 );
	g_nHeight		= lua_tonumber( pLuaVM, 2 );

	g_nColorDepth	= lua_tonumber( pLuaVM, 3 );
	g_bFullscreen	= lua_tonumber( pLuaVM, 4 );
	g_nTextureLevel = lua_tonumber( pLuaVM, 5 );
	g_bShadowOn		= lua_tonumber( pLuaVM, 6 );
//	g_nMapQuality	= lua_tonumber( pLuaVM, 7 );
	g_bFPS_On		= lua_tonumber( pLuaVM, 7 );

	if( g_bFullscreen == 1 )
	{
		g_nWidth		= GetSystemMetrics( SM_CXSCREEN );
		g_nHeight		= GetSystemMetrics( SM_CYSCREEN );
		
		if( g_nWidth > 1920 )
		{
			g_nWidth = 1920;
			g_nHeight = 1080;
		}
	}

	g_bFPS_On = !g_bFPS_On;
	g_bShadowOn = !g_bShadowOn;
	return 1;
} //l_SetVideo

/**
 * Lua "SetLoginServer" Binding Function.
 */
int l_SetLoginServer( lua_State* pLuaVM )
{
//	g_sDaumAccountHost = lua_tostring( pLuaVM, 1 );
	g_sLoginServerHost = lua_tostring( pLuaVM, 1 );
	g_btGameServerNum  = lua_tonumber( pLuaVM, 2 );
	Assert( g_btGameServerNum <= NUM_GAMESERVER );

	int		nReadLine = 3;
	for( int nServerNum = 0; nServerNum < g_btGameServerNum; nServerNum++ )
	{
		g_asGameServerHost[nServerNum] = lua_tostring( pLuaVM, nReadLine );
		nReadLine++;
	} //if
	
	return 1;
} //l_SetLoginServer

/**
 * Lua "AddPackage" Binding Function.
 */
int l_AddPackage( lua_State* pLuaVM )
{
	string	strPackageName = lua_tostring( pLuaVM, 1 );

	g_astrEnginePackage.push_back( strPackageName );

	return 1;
} //l_AddPackage

/**
 * Lua Virtual Machine을 초기화한다.
 */
BOOL InitLua()
{
	Assert( NULL == g_pLuaVM );

	g_pLuaVM = lua_open();

	if( !g_pLuaVM )
	{
		Assert( !"스크립트 엔진 초기화 실패" );
		Err_Msg( "스크립트 엔진 초기화 실패입니다." );
		return FALSE;
	} //if

	lua_register( g_pLuaVM, "SetVideo",			l_SetVideo );
	lua_register( g_pLuaVM, "SetLoginServer",	l_SetLoginServer );
	lua_register( g_pLuaVM, "AddPackage",		l_AddPackage );

	return TRUE;
} //InitLua

/**
 * Lua Virtual Machine을 종료한다.
 */
void CloseLua()
{
	if( g_pLuaVM )
	{
		lua_close( g_pLuaVM );
		g_pLuaVM = NULL;
	} //if
} //CloseLua

/**
 * 애플리케이션이 실행된 디렉토리로 이동한다.
 */
void SetDirectory()
{
	char	szModuleFileName[MAX_PATH];
	GetModuleFileName( NULL, szModuleFileName, MAX_PATH );

	char	szDrive[MAX_PATH];
	char	szDir[MAX_PATH];
	char	szName[MAX_PATH];
	char	szExt[MAX_PATH];
	_splitpath( szModuleFileName, szDrive, szDir, szName, szExt );

	char	szPathName[MAX_PATH];
	sprintf( szPathName, "%s%s", szDrive, szDir );

	SetCurrentDirectory( szPathName );
} //SetDirectory

/**
 * 메인 윈도우를 생성한다.
 *
 * @return		윈도우 생성 성공 여부.
 *
 * @exception	윈도우 생성 실패할 경우, W3DException.
 */
BOOL CreateMainWindow(
	HINSTANCE	hInstance,		///< [IN] 어플리케이션 인스턴스
	int			nCmdShow,		///< [IN] Command Show
	int			nWidth,			///< [IN] 창의 가로 길이
	int			nHeight,		///< [IN] 창의 세로 길이
	BOOL		bFullscreen		///< [IN] 전체 화면 사용 여부
	)
{
#ifdef _CHINAVERSION
	int		nIcon = IDI_DEICIDE_ICON;
#else
	int		nIcon = IDI_DEICIDE_ICON;//IDI_LAQIA_ICON;
#endif

	WNDCLASS wc;
	wc.style         = NULL;
	wc.lpfnWndProc   = (WNDPROC)MainWndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetModuleHandle( NULL );
	wc.hIcon         = LoadIcon( hInstance, MAKEINTRESOURCE( nIcon ) );
	wc.hCursor       = NULL;//(HCURSOR)LoadImage( NULL, "cursor_nw.wtm", IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE );
	wc.hbrBackground = NULL;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = WND_CLASS_NAME;

	if( !RegisterClass( &wc ) )
	{
		Err_Msg( "윈도우 생성 실패입니다." );
		Assert( !"윈도우 생성 실패!" );
		return FALSE;
	} //if

	// Set Window Style & Rect

	DWORD dwStyle;

//#ifdef _CHINAVERSION
//	dwStyle = WS_POPUP | WS_VISIBLE;
//#else
	if( bFullscreen )
	{
		dwStyle = WS_POPUP | WS_VISIBLE;
	}
	else
	{
		dwStyle = 
			WS_OVERLAPPED	|
			WS_CAPTION		|
			WS_SYSMENU		|
			WS_MINIMIZEBOX;
	} //if..else..
//#endif

	RECT rc;
	rc.left   = 0;
	rc.top    = 0;
	rc.right  = nWidth;
	rc.bottom = nHeight;

	AdjustWindowRect( &rc, dwStyle, FALSE );

	int		x;
	int		y;
	int		width  = rc.right - rc.left;
	int		height = rc.bottom - rc.top;

	if( bFullscreen )
	{
		x = 0;
		y = 0;
	}
	else
	{
		x = ( GetSystemMetrics( SM_CXSCREEN ) - width  ) / 2;
		y = ( GetSystemMetrics( SM_CYSCREEN ) - height ) / 2;
	} //if..else..

	// Create Window

	HWND	hWnd;

	hWnd = CreateWindowEx(
		NULL,
		WND_CLASS_NAME,
		WND_TITLE,
		dwStyle,
		x,
		y,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL );

	if( NULL == hWnd )
	{
		Assert( !"윈도우 생성 실패!" );
		Err_Msg( "윈도우 생성 실패입니다." );
		return FALSE;
	} //if

#ifdef _DEBUG
	char		szTitleName[125];
	sprintf( szTitleName, "%s - %s", WND_TITLE, g_asGameServerHost[g_btServer].c_str() );
	SetWindowText( hWnd, szTitleName );
#endif
	// Show Window

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );
	SetFocus( hWnd );

	g_hWnd = hWnd;

	return TRUE;
} //CreateMainWindow

/**
 * 메인 윈도우 메세지 프로시져.
 */
LRESULT CALLBACK MainWndProc(
	HWND	hWnd,
	UINT	uMsg,
	WPARAM	wParam,
	LPARAM	lParam
	)
{
	if( g_pGUI &&
		g_pGUI->IMEProc( hWnd, uMsg, wParam, lParam ) )
	{	// 게임 엔진의 유저 인터페이스의 IME Proc로 메세지를 넘겨준다.
		return 0;
	} //if

	switch( uMsg )
	{
	case WM_ACTIVATEAPP:
		g_bActive = (BOOL)wParam;
		return 0;

	case WM_SYSCOMMAND:
		switch( wParam )
		{
		case SC_KEYMENU:
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		} //switch
		break;

	case 0x020A:	//WM_MOUSEWHEEL:
		g_pInput->SetMouseWheel( HIWORD(wParam) );
		break;

	case WM_KEYDOWN:
		break;

	case WM_CLOSE:				// Window Close
		PostQuitMessage( 0 );	// WM_QUIT
		return 0;

	} //switch

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
} //MainWndProc

/**
 *
 */
void	Err_Msg( char* szMsg )
{
	char szText[256];

	sprintf( szText, "%s\n프로그램을 강제 종료 합니다.\n자세한 사항은 라키아 홈페이지에 문의하여 주시길 바랍니다.", szMsg );
//	sprintf( szText, "%s\nYou can't run this program.", szMsg );
	ERR_MSG( szText );
} //Err_Msg

/**
 *
 */
void	Caution_Msg( char* szMsg )
{
	char szText[256];

	sprintf( szText, "%s\n프로그램이 실행 도중 비정상 종료 될수 있습니다.\n자세한 사항은 라키아 홈페이지에 문의하여 주시길 바랍니다.", szMsg );

	CAUTION_MSG( szText );
} //Caution_Msg

/**
 * 디스플레이 해상도 변경
 */ 
BOOL ChangeDisplay( BOOL bChange, int nWidth, int nHeight, int nDepth )
{
	DEVMODE dm;
	ZeroMemory(&dm, sizeof( DEVMODE ) );

	if( !bChange )
	{
		// 원래대로 복구
		ChangeDisplaySettings( NULL, 0 );
	}
	else
	{
		Log( "%d x %d x %d", nWidth, nHeight, nDepth );
		dm.dmSize		= sizeof( DEVMODE );
		nDepth  ? dm.dmFields |=DM_BITSPERPEL : dm.dmFields;
		nWidth  ? dm.dmFields |=DM_PELSWIDTH  : dm.dmFields;
		nHeight ? dm.dmFields |=DM_PELSHEIGHT : dm.dmFields;

		dm.dmBitsPerPel	= nDepth;
		dm.dmPelsWidth	= nWidth;
		dm.dmPelsHeight	= nHeight;

		// 바꿀 수 있는지 시험해보고 바꿈
		if( ChangeDisplaySettings( &dm,CDS_TEST ) == DISP_CHANGE_SUCCESSFUL )		
			ChangeDisplaySettings( &dm,0 );
		else
		{
			dm.dmSize		= sizeof( DEVMODE );
			dm.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
			dm.dmBitsPerPel	= nDepth;
			dm.dmPelsWidth	= 800;
			dm.dmPelsHeight	= 600;

			// 바꿀 수 있는지 시험해보고 바꿈
			if( ChangeDisplaySettings( &dm,CDS_TEST ) == DISP_CHANGE_SUCCESSFUL )		
				ChangeDisplaySettings( &dm,0 );
		}
	}

	return TRUE;
} // ChangeDisplay



BOOL CheckFolderAndFiles()
{
	//파일과 길이를 비교하여 다르면 종료 같으면 실행시킨다.
	int handle;
	long length;

	handle = open( "Skill/skill.dat", O_RDONLY | O_BINARY );
	if( handle == -1 )
	{
		return FALSE;
	}
	else
	{
		long SkillDatLength = 6301;
		length = filelength(handle);
		if( SkillDatLength != length )
		{
			return FALSE;
		}		
	}
	return TRUE;
}


/**
 * 몬스터 파일 없을시 프로그램 종료
 * [4/8/2009 18:22]
 */
BOOL CheckMonsterFiles()
{
#define MONSTER_FILE						"Monster/%s.mod"
	char	*aFile[32] = 
	{
		"ice_knight_bb",
		"ice_knight_red",
		"ice_knight_red_bb",
		"icknig-l2",
		"icknig-l2_bb",
		"settiros",
		"settiros_bb",
		"succubu2",
		"succubu2_bb"
//		"ice_knight"
	};//aFile

	char	szTemp[1024];

	long	handle = 0;;

	for( int i = 0; i < 9; i++ )
	{
		sprintf( szTemp, MONSTER_FILE, aFile[i] );
		handle = open( szTemp, O_RDONLY | O_BINARY );
		if( handle == -1 )
		{
			return FALSE;
		}
	}//for( int i=0; i < 9; i++ )

	return TRUE;
}