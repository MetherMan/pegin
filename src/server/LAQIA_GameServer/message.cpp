/**
 *	$Workfile: message.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 78 $
 *	$Date: 04-12-31 12:08p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/message.cpp $
 * 
 * 78    04-12-31 12:08p Sk8snow
 * 
 * 77    04-12-22 5:18p Sk8snow
 * 
 * 76    04-12-15 10:40a Sk8snow
 * 
 * 75    04-11-18 1:40a Sk8snow
 * 
 * 74    04-10-29 11:56p Sk8snow
 * 
 * 73    04-10-19 11:14p Sk8snow
 * 
 * 72    04-09-20 10:53p Sk8snow
 * 
 * 71    04-09-11 11:20p Sk8snow
 * 
 * 70    04-09-08 3:54p Sk8snow
 * 
 * 69    04-08-04 9:27a Sk8snow
 * 
 * 68    04-07-13 7:26a Sk8snow
 * 
 * 67    04-05-29 10:12p Sk8snow
 * 
 * 66    04-05-23 1:45p Sk8snow
 * 
 * 65    04-05-21 11:37a Sk8snow
 * 
 * 64    04-05-18 1:17a Sk8snow
 * 
 * 63    04-05-14 10:38p Sk8snow
 * 
 * 62    04-05-11 1:45a Sk8snow
 * 
 * 61    04-04-30 9:46a Sk8snow
 * 
 * 60    04-04-07 10:17p Sk8snow
 * 
 * 59    04-04-03 4:03p Sk8snow
 * 
 * 58    04-04-01 5:45p Sk8snow
 * 
 * 57    04-04-01 5:33p Sk8snow
 * 
 * 56    04-03-31 8:22a Sk8snow
 *        
 */

#include "def.h"
#include <stdarg.h>

#include "structs.h"
#include "extern.h"
#include "message.h"

#include "usestring.h"

#include "world.h"
#include "player.h"
#include "monster.h"
#include "item.h"
#include "enchant_rates.h"
#include "fight.h"
#include "skill.h"

#include "dataserver.h"
#include "admin.h"

#include "itemdb.h"
#include "shop.h"

#include "party.h"
#include "quest.h"

#include "house.h"

#include "effect.h"
#include "guild.h"

#include "friends.h"

#include "npc.h"
#include "fishing.h"
#include "pet.h"

#include "war.h"

#include "billing.h"
#include <time.h>

#include "encrypt.h"

char g_Packet[dMAX_DATA_SOCK_BUFF];
int g_nPos;

#ifdef USE_ENCRYPT_PACKET	
char g_EncPacket[dMAX_DATA_SOCK_BUFF+512];
#endif


BOOL LoadWarPlayer( sPDESC_DATA pPlayer )
{
	FILE	*fp, *fp2;
	fp = fopen( "./DATA/GUILDWAR/TeamInformation.txt", "rb" );
	fp2 = fopen( "./DATA/GUILDWAR/EngineerInformation.txt", "rb" );
	
	if( !fp && !fp2 )
	{
		return 0;
	}
	
	char	szID[256];
	
	BOOL	checkFile = TRUE;

	memset( szID, 0, 256 );
		
	while( !feof( fp ) )
	{
		fscanf( fp, "%s", szID );
		
		if( strcmp( szID, GET_ID( pPlayer ) ) == 0 )
		{
			checkFile = TRUE;
			break;
		}
		checkFile = FALSE;
	}
	
	if( checkFile == FALSE )
	{
		while( !feof( fp2 ) )
		{
			fscanf( fp2, "%s", szID );
			
			if( strcmp( szID, GET_ID( pPlayer ) ) == 0 )
			{
				checkFile = TRUE;
				break;
			}
			checkFile = FALSE;
		}
	}

	if( checkFile == FALSE )
	{
		return 0;
	}
	return 1;
}

//
// Function Name: SendSystemMsg
// Date:
// Description: 
//
void SendSystemMsg( sPDESC_DATA pPlayer, char *str, ... )
{
	char tmp_buf[1024];
	va_list args;

	string	test;
			
	va_start( args, str );
	vsprintf( tmp_buf, str, args );
	va_end( args );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SYSTEM_MSG, g_nPos );
	PutString( g_Packet, tmp_buf, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos ); 
}


void SendSystemMsg( sPDESC_DATA pPlayer, const char *str )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SYSTEM_MSG, g_nPos );
	PutString( g_Packet, (char*)str, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos ); 
}


//
// Function Name: SendFriendMsg
// Date:
// Description: 
//
void SendFriendMsg( sPDESC_DATA pPlayer, char *str, ... )
{
	char tmp_buf[1024];
	va_list args;
	
	va_start( args, str );
	vsprintf( tmp_buf, str, args );
	va_end( args );
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_FRIEND_MSG, g_nPos );
	PutString( g_Packet, tmp_buf, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos ); 
}


//
// Function Name: RecvFromClient
// Date: 2003-02-12
// Description: 클라이언트에서 전송한 데이터를 버퍼에 담아온다
//
BOOL RecvFromClient( sPDESC_DATA pPlayer )
{
	int recvSize;
	char recvBuff[dMAX_SOCK_BUFF];//[40960];//[dMAX_SOCK_BUFF];

	recvSize = recv( pPlayer->sock, recvBuff,dMAX_SOCK_BUFF, 0 ); // 40960, 0 );//
	
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

	if( ( pPlayer->recvSize + recvSize ) >= dMAX_SOCK_BUFF ) //40960 ) //
	{
		log( "%s's recvBuff Overflow\r\n", pPlayer->ch.id );
		return 0;
	}
	
	pPlayer->lastRecvTime = g_CurrTime;

	memcpy( &pPlayer->recvBuff[pPlayer->recvSize], recvBuff, recvSize );
	pPlayer->recvSize += recvSize;

	return 1;
}


//
// Function Name: NullFunc
// Date: 2003-02-12
// Description: 
//
BOOL PACKET_NullFunc( sPDESC_DATA pPlayer )
{
	return 0;
}

//
// Function Name: PACKET_AliveCheck
// Date: 
// Description: 
//
BOOL PACKET_AliveCheck( sPDESC_DATA pPlayer )
{
#ifdef NO_TIMELIMIT
	return 1;
#endif
	if( GET_STATE( pPlayer ) == dDESC_STATE_PLAYING ||
		GET_STATE( pPlayer ) == dDESC_STATE_FIGHT )
	{
		if( pPlayer->aliveCnt > 200 )
		{
			pPlayer->aliveCnt = 0;
			pPlayer->aliveCheckTick = g_CurrRealTime;
		}
		else
			pPlayer->aliveCnt++;
	}

	if( g_CurrTime < pPlayer->lastAliveCheck + dKEEP_ALIVE_TIME )
	{
		if( pPlayer->isSpeedHack >= 2 )
		{	
			if( GET_STATE( pPlayer ) == dDESC_STATE_PLAYING ||
				GET_STATE( pPlayer ) == dDESC_STATE_FIGHT )
			{
				g_nPos = 2;
				PutWord( g_Packet, dPACKET_SYSTEM_MSG, g_nPos );
				PutString( g_Packet, g_LANG_STR[196], g_nPos );
				PutSize( g_Packet, g_nPos );

				SendData( pPlayer, g_Packet, g_nPos );
				log_file( "discon_log.txt", "PACKET_AliveCheck : disconnect [%s/%s]\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );
			}
			return 0;
		}
		else
		{
			pPlayer->lastAliveCheck = g_CurrTime;
			pPlayer->isSpeedHack++;
		}
	}
	else
	{
		pPlayer->lastAliveCheck = g_CurrTime;
		pPlayer->isSpeedHack = 0;
	}

	return 1;
}



//
// Function Name: PACKET_LoginInfo
// Date: 
// Description: 
//
BOOL PACKET_LoginInfo( sPDESC_DATA pPlayer )
{	
	/*
	2. 로그인의 결과
	로그인 결과 패킷 번호로 BYTE형의 결과값을 포함 
	결과값이 0이면 아이디 없음 1이면 패스워드 틀림
	2 이면 OK
	*/
	char		id[50];
	char		pw[50];
	//int			nPatchVer=0;
	BYTE		retVal = 0;

	// Daum Auth 를 사용할 경우 
	// ID는 DAUMID 
	// PW는 USERID

	GetString( pPlayer->TemprecvBuff, id, pPlayer->recvPos );
	GetString( pPlayer->TemprecvBuff, pw, pPlayer->recvPos );	
	int	nPatchVer = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

#ifdef dUSE_ENCRYPT_ID
	char		decID[512];

	if( !SimpleDecrypt( id, decID ) )
		return 0;

	strcpy( id, decID );
#endif
	strcpy( GET_ID( pPlayer ) , id );

	sPDESC_DATA pFindPlayer;
	pFindPlayer = FindPlayerIdList( id );

	if( pFindPlayer )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_ALREADY_CONNECTED, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );

		pFindPlayer->disconnectFlag = 1;
		return 1;
	}

/*	GetBadUserType( pPlayer );

	// 불량 유져
	if( pPlayer->badUserType == dBADUSER_TYPE_DISCON )
	{
		g_nPos = 2;

		PutWord( g_Packet, dPACKET_LOGINRESULT, g_nPos );
		PutByte( g_Packet, 3, g_nPos );
		PutSize( g_Packet, g_nPos );

		SendData( pPlayer, g_Packet, g_nPos );
		return 1;
	}

	if( nPatchVer != dPATCHNVER )
	{
		g_nPos = 2;

		PutWord( g_Packet, dPACKET_LOGINRESULT, g_nPos );
		PutByte( g_Packet, 3, g_nPos );
		PutSize( g_Packet, g_nPos );

		SendData( pPlayer, g_Packet, g_nPos );

		pPlayer->disconnectFlag = 1;
		log_file( "DFClientVer.txt", "IP=[%s]\r\n", pPlayer->ip );
		return 1;
	}
*/
	int hashIdx = GetStrHashIndex( GET_ID( pPlayer ) );
	INSERT_TO_LIST( gPLAYERS.id_list[hashIdx], pPlayer, id_prev, id_next );
	pPlayer->isInIdList = 1;

	retVal = 2;

	// 로그인 결과
	g_nPos = 2;

	// 1 이면 연결 해제
	// 2 이면 OK
	PutWord( g_Packet, dPACKET_LOGINRESULT, g_nPos );
	PutByte( g_Packet, retVal, g_nPos );	
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}


//
// Function Name: EndMapLoading
// Date: 2003-02-12
// Description: 
//
BOOL PACKET_EndMapLoading( sPDESC_DATA pPlayer )
{
	BYTE btIsFirstLoad = pPlayer->ch2.btFirstLoad;

	// 게임 속으로
	if( GET_STATE( pPlayer ) != dDESC_STATE_LOADING )
		return 1;

	if( pPlayer->ch2.gotoPosMapKind == dMAP_KIND_INDOOR )
	{
		PlayerIntoIndoor( pPlayer,
			pPlayer->ch2.gotoMapNum, 
			pPlayer->ch2.gotoPosX, 
			pPlayer->ch2.gotoPosY,
			pPlayer->ch2.gotoPosMapKind );			
	}
	else
	{
		PlayerIntoMap( pPlayer, 
			pPlayer->ch2.gotoMapNum, 
			pPlayer->ch2.gotoPosX, 
			pPlayer->ch2.gotoPosY,
			pPlayer->ch2.gotoPosMapKind );
	}
	GET_STATE( pPlayer ) = dDESC_STATE_PLAYING;

	// 공성중인 맵인 경우
	// 공성중인 맵에 처음 접속한 경우 
	// 마을로 보낸다 
#ifdef USE_GUILD_WAR
	if( btIsFirstLoad )
	{
		if( gWORLD[GET_MAP_NUM( pPlayer )]->underAttack )
		{
			sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];
			
			if( GET_GUILD( pPlayer ) == pWorld->warCristal.allowGuild[0] )
			{				
				pPlayer->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
				pPlayer->ch2.gotoPosX = pWorld->warCristal.posX + number( 2, 3 );
				pPlayer->ch2.gotoPosY = pWorld->warCristal.posY + number( 2, 3 );
				
				PlayerFromMap( pPlayer );
				SendMapLoading( pPlayer );
				return 1;
			}
			else 
			{
				PlayerFromMap( pPlayer );
				PlayerToStartPoint( pPlayer ); 
				SendMapLoading( pPlayer );
				return 1;
			}
		}
	}
#endif
	// END
	if( GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE1 ||
		GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE2 ||
		GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE3 )
	{
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] &&
			!pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{			
			PlayerFromMap( pPlayer );
			
			GET_GOTO_MAPKIND( pPlayer ) = dMAP_KIND_FIELD;
			
			GET_GOTO_MAPNUM( pPlayer ) = 20;
			GET_GOTO_POSX( pPlayer ) = 100;
			GET_GOTO_POSY( pPlayer ) = 60;
			
			SendMapLoading( pPlayer );
		}
	}

	DATASERV_SAVECHA( pPlayer );
	return 1;
}

//
// Function Name: CharacterMove
// Date: 2003-02-12
// Description: 
//
BOOL PACKET_CharacterMove( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE &&
		GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_GHOST )
		return 1;

	int posX, posY;
	BYTE layer;

	posX = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	posY = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	layer = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	return MovePlayer( pPlayer, posX, posY , layer );
}





//
// Function Name: MakeChatMsg
// Date: 2003-03-12
// Description: 
//
void MakeChatMsg( sPDESC_DATA pPlayer , char *str )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHAT_MSG, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	//PutString( g_Packet, (char*)test.c_str(), g_nPos );
	PutString( g_Packet, str, g_nPos );
	PutSize( g_Packet, g_nPos );
}


//
// Function Name: ChatingProc
// Date: 2003-02-12
// Description: 
//
BOOL PACKET_ChatingProc( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE &&
		GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_CORPSE )
		return 1;

	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING &&
		GET_STATE( pPlayer ) != dDESC_STATE_FIGHT )
		return 1;

	if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_DENY_CHAT ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[3] );
		return 1;
	}

	// 버로우 , 스텔스
	UnSetVisibleSkill( pPlayer );
	
	// time check
	if( pPlayer->chatEndTime > g_CurrTime )
		return 1;
	else
		pPlayer->chatEndTime = g_CurrTime + dCHAT_DELAY_TIME;

	char str[1024];

	GetString( pPlayer->TemprecvBuff, str, pPlayer->recvPos );
	
	//5000실드 이상이 있는지 체크한다. 없으면 돈이 없다고 메시지를 띄워줌
	if( *str == '+' && strlen( str ) > 1 )
	{
		if( pPlayer->ch2.worldChatDelay + 10 > g_CurrRealTime )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[149], ( pPlayer->ch2.worldChatDelay + 10 ) - g_CurrRealTime );
			return 1;
		}
		
		/*if( GET_ALL_SKILL( pPlayer ) < 20 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[156], 25 );
			return 1;
		}*/
		
		if( GET_MONEY( pPlayer ) < 50000 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[157], 50000 );
			return 1;
		}

		g_nPos = 2;
		PutWord( g_Packet, dPACKET_MAP_SHOUT, g_nPos );
		PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
		PutString( g_Packet, &str[1], g_nPos );
		PutSize( g_Packet, g_nPos );
		
		SendToMap( GET_MAP_NUM( pPlayer ), g_Packet, g_nPos );

		GET_MONEY( pPlayer ) = MIN( GET_MONEY( pPlayer ) - 50000, 0 );
		DATASERV_SendUpdateMoney( pPlayer );
		UpdateMoney( pPlayer );
		
		pPlayer->ch2.worldChatDelay = g_CurrRealTime;
		
		SendSystemMsg( pPlayer, g_LANG_STR[275], 50000 );
		
		return 1;
	}
	
#ifdef dDAUM_BILLING//이것으로 변경 할것 dDAUM_BILLING*/ World Shout
	if( *str == '<' && strlen( str ) > 1 )
	{
#ifdef dTEST_SERVER
#else
		BYTE haveShoutItem = 0;
		
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM13 ) )
		{
			haveShoutItem = 1;
		}
		
		if( !haveShoutItem )
		{
			if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM14 ) )
			{
				haveShoutItem = 1;
			}
		}
		
		if( !haveShoutItem )
		{
			if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM15 ) )
			{
				haveShoutItem = 1;
			}
		}
		
		if( !haveShoutItem )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[222] );
			return 1;
		}
#endif
		
		g_nPos = 2;
		int		isadmin=0;
		PutWord( g_Packet, dPACKET_SELLING_CHAT, g_nPos );
		PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
		PutString( g_Packet, &str[1], g_nPos );
		if( pPlayer->isAdmin == 1 )
		{
			isadmin = 1;
			PutInteger( g_Packet, isadmin, g_nPos );
		}
		else
		{
			isadmin = 0;
			PutInteger( g_Packet, isadmin, g_nPos );
		}
		PutSize( g_Packet, g_nPos );
		
		sPDESC_DATA d, next_d;
		
		LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
		
		if( GET_STATE( d ) == dDESC_STATE_FIGHT ||
			GET_STATE( d ) == dDESC_STATE_PLAYING )
		{
			SendData( d, g_Packet, g_nPos );
		}
		
		LIST_WHILEEND( gPLAYERS.list, d, next_d );
				
		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL4 );
		
		return 1;
	}
#endif


	if( *str == '?' )		// 한국

	{
		if( CheckItemCommand( pPlayer, &str[1] ) )
			return 1;
	}

	if( pPlayer->isAdmin && *str == '@' )		// 개발자
	{
		if( CheckAdminCommand( pPlayer, &str[1] ) )
			return 1;
	}

	MakeChatMsg( pPlayer, str );

	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR )
	{
		
		int posX = (int)(pPlayer->ch.posX/dONE_TILE);
		int posY = (int)(pPlayer->ch.posY/dONE_TILE);
		
		for( int y = posY - 1; y <= posY + 1; y++ )
		{
			for( int x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					sPDESC_DATA d, next_d;
					int rangeX, rangeY;
					BYTE range = 12;

					LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d , w_next, WMgr );
					rangeX = abs( GET_POSX( pPlayer ) - GET_POSX( d ) );
					rangeY = abs( GET_POSY( pPlayer ) - GET_POSY( d ) );
					if( rangeX <= range && rangeY <= range && !IS_SET( d->ch2.flag, dPLAYER_FLG_DENY_CHAT ) )
						SendData( d, g_Packet, g_nPos );
					LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->player[y][x], d, next_d );
				}
			}
		}
	}
	else
	{
		sPDESC_DATA player, next_player;
		
		LIST_WHILE( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player, w_next, WMgr );
		if( !IS_SET( player->ch2.flag, dPLAYER_FLG_DENY_CHAT ) )
			SendData( player , g_Packet, g_nPos );
		LIST_WHILEEND( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player );
	}

	return 1;
}


//
// Function Name: AttackMonster
// Date: 2003-02-12
// Description: 
//
BOOL PACKET_AttackMonster( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		return 1;

	if( GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) )
		return 1;

	if( CheckWaitSkill( pPlayer ) )
		return 1;

	// 마법은 Attack 메시지 사용 않함
	if( GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM &&
		GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
		return 1;

	if( !IsCanAttackWeight( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[179] );
		return 1;
	}

	if( pPlayer->ch2.actionDelay > g_CurrTime )
		return 1;

	int mobIdx = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPMOB_DATA pMob = FindMobInArea( 
		pPlayer->ch.mapNum, 
		pPlayer->ch.posX/dONE_TILE, 
		pPlayer->ch.posY/dONE_TILE, 
		mobIdx 
		);
	
	if( pMob )
	{
		if( pMob->isDead )
			return 1;

#ifdef AUTO_ATTACK
		if( pMob == pPlayer->enemyMob )
			return 1;
#endif
		
#ifdef USE_GUILD_WAR
		if( pMob->mobNum == dMONSTER_CASTLEDOOR ||
			pMob->mobNum == dMONSTER_WARCRISTAL )
		{
			sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];
			
			if( !pWorld )
				return 1;
			
			if( pWorld->underAttack )
			{
				if( GET_ALL_SKILL( pPlayer ) <= 25 )
					return 1;

				if( GET_GUILD( pPlayer ) != pWorld->warCristal.allowGuild[1] )
					return 1;
			}
		}
#endif
		set_fighting( pPlayer, pMob );
	}

	return 1;
}	



//
// Function Name: RestartOption
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_RestartOption( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) == dDEAD_STATE_NONE )
		return 1;

	BYTE res = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	switch( res )
	{
	// 마을로 가기
	case 0:
#ifdef dUSE_DEAD_GHOST
	case 1:
#endif


		
#ifdef USE_GUILD_WAR
		if( gWORLD[GET_MAP_NUM( pPlayer )]->underAttack )
		{
			sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];
			
			if( GET_GUILD( pPlayer ) == pWorld->warCristal.allowGuild[0] )
			{
				GET_HP( pPlayer ) = (int)( GET_MAX_HP( pPlayer ) * 0.3 );
				
				SendPlayerHP( pPlayer );
				SendPlayerMP( pPlayer );

				GET_DEAD_STATE( pPlayer ) = dDEAD_STATE_NONE;

				pPlayer->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
				pPlayer->ch2.gotoPosX = pWorld->warCristal.posX + number( 2, 3 );
				pPlayer->ch2.gotoPosY = pWorld->warCristal.posY + number( 2, 3 );

				PlayerFromMap( pPlayer );
				SendMapLoading( pPlayer );
				return 1;
			}
			else if( GET_GUILD( pPlayer ) == pWorld->warCristal.allowGuild[1] )
			{
				GET_HP( pPlayer ) = (int)( GET_MAX_HP( pPlayer ) * 0.3 );
				
				SendPlayerHP( pPlayer );
				SendPlayerMP( pPlayer );

				GET_DEAD_STATE( pPlayer ) = dDEAD_STATE_NONE;

				RandomTrans( pPlayer );
				return 1;
			}
		}
#endif

		if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
			PlayerFromIndoor( pPlayer );
		else
			PlayerFromMap( pPlayer );
		
		GET_HP( pPlayer ) = (int)( GET_MAX_HP( pPlayer ) * 0.3 );
		
		SendPlayerHP( pPlayer );
		SendPlayerMP( pPlayer );

		PlayerToStartPoint( pPlayer ); 

		GET_DEAD_STATE( pPlayer ) = dDEAD_STATE_NONE;
		
		SendMapLoading( pPlayer );
		
		break;
	// 유령으로 돌아다니기 
#ifndef dUSE_DEAD_GHOST
	case 1:
		GET_DEAD_STATE( pPlayer ) = dDEAD_STATE_GHOST;

		SetVisible( pPlayer, 1 );

		GET_GHOST_TIMER( pPlayer ) = g_CurrTime;
		break;
#endif

	}
	
	return 1;
}

//
// Function Name: SendWhisperMsg
// Date: 2003-05-16
// Description: 
//
void SendWhisperMsg( sPDESC_DATA pPlayer,  char *name, char *msg )
{
	char sendBuffer[2048];
	int sendPos = 2;

	PutWord( sendBuffer, dPACKET_CHAT_WHISPER, sendPos );
	PutString( sendBuffer, name, sendPos );
	PutString( sendBuffer, msg, sendPos );
	PutSize( sendBuffer, sendPos );
	
	SendData( pPlayer, sendBuffer, sendPos );
}

//
// Function Name: WhisperProc
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_WhisperProc( sPDESC_DATA pPlayer )
{
#ifndef __CHINA_VER__
	if( GET_ALL_SKILL( pPlayer ) < 11 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[223] );
		return 1;
	}
#endif

	char to_name[128];
	char str[256];

	GetString( pPlayer->TemprecvBuff, to_name, pPlayer->recvPos );
	GetString( pPlayer->TemprecvBuff, str, pPlayer->recvPos );

	//공지하자
	if( pPlayer->isAdmin )
	{
		if( !strcmp( to_name, "Notice" ) || 
			!strcmp( to_name, "Admin" ) )
		{
			BroadCastingMsg( str );
			return 1;
		}
	}

	if( *str == '/' )
		return 1;
	
	// time check
	if( pPlayer->chatEndTime > g_CurrTime )
		return 1;
	else
		pPlayer->chatEndTime = g_CurrTime + dCHAT_DELAY_TIME;

	if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_DENY_WHISPER ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[4] );
		return 1;
	}

	sPDESC_DATA to = FindPlayerNameList( to_name );

	// 해당 케릭 없음
	if( !to )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_CHAT_WHISPER_RES, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		return 1;
	}

	if( GET_STATE( to ) != dDESC_STATE_PLAYING &&
		GET_STATE( to ) != dDESC_STATE_FIGHT )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_CHAT_WHISPER_RES, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		return 1;
	}

	if( IS_SET( to->ch2.flag, dPLAYER_FLG_DENY_WHISPER ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[5] );
		return 1;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHAT_WHISPER, g_nPos );
	PutString( g_Packet, pPlayer->ch.name, g_nPos );
	PutString( g_Packet, str, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( to, g_Packet, g_nPos );

	// 귓말 성공 
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHAT_WHISPER_RES, g_nPos );
	PutByte( g_Packet, 1, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}


//
// Function Name: GetAllCharacterInfo
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_GetAllCharacterInfo( sPDESC_DATA pPlayer )
{
	if( pPlayer->isWaitAllData )
		return 1;

	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_GETSELECTINFO, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );

	gPLAYERS.WaitAllData.push( pPlayer );
	pPlayer->isWaitAllData = 1;

	return 1;
}


//
// Function Name: SelectCharacter
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_SelectCharacter( sPDESC_DATA pPlayer )
{
	if( pPlayer->isWaitCharData )
		return 1;

	int charPos;

	charPos = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_GETCHARINFO, g_nPos );
	PutString( g_Packet, pPlayer->ch.id, g_nPos );
	PutByte( g_Packet, charPos, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );

	gPLAYERS.WaitCharData.push( pPlayer );
	pPlayer->isWaitCharData = 1;
	return 1;
}


