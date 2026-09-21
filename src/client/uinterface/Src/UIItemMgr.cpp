/**
 * 아이템 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-05-25 1:17a $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIItemMgr.cpp $
 * 
 * 13    04-05-25 1:17a Paran
 * 
 * 12    04-05-14 4:31p Paran
 * 
 * 11    04-05-10 11:24p Paran
 * 
 * 6     03-12-09 5:58p Paran			아이템 데이터 수정
 * 
 * 9     03-05-06 6:18p Paran			상점기능 추가
 * 
 * 7     03-04-23 8:36p Paran			마법창.
 *
 * 4     03-03-26 9:19p Paran			인벤토리 어색한 부분 수정하고 체크.
 * <HR>
 *
 * @file	UIItemMgr.cpp
 */


#include "UIItemMgr.h"


/**
 * 생성자.
 */
CUIItemMgr::CUIItemMgr()
:	m_pHoldItemData( NULL ),
	m_pItemInfoMgr( NULL ),
	m_btSkillInfo( FALSE )
{
} //CUIItemMgr::CUIItemMgr

/**
 * 소멸자.
 */
CUIItemMgr::~CUIItemMgr()
{
	Cleanup();
} //CUIItemMgr::~CUIItemMgr

/** 
 * 클린업.
 */
void CUIItemMgr::Cleanup()
{
	MAP_UIITEMDATA::iterator		itor;
	MAP_UIITEMDATA::iterator		itorNext;

	itor = m_mapItemData.begin();
	while( itor != m_mapItemData.end() )
	{
		itorNext = itor;
		itorNext++;

		SItemData*		pItemData = itor->second;
		SAFE_DELETE( pItemData );

		m_mapItemData.erase( itor );

		itor = itorNext;
	} //while

	ClearItemInTemp();
	
	m_pHoldItemData	= NULL;
	m_btHandFrom	= 0;
} //CUIItemMgr::Cleanup

void CUIItemMgr::ClearItemInTemp()
{
	MAP_UIITEMDATA::iterator		itor;
	MAP_UIITEMDATA::iterator		itorNext;

	itor = m_mapItemDataInTemp.begin();
	while( itor != m_mapItemDataInTemp.end() )
	{
		itorNext = itor;
		itorNext++;

		SItemData*		pItemData = itor->second;
		SAFE_DELETE( pItemData );

		m_mapItemDataInTemp.erase( itor );

		itor = itorNext;
	} //while
} //CUIItemMgr::ClearItemInTemp

/**
 * 아이템 인포 메니져 설정
 */
void CUIItemMgr::SetItemInfoMgr( IW3DItemInfoMgr* pItemInfoMgr )
{
	Assert( pItemInfoMgr );
	m_pItemInfoMgr = pItemInfoMgr;
} //CUIItemMgr::SetItemInfoMgr

/**
 * 아이템을 추가한다.
 *
 * @return	아이템 추가 성공 여부.
 */
BOOL CUIItemMgr::InsertItem(
	const int		nId,			///< [IN] 아이템 아이디
	const int		nType,			///< [IN] 아이템 타입 인덱스
	SItemInfoData	*pItemInfo,		///< [IN] 아이템 정보
	const WORD		btCount,		///< [IN] 아이템 개수
	const int		nSellPrice		///< [IN] 아이템 가격
	)
{
	if( !pItemInfo )
	{
		Assert( pItemInfo );
		return FALSE;
	}
	MAP_UIITEMDATA::iterator	itor = m_mapItemData.find( nId );
	if( itor != m_mapItemData.end() )
	{	// 이미 존재하는 아이템일 경우 FALSE 리턴을 한다.
		return FALSE;
	} //if

	SItemData*		pItemData = new SItemData;
	CHECK_PTR_RETURN( pItemData );

	pItemData->nId				= nId;
	pItemData->nType			= nType;
	pItemData->btCount			= btCount;
	pItemData->pItemInfoData	= pItemInfo;
	pItemData->nSellPrice		= nSellPrice;

	char*		pFileName;

	if( !pItemInfo->pSkillData )
	{
		pFileName = ITEMFILENAME4( m_pItemInfoMgr->GetSex(), pItemInfo );
	}
	else
	{
		switch( pItemInfo->pSkillData->shSkillType ) 
		{
		case 5:
		case 7:
			if( GetCheckSkillInfo() == TRUE )
			{
				pFileName = ITEMFILENAME5( m_pItemInfoMgr->GetSex(), pItemInfo );	//버프 스킬 이미지 관련...
				SetCheckSkillInfo( FALSE );
			}
			else
				pFileName = ITEMFILENAME4( m_pItemInfoMgr->GetSex(), pItemInfo );	//버프 스킬 이미지 관련...
			break;
		default:
			pFileName = ITEMFILENAME4( m_pItemInfoMgr->GetSex(), pItemInfo );
		}
	}

	if( *pFileName != '\0' )
	{
		string	strFileName = string( "Item/" ) + pFileName;

		pItemData->pimgItem = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( pItemData->pimgItem );

		if( !pItemData->pimgItem->Load( strFileName.c_str(), SIZE_ICON_WIDTH, SIZE_ICON_HEIGHT ) )
		{
  			Assert( !"아이템 이미지 읽기 실패" );
			DEBUG_OUT( "아이템 아이콘 읽기 실패 - " + strFileName.c_str() );
			SAFE_DELETE( pItemData );
			return FALSE;
		} //if
		pItemData->pimgItem->SetSize( SIZE_ICON_WIDTH, SIZE_ICON_HEIGHT );
	} //if

	m_mapItemData.insert( std::make_pair( nId, pItemData ) );

	return TRUE;
} //CUIItemMgr::InsertItem

