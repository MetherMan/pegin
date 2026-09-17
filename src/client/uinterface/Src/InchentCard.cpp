/**
 * 인첸트카드 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-01-29 10:33a $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /UInterface/Src/InchentCard.cpp $
 * 
 * 13    04-01-29 10:33a Paran
 * 
 * 12    04-01-13 2:20p Paran
 * 
 * 11    04-01-10 1:00a Paran
 * 
 * 2     03-12-10 2:04p Paran			인첸츠 카드 추가
 *
 * <HR>
 *
 * @file	InchentCard.cpp
 */

#include "UInterface_Export.h"
#include "InchentCard.h"

/**
 * 생성자
 */
CInchentCard::CInchentCard()
:	m_pCamera				( NULL ),
	m_pCardItem				( NULL ),
	m_pEnchantItem			( NULL ),
	m_nSelectCard			( -1 )
{
	m_pCardModel[0] = NULL;
	m_pCardModel[1] = NULL;
	m_pResultCard[0] = NULL;
	m_pResultCard[1] = NULL;
} //CInchentCard::CInchentCard

/**
 * 소멸자
 */
CInchentCard::~CInchentCard()
{
	Cleanup();
} //CInchentCard::~CInchentCard

/**
 * 클린 업
 */
void CInchentCard::Cleanup()
{
	m_nState[0] = 0;
	m_nState[1] = 0;

	SAFE_RELEASE( m_pCardModel[0] );
	SAFE_RELEASE( m_pCardModel[1] );
	m_pResultCard[0] = NULL;
	m_pResultCard[1] = NULL;

	m_nSelectCard	 = -1;
} //CInchentCard::Cleanup

/**
 * 초기화
 */
void CInchentCard::Init( IW3DCamera* pCamera )
{
	Assert( pCamera )
	m_pCamera = pCamera;
} //Init

/**
 * 인첸트 작업 중 확인
 */
BOOL CInchentCard::IsWorking()
{
	if( m_pCardModel[0] )
	{
		return TRUE;
	} //if
	return FALSE;
} //CInchentCard::IsWorking

/**
 * 인첸트 정보
 */
BOOL CInchentCard::GetEnchantInfo( DWORD& dwCardId, DWORD& dwItemId )
{
	if( !m_pEnchantItem )
	{
		return FALSE;
	} //if

	dwCardId = m_pCardItem->nId;
	dwItemId = m_pEnchantItem->nId;

	return TRUE;
} //CInchentCard::GetEnchantInfo

/**
 * 인첸트 카드 아이디
 */
SItemData* CInchentCard::GetCardItem()
{
	return m_pCardItem;
} //CInchentCard::GetCardId

/**
 * 인첸트 카드 아이디 설정
 */
void CInchentCard::SetCardItem( SItemData* pItemDat )
{
	m_pCardItem = pItemDat;
} //CInchentCard::SetCardItemId

/**
 * 인첸트 아이템 설정
 */
void CInchentCard::SetEnchantItem( SItemData* pItemDat )
{
	Assert( pItemDat );
	m_pEnchantItem = pItemDat;
} //CInchentCard::SetEnchantItem

/**
 * 인첸트 시작
 */
void CInchentCard::Begin()
{
	Cleanup();
	
	m_pCardModel[0] = MAKE_COMPONENT( Model );
	m_pCardModel[1] = MAKE_COMPONENT( Model );

	if( !m_pCardModel[0] || !m_pCardModel[1] )
	{
		Assert( !"할당실패" );
		Cleanup();
		return;
	} //if

	char		szModel[2][32];
	strcpy( szModel[0], "inchent_02.mod" );
	strcpy( szModel[1], "inchent_01.mod" );

	if( !m_pCardModel[0]->Load( "Equip", szModel[0] ) ||
		!m_pCardModel[1]->Load( "Equip", szModel[1] ) )
	{
		Assert( !"로드실패" );
		SAFE_RELEASE( m_pCardModel[0] );
		SAFE_RELEASE( m_pCardModel[1] );
		return;
	} //if

	m_pResultCard[0] = m_pCardModel[0];
	m_pResultCard[1] = m_pCardModel[0];

	m_nState[0] = 0;
	m_nState[1] = 0;
	m_dwTick[0] = 0;
	m_dwTick[1] = 0;

	m_vOldCameraPos = m_pCamera->GetEye();

	Update( 0 );
} //CInchentCard::Begin

/**
 * 카드 설정
 */
void CInchentCard::SetCard( BOOL bResult )
{
	if( m_nSelectCard < 0 )
	{
		return;
	} //if
	
	if( bResult )
	{
		m_pResultCard[m_nSelectCard]	    = m_pCardModel[1];
		m_pResultCard[(!m_nSelectCard)?1:0] = m_pCardModel[0];
	}
	else
	{
		m_pResultCard[m_nSelectCard]	    = m_pCardModel[0];
		m_pResultCard[(!m_nSelectCard)?1:0] = m_pCardModel[1];
	} //if..else

	m_nState[m_nSelectCard] = 1;
	m_dwTick[m_nSelectCard] = 0;
} //CInchentCard::SetCard

