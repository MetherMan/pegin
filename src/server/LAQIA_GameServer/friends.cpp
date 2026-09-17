/**
 *	$Workfile: friends.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 8 $
 *	$Date: 04-11-18 1:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/friends.cpp $
 * 
 * 8     04-11-18 1:40a Sk8snow
 * 
 * 7     04-07-13 7:26a Sk8snow
 * 
 * 6     04-04-30 9:46a Sk8snow
 * 
 * 5     04-03-31 8:22a Sk8snow
 *        
 */

#include "def.h"
#include "structs.h"
#include "extern.h"
#include "message.h"

#include "world.h"
#include "player.h"
#include "monster.h"
#include "item.h"

#include "fight.h"
#include "party.h"

#include "skill.h"
#include "mercenary.h"
#include "quest.h"

#include "house.h"

#include "itemdb.h"
#include "questdb.h"
#include "guild.h"

#include "effect.h"
#include "friends.h"

//
// Function Name: FRIEND_AddFriend
// Date: 
// Description: 
//
void FRIEND_AddFriend( sPDESC_DATA pPlayer , char *friendName )
{
	if( FRIEND_FindData( pPlayer, friendName ) )
		return;
	
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	char escFriendName[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escFriendName, friendName, strlen( friendName ) );

	sprintf( query, "insert into UserFriend values( '', '%s', %d, '%s' )",
		escID, GET_CHAR_POS( pPlayer ), escFriendName );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_ADDED, g_nPos );
		PutString( g_Packet, friendName, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		GET_FRIEND_CNT( pPlayer )++;
		
		// 리스트에 추가
		FRIEND_AddData( pPlayer, friendName );
		FRIEND_GetFriendList( pPlayer );
	}
	SendSystemMsg( pPlayer, "Added to your friend list." );//	SendSystemMsg( pPlayer, g_LANG_STR[286], friendName );

	
	sPDESC_DATA pFriend = FindPlayerNameList( escFriendName );
	
	mysql_escape_string( escID, GET_ID( pFriend ), strlen( GET_ID( pFriend ) ) );
	mysql_escape_string( escFriendName, GET_NAME( pPlayer ), strlen( GET_NAME( pPlayer ) ) );

	sprintf( query, "insert into UserFriend values( '', '%s', %d, '%s' )",
		escID, GET_CHAR_POS( pFriend ), escFriendName );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_ADDED, g_nPos );
		PutString( g_Packet, escFriendName, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pFriend, g_Packet, g_nPos );
		GET_FRIEND_CNT( pFriend )++;

		// 리스트에 추가
		FRIEND_AddData( pFriend, escFriendName );
		FRIEND_GetFriendList( pFriend );
	}
	SendSystemMsg( pFriend, "Added to your friend list." );
}

void FRIENDS_AddFriend( sPDESC_DATA pPlayer, sPDESC_DATA pFriend )
{
	if( FRIENDS_FindData( pPlayer, pFriend ) )
		return;
	
	char query[dMAX_QUERY_SIZE];
	char escID[50];
	char escFriendID[50];
	char escFriendName[50];
		
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escFriendID, GET_ID( pFriend ), strlen(  GET_ID(pFriend) ) );
	mysql_escape_string( escFriendName, GET_NAME( pFriend ), strlen( GET_NAME( pFriend ) ) );
			
	sprintf( query, "insert into UserFriends values( '', '%s', %d, '%s', %d, '%s' )",
		escID, GET_CHAR_POS( pPlayer ), escFriendID, GET_CHAR_POS(pFriend), escFriendName );
	
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_ADDED, g_nPos );
		PutString( g_Packet, GET_NAME( pFriend ), g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		GET_FRIEND_CNT( pPlayer )++;
		
		// 리스트에 추가
		FRIENDS_AddData( pPlayer, pFriend, GET_NAME( pFriend ) );
		FRIEND_GetFriendList( pPlayer );
		FRIENDS_OnlineCheck( pPlayer );
	}
	SendSystemMsg( pPlayer, "Added to your friend list." );
	
	mysql_escape_string( escID, GET_ID( pFriend ), strlen( GET_ID( pFriend ) ) );
	mysql_escape_string( escFriendID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escFriendName, GET_NAME( pPlayer ), strlen( GET_NAME( pPlayer ) ) );
		
	sprintf( query, "insert into UserFriends values( '', '%s', %d, '%s', %d, '%s' )",
		escID, GET_CHAR_POS( pFriend ), escFriendID, GET_CHAR_POS( pPlayer ), escFriendName );
	
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_ADDED, g_nPos );
		PutString( g_Packet, GET_NAME(pPlayer), g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pFriend, g_Packet, g_nPos );
		GET_FRIEND_CNT( pFriend )++;
		
		// 리스트에 추가
		FRIENDS_AddData( pFriend, pPlayer, GET_NAME(pPlayer) );
		FRIENDS_GetFriendList( pFriend );
		FRIENDS_OnlineCheck( pFriend );
	}
	SendSystemMsg( pFriend, "Added to your friend list." );
}