/**
 * 임시아이템을 추가한다.
 *
 * @return	아이템 추가 성공 여부.
 */
BOOL CUIItemMgr::InsertItemInTemp(
	const int		nId,			///< [IN] 아이템 아이디
	const int		nType,			///< [IN] 아이템 타입 인덱스
	SItemInfoData	*pItemInfo,		///< [IN] 아이템 정보
	WORD			btCount,		///< [IN] 아이템 개수
	const int		nSellPrice		///< [IN] 아이템 가격
	)
{
	MAP_UIITEMDATA::iterator	itor = m_mapItemDataInTemp.find( nId );
	if( itor != m_mapItemDataInTemp.end() )
	{	// 이미 존재하는 아이템일 경우 FALSE 리턴을 한다.
		if( !ISSKIPITEM( nType ) )
		{
			Assert( !"등록되지 않은 아이템" );
			return FALSE;
		} //if
	} //if

	SItemData*	pItemData = new SItemData;
	CHECK_PTR_RETURN( pItemData );
	pItemData->nId				= nId;
	pItemData->nType			= nType;
	pItemData->btCount			= btCount;
	pItemData->pItemInfoData	= pItemInfo;
	pItemData->nSellPrice		= nSellPrice;

	if( !ISSKIPITEM( nType ) )
	{
		char*		pFileName = ITEMFILENAME4( m_pItemInfoMgr->GetSex(), pItemInfo );
		if( *pFileName != '\0' )
		{
			string	strFileName = string( "Item/" ) + pFileName;

			pItemData->pimgItem = MAKE_COMPONENT( Sprite );
			CHECK_PTR_RETURN( pItemData->pimgItem );

			if( !pItemData->pimgItem->Load( strFileName.c_str(), SIZE_ICON_WIDTH, SIZE_ICON_HEIGHT ) )
			{
				DEBUG_OUT( "아이템 이미지 읽기 실패 ( ImageName3 ) - " + strFileName.c_str() );
				SAFE_DELETE( pItemData );
				return FALSE;
			} //if

			pItemData->pimgItem->SetSize( SIZE_ICON_WIDTH, SIZE_ICON_HEIGHT );
		} //if
	} //if

	m_mapItemDataInTemp.insert( std::make_pair( nId, pItemData ) );

	return TRUE;
} //CUIItemMgr::InsertItem

/**
 * 아이템을 찾는다.
 */
SItemData* CUIItemMgr::FindItem( int nId, BOOL bTemp )
{
	SItemData*					pItemData = NULL;
	MAP_UIITEMDATA::iterator	iterMap_ItemData;

	if( !bTemp )
	{
		iterMap_ItemData = m_mapItemData.find( nId );
		if( iterMap_ItemData == m_mapItemData.end() )
		{
			return NULL;
		} //if
		pItemData = iterMap_ItemData->second;
	}
	else
	{
		iterMap_ItemData = m_mapItemDataInTemp.find( nId );
		if( iterMap_ItemData == m_mapItemDataInTemp.end() )
		{
			return NULL;
		} //if
		pItemData = iterMap_ItemData->second;
	}

	return pItemData;
} //CUIItemMgr::FindItem

/**
 * 손에 쥔 아이템을 얻는다.
 */
SItemData* CUIItemMgr::GetHandInItemDat()
{
	return m_pHoldItemData;
} //CUIItemMgr::GetHandInItemDat

/**
 * 아이템을 손에 쥔다.
 */
void CUIItemMgr::SetHandInItemDat( SItemData* pItemData, BYTE btTo )
{
	if( pItemData && ISSKIPITEM( pItemData->nType ) )
	{
		return;
	} //if
	m_btHandFrom	= btTo;
	m_pHoldItemData = pItemData;
} //CUIItemMgr::GetHandInItemDat

/**
 * 손에 쥔 아템 어디서..
 */
BYTE CUIItemMgr::FromHandItem()
{
	return m_btHandFrom;
} //CUIItemMgr::FromHandItem

/**
 * 아이템을 손에 쥔다.
 */
/** 삭제 **
BOOL CUIItemMgr::SetHandInItem( int nId, BOOL bTemp )
{
	if( nId < 0 )
	{
		m_pHoldItemData = NULL;
		return TRUE;
	} //if

	SItemData*		pItemData = FindItem( nId, bTemp );
	if( !pItemData )
	{
		Assert( !"없는 아이템" );
		return FALSE;
	}

	m_pHoldItemData = pItemData;
	return TRUE;
} //CUIItemMgr::SetHandInItem
**/

/**
 * 손에 쥔 아이템을 그린다.
 */
void CUIItemMgr::Draw( const int nX, const int nY )
{
	if( m_pHoldItemData && m_pHoldItemData->pimgItem )
	{
		Assert( m_pHoldItemData->pimgItem );
		m_pHoldItemData->pimgItem->Draw( nX, nY );
	} //if
} //CUIItemMgr::Draw
