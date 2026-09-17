/**
 *	$Workfile: house.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 8 $
 *	$Date: 04-09-11 11:20p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/house.cpp $
 * 
 * 8     04-09-11 11:20p Sk8snow
 * 
 * 7     04-07-13 7:26a Sk8snow
 * 
 * 6     04-03-31 8:22a Sk8snow
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

sPHOUSE		gHOUSE[dMAX_HOUSE];


//////////////////////////////////////////////////
// HOUSE DOOR INFO
//////////////////////////////////////////////////
int FindHouse( int mapNum, int doorNum )
{
	for( int i = 0; i < dMAX_HOUSE; i++ )
	{
		if( !IS_VALID_HOUSE( i ) )
			return -1;

		if( gHOUSE[i]->mapNum == mapNum &&
			gHOUSE[i]->doorNum == doorNum )
			return i;
	}

	return -1;
}

int GetEmptyHouseNum()
{
	for( int i = 0; i < dMAX_HOUSE; i++ )
	{
		if( !IS_VALID_HOUSE( i ) )
			return i;
	}
	return -1;
}


//
// Function Name: InitHouse
// Date: 
// Description: 
//
BOOL InitHouse( int hNum )
{
	if( IS_VALID_HOUSE( hNum ) )
	{
		log( "InitHouse : %d house is already Inited\r\n" );
		return 1;
	}
	
	gHOUSE[hNum] = (sPHOUSE)malloc(sizeof(sHOUSE));

	if( !gHOUSE[hNum] )
	{
		log( "InitHouse : %d house malloc failed\r\n" );
		return 0;
	}
	
	gHOUSE[hNum]->mapNum = 0;								// 소속 맵
	gHOUSE[hNum]->houseNum = 0;								// 집번호 

	*gHOUSE[hNum]->ownerID = NULL_STR;							// 주인 ID
	gHOUSE[hNum]->ownerPos = 0;								// 케릭터 번호 

	gHOUSE[hNum]->currPlayerCnt = 0;
	gHOUSE[hNum]->currMobCnt = 0;
	gHOUSE[hNum]->mobIdxCnt = 0;
	gHOUSE[hNum]->itemIdxCnt = 0;

	gHOUSE[hNum]->doorNum = 0;								// 집에 들어가기 위한 문번호
	gHOUSE[hNum]->houseType = 0;							// 집 모델 번호

	gHOUSE[hNum]->targetX = 0;
	gHOUSE[hNum]->targetY = 0;

	gHOUSE[hNum]->linkHouse = NULL;
	gHOUSE[hNum]->linkMap = NULL;

	gHOUSE[hNum]->linkMapDoor = 0;
	gHOUSE[hNum]->linkHouseDoor = 0;

	gHOUSE[hNum]->in_player = NULL;
	gHOUSE[hNum]->in_items = NULL;
	gHOUSE[hNum]->in_mob = NULL;

	return 1;	
}

sPHOUSE CreateHouseData()
{
	sPHOUSE pHouse = (sPHOUSE)malloc(sizeof(sHOUSE));

	if( !pHouse )
	{
		log( "CreateHouseData : %d house malloc failed\r\n" );
		return NULL;
	}
	
	pHouse->mapNum = 0;								// 소속 맵
	pHouse->houseNum = 0;								// 집번호 

	*pHouse->ownerID = NULL_STR;							// 주인 ID
	pHouse->ownerPos = 0;								// 케릭터 번호 

	pHouse->currPlayerCnt = 0;
	pHouse->currMobCnt = 0;
	pHouse->mobIdxCnt = 0;
	pHouse->itemIdxCnt = 0;

	pHouse->doorNum = 0;								// 집에 들어가기 위한 문번호
	pHouse->houseType = 0;								// 집 모델 번호

	pHouse->targetX = 0;
	pHouse->targetY = 0;

	pHouse->linkHouse = NULL;
	pHouse->linkMap = NULL;

	pHouse->linkMapDoor = 0;
	pHouse->linkHouseDoor = 0;

	pHouse->in_player = NULL;
	pHouse->in_items = NULL;
	pHouse->in_mob = NULL;

	return pHouse;	
}

void LinkIndoorHouse( sPHOUSE pHouse )
{
/*	if( pHouse->linkHouse )
		return;

	FILE *fp;
	char filename[128];

	sprintf( filename, "./MAP/DOOR/bi_%04d.door", pHouse->houseNum );

	fp = fopen( filename, "rb" );

	if( !fp )
		return;
	
	sDOOR door;
	DWOR ver;
	DWORD doorCnt;

	fread( &ver, sizeof(DWORD), 1, fp );
	fread( &doorCnt, sizeof(DWORD), 1, fp );
	
	for( DWORD i = 0; i < doorCnt; i++ )
	{
		fread( &door, sizeof(sDOOR), 1, fp );

		sPHOUSE pNewHouse = (sPHOUSE)malloc(sizeof(sHOUSE));

		if( !pNewHouse )
		{
			log( "LinkIndoorHouse : malloc failed\r\n" );
			return;
		}

		pNewHouse->mapNum = pHouse->mapNum;
		pNewHouse->houseNum = 

		// 내부->필드 = 0, 필드->집안 = 1, 필드->성내부 = 2
		gHOUSE[hNum]->mapNum = mapNum;						// 소속 맵
		gHOUSE[hNum]->houseNum = hNum;						// 집번호 
						
		gHOUSE[hNum]->doorNum = door.dwID;					// 집에 들어가기 위한 문번호
		gHOUSE[hNum]->houseType = door.dwMoveMap;			// 집 모델 번호
		
		gHOUSE[hNum]->targetX = (int)door.fX;
		gHOUSE[hNum]->targetY = (int)door.fY;

		gHOUSE[hNum]->linkMap = gWORLD[mapNum];
	}*/
}

