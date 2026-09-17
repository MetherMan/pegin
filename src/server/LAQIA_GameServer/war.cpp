/**
*	$Workfile: war.cpp $
*
*	Copyright (c) 2003 팀메이, All rights reserved.
*
*	Author	sk8snow
* 
*
*	$Revision: 19 $
*	$Date: 04-12-22 5:18p $
*     
*	@Build Log :
*	$Log: /GameServer/war.cpp $
* 
* 19    04-12-22 5:18p Sk8snow
* 
* 18    04-12-15 10:40a Sk8snow
* 
* 17    04-10-29 11:56p Sk8snow
* 
* 16    04-09-08 3:54p Sk8snow
* 
* 15    04-08-04 9:27a Sk8snow
* 
* 14    04-07-13 7:26a Sk8snow
* 
* 13    04-05-29 10:12p Sk8snow
* 
* 12    04-05-21 11:37a Sk8snow
* 
* 11    04-05-11 1:45a Sk8snow
* 
* 10    04-04-30 9:46a Sk8snow
* 
* 9     04-04-07 10:17p Sk8snow
* 
* 8     04-04-01 5:34p Sk8snow
* 
* 7     04-03-31 8:22a Sk8snow
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

#include "pet.h"

#include "war.h"
#include "db_rows.h"

#include "usestring.h"

//
// Function Name: LoadWarInfo
// Date: 
// Description: 
//
BYTE LoadWarInfo()
{
	FILE *fp = fopen( dWAR_INFO_TABLE_FILE, "rb" );
	
	if( !fp )
	{
		log( "LoadWarInfo : file load faild!\r\n" );
		return 0;
	}
	
	char readBuff[1024];
	
	int mapNum;
	int linkedMap[dMAX_LINKED_CASTLE];
	int campTarget;
	int i;
	
	while( 1 )
	{
		if( feof( fp ) )
			break;
		
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		if( sscanf( readBuff, "%d %d %d %d %d %d", 
			&mapNum, &linkedMap[0], &linkedMap[1], &linkedMap[2], &linkedMap[3], &campTarget ) != 6 )
		{
			log( "LoadWarInfo : Bad file format!\r\n" );
			fclose( fp );
			return 0;
		}
		
		// 맵번호 검사 
		if( !IS_VALID_WORLD( mapNum ) )
		{
			log( "LoadWarInfo : [%d] bad map number\r\n", mapNum );
			fclose( fp );
			return 0;
		}
		
		// 인접성 정보 유효성 검사 
		for( i = 0; i < dMAX_LINKED_CASTLE; i++ )
		{
			if( linkedMap[i] )
			{
				if( !IS_VALID_WORLD( linkedMap[i] ) )
				{
					log( "LoadWarInfo : bad linked map [map:%d slot:%d linkMap:%d]\r\n", mapNum, i, linkedMap[i] );
					fclose( fp );
					return 0;
				}
			}
		}
		
		// 진지의 대상맵 유효성 검사 
		if( campTarget )
		{
			if( !IS_VALID_WORLD( campTarget ) )
			{
				log( "LoadWarInfo : Bad Camp Target [%d]\r\n", mapNum );
				fclose( fp );
				return 0;
			}
		}
		
		// 인접성 정보 대입
		for( i = 0; i < dMAX_LINKED_CASTLE; i++ )
		{
			gWORLD[mapNum]->linkedCastle[i] = linkedMap[i];
			
			if( gWORLD[mapNum]->linkedCastle[i] )
				gWORLD[mapNum]->hasCastle = 1;
		}
		
		gWORLD[mapNum]->campTarget = campTarget;
	}
	
	fclose( fp );
	
	return 1;
}


//
// Function Name: IsCanMakeCampDate
// Date: 
// Description: 
//
BYTE IsCanMakeCampDate()
{
#ifdef NOCHECK_WARTIME
	return 1;
#endif
	
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	
	// 월요일 
	if( currTM->tm_wday == 6 && currTM->tm_hour >= 21 && currTM->tm_hour <= 22 )	
		return 1;
	else
		return 0;
	return 0;
}


//
// Function Name: IsBeginWarTime
// Date: 
// Description: 
//
BYTE IsBeginWarTime( BYTE mode )
{
	if( mode )
		return 1;
	
#ifdef NOCHECK_WARTIME
	return 1;
#endif
	
	time_t tval = g_CurrRealTime;
	struct tm *currTM = localtime( &tval );
	
	// 툐요일 19시 ( 오후 7시 ) 공성시작
	if( currTM->tm_wday == 6 && currTM->tm_hour == 19 )
		return 1;
	else
		return 0;
	return 0;
}

//
// Function Name: IsFinishWarTime
// Date: 
// Description: 
//
BYTE IsFinishWarTime( BYTE mode )
{
	if( mode )
		return 1;
	
#ifdef NOCHECK_WARTIME
	return 1;
#endif
	time_t tval = g_CurrRealTime;
	struct tm *currTM = localtime( &tval );
	
	// 툐요일 21시 ( 오후 9시 ) 공성종료
	if( currTM->tm_wday == 6 && currTM->tm_hour == 21 )
		return 1;
	else
		return 0;
	
	return 0;
}

//
// Function Name: IsFinishWarTime
// Date: 
// Description: 
//
BYTE IsPointTime()
{
	return 1;
}

//
// Function Name: NewWarData
// Date: 
// Description: 
//
sPWAR_DATA NewWarData()
{
	sPWAR_DATA pData = NULL;
	
	if( gGUILDS.war_memory )
	{
		pData = gGUILDS.war_memory;
		
		REMOVE_FROM_LIST( gGUILDS.war_memory, pData, prev, next );
	}
	else
	{
		pData = (sPWAR_DATA)malloc(sizeof(sWAR_DATA));
		
		if( !pData )
			return NULL;
	}
	
	pData->ownerGuild = 0;
	pData->ownerPoint = 0;
	
	pData->enemyGuild = 0;
	
	pData->targetMapNum = 0;
	pData->type = dWARDATA_TYPE_NONE;
	
	pData->lastUpdateTime = g_CurrRealTime;
	
	pData->prev = NULL;
	pData->next = NULL;
	
	return pData;
}

//
// Function Name: WAR_DestroyWarData
// Date: 
// Description: 
//
void WAR_DestroyWarData()
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	REMOVE_FROM_LIST( gGUILDS.war_list, data, prev, next );
	SAFE_FREE( data );
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	LIST_WHILE( gGUILDS.war_memory, data, next_data, next, WMgr2 );
	REMOVE_FROM_LIST( gGUILDS.war_memory, data, prev, next );
	SAFE_FREE( data );
	LIST_WHILEEND( gGUILDS.war_memory, data, next_data );
}

//
// Function Name: WAR_FindData
// Date: 
// Description: 
//
sPWAR_DATA WAR_FindData2( int ownerGuild, int enemyGuild, BYTE type )
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->ownerGuild == ownerGuild &&
		data->enemyGuild == enemyGuild &&
		data->type == type )
		return data;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return NULL;
}

//
// Function Name: WAR_FindData
// Date: 
// Description: 
//
sPWAR_DATA WAR_FindDataByAll( int ownerGuild, int enemyGuild, int targetMapNum, BYTE type )
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->ownerGuild == ownerGuild &&
		data->enemyGuild == enemyGuild &&
		data->targetMapNum == targetMapNum &&
		data->type == type )
		return data;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return NULL;
}

//
// Function Name: WAR_FindOwnerData
// Date: 
// Description: 
//
sPWAR_DATA WAR_FindOwnerData( int ownerGuild )
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->ownerGuild == ownerGuild )
		return data;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return NULL;
}

//
// Function Name: WAR_FindCastleData
// Date: 
// Description: 
//
sPWAR_DATA WAR_FindDataByType( int ownerGuild, BYTE type )
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->ownerGuild == ownerGuild && data->type == type )
		return data;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return NULL;
}

//
// Function Name: WAR_FindDataByMap
// Date: 
// Description: 
//
sPWAR_DATA WAR_FindDataByMap( int ownerGuild, int enemyGuild, int mapNum )
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->ownerGuild == ownerGuild &&
		data->enemyGuild == enemyGuild &&
		data->targetMapNum == mapNum )
		return data;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return NULL;
}

//
// Function Name: WAR_FindEnemyData
// Date: 
// Description: 
//
sPWAR_DATA WAR_FindEnemyData( int enemyGuild )
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->enemyGuild == enemyGuild )
		return data;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return NULL;
}

//
// Function Name: WAR_FindData
// Date: 
// Description: 
//
sPWAR_DATA WAR_FindData( int ownerGuild, int enemyGuild )
{
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->ownerGuild == ownerGuild &&
		data->enemyGuild == enemyGuild )
		return data;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return NULL;
}


//
// Function Name: WAR_MakeWarDataSet
// Date: 
// Description: 
//
BOOL WAR_MakeWarDataSet( sPWORLD pWorld , sPWARDATA_SET pDataSet )
{
	if( !pDataSet )
		return 0;
	
	pDataSet->camp[0] = NULL;
	pDataSet->camp[1] = NULL;
	
	for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
		pDataSet->linkedCastle[i] = NULL;
	
	sPWAR_DATA data, next_data;
	
	BYTE campArrayNum = 0;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->type == dWARDATA_TYPE_CAMP && data->targetMapNum == pWorld->mapNum )
	{
		pDataSet->camp[campArrayNum] = data;
		campArrayNum++;
		
		if( campArrayNum >= 2 )
			break;
	}
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	BYTE castleArrayNum = 0;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr2 );
	
	if( data->type == dWARDATA_TYPE_CASTLE && data->targetMapNum == pWorld->mapNum )
	{		
		pDataSet->linkedCastle[castleArrayNum] = data;
		
		castleArrayNum++;
		
		if( castleArrayNum >= dMAX_LINKED_CASTLE )
			break;
	}
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return 1;
}


sPFIND_WAR_ENEMY WAR_FindWarEnemy( int guildNum )
{
	static sFIND_WAR_ENEMY value;
	
	for( int i = 0; i < 6; i++ )
		value.guildNum[i] = 0;
	
	sPWAR_DATA data, next_data;
	
	int arrayNum = 0;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( arrayNum >= 6 )
		break;
	
	if( data->enemyGuild == guildNum )
	{
		value.guildNum[arrayNum] = data->ownerGuild;
		arrayNum++;
	}
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return &value;
}

void WAR_SendEnemyInfo( sPDESC_DATA pPlayer )
{
	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return;
	
	sPFIND_WAR_ENEMY pEnemy = WAR_FindWarEnemy( GET_GUILD( pPlayer ) );
	
	if( pEnemy )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_ENEMY_DIVISION, g_nPos );
		for( int i = 0; i < 6; i++ )
		{
			PutInteger( g_Packet, pEnemy->guildNum[i], g_nPos );
		}
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
	}
}

//
// Function Name: WAR_GetCampCount
// Date: 
// Description: 
//
int WAR_GetCampCount( int targetMap )
{
	sPWAR_DATA data, next_data;
	
	int cnt = 0;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->type == dWARDATA_TYPE_CAMP && data->targetMapNum == targetMap )
		cnt++;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
	
	return cnt;
}

//
// Function Name: WAR_MakeCamp
// Date: 
// Description: 
//
void WAR_MakeCamp( sPDESC_DATA pPlayer )
{
	int i;
	
	if( gWorldUnderAttack )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[197] );
		return;
	}
	
	int mapNum = GET_MAP_NUM( pPlayer );
	sPWORLD pMap = gWORLD[mapNum];
	
	if( !pMap )
		return;
	
	sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );
	
	if( !pGuild )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[136] );
		return;
	}
	
	// 플레이어 권한 체크 
	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[136] );
		return;
	}
	
	// 길드 종류 체크
	if( pGuild->guildType != dGUILD_TYPE_MERCENARY )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[137] );
		return;
	}
	
	if( GET_ALL_SKILL( pPlayer ) < 50 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[272] );
		return;
	}
	
	if( GUILD_MemberCount( pGuild ) < 7 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[273] );
		return;
	}
	
	if( !pMap->campTarget )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[138] );
		return;
	}
	else if( !gWORLD[pMap->campTarget]->hasCastle )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[138] );
		return;
	}
	
	if( WAR_FindOwnCastle( GET_GUILD( pPlayer ) ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[193] );
		return;
	}
	
	
	if( WAR_FindDataByType( GET_GUILD( pPlayer ), dWARDATA_TYPE_CAMP ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[140] );
		return;
	}
	
	if( WAR_GetCampCount( pMap->campTarget ) >= 2 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[184] );
		return;
	}
	// 주인없는 성이면 
	if( !gWORLD[pMap->campTarget]->ownerGuild )
	{
		if( GET_MONEY( pPlayer ) < 5000000 )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[157], 5000000 );
			return;
		}
		
		GET_MONEY( pPlayer ) -= 5000000;
		UpdateMoney( pPlayer );
		DATASERV_SendUpdateMoney( pPlayer );
		
		sPWORLD pWorld = gWORLD[pMap->campTarget];
		pWorld->ownerGuild = GET_GUILD( pPlayer );
		
		char query[128];
		
		sprintf( query, "delete from MAPOwnerGuild where mapNum = %d", pWorld->mapNum );
		
		if( mysql_query( g_MYSQL, query ) )
			log( "%s",mysql_error( g_MYSQL ) );
		
		sprintf( query, "insert into MAPOwnerGuild values( '', %d, %d, 5, 0 )", pWorld->mapNum, pWorld->ownerGuild );
		
		if( mysql_query( g_MYSQL, query ) )
			log( "%s",mysql_error( g_MYSQL ) );
		
		sprintf( query, "delete from WarData where type = %d", dWARDATA_TYPE_CASTLE );
		
		if( mysql_query( g_MYSQL, query ) )
			log( "%s",mysql_error( g_MYSQL ) );
		
		sWAR_DATA warDataMemory[128];
		
		for( i = 0; i < 128; i++ )
		{
			warDataMemory[i].ownerGuild = 0;
			warDataMemory[i].ownerPoint = 0;
			
			warDataMemory[i].enemyGuild = 0;
			
			warDataMemory[i].targetMapNum = 0;
			
			warDataMemory[i].type = 0;
			
			warDataMemory[i].lastUpdateTime = 0;
			
			warDataMemory[i].prev = NULL;
			warDataMemory[i].next = NULL;
		}
		
		int currWarDataMemoryCnt = 0;
		
		// 공성 정보 Clear 
		sPWAR_DATA warData, nextWarData;
		LIST_WHILE( gGUILDS.war_list, warData, nextWarData, next, WMgr );
		if( warData->type == dWARDATA_TYPE_CASTLE )
		{
			if( warData->ownerPoint )
			{
				warDataMemory[currWarDataMemoryCnt].ownerGuild = warData->ownerGuild;
				warDataMemory[currWarDataMemoryCnt].ownerPoint = warData->ownerPoint;
				warDataMemory[currWarDataMemoryCnt].enemyGuild = warData->enemyGuild;
				warDataMemory[currWarDataMemoryCnt].targetMapNum = warData->targetMapNum;
				warDataMemory[currWarDataMemoryCnt].type = warData->type;
				
				currWarDataMemoryCnt++;
			}
			
			REMOVE_FROM_LIST( gGUILDS.war_list, warData, prev, next );
			INSERT_TO_LIST( gGUILDS.war_memory, warData, prev, next );
		}
		LIST_WHILEEND( gGUILDS.war_list, warData, nextWarData );
		
		//
		sPLIST_BLOCK list, block, next_block;
		sPWORLD pListWorld;
		
		list = gWORLDLIST.GetFirstBlock();
		
		LIST_WHILE( list, block, next_block, next, WMgr2 );
		
		pListWorld = (sPWORLD)block->data;
		
		if( pListWorld->ownerGuild )
		{
			for( i = 0; i < dMAX_LINKED_CASTLE; i++ )
			{
				if( IS_VALID_WORLD( pListWorld->linkedCastle[i] ) )
				{
					if( gWORLD[pListWorld->linkedCastle[i]]->ownerGuild )
					{
						if( gWORLD[pListWorld->linkedCastle[i]]->ownerGuild == pListWorld->ownerGuild )
						{
							continue;
						}
						
						sPWAR_DATA pWarData = NewWarData();
						
						if( pWarData )
						{
							pWarData->ownerGuild = pListWorld->ownerGuild;
							pWarData->enemyGuild = gWORLD[pListWorld->linkedCastle[i]]->ownerGuild;
							pWarData->type = dWARDATA_TYPE_CASTLE;
							pWarData->targetMapNum = pListWorld->linkedCastle[i];
							
							INSERT_TO_LIST( gGUILDS.war_list, pWarData, prev, next );
							
							WAR_WriteWarData( pWarData );
						}
						else
						{
							log( "WAR_MakeCamp : !pWarData\r\n" );
							continue;
						}
					}
				}
			}
		}
		LIST_WHILEEND( list, block, next_block );
		
		for( i = 0; i < 128; i++ )
		{
			if( warDataMemory[i].ownerGuild == 0 &&
				warDataMemory[i].enemyGuild == 0 )
				continue;
			
			sPWAR_DATA restoreWarData = WAR_FindDataByAll( warDataMemory[i].ownerGuild, warDataMemory[i].enemyGuild, warDataMemory[i].targetMapNum, warDataMemory[i].type );
			
			if( restoreWarData )
			{
				restoreWarData->ownerPoint = warDataMemory[i].ownerPoint;
				WAR_UpdateWarData( restoreWarData );
			}
		}
		
		// %s 성을 구입하였습니다.
		SendSystemMsg( pPlayer, g_LANG_STR[194], pWorld->mapName );
		return;
	}
	
	
	if( !IsCanMakeCampDate() )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[141] );
		return;
	}
	
	if( GET_MONEY( pPlayer ) < 5000000 )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[157], 5000000);  //no hall = 30mil
		return;
	}
	
	GET_MONEY( pPlayer ) -= 5000000;
	DATASERV_SendUpdateMoney( pPlayer );
	UpdateMoney( pPlayer );
	
	sPWAR_DATA pNewWarData = NewWarData();
	
	sPWAR_DATA pNewWarData2 = NewWarData();
	
	if( !pNewWarData || !pNewWarData2 )
	{
		log( "WAR_MakeCamp : malloc failed!\r\n" );
		return;
	}
	
	
	pNewWarData->enemyGuild = gWORLD[pMap->campTarget]->ownerGuild;
	pNewWarData->ownerGuild = GET_GUILD( pPlayer );
	pNewWarData->ownerPoint = 0;
	
	pNewWarData->targetMapNum = pMap->campTarget;
	
	pNewWarData->type = dWARDATA_TYPE_CAMP;
	
	pNewWarData->prev = NULL;
	pNewWarData->next = NULL;
	
	INSERT_TO_LIST( gGUILDS.war_list, pNewWarData, prev, next );
	
	pNewWarData2->enemyGuild = GET_GUILD( pPlayer );
	pNewWarData2->ownerGuild = gWORLD[pMap->campTarget]->ownerGuild;
	pNewWarData2->ownerPoint = 0;
	
	pNewWarData2->targetMapNum = pMap->campTarget;
	pNewWarData2->type = dWARDATA_TYPE_CAMP;
	
	pNewWarData2->prev = NULL;
	pNewWarData2->next = NULL;
	
	INSERT_TO_LIST( gGUILDS.war_list, pNewWarData2, prev, next );
	
	SendSystemMsg( pPlayer, g_LANG_STR[185] );
	
	WAR_SendWarCampInfo( pPlayer, 1 );
	WAR_WriteWarData( pNewWarData );
	WAR_WriteWarData( pNewWarData2 );
	
}



//
// Function Name: WAR_ClearWorldMob
// Date: 
// Description: 
//
void WAR_ClearWorldMob( int mapNum )
{
	sPWORLD pMap = gWORLD[mapNum];
	
	if( !pMap )
	{
		log( "WAR_ClearWorldMob : !IS_VALID_WORLD( mapNum )\r\n" );
		return;
	}
	
	sPMOB_DATA mob, next_mob;
	sPDESC_DATA player, next_player;
	int x, y;
	
	// 몹 삭제
	for( y = 0; y < dONE_TILE_SIZE; y++ )
	{
		for( x = 0; x < dONE_TILE_SIZE; x++ )
		{
			LIST_WHILE( pMap->player[y][x], player, next_player, w_next, WMgr );
			OutCurrAreaMob( pMap->player[y][x] );
			LIST_WHILEEND( pMap->player[y][x], player, next_player );
		}
	}
	
	for( y = 0; y < dONE_TILE_SIZE; y++ )
	{
		for( x = 0; x < dONE_TILE_SIZE; x++ )
		{
			LIST_WHILE( pMap->mob[y][x], mob, next_mob, w_next, WMgr );
			DelMobData( pMap->mob[y][x] , __FILE__, __LINE__ );
			LIST_WHILEEND( pMap->mob[y][x], mob, next_mob );
		}
	}
}

//
// Function Name: LoadMapOwnerGuild
// Date: 
// Description: 
//
void LoadMapOwnerGuild()
{
	// 성 소유정보 처리
	if( mysql_query( g_MYSQL, "select * from MAPOwnerGuild order by mapNum" ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		MYSQL_ROW row;
		MYSQL_RES *result;
		
		int mapNum;
		int guildNum;
		
		result = mysql_store_result( g_MYSQL );
		
		if( result )
		{			
			while( ( row = mysql_fetch_row( result ) ) )
			{
				mapNum = atoi( row[MAPOWNERDATA_MAPNUM] );
				guildNum = atoi( row[MAPOWNERDATA_GUILDNUM] );
				
				if( !IS_VALID_GUILD( guildNum ) )
				{
					log( "LoadMapOwnerGuild : Invalid Guild Number map - %d , guild - %d\r\n", mapNum, guildNum );
					continue;
				}
				
				if( !IS_VALID_WORLD( mapNum ) )
				{
					log( "LoadMapOwnerGuild : Invalid Map Number map - %d, guild - %d\r\n", mapNum, guildNum );
					continue;
				}
				
				gWORLD[mapNum]->ownerGuild = guildNum;
				gWORLD[mapNum]->itemTax = atoi( row[MAPOWNERDATA_TAXVALUE] );
				gWORLD[mapNum]->TaxBank = atol( row[MAPOWNERDATA_TAXBANK] );
			}	
			
			mysql_free_result( result );
		}
	}
}

//
// Function Name: BuildWarDataList
// Date: 
// Description: 
//
void BuildWarDataList()
{
	// 전쟁정보 다 지우기
	if( mysql_query( g_MYSQL, "delete from WarData" ) )
		log( "%s",mysql_error( g_MYSQL ) );
	
	//
	sPLIST_BLOCK list, block, next_block;
	sPWORLD pWorld;
	
	list = gWORLDLIST.GetFirstBlock();
	
	LIST_WHILE( list, block, next_block, next, WMgr );
	
	pWorld = (sPWORLD)block->data;
	
	if( pWorld->ownerGuild )
	{
		for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
		{
			if( IS_VALID_WORLD( pWorld->linkedCastle[i] ) )
			{
				if( gWORLD[pWorld->linkedCastle[i]]->ownerGuild )
				{
					if( gWORLD[pWorld->linkedCastle[i]]->ownerGuild == pWorld->ownerGuild )
					{
						continue;
					}
					
					sPWAR_DATA pWarData = NewWarData();
					
					if( pWarData )
					{
						pWarData->ownerGuild = pWorld->ownerGuild;
						pWarData->enemyGuild = gWORLD[pWorld->linkedCastle[i]]->ownerGuild;
						pWarData->type = dWARDATA_TYPE_CASTLE;
						pWarData->targetMapNum = pWorld->linkedCastle[i];
						
						INSERT_TO_LIST( gGUILDS.war_list, pWarData, prev, next );
						
						WAR_WriteWarData( pWarData );
					}
					else
					{
						log( "BuildWarDataList : !pWarData\r\n" );
						continue;
					}
				}
			}
		}
	}
	
	LIST_WHILEEND( list, block, next_block );
}


//
// Function Name: WAR_CheckWarTime
// Date: 
// Description: 
//
void WAR_CheckWarTime( BYTE noTimeCheck )
{
	if( !gWorldUnderAttack && IsBeginWarTime( noTimeCheck ) )
	{
		gWorldUnderAttack = 1;
		
		sPLIST_BLOCK list, block, next_block;
		sPWORLD pWorld;
		
		list = gWORLDLIST.GetFirstBlock();
		
		LIST_WHILE( list, block, next_block, next , WMgr );
		
		pWorld = (sPWORLD)block->data;
		
		if( pWorld->hasCastle )
		{	
			if( !pWorld->ownerGuild )
			{
				LIST_SKIP( block, next_block );
			}
			
			sWARDATA_SET warDataSet;
			
			WAR_MakeWarDataSet( pWorld, &warDataSet );
			
			sPWAR_DATA pCampAttacker = NULL;
			
			if( warDataSet.camp[0] && !warDataSet.camp[1] )
			{
				pCampAttacker = warDataSet.camp[0];
			}
			else if( !warDataSet.camp[0] && !warDataSet.camp[1] )
			{
				pCampAttacker = warDataSet.camp[1];
			}
			else if( warDataSet.camp[0] && warDataSet.camp[1] )
			{
				if( warDataSet.camp[0]->ownerPoint > warDataSet.camp[1]->ownerPoint )
				{
					pCampAttacker = warDataSet.camp[0];
				}
				else
				{
					pCampAttacker = warDataSet.camp[1];
				}
			}
			else 
				pCampAttacker = NULL;
			
			int castleArray = 0;
			int castleTopPoint = 0;
			sPWAR_DATA pCastleAttacker = NULL;
			
			for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
			{
				if( warDataSet.linkedCastle[i] )
				{
					if( warDataSet.linkedCastle[i]->ownerPoint > castleTopPoint )
						castleArray = i;
				}
			}
			
			if( warDataSet.linkedCastle[castleArray] )
			{
				pCastleAttacker = warDataSet.linkedCastle[castleArray];
			}
			else
			{
				pCastleAttacker = NULL;
			}
			
			if( !pCampAttacker && !pCastleAttacker )
			{
				LIST_SKIP( block, next_block );
			}
			
			// 인접성 
			if( !pCampAttacker && pCastleAttacker )
			{
				int enemyOwnMap = 0;
				
				for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
				{
					if( IS_VALID_WORLD( pWorld->linkedCastle[i] ) )
					{
						if( gWORLD[pWorld->linkedCastle[i]]->ownerGuild == pCastleAttacker->ownerGuild )
						{
							enemyOwnMap = pWorld->linkedCastle[i];
							break;
						}
					}
				}
				
				sPWAR_DATA pOwnWarData = WAR_FindDataByMap( pWorld->ownerGuild, pCastleAttacker->ownerGuild, enemyOwnMap );
				
				// 정보가 없으면 
				if( !pOwnWarData )
				{
					LIST_SKIP( block, next_block );
				}
				
				// 다른 정보면 
				if( pOwnWarData->enemyGuild != pCastleAttacker->ownerGuild )
				{
					LIST_SKIP( block, next_block );
				}
				
				if( pOwnWarData->ownerPoint >= pCastleAttacker->ownerPoint )
				{
					LIST_SKIP( block, next_block );
				}
				
				pWorld->warCristal.allowGuild[0] = pWorld->ownerGuild;
				pWorld->warCristal.allowGuild[1] = pCastleAttacker->ownerGuild;
			}
			// 진지만 
			else if( pCampAttacker && !pCastleAttacker )
			{
				sPWAR_DATA tmpWarData = WAR_FindData( pWorld->ownerGuild, pCampAttacker->ownerGuild );
				
				if( tmpWarData )
				{
					if( tmpWarData->ownerPoint < pCampAttacker->ownerPoint )
					{
						pWorld->warCristal.allowGuild[0] = pWorld->ownerGuild;
						pWorld->warCristal.allowGuild[1] = pCampAttacker->ownerGuild;
					}
					else
					{
						LIST_SKIP( block, next_block );
					}
				}
				else
				{
					LIST_SKIP( block, next_block );
				}
			}
			// 진지, 인접성 둘다 존재시 
			else if( pCampAttacker && pCastleAttacker )
			{
				// 진지의 포인트가 더 높다 
				if( pCampAttacker->ownerPoint > pCastleAttacker->ownerPoint )
				{
					sPWAR_DATA tmpWarData = WAR_FindData( pWorld->ownerGuild, pCampAttacker->ownerGuild );
					
					if( tmpWarData )
					{
						if( tmpWarData->ownerPoint < pCampAttacker->ownerPoint )
						{
							pWorld->warCristal.allowGuild[0] = pWorld->ownerGuild;
							pWorld->warCristal.allowGuild[1] = pCampAttacker->ownerGuild;
						}
						else
						{
							LIST_SKIP( block, next_block );
						}
					}
					else
					{
						LIST_SKIP( block, next_block );
					}
				}
				// 인접성의 포인트가 더 높다 
				else
				{
					int enemyOwnMap = 0;
					
					// 인접성 길드번호
					for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
					{
						if( IS_VALID_WORLD( pWorld->linkedCastle[i] ) )
						{
							if( gWORLD[pWorld->linkedCastle[i]]->ownerGuild == pCastleAttacker->ownerGuild )
							{
								enemyOwnMap = pWorld->linkedCastle[i];
								break;
							}
						}
					}
					
					sPWAR_DATA pOwnWarData = WAR_FindDataByMap( pWorld->ownerGuild, pCastleAttacker->ownerGuild, enemyOwnMap );
					
					// 정보가 없으면 
					if( !pOwnWarData )
					{
						LIST_SKIP( block, next_block );
					}
					
					// 다른 정보면 
					if( pOwnWarData->enemyGuild != pCastleAttacker->ownerGuild )
					{
						LIST_SKIP( block, next_block );
					}
					
					if( pOwnWarData->ownerPoint >= pCastleAttacker->ownerPoint )
					{
						LIST_SKIP( block, next_block );
					}
					
					pWorld->warCristal.allowGuild[0] = pWorld->ownerGuild;
					pWorld->warCristal.allowGuild[1] = pCastleAttacker->ownerGuild;
				}
			}
			else
			{
				LIST_SKIP( block, next_block );
			}
			
			pWorld->warPoint = 50;
			
			char noticeMsg[1024];
			string test;
			test = pWorld->mapName;
			test.append( " castle attack: " );
			test.append( GET_GUILD_DATA( pWorld->warCristal.allowGuild[1] )->guildName );
			test.append( " defense : " );
			test.append( GET_GUILD_DATA( pWorld->warCristal.allowGuild[0] )->guildName );
			/*sprintf( noticeMsg, g_LANG_STR[188], pWorld->mapName, 
				GET_GUILD_DATA( pWorld->warCristal.allowGuild[1] )->guildName,
				GET_GUILD_DATA( pWorld->warCristal.allowGuild[0] )->guildName );*/

			strcpy( noticeMsg, test.c_str() );
			
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_GUILD_CHAT, g_nPos );
			PutString( g_Packet, g_LANG_STR[186], g_nPos );
			PutString( g_Packet, noticeMsg, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendToMap( pWorld->mapNum, g_Packet, g_nPos );
			
			pWorld->underAttack = 1;
			
			// 성문, 성물 등장
			WAR_ClearWorldMob( pWorld->mapNum );
			WAR_PopMapObject( pWorld->mapNum );
			
			WAR_SummonWarPosition( pWorld , pWorld->warCristal.allowGuild[0] );
		}
		
		LIST_WHILEEND( list, block, next_block );
	}
	// 전쟁 종료 
	else if( gWorldUnderAttack && IsFinishWarTime( noTimeCheck ) )
	{	
		sPLIST_BLOCK list, block, next_block;
		sPWORLD pWorld;
		
		list = gWORLDLIST.GetFirstBlock();
		
		LIST_WHILE( list, block, next_block, next, WMgr );
		
		pWorld = (sPWORLD)block->data;
		
		// 공성중이었던 경우
		if( pWorld->underAttack )
		{	
			pWorld->underAttack = 0;
			pWorld->warCristal.allowGuild[0] = 0;
			pWorld->warCristal.allowGuild[1] = 0;
			pWorld->warPoint = 0;
			
			char noticeMsg[1024];
//			sprintf( noticeMsg, g_LANG_STR[189], pWorld->mapName, GET_GUILD_DATA( pWorld->ownerGuild )->guildName );
			string test;
			test = pWorld->mapName;
			test.append( " castle war end : " );
			test.append( GET_GUILD_DATA( pWorld->ownerGuild )->guildName );
			test.append( " win!!" );

			strcpy( noticeMsg, test.c_str() );
			
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_GUILD_CHAT, g_nPos );
			PutString( g_Packet, g_LANG_STR[186], g_nPos );
			PutString( g_Packet, noticeMsg, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendToMap( pWorld->mapNum, g_Packet, g_nPos );
			
			WAR_ClearWorldMob( pWorld->mapNum );
			
			WAR_SummonWarPosition( pWorld, pWorld->ownerGuild );
			
			SPoilsCastleTaxBank( pWorld->ownerGuild, pWorld->TaxBank );
			pWorld->TaxBank = 0;
			UpdateCastleTaxBank( pWorld->mapNum, 0 );
		}
		
		LIST_WHILEEND( list, block, next_block );	
		
		// 공성 정보 Clear 
		sPWAR_DATA warData, nextWarData;
		LIST_WHILE( gGUILDS.war_list, warData, nextWarData, next, WMgr2 );
		REMOVE_FROM_LIST( gGUILDS.war_list, warData, prev, next );
		INSERT_TO_LIST( gGUILDS.war_memory, warData, prev, next );
		LIST_WHILEEND( gGUILDS.war_list, warData, nextWarData );
		
		BuildWarDataList();
		
		// 전쟁 종료
		gWorldUnderAttack = 0;
	}
}


