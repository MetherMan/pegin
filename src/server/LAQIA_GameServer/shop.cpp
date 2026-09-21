/**
 *	$Workfile: shop.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 38 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/shop.cpp $
 * 
 * 38    04-12-22 5:18p Sk8snow
 * 
 * 37    04-10-19 11:14p Sk8snow
 * 
 * 36    04-09-08 3:54p Sk8snow
 * 
 * 35    04-08-04 9:27a Sk8snow
 * 
 * 34    04-07-13 7:26a Sk8snow
 * 
 * 33    04-05-29 10:12p Sk8snow
 * 
 * 32    04-05-21 11:37a Sk8snow
 * 
 * 31    04-05-18 1:17a Sk8snow
 * 
 * 30    04-05-11 1:45a Sk8snow
 * 
 * 29    04-04-30 9:46a Sk8snow
 * 
 * 28    04-04-01 5:33p Sk8snow
 * 
 * 27    04-03-31 8:22a Sk8snow
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

#include "skill.h"
#include "shop.h"

#include "itemdb.h"
#include "guild.h"


sPSHOP_INFO g_SHOP[dMAX_SHOP];


//
// Function Name: InitShopInfo
// Date: 2003-05-05
// Description: 
//
void InitShopInfo()
{
	for( int i = 0; i < dMAX_SHOP; i++ )
		g_SHOP[i] = NULL;
}

//
// Function Name: DestroyShopInfo
// Date: 2003-05-05
// Description: 
//
void DestroyShopInfo()
{
	for( int i = 0; i < dMAX_SHOP; i++ )
	{
		if( IS_VALID_SHOP( i ) )
		{
			g_SHOP[i]->item.DestroyWithData();
			SAFE_FREE( g_SHOP[i] );
		}
	}
}

//
// Function Name: MakeShopItemCnt
// Date: 2003-05-05
// Description: 
//
void MakeShopItemCnt()
{
	sPLIST_BLOCK list, block, block_next;
	int cnt;

	for( int i = 0; i < dMAX_SHOP; i++ )
	{
		if( IS_VALID_SHOP( i ) )
		{
			list = g_SHOP[i]->item.GetFirstBlock();
			cnt = 0;
			LIST_WHILE( list, block, block_next, next , WMgr );
			cnt++;
			LIST_WHILEEND( list, block, block_next );
			g_SHOP[i]->itemCnt = cnt;
		}
	}
}

//
// Function Name: SendShopItem
// Date: 2003-05-05
// Description: 
//
void SHOP_SendItemList( sPDESC_DATA pPlayer, WORD shopNum )
{
	if( !IS_VALID_SHOP( shopNum ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[51] );
		return;
	}
	if( shopNum >= dMAX_SHOP )
		return;

#ifdef dUSE_AUTO_SHOP
	GET_CURR_OPENSHOP( pPlayer ) = shopNum;
#endif

	if( IS_VALID_GUILD( gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild ) &&
		GET_GUILD( pPlayer ) != gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild )
	{
		/*StrSendSysMsg( pPlayer, 264, 
			GET_GUILD_DATA( gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild )->guildName, 
			gWORLD[GET_MAP_NUM( pPlayer )]->itemTax );*/
		SendSystemMsg( pPlayer, g_LANG_STR[264],
			GET_GUILD_DATA( gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild )->guildName, 
			gWORLD[GET_MAP_NUM( pPlayer )]->itemTax);
	}

	sPLIST_BLOCK list, block, next_block;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SHOPITEM_LIST, g_nPos );
	PutByte( g_Packet, g_SHOP[shopNum]->itemCnt, g_nPos );

	list = g_SHOP[shopNum]->item.GetFirstBlock();
	sPSHOP_ITEM item;

	BYTE page = 0;	BYTE invenX = 0;
	BYTE invenY = 0;


	LIST_WHILE( list, block, next_block, next, WMgr );

	if( page >= 3 )
		break;
		
	item = (sPSHOP_ITEM)block->data;

	PutInteger( g_Packet, item->itemNum, g_nPos );

	PutByte( g_Packet, page, g_nPos );
	PutByte( g_Packet, invenX, g_nPos );
	PutByte( g_Packet, invenY, g_nPos );

	if( invenY == 8 && invenX == 4 )
	{
		page++;
		invenX = 0;
		invenY = 0;
	}
	else if( invenX == 4 )
	{
		invenX = 0;
		invenY++;
	}
	else
		invenX++;

	LIST_WHILEEND( list, block, next_block );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: LoadShopInfo