//
// Function Name: MakeCharProc
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_MakeCharProc( sPDESC_DATA pPlayer )
{
	char name[128];
	BYTE sex;
	WORD hairStyle, faceStyle;
	WORD jacket, pants, glove, shoes;
	BYTE charPos;
	BYTE str, intelli, dex;

	sex = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	hairStyle = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	faceStyle = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	jacket = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	pants = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	glove = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	shoes = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );

	str = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	intelli = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	dex = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	charPos = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );	

	GetString( pPlayer->TemprecvBuff, name, pPlayer->recvPos );

	// 머리 
	switch( hairStyle )
	{
	case 10085:
	case 10086:
	case 10087:
	case 10088:
	case 10089:
	case 10090:
		break;
	default:
		return 0;
	}
	// 얼굴 
	switch( faceStyle )
	{
	case 10081:
	case 10082:
	case 10083:
	case 10084:
		break;
	default:
		return 0;
	}
	// 상의 
	switch( jacket )
	{
	case 3970:
	case 3991:
	case 4012:
		break;
	default:
		return 0;
	}
	// 하의
	switch( pants )
	{
	case 5986:
	case 6007:
	case 6028:
		break;
	default:
		return 0;
	}

	// 장갑 
	switch( glove )
	{
	case 4978:
	case 4999:
	case 5020:
		break;
	default:
		return 0;
	}

	// 신발 
	switch( shoes )
	{
	case 6994:
	case 7015:
	case 7036:
		break;
	default:
		return 0;
	}

	if( !strlen( name ) )
	{
		log( "Invalid name length '%s'\r\n", pPlayer->ch.id );
		return 0;
	}

	// 추가 보너스는 20이다 
	if( str < 10 || str > 35 )
		return 0;
	
	if( intelli < 10 || intelli > 35 )
		return 0;

	if( dex < 10 || dex > 35 )
		return 0;

	if( ( str + intelli + dex ) > 55 )
		return 0;
	
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_MAKENEWCHAR, g_nPos );
	PutString( g_Packet, pPlayer->ch.id , g_nPos );

	PutByte( g_Packet, sex, g_nPos );
	PutWord( g_Packet, hairStyle, g_nPos );
	PutWord( g_Packet, faceStyle, g_nPos );
	PutWord( g_Packet, jacket, g_nPos );
	PutWord( g_Packet, pants, g_nPos );
	PutWord( g_Packet, glove, g_nPos );
	PutWord( g_Packet, shoes, g_nPos );

	PutByte( g_Packet, str, g_nPos );
	PutByte( g_Packet, intelli, g_nPos );
	PutByte( g_Packet, dex, g_nPos );

	PutByte( g_Packet, charPos, g_nPos );

	PutString( g_Packet, name, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );


	return 1;
}


//
// Function Name: DeleteCharacter
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_DeleteCharacter( sPDESC_DATA pPlayer )
{
	if( pPlayer->isWaitDelData )
		return 1;

	BYTE charPos = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	if( charPos > 2 )
	{
		return 0;
	}

#ifdef _dUMENOKOJI	//단장 캐릭터 삭제 가능 Deletion of a Guild master Character is possible
		

#else
	// 길드마스터는 케릭터 삭제 불가 
	if( pPlayer->selectGuildLevel[charPos] == dGUILD_LEVEL_MASTER )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_SELCHAR_SYSTEMMSG, g_nPos );
		PutString( g_Packet, "Cannot be deleted", g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		return 1;
	}
#endif

	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_DELCHAR, g_nPos );
	PutString( g_Packet, pPlayer->ch.id, g_nPos );
	PutByte( g_Packet, charPos, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendToDataServer( g_Packet, g_nPos );

	gPLAYERS.WaitDelData.push( pPlayer );
	pPlayer->isWaitDelData = 1;

	return 1;
}





//
// Function Name: DropItem
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_DropItem( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.dealVict || pPlayer->ch2.dealItems.GetSize() )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE pos = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem;

	if( !pos ) // 인벤토리에서 맵으로 Inventory to Map
	{
		pItem = FindItemFromInven( pPlayer, idxNum );

		if( pItem )
		{
			if( IS_SET( GET_ITEM_ATTR( pItem ) , dITEM_ATTR_NODROP ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[6] );
				return 1;
			}

#ifdef dDAUM_BILLING
			if( IS_BILLING_ITEM( pItem ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[224] );
				return 1;
			}
#endif

			if( GET_RIDE_STATE( pPlayer ) && 
				( pItem->itemNum == dRIDE_ITEM1 || pItem->itemNum == dRIDE_ITEM2 || pItem->itemNum == dRIDE_ITEM3 ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[7] );
				return 1;
			}

			ItemFromInventory( pPlayer, pItem );

			if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
				ItemToHouse( pItem, GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ) , GET_POSY( pPlayer ) );
			else
				ItemToMap( pItem, GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ) , GET_POSY( pPlayer ) );

#ifdef dUSE_ITEMLOG
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_DROP );
#endif
		}
	}
	else // 장비에서 맵으로
	{
		pItem = FindItemFromEquip( pPlayer, idxNum );

		if( pItem )
		{
			if( IS_SET( GET_ITEM_ATTR( pItem ) , dITEM_ATTR_NODROP ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[6] );
				return 1;
			}

			ItemFromEquip( pPlayer, pItem );
			
			if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
				ItemToHouse( pItem, GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ), GET_POSY( pPlayer ) );
			else
				ItemToMap( pItem, GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ), GET_POSY( pPlayer ) );

#ifdef dUSE_ITEMLOG
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_DROP );
#endif
		}
	}

	return 1;
}


//
// Function Name: GetItemProc
// Date: 2003-03-12
// Description: 
//
BYTE GetItemProc( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	sCHECK_INVEN checkInven;
	
	// 아이템이 포션인 경우 
	if( GetItemStackLimit(pItem) > 1 )
	{
		sPITEM_DATA pPotion = FindPotionItem( pPlayer, pItem );
		
		// 같은 종류의 포션이 존재하면 
		if( pPotion )
		{
			pPotion->exVal[0] = MIN( pPotion->exVal[0], 1 ) + MIN( pItem->exVal[0], 1 );
			SendUpdatePotionCnt( pPlayer, pPotion );
			GET_CURRITEM_WEIGHT( pPlayer ) += ( GET_ITEM_WEIGHT( pItem ) * MIN( pItem->exVal[0], 1 ) );
			SendWeightGageInfo( pPlayer );
			
			if( pItem->mapKind == dMAP_KIND_INDOOR )
				ItemFromHouse( pItem );
			else
				ItemFromMap( pItem );
			
			INSERT_ITEM_TO_MEMORY( pItem );
			
			SendSystemMsg( pPlayer, g_LANG_STR[10], GET_ITEM_HNAME( pItem ) );			
		}
		// 같은 종류의 포션이 존재하지 않을경우
		// 새로 넣는다 
		else
		{
			if( !CheckEmptyInven( pPlayer, &checkInven ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[11] );
				return 1;
			}
			if( pItem->mapKind == dMAP_KIND_INDOOR )
				ItemFromHouse( pItem );
			else
				ItemFromMap( pItem );
			ItemToInventory( pPlayer, pItem, &checkInven );
			
			SendSystemMsg( pPlayer, g_LANG_STR[10], GET_ITEM_HNAME( pItem ) );				
		}
	}
	else
	{
		if( !CheckEmptyInven( pPlayer, &checkInven ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[11] );
			return 1;
		}
		
		if( pItem->mapKind == dMAP_KIND_INDOOR )
			ItemFromHouse( pItem );
		else
			ItemFromMap( pItem );
		
		ItemToInventory( pPlayer, pItem, &checkInven );
		
		SendSystemMsg( pPlayer, g_LANG_STR[10], GET_ITEM_HNAME( pItem ) );
		
#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_GET );
#endif
	}
	return 1;
}

//
// Function Name: PACKET_GetItem
// Date: 2003-03-12
// Description: 
//
BOOL PACKET_GetItem( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem;
	
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		pItem = FindItemMyHouse( pPlayer, idxNum );
	else
		pItem = FindItemMyArea( pPlayer, idxNum );

	if( pItem )
	{
		int rangeX = abs( GET_POSX( pPlayer ) - pItem->mapX );
		int rangeY = abs( GET_POSY( pPlayer ) - pItem->mapY );
		
		int rangeValue = (int)sqrt( ( rangeX * rangeX ) + ( rangeY * rangeY ) );
		
		if( rangeValue >= 4 )
			return 1;

#ifdef USE_ITEM_WEIGHT
		if( GET_MAP_NUM( pPlayer ) != dDEAL_MAP_NUM )
		{
			if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION )
			{
				if( GET_CURRITEM_WEIGHT( pPlayer ) + ( GET_ITEM_WEIGHT( pItem ) * MIN( pItem->exVal[0], 1 ) ) > CalsAllowTotalWeight( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[180] );
					return 1;
				}
			}
			else
			{
				if( GET_CURRITEM_WEIGHT( pPlayer ) + GET_ITEM_WEIGHT( pItem ) > CalsAllowTotalWeight( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[180] );
					return 1;
				}
			}
		}
#endif

		// 줍지 못함
		if( IS_SET( GET_ITEM_ATTR( pItem ), dITEM_ATTR_NOGET ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[70] );
			return 1;
		}

		if( *pItem->ownerName != NULL_STR )
		{
			//3... ... ......
			/*if( pItem->ownerCheckTick + 15000 > g_CurrTime )
			{
				SendSystemMsg( pPlayer, "3sec of test" );
				return 1;
			}*/
			// 아직 소유권 체크 시간이 남았으면 
			if( pItem->ownerCheckTick + dOWNER_CHECK_TIME_LIMIT > g_CurrTime )
			{
				if( strcmp( pItem->ownerName, GET_NAME( pPlayer ) ) )
				{
					if( !GET_PARTY_MASTER( pPlayer ) )
					{	
						//SendSystemMsg( pPlayer, g_LANG_STR[8], pItem->ownerName );
						StrSendSysMsg( pPlayer, 8, pItem->ownerName );
						return 1;
					}
					else
					{
						if( strcmp( pItem->masterName, GET_NAME( GET_PARTY_MASTER( pPlayer ) ) ) )
						{
							//SendSystemMsg( pPlayer, g_LANG_STR[8], pItem->ownerName );
							StrSendSysMsg( pPlayer, 8, pItem->ownerName );
							return 1;
						}
					}
				}
				*pItem->ownerName = NULL_STR;
				pItem->ownerCheckTick = 0;
			}
			else
			{
				*pItem->ownerName = NULL_STR;
				pItem->ownerCheckTick = 0;
			}
		}

		// 버로우 , 스텔스
		UnSetVisibleSkill( pPlayer );
		//
		if( pItem->itemNum == dMONEY_NUM ) // 돈
		{
			if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pItem ) > dMAX_HAVE_MONEY )
				return 1;

			if( pItem->mapKind == dMAP_KIND_INDOOR )
				ItemFromHouse( pItem );
			else
				ItemFromMap( pItem );
			
			if( GET_PARTY_MASTER( pPlayer ) )
			{
				PARTY_MoneyShare( pPlayer, GET_MONEY_AMOUNT( pItem ) );
			}
			else
			{
				GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pItem );
				UpdateMoney( pPlayer );
				DATASERV_SendUpdateMoney( pPlayer );
				
				SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pItem ) );
			}

			//
			INSERT_ITEM_TO_MEMORY( pItem );
		}
		else
		{
			if( GET_PARTY_MASTER( pPlayer ) && pItem->isMobDrop )
			{
				sPDESC_DATA pShareOwner = PARTY_ItemShare( pPlayer );

				if( pShareOwner )
				{
					GetItemProc( pShareOwner, pItem );
					char tmp[128];

					g_nPos = 2;
					PutWord( g_Packet, dPACKET_SYSTEM_MSG, g_nPos );
					
					//한국어
					sprintf( tmp, "%s acquired the'%s' item.", GET_NAME( pShareOwner ), GET_ITEM_HNAME( pItem ) );
					
					PutString( g_Packet, tmp, g_nPos );
					PutSize( g_Packet, g_nPos );
					
					PARTY_SendToAll( pShareOwner, g_Packet, g_nPos, dSENDMODE_TOOTHER );
				}
				else
					GetItemProc( pPlayer, pItem );
			}
			else
				GetItemProc( pPlayer, pItem );
		}
	}
	return 1;
}


//
// Function Name: PACKET_SendShopItemList
// Date: 
// Description: 
//
BOOL PACKET_SendShopItemList( sPDESC_DATA pPlayer )
{
#ifndef dNON_PVP_MODE
	if( GET_CHA( pPlayer ) <= -25000 ) // 무법상태
	{
		SendSystemMsg( pPlayer, g_LANG_STR[12] );
		return 1;
	}
#endif

	WORD shopNum = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( !IsShopKeeperArea( pPlayer, shopNum ) )
		return 1;

	SHOP_SendItemList( pPlayer, shopNum );

	return 1;
}

//
// Function Name: PACKET_SellItem
// Date: 
// Description: 
//
BOOL PACKET_SellItem( sPDESC_DATA pPlayer )
{
	WORD shopNum = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE pos = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( !IsShopKeeperArea( pPlayer, shopNum ) )
		return 1;

	SHOP_SellItem( pPlayer, shopNum, idxNum , pos );

	return 1;
}

//
// Function Name: PACKET_BuyItem
// Date: 
// Description: 
//
BOOL PACKET_BuyItem( sPDESC_DATA pPlayer )
{
    WORD requestSize=0; memcpy(&requestSize,pPlayer->TemprecvBuff,2);
    if (pPlayer->recvPos+8!=requestSize) return 0;

	WORD shopNum = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	int itemNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	WORD cnt = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( !IsShopKeeperArea( pPlayer, shopNum ) )
		return 1;

	if (itemNum==12097 || itemNum==12098) {
        int neededItem=itemNum==12097?10193:10194, available=0;
        for(sPITEM_DATA card=pPlayer->inven;card;card=card->i_next)
            if(card->itemNum==neededItem) available+=GetItemStackCount(card);
        if(cnt<1 || available<cnt) return 1;
    }
    if (!SHOP_BuyItemWide( pPlayer, shopNum, itemNum , cnt )) return 1;

	if( itemNum == 12098 || itemNum == 12097 )
	{
		sPITEM_DATA pCardItem = NULL;
		if( itemNum == 12097 )
		{
			for( int i=0; i < cnt; i++ )
			{
				pCardItem = FindItemFromInvenByNum( pPlayer, 10193 );
				ConsumeOneEnchantCard( pPlayer, pCardItem );
			}
		}
		else if( itemNum == 12098 )
		{
			for( int i=0; i < cnt; i++ )
			{
				pCardItem = FindItemFromInvenByNum( pPlayer, 10194 );
				ConsumeOneEnchantCard( pPlayer, pCardItem );
			}
		}
	}

	return 1;
}


//
// Function Name: PACKET_EndPlayerAttack
// Date: 
// Description: 
//
BOOL PACKET_EndPlayerAttack( sPDESC_DATA pPlayer )
{
//	if( !pPlayer->enemyMob && !pPlayer->victPlayer )
//		return 1;

//	pPlayer->ch2.attackDelay = g_CurrTime + GetAttackDelay( pPlayer );
//	AttackToMob( pPlayer, pPlayer->enemyMob );

	return 1;
}


//
// Function Name: PACKET_CastItem
// Date: 
// Description: 
//
BOOL PACKET_CastItem( sPDESC_DATA pPlayer )
{
	return 1;
}

//
// Function Name: PACKET_DealRequest
// Date: 
// Description: 
//
BOOL PACKET_DealRequest( sPDESC_DATA pPlayer )
{
	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING )
		return 1;

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( pPlayer->ch2.dealVict )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pDealVict = NULL;
	
	// 
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pDealVict = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pDealVict = FindPlayerInArea( pPlayer->ch.mapNum, pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}

	if( !pDealVict )
		return 1;

	if( pDealVict->ch2.dealVict )
		return 1;

	if( pDealVict == pPlayer )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[71] );
		return 1;
	}

	if( IS_SET( pDealVict->ch2.flag, dPLAYER_FLG_DENY_DEAL ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[13] );
		return 1;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DEAL_BEGIN, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_IDX( pDealVict ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pDealVict );
	SendData( pPlayer );

	InitDealData( pPlayer );
	InitDealData( pDealVict );

	pDealVict->ch2.dealVict = pPlayer;
	pPlayer->ch2.dealVict = pDealVict;

	return 1;
}

//
// Function Name: PACKET_DealCancel
// Date: 
// Description: 
//
BOOL PACKET_DealCancel( sPDESC_DATA pPlayer )
{
	if( !pPlayer->ch2.dealVict )
	{
		//		log( "PACKET_DealCancel : !pPlayer->ch2.dealVict\r\n" );
		return 1;
	}

	sPDESC_DATA pDealVict = pPlayer->ch2.dealVict;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DEAL_FINISH, g_nPos );
	PutSize( g_Packet, g_nPos );

	//
	SendData( pPlayer );
	SendData( pDealVict );

	InitDealData( pPlayer );
	InitDealData( pDealVict );

	return 1;
}


//
// Function Name: PACKET_DealAddItem
// Date: 
// Description: 
//
BOOL PACKET_DealAddItem( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( !pPlayer->ch2.dealVict )
		return 1;

	if( pPlayer->ch2.dealVict->ch2.dealCheckIn )
	{
		StrSendSysMsg( pPlayer, 14, GET_NAME( pPlayer->ch2.dealVict ) );
		//SendSystemMsg( pPlayer, g_LANG_STR[14], GET_NAME( pPlayer->ch2.dealVict ) );
		return 1;
	}

	if( pPlayer->ch2.dealCheckIn )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[72] );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE posX = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE posY = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( FindItemFromDeal( pPlayer, idxNum ) )
		return 1;
	
	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );

	if( !pItem )
		return 1;

	if( GET_RIDE_STATE( pPlayer ) && 
		( pItem->itemNum == dRIDE_ITEM1 || pItem->itemNum == dRIDE_ITEM2 || pItem->itemNum == dRIDE_ITEM3 ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[15] );
		return 1;
	}

	if( IS_SET( GET_ITEM_ATTR( pItem ), dITEM_ATTR_NODEAL ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[16] );
		return 1;
	}

#ifndef dDAUM_BILLING
//	if( IS_BILLING_ITEM( pItem ) )
	if( pItem->itemNum == 11937 || pItem->itemNum == 11938 || pItem->itemNum == 11939 ||
		pItem->itemNum == 11940 || pItem->itemNum == 11941 || pItem->itemNum == 11942 ||
		pItem->itemNum == 11996	|| pItem->itemNum == 11995 )
	{
		SendSystemMsg( pPlayer, "You can't trade this item." );
		return 1;
	}
#endif

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DEAL_INSERT_ITEM, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutWord( g_Packet, pItem->itemNum, g_nPos );
	PutByte( g_Packet, posX, g_nPos );
	PutByte( g_Packet, posY, g_nPos );
	PutSize( g_Packet , g_nPos );

	SendData( pPlayer );
	SendData( pPlayer->ch2.dealVict );

	pPlayer->ch2.dealItems.push( pItem );

	return 1;
}


//
// Function Name: PACKET_DealDelItem
// Date: 
// Description: 
//
BOOL PACKET_DealDelItem( sPDESC_DATA pPlayer )
{
	SendSystemMsg( pPlayer, g_LANG_STR[225] );
	return 1;

/*	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( !pPlayer->ch2.dealVict )
		return 1;

	if( pPlayer->ch2.dealVict->ch2.dealCheckIn )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[18], GET_NAME( pPlayer->ch2.dealVict ) );
		return 1;
	}

	if( pPlayer->ch2.dealCheckIn )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[72] );
		return 1;
	}

	int idxNum = GetInteger( readBuff, readPos );

	sPITEM_DATA pItem = FindItemFromDeal( pPlayer, idxNum );

	if( !pItem )
		return 1;

	g_nPos = 2;
	PutHeader( g_Packet, dPACKET_DEAL_REMOVE_ITEM, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutInteger( g_Packet, pItem->idxNum , g_nPos );
	
	SendData( pPlayer );
	SendData( pPlayer->ch2.dealVict );

	pPlayer->ch2.dealItems.remove( pItem );*/

	return 1;
}


//
// Function Name: PACKET_DealAddMoney
// Date: 
// Description: 
//
BOOL PACKET_DealAddMoney( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.dealMoney )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[226] );
		return 1;
	}

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( !pPlayer->ch2.dealVict )
		return 1;

	if( pPlayer->ch2.dealVict->ch2.dealCheckIn )
	{
		StrSendSysMsg( pPlayer, 18, GET_NAME( pPlayer->ch2.dealVict ) );
		//SendSystemMsg( pPlayer, g_LANG_STR[18], GET_NAME( pPlayer->ch2.dealVict ) );
		return 1;
	}

	if( pPlayer->ch2.dealCheckIn )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[72] );
		return 1;
	}

	int amount = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( amount <= 0 )
		return 1;

	if( GET_MONEY( pPlayer ) < amount )
		return 1;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DEAL_UPDATEMONEY, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutInteger( g_Packet, amount, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer );
	SendData( pPlayer->ch2.dealVict );

	pPlayer->ch2.dealMoney = amount;

	return 1;
}


//
// Function Name: PACKET_DealCheckIn
// Date: 
// Description: 
//
BOOL PACKET_DealCheckIn( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	if( !pPlayer->ch2.dealVict )
		return 1;

	if( pPlayer->ch2.dealCheckIn )
		return 1;

	sPDESC_DATA pDealVict = pPlayer->ch2.dealVict;

	if( pPlayer->ch2.dealItems.GetSize() > CountEmptyInven( pDealVict ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[17] );
		return 1;
	}

	pPlayer->ch2.dealCheckIn = 1;

//	SendSystemMsg( pPlayer->ch2.dealVict, g_LANG_STR[18], GET_NAME( pPlayer ) );
	StrSendSysMsg( pPlayer, 18, GET_NAME( pPlayer->ch2.dealVict ) );
	SendSystemMsg( pPlayer, g_LANG_STR[73] );

	if( pPlayer->ch2.dealCheckIn && pDealVict->ch2.dealCheckIn )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_DEAL_REQUEST_SUBMIT, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );
		SendData( pDealVict );
	}
	return 1;
}


//
// Function Name: PACKET_DealSubmitOk
// Date: 
// Description: 
//
BOOL PACKET_DealSubmitOk( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	if( !pPlayer->ch2.dealVict )
		return 1;

	sPDESC_DATA pDealVict = pPlayer->ch2.dealVict;

	if( !CheckInvalidDealItem( pPlayer ) || !CheckInvalidDealItem( pDealVict ) )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_DEAL_FINISH, g_nPos );
		PutSize( g_Packet, g_nPos );

		//
		SendData( pPlayer );
		SendData( pDealVict );

		InitDealData( pPlayer );
		InitDealData( pDealVict );
		return 1;
	}


	if( CountEmptyInven( pPlayer ) < pDealVict->ch2.dealItems.GetSize() )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[19] );
		SendSystemMsg( pDealVict, g_LANG_STR[181] );
		return 1;
	}

	if( CountEmptyInven( pDealVict ) < pPlayer->ch2.dealItems.GetSize() )
	{
		SendSystemMsg( pDealVict, g_LANG_STR[19] );
		SendSystemMsg( pPlayer, g_LANG_STR[181] );
		return 1;
	}
	
#ifdef USE_ITEM_WEIGHT

	// 무게제한 
	if( GET_MAP_NUM( pPlayer ) != dDEAL_MAP_NUM )
	{
		if( GetDealItemsTotalWeight( pPlayer ) + GET_CURRITEM_WEIGHT( pDealVict ) > CalsAllowTotalWeight( pDealVict ) )
		{
			SendSystemMsg( pDealVict, g_LANG_STR[180] );
			SendSystemMsg( pPlayer, g_LANG_STR[182] );
			return 1;
		}
	}

	if( GET_MAP_NUM( pDealVict ) != dDEAL_MAP_NUM )
	{
		if( GetDealItemsTotalWeight( pDealVict ) + GET_CURRITEM_WEIGHT( pPlayer ) > CalsAllowTotalWeight( pPlayer ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[180] );
			SendSystemMsg( pDealVict, g_LANG_STR[182] );
			return 1;
		}
	}
	
#endif

	if( pPlayer->ch2.dealMoney < 0 )
		return 1;

	if( pDealVict->ch2.dealMoney < 0 )
		return 1;

	if( GET_MONEY( pPlayer ) < pPlayer->ch2.dealMoney )
		return 1;
	
	if( GET_MONEY( pDealVict ) < pDealVict->ch2.dealMoney )
		return 1;

	if( GET_MONEY( pPlayer ) + pDealVict->ch2.dealMoney > dMAX_HAVE_MONEY )
		return 1;

	if( GET_MONEY( pDealVict ) + pPlayer->ch2.dealMoney > dMAX_HAVE_MONEY )
		return 1;

	pPlayer->ch2.dealFinish = 1;

	sCHECK_INVEN checkInven;

	if( pPlayer->ch2.dealFinish && pDealVict->ch2.dealFinish )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_DEAL_FINISH, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );
		SendData( pDealVict );

		// 돈
		if( pPlayer->ch2.dealMoney > 0 )
		{
#ifdef dUSE_ITEMLOG
			if( pPlayer->ch2.dealMoney >= 1000000 )
				ITEMLOG_MoneyLog( pPlayer, pDealVict, pPlayer->ch2.dealMoney, MONEYLOG_ACT_DEAL_OUT );
#endif
			GET_MONEY( pPlayer ) -= pPlayer->ch2.dealMoney;
			UpdateMoney( pPlayer );
			DATASERV_SendUpdateMoney( pPlayer );

#ifdef dUSE_ITEMLOG
			if( pPlayer->ch2.dealMoney >= 1000000 )
				ITEMLOG_MoneyLog( pDealVict, pPlayer, pPlayer->ch2.dealMoney, MONEYLOG_ACT_DEAL_IN );
#endif
			GET_MONEY( pDealVict ) += pPlayer->ch2.dealMoney;
			UpdateMoney( pDealVict );
			DATASERV_SendUpdateMoney( pDealVict );
		}
		if( pDealVict->ch2.dealMoney > 0 )
		{
#ifdef dUSE_ITEMLOG
			if( pDealVict->ch2.dealMoney >= 1000000 )
				ITEMLOG_MoneyLog( pDealVict, pPlayer, pDealVict->ch2.dealMoney, MONEYLOG_ACT_DEAL_OUT );
#endif
			GET_MONEY( pDealVict ) -= pDealVict->ch2.dealMoney;
			UpdateMoney( pDealVict );
			DATASERV_SendUpdateMoney( pDealVict );

#ifdef dUSE_ITEMLOG
			if( pDealVict->ch2.dealMoney >= 1000000 )
				ITEMLOG_MoneyLog( pPlayer, pDealVict, pDealVict->ch2.dealMoney, MONEYLOG_ACT_DEAL_IN );
#endif
			GET_MONEY( pPlayer ) += pDealVict->ch2.dealMoney;
			UpdateMoney( pPlayer );
			DATASERV_SendUpdateMoney( pPlayer );
		}

		// 아이템 들
		sPLIST_BLOCK list, block, next_block;
		sPITEM_DATA pDealItem;

		// pPlayer ==> pDealVict
		list = pPlayer->ch2.dealItems.GetFirstBlock();

		LIST_WHILE( list, block, next_block, next, WMgr );

		pDealItem = (sPITEM_DATA)block->data;

		if( !CheckEmptyInven( pDealVict, &checkInven ) )
		{
//			log( "PACKET_DealSubmitOk : pDealVict inven is FULL [%s/%s]\r\n", GET_ID( pDealVict ), GET_NAME( pDealVict ) );
			return 1;
		}

		ItemFromInventory( pPlayer, pDealItem );
		ItemToInventory( pDealVict, pDealItem , &checkInven );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, pDealVict, pDealItem, ITEMLOG_ACT_DEALGIVE );
		ITEMLOG_ItemLog( pDealVict, pPlayer, pDealItem, ITEMLOG_ACT_DEALTAKE );
