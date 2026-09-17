/**
 * 퀵슬롯 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:07a $
 * @version	$Revision: 15 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIQuickSlot.cpp $
 * 
 * 15    04-12-08 3:07a Sk8snow2
 * 
 * 14    04-09-10 11:28p Paran
 * 
 * 10    03-11-26 9:16p Paran			퀵 슬롯 번호 출력
 * 
 * 7     03-09-15 1:42a Paran			Quick Slot 포션 갯수 수정.
 * 
 * <HR>
 *
 * @file	UIQuickSlot.cpp
 */

#include <vector>
#include "UIComponent.h"
#include "UIProgress.h"
#include "UIQuickSlot.h"

#include <MMSYSTEM.H>

#pragma comment( lib, "winmm.lib" )

/**
 * 생성자.
 */
UIQuickSlot::UIQuickSlot()
:	m_pFont			( NULL ),
	m_pRectangle	( NULL )
{
	m_dwTipShowTime = 0;
	
	for( int i = 0; i < dMAX_QUICK_TIPINFO_LINE; i++ )
		*m_szTipStr[i] = NULL;

	m_nTipX = 0;
} //UIQuickSlot::UIQuickSlot

/**
 * 소멸자.
 */
UIQuickSlot::~UIQuickSlot()
{
	SAFE_RELEASE( m_pRectangle );

	for( UINT nCnt = 0; nCnt < m_vecQuick.size(); nCnt++ )
	{
		SAFE_DELETE( m_vecQuick[nCnt].pTimer );
	} //for
} //UIQuickSlot::~UIQuickSlot

/**
 * 클린업.
 */
void UIQuickSlot::Cleanup()
{
	for( UINT nCnt = 0; nCnt < m_vecQuick.size(); nCnt++ )
	{
		m_vecQuick[nCnt].pItemData = NULL;
	} //for
} //UIQuickSlot::Cleanup

/**
 * 퀵 아이콘 정보 설정.
 */
void UIQuickSlot::SetUintPosInto( const int nNum, const int nGap )
{
	m_vecQuick.clear();
	m_vecQuick.resize( nNum );

	Cleanup();
	for( int nCnt = 0; nCnt < nNum; nCnt++ )
	{
		m_vecQuick[nCnt].rcPos.left		= nCnt * ( SIZE_ICON_WIDTH + nGap ) + m_rcRegion.left;
		m_vecQuick[nCnt].rcPos.top		= m_rcRegion.top;
		m_vecQuick[nCnt].rcPos.right    = m_vecQuick[nCnt].rcPos.left + SIZE_ICON_WIDTH;
		m_vecQuick[nCnt].rcPos.bottom   = m_vecQuick[nCnt].rcPos.top + SIZE_ICON_HEIGHT;

		m_vecQuick[nCnt].pTimer = NULL;
		m_vecQuick[nCnt].pTimer = new UIProgressCircular;
		if( !m_vecQuick[nCnt].pTimer )
		{
			Assert( !"It failed" );
			continue;
		} //if
		m_vecQuick[nCnt].pTimer->SetPosition( m_vecQuick[nCnt].rcPos.left, m_vecQuick[nCnt].rcPos.top );
	} //for

	SAFE_RELEASE( m_pRectangle );
	m_pRectangle = MAKE_COMPONENT( Rectangle );
	if( !m_pRectangle )
	{
		Assert( !"할당실패" );
		return;
	} //if
} //UIQuickSlot::SetUintPosInto

/**
 * 아이템 삽입.
 */
void UIQuickSlot::InsertSlot( const int nNumSlot, const SItemData* pItemData )
{
	if( nNumSlot < 0 || nNumSlot >= m_vecQuick.size() )
	{
		return;
	} //if

	m_vecQuick[nNumSlot].pItemData   = const_cast<SItemData*>( pItemData );
} //UIQuickSlot::InsertItem

/**
 * 아이템 제거.
 */
void UIQuickSlot::RemoveItem( const int nNumSlot )
{
	if( nNumSlot < 0 || nNumSlot >= m_vecQuick.size() )
	{
		return;
	} //if

	m_vecQuick[nNumSlot].pItemData	= NULL;
} //UIQuickSlot::RemoveItem

/**
 * 퀵 정보를 얻는다.
 */
SQuickDat* UIQuickSlot::GetQuickDat( const int nX, const int nY )
{
	int		nSlotNum = GetSlotIndex( nX, nY );
	
	if( nSlotNum < 0 )
	{
		return NULL;
	} //if
	//todo :// 작업을...
	return NULL;// &m_vecQuick[nSlotNum].sData;
} //UIQuickSlot::GetQuickDat

