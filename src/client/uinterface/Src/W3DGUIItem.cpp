/**
 * 유저 인터페이스 아이템 관련.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-21 2:17p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/W3DGUIItem.cpp $
 * 
 * 2     05-02-21 2:17p Umenokoji
 * 
 * 1     05-02-21 2:16p Umenokoji		InsertEquipItem() -Line542- , RemoveEquipItem() -Line570-에 SetMagicResis()추가		
 * 
 * 91    04-12-08 3:07a Sk8snow2
 * 
 * 90    04-09-24 4:46p Paran
 * 
 * 89    04-09-10 11:28p Paran
 * 
 * 67    04-01-02 2:21p Paran			메세지 define 처리
 * 
 * 55    03-12-10 2:04p Paran			인첸츠 카드 추가
 * 
 * 52    03-12-09 5:58p Paran			아이템 데이터 수정
 * 
 * 50    03-11-18 5:11p Sk8snow2		CheckPlayerSkill 수정
 * 
 * 44    03-09-15 1:42a Paran			Quick Slot 포션 갯수 수정.
 * 
 * 11    03-06-23 1:33a Paran			실드 아이템 처리.
 * 
 * 8     03-06-05 3:19p Paran			양손검
 * 
 * 7     03-05-28 4:14p Paran			미니 맵 파티 관련 수정
 * 
 * 5     03-05-25 4:10p Paran			물약 다량 구입 인터페이스 추가
 * 
 * 3     03-05-23 3:43p Paran			버그수정 중...
 * 
 * <HR>
 *
 * @file	W3DGUIItem.cpp
 */

#include "UInterface_Export.h"
#include "W3DUInterface.h"

#include <io.h>
#include <errno.h>

/**
 * 일회성 아이템을 사용.
 */
BOOL CW3DUInterface::EventCastItem( int nType )
{
	SItemData*		pItemDat = GetItemDataInInventory( nType );
	if( pItemDat )
	{
		PushEvent( "UI_KEYDOWN", EVENT_CAST_ITEM, pItemDat->nId, 0 );
		return TRUE;
	}

	return FALSE;
} //CW3DUInterface::EventCastItem

/**
 * 퀵슬롯 갱신.
 */
void CW3DUInterface::RefreshQuickSlot( UIQuickSlot*	pQuickSlot )
{
	SItemData*		pItemDat = NULL;
	for( int nCnt = 0; nCnt < NUM_QUICKSLOT; nCnt++ )
	{
		pItemDat = pQuickSlot->GetItem( nCnt );
		if( !pItemDat )
		{
			continue;
		} //if

		if( pItemDat->pItemInfoData &&
			!ISMAGICSCROLL( pItemDat->pItemInfoData->btItemType ) )
		{
			int		nNumPotion = GetItemNumInInventory( pItemDat->nType );
			if( nNumPotion > 0 )
			{
				pItemDat = GetItemDataInInventory( pItemDat->nType );
				Assert( pItemDat );

				pQuickSlot->InsertSlot( nCnt, pItemDat );
				pQuickSlot->SetPotionNum( pItemDat->nType, nNumPotion );
			}
			else if( !ISWEAPONITEM( pItemDat->pItemInfoData->btItemType )  &&
					 !ISDEFENSEITEM( pItemDat->pItemInfoData->btItemType ) )
			{
				pQuickSlot->RemoveItem( nCnt );
			} //if..else
		} //if
	} //for
} //CW3DUInterface::RefreshQuickSlot

/**
 *
 */
int CW3DUInterface::GetOnItemType( int nPart )
{
	UIInventoryON*	pInventoryOn = (UIInventoryON*)GetComponent( "INVENTORY_ON" );
	Assert( pInventoryOn );

	return pInventoryOn->GetOnItemType( nPart );
} //CW3DUInterface::GetOnItemType

/**
 * 아이템 내용 수정.
 */
BOOL CW3DUInterface::UpdateItemData( const int nId, const WORD btCount )
{
	SItemData*		pItemData = m_ItemMgr.FindItem( nId );
	if( !pItemData )
	{
		Assert( !"없는 아템" );
		return FALSE;
	} //if
	pItemData->btCount = btCount;

	for( int nCnt = 0; nCnt < NUM_QUICKSLOTBAR; nCnt++ )
	{
		UIQuickSlot*	pQuickSlot = GetQuickBar( nCnt );
		Assert( pQuickSlot );
		RefreshQuickSlot( pQuickSlot );
	} //for
	
	return TRUE;
} //CW3DUInterface::UpdateItemData

/**
 * 인베토리내 특정타입 아이템 갯수를 얻는다.
 */
int CW3DUInterface::GetItemNumInInventory( const int nType )
{
	int				nItemNum = 0;
	UIInventoryIN*	pInventory;
	for( int nCnt = 1 ; pInventory = GetInventoryIN( "INVENTORY_IN_", nCnt ); nCnt++ )
	{
		nItemNum += pInventory->GetItemNum( nType );
	} //for
	return nItemNum;
} //CW3DUInterface::GetItemNumInInventory

/**
 * 인벤토리내 특정타입 첫아이템을 얻는다.
 */
SItemData* CW3DUInterface::GetItemDataInInventory( const int nType )
{
	UIInventoryIN*	pInventory = NULL;
	for( int nCnt = 1 ; pInventory = GetInventoryIN( "INVENTORY_IN_", nCnt ); nCnt++ )
	{
		SItemData*		pItemDat = pInventory->GetItem( nType );
		if( pItemDat )
		{
			return pItemDat;
		} //if
	} //for
	return NULL;
} //CW3DUInterface::GetItemDataInInventory

/**
 * 장착창내 아이템을 찾는다.
 */
SItemData* CW3DUInterface::GetItemDataInEquip( const int nType )
{
	UIInventoryON*	pEquip = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
	if( !pEquip )
	{
		Assert( !"Not Found Equipment" );
		return NULL;
	} //if

	SItemData*	pItem = pEquip->GetItem( nType );
	return pItem;
} //CW3DUInterface::GetItemDataInEquip

/**
 * 인벤토리 빈칸 여부.
 */
int CW3DUInterface::InventoryBlank()
{
	UIInventoryIN*		pInvenIN = NULL;
	int					nItemNum = 0;

	for( int nCnt = 1 ; pInvenIN = GetInventoryIN( "INVENTORY_IN_", nCnt ) ; nCnt++ )
	{
		Assert( pInvenIN );

		nItemNum += pInvenIN->GetBlankNum();
	} //for
	return nItemNum;
} //CW3DUInterface::InventoryBlank

/**
 * 인벤토리내 아이템을 추가한다.
 */