#endif
	
		LIST_WHILEEND( list, block, next_block );

		// pDealVict ==> pPlayer 
		list = pDealVict->ch2.dealItems.GetFirstBlock();

		LIST_WHILE( list, block, next_block, next, WMgr2 );
		
		pDealItem = (sPITEM_DATA)block->data;

		if( !CheckEmptyInven( pPlayer, &checkInven ) )
		{
//			log( "PACKET_DealSubmitOk : pPlayer inven is FULL [%s/%s]\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );
			return 1;
		}

		ItemFromInventory( pDealVict, pDealItem );
		ItemToInventory( pPlayer, pDealItem , &checkInven );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pDealVict, pPlayer, pDealItem, ITEMLOG_ACT_DEALGIVE );
		ITEMLOG_ItemLog( pPlayer, pDealVict, pDealItem, ITEMLOG_ACT_DEALTAKE );
#endif
		
		LIST_WHILEEND( list, block, next_block );

		// Init Deal Data 
		InitDealData( pPlayer );
		InitDealData( pDealVict );
	}

	return 1;
}


//
// Function Name: PACKET_PartyCreate
// Date: 
// Description: 
//
BOOL PACKET_PartyCreate( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	if( PARTY_Create( pPlayer ) )
	{
		g_nPos = 2; 
		PutWord( g_Packet, dPACKET_PARTY_CREATE_OK, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );
	}
	return 1;
}


//
// Function Name: PACKET_PartyDestroy
// Date: 
// Description: 
//
BOOL PACKET_PartyDestroy( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	PARTY_Destroy( pPlayer );
	return 1;
}


//
// Function Name: PACKET_PartyJoin
// Date: 
// Description: 
//
BOOL PACKET_PartyJoin( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pMaster;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pMaster = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pMaster = FindPlayerInArea( pPlayer->ch.mapNum, pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}

	if( !pMaster )
		return 1;

	if( GET_PARTY_MASTER( pPlayer ) )
		return 1;

	// 파티장이 아니면
	if( GET_PARTY_MASTER( pMaster ) != pMaster )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_PARTY_JOIN_RESULT, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );
		return 1;
	}

	// 파티가 꽉 차있으면
	if( pMaster->ch2.party.memberCnt >= 7 )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_PARTY_JOIN_RESULT, g_nPos );
		PutByte( g_Packet, 1, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );
		return 1;
	}

	PARTY_AddNewMember( pMaster, pPlayer );

	return 1;
}


//
// Function Name: PACKET_PartyOut
// Date: 
// Description: 
//
BOOL PACKET_PartyOut( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	if( !GET_PARTY_MASTER( pPlayer ) )
		return 1;

	if( GET_PARTY_MASTER( pPlayer ) == pPlayer )
		return 1;

	PARTY_OutMember( pPlayer );

	return 1;
}


//
// Function Name: PACKET_PartyRemoveMember
// Date: 
// Description: 
//
BOOL PACKET_PartyRemoveMember( sPDESC_DATA pPlayer )
{
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( !GET_PARTY_MASTER( pPlayer ) )
		return 1;

	if( GET_PARTY_MASTER( pPlayer ) != pPlayer )
		return 1;

	if( idxNum == GET_IDX( pPlayer ) )
		return 1;

	sPDESC_DATA pOutMember = PARTY_FindMember( pPlayer, idxNum );

	if( !pOutMember )
		return 1;

	PARTY_OutMember( pOutMember );

	return 1;
}

//
// Function Name: PACKET_MagicCast
// Date: 
// Description: 
//
BOOL PACKET_MagicCast( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE target = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	SKILL_UseSkill( pPlayer, idxNum, target, GET_USE_SKILL( pPlayer ) );

	return 1;
}




//
// Function Name: PACKET_ItemDBClick
// Date: 
// Description: 
//
BOOL PACKET_ItemDBClick( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.dealVict || pPlayer->ch2.dealItems.GetSize() )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE pos = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

#ifndef dDAUM_BILLING
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;
#else
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE && pos )
		return 1;
#endif

	sPITEM_DATA pItem;
	sCHECK_INVEN checkInven;

	if( !pos ) // 인벤토리
	{
		pItem = FindItemFromInven( pPlayer, idxNum );

		if( pItem )
		{
#ifdef dDAUM_BILLING
			if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE &&
				pItem->itemNum != dBILL_ITEM_NUM22 &&
				pItem->itemNum != dBILL_ITEM_NUM23 &&
				pItem->itemNum != dBILL_ITEM_NUM24 
				)
				return 1;			
#endif

			BYTE equipWhere = GetEquipPositionByNum( pItem->itemNum );

			if( equipWhere == 255 ) // 장착하는 아이템 아님
			{
				switch( GET_ITEM_TYPE( pItem ) )
				{
				case dITEMTYPE_MAGICSCROLL:					
					if( AddPlayerSkill( pPlayer, pItem ) )
					{
						ItemFromInventory( pPlayer, pItem );	
						INSERT_ITEM_TO_MEMORY( pItem );
					}
					break;
				case dITEMTYPE_POTION:
					switch( pItem->itemNum )
					{
					case dMP_POSION_L: // MP 포션
					case dMP_POSION_M: // MP 포션
					case dMP_POSION_S: // MP 포션
						UseMPPotion( pPlayer, pItem->itemNum );
						break;
					case dHP_POSION_L: // HP 포션
					case dHP_POSION_M: // HP 포션
					case dHP_POSION_S: // HP 포션
						UseHPPotion( pPlayer, pItem->itemNum );
						break;
#ifdef dDAUM_BILLING	
					default:
						UseOtherPotion( pPlayer, pItem );
						break;
#endif
					}
					break;
				// 일반 스크롤
				case dITEMTYPE_SCROLL:

					if( CastItem( pPlayer, pItem ) )
					{
						ItemFromInventory( pPlayer, pItem );
						INSERT_ITEM_TO_MEMORY( pItem );
					}

					break;
				// 일회용 
				case dITEMTYPE_ONETIME:
					UseOtherOneTimeItem( pPlayer, pItem );
					break;
				default:

					EventItemProcess( pPlayer, pItem );

					break;
				}
				return 1;
			}
			else
			{
				if( !pPlayer->isAdmin )
				{
					if( !IsEquipOk( pPlayer, pItem ) )
						return 1;
				}

				// 
				ChangeEquipItem( pPlayer, pItem, equipWhere );
			}
		}
	}
	else	// 장착창 
	{
		pItem = FindItemFromEquip( pPlayer, idxNum );

		if( pItem )
		{
		//	if( !IsEquipOk( pPlayer, pItem ) )
		//		return 1;

			if( !CheckEmptyInven( pPlayer, &checkInven ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[11] );
				return 1;
			}
			ItemFromEquip( pPlayer, pItem );
			ItemToInventory( pPlayer, pItem, &checkInven );
		}
	}
	return 1;
}




//
// Function Name: PACKET_UseQuickItem
// Date: 
// Description: 
//
BOOL PACKET_UseQuickItem( sPDESC_DATA pPlayer )
{
	WORD itemNum = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( !IS_VALID_ITEM( itemNum ) )
		return 1;

	if( itemNum == dSATTACK_SKILL_ITEM )
	{
		if( pPlayer->ch2.actionDelay > g_CurrTime )
			return 1;

		GET_USE_SKILL( pPlayer ) = dSATTACK_SKILL_ITEM;
		SendPlayerMainSkill( pPlayer );
		SendPlayerAttackRange( pPlayer );
		return 1;
	}

	if( itemNum == dLATTACK_SKILL_ITEM )
	{
		if( pPlayer->ch2.actionDelay > g_CurrTime )
			return 1;

		GET_USE_SKILL( pPlayer ) = dLATTACK_SKILL_ITEM;
		SendPlayerMainSkill( pPlayer );
		SendPlayerAttackRange( pPlayer );
		return 1;
	}

	switch( GET_ITEM_TYPE2( itemNum ) )
	{
	// 마법 스크롤 
	case dITEMTYPE_MAGICSCROLL:
		{
			switch( itemNum )
			{
			case dDENY_ITEM_FRIEND:		// 친구차단
			case dDENY_ITEM_CHAT:		// 채팅차단
			case dDENY_ITEM_WHISPER:	// 귓말차단
			case dDENY_ITEM_PARTY:		// 파티차단
			case dDENY_ITEM_DEAL:		// 거래차단
				TogglePlayerFlag( pPlayer, itemNum );
				return 1;
			}

			sPSKILL_DATA pSkill = g_SKILL[g_ItemInfo[itemNum]->skillIdx];
			
			if( !pSkill )
				return 1;
			
			if( IS_SATTACK( pSkill ) || IS_LATTACK( pSkill ) )
				return 1;

			if( !IsHaveSkill( pPlayer, itemNum ) )
				return 1;
			
			GET_USE_SKILL( pPlayer ) = itemNum;
			SendPlayerMainSkill( pPlayer );
			SendPlayerAttackRange( pPlayer );
			MAGIC_SendMagicInfo( pPlayer );
		}
		break;
	// 일반 스크롤 
	case dITEMTYPE_SCROLL:
		{
			sPITEM_DATA pItem = FindItemFromInvenByNum( pPlayer, itemNum );

			if( pItem )
			{
				if( CastItem( pPlayer, pItem ) )
				{
					ItemFromInventory( pPlayer, pItem );
					INSERT_ITEM_TO_MEMORY( pItem );
					return 1;
				}
			}
		}
		break;
	// 포션
	case dITEMTYPE_POTION:
		switch( itemNum )
		{
		case dMP_POSION_L: // MP 포션
		case dMP_POSION_M: // MP 포션
		case dMP_POSION_S: // MP 포션
			UseMPPotion( pPlayer, itemNum );
			break;
		case dHP_POSION_L: // HP 포션
		case dHP_POSION_M: // HP 포션
		case dHP_POSION_S: // HP 포션
			UseHPPotion( pPlayer, itemNum );
			break;
		}
		break;
	// 일회용 아이템 
	case dITEMTYPE_ONETIME:
		break;
	// 음식
	case dITEMTYPE_FOOD:
		break;
	default:
		{
			// 퀵슬롯을 통해 장착 아이템 갈아 치우기
			BYTE equipWhere = GetEquipPositionByNum( itemNum );
			
			if( equipWhere != 255 )
			{
				sPITEM_DATA pItem = FindItemFromInvenByNum( pPlayer, itemNum );
				
				if( pItem )
				{
					ChangeEquipItem( pPlayer, pItem, equipWhere );
				}
			}
		}
		break;
	}
	
	return 1;
}

//
// Function Name: PACKET_QuestRequest
// Date: 
// Description: 
//
BOOL PACKET_QuestRequest( sPDESC_DATA pPlayer )
{
/*	WORD wNpcNum = GetWord( readBuff, readPos );
	WORD wQuestNum = GetWord( readBuff, readPos );

	QUEST_Request( pPlayer, wQuestNum , wNpcNum );*/

	return 1;
}

//
// Function Name: PACKET_DoorEvent
// Date: 
// Description: 
//
BOOL PACKET_DoorEvent( sPDESC_DATA pPlayer )
{
	int wDoorID = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING &&
		GET_STATE( pPlayer ) != dDESC_STATE_FIGHT )
		return 1;
	
	// 살아있는 케릭터가 아니면
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	// 내부에서가 아니라면
	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR )
	{
		int hNum = FindHouse( GET_MAP_NUM( pPlayer ), wDoorID );
		
		if( hNum == -1 )
		{
			return 1;
		}
		
		pPlayer->ch.prevMapNum = GET_MAP_NUM( pPlayer );
		pPlayer->ch.prevPosX = GET_POSX( pPlayer );
		pPlayer->ch.prevPosY = GET_POSY( pPlayer );

		pPlayer->ch2.gotoMapNum = hNum;
		pPlayer->ch2.gotoPosX = gHOUSE[hNum]->targetX;
		pPlayer->ch2.gotoPosY = gHOUSE[hNum]->targetY;
		pPlayer->ch2.gotoPosMapKind = dMAP_KIND_INDOOR;
		
		PlayerFromMap( pPlayer );
		
		SendMapLoading( pPlayer );
	}
	else if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		if( !gHOUSE[GET_MAP_NUM(pPlayer)]->linkMap )
			return 1;
		
		if( IS_VALID_WORLD( pPlayer->ch.prevMapNum ) )
		{
			pPlayer->ch2.gotoMapNum = pPlayer->ch.prevMapNum;
			pPlayer->ch2.gotoPosX = pPlayer->ch.prevPosX;
			pPlayer->ch2.gotoPosY = pPlayer->ch.prevPosY;
			pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
		}
		else
			PlayerToHomeTown( pPlayer );
		
		PlayerFromIndoor( pPlayer );
		SendMapLoading( pPlayer );
	}
	
	return 1;
}


//
// Function Name: PACKET_OpenGarbage
// Date: 
// Description: 
//
BOOL PACKET_OpenGarbage( sPDESC_DATA pPlayer )
{
	if( !IsGarbageArea( pPlayer ) )
		return 1;

	g_nPos = 2;

	PutWord( g_Packet, dPACKET_GARBAGE_ITEMLIST, g_nPos );
	PutWord( g_Packet, pPlayer->garbageCnt, g_nPos );

	sPITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->garbage, item, next_item, i_next, WMgr );

	PutWord( g_Packet, item->itemNum, g_nPos );
	PutInteger( g_Packet, item->idxNum, g_nPos );
	PutWord( g_Packet, GetItemStackCount(item), g_nPos );

	LIST_WHILEEND( pPlayer->garbage, item, next_item );

	PutInteger( g_Packet, pPlayer->garbageMoney, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_InputGarbageItem
// Date: 
// Description: 
//
BOOL PACKET_InputGarbageItem( sPDESC_DATA pPlayer )
{
	if( !IsGarbageArea( pPlayer ) )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( pPlayer->garbageCnt >= dMAX_GARBAGE_CNT )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[74] );
		return 1;
	}

	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );

	if( pItem )
	{
		if( pItem->itemNum == dMAY_BOX_ITEM )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[20] );
			return 1;
		}

		if( GET_RIDE_STATE( pPlayer ) && 
			( pItem->itemNum == dRIDE_ITEM1 || pItem->itemNum == dRIDE_ITEM2 || pItem->itemNum == dRIDE_ITEM3 ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[128] );
			return 1;
		}

		ItemFromInventory( pPlayer, pItem );
		ItemToGarbage( pPlayer, pItem );	

#ifdef dUSE_ITEMLOG
		ITEMLOG_GarbageLog( pPlayer, pItem, GARBAGE_ACT_PUT );
#endif
	}

	return 1;
}


//
// Function Name: PACKET_OutputGarbageItem
// Date: 
// Description: 
//
BOOL PACKET_OutputGarbageItem( sPDESC_DATA pPlayer )
{
	if( !IsGarbageArea( pPlayer ) )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem = FindItemFromGarbage( pPlayer, idxNum );

	if( pItem )
	{
#ifdef USE_ITEM_TAX
		if( GET_MONEY( pPlayer ) < 50 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[157], 50 );
			return 1;
		}
#endif

		sCHECK_INVEN checkInven;
		
		if( !CheckEmptyInven( pPlayer, &checkInven ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[19] );
			return 1;
		}

#ifdef USE_ITEM_WEIGHT
		if( GET_MAP_NUM( pPlayer ) != dDEAL_MAP_NUM )
		{
			if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION )
			{
				if( GET_CURRITEM_WEIGHT( pPlayer ) + ( GET_ITEM_WEIGHT( pItem ) * MIN( pItem->exVal[0], 1 ) ) > CalsAllowTotalWeight( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[180] );
					return 1;
				}
			}
			else
			{
				if( GET_CURRITEM_WEIGHT( pPlayer ) + GET_ITEM_WEIGHT( pItem ) > CalsAllowTotalWeight( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[180] );
					return 1;
				}
			}
		}
#endif

		ItemFromGarbage( pPlayer, pItem );
		ItemToInventory( pPlayer, pItem , &checkInven );

#ifdef dUSE_ITEMLOG
		ITEMLOG_GarbageLog( pPlayer, pItem, GARBAGE_ACT_GET );
#endif

#ifdef USE_ITEM_TAX
		GET_MONEY( pPlayer ) -= 50;
		UpdateMoney( pPlayer );
		DATASERV_SendUpdateMoney( pPlayer );
#endif
	}
	return 1;
}

//
// Function Name: PACKET_BeginCastMagic
// Date: 
// Description: 
//
BOOL PACKET_BeginCastMagic( sPDESC_DATA pPlayer )
{
	if( GET_RIDE_STATE( pPlayer ) )
		return 1;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		return 1;

	if( GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) )
		return 1;

	if( !IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) ||
		GET_ITEM_TYPE2( GET_USE_SKILL( pPlayer ) ) != dITEMTYPE_MAGICSCROLL )
		return 1;
	
	if( GET_MP( pPlayer ) < MAGIC_GetNeedMp( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[1] );
		return 1;
	}

	int targetIDX = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE targetType = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPSKILL_DATA pSkill = GET_SKILL_DATA( pPlayer );
	
	if( pSkill )
	{
		if( pSkill->coolTimeIdx )
		{
			if( pPlayer->ch2.skillCoolTime[pSkill->coolTimeIdx] > g_CurrTime )
				return 1;
		}
	}
	else
		return 1;
	
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_BEGIN_CASTMAGIC2, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, GET_USE_SKILL( pPlayer ), g_nPos );
	PutInteger( g_Packet, targetIDX, g_nPos );
	PutByte( g_Packet, targetType, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToRange( pPlayer, g_Packet, g_nPos, dATTACK_PACKET_RANGE );

	return 1;
}

//
// Function Name: PACKET_MoveInvenItem
// Date: 
// Description: 
//
BOOL PACKET_MoveInvenItem( sPDESC_DATA pPlayer )
{
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE page = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE invenX = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE invenY = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );


	if( !pItem )
		return 1;
	
	// 해당 자리에 아이템이 없을 경우
	if( !pPlayer->invenArray[page][invenY][invenX] )
	{
		ItemFromInventory( pPlayer, pItem );
		sCHECK_INVEN checkInven;
		checkInven.page = page;
		checkInven.x = invenX;
		checkInven.y = invenY;
		ItemToInventory( pPlayer, pItem, &checkInven );
	}
	else
	{
		sPITEM_DATA pTarget = FindItemFromInvenByPos( pPlayer, page, invenX, invenY );

		if( !pTarget )
			return 1;

		if( pItem == pTarget )
			return 1;
	
		// 아이템이 포션인 경우 
		if( GetItemStackLimit(pItem) > 1 && pTarget->itemNum == pItem->itemNum )
		{
			// 겹칠 수 없는 경우
			if( pTarget->exVal[0] >= GetItemStackLimit(pTarget) )
			{
				sCHECK_INVEN srcInven;
				sCHECK_INVEN targetInven;
				
				srcInven.page = pItem->invenPage;
				srcInven.x = pItem->invenX;
				srcInven.y = pItem->invenY;
				
				targetInven.page = pTarget->invenPage;
				targetInven.x = pTarget->invenX;
				targetInven.y = pTarget->invenY;
				
				ItemFromInventory( pPlayer, pItem );
				ItemFromInventory( pPlayer, pTarget );
				ItemToInventory( pPlayer, pItem, &targetInven );
				ItemToInventory( pPlayer, pTarget, &srcInven );
				return 1;
			}

			if( GetItemStackCount(pTarget) + GetItemStackCount(pItem) <= GetItemStackLimit(pTarget) )
			{
				pTarget->exVal[0] = MIN( pTarget->exVal[0], 1 ) + MIN( pItem->exVal[0], 1 );
				int itemWeight = GetItemWeight( pItem );
				ItemFromInventory( pPlayer, pItem );
				INSERT_ITEM_TO_MEMORY( pItem );
				GET_CURRITEM_WEIGHT( pPlayer ) += itemWeight;
				SendWeightGageInfo( pPlayer );
				SendUpdatePotionCnt( pPlayer, pTarget );
			}
			else
			{
				int tmpVal = GetItemStackCount(pTarget) + GetItemStackCount(pItem) - GetItemStackLimit(pTarget);
				pTarget->exVal[0] = GetItemStackLimit(pTarget);
				pItem->exVal[0] = MIN( tmpVal, 1 );
				SendUpdatePotionCnt( pPlayer, pTarget );
				SendUpdatePotionCnt( pPlayer, pItem );
			}
		}
		else
		{
			sCHECK_INVEN srcInven;
			sCHECK_INVEN targetInven;

			srcInven.page = pItem->invenPage;
			srcInven.x = pItem->invenX;
			srcInven.y = pItem->invenY;
			
			targetInven.page = pTarget->invenPage;
			targetInven.x = pTarget->invenX;
			targetInven.y = pTarget->invenY;

			ItemFromInventory( pPlayer, pItem );
			ItemFromInventory( pPlayer, pTarget );
			ItemToInventory( pPlayer, pItem, &targetInven );
			ItemToInventory( pPlayer, pTarget, &srcInven );
		}
	}

	return 1;
}

//
// Function Name: PACKET_AddPartyBoard
// Date: 
// Description: 
//
BOOL PACKET_AddPartyBoard( sPDESC_DATA pPlayer )
{
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[21] );
		return 1;
	}

	if( GET_PARTY_MASTER( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[22] );
		return 1;
	}

	if( FindPartyBoardArticle( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[23] );
		return 1;
	}

	sPWORLD_PARTY_BOARD pArticle = NewPartyBoardArticle();

	if( !pArticle )
		return 1;

	pArticle->next = NULL;
	pArticle->prev = NULL;
	pArticle->pPlayer = pPlayer;
	pArticle->time = time(NULL);

	INSERT_TO_LIST( gWORLD[GET_MAP_NUM( pPlayer )]->partyBoard, pArticle, prev, next );

	SendSystemMsg( pPlayer, g_LANG_STR[24] );

	return 1;
}

//
// Function Name: PACKET_GetPartyBoard
// Date: 
// Description: 
//
BOOL PACKET_GetPartyBoard( sPDESC_DATA pPlayer )
{
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[21] );
		return 1;
	}

	WORD page = GetWord( pPlayer->TemprecvBuff , pPlayer->recvPos );

	int cnt = 0;
	WORD containCnt = 0;

	sPWORLD_PARTY_BOARD board, next_board;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTYBOARD_LIST, g_nPos );
	g_nPos += 2;

	LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->partyBoard, board, next_board, next, WMgr );

	cnt++;

	if( cnt > ( MIN( page, 1 ) * 10) )
		break;

	PutString( g_Packet, board->pPlayer->ch.name, g_nPos );
	PutInteger( g_Packet, GET_ALL_SKILL( board->pPlayer ), g_nPos );
	PutInteger( g_Packet, board->pPlayer->ch.skill[0], g_nPos );
	PutInteger( g_Packet, board->pPlayer->ch.skill[1], g_nPos );
	PutInteger( g_Packet, board->pPlayer->ch.skill[2], g_nPos );
	PutInteger( g_Packet, board->pPlayer->ch.skill[3], g_nPos );

	containCnt++;

	LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->partyBoard, board, next_board );

	*(WORD*)(g_Packet + 4 ) = containCnt;

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_AttackPlayer
// Date: 
// Description: 
//
BOOL PACKET_AttackPlayer( sPDESC_DATA pPlayer )
{
#ifndef dNON_PVP_MODE
	if( !IsPvPMode( pPlayer ) )
		return 1;

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		return 1;

	if( pPlayer->ch2.actionDelay > g_CurrTime )
		return 1;

	if( !IsPvPArea( pPlayer ) )
		return 1;

	if( !IsCanAttackWeight( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[179] );
		return 1;
	}

#ifndef dNON_PVP_MODE
	if( GET_ALL_SKILL( pPlayer ) <= 25 )
	{
		if( !gWORLD[GET_MAP_NUM( pPlayer )]->underAttack )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[134] );
			return 1;
		}
	}
#endif
	
	// 마법은 Attack 메시지 사용 않함
	if( GET_USE_SKILL( pPlayer ) != dSATTACK_SKILL_ITEM &&
		GET_USE_SKILL( pPlayer ) != dLATTACK_SKILL_ITEM )
		return 1;

	int idxNum;

	idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pVict = NULL;

#ifdef dAUTO_ATTACK
	if( pPlayer->victPlayer )
	{
		if( GET_IDX( pPlayer->victPlayer ) == idxNum )
			return 1;
	}

	pVict = FindPlayerInArea( GET_MAP_NUM( pPlayer ),
						GET_POSX( pPlayer )/dONE_TILE, GET_POSY( pPlayer )/dONE_TILE, idxNum );
#else
	if( pPlayer->victPlayer )
	{
		if( GET_IDX( pPlayer->victPlayer ) == idxNum )
			pVict = pPlayer->victPlayer;
	}

	if( !pVict )
	{
		pVict = FindPlayerInArea( GET_MAP_NUM( pPlayer ),
			GET_POSX( pPlayer )/dONE_TILE, GET_POSY( pPlayer )/dONE_TILE, idxNum );
	}

#endif

	if( !pVict )
		return 1;

	if( !IsValidPvPTarget( pPlayer, pVict ) )
		return 1;

	if( GET_DEAD_STATE( pVict ) != dDEAD_STATE_NONE )
		return 1;
	
	if( CheckMyTeam( pPlayer, pVict ) )
		return 1;

#ifndef dNON_PVP_MODE
	if( GET_ALL_SKILL( pVict ) <= 25 )
	{
		if( !gWORLD[GET_MAP_NUM( pVict )]->underAttack )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[135] );
			return 1;
		}
	}

	if( !pPlayer->victPlayer && pVict->victPlayer != pPlayer )
	{
		pPlayer->ch2.isEvilCha = 1;
		pPlayer->ch2.evilChaTime = g_CurrRealTime;
		
		sNAME_COLOR nameColor;
		
		GetNameColor( dNAMECOLOR_EVILCHA, &nameColor );
		SendChangeNameColor( pPlayer, &nameColor );
	}		

#endif

	/*if( CheckEquipItem( pVict, dEQUIP_NECK, dAMULET_INVISIBILITY )	)
	{
		SendSystemMsg( pPlayer, "%sYou are wearing the Invisibility Amulet.", GET_NAME( pVict ) );
		return 1;
	}*/

	set_pvp_fight( pPlayer, pVict );
