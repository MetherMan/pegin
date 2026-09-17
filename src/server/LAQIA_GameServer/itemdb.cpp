/**
 *	$Workfile: itemdb.cpp $
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
 *	$Log: /GameServer/itemdb.cpp $
 * 
 * 21    04-09-08 3:54p Sk8snow
 * 
 * 20    04-07-13 7:26a Sk8snow
 * 
 * 19    04-05-29 10:12p Sk8snow
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

#include "player.h"
#include "world.h"

#include "monster.h"
#include "item.h"
#include "db_rows.h"
#include "itemdb.h"

#include "skill.h"


//
// Function Name: ITEMDB_Query
// Date: 2003-05-08
// Description: 
//
BOOL ITEMDB_Query( char *query )
{
	if( mysql_query( g_MYSQL, query ) )
	{
		log( "ITEMDB_Query : [%s]\r\n", mysql_error(g_MYSQL) );
		return 0;
	}
	return 1;
}

//
// Function Name: ITEMDB_DeleteAllEquip
// Date: 2003-04-01
// Description: 해당 아이디의 장착 아이템 모두 지우기
//
void ITEMDB_DeleteAllEquip( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	sprintf( query, "delete from EquipItems where ownerID = '%s' and ownerPos = %d",
		escID, pPlayer->ch.charPos );

	ITEMDB_Query( query );
}

//
// Function Name: ITEMDB_DeleteAllInven
// Date: 2003-04-01
// Description: 해당 아이디의 소지품 아이템 모두 지우기
//
void ITEMDB_DeleteAllInven( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from InvenItems where ownerID = '%s' and ownerPos = %d",
		escID, pPlayer->ch.charPos );

	ITEMDB_Query( query );
}


//
// Function Name: ITEMDB_DelEquip
// Date: 2003-04-01
// Description: 
//
void ITEMDB_DelEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from EquipItems where ownerID = '%s' and ownerPos = %d and equipPos = %d",
		escID, 
		pPlayer->ch.charPos, 
		pItem->equipWhere );

	ITEMDB_Query( query );
}

//
// Function Name: ITEMDB_DelAllSkills
// Date: 
// Description: 
//
void ITEMDB_DelAllSkills( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from UserSkills where ownerId = '%s' and ownerPos = %d",
		escID, GET_CHAR_POS( pPlayer ) );

	ITEMDB_Query( query );
}


//
// Function Name: ITEMDB_DelInven
// Date: 
// Description: 
//
void ITEMDB_DelInven( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from InvenItems where ownerID = '%s' and ownerPos = %d and invenPage = %d and "
		"invenX = %d and invenY = %d", escID, pPlayer->ch.charPos, pItem->invenPage, pItem->invenX,
		pItem->invenY );

	ITEMDB_Query( query );
}


//
// Function Name: ITEMDB_ToInven
// Date: 2003-04-01
// Description: 
//
/*
#define INVEN_IDX					0
#define INVEN_ITEMNUM				1
#define INVEN_PAGE					2
#define INVEN_X						3
#define INVEN_Y						4
#define INVEN_OWNERID				5
#define INVEN_OWNERPOS				6
#define INVEN_LIFE					7
*/
void ITEMDB_ToInven( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

#ifdef USE_ITEM_SERIAL
	sprintf( query, "insert into InvenItems values( '', %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, '%s' )",
		pItem->itemNum, pItem->invenPage, pItem->invenX, pItem->invenY, 
		escID, pPlayer->ch.charPos, pItem->life ,
		pItem->exVal[0], pItem->exVal[1], pItem->exVal[2],
		pItem->serial );
#else
	sprintf( query, "insert into InvenItems values( '', %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d )",
		pItem->itemNum, pItem->invenPage, pItem->invenX, pItem->invenY, 
		escID, pPlayer->ch.charPos, pItem->life ,
		pItem->exVal[0], pItem->exVal[1], pItem->exVal[2] );
#endif
	ITEMDB_Query( query );
}

//
// Function Name: ITEMDB_ToInven2
// Date: 
// Description: 
//
void ITEMDB_ToInven2( sPDESC_DATA pPlayer, int itemNum, sPCHECK_INVEN inven )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