//
// Function Name: LoadWarCristalInfo
// Date: 
// Description: 
//
BOOL LoadWarCristalInfo()
{
	FILE *fp = fopen( dWARCRISTAL_INFO_FILE, "rb" );
	
	if( !fp )
	{
		log( "LoadWarCristalInfo : File Not Found : %s\r\n", dWARCRISTAL_INFO_FILE );
		return 0;
	}
	
	int mapNum;
	int posX;
	int posY;
	
	char readBuff[1024];
	
	while( 1 )
	{
		if( feof( fp ) )
			break;
		
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		if( sscanf( readBuff, "%d %d %d", &mapNum, &posX, &posY ) != 3 )
		{
			log( "LoadWarCristalInfo : Bad file format!\r\n" );
			fclose( fp );
			return 0;
		}
		
		if( !IS_VALID_WORLD( mapNum ) )
		{
			log( "LoadWarCristalInfo : Bad Map Num %d\r\n", mapNum );
			continue;
		}
		
		gWORLD[mapNum]->warCristal.posX = posX;
		gWORLD[mapNum]->warCristal.posY = posY;
	}
	
	fclose( fp );
	return 1;
}

//
// Function Name: LoadCastleDoorInfo
// Date: 
// Description: 
//
BOOL LoadCastleDoorInfo()
{
	FILE *fp = fopen( dCASTLE_DOOR_INFO_FILE, "rb" );
	
	if( !fp )
	{
		log( "LoadCastleDoorInfo : File Not Found %s\r\n", dCASTLE_DOOR_INFO_FILE );
		return 0;
	}
	
	int mapNum;
	int doorNum;
	int posX;
	int posY;
	int angle;
	
	char readBuff[1024];
	
	while( 1 )
	{
		if( feof( fp ) )
			break;
		
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		if( sscanf( readBuff, "%d %d %d %d %d", &mapNum, &doorNum, &posX, &posY, &angle ) != 5 )
		{
			log( "LoadCastleDoorInfo : Bad File Format\r\n" );
			fclose( fp );
			return 0;
		}
		
		if( !IS_VALID_WORLD( mapNum ) )
		{
			log( "LoadCastleDoorInfo : Bad Map Num %d\r\n", mapNum );
			continue;
		}
		
		if( doorNum >= dMAX_CASTLE_DOOR )
		{
			log( "LoadCastleDoorInfo : Bad Door Number %d\r\n", doorNum );
			continue;
		}
		
		gWORLD[mapNum]->castleDoor[doorNum].posX = posX;
		gWORLD[mapNum]->castleDoor[doorNum].posY = posY;
		gWORLD[mapNum]->castleDoor[doorNum].angle = angle;
	}
	
	fclose( fp );
	return 1;
}



