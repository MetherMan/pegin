/**
 * 유저 인터페이스용 진행바 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-12-08 3:07a $
 * @version	$Revision: 14 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIProgress.cpp $
 * 
 * 14    04-12-08 3:07a Sk8snow2
 * 
 * 13    04-09-10 11:28p Paran
 * 
 * 12    04-04-08 12:18a Paran2
 * 
 * 11    03-10-07 9:07p Paran
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * <HR>
 *
 * @file	UIProgress.cpp
 */


#include "UInterface_Export.h"
#include "UIComponent.h"
#include "UIProgress.h"


/**
 * 생성자.
 */
UIProgress::UIProgress()
{
	m_nMin		= 0;
	m_nMax		= 100;
	m_pGraph	= NULL;
	m_pRectangle= NULL;
	m_nCurrPos	= 50;
	m_nValue	= 0;
	m_dwTick	= 0;
} //UIProgress::UIProgress

/**
 * 소멸자.
 */
UIProgress::~UIProgress()
{
	ReleaseAll();
} //UIProgress::~UIProgress

/**
 *
 */
void UIProgress::ReleaseAll()
{
	SAFE_RELEASE( m_pGraph );
	SAFE_RELEASE( m_pRectangle );
} //UIProgress::ReleaseAll

/**
 *
 */
void UIProgress::Cleanup()
{
	UICComponent::Cleanup();
} //UIProgress::Cleanup

/**
 *
 */
BOOL UIProgress::Load( const char* szFileName, const int nRow, const int nCol )
{
	Assert( szFileName );

	int		nWidth	= ( m_rcRegion.right - m_rcRegion.left );
	int		nHeight	= ( m_rcRegion.bottom - m_rcRegion.top );

	SAFE_RELEASE( m_pGraph );
	m_pGraph = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pGraph );

	if( !m_pGraph->Load( szFileName, nWidth, nHeight ) )
	{
		Assert( !"그림 읽기 실패" );
		return FALSE;
	} //if

	if( nRow > 1 || nCol > 1 )
	{
		m_pGraph->SetAnimation( nRow, nCol, 100 );
	} //if

	return TRUE;
} //UIProgress::Load

/**
 * Create Rectangle
 */
BOOL UIProgress::CreateRectangle( const DWORD dwColor )
{
	int		nWidth	= ( m_rcRegion.right - m_rcRegion.left );
	int		nHeight	= ( m_rcRegion.bottom - m_rcRegion.top );

	SAFE_RELEASE( m_pRectangle )
	m_pRectangle = MAKE_COMPONENT( Rectangle );
	CHECK_PTR_RETURN( m_pRectangle );

	m_dwColor = dwColor;

	return TRUE;
} //UIProgress::CreateRectangle

/**
 *
 */
void UIProgress::SetPos( int nPos, BOOL bProgress )
{ 
	m_nValue = nPos;

	if( m_nValue > m_nMax )			m_nValue = m_nMax;
	else if( m_nValue < 0 )			m_nValue = 0;

	switch( m_bType )
	{
	case HORIZONTAL:
		{
			int		nWidth = m_rcRegion.right - m_rcRegion.left;
			m_nGoalPos = (int)( (float)nWidth / (float)m_nMax * m_nValue );
		}
		break;

	case VERTICAL:
		{
			int		nHeight = m_rcRegion.bottom - m_rcRegion.top;
			m_nGoalPos = (int)( (float)nHeight / (float)m_nMax * m_nValue );
		}
		break;
	} //switch

	if( !bProgress )
	{
		m_nCurrPos = m_nGoalPos;
	} //if
} //UIProgress::SetPos

/**
 * 업뎃.
 */
