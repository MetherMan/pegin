/**
 * 유저 인터페이스 상점모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-09-24 4:46p $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /UInterface/Src/W3DUIShop.cpp $
 * 
 * 10    04-09-24 4:46p Paran
 * 
 * 9     04-08-18 5:34p Paran
 * 
 * 8     04-07-16 3:08p Paran
 * 
 * 7     04-07-03 3:38p Paran
 * 
 * 6     04-06-26 8:59p Paran2
 * 
 * 5     04-05-29 10:07p Paran
 * 
 * 4     04-05-21 10:38a Paran
 * 
 * 3     04-05-14 4:31p Paran
 * 
 * 2     04-05-10 11:24p Paran
 * 
 * <HR>
 *
 * @file	W3DUIShop.cpp
 */

#include "UInterface_Export.h"
#include "W3DUInterface.h"

/**
 * 가격을 입력 받는다
 */
void CW3DUInterface::InputPrice( const char* szRequestComp )
{
	static char		s_szRequestComp[32];
	if( szRequestComp )
	{
		strcpy( s_szRequestComp, szRequestComp );

		m_pCurrEditor = reinterpret_cast<UIEditText*>( GetComponent( "EDT_GOLD_INPUT" ) );
		Assert( m_pCurrEditor );
		m_pCurrEditor->Cleanup();
		EditBox_LDown( m_pCurrEditor );
		ShowDlg( "DLG_GOLD", TRUE );
	}
	else
	{
		char*		szGold = const_cast<char*>( GetText( "EDT_GOLD_INPUT" ) );
		if( !szGold )
		{
			return;
		} //if

		int		nLength = strlen( szGold );
		for( int nCnt = 0; nCnt < nLength; nCnt++ )
		{
			if( szGold[nCnt] < 0x30 || szGold[nCnt] > 0x39 )
			{
				ChattingInput( "", STR_SYSMSG_EXCEPTSTRING, FONT_MSGCOLOR, 0, 0 );
				return;
			} //if
		} //for

		int		nGold = atoi( szGold );
		if( nLength > 9 ||
			nGold > 100000000 )
		{
			ChattingInput( "", STR_UIMSG_ONCEINPUTPRICE, FONT_MSGCOLOR, 0, 0 );
			return;
		} //if
		

		if( !strcmp( s_szRequestComp, "INVENTORY_MYSHOP" ) &&
			m_pBuyItemData )
		{ // 내 개인 상점에 물품등록
			m_pBuyItemData->nSellPrice = nGold;

			PushEvent(
				"UI_INVENTORYIN",
				EVENT_MYSHOP_ADDITEM,
				m_pBuyItemData->nId,
				nGold );
		}
		else if( !strcmp( s_szRequestComp, "BTN_STORAGE_GOLD" ) )
		{ // Storage
			PushEvent( "STORAGEGOLD", EVENT_STORAGE_GETGOLD, 0, nGold );
		}

		//통합서버 창고의 돈을 입력한다~
		else
		{
			if( nGold <= 0 )
			{
				return;
			} //if

			if( nGold > m_nGold )
			{
				MessageBox( STR_INVENERR_NOMATCHGOLD, "EMPTYGOLD", MSG_OK );
				return;
			} //if

			if( IsShowDlg( "DLG_DEAL" ) )
			{ //돈을 유저거래에 올린다.
				SetText( "TXT_DEAL_GOLD1", GetUnitNum( nGold ) );
				PushEvent( "UI_USERDEAL", EVENT_DEAL_ADDMONEY, nGold, 0 );
			}
			else if( IsShowDlg( "DLG_STORAGE" ) )
			{
				PushEvent( "STORAGEGOLD", EVENT_STORAGE_SAVEGOLD, 0, nGold );
			}
			else
			{
				///< todo : 돈을 버리는 처리..
			} //if..else..
		} //if..ese
	} //if..else
} //CW3DUInterface::InputPrice

/**
 * 가격을 입력 받는다
 */
