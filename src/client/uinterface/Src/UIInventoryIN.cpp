/**
 * 유저 인터페이스용 인벤토리 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:07a $
 * @version	$Revision: 31 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIInventoryIN.cpp $
 * 
 * 31    04-12-08 3:07a Sk8snow2
 * 
 * 30    04-09-10 11:28p Paran
 * 
 * 29    04-05-14 4:31p Paran
 * 
 * 28    04-05-10 11:24p Paran
 * 
 * 27    04-01-10 1:00a Paran
 * 
 * 20    03-09-15 1:42a Paran			Quick Slot 포션 갯수 수정.
 * 
 * 9     03-06-23 2:25a Paran			SetPlayerPower 추가.
 * 
 * 6     03-06-18 10:16p Paran			인벤토리 손 바꾸기, 캐릭터 선택 창 수정
 * 
 * 4     03-05-25 4:10p Paran			물약 다량 구입 인터페이스 추가
 * 
 * 2     03-05-17 4:20a Paran			각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIInventoryIN.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIInventoryIN.h"

/**
 * 생성자.
 */
UIInventoryIN::UIInventoryIN()
:	m_pItemInfoMgr	( NULL ),
	m_nCursorX		( -1 ),
	m_nCursorY		( -1 ),
	m_pFocusItemDat ( NULL ),
	m_pRectangle	( NULL )
{
	m_bShow					= FALSE;
	m_nMode					= 0;
} //UIInventoryIN::UIInventoryIN

/**
 * 소멸자.
 */
UIInventoryIN::~UIInventoryIN()
{
	SAFE_RELEASE( m_pRectangle );
} //UIInventoryIN::~UIInventoryIN

/**
 * 초기화.
 */
BOOL UIInventoryIN::Init( 
	const int nCol, 
	const int nRow, 
	const int nGap,
	const IW3DItemInfoMgr* pItemInfoMgr 
	)
{
	m_vecItemRect.clear();

	m_pItemInfoMgr = const_cast<IW3DItemInfoMgr*>( pItemInfoMgr );

	SAFE_RELEASE( m_pRectangle );
	m_pRectangle = MAKE_COMPONENT( Rectangle );
	Assert( m_pRectangle );

	m_nCol = nCol;
	m_nRow = nRow;

	ItemInRect		itemRect;
	for( int nY = 0; nY < m_nRow; nY++ )
	{
		for( int nX = 0; nX < m_nCol; nX++ )
		{
			itemRect.rcItem.left	= m_rcRegion.left + ( ( SIZE_ICON_WIDTH + nGap ) * nX );
			itemRect.rcItem.top		= m_rcRegion.top + ( (SIZE_ICON_HEIGHT + nGap ) * nY );
			itemRect.rcItem.right	= itemRect.rcItem.left + SIZE_ICON_WIDTH;
			itemRect.rcItem.bottom	= itemRect.rcItem.top + SIZE_ICON_HEIGHT;
			itemRect.pItemData = NULL;

			m_vecItemRect.push_back( itemRect );
		} //for
	} //for

	return TRUE;
} //UIInventoryIN::Init

/**
 * 클린업.
 */
void UIInventoryIN::Cleanup()
{
	UICComponent::Cleanup();
	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		m_vecItemRect[nCnt].pItemData = NULL;
	} //for
	m_pFocusItemDat = NULL;
} //UIInventoryIN::Cleanup

/**
 * 창 이동.
 */
void UIInventoryIN::Move( int nOffsetX, int nOffsetY )
{
	UICComponent::Move( nOffsetX, nOffsetY );
	
	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		m_vecItemRect[nCnt].rcItem.left		+= nOffsetX;
		m_vecItemRect[nCnt].rcItem.top		+= nOffsetY;
		m_vecItemRect[nCnt].rcItem.right	+= nOffsetX;
		m_vecItemRect[nCnt].rcItem.bottom	+= nOffsetY;
	} //for
} //UIInventoryIN::Move

/**
 * 가로, 세로 칸 설정.
 */
