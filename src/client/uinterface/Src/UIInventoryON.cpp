/**
 * 유저 인터페이스용 인벤토리ON 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 05-02-19 10:36a $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/UIInventoryON.cpp $
 * 
 * 2     05-02-19 10:36a Umenokoji
 * 
 * 1     05-02-18 6:55p Umenokoji	Line 298	마법저항력 얻기 추가
 *
 * 15    04-04-22 11:55a Paran
 * 
 * 14    03-12-24 1:14a Paran
 * 
 * 13    03-12-22 1:08a Paran
 * 
 * 12    03-12-09 5:58p Paran
 * 아이템 데이터 수정
 * 
 * 11    03-10-07 9:07p Paran
 * 
 * 10    03-09-24 7:48p Paran
 * 
 * 9     03-08-13 10:33p Paran
 * 
 * 8     03-07-25 12:05a Paran
 * 
 * 7     03-07-18 4:24a Paran
 * 
 * 6     03-07-16 7:30p Paran
 * 
 * 5     03-06-23 2:25a Paran
 * SetPlayerPower 추가.
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIInventoryON.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIInventoryON.h"

/**
 * 생성자.
 */
UIInventoryON::UIInventoryON()
:	m_pItemInfoMgr	( NULL ),
	m_pFont			( NULL ),
	m_pFocusItemDat ( NULL )
{
} //UIInventoryON::UIInventoryON

/**
 * 소멸자.
 */
UIInventoryON::~UIInventoryON()
{
	m_mapItemRect.clear();
} //UIInventoryON::~UIInventoryON

/**
 * 초기화.
 */
BOOL UIInventoryON::Init( IW3DItemInfoMgr* pItemInfoMgr )
{
	m_pItemInfoMgr = pItemInfoMgr;
	return TRUE;
} //UIInventoryON::Init

/**
 * 클린 업.
 */
void UIInventoryON::Cleanup()
{
	UICComponent::Cleanup();
	
	MAP_ITEMONRECT::iterator	iterMap_ItemOn = m_mapItemRect.begin();
	ItemInRect*					pItem;
	while( iterMap_ItemOn != m_mapItemRect.end() )
	{
		pItem = &( iterMap_ItemOn->second );
		pItem->pItemData = NULL;
		iterMap_ItemOn++;
	} //while
} //UIInventoryON::Cleanup

/**
 * 창 이동.
 */
void UIInventoryON::Move( int nOffsetX, int nOffsetY )
{
	UICComponent::Move( nOffsetX, nOffsetY );

	ItemInRect*		prcItemInRect;
	MAP_ITEMONRECT::iterator	iterMap_ItemRect = m_mapItemRect.begin();
	while( iterMap_ItemRect != m_mapItemRect.end() )
	{
		prcItemInRect = &iterMap_ItemRect->second;
		prcItemInRect->rcItem.left		+= nOffsetX;
		prcItemInRect->rcItem.top		+= nOffsetY;
		prcItemInRect->rcItem.right		+= nOffsetX;
		prcItemInRect->rcItem.bottom	+= nOffsetY;
		
		iterMap_ItemRect++;
	} //while
} //UIInventoryON::Move

/**
 * 장착부분 개수 설정.
 */
void UIInventoryON::SetPartCount( const int nCount )
{ 
	m_nPartCount = nCount;
} //UIInventoryON::SetPartCount

/**
 * 장착부분 개수 얻는다.
 */
const int	UIInventoryON::GetPartCount() const
{
	return m_nPartCount;
} //UIInventoryON::GetPartCount

/**
 * 장착부분 영역을 추가.
 */
BOOL UIInventoryON::AddPartRect( const int nIndex, const int nX, const int nY )
{ 
	ItemInRect			Item;
	int					nPos	= nIndex;

	MAP_ITEMONRECT::iterator	iterMap_Item = m_mapItemRect.find( nPos );
	if( iterMap_Item != m_mapItemRect.end() )
	{
		Assert( !"이런 있다" );
		return FALSE;
	} //if

	ZeroMemory( &Item, sizeof( ItemInRect ) );
	Item.rcItem.left		= nX;
	Item.rcItem.top			= nY;
	Item.rcItem.right		= Item.rcItem.left + SIZE_ICON_WIDTH;
	Item.rcItem.bottom		= Item.rcItem.top + SIZE_ICON_HEIGHT;
	m_mapItemRect.insert( std::make_pair( nPos, Item ) );
	
	return TRUE;
} //UIInventoryON::AddPartRect

/**
 *
 */
int UIInventoryON::GetPosIndex( int nX, int nY, SItemData* pItemData )
{
	if( !pItemData )
	{
		return FALSE;
	} //if

	POINT	ptCheck = { nX, nY };

	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();

	while( itor != m_mapItemRect.end() )
	{
		ItemInRect*		pItem = &( itor->second );

		if( PtInRect( &pItem->rcItem, ptCheck ) )
		{
			if( pItemData->pItemInfoData )
			{
				int			nPart = pItemData->pItemInfoData->nItemKind;
				if( itor->first == nPart )
				{
					return nPart;
				} //if
			} //if
		} //if

		itor++;
	} //while

	return -1;
} //UIInventoryON::GetPosIndex