BOOL CW3DUInterface::InsertInventory(
	const int	nId,		///< [IN]
	const int	nType,		///< [IN]
	const BYTE	btPage,		///< [IN]
	const BYTE	btCol,		///< [IN]
	const BYTE	btRow,		///< [IN]
	const WORD	btCnt,		///< [IN]
	BOOL bSave				///< [IN]
	)
{
	SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nType );
	if( !pItemInfoDat )
	{
		Assert( !"아이템 데이터 얻기 실패" );
		return FALSE;
	} //if

	WORD		btItemCount = btCnt;
	if( ISPOTION( nType ) && btCnt == 0 )
	{
		btItemCount = 1;
	} //if

	/*if( IS_CASH_CNT_ITEM( nType ) && btCnt == 0 )
	{
		bt
	}*/

	m_ItemMgr.InsertItem(
		nId,
		nType,
		pItemInfoDat,
		btItemCount
		);

	SItemData*		pItemData = m_ItemMgr.FindItem( nId );
	Assert( pItemData );

	UIInventoryIN*	pInventoryIn = NULL;

	if( !btPage )
		pInventoryIn = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_IN_1" ) );
	else
		pInventoryIn = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_IN_2" ) );

	Assert( pInventoryIn );
	
	if( pInventoryIn )
	{
		pInventoryIn->InsertItem( btCol, btRow, pItemData );
	} //if

/*	UIInventoryIN*		pInventoryIn = NULL;
	pInventoryIn = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_IN_1" ) );
	if( !pInventoryIn )
	{
		Assert( !"인벤이 없다" );
		return FALSE;
	} //if

	if( !pInventoryIn->GetBlankNum() )
	{
		pInventoryIn = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_IN_2" ) );
		if( !pInventoryIn )
		{
			Assert( !"인벤이 없다" );
			return FALSE;
		} //if
		if( !pInventoryIn->GetBlankNum() )
		{
			Assert( !"빈공간이 없다" );
			return FALSE;
		} //if
	} //if

	if( !pInventoryIn->InsertItem( pItemData ) )
	{
		Assert( !"인벤아템 저장실패" );
		return FALSE;
	} //if
*/
	for( int nCnt = 0; nCnt < NUM_QUICKSLOTBAR; nCnt++ )
	{
		UIQuickSlot*	pQuickSlot = GetQuickBar( nCnt );
		Assert( pQuickSlot );
		RefreshQuickSlot( pQuickSlot );
	} //for

	if( bSave )
	{
		SaveUserData( FALSE );
	} //if

	return TRUE;
} //CW3DUInterface::InsertInventory

BOOL CW3DUInterface::InsertSerIntInventory( const int	nId,		///< [IN]
											const int	nType,		///< [IN]
											const BYTE	btPage,		///< [IN]
											const BYTE	btCol,		///< [IN]
											const BYTE	btRow,		///< [IN]
											const WORD	btCnt,		///< [IN]
											BOOL bSave )				///< [IN]
{
	SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nType );
	if( !pItemInfoDat )
	{
		Assert( !"아이템 데이터 얻기 실패" );
		return FALSE;
	} //if

	WORD		btItemCount = btCnt;
	if( ISPOTION( nType ) && btCnt == 0 )
	{
		btItemCount = 1;
	} //if

	m_ItemMgr.InsertItem(
		nId,
		nType,
		pItemInfoDat,
		btItemCount
		);

	SItemData*		pItemData = m_ItemMgr.FindItem( nId );
	Assert( pItemData );

	UIInventoryIN*	pInventoryIn = NULL;

	if( !btPage )
		pInventoryIn = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_SERVER_IN_1" ) );
	else
		pInventoryIn = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVENTORY_SERVER_IN_2" ) );

	Assert( pInventoryIn );
	
	if( pInventoryIn )
	{
		pInventoryIn->InsertItem( btCol, btRow, pItemData );
	} //if

	return TRUE;
}

/**
 * 인벤토리내 아이템을 지운다.
 */
BOOL CW3DUInterface::RemoveItemInInventory( int nId )
{
	UIInventoryIN*	pInventoryIn;
	int				nType;
	int				nCnt;

	for( nCnt = 1; pInventoryIn = GetInventoryIN( "INVENTORY_IN_", nCnt ); nCnt++ )
	{
		Assert( pInventoryIn );

		if( pInventoryIn )
		{
			nType = pInventoryIn->DeleteItem( nId );
		} //if

		if( nType > 0 )		break;
	} //for

	for( nCnt = 0; nCnt < NUM_QUICKSLOTBAR; nCnt++ )
	{
		UIQuickSlot*	pQuickSlot = GetQuickBar( nCnt );
		Assert( pQuickSlot );
		RefreshQuickSlot( pQuickSlot );
	} //for

	SaveUserData( FALSE );

	return TRUE;
} //CW3DUInterface::RemoveItemInInventory

/**
 * 인벤토리내 아이템을 지운다.
 */
BOOL CW3DUInterface::RemoveSerInventory( int nId )
{
	UIInventoryIN*	pInventoryIn;
	int				nType;
	int				nCnt;

	for( nCnt = 1; pInventoryIn = GetInventoryIN( "INVENTORY_SERVER_IN_", nCnt ); nCnt++ )
	{
		Assert( pInventoryIn );

		if( pInventoryIn )
		{
			nType = pInventoryIn->DeleteItem( nId );
		} //if

		if( nType > 0 )		break;
	} //for

	return TRUE;
} //CW3DUInterface::RemoveItemInInventory

/**
 * 창고에 아이템을 넣는다.
 */
BOOL CW3DUInterface::InsertItemInStorage(
	const int	nId,		///< [IN]
	const int	nType,		///< [IN]
	const WORD	btCnt		///< [IN]
	)
{
	SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nType );
	if( !pItemInfoDat )
	{
		Assert( !"아이템 데이터 얻기 실패" );
		return FALSE;
	} //if

	WORD		btItemCount = btCnt;
	if( ISPOTION( nType ) && btCnt == 0 )
	{
		btItemCount = 1;
	} //if

	m_ItemMgr.InsertItem(
		nId,
		nType,
		pItemInfoDat,
		btItemCount
		);

	SItemData*		pItemData = m_ItemMgr.FindItem( nId );
	Assert( pItemData );

	UIInventoryIN*		pInven1 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_1" ) );
	UIInventoryIN*		pInven2 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_2" ) );
	Assert( pInven1 );
	Assert( pInven2 );

	if( pInven1->InsertItem( pItemData ) )
	{
		return TRUE;
	} //if

	if( pInven2->InsertItem( pItemData ) )
	{
		return TRUE;
	} //if

	Assert( !"공간이 없다." );
	return FALSE;
} //CW3DUInterface::InsertItemInStorage


BOOL CW3DUInterface::InsertItemInSerGarbage( const int nId, const int nType, const WORD btCnt )
{
	SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nType );
	if( !pItemInfoDat )
	{
		Assert( !"아이템 데이터 얻기 실패" );
		return FALSE;
	} //if
	
	WORD		btItemCount = btCnt;
	if( ISPOTION( nType ) && btCnt == 0 )
	{
		btItemCount = 1;
	} //if
	
	m_ItemMgr.InsertItem(
		nId,
		nType,
		pItemInfoDat,
		btItemCount
		);
	
	SItemData*		pItemData = m_ItemMgr.FindItem( nId );
	Assert( pItemData );
	
	UIInventoryIN*		pSerInven1 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_SERVER_STORAGE_1" ) );
	UIInventoryIN*		pSerInven2 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_SERVER_STORAGE_2" ) );
	Assert( pSerInven1 );
	Assert( pSerInven2 );
	
	if( pSerInven1->InsertItem( pItemData ) )
	{
		return TRUE;
	} //if
	
	if( pSerInven2->InsertItem( pItemData ) )
	{
		return TRUE;
	} //if
	
	Assert( !"공간이 없다." );
	return FALSE;
}

