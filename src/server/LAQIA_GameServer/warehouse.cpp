#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "player.h"
#include "player_world.h"
#include "item.h"
#include "item_world.h"
#include "world.h"
#include "message.h"
#include "mob.h"

#include "combat.h"
#include "constants.h"
#include "warehouse.h"


/*
 *	WAREHOUSE_LoadData
 */
void WAREHOUSE_LoadData( sPDESC_DATA pPlayer )
{
	enum
	{
		eSERIAL = 0,
		eNAME,
		eMAKER,
		eOWNERID,
		eITEMNUM,
		eCNT,
		eITEMATTR,
		eEXATTR1,
		eEXVALUE1,
		eEXATTR2,
		eEXVALUE2,
		eEXATTR3,
		eEXVALUE3,
		eEXATTR4,
		eEXVALUE4,
		eEXATTR5,
		eEXVALUE5,
		eSETNUM,
		eLEVEL,
		eINVENPOS,
		eINVENARRAY,
		eISEQUIP,
		eISMAJOR
	};

	char escID[128];

	if( g_ItemSql.Query( "select * from WareHouse where ownerID = '%s'", 
		g_ItemSql.StringConvert( GET_ID( pPlayer ), escID ) ) )
	{
		log( "WAREHOUSE_LoadData : %s\r\n", g_ItemSql.GetLastError() );
		return;
	}
	else
	{
		if( g_ItemSql.GetResult() )
		{
			while( g_ItemSql.GetRowData() )
			{
				int itemNum = g_ItemSql.GetInt( eITEMNUM );

				sPITEM_DATA pItem = CreateItem( itemNum );

				if( !pItem )
				{
					log( "WAREHOUSE_LoadData : malloc failed\r\n" );
					g_ItemSql.FreeResult();
					return;
				}
				
				pItem->itemNum = itemNum;									// 아이템 번호 
				
				pPlayer->invenItemIndex++;
				pItem->idxNum = pPlayer->invenItemIndex;

				strcpy( pItem->name, g_ItemSql.GetRow( eNAME ) );			// 이름
				strcpy( pItem->serial, g_ItemSql.GetRow( eSERIAL ) );		// 시리얼 
				strcpy( pItem->maker, g_ItemSql.GetRow( eMAKER ) );			// 제작자
				
				pItem->containCnt = g_ItemSql.GetInt( eCNT );				// 수량
												
				pItem->exAttr[0] = g_ItemSql.GetInt( eEXATTR1 );
				pItem->exValue[0] = g_ItemSql.GetInt( eEXVALUE1 );
				pItem->exAttr[1] = g_ItemSql.GetInt( eEXATTR2 );
				pItem->exValue[1] = g_ItemSql.GetInt( eEXVALUE2 );
				pItem->exAttr[2] = g_ItemSql.GetInt( eEXATTR3 );
				pItem->exValue[2] = g_ItemSql.GetInt( eEXVALUE3 );
				pItem->exAttr[3] = g_ItemSql.GetInt( eEXATTR4 );
				pItem->exValue[3] = g_ItemSql.GetInt( eEXVALUE4 );
				pItem->exAttr[4] = g_ItemSql.GetInt( eEXATTR5 );
				pItem->exValue[4] = g_ItemSql.GetInt( eEXVALUE5 );

				pItem->invenPos = g_ItemSql.GetInt( eINVENPOS );
				pItem->invenArray = g_ItemSql.GetInt( eINVENARRAY );

				pItem->level = g_ItemSql.GetInt( eLEVEL );
				pItem->setNum = g_ItemSql.GetInt( eSETNUM );
				pItem->attr = g_ItemSql.GetInt( eITEMATTR );

				pItem->equipPos = g_ItemSql.GetInt( eISEQUIP );

				INSERT_TO_LIST( pPlayer->ch.warehouse, pItem, inven_prev, inven_next );
			}
			g_ItemSql.FreeResult();
		}
	}

	// 창고돈
	if( g_ItemSql.Query( "select amount from Bank where ownerID = '%s' limit 1", g_ItemSql.StringConvert( GET_ID( pPlayer ), escID ) ) )
	{
		log( "WAREHOUSE_LoadData : %s\r\n", g_ItemSql.GetLastError() );
		return;
	}
	else
	{
		if( g_ItemSql.GetResult() )
		{
			if( g_ItemSql.GetRowData() )
			{
				pPlayer->ch.btHasBankRow = 1;
				pPlayer->ch.bankMoney = g_ItemSql.GetInt( 0 );
			}
			g_ItemSql.FreeResult();
		}
	}
}


/*
 *	ItemToDB
 */