/**
 * 슬롯 선택.
 */
SQuickDat* UIQuickSlot::GetQuickDat( const int nNumSlot )
{
	if( nNumSlot < 0 || nNumSlot >= m_vecQuick.size() )
	{
		return NULL;
	} //if

	//todo :// 작업을...
	return NULL;// &m_vecQuick[nNumSlot].sData;
} //UIQuickSlot::SelectSlot

/**
 * 슬롯 선택.
 */
SItemData* UIQuickSlot::GetItem( const int nNumSlot )
{
	if( nNumSlot < 0 || nNumSlot >= m_vecQuick.size() )
	{
		return NULL;
	} //if
	return m_vecQuick[nNumSlot].pItemData;
} //UIQuickSlot::GetItem

/**
 * 퀵슬롯 번호 얻는다.
 */
int UIQuickSlot::GetSlotIndex( const int nX, const int nY )
{
	POINT	ptPos = { nX, nY };
	for( UINT nCnt = 0; nCnt < m_vecQuick.size(); nCnt++ )
	{
		if( PtInRect( &m_vecQuick[nCnt].rcPos, ptPos ) )
		{
			return nCnt;
		} //if
	} //for
	return -1;
} //UIQuickSlot::GetQuickDat

/**
 * 슬롯의 갯수를 얻는다.
 */
int UIQuickSlot::GetSlotNum()
{
	return m_vecQuick.size();
} //UIQuickSlot::GetSlotIndex

/**
 * 포션개수 설정.
 */
void UIQuickSlot::SetPotionNum( 
	const int  nType,			///< [IN] 아이템 타입.
	const int  nNum				///< [IN] 포션 개수.
	)
{
	switch( nType )
	{
	case ITEM_HP_L:			m_nNumHPPotion[2] = nNum;			break;
	case ITEM_HP_M:			m_nNumHPPotion[1] = nNum;			break;
	case ITEM_HP_S:			m_nNumHPPotion[0] = nNum;			break;
	case ITEM_MP_L:			m_nNumMPPotion[2] = nNum;			break;
	case ITEM_MP_M:			m_nNumMPPotion[1] = nNum;			break;
	case ITEM_MP_S:			m_nNumMPPotion[0] = nNum;			break;
	default:													return;
	} //switch
} //UIQuickSlot::SetPotionNum

/**
 * 포션개수 얻는다.
 */
int UIQuickSlot::GetPotionNum( 
	const int nType				///< [IN] 아이템 타입.
	)
{
	switch( nType )
	{
	case ITEM_HP_L:			return m_nNumHPPotion[2];
	case ITEM_HP_M:			return m_nNumHPPotion[1];
	case ITEM_HP_S:			return m_nNumHPPotion[0];
	case ITEM_MP_L:			return m_nNumMPPotion[2];
	case ITEM_MP_M:			return m_nNumMPPotion[1];
	case ITEM_MP_S:			return m_nNumMPPotion[0];
	default:				return 0;
	} //switch

	return 0;
} //UIQuickSlot::GetPotionNum

/**
 *  
 */
void UIQuickSlot::SetTimer( int nItemId, int nTime )
{
	for( UINT nCnt = 0; nCnt < m_vecQuick.size(); nCnt++ )
	{
		SItemData*		pItemDat = m_vecQuick[nCnt].pItemData;
		if( pItemDat && pItemDat->nType == nItemId )
		{
			UIProgressCircular*			pTimer = m_vecQuick[nCnt].pTimer; 
			if( pTimer )
			{
				pTimer->SetTime( nTime );
			} //if
		} //if
	} //for
} //UIQuickSlot::SetTimer

/**
 * 텍스트 그리기.
 */