/**
 * 카메라 메트릭스 설정
 */
void CInchentCard::SetCamera( IW3DCamera* pCamera )
{
	Assert( pCamera );
	
	m_pCamera = pCamera;
} //CInchentCard::SetCameraMatrix

/**
 * 카드 클릭
 */
void CInchentCard::ClickCard( int nCard )
{
	if( nCard > 1 || nCard < 0 )
	{
		Assert( !"없는 카드" );
		return;
	} //if

	if( m_nState[0] != 0 || m_nState[1] != 0 )
	{
		return;
	} //if

	m_nSelectCard = nCard;

	if( !m_pCardItem || !m_pEnchantItem )
	{
		Assert( !"인첸트 아템이 없다" );
		Cleanup();
		return;
	} //if

	PushEvent( "ENCHANT", EVENT_ENCHANT, m_pEnchantItem->nId, m_pCardItem->nId );
} //CInchentCard::ClickCard

/**
 * 카드 업뎃
 */
void CInchentCard::UpdateCardState( int nCardIdx, const DWORD dwTick )
{
	if( !m_pCamera )
	{
		return;
	} //if

	if( nCardIdx > 1 || nCardIdx < 0 )
	{
		Assert( !"없는 카드" );
		return;
	} //if

	Vector3				vDir = m_pCamera->GetFront();
	Vector3				vPos = m_pCamera->GetEye() + vDir;
	m_matCamera.SetWorld( Vector3( 0.0F, 0.0F, 0.0F ), vDir, Vector3( 0.0F, 0.0F, 1.0F ) );
	
	float		fAngle;
	switch( m_nState[nCardIdx] )
	{
	case 0: 
		fAngle = ANGLE( 180 );
		break;

	case 1:
		{
		float		fTransAngle = ( 180.0F / 1000.0F ) * m_dwTick[nCardIdx];
		fAngle = ANGLE( 180 - fTransAngle );
		if( m_dwTick[nCardIdx] > 1000 )
		{
			m_nState[nCardIdx]++;
			m_dwTick[nCardIdx] = 0;
		} //if
		}
		break;
		
	case 2:
		fAngle = 0;

		if( m_nState[0] == m_nState[1] )
		{
			if( m_dwTick[nCardIdx] > 2000 )
			{
				Cleanup();
				m_pCardItem			= NULL;
				m_pEnchantItem		= NULL;
				PushEvent( "ENCHANT", EVENT_ENCHANTCARD_END, 0, 0 );
			} //if
		} //if

		int			nOtherCard;
		if( nCardIdx )		nOtherCard = 0;
		else				nOtherCard = 1;

		if( m_nState[nOtherCard] == 0 )
		{
			m_nState[nOtherCard]++;
			m_dwTick[nOtherCard] = 0;
		} //if
		break;
	} //switch

	Vector3		vOffset = Vector3( 0.3F, 0.0F, 0.0F ) * m_matCamera;

	m_matCard[nCardIdx].SetIdentity();
	m_matCard[nCardIdx].SetRotationY( fAngle );
	Quaternion&		qRot = ( m_matCard[nCardIdx] * m_matCamera ).GetQuaternion();
	if( nCardIdx == 0 )
	{
		m_matCard[nCardIdx].SetWorld( vPos - vOffset,  qRot ); 
	}
	else if( nCardIdx == 1 )
	{
		m_matCard[nCardIdx].SetWorld( vPos + vOffset,  qRot ); 
	} //if..else

	m_dwTick[nCardIdx] += dwTick;
} //CInchentCard::UpdateCardState

/**
 * 업뎃
 */
void CInchentCard::Update( const DWORD dwTick )
{
	if( !m_pResultCard[0] || !m_pResultCard[1] )
	{
		return;
	}// if

	Assert( m_pCamera );

	if( m_vOldCameraPos != m_pCamera->GetEye() )
	{
		if( m_nState[0] == 0 && m_nState[1] == 0 )
		{
			Cleanup();
			m_pCardItem			= NULL;
			m_pEnchantItem		= NULL;

			PushEvent( "ENCHANT", EVENT_ENCHANTCARD_END, 0, 0 );
		} //if
	} //if

	UpdateCardState( 0, dwTick );
	UpdateCardState( 1, dwTick );
} //CInchentCard::Update

/**
 * 그리기
 */
void CInchentCard::Draw()
{
	if( !m_pResultCard[0] || !m_pResultCard[1] )
	{
		return;
	} //if
	Assert( m_pCamera );

	m_pResultCard[0]->SetWorld( m_matCard[0] );
	m_pResultCard[0]->Render( m_dwTick[0], FALSE );

	m_pResultCard[1]->SetWorld( m_matCard[1] );
	m_pResultCard[1]->Render( m_dwTick[1], FALSE );
} //CInchentCard::Render