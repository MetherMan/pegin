/**
 * 타격치 출력
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-01 10:25a $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /Engine/Src/HitIndication.cpp $
 * 
 * 13    03-12-01 10:25a Kjmgo
 * 코드 정리
 * 
 * 12    03-06-10 2:02a Kjmgo
 * 
 * 11    03-06-09 8:18p Kjmgo
 * 크리티컬 수정
 * 
 * 10    03-06-09 7:16p Kjmgo
 * 
 * 9     03-06-08 3:22p Kjmgo
 * 
 * 8     03-05-29 8:11p Kjmgo
 * 5     03-05-11 2:53p Kjmgo	타격치 수정
 * <HR>
 *
 * @file	HitIndication.cpp		
 */

#include "Engine_Export.h"
#include "HitIndication.h"
#include <string>

using std::string;

#define SIZE	128
#define MAXTIME 1000

/**
 * 생성자
 */
CHitIndication::CHitIndication()
:	m_pSprite		( NULL ),
	m_pCritical		( NULL ),
	m_nWidth		( 0 ),
	m_nHeight		( 0 )
{
} //CHitIndication::CHitIndication

/**
 * 소멸자
 */
CHitIndication::~CHitIndication()
{
	Cleanup();

	Assert( m_listHit.empty() );
	SAFE_RELEASE( m_pSprite );
	SAFE_RELEASE( m_pCritical );
} //CHitIndication~CHitIndication

/**
 * 클린업
 */
void CHitIndication::Cleanup()
{
	HitList::iterator itor = m_listHit.begin();

	while( itor != m_listHit.end() )
	{
		SAFE_DELETE( (*itor) );
		itor = m_listHit.erase( itor );
	} //while
} //CHitIndication::Cleanup

/**
 *	생성
 */
BOOL CHitIndication::Create( const char* szFile,		///< Bitmap파일..
							 const int nWidth,			///< Font 너비
							 const int nHeight			///< Font 높이
							 )		
{
	Assert( nWidth > 0 && nHeight > 0 );
	Assert( nWidth < 128 && nHeight < 128 );

	m_nWidth = nWidth;
	m_nHeight = nHeight;

	string sFile = "Texture/Effect/";
	sFile += szFile;

	m_pScene = MAKE_COMPONENT( Scene );
	m_pSprite = MAKE_COMPONENT( Sprite );

	if( !m_pSprite || !m_pSprite->Load( sFile.c_str(), SIZE, SIZE ) )
	{
		return FALSE;
	} //if

	m_pCritical = MAKE_COMPONENT( Sprite );

	if( !m_pCritical || !m_pCritical->Load( "Texture/Effect/damage_critical.tga", SIZE, SIZE ) )
	{
		return FALSE;
	} //if
	m_nColCount = SIZE / m_nWidth;
	
	return TRUE;
} //CHitIndication::Create

/**
 * 데미지를 쪼개어 배열에 넣는다.
 */
SHitData* CHitIndication::MakeHit( DWORD dwDamage )
{
	SHitData* pData = new SHitData;

	if( !pData )
	{
		return NULL;
	} //if

	static DWORD dwMultiple = 10;
	static DWORD dwRemnant = 0;

	dwMultiple = 10;

	pData->nCount = 0;

	for( int i = 0 ; i < MAXNUMBER ; i++ )
	{
		/*
			자리수 대로 배열에 넣는 알고리즘은...
			10으로 나눈 나머지를 다시 10으로 나눈 몫이 곧 그 자리수의
			숫자가 된다.
			이때... 차례 차례 작은 자리수부터 들어가므로...
			출력할떄 주의하자
		*/
		dwRemnant = dwDamage % dwMultiple;
		BYTE btNumber = dwRemnant / ( dwMultiple / 10 );
		pData->aNumber[pData->nCount] = btNumber;
		pData->nCount++;

		if( dwRemnant == dwDamage )
		{
			break;
		} //if

		dwMultiple *= 10;
	} //for		

	pData->wTotalWidth = pData->nCount * m_nWidth;
	return pData;
} //CHitIndication::MakeHit