//
// Function Name: FRIEND_RemoveFriend
// Date: 
// Description: 
//
void FRIEND_RemoveFriend( sPDESC_DATA pPlayer, char *friendName )
{
	if( !FRIEND_FindData( pPlayer, friendName ) )
	{
		SendSystemMsg( pPlayer, "%s is Not Your Friend.", friendName );
		return;
	}
	
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	char escFriendName[128];

	//여기서 칙구등록이 되어있다면.. 삭제...

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escFriendName, friendName, strlen( friendName ) );
	
	sPDESC_DATA pFriend = FindPlayerNameList( escFriendName );

	if( !pFriend )
	{
		SendSystemMsg( pPlayer, "You can not delete %s from your friend list because he is not online.", friendName );
		return;
	}

	sprintf( query, "delete from UserFriend where ownerID = '%s' and ownerPos = %d and friendName = '%s'",
		escID, GET_CHAR_POS( pPlayer ), escFriendName );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_REMOVED, g_nPos );
	//	PutString( g_Packet, friendName, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		GET_FRIEND_CNT( pPlayer )--;

		sPFRIEND_DATA pData = FRIEND_FindData( pPlayer, friendName );

		//
		if( pData )
			FRIEND_DelData( pPlayer, pData );

		//여기서 친구등록을 한번더 스케닝한다.
		FRIEND_ClearFriendList( pPlayer );
		FRIEND_GetFriendList( pPlayer );
		FRIEND_OnlineCheck( pPlayer );
		SendSystemMsg( pPlayer, g_LANG_STR[287], friendName );
	}


	//상대방꺼...	
	mysql_escape_string( escID, GET_ID( pFriend ), strlen( GET_ID( pFriend ) ) );
	mysql_escape_string( escFriendName, GET_NAME( pPlayer ), strlen( GET_NAME( pPlayer ) ) );
	
	sprintf( query, "delete from UserFriend where ownerID = '%s' and ownerPos = %d and friendName = '%s'",
		escID, GET_CHAR_POS( pFriend ), escFriendName );
	
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_REMOVED, g_nPos );
		//	PutString( g_Packet, friendName, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pFriend, g_Packet, g_nPos );
		GET_FRIEND_CNT( pFriend )--;
		
		sPFRIEND_DATA pData = FRIEND_FindData( pFriend, escFriendName );
		
		//
		if( pData )
			FRIEND_DelData( pFriend, pData );
		//여기서 친구등록을 한번더 스케닝한다.
		FRIEND_ClearFriendList( pFriend );
		FRIEND_GetFriendList( pFriend );
		FRIEND_OnlineCheck( pFriend );
		SendSystemMsg( pFriend, g_LANG_STR[287], escFriendName );
	}	
	
}