/**
 * 창고안에 아이템을 삭제.
 */
BOOL CW3DUInterface::RemoveItemInStorage( const int nId )
{
	UIInventoryIN*		pInven = NULL;

	pInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_1" ) );
	Assert( pInven );
	if( pInven->DeleteItem( nId ) != -1 )
	{
		return TRUE;
	} //if

	pInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_2" ) );
	Assert( pInven );
	if( pInven->DeleteItem( nId ) != -1 )
	{
		return TRUE;
	} //if
	
	Assert( !"없는 아이템인감" );
	return FALSE;
}  //CW3DUInterface::RemoveItemInStorage

BOOL CW3DUInterface::RemoveSerItemInStorage( const int nId )
{
	UIInventoryIN*		pInven = NULL;
	
	pInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_SERVER_STORAGE_1" ) );
	Assert( pInven );
	if( pInven->DeleteItem( nId ) != -1 )
	{
		return TRUE;
	} //if
	
	pInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_SERVER_STORAGE_2" ) );
	Assert( pInven );
	if( pInven->DeleteItem( nId ) != -1 )
	{
		return TRUE;
	} //if
	
	Assert( !"없는 아이템인감" );
	return FALSE;
}

/**
 * 창고 모든 아이템을 삭제.
 */
void CW3DUInterface::RemoveAllItemInStorage()
{
	UIInventoryIN*		pInven1 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_1" ) );
	UIInventoryIN*		pInven2 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_2" ) );
	Assert( pInven1 );
	Assert( pInven2 );
	
	pInven1->Cleanup();
	pInven2->Cleanup();
} //CW3DUInterface::RemoveAllItemInStorage

void CW3DUInterface::RemoveAllSerItemInStorage()
{
	UIInventoryIN*		pInven1 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_SERVER_STORAGE_1" ) );
	UIInventoryIN*		pInven2 = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_SERVER_STORAGE_2" ) );
	Assert( pInven1 );
	Assert( pInven2 );
	
	pInven1->Cleanup();
	pInven2->Cleanup();
} //CW3DUInterface::RemoveAllItemInStorage

/**
 * 창고에 아이템 저장 요청.
 */
void CW3DUInterface::RequestSaveItemInStorage( SItemData* pItemDat )
{
	Assert( pItemDat );

	if( m_ItemMgr.FromHandItem() == FROM_STORAGE )
	{
		return;
	} //if

	WORD			wX, wY;
	int				nBlankNum;
	UIInventoryIN*	pStorageInven;

	pStorageInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_1" ) );
	Assert( pStorageInven );
	nBlankNum = pStorageInven->GetBlankNum( &wX, &wY );
	if( nBlankNum == 0  )
	{
		pStorageInven = reinterpret_cast<UIInventoryIN*>( GetComponent( "INVEN_STORAGE_2" ) );
		Assert( pStorageInven );
		nBlankNum = pStorageInven->GetBlankNum( &wX, &wY );
	} //if

	if( nBlankNum > 0 )
	{
		DWORD		dwParam2 = wX << 16 | wY;
		PushEvent(
			"UI_INVENTORYIN",
			EVENT_STORAGE_ADDITEM,
			pItemDat->nId,
			dwParam2 );
	} //if
} //CW3DUInterface::RequestSaveItemInStorage

void CW3DUInterface::RequestSaveitemInSmith( SItemData* pItemDat )
{
	Assert( pItemDat );

	if( m_ItemMgr.FromHandItem() == FROM_STORAGE )
	{
		return;
	} //if

	SItemInfoData*	pItemInfoDat = pItemDat->pItemInfoData;

	if( m_SelectIntegration )
	{
		if( ISWEAPONITEM( pItemInfoDat->btItemType ) )
		{
			if( pItemDat->pItemInfoData->bItemIndex < 5 || pItemDat->pItemInfoData->bItemIndex > 9 )
			{
				ChattingInput( "", STR_ENCHANT_ITEM, FONT_MSGCOLOR, 0, 0 );
				return;
			}			
		}
		else if( pItemDat->nType != 12097 )
		{
			ChattingInput( "", STR_ENCHANT_ITEM, FONT_MSGCOLOR, 0, 0 );
			return;
		}
	}
	
	else
	{
		if( ISDEFENSEITEM( pItemInfoDat->btItemType ) )
		{
			if( pItemDat->pItemInfoData->bItemIndex < 3 || pItemDat->pItemInfoData->bItemIndex > 6 )
			{
				ChattingInput( "", STR_ENCHANT_ITEM, FONT_MSGCOLOR, 0, 0 );
				return;
			}
		}	
		else if( pItemDat->nType != 12098 )
		{
			ChattingInput( "", STR_ENCHANT_ITEM, FONT_MSGCOLOR, 0, 0 );
			return;
		}
	}

//	WORD			wX, wY;
	UIInventoryIN*	pStorageInven;
	
	pStorageInven = reinterpret_cast<UIInventoryIN*>( GetComponent( m_szUIccompentSmith ) );

	
	if( pStorageInven )
	{
	//	DWORD		dwParam2 = wX << 16 | wY;
		PushEvent(
			"UI_INVENTORYIN",
			EVENT_SMITH_ADDITEM,
			pItemDat->nId,
			0 );
	} //if

}

/**
 * 아이템을 손으로 옮긴다.
 */
BOOL CW3DUInterface::ItemToHand( int nId )
{
	SItemData*		pItemData = m_ItemMgr.FindItem( nId );

	if( !pItemData )
	{
		Assert( !"등록된 아이템 없다" );
		return FALSE;
	} //if

	UIInventoryON*	pInventoryOn = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
	Assert( pInventoryOn );

	RemoveItemInInventory( nId );

	return TRUE;
} //CW3DUInterface::ItemToHand

/**
 * 아이템을 손에서 옮긴다.
 */
BOOL CW3DUInterface::ItemFromHand()
{
	if( m_ItemMgr.GetHandInItemDat() )
	{ // 손에 아이템을 가지고 있을 경우.
		SItemData*		pItemData = m_ItemMgr.GetHandInItemDat();
		BYTE			btFrom	  = m_ItemMgr.FromHandItem();
		if( btFrom >= FROM_QUICK )
		{
			btFrom -= FROM_QUICK;				///< 100 이상값은 슬롯

			UIQuickSlot*	pQuickSlot = GetQuickBar( m_btQuickBarNum );
			Assert( pQuickSlot );
			pQuickSlot->RemoveItem( btFrom );
		}
		else if( btFrom == FROM_EQUIP || btFrom == FROM_INVENTORY )
		{
			SItemInfoData*	pItemInfoDat = pItemData->pItemInfoData;
			if( pItemInfoDat )
			{
				char	szText[64];
				sprintf( szText, "%s\n%s", pItemInfoDat->szHName, STR_UIMSG_DROPITEM );
				MessageBox( szText, "DROPITEM", MSG_YESNO );
				m_pConfirmItemData = pItemData;
				m_btFrom = btFrom;
			} //if
		} //if..else
		m_ItemMgr.SetHandInItemDat( NULL, FROM_NULL );

		return TRUE;
	} //if
	return FALSE;
} //CW3DUInterface::ItemFromHand

/**
 * 아이템을 장착한다.
 */
