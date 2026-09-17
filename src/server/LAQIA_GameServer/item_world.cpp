#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "player.h"
#include "item.h"
#include "item_world.h"

#include "world.h"
#include "player_world.h"

#include "message.h"

/*
 *	MakeItemInPacket
 */
void MakeItemInPacket( sPITEM_DATA pItem )
{
	PutHeader( dPACKET_INSERT_ITEM );
	PutInteger( pItem->idxNum );
	PutInteger( pItem->itemNum );
	PutInteger( pItem->mapX );
	PutInteger( pItem->mapY );
	PutInteger( pItem->containCnt );
}

/*
 *	MakeItemOutPacket
 */
void MakeItemOutPacket( sPITEM_DATA pItem )
{
	PutHeader( dPACKET_REMOVE_ITEM );
	PutInteger( pItem->idxNum );
}


/*
 *	SendToItemArea
 */
void SendToItemArea( sPITEM_DATA pItem, char *data, int size )
{
	if( !IS_VALID_WORLD( pItem->mapNum ) )
		return;

	int posX = (int)(pItem->mapX/dONE_TILE);
	int posY = (int)(pItem->mapY/dONE_TILE);
		
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[pItem->mapNum]->player[y][x], data, size , dSENDMODE_TOALL );
}

/*
 *	GetItemSector
 */
void GetItemSector( sPDESC_DATA pPlayer, int x, int y )
{
	if( !IS_VALID_WORLD( pPlayer->ch.mapNum ) )
		return;

	sPITEM_DATA item, next_item;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->item[y][x], item, next_item , w_next );
	MakeItemInPacket( item );
	SendData( pPlayer, g_Packet, g_nPos );
	LIST_WHILEEND();
}

/*
 *	GetItemVSector
 */
void GetItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				GetItemSector( pPlayer,  x, y );
		}
	}
}

/*
 *	GetItemHSector
 */
void GetItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				GetItemSector( pPlayer, x, y );
		}
	}
}

/*
 *	OutItemSector
 */
void OutItemSector( sPDESC_DATA pPlayer, int x, int y )
{
	if( !IS_VALID_WORLD( pPlayer->ch.mapNum ) )
		return;

	sPITEM_DATA item, next_item;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->item[y][x], item, next_item , w_next );
	MakeItemOutPacket( item );
	SendData( pPlayer, g_Packet, g_nPos );
	LIST_WHILEEND();
}

/*
 *	OutItemVSector
 */
void OutItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				OutItemSector( pPlayer,  x, y );
		}
	}
}

/*
 *	OutItemHSector
 */
void OutItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				OutItemSector( pPlayer, x, y );
		}
	}
}

/*
 *	GetCurrAreaItem
 */
void GetCurrAreaItem( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				GetItemSector( pPlayer, x, y );
}

/*
 *	OutCurrAreaItem
 */
void OutCurrAreaItem( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				OutItemSector( pPlayer, x, y );
}

/*
 *	CheckItemLifeInMap
 */
void CheckItemLifeInMap()
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( g_ITEMS.list, item, next_item, next );

	item->life_in_map--;

	if( item->life_in_map <= 0 )
	{
		// ¾ÆÀÌÅÛ ¼Ò¸ê
		ItemFromMap( item );
		REMOVE_ITEM( item );
		LIST_SKIP();
	}

	LIST_WHILEEND();
}

//EOF