void FRIENDS_RemoveFriend( sPDESC_DATA pPlayer, char *friendName) //sPDESC_DATA pFriend )
{
	if( !FRIEND_FindData( pPlayer, friendName ) )
	{
		SendSystemMsg( pPlayer, "This is not your friend" );
		return;
	}

	char query[dMAX_QUERY_SIZE];
	char escID[50];
	char escFriendID[50];
	char escFriendName[50];
	
	sPDESC_DATA pFriend = FindPlayerNameList( friendName );

	if( !pFriend )
	{
		SendSystemMsg( pPlayer, "You can only delete logged in friend" );
		return;
	}

	//여기서 칙구등록이 되어있다면.. 삭제...
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escFriendID, GET_ID( pFriend ), strlen( GET_ID( pFriend ) ) );
	mysql_escape_string( escFriendName, GET_NAME(pFriend), strlen( GET_NAME(pFriend) ) );

	sprintf( query, "delete from UserFriends where ownerID = '%s' and ownerPos = %d and friendID = '%s' and friendPos = %d and friendName= '%s'",
		escID, GET_CHAR_POS( pPlayer ), escFriendID, GET_CHAR_POS( pFriend ), escFriendName );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_REMOVED, g_nPos );
		PutString( g_Packet, escFriendID, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		GET_FRIEND_CNT( pPlayer )--;

		sPFRIEND_DATA pData = FRIENDS_FindData( pPlayer, pFriend );

		//
		if( pData )
			FRIEND_DelData( pPlayer, pData );

		//여기서 친구등록을 한번더 스케닝한다.
		FRIEND_ClearFriendList( pPlayer );
		FRIENDS_GetFriendList( pPlayer );
		FRIENDS_OnlineCheck( pPlayer );
		SendSystemMsg( pPlayer, "Deleted from your friend list" );
	}


	//상대방꺼...	
	mysql_escape_string( escID, GET_ID( pFriend ), strlen( GET_ID( pFriend ) ) );
	mysql_escape_string( escFriendID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escFriendName, GET_NAME(pPlayer), strlen( GET_NAME(pPlayer) ) );

	sprintf( query, "delete from UserFriends where ownerID = '%s' and ownerPos = %d and friendName = '%s' and friendPos = %d and friendName= '%s'",
		escID, GET_CHAR_POS( pFriend ), escFriendID, GET_CHAR_POS( pPlayer ), escFriendName );
			
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_REMOVED, g_nPos );
		PutString( g_Packet, escFriendName, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pFriend, g_Packet, g_nPos );
		GET_FRIEND_CNT( pFriend )--;
		
		sPFRIEND_DATA pData = FRIENDS_FindData( pFriend, pPlayer );
		
		//
		if( pData )
			FRIEND_DelData( pFriend, pData );
		//여기서 친구등록을 한번더 스케닝한다.
		FRIEND_ClearFriendList( pFriend );
		FRIENDS_GetFriendList( pFriend );
		FRIENDS_OnlineCheck( pFriend );
		SendSystemMsg( pFriend, "Deleted from your friend list" );
	}	

}

//
// Function Name: FRIEND_GetFriendList
// Date: 
// Description: 
//
void FRIEND_GetFriendList( sPDESC_DATA pPlayer ) 
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	sprintf( query, "select friendName from UserFriend where ownerID = '%s' and ownerPos = %d limit 255",
		escID, GET_CHAR_POS( pPlayer ) );
	
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
		BYTE cnt = 0;
		
		result = mysql_store_result( g_MYSQL );
		
		if( result )
		{	
			while( ( row = mysql_fetch_row( result ) ) )
			{
				//
				FRIEND_AddData( pPlayer, row[0] );	
				
				PutString( g_Packet, row[0], g_nPos );
				cnt++;
			}
			
			*(BYTE*)(g_Packet + 4) = cnt;
			PutSize( g_Packet, g_nPos );
			SendData( pPlayer );
			
			GET_FRIEND_CNT( pPlayer ) = cnt;
			
			mysql_free_result( result );
		}
	}
}

void FRIENDS_GetFriendList( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	sPDESC_DATA pFriend;
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	sprintf( query, "select friendID, friendPos, friendName from UserFriends where ownerID = '%s' and ownerPos = %d limit 255",
		escID, GET_CHAR_POS( pPlayer ) );
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_GET_FRIENDLIST, g_nPos );
	g_nPos += sizeof(BYTE);

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
		BYTE cnt = 0;
		
		result = mysql_store_result( g_MYSQL );
		
		if( result )
		{	
			while( ( row = mysql_fetch_row( result ) ) )
			{
				//
				pFriend = FindPlayerIdList( row[0] );
				if( !pFriend )
				{
					pFriend = (sPDESC_DATA)malloc(sizeof(sDESC_DATA));
					strcpy( pFriend->ch.id , row[0] );
					pFriend->ch.charPos = atoi(row[1]);
					FRIENDS_AddData( pPlayer, pFriend, row[2] );
					PutString( g_Packet, row[2], g_nPos );
					cnt++;
				}
				else
				{
					FRIENDS_AddData( pPlayer, pFriend, GET_NAME( pFriend ) );
					PutString( g_Packet, row[2], g_nPos );
					cnt++;
				}
			}
			
			*(BYTE*)(g_Packet + 4) = cnt;
			PutSize( g_Packet, g_nPos );
			SendData( pPlayer );
			
			
			GET_FRIEND_CNT( pPlayer ) = cnt;
			
			mysql_free_result( result );
		}
	}
}