WORD CW3DUInterface::InsertEquipItem(
	const int	nId,	///< [IN]
	const int	nType	///< [IN]
	)
{
	SItemData*		pItemData = m_ItemMgr.FindItem( nId );

	if( !pItemData )
	{	// 존재하지 않는 아이템이라면 등록한다.
		SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nType );
		Assert( pItemInfoDat );

		m_ItemMgr.InsertItem(
			nId,
			nType,
			pItemInfoDat
			);

		pItemData = m_ItemMgr.FindItem( nId );
	} //if

	Assert( pItemData );

	UIInventoryON*	pInventoryOn = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
	Assert( pInventoryOn );

	if( !pInventoryOn->InsertItem( 0, pItemData ) )
	{
		return 0;
	} //if

	SetDefence();
	//마   법 저항력 설정
	SetMagicResis(  );
	SetDamage( pItemData->pItemInfoData );

	return pItemData->nType;
} //CW3DUInterface::InsertEquipItem


/**
 * 대장장이창에 아이템 넘긴다.
 */ 
void CW3DUInterface::InsertSmithItem( const int nId, const int nType, const int price )
{
	SItemData*		pItemData = m_ItemMgr.FindItem( nId );
	
	if( !pItemData )
	{	// 존재하지 않는 아이템이라면 등록한다. 
		SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nType );
		Assert( pItemInfoDat );
		
		m_ItemMgr.InsertItem(
			nId,
			nType,
			pItemInfoDat
			);
		
		pItemData = m_ItemMgr.FindItem( nId );
	} //if
	
	//Assert( pItemData );
	if( !pItemData )
	{
		Assert( "Not add the ItemDat" );
		return;
	}
	UIInventoryIN*	pSmithInventoryIn = reinterpret_cast<UIInventoryIN*>( GetComponent( m_szUIccompentSmith ) );
	Assert( pSmithInventoryIn );

	if( !pSmithInventoryIn )
	{
		Assert( !"Not Found the InvenComp" )
		return;
	}

	int nX = 0;

	if( pItemData->nType == 12097 || pItemData->nType == 12098 )
	{
		nX = 1;
	}
	else 
	{
		nX = 0;
	}
	
	if( pSmithInventoryIn->InsertItem( nX, 0, pItemData ) )
	{
		ChattingInput( "", "dlaldlTek.", FONT_MSGCOLOR, 0, 0 );
		return;
	}

	//결과 보낼 아이템 설정하는곳
	if( nX == 0 )
	{
	//	memcpy( &m_EnchantSmithItem , pItemData, sizeof( SItemData ) );
		m_EnchantSmithItem = pItemData;
	}
	else
	{
		m_EnchantSmithCard.SetCardItem( pItemData );
	}

	if( nX == 0 )
		m_EnchantPricemoney = price;

	SetText( m_szTxtName, m_EnchantPricemoney );
	
	//return pItemData->nType;
}

/**
 * 아이템을 장착 해제한다.
 */
WORD CW3DUInterface::RemoveEquipItem(
	const int	nId,	///< [IN]
	const int	nPos	///< [IN]
	)
{
	SItemData*	pItemData = m_ItemMgr.FindItem( nId );

	WORD	wType	= pItemData->nType;
	WORD	wPart	= pItemData->pItemInfoData->nItemKind;

	UIInventoryON*	pInventoryOn = reinterpret_cast<UIInventoryON*>( GetComponent( "INVENTORY_ON" ) );
	if( !pInventoryOn )
	{
		Assert( pInventoryOn );
		return 0;
	}
	pInventoryOn->DeleteItem( wPart );

	SetDefence();
	//마법 저항력 설정
	SetMagicResis(  );
	SetDamage( pItemData->pItemInfoData );
	
	return wType;
} //CW3DUInterface::RemoveEquipItem

/**
 * 대장장이창에 아이템 해제
 */
void CW3DUInterface::RemoveSmithItem( const int nId )
{
	UIInventoryIN*		pSmithInven = reinterpret_cast<UIInventoryIN*>( GetComponent( m_szUIccompentSmith ) );
	if( !pSmithInven )
	{
		Assert( !"Not Found The InvenComp" );
		return;
	} //if
	pSmithInven->DeleteItem( nId );	
}

void CW3DUInterface::RequestSellItem( SItemData* pItemDat )
{
	Assert( pItemDat );

	if( m_ItemMgr.FromHandItem() == FROM_SHOP )		return;

	m_nSellItem = pItemDat->nId;

	SetPicture( "PIC_DEALSELL_ICON", pItemDat->pimgItem );
	SItemInfoData*		pItemInfo = pItemDat->pItemInfoData;
	Assert( pItemInfo );

	if( pItemInfo )
	{
		SetText( "TXT_DEALSELL_SHILD", GetUnitNum( pItemInfo->nSellPrice ) );
	} //if

	ShowDlg( "DLG_DEALSELLBOX", TRUE );
} //CW3DUInterface::SellItemToInventory

/**
 * 창고에서 아이템을 꺼낸다.
 */
void CW3DUInterface::TakeItemInStorage( SItemData* pItemDat )
{
	Assert( pItemDat );

	PushEvent(
		"UI_INVENTORYIN",
		EVENT_STORAGE_REMOVEITEM,
		pItemDat->nId,
		0 );
} //CW3DUInterface::TakeItemInStorage


void CW3DUInterface::TakeItemInSerStorage( SItemData* pItemDat, int ServerNum )
{
	Assert( pItemDat );

	PushEvent(
		"UI_INVENTORYIN",
		EVENT_SERINT_GARBAGE_REMOVEITEM,
		pItemDat->nId,
		ServerNum );
}

void CW3DUInterface::TakeItemInSerInven( SItemData* pItemDat, int ServerNum, int CharPos )
{
	Assert( pItemDat );

	DWORD dwParam2 = ( ServerNum << 16 ) | CharPos;
	PushEvent( "UI_INVENTORYIN",
		EVENT_SERINT_INVEN_REMOVEITEM,
		pItemDat->nId,
		dwParam2
		);
}

/**
 * 장착 아이템 제거.
 */
void CW3DUInterface::RemoveItemInEquip( SItemData* pItemDat )
{
	SoundItem( pItemDat );

	PushEvent( 
		"UI_INVENTORYON", 
		EVENT_DBCLICK_ITEM, 
		pItemDat->nId, 
		1					///< '1' 장착창
		);
} //CW3DUInterface::RemoveItemInEquip

/**
 * 상점번호 저장.
 */
void CW3DUInterface::SetShopNum( const WORD wShop )
{
	if( wShop <= 0 )
	{
		Assert( !"상점 번호가 이상하다" );
		return;
	} //if

	m_wShop = wShop;
} //CW3DUInterface::SetShopNum

/**
 * 퀘스트번호 저장.
 */
void CW3DUInterface::SetQuestNum( const WORD wQuest )
{
	if( wQuest <= 0 )
	{
		Assert( !"퀘스트 번호가 이상하다" );
		return;
	} //if

	m_wQuest = wQuest;
} //CW3DUInterface::SetShopNum

/**
 * 퀘스트 설정.
 */
