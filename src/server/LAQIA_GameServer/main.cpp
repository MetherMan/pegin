/**
 *	$Workfile: main.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 59 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/main.cpp $
 * 
 * 59    04-12-15 10:40a Sk8snow
 * 
 * 58    04-10-19 11:14p Sk8snow
 * 
 * 57    04-09-20 10:53p Sk8snow
 * 
 * 56    04-09-11 11:20p Sk8snow
 * 
 * 55    04-09-08 3:54p Sk8snow
 * 
 * 54    04-08-04 9:27a Sk8snow
 * 
 * 53    04-07-13 7:26a Sk8snow
 * 
 * 52    04-05-29 10:12p Sk8snow
 * 
 * 51    04-05-21 11:37a Sk8snow
 * 
 * 50    04-05-18 1:17a Sk8snow
 * 
 * 49    04-05-14 10:38p Sk8snow
 * 
 * 48    04-05-11 1:45a Sk8snow
 * 
 * 47    04-04-30 9:46a Sk8snow
 * 
 * 46    04-04-01 5:33p Sk8snow
 * 
 * 45    04-03-31 8:22a Sk8snow
 *        
 */

// main 함수들을 위한 화일  
#include "def.h"

#include "structs.h"
#include "extern.h"
#include "net_init.h"
#include "message.h"
#include "player.h"
#include "monster.h"
#include "item.h"
#include "fight.h"
#include "world.h"
#include "house.h"
#include "shop.h"
#include "effect.h"
#include "guild.h"
#include "billing.h"
#include "fishing.h"

#include "dataserver.h"
#include "admin_server.h"

#include "skill.h"
#include "war.h"

#include "billing.h"

#include "quest.h"


BOOL	g_GameLife = 1;
SOCKET	g_MainSock = -1;
MYSQL	*g_MYSQL = NULL;
MYSQL	*g_LOGSQL = NULL;
MYSQL	*g_CASHMYSQL = NULL;
char	g_TMP_STR[1024];
int		g_ServerNumber;

sDB_INFO g_ITEMDBINFO;
sDB_INFO g_DATASERVERINFO;
sDB_INFO g_LOGDBINFO;
sDB_INFO g_CASHINFO;

sPLAYERS gPLAYERS;
sMOBS gMOBS;
sITEMS gITEMS;

#ifndef WIN32
DWORD g_ProcessStartTime;
#endif

DWORD g_CurrTime;
DWORD g_CurrRealTime;

// 실행인자 처리용 For execution factor processing
BYTE g_itemRemoverOn = 0;
WORD g_itemRemoverNum = 0;
BYTE g_ExpJump = 2;
BYTE g_EventExp = 1;

int	 g_nWeapon = 0;
int  g_nArmor = 0;
int  g_nAccessori = 0;

int g_nWeaponP[21];
int g_nArmorP[21];
int g_nAccessoriP[21];

int g_nEnchantTotal=0;

int nOver50=0;
int nTotal=0;

char g_LANG_STR[dMAX_LANGUAGE_STR][dMAX_LANGUAGE_STR_LEN];

// Function
#ifndef WIN32
void Sleep( DWORD time );
#endif
void GameLoop();

// Init World (MAP) Data
BOOL InitWorldData();
// Destroy Global Data
void DestroyServerData();
// Load Admin Permition
void LoadAdminIDLEVEL();
// Load Npc Information
BOOL LoadNpcInfo();
// Load Multi Language String Table
BYTE LoadLanguageTxt();
// Load Guild War Information
BYTE LoadWarInfo();
//
void DestroyFriendMemory();

void CheckBillItemTime( sPDESC_DATA pPlayer );

#ifndef dNOT_USE_PET
BOOL LoadPetInfo();
#endif

void e_srandom( unsigned long initial_seed );

// for WIN32
#if defined( WIN32 ) 
#define dAPP_NAME	"LAQIA_GameServer"

HINSTANCE		hInst;		// Current instance
HWND			g_hwnd;		// Window Handle

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

//
// Function Name: WinMain
// Date: 2003-02-12
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
	
	/*
	메뉴의 높이를 구함
	*/
	DWORD dwMenuHeight    = GetSystemMetrics( SM_CYMENU );
    DWORD dwCaptionHeight = GetSystemMetrics( SM_CYCAPTION );
	DWORD dwFrameHeight   = GetSystemMetrics( SM_CYSIZEFRAME );

	g_hwnd = CreateWindow(
		dAPP_NAME,
		dAPP_NAME,
		WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 
		0,
		350, 
		dwMenuHeight + dwCaptionHeight + dwFrameHeight,
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

#ifdef _CONSOLE_MODE_
	AllocConsole();
	SetConsoleTitle( "LAQIA SERVER LOG" );
#endif

	GameLoop();

#ifdef _CONSOLE_MODE_
	FreeConsole();
#endif

	return(WM_DESTROY);
}

//
// Function Name: WndProc
// Date: 2003-02-12
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
	if( g_sendSize > 0 )
	{
		log( "Flushing DataServer SendBuff [size : %d]\r\n", g_sendSize );
		FlushDataSendBuff();
	}

	g_GameLife = FALSE;
}