//
// Function Name: FRIEND_DeleteAllFriend
// Date: 
// Description: 
//
void FRIEND_DeleteAllFriend( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from UserFriend where ownerID = '%s' and ownerPos = %d",
		escID, GET_CHAR_POS( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}


void FRIENDS_DeleteAllFriend( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	sprintf( query, "delete from UserFriends where ownerID = '%s' and ownerPos = %d",
		escID, GET_CHAR_POS( pPlayer ) );
	
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}

//
// Function Name: FRIEND_NewData
// Date: 
// Description: 
//
sPFRIEND_DATA FRIEND_NewData()
{
	sPFRIEND_DATA pData = NULL;

	if( gPLAYERS.friend_memory )
	{
		pData = gPLAYERS.friend_memory;

		REMOVE_FROM_LIST( gPLAYERS.friend_memory, pData, prev, next );
	}
	else
	{
		pData = (sPFRIEND_DATA)malloc(sizeof(sFRIEND_DATA));

		if( !pData )
			return NULL;
	}

	pData->isOnline = 0;
	pData->charPos = 0;
	*pData->name = NULL_STR;
	*pData->id = NULL_STR;
	pData->prev = NULL;
	pData->next = NULL;

	return pData;
}

//
// Function Name: FRIEND_DelData
// Date: 
// Description: 
//
void FRIEND_DelData( sPDESC_DATA pPlayer, sPFRIEND_DATA pData )
{
	REMOVE_FROM_LIST( pPlayer->ch2.friendList, pData, prev, next );
	INSERT_TO_LIST( gPLAYERS.friend_memory, pData, prev, next );
}

//
// Function Name: FRIEND_AddData
// Date: 
// Description: 
//
void FRIEND_AddData( sPDESC_DATA pPlayer, char *name )
{
	sPFRIEND_DATA data = FRIEND_NewData();

	if( !data )
	{
		log( "FRIEND_AddData : !data\r\n" );
		return;
	}
	strcpy( data->name, name );
	INSERT_TO_LIST( pPlayer->ch2.friendList, data, prev, next );
}

void FRIENDS_AddData( sPDESC_DATA pPlayer, sPDESC_DATA pFriend, char *szName )//, int friendPos )
{
	sPFRIEND_DATA data = FRIEND_NewData();
	
	if( !data )
	{
		log( "FRIEND_AddData : !data\r\n" );
		return;
	}
	strcpy( data->id, GET_ID( pFriend ) );
	data->charPos = GET_CHAR_POS( pFriend );
	strcpy( data->name, szName );
	INSERT_TO_LIST( pPlayer->ch2.friendList, data, prev, next );
}

//
// Function Name: FRIEND_FindData
// Date: 
// Description: 
//
sPFRIEND_DATA FRIEND_FindData( sPDESC_DATA pPlayer, char *name )
{
	sPFRIEND_DATA data, next_data;

	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	if( !strcmp( data->name, name ) )
		return data;
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
	return NULL;
}


//
// Function Name: FRIEND_FindData
// Date: 
// Description: 
//
sPFRIEND_DATA FRIENDS_FindData( sPDESC_DATA pPlayer, sPDESC_DATA pFriend )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	if( !strcmp( data->name, GET_NAME( pFriend ) ) )
			return data;	
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
	return NULL;
}

//
// Function Name: FRIEND_ClearFriendList
// Date: 
// Description: 
//
void FRIEND_ClearFriendList( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;

	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	REMOVE_FROM_LIST( pPlayer->ch2.friendList, data, prev, next );
	INSERT_TO_LIST( gPLAYERS.friend_memory, data, prev, next );
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
}

//
// Function Name: DestroyFriendMemory
// Date: 
// Description: 
//
void DestroyFriendMemory()
{
	sPFRIEND_DATA data, next_data;

	LIST_WHILE( gPLAYERS.friend_memory, data, next_data, next, WMgr );
	REMOVE_FROM_LIST( gPLAYERS.friend_memory, data, prev, next );
	SAFE_FREE( data );
	LIST_WHILEEND( gPLAYERS.friend_memory, data, next_data );
}

