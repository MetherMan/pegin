/**
 *	$Workfile: dataserver.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 27 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/dataserver.cpp $
 * 
 * 27    04-12-15 10:40a Sk8snow
 * 
 * 26    04-09-08 3:54p Sk8snow
 * 
 * 25    04-07-13 7:26a Sk8snow
 * 
 * 24    04-05-11 1:45a Sk8snow
 * 
 * 23    04-04-30 9:46a Sk8snow
 * 
 * 22    04-04-07 10:17p Sk8snow
 * 
 * 21    04-04-03 4:03p Sk8snow
 * 
 * 20    04-03-31 8:22a Sk8snow
 *        
 */
#include "def.h"

#include "structs.h"
#include "extern.h"
#include "net_init.h"
#include "message.h"
#include "world.h"
#include "player.h"
#include "monster.h"
#include "skill.h"

#include "dataserver.h"

#include "item.h"
#include "itemdb.h"

#include "mercenary.h"
#include "questdb.h"
#include "quest.h"
#include "guild.h"

#include "friends.h"
#include "billing.h"

#include "pet.h"

#include "guild.h"

SOCKET g_DataSock;


char g_recvBuff[dMAX_DATA_SOCK_BUFF];		// recv 데이터 버퍼 
int g_recvSize;								// recv 버퍼 상의 총 데이터 량
int	g_recvPos;						

char g_sendBuff[dMAX_DATA_SOCK_BUFF];		// send 대기중인 데이터 
int g_sendSize;								// send 할 데이터량

void IsAdminID( sPDESC_DATA pPlayer );

int g_maxSendSize = 0;


void InitDataServer()
{
	*g_recvBuff = NULL_STR;
	g_recvSize = 0;
	g_recvPos = 0;
	
#ifdef dUSE_SENDBUFF
	*g_sendBuff = NULL_STR;
	*g_sendBuff2 = NULL_STR;
	g_sendSize = 0;
	g_sendSize2 = 0;
#endif
}

//
// Function Name: RecvFromDataServer
// Date: 2003-03-20
// Description: 데이터 Recv
//
BOOL RecvFromDataServer()
{
	int recvSize;
	char recvBuff[dMAX_SOCK_BUFF];

	recvSize = recv( g_DataSock, recvBuff, dMAX_SOCK_BUFF, 0 );
	
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

	if( ( g_recvSize + recvSize ) >= dMAX_DATA_SOCK_BUFF )
	{
		log( "recvBuff Overflow from DataServer\r\n" );
		return 0;
	}
	
	memcpy( &g_recvBuff[g_recvSize], recvBuff, recvSize );
	g_recvSize += recvSize;

	return 1;
}

//
// Function Name: NullDataFunc
// Date: 2003-03-20
// Description: 
//
BOOL NullDataFunc()
{
	return 1;
}

