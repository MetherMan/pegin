/* main.cpp */

#include "def.h"
#include "structs.h"
#include "extern.h"
#include "net_init.h"
#include "client.h"

#ifndef __CHINA_VER__
#include "DaumGameAuth.h"
#endif

#include "admin_server.h"

#ifdef __CHINA_VER__
MYSQL	*g_pMYSQL;
sDB_INFO g_IDPWDB;
#endif

BOOL		g_bProcessLife			=  1;	 
SOCKET		g_nClientListenSocket	= -1;	// Client 접속 받을 소켓  Socket to receive client connection

BOOL		g_TeammayTestMode = FALSE; // Teammay 내부 아이피만 접속 가능 (210.116.128.x)

//(TODO) 전역 변수 - 사용전에 반드시 초기화 해주자!  global variable-be sure to initialize it before use!

DWORD		g_CurrentTime;	// 현재시간 - ProcessLoop 에서 갱신된다. 
DWORD		g_CurrRealTime;
sClients	g_Clients;		// 전체 유저 리스트 

// Function
void	DestroySignal( int sigNum );
int		InitAllSocket();
void	InitServerData();
void	TimePulse( int pulse );
void	ProcessLoop();
void	DestroyServer();

// for WIN32
#if defined( WIN32 ) 

#define			dAPP_NAME		"Deicide Login Server"

HINSTANCE		hInst;			 // Current instance
HWND			g_hWnd;			 // Window Handle 
	

LRESULT	CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

//
// Function Name: WinMain
// Date: 2002-02-17
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

	g_hWnd = CreateWindow( dAPP_NAME,
		dAPP_NAME,
		WS_SYSMENU | WS_MINIMIZEBOX,
		300, 300,
		350, 44,//CW_USEDEFAULT, 0,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	
	if( !g_hWnd )
		return (FALSE);
	
	// 모든 소켓 초기화! 
	if( InitAllSocket() < 0 )
	{
		MessageBox( g_hWnd, "InitAllSocket() failed", "failed", MB_OK );
		return -1;
	}

	ShowWindow( g_hWnd, nCmdShow );
	UpdateWindow( g_hWnd );

// Console Mode 
#ifdef _CONSOLE_MODE_
	AllocConsole();
	SetConsoleTitle( "LAQIA LOGIN SERVER LOG" );
#endif

	if( __argv[1] )
	{
		g_TeammayTestMode = TRUE;
		Log( "\r\n=====================================================\r\n"
			   "TeamMay internal test: Only 118.35.89.85 can be accessed.\r\n"
			 "======================================================\r\n" );
	}

	// 실제 Main Loop
	ProcessLoop();

#ifdef _CONSOLE_MODE_
	FreeConsole();
#endif
	
	return(WM_DESTROY);
}

//
// Function Name: WndProc
// Date: 2002-02-13
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
			if( MessageBox( hWnd, "Would you like to take responsibility after closing LoginServer?", "Warning", MB_YESNO ) == IDNO )
				break;
			SendMessage( g_hWnd, WM_DESTROY, 0, 0 );
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		g_bProcessLife = 0;
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

#else


//
// Function Name: DestroySignal
// Date: 2003-03-10
// Description: ProcessLoop() 를 종료합니다. 
//
void DestroySignal( int sigNum )
{
	g_bProcessLife = FALSE;
}


//
// Function Name: main 
// Date: 2002-02-13
// Description: main function for Linux/FreeBSD
//
int main(  )
{
	// Signal
	struct sigaction act; 
	
	act.sa_handler = SIG_IGN; 
	act.sa_flags &= ~SA_RESETHAND; /* 어떤 데는 SA_ONESHOT */ 
	sigaction(SIGPIPE, &act, NULL); 

	signal( SIGINT, DestroySignal );	
	signal( SIGKILL, DestroySignal );
	signal( SIGQUIT, DestroySignal );
	signal( SIGTERM, DestroySignal );
	// Signal End

	// 모든 소켓 초기화!
	if( InitAllSocket() < 0 )
	{
		Log( "InitAllSocket() failed\n" );
		return -1;
	}

//#ifdef _TEST_SERVER_
	//g_TeammayTestMode = TRUE;
	//Log( "\r\n====================================================\r\n"
	//	"TeamMay .. ...: 118.35.89.85 . .. ......\r\n"
       //      "=====================================================\r\n" );
//#endif

	// 실제 Main Loop
	ProcessLoop();

	return 0;
}