void UIInventoryIN::SetRowCol( int nRow_, int nCol_ )
{
	m_nRow = nRow_;
	m_nCol = nCol_;
} //UIInventoryIN::SetRowCol

/**
 * 가로, 세로 칸 설정.
 */
void UIInventoryIN::GetRowCol( int& nRow, int& nCol )
{
	nRow = m_nRow;
	nCol = m_nCol;
} //UIInventoryIN::SetRowCol

/**
 * 아이템 추가
 */
BOOL UIInventoryIN::InsertItem( int nX, int nY, SItemData* pItemData )
{
/*	POINT		ptCheck = { nX, nY };

	int			nIndex = 0;
	while( nIndex < m_vecItemRect.size() )
	{
		if( PtInRect( &m_vecItemRect[nIndex].rcItem, ptCheck ) )
		{
			InsertItem( nIndex, pItemData );
			return TRUE;
		} //if
		nIndex++;
		} //for
	return FALSE;*/

	if( !pItemData )
	{
		Assert( !"뭐하는겨" );
		return FALSE;
	} //if
	
	int		nIndex = ( nY * m_nCol ) + nX;
	if( m_vecItemRect[nIndex].pItemData )
	{
//		Assert( !"이미 있는 있는 자리" );
		return TRUE;
	} //if
	
	m_vecItemRect[nIndex].pItemData = pItemData;
	
	return FALSE;
} //UIInventoryIN::InsertItem

/**
 * 아이템 추가.
 */
BOOL UIInventoryIN::InsertItem( BYTE btIndex, SItemData* pItemData )
{
	if( !pItemData || m_vecItemRect.empty() )
	{
		Assert( !"뭐하는겨" );
		return FALSE;
	} //if

	if( btIndex >= m_vecItemRect.size() )
	{
		Assert( !"오버요" );
		return FALSE;
	} //if

	if( !GetBlankNum() )
	{ //남은 공간 확인, (주의!!!)
		Assert( !"풀이요" );
		return FALSE;
	} //if
	
	if( m_vecItemRect[btIndex].pItemData )
	{ //현재 아템 있는 위치
		SItemData*		pTempItemData1   = m_vecItemRect[btIndex].pItemData;
		m_vecItemRect[btIndex].pItemData = pItemData;

		UINT	nCurr = btIndex + 1;
		while( nCurr < m_vecItemRect.size() )
		{
			if( !pTempItemData1 )
			{
				return TRUE;
			} //if

			SItemData*		pTempItemData2 = m_vecItemRect[nCurr].pItemData;
			m_vecItemRect[nCurr].pItemData = pTempItemData1;
			pTempItemData1 = pTempItemData2;

			nCurr++;
		} //while
		return TRUE;
	}
	else
	{ //빈 자리
		for( UINT nCnt = 0; m_vecItemRect.size(); nCnt++ )
		{
			if( !m_vecItemRect[nCnt].pItemData )
			{
				m_vecItemRect[nCnt].pItemData = pItemData;
				return TRUE;
			} //if
		} //for
		Assert( !"꽉찬인벤" );
		return FALSE;
	} //if..else

	return FALSE;
} //UIInventoryIN::InsertItem

/**
 * 아이템 추가.
 */
BOOL UIInventoryIN::InsertItem( SItemData* pItemData )
{
	if( !pItemData )
	{
		Assert( !"뭐하는겨" );
		return FALSE;
	} //if

	UINT nCnt = 0;
	while(  nCnt < m_vecItemRect.size() )
	{
		if( !m_vecItemRect[nCnt].pItemData )
		{
			m_vecItemRect[nCnt].pItemData = pItemData;
			return TRUE;
		}
		else if( m_vecItemRect[nCnt].pItemData->nType == ITEM_LINESKIP )
		{
			nCnt = m_nCol * ( ( nCnt / m_nCol ) + 1 );
			continue;
		}
		else if( m_vecItemRect[nCnt].pItemData->nType == ITEM_PAGESKIP )
		{
			return FALSE;
		} //if..else

		nCnt++;
	} //for

	return FALSE;
} //UIInventoryIN::InsertItem

/**
 * 아이템 삭제.
 *
 * @return			성공( -1이 아닌 값 )
 */