// Date: 2003-05-05
// Description: 
//
BOOL LoadShopInfo()
{
	FILE *fp = fopen( dSHOPINFO_TABLE_FILE, "rb" );

	if( !fp )
		return 0;

	int shopNum;
	int itemNum;

	sPSHOP_ITEM item;
	
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

		if( sscanf( readBuff, "%d %d", &shopNum, &itemNum ) != 2 )
		{
			log( "Load Shop Info : bad file format\r\n" );
			fclose( fp );
			return 0;
		}

		if( shopNum >= dMAX_SHOP )
		{
			log( "LoadShopInfo : shopNum >= dMAX_SHOP \r\n" );
			fclose(fp);
			return 0;
		}
		
		if( !IS_VALID_SHOP( shopNum ) )
		{
			g_SHOP[shopNum] = (sPSHOP_INFO)malloc(sizeof(sSHOP_INFO));
			
			if( !g_SHOP[shopNum] )
			{
				log( "LoadShopInfo : g_SHOP[shopNum] malloc failed\r\n" );
				fclose( fp );
				return 0;
			}
			g_SHOP[shopNum]->item.Init();
			g_SHOP[shopNum]->itemCnt = 0;
			g_SHOP[shopNum]->keeperNum = 0;
			g_SHOP[shopNum]->mapNum = 0;
		}
		
		item = (sPSHOP_ITEM)malloc(sizeof(sSHOP_ITEM));
		
		if( !item )
		{
			log( "LoadShopInfo : sPSHOP_ITEM data malloc failed\r\n" );
			fclose( fp );
			return 0;
		}
		
		item->itemNum = itemNum;
			
		g_SHOP[shopNum]->item.push( item );
	}

	fclose( fp );
	
	MakeShopItemCnt();

	return 1;
}



//
// Function Name: SHOP_SellItem
// Date: 2003-05-05
// Description: 
//
void SHOP_SellItem( sPDESC_DATA pPlayer, WORD shopNum, int idxNum , BYTE pos )
{
	sPITEM_DATA sellItem;

	switch( pos )
	{
	case 0:	// 소지품
		sellItem = FindItemFromInven( pPlayer, idxNum );

		if( !sellItem )
			return;

	/*	if( GET_RIDE_STATE( pPlayer ) && 
			( sellItem->itemNum == dRIDE_ITEM1 || sellItem->itemNum == dRIDE_ITEM2 || sellItem->itemNum == dRIDE_ITEM3 ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[52] );
			return;
		}*/

		if( IS_SET( GET_ITEM_ATTR( sellItem ), dITEM_ATTR_NOSELL ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[53] );
			return;
		}

#ifdef dDAUM_BILLING
#ifndef IS_TEST_SERVER
		if( IS_BILLING_ITEM( sellItem ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[265] );
			return;
		}
#endif
#endif
		
		if( GET_MONEY( pPlayer ) + GET_ITEM_SELLPRICE( sellItem ) > dMAX_HAVE_MONEY )
			return;

		ItemFromInventory( pPlayer, sellItem );
	
		break;
	case 1: // 장착창 
		sellItem = FindItemFromEquip( pPlayer, idxNum );

		if( !sellItem )
			return;

		if( IS_SET( GET_ITEM_ATTR( sellItem ), dITEM_ATTR_NOSELL ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[53] );
			return;
		}
		
#ifdef dDAUM_BILLING
		if( IS_BILLING_ITEM( sellItem ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[265] );
			return;
		}
#endif

		if( GET_MONEY( pPlayer ) + GET_ITEM_SELLPRICE( sellItem ) > dMAX_HAVE_MONEY )
			return;

		ItemFromEquip( pPlayer, sellItem );

		break;
	default:
		return;
	}

	/*if( GET_ITEM_SELLPRICE( sellItem ) > 1000000 && 
		sellItem->itemNum != dRIDE_ITEM1 && 
		sellItem->itemNum != dRIDE_ITEM2 &&
		sellItem->itemNum != dRIDE_ITEM3 )
	{
		log( "SHOP_SellItem : GET_ITEM_SELLPRICE( sellItem ) > 1000000 [%d/%s/%s]\r\n", GET_ITEM_SELLPRICE( sellItem ), GET_NAME( pPlayer ), GET_ID( pPlayer ) );
	}*/

	if( GET_ITEM_TYPE( sellItem ) == dITEMTYPE_POTION )
	{
		if( GET_ITEM_SELLPRICE( sellItem ) * MIN( sellItem->exVal[0], 1 ) > 1000000 )
			log( "SHOP_SellItem : GET_ITEM_SELLPRICE( sellItem ) * MIN( sellItem->exVal[0], 1 ) > 1000000 [%d/%d/%s/%s]\r\n", GET_ITEM_SELLPRICE( sellItem ) * MIN( sellItem->exVal[0], 1 ), sellItem->itemNum, GET_NAME( pPlayer ), GET_ID( pPlayer ) );

		GET_MONEY( pPlayer ) += GET_ITEM_SELLPRICE( sellItem ) * MIN( sellItem->exVal[0], 1 );
	}
	else
		GET_MONEY( pPlayer ) += GET_ITEM_SELLPRICE( sellItem );

#ifdef dUSE_ITEMLOG
	ITEMLOG_ItemLog( pPlayer, NULL, sellItem, ITEMLOG_ACT_SELLITEM );
#endif

	DATASERV_SendUpdateMoney( pPlayer );
	UpdateMoney( pPlayer );

	INSERT_ITEM_TO_MEMORY( sellItem );
}