//
// Function Name: main
// Date: 2003-02-12
// Description: main function for Linux/FreeBSD
//
int main( int argc, char *argv[] )
{
	system( "rdate -s time.bora.net" );

	if( argc > 1 )
	{
		if( !strcmp( argv[1], "--remove_uniq" ) )
		{
			if( argc != 3 )
			{
				printf( "Invalid Argument! need item uniq number\r\n" );
				return 0;
			}
			else
			{
				g_itemRemoverOn = 1;
				g_itemRemoverNum = atoi( argv[2] );

				log( "ITEM REMOVER ON! uniq number [%d]\r\n", g_itemRemoverNum );
			}
		}
		else if( !strcmp( argv[1], "--exp_jump" ) )
		{
			if( argc != 3 )
			{
				printf( "Invalid Argument! need value\r\n" );
				return 0;
			}
			else
			{
				g_ExpJump = MAX( atoi( argv[2] ), 255 );
				log( "Exp Jump ON! [%d]\r\n", g_ExpJump );
			}
		}
	}

	// max open file check
	int maxOpenDesc = getdtablesize();

	if( maxOpenDesc < 10240 )
	{
		log( "########################################################################\r\n" );
		log( "# Open file limit allow : %d\r\n", maxOpenDesc );
		log( "# Please running on ROOT! * useing [ulimit -n 10240] command\r\n" );
		log( "########################################################################\r\n" );
		return 0;
	}

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

	g_MainSock = InitServerSock( dSERVER_PORT, dMAX_LISTEN );
	log( "SERVER PORT : %d \r\n" , dSERVER_PORT );
	log( "Server Version : %d \r\n" , dPATCHNVER );
	if( g_MainSock == -1 )
		return 0;
	
	GameLoop();

	return 1;
}

#endif // End of WIN32, *nix 

//
// Function Name: InitServerData
// Date: 2003-02-12
// Description: 서버의 기본 데이터 초기화
//
BOOL InitServerData()
{
#ifdef dEVENT_MOB_ON
	g_EVENT_MOB[0].isAlive = 0;
	g_EVENT_MOB[0].prevMapNum = number( 9, 11 );
	g_EVENT_MOB[0].updateTime = g_CurrRealTime + 600;

	g_EVENT_MOB[1].isAlive = 0;
	g_EVENT_MOB[1].prevMapNum = number( 9, 11 );
	g_EVENT_MOB[1].updateTime = g_CurrRealTime + 600;

	g_EVENT_MOB[2].isAlive = 0;
	g_EVENT_MOB[2].prevMapNum = number( 9, 11 );
	g_EVENT_MOB[2].updateTime = g_CurrRealTime + 600;
#endif

	g_EventHour[0] = 21;
	g_EventHour[1] = 23;

	////////////////////////////////////
	// INIT PLAYER GLOBAL DATA
	////////////////////////////////////
	int i;
	for( i = 0; i < dHASHKEY_STR_MAX; i++ )
	{
		gPLAYERS.name_list[i] = NULL;
		gPLAYERS.id_list[i] = NULL;
	}

	gPLAYERS.list = NULL;
	gPLAYERS.memory = NULL;
	gPLAYERS.total = 0;
	gPLAYERS.total_memory = 0;

	gPLAYERS.userSkill_memory = NULL;
	gPLAYERS.friend_memory = NULL;

	gPLAYERS.fishingList = NULL;

	gPLAYERS.WaitAllData.Init();
	gPLAYERS.WaitCharData.Init();
	gPLAYERS.WaitDelData.Init();

	////////////////////////////////////
	// INIT MOB GLOBAL DATA
	////////////////////////////////////	
	gMOBS.list = NULL;
	gMOBS.memory = NULL;
	gMOBS.total = 0;
	gMOBS.total_memory = 0;

	////////////////////////////////////
	// INIT ITEM GLOBAL DATA
	////////////////////////////////////
	gITEMS.list = NULL;
	gITEMS.memory = NULL;
	gITEMS.total = 0;
	gITEMS.total_memory = 0;
	gITEMS.myshopitem_memory = NULL;
	gITEMS.billItem_Memory = NULL;

	////////////////////////////////////
	// Init Guild Global
	//////////////////////////////////// 
	for( i = 0; i < dMAX_GUILD; i++ )
		gGUILDS.array[i] = NULL;

	gGUILDS.list = NULL;
	gGUILDS.memory = NULL;
	gGUILDS.member_memory = NULL;

	gGUILDS.war_list = NULL;
	gGUILDS.war_memory = NULL;

	////////////////////////////////////
	// LOAD ADMIN INFO
	////////////////////////////////////
	LoadAdminIDLEVEL();

	for( i = 0; i < dMAX_ITEM_EVENT; i++ )
		g_ItemEvent[i] = 0;

	return 1;
}

