/**
 *	$Workfile: guild.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 27 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/guild.cpp $
 * 
 * 27    04-12-22 5:18p Sk8snow
 * 
 * 26    04-10-29 11:56p Sk8snow
 * 
 * 25    04-09-08 3:54p Sk8snow
 * 
 * 24    04-07-13 7:26a Sk8snow
 * 
 * 23    04-05-29 10:12p Sk8snow
 * 
 * 22    04-05-21 11:37a Sk8snow
 * 
 * 21    04-05-18 1:17a Sk8snow
 * 
 * 20    04-05-14 10:38p Sk8snow
 * 
 * 19    04-05-11 1:45a Sk8snow
 * 
 * 18    04-04-30 9:46a Sk8snow
 * 
 * 17    04-03-31 8:22a Sk8snow
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
#include "quest.h"

#include "house.h"
#include "guild.h"

#include "db_rows.h"

sGUILD gGUILDS;


//
// Function Name: GUILD_LoadGuildInfo
// Date: 
// Description: 
//
BOOL GUILD_LoadGuildInfo()
{
	for( int i = 0; i < dMAX_GUILD; i++ )
	{
		gGUILDS.array[i] = NULL;
	}
	
	MYSQL *pDB = DBConnect( g_ITEMDBINFO.host,
						g_ITEMDBINFO.port,
						g_ITEMDBINFO.id,
						g_ITEMDBINFO.pw,
						g_ITEMDBINFO.dbName );
	
	if( !pDB )
		return 0;
	
	char query[dMAX_QUERY_SIZE];
	
	sprintf( query, "select * from GuildData order by guildNo" );
	
	MYSQL_RES *result;
	MYSQL_ROW row;

	int guildNo;
	int guildMaxNo = 0;
	int totalGuildCnt = 0;
		
	if( !mysql_query( pDB, query ) )
	{
		result = mysql_store_result( pDB );
		
		if( result )
		{			
			while( ( row = mysql_fetch_row( result ) ) )
			{			
				guildNo = atoi( row[GUILDDATA_NUM] );

				if( guildMaxNo < guildNo )
					guildMaxNo = guildNo;

				if( guildNo >= dMAX_GUILD )
				{
					log( "GUILD_LoadGuildInfo : guildNo is over\r\n" );
					mysql_close( pDB );
					return 0;
				}

				if( IS_VALID_GUILD( guildNo ) )
				{
					log( "GUILD_LoadGuildInfo : IS_VALID_GUILD( guildNo )\r\n" );
					mysql_close( pDB );
					return 0;
				}
				
				gGUILDS.array[guildNo] = NewGuildData();

				if( !IS_VALID_GUILD( guildNo ) )
				{
					log( "GUILD_LoadGuildInfo : malloc failed\r\n" );
					mysql_close( pDB );
					return 0;
				}

				GET_GUILD_DATA( guildNo )->guildNum = guildNo;
				GET_GUILD_DATA( guildNo )->memberCnt = atoi( row[GUILDDATA_MEMBERCNT] );
				strcpy( GET_GUILD_DATA( guildNo )->masterName, row[GUILDDATA_MASTERNAME] );
				GET_GUILD_DATA( guildNo )->guildType = atoi( row[GUILDDATA_TYPE] );
				strcpy( GET_GUILD_DATA( guildNo )->guildName, row[GUILDDATA_NAME] );
				GET_GUILD_DATA( guildNo )->guildMark = atoi( row[GUILDDATA_MARK] );
				GET_GUILD_DATA( guildNo )->guildPoint = atoi( row[GUILDDATA_POINT]);

				// 리스트 입력
				INSERT_TO_LIST( gGUILDS.list, GET_GUILD_DATA( guildNo ), prev, next );

				totalGuildCnt++;
			}
			mysql_free_result( result );
		}
	}
	else
	{
		mysql_close( pDB );
		return 0;
	}

	log( "LoadGuildInfo : maxGuild [%d] total [%d/%d]\r\n", guildMaxNo, totalGuildCnt, dMAX_GUILD );
	
	mysql_close( pDB );
	return 1;	
}


//
// Function Name: GetNewGuildNum
// Date: 
// Description: 
//
int GetNewGuildNum()
{
	for( int i = 1; i < dMAX_GUILD; i++ )
	{
		if( !IS_VALID_GUILD( i ) )
			return i;
	}
	return -1;
}


//
// Function Name: NewGuildData
// Date: 
// Description: 
//
sPGUILD_DATA NewGuildData()
{
	sPGUILD_DATA pGuild = NULL;

	if( gGUILDS.memory )
	{
		pGuild = gGUILDS.memory;

		// Output Memory
		REMOVE_FROM_LIST( gGUILDS.memory, pGuild, prev, next );
	}
	else
	{
		pGuild = (sPGUILD_DATA)malloc(sizeof(sGUILD_DATA));

		if( !pGuild )
			return NULL;
	}

	pGuild->guildNum = 0;						// 길드번호 
	*pGuild->guildName = NULL_STR;				// 길드이름
	*pGuild->masterName = NULL_STR;				// 마스터 이름 
	
	pGuild->memberCnt = 0;						// 멤버수 

	pGuild->guildType = 0;						// 길드타입( 용병단이냐 군단이냐 )
	pGuild->guildMark = 0;						// 길드마크

	pGuild->makeDate = 0;						// 생성 일자
	
	pGuild->memberList = NULL;					// 멤버 리스트 
	pGuild->mercenaryList = NULL;				// 용병 리스트
	pGuild->memberInfoList = NULL;				// 맴버 리스트
	*pGuild->notice = NULL_STR;					// 공지 

	pGuild->guildPoint = 0;						// 길드포인트


	return pGuild;
}


//
// Function Name: DeleteGuildData
// Date: 
// Description: 
//
void DeleteGuildData( sPGUILD_DATA pGuild )
{
	if( !pGuild )
		return;

	GET_GUILD_DATA( pGuild->guildNum ) = NULL;

	sPMEMBER_INFO info, next_info;

	LIST_WHILE( pGuild->memberInfoList, info, next_info, next, WMgr );
	REMOVE_FROM_LIST( pGuild->memberInfoList, info, prev, next );
	INSERT_TO_LIST( gGUILDS.member_memory, info, prev, next );
	LIST_WHILEEND( pGuild->memberInfoList, info, next_info );

	REMOVE_FROM_LIST( gGUILDS.list, pGuild, prev, next );
	INSERT_TO_LIST( gGUILDS.memory, pGuild, prev, next );
	pGuild = NULL;
}


//
// Function Name: DestroyGuildData
// Date: 
// Description: 
//
void DestroyGuildData()
{
	sPGUILD_DATA data, next_data;

	// 길드 리스트
	LIST_WHILE( gGUILDS.list, data, next_data, next, WMgr );
	DeleteGuildData( data );
	LIST_WHILEEND( gGUILDS.list, data, next_data );

	// Clear Memory List
	LIST_WHILE( gGUILDS.memory, data, next_data, next, WMgr2 );
	REMOVE_FROM_LIST( gGUILDS.memory, data, prev, next );
	SAFE_FREE( data );
	LIST_WHILEEND( gGUILDS.memory, data, next_data );

	sPMEMBER_INFO info, next_info;

	// Clear Member Info Memory
	LIST_WHILE( gGUILDS.member_memory, info, next_info, next, WMgr3 );
	REMOVE_FROM_LIST( gGUILDS.member_memory, info, prev, next );
	SAFE_FREE( info );
	LIST_WHILEEND( gGUILDS.member_memory, info, next_info );
}


//
// Function Name: CreateNewGuild
// Date: 
// Description: 
//
BOOL CreateNewGuild( sPDESC_DATA pMaster , char *guildName , int guildMark )
{
	if( GET_ALL_SKILL( pMaster ) < 25 )
	{
		SendSystemMsg( pMaster, g_LANG_STR[122] );
		return 0;
	}

	if( GET_MONEY( pMaster ) < 3000000 )
	{
		SendSystemMsg( pMaster, g_LANG_STR[123], 3000000 );
		return 0;
	}

	// 이름 중복체크
	for( int i = 0; i < dMAX_GUILD; i++ )
	{
		if( IS_VALID_GUILD( i ) )
		{
			if( !strcmp( GET_GUILD_DATA( i )->guildName, guildName ) )
			{
				SendSystemMsg( pMaster, g_LANG_STR[2] );
				return 0;
			}
		}
	}

	if( GET_GUILD( pMaster ) )
	{
		SendSystemMsg( pMaster, g_LANG_STR[68] );
		return 0;
	}

	if( GET_TMP_GUILD( pMaster ) )
	{
		SendSystemMsg( pMaster, g_LANG_STR[69] );
		return 0;
	}

	int guildNo = GetNewGuildNum();

	// err
	if( guildNo == -1 )
		return 0;

	sPGUILD_DATA pNewGuild = NewGuildData();

	if( !pNewGuild )
	{
		log( "CreateNewGuild : NewGuildData return NULL\r\n" );
		return 0;
	}

	char query[dMAX_QUERY_SIZE];
	char escMasterName[128];
	char escGuildName[128];

	mysql_escape_string( escMasterName, GET_NAME( pMaster ), strlen( GET_NAME( pMaster ) ) );
	mysql_escape_string( escGuildName, guildName, strlen( guildName ) );

	int guildPoint = pNewGuild->guildPoint;

	sprintf( query, "insert into GuildData values( '', %d, '%s', 1, 1, '%s', %d, %d )", 
		guildNo, escMasterName, escGuildName , guildMark, guildPoint );
	
	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
		SAFE_FREE( pNewGuild );
		return 0;
	}
	else
	{
		GET_GUILD( pMaster ) = guildNo;
		GET_GUILD_LEVEL( pMaster ) = dGUILD_LEVEL_MASTER;

		DATASERV_SendUpdateGuildInfo( pMaster );
		
		pNewGuild->guildNum = guildNo;
		pNewGuild->guildType = dGUILD_TYPE_MERCENARY;
		strcpy( pNewGuild->masterName, GET_NAME( pMaster ) );
		strcpy( pNewGuild->guildName, guildName );
		pNewGuild->guildMark = guildMark;
		pNewGuild->makeDate = g_CurrRealTime;
		pNewGuild->memberCnt = 1;
		
		GET_GUILD_DATA( guildNo ) = pNewGuild;

		// 리스트 입력
		INSERT_TO_LIST( gGUILDS.list, GET_GUILD_DATA( guildNo ), prev, next );

		// 길드정보 전송
		GUILD_SendGuildInfo( pMaster, dSENDMODE_TOALL );

		INSERT_TO_LIST( pNewGuild->memberList, pMaster, guild_prev, guild_next );

		sPMEMBER_INFO pMemberInfo = GUILD_NewMemberInfo();
		
		if( pMemberInfo )
		{
			strcpy( pMemberInfo->name, GET_NAME( pMaster ) );
			pMemberInfo->level = GET_ALL_SKILL( pMaster );
			pMemberInfo->memberLevel = GET_GUILD_LEVEL( pMaster );
			pMemberInfo->btClass = GET_CLASS( pMaster );
			pMemberInfo->btKickOut = 0;
			pMemberInfo->prev = NULL;
			pMemberInfo->next = NULL;
			
			INSERT_TO_LIST( pNewGuild->memberInfoList, pMemberInfo, prev, next );
		}

		GET_MONEY( pMaster ) = MIN( GET_MONEY( pMaster ) - 3000000, 0 );
		UpdateMoney( pMaster );
		DATASERV_SendUpdateMoney( pMaster );
	}
	return 1;
}

//
// Function Name: CrashGuild
// Date: 
// Description: 
//
void CrashGuild( int guildNo )
{
	if( !IS_VALID_GUILD( guildNo ) )
		return;

	char query[dMAX_QUERY_SIZE];
	
	sprintf( query, "delete from GuildData where guildNo = %d", guildNo );
	
	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
		return;
	}

	sPDESC_DATA player, next_player;

	// 
	LIST_WHILE( GET_GUILD_DATA( guildNo )->memberList, player, next_player, guild_next, WMgr );

	GET_GUILD_LEVEL( player ) = 0;
	GET_GUILD( player ) = 0;
	DATASERV_SendUpdateGuildInfo( player );
	GUILD_SendGuildInfo( player, dSENDMODE_TOALL );

	REMOVE_FROM_LIST( GET_GUILD_DATA( guildNo )->memberList, player, guild_prev, guild_next );

	LIST_WHILEEND( GET_GUILD_DATA( guildNo )->memberList, player, next_player );

	DATASERV_CrashGuild( guildNo );

	// Delete Data 
	DeleteGuildData( GET_GUILD_DATA( guildNo ) );
}

//
// Function Name: GUILD_AddMember
// Date: 
// Description: 
//
void GUILD_AddMember( sPDESC_DATA pNewbie, int guildNo )
{
	if( !IS_VALID_GUILD( guildNo ) )
		return;

	if( GET_GUILD( pNewbie ) )
		return;

	GET_GUILD( pNewbie ) = guildNo;
	GET_GUILD_LEVEL( pNewbie ) = dGUILD_LEVEL_MEMBER;

	DATASERV_SendUpdateGuildInfo( pNewbie );
	GUILD_SendGuildInfo( pNewbie, dSENDMODE_TOALL );

	INSERT_TO_LIST( GET_GUILD_DATA( guildNo )->memberList, pNewbie, guild_prev, guild_next );

	sPMEMBER_INFO pMemberInfo = GUILD_NewMemberInfo();

	if( pMemberInfo )
	{
		strcpy( pMemberInfo->name, GET_NAME( pNewbie ) );
		pMemberInfo->level = GET_ALL_SKILL( pNewbie );
		pMemberInfo->memberLevel = GET_GUILD_LEVEL( pNewbie );
		pMemberInfo->btClass = GET_CLASS( pNewbie );
		pMemberInfo->btKickOut = 0;
		pMemberInfo->prev = NULL;
		pMemberInfo->next = NULL;

		INSERT_TO_LIST( GET_GUILD_DATA( guildNo )->memberInfoList, pMemberInfo, prev, next );
	}
}

//
// Function Name: GUILD_AddMercenary
// Date: 
// Description: 
//
void GUILD_AddMercenary( sPDESC_DATA pNewbie, int guildNo )
{
	if( !IS_VALID_GUILD( guildNo ) )
		return;

	if( GET_TMP_GUILD( pNewbie ) )
		return;

	GET_TMP_GUILD( pNewbie ) = guildNo;

	DATASERV_SendUpdateGuildInfo( pNewbie );
	GUILD_SendGuildInfo( pNewbie, dSENDMODE_TOALL );

	INSERT_TO_LIST( GET_GUILD_DATA( guildNo )->mercenaryList, pNewbie, merc_prev, merc_next );
}


//
// Function Name: GUILD_DropMercenary
// Date: 
// Description: 
//
void GUILD_DropMercenary( sPDESC_DATA pMember, int guildNo )
{
	if( !IS_VALID_GUILD( guildNo ) )
		return;

	if( !GET_TMP_GUILD( pMember ) )
		return;

	GET_TMP_GUILD( pMember ) = 0;

	DATASERV_SendUpdateGuildInfo( pMember );
	GUILD_SendGuildInfo( pMember, dSENDMODE_TOALL );

	REMOVE_FROM_LIST( GET_GUILD_DATA( guildNo )->mercenaryList, pMember, merc_prev, merc_next );
}


//
// Function Name: GUILD_DropMember
// Date: 
// Description: 
//
void GUILD_DropMember( sPDESC_DATA pMember, int guildNo )
{
	if( !IS_VALID_GUILD( guildNo ) )
		return;
	
	if( GET_GUILD( pMember ) != guildNo )
		return;

	GET_GUILD( pMember ) = 0;
	GET_GUILD_LEVEL( pMember ) = 0;

	DATASERV_SendUpdateGuildInfo( pMember );
	GUILD_SendGuildInfo( pMember, dSENDMODE_TOALL );

	REMOVE_FROM_LIST( GET_GUILD_DATA( guildNo )->memberList, pMember, guild_prev, guild_next );

	sPMEMBER_INFO pMemberInfo = GUILD_FindMemberInfoFromList( GET_GUILD_DATA( guildNo ), GET_NAME( pMember ) );

	if( pMemberInfo )
	{
		REMOVE_FROM_LIST( GET_GUILD_DATA( guildNo )->memberInfoList, pMemberInfo, prev, next );
		INSERT_TO_LIST( gGUILDS.member_memory, pMemberInfo, prev, next );
	}
}

//
// Function Name: GUILD_SendGuildInfo
// Date: 
// Description: 
//
void GUILD_SendGuildInfo( sPDESC_DATA pPlayer , BYTE mode )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_GUILDINFO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	
	// 길드명
	if( GET_GUILD( pPlayer ) && !GET_TMP_GUILD( pPlayer ) )
	{
		if( IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		{
			PutString( g_Packet, GET_GUILD_DATA( GET_GUILD( pPlayer ) )->guildName, g_nPos );
			PutWord( g_Packet, GET_GUILD_DATA( GET_GUILD( pPlayer ) )->guildMark, g_nPos );
		}
		else
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, 0, g_nPos );
		}
	}
	else if( GET_GUILD( pPlayer ) && GET_TMP_GUILD( pPlayer ) )
	{
		if( IS_VALID_GUILD( GET_GUILD( pPlayer ) ) &&
			IS_VALID_GUILD( GET_TMP_GUILD( pPlayer ) ) )
		{
			PutString( g_Packet, GET_GUILD_DATA( GET_TMP_GUILD( pPlayer ) )->guildName, g_nPos );
			PutWord( g_Packet, GET_GUILD_DATA( GET_TMP_GUILD( pPlayer ) )->guildMark, g_nPos );
		}
		else
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, 0, g_nPos );
		}
	}
	else if( !GET_GUILD( pPlayer ) && GET_TMP_GUILD( pPlayer ) )
	{
		if( IS_VALID_GUILD( GET_TMP_GUILD( pPlayer ) ) )
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, GET_GUILD_DATA( GET_TMP_GUILD( pPlayer ) )->guildMark, g_nPos );
		}
		else
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, 0, g_nPos );
		}
	}
	else
	{
		PutString( g_Packet, " ", g_nPos );
		PutWord( g_Packet, 0, g_nPos );
	}  
	// 길드정보 END
	PutByte( g_Packet, GET_GUILD_LEVEL( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_GUILD( pPlayer ), g_nPos );

	PutSize( g_Packet, g_nPos );

	if( mode == dSENDMODE_TOOWNER )
		SendData( pPlayer, g_Packet, g_nPos );
	else
		SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}

//
// Function Name: GUILD_SendToMember
// Date: 
// Description: 
//
void GUILD_SendToMember( sPGUILD_DATA pGuild , char *msg, int size )
{
	sPDESC_DATA p, next_p;
	
	LIST_WHILE( pGuild->memberList, p, next_p, guild_next, WMgr );
	SendData( p, msg, size );
	LIST_WHILEEND( pGuild->memberList, p, next_p );
}

//
// Function Name: GUILD_SendToMercenery
// Date: 
// Description: 
//
void GUILD_SendToMercenery( sPGUILD_DATA pGuild, char *msg, int size )
{
	sPDESC_DATA p, next_p;

	LIST_WHILE( pGuild->mercenaryList, p, next_p, merc_next, WMgr );
	SendData( p, msg, size );
	LIST_WHILEEND( pGuild->mercenaryList, p, next_p );
}

//
// Function Name: GUILD_ChangeMaster
// Date: 
// Description: 
//
void GUILD_ChangeMaster( sPDESC_DATA pMaster, sPDESC_DATA pNewMaster )
{
	// 길드 유효성 검사 
	if( !IS_VALID_GUILD( GET_GUILD( pMaster ) ) )
		return;

	// 길드 마스터인지 
	if( GET_GUILD_LEVEL( pMaster ) != dGUILD_LEVEL_MASTER )
		return;

	// 같은 길드인지
	if( GET_GUILD( pMaster ) != GET_GUILD( pNewMaster ) )
		return;

	// 예외처리;; ( 일어나면 안될일 ) 
	if( GET_GUILD_LEVEL( pMaster ) <= GET_GUILD_LEVEL( pNewMaster ) )
	{
		log( "GUILD_ChangeMaster : GET_GUILD_LEVEL( pMaster ) <= GET_GUILD_LEVEL( pNewMaster )\r\n" );
		return;
	}

	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pMaster ) );

	if( !pGuild )
		return;

	// 권한 변경
	GET_GUILD_LEVEL( pMaster ) = dGUILD_LEVEL_MEMBER;
	GET_GUILD_LEVEL( pNewMaster ) = dGUILD_LEVEL_MASTER;

	DATASERV_SendUpdateGuildInfo( pMaster );
	DATASERV_SendUpdateGuildInfo( pNewMaster );
	
	SendSystemMsg( pMaster, g_LANG_STR[151], GET_NAME( pNewMaster ) );
	SendSystemMsg( pNewMaster, g_LANG_STR[152], GET_NAME( pMaster ) );

	sPMEMBER_INFO pMemberInfo = GUILD_FindMemberInfoFromList( pGuild, GET_NAME( pMaster ) );

	if( pMemberInfo )
		pMemberInfo->memberLevel = GET_GUILD_LEVEL( pMaster );

	pMemberInfo = GUILD_FindMemberInfoFromList( pGuild, GET_NAME( pNewMaster ) );

	if( pMemberInfo )
		pMemberInfo->memberLevel = GET_GUILD_LEVEL( pNewMaster );
}

//
// Function Name: GUILD_NewMemberInfo
// Date: 
// Description: 
//
sPMEMBER_INFO GUILD_NewMemberInfo()
{
	sPMEMBER_INFO pInfo = NULL;

	if( gGUILDS.member_memory )
	{
		pInfo = gGUILDS.member_memory;

		REMOVE_FROM_LIST( gGUILDS.member_memory, pInfo, prev, next );
	}
	else
	{
		pInfo = (sPMEMBER_INFO)malloc( sizeof( sMEMBER_INFO ) );

		if( !pInfo )
			return NULL;
	}

	*pInfo->name = NULL_STR;
	
	pInfo->level = 0;
	
	pInfo->memberLevel = dGUILD_LEVEL_NONE;
	pInfo->btClass = dCLASS_NONE;
	pInfo->btKickOut = 0;
	
	pInfo->prev = NULL;
	pInfo->next = NULL;

	return pInfo;
}

//
// Function Name: GUILD_FindMemberInfoFromList
// Date: 
// Description: 
// 
sPMEMBER_INFO GUILD_FindMemberInfoFromList( sPGUILD_DATA pGuild, char *name )
{
	sPMEMBER_INFO info, next_info;

	LIST_WHILE( pGuild->memberInfoList, info, next_info, next, WMgr );
	
	if( !strcmp( name, info->name ) )
		return info;

	LIST_WHILEEND( pGuild->memberInfoList, info, next_info );

	return NULL;
}

//
// Function Name: GUILD_ChangeMemberInfo
// Date: 
// Description: 
// 
void GUILD_ChangeMemberInfo( sPDESC_DATA pPlayer )
{
	if( !GET_GUILD( pPlayer ) )
		return;

	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return;

	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

	if( !pGuild )
		return;

	sPMEMBER_INFO pMemberInfo = GUILD_FindMemberInfoFromList( pGuild, GET_NAME( pPlayer ) );

	if( pMemberInfo )
	{
		pMemberInfo->level = GET_ALL_SKILL( pPlayer );
		pMemberInfo->memberLevel = GET_GUILD_LEVEL( pPlayer );
		pMemberInfo->btClass = GET_CLASS( pPlayer );

		if( pMemberInfo->btKickOut )
		{
			GET_GUILD( pPlayer ) = 0;
			GET_GUILD_LEVEL( pPlayer ) = dGUILD_LEVEL_NONE;
			DATASERV_SendUpdateGuildInfo( pPlayer );
			pPlayer->aliveCheckTick = 1;
		}
	}
}


int GUILD_MemberCount( sPGUILD_DATA pGuild )
{
	int cnt = 0;

	sPMEMBER_INFO info, next_info;

	LIST_WHILE( pGuild->memberInfoList, info, next_info, next, WMgr );
	cnt++;
	LIST_WHILEEND( pGuild->memberInfoList, info, next_info );

	return cnt;
}

//
// Function Name: GUILD_LoadMemberInfo
// Date: 
// Description: 
// 
void GUILD_LoadMemberInfo()
{
	char query[dMAX_QUERY_SIZE];

	sprintf( query, "select name, skill_level0+skill_level1+skill_level2+skill_level3, guildNo, guildLevel, charClass from UserTable where guildNo != 0" );

	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
		return;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	
	result = mysql_store_result( g_MYSQL );
	
	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			int guildNo = atoi( row[2] );
			int guildLevel = atoi( row[3] );
			int level = atoi( row[1] );
			int charClass = atoi( row[4] );

			if( !IS_VALID_GUILD( guildNo ) )
				continue;

			sPGUILD_DATA pGuild = GET_GUILD_DATA( guildNo );
			
			if( !pGuild )
				continue;

			sPMEMBER_INFO pMemberInfo = GUILD_NewMemberInfo();

			if( pMemberInfo )
			{
				strcpy( pMemberInfo->name, row[0] );
				pMemberInfo->level = level;
				pMemberInfo->memberLevel = guildLevel;
				pMemberInfo->btClass = (BYTE)charClass;
				pMemberInfo->btKickOut = 0;
				pMemberInfo->next = NULL;
				pMemberInfo->prev = NULL;

				INSERT_TO_LIST( pGuild->memberInfoList, pMemberInfo, prev, next );
			}
		}
		mysql_free_result( result );
	}

	
}

//
// Function Name: GUILD_SendGuildInfo
// Date: 
// Description: 
// 
void GUILD_SendGuildMember( sPDESC_DATA pPlayer )
{
	if( !GET_GUILD( pPlayer ) )
		return;

	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return;

	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

	if( !pGuild )
		return;

	WORD totalCnt = 0;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_GUILD_MEMBERLIST, g_nPos );
	g_nPos += sizeof( WORD );

	sPMEMBER_INFO info, next_info;

	LIST_WHILE( pGuild->memberInfoList, info, next_info, next, WMgr );
	
	PutString( g_Packet, info->name, g_nPos );
	PutInteger( g_Packet, info->level, g_nPos );
	PutByte( g_Packet, info->btClass, g_nPos );
//	PutByte( g_Packet, info->memberLevel, g_nPos );
	totalCnt++;
	
	LIST_WHILEEND( pGuild->memberInfoList, info, next_info );
	*(WORD*)( g_Packet + 4 ) = totalCnt;

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: GUILD_AdvanceLevel
// Date: 
// Description: 
//
void GUILD_AdvanceLevel( sPDESC_DATA pPlayer , BYTE guildLevel )
{
	GET_GUILD_LEVEL( pPlayer ) = guildLevel;

	DATASERV_SendUpdateGuildInfo( pPlayer );

	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

	GUILD_SendGuildInfo( pPlayer, dSENDMODE_TOALL );

	if( pGuild )
	{
		sPMEMBER_INFO pMemberInfo = GUILD_FindMemberInfoFromList( pGuild, GET_NAME( pPlayer ) );

		if( pMemberInfo )
		{
			pMemberInfo->memberLevel = GET_GUILD_LEVEL( pPlayer );
		}
	}
}

//
// Function Name: GUILD_CountGuildLevel
// Date: 
// Description: 
//
int GUILD_CountGuildLevel( sPGUILD_DATA pGuild, BYTE guildLevel )
{
	int cnt = 0;

	sPMEMBER_INFO info, next_info;

	LIST_WHILE( pGuild->memberInfoList, info, next_info, next, WMgr );
	if( info->memberLevel == guildLevel )
		cnt++;
	LIST_WHILEEND( pGuild->memberInfoList, info, next_info );
	return cnt;
}


//
// Function Name: GUILD_CountHaveCastle
// Date: 
// Description: 
//
int GUILD_CountHaveCastle( int guildNum )
{
	sPLIST_BLOCK list, block, next_block;
	sPWORLD pWorld;
	int cnt = 0;

	list = gWORLDLIST.GetFirstBlock();
	
	LIST_WHILE( list, block, next_block, next, WMgr );
	
	pWorld = (sPWORLD)block->data;

	if( pWorld->ownerGuild == guildNum )
		cnt++;
	
	LIST_WHILEEND( list, block, next_block );

	return cnt;
}


//EOF