//
// Function Name: WAR_PopMapObject
// Date: 
// Description: 
//
void WAR_PopMapObject( int mapNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return;
	
	if( !gWORLD[mapNum]->hasCastle )
		return;
	
	// 성문 생성 
	for( int i = 0; i < dMAX_CASTLE_DOOR; i++ )
	{
		CreateObjectMob( dMONSTER_CASTLEDOOR, mapNum, 
			gWORLD[mapNum]->castleDoor[i].posX,
			gWORLD[mapNum]->castleDoor[i].posY,
			gWORLD[mapNum]->castleDoor[i].angle );
	}
	
	// 성물 생성
	CreateObjectMob( dMONSTER_WARCRISTAL, mapNum,
		gWORLD[mapNum]->warCristal.posX,
		gWORLD[mapNum]->warCristal.posY,
		0 );
}


//
// Function Name: WAR_PopWarCristal
// Date: 
// Description: 
//
void WAR_PopWarCristal( int mapNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return;
	
	if( !gWORLD[mapNum]->hasCastle )
		return;
	
	// 성문 생성 
	CreateObjectMob( dMONSTER_WARCRISTAL, mapNum, gWORLD[mapNum]->warCristal.posX, gWORLD[mapNum]->warCristal.posY, 0 );
}


//
// Function Name: WAR_FindCampData
// Date: 
// Description: 
//
void WAR_FindCampData( sPWAR_DATA camp[2], int mapNum )
{
	camp[0] = NULL;
	camp[1] = NULL;
	
	int arrayNum = 0;
	
	sPWAR_DATA data, next_data;
	
	LIST_WHILE( gGUILDS.war_list, data, next_data, next, WMgr );
	
	if( data->type == dWARDATA_TYPE_CAMP && data->targetMapNum == mapNum )
		camp[arrayNum] = data;
	
	
	arrayNum++;
	
	if( arrayNum >= 2 )
		break;
	
	LIST_WHILEEND( gGUILDS.war_list, data, next_data );
}