void CW3DUInterface::Quest( WORD wNPCId, WORD wQuest, BYTE btProc )
{
	char		szQuestName[64];
	sprintf( szQuestName, "NPC/quest%04d.scr", wQuest );

	m_pQuestScript = MAKE_COMPONENT( NPCScript );
	if( !m_pQuestScript->Load( szQuestName ) )
	{
		Assert( !"없는 퀘스트 스크립트" );
		return;
	} //if
	
	if( m_pQuestScript->OpenNPC( wNPCId ) )
	{
		m_bNPCTalkMode = wQuest;			///< '0' - 일반대화, '1' - 퀘스트, '2' - 대장장이;
		/*
		m_NPCNum = (int)wNPCId;
		BeginNPC( m_pQuestScript, btProc );*/
#ifdef _dUMENOKOJI_QUEST_
		BeginQuestNPC( m_pQuestScript, btProc );
#endif
	} //if
} //CW3DUInterface::Quest

/**
 * 퀘스트 리스트 저장.
 */
void CW3DUInterface::AddQuestInList( WORD wQuestNum, BYTE btState )
{
	DWORD		dwColor;
	if( btState == 0 )			return;
	else if( btState == 1 )		dwColor = 0;
	else						dwColor = 0xFF8E8E8E;

	UIStringList*	pStrList = reinterpret_cast<UIStringList*>( GetComponent( "LIST_QUEST_LIST" ) );
	Assert( pStrList );

	char*	pTitle = NULL;
	if( m_pQuestScript->OpenNPC( wQuestNum ) )
	{
		pTitle = const_cast<char*>( m_pQuestScript->GetNPCName() );
		if( !pTitle )
		{
			Assert( !"없는 타이틀" );
			return;
		} //if
	} //if

	pStrList->SetText( pTitle, dwColor );
} //CW3DUInterface::AddQuestInList

/**
 * 샾 아이템 추가.
 */
void CW3DUInterface::InsertItemInShop( int nItemType, BYTE btPage, BYTE btCol, BYTE btRow )
{
	Assert( btPage >= 0 );

	SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nItemType );
	if( !pItemInfoDat ) 
	{
		Assert( !"아이템 데이터 얻기 실패" );
		return;
	} //if
	
	m_ItemMgr.InsertItemInTemp(
		nItemType,
		nItemType,
		pItemInfoDat 
		);
	SItemData*		pItemData = m_ItemMgr.FindItem( nItemType, TRUE );
	if( !pItemData )
	{
		Assert( pItemData );
		return;
	} //if

	UIInventoryIN*	pInventory = NULL;
	for( int nCnt = 0; nCnt < 3; nCnt++ )
	{
		pInventory = GetInventoryIN( "SHOPINVENTORY_", nCnt + 1 );
		if( !pInventory )
		{
			Assert( pInventory );
			return ;
		} //if

		if( pInventory->InsertItem( pItemData ) )
		{
			return;
		} //if
	} //for
} //CW3DUInterface::InsertItemInShop

void CW3DUInterface::InsertItemInUserDeal( int nItemId, int nItemType, 
										   WORD wDestCol, WORD wDestRow, BOOL bMe )
{
	if( !IsShowDlg( "DLG_DEAL" ) )
	{
		Assert( !"거래준비가 되지 않은 상태" );
		return;
	} //if

	char		szInvenKey[32];
	if( bMe )	strcpy( szInvenKey, "INVENTORY_DEALME_" );
	else		strcpy( szInvenKey, "INVENTORY_DEALYOU_" );

	UIInventoryIN*		pInventory;
	int					nIndex = 1;
	while( pInventory = GetInventoryIN( szInvenKey, nIndex ) )
	{
		if( pInventory->GetBlankNum() )
		{
			break;
		} //if
		nIndex++;
	} //if

	if( !pInventory )
	{
		return ;
	} //if

	SItemData*		pItemData = NULL;
	if( bMe )
	{
		pItemData = m_ItemMgr.FindItem( nItemId );
		if( !pItemData )
		{	// 존재하지 않는 아이템이라면 등록한다.
			SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nItemType );
			if( pItemInfoDat )
			{
				Assert( !"아이템 데이터 얻기 실패" );
				return;
			} //if

			m_ItemMgr.InsertItem(
				nItemId,
				nItemType,
				pItemInfoDat
				);

			pItemData = m_ItemMgr.FindItem( nItemId );
		} //if
	}
	else
	{
		pItemData = m_ItemMgr.FindItem( nItemId, TRUE );
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
	}

	if( !pItemData )
	{
		Assert( pItemData );
		return;
	} //if

	pInventory->InsertItem( pItemData );
} //CW3DUInterface::InsertItemInUserDeal

void CW3DUInterface::RemoveItemInUserDeal( int nItemId, BOOL bMe )
{
	char				szInvenKey[32];
	if( bMe )	strcpy( szInvenKey, "INVENTORY_DEALME_" );
	else		strcpy( szInvenKey, "INVENTORY_DEALYOU_" );

	UIInventoryIN*		pInventory;
	int					nIndex = 1;
	while( pInventory = GetInventoryIN( szInvenKey, nIndex ) )
	{
		if( pInventory->DeleteItem( nItemId ) >= 0 )
		{
			return;
		} //if
		nIndex++;
	} //while
} //CW3DUInterface::RemoveItemInUserDeal

/**
 *  
 */
void CW3DUInterface::CleanupCashInven()
{
	UIInventoryIN*		pInven;
	
	pInven = GetInventoryIN( "CASHITEM_IN_", 1 );
	if( pInven ) pInven->Cleanup();
	
	pInven = GetInventoryIN( "CASHITEM_IN_", 2 );
	if( pInven ) pInven->Cleanup();

	pInven = GetInventoryIN( "CASHBUFF_IN_", 1 );
	if( pInven ) pInven->Cleanup();
	
	m_ItemMgr.ClearItemInTemp();
} //CW3DUInterface::CleanupCashInven

/**
 *  
 */
void CW3DUInterface::InsertItemInCash_Buff( int nItemNum )
{
	Assert( m_pItemInfoMgr );

	SItemData*		pItemData = m_ItemMgr.FindItem( nItemNum, TRUE );
	if( pItemData )
	{
		Assert( !"Warning, Already Exist The Item" );
	}
	else
	{
		SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nItemNum );
		if( !pItemInfoDat ) 
		{
			Assert( !"아이템 데이터 얻기 실패" );
			return;
		} //if
		
		m_ItemMgr.InsertItemInTemp(
			nItemNum,
			nItemNum,
			pItemInfoDat 
			);

		pItemData = m_ItemMgr.FindItem( nItemNum, TRUE );
	} //if..else

	if( !pItemData )
	{
		Assert( !"Can Not InsertItem" );
		return;
	} //if

	UIInventoryIN*	pBuffInven = GetInventoryIN( "CASHBUFF_IN_", 1 );
	if( !pBuffInven )
	{
		Assert( !"Not Found The BuffInven" );
		return;
	} //if

	if( !pBuffInven->GetBlankNum() )
	{
		Assert( !"Not Enough The Buff Inven" );
		return;
	} //if
	pBuffInven->InsertItem( pItemData );
} //CW3DUInterface::InsertItemInCash_Buff

/**
 *  
 */
