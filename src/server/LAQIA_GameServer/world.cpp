/**
 *	$Workfile: world.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 21 $
 *	$Date: 04-09-08 3:54p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/world.cpp $
 * 
 * 21    04-09-08 3:54p Sk8snow
 * 
 * 20    04-08-04 9:27a Sk8snow
 * 
 * 19    04-07-13 7:26a Sk8snow
 * 
 * 18    04-05-29 10:12p Sk8snow
 * 
 * 17    04-05-21 11:37a Sk8snow
 * 
 * 16    04-04-30 9:46a Sk8snow
 * 
 * 15    04-04-07 10:17p Sk8snow
 * 
 * 14    04-03-31 8:22a Sk8snow
 *        
 */


#include "def.h"

#include "structs.h"
#include "extern.h"
#include "message.h"

#include "world.h"

#include "player.h"
#include "monster.h"

#include "house.h"

#include "billing.h"

BYTE		gWorldTime = 0;
DWORD		gWorldTimeTick = 0;

sPWORLD		gWORLD[dMAX_MAP_CNT];
BYTE		gWorldUnderAttack = 0;		// 전쟁중?
CList		gWORLDLIST;

sPWORLD_PARTY_BOARD gPARTY_BOARD_MEMORY = NULL;

//////////////////////////////////////////////////////////////
// World Time Table
//////////////////////////////////////////////////////////////
sTIME_SET gWorldTimeTable[7] =
{
	{ 1, 150000 * 2 },		// 1 ~ 2
	{ 3, 150000 * 3 },		// 3 ~ 5
	{ 6, 150000 * 3 },		// 6 ~ 8
	{ 9, 150000 * 4 },		// 9 ~ 12
	{ 13, 150000 * 5 },		// 13 ~ 19
	{ 20, 150000 * 3 },		// 20 ~ 22
	{ 23, 150000 * 2 }		// 23 ~ 24
};

///////////////////////////////////////////////////////////////
// MAP LOADING
///////////////////////////////////////////////////////////////
sLOADPOINT_INFO g_MAP_LOAD_POINT[dMAX_LOADING_POINT];


//
// Function Name: LoadLoadingPointInfo
// Date: 
// Description: 
//
BOOL LoadLoadingPointInfo()
{
	for( int k = 0; k < dMAX_LOADING_POINT; k++ )
	{
		g_MAP_LOAD_POINT[k].srcMapNum = -1;
		g_MAP_LOAD_POINT[k].srcPosX = -1;
		g_MAP_LOAD_POINT[k].srcPosY = -1;
		
		g_MAP_LOAD_POINT[k].destMapNum = -1;
		g_MAP_LOAD_POINT[k].destPosX = -1;
		g_MAP_LOAD_POINT[k].destPosY = -1;
		
		g_MAP_LOAD_POINT[k].posMapKind = 0;
	}

	FILE *fp = fopen( dMAP_LOADING_POINT_FILE, "rb" );

	if( !fp )
	{
		log( "LoadLoadingPointInfo : file open failed!\r\n" );
		return 0;
	}	

	int i = 0;

	int srcMapNum, srcPosX, srcPosY, destMapNum, destPosX, destPosY, posMapKind;

	while( !feof( fp ) )
	{
		fscanf( fp, "%d%d%d%d%d%d%d",
			&srcMapNum,
			&srcPosX,
			&srcPosY, 
			&destMapNum,
			&destPosX,
			&destPosY, 
			&posMapKind );

		if( destMapNum == 0 )
			continue;

		g_MAP_LOAD_POINT[i].srcMapNum = srcMapNum;
		g_MAP_LOAD_POINT[i].srcPosX = srcPosX;
		g_MAP_LOAD_POINT[i].srcPosY = srcPosY;
		g_MAP_LOAD_POINT[i].destMapNum = destMapNum;
		g_MAP_LOAD_POINT[i].destPosX = destPosX;
		g_MAP_LOAD_POINT[i].destPosY = destPosY;
		g_MAP_LOAD_POINT[i].posMapKind = posMapKind;

		gWORLD[srcMapNum]->objectMap[MIN( (int)srcPosY-1, 0 )][(int)srcPosX] = dWORLD_OBJ_LOADPOINT;			
		gWORLD[srcMapNum]->objectMap[MAX( (int)srcPosY+1, dONE_MAP_SIZE-1 )][(int)srcPosX] = dWORLD_OBJ_LOADPOINT;
		gWORLD[srcMapNum]->objectMap[(int)srcPosY][(int)srcPosX] = dWORLD_OBJ_LOADPOINT;
		gWORLD[srcMapNum]->objectMap[(int)srcPosY][MIN( (int)srcPosX-1, 0 )] = dWORLD_OBJ_LOADPOINT;
		gWORLD[srcMapNum]->objectMap[(int)srcPosY][MAX( (int)srcPosX+1, dONE_MAP_SIZE-1 )] = dWORLD_OBJ_LOADPOINT;

		i++;
	}

	fclose( fp );
	return 1;
}

