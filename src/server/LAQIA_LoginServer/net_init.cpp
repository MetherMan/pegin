/* net_init.cpp 네트워크 초기화 */

#include "def.h"
#include "structs.h"
#include "extern.h"
#include "net_init.h"

//
// Function Name: nonbolck
// Date:
// Description: for set nonblocking mode socket
//
#if defined ( WIN32 )
void nonblock( SOCKET s )
{
	u_long            ulOn = 1L;
	
	ioctlsocket(s, FIONBIO, (unsigned long*)&ulOn);
}
#else
void nonblock( SOCKET s )
{
	int		flags;
	
	flags =	fcntl(s, F_GETFL, 0);
	flags |= O_NONBLOCK;
	
	if (fcntl(s, F_SETFL, flags) < 0) 
		Log( "* nonblock Error\r\n" );
}
#endif


//
// Function Name: InitListenServerSock
// Date: 2002-02-12
// Description: 서버 소켓 초기화 ( Listen 될 소켓 ) 
//
SOCKET InitListenServerSock( int port, int backLog )
{
	struct sockaddr_in sa;
	SOCKET sock;

	sock = socket( PF_INET, SOCK_STREAM, 0 );
	
	if( sock < 0 )
	{
		Log( "InitListenServerSock(), socket(..) failed.. \r\n" );
		return -1;
	}
	
#ifndef WIN32
	int opt = 1;

	if( setsockopt( sock , SOL_SOCKET , SO_REUSEADDR , (char *) &opt , sizeof(opt) ) < 0 )
	{
		Log( "* Error: setsockopt REUSEADDR\r\n" );
		exit(1);
	}	
#endif

	struct linger ld;
	
	ld.l_onoff = 0;
	ld.l_linger = 0;

	if ( setsockopt( sock, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof( ld ) ) < 0 )
		Log( "* Error: setsockopt SO_LINGER...\r\n" );
	
	/* Clear the structure */
	memset( (char *)&sa, 0, sizeof(sa) );
	
	sa.sin_family = PF_INET;
	sa.sin_port	= htons( port );
	sa.sin_addr.s_addr = INADDR_ANY;
	
	// 주소지 연결
	if( bind( sock, (struct sockaddr *)&sa, sizeof( sa ) ) < 0 )
	{
		Log( "InitListenServerSock(), bind(..) failed.. \r\n" );
		closesocket( sock );
		return -1;
	}
		
	// socket nonblocking
	nonblock( sock );
	listen( sock, backLog );

	return sock;
}

//
// Function Name: InitConnectServerSock
// Date: 2002-02-14
// Description: 서버 소켓 초기화 ( Connect 될 소켓 )
//

SOCKET InitConnectServerSock( char* TargetAddress, int port )
{	
	struct sockaddr_in sa;
	SOCKET sock;

	sock = socket( PF_INET, SOCK_STREAM, 0 );
	
	if( sock < 0 )
	{
		Log( "InitConnectServerSock(), socket(..) failed.. \r\n" );
		return -1;
	}
	
#ifndef WIN32
	int opt = 1;

	if( setsockopt( sock , SOL_SOCKET , SO_REUSEADDR , (char *) &opt , sizeof(opt) ) < 0 )
	{
		Log( "* Error: setsockopt REUSEADDR\r\n" );
		exit(1);
	}	
#endif

	struct linger ld;
	
	ld.l_onoff = 0;
	ld.l_linger = 0;

	if ( setsockopt( sock, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof( ld ) ) < 0 )
		Log( "* Error: setsockopt SO_LINGER...\r\n" );
	
	/* Clear the structure */
	memset( (char *)&sa, 0, sizeof(sa) );
	
	sa.sin_family = PF_INET;
	sa.sin_port	=  htons( port ); 
	sa.sin_addr.s_addr = inet_addr( TargetAddress );
		
	if( connect( sock, (struct sockaddr *)&sa, sizeof( sa ) ) < 0 )
	{
		Log( "InitConnectServerSock(), connect() failed\r\n" );
		return -1;
	}

	nonblock( sock );
	return sock;
}

//
// Function Name: InitUDPServerSock
// Date: 2003-02-12
// Description: 서버 소켓 초기화 
//
SOCKET InitUDPServerSock( int port )
{
	struct sockaddr_in sa;
	SOCKET sock;

	sock = socket( PF_INET, SOCK_DGRAM, 0 );
	
	if( sock < 0 )
	{
		Log( "InitUDPServerSock(), socket(..) failed [PORT:%d].. \r\n" , port );
		return -1;
	}
	
#ifndef WIN32
	int opt = 1;

	if( setsockopt( sock , SOL_SOCKET , SO_REUSEADDR , (char *) &opt , sizeof(opt) ) < 0 )
	{
		Log( "* Error: sersockopt REUSEADDR\r\n" );
		exit(1);
	}	
#endif

	struct linger ld;
	
	ld.l_onoff = 0;
	ld.l_linger = 0;

	if ( setsockopt( sock, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof( ld ) ) < 0 )
		Log( "* Error: sersockopt SO_LINGER...\r\n" );
	
	/* Clear the structure */
	memset( (char *)&sa, 0, sizeof(sa) );
	
	sa.sin_family = PF_INET;
	sa.sin_port	= htons( port );
	sa.sin_addr.s_addr = INADDR_ANY;
	
	// 주소지 연결
	if( bind( sock, (struct sockaddr *)&sa, sizeof( sa ) ) < 0 )
	{
		Log( "InitUDPServerSock(), bind(..) failed.. [PORT:%d]\r\n" , port );
		closesocket( sock );
		return -1;
	}
		
	// socket nonblocking
	nonblock( sock );

	return sock;
}

void SendUDPData( SOCKET sock, const char *data, int size, struct sockaddr_in *addr )
{
	sendto( sock, data, size, 0, (struct sockaddr*)addr, sizeof(struct sockaddr_in) ); 
}





