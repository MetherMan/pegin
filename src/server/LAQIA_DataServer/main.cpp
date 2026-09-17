/*
	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	main.cpp

	Build log...
*/

#include "def.h"

#include "structs.h"

#include "clist.h"
#include "net_init.h"
#include "extern.h"

#include "message.h"

#define dAPP_NAME	"Data Server"
BOOL	g_GameLife = 1;
SOCKET	g_MainSock = -1;

CList	g_ServerList;		// 연결된 서버 리스트

MYSQL *g_USER_DB = NULL;		// MYSQL 구조체
sDB_INFO g_DATA_DB;

// Function
void LoopDelay( DWORD delayTime );
void GameLoop();
void DestroyServer();

// for WIN32
#if defined( WIN32 ) 

HINSTANCE		hInst;		// Current instance
HWND			g_hwnd;		// Window Handle

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

//
// Function Name: WinMain
// Date: 2002-02-12
// Description: 윈도우 모드의 실행시 사용되는 API Main 
//
int APIENTRY WinMain( HINSTANCE hInstance, 
					 HINSTANCE	hPrevInstance,
					 LPSTR lpCmdLine, 
					 int nCmdShow )
{
	WNDCLASS	wc;
	
	if( !hPrevInstance )
	{
		wc.style				=		CS_HREDRAW;
		wc.lpfnWndProc			=		(WNDPROC)WndProc;
		wc.cbClsExtra			=		0;
		wc.cbWndExtra			=		0;
		wc.hInstance			=		hInstance;
		wc.hIcon				=		NULL;//LoadIcon( hInstance, (LPSTR)IDI_GFSERVER );
		wc.hCursor				=		LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground		=		(HBRUSH)(BLACK_BRUSH);
		wc.lpszMenuName			=		(LPSTR)IDR_MAIN_MENU;
		wc.lpszClassName		=		dAPP_NAME;
		
		if( RegisterClass( &wc ) == 0 )
			return(FALSE);
	}
	hInst = hInstance;

	g_hwnd = CreateWindow( dAPP_NAME,
		dAPP_NAME,
		WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0,
		350, 44,//CW_USEDEFAULT, 0,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	
	if( !g_hwnd )
		return (FALSE);

	ShowWindow( g_hwnd, nCmdShow );
	UpdateWindow( g_hwnd );

	// Server Socket Init 
	g_MainSock = InitServerSock( dSERVER_PORT, dMAX_LISTEN );

	if( g_MainSock == -1 )
	{
		MessageBox( g_hwnd, "InitServerSock Error", NULL, MB_OK );
		return 0;
	}

	GameLoop();

	WSACleanup();

	return(WM_DESTROY);
}

//
// Function Name: WndProc
// Date: 2002-02-12
// Description: 윈도우 메시지 헨들러
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDM_SHUTDOWN:
			if( MessageBox( g_hwnd, "정말로??", "진짜루??", MB_YESNO ) == IDNO )
				break;
			SendMessage( g_hwnd, WM_DESTROY, 0, 0 );
			g_GameLife = 0;
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam);
	}
	return 0;
}


#else  // for *nix 


void DestroySignal( int sigNum )
{
	g_GameLife = FALSE;
}


//
// Function Name: main
// Date: 2002-02-12
// Description: main function for Linux/FreeBSD
//
int main()
{
	//
	struct sigaction act; 
	
	act.sa_handler = SIG_IGN; 
	act.sa_flags &= ~SA_RESETHAND; /* 어떤 데는 SA_ONESHOT */ 
	sigaction(SIGPIPE, &act, NULL); 
	
	signal( SIGINT, DestroySignal );	
	signal( SIGTERM, DestroySignal );
	//
	
	log( "LAQUA DataServer Start\r\n" );
	g_MainSock = InitServerSock( dSERVER_PORT, dMAX_LISTEN );

	if( g_MainSock == -1 )
		return 0;
	
	GameLoop();

	return 1;
}

#endif // End of WIN32, *nix 