BOOL LoadHouseInfo( int mapNum )
{
	sDOOR door;
	DWORD ver;
	DWORD doorCnt;
	
	int hNum; 

	char filename[128];

	sprintf( filename, "./MAP/DOOR/land%04d.door", mapNum );

	FILE *fp = fopen( filename, "rb" );

	if( !fp )
	{
//		log( "LoadHouseInfo : Map[%d] House is Empty\r\n" , mapNum );
		return 0;
	}
	fread( &ver, sizeof(DWORD), 1, fp );
	fread( &doorCnt, sizeof(DWORD), 1, fp );

	for( DWORD i = 0; i < doorCnt; i++ )
	{
		fread( &door, sizeof(sDOOR), 1, fp );

		// 아직 성 내부는 안됨
		if( door.btType == 2 )
			continue;

		hNum = GetEmptyHouseNum();

		if( hNum == -1 )
		{
			log( "LoadHouseInfo : House is FULL!!\r\n" );
			abort();
		}
		
		InitHouse( hNum );

		// 내부->필드 = 0, 필드->집안 = 1, 필드->성내부 = 2
		gHOUSE[hNum]->mapNum = mapNum;						// 소속 맵
		gHOUSE[hNum]->houseNum = hNum;						// 집번호 
						
		gHOUSE[hNum]->doorNum = door.dwID;					// 집에 들어가기 위한 문번호
		gHOUSE[hNum]->houseType = door.dwMoveMap;			// 집 모델 번호
		
		gHOUSE[hNum]->targetX = (int)door.fX;
		gHOUSE[hNum]->targetY = (int)door.fY;

		gHOUSE[hNum]->linkMap = gWORLD[mapNum];
	}

	fclose( fp );

	return 1;
}



//
// Function Name: DestroyWorldHouse
// Date: 
// Description: 
//
void DestroyWorldHouse()
{
	for( int i = 0; i < dMAX_HOUSE; i++ )
	{
		if( IS_VALID_HOUSE( i ) )
			SAFE_FREE( gHOUSE[i] );
	}
}


//
// Function Name: SendToHouse
// Date: 
// Description: 
//
void SendToHouse( int mapNum, char *data, int size )
{
	if( !IS_VALID_HOUSE( mapNum ) )
	{
		log( "SendToHouse : mapNum is INVALID\r\n" );
		return;
	}

	sPDESC_DATA player, next_player;

	LIST_WHILE( gHOUSE[mapNum]->in_player, player, next_player, w_next, WMgr );
	SendData( player , data, size );
	LIST_WHILEEND( gHOUSE[mapNum]->in_player, player, next_player );
}



//