/*
void CheckMobInvenTemp( int mapNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return;

	sPMOB_RESET reset, next_reset;

	LIST_WHILE( gWORLD[mapNum]->mobReset, reset, next_reset, next );

	if( IS_SET( g_MOBINFO[reset->mobNum]->mobAttr, MOBATTR_DEFENSE ) )
	{
		log( "OK! : map %d\r\n", mapNum );
		return;
	} 

	LIST_WHILEEND( gWORLD[mapNum]->mobReset, reset, next_reset );
}
*/

//
// Function Name: InitMap
// Date: 2003-07-25
// Description: 
//
void InitMap( int mapNum )
{
	int x,y,j;
	
	if( IS_VALID_WORLD( mapNum ) )
	{
		log( "InitMap : %d map already init\r\n" );
		return;
	}

	gWORLD[mapNum] = (sPWORLD)malloc(sizeof(sWORLD));

	if( !gWORLD[mapNum] )
	{
		log( "InitMap : World Data Malloc Failed\r\n" );
		abort();
	}

	///////////////////////////////////////////////
	// 일반 필드 데이터 
	///////////////////////////////////////////////
	for( y = 0; y < dONE_TILE_SIZE; y++ )
	{
		for( x = 0; x < dONE_TILE_SIZE; x++ )
		{
			gWORLD[mapNum]->player[y][x] = NULL;		
			gWORLD[mapNum]->mob[y][x] = NULL;
			gWORLD[mapNum]->item[y][x] = NULL;
		}
	}

	sprintf( gWORLD[mapNum]->mapName, "MAP%04d", mapNum );
	gWORLD[mapNum]->mapNum = mapNum;
	gWORLD[mapNum]->npcList = NULL;

	gWORLD[mapNum]->mobReset = NULL;
	gWORLD[mapNum]->mobResetCnt = 0;
	gWORLD[mapNum]->mobIdxCnt = 1;
	gWORLD[mapNum]->itemIdxCnt = 1;
	gWORLD[mapNum]->itemTax = 5;						// 세율 
	gWORLD[mapNum]->TaxBank = 0;						// 쌓인 세금
	gWORLD[mapNum]->currMobCnt = 0;
	gWORLD[mapNum]->currPlayerCnt = 0;

	gWORLD[mapNum]->pChatList = NULL;
	
	for( j = 0; j < dMAX_CASTLE_DOOR; j++ )
	{
		gWORLD[mapNum]->castleDoor[j].posX = 0;
		gWORLD[mapNum]->castleDoor[j].posY = 0;
		gWORLD[mapNum]->castleDoor[j].angle = 0;
	}

	gWORLD[mapNum]->warCristal.allowGuild[0] = 0;
	gWORLD[mapNum]->warCristal.allowGuild[1] = 0;
	gWORLD[mapNum]->warCristal.posX = 0;
	gWORLD[mapNum]->warCristal.posY = 0;
	
	gWORLD[mapNum]->hasCastle = 0;
	gWORLD[mapNum]->underAttack = 0;
	gWORLD[mapNum]->campTarget = 0;
	gWORLD[mapNum]->ownerGuild = 0;
	gWORLD[mapNum]->warPoint = 0;

	for( j = 0; j < dMAX_LINKED_CASTLE; j++ )
		gWORLD[mapNum]->linkedCastle[j] = 0;

	for( j = 0; j < dMAX_MOBINFO; j++ )
		gWORLD[mapNum]->MobCnt[j] = 0;
	
	for( y = 0; y < dONE_MAP_SIZE; y++ )
	{
		for( x = 0; x < dONE_MAP_SIZE; x++ )
		{
			gWORLD[mapNum]->mobMapAttr[y][x] = 0;
			gWORLD[mapNum]->playerMoveAttr[y][x] = 0;
			gWORLD[mapNum]->objectMap[y][x] = 0;
		}
	}

	gWORLD[mapNum]->partyBoard = NULL;

	// 리스트에 넣기
	gWORLDLIST.push( gWORLD[mapNum] );
	
	LoadMobMapAttr( mapNum );
	LoadMobReset( mapNum );	
	LoadHouseInfo( mapNum );
}