/**
 * 아이템 장착.
 */
BOOL UIInventoryON::InsertItem(
	const int	nPos,
	SItemData*	pItemData
	)
{
	if( !pItemData || !pItemData->pItemInfoData )
	{
		Assert( !"뭐야" );
		return FALSE;
	} //if

	int			nPart = pItemData->pItemInfoData->nItemKind;

	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.find( nPart );
	if( itor == m_mapItemRect.end() )
	{
 		Assert( !"존재하지 않음" );
		return FALSE;
	} //if
	ItemInRect*		pOldItem = &( itor->second );
	
	if( pOldItem->pItemData )
	{	// 이미 아이템 정보 설정이 된 경우라면 실패한다.
		return FALSE;
	} //if

	pOldItem->pItemData = pItemData;

	return TRUE;
} //UIInventoryON::InsertItem

/**
 * 공격력 얻기.
 */
int UIInventoryON::GetTotDamage()
{
	ItemInRect*		pItemRect = NULL;
	SItemInfoData*	pItemInfo = NULL;;
	int				nDamage = 0;

	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();
	while( itor != m_mapItemRect.end() )
	{
		pItemRect = &( itor->second );
		itor++;
		
		if( !pItemRect || !pItemRect->pItemData )
		{
			continue;
		} //if

		pItemInfo = pItemRect->pItemData->pItemInfoData;
		if( !pItemInfo )
		{
			Assert( !"정보가 없다" );
			continue;
		} //if

		nDamage += pItemInfo->wMaxDamage;
	} //while

	return nDamage;
} //UIInventoryON::GetTotDamage

/**
 * 방어력 얻기.
 */
int UIInventoryON::GetTotDefence()
{
	ItemInRect*		pItemRect = NULL;
	SItemInfoData*	pItemInfo = NULL;;
	int				nDefence  = 0;

	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();
	while( itor != m_mapItemRect.end() )
	{
		pItemRect = &( itor->second );

		if( EQUIP_SHORTWEAPON == itor->first || 
			EQUIP_LONGWEAPON == itor->first )
		{
			itor++;
			continue;
		} 
		itor++;

		if( !pItemRect || !pItemRect->pItemData )
		{
			continue;
		} //if

		pItemInfo = pItemRect->pItemData->pItemInfoData;
		if( !pItemInfo )
		{
			Assert( !"정보가 없다" );
			continue;
		} //if

		nDefence += pItemInfo->wMinDamage;
	} //while

	return nDefence;
} //UIInventoryON::GetTotDefence


/************************************************************************/
/* Name : GetTotMagicDefence(  )                                        */
/* Desc : 마법저항력 얻기 추가				                            */
/* Date : 2005-02-18                Make : Umenokoji                    */
/************************************************************************/
int	UIInventoryON::GetTotMagicDefence(  )
{
	ItemInRect*		pItemRect = NULL;
	SItemInfoData*	pItemInfo = NULL;;
	int				nMagicDefence  = 0;
	
	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();
	while( itor != m_mapItemRect.end() )
	{
		pItemRect = &( itor->second );
		
		if( EQUIP_SHORTWEAPON == itor->first || 
			EQUIP_LONGWEAPON == itor->first )
		{
			itor++;
			continue;
		} 
		itor++;
		
		if( !pItemRect || !pItemRect->pItemData )
		{
			continue;
		} //if
		
		pItemInfo = pItemRect->pItemData->pItemInfoData;
		if( !pItemInfo )
		{
			Assert( !"정보가 없다" );
			continue;
		} //if
		
		nMagicDefence += pItemInfo->wPluseMagicDefense;
	} //while
	
	return nMagicDefence;
}

/**
 * 아이템 장착.
 */
BOOL UIInventoryON::InsertItem(
	const int	nX,			///< [IN]
	const int	nY,			///< [IN]
	SItemData*	pItemData	///< [IN]
	)
{
	if( !pItemData || pItemData->pItemInfoData )
	{
		Assert( !"뭐야" );
		return FALSE;
	} //if

	POINT	ptCheck = { nX, nY };

	int			nPart = pItemData->pItemInfoData->nItemKind;
	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();
	while( itor != m_mapItemRect.end() )
	{
		ItemInRect*		pItem = &( itor->second );
		Assert( pItem );
		if( PtInRect( &pItem->rcItem, ptCheck ) )
		{
			if( itor->first == nPart )
			{
				if( pItem->pItemData )
				{
					return FALSE;
				}
				else
				{
					int		nWidth  = pItem->rcItem.right  - pItem->rcItem.left;
					int		nHeight = pItem->rcItem.bottom - pItem->rcItem.top;

					pItem->pItemData = pItemData;
					return TRUE;
				} //if..else..
			} //if
		} //if

		itor++;
	} //while

	return FALSE;
} //UIInventoryON::InsertItem

/**
 * 아이템 벗기.
 */