//
// Function Name: FRIEND_OnOffCheck
// Date: 
// Description: 
//
BOOL FRIEND_CheckOnOFFOnline( sPDESC_DATA pPlayer, char *friendName )
{
	sPDESC_DATA	pFriend = FindPlayerNameList( friendName );

	if( !pFriend )
	{
		return FALSE;
	}
	return TRUE;
}


//
// Function Name: FRIEND_OnOffCheck
// Date: 
// Description: 
//
void FRIEND_OnlineCheck( sPDESC_DATA pPlayer )
{
	OnlineOn( pPlayer );
	OnlineOff( pPlayer );
}

void FRIENDS_OnlineCheck( sPDESC_DATA pPlayer )
{
	OnlineOns( pPlayer );
	OnlineOffs( pPlayer );
}

void OnlineOn( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name ); //name==id
	
	if( pFriend )
	{
		if( data->charPos == GET_CHAR_POS(pFriend) )
		{
			data->isOnline = 1;
			
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_FRIEND_LOGIN, g_nPos );
			PutString( g_Packet, GET_NAME( pFriend ), g_nPos );
			PutSize( g_Packet, g_nPos );
			SendData( pPlayer, g_Packet, g_nPos );
			
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_FRIEND_LOGIN, g_nPos );
			PutString( g_Packet, GET_NAME( pPlayer ) , g_nPos );
			PutSize( g_Packet, g_nPos ); 
			SendData( pFriend, g_Packet, g_nPos );
		}
	}
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
}

void OnlineOns( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name ); //name==id
	
	if( pFriend )
	{
		data->isOnline = 1;
		
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_LOGIN, g_nPos );
		PutString( g_Packet, GET_NAME( pFriend ), g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_LOGIN, g_nPos );
		PutString( g_Packet, GET_NAME( pPlayer ) , g_nPos );
		PutSize( g_Packet, g_nPos ); 
		SendData( pFriend, g_Packet, g_nPos );
		
	}
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
}


void OnlineOff( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr1 );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name );
	if( !pFriend )
	{
		data->isOnline = 0;
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_LOGOUT, g_nPos );
		PutString( g_Packet, data->name, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
	}	
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
}

void OnlineOffs( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;

	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr1 );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name );
	if( !pFriend )
	{
		data->isOnline = 0;
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_LOGOUT, g_nPos );
		PutString( g_Packet, data->name, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
	}	
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
}



/*
 * Function Name: FRIEND_IMLogin
 * Date: 
 * Description: 나 로그인하오~
 */
void LogInMesg( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name );
	
	if( pFriend )
	{
		SendFriendMsg( pFriend, g_LANG_STR[288], GET_NAME( pPlayer ) );
	}

	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
}


/*
 * Function Name: FRIEND_IMLogin
 * Date: 
 * Description: 나 로그아웃하오~
 */
void LogOutMesg( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name );
	
	if( pFriend )
	{
		SendFriendMsg( pFriend, g_LANG_STR[289], GET_NAME( pPlayer ) );
	}
	
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
}





/*
 * Function Name: FRIEND_IMLogin
 * Date: 
 * Description: 나 로그인하오~
 */
void FRIEND_IMLogin( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char UserName[128];

	mysql_escape_string( UserName, GET_NAME( pPlayer ), strlen( GET_NAME( pPlayer ) ) );
	
	sprintf( query, "select * from UserFriend where friendName = '%s'", UserName );
	
	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
	}
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
	//	BYTE cnt = 0;
		
		result = mysql_store_result( g_MYSQL );
		
		if( result )
		{

			g_nPos = 2;
			PutWord( g_Packet, dPACKET_FRIEND_LOGIN, g_nPos );

			PutSize( g_Packet, g_nPos );
			

			while( ( row = mysql_fetch_row( result ) ) )
			{
				//
				PutString( g_Packet, row[1], g_nPos );
				FRIEND_AddData( pPlayer, row[1] );
				SendData( pPlayer, g_Packet, g_nPos );
			}		
				
			mysql_free_result( result );
		}
	}
	
}


// EOF