//
// Function Name: DestroyServerData
// Date: 2003-02-12
// Description: 서버의 종료시 사용중이던 메모리들을 해제
//
void DestroyServerData()
{
	sPDESC_DATA d, next_d;
	sPMOB_DATA m, next_m;
	sPITEM_DATA item, next_item;
	sPMYSHOP_ITEM myshop_item, next_myshop_item;

	////////////////////////////////////
	// DESTROY PLAYER DATA
	////////////////////////////////////
	LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr );
	log( "1\r\n");
	DisconnectClient( d );
	FlushDataSendBuff();
	LIST_WHILEEND( gPLAYERS.list, d, next_d );

	LIST_WHILE( gPLAYERS.memory, d, next_d , next, WMgr2 );
	SAFE_FREE( d );
	LIST_WHILEEND( gPLAYERS.memory, d, next_d );

	//
	DestroyFriendMemory();

	////////////////////////////////////
	// D E S T R O Y  M O B  D A T A
	////////////////////////////////////
	LIST_WHILE( gMOBS.list, m, next_m , next, WMgr3 );
	DelMobData( m , __FILE__ , __LINE__ );
	LIST_WHILEEND( gMOBS.list, m, next_m );

	LIST_WHILE( gMOBS.list, m, next_m , next, WMgr4 );
	SAFE_FREE( m );
	LIST_WHILEEND( gMOBS.list, m, next_m );

	////////////////////////////////////
	// D E S T R O Y  I T E M  D A T A
	////////////////////////////////////
	LIST_WHILE( gITEMS.list, item, next_item , next, WMgr5 );
	SAFE_FREE( item );
	LIST_WHILEEND( gITEMS.list, item, next_item );

	LIST_WHILE( gITEMS.memory, item, next_item , next, WMgr6 );
	SAFE_FREE( item );
	LIST_WHILEEND( gITEMS.memory, item, next_item );

	LIST_WHILE( gITEMS.myshopitem_memory, myshop_item, next_myshop_item, next, WMgr7 );
	SAFE_FREE( myshop_item );
	LIST_WHILEEND( gITEMS.myshopitem_memory, myshop_item, next_myshop_item );

	////////////////////////////////////
	// DESTROY PLAYER GLOBAL DATA
	////////////////////////////////////
	gPLAYERS.WaitAllData.Destroy();
	gPLAYERS.WaitCharData.Destroy();
	gPLAYERS.WaitDelData.Destroy();

	////////////////////////////////////
	// DESTROY MOB GLOBAL DATA
	////////////////////////////////////
	DeleteDefaultMobInfo();

	////////////////////////////////////
	// DESTROY ITEM GLOBAL DATA
	////////////////////////////////////
	DeleteItemInfo();

	////////////////////////////////////
	// DESTROY SHOP INFO
	////////////////////////////////////
	DestroyShopInfo();
	
	////////////////////////////////////
	// DESTROY GUILD DATA
	////////////////////////////////////
	WAR_DestroyWarData();
	DestroyGuildData();

	////////////////////////////////////
	// DESTROY GUILD DATA
	////////////////////////////////////
	DestroyUserSkillMemory();

	// Billing Item
	DestroyBillItemMemory();

	closesocket( g_AdminSock );
	closesocket( g_DataSock );
	CloseMotherSock( g_MainSock );

	if( g_MYSQL )
		mysql_close( g_MYSQL );
	if( g_LOGSQL )
		mysql_close( g_LOGSQL );

#ifdef dDAUM_BILLING
	if( g_BILL_SQL )
		mysql_close( g_BILL_SQL );
#endif
}

//
// Function Name: Pulse
// Date: 2003-02-12
// Description: 서버의 전체 시간처리 
//
void Pulse( int pulse )
{
	// 1/3 초
//	if( !( pulse % 4 ) )
	MonsterMoveProc();

#ifdef USE_GUILD_WAR
#ifndef NOCHECK_WARTIME
	if( !( pulse % ( 12 * 60 ) ) )
		WAR_CheckWarTime( 0 );	
#endif
#endif

	if( !( pulse % (12*g_MobResetTime) ) ) 
		MobResetPulse();

	if( !( pulse % 4 ) )
		MoveResetPoint();


	// ITEM 수명체크 ( 맵상의 )
	if( !( pulse % ( 12 * 60 ) ) )
		CheckItemLifeInMap();

	/////////////////////////////////////
	// CHANGE WORLD TIME
	/////////////////////////////////////	

	if( g_CurrTime >= gWorldTimeTick )
	{
		if( gWorldTime == 6 )
			gWorldTime = 0;
		else
			gWorldTime++;

		gWorldTimeTick = g_CurrTime + gWorldTimeTable[gWorldTime].delay;
		
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_CHANGE_TIME, g_nPos );
		// 현재 시간 
		PutByte( g_Packet, gWorldTimeTable[gWorldTime].time, g_nPos );		

		// 다음 시간 
		if( gWorldTime == 6 )
			PutByte( g_Packet, gWorldTimeTable[0].time, g_nPos );			
		else
			PutByte( g_Packet, gWorldTimeTable[gWorldTime+1].time, g_nPos );

		// 다음 시간까지의 딜레이		
		PutInteger( g_Packet, gWorldTimeTable[gWorldTime].delay, g_nPos );
		
		PutSize( g_Packet, g_nPos );
		
		//
		sPDESC_DATA d, next_d;
		
		LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr );
		if( ( GET_STATE( d ) == dDESC_STATE_PLAYING ||
			GET_STATE( d ) == dDESC_STATE_FIGHT ) && 
			GET_MAP_KIND( d ) != dMAP_KIND_INDOOR &&
			GET_MAP_NUM( d ) != 10 
#ifdef dDAUM_BILLING
			&& GET_MAP_NUM( d ) != dPRIMIUM_ZONE1
			&& GET_MAP_NUM( d ) != dPRIMIUM_ZONE2
			&& GET_MAP_NUM( d ) != dPRIMIUM_ZONE3
#endif
			)
			SendData( d, g_Packet, g_nPos );
		LIST_WHILEEND( gPLAYERS.list, d, next_d );
		//
	}

}