#endif // END of dNON_PVP_MODE
	return 1;
}


//
// Function Name: PACKET_BroadCastChat
// Date: 
// Description: 
//
BOOL PACKET_BroadCastChat( sPDESC_DATA pPlayer )
{
//#ifndef __CHINA_VER__
	//return 1;
//#endif
	// time check
	if( pPlayer->chatEndTime > g_CurrTime )
		return 1;
	else
		pPlayer->chatEndTime = g_CurrTime + dCHAT_DELAY_TIME;

	char str[1024];

	GetString( pPlayer->TemprecvBuff, str, pPlayer->recvPos );
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_BROADCAST_CHAT, g_nPos );
	PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
	PutString( g_Packet, str, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	// 실내일 경우
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		SendToHouse( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
	}
	else
	{
		sPDESC_DATA player, next_player;

		LIST_WHILE( gWORLD[GET_MAP_NUM(pPlayer)]->pChatList, player, next_player, c_next, WMgr );
		SendData( player, g_Packet, g_nPos );
		LIST_WHILEEND( gWORLD[GET_MAP_NUM(pPlayer)]->pChatList, player, next_player );
	}
	return 1;
}

//
// Function Name: PACKET_SellingChat
// Date: 
// Description: 
//
BOOL PACKET_SellingChat( sPDESC_DATA pPlayer )
{
/*#ifndef __CHINA_VER__
	return 1;
#endif*/
	char str[1024];
	
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;
	
	if( pPlayer->ch2.sellingChatDelay + 5 > g_CurrRealTime )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[155], ( pPlayer->ch2.sellingChatDelay + 5 ) - g_CurrRealTime );
		return 1;
	}
	
	/*if( GET_ALL_SKILL( pPlayer ) < 20 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[156], 20 );
		return 1;
	}*/
	
	if( GET_MONEY( pPlayer ) < 1000 )	//Trading chat price
	{
		SendSystemMsg( pPlayer, g_LANG_STR[157], 1000 );
		return 1;
	}
	
	GetString( pPlayer->TemprecvBuff, str, pPlayer->recvPos );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SELLING_CHAT, g_nPos );
	PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
	PutString( g_Packet, str, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	sPDESC_DATA d, next_d;
	
	LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
	
	if( GET_STATE( d ) == dDESC_STATE_FIGHT ||
		GET_STATE( d ) == dDESC_STATE_PLAYING )
	{
		SendData( d, g_Packet, g_nPos );
	}
	
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
	
	GET_MONEY( pPlayer ) -= 1000;
	DATASERV_SendUpdateMoney( pPlayer );
	UpdateMoney( pPlayer );
	SendSystemMsg( pPlayer, g_LANG_STR[275], 1000 );
	
	pPlayer->ch2.sellingChatDelay = g_CurrRealTime;
	return 1;
}

//
// Function Name: PACKET_GuildChat
// Date: 
// Description: 
//
BOOL PACKET_GuildChat( sPDESC_DATA pPlayer )
{
	if( !GET_GUILD( pPlayer ) )
		return 1;

	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return 1;
	
	// time check
	if( pPlayer->chatEndTime > g_CurrTime )
		return 1;
	else
		pPlayer->chatEndTime = g_CurrTime + dCHAT_DELAY_TIME;

	char str[256];

	GetString( pPlayer->TemprecvBuff, str, pPlayer->recvPos );

	// 길드공지
	if( GET_GUILD_LEVEL( pPlayer ) == dGUILD_LEVEL_MASTER )
	{
		if( strlen( str ) < dMAX_GUILD_NOTICE_LENGTH && *str == '&' )
		{
			sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );
			
			if( pGuild )
			{
				if( strlen( str ) == 1 )
				{
					*pGuild->notice = NULL_STR;
					SendSystemMsg( pPlayer, g_LANG_STR[158] );
					return 1;
				}

#ifdef dDAUM_BILLING
				BYTE haveNoticeItem = 0;

				if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM25 ) )
				{
					haveNoticeItem = 1;
				}

				if( !haveNoticeItem )
				{
					if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM26 ) )
					{
						haveNoticeItem = 1;
					}
				}

				if( !haveNoticeItem )
				{
					if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM27 ) )
					{
						haveNoticeItem = 1;
					}
				}

				if( !haveNoticeItem )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[227] );
					return 1;
				}
#endif

				strcpy( pGuild->notice, &str[1] );
				
				g_nPos = 2;
				PutWord( g_Packet, dPACKET_GUILD_CHAT, g_nPos );
				PutString( g_Packet, "NOTICE", g_nPos );
				PutString( g_Packet, pGuild->notice, g_nPos );
				PutSize( g_Packet, g_nPos );

				GUILD_SendToMember( pGuild, g_Packet, g_nPos );

				SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL7 );
				return 1;
			}
		}
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_GUILD_CHAT, g_nPos );
	PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
	PutString( g_Packet, str, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

	GUILD_SendToMember( pGuild, g_Packet, g_nPos );

	return 1;
}


//
// Function Name: PACKET_PartyChat
// Date: 
// Description: 
//
BOOL PACKET_PartyChat( sPDESC_DATA pPlayer )
{
	if( !GET_PARTY_MASTER( pPlayer ) )
		return 1;
	
	// time check
	if( pPlayer->chatEndTime > g_CurrTime )
		return 1;
	else
		pPlayer->chatEndTime = g_CurrTime + dCHAT_DELAY_TIME;

	char msg[1024];

	GetString( pPlayer->TemprecvBuff, msg, pPlayer->recvPos );

	g_nPos = 2;

	PutWord( g_Packet, dPACKET_PARTY_CHAT, g_nPos );
	PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
	PutString( g_Packet, msg, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	PARTY_SendToAll( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );

	return 1;
}


//
// Function Name: PAKCET_MemberChat
// Date: 
// Description: 
//
BOOL PACKET_MemberChat( sPDESC_DATA pPlayer )
{	
	// time check
	if( pPlayer->chatEndTime > g_CurrTime )
		return 1;
	else
		pPlayer->chatEndTime = g_CurrTime + dCHAT_DELAY_TIME;

	return 1;
}


//
// Function Name: PAKCET_MemberChat
// Date: 
// Description: 
//
BOOL PACKET_SmithEnchat( sPDESC_DATA pPlayer )
{
	int		itemIdx;
	int		cardIdx;

	itemIdx = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	cardIdx = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPITEM_DATA pItem = NULL;
	sPITEM_DATA pCardItem = NULL;
	
	pItem = FindItemFromInven( pPlayer, itemIdx );
	pCardItem = FindItemFromInven( pPlayer, cardIdx );
	
	if( !pItem || !pCardItem )
	{
		return 1;
	}

	if( pCardItem->itemNum != 12098 && pCardItem->itemNum != 12097 )
	{
		return 1;
	}

	int enchantNum = pItem->itemNum+1;

	if( !IS_VALID_ITEM( enchantNum ) )
	{
		return 1;
	}

	int cardNum = 0;
	int ItemInfor = g_ItemInfo[pItem->itemNum]->isUniq;
	// 카드와 아이템 확인
	if( pCardItem->itemNum == 12098 )
	{
		cardNum = 12098;
		if( !IS_ARMOR( GET_ITEM_TYPE( pItem ) ) )
		{
			return 1;
		}
		ItemInfor = ItemInfor+2;
	}
	else if( pCardItem->itemNum == 12097 )
	{
		cardNum = 12097;
		if( !IS_WEAPON( GET_ITEM_TYPE( pItem ) ) )
		{
			return 1;
		}
	}

	int payCard=0;
	int payMoney=0;

	//카드 장수와 머니 계산
	switch( ItemInfor )
	{
	case 5:
		if(  ( pCardItem->exVal[0] >= 5 ) && ( GET_MONEY( pPlayer ) >= 30000 ) )
		{
			if( cardNum == 12098 )
			{
				if( pCardItem->exVal[0] < 5 )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[277] );
					return 1;
				}
				payCard = 5;
				payMoney = 30000;
				break;
			}
			payCard = 5;
			payMoney = 40000;
		}	
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[278] );
			return 1;
		}
		break;
	case 6:
		if(  ( pCardItem->exVal[0] >= 10 ) && ( GET_MONEY( pPlayer ) >= 60000 ) )
		{
			if( cardNum == 12098 )
			{
				if( pCardItem->exVal[0] < 10 )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[277] );
					return 1;
				}
				payCard = 10;
				payMoney = 60000;
				break;
			}
			payCard = 10;
	  	    payMoney = 50000;
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[278] );
			return 1;
		}
		break;
	case 7:
		if(  ( pCardItem->exVal[0] >= 15 ) && ( GET_MONEY( pPlayer ) >= 60000 ) )
		{
			if( cardNum == 12098 )
			{
				if(  GET_MONEY( pPlayer ) < 120000 )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[277] );
					return 1;
				}
				payCard = 15;
				payMoney = 120000;
				break;
			}
			payCard = 15;
			payMoney = 60000;
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[278] );
			return 1;
		}
		break;
	case 8:
		if(  ( pCardItem->exVal[0] >= 20 ) && ( GET_MONEY( pPlayer ) >= 80000 ) )
		{
			if( cardNum == 12098 )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[279] );
				return 1;
			}
			payCard = 20;
			payMoney = 80000;
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[278] );
			return 1;
		}
		break;
	case 9:
		if(  ( pCardItem->exVal[0] >= 30 ) && ( GET_MONEY( pPlayer ) >= 100000 ) )
		{
			if( cardNum == 12098 )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[279] );
				return 1;
			}
			payCard = 30;
			payMoney = 100000;
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[278] );
			return 1;
		}
		break;
		
	default:
		SendSystemMsg( pPlayer, g_LANG_STR[279] );
		return 1;
		break;
	}


	
	sPITEM_DATA pEnchantedItem = CreateItem( enchantNum, "PACKET_EnchantItem() - rndSkip", __FILE__, __LINE__ );
		
		if( !pEnchantedItem )
			return 1;
	
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
		PutByte( g_Packet, 1, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		

#ifdef USE_ITEM_SERIAL
		strcpy( pEnchantedItem->serial, pItem->serial );
#endif

		UseEnchantCardBox( pPlayer, pCardItem, payCard );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, NULL, pCardItem, ITEMLOG_ACT_INCHENT_REMOVE );
#endif
//	INSERT_ITEM_TO_MEMORY( pCardItem );
		
		// 인첸트 대상 아이템
		sCHECK_INVEN checkInven;

		checkInven.page = pItem->invenPage;
		checkInven.x = pItem->invenX;
		checkInven.y = pItem->invenY;

		ItemFromInventory( pPlayer, pItem );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_INCHENT_REMOVE );
#endif

		INSERT_ITEM_TO_MEMORY( pItem );
				
/*		if( !CheckEmptyInven( pPlayer, &checkInven ) )
		{
			log( "PACKET_EnchantItem : !CheckEmptyInven( pPlayer, &checkInven )\r\n" );
			return 1;
		}*/
		
		ItemToInventory( pPlayer, pEnchantedItem, &checkInven );

#ifdef dUSE_ITEMLOG
		// Log
		ITEMLOG_ItemLog( pPlayer, NULL, pEnchantedItem, ITEMLOG_ACT_INCHENT_CREATE );
#endif	
		GET_MONEY( pPlayer ) -= payMoney;
		DATASERV_SendUpdateMoney( pPlayer );
		UpdateMoney( pPlayer );

		ClearSmith( pPlayer );

		return 1;
}

//
// Function Name: PACKET_EnchantItem
// Date: 
// Description: 
//
BOOL PACKET_EnchantItem( sPDESC_DATA pPlayer )
{
	int itemIdx;
	int cardIdx;

	itemIdx = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	cardIdx = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem = NULL;
	sPITEM_DATA pCardItem = NULL;

	pItem = FindItemFromInven( pPlayer, itemIdx );
	pCardItem = FindItemFromInven( pPlayer, cardIdx );

	if( !pItem || !pCardItem )
	{
		return 1;
	}

	if( pCardItem->itemNum != dENCHANT_CARD_ARMOR &&
		pCardItem->itemNum != dENCHANT_CARD_WEAPON &&
		pCardItem->itemNum != dENCHANT_CARD_OPTION )
	{
		return 1;
	}

    // Reject capped items before consuming either the item or the card.
    // Contiguous IDs after +20 can belong to an entirely different family.
    if (GET_ITEM_UNIQ2(pItem->itemNum) >= 20) return 1;

	int enchantNum = pItem->itemNum+1;
	
	if( !IS_VALID_ITEM( enchantNum ) )
	{
		return 1;
	}

	// 카드와 아이템 확인
	if( pCardItem->itemNum == dENCHANT_CARD_ARMOR )
	{
		if( !IS_ARMOR( GET_ITEM_TYPE( pItem ) ) )
		{
			return 1;
		}
	}
	else if( pCardItem->itemNum == dENCHANT_CARD_WEAPON )
	{
		if( !IS_WEAPON( GET_ITEM_TYPE( pItem ) ) )
		{
			return 1;
		}
	}
	
	// 상위 아이템이 동일계열이 아닐경우 
	if( g_ItemInfo[enchantNum]->isUniq <= g_ItemInfo[pItem->itemNum]->isUniq )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );

		// 아이템 삭제 
		ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
		/* Card consumption is logged once by the helper. */
#endif
		/* Card lifetime is handled by ConsumeOneEnchantCard. */
		ItemFromInventory( pPlayer, pItem );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_INCHENT_REMOVE );
#endif
		INSERT_ITEM_TO_MEMORY( pItem );

		return 1;
	}

	BYTE rndSkip = 0;

	if( IS_WEAPON( GET_ITEM_TYPE( pItem ) ) && g_ItemInfo[enchantNum]->isUniq <= 5 )
		rndSkip = 1;
	else if( ( IS_ARMOR( GET_ITEM_TYPE( pItem ) ) ) && g_ItemInfo[enchantNum]->isUniq <= 3 )
		rndSkip = 1;
	else
		rndSkip = 0;

	if( pPlayer->adminLevel >= dADMIN_GRGOD && pPlayer->ch2.EnchantNoFail )
		rndSkip = 1;

	if( rndSkip )
	{
		sPITEM_DATA pEnchantedItem = CreateItem( enchantNum, "PACKET_EnchantItem() - rndSkip", __FILE__, __LINE__ );
		
		if( !pEnchantedItem )
			return 1;
		
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
		PutByte( g_Packet, 1, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		

#ifdef USE_ITEM_SERIAL
		strcpy( pEnchantedItem->serial, pItem->serial );
#endif

		// 아이템 삭제 ( 카드 )
		ConsumeOneEnchantCard( pPlayer, pCardItem );

#ifdef dUSE_ITEMLOG
		/* Card consumption is logged once by the helper. */
#endif

		/* Card lifetime is handled by ConsumeOneEnchantCard. */
		
		// 인첸트 대상 아이템
		sCHECK_INVEN checkInven;

		checkInven.page = pItem->invenPage;
		checkInven.x = pItem->invenX;
		checkInven.y = pItem->invenY;

		ItemFromInventory( pPlayer, pItem );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_INCHENT_REMOVE );
#endif

		INSERT_ITEM_TO_MEMORY( pItem );
				
/*		if( !CheckEmptyInven( pPlayer, &checkInven ) )
		{
			log( "PACKET_EnchantItem : !CheckEmptyInven( pPlayer, &checkInven )\r\n" );
			return 1;
		}*/
		
		ItemToInventory( pPlayer, pEnchantedItem, &checkInven );

		if( GET_ITEM_UNIQ2( enchantNum ) >= 10 )
		{
			char szTemp[256];
			sprintf( szTemp, "Congrats! %s successfully upgraded %s.", GET_NAME( pPlayer ), GET_ITEM_HNAME( pEnchantedItem )  );
			BroadCastingMsg( szTemp );
		}

#ifdef dUSE_ITEMLOG
		// Log
		ITEMLOG_ItemLog( pPlayer, NULL, pEnchantedItem, ITEMLOG_ACT_INCHENT_CREATE );
#endif
	}
	else
	{
		//		BYTE rndNum = number( 0, 2 ); 인첸트률

        // Published new-card success rates. Keep the roll as an int:
        // the old BYTE conversion could wrap random values above 255.
        const bool enchantWeapon = IS_WEAPON(GET_ITEM_TYPE(pItem));
        const int targetLevel = GET_ITEM_UNIQ2(enchantNum);
        const int successRoll = number(1, 100);
        const BYTE successRnd = LaqiaEnchantRollSucceeds(
            enchantWeapon, targetLevel, successRoll) ? 1 : 0;
        // No per-weapon or armour forced-failure gate above +14.

		// 성공 
		if( successRnd == 1 )
		{
			sPITEM_DATA pEnchantedItem = CreateItem( enchantNum, "PACKET_EnchantItem - use rand", __FILE__, __LINE__ );
			
			if( !pEnchantedItem )
				return 1;
	
#ifdef USE_ITEM_SERIAL
			strcpy( pEnchantedItem->serial, pItem->serial );			
#endif
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
			PutByte( g_Packet, 1, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendData( pPlayer, g_Packet, g_nPos );
			
			// 아이템 삭제 
			ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
			/* Card consumption is logged once by the helper. */
#endif

			/* Card lifetime is handled by ConsumeOneEnchantCard. */

			sCHECK_INVEN checkInven;
	
			checkInven.page = pItem->invenPage;
			checkInven.x = pItem->invenX;
			checkInven.y = pItem->invenY;

			ItemFromInventory( pPlayer, pItem );

#ifdef dUSE_ITEMLOG
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_INCHENT_REMOVE );
#endif
			INSERT_ITEM_TO_MEMORY( pItem );
								
			ItemToInventory( pPlayer, pEnchantedItem, &checkInven );
#ifdef dUSE_ITEMLOG
			// Log
			ITEMLOG_ItemLog( pPlayer, NULL, pEnchantedItem, ITEMLOG_ACT_INCHENT_CREATE );
#endif
			if( GET_ITEM_UNIQ2( enchantNum ) >= 10 )
			{
				char szTemp[256];
				sprintf( szTemp, "Congrats! %s successfully upgraded %s.", GET_NAME( pPlayer ), GET_ITEM_HNAME( pEnchantedItem )  );
				BroadCastingMsg( szTemp );
			}
		}
		// 실패 
		else	 
		{
			BYTE subRnd = number( 0, 2 );

			// 고인첸은 실패
			/*if( ( IS_WEAPON( GET_ITEM_TYPE( pItem ) ) && GET_ITEM_UNIQ2( enchantNum ) >= 10 ) ||//14 ) ||
				( IS_ARMOR( GET_ITEM_TYPE( pItem ) ) && GET_ITEM_UNIQ2( enchantNum ) >= 10 ) )//13 ) )
				subRnd = 3;

			// 고인첸은 실패
			else if( ( IS_WEAPON( GET_ITEM_TYPE( pItem ) ) && GET_ITEM_UNIQ2( enchantNum ) <= 9 ) ||//14 ) ||
				( IS_ARMOR( GET_ITEM_TYPE( pItem ) ) && GET_ITEM_UNIQ2( enchantNum ) <= 9 ) )//13 ) )
			{
				subRnd = number( 0, 2 );
			}*/
			//BYTE rndMaxValue = 1;

			if( ( IS_WEAPON( GET_ITEM_TYPE( pItem ) ) && GET_ITEM_UNIQ2( enchantNum ) <= 14 ) ||//14 ) ||
				( IS_ARMOR( GET_ITEM_TYPE( pItem ) ) && GET_ITEM_UNIQ2( enchantNum ) <= 13 ) )//13 ) )
			{
				subRnd = 2;
			}
			else
			{
				subRnd = 3;
			}

			subRnd = number( 0, subRnd );
			
			int		nSaveItem = 0;

			switch( subRnd )
			{
			case 0:		// 인첸트 않됨 Not enchanted
				g_nPos = 2;
				PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
				PutByte( g_Packet, 0, g_nPos );
				PutSize( g_Packet, g_nPos );
				SendData( pPlayer, g_Packet, g_nPos );

				ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
				/* Card consumption is logged once by the helper. */
#endif
				/* Card lifetime is handled by ConsumeOneEnchantCard. */

				SendSystemMsg( pPlayer, g_LANG_STR[228] );
				break;
			case 1:		// -1 
			case 2:		// -2
				{			
					if( IS_WEAPON( GET_ITEM_TYPE( pItem ) ) )
					{
						if( GET_ITEM_UNIQ2( pItem->itemNum ) == 11 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 12  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 Delete item
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 13 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 14  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 Delete item
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 15 )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
					}
					else if( IS_ARMOR( GET_ITEM_TYPE( pItem ) )  )
					{
						if(  GET_ITEM_UNIQ2( pItem->itemNum ) == 11 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 12  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 13 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 14  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 15 )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 15 )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) <= 4 )
						{
							nSaveItem = 0;
						}
						else
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
					}
					if( nSaveItem == 0 )
					{
						int minusItem;

						if( subRnd == 1 )
							minusItem = pItem->itemNum - 1;
						else
						{
							if( number( 1, 5 ) == 1 )
								minusItem = pItem->itemNum - 2;
							else
								minusItem = pItem->itemNum - 1;
						}

						if( !IS_VALID_ITEM( minusItem ) )
							return 1;

						if( subRnd == 1 )
						{
							if( GET_ITEM_UNIQ2( minusItem ) != ( GET_ITEM_UNIQ( pItem ) - 1 ) )
								return 1;
						}
						else
						{
							if( GET_ITEM_UNIQ2( minusItem ) != ( GET_ITEM_UNIQ( pItem ) - (pItem->itemNum - minusItem) ) )
								return 1;
						}

						sPITEM_DATA pEnchantItem = CreateItem( minusItem, "PACKET_EnchantItem - useSubRnd 1", __FILE__, __LINE__ );

						if( !pEnchantItem )
							return 1;

#ifdef USE_ITEM_SERIAL
						strcpy( pEnchantItem->serial, pItem->serial );
#endif

						g_nPos = 2;
						PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
						PutByte( g_Packet, 0, g_nPos );
						PutSize( g_Packet, g_nPos );
						SendData( pPlayer, g_Packet, g_nPos );

						// 아이템 삭제 
						ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
						/* Card consumption is logged once by the helper. */
#endif
						/* Card lifetime is handled by ConsumeOneEnchantCard. */

						sCHECK_INVEN checkInven;

						checkInven.page = pItem->invenPage;
						checkInven.x = pItem->invenX;
						checkInven.y = pItem->invenY;

						ItemFromInventory( pPlayer, pItem );
#ifdef dUSE_ITEMLOG
						ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_INCHENT_REMOVE );
#endif
						INSERT_ITEM_TO_MEMORY( pItem );

						ItemToInventory( pPlayer, pEnchantItem, &checkInven );
#ifdef dUSE_ITEMLOG
						// Log
						ITEMLOG_ItemLog( pPlayer, NULL, pEnchantItem, ITEMLOG_ACT_INCHENT_CREATE );
#endif 
						if( subRnd == 1 )
							SendSystemMsg( pPlayer, g_LANG_STR[229] );
						else
							SendSystemMsg( pPlayer, g_LANG_STR[230] );
					}
				}
				break;
			case 3:		// 실패
				{
					if( IS_WEAPON( GET_ITEM_TYPE( pItem ) ) )
					{
						if(  GET_ITEM_UNIQ2( pItem->itemNum ) == 11 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 12  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 13 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 14  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 15 )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM125, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
					}
					else if( IS_ARMOR( GET_ITEM_TYPE( pItem ) )  )
					{
						if(  GET_ITEM_UNIQ2( pItem->itemNum ) == 11 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 12  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 13 || 
							GET_ITEM_UNIQ2( pItem->itemNum ) == 14  )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else if( GET_ITEM_UNIQ2( pItem->itemNum ) == 15 )
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
						else
						{
							if( UseOtherInvenItemCnt( pPlayer, dBILL_ITEM_NUM126, 1 ) )
							{
								SendSystemMsg( pPlayer, g_LANG_STR[293], 1 );
								nSaveItem = 1;

								g_nPos = 2;
								PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
								PutByte( g_Packet, 0, g_nPos );
								PutSize( g_Packet, g_nPos );
								SendData( pPlayer, g_Packet, g_nPos );

								// 아이템 삭제 
								ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
								/* Card consumption is logged once by the helper. */
#endif 
							}
						}
					}
					if( nSaveItem == 0 )
					{
						g_nPos = 2;
						PutWord( g_Packet, dPACKET_ENCHANT_ITEM_RES, g_nPos );
						PutByte( g_Packet, 0, g_nPos );
						PutSize( g_Packet, g_nPos );
						SendData( pPlayer, g_Packet, g_nPos );

						// 아이템 삭제 
						ConsumeOneEnchantCard( pPlayer, pCardItem );
#ifdef dUSE_ITEMLOG
						/* Card consumption is logged once by the helper. */
#endif 
						/* Card lifetime is handled by ConsumeOneEnchantCard. */
						ItemFromInventory( pPlayer, pItem );
#ifdef dUSE_ITEMLOG
						ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_INCHENT_REMOVE );
#endif 
						INSERT_ITEM_TO_MEMORY( pItem );
					}
				}
				break;
			} // end switch
		} // end else
	}
	g_nEnchantTotal++;
	return 1;
}

//
// Function Name: PACKET_PartyAskInvite
// Date: 
// Description: 
//
BOOL PACKET_PartyAskInvite( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pInvitePlayer = NULL;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pInvitePlayer = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pInvitePlayer = FindPlayerInArea( pPlayer->ch.mapNum, 
			pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}

	if( !pInvitePlayer )
		return 1;

	if( abs( GET_ALL_SKILL( pInvitePlayer ) - GET_ALL_SKILL( pPlayer ) ) >= 100)//100 )Party restrictions
	{
		//SendSystemMsg( pPlayer, g_LANG_STR[231] );
		SendSystemMsg( pPlayer, "You need to be within 26 levels of this player to join the party." );
			return 1;
	}

	if( !GET_PARTY_MASTER( pPlayer ) )
	{
		if( !PARTY_Create( pPlayer ) )
			return 1;
	}
	
	if( GET_PARTY_MASTER( pPlayer ) != pPlayer )
		return 1;

	if( IS_SET( pInvitePlayer->ch2.flag, dPLAYER_FLG_DENY_PARTY ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[26] );
		return 1;
	}

	if( GET_PARTY_MASTER( pInvitePlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[27] );
		return 1;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTY_ASKINVITE, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pInvitePlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_PartyInviteRes
// Date: 
// Description: 
//
BOOL PACKET_PartyInviteRes( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
		return 1;

	if( GET_PARTY_MASTER( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[28] );
		return 1;
	}

	int idxNum;
	BYTE result;

	idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	result = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pMaster;
	
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pMaster = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pMaster = FindPlayerInArea( pPlayer->ch.mapNum,
			pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}

	if( !pMaster )
		return 1;
	
	// 거부할 경우
	if( !result )
	{
		StrSendSysMsg( pPlayer, 29, GET_NAME( pPlayer ) );
		//SendSystemMsg( pMaster, g_LANG_STR[29], GET_NAME( pPlayer ) );
		return 1;
	}
	
	// 파티장이 아니면
	if( GET_PARTY_MASTER( pMaster ) != pMaster )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_PARTY_JOIN_RESULT, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );
		return 1;
	}

	// 파티가 꽉 차있으면
	if( pMaster->ch2.party.memberCnt >= 7 ) //When the party is full
	{	
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_PARTY_JOIN_RESULT, g_nPos );
		PutByte( g_Packet, 1, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer );
		return 1;
	}
	
	PARTY_AddNewMember( pMaster, pPlayer );

	return 1;
}