#endif // End of WIN32, *nix 


//
// Function Name: InitAllSocket
// Date: 2002-02-14
// Description: 모든 Socket Init
//
int InitAllSocket()
{
#if defined (WIN32)
	WSADATA	WSAData;
	WSAStartup( 0x0101, &WSAData );
#endif
	g_CurrentTime = timeGetTime();
	g_CurrRealTime = time( NULL );
	
	if( ( g_AdminSock = InitUDPServerSock( dADMIN_PORT ) ) < 0 )
		return -1;

	// listen 되야할 소켓
	if( ( g_nClientListenSocket = InitListenServerSock( dCLIENT_ACCEPT_PORT, dMAX_LISTEN ) ) < 0 )
		return -1;
	
	Log( "InitAllSocket() OK....\n" );
	return 0;	
}

void InitServerData()
{
	g_Clients.list = NULL;
	g_Clients.memory = NULL;
	g_Clients.total = 0;
	g_Clients.total_memory = 0;
	
	for( int count = 0; count <= dHASHKEY_STR_ETC; count++ )
	{
		g_Clients.id_list[count] = NULL;
	}
}

//
// Function Name: TimePulse
// Date: 2003-03-03
// Description:
//
void TimePulse( int pulse )
{			
	return ;
}

//
// Function Name: ProcessLoop 
// Date: 2002-02-17
// Description: 실제적인 Login,Logout 처리 루프 g_ProcessLife = FALSE 가 되면 종료 
//Actual Login,Logout processing loop ends when g_ProcessLife = FALSE
void ProcessLoop()
{
#ifdef __CHINA_VER__
	Log( "LANGUAGE - English\r\n" );
#endif

	Log( "\n-------- Server Start -- %s ..\r\n", 
	GetLogTime() );

	Log( "Entering ProcessLoop! .... \r\n" );

#ifdef __CHINA_VER__
	if( !LoadDbInfo( "IDPWDB.INI", &g_IDPWDB ) )
	{
		Log( "!LoadDbInfo Failed\r\n" );
		return;
	}
	else
		Log( "LoadDbInfo OK!\r\n" );

	g_pMYSQL = DBConnect( g_IDPWDB.host, g_IDPWDB.port, g_IDPWDB.id, g_IDPWDB.pw, g_IDPWDB.dbName );

	if( !g_pMYSQL )
	{
		Log( "IDPWDB Connect Failed\r\n" );
		return;
	}
	else
		Log( "Connect IDPW DB OK!\r\n" );
#endif
	// 서버데이터 초기화 Server data initialization
	InitServerData();

	//(TODO) DB 커넥트 DB CONNECT
	
	
#if defined( WIN32 )
	MSG msg;
#endif

	DWORD	lastTime = timeGetTime();
	int		delayTime;
	DWORD	timePulse = 0;
	fd_set	read_set, write_set, exc_set;
	struct	timeval tv;
	SOCKET	nfds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	pClient_Data d, next_d;
//	GameServer_Data d2, next_d2;

	// MainProcessLoop 
	while( g_bProcessLife ) 
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
		// LoopDelay 
		g_CurrentTime = timeGetTime();
		g_CurrRealTime = time( NULL );

		delayTime = dLOOP_DELAY_TIME - ( g_CurrentTime - lastTime );

		if( delayTime > 0 )
			LoopDelay( delayTime );

                if( !g_pMYSQL )
                {
                   g_pMYSQL = DBConnect( g_IDPWDB.host, g_IDPWDB.port, g_IDPWDB.id, g_IDPWDB.pw, g_IDPWDB.dbName );

		  	if( !g_pMYSQL )
	                {
		           Log( "IDPWDB Connect Failed\r\n" );
		           return;
	                }
	                else
		           Log( "Connect IDPW DB OK!\r\n" );

                }


		// for select 
		FD_ZERO( &read_set );
		FD_ZERO( &write_set );
		FD_ZERO( &exc_set );

		// Client 접속 받을 소켓 
		FD_SET( g_nClientListenSocket, &read_set );
		nfds = g_nClientListenSocket;

		for( d = g_Clients.list; d; d = next_d )
		{
			next_d = d->next;

			if( d->sock > nfds )
				nfds = d->sock;

			FD_SET( d->sock, &read_set);
			FD_SET( d->sock, &write_set);
			FD_SET( d->sock, &exc_set );

		}
		/*LIST_WHILE( g_Clients.list, d, next_d, next ); 

		if( d->sock > nfds )
			nfds = d->sock;

		FD_SET( d->sock, &read_set);
		FD_SET( d->sock, &write_set);
		FD_SET( d->sock, &exc_set );

		LIST_WHILEEND();*/


		if( nfds < g_AdminSock )
			nfds = g_AdminSock;

		FD_SET( g_AdminSock, &read_set );
		
		if( select( nfds + 1, &read_set, &write_set, &exc_set, &tv ) < -1 )
		{
			Log( "select Error!\r\n" );
			continue;
		}

		if( FD_ISSET( g_AdminSock, &read_set ) )
			RecvAdminPacket();

		if( FD_ISSET( g_nClientListenSocket , &read_set) )
			AcceptNewClient( g_nClientListenSocket );

		/////////////////////////////////////////////
		// PLAYER RECV AND EXCEPTION
		/////////////////////////////////////////////
		for( d = g_Clients.list; d; d = next_d )
		{
			next_d = d->next;
			//
			// 30초 이상 데이터 전송이 없을 경우
			/*	if( d->lastRecvTime + 30000 < g_CurrentTime )
			{
			DisconnectClient( d );
			continue;
			}*/
			if( d->disconnectFlag )
			{
				DisconnectClient( d );
				continue;
			}

			if( FD_ISSET( d->sock , &exc_set ) )
			{
				DisconnectClient( d );
				continue;
			}

			if( FD_ISSET( d->sock , &read_set ) )
			{
				if( !RecvFromClient( d ) )
				{
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


	/*	for( d = g_Clients.list; d; d = next_d )
		{
			next_d = d->next;

			if( FD_ISSET( d->sock , &exc_set ) )
			{
				DisconnectClient( d );
				continue;
			}

			// 30초 이상 데이터 전송이 없을 경우
			if( d->lastRecvTime + 30000 < g_CurrentTime )
			{
				DisconnectClient( d );
				continue;
			}

			//(TODO) 테스트를 위한주석 
			
			// 연결상태 패킷 20초동안 없으면 
			if( d->lastAliveCheck + 20000 <= g_CurrentTime )
			{
				DisconnectClient( d );
				continue;
			}//
			
				
			if( FD_ISSET( d->sock , &read_set ) )
			{
				if( !RecvFromClient( d ) )
				{
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

		}*/

		/////////////////////////////////////////////
		// FLUSH SEND BUFF
		/////////////////////////////////////////////
		for( d = g_Clients.list; d; d = next_d )
		{
			next_d = d->next;

			if( d->sendSize && FD_ISSET( d->sock , &write_set ) )
			{
				if( FlushSendBuff( d ) < 0 )
				{
					DisconnectClient( d );
					continue;
				}
			}
		}

		/////////////////////////////////////////////
		// Pulse ( 시간 처리 )
		/////////////////////////////////////////////
		TimePulse( ++timePulse );

		if( timePulse == 48000 )
		{
			//g_LoginSql.ConnectionKeeper();
			timePulse = 0;
		}

		lastTime = timeGetTime();

		/*
		TimePulse( ++timePulse );

		if( timePulse >= 43200 )
			timePulse = 0;
		*/

		/*lastTime = timeGetTime();
		LoopDelay( 1 );*/
	}	
	// 서버 종료시 
	DestroyServer();
}


//
// Function Name: DestroyServer
// Date: 2003-05-12
// Description: 서버 종료 처리 
//
void DestroyServer()
{
	pClient_Data d, next_d;
	for( d = g_Clients.list; d; d = next_d )
	{
		next_d = d->next;
		DisconnectClient( d );		// 연결을 끊고
		free( d );					// 메모리를 해제 해준다. 
	}

	for( d = g_Clients.memory; d; d = next_d )
	{
		next_d = d->next;			
		free( d );					// 메모리를 해제 해준다. 
	}

	closesocket( g_nClientListenSocket );		// 소켓을 close 한다
	closesocket( g_AdminSock );	

	Log( "CloseAllSocket() OK....\n" );
#if defined( WIN32 )
	WSACleanup();
#endif
}