#ifdef USE_ITEM_SERIAL
	sprintf( query, "insert into InvenItems values( '', %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, '%s' )",
		itemNum, inven->page, inven->x, inven->y, 
		escID, pPlayer->ch.charPos, 0 ,
		0, 0, 0, GetItemSerial() );
#else
	sprintf( query, "insert into InvenItems values( '', %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d )",
		itemNum, inven->page, inven->x, inven->y, 
		escID, pPlayer->ch.charPos, 0 ,
		0, 0, 0 );
#endif

	ITEMDB_Query( query );
}

//
// Function Name: ITEMDB_UpdateInvenItem
// Date: 
// Description: 
//
void ITEMDB_UpdateInvenItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "update InvenItems set "
		"life = %d,"
		"exValue1 = %d,"
		"exValue2 = %d,"
		"exValue3 = %d "
		"where ownerID = '%s' and "
		"ownerPos = %d and "
		"invenPage = %d and "
		"invenX = %d and "
		"invenY = %d",
		pItem->life, 
		pItem->exVal[0],
		pItem->exVal[1],
		pItem->exVal[2],
		escID, 
		GET_CHAR_POS( pPlayer ),
		pItem->invenPage,
		pItem->invenX,
		pItem->invenY );

	ITEMDB_Query( query );
}




//
// Function Name: ITEMDB_ToEquip
// Date: 2003-04-01
// Description: 
//
/*
#define EQ_IDX						0
#define EQ_ITEMNUM					1
#define EQ_POS						2
#define EQ_LIFE						3
#define EQ_OWNERID					4
#define EQ_OWNERPOS					5
*/
void ITEMDB_ToEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

#ifdef USE_ITEM_SERIAL
	sprintf( query, "insert into EquipItems values( '', %d, %d, %d, '%s', %d, '%s' )",
		pItem->itemNum, 
		pItem->equipWhere, 
		pItem->life, 
		escID, 
		pPlayer->ch.charPos,
		pItem->serial );
#else
	sprintf( query, "insert into EquipItems values( '', %d, %d, %d, '%s', %d )",
		pItem->itemNum, 
		pItem->equipWhere, 
		pItem->life, 
		escID, 
		pPlayer->ch.charPos );
#endif

	ITEMDB_Query( query );
}

//
// Function Name: ITEMDB_ToEquip2
// Date: 
// Description: 
//
void ITEMDB_ToEquip2( char *id, BYTE charPos, WORD itemNum , BYTE equipWhere )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, id, strlen( id ) );

#ifdef USE_ITEM_SERIAL
	sprintf( query, "insert into EquipItems values( '', %d, %d, %d, '%s', %d, '%s' )",
		itemNum, 
		equipWhere, 
		0, 
		id, 
		charPos,
		GetItemSerial() );
#else
	sprintf( query, "insert into EquipItems values( '', %d, %d, %d, '%s', %d )",
		itemNum, 
		equipWhere, 
		0, 
		id, 
		charPos );
#endif

	ITEMDB_Query( query );
}

//
// Function Name: ITEMDB_ToGarbage
// Date: 
// Description: 
//
void ITEMDB_ToGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

#ifdef USE_ITEM_SERIAL
	sprintf( query, "insert into UserGarbage values( '', '%s', %d, %d, %d, %d, %d, '%s' )",
				escID, pItem->itemNum, pItem->life, pItem->exVal[0], pItem->exVal[1], pItem->exVal[2], pItem->serial );
#else
	sprintf( query, "insert into UserGarbage values( '', '%s', %d, %d, %d, %d, %d )",
				escID, pItem->itemNum, pItem->life, pItem->exVal[0], pItem->exVal[1], pItem->exVal[2] );
#endif
	ITEMDB_Query( query );
}