void CW3DUInterface::InsertItemInCash( int nIdx, int nItemNum, int nContainCnt )
{
	Assert( m_pItemInfoMgr );

	SItemData*		pItemData = m_ItemMgr.FindItem( nIdx );
	if( !pItemData )
	{
		SItemInfoData*	pItemInfoDat = m_pItemInfoMgr->GetItemInfo( nItemNum );
		if( !pItemInfoDat ) 
		{
			Assert( !"아이템 데이터 얻기 실패" );
			return;
		} //if
		
		m_ItemMgr.InsertItem( nIdx, nItemNum, pItemInfoDat, nContainCnt );
		pItemData = m_ItemMgr.FindItem( nIdx );
		if( !pItemData )
		{
			Assert( !"Can Not InsertItem" );
			return;
		} //if
	} //if

	UIInventoryIN*		pCashInven;
	pCashInven = GetInventoryIN( "CASHITEM_IN_", 1 );
	if( !pCashInven->GetBlankNum() )
	{
		pCashInven = GetInventoryIN( "CASHITEM_IN_", 2 );
		if( !pCashInven->GetBlankNum() )
		{
			Assert( !"Not Enough The CashInventory" );
			return;
		} //if
	} //if
	pCashInven->InsertItem( pItemData );

} //CW3DUInterface::InsertItemInCash

/**
 * This function delete the item in cash.
 */
void CW3DUInterface::DeleteItemInCash( int nIdx )
{
	UIInventoryIN*		pCashInven;
	pCashInven = GetInventoryIN( "CASHITEM_IN_", 1 );
	if( pCashInven && pCashInven->DeleteItem( nIdx ) < 0 )
	{
		pCashInven = GetInventoryIN( "CASHITEM_IN_", 2 );
		if( pCashInven && pCashInven->DeleteItem( nIdx ) < 0 )
		{
			Assert( !"Can Not Delete The Item" );
		} //if
	} //if
} //CW3DUInterface::DeleteItemInCash

/**
 * This function use cash item. 
 */
void CW3DUInterface::CashItemUse( SItemData* pItemDat )
{
	PushEvent( "CASHITEM", EVENT_USECASHITEM, pItemDat->nId, 0 );
} //CW3DUInterface::CashItemUse

void CW3DUInterface::UpdateMoneyInUserDeal( int	nMoney, BOOL bMe )
{
	if( !IsShowDlg( "DLG_DEAL" ) )
	{
		Assert( !"거래준비가 되지 않은 상태" );
		return;
	} //if

	if( !bMe )		SetText( "TXT_DEAL_GOLD2", GetUnitNum( nMoney ) );
	else			SetText( "TXT_DEAL_GOLD1", GetUnitNum( nMoney ) );
} //CW3DUInterface::UpdateMoneyInUserDeal


/**
 *
 */
void CW3DUInterface::SetNowWeight( int nowWeight, int MaxWeight )
{
	if( nowWeight < MaxWeight )
	{
		m_bCheckWeight = TRUE;
	}
	else if( nowWeight >= MaxWeight )
	{
		m_bCheckWeight = FALSE;
	}
}

/**
 * 포션구입 창을 다시...
 */
void CW3DUInterface::RefreshBuyPosion()
{
	 if( m_pBuyItemData->nId == 12098 || m_pBuyItemData->nId == 12097 )
	{
		m_bNumPosion = TRUE;
		int				nInvenSpace = 0;
		int				getCardCnt = 0;
		UIInventoryIN*	pInven;
		for( int nCnt = 1 ; pInven = GetInventoryIN( "INVENTORY_IN_", nCnt ) ; nCnt++ )
		{ // 인벤토리 단위 공간 총 갯수를 얻는다.
			nInvenSpace += pInven->GetBlankNum();
			if( m_pBuyItemData->nId == 12098 )
				getCardCnt += pInven->GetItemNum( 10194 );
			else if( m_pBuyItemData->nId == 12097 )
				getCardCnt += pInven->GetItemNum( 10193 );
		} //for
		if( m_nNumPosion < 0 )		m_nNumPosion = 0;
		if( m_nNumPosion > getCardCnt )		m_nNumPosion = getCardCnt;

		if( nInvenSpace < 1 )
		{			
			m_nNumPosion--;
			
			if( m_nNumPosion <= -1 )
				m_nNumPosion=0;

			MessageBox( STR_INVENERR_NOTENOUGHSPACE, "NOSPACE", MSG_OK );
			m_bNumPosion = FALSE;
			//거시기허면 창을 다다삔다.
		} //if
		
		m_bCheckSild = TRUE;
		int		nPrice = m_pBuyItemData->pItemInfoData->nPrice * m_nNumPosion;
		if( nPrice > m_nGold )
		{
			MessageBox( STR_INVENERR_NOTENOUGHGOLD, "NOMONEY", MSG_OK );
			m_bCheckSild = FALSE;
			return;
		} //if	
		
		SetText( "EDT_CARDBUY_NUM", m_nNumPosion );
		SetText( "TXT_CARDBUY_SHILD", nPrice );		
	}
	else
	{
		m_bNumPosion = TRUE;
		int				nInvenSpace = 0;
		UIInventoryIN*	pInven;
		for( int nCnt = 1 ; pInven = GetInventoryIN( "INVENTORY_IN_", nCnt ) ; nCnt++ )
		{ // 인벤토리 단위 공간 총 갯수를 얻는다.
			nInvenSpace += pInven->GetBlankNum();
		} //for
		if( m_nNumPosion < 0 )		m_nNumPosion = 0;
		if( m_nNumPosion > 9999 )		m_nNumPosion = 9999;
		
		if( nInvenSpace < 1 )
		{
			m_nNumPosion--;
			MessageBox( STR_INVENERR_NOTENOUGHSPACE, "NOSPACE", MSG_OK );
			m_bNumPosion = FALSE;
		} //if
		
		int		nPrice = m_pBuyItemData->pItemInfoData->nPrice * m_nNumPosion;
		m_bCheckSild = TRUE;
		if( nPrice > m_nGold )
		{
			MessageBox( STR_INVENERR_NOTENOUGHGOLD, "NOMONEY", MSG_OK );
			m_bCheckSild = FALSE;
			return;
		} //if	
		SetText( "EDT_POSIONBUY_NUM", m_nNumPosion );
		SetText( "TXT_POSIONBUY_SHILD", nPrice );
	}

} //CW3DUInterface::RefreshBuyPosion


/**
 * 스킨등록 요청.
 */
void CW3DUInterface::RequestUseItem( SItemData* pItemDat )
{
	Assert( pItemDat );

	SItemInfoData*		pItemInfo = m_pItemInfoMgr->GetItemInfo( pItemDat->nType );
	if( !pItemInfo )
	{
		Assert( !"없는 아템" );
		return;
	} //if

	if( ISITEMROOK( pItemDat->nType ) )
	{
		PushEvent( "EVENT_ITEMROOK", EVENT_ITEMROOK, 0, 0 );
	}
	else if( ISLEVELROOK( pItemDat->nType ) )
	{
		PushEvent( "EVENT_LEVELROOK", EVENT_LEVELROOK, 0, 0 );
	}
	else if( ISENCHANT( pItemDat->nType ) )
	{ // 인첸트 아이템 선택
		SelectedEnchantItem( pItemDat, TRUE );
		return;
	}
	else if( m_EnchantCard.IsWorking() )
	{ // 선택한 인첸트 아이템이 있을 때
		UseEnchantItem( pItemDat );
		return;
	} //if

	if( pItemInfo->btItemType == dITEMTYPE_MAGICSCROLL )
	{ //이미 등록된 스킬인 경우.
		if( !pItemInfo->pSkillData )
		{
			Assert( !"스킬정보 얻기 실패" );
			return;
		} //if

		DWORD	dwSkillId = pItemInfo->pSkillData->nItemNum;
		if( m_ItemMgr.FindItem( dwSkillId ) )
		{
			return;
		} //if
	} //if

	if( pItemInfo->btItemType != dITEMTYPE_MAGICSCROLL )
	{
		SoundItem( pItemDat );
	} //if
	
	PushEvent( "UI_INVENTORYIN", EVENT_DBCLICK_ITEM, pItemDat->nId, 0 );		// '0' 인벤토리	
} //CW3DUInterface::RequestUseItem