//
// Function Name: WAR_SendWarCampInfo
// Date: 
// Description: 
//
void WAR_SendWarCampInfo( sPDESC_DATA pPlayer, BYTE toWorld )
{
	if( !IS_VALID_WORLD( gWORLD[GET_MAP_NUM( pPlayer )]->campTarget ) )
		return;
	
	sPWAR_DATA pCamp[2];
	
	WAR_FindCampData( pCamp, gWORLD[GET_MAP_NUM( pPlayer )]->campTarget );
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_WARCAMP, g_nPos );
	
	if( pCamp[0] )
	{
		PutInteger( g_Packet, pCamp[0]->ownerGuild, g_nPos );
		PutString( g_Packet, GET_GUILD_DATA( pCamp[0]->ownerGuild )->guildName, g_nPos );
	}
	else
	{
		PutInteger( g_Packet, 0, g_nPos );
		PutString( g_Packet, " ", g_nPos );
	}
	
	if( pCamp[1] )
	{
		PutInteger( g_Packet, pCamp[1]->ownerGuild, g_nPos );
		PutString( g_Packet, GET_GUILD_DATA( pCamp[1]->ownerGuild )->guildName, g_nPos );
	}
	else
	{
		PutInteger( g_Packet, 0, g_nPos );
		PutString( g_Packet, " ", g_nPos );
	}
	
	PutSize( g_Packet, g_nPos );
	
	if( !toWorld )
		SendData( pPlayer, g_Packet, g_nPos );
	else
		SendToMap( GET_MAP_NUM( pPlayer ), g_Packet, g_nPos );
}