void CW3DUInterface::BuyItem( const char* szRequestComp )
{
	static char		s_szRequestComp[32];

	if( !m_pBuyItemData )
	{
		Assert( !"It NULL The BuyItem" );
		return;
	} //if

	if( szRequestComp )
	{
		strcpy( s_szRequestComp, szRequestComp );

		if( !InventoryBlank() )
		{ // 여유공간 확인.
			MessageBox( STR_INVENERR_NOTENOUGHSPACE, "NOSPACE", MSG_OK );
			return;
		} //if
		
		SItemInfoData*		pItemInfo = m_pBuyItemData->pItemInfoData;
		Assert( pItemInfo );
		if( pItemInfo )
		{
			if( !strcmp( s_szRequestComp, "DLG_PERSONALSHOP" ) )
			{
				if( m_pBuyItemData->nSellPrice > m_nGold )
				{
					MessageBox( STR_INVENERR_NOTENOUGHGOLD, "NOMONEY", MSG_OK );
					return;
				} //if
			}
			else
			{
				//if( m_pBuyItemData->nId <= 12030 && m_pBuyItemData->nId >= ITEM_WOARAIM13_10H )
				if( m_pBuyItemData->nId >= 12102 ||
					m_pBuyItemData->nId == 11997 ||
					m_pBuyItemData->nId == 11996 )
				{
					if( pItemInfo->nPrice > m_nCash )	
					{
						MessageBox( "Not enough Cash!", "NOCASH", MSG_OK );
						return;
					}
				}
				else 
				{
					if( pItemInfo->nPrice > m_nGold )
					{
						MessageBox( STR_INVENERR_NOTENOUGHGOLD, "NOMONEY", MSG_OK );
						return;
					} //if
				}
			} //if..else

			SetPicture( "PIC_DEALBUY_ICON", m_pBuyItemData->pimgItem );
			int			nPrice;
			if( !strcmp( szRequestComp, "DLG_PERSONALSHOP" ) )	nPrice = m_pBuyItemData->nSellPrice;
			else												nPrice = pItemInfo->nPrice;
			SetText( "TXT_DEALBUY_SHILD", GetUnitNum( nPrice ) );
		} //if
		ShowDlg( "DLG_DEALBUYBOX", TRUE );
	}
	else
	{
		if( !strcmp( s_szRequestComp, "DLG_PERSONALSHOP" ) )
		{ //개인사정에서 구입
			PushEvent( "MYSHOP", EVENT_MYSHOP_BUYITEM, m_pBuyItemData->nId, 0 );
		}
		else
		{ //일반상점에서 구입
			DWORD		dwParam1 = ( m_pBuyItemData->nId << 16 ) | m_wShop;
			PushEvent(
				const_cast<char*>( "SHOP" ),
				EVENT_ITEM_BUY,
				dwParam1,
				0 );
		} //if..else
	} //if..else
} //CW3DUInterface::BuyItem

/**
 * Show the OpenMyShop Interface
 */
void CW3DUInterface::ShowOpenMyShop( BOOL bShow )
{
	if( IsShowDlg( "DLG_CREATEMYSHOP" ) == bShow )
	{
		return;
	} //if

	UICComponent*	pXBtn			= GetComponent( "BTN_INVEN_X" );
	UICComponent*	pMyShopDlg		= GetComponent( "DLG_CREATEMYSHOP" );

	Assert( pXBtn );
	Assert( pMyShopDlg );

	if( !pXBtn || !pMyShopDlg )
	{
		return ;
	} //if

	if( bShow )
	{
		pXBtn->Show( FALSE );

		ShowDlg( pMyShopDlg, TRUE );
		ShowMyInventoryDlg( TRUE );

		UICComponent*	pMyShopInven = GetComponent( "INVENTORY_MYSHOP" );
		if( !pMyShopInven )
		{
			Assert( !"Not Found The Comp" );
			return;
		} //if
		
		pMyShopInven->Cleanup();
		
		SetText( "EDT_MYSHOP_TITLE", "" );
		SetText( "TXT_MYSHOP_PRICE", 0 );
		
		m_ItemMgr.ClearItemInTemp();
	}
	else
	{
		pXBtn->Show( TRUE );
		
		ShowDlg( "DLG_INVENTORY", FALSE );
		ShowDlg( pMyShopDlg, FALSE );
		
		UICComponent*	pMyShopInven = GetComponent( "INVENTORY_MYSHOP" );
		if( !pMyShopInven )
		{
			Assert( !"Not Found The Comp" );
			return;
		} //if
		
		pMyShopInven->Cleanup();
		
		SetText( "EDT_MYSHOP_TITLE", "" );
		SetText( "TXT_MYSHOP_PRICE", 0 );
		
		m_ItemMgr.ClearItemInTemp();
	} //if..else
} //CW3DUInterface::ShowOpenMyShop

/**
 * Show the PersonalShop Interface
 */
void CW3DUInterface::ShowPersonalShop( BOOL bShow )
{
	UIDialog*		pShopDlg = reinterpret_cast<UIDialog*>( GetComponent( "DLG_PERSONALSHOP" ) );
	if( !pShopDlg )
	{
		Assert( !"Not Found The Comp" );
		return;
	} //if

	ShowDlg( "DLG_PERSONALSHOP", bShow );
	ShowMyInventoryDlg( bShow );

	if( !bShow ||
		( pShopDlg->IsShow() && bShow ) )
	{
		UIInventoryIN*		pInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_PERSONALSHOP" ) );
		if( !pInven )
		{
			Assert( !"Not Found The Comp" );
			return;
		} //if

		pInven->Cleanup();
		m_ItemMgr.ClearItemInTemp();
	} //if
} //ShowPersonalShop

