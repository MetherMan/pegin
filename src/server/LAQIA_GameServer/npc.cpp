/**
 *	$Workfile: npc.cpp $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 12 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/npc.cpp $
 * 
 * 12    04-12-15 10:40a Sk8snow
 * 
 * 11    04-09-08 3:54p Sk8snow
 * 
 * 10    04-07-13 7:26a Sk8snow
 * 
 * 9     04-03-31 8:22a Sk8snow
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
#include "npc.h"

#include "db_rows.h"
#include "shop.h"


int GetNpcByModelName( char *szModelName )
{
	if( szModelName[0] != 'c' && szModelName[0] != 'C' )
	{
		return 0;
	} //if

	if( szModelName[1] != 'n' && szModelName[1] != 'N' )
	{
		return 0;
	} //if
	
	if( szModelName[2] != '_' )
	{
		return 0;
	} //if
	
	char		szNumber[5];
	memset( szNumber, 0, 5 );
	int			nNum;
	strncpy( szNumber, szModelName + 3, 4 );
	nNum = atoi( szNumber );

	return nNum;
} 

//
// Function Name: LoadNpcPos
// Date: 
// Description: 
//
BOOL LoadNpcPos( int mapNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return 0;

	FILE *fp;
	char filename[128];

	sprintf( filename, "./MAP/land%04d.map", mapNum );

	fp = fopen( filename, "rb" );

	if( !fp )
	{
		log( "LoadNpcPos : %s file not found\r\n", filename );
		return 0;
	}
	
	DWORD dwSize;
	DWORD m_dwSize;
	DWORD dwVersion;
	long skip_size = 0;

	fread( &dwVersion, sizeof( DWORD ), 1, fp );
	fread( &dwSize, sizeof( DWORD ), 1, fp );
	
	m_dwSize = dwSize * 4;
	
	skip_size += 32; // ≈ÿΩ∫√Á ¿Ã∏ß 	
	skip_size += ( dwSize * dwSize * sizeof( float ) );	// ≥Ù¿Ã ∏ 
	skip_size += ( dwSize * dwSize * sizeof( BYTE ) );	// ≈ÿΩ∫√Á ∏ 
	skip_size += ( dwSize * dwSize * sizeof( DWORD ) );	// ƒ√∑Ø

	// Layer ( «ˆ¿Á √— 3∞≥ )
	int i;
	
	for( i = 0; i < dMAX_MAP_LAYER; i++ )
	{
		skip_size += ( m_dwSize * m_dwSize * sizeof( float ) );
		skip_size += ( m_dwSize * m_dwSize * sizeof( BYTE ) );
		skip_size += ( dwSize * dwSize * sizeof( BYTE ) );
	}

	fseek( fp, skip_size, SEEK_CUR );

	int		numEntities;

	fread( &numEntities, sizeof( int ), 1, fp );

	char	szName[32];
	Vector3	vPos;
	float	fYaw;

	for( i = 0; i < numEntities; i++ )
	{	
		fread( &szName, 32, 1, fp );
		fread( &vPos, sizeof( Vector3 ), 1, fp );
		fread( &fYaw, sizeof( float ), 1, fp );

		int npcNum;

		if( ( npcNum = GetNpcByModelName( szName ) ) )
		{
			SetNpcPos( mapNum, (int)vPos.x, (int)vPos.y, npcNum );
		}
	}

	fclose(fp);

	return 1;
}

//
// Function Name: LoadNpcInfo
// Date: 
// Description:		
//
BOOL LoadNpcInfo()
{
#ifdef _dUMENOKOJI_QUEST_
	FILE	*fp = fopen( dTEST_MOB_NPCINFO_TABLE_FILE, "rb" );
#else
	FILE *fp = fopen( dNPCINFO_TABLE_FILE, "rb" );
#endif

	if( !fp )
	{
		log( "LoadNpcInfo : !fp\r\n" );
		return 0;
	}

	int mapNum;
	int npcNum;
	int isGarbageKeeper;
	int shopNum;
	int questNum;
	int isGuildMaster;

	char readBuff[1024];

	while( !feof( fp ) )
	{
		fgets( readBuff, 1024, fp );

		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		if( sscanf( readBuff, "%d %d %d %d %d %d", &mapNum, &npcNum, &isGarbageKeeper, &shopNum, &questNum, &isGuildMaster ) != 6 )
		{
			log( "LoadNpcInfo : Bad file format\r\n" );
			fclose( fp );
			return 0;
		}
		if( !IS_VALID_WORLD( mapNum ) )
		{
			log( "LoadNpcInfo : valid map num [map %d]\r\n", mapNum );
			fclose( fp );
			return 0;
		}
		
		sPNPC_DATA newNpc = (sPNPC_DATA)malloc(sizeof(sNPC_DATA));
		
		if( !newNpc )
		{
			log( "LoadNpcInfo : malloc failed\r\n" );
			fclose( fp );
			return 0;
		}
		
		newNpc->npcNum = npcNum;
		newNpc->questNum = questNum;
		newNpc->shopNum = shopNum;
		newNpc->isGarbage = isGarbageKeeper;
		newNpc->isGuildMaster = isGuildMaster;

		newNpc->mapNum = 0;
		newNpc->posX = 0;
		newNpc->posY = 0;

		newNpc->next = NULL;
		newNpc->prev = NULL;

		INSERT_TO_LIST( gWORLD[mapNum]->npcList, newNpc, prev, next );
	}

	fclose( fp );
	
	// Asign Npc 
	fp = fopen( dMAP_LIST_FILE, "rb" );
	
	if( !fp )
	{
		log( "LoadNpcInfo : map list file can't find\r\n" );
		return 0;
	}
	
	char tmp[128];
	
	while( !feof(fp) )
	{
		fgets( tmp, 128, fp );
		
		if( *tmp == ';' )
			continue;
		
		if( *tmp == '#' )
			break;
		
		LoadNpcPos( atoi( tmp ) );
	}
	
	fclose( fp );
	
	return 1;
}


//
// Function Name: DestroyWorldData
// Date: 2003-07-25
// Description: 
//
void SendNpcInfo( sPDESC_DATA pPlayer, int mapNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "SendNpcInfo : invalid map num [%s/%s]\r\n", GET_NAME( pPlayer ), GET_ID( pPlayer ) );
		return;
	}

	BYTE npcCnt = 0;
	sPNPC_DATA npc, next_npc;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_NPC_INFO, g_nPos );
	g_nPos += 1;

	LIST_WHILE( gWORLD[mapNum]->npcList, npc, next_npc, next, WMgr );
	PutInteger( g_Packet, npc->npcNum, g_nPos );

	if( !IS_VALID_SHOP( npc->shopNum ) )
		PutWord( g_Packet, 0, g_nPos );
	else
		PutWord( g_Packet, npc->shopNum, g_nPos );

	PutWord( g_Packet, npc->questNum, g_nPos );
	PutByte( g_Packet, npc->isGarbage, g_nPos );
	PutByte( g_Packet, npc->isGuildMaster, g_nPos );
	npcCnt++;
	LIST_WHILEEND( gWORLD[mapNum]->npcList, npc, next_npc );
	*(BYTE*)( g_Packet + 4 ) = npcCnt;
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: DestroyNpcInfo
// Date: 
// Description: 
//
void DestroyNpcInfo()
{
	sPNPC_DATA npc, next_npc;

	for( int i = 0; i < dMAX_MAP_CNT; i++ )
	{
		if( gWORLD[i] )
		{			
			LIST_WHILE( gWORLD[i]->npcList, npc, next_npc, next, WMgr );
			REMOVE_FROM_LIST( gWORLD[i]->npcList, npc, prev, next );
			SAFE_FREE( npc );
			LIST_WHILEEND( gWORLD[i]->npcList, npc, next_npc );			
		}
	}
}


//
// Function Name: SetNpcPos
// Date: 
// Description: 
//
void SetNpcPos( int mapNum, int posX, int posY, int npcNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "SetNpcPos : invalid map num [%d]\r\n", mapNum );
		return;
	}

	sPNPC_DATA npc, next_npc;

	LIST_WHILE( gWORLD[mapNum]->npcList, npc, next_npc, next, WMgr );
	if( npcNum == npc->npcNum )
	{
		npc->mapNum = mapNum;
		npc->posX = posX;
		npc->posY = posY;
		return;
	}
	LIST_WHILEEND( gWORLD[mapNum]->npcList, npc, next_npc );
}


//
// Function Name: IsGarbageArea
// Date: 
// Description: 
//
BOOL IsGarbageArea( sPDESC_DATA pPlayer )
{
	if( GET_MAP_NUM( pPlayer ) == dDEAL_MAP_NUM )
		return 1;

	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_FIELD )
		return 0;

	int mapNum = GET_MAP_NUM( pPlayer );

	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "IsGarbageArea : invalid map num [%d]\r\n", mapNum );
		return 0;
	}
	
	int checkX, checkY;

	sPNPC_DATA npc, next_npc;

	LIST_WHILE( gWORLD[mapNum]->npcList, npc, next_npc, next, WMgr );
	
	if( !npc->mapNum )
		LIST_SKIP( npc, next_npc );

	checkX = abs( GET_POSX( pPlayer ) - npc->posX );
	checkY = abs( GET_POSY( pPlayer ) - npc->posY );

	if( npc->isGarbage && ( checkX <= dNPC_AREA && checkY <= dNPC_AREA ) )
		return 1;

	LIST_WHILEEND( gWORLD[mapNum]->npcList, npc, next_npc );

	return 0;
}

BOOL FindNpcInMap( sPDESC_DATA pPlayer, int npcNum )
{
	int mapNum = GET_MAP_NUM( pPlayer );

	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "FindNpcInMap : invalid map num [%d]\r\n", mapNum );
		return 0;
	}
	
	int checkX, checkY;

	sPNPC_DATA npc, next_npc;

	LIST_WHILE( gWORLD[mapNum]->npcList, npc, next_npc, next, WMgr );
	
	if( !npc->mapNum )
		LIST_SKIP( npc, next_npc );

	checkX = abs( GET_POSX( pPlayer ) - npc->posX );
	checkY = abs( GET_POSY( pPlayer ) - npc->posY );

	if( npc->isGarbage && ( checkX <= dNPC_AREA && checkY <= dNPC_AREA ) && npc->npcNum == npcNum )
		return 1;

	LIST_WHILEEND( gWORLD[mapNum]->npcList, npc, next_npc );

	return 0;
}

//
// Function Name: IsShopKeeperArea
// Date: 
// Description: 
//
BOOL IsShopKeeperArea( sPDESC_DATA pPlayer, int shopNum )
{
	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_FIELD )
		return 0;

	int mapNum = GET_MAP_NUM( pPlayer );

	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "IsShopKeeperArea : invalid map num [%d]\r\n", mapNum );
		return 0;
	}
	
	int checkX, checkY;

	sPNPC_DATA npc, next_npc;

	LIST_WHILE( gWORLD[mapNum]->npcList, npc, next_npc, next, WMgr );

	if( !npc->mapNum )
		LIST_SKIP( npc, next_npc );

	checkX = abs( GET_POSX( pPlayer ) - npc->posX );
	checkY = abs( GET_POSY( pPlayer ) - npc->posY );

	if( npc->shopNum == shopNum && ( checkX <= dNPC_AREA && checkY <= dNPC_AREA ) )
		return 1;

	LIST_WHILEEND( gWORLD[mapNum]->npcList, npc, next_npc );

	return 0;
}

//
// Function Name: IsGuildKeeperArea
// Date: 
// Description: 
//
BOOL IsGuildKeeperArea( sPDESC_DATA pPlayer )
{
	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_FIELD )
		return 0;

	int mapNum = GET_MAP_NUM( pPlayer );

	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "IsGuildKeeperArea : invalid map num [%d]\r\n", mapNum );
		return 0;
	}
	
	int checkX, checkY;

	sPNPC_DATA npc, next_npc;

	LIST_WHILE( gWORLD[mapNum]->npcList, npc, next_npc, next, WMgr );

	if( !npc->mapNum )
		LIST_SKIP( npc, next_npc );

	checkX = abs( GET_POSX( pPlayer ) - npc->posX );
	checkY = abs( GET_POSY( pPlayer ) - npc->posY );

	if( npc->isGuildMaster && ( checkX <= dNPC_AREA && checkY <= dNPC_AREA ) )
		return 1;

	LIST_WHILEEND( gWORLD[mapNum]->npcList, npc, next_npc );
	
	return 0;
}


//EOF


