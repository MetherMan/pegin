/**
 * 유저 인터페이스 미니 맵 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-04 4:39p $
 * @version	$Revision: 14 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIMiniMap.cpp $
 * 
 * 14    03-12-04 4:39p Paran
 * 
 * 13    03-09-17 5:08p Paran
 * 
 * 12    03-09-07 4:40a Paran
 * 
 * 11    03-08-20 11:30a Paran
 * 
 * 10    03-07-30 3:39p Paran
 * 
 * 6     03-06-04 11:24a Paran		미니맵 상점 추가
 * 
 * 5     03-05-28 4:55p Paran		파티 위치 미니맵 표시 수정
 * 
 * 4     03-05-28 4:14p Paran		미니 맵 파티 관련 수정
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * 
 * 1     03-04-10 6:24p Paran		미니맵 추가
 * <HR>
 *
 * @file	UIMiniMap.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIStaticText.h"
#include "UIMiniMap.h"

#define UNIT_ZOOM		10

/**
 * 생성자.
 */
UIMiniMap::UIMiniMap()
:	m_psprMap	( NULL ),
	m_psprPoint	( NULL ),
	m_nZoom		( 0 ),
	m_wMap		( 0 ),
	m_btDirection( 0 )
{
	int		nCnt;
	for( nCnt = 0; nCnt < 8; nCnt++ )
	{
		m_apsprArrow[nCnt] = NULL;
	} //for
	
	for( nCnt = 0; nCnt < NUM_FIREND; nCnt++ )
	{
		m_nFriendX[nCnt] = -1;
		m_nFriendY[nCnt] = -1;
	} //for
} //UIMiniMap::UIMiniMap

/**
 * 소멸자.
 */
UIMiniMap::~UIMiniMap()
{
	SAFE_RELEASE( m_psprMap );
	SAFE_RELEASE( m_psprPoint );
	for( int nCnt = 0; nCnt < 8; nCnt++ )
	{
		SAFE_RELEASE( m_apsprArrow[nCnt] );
	} //for
} //UIMiniMap::~UIMiniMap

/**
 * 포인터 로드.
 */
BOOL UIMiniMap::Init()
{
	m_psprPoint		  = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_psprPoint );
	if( !m_psprPoint->Load( "Interface/MapPoint.tga", 5, 5 ) )
	{
		Assert( !"그림 읽기 실패" );
		return FALSE;
	} //if
	m_psprPoint->SetCenter( 2, 2 );
	
	char	szArrow[64];
	for( int nCnt = 0; nCnt < 8; nCnt++ )
	{
		m_apsprArrow[nCnt] = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_apsprArrow[nCnt] );
		sprintf( szArrow, "Interface/arrowmini_%d.tga", nCnt+1 );
		if( !m_apsprArrow[nCnt]->Load( szArrow, 8, 8 ) )
		{
			Assert( !"그림읽기 실패" );
			return FALSE;
		} //if
		m_apsprArrow[nCnt]->SetColor( 0xFFFF0000 );
		m_apsprArrow[nCnt]->SetCenter( 4, 4 );
	} //for

	return TRUE;
} //UIMiniMap::Load

/**
 * 미니 맵 로드.
 */
BOOL UIMiniMap::LoadMap( WORD wMap,	int nMapWidth, int	nMapHeight )
{
	Cleanup();

	m_wMap		  = wMap;
	if( m_wMap == 0 )
	{
		return TRUE;
	} //if

	if( !m_psprMap )
	{
		m_psprMap = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_psprMap );
	} //if

	SetMapSize( nMapWidth, nMapHeight );
	int		nMiniWidth	= m_rcRegion.right - m_rcRegion.left;
	int		nMiniHeight	= m_rcRegion.bottom - m_rcRegion.top;

	char szFile[125];
	sprintf( szFile, "map/land%04d.tga", wMap );
	if( !m_psprMap->Load( szFile, nMiniWidth, nMiniHeight ) )
	{
		Assert( !"그림 읽기 실패" );
		SAFE_RELEASE( m_psprMap );
		return FALSE;
	} //if

	return TRUE;
} //UIMiniMap::LoadMap

/**
 * 미니 맵 로드.
 */
BOOL UIMiniMap::LoadMap( const char* szMapName, int nMapWidth, int	nMapHeight )
{
	m_wMap = 1;

	SAFE_RELEASE( m_psprMap );
	if( !m_psprMap )
	{
		m_psprMap = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_psprMap );
	} //if

	SetMapSize( nMapWidth, nMapHeight );
	int		nMiniWidth	= m_rcRegion.right - m_rcRegion.left;
	int		nMiniHeight	= m_rcRegion.bottom - m_rcRegion.top;

	char szFile[125];
	sprintf( szFile, "map/%s.tga", szMapName );
	if( !m_psprMap->Load( szFile, nMiniWidth, nMiniHeight ) )
	{
		Assert( !"그림 읽기 실패" );
		SAFE_RELEASE( m_psprMap );
		return FALSE;
	} //if

	return TRUE;
} //UIMiniMap::LoadMap

/**
 * 클린업.
 */
