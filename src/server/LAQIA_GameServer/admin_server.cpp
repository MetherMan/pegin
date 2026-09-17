/**
 *	$Workfile: admin_server.cpp $
 *
 *	Copyright (c) 2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 10 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/admin_server.cpp $
 * 
 * 10    04-12-15 10:40a Sk8snow
 * 
 * 9     04-03-31 8:22a Sk8snow
 *        
 */
#include "def.h"

#include "structs.h"
#include "extern.h"
#include "net_init.h"
#include "message.h"
#include "player.h"
#include "monster.h"
#include "skill.h"

#include "dataserver.h"

#include "item.h"
#include "itemdb.h"

#include "mercenary.h"

#include "admin_server.h"

SOCKET g_AdminSock = INVALID_SOCKET;

#ifdef __CHINA_VER__	// Áß±¹
#define dADMIN_ID	"cignaretest"//ÇØÄ¿
#define dADMIN_PW	"kis123"//ÀÌ·±¾¾ºÎ·²
#else					// ÇÑ±¹ 
#define dADMIN_ID	"jamie175313"//
#define dADMIN_PW	"jamier263"//
#endif

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

	g_nPos = 0;
	PutByte( g_Packet, dADMIN_PACKET_SENDINFO, g_nPos );
	PutInteger( g_Packet, gPLAYERS.total, g_nPos );
	PutInteger( g_Packet, gPLAYERS.total+gPLAYERS.total_memory, g_nPos );
	PutInteger( g_Packet, gMOBS.total, g_nPos );

	SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
}


//
// Function Name: ADMINPACKET_GetBOSSInfo
// Date: 2003-06-12
// Description: 
//
void ADMINPACKET_GetStateInfo( char *packet, int nPos, struct sockaddr_in *fromAddr )
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
	PutByte( g_Packet, dADMIN_PACKET_SENDSTATEINFO, g_nPos );
	PutDword( g_Packet, g_EVENT_MOB[0].updateTime, g_nPos );
	PutDword( g_Packet,  g_EVENT_MOB[1].updateTime, g_nPos );
	PutDword( g_Packet,  g_EVENT_MOB[2].updateTime, g_nPos );

	SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
}


//
// Function Name: ADMINPACKET_NoticeMsg
// Date: 
// Description: 
//
void ADMINPACKET_NoticeMsg( char *packet, int nPos, struct sockaddr_in *fromAddr )
{
	char id[128];
	char pw[128];
	char msg[1024];

	GetString( packet, id, nPos );
	GetString( packet, pw, nPos );
	GetString( packet, msg, nPos );
	
	if( !CheckAdminServerAuth( id, pw, fromAddr ) )
	{
		SendBadUser( fromAddr );
		return;
	}
	BroadCastingMsg( msg );

	g_nPos = 0;
	PutByte( g_Packet, dADMIN_PACEKT_NOTICERET, g_nPos );
	
	SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
}

//
// Function Name: ADMINPACKET_DisconUser
// Date: 
// Description: 
//
void ADMINPACKET_DisconUser( char *packet, int nPos, struct sockaddr_in *fromAddr )
{
	char id[128];
	char pw[128];
	char targetID[128];
	BYTE targetType;

	GetString( packet, id, nPos );
	GetString( packet, pw, nPos );
	GetString( packet, targetID, nPos );
	targetType = GetByte( packet, nPos );	

	if( !CheckAdminServerAuth( id, pw, fromAddr ) )
	{
		SendBadUser( fromAddr );
		return;
	}	

	sPDESC_DATA pTarget = NULL;
	
	if( !targetType )
		pTarget = FindPlayerNameList( targetID );
	else
		pTarget = FindPlayerIdList( targetID );

	if( !pTarget )
	{
		g_nPos = 0;
		PutByte( g_Packet, dADMIN_PACKET_DISCONUSER_RES, g_nPos );
		PutString( g_Packet, targetID, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		
		SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
	}
	else
	{
		pTarget->disconnectFlag = 1;

		g_nPos = 0;
		PutByte( g_Packet, dADMIN_PACKET_DISCONUSER_RES, g_nPos );
		PutString( g_Packet, targetID, g_nPos );
		PutByte( g_Packet, 1, g_nPos );
		
		SendUDPData( g_AdminSock, g_Packet, g_nPos, fromAddr );
	}
}

void (*AdminMessageFuncsList[])( char *packet, int nPos , struct sockaddr_in *fromAddr ) =
{
//	ADMINPACKET_State,				//#define dADMIN_PACKET_STATE				0
//	ADMINPACKET_GetInfo,			//#define dADMIN_PACKET_GETINFO				1		
//	ADMINPACKET_Null,				//#define dADMIN_PACKET_STATE_RES			2
//	ADMINPACKET_Null,				//#define dADMIN_PACKET_SENDINFO			3
//	ADMINPACKET_Null,				//#define dADMIN_PACKET_BADUSER				4
//	ADMINPACKET_NoticeMsg,			//#define dADMIN_PACKET_NOTICEMSG			5
//	ADMINPACKET_Null,				//#define dADMIN_PACEKT_NOTICERET			6
//	ADMINPACKET_DisconUser,			//#define dADMIN_PACKET_DISCONUSER			7
//	ADMINPACKET_Null,				//#define dADMIN_PACKET_DISCONUSER_RES		8
//	ADMINPACKET_GetStateInfo,				//#define dADMIN_PACKET_GETSTATEINFO		9
//	ADMINPACKET_Null,      // #define dADMIN_PACKET_SENDSTATEINFO	  10
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



