/**
 * 유저 인터페이스용 이미지 리스트
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-03-02 10:13p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIImageList.cpp $
 * 
 * 6     04-03-02 10:13p Paran
 * 
 * 5     04-01-10 1:00a Paran
 * 
 * 4     04-01-06 1:18p Paran
 * 
 * 3     04-01-06 1:16a Paran
 * 
 * 2     03-12-15 10:52p Paran
 * 
 * <HR>
 *
 * @file	UIImageList.cpp
 */

#include "UIImageList.h"

/**
 * 생성자
 */
UIImageList::UIImageList() : m_pFont ( NULL )
{
} //UIImageList::UIImageList

/**
 * 소멸자
 */
UIImageList::~UIImageList()
{
	Cleanup();
} //UIImageList::~UIImageList

/**
 * Cleanup
 */
void UIImageList::Cleanup()
{
	LIST_RECTIMAGE::iterator	iter = m_listImage.begin();
	while( iter != m_listImage.end() )
	{
		SAFE_FREE( (*iter) );
		iter = m_listImage.erase( iter );
	} //while
} //UIImageList::Cleanup

/**
 * 기본정보 설정.
 */
void UIImageList::SetUintPosInfo( 
	const int nNumX, 
	const int nNumY,
	const int nGapX, 
	const int nGapY )
{
	m_nNumX = nNumX;
	m_nNumY = nNumY;
	m_nGapX = nGapX;
	m_nGapY = nGapY;
} //UIImageList::SetUintPosInto

/**
 * 리스트에 이미지 추가한다
 */
BOOL UIImageList::InsertImage( SItemData* pItemData, DWORD dwTick )
{
	if( !pItemData )
	{
		Assert( pItemData );
		return FALSE;
	} //if

	if( m_listImage.size() >= m_nNumX * m_nNumY )
	{
		Assert( !"할당이상" );
		return FALSE;
	} //if

	RECT_IMAGE*		pImageRect = (RECT_IMAGE*)_MemAlloc( sizeof( RECT_IMAGE ) );
	CHECK_PTR_RETURN( pImageRect );

	int		nY = (int)( (float)m_listImage.size() / (float)m_nNumX ) * ( m_nGapX + SIZE_ICON_WIDTH + 20 );
	int		nX = ( m_listImage.size() % m_nNumX ) * ( m_nGapX + SIZE_ICON_HEIGHT + 8 );
	nX += m_rcRegion.left;
	nY += m_rcRegion.top;

	pImageRect->rcImage.left	= nX;
	pImageRect->rcImage.top		= nY;
	pImageRect->rcImage.right	= nX + SIZE_ICON_WIDTH;
	pImageRect->rcImage.bottom	= nY + SIZE_ICON_HEIGHT;
	pImageRect->pItemData		= pItemData;
	pImageRect->nRemainTime		= dwTick;

	m_listImage.push_back( pImageRect );

	return TRUE;	
} //UIImageList::InsertImage

/**
 * 리스트에 이미지 삭제
 */
void UIImageList::RemoveImage( WORD wId )
{
	LIST_RECTIMAGE::iterator	iter = m_listImage.begin();
	while( iter != m_listImage.end() )
	{
		RECT_IMAGE*		pImageRect = (*iter);
		Assert( pImageRect );

		if( pImageRect->pItemData && pImageRect->pItemData->nType == wId )
		{
			SAFE_FREE( pImageRect );
			iter = m_listImage.erase( iter );
			continue;
		} //if
		iter++;
	} //while
} //UIImageList::RemoveImage

/**
 * 업뎃
 */
void UIImageList::Update( const DWORD dwTick )
{
	LIST_RECTIMAGE::iterator	iter = m_listImage.begin();
	while( iter != m_listImage.end() )
	{
		RECT_IMAGE*		pImageRect = *iter;
		Assert( pImageRect );

		pImageRect->nRemainTime -= dwTick;
		if( pImageRect->nRemainTime < 0 )
		{
			SAFE_FREE( pImageRect );
			iter = m_listImage.erase( iter );
			continue;
		} //if

		iter++;
	} //while
} //UIImageList::Update

/**
 * 그리기
 */
void UIImageList::Draw()
{
	LIST_RECTIMAGE::iterator	iter = m_listImage.begin();
	while( iter != m_listImage.end() )
	{
		RECT_IMAGE*		pImageRect = *iter;
		if( !pImageRect || !pImageRect->pItemData )
		{
			Assert( !"이미지리스트 이미지가 없다" );
			continue;
		} //if

		IW3DSprite*		pImage	= pImageRect->pItemData->pimgItem;
		RECT*			pRect	= &pImageRect->rcImage;
		DWORD			dwTick	= pImageRect->nRemainTime;

		if( !pImage || !pRect )
		{
			Assert( !"이미지 없다" );
			continue;
		} //if

		if( dwTick < 3000 )
		{
			if( ( ( 3000 - dwTick ) % 500 ) > 250 )
			{
				pImage->SetColor( 0x64FFFFFF );
			} //if
		} //if

		pImage->Draw( pRect->left, pRect->top );
		pImage->SetColor( 0xFFFFFFFF );
		m_nSec = ( dwTick / 1000 ) % 60;
		m_nMin = ( dwTick / 1000 ) / 60;
		sprintf( m_szSec, "%02d'%02d""", m_nMin, m_nSec ); 
		RECT	rt;
		rt.top = pRect->top + 14;
		rt.left = pRect->left;
		rt.right = pRect->right;
		rt.bottom = pRect->bottom + 29;
		m_pFont->DrawText( m_szSec, &rt, DT_LEFT | DT_VCENTER, 0xFF00C0FF );

		iter++;
	} //while
} //UIImageList::Draw