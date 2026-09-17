/**
 * 스킬등록 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-24 6:09p $
 * @version	$Revision: 18 $
 *
 * <HR>
 * $Log: /UInterface/Src/UISkillEntry.cpp $
 * 
 * 18    03-12-24 6:09p Paran
 * 
 * 17    03-12-22 1:08a Paran
 * 
 * 16    03-12-20 7:56a Paran
 * 
 * 15    03-12-16 5:05p Paran
 * 
 * 14    03-12-15 10:52p Paran
 * 
 * 13    03-12-12 11:58a Paran
 * 
 * 12    03-12-09 5:58p Paran
 * 아이템 데이터 수정
 * 
 * 11    03-09-24 7:48p Paran
 * 
 * 10    03-09-23 1:39a Paran
 * 
 * 9     03-09-16 11:56a Paran
 * 
 * <HR>
 *
 * @file	UISkillEntry.cpp
 */

#include "UIComponent.h"
#include "UISkillEntry.h"
#include <io.h>

/**
 * 생성자.
 */
UISkillEntry::UISkillEntry()
:	m_pFont			( NULL ),
	m_pFocusItemDat	( NULL ),
	m_pItemInfoMgr	( NULL ),
	m_nStartLine	( 0 )
{
} //UISkillEntry::UISkillEntry

/**
 * 소멸자.
 */
UISkillEntry::~UISkillEntry()
{
	Cleanup();
} //UISkillEntry::~UISkillEntry

/**
 * 초기화.
 */
BOOL UISkillEntry::Init( IW3DItemInfoMgr* pItemInfoMgr )
{
	m_pItemInfoMgr = pItemInfoMgr;

	return TRUE;
} //UISkillEntry::Init

/**
 * 각 스킬 이미지 로드.
 */
void UISkillEntry::InitSkillImage()
{
	Cleanup();
/*
	char	szKey[3];
	switch( m_btType )
	{
	case SKILL_SHORT:		strcpy( szKey, "ss_" );		break;
	case SKILL_LONG:		strcpy( szKey, "ls_" );		break;
	case SKILL_BLACK:		strcpy( szKey, "mb_" );		break;
	case SKILL_WHITE:		strcpy( szKey, "mw_" );		break;

	case SKILL_LIFE:		strcpy( szKey, "k2_" );		break;
	} //switch

	char			szFullName[64];
	SSkill			sSkillTemp[5];
	int				nSaveLineCnt;

	for( int nY = 1; nY < 10; nY++ )
	{
		nSaveLineCnt = 0;

		for( int nX = 1; nX <= m_nNumX; nX++ )
		{
			sSkillTemp[nX-1].pImage    = NULL;
			sSkillTemp[nX-1].pItemData = NULL;

			sprintf( szFullName, "Interface/%s%dx%db.wtm", szKey, nY, nX );
			if( _access( szFullName, 0 ) == -1 )
			{ // 파일이 없담...
				continue;
			} //if

			sSkillTemp[nX-1].pImage = MAKE_COMPONENT( Sprite );
			if( !sSkillTemp[nX-1].pImage )
			{
				Assert( !"없당" );
				return;
			} //if
			
			if( !sSkillTemp[nX-1].pImage->Load( szFullName, SIZE_ICON_WIDTH, SIZE_ICON_HEIGHT ) )
			{
				SAFE_RELEASE( sSkillTemp[nX-1].pImage );
				continue;
			} //if
			nSaveLineCnt++;
		} //for

		if( nSaveLineCnt > 0 )
		{
			for( int nCnt = 0; nCnt < m_nNumX; nCnt++ )
			{
				m_vecItem.push_back( sSkillTemp[nCnt] );
			} //for
		} //if
	} //for
*/
} //UISkillEntry::InitSkillImage

/**
 * 클린업.
 */
void UISkillEntry::Cleanup()
{
	UICComponent::Cleanup();

	m_vecItem.clear();
} //UISkillEntry::Cleanup

/**
 * 마우스 이동.
 */
void UISkillEntry::MouseMove( int nMouseX, int nMouseY )
{
	m_nCursorX = nMouseX;
	m_nCursorY = nMouseY;
	m_pFocusItemDat = GetItemData( nMouseX, nMouseY );
} //UISkillEntry::MouseMove

/**
 * 스킬 아이콘 정보.
 */
void UISkillEntry::SetUintPosInfo( 
	const int nNumX, 
	const int nNumY,
	const int nGapX, 
	const int nGapY )
{
	m_nNumX = nNumX;
	m_nNumY = nNumY;
	m_nGapX = nGapX;
	m_nGapY = nGapY;
} //UISkillEntry::SetUintPosInto

/**
 * 스킬 추가.
 */