//
// Function Name: GetAllPlayerData
// Date: 2003-03-20
// Description: 초기 케릭터 선택 데이터 ( 3개의 케릭터 데이터 )
// 
BOOL GetAllPlayerData()
{
	char id[128];
	sCHAR_SELECT_INFO charInfo[3];

	GetString( g_recvBuff, id, g_recvPos );
	memcpy( &charInfo[0], &g_recvBuff[g_recvPos], sizeof( sCHAR_SELECT_INFO ) );
	g_recvPos += sizeof( sCHAR_SELECT_INFO );
	memcpy( &charInfo[1], &g_recvBuff[g_recvPos], sizeof( sCHAR_SELECT_INFO ) );
	g_recvPos += sizeof( sCHAR_SELECT_INFO );
	memcpy( &charInfo[2], &g_recvBuff[g_recvPos], sizeof( sCHAR_SELECT_INFO ) );

	sPDESC_DATA pPlayer = FindWaitAllPlayer( id );

	if( !pPlayer )
		return 1;

	gPLAYERS.WaitAllData.remove( pPlayer );

	//
	pPlayer->selectGuildLevel[0] = charInfo[0].guildLevel;
	pPlayer->selectGuildLevel[1] = charInfo[1].guildLevel;
	pPlayer->selectGuildLevel[2] = charInfo[2].guildLevel;

	
	/*
	char		name[dNAME_LEN];				// 케릭터 이름 
	BYTE		sex;							// 성별
	BYTE		headStyle;						// 머리	스타일
	BYTE		faceStyle;						// 얼굴 스타일
	
	  WORD		equip[dCHARINFO_EQUIP_NUM];		// 착용 장비
	  BYTE		charPos;						// 케릭터 번호
	*/
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_GETALLCHAR_RESULT, g_nPos );

	// 0번 케릭터 
	PutString( g_Packet, charInfo[0].name, g_nPos );
	
	PutInteger( g_Packet, charInfo[0].skill[0], g_nPos );
	PutInteger( g_Packet, charInfo[0].skill[1], g_nPos );
	PutInteger( g_Packet, charInfo[0].skill[2], g_nPos );
	PutInteger( g_Packet, charInfo[0].skill[3], g_nPos );

	PutByte( g_Packet, charInfo[0].sex, g_nPos );
	PutWord( g_Packet, charInfo[0].headStyle, g_nPos );
	PutWord( g_Packet, charInfo[0].faceStyle, g_nPos );
	/*
	charInfo[charPos].equip[0] = atoi( row[26] );		// 투구 
	charInfo[charPos].equip[1] = atoi( row[27] );		// 상의 
	charInfo[charPos].equip[2] = atoi( row[28] );		// 하의 
	charInfo[charPos].equip[3] = atoi( row[29] );		// 신발 
	charInfo[charPos].equip[4] = atoi( row[30] );		// 장갑
	*/
	PutWord( g_Packet, charInfo[0].equip[0], g_nPos );
	PutWord( g_Packet, charInfo[0].equip[1], g_nPos );
	PutWord( g_Packet, charInfo[0].equip[2], g_nPos );
	PutWord( g_Packet, charInfo[0].equip[3], g_nPos );
	PutWord( g_Packet, charInfo[0].equip[4], g_nPos );

	PutByte( g_Packet, charInfo[0].charClass, g_nPos );

	PutByte( g_Packet, charInfo[0].charPos, g_nPos );

	// 1번 케릭터 
	PutString( g_Packet, charInfo[1].name, g_nPos );
	
	PutInteger( g_Packet, charInfo[1].skill[0], g_nPos );
	PutInteger( g_Packet, charInfo[1].skill[1], g_nPos );
	PutInteger( g_Packet, charInfo[1].skill[2], g_nPos );
	PutInteger( g_Packet, charInfo[1].skill[3], g_nPos );
		
	PutByte( g_Packet, charInfo[1].sex, g_nPos );
	PutWord( g_Packet, charInfo[1].headStyle, g_nPos );
	PutWord( g_Packet, charInfo[1].faceStyle, g_nPos );
	/*
	charInfo[charPos].equip[0] = atoi( row[26] );		// 투구 
	charInfo[charPos].equip[1] = atoi( row[27] );		// 상의 
	charInfo[charPos].equip[2] = atoi( row[28] );		// 하의 
	charInfo[charPos].equip[3] = atoi( row[29] );		// 신발 
	charInfo[charPos].equip[4] = atoi( row[30] );		// 장갑
	*/
	PutWord( g_Packet, charInfo[1].equip[0], g_nPos );
	PutWord( g_Packet, charInfo[1].equip[1], g_nPos );
	PutWord( g_Packet, charInfo[1].equip[2], g_nPos );
	PutWord( g_Packet, charInfo[1].equip[3], g_nPos );
	PutWord( g_Packet, charInfo[1].equip[4], g_nPos );

	PutByte( g_Packet, charInfo[1].charClass, g_nPos );

	PutByte( g_Packet, charInfo[1].charPos, g_nPos );

	// 2번 케릭터 
	PutString( g_Packet, charInfo[2].name, g_nPos );
	
	PutInteger( g_Packet, charInfo[2].skill[0], g_nPos );
	PutInteger( g_Packet, charInfo[2].skill[1], g_nPos );
	PutInteger( g_Packet, charInfo[2].skill[2], g_nPos );
	PutInteger( g_Packet, charInfo[2].skill[3], g_nPos );

	PutByte( g_Packet, charInfo[2].sex, g_nPos );
	PutWord( g_Packet, charInfo[2].headStyle, g_nPos );
	PutWord( g_Packet, charInfo[2].faceStyle, g_nPos );
	/*
	charInfo[charPos].equip[0] = atoi( row[26] );		// 투구 
	charInfo[charPos].equip[1] = atoi( row[27] );		// 상의 
	charInfo[charPos].equip[2] = atoi( row[28] );		// 하의 
	charInfo[charPos].equip[3] = atoi( row[29] );		// 신발 
	charInfo[charPos].equip[4] = atoi( row[30] );		// 장갑
	*/
	PutWord( g_Packet, charInfo[2].equip[0], g_nPos );
	PutWord( g_Packet, charInfo[2].equip[1], g_nPos );
	PutWord( g_Packet, charInfo[2].equip[2], g_nPos );
	PutWord( g_Packet, charInfo[2].equip[3], g_nPos );
	PutWord( g_Packet, charInfo[2].equip[4], g_nPos );

	PutByte( g_Packet, charInfo[2].charClass, g_nPos );

	PutByte( g_Packet, charInfo[2].charPos, g_nPos );
	
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

	
	int		charLevel[3],max, maxindex,i;
	for( i=0;i < 3; i++ )
	{
		charLevel[i]=0;
	}
	
	for( i = 0; i < 3; i++ )
	{
		charLevel[i] = charInfo[i].skill[0] + charInfo[i].skill[1] + charInfo[i].skill[2] + charInfo[i].skill[3];	
	}
	
	//3개의 숫자를 비교한다.
	for( i=0; i < 3; i++ )
	{
		maxindex=i;
		max=charLevel[i];
		for( int j = i+1; j<3; j++ )
		{
			if( max < charLevel[j] )
			{
				max = charLevel[j];
				maxindex = j;
			}
		}
		charLevel[maxindex] = charLevel[i];
		charLevel[i] = max;	
	}

	pPlayer->ch2.MaxLevel = charLevel[0];

	pPlayer->isWaitAllData = 0;

	return 1;
}