//
// Function Name: InitWorld
// Date: 2003-02-12
// Description: 
//
BOOL InitWorldData()
{
	gWORLDLIST.Init();

	gWorldTime = 0;

	for( int i = 0; i < dMAX_MAP_CNT; i++ )
		gWORLD[i] = NULL;
	
	FILE *fp = fopen( dMAP_LIST_FILE, "rb" );

	if( !fp )
	{
		log( "InitWorldData : map list file can't find\r\n" );
		return 0;
	}

	char tmp[1024];
	int mapNum;
	
	while( 1 )
	{
		if( feof( fp ) )
			break;

		fgets( tmp, 128, fp );

		if( *tmp == ';' )
			continue;

		if( *tmp == '#' )
			break;

		if( sscanf( tmp, "%d", &mapNum ) != 1 )
		{
			log( "InitWorldData : Bad file Format\r\n" );
			fclose( fp );
			return 0;
		}

		InitMap( mapNum );
	}
	
	fclose( fp );

	return 1;
}

void LoadMapName()
{
	FILE *fp = fopen( dMAP_NAME_FILE, "rb" );

	if( !fp )
	{
		log( "InitWorldData : map name file can't find\r\n" );
		return;
	}

	char tmp[1024];
	int mapNum;
	char mapName[128];
	
	while( 1 )
	{
		if( feof( fp ) )
			break;

		fgets( tmp, 128, fp );

		if( *tmp == ';' )
			continue;

		if( *tmp == '#' )
			break;

		if( sscanf( tmp, "%d %s", &mapNum, mapName ) != 2 )
		{
			log( "LoadMapName : Bad file Format\r\n" );
			fclose( fp );
			return;
		}

		if( IS_VALID_WORLD( mapNum ) )
			strcpy( gWORLD[mapNum]->mapName, mapName );
	}
	
	fclose( fp );
}


//
// Function Name: DestroyWorldData
// Date: 2003-07-25
// Description: 
//
void DestroyWorldData()
{
	gWORLDLIST.Destroy();
	sPMOB_RESET reset, next_reset;
	sPNPC_DATA npc, next_npc;
	sPWORLD_PARTY_BOARD board, next_board;

	for( int i = 0; i < dMAX_MAP_CNT; i++ )
	{
		if( gWORLD[i] )
		{
			LIST_WHILE( gWORLD[i]->mobReset, reset, next_reset, next, WMgr );
			SAFE_FREE( reset );
			LIST_WHILEEND( gWORLD[i]->mobReset, reset, next_reset );
	
			LIST_WHILE( gWORLD[i]->npcList, npc, next_npc, next, WMgr2 );
			SAFE_FREE( npc );
			LIST_WHILEEND( gWORLD[i]->npcList, npc, next_npc );
			
			LIST_WHILE( gWORLD[i]->partyBoard, board, next_board , next, Wmgr3 );
			SAFE_FREE( board );
			LIST_WHILEEND( gWORLD[i]->partyBoard, board, next_board );

			SAFE_FREE( gWORLD[i] );
		}
	}

	LIST_WHILE( gPARTY_BOARD_MEMORY, board, next_board, next, WMgr );
	SAFE_FREE( board );
	LIST_WHILEEND( gPARTY_BOARD_MEMORY, board, next_board );

}