void UIProgress::Update( const DWORD dwTick )
{
	if( m_pGraph )
	{
		m_pGraph->Update( dwTick );
	} //if

	if( m_nGoalPos == m_nCurrPos )
	{
		return;
	} //if

	m_dwTick += dwTick;
	if( m_dwTick < 100 )
	{
		return;
	} //if

/*	if( m_nGoalPos > m_nCurrPos )
	{
		m_nCurrPos += m_dwTick / 50;
		if( m_nGoalPos < m_nCurrPos )
		{
			m_nCurrPos = m_nGoalPos;
		} //if
	}
	else
	{
		m_nCurrPos -= m_dwTick / 50;
		if( m_nGoalPos > m_nCurrPos )
		{
			m_nCurrPos = m_nGoalPos;
		} //if
	} //if..else*/
	m_nCurrPos = m_nGoalPos;

	m_dwTick = 0;
} //UIProgress::Update

/**
 * Draw Rectangle
 */
void UIProgress::DrawRectangle()
{
	switch( m_bType )
	{
	case HORIZONTAL:
		m_pRectangle->Draw( 
			m_rcRegion.left, 
			m_rcRegion.top, 
			m_nCurrPos, 
			m_rcRegion.bottom - m_rcRegion.top,
			m_dwColor 
			);
		break;

	case VERTICAL:
		m_pRectangle->Draw( 
			m_rcRegion.left, 
			m_rcRegion.bottom - m_nCurrPos,
			m_rcRegion.right - m_rcRegion.left,
			m_nCurrPos,
			m_dwColor
			);
		break;
	} //switch
} //UIProgress::DrawRectangle

/**
 * Draw Image
 */
void UIProgress::DrawImage()
{
	switch( m_bType )
	{
	case HORIZONTAL:
		m_pGraph->Draw( 
			m_rcRegion.left, m_rcRegion.top, 
			0, 0, 
			m_nCurrPos, m_rcRegion.bottom - m_rcRegion.top  );
		break;

	case VERTICAL:
		m_pGraph->Draw( 
			m_rcRegion.left, m_rcRegion.bottom - m_nCurrPos,
			0, 0,
			m_rcRegion.right - m_rcRegion.left, m_nCurrPos );
		break;
	} //switch
} //UIProgress::DrawImage

/**
 *
 */
void UIProgress::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	if( m_pRectangle )
	{
		DrawRectangle();
	}
	else
	{
		DrawImage();		
	} //if..else
} //UIProgress::Draw










/**
 * 생성자.
 */
UIProgressEx::UIProgressEx()
{
	m_nMin		= 0;
	m_nMax		= 100;
	m_pGraph	= NULL;
	m_nCurrPos	= 50;
	m_nMaxNum	= 1;
	m_nValue	= 0;
} //UIProgressEx::UIProgressEx

/**
 * 소멸자.
 */
UIProgressEx::~UIProgressEx()
{
	ReleaseAll();
} //UIProgressEx::~UIProgressEx

/**
 * 릴리즈.
 */
void UIProgressEx::ReleaseAll()
{
	SAFE_RELEASE( m_pGraph );
	SAFE_RELEASE( m_pGraphBack );
} //UIProgressEx::ReleaseAll

/**
 * 클린 업.
 */
void UIProgressEx::Cleanup()
{
	UICComponent::Cleanup();
} //UIProgressEx::Cleanup

/**
 * 로드.
 */
BOOL UIProgressEx::Load( const char* szGraphFile, const char* szBackFile, const int nNum )
{
	Assert( szGraphFile && szBackFile );

	int		nWidth	= m_rcRegion.right - m_rcRegion.left;
	int		nHeight	= m_rcRegion.bottom - m_rcRegion.top;

	m_pGraph = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pGraph );
	if( !m_pGraph->Load( szGraphFile, nWidth, nHeight ) )
	{
		Assert( !"그림 읽기 실패" );
		return FALSE;
	} //if

	m_pGraphBack = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pGraphBack );
	if( !m_pGraphBack->Load( szBackFile, nWidth, nHeight ) )
	{
		Assert( !"그림 읽기 실패" );
		return FALSE;
	} //if

	m_nMaxNum = nNum;
	m_nLength = nWidth;
	return TRUE;
} //UIProgressEx::Load