int CheckCashMoney( char *szID )
{
	char		query[1024];
	char		escID[128];
	int			nCash=0;
	
	mysql_escape_string( escID, szID, strlen( szID ) );
	
	sprintf( query, "select * from StorCash where UserID= '%s' and Status = '2'", escID );
	
	nCash = 0;
	
	if( !mysql_query( g_CASHMYSQL, query ) )
	{
		MYSQL_RES *result = mysql_store_result( g_CASHMYSQL );
		MYSQL_ROW	row=NULL;	
		if( result )
		{
			while( ( row = mysql_fetch_row( result ) ) )
			{			
				nCash += atoi( row[3] );
			}
			
			sprintf( query, "update StorCash set Status = 3 where UserID='%s'", escID );
			
			if( mysql_query( g_CASHMYSQL, query ) )
			{
				log( "error StorCash\r\n" );
			}
		}
		mysql_free_result( result );
	}
	
	else
	{
		mysql_close( g_CASHMYSQL );
		return 0;
	}
	
	if( nCash )
	{
		sprintf( query, "select * from UserCashMoney where UserID='%s'", escID );
		
		if( !mysql_query( g_CASHMYSQL, query ) )
		{
			MYSQL_RES *result = mysql_store_result( g_CASHMYSQL );
			MYSQL_ROW	row=NULL;
			if( result )
			{
				if( ( row = mysql_fetch_row( result ) ) )
				{			
					int nSaveCash = atoi( row[2] );
					nCash = nSaveCash + nCash;
					sprintf( query, "update UserCashMoney set amount = %d where UserID ='%s' limit 1", nCash, escID );
				}
				else
				{
					sprintf( query, "insert into UserCashMoney values( '', '%s', %d )", escID, nCash );
				}
				
				if( mysql_query( g_CASHMYSQL, query ) )
				{
					log( "ITEMDB_Query : [%s]\r\n", mysql_error(g_CASHMYSQL) );
					return 0;
				}
			}
			mysql_free_result( result );
		}
		else
		{
			mysql_close( g_CASHMYSQL );
			return 0;
		}
	}
	
	nCash = 0;
	
	sprintf( query, "select * from UserCashMoney where UserID='%s'", escID );
	
	if( !mysql_query( g_CASHMYSQL, query ) )
	{
		MYSQL_RES *result = mysql_store_result( g_CASHMYSQL );
		MYSQL_ROW	row=NULL;
		if( result )
		{
			if( ( row = mysql_fetch_row( result ) ) )
			{			
				nCash = atoi( row[2] );
			}
		}
		mysql_free_result( result );
	}
	else
	{
		nCash = 0;
	}
	return nCash;
}