/** 
 * Hit 정보를 추가한다.
 */
void CHitIndication::AddHit( DWORD dwDamage,			///< 데미지
							 const Vector3& vPos,		///< 위치
							 DWORD dwColor,				///< 색
							 BYTE  btCritical			///< 크리티컬 여뷰 ( Default : FALSE )
							 )
{
	SHitData* pData = MakeHit( dwDamage );

	if( !pData )
	{
		return;
	} //if

	pData->dwDamage = dwDamage;
	pData->dwFirstColor = dwColor;
	pData->dwColor = dwColor;
	pData->fSpeed = 0.0F;
	pData->dwTick = 0;
	pData->nColorCount = 0;
	pData->vPos = vPos;
	pData->nWidth = m_nWidth + 10;
	pData->nHeight = m_nHeight + 10;
	pData->btCritical = btCritical;
	pData->bFlag = FALSE;
	
	if( pData->btCritical )
	{
		pData->nWidth = 176 + 10;
		pData->nHeight = 75+ 10;
		pData->vPos.z += 0.2F;

		pData->fSpeed = 10.0F;
	} //if
	
	pData->vScreenPos = m_pScene->GetScreenPos( pData->vPos );
	pData->wTotalWidth = pData->nCount * pData->nWidth;

	m_listHit.push_back( pData );
} //CHitIndication::AddHit

/** 
 * Hit 정보를 추가한다.
 */
void CHitIndication::AddMiss( const Vector3& vPos,		///< 위치
							  DWORD dwColor				///< 색
							 )
{
	SHitData* pData = new SHitData;

	if( !pData )
	{
		return;
	} //if

	pData->dwFirstColor = dwColor;
	pData->dwColor = dwColor;
	pData->btCritical = 0;
	pData->nCount = 0;
	pData->fSpeed = 0.0F;
	pData->dwTick = 0;
	pData->nColorCount = 0;
	pData->vPos = vPos;
	pData->nWidth = 62 + 10;
	pData->nHeight = 17 + 10;
	
	pData->wTotalWidth = pData->nCount * pData->nWidth;

	pData->vScreenPos = m_pScene->GetScreenPos( pData->vPos );

	m_listHit.push_back( pData );
} //CHitIndication::AddHit

/**
 * 업데이트 한다.
 */
void CHitIndication::Update( DWORD dwTick )
{
	if( m_listHit.empty() )
	{
		return;
	} //if

	HitList::iterator itor = m_listHit.begin();

	DWORD dwAlpha = 0x09000000;

	SHitData* pData = NULL;

	while( itor != m_listHit.end() )
	{
		pData = (*itor);

		pData->dwTick += dwTick;

		if( pData->dwTick >= MAXTIME || pData->nColorCount >= 25 )
		{	
			SAFE_DELETE( pData );
			itor = m_listHit.erase( itor );
			continue;
		} //if

		if( pData->nColorCount >= 14 && pData->btCritical  )
		{
			pData->bFlag = TRUE;
			AddHit( pData->dwDamage, pData->vPos, pData->dwFirstColor, FALSE );
			SAFE_DELETE( pData );
			itor = m_listHit.erase( itor );
			continue;
		} //if

		static float fAngle = 10.0F;
		static Vector3 vPos;
		
		vPos = pData->vPos;
		pData->nColorCount++;
		
		if( !pData->btCritical )
		{
			pData->fSpeed = ANGLE( pData->dwTick / 15 );
			vPos.z = pData->vPos.z + ( pData->fSpeed );
		}
		else
		{
			pData->fSpeed -= pData->dwTick / 100;

			if( pData->fSpeed <= 0 )
			{
				pData->fSpeed = 0;
			}
			
			float fOffset = pData->fSpeed;
			pData->nWidth += fOffset;
			pData->nHeight += fOffset;
		} //if..else..

		pData->dwColor = pData->dwColor - dwAlpha;
		pData->vScreenPos = m_pScene->GetScreenPos( vPos );
		pData->wTotalWidth = pData->nCount * pData->nWidth;
		
		itor++;
	} //while
} //CHitIndication::Update