//
// Function Name: GameLoop
// Date:
// Description: 전체 프로세스 루프
//
void GameLoop()
{	
	if( !LoadAllowIp() )
	{
		log( "!LoadAllowIp Failed\r\n" );
		return;
	}

	g_ServerList.Init();

	if( !LoadDbInfo( "DATADB.INI", &g_DATA_DB ) )
		return;

	log( "Connect MySQL DB Server\r\n" );

	g_USER_DB = DBConnect( g_DATA_DB.host, g_DATA_DB.port, g_DATA_DB.id, g_DATA_DB.pw, g_DATA_DB.dbName );

	if( !g_USER_DB )
	{
		log( "LAQIA DB Connect failed\r\n" );
		return;
	}

#if defined( WIN32 )
	MSG msg;
#endif
	DWORD lastTime = timeGetTime();
	DWORD currTime;
	int	  delayTime;
	DWORD timePulse;
	timePulse = 0;
	fd_set read_set, write_set, exc_set;
	struct timeval tv;
	SOCKET nfds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	sPLIST_BLOCK b, next_b;
	sPDESC_DATA d;

	while( g_GameLife )
	{
#if defined( WIN32) // 윈도우 메시지 처리
		if( PeekMessage(&msg,NULL,0,0,PM_NOREMOVE) ) 
		{
			if( !GetMessage( &msg, NULL, 0, 0 ) )
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
#endif
               if( !g_USER_DB )
		{
		   g_USER_DB = DBConnect( g_DATA_DB.host, g_DATA_DB.port, g_DATA_DB.id, g_DATA_DB.pw, g_DATA_DB.dbName );

		   if( !g_USER_DB )
		   {
			log( "LAQIA DB Connect failed\r\n" );
			return;
		   }
		}


		// 루프 제어 
		currTime = timeGetTime();

		delayTime = dLOOP_DELAY_TIME - ( currTime - lastTime );

		if( delayTime > 0 )
			LoopDelay( delayTime );

		// for select 
		FD_ZERO( &read_set );
		FD_ZERO( &write_set );
		FD_ZERO( &exc_set );

		FD_SET( g_MainSock, &read_set );
		nfds = g_MainSock;

		for( b = g_ServerList.GetFirstBlock(); b; b = next_b )
		{
			next_b = b->next;

			d = (sPDESC_DATA)b->data;

			if( d->sock > nfds )
				nfds = d->sock;

			FD_SET( d->sock, &read_set);
			FD_SET( d->sock, &write_set);
			FD_SET( d->sock, &exc_set );
		}
		
		if( select( nfds + 1, &read_set, &write_set, &exc_set, &tv ) < -1 )
		{
			log( "select Error!\r\n" );
			continue;
		}

		if( FD_ISSET( g_MainSock , &read_set) )
			AcceptNewClient( g_MainSock );

	
		for( b = g_ServerList.GetFirstBlock(); b; b = next_b )
		{
			next_b = b->next;

			d = (sPDESC_DATA)b->data;


			if( FD_ISSET( d->sock , &exc_set ) )
			{
				DisconnectClient( d );
				continue;
			}
				
			if( FD_ISSET( d->sock , &read_set ) )
			{
				if( !RecvFromClient( d ) )
				{
					log( "Disconnect Game Server - %s\r\n", d->ip );
					DisconnectClient( d );
					continue;
				}
			}

			if( d->recvSize )
			{
				if( !ReadRecvBuff( d ) )
				{
					DisconnectClient( d );
					continue;
				}
			}

		}

#ifdef dUSE_SEND_BUFF
		for( b = g_ServerList.GetFirstBlock(); b; b = next_b )
		{
			next_b = b->next;

			d = (sPDESC_DATA)b->data;

			if( d->sendSize && FD_ISSET( d->sock , &write_set ) )
				FlushSendBuff( d );
		}
#endif

		lastTime = timeGetTime();
	}

	log( "Destroy Diecide Data Server\r\n" );
	DestroyServer();
}

void DestroyServer()
{
	CloseMotherSock( g_MainSock );

	sPLIST_BLOCK block, next_block;
	sPDESC_DATA d;

	for( block = g_ServerList.GetFirstBlock(); block; block = next_block )
	{
		next_block = block->next;

		d = (sPDESC_DATA)block->data;

		DisconnectClient( d );
	}
	
	g_ServerList.Destroy();

	// 데이터 저장 And 종료 처리
	if( g_USER_DB )
		mysql_close( g_USER_DB );
}
