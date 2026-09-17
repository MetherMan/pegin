/**
 *	$Workfile: admin_server.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 5 $
 *	$Date: 04-03-31 8:22a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/admin_server.h $
 * 
 * 5     04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __ADMIN_SERVER__
#define __ADMIN_SERVER__

#define dADMIN_PORT						9999

#define dADMIN_PACKET_STATE				0		// 서버상태 처리 
#define dADMIN_PACKET_GETINFO			1		// 유져, 몬스터 수치 
#define dADMIN_PACKET_STATE_RES			2		// 0 번 패킷 return value
#define dADMIN_PACKET_SENDINFO			3		// 1 번 패킷 return value
#define dADMIN_PACKET_BADUSER			4		// 인증 실패 
#define dADMIN_PACKET_NOTICEMSG			5		// 공지
#define dADMIN_PACEKT_NOTICERET			6		// 공지 return value 
#define dADMIN_PACKET_DISCONUSER		7		// 접속 해제
#define dADMIN_PACKET_DISCONUSER_RES	8		// 접속 해제 return value
#define dADMIN_PACKET_GETSTATEINFO		9		// 접속 해제
#define dADMIN_PACKET_SENDSTATEINFO	10		// 접속 해제 return value



#define dMAX_ADMIN_PACKET				10

extern SOCKET g_AdminSock;

void RecvAdminPacket();

#endif

//