#ifdef dUSE_DBAGENT
BOOL DBCheckProc()
{
	int cnt = 0;

	while( 1 )
	{
		if( *g_GAMEDB_NAME[cnt] == '\n' )
			break;

		if( !DBTableManagement( g_GAMEDB_NAME[cnt], "CHECK", g_MYSQL ) )
		{
			log( "CHECK TABLE [%s] has problem\r\n", g_GAMEDB_NAME[cnt] );

			if( !DBTableManagement( g_GAMEDB_NAME[cnt], "REPAIR", g_MYSQL ) )
			{
				log( "REPAIR TABLE [%s] failed\r\n", g_GAMEDB_NAME[cnt] );
				return 0;
			}
		}
		
		cnt++;		
	}

#ifdef dUSE_ITEMLOG
	cnt = 0;
	
	while( 1 )
	{
		if( *g_LOGDB_NAME[cnt] == '\n' )
			break;
		
		if( !DBTableManagement( g_LOGDB_NAME[cnt], "CHECK", g_MYSQL ) )
		{
			log( "CHECK TABLE [%s] has problem\r\n", g_LOGDB_NAME[cnt] );
			
			if( !DBTableManagement( g_LOGDB_NAME[cnt], "REPAIR", g_MYSQL ) )
			{
				log( "REPAIR TABLE [%s] failed\r\n", g_LOGDB_NAME[cnt] );
				return 0;
			}
		}	
		cnt++;
	}
#endif

	return 1;
}
#endif


void PrintDefines()
{
	//check define
	log( "###############################################\r\n Use TEA Encrypt Packet\r\n###############################################\r\n" );

#ifdef USE_ONE_WAY_LIST
	log( "USE_ONE_WAY_LIST ON!\r\n" );
#else
	log( "USE_ONE_WAY_LIST OFF!! Use Two-Way Linked List!\r\n" );
#endif

#ifdef dUSE_ITEMLOG
	log( "Item Log ON!\r\n" );
#else
	log( "Item Log OFF!\r\n" );
#endif

#ifndef dNOT_USE_PET
	log( "Use User Pet!\r\n" );
#endif

#ifdef __USE_SELECT__
	log( "Using select() for client\r\n" );
#else
	log( "Not Using select() for client\r\n" );
#endif

#ifdef TEAMMAY_IS_ADMIN
	log( "Teammay is Super User\r\n" );
#endif

#ifdef USE_GUILD_WAR
	log( "USE GUILD WAR\r\n" );
#endif
	// check define end
}