//
// Function Name: GetCharInfo
// Date: 2003-03-20
// Description: 케릭터 데이터 받기
// 
BOOL GetCharInfo()
{
	char id[128];

	GetString( g_recvBuff, id, g_recvPos );

	sCHAR_DATA charInfo;
	memcpy( &charInfo, &g_recvBuff[g_recvPos], sizeof( sCHAR_DATA ) );
	
	sPDESC_DATA pPlayer = FindWaitCharPlayer( id );

	if( !pPlayer )
		return 1;

	if( pPlayer->chDataLoaded )
		return 1;

	memcpy( &pPlayer->ch, &charInfo, sizeof( sCHAR_DATA ) );

	pPlayer->chDataLoaded = 1;

	GET_CASHMONEY( pPlayer ) = CheckCashMoney( id );

	IsAdminID( pPlayer );

	///////////////////////////////////////////////////
	// INSERT INTO NAME LIST
	///////////////////////////////////////////////////
	int hashIdx;
	hashIdx = GetStrHashIndex( pPlayer->ch.name );
	INSERT_TO_LIST( gPLAYERS.name_list[hashIdx], pPlayer, name_prev, name_next );
	pPlayer->isInNameList = 1;

	if( !GET_USE_SKILL( pPlayer ) )
		GET_USE_SKILL( pPlayer ) = dSATTACK_SKILL_ITEM;

	if( pPlayer->ch.isNewCharacter )
		NewCharacterProc( pPlayer );

	if( GET_GUILD( pPlayer ) && IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
	{
		INSERT_TO_LIST( gGUILDS.array[GET_GUILD( pPlayer )]->memberList,
			pPlayer, guild_prev, guild_next );
	}

	if( GET_TMP_GUILD( pPlayer) && IS_VALID_GUILD( GET_TMP_GUILD( pPlayer ) ) )
	{
		INSERT_TO_LIST( gGUILDS.array[GET_TMP_GUILD( pPlayer )]->mercenaryList,
			pPlayer, merc_prev, merc_next );
	}

	///////////////////////////////////////////////////
	// GET PLAYERS INVENTORY and EQUIPMENT
	///////////////////////////////////////////////////
#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	ITEMDB_GetPlayerItem( pPlayer );
#endif 

	if( pPlayer->ch.posMapKind == dMAP_KIND_FIELD &&
		!IS_VALID_WORLD( GET_MAP_NUM( pPlayer ) ) )
		PlayerToHomeTown( pPlayer );
	else
	{
		pPlayer->ch2.gotoMapNum = pPlayer->ch.mapNum;
		pPlayer->ch2.gotoPosX = pPlayer->ch.posX;
		pPlayer->ch2.gotoPosY = pPlayer->ch.posY;
		pPlayer->ch2.gotoPosMapKind = pPlayer->ch.posMapKind;
	}
	
	// 케릭터 정보 보내기 TODO: 다른곳으로 이동하게 될것임
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHARINFO, g_nPos );
	PutString( g_Packet, pPlayer->ch.name, g_nPos );			// 케릭터 이름
	PutByte( g_Packet, pPlayer->ch.sex, g_nPos );				// 성별 
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );			// 고유번호 
	
	PutInteger( g_Packet, GET_HP( pPlayer ), g_nPos );			// 체력 
	PutInteger( g_Packet, GET_MAX_HP( pPlayer ), g_nPos );		// 최대 체력 
	PutInteger( g_Packet, GET_MP( pPlayer ), g_nPos );			// 마나력
	PutInteger( g_Packet, GET_MAX_MP( pPlayer ), g_nPos );		// 최대 마나력 
	
	PutWord( g_Packet, GET_STR_TOTAL( pPlayer ), g_nPos );		// 힘 
	PutWord( g_Packet, GET_INT_TOTAL( pPlayer ), g_nPos );		// 지력
	PutWord( g_Packet, GET_DEX_TOTAL( pPlayer ), g_nPos );		// 민첩 
	
	PutWord( g_Packet, GET_SATTACK( pPlayer ), g_nPos );		// 근거리 
	PutWord( g_Packet, GET_LATTACK( pPlayer ), g_nPos );		// 원거리 
	PutWord( g_Packet, GET_BMAGIC( pPlayer ), g_nPos );			// 흑마법 
	PutWord( g_Packet, GET_WMAGIC( pPlayer ), g_nPos );			// 백마법 
	
	PutInteger( g_Packet, GET_SATTACK_EXP( pPlayer ), g_nPos ); // 근거리 경험치 
	PutInteger( g_Packet, GET_LATTACK_EXP( pPlayer ), g_nPos );	// 원거리 경험치 
	PutInteger( g_Packet, GET_BMAGIC_EXP( pPlayer ), g_nPos );  // 흑마법 경험치 
	PutInteger( g_Packet, GET_WMAGIC_EXP( pPlayer ), g_nPos );	// 백마법 경험치
	
	PutInteger( g_Packet, get_skill_exp( pPlayer->ch.skill[0], 0 ), g_nPos );	// 근거리 목표 경험치 
	PutInteger( g_Packet, get_skill_exp( pPlayer->ch.skill[1], 1 ), g_nPos );	// 원거리 목표 경험치 
	PutInteger( g_Packet, get_skill_exp( pPlayer->ch.skill[2], 2 ), g_nPos );	// 흑마법 목표 경험치 
	PutInteger( g_Packet, get_skill_exp( pPlayer->ch.skill[3], 3 ), g_nPos );	// 백마법 목표 경험치
	
	PutInteger( g_Packet, GET_MONEY( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_CASHMONEY( pPlayer ), g_nPos );//캐쉬보내기

	PutWord( g_Packet, GET_RIDE_STATE( pPlayer ) ? GET_RIDE_TYPE( pPlayer ) : 0 , g_nPos );
	PutByte( g_Packet, GetRideSpeed( pPlayer ) , g_nPos );

	PutInteger( g_Packet, GET_CHA( pPlayer ), g_nPos );

	PutByte( g_Packet, GET_CLASS( pPlayer ), g_nPos );				// 직업

	PutInteger( g_Packet, GET_GUILD( pPlayer ), g_nPos );			// 길드

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

	// 케릭 정보 보내기 End 
	gPLAYERS.WaitCharData.remove( pPlayer );
	pPlayer->isWaitCharData = 0;

	// 게임 시작 	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_GAMESTART, g_nPos );
	PutSize( g_Packet, g_nPos );
		
	SendData( pPlayer, g_Packet, g_nPos );
#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	SendPlayerItemInfo( pPlayer );
	SendPlayerSkillList( pPlayer );
#ifndef dNOT_USE_PET
	PET_LoadUserPet( pPlayer );
#endif
#endif

	// 맵 로딩 정보 보내기 TODO: 이것 또한 다른곳으로 이동 예정	
	SendMapLoading( pPlayer, pPlayer->ch.posLayer );

	FlushSendBuff( pPlayer );
/*#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	SendPlayerItemInfo( pPlayer );
	SendPlayerSkillList( pPlayer );
#ifndef dNOT_USE_PET
	PET_LoadUserPet( pPlayer );
#endif
#endif*/

	/////////////////////////////////////
	// S E N D   Q U E S T   I N F O
	/////////////////////////////////////

	// SEND FRIEND LIST
#ifndef dNOT_USE_FRIEND
	//FRIEND_GetFriendList( pPlayer );
	//FRIEND_OnlineCheck( pPlayer );
	FRIENDS_GetFriendList( pPlayer );
	FRIENDS_OnlineCheck( pPlayer );
#endif

#ifndef dNOT_USE_GUILD
	GUILD_SendGuildInfo( pPlayer, dSENDMODE_TOOWNER );
#endif

#ifdef dUSE_LOGIN_LOG
	LOGINLOG_Write( pPlayer );
#endif
	
	GUILD_ChangeMemberInfo( pPlayer );
	
#ifdef dDAUM_BILLING
	LoadPlayerBillingInfo( pPlayer );
#ifndef __CHINA_VER__
	LoadNonUsedBillItems( pPlayer );
#endif
#endif
	//LoadReward( pPlayer );

/*	//캐릭터 가장 높은 녀석을 찾아서 GetBillingInfo함수에 집어 넣는다.
	
	GetBillingInfo( GET_ID( pPlayer ), "aa", pPlayer->ip, pPlayer->ch2.MaxLevel, g_ServerNumber );
*/	return 1;
}