//
// Function Name: ITEMDB_DelGarbage
// Date: 
// Description: 
//
void ITEMDB_DelGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from UserGarbage where "
		"ownerID = '%s' and "
		"itemNum = %d and "
		"exValue1 = %d and "
		"exValue2 = %d and "
		"exValue3 = %d limit 1",
		escID,
		pItem->itemNum,
		pItem->exVal[0],
		pItem->exVal[1],
		pItem->exVal[2] );

	ITEMDB_Query( query );
}


//
// Function Name: ITEMDB_GetPlayerItem
// Date: 2003-04-02
// Description: 
//
BOOL ITEMDB_GetPlayerItem( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	////////////////////////////////////////
	// I N V E N T O R Y 
	////////////////////////////////////////
	sprintf( query, "select * from InvenItems where ownerID = '%s' and ownerPos = %d ", escID, GET_CHAR_POS( pPlayer ) );

	if( !ITEMDB_Query( query ) )
		return 0;
	
	MYSQL_RES *result;
	MYSQL_ROW row;

	result = mysql_store_result( g_MYSQL );
	
	/////////////////////////////////////
	// I N V E N T O R Y 
	/////////////////////////////////////
	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			sPITEM_DATA pItem = CreateItem( atoi( row[INVEN_ITEMNUM] ) , "ITEMDB_GetPlayerItem() - inven", __FILE__ , __LINE__ );

			if( !pItem )
			{
				mysql_free_result( result );
				return 0;
			}
			pPlayer->invenIdx++;
			pItem->idxNum = pPlayer->invenIdx;
			pItem->life = atoi( row[INVEN_LIFE] );
			pItem->invenPage = atoi( row[INVEN_PAGE] );
			pItem->invenX = atoi( row[INVEN_X] );
			pItem->invenY = atoi( row[INVEN_Y] );
			pItem->exVal[0] = atoi( row[INVEN_EXVALUE1] );
			pItem->exVal[1] = atoi( row[INVEN_EXVALUE2] );
			pItem->exVal[2] = atoi( row[INVEN_EXVALUE3] );

#ifdef USE_ITEM_SERIAL
			strcpy( pItem->serial, row[INVEN_SERIAL] );
#endif

			FillInvenArray( pPlayer, pItem->invenPage, pItem->invenX, pItem->invenY );

			INSERT_TO_LIST( pPlayer->inven, pItem, i_prev, i_next );

			if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_SCROLL )
				CheckItemValue( pPlayer, pItem );

#ifdef USE_ITEM_WEIGHT
			AddCurrItemWeight( pPlayer, pItem );
#endif
		}
		mysql_free_result( result );
	}

	////////////////////////////////////////
	// E Q U I P M E N T
	////////////////////////////////////////
	sprintf( query, "select * from EquipItems where ownerID = '%s' and ownerPos = %d",
		escID, pPlayer->ch.charPos );

	if( !ITEMDB_Query( query ) )
		return 0;

	result = mysql_store_result( g_MYSQL );
	
	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			sPITEM_DATA pItem = CreateItem( atoi( row[EQ_ITEMNUM] ), "ITEMDB_GetPlayerItem() - Equip", __FILE__, __LINE__ );
			
			if( !pItem )
			{
				mysql_free_result( result );
				return 0;
			}

			pPlayer->invenIdx++;
			pItem->idxNum = pPlayer->invenIdx;
			pItem->itemNum = atoi( row[EQ_ITEMNUM] );
			pItem->life = atoi( row[EQ_LIFE] );
			pItem->equipWhere = atoi( row[EQ_POS] );

#ifdef USE_ITEM_SERIAL
			strcpy( pItem->serial, row[EQ_SERIAL] );
