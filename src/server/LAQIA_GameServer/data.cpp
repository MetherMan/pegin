#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "message.h"

#include "player.h"
#include "player_world.h"
#include "item.h"
#include "player.h"
#include "party.h"
#include "deal.h"
#include "vehicle.h"
#include "data.h"

SOCKET		g_DataSock = INVALID_SOCKET;

/*
 *	DATA_SendLogin
 */
void DATA_SendLogin( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_LOGIN_USER );

	//
	PutString( GET_ID( pPlayer ) );			// id
	PutByte( GET_CHARPOS( pPlayer ) );			// charPos 		
	PutInteger( GET_LEVEL( pPlayer ) );		// ·¹º§ 
	PutInteger( GET_EXP( pPlayer ) );			// °æÇèÄ¡
	PutInteger( GET_CLASS( pPlayer ) );		// Á÷¾÷ 
	PutInteger( GET_HP( pPlayer ) );			// HP
	PutInteger( GET_MAXHP( pPlayer ) );		// MAX HP
	PutInteger( GET_MP( pPlayer ) );			// MP
	PutInteger( GET_MAXMP( pPlayer ) );		// MAX MP
	PutInteger( GET_MONEY( pPlayer ) );		// µ·
	PutInteger( GET_MAP_NUM( pPlayer ) );		// ¸Ê¹øÈ£ 
	PutInteger( GET_POSX( pPlayer ) );			// ÁÂÇ¥ 
	PutInteger( GET_POSY( pPlayer ) );
	PutInteger( GET_POSZ( pPlayer ) );

	PutInteger( GET_PRE_MAPNUM( pPlayer ) );		// ÀÌÀü¸Ê¹øÈ£
	PutInteger( GET_PRE_POSX( pPlayer ) );		// ÁÂÇ¥
	PutInteger( GET_PRE_POSY( pPlayer ) );
	PutInteger( GET_PRE_POSZ( pPlayer ) );
	//
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendLogout
 */
void DATA_SendLogout( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_LOGOUT_USER );
	PutString( GET_ID( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdateLevel
 */
void DATA_SendUpdateLevel( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_LEVEL );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_LEVEL( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdateExp
 */
void DATA_SendUpdateExp( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_EXP );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_EXP( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdateClass
 */
void DATA_SendUpdateClass( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_CLASS );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_CLASS( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdateHP
 */
void DATA_SendUpdateHP( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_HP );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_HP( pPlayer ) );
	PutInteger( GET_MAXHP( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdateMP
 */
void DATA_SendUpdateMP( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_MP );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_MP( pPlayer ) );
	PutInteger( GET_MAXMP( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdateMoney
 */
void DATA_SendUpdateMoney( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_MONEY );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_MONEY( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdateMapInfo
 */
void DATA_SendUpdateMapInfo( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_MAPINFO );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_MAP_NUM( pPlayer ) );
	PutInteger( GET_POSX( pPlayer ) );
	PutInteger( GET_POSY( pPlayer ) );
	PutInteger( GET_POSZ( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	DATA_SendUpdatePreMapInfo
 */
void DATA_SendUpdatePreMapInfo( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( dDATA_UPDATE_PREMAPINFO );
	PutString( GET_ID( pPlayer ) );
	PutInteger( GET_PRE_MAPNUM( pPlayer ) );
	PutInteger( GET_PRE_POSX( pPlayer ) );
	PutInteger( GET_PRE_POSY( pPlayer ) );
	PutInteger( GET_PRE_POSZ( pPlayer ) );
	PutSize( g_Packet, g_nPos );

	SEND_TO_DATA( g_Packet, g_nPos );
}

/*
 *	InitDataSock
 */
BOOL InitDataSock()
{
	sDB_INFO dbInfo;

	if( !LoadDbInfo( "DATASERVER.INI", &dbInfo ) )
	{
		log( "InitDataSock : !DATASERVER.INI\r\n" );
		return 0;
	}

	g_DataSock = ConnectToServer( dbInfo.host, dbInfo.port, 0 );

	if( g_DataSock == INVALID_SOCKET )
		return 0;

	return 1;
}

/*
 *	DestroyDataSock
 */
void DestroyDataSock()
{
	closesocket( g_DataSock );
}

/*
 *	RecvFromDataServer
 */
BOOL RecvFromDataServer()
{
	char recvBuff[1024];

	int recvSize = recv( g_DataSock, recvBuff, 1024, 0 );

	if( recvSize == 0 )
		return 0;

	if( recvSize < 0 )
	{
#if defined(WIN32)
		if( WSAGetLastError() != WSAEWOULDBLOCK )
			return 0;
#else
		if( errno != EWOULDBLOCK )
			return 0;
#endif
		else
			return 1;
	}
	return 1;
}



// EOF