//
// Function Name: FindShopItem
// Date: 2003-05-05
// Description: 
//
BOOL FindShopItem( WORD shopNum, int itemNum )
{
	if( shopNum >= dMAX_SHOP )
		return 0;

	if( !IS_VALID_SHOP( shopNum ) )
		return 0;

	sPLIST_BLOCK list, block, next_block;

	list = g_SHOP[shopNum]->item.GetFirstBlock();
	sPSHOP_ITEM item;

	LIST_WHILE( list, block, next_block, next, WMgr );

	item = (sPSHOP_ITEM)block->data;

	if( item->itemNum == itemNum )
		return 1;

	LIST_WHILEEND( list, block, next_block );

	return 0;
}


/*
0 해당 아이템 존재 않함
1 잔액 부족 
2 기타 에러
*/
void SHOP_Result( sPDESC_DATA pPlayer, BYTE result )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_SHOP_RESULT, g_nPos );
	PutByte( g_Packet, result, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: SHOP_BuyItem
// Date: 2003-05-05
// Description: 
//
BOOL SHOP_BuyItemWide( sPDESC_DATA pPlayer, WORD shopNum, int itemNum , WORD cnt )
{
#ifdef dUSE_AUTO_SHOP
	shopNum = GET_CURR_OPENSHOP( pPlayer );
#endif
	BOOL	nCheck = FALSE;
	if( itemNum == 12098 || itemNum == 12097 )
	{
		nCheck = TRUE;
	}
			
	if( nCheck == FALSE )
	{
		if( !FindShopItem( shopNum, itemNum ) )
		{
			SHOP_Result( pPlayer, 0 );
			return 0;
		}
	}

	if( !IS_VALID_ITEM( itemNum ) )
		return 0;

// Reject impossible quantities before allocating an item or charging money.
    const int stackLimit=LaqiaStackLimit(GET_ITEM_TYPE2(itemNum),itemNum);
    if(cnt==0) cnt=1;
    int purchaseCount=1;
    if(GET_ITEM_TYPE2(itemNum)==dITEMTYPE_POTION || GET_ITEM_TYPE2(itemNum)==dITEMTYPE_CONTAINER)
        purchaseCount=ITEMCASHCNT(itemNum)>0?ITEMCASHCNT(itemNum):cnt;
    if(purchaseCount>stackLimit) return 0;
    sPITEM_DATA stackTarget=NULL;
    if(stackLimit>1) for(sPITEM_DATA candidate=pPlayer->inven;candidate;candidate=candidate->i_next)
        if(candidate->itemNum==itemNum && GetItemStackCount(candidate)+purchaseCount<=stackLimit) {stackTarget=candidate;break;}

    if (stackLimit>1 && (cnt<1 || cnt>stackLimit)) return 0;
    if (stackLimit>1 && (long long)GET_ITEM_PRICE2(itemNum)*cnt>2147483647LL) return 0;
#ifdef USE_ITEM_WEIGHT
	if( GET_CURRITEM_WEIGHT( pPlayer ) + ( GET_ITEM_WEIGHT2( itemNum ) * MIN( cnt, 1 ) ) > CalsAllowTotalWeight( pPlayer ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[180] );
		return 0;
	}
#endif

//Cash system Items.
if( itemNum == 11996 ||
	itemNum == 11907 ||
	itemNum == 11908 ||
	itemNum == 11910 ||
	itemNum == 11911 ||
	itemNum == 11968 )






	{
		sCHECK_INVEN checkInven;
		int itemPrice = GET_ITEM_PRICE2( itemNum );
		
		if( !CheckEmptyInven( pPlayer, &checkInven ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[19] );
			return 0;
		}

		if( GET_CASHMONEY( pPlayer ) < itemPrice )
		{
			// 잔액 부족
			SendSystemMsg( pPlayer, "Cash amount is insufficient." );
			return 0;
		}

		sPITEM_DATA buyItem =  CreateItem( itemNum , "SHOP_BuyItem()", __FILE__, __LINE__ );

		if( !buyItem )
		{
			SHOP_Result( pPlayer, 2 );
			return 0;
		}

		GET_CASHMONEY( pPlayer ) -= itemPrice;

		ItemToInventory( pPlayer, buyItem , &checkInven );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, NULL, buyItem, ITEMLOG_ACT_BUYITEM );
#endif

		DATASERV_SendUpdateCash( pPlayer );
		UpdateCash( pPlayer );
		ITEMDB_UpdateCashMoney( pPlayer );
	}

	else
	{

		sCHECK_INVEN checkInven;
		int itemPrice = GET_ITEM_PRICE2( itemNum );
		int itemTax = 0;

		// 바터 마스터리 링 착용시 5% DC 
		if( CheckEquipItem( pPlayer, dEQUIP_RING, dRING_BARTERMASTERY ) )
		{
			itemPrice -= (int)( itemPrice * 0.05 );
		}

		// 세금 추가
		if( IS_VALID_GUILD( gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild ) &&
			GET_GUILD( pPlayer ) != gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild )
		{
			itemTax = (int)( itemPrice * (float)( (float)gWORLD[GET_MAP_NUM( pPlayer )]->itemTax / 100.0F ) );
			itemPrice += itemTax;
		}

		if( !CheckEmptyInven( pPlayer, &checkInven ) && !stackTarget )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[19] );
			return 0;
		}

		if( GET_ITEM_TYPE2( itemNum ) == dITEMTYPE_POTION || GET_ITEM_TYPE2( itemNum ) == dITEMTYPE_CONTAINER )
		{
			if( GET_MONEY( pPlayer ) < ( itemPrice * MIN( cnt, 1 ) ) )
			{
				// 잔액 부족
				SHOP_Result( pPlayer, 1 );
				return 0;
			}
		}
		else if( GET_MONEY( pPlayer ) < itemPrice )
		{
			// 잔액 부족
			SHOP_Result( pPlayer, 1 );
			return 0;
		}

		// 세금 계산
		if( IS_VALID_GUILD( gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild ) &&
			GET_GUILD( pPlayer ) != gWORLD[GET_MAP_NUM( pPlayer )]->ownerGuild )
		{
			gWORLD[GET_MAP_NUM( pPlayer )]->TaxBank += itemTax;

			UpdateCastleTaxBank( GET_MAP_NUM( pPlayer ), gWORLD[GET_MAP_NUM( pPlayer )]->TaxBank );
		}

		if( itemNum == dRIDE_ITEM1 ||
			itemNum == dRIDE_ITEM2 ||
			itemNum == dRIDE_ITEM3 )
		{
			if( FindItemFromInvenByNum( pPlayer, itemNum ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[54] );
				return 0;
			}
		}

		sPITEM_DATA buyItem =  CreateItem( itemNum , "SHOP_BuyItem()", __FILE__, __LINE__ );

		if( !buyItem )
		{
			SHOP_Result( pPlayer, 2 );
			return 0;
		}

		//여기서 캐쉬아이템 체크
		int nCash = ITEMCASHCNT( itemNum );

		if( GET_ITEM_TYPE( buyItem ) == dITEMTYPE_POTION || GET_ITEM_TYPE( buyItem ) == dITEMTYPE_CONTAINER )
		{
			if( nCash > 0 )
			{
				buyItem->exVal[0] = nCash;
				GET_MONEY( pPlayer ) -= itemPrice;
			}

			else
			{
				if( cnt > stackLimit )
					return 0;

				itemPrice = ( itemPrice * MIN( cnt, 1 ) );
				GET_MONEY( pPlayer ) -= itemPrice;

				buyItem->exVal[0] = cnt;
			}
		}
		else
		{
			if( nCash >0 )
			{
				buyItem->exVal[0] = nCash;
			}
			GET_MONEY( pPlayer ) -= itemPrice;
		}
		if(stackTarget) {
            stackTarget->exVal[0]=GetItemStackCount(stackTarget)+purchaseCount;
            SendUpdatePotionCnt(pPlayer,stackTarget);
            INSERT_ITEM_TO_MEMORY(buyItem);
            buyItem=stackTarget;
        } else ItemToInventory( pPlayer, buyItem , &checkInven );