#endif

			BYTE equipWhere = GetEquipPositionByNum( pItem->itemNum );

			if( equipWhere == 255 )
				log( "ITEMDB_GetPlayerItem : equipWhere == 255 \r\n" );

			if( equipWhere != pItem->equipWhere )
			{
				log( "ITEMDB_GetPlayeItem : equipWhere != pItem->equipWhere [%s/%d] [%s/%d/%d]\r\n", 
				pPlayer->ch.id, pPlayer->ch.charPos ,
				GET_ITEM_HNAME( pItem ), equipWhere, pItem->equipWhere );
			}

			if( equipWhere >= dMAX_EQUIP )
			{
				log( "ITEMDB_GetPlayerItem : error [%s/%d] BAD ITEM\r\n", pPlayer->ch.id, pPlayer->ch.charPos );				

				mysql_free_result( result );
				
				INSERT_ITEM_TO_MEMORY( pItem );
				
				return 0;
			}

			if( pPlayer->equip[equipWhere] )
			{
				log( "ITEMDB_GetPlayerItem : error [%s/%d][pPlayer->equip[equipWhere]][%s/%s]\r\n", 
					pPlayer->ch.id, pPlayer->ch.charPos , 
					GET_ITEM_HNAME( pItem ), GET_ITEM_HNAME( pPlayer->equip[equipWhere] ));
				mysql_free_result( result );

				INSERT_ITEM_TO_MEMORY( pItem );
				
				return 0;
			}
			pPlayer->equip[equipWhere] = pItem;
			pItem->equipWhere = equipWhere;

#ifdef USE_ITEM_WEIGHT
			AddCurrItemWeight( pPlayer, pItem );
#endif

			// HP ADD
			if( GET_ITEM_PLUSEHP( pItem ) )
				GET_HPADD( pPlayer ) += GET_ITEM_PLUSEHP( pItem );
			// MP ADD
			if( GET_ITEM_PLUSEMP( pItem ) )
				GET_MPADD( pPlayer ) += GET_ITEM_PLUSEMP( pItem );
			
			// STR ADD
			if( GET_ITEM_PLUSESTR( pItem ) )
				GET_STRADD_EX( pPlayer ) += GET_ITEM_PLUSESTR( pItem );
			// DEX ADD
			if( GET_ITEM_PLUSEDEX( pItem ) )
				GET_DEXADD_EX( pPlayer ) += GET_ITEM_PLUSEDEX( pItem );
			// INT ADD
			if( GET_ITEM_PLUSEINT( pItem ) )
				GET_INTADD_EX( pPlayer ) += GET_ITEM_PLUSEINT( pItem );
		}
		mysql_free_result( result );
	}



	////////////////////////////////////////
	// G A R B A G E
	////////////////////////////////////////
	sprintf( query, "select * from UserGarbage where ownerID = '%s'", escID );
	
	if( !ITEMDB_Query( query ) )
		return 0;
	
	result = mysql_store_result( g_MYSQL );
	
	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			sPITEM_DATA pItem = CreateItem( atoi( row[GARBAGE_ITEMNUM] ), "ITEMDB_GetPlayerItem() - Garbage", __FILE__, __LINE__ );
			
			if( !pItem )
			{
				log( "ITEMDB_GetPlayerItem : pItem malloc failed! [GARBAGE]\r\n" );
				mysql_free_result( result );
				return 0;
			}
			
			pPlayer->invenIdx++;
			pItem->idxNum = pPlayer->invenIdx;
			pItem->itemNum = atoi( row[GARBAGE_ITEMNUM] );
			pItem->life = atoi( row[GARBAGE_LIFE] );
			pItem->exVal[0] = atoi( row[GARBAGE_EXVALUE1] );
			pItem->exVal[1] = atoi( row[GARBAGE_EXVALUE2] );
			pItem->exVal[2] = atoi( row[GARBAGE_EXVALUE3] );

#ifdef USE_ITEM_SERIAL
			strcpy( pItem->serial, row[GARBAGE_SERIAL] );
#endif
			
			INSERT_TO_LIST( pPlayer->garbage, pItem, i_prev, i_next );

			pPlayer->garbageCnt++;
		}
		mysql_free_result( result );
	}

#ifdef USE_GARBAGE_MONEY
	sprintf( query, "select * from UserGarbageMoney where ownerID = '%s' LIMIT 1", escID );

	if( !ITEMDB_Query( query ) )
		return 0;

	result = mysql_store_result( g_MYSQL );
	
	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			pPlayer->useGarbageMoney = 1;
			pPlayer->garbageMoney = atol( row[GARBAGE_MONEY_AMOUNT] );
		}
		mysql_free_result( result );
	}