/**
 * 사용가능 스킬인지 확인.
 */
BOOL CW3DUInterface::CheckPlayerSkill( SItemData* pItemDat )
{
	Assert( pItemDat );

	SItemInfoData*	pItemInfoDat = pItemDat->pItemInfoData;
	if( !pItemInfoDat )
	{
		Assert( !"NULL" );
		return FALSE;
	} //if

/*	if( ISMAGICSCROLL( pItemInfoDat->btItemType ) )
	{
		int		nSkillLev;
		if( pItemInfoDat->btMagicKind == 1 )		///< 흑마법.
		{
			nSkillLev = atoi( GetText( "TXT_CINFO_SK3" ) );
			if( pItemInfoDat->m_MagicMinLev > nSkillLev )
			{
				char		szText[64];
				sprintf( szText, "흑마법 %d부터 사용가능 합니다.", pItemInfoDat->m_MagicMinLev );
				SetText( "LIST_CHATTING", szText );
				return FALSE;
			} //if
		}
		else if( pItemInfoDat->btMagicKind == 2 )	///< 백마법.
		{
			nSkillLev = atoi( GetText( "TXT_CINFO_SK4" ) );
			if( pItemInfoDat->m_MagicMinLev > nSkillLev )
			{
				char		szText[64];
				sprintf( szText, "백마법 %d부터 사용가능 합니다.", pItemInfoDat->m_MagicMinLev );
				SetText( "LIST_CHATTING", szText );
				return FALSE;
			} //if
		} //if..else
	} //if*/
	
	return TRUE;
} //CW3DUInterface::CheckPlayerSkill


/**
 * 아이템을 내 인벤토리에...
 */
void CW3DUInterface::ItemInMyInventory( SItemData* pItemDat, UIInventoryIN*	pInventory )
{
	switch( m_ItemMgr.FromHandItem() )
	{
	case FROM_INVENTORY:	
		MoveItemInInventory( pItemDat, pInventory );
		return;

	case FROM_EQUIP:		RemoveItemInEquip( pItemDat );				return;
	case FROM_SHOP:			BuyItemInShop( pItemDat );					return;
	case FROM_STORAGE:		TakeItemInStorage( pItemDat );				return;
	case FROM_USERDEAL:		RequestDeleteItemInUserDeal( pItemDat );	return;
	case FROM_MYSHOP:		RequestDeleteItemInPersonalShop( pItemDat );return;
	case FROM_PSHOP:		BuyItemToPersonalShop( pItemDat );			return;
	case FROM_CASH:			CashItemUse( pItemDat );					return;
	case FROM_WP:			
	case FROM_AM:
		MoveItemInInventory( pItemDat, pInventory );
		RemoveItemInSmith( pItemDat );
		return;
//	case FROM_CARD:			BuyCardInShop( pItemDat );					return;

	default:	return;
	} //switch
} //CW3DUInterface::ItemInMyInventory

/**
 * 아이템 사운드.
 */
void CW3DUInterface::SoundItem( SItemData* pItemDat )
{
	Assert( m_pSound );

	if( pItemDat->nType == ITEM_MONEY )			m_pSound->Play( "MoneyDrop.wav" );
	else if( ( ISHORSE( pItemDat->nType ) || pItemDat->nType == 19130 ) )		m_pSound->Play( "HorseWhinney.wav" );
	else
	{
		SItemInfoData*	pItemInfo = pItemDat->pItemInfoData;
		if( !pItemInfo )	return;

		if( ISWEAPONITEM( pItemInfo->btItemType ) )			m_pSound->Play( "SwordEquip.wav" );
		else if( ISDEFENSEITEM( pItemInfo->btItemType ) )	m_pSound->Play( "ArmorEquip.wav" );
	} //if..else
} //CW3DUInterface::SoundItem

/**
 * 인첸트 결과
 */
void CW3DUInterface::EnchantResult( BOOL bResult )
{
	if( bResult )
	{
		ChattingInput( "", STR_UIMSG_ENCHANTSUCCESS, FONT_MSGCOLOR, 0, 0 );
	}
	else
	{
		ChattingInput( "", STR_UIMSG_ENCHANTFAIL, FONT_MSGCOLOR, 0, 0 );
	} //if..else

	//여기서 파일 지우기
	
	m_EnchantCard.SetCard( bResult );
} //CW3DUInterface::EnchantResult

/**
* 인첸트 카드 사용
*/
BOOL CW3DUInterface::SelectedEnchantItem( SItemData* pItemDat, BOOL bShowMsg )
{
	switch( pItemDat->nType )
	{
	case ITEM_ENCHANT_WEAPON:	
		if( bShowMsg )	ChattingInput( "", STR_UIMSG_SELECTWEAPONITEM, FONT_MSGCOLOR, 0, 0 );
		break;
		
	case ITEM_ENCHANT_ARMOR:	
		if( bShowMsg )	ChattingInput( "", STR_UIMSG_SELECTARMORITEM, FONT_MSGCOLOR, 0, 0 );
		break;
		
	case ITEM_ENCHANT_OPTION:	
		if( bShowMsg )	ChattingInput( "", STR_UIMSG_SELECTOPTIONITEM, FONT_MSGCOLOR, 0, 0 );
		break;
		
	default:
		if( !ISENCHANTCHANGE( pItemDat->nType ) &&
			!ISENCHANTCOLLECTION( pItemDat->nType ) )
		{
			return FALSE;
		} //if..else if..else
		//if
	}
		
	m_EnchantCard.SetCardItem( pItemDat );
	return TRUE;
} //CW3DUInterface::SelectedEnchantItem

void CW3DUInterface::UseSmithEnchant()
{
	SItemData*		pCardItem = m_EnchantSmithCard.GetCardItem();
	SItemData*		pItem = m_EnchantSmithItem;
	
	if( !pCardItem && !pItem )
	{
		ChattingInput( "", STR_ENCHANT_NOT, FONT_MSGCOLOR, 0, 0  );
//		Assert( !"인첸트 아이템 없다!!!" );
		return;
	}

	DWORD		dwParam1 = pItem->nId;
	DWORD		dwParam2 =  pCardItem->nId;
	
	PushEvent( "SMITH_ENCHANT", EVENT_SMITH_ENCHENT, dwParam1, dwParam2 );
}

  /** 
 * 인첸트 사용
 */