#ifdef dUSE_ITEMLOG
		ITEMLOG_ItemLog( pPlayer, NULL, buyItem, ITEMLOG_ACT_BUYITEM );
#endif

		DATASERV_SendUpdateMoney( pPlayer );
		UpdateMoney( pPlayer );
		SendSystemMsg( pPlayer, "You paid the %d shild as tax.", itemTax );
		SendSystemMsg( pPlayer, "You have paid a total of %d shild to %s items.", itemPrice, GET_ITEM_HNAME2( itemNum ) );
	}
    return 1;
}


///////////////
// 개인상점 
///////////////


//
// Function Name: InitMySHop
// Date: 
// Description: 
//
void InitMyShop( sPDESC_DATA pPlayer )
{
	pPlayer->ch2.myShop.isMyShopOpen = 0;
	pPlayer->ch2.myShop.myShopCashBox = 0;
	pPlayer->ch2.myShop.myShopSellList = NULL;
	*pPlayer->ch2.myShop.myShopName = NULL_STR;
}

//
// Function Name: NewMyShopItem
// Date: 
// Description: 
//
sPMYSHOP_ITEM NewMyShopItem()
{
	sPMYSHOP_ITEM pNewItem = NULL;

	if( gITEMS.myshopitem_memory )
	{
		pNewItem = gITEMS.myshopitem_memory;

		REMOVE_FROM_LIST( gITEMS.myshopitem_memory, pNewItem, prev, next );
	}
	else
	{
		pNewItem = (sPMYSHOP_ITEM)malloc(sizeof(sMYSHOP_ITEM));

		if( !pNewItem )
			return NULL;
	}
	pNewItem->idxNum = 0;
	pNewItem->itemNum = 0;
	pNewItem->price = 0;
	pNewItem->cnt = 0;
	pNewItem->prev = NULL;
	pNewItem->next = NULL;
	
	return pNewItem;
}


