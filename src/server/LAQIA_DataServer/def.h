#ifndef __DEF_H__
#define __DEF_H__

#define dSERVER_PORT		7224 // Public PORT
#define dMAX_LISTEN			200 

#define dLOOP_DELAY_TIME	83		// ms 
#define dLOOP_ONE_SEC		12		// 1 √ 

#define FD_SETSIZE			6000

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

typedef int				SOCKET;
typedef	int				BOOL;
typedef unsigned char			BYTE;
typedef unsigned long			DWORD;
typedef unsigned short			WORD;

#endif // End of Win32

#include "clist.h"

#include <mysql.h>

#endif