/**
 * 최소최대 값 설정.
 */
void UIProgressEx::SetMinMax( int nMin, int nMax )
{ 
	m_nMax = nMax; 
	m_nMin = nMin; 
} //UIProgressEx::SetMinMax

/**
 * 현재 위치얻기.
 */
int	UIProgressEx::GetPos()
{ 
	return m_nValue; 
} //UIProgressEx::GetPos

/**
 * 최대 값.
 */
int	UIProgressEx::GetMax()
{ 
	return m_nMax; 
} //UIProgressEx::GetMax

/**
 * 현재 위치설정.
 */
void UIProgressEx::SetPos( int nPos, BOOL bProgress )
{ 
	m_nValue = nPos;
	if( m_nValue > m_nMax )			m_nValue = m_nMax;
	if( m_nValue < 0 )				m_nValue = 0;

	int		nWidth = ( m_rcRegion.right - m_rcRegion.left ) * m_nMaxNum;
	m_nGoalPos = (int)( (float)nWidth / (float)m_nMax * m_nValue );

	if( !bProgress )
	{
		m_nCurrPos = m_nGoalPos;
		m_nFillNum		= m_nCurrPos / ( m_rcRegion.right - m_rcRegion.left );
		m_nPartCurrPos	= m_nCurrPos % m_nLength;
	} //if
} //UIProgressEx::SetPos

/**
 * 업뎃.
 */
void UIProgressEx::Update( const DWORD dwTick )
{
	if( m_nGoalPos == m_nCurrPos )
	{
		return;
	} //if

	m_dwTick += dwTick;
	if( m_dwTick < 100 )
	{
		return;
	} //if

	if( m_nGoalPos > m_nCurrPos )
	{
		m_nCurrPos += m_dwTick / 25;
		if( m_nGoalPos < m_nCurrPos )
		{
			m_nCurrPos = m_nGoalPos;
		} //if
	}
	else
	{
		m_nCurrPos -= m_dwTick / 25;
		if( m_nGoalPos > m_nCurrPos )
		{
			m_nCurrPos = m_nGoalPos;
		} //if
	} //if..else

	m_nFillNum		= m_nCurrPos / ( m_rcRegion.right - m_rcRegion.left );
	m_nPartCurrPos	= m_nCurrPos % m_nLength;

	m_dwTick = 0;
} //UIProgressEx::Update

/**
 * 그리기.
 */
void UIProgressEx::Draw()
{
	if( !IsShow() )
	{
		return;
	} //if

	int			nX, nY;
	nY = m_rcRegion.top;

	for( int nCnt = 0; nCnt < m_nMaxNum; nCnt++ )
	{
		nX = m_rcRegion.left + ( m_nLength * nCnt );

		m_pGraphBack->Draw( nX, nY );
		if( nCnt < m_nFillNum )
		{
			m_pGraph->Draw( nX, nY );
		} //if
	} //for

	if( m_nFillNum < m_nMaxNum )
	{
		int		nHeight = m_rcRegion.bottom - m_rcRegion.top;
		nX		= m_nFillNum * m_nLength;
		m_pGraph->Draw( nX, nY, 0, 0, m_nPartCurrPos, nHeight );
		m_pGraph->SetSize( m_nLength, nHeight );
	} //if
} //UIProgressEx::Draw



/**
 * Construct 
 */
UIProgressCircular::UIProgressCircular()
{
	m_dwTotalTick = 0;
	m_dwCurTick   = 0;

	m_apImage[0] = NULL;
	m_apImage[1] = NULL;
	m_apImage[2] = NULL;
	m_apImage[3] = NULL;

	m_nX	 = 0;
	m_nY	 = 0;

	m_dwColor = 0xFFFFFFFF;

	Load();
} //UIProgressCircular::UIProgressCircular

/**
 * Destruct 
 */