//
// Function Name: WAR_SendWarInfo
// Date: 
// Description: 
//
void WAR_SendWarInfo( sPDESC_DATA pPlayer, int mapNum , BYTE toWorld )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return;
	
	if( !gWORLD[mapNum]->underAttack )
		return;
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_WARINFO, g_nPos );
	
	// 성의 주인
	if( !gWORLD[mapNum]->ownerGuild )
	{
		PutInteger( g_Packet, 0, g_nPos );
		PutString( g_Packet, "NPC", g_nPos );
	}
	else
	{
		PutInteger( g_Packet, gWORLD[mapNum]->ownerGuild, g_nPos );
		PutString( g_Packet, GET_GUILD_DATA( gWORLD[mapNum]->ownerGuild )->guildName, g_nPos );
	}
	
	// 공성중인 두 길드의 정보 
	if( IS_VALID_GUILD( gWORLD[mapNum]->warCristal.allowGuild[0] ) )
	{
		PutInteger( g_Packet, gWORLD[mapNum]->warCristal.allowGuild[0], g_nPos );
		PutString( g_Packet, GET_GUILD_DATA( gWORLD[mapNum]->warCristal.allowGuild[0] )->guildName, g_nPos );
	}
	else
	{
		PutInteger( g_Packet, 0, g_nPos );
		PutString( g_Packet, " ", g_nPos );
	}
	
	if( IS_VALID_GUILD( gWORLD[mapNum]->warCristal.allowGuild[1] ) )
	{
		PutInteger( g_Packet, gWORLD[mapNum]->warCristal.allowGuild[1], g_nPos );
		PutString( g_Packet, GET_GUILD_DATA( gWORLD[mapNum]->warCristal.allowGuild[1] )->guildName, g_nPos );
	}
	else
	{
		PutInteger( g_Packet, 0, g_nPos );
		PutString( g_Packet, " ", g_nPos );
	}
	PutSize( g_Packet, g_nPos );
	
	if( !toWorld )
		SendData( pPlayer, g_Packet, g_nPos );
	else
		SendToMap( mapNum, g_Packet, g_nPos );
}


