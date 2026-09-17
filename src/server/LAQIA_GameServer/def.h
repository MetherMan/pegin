/**
 *	$Workfile: def.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 29 $
 *	$Date: 04-09-08 3:54p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/def.h $
 * 
 * 29    04-09-08 3:54p Sk8snow
 * 
 * 28    04-08-04 9:27a Sk8snow
 * 
 * 27    04-07-13 7:26a Sk8snow
 * 
 * 26    04-05-29 10:12p Sk8snow
 * 
 * 25    04-04-30 9:46a Sk8snow
 * 
 * 24    04-04-01 5:33p Sk8snow
 * 
 * 23    04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __DEF_H__
#define __DEF_H__

//#define __CHINA_VER__	
//#define USE_ITEM_WEIGHT

#ifdef __CHINA_VER__
#define dSERVER_PORT		2560//8999
#else
#define dSERVER_PORT		2560
#endif

#define dMAX_LISTEN		200

#define dLOOP_DELAY_TIME	83		// ms 
#define dLOOP_ONE_SEC	12		// 1 초

#define FD_SETSIZE		6000

//#ifdef __CHINA_VER__
//#define dPATCHNVER	2019122701
//#else
#define dPATCHNVER	2020092801
//#endif

// 1024 제한을 피하기 위한;; 편법 
// select 함수를 사용하지 않는다
// 그리 좋은 방법은 -_-; 아님 
#ifndef __DONT_SELECT__
#define __USE_SELECT__
#endif

#define NULL_STR			'\0'

#if defined( WIN32 ) // for Windows

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "wsock32.lib" )
#pragma comment( lib, "libmysql.lib" )

#include <windows.h>
#include <winsock.h>
#include <mmsystem.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "resource.h"

#define _CONSOLE_MODE_
 
#else

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h> 
#include <signal.h>
#include <unistd.h> 
#include <time.h>
#include <math.h>

#define closesocket(sock) close(sock)

#define INVALID_SOCKET (-1)
#define TRUE 1
#define FALSE 0

typedef int						SOCKET;
typedef	int						BOOL;
typedef unsigned char			BYTE;
typedef unsigned long			DWORD;
typedef unsigned short			WORD;

#endif // End of Win32

#include "clist.h"

#include <mysql.h>

#endif