//
// Function Name: FindMyShopItem
// Date: 
// Description: 
//
sPMYSHOP_ITEM FindMyShopItem( sPDESC_DATA pPlayer, int idxNum, int itemNum )
{
	sPMYSHOP_ITEM item, next_item;

	LIST_WHILE( pPlayer->ch2.myShop.myShopSellList, item, next_item, next, WMgr );
	if( item->idxNum == idxNum && item->itemNum == itemNum )
		return item;
	LIST_WHILEEND( pPlayer->ch2.myShop.myShopSellList, item, next_item );

	return NULL;
}

sPMYSHOP_ITEM FindMyShopItem( sPDESC_DATA pPlayer, int idxNum )
{
	sPMYSHOP_ITEM item, next_item;

	LIST_WHILE( pPlayer->ch2.myShop.myShopSellList, item, next_item, next, WMgr );
	if( item->idxNum == idxNum )
		return item;
	LIST_WHILEEND( pPlayer->ch2.myShop.myShopSellList, item, next_item );

	return NULL;
}

void ClearMyShop( sPDESC_DATA pPlayer )
{
	if( !pPlayer->ch2.myShop.isMyShopOpen )
		return;

	pPlayer->ch2.myShop.isMyShopOpen = 0;
	
	// 아이템 리스트 정리
	sPMYSHOP_ITEM item, next_item;
	
	LIST_WHILE( pPlayer->ch2.myShop.myShopSellList, item, next_item, next, WMgr );
	REMOVE_FROM_LIST( pPlayer->ch2.myShop.myShopSellList, item, prev, next );
	INSERT_TO_LIST( pPlayer->ch2.myShop.myShopSellList, item, prev, next );
	LIST_WHILEEND( pPlayer->ch2.myShop.myShopSellList, item, next_item );

/*	if( pPlayer->ch2.myShop.myShopCashBox )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[170], pPlayer->ch2.myShop.myShopCashBox );
		GET_MONEY( pPlayer ) += pPlayer->ch2.myShop.myShopCashBox;
		DATASERV_SendUpdateMoney( pPlayer );
		UpdateMoney( pPlayer );
	}*/
	pPlayer->ch2.myShop.myShopCashBox = 0;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MYSHOP_SELLFINISH_OK, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MYSHOP_UPDATEINFO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutByte( g_Packet, 0, g_nPos );
	PutString( g_Packet, "Closed", g_nPos );
	PutSize( g_Packet, g_nPos );
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}