void CW3DUInterface::RequestDeleteItemInUserDeal( SItemData* pItemDat )
{
	Assert( pItemDat );

	PushEvent(
		"UI_INVENTORYIN",
		EVENT_DEAL_DELETEITEM,
		pItemDat->nId,
		0 );
} //CW3DUInterface::RequestDeleteItemInUserDeal

void CW3DUInterface::InsertItemInMyShop( int nItemId, int nItemType )
{
	SItemData*		pItemData = m_ItemMgr.FindItem( nItemId, TRUE );
	if( !pItemData )
	{	// 존재하지 않는 아이템이라면 등록한다.
		SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nItemType );
		if( !pItemInfoDat )
		{
			Assert( !"아이템 데이터 얻기 실패" );
			return;
		} //if

		m_ItemMgr.InsertItemInTemp(
			nItemId,
			nItemType,
			pItemInfoDat
			);

		pItemData = m_ItemMgr.FindItem( nItemId, TRUE );
	} //if

	if( !pItemData )
	{
		Assert( "Not Add the ItemDat" );
		return;
	} //if
	SItemData*		pItemDataOring = m_ItemMgr.FindItem( nItemId );
	if( pItemDataOring )
	{ // 원래의 아이템에서 필요값
		pItemData->nSellPrice = pItemDataOring->nSellPrice;
	} //if

	UIInventoryIN*		pMyShopInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_MYSHOP" ) );
	if( !pMyShopInven )
	{
		Assert( !"Not Found The InvenComp" );
		return;
	} //if
	pMyShopInven->InsertItem( pItemData );
} //CW3DUInterface::InsertItemInMyShop

void CW3DUInterface::RemoveItemInMyShop( int nItemId )
{
	UIInventoryIN*		pMyShopInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_MYSHOP" ) );
	if( !pMyShopInven )
	{
		Assert( !"Not Found The InvenComp" );
		return;
	} //if
	pMyShopInven->DeleteItem( nItemId );
} //CW3DUInterface::RemoveItemInMyShop


void CW3DUInterface::InsertItemInPersonalShop( int nItemId, int nItemType, int nSellPrice, BYTE btNum )
{
	SItemData*		pItemData = pItemData = m_ItemMgr.FindItem( nItemId, TRUE );
	if( !pItemData )
	{	// 존재하지 않는 아이템이라면 등록한다.
		SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nItemType );
		if( !pItemInfoDat )
		{
			Assert( !"아이템 데이터 얻기 실패" );
			return;
		} //if

		m_ItemMgr.InsertItemInTemp(
			nItemId,
			nItemType,
			pItemInfoDat,
			btNum,
			nSellPrice
			);

		pItemData = m_ItemMgr.FindItem( nItemId, TRUE );
	} //if

	if( !pItemData )
	{
		Assert( "Not Add the ItemDat" );
		return;
	} //if

	pItemData->nSellPrice = nSellPrice;

	UIInventoryIN*		pPersonalShopInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_PERSONALSHOP" ) );
	if( !pPersonalShopInven )
	{
		Assert( !"Not Found The InvenComp" );
		return;
	} //if
	pPersonalShopInven->InsertItem( pItemData );
} //CW3DUInterface::InsertItemInPersonalShop

void CW3DUInterface::RemoveItemInPersonalShop( int nItemId )
{
	UIInventoryIN*		pPersonalShopInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_PERSONALSHOP" ) );
	if( !pPersonalShopInven )
	{
		Assert( !"Not Found The InvenComp" );
		return;
	} //if
	pPersonalShopInven->DeleteItem( nItemId );
} //CW3DUInterface::RemoveItemInPersonalShop

/**
 * 샾에서 아이템을 구입합니다.
 */
void CW3DUInterface::BuyItemToPersonalShop( SItemData* pItemDat )
{
	Assert( pItemDat );

	m_pBuyItemData = pItemDat;
	
	BuyItem( "DLG_PERSONALSHOP" );
} //CW3DUInterface::BuyItemToPersonalShop

void CW3DUInterface::RequestDeleteItemInPersonalShop( SItemData* pItemDat )
{
	Assert( pItemDat );

	PushEvent(
		"UI_INVENTORYIN",
		EVENT_MYSHOP_REMOVEITEM,
		pItemDat->nId,
		0 );
} //CW3DUInterface::DeleteItemInUserDeal

void CW3DUInterface::RemoveItemInSmith( SItemData* pItemDat )
{
	Assert( pItemDat );
	PushEvent(
		"UI_INVENTORYIN",
		EVENT_SMITH_REMOVEITEM,
		pItemDat->nId,
		0
		);
}

