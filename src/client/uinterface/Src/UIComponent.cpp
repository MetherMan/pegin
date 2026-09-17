/**
 * 유저 인터페이스용 컴포넌트 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-10-15 2:41p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIComponent.cpp $
 * 
 * 7     03-10-15 2:41p Paran
 * 
 * 6     03-08-21 11:29p Paran
 * 
 * 5     03-08-14 3:59a Paran
 * 
 * 4     03-05-24 9:29p Paran
 * 
 * 3     03-05-20 9:35p Paran
 * 계속 수정 중...
 * 
 * 2     03-05-17 4:19a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIComponent.cpp
 */

#include "UInterface_Export.h"
#include "UIComponent.h"

/**
 * 생성자.
 */
UICComponent::UICComponent()
	: m_pOwnerComp	( NULL ),
	  m_nType		( -1 ),
	  m_nID			( -1 ),
	  m_bFocusIn	( FALSE ),
	  m_bShow		( TRUE ),
	  m_bEnable		( TRUE )
{ 
	_ZeroMemory( &m_rcRegion, sizeof( RECT ) );
} //UICComponent::UICComponent

/**
 * 영역을 얻는다.
 */
const RECT*	UICComponent::GetRegion()
{
	return &m_rcRegion;
} //UICComponent::GetRegion

/**
 * 영역 설정.
 */
void UICComponent::SetRegion( int nLeft, int nTop, int nRight, int nBottom ) 
{
	m_rcRegion.left			= nLeft;
	m_rcRegion.top			= nTop;
	m_rcRegion.right		= nRight;
	m_rcRegion.bottom		= nBottom;
} //UICComponent::SetRegion

/**
 * 보이기 여부.
 */
void UICComponent::Show( const BOOL bShow )
{
	m_bShow = bShow;
} //UICComponent::Show

/**
 * 현재보이기 상태.
 */
BOOL UICComponent::IsShow()
{
	return m_bShow;
} //UICComponent::IsShow

/**
 * 왼쪽 버튼 상태 설정.
 */
void UICComponent::LButton( BOOL bPress )
{
	m_bLButton = bPress;
} //UICComponent::SetLButton

/**
 * 왼쪽 버튼 상태.
 */
BOOL UICComponent::IsLButton()
{
	return m_bLButton;
} //UICComponent::IsLButton

/**
 * 오른쪽 버튼 상태 설정.
 */
void UICComponent::RButton( BOOL bPress )
{
	m_bRButton = bPress;
} //UICComponent::RButton

/**
 * 오른쪽 버튼 상태.
 */
BOOL UICComponent::IsRButton()
{
	return m_bRButton;
} //UICComponent::IsRButton

/**
 * 사용여부설정.
 */
void UICComponent::Enable( BOOL bEnable )
{
	m_bEnable = bEnable;
} //UICComponent::Enable

/**
 * 사용여부.
 */
BOOL UICComponent::IsEnable()
{
	return m_bEnable;
} //UICComponent::IsEnable

/**
 *
 */
void UICComponent::FocusIn()
{
	m_bFocusIn = TRUE;
} //UICComponent::FocusIn

/**
 *
 */
void UICComponent::FocusOut()
{
	m_bFocusIn = FALSE;
} //UICComponent::FocusOut

/**
 *
 */
void UICComponent::Release()
{
	delete this;
} //UICComponent::Release

/**
 *
 */
void UICComponent::Cleanup()
{
	FocusOut();
} //UICComponent::Cleanup

/** 
 * 창 이동.
 */
void UICComponent::Move( int nOffsetX, int nOffsetY )
{
	m_rcRegion.left		+= nOffsetX;
	m_rcRegion.top		+= nOffsetY;
	m_rcRegion.right	+= nOffsetX;
	m_rcRegion.bottom	+= nOffsetY;
} //UICComponent::Move

/**
 * 그리기
 */
void UICComponent::Draw()
{
} //UICComponent::Draw

/**
 *
 */
BOOL UICComponent::PointIn( int nX, int nY )	
{
	if( !IsShow() )
	{
		return FALSE;
	} //if

	POINT		ptPos = { nX, nY };

	RECT*	rcText = (RECT*)GetRegion();
	if( PtInRect( GetRegion(), ptPos ) )
	{
		return TRUE;
	}
	return FALSE;
} //UICComponent::PointIn