/**
 * 그려준다.
 */
void CHitIndication::Draw()
{
	if( m_listHit.empty() )
	{
		return;
	} //if

	HitList::iterator itor = m_listHit.begin();

	SHitData* pData = NULL;

	while( itor != m_listHit.end() )
	{
		pData = (*itor);

		if( pData->nCount != 0 )
		{
			if( pData->btCritical > 0 )
			{
				DrawCritical( pData );
			}
			else
			{
				DrawHit( pData );
			} //if..else..
		}
		else
		{
			DrawMiss( pData );
		} //if..else..

		itor++;
	} //while
} //CHitIndication::Draw

/**
 * 데미지를 그려준다.
 */
void CHitIndication::DrawHit( SHitData* pData )
{
	Assert( pData );
	Assert( m_pSprite );

	float fCenter = pData->wTotalWidth / 2;
	Vector3 vPos = pData->vScreenPos - Vector3( fCenter, 0.0F, 0.0F );

	m_pSprite->SetColor( pData->dwColor );

	for( int i = pData->nCount - 1; i >= 0 ; i-- )
	{
		// 가장 큰 자리수부터 하나씩 꺼내온다.
		BYTE btNumber = pData->aNumber[i];

		DrawNumber( btNumber, vPos, pData->nWidth, pData->nHeight );

		vPos.x += (float)m_nWidth;
	} //for
	 
} //CHitIndication::DrawHit

/**
 * 데미지를 그려준다.
 */
void CHitIndication::DrawMiss( SHitData* pData )
{
	Assert( pData );
	Assert( m_pSprite );

	float fCenter = pData->wTotalWidth / 2;
	Vector3 vPos = pData->vScreenPos - Vector3( fCenter, 0.0F, 0.0F );

	m_pSprite->SetColor( pData->dwColor );

	int	nX = (int)vPos.x;
	int	nY = (int)vPos.y;

	m_pSprite->Draw( nX, nY, pData->nHeight, pData->nWidth, 1, 49, 62, 17 );
 
} //CHitIndication::DrawMiss


/**
 * 숫자를 찍어준다.
 */
void CHitIndication::DrawNumber( BYTE btNumber, Vector3 vPos, int nWidth, int nHeight )
{
	Assert( btNumber >= 0 && btNumber < 10 );
	Assert( m_pSprite );

	int nPosX = ( btNumber % m_nColCount ) * m_nWidth;
	int nPosY = ( btNumber / m_nColCount ) * m_nHeight;

	int		nX = (int)vPos.x;
	int		nY = (int)vPos.y;

	m_pSprite->Draw( nX, nY, nWidth, nHeight, nPosX, nPosY, m_nWidth, m_nHeight );

} //CHitIndication::DrawNumber

/**
 * Critical 을 출력한다.
 */
void CHitIndication::DrawCritical( SHitData* pData )
{
	BYTE btCritical = pData->btCritical;
	Assert( btCritical <= 3 && btCritical >= 1 );

	float fCenterX = pData->nWidth / 2;
	float fCenterY = pData->nHeight / 2;
	Vector3 vPos = pData->vScreenPos - Vector3( fCenterX, fCenterY, 0.0F );

	int nPosX = 0;		///< Texture 상의 위치
	int nPosY = 0;		///< 이하동문

	int	nX = (int)vPos.x;
	int	nY = (int)vPos.y;

	//176x75 여요
	static int nWidth = 176;
	static int nHeight = 75;

	switch( btCritical )
	{
	case 1: // Good
		nPosY = nHeight * 2;
		break;
	case 2: // Great
		nPosY = nHeight;
		break;
	case 3: // Critical
		nPosY = 0;
		break;
	} //switch

	m_pCritical->SetColor( pData->dwColor );
	m_pCritical->Draw( nX, nY, pData->nHeight, pData->nWidth, nPosX, nPosY, nWidth, nHeight );
	
} //CHitIndication::DrawCritical