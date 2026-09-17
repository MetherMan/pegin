/**
 * 유저 인터페이스용 매직리스트 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-06-21 12:39p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMagicList.cpp $
 * 
 * 3     03-06-21 12:39p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIMagicList.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIMagicList.h"

UIMagicList::UIMagicList()
	: m_nLineNum ( 0 ),
	  m_pItemInfoMgr	( NULL ),
	  m_pMagicData		( NULL ),
	  m_prcLineRect		( NULL ),
	  m_pFocusMagic		( NULL ),
	  m_pFont		    ( NULL )
{
}

UIMagicList::~UIMagicList()
{
	SAFE_FREE( m_pMagicData );
	SAFE_FREE( m_prcLineRect );
}

void UIMagicList::SetItemInfoMgr( IW3DItemInfoMgr* pItemInfoMgr )
{
	m_pItemInfoMgr = pItemInfoMgr;
}

void UIMagicList::Move( int nOffsetX, int nOffsetY )
{
	UICComponent::Move( nOffsetX, nOffsetY );
	for( int nCnt = 0; nCnt < m_nLineNum; nCnt++ )
	{
		m_prcLineRect[nCnt].left	+= nOffsetX;
		m_prcLineRect[nCnt].top		+= nOffsetY;
		m_prcLineRect[nCnt].right	+= nOffsetX;
		m_prcLineRect[nCnt].bottom	+= nOffsetY;
	}
	
}

int UIMagicList::GetLineNum( int nX, int nY )
{
	POINT		ptPos = { nX, nY };
	for( UINT nCnt = 0; nCnt < m_nLineNum; nCnt++ )		// 해당 좌표 공간을 찾는다.
	{
		if( PtInRect( &m_prcLineRect[nCnt], ptPos ) )
		{
			return nCnt;
		}
	}
	return -1;
}

void UIMagicList::SetLineCount( int nLNum )
{
	m_nLineNum = nLNum;
	m_pMagicData	= (UIMagicData**)_MemAlloc( sizeof( UIMagicData* ) * m_nLineNum );
	Assert( m_pMagicData );
	m_prcLineRect   = (RECT*)_MemAlloc( sizeof( RECT ) * m_nLineNum );
	Assert( m_prcLineRect );

	for( int nCnt = 0; nCnt < m_nLineNum; nCnt++ )
	{
		m_pMagicData[nCnt] = NULL;
		m_prcLineRect[nCnt].left = m_prcLineRect[nCnt].top = 0;
		m_prcLineRect[nCnt].right= m_prcLineRect[nCnt].bottom = 0;
	}
}

int	UIMagicList::GetLineCount()
{
	return m_nLineNum;
}

void UIMagicList::SetLine( int nLine, RECT* prcLine )
{
	Assert( prcLine );
	m_prcLineRect[nLine] = *prcLine;
}

BOOL UIMagicList::InsertMagic( int nLineNum, UIMagicData* pMagicData )
{
	Assert( pMagicData );
	Assert( nLineNum < m_nLineNum );

	if( m_pMagicData[nLineNum] )
	{
		return FALSE;
	}
	m_pMagicData[nLineNum] = pMagicData;
	return TRUE;
}

BOOL UIMagicList::DeleteMagic( int nId )
{
	for( int nCnt = 0; nCnt < m_nLineNum; nCnt++ )
	{
		if( m_pMagicData[nCnt] &&
			m_pMagicData[nCnt]->nIndex == nId )
		{
			m_pMagicData[nCnt] = NULL;
			return TRUE;
		}
	}
	return FALSE;
}

UIMagicData* UIMagicList::GetMagic( int nLineNum )
{
	if( m_nLineNum <= nLineNum || nLineNum < 0 )
	{
		return NULL;
	}
	return m_pMagicData[nLineNum];
}

/**
 * 마우스 움직임.
 */
void UIMagicList::MouseMove( int nMouseX, int nMouseY )
{
	m_nCursorX = nMouseX;
	m_nCursorY = nMouseY;

	int		nLine = GetLineNum( nMouseX, nMouseY );
	m_pFocusMagic = GetMagic( nLine );
} //UIMagicList::MouseMove

/**
 * 아이템 팁 정보 보이기.
 */
void UIMagicList::DrawTipInfo()
{
	return;
} //UIMagicList::DrawTipInfo

void UIMagicList::Draw()
{
	return;
}