//
// Function Name: NullCharData
// Date: 2003-03-20
// Description: 
// 
BOOL NullCharData()
{
	char id[128];

	GetString( g_recvBuff, id, g_recvPos );

	sPDESC_DATA pPlayer = FindWaitCharPlayer( id );

	if( !pPlayer )
		return 1;

	gPLAYERS.WaitCharData.remove( pPlayer );
	pPlayer->isWaitCharData = 0;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_NULL_CHARACTER, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );
	return 1;
}


//
// Function Name: MakeNewCharResult
// Date: 2003-03-20
// Description: 
// 
BOOL MakeNewCharResult()
{
	char id[128];
	BYTE result;

	GetString( g_recvBuff, id, g_recvPos );
	result = GetByte( g_recvBuff, g_recvPos );

	sPDESC_DATA pPlayer, temp_player;

	LIST_WHILE( gPLAYERS.list, pPlayer, temp_player, next, WMgr );
	if( !strcmp( GET_ID( pPlayer ), id ) )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_MAKECHAR_RESULT, g_nPos );
		PutByte( g_Packet, result, g_nPos );
		PutSize( g_Packet, g_nPos );
		
		SendData( pPlayer, g_Packet, g_nPos );
		return 1;
	}
	LIST_WHILEEND( gPLAYERS.list, pPlayer, temp_player );

	return 1;
}