//
// Function Name: PACKET_UseSkill
// Date: 
// Description: 
//
BOOL PACKET_UseSkill( sPDESC_DATA pPlayer )
{
	WORD wSkill;
	int nTargetIdx;
	BYTE btTargetType;		// 0 = 플레이어 , 1 = 몹 

	if( CheckWaitSkill( pPlayer ) )
		return 1;

	wSkill = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	nTargetIdx = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	btTargetType = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	SKILL_UseSkill( pPlayer, nTargetIdx, btTargetType, wSkill );

	return 1;
}

//
// Function Name: PACKET_CreateGuild
// Date: 
// Description: 
//
BOOL PACKET_CreateGuild( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_GUILD
	return 1;
#endif
	if( !IsGuildKeeperArea( pPlayer ) )
		return 1;

	char guildName[1024];

	GetString( pPlayer->TemprecvBuff, guildName, pPlayer->recvPos );

	if( strlen( guildName ) > dGUILD_NAME_LEN )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[75] );
		return 1;
	}

	if( !CreateNewGuild( pPlayer, guildName, 0 ) )
		return 1;

	SendSystemMsg( pPlayer, g_LANG_STR[76] );
	return 1;
}

//
// Function Name: PACKET_DestoryGuild
// Date: 
// Description: 
//
BOOL PACKET_DestroyGuild( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_GUILD
	return 1;
#endif

	if( !IsGuildKeeperArea( pPlayer ) )
		return 1;

	if( !GET_GUILD( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[77] );
		return 1;
	}

	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[78] );
		return 1;
	}
	
	if( WAR_FindOwnerData( GET_GUILD( pPlayer ) ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[191] );
		return 1;
	}

	if( WAR_FindOwnCastle( GET_GUILD( pPlayer ) ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[190] );
		return 1;
	}

	if( !GET_GUILD_DATA( GET_GUILD( pPlayer ) ) )
		return 1;

	if( GUILD_CountGuildLevel( GET_GUILD_DATA( GET_GUILD( pPlayer ) ), dGUILD_LEVEL_SECONDMASTER ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[198] );
		return 1;
	}

#ifdef __CHINA_VER__
	GET_CHA( pPlayer ) -= 30000;
	SendUpdateCha( pPlayer );
	DATASERV_SendUpdateCha( pPlayer );
#endif

	CrashGuild( GET_GUILD( pPlayer ) );

	SendSystemMsg( pPlayer, g_LANG_STR[129] );
	return 1;
}


//
// Function Name: PACKET_AddFriend
// Date: 
// Description: 
//
BOOL PACKET_AddFriend( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_FRIEND
	return 1;
#endif

	if( GET_FRIEND_CNT( pPlayer ) >= dMAX_FRIEND )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[107], dMAX_FRIEND );
		return 1;
	}

	char friendName[256];

	GetString( pPlayer->TemprecvBuff, friendName, pPlayer->recvPos );

	if( !strcmp( GET_NAME( pPlayer ), friendName ) )
	{
		SendSystemMsg( pPlayer, "You cannot make friends with yourself." );
		return 1;
	}

	if( strlen( friendName ) >= dNAME_LEN )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[75] );
		return 1;
	}

	FRIEND_AddFriend( pPlayer, friendName ); 

	return 1;
}


//
// Function Name: PACKET_RemoveFriend
// Date: 
// Description: 
//
BOOL PACKET_RemoveFriend( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_FRIEND
	return 1;
#endif
	char friendName[256];

	GetString( pPlayer->TemprecvBuff, friendName, pPlayer->recvPos );

	FRIENDS_RemoveFriend( pPlayer, friendName );

	return 1;
}

//
// Function Name: PACKET_FriendMemo
// Date: 
// Description: 
//
BOOL PACKET_FriendMemo( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_FRIEND
	return 1;
#endif

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[108] );
		return 1;
	}

	char name[256];
	char str[1024];

	GetString( pPlayer->TemprecvBuff, name, pPlayer->recvPos );
	GetString( pPlayer->TemprecvBuff, str, pPlayer->recvPos );	

	if( strlen( name ) >= dNAME_LEN )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[75] );
		return 1;
	}

	sPDESC_DATA to = FindPlayerNameList( name );
	
	if( !to )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[109] );
		return 1;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_FRIEND_MEMO, g_nPos );
	PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
	PutString( g_Packet, str, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( to, g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	//StrSendSysMsg( pPlayer, 110, GET_NAME(to) );
	SendSystemMsg( pPlayer, g_LANG_STR[110], GET_NAME( to ) );

	return 1;
}


//
// Function Name: PACKET_GuildAddMember
// Date: 
// Description: 
//
BOOL PACKET_GuildAddMember( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_GUILD
	return 1;
#endif

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( gWorldUnderAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[197] );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	WORD guildNo = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
	BYTE allowJoin = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( !IS_VALID_GUILD( guildNo ) )
		return 1;

 	sPDESC_DATA pMaster = FindPlayerInArea( GET_MAP_NUM( pPlayer ), 
		GET_POSX( pPlayer ) / dONE_TILE, GET_POSY( pPlayer ) / dONE_TILE, idxNum );

	if( pMaster )
	{
		if( !allowJoin )
		{
			SendSystemMsg( pMaster, g_LANG_STR[111], GET_NAME( pPlayer ) );
			return 1;
		}

		if( GET_GUILD( pMaster ) != guildNo || 
			( GET_GUILD_LEVEL( pMaster ) != dGUILD_LEVEL_MASTER && GET_GUILD_LEVEL( pMaster ) != dGUILD_LEVEL_SECONDMASTER ) )
			return 1;

		GUILD_AddMember( pPlayer, guildNo );
		SendSystemMsg( pPlayer, g_LANG_STR[112], GET_GUILD_DATA( guildNo )->guildName );
		SendSystemMsg( pMaster, g_LANG_STR[113], GET_NAME( pPlayer ) );
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[114] );
	}

	return 1;
}


//
// Function Name: PACKET_GuildDropMember
// Date: 
// Description: 
//
BOOL PACKET_GuildDropMember( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_GUILD
	return 1;
#endif
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( !GET_GUILD( pPlayer ) )
		return 1;

	if( gWorldUnderAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[197] );
		return 1;
	}

	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER &&
		GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_SECONDMASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[115] );
		return 1;
	}

	char name[128];

	GetString( pPlayer->TemprecvBuff, name, pPlayer->recvPos );

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( pTarget )
	{
		if( pPlayer == pTarget )
			return 1;

		if( GET_GUILD( pPlayer ) != GET_GUILD( pTarget ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[116] );
			return 1;
		}

		if( GET_GUILD_LEVEL( pTarget ) >= GET_GUILD_LEVEL( pPlayer ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[115] );
			return 1;
		}

		GUILD_DropMember( pTarget, GET_GUILD( pPlayer ) );
		SendSystemMsg( pTarget, g_LANG_STR[117], GET_GUILD_DATA( GET_GUILD( pPlayer ) )->guildName );
		SendSystemMsg( pPlayer, g_LANG_STR[118], GET_NAME( pTarget ) );
	}
	else
	{
		sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

		if( pGuild )
		{
			sPMEMBER_INFO pMemberInfo = GUILD_FindMemberInfoFromList( pGuild, name );

			if( pMemberInfo )
			{				
				if( pMemberInfo->memberLevel >= GET_GUILD_LEVEL( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[115] );
					return 1;
				}
				
				pMemberInfo->btKickOut = 1;
				
				SendSystemMsg( pPlayer, g_LANG_STR[118], pMemberInfo->name );
			}
		}
		else
			SendSystemMsg( pPlayer, g_LANG_STR[109] );
	}

	return 1;
}

//
// Function Name: PACKET_GuildQuit
// Date: 
// Description: 
//
BOOL PACKET_GuildQuit( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_GUILD
	return 1;
#endif

	if( !GET_GUILD( pPlayer ) )
		return 1;

	if( gWorldUnderAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[197] );
		return 1;
	}

	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return 1;

	if( GET_GUILD_LEVEL( pPlayer ) == dGUILD_LEVEL_MASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[120] );
		return 1;
	}

	GUILD_DropMember( pPlayer, GET_GUILD( pPlayer ) );

	SendSystemMsg( pPlayer, g_LANG_STR[121] );

	return 1;
}

//
// Function Name: PACKET_AddMemberRequest
// Date: 
// Description: 
//
BOOL PACKET_AddMemberRequest( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( !GET_GUILD( pPlayer ) )
		return 1;

	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER &&
		GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_SECONDMASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[115] );
		return 1;
	}

	if( gWorldUnderAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[197] );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pTarget = FindPlayerInArea( GET_MAP_NUM( pPlayer ), 
		GET_POSX( pPlayer ) / dONE_TILE, GET_POSY( pPlayer ) / dONE_TILE, idxNum );

	if( pTarget )
	{
		if( GET_GUILD( pTarget ) || GET_TMP_GUILD( pTarget ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[119] );
			return 1;
		}

		g_nPos = 2;
		PutWord( g_Packet, dPACKET_GUILD_ADDMEMBER_REQUEST, g_nPos );
		PutWord( g_Packet, GET_GUILD( pPlayer ), g_nPos );
		PutString( g_Packet, GET_GUILD_DATA( GET_GUILD( pPlayer ) )->guildName, g_nPos );
		PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pTarget, g_Packet, g_nPos );
	}

	return 1;
}

//
// Function Name: PACKET_MoveDealMap
// Date: 
// Description: 
//
BOOL PACKET_MoveDealMap( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( GET_MAP_NUM( pPlayer ) == dDEAL_MAP_NUM )
	{
		if( !FindNpcInMap( pPlayer, 66 ) )
			return 1;

		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] &&
			!pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[232] );
			return 1;
		}
		else
		{
			GET_GOTO_MAPNUM( pPlayer ) = 31;
			GET_GOTO_POSX( pPlayer ) = 76;
			GET_GOTO_POSY( pPlayer ) = 60;
			
			PlayerFromMap( pPlayer );
			SendMapLoading( pPlayer );
			return 1;
		}
	}

	if( GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE1 ||
		GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE2 ||
		GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE3 )
	{
		return 1;
	}

	// 거래 맵일 경우 
	if( GET_MAP_NUM( pPlayer ) == dDEAL_MAP_NUM )
		return 1;

	if( GET_RIDE_STATE( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[233] );
		return 1;
	}

	pPlayer->ch.prevMapNum = GET_MAP_NUM( pPlayer );
	pPlayer->ch.prevPosX = GET_POSX( pPlayer );
	pPlayer->ch.prevPosY = GET_POSY( pPlayer );
	
	pPlayer->ch2.gotoMapNum = dDEAL_MAP_NUM;
	pPlayer->ch2.gotoPosX = dDEAL_MAP_POSX;
	pPlayer->ch2.gotoPosY = dDEAL_MAP_POSY;
	pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
	
	PlayerFromMap( pPlayer );
	SendMapLoading( pPlayer );

	return 1;
}


//
// Function Name: PACKET_MakeCamp
// Date: 
// Description: 
//
BOOL PACKET_MakeCamp( sPDESC_DATA pPlayer )
{
#ifdef USE_GUILD_WAR
	WAR_MakeCamp( pPlayer );
#endif
	return 1;
}

//
// Function Name: PACKET_FishingAction
// Date: 2004.03.24
// Description: 낚시 시작 보고와 건져 올림.
// 

BOOL PACKET_FishingAction( sPDESC_DATA pPlayer )
{
	return 1;
}

//
// Function Name: PACKET_FishingPos
// Date: 2004.03.24
// Description: 
// 
BOOL PACKET_FishingPos( sPDESC_DATA pPlayer )
{
	return 1;
}

//
// Function Name: PACKET_PetOnOff
// Date: 
// Description: 
// 
BOOL PACKET_PetOnOff( sPDESC_DATA pPlayer )
{
#ifndef dNOT_USE_PET
	if( !GET_PET_TYPE( pPlayer ) )
		return 1;

	if( IS_USING_PET( pPlayer ) )
	{
		PET_SendPetOff( pPlayer );
	}
	else
	{
		PET_SendPetOn( pPlayer );
	}
#endif
	return 1;
}

//
// Function Name: PACKET_PetChangeName
// Date: 
// Description: 
// 
BOOL PACKET_PetChangeName( sPDESC_DATA pPlayer )
{
#ifndef dNOT_USE_PET
	if( !GET_PET_TYPE( pPlayer ) )
		return 1;

	if( !IS_USING_PET( pPlayer ) )
		return 1;

	char name[128];

	GetString( readBuff, name, readPos );

	// 이름이 너무 길면
	if( strlen( name ) >= dPET_NAME_LEN )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[75] );
		return 1;
	}

	PET_UpdatePetName( pPlayer, name );

	// 이름 복사
	strcpy( GET_PET_NAME( pPlayer ), name );

	g_nPos = 2;

	PutWord( g_Packet, dPACKET_PET_UPDATENAME, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutString( g_Packet, name, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
#endif
	return 1;
}


//
// Function Name: PACKET_DealOk
// Date: 
// Description: 
// 
BOOL PACKET_DealOk( sPDESC_DATA pPlayer )
{
	if( !pPlayer->ch2.dealVict )
		return 1;

	if( pPlayer->ch2.dealVict->ch2.dealVict != pPlayer )
		return 1;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DEAL_BEGINOK, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer );
	SendData( pPlayer->ch2.dealVict );

	return 1;
}

//
// Function Name: PACKET_GuildChangeMaster
// Date: 
// Description: 
// 
BOOL PACKET_GuildChangeMaster( sPDESC_DATA pPlayer )
{
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPDESC_DATA pNewMaster = FindPlayerInArea( GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ) / dONE_TILE, GET_POSY( pPlayer ) / dONE_TILE, idxNum );

	if( !pNewMaster )
		return 1;

	GUILD_ChangeMaster( pPlayer, pNewMaster );

	return 1;
}


//
// Function Name: PACKET_GuildGetMemberList
// Date: 
// Description: 
// 
BOOL PACKET_GuildGetMemberList( sPDESC_DATA pPlayer )
{
	GUILD_SendGuildMember( pPlayer );
	return 1;
}

//
// Function Name: PACKET_GuildAdvance
// Date: 
// Description: 
// 
BOOL PACKET_GuildAdvance( sPDESC_DATA pPlayer )
{
	if( !GET_GUILD( pPlayer ) )
		return 1;

	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return 1;
	
	if( gWorldUnderAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[197] );
		return 1;
	}

	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

	if( !pGuild )
		return 1;

	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[115] );
		return 1;
	}

	BYTE haveCastle = GUILD_CountHaveCastle( GET_GUILD( pPlayer ) );
	BYTE secondMasterCnt = GUILD_CountGuildLevel( pGuild, dGUILD_LEVEL_SECONDMASTER );

	if( !haveCastle )
	{
		if( secondMasterCnt == 3 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[192], 3 );
			return 1;
		}
	}
	else
	{
		if( secondMasterCnt >= ( haveCastle * 2 ) + 1 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[192], ( ( haveCastle * 2 ) + 1 ) );
			return 1;
		}
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pTarget = FindPlayerInArea( GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer )/dONE_TILE, GET_POSY( pPlayer )/dONE_TILE, idxNum );

	if( pTarget )
	{
		if( GET_STATE( pTarget ) != dDESC_STATE_PLAYING &&
			GET_STATE( pTarget ) != dDESC_STATE_FIGHT )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[114] );
			return 1;
		}

		if( GET_GUILD( pTarget ) != GET_GUILD( pPlayer ) )
			return 1;

		if( GET_GUILD_LEVEL( pTarget ) >= GET_GUILD_LEVEL( pPlayer ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[115] );
			return 1;
		}

		if( GET_GUILD_LEVEL( pTarget ) == dGUILD_LEVEL_SECONDMASTER )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[153] );
			return 1;
		}

		GUILD_AdvanceLevel( pTarget, dGUILD_LEVEL_SECONDMASTER );
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[114] );
		return 1;
	}

	return 1;
}

//
// Function Name: PACKET_GuildDemotion
// Date: 
// Description: 
// 
BOOL PACKET_GuildDemotion( sPDESC_DATA pPlayer )
{
	if( !GET_GUILD( pPlayer ) )
		return 1;

	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return 1;

	if( gWorldUnderAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[197] );
		return 1;
	}
	
	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

	if( !pGuild )
		return 1;

	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[115] );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pTarget = FindPlayerInArea( GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer )/dONE_TILE, GET_POSY( pPlayer )/dONE_TILE, idxNum );

	if( pTarget )
	{
		if( GET_STATE( pTarget ) != dDESC_STATE_PLAYING &&
			GET_STATE( pTarget ) != dDESC_STATE_FIGHT )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[114] );
			return 1;
		}

		if( GET_GUILD( pTarget ) != GET_GUILD( pPlayer ) )
			return 1;

		if( GET_GUILD_LEVEL( pTarget ) >= GET_GUILD_LEVEL( pPlayer ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[115] );
			return 1;
		}

		if( GET_GUILD_LEVEL( pTarget ) != dGUILD_LEVEL_SECONDMASTER )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[154] );
			return 1;
		}

		GUILD_AdvanceLevel( pTarget, dGUILD_LEVEL_MEMBER );
	}
	else
	{
		SendSystemMsg( pPlayer, g_LANG_STR[114] );
		return 1;
	}

	return 1;
}



//
// Function Name: PACKET_MyShopOpen
// Date: 
// Description: 
// 
BOOL PACKET_MyShopOpen( sPDESC_DATA pPlayer )
{ 
		SendSystemMsg( pPlayer, "PlayerShop is currently disabled." );
/*
	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( pPlayer->ch2.myShop.isMyShopOpen )
		return 1;

	InitMyShop( pPlayer );

	sprintf( pPlayer->ch2.myShop.myShopName, g_LANG_STR[200], GET_NAME( pPlayer ) );
//	string	test;
//	test = GET_NAME( pPlayer );
//	test.append( "'s user shop." );
//	strcpy( pPlayer->ch2.myShop.myShopName, test.c_str() );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MYSHOP_OPENRESULT, g_nPos );
	PutString( g_Packet, pPlayer->ch2.myShop.myShopName, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
*/
	return 1;
}

//
// Function Name: PACKET_MyShopChangeTitle
// Date: 
// Description: 
// 
BOOL PACKET_MyShopChangeTitle( sPDESC_DATA pPlayer )
{
	char title[1024];

	GetString( pPlayer->TemprecvBuff, title, pPlayer->recvPos );

	if( strlen( title ) <= 2 )
	{
//		SendSystemMsg( pPlayer, g_LANG_STR[159] );
		return 1;
	}

	if( strlen( title ) >= dMAX_MYSHOP_NAME_LEN )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[160] );
		return 1;
	}
	strcpy( pPlayer->ch2.myShop.myShopName, "%s(실드상점)" title );
	//strcpy( pPlayer->ch2.myShop.myShopName, title );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MYSHOP_TITLECHANGED, g_nPos );
	PutString( g_Packet, pPlayer->ch2.myShop.myShopName, g_nPos );
	//PutString( g_Packet, title, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_MyShopAddItem
// Date: 
// Description: 
// 
BOOL PACKET_MyShopAddItem( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.myShop.isMyShopOpen )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[161] );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	//int price = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
char szTemp[16];
	GetString( pPlayer->TemprecvBuff, szTemp, pPlayer->recvPos );

	double price = atof( szTemp );
	
	if( price <= 0 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[162] );
		return 1;
	}

	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );
	

		if( !pItem )
	{
		return 0;
	}

	if( GET_ITEM_SELL_TYPE1( pItem ) == 3 )
	{
		SendSystemMsg(pPlayer, "Test");
		return 1;
	}
	
	
	if( pItem )
	{
		switch( pItem->invenPage )
		{
		case 2:
			{
				if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT20] )
				{
					//SendSystemMsgNum( pPlayer, 304, 0, "a" );
					SendSystemMsg( pPlayer, "55인벤확장카드를 사용해주십시오." );
					return 1;
				}
			}
			break;
		case 3:
			{
				if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT21] )
				{
					//SendSystemMsgNum( pPlayer, 304, 0, "a" );
					SendSystemMsg( pPlayer, "56인벤확장카드를 사용해주십시오." );
					return 1;
				}
			}
			break;
		case 4:
			{
				if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT22] )
				{
					//SendSystemMsgNum( pPlayer, 304, 0, "a" );
					SendSystemMsg( pPlayer, "57인벤확장카드를 사용해주십시오." );
					return 1;
				}
			}
			break;
		}

		if( FindMyShopItem( pPlayer, pItem->idxNum, pItem->itemNum ) )
			return 1;
		
#ifdef dDAUM_BILLING
			if( IS_BILLING_ITEM( pItem ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[234] );
				return 1;
			}
#endif
//here
		sPMYSHOP_ITEM pShopItem = NewMyShopItem();

		if( pShopItem )
		{
			pShopItem->idxNum = pItem->idxNum;
			pShopItem->itemNum = pItem->itemNum;
			pShopItem->price = price;
			
			if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION )
				pShopItem->cnt = pItem->exVal[0];
			else
				pShopItem->cnt = 0;

			pShopItem->prev = NULL;
			pShopItem->next = NULL;

			INSERT_TO_LIST( pPlayer->ch2.myShop.myShopSellList, pShopItem, prev, next );

			g_nPos = 2;
			PutWord( g_Packet, dPACKET_MYSHOP_ITEMADDED, g_nPos );
			PutInteger( g_Packet, pItem->idxNum, g_nPos );
			PutInteger( g_Packet, pItem->itemNum, g_nPos );
			PutSize( g_Packet, g_nPos );

			SendData( pPlayer, g_Packet, g_nPos );
		}
	}

	return 1;
}

//
// Function Name: PACKET_MyShopRemoveItem
// Date: 
// Description: 
// 
BOOL PACKET_MyShopRemoveItem( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.myShop.isMyShopOpen )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[163] );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPMYSHOP_ITEM pItem = FindMyShopItem( pPlayer, idxNum );

	if( pItem )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_MYSHOP_ITEMREMOVED, g_nPos );
		PutInteger( g_Packet, pItem->idxNum, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );

		REMOVE_FROM_LIST( pPlayer->ch2.myShop.myShopSellList, pItem, prev, next );
		INSERT_TO_LIST( gITEMS.myshopitem_memory, pItem, prev, next );
	}

	return 1;
}

//
// Function Name: PACKET_MyShopSellBegin
// Date: 
// Description: 
// 
BOOL PACKET_MyShopSellBegin( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.myShop.isMyShopOpen )
		return 1;

	if( !pPlayer->ch2.myShop.myShopSellList )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[164] );
		return 1;
	}

	pPlayer->ch2.myShop.isMyShopOpen = 1;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MYSHOP_SELLBEGIN_RESULT, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MYSHOP_UPDATEINFO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutByte( g_Packet, 1, g_nPos );
	PutString( g_Packet, pPlayer->ch2.myShop.myShopName, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );

	return 1;
}

//
// Function Name: PACKET_MyShopSellFinish
// Date: 
// Description: 
// 
BOOL PACKET_MyShopSellFinish( sPDESC_DATA pPlayer )
{
	ClearMyShop( pPlayer );
	return 1;
}

//
// Function Name: PACKET_MyShopGetItemList
// Date: 
// Description: 
// 
BOOL PACKET_MyShopGetItemList( sPDESC_DATA pPlayer )
{
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pShopkeeper = FindPlayerInArea( GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ) / dONE_TILE, GET_POSY( pPlayer ) / dONE_TILE, idxNum );

	if( pShopkeeper )
	{	
		if( !pShopkeeper->ch2.myShop.isMyShopOpen )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[165] );
			return 1;
		}

		WORD cnt = 0;
		char szTemp[16];
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_MYSHOP_SENDITEMLIST, g_nPos );
		g_nPos += sizeof( WORD );

		sPMYSHOP_ITEM item, next_item;

		LIST_WHILE( pShopkeeper->ch2.myShop.myShopSellList, item, next_item, next, WMgr );
		PutInteger( g_Packet, item->idxNum, g_nPos );
		PutInteger( g_Packet, item->itemNum, g_nPos );
		//PutInteger( g_Packet, item->price, g_nPos );
		sprintf( szTemp, "%.0f", item->price );
		PutString( g_Packet, szTemp, g_nPos );
		PutByte( g_Packet, item->cnt, g_nPos );
		cnt++;
		LIST_WHILEEND( pShopkeeper->ch2.myShop.myShopSellList, item, next_item );

		*(WORD*)( g_Packet + 4 ) = cnt;
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
	}
	return 1;
}

//
// Function Name: PACKET_MyShopBuyItem
// Date: 
// Description: 
// 
BOOL PACKET_MyShopBuyItem( sPDESC_DATA pPlayer )
{
	int playerIdx = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	int itemIdx = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pShopkeeper = FindPlayerInArea( GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ) / dONE_TILE, GET_POSY( pPlayer ) / dONE_TILE, playerIdx );

	if( pShopkeeper )
	{
		if( pShopkeeper == pPlayer )
			return 1;

		if( !pShopkeeper->ch2.myShop.isMyShopOpen )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[165] );
			return 1;
		}

		sPMYSHOP_ITEM pShopItem = FindMyShopItem( pShopkeeper, itemIdx );
		sPITEM_DATA pItem = FindItemFromInven( pShopkeeper, itemIdx );

		if( !pShopItem || !pItem )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[166] );
			return 1;
		}

#ifdef USE_ITEM_WEIGHT
		if( GET_MAP_NUM( pPlayer ) != dDEAL_MAP_NUM )
		{
			if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION )
			{
				if( GET_CURRITEM_WEIGHT( pPlayer ) + ( GET_ITEM_WEIGHT( pItem ) * MIN( pItem->exVal[0], 1 ) ) > CalsAllowTotalWeight( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[180] );
					return 1;
				}
			}
			else
			{
				if( GET_CURRITEM_WEIGHT( pPlayer ) + GET_ITEM_WEIGHT( pItem ) > CalsAllowTotalWeight( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[180] );
					return 1;
				}
			}
		}
