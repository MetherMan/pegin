/**
 *	$Workfile: shop.h $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 6 $
 *	$Date: 04-05-11 1:45a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/shop.h $
 * 
 * 6     04-05-11 1:45a Sk8snow
 * 
 * 5     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __SHOP_H__
#define __SHOP_H__

#define dMAX_SHOP		256				// √÷¥Î ªÛ¡° ∞πºˆ

#define IS_VALID_SHOP( num ) ( num < dMAX_SHOP && g_SHOP[num] )


struct shop_item_data
{
	int itemNum;
};

typedef struct shop_item_data sSHOP_ITEM, *sPSHOP_ITEM;


typedef struct
{
	int mapNum;
	int keeperNum;
	BYTE itemCnt;

	CList item;
} sSHOP_INFO, *sPSHOP_INFO;

extern sPSHOP_INFO g_SHOP[dMAX_SHOP];


////////////////////////////////////////////
// F U N C S
////////////////////////////////////////////
void InitShopInfo();
void DestroyShopInfo();
BOOL LoadShopInfo();

void SHOP_SendItemList( sPDESC_DATA pPlayer, WORD shopNum );
void SHOP_SellItem( sPDESC_DATA pPlayer, WORD shopNum, int idxNum , BYTE pos );
void SHOP_BuyItem( sPDESC_DATA pPlayer, WORD shopNum, int itemNum, BYTE cnt );

BOOL FindShopItem( WORD shopNum, int itemNum );
void SHOP_Result( sPDESC_DATA pPlayer, BYTE result );

void InitMyShop( sPDESC_DATA pPlayer );
sPMYSHOP_ITEM NewMyShopItem();
sPMYSHOP_ITEM FindMyShopItem( sPDESC_DATA pPlayer, int idxNum, int itemNum );
sPMYSHOP_ITEM FindMyShopItem( sPDESC_DATA pPlayer, int idxNum );
void ClearMyShop( sPDESC_DATA pPlayer );
int	ITEMCASHCNT( int itemNum );

#endif



//