//
// Function Name: DeleteCharResult
// Date: 2003-03-20
// Description: 
// 
BOOL DeleteCharResult()
{
	char id[50];
	BYTE result;

	GetString( g_recvBuff, id, g_recvPos );
	result = GetByte( g_recvBuff, g_recvPos );

	sPDESC_DATA pPlayer = FindWaitDelPlayer( id );

	if( !pPlayer )
		return 1;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_DELETE_CHARACTER_RESULT, g_nPos );
	PutByte( g_Packet, result, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	gPLAYERS.WaitDelData.remove( pPlayer );
	pPlayer->isWaitDelData = 0;

	return 1;
}

//
// Function Name: IsConnectedUser
// Date: 
// Description: 
//
BOOL IsConnectedUser()
{
	char id[50];

	GetString( g_recvBuff, id, g_recvPos );

	sPDESC_DATA pPlayer = FindWaitAllPlayer( id );

	if( !pPlayer ) 
		return 1;

	gPLAYERS.WaitAllData.remove( pPlayer );
	pPlayer->isWaitAllData = 0;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_ALREADY_CONNECTED, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );

	return 1;
}


//
// Function Name: MessageDataFuncsList[]
// Date: 2003-03-20
// Description: 
// 
BOOL (*MessageDataFuncsList[])() =
{
	NullDataFunc,	
	NullDataFunc,					//#define dDATA_PACKET_GETSELECTINFO				1
	GetAllPlayerData,				//#define dDATA_PACKET_SENDSELECTINFO				2
	NullDataFunc,					//#define dDATA_PACKET_GETCHARINFO					3
	GetCharInfo,					//#define dDATA_PACKET_GETCHARINFO_RESULT			4
	NullCharData,					//#define dDATA_PACKET_NULLCHAR						5		// 케릭터 존재하지 않음
	NullDataFunc,					//#define dDATA_PACKET_MAKENEWCHAR					6		// 신규 케릭터 생성
	MakeNewCharResult,				//#define dDATA_PACKET_MAKENEWCHAR_RESULT			7		// 신규 케릭터 생성 결과
	NullDataFunc,					//#define dDATA_PACKET_CHAR_LOGOUT					8		// 로그아웃 ( 데이터 저장 )

	NullDataFunc,					//#define dDATA_PACKET_UPDATELEVEL					9		// 레벨업 정보
	NullDataFunc,					//#define dDATA_PACKET_UPDATESKILL					10		// 스킬업 정보
	NullDataFunc,					//#define dDATA_PACKET_UPDATESKILL_EXP				11		// 스킬 경치 정보 

	NullDataFunc,					//#define dDATA_PACKET_UPDATESTR					12
	NullDataFunc,					//#define dDATA_PACKET_UPDATESTR_EXP				13
	NullDataFunc,					//#define dDATA_PACKET_UPDATEINT					14
	NullDataFunc,					//#define dDATA_PACKET_UPDATEINT_EXP				15
	NullDataFunc,					//#define dDATA_PACKET_UPDATEDEX					16
	NullDataFunc,					//#define dDATA_PACKET_UPDATEDEX_EXP				17

	NullDataFunc,					//#define dDATA_PACKET_UPDATEHP						18		// HP
	NullDataFunc,					//#define dDATA_PACKET_UPDATEMP						19		// MP
	NullDataFunc,					//#define dDATA_PACKET_UPDATEMAPINFO				20		// mapNum, posX, posY
	NullDataFunc,					//#define dDATA_PACKET_UPDATEMONEY					21		// 소유 금액

	NullDataFunc,					//#define dDATA_PACKET_DELCHAR						22		// 케릭터 삭제
	DeleteCharResult,				//#define dDATA_PACKET_DELCHAR_RESULT				23		// 케릭터 삭제 결과 
	IsConnectedUser,				//#define dDATA_PACKET_ISCONNECTED					24		// 이미 접속중인 계정
	NullDataFunc,					//#define dDATA_PACKET_UPDATEEQUIP					25		// 장비

	NullDataFunc,					//#define dDATA_PACKET_UPDATEMERC					26		// 용병 정보

	NullDataFunc,					//#define dDATA_PACKET_UPDATECHA					27		// 카리스마

	NullDataFunc,					//#define dDATA_PACKET_UPDATECOUNTRY				28		// 국가
	NullDataFunc,					//#define dDATA_PACKET_UPDATEHOMETOWN				29		// 고향마을

	NullDataFunc,					//#define dDATA_PACKET_UPDATEGUILD					30		// 길드정보
	NullDataFunc,					//#define dDATA_PACKET_UPDATECOUNTRYLEVEL			31		// 국가 레벨

	NullDataFunc,					//#define dDATA_PACKET_CRASHGUILD					32		// 길드 아작
	NullDataFunc,					//#define dDATA_PACKET_UPDATECLASS					33		// 직업 업데이트
};