void CW3DUInterface::RequestInsertItemInUserDeal( SItemData* pItemDat )
{
	Assert( pItemDat );
	//여기서 유료아이템 거래 불가하게 한다....
/*	if( pItemDat->nType <= 12030 && pItemDat->nType >= 11907 )
	{
		ChattingInput( "", "This item can not trade.", 0xFF00FF00, 0 , 0 );
		return;
	}*/

	if( m_ItemMgr.FromHandItem() == FROM_USERDEAL )		return;
	
	WORD		wX, wY;

	UIInventoryIN*		pInventory;
	int					nIndex = 1;
	while( pInventory = GetInventoryIN( "INVENTORY_DEALME_", nIndex ) )
	{
		if( pInventory->GetBlankNum( &wX, &wY ) )
		{
			break;
		} //if
		nIndex++;
	} //if

	if( pInventory )
	{
/*		if( nIndex > 1 )
		{
			wX |= 0x80;
			wY |= 0x80;
		} //if
*/
		DWORD		dwParam2 = wX << 16 | wY;

		PushEvent(
			"UI_INVENTORYIN",
			EVENT_DEAL_ADDITEM,
			pItemDat->nId,
			dwParam2 );		
	}
	else
	{
		MessageBox( STR_INVENERR_NOTENOUGHSPACE );
	} //if..else
} //CW3DUInterface::RequestInsertItemInUserDeal

void CW3DUInterface::RequestInsertItemInMyShop( SItemData* pItemDat )
{
	Assert( pItemDat );

	if( m_ItemMgr.FromHandItem() == FROM_MYSHOP )		return;

	UIInventoryIN*		pMyShopInven = (UIInventoryIN*)GetComponent( "INVENTORY_MYSHOP" );
	Assert( pMyShopInven );

	WORD		wX, wY;
	if( pMyShopInven->GetBlankNum( &wX, &wY ) )
	{
		m_pBuyItemData = pItemDat;
		InputPrice( "INVENTORY_MYSHOP" );
	} //if
} //CW3DUInterface::RequestInsertItemInMyShop

/**
 * 샾에서 아이템을 구입합니다.
 */
void CW3DUInterface::BuyItemInShop( SItemData* pItemDat )
{
	if( !pItemDat )
	{
		Assert( !"This NULL The ItemDat" );
		return;
	} //if

	m_pBuyItemData = pItemDat;
	if( ISPOTION( m_pBuyItemData->nType ) )
	{ // 물약 구입시 개수 선택박스.
		Assert( m_pBuyItemData->pimgItem );
		SetPicture( "PIC_POSIONBUY_ICON", m_pBuyItemData->pimgItem );

		UIEditText*		pEdtPotion = reinterpret_cast<UIEditText*>( GetComponent( "EDT_POSIONBUY_NUM" ) );
		Assert( pEdtPotion );
		pEdtPotion->Cleanup();
		EditBox_LDown( (UICComponent*)pEdtPotion );
		m_nNumPosion = 0;

		ShowDlg( "DLG_BUYPOSION", TRUE );
		return;
	} //if
	
	BuyItem( "DLG_SHOPINVENTORY" );
} //CW3DUInterface::BuyItemToShop

void CW3DUInterface::BuyCardInShop( SItemData* pItemDat, int itemNum )
{
	if( !pItemDat )
	{
		pItemDat = m_ItemMgr.FindItem( itemNum );
		if( !pItemDat )
		{	// 존재하지 않는 아이템이라면 등록한다.
			SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( itemNum );
			if( !pItemInfoDat )
			{
				Assert( !"아이템 데이터 얻기 실패" );
				return;
			} //if
			
			m_ItemMgr.InsertItem(
				itemNum,
				itemNum,
				pItemInfoDat
				);
			
			pItemDat = m_ItemMgr.FindItem( itemNum );
		} //if
		if( !pItemDat )
		{
			Assert( !"This NULL The ItemDat" );
			return;
		}		
	} //if
	
	m_pBuyItemData = pItemDat;
	if( SMITHENCHANT( m_pBuyItemData->nType ) )
	{ // 카드 구입시 개수 선택박스.
		Assert( m_pBuyItemData->pimgItem );
		SetPicture( "PIC_CARDBUY_ICON", m_pBuyItemData->pimgItem );
		
		UIEditText*		pEdtPotion = reinterpret_cast<UIEditText*>( GetComponent( "EDT_CARDBUY_NUM" ) );
		Assert( pEdtPotion );
		pEdtPotion->Cleanup();
		EditBox_LDown( (UICComponent*)pEdtPotion );
		m_nNumPosion = 0;
		
		ShowDlg( "DLG_BUYCARD", TRUE );
		return;
	} //if
	
}