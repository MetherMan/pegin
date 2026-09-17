/**
*	$Workfile: friends.h $
*
*	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
*
*	Author	sk8snow
* 
*
*	$Revision: 4 $
*	$Date: 04-04-30 9:46a $
*     
*	@Build Log :
*	$Log: /GameServer/friends.h $
* 
* 4     04-04-30 9:46a Sk8snow
* 
* 3     04-03-31 8:22a Sk8snow
*        
*/
#ifndef __FRIEND_H__
#define __FRIEND_H__

#define dMAX_FRIEND		30

void FRIEND_AddFriend( sPDESC_DATA pPlayer , char *friendName );
void FRIENDS_AddFriend( sPDESC_DATA pPlayer, sPDESC_DATA pFriend );

void FRIEND_RemoveFriend( sPDESC_DATA pPlayer, char *friendName );
void FRIENDS_RemoveFriend( sPDESC_DATA pPlayer, char *friendName );//sPDESC_DATA pFriend );

void FRIEND_GetFriendList( sPDESC_DATA pPlayer );
void FRIENDS_GetFriendList( sPDESC_DATA pPlayer );

void FRIEND_DeleteAllFriend( sPDESC_DATA pPlayer );
void FRIEND_DeleteAllFriends( sPDESC_DATA pPlayer );

sPFRIEND_DATA FRIEND_FindData( sPDESC_DATA pPlayer, char *name );
sPFRIEND_DATA FRIENDS_FindData( sPDESC_DATA pPlayer, sPDESC_DATA pFriend );

void FRIEND_AddData( sPDESC_DATA pPlayer, char *name );
void FRIENDS_AddData( sPDESC_DATA pPlayer, sPDESC_DATA pFriend, char *szName );//, int friendPos );

void FRIEND_OnlineCheck( sPDESC_DATA pPlayer );
void FRIENDS_OnlineCheck( sPDESC_DATA pPlayer );

void OnlineOn( sPDESC_DATA pPlyaer );
void OnlineOns( sPDESC_DATA pPlyaer );

void OnlineOff( sPDESC_DATA pPlyaer );
void OnlineOffs( sPDESC_DATA pPlyaer );

void FRIEND_DelData( sPDESC_DATA pPlayer, sPFRIEND_DATA pData );
void DestroyFriendMemory();
void FRIEND_ClearFriendList( sPDESC_DATA pPlayer );


BOOL FRIEND_CheckOnOFFOnline( sPDESC_DATA pPlayer, char *friendName );
void FRIEND_IMLogin( sPDESC_DATA pPlayer );
void LogInMesg( sPDESC_DATA pPlayer );
void LogOutMesg( sPDESC_DATA pPlyaer );



#endif

//eof