//
// Function Name: ReadDataRecvBuff
// Date: 2003-03-20
// Description: 
// 
BOOL ReadDataRecvBuff()
{
	WORD msgSize;
	WORD tag;

	CWhileMgr WMgr;

	while( g_recvSize > 0 )
	{
		WMgr.CheckDump( __FILE__, __LINE__ );

		g_recvPos = 0;

		msgSize = GetWord( g_recvBuff, g_recvPos );
		tag = GetWord( g_recvBuff, g_recvPos );

		if( tag > dMAX_DATA_TAG  )
			return 0;

		if( g_recvSize < msgSize )
			return 1;

		if( !MessageDataFuncsList[tag]() )
			return 0;
		
		memmove( &g_recvBuff[0], &g_recvBuff[msgSize], g_recvSize - msgSize );
		g_recvSize -= msgSize;
	}
	return 1;
}

//
// Function Name: SendToDataServer
// Date: 2003-03-20
// Description: 
// 
void SendToDataServer( char *data , int size )
{		
	BYTE overFlow = 1;

	while( ( g_sendSize + size ) >= dMAX_DATA_SOCK_BUFF )
	{
		if( overFlow )
		{
			log( "SendToDataServer : g_sendBuff overFlow!!!!!!!!!!!!!!!!\r\n" );
		}
		overFlow = 0;
		
		if( FlushDataSendBuff() < 0 )
			return;
	}
	memcpy( &g_sendBuff[g_sendSize], data, size );
	g_sendSize += size;
}

//
// Function Name: FlushDataSendBuff
// Date: 2003-03-20
// Description: 
// 
int FlushDataSendBuff()
{
	int sendSize;

	sendSize = send( g_DataSock, g_sendBuff, g_sendSize, 0 );

	if( sendSize > 0 )
	{
		if( sendSize == g_sendSize )
		{
			*g_sendBuff = NULL_STR;
			g_sendSize = 0;
		}
		else
		{
			memmove( &g_sendBuff[0], &g_sendBuff[sendSize], g_sendSize - sendSize );
			g_sendSize -= sendSize;
		}
	}
	else
	{
		// This should never happen! 
		if( sendSize == 0 )
			return -1;
		
#ifdef WIN32
		if( WSAGetLastError() != WSAEWOULDBLOCK &&
			WSAGetLastError() != WSAEINTR )
		{
			return -1;
		}
		else
			return 0;
#else
		if( errno != EWOULDBLOCK &&
			errno != EINTR )
		{
			return -1;
		}
		else
			return 0;
#endif
	}
	return sendSize;
}



///////////////////////////////////////////////////
// Send Update Info
///////////////////////////////////////////////////