int UIInventoryON::DeleteItem( const int nPos )
{
	MAP_ITEMONRECT::iterator	iterMap_ItemOn = m_mapItemRect.find( nPos );
	if( iterMap_ItemOn == m_mapItemRect.end() )
	{
		Assert( !"그럼 부분 없당" );
		return -1;
	} //if

	ItemInRect*		pItem = &( iterMap_ItemOn->second );
	if( !pItem->pItemData )
	{
		Assert( pItem->pItemData );
		return 0;
	} //if

	int		nId = pItem->pItemData->nId;

	pItem->pItemData = NULL;

	return nId;
} //UIInventoryON::DeleteItem

/**
 * 아이템 얻기.
 */
SItemData* UIInventoryON::GetItem(
	const int	nX,		///< [IN]
	const int	nY		///< [IN]
	)
{
	POINT	ptCheck = { nX, nY };

	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();
	while( itor != m_mapItemRect.end() )
	{
		ItemInRect*		pItem = &( itor->second );

		if( PtInRect( &pItem->rcItem, ptCheck ) && pItem->pItemData )
		{
			if( !ISSKIPITEM( pItem->pItemData->nType ) )
			{
				return pItem->pItemData;
			} //if
		} //if

		itor++;
	} //while

	return NULL;
} //UIInventoryON::GetItem

/**
 * 아이템 얻기.
 */
SItemData* UIInventoryON::GetItem( int nType )
{
	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();
	while( itor != m_mapItemRect.end() )
	{
		ItemInRect*		pItem = &( itor->second );
		Assert( pItem );

		if( pItem->pItemData )
		{
			if( pItem->pItemData->nType == nType )
			{
				return pItem->pItemData;
			} //if
		} //if

		itor++;
	} //while

	return NULL;
} //UIInventoryON::GetItem

/**
 * 아이템정보를 얻는다.
 */
SItemData* UIInventoryON::GetItemPart( int nPart )
{
	MAP_ITEMONRECT::iterator	iterMap_ItemOn = m_mapItemRect.find( nPart );
	if( iterMap_ItemOn == m_mapItemRect.end() )
	{
		Assert( !"존재하지 않는 부분이 선택되었습니다." );
		return NULL;
	} //if

	ItemInRect*		pItem = &( iterMap_ItemOn->second );
	Assert( pItem );

	SItemData*		pItemData = pItem->pItemData;
	if( pItemData && ISSKIPITEM( pItemData->nType ) )
	{
		return NULL;
	} //if

	return pItemData;
} //UIInventoryON::GetItemPart

/**
 * 장착 위치에서 아이템 타입을 얻는다.
 */
int UIInventoryON::GetOnItemType( int nPart )
{
	MAP_ITEMONRECT::iterator	iterMap_ItemOn = m_mapItemRect.find( nPart );
	if( iterMap_ItemOn == m_mapItemRect.end() )
	{
		Assert( !"존재하지 않는 부분이 선택되었습니다." );
		return FALSE;
	} //if

	ItemInRect*		pItem = &( iterMap_ItemOn->second );
	Assert( pItem );

	if( pItem->pItemData )
	{
		int		nType = pItem->pItemData->nType;
		return nType;
	} //if

	return 0;
} //UIInventoryON::GetOnItem

/**
 * 마우스 이동.
 */
void UIInventoryON::MouseMove( int nMouseX, int nMouseY )
{
	m_nCursorX = nMouseX;
	m_nCursorY = nMouseY;

	m_pFocusItemDat = GetItem( nMouseX, nMouseY );
} //UIInventoryON::MouseMove

/**
 * 현재 플래이어 스킬레벨.
 */
void UIInventoryON::SetPlayerPower( const int nType, const int nLevel )
{
	Assert( nType < 3 && nType >= 0 );
	m_anPlayer_Skill[nType] = nLevel;
} //UIInventoryIN::SetKillLevel

/**
 * 아이템 팁 정보 보이기.
 */
void UIInventoryON::DrawTipInfo()
{
	if( !m_pFocusItemDat )
	{
		return;
	} //if

	int		nX = m_nCursorX;
	int		nY = m_nCursorY;

	if( m_nCursorX + WIDTH_TIPLINEBOX > g_nScreenWidth )
	{
		nX -= WIDTH_TIPLINEBOX;
	} //if

	if( m_pItemInfoMgr )
	{
		m_pItemInfoMgr->DrawTipInfo( 
			m_anPlayer_Skill, 
			m_pFocusItemDat->pItemInfoData, 
			g_nScreenHeight,
			nX,
			nY, 
			m_pFont 
			);
	} //if
} //UIInventoryON::DrawTipInfo

/**
 * 그리기.
 */
void UIInventoryON::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	int				nX, nY;
	IW3DSprite*		psprItem;
	ItemInRect*		pItem;
	MAP_ITEMONRECT::iterator	itor = m_mapItemRect.begin();
	while( itor != m_mapItemRect.end() )
	{
		pItem = &( itor->second );
		if( pItem && pItem->pItemData )
		{
			psprItem = pItem->pItemData->pimgItem;
			Assert( psprItem );

			nX = pItem->rcItem.left;
			nY = pItem->rcItem.top;
			psprItem->Draw( nX, nY );
		} //if
		itor++;
	} //while
} //UIInventoryON::Draw