BOOL ItemToWarehouseDB( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pPlayer || !pItem )
		return 0;
	
	int dbRet = g_ItemSql.Query( "insert into WareHouse values( "
		"'%s',"		// serial
		"'%s',"		// name 
		"'%s',"		// Maker
		"'%s',"		// ownerID
		"%d,"		// itemNum
		"%d,"		// cnt
		"%d,"		// itemAttr
		"%d,"		// exAttr1
		"%d,"		// exValue1
		"%d,"		// exAttr2
		"%d,"		// exValue2
		"%d,"		// exAttr3
		"%d,"		// exValue3
		"%d,"		// exAttr4
		"%d,"		// exValue4
		"%d,"		// exAttr5
		"%d,"		// exValue5
		"%d,"		// set num
		"%d,"		// level
		"%d,"		// invenPos
		"%d,"		// invenArray
		"%d,"		// IsEquip
		"%d"		// IsMajor
		")",
		g_ItemSql.StringConvert( pItem->serial, escSerial ),
		g_ItemSql.StringConvert( pItem->name, escItemName ),
		g_ItemSql.StringConvert( pItem->maker, escMaker ),
		g_ItemSql.StringConvert( GET_ID( pPlayer ), escID ),
		pItem->itemNum,
		pItem->containCnt,
		pItem->attr,
		pItem->exAttr[0],
		pItem->exValue[0],
		pItem->exAttr[1],
		pItem->exValue[1],
		pItem->exAttr[2],
		pItem->exValue[2],
		pItem->exAttr[3],
		pItem->exValue[3],
		pItem->exAttr[4],
		pItem->exValue[4],
		pItem->setNum,
		pItem->level,
		pItem->invenPos,
		pItem->invenArray,
		-1,
		pItem->isMajor );
	
	if( dbRet )
	{
		log( "ItemToWarehouse : Item Insert Failed[%s/%s/%s]\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ), pItem->serial );
		return 0;
	}
	return 1;
}

/*
 *	ItemFromDB
 */
BOOL ItemFromWareHouseDB( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( g_ItemSql.Query( "delete from WareHouse where serial = '%s' limit 1", g_ItemSql.StringConvert( pItem->serial, escSerial ) ) )
	{
		log( "ItemFromWareHouse : %s\r\n", g_ItemSql.GetLastError() );
		return 0;
	}
	return 1;
}

/*
 *	FindItemFromWarehouse
 */
sPITEM_DATA FindItemFromWarehouse( sPDESC_DATA pPlayer, int idxNum )
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->ch.warehouse, item, next_item, inven_next );
	
	if( item->idxNum == idxNum )
		return item;

	LIST_WHILEEND();

	return NULL;
}

/*
 *	ItemToWarehouse
 */
void ItemToWarehouse( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pPlayer || !pItem )
		return;

	pPlayer->invenItemIndex++;
	if( pPlayer->invenItemIndex >= 999999999 )
		pPlayer->invenItemIndex = 1;

	pItem->idxNum = pPlayer->invenItemIndex;


	INSERT_TO_LIST( pPlayer->ch.warehouse, pItem, inven_prev, inven_next );

	PutHeader( dPACKET_WAREHOUSE_INSERTITEM );
	PutInteger( pItem->idxNum );
	PutInteger( pItem->itemNum );
	PutString( pItem->name );
	PutString( pItem->maker );
	PutInteger( pItem->containCnt );
	for( i = 0; i < dITEM_EXVALUE; i++ )
	{
		PutInteger( pItem->exAttr[i] );
		PutInteger( pItem->exValue[i] );
	}
	SendData( pPlayer, g_Packet, g_nPos );
}

/*
 *	ItemFromWarehouse
 */
void ItemFromWarehouse( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pPlayer || !pItem )
		return;

	REMOVE_FROM_LIST( pPlayer->ch.warehouse, pItem, inven_prev, inven_next );

	//
	PutHeader( dPACKET_WAREHOUSE_DELETEITEM );
	PutInteger( pItem->idxNum );
	SendData( pPlayer, g_Packet, g_nPos );
}

/*
 *	WAREHOUSE_UpdateMoney
 */
void WAREHOUSE_UpdateMoney( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch.btHasBankRow )
	{
		if( g_ItemSql.Query( "update Bank set amount = %d where ownerID = '%s' limit 1", GET_BANK( pPlayer ), g_ItemSql.StringConvert( GET_ID( pPlayer ), escID ) ) )
		{
			log( "WAREHOUSE_UpdateMoney : %s\r\n", g_ItemSql.GetLastError() );
		}
	}
	else
	{
		if( g_ItemSql.Query( "insert into Bank values( '', '%s', %d )", g_ItemSql.StringConvert( GET_ID( pPlayer ), escID ), GET_BANK( pPlayer ) ) )
		{
			log( "WAREHOUSE_UpdateMoney : %s\r\n", g_ItemSql.GetLastError() );
		}
		else
			pPlayer->ch.btHasBankRow = 1;
	}
}


/*
 *	CountWarehouseItem
 */
int CountWarehouseItem( sPDESC_DATA pPlayer )
{
	int itemCnt = 0;

	sPITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->ch.warehouse, item, next_item, inven_next );
	itemCnt++;
	LIST_WHILEEND();

	return itemCnt;
}

//EOF


