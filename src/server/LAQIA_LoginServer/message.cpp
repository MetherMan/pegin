/* message.cpp client 와의 패킷 처리 */

#include "def.h"

#include "clist.h"
#include "structs.h"
#include "client.h"
#include "extern.h"
#include "protocol.h"
#include "message.h"

#ifndef __CHINA_VER__
#include "DaumGameAuth.h"
#endif

char	g_Packet[dMAX_SOCK_BUFF];
int		g_nPos;

//
// Function Name: NullFunc
// Date: 2002-02-12
// Description: 
//
BOOL NullFunc( pClient_Data pClient, char *readBuff, int readPos )
{
	return 1;
}

//
// Function Name: PacketAliveCheck
// Date: 2003-06-02
// Description: 연결상태 체크패킷 처리 
//
BOOL PacketAliveCheck( pClient_Data pClient )
{
	if( g_CurrentTime < pClient->lastAliveCheck + dKEEP_ALIVE_TIME )
	{
		// (TODO)스피드핵 .. 
		// 정해진 시간보다 패킷이 더 빨리 올때는 client 를 잘라버리자. 
	}
	
	pClient->lastAliveCheck = g_CurrentTime;

	return 1;
}

BOOL CheckAllowId( char *id )
{
	char filename[128];

	sprintf( filename, "ALLOW_ID/%s", id );

	FILE *fp = fopen( filename, "r" );

	if( !fp )
		return 0;

	fclose(fp);
	return 1;
}

BOOL CheckDenyId( char *id )
{
	char filename[128];
	
	sprintf( filename, "DENY_ID/%s", id );
	
	FILE *fp = fopen( filename, "r" );
	
	if( !fp )
		return 0;
	
	fclose(fp);
	return 1;
}


//
// Function Name: PacketLoginRequest
// Date: 2024-2-29
// Description: Login Server Packet requests
//

BOOL LoginPacketLoginInfo( pClient_Data pClient, char *readBuff, int readPos )
{
	char id[128];
	char pw[128];

	GetString( readBuff, id, readPos );
	GetString( readBuff, pw, readPos );

	char realID[128];
	char realPW[128];

	mysql_escape_string( realID, id, strlen( id ) );
	mysql_escape_string( realPW, pw, strlen( pw ) );
	
/* Function code for a certain IP
	if( g_TeammayTestMode )
	{
		if( strncmp( pClient->ip, "0.0.0.0", strlen( "0.0.0.0" ) ) &&
			!CheckAllowId( id ) ) 
		{
			g_nPos = 2;
			PutWord( g_Packet, dLOGIN_PACKET_LOGINRESULT, g_nPos );
			PutByte( g_Packet, 0, g_nPos );		
			PutSize( g_Packet, g_nPos );
			
			SendData( pClient, g_Packet	, g_nPos );
			return 1;
		}
	}
	*/
	char query[1024];
// Allows you to run a query in the database for USER ID And Password that aren't hashed
	sprintf( query, "SELECT mb_id, mb_password FROM g4_member where mb_id = '%s' and mb_password = PASSWORD('%s') limit 1", realID, realPW); 


	if( mysql_query( g_pMYSQL, query ) )
	{
		Log( (char*)mysql_error( g_pMYSQL ) );
		
		g_nPos = 2;
		//아이디 없음 NO ID
		PutWord( g_Packet, dLOGIN_PACKET_LOGINRESULT, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pClient, g_Packet, g_nPos );
	}
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
		
		result = mysql_store_result( g_pMYSQL );
		
		if( result )
		{	
			if( ( row =  mysql_fetch_row( result ) ) )
			{
				g_nPos = 2;
				PutWord( g_Packet, dLOGIN_PACKET_LOGINRESULT, g_nPos );
				PutByte( g_Packet, 2, g_nPos ); // Local GameClient success code
				PutString( g_Packet, id, g_nPos );
				PutString( g_Packet, pw, g_nPos );
				PutSize( g_Packet, g_nPos );
				SendData( pClient, g_Packet, g_nPos );
			}
			else
			{
				g_nPos = 2;
				PutWord( g_Packet, dLOGIN_PACKET_LOGINRESULT, g_nPos );
				PutByte( g_Packet, 4, g_nPos );
				PutSize( g_Packet, g_nPos );
				SendData( pClient, g_Packet, g_nPos );
			}
			mysql_free_result( result );
		}
		else
		{
			g_nPos = 2;
			PutWord( g_Packet, dLOGIN_PACKET_LOGINRESULT, g_nPos );
			PutByte( g_Packet, 4, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendData( pClient, g_Packet, g_nPos );
		}
	}
	return 1;	
}



//
// Function Name: MessageFuncsList
// Date: 2003-06-2
// Description: 패킷 처리 함수들의 포인터 배열
//
BOOL (*MessageFuncsList[])( pClient_Data pClient, char *readBuff, int readPos ) =
{
	NullFunc,				//#define dPACKET_RESERVED					0	// 임시패킷 
	LoginPacketLoginInfo,	//#define dLOGIN_PACKET_LOGININFO			1	// client -> server
	NullFunc,				//#define dLOGIN_PACKET_LOGINRESULT			2	// server -> client 
};