//
// Function Name: GameLoop
// Date:
// Description: 전체 프로세스 루프
//
void GameLoop()
{
	e_srandom( time(0) );

#ifndef WIN32
	g_ProcessStartTime = time( NULL );
#endif

	PrintDefines();

#if defined (WIN32)
	WSADATA WSAData;
	WSAStartup( 0x0101, &WSAData );
#endif
	g_CurrTime = timeGetTime();

	log( "LAQIA_GameServer Start\r\n" );

	if( !LoadDbInfo( "DATASERVER.INI", &g_DATASERVERINFO ) ||
		!LoadDbInfo( "ITEMDB.INI", &g_ITEMDBINFO ) )
		return;

#ifdef dUSE_ITEMLOG
	if( !LoadDbInfo( "LOGDB.INI", &g_LOGDBINFO ) )
		return;
#endif

#ifndef __CHINA_VER__
	//빌링서버 정보를 읽어 들인다.    이세호
	if( !LoadBillingIP( "BILLSERVER.INI", &g_BILL_INFO ) )
		return ;
#endif

#ifdef dDAUM_BILLING//dDAUM_BILLING
	if( !LoadDbInfo( "BILLDB.INI", &g_BILL_DBINFO ) )
		return;
#endif

	if( !LoadDbInfo( "CASHDB.INI", &g_CASHINFO ) )	
		return;	

	//if( !LoadIPTable( ) )
	//	return;
//	void PrintUserInfo();
//	PrintUserInfo();

	///////////////////////////////////////////////
	// CONNECT TO DATABASE
	///////////////////////////////////////////////
	log( "Connect to Game Database Server\r\n" );

	g_MYSQL = DBConnect( g_ITEMDBINFO.host, 
						g_ITEMDBINFO.port, 
						g_ITEMDBINFO.id, 
						g_ITEMDBINFO.pw, 
		g_ITEMDBINFO.dbName );

	if( !g_MYSQL )
	{
		log( "DBConnect failed\r\n" );
		return;
	}

#ifdef dUSE_ITEMLOG
	log( "Connect to LOG Database Server\r\n" );

	g_LOGSQL = DBConnect( g_LOGDBINFO.host,
							g_LOGDBINFO.port,
							g_LOGDBINFO.id,
							g_LOGDBINFO.pw,
							g_LOGDBINFO.dbName );

	if( !g_LOGSQL )
	{
		log( "Log DB Connect failed\r\n" );
		return;
	}
#endif

//빌링서버 접속 결과 보여준다.
#ifndef __CHINA_VER__
	log( "Connect to Billing Databae Server (China)\r\n" );

	g_BillSoket = ConnectToServer( g_BILL_INFO.host, g_BILL_INFO.port );

	if( !g_BillSoket )
	{
		log( "Log Billing Server Connect Failed\r\n " );
		return;
	}
	else
	{
		g_ServerNumber = g_BILL_INFO.ServerNum;
		StartBilling( g_BILL_INFO.ServerNum );
		log( "Start Billing\r\n" );
	}	
#endif

#ifdef dDAUM_BILLING
	log( "Connect to BILLING Database Server (DAUM )\r\n" );

	g_BILL_SQL = DBConnect( g_BILL_DBINFO.host,
							g_BILL_DBINFO.port,
							g_BILL_DBINFO.id,
							g_BILL_DBINFO.pw,
							g_BILL_DBINFO.dbName );
	if( !g_BILL_SQL )
	{
		log( "Billing DB Connect failed\r\n" );
		return;
	}

#endif

	g_CASHMYSQL = DBConnect( g_CASHINFO.host,
							 g_CASHINFO.port,
							 g_CASHINFO.id,
							 g_CASHINFO.pw,
							 g_CASHINFO.dbName );
	
	if( !g_CASHMYSQL )
	{
		log( "Cash DB Connect failed\r\n" );
		return;
	}

#ifdef dUSE_DBAGENT
	DBCheckProc();
#endif

	///////////////////////////////////////////////
	// INIT ADMIN SOCKET
	///////////////////////////////////////////////
	g_AdminSock = InitUDPServerSock( dADMIN_PORT );

	if( g_AdminSock < 0 )
	{
		log( "Init Admin UDP Socket Failed\r\n" );
		return;
	}

	///////////////////////////////////////////////
	// CONNECT TO DATASERVER
	///////////////////////////////////////////////
	log( "Connect to DataServer\r\n" );
	InitDataServer();
	g_DataSock = ConnectToServer( g_DATASERVERINFO.host, g_DATASERVERINFO.port );

	if( g_DataSock == INVALID_SOCKET )
	{
		log( "ConnectDataServer Failed\r\n" );
		return;
	}

	///////////////////////////////////////////////
	// LOAD BALANCE TABLE
	///////////////////////////////////////////////
	if( !LoadLanguageTxt() )
		return;

	if( !LoadDamageTable() )
		return;

	if( !LoadPlayerAbilityTable() )
		return;
	
	if( !LoadPlayerHPMPTable() )
		return;

	if( !LoadSkillExpTable() || !LoadStateExpTable() )
		return;
	
	if( !LoadBaseHitTable() )
		return;

	if( !LoadExpProtectTable() )
		return;

#ifndef dNOT_USE_PET
	if( !LoadPetInfo() )
		return;
#endif

	///////////////////////////////////////////////
	// LOAD MOB INFO
	///////////////////////////////////////////////
	log( "Load Mob Info \r\n" );
	if( !LoadMobInfo() )
		return;

	///////////////////////////////////////////////
	// LOAD ITEM INFO
	///////////////////////////////////////////////
	if( !LoadSkillData() || !LoadSpeSkillData() )
		return;

	log( "Load Item Info \r\n" );
	if( !LoadItemInfo() )
		return;

	AsignSkillFunc();

#ifdef dDAUM_BILLING
	if( !LoadBillingTable() )
	{
		return;
	}
#endif


	if( !LoadNeedLogTable() )
		return;

	if( !LoadMobDropItemTable() )
		return;


	///////////////////////////////////////////////
	// INIT SERVER DATA
	///////////////////////////////////////////////
	log( "Init Server Data\r\n" );
	InitServerData();
	
	///////////////////////////////////////////////
	// INIT WORLD DATA
	///////////////////////////////////////////////
	log( "InitWorldData ...\r\n" );
	if( !InitWorldData() )
		return;

	LoadMapName();

	if( !LoadLoadingPointInfo() )
		return;

#ifdef _dUMENOKOJI_QUEST_
	log( "Load Quest Info\r\n" );
	if( !LoadQuestData() )
		return;
#endif

	log( "Load Npc Info... \r\n" );
	if( !LoadNpcInfo() )
		return;

	log( "Load Guild War Info ...\r\n" );
	if( !LoadWarInfo() )
		return;

	if( !GUILD_LoadGuildInfo() )
		return;

	GUILD_LoadMemberInfo();

#ifdef USE_GUILD_WAR
	LoadMapOwnerGuild();

	if( !LoadCastleDoorInfo() )
		return;
	if( !LoadWarCristalInfo() )
		return;
	if( !LoadWarData() )
		return;
#endif

	///////////////////////////////////////////////
	// SHOP INFO
	///////////////////////////////////////////////
	log( "Load Shop Info\r\n" );
	if( !LoadShopInfo() )
		return;

	MobResetPulse();

#ifdef _DUMMY_MODE_
	log( "Dummy Player mode ON!! map: [%d] max [%d]\r\n", DUMMY_MAP,dMAX_DUMMY );
	char dummyName[128];
	int dummyPosX, dummyPosY;

	for( int i = 0; i < dMAX_DUMMY; i++ )
	{
		sprintf( dummyName, "TEST_DUMMY%d", i );
		dummyPosX = number( 0, 450 );
		dummyPosY = number( 0, 450 );
		CreateDummy( DUMMY_MAP, dummyPosX, dummyPosY, dummyName, i+10000 );
	}
#endif

	//여기서 게임상 존재하는 모든 무기, 방어구, 악세사리의 갯수를 파악한다.
	LoadCountItem();

	////////////////////////////////////////////////
	// Entering Game Loop!!!!
	////////////////////////////////////////////////
	log( "Entering Game Loop! .... \r\n" );

#if defined( WIN32 )
	MSG msg;
#endif
	DWORD lastTime = timeGetTime();
	int	  delayTime;
	DWORD timePulse = 0;
	fd_set read_set, write_set, exc_set;
	struct timeval tv;
	SOCKET nfds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	sPDESC_DATA d, next_d;

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

		// 루프 제어 
		g_CurrTime = timeGetTime();
		g_CurrRealTime = time( NULL );

		delayTime = dLOOP_DELAY_TIME - ( g_CurrTime - lastTime );

		if( delayTime > 0 )
			Sleep( delayTime );

		lastTime = timeGetTime();

		// for select 
		FD_ZERO( &read_set );
		FD_ZERO( &write_set );
		FD_ZERO( &exc_set );

		FD_SET( g_MainSock, &read_set );
		nfds = g_MainSock;
		
		// for DataServer
		FD_SET( g_DataSock, &read_set );
		FD_SET( g_DataSock, &write_set );
		
		if( nfds < g_DataSock )
			nfds = g_DataSock;
		// end for DataServer

		// for ADMIN Server
		FD_SET( g_AdminSock, &read_set );

		if( nfds < g_AdminSock )
			nfds = g_AdminSock;
		// end for ADMIN Server

		LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr );
#ifdef _DUMMY_MODE_
		if( d->isDummy )
		{
/*			if( d->dummyMoveTime + 5000 < g_CurrTime )
			{
				if( GET_STATE( d ) == dDESC_STATE_PLAYING )
					DummyMove( d );
				d->dummyMoveTime = g_CurrTime;
			}*/
			LIST_SKIP( d, next_d );
		}
#endif
		//
		if( d->disconnectFlag )
		{
			DisconnectClient( d );
			LIST_SKIP( d, next_d );
		}
#ifdef __USE_SELECT__
		if( d->sock > nfds )
			nfds = d->sock;
		
		FD_SET( d->sock, &read_set);
		FD_SET( d->sock, &write_set);
		FD_SET( d->sock, &exc_set );
#endif
		LIST_WHILEEND( gPLAYERS.list, d, next_d );

		if( select( nfds + 1, &read_set, &write_set, &exc_set, &tv ) == -1 )
		{
			log( "select Error!\r\n" );
			continue;
		}

		if( FD_ISSET( g_MainSock , &read_set) )
			AcceptNewClient( g_MainSock );

		if( FD_ISSET( g_AdminSock, &read_set ) )
			RecvAdminPacket();

		/////////////////////////////////////////
		// for Data Server
		/////////////////////////////////////////
		if( FD_ISSET( g_DataSock , &read_set ) )
		{
			if( !RecvFromDataServer() )
			{
				log( "Lost Connection DataServer\r\n" );
				g_GameLife = 0;
				return;
			}
			if( g_recvSize )
			{
				if( !ReadDataRecvBuff() )
				{
					log( "ReadDataRecvBuff Error\r\n" );
					g_GameLife = 0;
					return;
				}
			}		
		}	

		if( FD_ISSET( g_DataSock, &write_set ) )
			FlushDataSendBuff();

		/////////////////////////////////////////////
		// PLAYER RECV AND EXCEPTION
		/////////////////////////////////////////////
		LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr1 );
#ifdef _DUMMY_MODE_
		if( d->isDummy )
			LIST_SKIP( d, next_d );
#endif

#ifdef __USE_SELECT__
		if( FD_ISSET( d->sock , &exc_set ) )
		{
			DisconnectClient( d );
			LIST_SKIP( d, next_d );
		}
#endif

		// 20초 이상 데이터 전송이 없을 경우
#ifndef NO_TIMELIMIT
		if( GET_STATE( d ) != dDESC_STATE_LOADING )
		{
			if( d->lastRecvTime + 60000 <= g_CurrTime )
			{
				DisconnectClient( d );
				LIST_SKIP( d, next_d );
			}
		}
		else
		{
			if( d->lastRecvTime + 300000 <= g_CurrTime )
			{
				DisconnectClient( d );
				LIST_SKIP( d, next_d );
			}
		}

/*		if( d->lastRecvTime + 300000 <= g_CurrTime )
		{
			DisconnectClient( d );
			LIST_SKIP( d, next_d );
		}*/

		// 스피드핵 체크 패킷이 정상적으로 들어오는지 체크
		if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
			GET_STATE( d ) == dDESC_STATE_FIGHT )
		{
			if( (long)( g_CurrRealTime - d->aliveCheckTick ) > 30 )
			{
				if( !d->aliveCnt )
				{
					DisconnectClient( d );
					LIST_SKIP( d, next_d );
				}
				else
				{
					d->aliveCheckTick = g_CurrRealTime;
					d->aliveCnt = 0;
				}
			}
		}
#endif

#ifdef __USE_SELECT__
		if( FD_ISSET( d->sock , &read_set ) )
		{
#endif
			if( !RecvFromClient( d ) )
			{
				DisconnectClient( d );
				LIST_SKIP( d, next_d );
			}
#ifdef __USE_SELECT__
		}
#endif
		
        // Drain every socket (including FIN/RST) before handling new logins.
        // A new descriptor is inserted before an older connection in this list.
        LIST_WHILEEND( gPLAYERS.list, d, next_d );

        LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgrPackets );
#ifdef _DUMMY_MODE_
        if( d->isDummy )
            LIST_SKIP( d, next_d );