#endif

	sprintf( query, "select * from UserCashMoney where ownerID = '%s' LIMIT 1", escID );

	if( !ITEMDB_Query( query ) )
		return 0;

	result = mysql_store_result( g_MYSQL );

	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			pPlayer->useCashMoney = 1;
			pPlayer->ch.cash = atol( row[GARBAGE_MONEY_AMOUNT] );
		}
		mysql_free_result( result );
	}

	return 1;
}



//
// Function Name: ITEMDB_DeleteAllItem
// Date: 
// Description: 
//
void ITEMDB_DeleteAllItem( int itemNum )
{
	char query[1024];

	sprintf( query, "delete from InvenItems where itemNum = %d", itemNum );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );

	sprintf( query, "delete from EquipItems where itemNum = %d", itemNum );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );

	sprintf( query, "delete from UserGarbage where itemNum = %d", itemNum );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );

	log( "DeleteAllItem : Done [ %d / %s ] \r\n", itemNum , g_ItemInfo[itemNum]->hName );
}

void ITEMDB_UpdateGarbageMoney( sPDESC_DATA pPlayer )
{
	char query[1024];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) ); 

	if( pPlayer->useGarbageMoney )
	{
		sprintf( query, "update UserGarbageMoney set amount = %d where ownerID = '%s' limit 1", pPlayer->garbageMoney, escID ); 
	}
	else
	{
		pPlayer->useGarbageMoney = 1;

		sprintf( query, "insert into UserGarbageMoney values( '', '%s', %d )", escID, pPlayer->garbageMoney );
	}
	ITEMDB_Query( query );
}

void ITEMDB_UpdateCashMoney( sPDESC_DATA pPlayer )
{
	char query[1024];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) ); 

	if( pPlayer->useCashMoney )
	{
		sprintf( query, "update UserCashMoney set amount = %d where ownerID = '%s' limit 1", GET_CASHMONEY( pPlayer), escID ); 
	}
	else
	{
		pPlayer->useCashMoney = 1;

		sprintf( query, "insert into UserCashMoney values( '', '%s', %d )", escID, GET_CASHMONEY( pPlayer) );
	}
	ITEMDB_Query( query );
}

//////////////////////////////////////////////////////////////
// LOG 
//////////////////////////////////////////////////////////////


//
// Function Name: ITEMLOG_Query
// Date: 
// Description: 
//
BOOL ITEMLOG_Query( char *query )
{
	if( mysql_query( g_LOGSQL, query ) )
	{
		log( "ITEMLOG_Query : [%s]\r\n", mysql_error(g_LOGSQL) );
		return 0;
	}
	return 1;
}


//
// Function Name: ITEMLOG_ItemLog
// Date: 
// Description: 
//
void ITEMLOG_ItemLog( sPDESC_DATA pPlayer, sPDESC_DATA pActionPlayer, sPITEM_DATA pItem , BYTE action )
{
#ifdef dDAUM_BILLING
	if( !IS_BILLING_ITEM( pItem ) )
	{
#endif
		if( ( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION ) || 
			( !GET_ITEM_UNIQ( pItem ) && !GET_ITEM_NEEDLOG( pItem ) ) )
			return;
#ifdef dDAUM_BILLING
	}
#endif

	char query[dMAX_QUERY_SIZE];
	char escID[128];
	char escActionID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	if( pActionPlayer )
		mysql_escape_string( escActionID, GET_ID( pActionPlayer ), strlen( GET_ID( pActionPlayer ) ) );

#ifdef USE_ITEM_SERIAL
	sprintf( query, "insert into ItemLog values( '', '%s', %d, %d, %d, '%s', %d, %ld, '%s' )",
		escID, GET_CHAR_POS( pPlayer ), pItem->itemNum, action, 
		pActionPlayer ? escActionID : "", pActionPlayer ? GET_CHAR_POS( pActionPlayer ) : 0,
		g_CurrRealTime, pItem->serial );
#else
	sprintf( query, "insert into ItemLog values( '', '%s', %d, %d, %d, '%s', %d, %ld )",
		escID, GET_CHAR_POS( pPlayer ), pItem->itemNum, action, 
		pActionPlayer ? escActionID : "", pActionPlayer ? GET_CHAR_POS( pActionPlayer ) : 0,
		g_CurrRealTime );
#endif

	ITEMLOG_Query( query );
}