#endif

		if( pShopItem->itemNum == pItem->itemNum &&
			pShopItem->idxNum == pItem->idxNum )
		{
			if( GET_MONEY( pPlayer ) < pShopItem->price )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[157], pShopItem->price );
				return 1;
			}

			sCHECK_INVEN checkInven;

			if( !CheckEmptyInven( pPlayer, &checkInven ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[11] );
				return 1;
			}

			char szTemp[16];

			GET_MONEY( pPlayer ) -= pShopItem->price;
			UpdateMoney( pPlayer );
			DATASERV_SendUpdateMoney( pPlayer );
			DATASERV_SAVECHA( pPlayer );

			// 세금 0.5%
			int tax = (int)( pShopItem->price * 0.05 );

			pShopkeeper->ch2.myShop.myShopCashBox += ( pShopItem->price - tax );

			// 돈 저장
			GET_MONEY( pShopkeeper ) += ( pShopItem->price - tax );
			UpdateMoney( pShopkeeper );
			DATASERV_SendUpdateMoney( pShopkeeper );

#ifdef dUSE_ITEMLOG
			ITEMLOG_ItemLog( pShopkeeper, pPlayer, pItem, ITEMLOG_ACT_MYSHOP_REMOVE );
			ITEMLOG_ItemLog( pPlayer, pShopkeeper, pItem, ITEMLOG_ACT_MYSHOP_CREATE );
			ITEMLOG_MyShopLog( pShopkeeper, pPlayer, pItem, pShopItem->price );
#endif

			ItemFromInventory( pShopkeeper, pItem );
			ItemToInventory( pPlayer, pItem, &checkInven );

			SendSystemMsg( pPlayer, g_LANG_STR[167], GET_NAME( pShopkeeper ), GET_ITEM_HNAME( pItem ), pShopItem->price );
			SendSystemMsg( pShopkeeper, g_LANG_STR[168], GET_ITEM_HNAME( pItem ) , GET_NAME( pPlayer ), ( pShopItem->price - tax ), tax );

			g_nPos = 2;
			PutWord( g_Packet, dPACKET_MYSHOP_UPDATECASH, g_nPos );
			PutInteger( g_Packet, pShopkeeper->ch2.myShop.myShopCashBox, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendData( pShopkeeper, g_Packet, g_nPos );

			g_nPos = 2;
			PutWord( g_Packet, dPACKET_MYSHOP_ITEMREMOVED, g_nPos );
			PutInteger( g_Packet, pShopItem->idxNum, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendData( pShopkeeper, g_Packet, g_nPos );

			g_nPos = 2;
			PutWord( g_Packet, dPACKET_MYSHOP_REMOVELISTITEM, g_nPos );
			PutInteger( g_Packet, pShopItem->idxNum, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendData( pPlayer, g_Packet, g_nPos );

			REMOVE_FROM_LIST( pShopkeeper->ch2.myShop.myShopSellList, pShopItem, prev, next );
			INSERT_TO_LIST( gITEMS.myshopitem_memory, pShopItem, prev, next );
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[168] );
			return 1;
		}
	}

	return 1;
}


//
// Function Name: PACKET_WorldChat
// Date: 
// Description: 
// 
BOOL PACKET_WorldChat( sPDESC_DATA pPlayer )
{
#ifdef __CHINA_VER__
	char str[1024];

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return 1;

	if( pPlayer->ch2.worldChatDelay + 10 > g_CurrRealTime )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[149], ( pPlayer->ch2.worldChatDelay + 10 ) - g_CurrRealTime );
		return 1;
	}

	/*if( GET_ALL_SKILL( pPlayer ) < 25 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[156], 25 );
		return 1;
	}*/

	if( GET_MONEY( pPlayer ) < 2000 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[157], 2000 );
		return 1;
	}

	GetString( pPlayer->TemprecvBuff, str, pPlayer->recvPos );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_WORLD_CHAT, g_nPos );
	PutString( g_Packet, GET_NAME( pPlayer ), g_nPos );
	PutString( g_Packet, str, g_nPos );
	PutSize( g_Packet, g_nPos );

	sPDESC_DATA d, next_d;

	LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
	
	if( GET_STATE( d ) == dDESC_STATE_FIGHT ||
		GET_STATE( d ) == dDESC_STATE_PLAYING )
	{
		SendData( d, g_Packet, g_nPos );
	}

	LIST_WHILEEND( gPLAYERS.list, d, next_d );

	GET_MONEY( pPlayer ) -= 2000;
	DATASERV_SendUpdateMoney( pPlayer );
	UpdateMoney( pPlayer );

	pPlayer->ch2.worldChatDelay = g_CurrRealTime;
		return 1;
#else
	return 1;
#endif
}

//
// Function Name: PACKET_ReceiptMoney
// Date: 
// Description: 
// 
BOOL PACKET_ReceiptMoney( sPDESC_DATA pPlayer )
{
#ifndef USE_GARBAGE_MONEY
	return 1;
#endif

	if( !IsGarbageArea( pPlayer ) )
		return 1;

	int amount = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( amount <= 0 )
		return 1;

	if( GET_MONEY( pPlayer ) < amount )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[235] );
		return 1;
	}

	if( pPlayer->garbageMoney + amount > dMAX_HAVE_MONEY )
		return 1;

	GET_MONEY( pPlayer ) -= amount;
	UpdateMoney( pPlayer );
	DATASERV_SendUpdateMoney( pPlayer );

	pPlayer->garbageMoney += amount;
	ITEMDB_UpdateGarbageMoney( pPlayer );
	SendPlayerGarbageMoney( pPlayer );

	ITEMLOG_GarbageMoneyLog( pPlayer, amount, GARBAGEMONEY_LOG_ACT_IN );

	return 1;
}

//
// Function Name: PACKET_ReceiptMoney
// Date: 
// Description: 
// 
BOOL PACKET_WithdrawMoney( sPDESC_DATA pPlayer )
{
#ifndef USE_GARBAGE_MONEY
	return 1;
#endif

	if( !IsGarbageArea( pPlayer ) )
		return 1;

	int amount = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos  );

	if( amount <= 0 )
	{
		//접속끝기
		return 0;
	}

	if( pPlayer->garbageMoney < amount )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[235] );
		return 1;
	}

	if( GET_MONEY( pPlayer ) + amount > dMAX_HAVE_MONEY )
		return 1;

	pPlayer->garbageMoney -= amount;
	ITEMDB_UpdateGarbageMoney( pPlayer );
	SendPlayerGarbageMoney( pPlayer );

	GET_MONEY( pPlayer ) += amount;
	UpdateMoney( pPlayer );
	DATASERV_SendUpdateMoney( pPlayer );

	ITEMLOG_GarbageMoneyLog( pPlayer, amount, GARBAGEMONEY_LOG_ACT_OUT );

	return 1;
}


//
// Function Name: PACKET_SetPvPMode
// Date: 
// Description: 
// 
BOOL PACKET_SetPvPMode( sPDESC_DATA pPlayer )
{
	BYTE btMode = GetByte( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( btMode > 3 )
		return 1;

	GET_PVPMODE( pPlayer ) = btMode;

	SendPlayerPvPMode( pPlayer );
		
	return 1;
}

//
// Function Name: PACKET_GetCastleList
// Date: 
// Description: 
// 
BOOL PACKET_GetCastleList( sPDESC_DATA pPlayer )
{
#ifndef USE_GUILD_WAR
	return 1;
#endif
	sPLIST_BLOCK block, next_block, list;
	sPWORLD pWorld;

	WORD  cnt = 0;

	list = gWORLDLIST.GetFirstBlock();

	g_nPos = 2;
	PutWord( g_Packet, dAPCKET_SEND_CASTLELIST, g_nPos );
	g_nPos += 2;

	LIST_WHILE( list, block, next_block, next, WMgr );
	
	pWorld = (sPWORLD)block->data;

	if( pWorld->hasCastle )
	{
		PutInteger( g_Packet, pWorld->mapNum, g_nPos );	
		cnt++;
	}
	LIST_WHILEEND( list, block, next_block );

	*(WORD*)( g_Packet + 4 ) = cnt;

	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_GetWarState
// Date: 
// Description: 
// 
BOOL PACKET_GetWarState( sPDESC_DATA pPlayer )
{
#ifndef USE_GUILD_WAR
	return 1;
#endif

	enum
	{
		TYPE_NONE = 0,
		TYPE_CAMP = 1,
		TYPE_CASTLE = 2,
	};
		
	int mapNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( !IS_VALID_WORLD( mapNum ) )
		return 1;

	if( !IS_VALID_GUILD( gWORLD[mapNum]->ownerGuild ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[236] );
		return 1;
	}

//	StrSendSysMsg( pPlayer, 237, gWORLD[mapNum]->mapName, " Army occupied ",
//		GET_GUILD_DATA( gWORLD[mapNum]->ownerGuild )->guildName );
	SendSystemMsg( pPlayer, g_LANG_STR[237], gWORLD[mapNum]->mapName, GET_GUILD_DATA( gWORLD[mapNum]->ownerGuild )->guildName );

	sWARDATA_SET warDataSet;

	WAR_MakeWarDataSet( gWORLD[mapNum], &warDataSet );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SEND_WARSTATE, g_nPos );
	PutString( g_Packet, gWORLD[mapNum]->mapName, g_nPos );										// 맵이름
	PutString( g_Packet, GET_GUILD_DATA( gWORLD[mapNum]->ownerGuild )->guildName, g_nPos );		// 주인길드이름
	PutInteger( g_Packet, gWORLD[mapNum]->itemTax, g_nPos );									// 세율 
	PutInteger( g_Packet, gWORLD[mapNum]->TaxBank, g_nPos );									// 쌓인 세금

	int i;
	// 진지 관련 
	for( i = 0; i < 2; i++ )
	{
		if( warDataSet.camp[i] )
		{
			// 진지구축단
			if( IS_VALID_GUILD( warDataSet.camp[i]->ownerGuild ) )
			{
				PutByte( g_Packet, TYPE_CAMP, g_nPos );
				PutString( g_Packet, GET_GUILD_DATA( warDataSet.camp[i]->ownerGuild )->guildName, g_nPos );
				PutInteger( g_Packet, warDataSet.camp[i]->ownerPoint, g_nPos );
				PutByte( g_Packet, TYPE_CASTLE, g_nPos );
				PutString( g_Packet, GET_GUILD_DATA( gWORLD[mapNum]->ownerGuild )->guildName, g_nPos );

				sPWAR_DATA pTmpWarData = WAR_FindData( gWORLD[mapNum]->ownerGuild, warDataSet.camp[i]->ownerGuild );

				if( pTmpWarData )
					PutInteger( g_Packet, pTmpWarData->ownerPoint, g_nPos );
				else
					PutInteger( g_Packet, 0, g_nPos );
			}
			else
			{
				PutByte( g_Packet, TYPE_NONE, g_nPos );
				PutString( g_Packet, "", g_nPos );
				PutInteger( g_Packet, 0, g_nPos );
				PutByte( g_Packet, TYPE_NONE, g_nPos );
				PutString( g_Packet, "", g_nPos );
				PutInteger( g_Packet, 0, g_nPos );
			}
		}
		else
		{
			PutByte( g_Packet, TYPE_NONE, g_nPos );
			PutString( g_Packet, "", g_nPos );
			PutInteger( g_Packet, 0, g_nPos );
			PutByte( g_Packet, TYPE_NONE, g_nPos );
			PutString( g_Packet, "", g_nPos );
			PutInteger( g_Packet, 0, g_nPos );
		}
	}

	// 주변성 길드들
	for( i = 0; i < dMAX_LINKED_CASTLE; i++ )
	{
		if( warDataSet.linkedCastle[i] )
		{
			// 인접성 
			if( IS_VALID_GUILD( warDataSet.linkedCastle[i]->ownerGuild ) )
			{
				sPWAR_DATA pWarData = WAR_FindData2( gWORLD[mapNum]->ownerGuild, warDataSet.linkedCastle[i]->ownerGuild, dWARDATA_TYPE_CASTLE );
				
				if( pWarData )
				{
					PutByte( g_Packet, TYPE_CASTLE, g_nPos );
					PutString( g_Packet, GET_GUILD_DATA( warDataSet.linkedCastle[i]->ownerGuild )->guildName, g_nPos );
					PutInteger( g_Packet, warDataSet.linkedCastle[i]->ownerPoint, g_nPos );
					PutByte( g_Packet, TYPE_CASTLE, g_nPos );
					PutString( g_Packet, GET_GUILD_DATA( pWarData->ownerGuild )->guildName, g_nPos );
					PutInteger( g_Packet, pWarData->ownerPoint, g_nPos );
				}
				else
				{
					PutByte( g_Packet, TYPE_NONE, g_nPos );
					PutString( g_Packet, "", g_nPos );
					PutInteger( g_Packet, 0, g_nPos );
					PutByte( g_Packet, TYPE_NONE, g_nPos );
					PutString( g_Packet, "", g_nPos );
					PutInteger( g_Packet, 0, g_nPos );
				}
			}
			else
			{
				PutByte( g_Packet, TYPE_NONE, g_nPos );
				PutString( g_Packet, "", g_nPos );
				PutInteger( g_Packet, 0, g_nPos );
				PutByte( g_Packet, TYPE_NONE, g_nPos );
				PutString( g_Packet, "", g_nPos );
				PutInteger( g_Packet, 0, g_nPos );
			}
		}
		else
		{
			PutByte( g_Packet, TYPE_NONE, g_nPos );
			PutString( g_Packet, "", g_nPos );
			PutInteger( g_Packet, 0, g_nPos );
			PutByte( g_Packet, TYPE_NONE, g_nPos );
			PutString( g_Packet, "", g_nPos );
			PutInteger( g_Packet, 0, g_nPos );
		}

	}
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_GetTaxInfo
// Date: 
// Description: 
// 
BOOL PACKET_GetTaxInfo( sPDESC_DATA pPlayer )
{
#ifndef USE_GUILD_WAR
	return 1;
#endif

	sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];

	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[238] );
		return 1;
	}

	if( GET_GUILD( pPlayer ) != pWorld->ownerGuild )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[239] );
		return 1;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_OPEN_TAXWINDOW, g_nPos );
	PutInteger( g_Packet, pWorld->itemTax, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_ChangeTax
// Date: 
// Description: 
// 
BOOL PACKET_ChangeTax( sPDESC_DATA pPlayer )
{
#ifndef USE_GUILD_WAR
	return 1;
#endif

	sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];

	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
	{
		return 1;
	}

	if( GET_GUILD( pPlayer ) != pWorld->ownerGuild )
	{
		return 1;
	}

	int taxValue = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	if( taxValue < 5 || taxValue > 50 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[240] );
		return 1;
	}

	pWorld->itemTax = taxValue;

	UpdateCastleTaxValue( pWorld->mapNum, taxValue );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_TAX, g_nPos );
	PutInteger( g_Packet, pWorld->itemTax, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}

//
// Function Name: PACKET_EnchantChange
// Date: 
// Description: 
// 
BOOL PACKET_EnchantChange( sPDESC_DATA pPlayer )
{
#ifndef dDAUM_BILLING
	return 1;
#endif

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );

	if( !pItem )
		return 1;

	// 인첸트 카드가 아니면
	if( pItem->itemNum != dENCHANT_CARD_ARMOR &&
		pItem->itemNum != dENCHANT_CARD_WEAPON )
	{
		return 1;
	}

	BYTE haveChangeItem = 0;

	if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM40 ) )
	{
		haveChangeItem = 1;
	}

	if( !haveChangeItem )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM41 ) )
		{
			haveChangeItem = 1;
		}	
	}

	if( !haveChangeItem )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM42 ) )
		{
			haveChangeItem = 1;
		}	
	}

	// 인첸트 체인지 아이템 없음
	if( !haveChangeItem )
		return 1;
	
	sPITEM_DATA pChangeItem = NULL;

	if( pItem->itemNum == dENCHANT_CARD_ARMOR )
		pChangeItem = CreateItem( dENCHANT_CARD_WEAPON, "인첸트 체인지", __FILE__, __LINE__ );
	else if( pItem->itemNum == dENCHANT_CARD_WEAPON )
		pChangeItem = CreateItem( dENCHANT_CARD_ARMOR, "인첸트 체인지", __FILE__, __LINE__ );
	else
		return 1;

	if( !pChangeItem )
		return 1;

	// 시리얼 복사.
#ifdef USE_ITEM_SERIAL
	strcpy( pChangeItem->serial, pItem->serial );
#endif

	sCHECK_INVEN checkInven;

	checkInven.page = pItem->invenPage;
	checkInven.x = pItem->invenX;
	checkInven.y = pItem->invenY;
	
	ItemFromInventory( pPlayer, pItem );
	ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_ENCHANT_CHANGE1 );
	INSERT_ITEM_TO_MEMORY( pItem );

	ItemToInventory( pPlayer, pChangeItem, &checkInven );
	ITEMLOG_ItemLog( pPlayer, NULL, pChangeItem, ITEMLOG_ACT_ENCHANG_CHANGE2 );

	SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL12 );

	return 1;
}


BOOL PACKET_CardUndo( sPDESC_DATA pPlayer )
{
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );
	
	if( !pItem )
		return 1;

	sCHECK_INVEN checkInven;

	if( !CheckEmptyInven( pPlayer, &checkInven ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[19] );
		return 1;
	}

	if( GET_MONEY( pPlayer ) < 10000  )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[157], 10000 );
		return 1;
	}
	
	// 인첸트 카드가 아니면
	if( pItem->itemNum != 12097 &&
		pItem->itemNum != 12098 )
	{
		return 1;
	}
	
	sPITEM_DATA pChangeItem = NULL;
	
	if( pItem->itemNum == 12097 )
	{
		if( pItem->exVal[0] <= 0 )
		{
			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
			pChangeItem = CreateItem( 10193, "카드박스 풀기", __FILE__, __LINE__ );			
		}		
		else
		{
			pItem->exVal[0]--;
			if( pItem->exVal[0] == 0 )
			{
				ItemFromInventory( pPlayer, pItem );
				INSERT_ITEM_TO_MEMORY( pItem );
				pChangeItem = CreateItem( 10193, "카드박스 풀기", __FILE__, __LINE__ );			
			}
			else
			{
			SendUpdateEnchantCnt( pPlayer, pItem );
			pChangeItem = CreateItem( 10193, "카드박스 풀기", __FILE__, __LINE__ );			
			}
		}
	}
	else if( pItem->itemNum == 12098 )
	{
		if( pItem->exVal[0] <= 0 )
		{
			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
			pChangeItem = CreateItem( 10194, "카드박스 풀기", __FILE__, __LINE__ );			
		}		
		else
		{
			pItem->exVal[0]--;
			if( pItem->exVal[0] == 0 )
			{
				ItemFromInventory( pPlayer, pItem );
				INSERT_ITEM_TO_MEMORY( pItem );
				pChangeItem = CreateItem( 10194, "카드박스 풀기", __FILE__, __LINE__ );			
			}
			else
			{
				SendUpdateEnchantCnt( pPlayer, pItem );
				pChangeItem = CreateItem( 10194, "카드박스 풀기", __FILE__, __LINE__ );			
			}
		}
	}
	else
		return 1;
	
	if( !pChangeItem )
		return 1;
	
	// 시리얼 복사.
#ifdef USE_ITEM_SERIAL
	strcpy( pChangeItem->serial, pItem->serial );
#endif
	GET_MONEY( pPlayer ) -= 10000;
	DATASERV_SendUpdateMoney( pPlayer );
	UpdateMoney( pPlayer );
	
//	ItemFromInventory( pPlayer, pItem );
	ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_ENCHANT_CHANGE1 );
//	INSERT_ITEM_TO_MEMORY( pItem );
	
	ItemToInventory( pPlayer, pChangeItem, &checkInven );
	ITEMLOG_ItemLog( pPlayer, NULL, pChangeItem, ITEMLOG_ACT_ENCHANG_CHANGE2 );
	
	SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL12 );
	
	return 1;
}

//
// Function Name: PACKET_EnchantCollection
// Date: 
// Description: 
//
BOOL PACKET_EnchantCollection( sPDESC_DATA pPlayer )
{
#ifndef dDAUM_BILLING
	return 1;
#endif
	
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );

	if( !pItem )
		return 1;

	if( GET_ITEM_UNIQ( pItem ) < 1 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[241] );
		return 1;
	}

	if( pItem->itemNum == dRIDE_ITEM1 ||
		pItem->itemNum == dRIDE_ITEM2 ||
		pItem->itemNum == dRIDE_ITEM3 )
		return 1;

	BYTE haveBillItem = 0;

	if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM43 ) )
	{
		haveBillItem = 1;
	}

	if( !haveBillItem )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM44 ) )
		{
			haveBillItem = 1;
		}
	}

	if( !haveBillItem )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM45 ) )
		{
			haveBillItem = 1;
		}
	}

	if( !haveBillItem )
	{
		return 1;
	}

	sCHECK_INVEN checkInven;

	if( !CheckEmptyInven( pPlayer, &checkInven ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[242] );
		return 1;
	}

	sPITEM_DATA pCardItem = NULL;

	if( IS_ARMOR( GET_ITEM_TYPE( pItem ) ) )
	{
		pCardItem = CreateItem( dENCHANT_CARD_ARMOR, "Enchant Collection", __FILE__, __LINE__ );
	}
	else if( IS_WEAPON( GET_ITEM_TYPE( pItem ) ) )
	{
		pCardItem = CreateItem( dENCHANT_CARD_WEAPON, "Enchant Collection", __FILE__, __LINE__ );
	}
	else
	{
		return 1;
	}

	if( !pCardItem )
		return 1;

	sPITEM_DATA pDownItem = CreateItem( pItem->itemNum-1, "Enchant Collection(2)", __FILE__, __LINE__ );
	
	if( !pDownItem )
	{
		INSERT_ITEM_TO_MEMORY( pCardItem );
		return 1;
	}

	pDownItem->invenPage = pItem->invenPage;
	pDownItem->invenX = pItem->invenX;
	pDownItem->invenY = pItem->invenY;

	// 기존 인첸트 아이템 없애기 
	ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_ENCHANT_COLLECT1 );
	ItemFromInventory( pPlayer, pItem );
	INSERT_ITEM_TO_MEMORY( pItem );

	// - 된 아이템으로 바꿔 넣기
	ItemToInventory( pPlayer, pDownItem, pDownItem->invenPage, pDownItem->invenX, pDownItem->invenY );
	ITEMLOG_ItemLog( pPlayer, NULL, pDownItem, ITEMLOG_ACT_ENCHANT_COLLECT2 );

	// 인첸트 카드 넣기
	ItemToInventory( pPlayer, pCardItem, &checkInven );
	ITEMLOG_ItemLog( pPlayer, NULL, pCardItem, ITEMLOG_ACT_ENCHANT_COLLECT2 );	

	SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL13 );

	return 1;
}

//
// Function Name: PACKET_ItemLook
// Date: 
// Description: 
// 
BOOL PACKET_ItemLook( sPDESC_DATA pPlayer )
{
#ifndef dDAUM_BILLING
	return 1;
#endif

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPDESC_DATA pTarget = FindPlayerInArea( GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ) / dONE_TILE, GET_POSY( pPlayer ) / dONE_TILE, idxNum );
	
	if( !pTarget )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[243] );
		return 1;
	}
	
	BYTE haveItemLook = 0;
	
	if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM46 ) )
	{
		haveItemLook = 1;
	}
	
	if( !haveItemLook )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM47 ) )
			haveItemLook = 1;
	}
	
	if( !haveItemLook )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM48 ) )
			haveItemLook = 1;
	}
	
	// 아이템 룩 아이템 없음
	if( !haveItemLook )
	{
		return 1;
	}
	
	BYTE targetHaveProtect = 0;
	
	if( UseOtherInvenItem( pTarget, dBILL_ITEM_NUM49 ) )
	{
		targetHaveProtect = 1;
	}
	
	if( !targetHaveProtect )
	{
		if( UseOtherInvenItem( pTarget, dBILL_ITEM_NUM50 ) )
			targetHaveProtect = 1;
	}
	
	if( !targetHaveProtect )
	{
		if( UseOtherInvenItem( pTarget, dBILL_ITEM_NUM51 ) )
			targetHaveProtect = 1;
	}
	
	if( targetHaveProtect )
	{
		SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL15 );
		SendSystemMsg( pPlayer, g_LANG_STR[244] );
		return 1;
	}
	StrSendSysMsg( pPlayer, 245, GET_NAME( pTarget ) );
	//SendSystemMsg( pPlayer, g_LANG_STR[245], GET_NAME( pTarget ) );


	if( pTarget->equip[dEQUIP_NECK] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[246], GET_ITEM_HNAME( pTarget->equip[dEQUIP_NECK] ) );
	}
	if( pTarget->equip[dEQUIP_BOW] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[247], GET_ITEM_HNAME( pTarget->equip[dEQUIP_BOW] ) );
	}
	if( pTarget->equip[dEQUIP_GLOVE] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[248], GET_ITEM_HNAME( pTarget->equip[dEQUIP_GLOVE] ) );
	}
	if( pTarget->equip[dEQUIP_WEAPON] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[249], GET_ITEM_HNAME( pTarget->equip[dEQUIP_WEAPON] ) );
	}
	if( pTarget->equip[dEQUIP_HELMET] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[250], GET_ITEM_HNAME( pTarget->equip[dEQUIP_HELMET] ) );
	}
	if( pTarget->equip[dEQUIP_JACKET] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[251], GET_ITEM_HNAME( pTarget->equip[dEQUIP_JACKET] ) );
	}
	if( pTarget->equip[dEQUIP_BELT] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[252], GET_ITEM_HNAME( pTarget->equip[dEQUIP_BELT] ) );
	}
	if( pTarget->equip[dEQUIP_PANTS] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[253], GET_ITEM_HNAME( pTarget->equip[dEQUIP_PANTS] ) );
	}
	if( pTarget->equip[dEQUIP_EARRING] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[254], GET_ITEM_HNAME( pTarget->equip[dEQUIP_EARRING] ) );
	}
	if( pTarget->equip[dEQUIP_RING] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[255], GET_ITEM_HNAME( pTarget->equip[dEQUIP_RING] ) );
	}
	if( pTarget->equip[dEQUIP_SHIELD] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[256], GET_ITEM_HNAME( pTarget->equip[dEQUIP_SHIELD] ) );
	}
	if( pTarget->equip[dEQUIP_SHOES] )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[257], GET_ITEM_HNAME( pTarget->equip[dEQUIP_SHOES] ) );
	}
	
	SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL14 );
	
	return 1;
}