#endif
		if( d->recvSize )
		{
			if( !ReadRecvBuff( d ) )
			{
				DisconnectClient( d );
				LIST_SKIP( d, next_d );
			}
		}

		if( d->chDataLoaded && d->lastDataUpdateTime + 180000 <= g_CurrTime )
		{
			d->lastDataUpdateTime = g_CurrTime;
		
			// 근거리 
			if( d->isUpdate[dDATA_UPDATE_SKILLEXP1] )
				DATASERV_SendUpdateSkillExp( d, 0 );
			
			// 원거리 
			if( d->isUpdate[dDATA_UPDATE_SKILLEXP2] )
				DATASERV_SendUpdateSkillExp( d, 1 );
			
			// 흑마법
			if( d->isUpdate[dDATA_UPDATE_SKILLEXP3] )
				DATASERV_SendUpdateSkillExp( d, 2 );
			
			// 백마법 
			if( d->isUpdate[dDATA_UPDATE_SKILLEXP4] )
				DATASERV_SendUpdateSkillExp( d, 3 );
			
			// 힘 (STR)
			if( d->isUpdate[dDATA_UPDATE_STREXP] )
				DATASERV_SendUpdateStrExp( d );
			
			// 민첩 (DEX)
			if( d->isUpdate[dDATA_UPDATE_DEXEXP] ) 
				DATASERV_SendUpdateDexExp( d );
			
			// 지력 (INT)
			if( d->isUpdate[dDATA_UPDATE_INTEXP] )
				DATASERV_SendUpdateIntExp( d );

			// 성향 (CHA)
			if( d->isUpdate[dDATA_UPDATE_CHA] )
				DATASERV_SendUpdateCha( d );
		}

		LIST_WHILEEND( gPLAYERS.list, d, next_d );


		LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr2 );
		if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
			GET_STATE( d ) == dDESC_STATE_FIGHT )
		{	
			if( d->chDataLoaded )
			{
				CheckBillItemTime( d );	

				if( d->ch2.lastSaveBillInfoTime + 180 <= g_CurrRealTime )
				{
					d->ch2.lastSaveBillInfoTime = g_CurrRealTime;
					SaveUserBillEffect( d );
				}
			}
			// Effect 시간 체크
			EFFECT_CheckTime( d );

			// 연속 공격 딜레이 처리
#ifdef AUTO_ATTACK
			if( ( d->victPlayer || d->enemyMob ) && 
				d->ch2.attackDelay <= g_CurrTime + 200 && 
				!GET_STATUS_DELAY( d, dSTATUS_DELAY_STUN ) &&
				( GET_USE_SKILL( d ) == dSATTACK_SKILL_ITEM || GET_USE_SKILL( d ) == dLATTACK_SKILL_ITEM ) )
			{
				if( CheckAllowAttack( d ) == 1 )
				{
					if( d->enemyMob )
						AttackToMob( d, d->enemyMob );
					else if( d->victPlayer )
						AttackPvP( d, d->victPlayer );
				}
			}
#endif
			
			if( !( timePulse % (12*3) ) )
				PlayerPointGain( d );

			// 유령 해제 
			if( !( timePulse % 12 ) && GET_DEAD_STATE( d ) == dDEAD_STATE_GHOST && 
				GET_GHOST_TIMER( d ) + dGHOST_TIMER_VALUE <= g_CurrTime )
			{
				GET_DEAD_STATE( d ) = dDEAD_STATE_NONE;
								
				SetVisible( d, 0 );
#ifdef dUSE_DEAD_GHOST
				SetMovable( d, 0 );
#endif
			}
		}
		LIST_WHILEEND( gPLAYERS.list, d, next_d );

		/////////////////////////////////////////////
		// FLUSH SEND BUFF
		/////////////////////////////////////////////
		LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr3 );
#ifdef _DUMMY_MODE_
		if( d->isDummy )
			LIST_SKIP( d, next_d );
#endif
#ifdef __USE_SELECT__
		if( d->sendSize && FD_ISSET( d->sock , &write_set ) )
#else
		if( d->sendSize )
#endif
		{
			if( FlushSendBuff( d ) < 0 )
			{
				DisconnectClient( d );
				LIST_SKIP( d, next_d );
			}
		}
		LIST_WHILEEND( gPLAYERS.list, d, next_d );

		/////////////////////////////////////////////
		// Pulse ( 시간 처리 )
		/////////////////////////////////////////////
		Pulse( ++timePulse );

		if( timePulse == 48000 )
		{
			timePulse = 0;
			DBConnectionKeeper( "InvenItems", "idx", g_MYSQL );
#ifdef dUSE_ITEMLOG
			DBConnectionKeeper( "ItemLog", "idx", g_LOGSQL );
#endif

#ifdef dDAUM_BILLING
			DBConnectionKeeper( "PurSell", "PurKey", g_BILL_SQL );
#endif
		}
//		lastTime = timeGetTime();

	}

	log( "Destroy Server\r\n" );
	DestroyServerData();
	DestroyWorldData();
	DestroyWorldHouse();
	
#if defined (WIN32)
	WSACleanup();
#endif
}