int ITEMCASHCNT( int itemNum )
{
	switch( itemNum )
	{
		//5장짜리들
		case 11916:
		case 11919:
		case 11931:
		case 11998:
		case 12001:
		case 12004:
		case 12007:
		case 12010:
		case 12013:
		case 12019:
		case 12022:
		case 12025:
		case 12028:
			return	5;

		// 10장짜리들
		case 11913:
		case 11917:
		case 11920:
		case 11932:
		case 11949:
		case 11952:
		case 11979:
		case 11982:
		case 11985:
		case 11988:
		case 11999:
		case 12002:
		case 12005:
		case 12008:
		case 12011:
		case 12014:
		case 12020:
		case 12023:
		case 12026:
		case 12029:
			return	10;

		//20장짜리
		case 11914:
		case 11918:
		case 11921:
		case 11922:
		case 11933:
		case 11934:
		case 11950:
		case 11953:
		case 11955:
		case 11958:
		case 11961:
		case 11964:
		case 11970:
		case 11973:
		case 11976:
		case 11980:
		case 11983:
		case 11986:
		case 11989:
		case 12000:
		case 12003:
		case 12006:
		case 12009:
		case 12012:
		case 12015:
		case 12021:
		case 12024:
		case 12027:
		case 12030:
			return	20;

		//40장
		case 11915:
		case 11923:
		case 11935:
		case 11951:
		case 11954:
		case 11956:
		case 11959:
		case 11962:
		case 11965:
		case 11971:
		case 11974:
		case 11977:
		case 11981:
		case 11984:
		case 11987:
		case 11990:
			return	40;

		//40장
		case 11924:
		case 11936:
		case 11957:
		case 11960:
		case 11963:
		case 11966:
		case 11972:
		case 11975:
		case 11978:
			return	80;
	}
	return 0;
}


//


//



void SHOP_BuyItem(sPDESC_DATA p, WORD shop, int item, BYTE count) { SHOP_BuyItemWide(p,shop,item,count); }