int UIInventoryIN::DeleteItem( int nId_ )
{
/*	int		nId;
	int		nType;
	int		nItemRectNum = m_vecItemRect.size();

	for( UINT nCnt = 0; nCnt < nItemRectNum; nCnt++ )
	{
		if( m_vecItemRect[nCnt].pItemData )
		{
			nId = m_vecItemRect[nCnt].pItemData->nId;
			if( nId == nId_ )
			{
				nType = m_vecItemRect[nCnt].pItemData->nType;
				m_pFocusItemDat = NULL;

				UINT	nCurr = nCnt;
				while( nCurr < nItemRectNum - 1 )
				{
					m_vecItemRect[nCurr].pItemData = m_vecItemRect[nCurr+1].pItemData;
					if( !m_vecItemRect[nCurr].pItemData )
					{
						break;
					} //if
					nCurr++;
				} //while
				m_vecItemRect[nItemRectNum-1].pItemData = NULL;
				return nType;
			} //if
		} //if
	} //for
	
	return -1;*/
	int		nId;
	int		nType;
	
	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		if( m_vecItemRect[nCnt].pItemData )
		{
			nId = m_vecItemRect[nCnt].pItemData->nId;
			if( nId == nId_ )
			{
				nType = m_vecItemRect[nCnt].pItemData->nType;
				m_vecItemRect[nCnt].pItemData = NULL;
				return nType;
			} //if
		} //if
	} //for
	
	return -1;
} //UIInventoryIN::DeleteItem

BOOL UIInventoryIN::GetCoord( int &nX, int &nY )
{
	int			nIndex;
	POINT		ptCheck = { nX, nY };
	for( int i = 0; i < m_nRow; i++ )
	{
		for( int j = 0; j < m_nCol; j++ )
		{
			nIndex = ( i * m_nCol ) + j;
			if( PtInRect( &m_vecItemRect[nIndex].rcItem, ptCheck ) )
			{
				nX = j;
				nY = i;
				return TRUE;
			} //if
		} //for
	} //for
	return FALSE;
}


/**
 * 아이템 삭제
 */
SItemData* UIInventoryIN::DeleteItem( BYTE btIndex )
{
	if( m_vecItemRect.size() <= btIndex )
	{
		Assert( !"인덱스 오류" );
		return NULL;
	} //if

	SItemData*	pItemDat = m_vecItemRect[btIndex].pItemData;
	m_vecItemRect[btIndex].pItemData = NULL;
	return pItemDat;
} //UIInventoryIN::DeleteItem

/**
 * 좌표에 있는 아이템 데이터.
 */
SItemData* UIInventoryIN::GetItem( int nX, int nY )
{
	POINT		ptCheck = { nX, nY };

	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		if( PtInRect( &m_vecItemRect[nCnt].rcItem, ptCheck ) )
		{
			SItemData*	pItemData = m_vecItemRect[nCnt].pItemData;
			if( pItemData && ISSKIPITEM( pItemData->nType) )
			{
				return NULL;
			} //if
			return pItemData;
		} //if
	} //for

	return NULL;
} //UIInventoryIN::GetItem

/**
 * 타입으로 아이템얻기.
 */
SItemData* UIInventoryIN::GetItem( int nType )
{
	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		if( m_vecItemRect[nCnt].pItemData )
		{
			SItemData*		pItemDat = m_vecItemRect[nCnt].pItemData;
			if( pItemDat->nType == nType )
			{
				return pItemDat;
			} //if
		} //if
	} //for
	return NULL;
} //UIInventoryIN::GetItem

/**
 * 인덱스로 아템정보 얻기
 */
SItemData* UIInventoryIN::GetItem( BYTE btIndex )
{
	if( m_vecItemRect.size() <= btIndex )
	{
		Assert( !"인덱스 오류" );
		return NULL;
	} //if

	return m_vecItemRect[btIndex].pItemData;
} //UIInventoryIN::GetItem

/**
 * 특정 아이템갯수를 얻는다.
 */