//
// Function Name: LoadWarData
// Date: 
// Description: 
//
BOOL LoadWarData()
{
	char query[dMAX_QUERY_SIZE];
	
	sprintf( query, "select * from WarData order by targetMapNum asc" );
	
	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
		return 0;
	}
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	result = mysql_store_result( g_MYSQL );
	
	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			sPWAR_DATA pNewWarData = NewWarData();
			
			if( !pNewWarData )
			{
				log( "LoadWarData : Malloc Failed\r\n" );
				mysql_free_result( result );
				return 0;
			}
			
			pNewWarData->ownerGuild = atoi( row[WARDATA_OWNERGUILD] );
			pNewWarData->ownerPoint = atoi( row[WARDATA_OWNERPOINT] );
			
			pNewWarData->enemyGuild = atoi( row[WARDATA_ENEMYGUILD] );
			
			pNewWarData->targetMapNum = atoi( row[WARDATA_TARGETMAPNUM] );
			
			pNewWarData->type = atoi( row[WARDATA_TYPE] );
			
			INSERT_TO_LIST( gGUILDS.war_list, pNewWarData, prev, next );
		}
		mysql_free_result( result );
	}
	return 1;
}



//
// Function Name: WAR_WriteWarData
// Date: 
// Description: 
//
void WAR_WriteWarData( sPWAR_DATA pWarData )
{
	if( !pWarData )
		return;
	
	char query[dMAX_QUERY_SIZE];
	
	sprintf( query, "insert into WarData values( '', %d, %d, %d, %d, %d )", 
		pWarData->ownerGuild, 
		pWarData->ownerPoint,
		pWarData->enemyGuild,
		pWarData->targetMapNum,
		pWarData->type );
	
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}


