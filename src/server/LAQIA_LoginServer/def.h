/* def.h */

#ifndef __DEF_H__
#define __DEF_H__

//(TODO) 각 Server Ip , Port 정의 
#define dCONFIRM_SERVER_ADDRESS		0

//(TODO) 포트고치기
#define dCLIENT_ACCEPT_PORT		8030 // Public PORT

#define dONE_LISTEN			1
#define dMAX_LISTEN			1024

#define dMAX_USER			1000
#define dMAX_GAMESERVER		10

#define dLOOP_DELAY_TIME	83		// ms 
#define dLOOP_ONE_SEC		12		// 1 초

#define dMAX_SOCK_BUFF		20480

#define dKEEP_ALIVE_TIME	10000	// 연결상태 체크 패킷 10초당 한번씩

// DB
#define dDB_USER_TABLE	"UserTable"			// DB USER TABLE NAME 
#define dDB_LOG_TABLE	"LoginLog"

#if defined( WIN32 ) // for Windows

#include <windows.h>
#include <stdio.h>
#include <winsock.h>
#include <mmsystem.h>
#include <time.h>
#include "resource.h"

#define _CONSOLE_MODE_

#else // for *nix

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

#define INVALID_SOCKET			(-1)
#define closesocket(sock)		close(sock)
#define TRUE 1
#define FALSE 0
typedef int						SOCKET;
typedef	int						BOOL;
typedef unsigned char			BYTE;
typedef unsigned long			DWORD;
typedef unsigned short			WORD;

#endif 

#include <stdarg.h>
#include <mysql.h>

#endif //__DEF_H__