//
// Function Name: PACKET_LevelLook
// Date: 
// Description: 
// 
BOOL PACKET_LevelLook( sPDESC_DATA pPlayer )
{
#ifndef dDAUM_BILLING
	return 1;
#endif

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pTarget = FindPlayerInArea( GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ) / dONE_TILE, GET_POSY( pPlayer ) / dONE_TILE, idxNum );


	if( !pTarget )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[243] );
		return 1;
	}

	BYTE haveLevelLook = 0;

	if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM52 ) )
	{
		haveLevelLook = 1;
	}

	if( !haveLevelLook )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM53 ) )
			haveLevelLook = 1;
	}

	if( !haveLevelLook )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM54 ) )
			haveLevelLook = 1;
	}

	// 레벨룩 아이템 없음
	if( !haveLevelLook )
	{
		return 1;
	}

	BYTE targetHaveProtect = 0;

	if( UseOtherInvenItem( pTarget, dBILL_ITEM_NUM55 ) )
	{
		targetHaveProtect = 1;
	}

	if( !targetHaveProtect )
	{
		if( UseOtherInvenItem( pTarget, dBILL_ITEM_NUM56 ) )
			targetHaveProtect = 1;
	}

	if( !targetHaveProtect )
	{
		if( UseOtherInvenItem( pTarget, dBILL_ITEM_NUM57 ) )
			targetHaveProtect = 1;
	}
	
	if( targetHaveProtect )
	{
		SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL17 );
		SendSystemMsg( pPlayer, g_LANG_STR[258] );
		return 1;
	}

//	SendSystemMsg( pPlayer, g_LANG_STR[259], GET_NAME( pTarget ) );
	StrSendSysMsg( pPlayer, 259, GET_NAME( pTarget ) );
	SendSystemMsg( pPlayer, g_LANG_STR[260], 
		GET_SATTACK( pTarget ), GET_LATTACK( pTarget ), GET_BMAGIC( pTarget ), 
		GET_WMAGIC( pTarget ), GET_STR( pTarget ), GET_DEX( pTarget ), GET_INT( pTarget ) );

	SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL16 );

	return 1;
}

//
// Function Name: PACKET_GetBillingItems
// Date: 
// Description: 
// 
BOOL PACKET_GetBillingItems( sPDESC_DATA pPlayer )
{
#ifndef dDAUM_BILLING
	return 1;
#endif

#ifdef IS_TEST_SERVER
	return 1;
#endif

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_BILLING_ITEM_INFO, g_nPos );

	WORD *cnt = (WORD*)( g_Packet + g_nPos );
	g_nPos += sizeof( WORD );
	*cnt = 0;
	
	// 현재 사용중인 유료 아이템
	
	//dBILL_ITEM_NUM1	dBILL_ITEM_EFFECT1	11907	웜쓰오브 아라임
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] )
	{	
		PutInteger( g_Packet, dBILL_ITEM_NUM1, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM4	dBILL_ITEM_EFFECT2	11910	웜쓰오브 아라임
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM4, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM16	dBILL_ITEM_EFFECT3	11925	아이템 오브 아라임
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM16, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM19	dBILL_ITEM_EFFECT4	11928	아이템 오브 아라임
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM19, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM28	dBILL_ITEM_EFFECT5	11937	프리미엄 존 서비스
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM28, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM31	dBILL_ITEM_EFFECT6	11940	프리미엄 존 플러스
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM31, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM37	dBILL_ITEM_EFFECT7	11946	신의축복
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM37, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM58	dBILL_ITEM_EFFECT8	11967	기동성의 물약
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM58, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM61	dBILL_ITEM_EFFECT9	11970	웨이트 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM61, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM64	dBILL_ITEM_EFFECT10	11973	HP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM64, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM67	dBILL_ITEM_EFFECT11	11976	MP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM67, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM70	dBILL_ITEM_EFFECT12	11913	그룹 윔쓰 오브 아라임
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM70, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM73	dBILL_ITEM_EFFECT13	11979	그룹 HP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM73, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM76	dBILL_ITEM_EFFECT14	11982	그룹 MP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM76, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM79	dBILL_ITEM_EFFECT15	11985	군단원 HP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM79, g_nPos );
		*cnt += 1;
	}
	
	//dBILL_ITEM_NUM82	dBILL_ITEM_EFFECT16	11988	군단원 MP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM82, g_nPos );
		*cnt += 1;
	}
	// 마이너스
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] )
	{
		PutInteger( g_Packet, dBILL_ITEM_NUM110, g_nPos );
		*cnt += 1;
	}
	
	cnt = (WORD*)( g_Packet + g_nPos );
	g_nPos += 2;
	*cnt = 0;
	
	sPBILL_ITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->billItems, item, next_item, next, WMgr );

	PutInteger( g_Packet, item->idxNum, g_nPos );
	PutInteger( g_Packet, item->itemNum, g_nPos );
	PutInteger( g_Packet, item->cnt, g_nPos );

	*cnt += 1;
	LIST_WHILEEND( pPlayer->billItems, item, next_item );

	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}


//
// Function Name: PACKET_GetBillingItems
// Date: 
// Description: 
// 
BOOL PACKET_UseBillItem( sPDESC_DATA pPlayer )
{
#ifdef IS_TEST_SERVER
	return 1;
#endif
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPBILL_ITEM_DATA pBillItem = FindBillItemByIdx( pPlayer, idxNum );

	if( !pBillItem )
		return 1;

	// DB Check 
	char query[dMAX_QUERY_SIZE];
	
	sprintf( query, "select * from PurSell where PurKey = '%s' and PurFlag = 0 limit 1", pBillItem->PurKey );

	if( mysql_query( g_BILL_SQL, query ) )
	{
              log( "%s" ,mysql_error( g_BILL_SQL ) );
		return 1;
	}
	else
	{
		MYSQL_ROW row;
		MYSQL_RES *result;

		result = mysql_store_result( g_BILL_SQL );
		
		if( result )
		{			
			if( ( row = mysql_fetch_row( result ) ) )
			{
				sCHECK_INVEN checkInven;

				if( !CheckEmptyInven( pPlayer, &checkInven ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[242] );
					mysql_free_result( result );					
					return 1;
				}

				sPITEM_DATA pItem = CreateItem( pBillItem->itemNum, "유료아이템 생성", __FILE__, __LINE__ );

				if( !pItem )
				{
					mysql_free_result( result );					
					return 1;
				}

				sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", pBillItem->PurKey );
				
				if( mysql_query( g_BILL_SQL, query ) )
				{
					INSERT_ITEM_TO_MEMORY( pItem );

					log( "%s" ,mysql_error( g_BILL_SQL ) );

					mysql_free_result( result );					
					return 1;
				}

				pItem->exVal[0] = MIN( pBillItem->cnt, 1 );

				ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_IN );
				ItemToInventory( pPlayer, pItem, &checkInven );

				g_nPos = 2;
				PutWord( g_Packet, dPACKET_REMOVE_BILLING_ITEM, g_nPos );
				PutInteger( g_Packet, pBillItem->idxNum, g_nPos );
				PutSize( g_Packet, g_nPos );
				SendData( pPlayer, g_Packet, g_nPos );
		
				//
				REMOVE_FROM_LIST( pPlayer->billItems, pBillItem, prev, next );
		

				mysql_free_result( result );

				return 1;
			}
			mysql_free_result( result );
		}
	}

	SendSystemMsg( pPlayer, g_LANG_STR[261] );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_REMOVE_BILLING_ITEM, g_nPos );
	PutInteger( g_Packet, pBillItem->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	REMOVE_FROM_LIST( pPlayer->billItems, pBillItem, prev, next );

	return 1;
}

BOOL	PACKET_Billing_Disconnect( sPDESC_DATA pPlayer )
{
	//OutBillingInfo( GET_ID(pPlayer), 1 );
	return 1;
}

BOOL PACKET_InputSmithItem( sPDESC_DATA pPlayer )
{
    /*if( !IsSmithArea( pPlayer ) )
		return 1;*/

	if( pPlayer->smithCnt > 1 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[280] );
		return 1;
	}
	
	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );
	
	if( pItem )
	{
		if( pItem->itemNum == dMAY_BOX_ITEM )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[20] );
			return 1;
		}
		
		if( GET_RIDE_STATE( pPlayer ) && ( pItem->itemNum == dRIDE_ITEM1 || pItem->itemNum == dRIDE_ITEM2 || pItem->itemNum == dRIDE_ITEM3 ) ) 
		{
			SendSystemMsg( pPlayer, g_LANG_STR[128] );
			return 1;
		}
		
		ItemToSmith( pPlayer, pItem, GET_MONEY( pPlayer ) );	
	}
	
	return 1;
}

BOOL PACKET_SmithFinish( sPDESC_DATA pPlayer )
{
	ClearSmith( pPlayer );
	return 1;
}

BOOL PACKET_RemoveSmithItem( sPDESC_DATA pPlayer )
{
	if( pPlayer->smithCnt <= 0 )
	{
		SendSystemMsg( pPlayer, "아이템이 없습니다." );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPITEM_DATA pItem = FindItemFromInven( pPlayer, idxNum );

	if( pItem )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_SMITH_ITEMDELED, g_nPos );
		PutInteger( g_Packet, pItem->idxNum, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		ItemFromSmith( pPlayer, pItem );
	}	

	return 1;
}
BOOL PACKET_FriendLoginRequest( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name );
	
	if( pFriend )
	{
		data->isOnline = 1;
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_LOGIN, g_nPos );
		PutString( g_Packet, GET_NAME(pFriend) , g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );

		SendFriendMsg( pPlayer, g_LANG_STR[288], data->name );
	}
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
	return 1;
}


BOOL PACKET_FriendLogOutRequest( sPDESC_DATA pPlayer )
{
	sPFRIEND_DATA data, next_data;
	
	LIST_WHILE( pPlayer->ch2.friendList, data, next_data, next, WMgr );
	sPDESC_DATA pFriend = FindPlayerNameList( data->name );

	if( pFriend )
	{
		data->isOnline = 0;
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_FRIEND_LOGOUT, g_nPos );
		PutString( g_Packet, GET_NAME( pPlayer ) , g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pFriend, g_Packet, g_nPos );
		SendFriendMsg( pFriend, g_LANG_STR[289], GET_NAME( pPlayer ) );
	}
	LIST_WHILEEND( pPlayer->ch2.friendList, data, next_data );
	return 1;
}

BOOL PACKET_Friend( sPDESC_DATA pPlayer )
{
#ifdef dNOT_USE_FRIEND
	return 1;
#endif
	
	if( GET_FRIEND_CNT( pPlayer ) >= dMAX_FRIEND )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[107], dMAX_FRIEND );
		return 1;
	}

	int idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPDESC_DATA pFriend;
	
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pFriend = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pFriend = FindPlayerInArea( pPlayer->ch.mapNum, pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}
	
	return 1;
}


BOOL PACKET_FriendAskInvit( sPDESC_DATA pPlayer )
{
	int idxNum = 0;
	
	idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pInvitePlayer = NULL;
	
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pInvitePlayer = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pInvitePlayer = FindPlayerInArea( pPlayer->ch.mapNum, 
			pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}
	
	if( !pInvitePlayer )
		return 1;
	
	//... .. .... ....~
	if( !strcmp( GET_NAME( pPlayer ), GET_NAME( pInvitePlayer ) ) )
	{
		SendSystemMsg( pPlayer, "You can't register yourself." );
		return 1;
	}

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ) 
	{
		SendSystemMsg( pPlayer, "You can't register DeadMan." );
		return 1;
	}
	
	//....
	//if( pPlayer->ch2.friendCutOff )
	if( IS_SET( pInvitePlayer->ch2.flag, dPLAYER_FLG_DENY_FRIEND ) )
	{
		SendSystemMsg( pPlayer, "Block Friends" );
		return 1;
	}

	//... ......... ....!
	if( FRIENDS_FindData( pPlayer, pInvitePlayer ) )
	{
		SendSystemMsg( pPlayer, "Your Friends" );
		return 1;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_FRIEND_ASKYESNO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pInvitePlayer, g_Packet, g_nPos );
	
	return 1;
}

BOOL PACKET_FriendJoinYes( sPDESC_DATA pPlayer )
{
	if( GET_FRIEND_CNT( pPlayer ) >= dMAX_FRIEND )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[107], dMAX_FRIEND );
		return 1;
	}

	int	idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );
	
	sPDESC_DATA pInvitePlayer = NULL;
	
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pInvitePlayer = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pInvitePlayer = FindPlayerInArea( pPlayer->ch.mapNum, 
			pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}
	
	if( !pInvitePlayer )
		return 1;
	
	if( !strcmp( GET_NAME( pPlayer ), GET_NAME( pInvitePlayer ) ) )
		return 1;
	
	if( strlen( GET_NAME( pInvitePlayer ) ) >= dNAME_LEN )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[75] );
		return 1;
	}

//	FRIEND_AddFriend( pPlayer, GET_NAME( pInvitePlayer ) ); 
//	FRIEND_OnlineCheck( pPlayer );
	FRIENDS_AddFriend( pPlayer, pInvitePlayer );
	FRIENDS_OnlineCheck( pPlayer );
	
	return 1;
}

BOOL PACKET_FriendJoinNo( sPDESC_DATA pPlayer )
{
	int	idxNum = GetInteger( pPlayer->TemprecvBuff, pPlayer->recvPos );

	sPDESC_DATA pInvitePlayer = NULL;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		pInvitePlayer = FindPlayerInHouse( pPlayer, idxNum );
	}
	else
	{
		pInvitePlayer = FindPlayerInArea( pPlayer->ch.mapNum, 
			pPlayer->ch.posX/dONE_TILE, pPlayer->ch.posY/dONE_TILE , idxNum );
	}
	
	if( !pInvitePlayer )
		return 1;
	
	//SendSystemMsg( pInvitePlayer, "%s님이 친구등록에 응하지 않았습니다.","%s has denied your friend request.", GET_NAME( pPlayer ) );
	SendSystemMsg( pInvitePlayer, "%s has denied your friend request.", GET_NAME( pPlayer ) );	
	return 1;
}

BOOL PACKET_CharacterDisconn( sPDESC_DATA pPlayer )
{
//	if( !pPlayer->disconnectFlag )
		pPlayer->disconnectFlag = 1;

	return 1;
}