//
// Function Name: LoadLanguageTxt
// Date:
// Description: 
//
BYTE LoadLanguageTxt()
{
	for( int i = 0; i < dMAX_LANGUAGE_STR; i++ )
		sprintf( g_LANG_STR[i], "UNDEFINE STRING [%d]", i );

#ifdef __CHINA_VER__
	log( "* Load String Table - CHINA \r\n" );
#else
	log( "* Load String Table - KOREA \r\n" );
#endif

	FILE *fp;

	fp = fopen( dLANG_LIST_FILE, "rb" );

	if( !fp )
	{
		log( "LoadLanguageTxt : file not found\r\n" );
		return 0;
	}

	char readBuff[1024];

	int strNum = 0;
	char langStr[1024];

	while( !feof( fp ) )
	{
		int nPos = 0;

		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		GetTabString( readBuff, langStr, nPos );

		if( strlen( langStr ) >= dMAX_LANGUAGE_STR_LEN )  
		{
			log( "LoadLanguageTxt : length over\r\n" );
			fclose( fp );
			return 0;
		}

		if( strNum >= dMAX_LANGUAGE_STR )
		{
			log( "LoadLanguageTxt : strNum >= dMAX_LANGUAGE_STR\r\n" );
			fclose( fp );
			return 0;
		}

		strcpy( g_LANG_STR[strNum], langStr );
		strNum++;
	}

	fclose( fp );
	return 1;
}


//
// Function Name: LoadCountItem
// Date: 2012 / 2 / 15
// Description: 인첸트확률관련 아이템 갯수 
//
void LoadCountItem()
{
	//
	int itemNum=0;
	int nPlus = 0;

	g_nWeapon = 0;
	g_nArmor = 0;
	g_nAccessori = 0;

	nOver50=0;
	nTotal=0;

	char query[1024];

	int i = 0;

	//인첸된 갯수로 나눈다.
	for( i=0; i < 21; i++ )
	{
		g_nWeaponP[i] = 0;
		g_nArmorP[i] = 0;
		g_nAccessoriP[i] = 0;
	}
	
	sprintf( query, "select itemNum, count( itemNum ) from InvenItems group by itemNum" );
	if( !mysql_query( g_MYSQL, query ) )
	{
		MYSQL_RES *result;
		MYSQL_ROW row;

		result = mysql_store_result( g_MYSQL );

		if( result )
		{
			while( ( row = mysql_fetch_row( result) ) )
			{
				itemNum = atoi( row[0] );
				nPlus = atoi( row[1] );

				if( IS_WEAPON( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nWeapon += nPlus;
					g_nWeaponP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
				else if( IS_ARMOR( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nArmor += nPlus;
					g_nArmorP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
				else if( IS_ACCESSORI( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nAccessori += nPlus;
					g_nAccessoriP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
			}
			mysql_free_result( result );
		}
	}

	sprintf( query, "select itemNum, count( itemNum ) from EquipItems group by itemNum");
	if( !mysql_query( g_MYSQL, query ) )
	{
		MYSQL_RES *result;
		MYSQL_ROW row;

		result = mysql_store_result( g_MYSQL );

		if( result )
		{
			while( ( row = mysql_fetch_row( result) ) )
			{
				itemNum = atoi( row[0] );
				nPlus = atoi( row[1] );

				if( IS_WEAPON( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nWeapon += nPlus;
					g_nWeaponP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
				else if( IS_ARMOR( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nArmor += nPlus;
					g_nArmorP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
				else if( IS_ACCESSORI( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nAccessori += nPlus;
					g_nAccessoriP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
			}
			mysql_free_result( result );
		}
	}

	sprintf( query, "select itemNum, count( itemNum ) from UserGarbage group by itemNum");
	if( !mysql_query( g_MYSQL, query ) )
	{
		MYSQL_RES *result;
		MYSQL_ROW row;

		result = mysql_store_result( g_MYSQL );

		if( result )
		{
			while( ( row = mysql_fetch_row( result) ) )
			{
				itemNum = atoi( row[0] );
				nPlus = atoi( row[1] );

				if( IS_WEAPON( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nWeapon += nPlus;
					g_nWeaponP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
				else if( IS_ARMOR( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nArmor += nPlus;
					g_nArmorP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
				else if( IS_ACCESSORI( GET_ITEM_TYPE2( itemNum ) ) )
				{
					g_nAccessori += nPlus;
					g_nAccessoriP[GET_ITEM_UNIQ2(itemNum)] += nPlus;
				}
			}
			mysql_free_result( result );
		}
	}

	int skill[4];
	sprintf( query, "select skill_level0, skill_level1,skill_level2, skill_level3 from UserTable");
	if( !mysql_query( g_MYSQL, query ) )
	{
		MYSQL_RES *result;
		MYSQL_ROW row;

		result = mysql_store_result( g_MYSQL );

		if( result )
		{
			while( ( row = mysql_fetch_row( result) ) )
			{
				skill[0] = atoi( row[0] );
				skill[1] = atoi( row[1] );
				skill[2] = atoi( row[2] );
				skill[3] = atoi( row[3] );

				int n = skill[0] + skill[1] + skill[2] + skill[3];
				nTotal++;
				if( n >= 50 )
					nOver50++;
			}
			mysql_free_result( result );
		}
	}

	char szTemp[64];
	sprintf( szTemp, "무기갯수[%d] 방어구갯수[%d] 악세사리갯수[%d]\r\n", g_nWeapon, g_nArmor, g_nAccessori  );
	log( szTemp );

	for( i=0; i < 21; i++ )
	{
		sprintf( szTemp, "[%d+] 무기[%d] 방어구[%d] 악세사리[%d] \r\n", i, g_nWeaponP[i], g_nArmorP[i], g_nAccessoriP[i] );
		log( szTemp );
	}

	sprintf( szTemp, "총%d명 50레벨이상 유저 %d명입니다\r\n", nTotal, nOver50 );
	log( szTemp );
	
}

// EOF



