#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "message.h"

#include "player.h"
#include "player_world.h"
#include "item.h"
#include "player.h"
#include "party.h"

#include "deal.h"


/*
 *	DEAL_Init
 */
void DEAL_Init( sPDESC_DATA pPlayer )
{
	pPlayer->ch.dealInven.Init();
	pPlayer->ch.pDealVict = NULL;
	GET_DEAL_MONEY( pPlayer ) = 0;
	pPlayer->ch.dealOk = 0;
}

/*
 *	DEAL_Clear
 */
void DEAL_Clear( sPDESC_DATA pPlayer )
{
	pPlayer->ch.dealInven.Destroy();
	pPlayer->ch.pDealVict = NULL;
	GET_DEAL_MONEY( pPlayer ) = 0;
	pPlayer->ch.dealOk = 0;
}

/*
 *	DEAL_Cancel
 */
void DEAL_Cancel( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch.pDealVict )
	{
		PutHeader( dPACKET_DEAL_FINISH );
		SendData( pPlayer, g_Packet, g_nPos );
		SendData( pPlayer->ch.pDealVict, g_Packet, g_nPos );
		
		MakeSystemMsg( "거래가 취소되었습니다." );
		SendData( pPlayer, g_Packet, g_nPos );
		SendData( pPlayer->ch.pDealVict, g_Packet, g_nPos );

		// 
		pPlayer->ch.pDealVict->ch.pDealVict = NULL;
		pPlayer->ch.pDealVict->ch.dealInven.Destroy();
		GET_DEAL_MONEY( pPlayer->ch.pDealVict ) = 0;
		pPlayer->ch.pDealVict->ch.dealOk = 0;

		pPlayer->ch.pDealVict = NULL;
		pPlayer->ch.dealInven.Destroy();
		GET_DEAL_MONEY( pPlayer ) = 0;
		pPlayer->ch.dealOk = 0;
	}
}

/*
 *	DEAL_Finish
 */
void DEAL_Finish( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch.pDealVict )
	{
		PutHeader( dPACKET_DEAL_FINISH );
		SendData( pPlayer, g_Packet, g_nPos );
		SendData( pPlayer->ch.pDealVict, g_Packet, g_nPos );

		MakeSystemMsg( "거래가 종료되었습니다." );
		SendData( pPlayer, g_Packet, g_nPos );
		SendData( pPlayer->ch.pDealVict, g_Packet, g_nPos );

		//
		pPlayer->ch.pDealVict->ch.pDealVict = NULL;
		pPlayer->ch.pDealVict->ch.dealInven.Destroy();
		GET_DEAL_MONEY( pPlayer->ch.pDealVict ) = 0;
		pPlayer->ch.pDealVict->ch.dealOk = 0;

		pPlayer->ch.pDealVict = NULL;
		pPlayer->ch.dealInven.Destroy();
		GET_DEAL_MONEY( pPlayer ) = 0;
		pPlayer->ch.dealOk = 0;
	}
}

/*
 *	CheckInvalidDealItem
 */
BOOL CheckInvalidDealItem( sPDESC_DATA pPlayer )
{
	sPLIST_BLOCK list, block, next_block;
	sPITEM_DATA pDealItem;
	sPITEM_DATA pTmpItem;

	list = pPlayer->ch.dealInven.GetFirstBlock();

	LIST_WHILE( list, block, next_block, next );
	pDealItem = (sPITEM_DATA)block->data;

	pTmpItem = FindItemByIdx( pPlayer, pDealItem->invenPos, pDealItem->idxNum );

	if( !pTmpItem )
		return 0;

	LIST_WHILEEND();
	return 1;
}

/*
 *	FindItemFromDeal
 */
sPITEM_DATA FindItemFromDeal( sPDESC_DATA pPlayer, int idxNum )
{
	sPLIST_BLOCK list, block, block_next;
	sPITEM_DATA pItem;

	list = pPlayer->ch.dealInven.GetFirstBlock();

	LIST_WHILE( list, block, block_next, next );
	pItem = (sPITEM_DATA)block->data;
	if( pItem->idxNum == idxNum )
		return pItem;
	LIST_WHILEEND();
	return NULL;
}

/*
 *	MYSHOP_Init
 */
void MYSHOP_Init( sPDESC_DATA pPlayer )
{
	pPlayer->ch.myshopOpen = 0;
	pPlayer->ch.myshopItem.Init();
	*pPlayer->ch.myshopTitle = NULL_STR;
}

/*
 *	MYSHOP_Close	
 */
void MYSHOP_Close( sPDESC_DATA pPlayer )
{
	PutHeader( dPACKET_MYSHOP_CLOSE );
	PutInteger( GET_IDX( pPlayer ) );
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );

	pPlayer->ch.myshopOpen = 0;
	pPlayer->ch.myshopItem.Destroy();
	*pPlayer->ch.myshopTitle = NULL_STR;
}

/*
 *	MYSHOP_SendItemList
 */
void MYSHOP_SendItemList( sPDESC_DATA pPlayer, sPDESC_DATA pShopKeeper )
{
	if( !pPlayer || !pShopKeeper )
		return;

	if( !pShopKeeper->ch.myshopOpen )
		return;

	PutHeader( dPACKET_MYSHOP_SENDITEMLIST );
	PutInteger( GET_IDX( pShopKeeper ) );
	PutInteger( pShopKeeper->ch.myshopItem.GetSize() );

	sPLIST_BLOCK list, block, next_block;
	sPITEM_DATA pItem;

	list = pShopKeeper->ch.myshopItem.GetFirstBlock();

	LIST_WHILE( list, block, next_block, next );
	pItem = (sPITEM_DATA)block->data;

	PutInteger( pItem->idxNum );
	PutInteger( pItem->itemNum );
	PutString( pItem->name );
	PutString( pItem->maker );
	PutInteger( pItem->sellCnt );
	for( i = 0; i < dITEM_EXVALUE; i++ )
	{
		PutInteger( pItem->exAttr[i] );
		PutInteger( pItem->exValue[i] );
	}
	PutInteger( pItem->sellPrice );
	LIST_WHILEEND();

	SendData( pPlayer, g_Packet, g_nPos );
}




//EOF