BOOL UISkillEntry::InsertSkill( SItemData* pItemData )
{
	if( !pItemData || !pItemData->pItemInfoData )
	{
		Assert( !"포인터 이상!" );
		return FALSE;
	} //if

	char			*pImageName = pItemData->pItemInfoData->szMimageFile5;
	IW3DSprite*		pImage;
	pImage = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( pImage );

	char	szFullName[64];
	sprintf( szFullName, "Item/%s", pImageName );
	if( !pImage->Load( szFullName, SIZE_ICON_WIDTH, SIZE_ICON_HEIGHT ) )
	{
		Assert( !"이미지 로드실패" );
		return FALSE;
	} //if

	SAFE_RELEASE( pItemData->pimgItem );
	pItemData->pimgItem = pImage;
	m_vecItem.push_back( pItemData );

	return TRUE;
} //UISkillEntry::InsertSkill

/**
 * 아이템 위치로 정보를 얻는다.
 */
SItemData* UISkillEntry::GetItemData( const int nX, const int nY )
{
	int			nSkillX, nSkillY;
	if( !GetSkillPos( nSkillX, nSkillY, nX, nY ) )
	{
		return NULL;	
	} //if

	int			nIndex = ( nSkillY + m_nStartLine ) * m_nNumX + nSkillX;
	if( nIndex >= m_vecItem.size() || nIndex < 0 )
	{
		return NULL;
	} //if

	return m_vecItem[nIndex];
} //UISkillEntry::GetItemData

/**
 * 아이템 타입으로 정보를 얻는다.
 */
SItemData* UISkillEntry::GetItem( const int nType )
{
	SItemData*		pItemData;
	for( UINT nCnt = 0; nCnt < m_vecItem.size(); nCnt++ )
	{
		pItemData = m_vecItem[nCnt];
		if( pItemData )
		{
			if( pItemData->nType == nType )
			{
				return pItemData;
			} //if
		} //if
	} //if
	return NULL;
} //UISkillEntry::GetItem

/**
 * 현재 플래이어 스킬레벨.
 */
void UISkillEntry::SetPlayerPower( const int nType, const int nLevel )
{
	Assert( nType < 3 && nType >= 0 );
	m_anPlayer_Skill[nType] = nLevel;
} //UISkillEntry::SetKillLevel

/**
 * 아이템 팁 정보 보이기.
 */
void UISkillEntry::DrawTipInfo()
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
			FALSE
			);
	} //if
} //UISkillEntry::DrawTipInfo

/**
 * 실제 스크린 위치얻기.
 */
void UISkillEntry::GetScreenPos( 
	int& nScreenX, 
	int& nScreenY, 
	const int nSkillX, 
	const int nSkillY 
	)
{
	Assert( nSkillX <= m_nNumX );

	nScreenX	= nSkillX * ( SIZE_ICON_WIDTH + m_nGapX ) + m_rcRegion.left;
	nScreenY	= nSkillY * ( SIZE_ICON_HEIGHT + m_nGapY ) + m_rcRegion.top;
} //UISkillEntry::GetScreenPos

/**
 * 스킬 위치얻기.
 */
BOOL UISkillEntry::GetSkillPos( 
	int& nSkillX, 
	int& nSkillY, 
	const int nScreenX,
	const int nScreenY
	)
{
	int		nAbX = nScreenX - m_rcRegion.left;
	int		nAbY = nScreenY - m_rcRegion.top;

	if( nAbX < 0 || nAbY < 0 )
	{
		return FALSE;
	} //if

	nSkillX = nAbX / ( SIZE_ICON_WIDTH + m_nGapX );
	nSkillY = nAbY / ( SIZE_ICON_HEIGHT + m_nGapY );

	if( nSkillX > m_nNumX )
	{
		return FALSE;
	} //if

	return TRUE;
} //UISkillEntry::GetSkillPos

/**
 * 스크롤 초대값.
 */
int	UISkillEntry::GetScrollMax()
{
	int		nScrollMax = ( m_vecItem.size() / m_nNumX ) - m_nNumY;
	if( ( m_vecItem.size() % m_nNumX ) > 0 )
	{
		nScrollMax++;	
	} //if
	
	if( nScrollMax < 0 )
	{
		nScrollMax = 0;
	} //if

	return nScrollMax;
} //UISkillEntry::GetScrollMax

/**
 * 스크롤 바 값.
 */
void UISkillEntry::SetScrollPos( const int nPos )
{
	if( GetScrollMax() < nPos )
	{
		return;
	} //if
	m_nStartLine = nPos;
} //UISkillEntry::SetScrollPos

/**
 * 스킬 추가.
 */
void UISkillEntry::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	int			nDrawX, nDrawY;
	int			nCnt = m_nStartLine * m_nNumX;
	for( int nY = 0; nY < m_nNumY; nY++ )
	{
		for( int nX = 0; nX < m_nNumX; nX++ )
		{
			if( m_vecItem.size() <= nCnt )
			{
				return;
			} //if

			SItemData*		pItemDat = m_vecItem[nCnt];
			if( pItemDat->pimgItem )
			{
				GetScreenPos( nDrawX, nDrawY, nX, nY );
				pItemDat->pimgItem->Draw( nDrawX, nDrawY );
			} //if
			
			nCnt++;
		} //for
	} //for
} //UISkillEntry::Draw