//
// Function Name: MessageFuncsList
// Date: 2003-02-12
// Description: 패킷 처리 함수들의 포인터 배열
//
BOOL (*MessageFuncsList[])( sPDESC_DATA pPlayer ) =
{
	PACKET_AliveCheck,		//#define dPACKET_RESERVED			0	// 사용 안함
	PACKET_LoginInfo,		//#define dPACKET_LOGIN			1	// 로그인 ID, Password
	PACKET_NullFunc,		//#define dPACKET_LOGINRESULT		2	// 로그인 결과 
	PACKET_NullFunc,		//#define dPACKET_CHARINFO			3	// 케릭터 정보
	PACKET_NullFunc,		//#define dPACKET_GAMESTART			4	// 게임 시작
	PACKET_NullFunc,		//#define dPACKET_MAPLOADING		5	// 맵 로딩
	PACKET_EndMapLoading,	//#define dPACKET_MAPLOADING_END		6	// 맵 로딩 끝

	PACKET_NullFunc,		//#define dPACKET_CHARACTER_INSERT		7	// 케릭터 추가
	PACKET_NullFunc,		//#define dPACKET_CHARACTER_REMOVE		8	// 케릭터 삭제

	PACKET_NullFunc,		//#define dPACKET_MAGIC_FAILED		9	// 마법 실패
	PACKET_NullFunc,		//#define dPACKET_CHARACTER_DISCONN	10	// 케릭터 삭제 ( 접속 해제시 )

	PACKET_CharacterMove,	//#define dPACKET_CHARACTER_MOVE		11	// 케릭터 이동
	PACKET_NullFunc,		//#define dPACKET_CHARACTER_DIRECTION	12	// 케릭터 바라봄

	PACKET_NullFunc,		//#define dPACKET_MONSTER_INSERT		13	// 몬스터 추가
	PACKET_NullFunc,		//#define dPACKET_MONSTER_REMOVE		14	// 몬스터 제거
	PACKET_NullFunc,		//#define dPACKET_MONSTER_MOVE		15	// 몬스터 이동

	PACKET_ChatingProc,		//#define dPACKET_CHAT_MSG			16	// 채팅
	
	PACKET_AttackMonster,	//#define dPACKET_ATTACK_USERMOB		17     // 캐릭터 -> 몹 공격
	PACKET_NullFunc,		//#define dPACKET_ATTACK_MOBUSER		18     // 몹     -> 캐릭터 공격
	PACKET_NullFunc,		//#define dPACKET_MOB_HPINFO		19	// 몬스터 체력 정보
	PACKET_NullFunc,		//#define dPACKET_DAMAGE_USER		20     // 캐릭터 피공 
	PACKET_NullFunc,		//#define dPACKET_DAMAGE_MOB		21     // 몹 피공

	PACKET_NullFunc,		//#define dPACKET_DIE_USER			22     // 유저의 죽음
	PACKET_NullFunc,		//#define dPACKET_DIE_MOB			23     // 몹의 죽음
	PACKET_RestartOption,	//#define dPACKET_RESTART			24	// 케릭터 죽었을때 재시작 옵션
	PACKET_WhisperProc,		//#define dPACKET_CHAT_WHISPER		25	// 귓속말
	PACKET_NullFunc,		//#define dPACKET_CHAT_WHISPER_RES		26	// 귓속말 결과
	
	PACKET_NullFunc,		//#define dPACKET_SKILL_UP			27	// 스킬 레벨 업 
	PACKET_NullFunc,		//#define dPACKET_GET_EXP			28	// 경험치 획득 메시지
	PACKET_NullFunc,		//#define dPACKET_SKILLUP_EVENT		29	// 레벨업 이벤트

	PACKET_NullFunc,		//#define dPACKET_HPGAIN_USER		30	// 유져 HP 회복
	PACKET_NullFunc,		//#define dPACKET_MPGAIN_USER		31	// 유져 MP 회복

	PACKET_NullFunc,		//#define dPACKET_HPGAIN_MOB		32	// 몹 HP 회복

	PACKET_GetAllCharacterInfo,	//#define dPACKET_GETALLCHAR		33	// 모든 케릭터 기본정보 
	PACKET_NullFunc,		//#define dPACKET_GETALLCHAR_RESULT	34	// 모든 케릭터 정보 보내기 
	PACKET_SelectCharacter,	//#define dPACKET_SELECT_CHARACTER		35		// 케릭터 선택
	PACKET_NullFunc,		//#define dPACKET_NULL_CHARACTER		36		// 케릭터 없음
	PACKET_MakeCharProc,		//#define dPACKET_MAKECHAR			37		// 신규 케릭터 생성
	PACKET_NullFunc,		//#define dPACKET_MAKECHAR_RESULT		38		// 신규 케릭 생성 결과

	PACKET_DeleteCharacter,	//#define dPACKET_DELETE_CHARACTER		39		// 케릭터 삭제 
	PACKET_NullFunc,		//#define dPACKET_DELETE_CHARACTER_RESULT	40		// 케릭터 삭제 결과

	PACKET_NullFunc,		//#define dPACKET_INSERT_MAPITEM		41		// 맵에 아이템 나타나기
	PACKET_NullFunc,		//#define dPACKET_REMOVE_MAPITEM		42		// 맵에서 아이템 사라지기
	PACKET_NullFunc,		//#define dPACKET_INSERT_INVENTORY		43		// 케릭터한테 아이템 주기
	PACKET_NullFunc,		//#define dPACKET_REMOVE_INVENTORY		44		// 케릭터한테서 아이템 뺏기
	PACKET_MoveInvenItem,	//#define dPACKET_MOVE_ITEM			45		// 아이템 이동 ( 인벤토리 )
	PACKET_DropItem,		//#define dPACKET_DROP_ITEM			46		// 아이템 버리기
	PACKET_GetItem,		//#define dPACKET_GET_ITEM			47		// 아이템 줍기
	PACKET_NullFunc,		//#define dPACKET_INSERT_EQUIP_ITEM	48		// 아이템 장비에 넣기
	PACKET_NullFunc,		//#define dPACKET_REMOVE_EQUIP_ITEM	49		// 아이템 장비에서 빼기
	PACKET_NullFunc,		//#define dPACKET_EQUIP_ITEM		50		// 아이템 장착 ( 클라이언트 )
	PACKET_NullFunc,		//#define dPACKET_CHANGE_HAND_ITEM		51		// 손에 쥔 아이템 바꾸기 ( 클라이언트 )

	// 임시
	PACKET_NullFunc,		//#define dPACKET_SET_MAIN_SKILL		52		// 메인 스킬 바꾸기
	PACKET_NullFunc,		//#define dPACKET_CHANGE_MAIN_SKILL	53		// 메인 스킬 바꾸기

	PACKET_NullFunc,		//#define dPACKET_ITEM_TO_HAND		54		// 아이템 손으로 
	PACKET_NullFunc,		//#define dPACKET_ITEM_FROM_HAND		55		// 아이템 손에서 빼기
	PACKET_NullFunc,		//#define dPACKET_HOLDING_ITEM		56		// 아이템 손에 쥐기 ( 클라이언트 )

	PACKET_NullFunc,		//#define dPACKET_SET_INVENXY		57
	
	PACKET_NullFunc,		//#define dPACKET_ALREADY_CONNECTED	58		// 이미 접속중인 아이디
	PACKET_NullFunc,		//#define dPACKET_UPDATE_MAXHPMP		59		// MP, HP Up

	PACKET_NullFunc,		//#define dPACKET_PLAYERS_ITEM_INFO	60		// 아이템 정보 전송 (소지품, 장비)
	PACKET_NullFunc,		//#define dPACKET_CHANGE_EQUIP		61		// 장비 바뀜

	PACKET_NullFunc,		//#define dPACKET_REMOVE_SPELLBOOK		62
	PACKET_NullFunc,		//#define dPACKET_MOVETO_SPELLBOOK		63		// 클라이언트->서버

	PACKET_NullFunc,		//#define dPACKET_CHANGE_TIME		64		// 시간 변화
	PACKET_NullFunc,		//#define dPACKET_UPDATE_MONEY		65		// 돈 업데이트
	PACKET_SendShopItemList,	//#define dPACKET_GET_SHOPITEM		66		// 상점의 판매목록 받기 (클->서)
	PACKET_NullFunc,		//#define dPACKET_SHOPITEM_LIST		67		// 상점 판매 목록 (서->클)
	PACKET_SellItem,		//#define dPACKET_SELL_ITEM			68		// 아이템 팔기 (클->서)
	PACKET_BuyItem,		//#define dPACKET_BUY_ITEM			69		// 아이템 사기 (클->서)
	PACKET_NullFunc,		//#define dPACKET_SHOP_RESULT		70		// 상점 거래 결과

	PACKET_NullFunc,		//#define dPACKET_BEGIN_PLAYER_ATTACK	71		// 공격 시작 패킷 (서버->클라)
	PACKET_EndPlayerAttack,	//#define dPACKET_END_PLAYER_ATTACK	72		// 공격 종료 패킷 (클라->서버)
	PACKET_CastItem,		//#define dPACKET_CAST_ITEM			73		// 아이템 사용 (클라->서버)

	PACKET_DealRequest,		//#define dPACKET_DEAL_REQUEST		74		// 거래 요청
	PACKET_NullFunc,		//#define dPACKET_DEAL_BEGIN		75		// 거래 시작
	PACKET_NullFunc,		//#define dPACKET_DEAL_FINISH		76		// 거래 종료		
	PACKET_DealCancel,		//#define dPACKET_DEAL_CANCEL		77		// 거래 취소

	PACKET_DealAddItem,		//#define dPACKET_DEAL_ADDITEM		78		// 거래 아이템 추가 (클라->서버)
	PACKET_DealDelItem,		//#define dPACKET_DEAL_DELITEM		79		// 거래 아이템 삭제 (클라->서버)
	PACKET_NullFunc,		//#define dPACKET_DEAL_INSERT_ITEM		80		// 거래 아이템 추가 (서버->클라)
	PACKET_NullFunc,		//#define dPACKET_DEAL_REMOVE_ITEM		81		// 거래 아이템 삭제 (서버->클라)

	PACKET_DealAddMoney,		//#define dPACKET_DEAL_ADDMONEY		82		// 거래창에 돈 추가
	PACKET_NullFunc,		//#define dPACKET_DEAL_UPDATEMONEY		83		// 거래창의 돈 정보 (서버->클라)
	PACKET_DealCheckIn,		//#define dPACKET_DEAL_CHECKIN		84		// 거래창에 아이템 다 올려놨음
	PACKET_NullFunc,		//#define dPACKET_DEAL_REQUEST_SUBMIT	85		// 최종 거래 수락 요청(서버->클라)
	PACKET_DealSubmitOk,		//#define dPACKET_DEAL_SUBMIT_OK		86		// 최종 거래 수락(클라->서버)

	PACKET_PartyCreate,		//#define dPACKET_PARTY_CREATE		87		// 파티 만들기 (클라->서버)
	PACKET_NullFunc,		//#define dPACKET_PARTY_CREATE_OK		88		// 파티 만들기 OK(서버->클라)
	PACKET_PartyDestroy,		//#define dPACKET_PARTY_DESTROY		89		// 파티 뽀개기 (클라->서버)
	PACKET_NullFunc,		//#define dPACKET_PARTY_DESTROYED		90		// 파티 뽀개기 (서버->클라)
	PACKET_PartyJoin,		//#define dPACKET_PARTY_JOIN		91		// 파티 조인 ( 클라->서버 )
	PACKET_NullFunc,		//#define dPACKET_PARTY_JOIN_RESULT	92		// 파티 조인 결과 ( 서버->클라 )
	PACKET_NullFunc,		//#define dPACKET_PARTY_ADDMEMBER		93		// 파티원 추가 (서버->클라)
	PACKET_PartyOut,		//#define dPACKET_PARTY_OUT			94		// 파티에서 나감 (클라->서버)
	PACKET_NullFunc,		//#define dPACKET_PARTY_DELMEMBER		95		// 파티원 삭제 (서버->클라)
	PACKET_PartyRemoveMember,	//#define dPACKET_PARTY_REMOVE_MEMBER	96		// 맴버 강퇴 (파티장용) (클라->서버)
	PACKET_NullFunc,		//#define dPACKET_PARTY_MEMBER_HPMP	97		// 멤버들의 체력 정보
	PACKET_NullFunc,		//#define dPACKET_PARTY_MEMBER_MAPINFO	98		// 멤버들의 맵상의 위치정보
	
	PACKET_NullFunc,		//#define dPACKET_UPDATE_POINT		99		// STR, INT, DEX 변경
	PACKET_NullFunc,		//#define dPACKET_UPDATE_LEVEL		100		// 통합 레벨 변경

	PACKET_NullFunc,		//#define dPACKET_SET_ATTACKRANGE		101		// 공격 가능 거리

	PACKET_NullFunc,		//#define dPACKET_ADD_HERO			102		// 화면에 영웅 추가 
	PACKET_NullFunc,		//#define dPACKET_REMOVE_HERO		103		// 화면에서 영웅 빼기

	PACKET_NullFunc,		//#define dPACKET_EQUIP_INFO		104		// 장비 정보
	PACKET_NullFunc,		//#define dPACKET_MAGIC_INFO		105		// 마법 정보
	
	PACKET_MagicCast,		//#define dPACKET_MAGIC_CAST		106		// 마법 사용 (클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_MAGIC_CAST_USER		107		// 마법 사용 (서버->클라이언트)

	PACKET_NullFunc,		//#define dPACKET_ADD_EFFECT		108		// 마법 이펙트 추가
	PACKET_NullFunc,		//#define dPACKET_REMOVE_EFFECT		109		// 마법 이펙트 삭제

	PACKET_NullFunc,		//#define dPACKET_SET_SKILLEXP		110		// 스킬 경험치 셋팅 
	PACKET_ItemDBClick,		//#define dPACKET_ITEM_DBCLICK		111		// 아이템 인터페이스 처리

	PACKET_NullFunc,		//#define dPACKET_ADD_SKILL			112		// 스킬 추가 ( 서버 ->클라이언트 )
	PACKET_NullFunc,		//#define dPACKET_SKILL_LIST		113		// 스킬 리스트
	PACKET_UseQuickItem,		//#define dPACKET_USE_QUICKITEM		114		// 퀵아이템 사용 

	PACKET_NullFunc,		//#define dPACKET_UPDATE_POTIONCNT		115		// 포션 수량 업데이트	

	PACKET_NullFunc,		//#define dPACKET_QUEST_LIST		116		// 퀘스트 리스트 (서버->클라이언트)
	PACKET_QuestRequest,		//#define dPACKET_QUEST_REQUEST		117		// 퀘스트 정보 요청 (클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_QUEST_RESULT		118		// 퀘스트 결과(서버->클라이언트)

	PACKET_DoorEvent,		//#define dPACKET_DOOR_EVENT		119		// Indoor 문 처리

	PACKET_OpenGarbage,		//#define dPACKET_OPEN_GARBAGE		120		// 창고 열기 (클라->서버)
	PACKET_NullFunc,		//#define dPACKET_GARBAGE_ITEMLIST		121		// 창고 아이템 리스트(서버->클라)
	PACKET_NullFunc,		//#define dPACKET_INSERT_TO_GARBAGE	122		// 창고에 아이템 추가(서버->클라)
	PACKET_NullFunc,		//#define dPACKET_REMOVE_FROM_GARBAGE	123		// 창고에 아이템 빼기(서버->클라)
	PACKET_InputGarbageItem,	//#define dPACKET_INPUT_GARBAGE_ITEM	124		// 창고에 아이템 넣기(클라->서버)
	PACKET_OutputGarbageItem,	//#define dPACKET_OUTPUT_GARBAGE_ITEM	125		// 창고에 아이템 빼기(클라->서버)

	PACKET_NullFunc,		//#define dPACKET_RIDE_INFO			126		// 탈것 정보

	PACKET_BeginCastMagic,	//#define dPAKCET_BEGIN_CASTMAGIC		127		// 마법 시전 시~작~ (클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_BEGIN_CASTMAGIC2		128		// 마법 시전 시작 (서버->클라이언트)
	PACKET_NullFunc,		//#define dPACKET_NPC_INFO			129		// NPC 정보 
	PACKET_NullFunc,		//#define dPACKET_MOB_STATE			130		// NPC 모드 변환

	PACKET_NullFunc,		//#define dPACKET_GHOST_STATE		131		// 유령 상태		(서버->클라이언트)

	PACKET_AddPartyBoard,	//#define dPACKET_ADD_PARTYBOARD		132		// 파티 게시판 등록
	PACKET_GetPartyBoard,	//#define dPACKET_GET_PARTYBOARD		133		// 파티 게시판 게시물 목록(클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_PARTYBOARD_LIST		134		// 파티 게시판(서버->클라이언트)

	PACKET_AttackPlayer,		//#define dPACKET_ATTACK_PLAYER		135		// PK 
	PACKET_NullFunc,		//#define dPACKET_DAMAGE_PVP		136		// 플레이어 -> 플레이어 공격

	PACKET_BroadCastChat,	//#define dPACKET_BROADCAST_CHAT		137		// 전체채팅
	PACKET_SellingChat,		//#define dPACKET_SELLING_CHAT		138		// 매매채팅
	PACKET_GuildChat,		//#define dPACKET_GUILD_CHAT		139		// 길드(군단)채팅
	PACKET_PartyChat,		//#define dPACKET_PARTY_CHAT		140		// 파티채팅
	PACKET_MemberChat,		//#define dPACKET_MEMBER_CHAT		141		// 단 채팅
	PACKET_NullFunc,		//#define dPACKET_CHANGE_WEATHER		142		// 날씨 변경
	PACKET_NullFunc,		//#define dPACKET_SOUND_EFFECT		143		// 사운드 이펙트
	PACKET_NullFunc,		//#define dPACKET_UPDATE_CHA		144		// 성향 업데이트 
	PACKET_EnchantItem,		//#define dPACKET_ENCHANT_ITEM		145		// 인첸트 아이템 
	PACKET_NullFunc,		//#define dPACKET_ENCHANT_ITEM_RES		146		// 인첸트 아이템 결과 
	PACKET_PartyAskInvite,	//#define dPACKET_PARTY_ASKINVITE		147		// 파티 초대
	PACKET_PartyInviteRes,	//#define dPACKET_PARTY_INVITE_RES		148		// 파티 초대 응답
	PACKET_NullFunc,		//#define dPACKET_UPDATE_CLASS		149		// 직업 업데이트 
	PACKET_NullFunc,		//#define dPACKET_SYSTEM_MSG		150		// 시스템 메세지
	PACKET_UseSkill,		//#define dPACKET_USE_SKILL			151		// 스킬 사용
	PACKET_NullFunc,		//#define dPACKET_BEGIN_SKILL		152		// 스킬 사용( 서버->클라이언트 )
	PACKET_NullFunc,		//#define dPACKET_SET_VISIBLE		153		// 케릭터 않보이게 하기
	PACKET_NullFunc,		//#define dPACKET_SET_MOVABLE		154		// 이동 가능, 불가 
	PACKET_CreateGuild,		//#define dPACKET_CREATE_GUILD		155		// 길드생성
	PACKET_DestroyGuild,		//#define dPACKET_DESTROY_GUILD		156		// 길드소멸
	PACKET_NullFunc,		//#define dPACKET_UPDATE_GUILDINFO		157		// 길드정보
	PACKET_NullFunc,		//#define dPACKET_ITEM_EVENT		158		// 아이템 이벤트

	PACKET_NullFunc,		//#define dPACKET_GET_FRIENDLIST		159		// 친구목록
	PACKET_AddFriend,		//#define dPACKET_FRIEND_ADD		160		// 친구등록
	PACKET_NullFunc,		//#define dPACKET_FRIEND_ADDED		161		// 친구등록결과
	PACKET_RemoveFriend,		//#define dPACKET_FRIEND_REMOVE		162		// 친구삭제
	PACKET_NullFunc,		//#define dPACKET_FRIEND_REMOVED		163		// 친구삭제결과
	PACKET_FriendMemo,		//#define dPACKET_FRIEND_MEMO		164		// 친구쪽지

	PACKET_AddMemberRequest,	//#define dPACKET_GUILD_ADDMEMBER_REQUEST	165		// 길드 가입 요청(?)
	PACKET_GuildAddMember,	//#define dPACKET_GUILD_ADDMEMBER		166		// 길드원 추가
	PACKET_GuildDropMember,	//#define dPACKET_GUILD_DROPMEMBER		167		// 길드원 추방
	PACKET_GuildQuit,		//#define dPACKET_GUILD_QUIT		168		// 길드 탈퇴

	PACKET_NullFunc,		//#define dPACKET_PET_INFO			169		// 애완동물 정보
	PACKET_NullFunc,		//#define dPACKET_PET_ON			170		// 애완동물 ON
	PACKET_NullFunc,		//#define dPACKET_PET_OFF			171		// 애완동물 OFF

	PACKET_NullFunc,		//#define dPACKET_UPDATE_PET_GAGE		172		// 애완동물 분노게이지
	PACKET_NullFunc,		//#define dPACKET_UPDATE_PET_INFO		173		// 애완동물 정보 업데이트 
	PACKET_NullFunc,		//#define dPACKET_PET_ATTACK		174		// 팻 -> 몹 공격 
	PACKET_NullFunc,		//#define dPACKET_ATTACK_TO_PET		175		// 몹 -> 팻 공격
	
	PACKET_MoveDealMap,		//#define dPACKET_MOVE_DEALMAP		176		// 거래맵으로 이동
	
	PACKET_NullFunc,		//#define dPACKET_CAMP_INFO			177		// 진지 정보
	PACKET_MakeCamp,		//#define dPACKET_MAKE_CAMP			178		// 진지 생성

    	PACKET_NullFunc,            //#define dPACKET_FISHING_STATUS		179  		// 낚시 관련 상태	( S -> C )
    	PACKET_NullFunc,            //#define dPACKET_FISHING_PATTERN		180     	// 낚시 패턴		( S -> C )
    	PACKET_FishingAction,       //#define dPACKET_FISHING_ACTION		181     	// 낚시 관련 행동	( C -> S )
	PACKET_FishingPos,          //#deifne dPACKET_FISHING_POS             182     	// 낚시할때 케릭터 보는 방향
	
	PACKET_PetOnOff,		//#define dPACKET_PET_ONOFF			183		// 애완동물 켜고 끄기 ( 클라이언트->서버 )	
	PACKET_PetChangeName,	//#define dPACKET_PET_CHANGENAME		184		// 애완동물 이름바꾸기 ( 클라이언트->서버 )
	PACKET_NullFunc,		//#define dPACKET_PET_UPDATENAME		185		// 애완동물 이름 바꾸기 ( 서버->클라이언트 )
	PACKET_NullFunc,		//#define dPACKET_PET_UPDATEEXP		186		// 애완동물 경험치 바꾸기 (서버->클라)

	PACKET_DealOk,		//#define dPACKET_DEAL_OK			187		// 거래 수락 ( 클라이언트->서버 )
	PACKET_NullFunc,		//#define dPACKET_DEAL_BEGINOK		188		// 거래 시작 ( 서버->클라이엄트 )

	PACKET_NullFunc,		//#define dPACKET_FRIEND_LOGIN		189		// 친구 로그인
	PACKET_NullFunc,		//#define dPACKET_FRIEND_LOGOUT		190		// 친구 로그아웃

	PACKET_NullFunc,		//#define dPACKET_MOB_POPEFFECT		191		// 몬스터 이팩트 출력 
	PACKET_NullFunc,		//#define dPACKET_PLAYER_POPEFFECT		192		// 플레이어 이펙트 출력

	PACKET_NullFunc,		//#define dPACKET_CHANGE_STATUS		193		// 케릭터 상태변화	
	
	PACKET_GuildChangeMaster,	//#define dPACKET_GUILD_CHANGEMASTER	194		// 길드 마스터 권한 위임	
	PACKET_GuildGetMemberList,	//#define dPACKET_GUILD_GETMEMBERLIST	195		// 길드 맴버 리스트 요청(클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_GUILD_MEMBERLIST		196		// 길드 맴버리스트 (서버->클라이언트)
	
	PACKET_GuildAdvance,		//#define dPACKET_GUILD_ADVANCE		197		// 부단장 승진
	PACKET_GuildDemotion,	//#define dPACKET_GUILD_DEMOTION		198		// 부단장 강등 

	PACKET_MyShopOpen,		//#define dPACKET_MYSHOP_OPEN		199		// 개인상점 오픈 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_OPENRESULT	200		// 개인상점 오픈 결과 ( 서버 -> 클라이언트 )
	PACKET_MyShopChangeTitle,	//#define dPACKET_MYSHOP_CHANGETITLE	201		// 개인상점 이름 바꾸기 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_TITLECHANGED	202		// 개인상점 이름 바꾸기 ( 서버 -> 클라이언트 )
	PACKET_MyShopAddItem,	//#define dPACKET_MYSHOP_ADDITEM		203		// 개인상점 아이템 추가 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_ITEMADDED		204		// 개인상점 아이템 추가 ( 서버->클라이언트 )
	PACKET_MyShopRemoveItem,	//#define dPACKET_MYSHOP_REMOVEITEM	205		// 개인상점 아이템 삭제 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_ITEMREMOVED	206		// 개인상점 아이템 삭제 ( 서버 -> 클라이언트 )
	PACKET_MyShopSellBegin,	//#define dPACKET_MYSHOP_SELLBEGIN		207		// 개인상점 판매시작 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_SELLBEGIN_RESULT	208		// 개인상점 판매시작 ( 서버 -> 클라이언트 )
	PACKET_MyShopSellFinish,	//#define dPACKET_MYSHOP_SELLFINISH	209		// 개인상점 판매종료(회수) (클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_SELLFINISH_OK	210		// 개인상점 판매종료(회수) (서버->클라이언트)
	PACKET_MyShopGetItemList,	//#define dPACKET_MYSHOP_GETITEMLIST	211		// 개인상점 판매목록 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_SENDITEMLIST	212		// 개인상점 판매목록 ( 서버 -> 클라이언트 )
	PACKET_MyShopBuyItem,	//#define dPACKET_MYSHOP_BUYITEM		213		// 개인상점 아이템구입 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_UPDATEINFO	214		// 개인상점 정보 업데이트 ( 서버->클라이언트 )
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_REMOVELISTITEM	215		// 개인상점 아이템 삭제 ( 서버->클라이언트 ) / 수입하는 사람의 화면에 보여지는 정보에 사용
	PACKET_NullFunc,		//#define dPACKET_MYSHOP_UPDATECASH	216		// 개인상점 판매대금 업데이트( 서버->클라이언트 )

	PACKET_WorldChat,		//#define dPACKET_WORLD_CHAT		217		// 세계채널 채팅
	PACKET_NullFunc,		//#define dPACKET_INSERT_OBJMOB		218		// 오브젝트 몬스터 추가 ( 성문, 성물 )
	PACKET_NullFunc,		//#define dPACKET_UPDATE_WEIGHTGAGE	219		// 무게게이지
	PACKET_NullFunc,		//#define dPACKET_CHANGE_NAMECOLOR		220		// 이름색 변경

	PACKET_NullFunc,		//#define dPACKET_UPDATE_WARCAMP		221		// 진지정보 업데이트
	PACKET_NullFunc,		//#define dPACKET_UPDATE_WARINFO		222		// 공성관련 정보 업데이트
	
	PACKET_NullFunc,		//#define dPACKET_UPDATE_GARBAGEMONEY	223		// 창고돈 업데이트
	PACKET_ReceiptMoney,		//#define dPACKET_RECEIPT_MONEY		224		// 창고에 돈 넣기
	PACKET_WithdrawMoney,	//#define dPACKET_WITHDRAW_MONEY		225		// 창고에서 돈 찾기
	
	PACKET_SetPvPMode,		//#define dPACKET_SET_PVP_MODE		226		// PvP 모드 변환      (클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_UPDATE_PVP_MODE		227		// PvP 모드 업데이트  (서버->클라이언트)

	PACKET_GetCastleList,	//#define dPACKET_GET_CASTLELIST		228		// 성 목록 
	PACKET_NullFunc,		//#define dAPCKET_SEND_CASTLELIST		229		// 성 목록 보내기
	PACKET_GetWarState,		//#define dPACKET_GET_WARSTATE		230		// 전쟁상황도의 전쟁정보요청 (클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_SEND_WARSTATE		231		// 전쟁상황도 정보 (서버->클라이언트 )	

	PACKET_GetTaxInfo,		//#define dPACKET_GET_TAXINFO		232		// 현재 세율 얻기 (클라이언트->서버)
	PACKET_NullFunc,		//#define dPACKET_OPEN_TAXWINDOW		233		// 세율 조정 인터페이스 열기 
	PACKET_ChangeTax,		//#define dPACKET_CHANGE_TAX		234		// 세율 조정
	PACKET_NullFunc,		//#define dPACKET_UPDATE_TAX		235		// 현재 세율 업데이트

	PACKET_EnchantChange,	//#define dPACKET_ENCHANT_CHANGE		236		// 인챈트 채인지( 유료화 아이템 사용 )
	PACKET_EnchantCollection,	//#define dPACKET_EHCNAHT_COLLECTION	237		// 인첸트 콜렉션( 유료화 아이템 사용 )
	PACKET_ItemLook,		//#define dPACKET_ITEMLOOK			238		// 아이템 룩 ( 유료화 아이템 사용 )
	PACKET_LevelLook,		//#define dPACKET_LEVELLOOK			239		// 레벨 룩 ( 유료화 아이템 사용 )

	PACKET_GetBillingItems,	//#define dPACKET_GET_BILLING_ITEMS	240		// 유료아이템 목록 요청 (클라->서버)
	PACKET_NullFunc,		//#define dPACKET_BILLING_ITEM_INFO	241		// 유료아이템 목록 (서버->클라)

	PACKET_NullFunc,		//#define dPACKET_REMOVE_BILLING_ITEM	242		// 유료아이템 유료인터페이스에서 삭제 (서버->클라)	
	PACKET_UseBillItem,		//#define dPACKET_USE_BILLITEM		243		// 유료 아이템 사용 ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_MAP_SHOUT			244		// 맵 전체 외침
	
	PACKET_NullFunc,		//#define dPACKET_ENEMY_DIVISION		245		// 피아구분

	PACKET_NullFunc,		//#define dPACKET_SHOW_ITEMTIMER		246		// 아이템 타이머 돌기
	PACKET_NullFunc,		//#define dPACKET_SELCHAR_SYSTEMMSG	247		// 시스템 메시지 (캐릭터 셀렉트용)

	PACKET_NullFunc,		//#define dPACKET_SMITH_REQUEST		248		// 대장장이 요청
	PACKET_SmithEnchat,		//#define dPACKET_SMITH_UPDATE		249		// 대장장이 업데이트
	PACKET_NullFunc,		//#define dPACKET_SMITH_QUIT		250		// 대장장이 종료
	
	PACKET_SmithFinish,		//#define dPACKET_SMITH_CANCEL		251		// 대장장이 취소
	PACKET_InputSmithItem,	//#define dPACKET_SMITH_ADDITEM		252		// 대장장이 아이템  추가 ( 클라이언트 -> 서버 )
	
	PACKET_RemoveSmithItem,	//#define dPACKET_SMITH_DELITEM		253		// 대장장이 아이템 삭제  ( 클라이언트 -> 서버 )
	PACKET_NullFunc,		//#define dPACKET_SMITH_ITEMADDED		254		// 대장장이에게 맡길 아이템 추가 ( 서버 -> 클라이언트 )255//
	PACKET_NullFunc,		//#define dPACKET_SMITH_ITEMDELED		255		// 대장장이 아이템 삭제 ( 서버 -> 클라이언트 )
	PACKET_NullFunc,		//#define dPACKET_UPDATE_CARDCNT		256		// 대장장이 아이템 삭제 ( 서버 -> 클라이언트 )
	PACKET_CardUndo,		//dPACKET_CARDUNDO				257

	PACKET_FriendLoginRequest,  //dPACKET_FRIEND_LOGIN_REQUEST		258		// dPACKET_FRIEND_LOGIN_REQUEST
	PACKET_FriendLogOutRequest,	//dPACKET_FRIEND_LOGOUT_REQUEST		259		// 친구 로그아웃 리퀘스트
	PACKET_NullFunc,		//dPACKET_FRIEND_MSG				260		// 친구로그인메시지	
	
	PACKET_NullFunc,		//#define dPACKET_UPDATE_CASH_MONEY	261		// 캐쉬금액 업데이트
	
	PACKET_FriendAskInvit,	//#define dPACKET_FRIEND_ASKYESNO		262
	PACKET_FriendJoinYes,	//#define dPACKET_FRIEND_JOIN		263
	PACKET_FriendJoinNo		//#define dPACKET_FRIEND_REQUEST		264
};


//
// Function Name: ReadRecvBuff
// Date: 2003-02-12
// Description: recv buffer에 있는 데이터들을 처리함
//200
BOOL ReadRecvBuff( sPDESC_DATA pPlayer )
{
	WORD msgSize;
	WORD tag;

	CWhileMgr WMgr;

	while( pPlayer->recvSize > 1 )
	{
		WMgr.CheckDump( __FILE__, __LINE__ );
	
		pPlayer->recvPos = 0;

		msgSize = GetWord( pPlayer->recvBuff, pPlayer->recvPos );

		char	szDecData[40960];
		DWORD	key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4 };
		int		decSize;
				
		memset( &szDecData, 0, sizeof( szDecData ) );
		decSize = TEA_DecryptData( &pPlayer->recvBuff[2], szDecData, msgSize - 2, key );

		if( szDecData[0] != dENC_FIRST_HEADER ||
			szDecData[1] != dENC_SECOND_HEADER )
		{
			log( "Bad Packet header [%s]\r\n", pPlayer->ip );
			return 0;
		}
			
		memcpy( pPlayer->TemprecvBuff, &szDecData[2], sizeof( szDecData ) - 2  );
		pPlayer->recvPos = 0;
		
		WORD omsgSize = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );
		tag = GetWord( pPlayer->TemprecvBuff, pPlayer->recvPos );

#ifdef DEBUG_RECVBUFF
		printf( "DEBUG_RECVBUFF : [%s/%s] packet : %d, size : %d\r\n", GET_NAME( pPlayer ), GET_ID( pPlayer ), tag, msgSize );
#endif
		
		if( omsgSize == 0 )		// 일어나면 안될일 
		{
			log( "ReadRecvBuff : msgSize == 0 [%s/%s]\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );
			return 0;
		}

		if( tag > dMAX_TAG  )
			return 0;
		
		if( tag == dFIRST_PACKET )
			pPlayer->isAccepted = 1;

		if( !pPlayer->isAccepted && tag != dFIRST_PACKET )
			return 0;

		if( pPlayer->recvSize < msgSize )
			return 1;

		if( !MessageFuncsList[tag]( pPlayer ) )
			return 0;
		
		memmove( &pPlayer->recvBuff[0], &pPlayer->recvBuff[msgSize], pPlayer->recvSize - msgSize );
		pPlayer->recvSize -= msgSize;
	}
	return 1;
}

void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1 )
{
	string	test;
	char	szTemp[32];
	switch( line ) 
	{
	case 8:
		{	
			test = sz1;
			test.append( " can have the item." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
	//
	case 14:
		{
			test = sz1;
			test.append( " already has confirmed the trade." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
	//
	case 18:
		{
			test = sz1;
			test.append( " has confirmed the trade." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 29:
		{
			test = sz1;
			test.append( " has declined party invitation." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
		
	case 110:
		{
			sprintf( szTemp, "You have sent massage to " );
			test = szTemp;
			test.append( sz1 );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 111:
		{
			test = sz1;
			test.append( " declines your guild invitation." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 112:
		{
			sprintf( szTemp,  "You have joined " );
			test = szTemp;
			test.append( sz1 );
			test.append( " guild." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 113:
		{
			test = sz1;
			test.append( " has joined the guild." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 117:
		{
			sprintf( szTemp, "You are removed from " );
			test = szTemp;
			test.append( sz1 );
			test.append( " guild." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 118:
		{
			test = sz1;
			test.append( " has been removed from the guild." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 133:
		{
			test = sz1;
			test.append( " beat you down." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 151:
		{
			sprintf( szTemp, "You authorize to " );
			test = szTemp;
			test.append( sz1 );
			test.append( " guild master." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 152:
		{
			test = sz1;
			test.append( " has authorized guild master to you." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 245:
		{
			test = sz1;
			test.append( "'s Item Information" );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
	case 259:
		{
			test = sz1;
			test.append( "'s Level Information" );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
		/*
		case :
		{
		}
		break;
		*/
	}	
}

void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2 )
{
	/*string test;
	char	szTemp[32];
	switch( line ) {
	case :
		break;
	
	case :
		break;
	
	}*/
}


void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, int n1 )
{
	string	test;
	char	szTemp[16];
	switch( line ) 
	{
	case 264:
		{	
			sprintf( szTemp,"This area is " );
			test = szTemp;
			sprintf( szTemp, "%d", n1 );
			test.append( szTemp );
			test.append( "% tax added area occupied by " );
			test.append( sz1 );
			test.append( " Army." );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
		//
	case 93:
		{
			sprintf( szTemp,"Absorb " );
			test = szTemp;
			sprintf( szTemp, "%d", n1 );
			test.append( szTemp );
			test.append( " vitality form " );
			test.append( sz1 );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
	}
	
}

void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2, int n1, int n2 )
{
	string	test;
	char	szTemp[16];
	switch( line ) 
	{
	case 144:
		{	
			sprintf( szTemp," map %d / %d", n1, n2 );
			test = sz1;
			test.append( " location : " );
			test.append( sz2 );
			test.append( szTemp );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
	//
	}
}


void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2, char *sz3, ... )
{
	string test;
	switch( line )
	{
	case 167:
		{
			test="You get ";
			test.append( sz1 );
			test.append( sz2 );
			test.append( sz3 );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 168:
		{
			test = "You have sold ";
			test.append( sz1 );
			test.append( sz2 );
			test.append( sz3 );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;

	case 195:
		{
			test = sz1;
			test.append( " that is attacking " );
			test.append( sz2 );
			test.append( sz3 );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
		
	case 237:
		{
			test = sz1;
			test.append( sz2 );
			test.append( sz3 );
			SendSystemMsg( pPlayer, test.c_str() );
		}
		break;
		/*
		case:
		{
		}
		break;
		 */
	}
}


void StrSendSysMsg( sPDESC_DATA pPlayer, int line, char *sz1, char *sz2, char *sz3, int n1, int n2, int n3 )
{
	char	szTemp[32];
	string test;

	switch( line ) {
	case 202:
		test = sz1;
		sprintf( szTemp, "(%d) map attack castle : ", n1 );
		test.append( szTemp );
		test.append( sz2 );
		sprintf( szTemp, "(%d) -> ", n2 );
		test.append( szTemp );
		test.append( sz3 );
		sprintf( szTemp, "(%d)", n3 );
		test.append( szTemp );
		SendSystemMsg( pPlayer, test.c_str() );
		break;
	}
		
}



//