//
// Function Name: ITEMLOG_GarbageLog
// Date: 
// Description: 
//
void ITEMLOG_GarbageLog( sPDESC_DATA pPlayer, sPITEM_DATA pItem, BYTE action )
{
	if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION || 
		( !GET_ITEM_UNIQ( pItem ) && !GET_ITEM_NEEDLOG( pItem ) ) )
		return;

	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "insert into GarbageLog values( '', '%s', %d, %d, %d, %ld, '%s' )",
		escID, GET_CHAR_POS( pPlayer ), pItem->itemNum, action, g_CurrRealTime, pItem->serial );

	ITEMLOG_Query( query );
}

//
// Function Name: ITEMLOG_MoneyLog
// Date: 
// Description: 
//
void ITEMLOG_MoneyLog( sPDESC_DATA pPlayer, sPDESC_DATA pActionPlayer, int amount, BYTE action )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "insert into MoneyLog values( '', '%s', %d, %d, %d, '%s', %d, %ld )",
		escID, GET_CHAR_POS( pPlayer ), amount, action, 
		pActionPlayer ? GET_ID( pActionPlayer ) : "", 
		pActionPlayer ? GET_CHAR_POS( pActionPlayer ) : 0, 
		g_CurrRealTime );

	ITEMLOG_Query( query );
}

void ITEMLOG_GarbageMoneyLog( sPDESC_DATA pPlayer, int amount, BYTE action )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "insert into GarbageMoneyLog values( '', '%s', %d, %d, %ld )",
		escID, amount, action, g_CurrRealTime );

	ITEMLOG_Query( query );
}


//
// Function Name: LOGINLOG_Write
// Date: 
// Description: 
//
#ifdef dUSE_LOGIN_LOG
void LOGINLOG_Write( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	char escName[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escName, GET_NAME( pPlayer ), strlen( GET_NAME( pPlayer ) ) );

	sprintf( query, "insert into LoginLog values( '', '%s', '%s', '%s', %ld )", 
		escID, escName, pPlayer->ip, g_CurrRealTime );

	ITEMLOG_Query( query );
}
#endif


//
// Function Name: ITEMLOG_MyShopLog
// Date: 
// Description: 
//
void ITEMLOG_MyShopLog( sPDESC_DATA pPlayer, sPDESC_DATA pActionPlayer, sPITEM_DATA pItem, int price )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	char escActionID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escActionID, GET_ID( pActionPlayer ), strlen( GET_ID( pActionPlayer ) ) );

#ifdef USE_ITEM_SERIAL
	sprintf( query, "insert into MyShopLog values( '', '%s', %d, '%s', %d, %d, %d, %ld, '%s' )",
		escID, GET_CHAR_POS( pPlayer ), escActionID, GET_CHAR_POS( pActionPlayer ), pItem->itemNum, price, g_CurrRealTime, pItem->serial );
#else
	sprintf( query, "insert into MyShopLog values( '', '%s', %d, '%s', %d, %d, %d, %ld )",
		escID, GET_CHAR_POS( pPlayer ), escActionID, GET_CHAR_POS( pActionPlayer ), pItem->itemNum, price, g_CurrRealTime );
#endif

	ITEMLOG_Query( query );
}


//
// Function Name: ITEMLOG_BillItemLog
// Date: 
// Description: 
//
void ITEMLOG_BillItemLog( sPDESC_DATA pPlayer, int itemNum, BYTE type )
{
#ifndef BILLITEM_LOG
	return;
#endif
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "insert into BillItemLog values( '', '%s', %d, %d, %d, %ld )", escID, GET_CHAR_POS( pPlayer ), itemNum, type, g_CurrRealTime );

	if( mysql_query( g_LOGSQL, query ) )
	{
		log( "%s",mysql_error( g_LOGSQL ) );
		log( "\r\n" );
	}
}

//