void UIQuickSlot::DrawText( SQuick* pQuickDat )
{
	SItemData*		pItemData = pQuickDat->pItemData;
	if( !pItemData )
	{
		Assert( !"아이템 데이터가 없당" );
		return;
	} //if

	char	szCount[8];
	switch( pItemData->nType )
	{
	case ITEM_HP_L:		sprintf( szCount, "%d", m_nNumHPPotion[2] );		break;
	case ITEM_HP_M:		sprintf( szCount, "%d", m_nNumHPPotion[1] );		break;
	case ITEM_HP_S:		sprintf( szCount, "%d", m_nNumHPPotion[0] );		break;
	case ITEM_MP_L:		sprintf( szCount, "%d", m_nNumMPPotion[2] );		break;
	case ITEM_MP_M:		sprintf( szCount, "%d", m_nNumMPPotion[1] );		break;
	case ITEM_MP_S:		sprintf( szCount, "%d", m_nNumMPPotion[0] );		break;
	default:																return;
	} //switch
	
	RECT	rcCount;
	rcCount.left	= pQuickDat->rcPos.left;
	rcCount.right	= rcCount.left + 28;
	rcCount.bottom  = pQuickDat->rcPos.bottom;
	rcCount.top		= rcCount.bottom - 12;

	m_pFont->DrawText( szCount, &rcCount, DT_LEFT | DT_VCENTER, 0xFFFFA60C );
} //UIQuickSlot::DrawText

/*
 *	DrawTipInfo
 */
void UIQuickSlot::DrawTipInfo()
{
	if( !m_dwTipShowTime )
		return;

	if( ( timeGetTime() - m_dwTipShowTime ) > 300 )
	{
		m_dwTipShowTime = 0;
		*m_szTipStr[0] = NULL;
		*m_szTipStr[1] = NULL;
		*m_szTipStr[2] = NULL;
		return;
	}
	else
	{
		if( *m_szTipStr[0] )
		{
			int nWidth, nHeight, nX, nY;
			RECT rcTip;

			nX = m_nTipX;
			nY = g_nScreenHeight - 65;
			
			m_pFont->GetStringSize( &nWidth, &nHeight, m_szTipStr[0] );
			
			rcTip.top = nY;
			rcTip.left = nX;
			rcTip.bottom = rcTip.top + nHeight;
			rcTip.right = 0;

			m_pRectangle->Draw( nX-4, nY-4, nWidth+8, nHeight+8, COLOR_ARGB( 150, 0, 0, 0 ) );
			m_pFont->DrawText( m_szTipStr[0], 
				&rcTip, 
				DT_LEFT | DT_VCENTER,
				COLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
}

void UIQuickSlot::UpdateTipInfo( int nX, char *szStr, char *szStr2, char *szStr3 )
{
//	if( m_dwTipShowTime && ( timeGetTime() - m_dwTipShowTime ) < 300 )
//		return;

	m_nTipX = nX;

	strcpy( m_szTipStr[0], szStr );
	strcpy( m_szTipStr[1], szStr2 );	
	strcpy( m_szTipStr[2], szStr3 );

	m_dwTipShowTime = timeGetTime();
}


/**
 * 업뎃.
 */
void UIQuickSlot::Update( const DWORD dwTick )
{
	for( UINT nCnt = 0; nCnt < m_vecQuick.size(); nCnt++ )
	{
		SItemData*		pItemDat = m_vecQuick[nCnt].pItemData;
		if( m_vecQuick[nCnt].pTimer ) m_vecQuick[nCnt].pTimer->Update( dwTick );
	} //for
} //UIQuickSlot::Update

/**
 * 그리기.
 */
void UIQuickSlot::Draw()
{
	if( !m_bShow )
	{
		return;
	} //if

	RECT			rcNum = m_vecQuick[0].rcPos;
	char			szNum[4];
	int				nX, nY;
	SItemData*		pItemData = NULL;
	for( UINT nCnt = 0; nCnt < m_vecQuick.size(); nCnt++ )
	{
		if( m_vecQuick[nCnt].pItemData )
		{
			pItemData = m_vecQuick[nCnt].pItemData;
			if( pItemData->pimgItem )
			{
				nX = m_vecQuick[nCnt].rcPos.left;
				nY = m_vecQuick[nCnt].rcPos.top;
				pItemData->pimgItem->Draw( nX, nY );

				if( m_vecQuick[nCnt].pTimer) 
				{
					m_vecQuick[nCnt].pTimer->Draw();
				} //if
			} //if
			DrawText( &m_vecQuick[nCnt] );
		} //if

		rcNum.left		= m_vecQuick[nCnt].rcPos.left;
		rcNum.right		= rcNum.left + 15;
		rcNum.top		= m_vecQuick[nCnt].rcPos.top;
		rcNum.bottom	= rcNum.top + 15;
		m_pRectangle->Draw( rcNum.left, rcNum.top, 15, 15, 0x32000000 );
		sprintf( szNum, "%d", nCnt + 1 );
		m_pFont->DrawText( szNum, &rcNum, DT_CENTER | DT_VCENTER, 0XFFC2C2C2 );
	} //for

	DrawTipInfo();
} //UIQuickSlot::Draw