void CW3DUInterface::UseEnchantItem( SItemData* pItemDat )
{
	Assert( pItemDat );

	SItemData*		pCardItem = m_EnchantCard.GetCardItem();
	if( !pCardItem )
	{
		Assert( !"카드가 없다" );
		return;
	} //if
	if( pItemDat->nId == pCardItem->nId )
	{
		return;
	} //if

//	if( _access( "Interface/HouseRade.w3d", 0 ) != -1 ) 
//	{
//		return;
//	}
	SItemInfoData*	pItemInfoDat = pItemDat->pItemInfoData;
	Assert( pItemInfoDat );
	BOOL	EnchantTest = TRUE;
	switch( pCardItem->nType )
	{
	case ITEM_ENCHANT_WEAPON:	
		if( !ISWEAPONITEM( pItemInfoDat->btItemType ) ) //여기서 말의 상태 체크
		{
			m_EnchantCard.Cleanup();
			ShowDlg( "DLG_ENCHANT", FALSE );
			EnchantTest = FALSE;
			ChattingInput( "", STR_UIMSG_AGAINENCHANTWEAPON, FONT_MSGCOLOR, 0, 0 );
			return;
		}
		else if( pItemInfoDat->bItemIndex < 8 )
		{
			m_EnchantCard.SetEnchantItem( pItemDat );
			EnchantTest = TRUE;
			MessageBox( STR_UIMSG_ENCHANTCONFIRM, "ENCHANTCONFIRM", MSG_YESNO );
			return;
		} //if..else
		break;

	case ITEM_ENCHANT_ARMOR:
		if( !ISDEFENSEITEM( pItemInfoDat->btItemType ) ) 
		{
			m_EnchantCard.Cleanup();
			ShowDlg( "DLG_ENCHANT", FALSE );
			EnchantTest = FALSE;
			ChattingInput( "", STR_UIMSG_AGAINENCHANTARMOR, FONT_MSGCOLOR, 0, 0 );
			return;
		} 
		else if( pItemInfoDat->bItemIndex < 6 )
		{
			m_EnchantCard.SetEnchantItem( pItemDat );
			EnchantTest = TRUE;
			MessageBox( STR_UIMSG_ENCHANTCONFIRM, "ENCHANTCONFIRM", MSG_YESNO );
			return;
		} //if..else
		break;

	case ITEM_ENCHANT_OPTION:
		if( !ISWEAPONITEM( pItemInfoDat->btItemType ) && !ISDEFENSEITEM( pItemInfoDat->btItemType ) )
		{
			m_EnchantCard.Cleanup();
			ShowDlg( "DLG_ENCHANT", FALSE );
			EnchantTest = FALSE;
			ChattingInput( "",STR_ENCHANT_ITEM, FONT_MSGCOLOR, 0, 0 );
			return;
		}
		else
		{
			EnchantTest = TRUE;
		}
		//m_EnchantCard.SetEnchantItem( pItemDat );		
		break;

	default:
		if( ISENCHANTCHANGE( pCardItem->nType ) )
		{
			PushEvent( "ITEM_ENCHANT_CHANGE", EVENT_ENCHANT_CHANGE, pItemDat->nId, 0 );
			EnchantTest = TRUE;
		}
		else if( ISENCHANTCOLLECTION( pCardItem->nType ) )
		{
			PushEvent( "ITEM_ENCHANT_COLLECTION", EVENT_ENCHANT_COLLECTION, pItemDat->nId, 0 );
			EnchantTest = TRUE;
		}
		else
		{
			Assert( !"없는 인첸트 타입" );
		} //if..else if..else
		m_EnchantCard.Cleanup();
		return;
	} //switch
	
	if( EnchantTest == TRUE )
	{
		m_EnchantCard.SetEnchantItem( pItemDat );
		m_EnchantCard.Begin();
		ShowDlg( "DLG_ENCHANT", TRUE );
	}
} //CW3DUInterface::UseEnchantItem

/**
 * 인벤토리 정렬
 */
void CW3DUInterface::InitInventory()
{
	int		nItemNum  = INVENTORY_PAGE * INVENTORY_ITEMNUM;
	WORD*	pItemBuff = (WORD*)_MemAlloc( sizeof( WORD ) * nItemNum );
	if( !pItemBuff )
	{
		Assert( !"할당실패" );
		return;
	} //if

	if( LoadInventoryData( pItemBuff, nItemNum ) )
	{
		SItemData*		pItemDat;
		SItemData*		pTempItemDat;
		UIInventoryIN*	pInventory[INVENTORY_PAGE];
		BYTE			btPage, btSavePage;
		BYTE			btInvenIdx, btSaveInvenIdx;

		for( btPage = 0; btPage < INVENTORY_PAGE; btPage++ )
		{
			pInventory[btPage] = GetInventoryIN( "INVENTORY_IN_", btPage + 1 );
			if( !pInventory[btPage] )
			{
				SAFE_FREE( pItemBuff );
				return;
			} //if
		} //for

		for( WORD btItemCount = 0; btItemCount < nItemNum; btItemCount++ )
		{ // 읽어온 아템 개수만큼
			for( BYTE btTotInvenIdx = btItemCount; btTotInvenIdx < nItemNum; btTotInvenIdx++ )
			{ //인벤토리 내 아템 개수 만큼
				btPage		= btTotInvenIdx / INVENTORY_ITEMNUM;
				btInvenIdx	= btTotInvenIdx % INVENTORY_ITEMNUM;
				Assert( btPage < INVENTORY_PAGE );
				Assert( btInvenIdx < INVENTORY_ITEMNUM );

				pItemDat = pInventory[btPage]->GetItem( btInvenIdx );
				if( pItemDat )
				{
					if( pItemBuff[btItemCount] )
					{
						if( pItemDat->nType == pItemBuff[btItemCount] )
						{
							pItemDat = pInventory[btPage]->DeleteItem( btInvenIdx );
							if( pItemDat )
							{ //아이템이 존재
								btSavePage		= btItemCount / INVENTORY_ITEMNUM;
								btSaveInvenIdx	= btItemCount % INVENTORY_ITEMNUM;

								if( !pInventory[btSavePage]->GetBlankNum() )
								{
									if( pInventory[btPage]->GetBlankNum() )
									{
										pTempItemDat = pInventory[btSavePage]->DeleteItem( btSaveInvenIdx );
										pInventory[btPage]->InsertItem( btInvenIdx, pTempItemDat );
										
//										pTempItemDat->pimgItem->Draw(  )
									}
									else
									{
										Assert( !"공간이 없다" );
									} //if..else
								} //if

								if( !pInventory[btSavePage]->InsertItem( btSaveInvenIdx, pItemDat ) )
								{
									Assert( !"아이템 넣기 실패" );
								} //if
							} //if
							break;
						} //if
					}
					else
					{
						btSavePage	= btPage + 1;
						if( btSavePage < INVENTORY_PAGE )
						{
							if( !pInventory[btSavePage]->GetBlankNum() )
							{
								Assert( !"공간이 없다" );
							} //if

							pItemDat	= pInventory[btPage]->DeleteItem( btInvenIdx );
							if( !pInventory[btSavePage]->InsertItem( pItemDat ) )
							{
								Assert( !"아이템 넣기 실패" );
							} //if
						} //if
					} //if..else
				} //if
			} //while
		} //for
	} //if

	SAFE_FREE( pItemBuff );
} //CW3DUInterface::InitInventory
