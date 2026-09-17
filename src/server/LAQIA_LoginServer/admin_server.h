/*
	모니터링 툴 , 운영자툴 용 

	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	admin_server.h

	Build log...
*/

#ifndef __ADMIN_SERVER__
#define __ADMIN_SERVER__

#define dADMIN_PORT			9998

#define dADMIN_PACKET_STATE				0
#define dADMIN_PACKET_GETINFO			1
#define dADMIN_PACKET_STATE_RES			2
#define dADMIN_PACKET_SENDINFO			3
#define dADMIN_PACKET_BADUSER			4

#define dMAX_ADMIN_PACKET				4

extern SOCKET g_AdminSock;

void RecvAdminPacket();

#endif

//