//
// Function Name: WAR_UpdateWarData
// Date: 
// Description: 
//
void WAR_UpdateWarData( sPWAR_DATA pWarData )
{
	if( !pWarData )
		return;
	
	pWarData->lastUpdateTime = g_CurrRealTime;
	
	char query[dMAX_QUERY_SIZE];
	
	sprintf( query, "update WarData set ownerPoint = %d where ownerGuild = %d and enemyGuild = %d and targetMapNum = %d and type = %d limit 1",
		pWarData->ownerPoint,
		pWarData->ownerGuild,
		pWarData->enemyGuild,
		pWarData->targetMapNum,
		pWarData->type );
	
	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );	
}

//
// Function Name: WAR_FindOwnCastle
// Date: 
// Description: 
//
BOOL WAR_FindOwnCastle( int guildNum )
{
	sPLIST_BLOCK list, block, next_block;
	sPWORLD pWorld;
	
	list = gWORLDLIST.GetFirstBlock();
	
	LIST_WHILE( list, block, next_block, next, WMgr );
	
	pWorld = (sPWORLD)block->data;
	
	if( pWorld->ownerGuild == guildNum )
		return 1;
	
	LIST_WHILEEND( list, block, next_block );
	return 0;
}

//
// Function Name: WAR_SummonWarPosition
// Date: 
// Description: 
//
void WAR_SummonWarPosition( sPWORLD pWorld , int ownerGuild )
{
	if( !pWorld->hasCastle )
		return;
	
	sPDESC_DATA d, next_d;
	
	LIST_WHILE( pWorld->pChatList, d, next_d, c_next, WMgr );
	
	if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
		GET_STATE( d ) == dDESC_STATE_FIGHT )
	{
		if( ownerGuild == GET_GUILD( d ) )
		{
			GET_GOTO_MAPKIND( d ) = dMAP_KIND_FIELD;
			GET_GOTO_MAPNUM( d ) = pWorld->mapNum;
			GET_GOTO_POSX( d ) = pWorld->warCristal.posX + number( 2, 7 );
			GET_GOTO_POSY( d ) = pWorld->warCristal.posY + number( 2, 7 );
			
			PlayerFromMap( d );
			SendMapLoading( d );
		}
		else
		{
			PlayerFromMap( d );
			PlayerToStartPoint( d, pWorld->mapNum );
			SendMapLoading( d );
		}
	}
	
	LIST_WHILEEND( pWorld->pChatList, d, next_d );
}