void UIMiniMap::Cleanup()
{
	SAFE_RELEASE( m_psprMap );

	for( int nCnt = 0; nCnt < NUM_FIREND; nCnt++ )
	{
		m_nFriendX[nCnt] = -1;
		m_nFriendY[nCnt] = -1;
	} //for
	
	m_aShopPoint.clear();

	UICComponent::Cleanup();
} //UIMiniMap::Cleanup

/**
 * 맵 사이즈 설정.
 */
void UIMiniMap::SetMapSize( const int nWidth, const int nHeight )
{
	m_fHRatio = (float)nWidth / (float)( m_rcRegion.right - m_rcRegion.left );
	m_fVRatio = (float)nHeight / (float)( m_rcRegion.bottom - m_rcRegion.top );
} //UIMiniMap::SetMapSize

/**
 * 항상 사용하는지 여부.
 */
void UIMiniMap::SetAlways( const BOOL bAlways )
{
	m_bAlways = bAlways;
} //UIMiniMap::SetAlways

/**
 * 보이기 여부.
 */
void UIMiniMap::Show( const BOOL bShow )
{
	UICComponent::Show( bShow );

	if( !m_bAlways && !bShow )
	{
		SAFE_RELEASE( m_psprMap );
	} //if
} //UIMiniMap::Show

/**
 * 현재 방향 설정.
 */
void UIMiniMap::SetDirection( float fAngle )
{
	if( fAngle > 0.0F )
	{
		fAngle = 360.0F - fAngle;
	} 
	else
	{
		fAngle = -fAngle;
	} //if..else
	fAngle += 180;

	m_btDirection = fAngle / 45.0F;
	m_btDirection %= 8;
} //UIMiniMap::SetPoint

/**
 * 미니 맵상 위치.
 */
void UIMiniMap::SetPoint( int nX, int nY, int nIndex )
{
	POINT		ptPos =	GetMiniMapPos( nX, nY );
	if( nIndex == -1 )
	{ //나
		m_nPointX = ptPos.x;
		m_nPointY = ptPos.y;
	}
	else if( nIndex >= 0 )
	{ //친구들
		m_nFriendX[nIndex] = ptPos.x; 
		m_nFriendY[nIndex] = ptPos.y;
	} //if..else
} //UIMiniMap::SetPoint

/**
 * 포인트 지운다.
 */
void UIMiniMap::DeletePoint( int nIndex )
{
	m_nFriendX[nIndex] = -1; 
	m_nFriendY[nIndex] = -1;
} //UIMiniMap::DeletePoint

/**
 * 상점 위치 설정.
 */
void UIMiniMap::AddShopPoint( int nX, int nY )
{
	POINT		ptPos =	GetMiniMapPos( nX, nY );
	m_aShopPoint.push_back( ptPos );
} //UIMiniMap::AddShopPoint

/**
 * 좌표를 미니맵 좌표로 바꿔줍니다.
 */
POINT UIMiniMap::GetMiniMapPos( int nSrcX, int nSrcY )
{
	POINT		ptPos;
	ptPos.x = (float)nSrcX / m_fHRatio;
	ptPos.y = (float)nSrcY / m_fVRatio;

	ptPos.x += m_rcRegion.left;
	ptPos.y += m_rcRegion.top;

	return ptPos;
} //UIMiniMap::GetMiniMapPos

/**
 * 맵번호 얻는다.
 */
int UIMiniMap::GetMapIndex()
{
	return m_wMap;
} //UIMiniMap::GetMapIndex

/**
 * 미니 맵 축소 확대.
 */
void UIMiniMap::Zoom( int nZoom )
{
	m_nZoom += ( UNIT_ZOOM * nZoom );
} //UIMiniMap::Zoom

/**
 * 미니 맵 그리기.
 */
void UIMiniMap::DrawMap()
{
	if( !m_bShow || !m_psprMap )
	{
		return;
	} //if
	m_psprMap->Draw( m_rcRegion.left, m_rcRegion.top );	

	m_psprPoint->SetColor( 0xFF00FF00 );
	for( int nCnt = 0; nCnt < NUM_FIREND; nCnt++ )
	{
		if( m_rcRegion.left > m_nFriendX[nCnt] || m_rcRegion.top  > m_nFriendY[nCnt] )		continue;
		m_psprPoint->Draw( m_nFriendX[nCnt], m_nFriendY[nCnt] );
	} //for

	m_psprPoint->SetColor( 0xFF0000FF );
	for( UINT nShopCnt = 0; nShopCnt < m_aShopPoint.size(); nShopCnt++ )
	{
		m_psprPoint->Draw( m_aShopPoint[nShopCnt].x, m_aShopPoint[nShopCnt].y );
	} //for

	m_apsprArrow[m_btDirection]->Draw( m_nPointX, m_nPointY );
} //UIMiniMap::DrawMap

/**
 * 그리기.
 */
void UIMiniMap::Draw()
{
	if( !IsShow() || !IsEnable() )
	{
		return;
	} //if

	if( m_wMap == 0 )	return;

	Assert( m_psprPoint );
		
	if( !m_psprMap )
	{
		return;
	} //if
	DrawMap();
} //UIMiniMap::Draw