//
// Function Name: SendUpdateSkill
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateSkill( sPDESC_DATA pPlayer , BYTE skillNum )
{
	if( skillNum >= dMAX_PLAYER_SKILL )
	{
		log( "SendUpdateSkill : skillNum >= dMAX_SKILL\r\n" );
		return;
	}

	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATESKILL, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutByte( g_Packet, skillNum, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.skill[skillNum], g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}


//
// Function Name: SendUpdateSkillExp
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateSkillExp( sPDESC_DATA pPlayer , BYTE skillNum )
{
	if( skillNum >= dMAX_PLAYER_SKILL )
	{
		log( "SendUpdateSkillExp : skillNum >= dMAX_SKILL\r\n" );
		return;
	}

	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATESKILL_EXP, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutByte( g_Packet, skillNum, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.skill_exp[skillNum], g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}


//
// Function Name: SendUpdateHp
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateHp( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEHP, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, pPlayer->ch.hp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.max_hp, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateMp
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateMp( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEMP, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, pPlayer->ch.mp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.max_mp, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateMapInfo
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateMapInfo( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEMAPINFO, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_MAP_NUM( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_POSX( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_POSY( pPlayer ), g_nPos );
	PutByte( g_Packet, GET_LAYER( pPlayer ), g_nPos );
	PutByte( g_Packet, GET_MAP_KIND( pPlayer ), g_nPos );
	PutInteger( g_Packet, pPlayer->ch.prevMapNum, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.prevPosX, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.prevPosY, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateMoney
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateMoney( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEMONEY, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_MONEY( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}


void DATASERV_SendUpdateCash( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATE_CASH, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_CASHMONEY( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateStr
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateStr( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATESTR, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_STR( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateStrExp
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateStrExp( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATESTR_EXP, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, pPlayer->ch.str_exp, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateInt
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateInt( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEINT, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_INT( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}


//
// Function Name: SendUpdateIntExp
// Date: 2003-03-20
// Description: 
// 
void DATASERV_SendUpdateIntExp( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEINT_EXP, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, pPlayer->ch.intelli_exp, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateDex
// Date:2003/03/03
// Description: Dex 업데이트
//
void DATASERV_SendUpdateDex( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEDEX, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_DEX( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateDexExp
// Date:2003/03/03
// Description: Dex 경험치 업데이트
//
void DATASERV_SendUpdateDexExp( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEDEX_EXP, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, pPlayer->ch.dex_exp, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: SendUpdateChEquip
// Date:2003/04/08
// Description: 
//
void DATASERV_SendUpdateChEquip( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEEQUIP, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );

	PutWord( g_Packet, pPlayer->equip[dEQUIP_SHOES] ? pPlayer->equip[dEQUIP_SHOES]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_GLOVE] ? pPlayer->equip[dEQUIP_GLOVE]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_HELMET] ? pPlayer->equip[dEQUIP_HELMET]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_JACKET] ? pPlayer->equip[dEQUIP_JACKET]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_PANTS] ? pPlayer->equip[dEQUIP_PANTS]->itemNum : 0, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}


//
// Function Name: SendUpdateMercInfo
// Date:2003/05/30
// Description: 
//
void DATASERV_SendUpdateMercInfo( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEMERC, g_nPos );

	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutByte( g_Packet, GET_MERC_TYPE( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_MERC_LEVEL( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_MERC_EXP( pPlayer ), g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}



//
// Function Name: DATASERV_SendUpdateCha
// Date: 
// Description: 
//
void DATASERV_SendUpdateCha( sPDESC_DATA pPlayer )
{
	g_nPos =2 ;
	PutWord( g_Packet, dDATA_PACKET_UPDATECHA, g_nPos );

	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_CHA( pPlayer ) , g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}


//
// Function Name: DATASERV_SendUpdateGuildInfo
// Date: 
// Description: 
//
void DATASERV_SendUpdateGuildInfo( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dDATA_PACKET_UPDATEGUILD, g_nPos );

	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_GUILD( pPlayer ) , g_nPos );
	PutInteger( g_Packet, GET_GUILD_LEVEL( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_TMP_GUILD( pPlayer ), g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}


//
// Function Name: DATASERV_CrashGuild
// Date: 
// Description: 
//
void DATASERV_CrashGuild( int guildNo )
{
	g_nPos = 2;

	PutWord( g_Packet, dDATA_PACKET_CRASHGUILD, g_nPos );
	PutInteger( g_Packet, guildNo, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

//
// Function Name: DATASERV_UpdateClass
// Date: 
// Description: 
//
void DATASERV_UpdateClass( sPDESC_DATA pPlayer )
{
	g_nPos = 2;

	PutWord( g_Packet, dDATA_PACKET_UPDATECLASS, g_nPos );
	PutString( g_Packet, GET_ID( pPlayer ), g_nPos );
	PutByte( g_Packet, GET_CLASS( pPlayer ), g_nPos );
	
	PutSize( g_Packet, g_nPos );

	SendToDataServer( g_Packet, g_nPos );
}

void DATASERV_SAVECHA( sPDESC_DATA pPlayer )
{
	DATASERV_SendUpdateHp( pPlayer );
	DATASERV_SendUpdateMp( pPlayer );
	DATASERV_SendUpdateMapInfo( pPlayer );
	DATASERV_SendUpdateMoney( pPlayer );
	DATASERV_SendUpdateCash( pPlayer );
	DATASERV_SendUpdateStr( pPlayer );
	DATASERV_SendUpdateStrExp( pPlayer );
	DATASERV_SendUpdateInt( pPlayer );
	DATASERV_SendUpdateIntExp( pPlayer );
	DATASERV_SendUpdateDex( pPlayer );
	DATASERV_SendUpdateDexExp( pPlayer );
	DATASERV_SendUpdateCha( pPlayer );
	DATASERV_UpdateClass( pPlayer );
	g_nPos = 2;
	//캐릭터 세이브
	PutWord( g_Packet, dDATA_PACKET_SAVE_CHA, g_nPos );
	PutString( g_Packet, pPlayer->ch.id, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendToDataServer( g_Packet, g_nPos );
}

// EOF