//
// Function Name: FindPlayerInWorld
// Date: 2003-02-12
// Description: 
//
sPDESC_DATA FindPlayerInWorld( char *name )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr );
	if( !strcmp( name, d->ch.name ) && GET_STATE( d ) == dDESC_STATE_PLAYING )
		return d;
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
	return NULL;
}


//
// Function Name: FindPlayerInWorld
// Date: 2003-02-12
// Description: 
//
BOOL IsLoadPoint( sLOAD_POINT *point, int mapNum, int posX, int posY )
{
	point->mapNum = -1;
	point->posX = -1;
	point->posY = -1;

	for( int i = 0; i < 512; i++ )		
	{
		if( g_MAP_LOAD_POINT[i].srcMapNum == -1 )
			return 0;

		if( g_MAP_LOAD_POINT[i].srcMapNum == mapNum &&
			( g_MAP_LOAD_POINT[i].srcPosX == posX ||
			  g_MAP_LOAD_POINT[i].srcPosX == MIN( posX-1, 0 ) ||
			  g_MAP_LOAD_POINT[i].srcPosX == MAX( posX+1, dONE_MAP_SIZE-1 ) ) &&
			( g_MAP_LOAD_POINT[i].srcPosY == posY ||
			  g_MAP_LOAD_POINT[i].srcPosY == MIN( posY-1, 0 ) ||
			  g_MAP_LOAD_POINT[i].srcPosY == MAX( posY+1, dONE_MAP_SIZE-1 ) ) )
		{
			point->mapNum = g_MAP_LOAD_POINT[i].destMapNum;
			point->posX = g_MAP_LOAD_POINT[i].destPosX;
			point->posY = g_MAP_LOAD_POINT[i].destPosY;
			point->posMapKind = g_MAP_LOAD_POINT[i].posMapKind;
			
			return 1;
		}
	}
	return 0;
}

//
// Function Name: PrintAsciiMap
// Date: 2003-03-12
// Description: 
//
void PrintAsciiMap( int mapNum )
{
	FILE *fp;
	char filename[128];

	sprintf( filename, "ascii_map%d.txt", mapNum );

	fp = fopen( filename, "wb" );

	for( int y = 0; y < dONE_MAP_SIZE; y++ )
	{
		for( int x = 0; x < dONE_MAP_SIZE; x++ )
		{
			fprintf( fp, "%d", gWORLD[mapNum]->mobMapAttr[y][x] ? 1 : 0 );
		}
		fprintf( fp, "\r\n" );
	}
	fclose( fp );
}


//
// Function Name: LoadMobMapAttr
// Date: 2003-03-12
// Description: 
//
BOOL LoadMobMapAttr( int mapNum )
{
	FILE *fp;
	char filename[128];

	sprintf( filename, "./MAP/land%04d.map", mapNum );

	fp = fopen( filename, "rb" );

	if( !fp )
	{
		log( "LoadMobMapAttr : %s load failed\r\n", filename );
		return 0;
	}

	DWORD dwSize;
	DWORD dwVersion;
	DWORD m_dwSize;
	long skip_size = 0;
	char szTextureName[33];

	fread( &dwVersion, sizeof( DWORD ), 1, fp );
	fread( &dwSize, sizeof( DWORD ), 1, fp );
	fread( &szTextureName, 32, 1, fp );

	skip_size += ( dwSize * dwSize * sizeof( float ) );	// 높이 맵
	skip_size += ( dwSize * dwSize * sizeof( BYTE ) );	// 텍스춰 맵
	skip_size += ( dwSize * dwSize * sizeof( DWORD ) );	// 컬러

	/*
	for( int i = 0 ; i < 3 ; i++ )
	{
	pTerrain->SetLayer(i);
	pWorld->SetLayer(i);
	ar.Serialize( pTerrain->GetObjectHeightMap(),		dwSize * dwSize * 4 * sizeof( float ) );
	ar.Serialize( pWorld->GetMovable(),			dwSize * 2 * dwSize * 2 * sizeof( BYTE ) );
	ar.Serialize( pTemp,						dwSize * dwSize * sizeof( BYTE ) );
	} //for
	*/

	m_dwSize = dwSize * 4;

	// 몬스터의 이동속성은 0번 레이어의 이동 속성 값 사용
	skip_size += ( m_dwSize * m_dwSize * sizeof( float ) );

	fseek( fp, skip_size, SEEK_CUR );

	DWORD y, x;
	for( y = 0; y < m_dwSize; y++ )
	{
		for( x = 0; x < m_dwSize; x++ )
		{
			fread( &gWORLD[mapNum]->mobMapAttr[y][x], sizeof(BYTE), 1, fp );
		}
	}
	
	fclose( fp );

	return 1;
}