int	UIInventoryIN::GetItemNum( const int nType )
{
	int			nTypeNum = 0;
	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		if( m_vecItemRect[nCnt].pItemData )
		{
			if( nType > 0 )
			{
				SItemData*		pItemDat = m_vecItemRect[nCnt].pItemData;
				if( pItemDat->nType == nType )
				{
					if( pItemDat->btCount > 0 )		nTypeNum += pItemDat->btCount;
					else							nTypeNum++;
				} //if
			}
			else
			{
				nTypeNum++;
			} //if..else
		} //if
	} //for

	return nTypeNum;
} //UIInventoryIN::GetItemNum

/**
 * 남은 공간 개수 얻는다.
 */
int	UIInventoryIN::GetBlankNum( WORD* pwX, WORD* pwY )
{
	int		nSpace = 0;
	for( UINT nCnt = 0 ; nCnt < m_vecItemRect.size() ; nCnt++ )
	{
		if( !m_vecItemRect[nCnt].pItemData )
		{
			if( pwX && pwY && nSpace == 0 )
			{
				*pwX = nCnt % m_nCol;
				*pwY = nCnt / m_nCol;
			} //if
			nSpace++;
		} //if
	} //for
	return nSpace;
} //UIInventoryIN::GetBlankNum

/**
 *
 */
BOOL UIInventoryIN::PointInItemRects( const int nX, const int nY )
{
	POINT						ptCheck = { nX, nY };

	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		if( PtInRect( &m_vecItemRect[nCnt].rcItem, ptCheck ) )
		{
			if( m_vecItemRect[nCnt].pItemData )
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			} //if..else
		} //if
	} //for

	return FALSE;
} //UIInventoryIN::PtInItemRects

/**
 * 마우스 이동.
 */
void UIInventoryIN::MouseMove( int nMouseX, int nMouseY )
{
	m_nCursorX = nMouseX;
	m_nCursorY = nMouseY;

	m_pFocusItemDat = GetItem( nMouseX, nMouseY );
} //UIInventoryIN::MouseMove

/**
 * 현재 플래이어 스킬레벨.
 */
void UIInventoryIN::SetPlayerPower( const int nType, const int nLevel )
{
	Assert( nType < 3 && nType >= 0 );
	m_anPlayer_Skill[nType] = nLevel;
} //UIInventoryIN::SetKillLevel

/**
 * 아이템 팁 정보 보이기.
 */
void UIInventoryIN::DrawTipInfo()
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
			m_pFont,
			TRUE,
			m_nMode,
			m_pFocusItemDat->nSellPrice
			);
	} //if
} //UIInventoryIN::DrawTipInfo

/**
 * 그리기.
 */
void UIInventoryIN::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	Assert( m_pRectangle );

	char			szCount[8];
	RECT			rcCount;
	int				nX, nY;
	SItemData*		pItemData;
	IW3DSprite*		pSpriteItem;
	for( UINT nCnt = 0; nCnt < m_vecItemRect.size(); nCnt++ )
	{
		pItemData	= m_vecItemRect[nCnt].pItemData;
		if( pItemData )
		{
			pSpriteItem = pItemData->pimgItem;
			if( pSpriteItem )	
			{
				nX = m_vecItemRect[nCnt].rcItem.left;
				nY = m_vecItemRect[nCnt].rcItem.top;
				pSpriteItem->Draw( nX, nY );

				if( pItemData->btCount > 0 )
				{ // 아이템이 하나 이상.
					rcCount.left	= m_vecItemRect[nCnt].rcItem.left;
					rcCount.top		= m_vecItemRect[nCnt].rcItem.bottom - 12;
					rcCount.right	= rcCount.left + 25;
					rcCount.bottom  = m_vecItemRect[nCnt].rcItem.bottom;

				//	m_pRectangle->Draw( rcCount.left, rcCount.top, 25, 12, 0xFFFFFFFF );

					sprintf( szCount, "%d", pItemData->btCount );
					m_pFont->DrawText( szCount, &rcCount, DT_LEFT | DT_VCENTER, 0xFF00FF00 );
					
				} //if
			} //if
		} //if
	} //while
} //UIInventoryIN::Draw