UIProgressCircular::~UIProgressCircular()
{
	SAFE_RELEASE( m_apImage[0] );
	SAFE_RELEASE( m_apImage[1] );
	SAFE_RELEASE( m_apImage[2] );
	SAFE_RELEASE( m_apImage[3] );
} //UIProgressCircular::~UIProgressCircular

/**
 * Load 
 */
BOOL UIProgressCircular::Load()
{
	SAFE_RELEASE( m_apImage[0] );
	SAFE_RELEASE( m_apImage[1] );
	SAFE_RELEASE( m_apImage[2] );
	SAFE_RELEASE( m_apImage[3] );
	
	m_apImage[0] = MAKE_COMPONENT( Sprite );
	m_apImage[1] = MAKE_COMPONENT( Sprite );
	m_apImage[2] = MAKE_COMPONENT( Sprite );
	m_apImage[3] = MAKE_COMPONENT( Sprite );

	if( !m_apImage[0] || !m_apImage[1] || !m_apImage[2] || !m_apImage[3] )
	{
		Assert( !"Failed" );
		return FALSE;
	} //if

	if( !m_apImage[0]->Load( "Interface/progangle1.tga", 14, 14 ) ||
		!m_apImage[1]->Load( "Interface/progangle2.tga", 14, 14 ) ||
		!m_apImage[2]->Load( "Interface/progangle3.tga", 14, 14 ) ||
		!m_apImage[3]->Load( "Interface/progangle4.tga", 14, 14 ) )
	{
		Assert( !"Load Not The File" );
		return FALSE;
	} //if
		
	m_apImage[0]->SetAnimation( 15, 1, 100 );
	m_apImage[1]->SetAnimation( 15, 1, 100 );
	m_apImage[2]->SetAnimation( 15, 1, 100 );
	m_apImage[3]->SetAnimation( 15, 1, 100 );

	return TRUE;
} //UIProgressCircular::Load

/**
 * 시간설정.
 */
void UIProgressCircular::SetTime( const DWORD dwTick )
{
	m_dwTotalTick = dwTick;
	m_dwCurTick   = 0;
} //UIProgressCircular::SetTime

/**
 * 업뎃.
 */
void UIProgressCircular::Update( const DWORD dwTick )
{
	if( m_dwCurTick > m_dwTotalTick )
	{
		return;
	} //if

	m_dwCurTick += dwTick;
} //UIProgressCircular::Update

/**
 * DrawFan 
 */
void UIProgressCircular::DrawFan( int nPart, int nFrame )
{
	if( nPart == 0 )		m_apImage[nPart]->DrawFrame( m_nX + 14, m_nY,	   nFrame );
	else if( nPart == 1 )	m_apImage[nPart]->DrawFrame( m_nX + 14, m_nY + 14, nFrame );
	else if( nPart == 2 )	m_apImage[nPart]->DrawFrame( m_nX,	    m_nY + 14, nFrame );
	else if( nPart == 3 )	m_apImage[nPart]->DrawFrame( m_nX,	    m_nY,	   nFrame );
} //UIProgressCircular::DrawFan

/**
 * Draw 
 */
void UIProgressCircular::Draw()
{
	if( m_dwCurTick > m_dwTotalTick )
	{
		return;
	} //if

	int		nWAngle  = m_dwTotalTick / 4;
	int		nPerPart = ( (float)m_dwCurTick / (float)nWAngle );
	int		nRestPart = m_dwCurTick % nWAngle;

	int			nCnt;
	for( nCnt = 0; nCnt < nPerPart; nCnt++ )
	{
		DrawFan( nCnt, 14 );
	} //for

	for( nCnt = nPerPart+1; nCnt < 4; nCnt++ )
	{
		DrawFan( nCnt, 0 );
	} //for

	if( nPerPart >=0 && nPerPart < 4 )
	{
		int		nFan = (int)( (float)15 / (float)nWAngle * nRestPart );
		
		DrawFan( nPerPart, nFan );
	} //if
} //UIProgressCircular::Draw