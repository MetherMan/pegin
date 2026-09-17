/*
	모니터링 툴 , 운영자툴 용 

	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	admin_server.cpp

	Build log...
*/

#include "def.h"

#include "structs.h"
#include "extern.h"
#include "net_init.h"

#include "admin_server.h"

SOCKET g_AdminSock = INVALID_SOCKET;

/*
#ifdef __CHINA_VER__	// 중국
#define dADMIN_ID	"deicide"
#define dADMIN_PW	"aucauddlfRk"
//#define dADMIN_ID	"laqiamonitor"
//#define dADMIN_PW	"laqiaadminmonitor"
#else					// 한국 
//#define dADMIN_ID	"laqiamonitor"
//#define dADMIN_PW	"fkzldkahslxjqlalfqjsgh" //라키아모니터비밀번호
#define dADMIN_ID	"deicide"
#define dADMIN_PW	"aucauddlfRk"
#endif*/	

#ifdef __CHINA_VER__	// 중국
//#define dADMIN_ID	"jamie175313"//해커
//#define dADMIN_PW	"jamier263"//이런씨부럴
#define dADMIN_ID	"deicide"
#define dADMIN_PW	"aucauddlfRk"
#endif

void SendUDPData( SOCKET sock, const char *data, int size, struct sockaddr_in *addr );

//
// Function Name: SendBadUser
// Date: 2003-06-12
// Description: 
//
void SendBadUser( struct sockaddr_in *fromAddr )
{
	g_nPos = 0;
	PutByte( g_Packet, dADMIN_PACKET_BADUSER, g_nPos );
	
	SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
}


//
// Function Name: CheckAdminServerAuth
// Date: 2003-06-12
// Description: 
//
BOOL CheckAdminServerAuth( char *id, char *pw, struct sockaddr_in *fromAddr )
{
	if( strcmp( id, dADMIN_ID ) )
		return 0;

	if( strcmp( pw, dADMIN_PW ) )
		return 0;

	return 1;
}

//
// Function Name: ADMINPACKET_Null
// Date: 2003-06-12
// Description: 
//
void ADMINPACKET_Null( char *packet, int nPos, struct sockaddr_in *fromAddr )
{
}

//
// Function Name: ADMINPACKET_State
// Date: 2003-06-12
// Description: 
//
void ADMINPACKET_State( char *packet, int nPos, struct sockaddr_in *fromAddr )
{
	char id[128];
	char pw[128];

	GetString( packet, id, nPos );
	GetString( packet, pw, nPos );

	if( !CheckAdminServerAuth( id, pw, fromAddr ) )
	{
		SendBadUser( fromAddr );
		return;
	}

	g_nPos = 0;
	PutByte( g_Packet, dADMIN_PACKET_STATE_RES, g_nPos );
	PutByte( g_Packet, 1, g_nPos );

	SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
}


//
// Function Name: ADMINPACKET_GetInfo
// Date: 2003-06-12
// Description: 
//
void ADMINPACKET_GetInfo( char *packet, int nPos, struct sockaddr_in *fromAddr )
{
	char id[128];
	char pw[128];

	GetString( packet, id, nPos );
	GetString( packet, pw, nPos );

	if( !CheckAdminServerAuth( id, pw, fromAddr ) )
	{
		SendBadUser( fromAddr );
		return;
	}

	/*
	 *		PutInteger( g_Packet, gPLAYERS.total, g_nPos );
	PutInteger( g_Packet, gPLAYERS.total+gPLAYERS.total_memory, g_nPos );
	PutInteger( g_Packet, gMOBS.total, g_nPos );
	 */
	g_nPos = 0;
	PutByte( g_Packet, dADMIN_PACKET_SENDINFO, g_nPos );
	PutInteger( g_Packet, g_Clients.total, g_nPos );
	PutInteger( g_Packet, g_Clients.total+g_Clients.total_memory, g_nPos );
	PutInteger( g_Packet, 0, g_nPos );

	SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
}

void (*AdminMessageFuncsList[])( char *packet, int nPos , struct sockaddr_in *fromAddr ) =
{
//	ADMINPACKET_State,				//#define dADMIN_PACKET_STATE				0
//	ADMINPACKET_GetInfo,			//#define dADMIN_PACKET_GETINFO				1		
//	ADMINPACKET_Null,				//#define dADMIN_PACKET_STATE_RES			2
//	ADMINPACKET_Null,				//#define dADMIN_PACKET_SENDINFO			3
//	ADMINPACKET_Null,				//#define dADMIN_PACKET_BADUSER				4
};


//
// Function Name: RecvAdminPacket
// Date: 2003-06-12
// Description: 
//
void RecvAdminPacket()
{
	int recvSize;
	char recvBuff[1024];
	struct sockaddr_in fromAddr;
#ifndef WIN32
	socklen_t fromLen;
#else
	int fromLen;
#endif
	BYTE tag;
	int nPos = 0;

	fromLen = sizeof( fromAddr );

	recvSize = recvfrom( g_AdminSock, recvBuff, 1024, 0, (struct sockaddr*)&fromAddr, &fromLen);

	if( recvSize <= 0 )
		return;
	
	tag = GetByte( recvBuff, nPos );
	
	if( tag > dMAX_ADMIN_PACKET )
		return;

	AdminMessageFuncsList[tag]( recvBuff, nPos , &fromAddr );	
}



//