//
// Function Name: IsOpenMap
// Date: 2003-03-12
// Description: 
//
BOOL IsOpenMap( int mapNum )
{
	if( mapNum <= 0 || mapNum >= dMAX_MAP_CNT )
		return 0;

	if( !IS_VALID_WORLD( mapNum ) )
		return 0;

	return 1;
}


//
// Function Name: SendWorldTime
// Date: 2003-05-22
// Description: 
//
void SendWorldTime( sPDESC_DATA pPlayer )
{	
	if( GET_MAP_NUM( pPlayer ) == 10 
#ifdef dDAUM_BILLING
		|| GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE1
		|| GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE2
		|| GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE3
#endif
		)
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_CHANGE_TIME, g_nPos );
		PutByte( g_Packet, 23, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutInteger( g_Packet, 0, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );
		return;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHANGE_TIME, g_nPos );
	// 현재 시간 
	PutByte( g_Packet, gWorldTimeTable[gWorldTime].time, g_nPos );		
	
	// 다음 시간 
	if( gWorldTime == 6 )
		PutByte( g_Packet, gWorldTimeTable[0].time, g_nPos );			
	else
		PutByte( g_Packet, gWorldTimeTable[gWorldTime+1].time, g_nPos );
	
	// 다음 시간까지의 딜레이		
	PutInteger( g_Packet, (gWorldTimeTick - g_CurrTime), g_nPos );
	
	PutSize( g_Packet, g_nPos );


	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: NewPartyBoardArticle
// Date: 
// Description: 
//
sPWORLD_PARTY_BOARD NewPartyBoardArticle()
{
	sPWORLD_PARTY_BOARD newArticle = NULL;

	if( gPARTY_BOARD_MEMORY )
	{
		newArticle = gPARTY_BOARD_MEMORY;

		REMOVE_FROM_LIST( gPARTY_BOARD_MEMORY, newArticle, prev, next );

		return newArticle;
	}
	else
	{
		newArticle = (sPWORLD_PARTY_BOARD)malloc(sizeof(sWORLD_PARTY_BOARD));

		if( !newArticle )
			return NULL;
	}

	return newArticle;
}


//
// Function Name: NewPartyBoardArticle
// Date: 
// Description: 
//
sPWORLD_PARTY_BOARD FindPartyBoardArticle( sPDESC_DATA pPlayer )
{
	// 집 내부일 경우
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		return NULL;

	sPWORLD_PARTY_BOARD board, next_board;

	LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->partyBoard, board, next_board, next, WMgr );
	
	if( board->pPlayer == pPlayer )
		return board;

	LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->partyBoard, board, next_board );
	
	return NULL;
}


//
// Function Name: SendChangeWeather
// Date:
// Description: 0 = 기본, 1 = 눈, 2 = 비
//
void SendChangeWeather( sPDESC_DATA pPlayer, BYTE value )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHANGE_WEATHER, g_nPos );
	PutByte( g_Packet, value, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: SendToMap
// Date:
// Description: 
//
void SendToMap( int mapNum , char *data, int size )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return;

	for( int y = 0; y < dONE_TILE_SIZE; y++ )
	{
		for( int x = 0; x < dONE_TILE_SIZE; x++ )
		{
			SendToSector( NULL, gWORLD[mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
		}
	}
}


void UpdateCastleTaxValue( int mapNum, int taxValue )
{
	char	query[1024];

	sprintf( query, "update MAPOwnerGuild set taxValue = %d where mapNum = %d", taxValue, mapNum );

	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
	}
}

void UpdateCastleTaxBank( int mapNum, int value )
{
	char query[1024];

	sprintf( query, "update MAPOwnerGuild set taxBank = %d where mapNum = %d", value, mapNum );

	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
	}
}

//