//
// Function Name: WAR_CheckEnd
// Date: 
// Description: 
//
void WAR_CheckEnd( sPWORLD pWorld , int point )
{
	if( pWorld->underAttack )
	{
		pWorld->warPoint += point;
		
		// 주인바뀐 
		if( pWorld->warPoint >= dWAR_WIN_POINT )
		{
			char query[128];
			
			sprintf( query, "delete from MAPOwnerGuild where mapNum = %d", pWorld->mapNum );
			
			if( mysql_query( g_MYSQL, query ) )
				log( "%s",mysql_error( g_MYSQL ) );
			
			sprintf( query, "insert into MAPOwnerGuild values( '', %d, %d, 5, 0 )", pWorld->mapNum, pWorld->warCristal.allowGuild[1] );
			
			if( mysql_query( g_MYSQL, query ) )
				log( "%s",mysql_error( g_MYSQL ) );
			
			pWorld->underAttack = 0;
			pWorld->ownerGuild = pWorld->warCristal.allowGuild[1];
			pWorld->warCristal.allowGuild[0] = 0;
			pWorld->warCristal.allowGuild[1] = 0;
			pWorld->warPoint = 0;
			
			char noticeMsg[1024];
			sprintf( noticeMsg, g_LANG_STR[189], pWorld->mapName, GET_GUILD_DATA( pWorld->ownerGuild )->guildName );
			//string test;
			//test = pWorld->mapName;
			//test.append( " castle war end : " );
			//test.append( GET_GUILD_DATA( pWorld->ownerGuild )->guildName );
			//test.append( " win!!" );
			
			//strcpy( noticeMsg, test.c_str() );
						
			g_nPos = 2;
			PutWord( g_Packet, dPACKET_GUILD_CHAT, g_nPos );
			PutString( g_Packet, g_LANG_STR[186], g_nPos );
			PutString( g_Packet, noticeMsg, g_nPos );
			PutSize( g_Packet, g_nPos );
			SendToMap( pWorld->mapNum, g_Packet, g_nPos );
			
			WAR_ClearWorldMob( pWorld->mapNum );
			
			WAR_SummonWarPosition( pWorld, pWorld->ownerGuild );
			
			SPoilsCastleTaxBank( pWorld->ownerGuild, pWorld->TaxBank );
			pWorld->TaxBank = 0;
			UpdateCastleTaxBank( pWorld->mapNum, 0 );
		}
	}
}


//
// Function Name: SPoilsCastleTaxBank
// Date: 
// Description: 
//
void SPoilsCastleTaxBank( int guildNum, int taxValue )
{
	if( !IS_VALID_GUILD( guildNum ) )
		return;
	
	int forMaster = (int)( taxValue * 0.1 );
	int forSecondMaster = (int)( taxValue * 0.05 );
	int forMember = (int)( taxValue * 0.85 );
	
	sPDESC_DATA d, next_d;
	
	int memberCnt = 0;
	
	//
	LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
	if( GET_GUILD( d ) == guildNum )
	{
		if( GET_GUILD_LEVEL( d ) == dGUILD_LEVEL_MEMBER )
		{
			memberCnt++;
		}
	}
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
	
	forMember = (int)( forMember / MIN( memberCnt, 1 ) );
	
	//
	LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr2 );
	
	if( GET_GUILD( d ) == guildNum )
	{
		if( GET_GUILD_LEVEL( d ) == dGUILD_LEVEL_MASTER )
		{
			if( forMaster )
			{
				GET_MONEY( d ) += forMaster;
				UpdateMoney( d );
				DATASERV_SendUpdateMoney( d );
				SendSystemMsg( d, g_LANG_STR[274], forMaster );
			}
		}
		else if( GET_GUILD_LEVEL( d ) == dGUILD_LEVEL_SECONDMASTER )
		{
			if( forSecondMaster )
			{
				GET_MONEY( d ) += forSecondMaster;
				UpdateMoney( d );
				DATASERV_SendUpdateMoney( d );
				SendSystemMsg( d, g_LANG_STR[274], forSecondMaster );
			}
		}
		else
		{
			if( forMember )
			{
				GET_MONEY( d ) += forMember;
				UpdateMoney( d );
				DATASERV_SendUpdateMoney( d );
				SendSystemMsg( d, g_LANG_STR[274], forMember );
			}
		}
	}
	
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
}


//
// Function Name: SPoilsCastleTaxBank
// Date: 
// Description: 
//
BOOL BattleSystem( sPDESC_DATA pAttacker, sPDESC_DATA pVic )
{
	time_t tval = g_CurrRealTime;
	struct tm *currTM = localtime( &tval );
	int n=0;

	// 일-금 21시 ( 오후 6시 ~ 오후12시 )<
	if( currTM->tm_wday > 6 && currTM->tm_hour >= 2 && currTM->tm_hour == 0 )
	{
		//sPWAR_DATA pWarData = NULL;

		//sPWORLD pWorld = gWORLD[GET_MAP_NUM( pAttacker )];
		// 맵이 길드소유인지 확인하고 맞으면 전투 아니면 나가기
		//if( pWorld->ownerGuild != GET_GUILD( pAttacker ) )
		//	return 0;		

		//적 길드인지 확인하고 맞으면 전투에 들어간다.
		//pWarData = WAR_FindData( GET_GUILD( pAttacker ), GET_GUILD( pVic );
		//if( pWarData )
		//{
			int nLevel = GET_ALL_SKILL( pAttacker) - GET_ALL_SKILL( pVic );
			if( nLevel== 0 )
			{
				//배틀포인트는 2점
				n = 2;
			}
			else if( nLevel > 0 )
			{
				//1점
				n = 1;
			}
			else if( nLevel < 0 )
			{
				//3점
				n = 3;
			}

			char query[1024];

			sprintf( query, "update GuildData set guildPoint = %d where guildNo = %d limit 1", 
				GET_GUILD_DATA( GET_GUILD( pAttacker ) )->guildNum, 
				GET_GUILD_DATA( GET_GUILD( pAttacker ) )->guildPoint );

			mysql_query( g_MYSQL, query );
			
			SendSystemMsg( pAttacker, "배틀포인트 %d를 추가하셨습니다.", n );
			GET_GUILD_DATA( GET_GUILD( pAttacker ) )->guildPoint += n;
			SendSystemMsg( pAttacker, "현재 길드 배틀포인트는 %d입니다.", GET_GUILD_DATA( GET_GUILD( pAttacker ) )->guildPoint );
			return 1;
	//	}
		//
		//update GuildData set guildPoint=30 where guildNo = 1
	//	else
	//		return 0;
	}
	else
		return 0;

	return 1;
}



// EOF

