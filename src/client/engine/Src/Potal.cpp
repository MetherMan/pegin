/**
 * 포탈. ( FPS에서 쓰는 포탈과는 조금 개념이 다르다. ) 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-12-09 6:28p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Engine/Src/Potal.cpp $
 * 
 * 3     04-12-09 6:28p Sk8snow2
 * 
 * 2     04-04-15 10:19p Servantes
 * 
 * 1     03-12-23 4:57p Kjmgo
 * 
 *
 * <HR>
 *
 * @file	Potal.cpp
 */

#include "Engine_Export.h"
#include "Potal.h"
#include <string>

/**
 *
 */
CPotal::CPotal()
{
	m_pArea		= NULL;
	m_bViewAll	= FALSE;	
} //CPotal::CPotal

/**
 *
 */
CPotal::~CPotal()
{
	SAFE_DELETE( m_pArea );
} //CPotal::~CPotal

/**
 *	따로 영역을 지정하지 않고, 넣는대로 찍는걸로 설정한다.
 */
void CPotal::SetViewAll()
{
	m_bViewAll = TRUE;
} //CPotal::SetViewAll

/**
 *
 */
BOOL CPotal::Load( const char* szFile )
{
	Assert( szFile );

	std::string str( "Map/Potal/" );

	str += szFile;
	FILE* fp = fopen( str.c_str(), "rt" );

	if( NULL == fp )
	{
		return FALSE;
	} //if

	char szBuffer[256];
	fgets( szBuffer, 256, fp );

	int nCount = 0;
	if( sscanf( szBuffer, "%d", &nCount ) != 1 )
	{
		Assert( !"sscanf 오류" );
	}


	m_pArea = new CInArea;

	if( !m_pArea ||
		!m_pArea->Init( nCount ) )
	{
		fclose( fp );
		return FALSE;
	} //if

	for( int i = 0 ; i < nCount ; i++ )
	{
		int nId = 0;
		Vector2 vPos;
		fgets( szBuffer, 256, fp );
		if( sscanf( szBuffer, "%d %f %f", &nId, &vPos.x, &vPos.y ) != 3 )
		{
			Assert( !"sscanf 오류" );
		}


		m_pArea->AddPoint( vPos );

	} //for

//	fclose( fp );


	fclose( fp );
	return TRUE;
} //CPotal::Load

/**
 *
 */
void CPotal::Cleanup()
{
	m_listCreature.clear();
} //CPotal::Cleanup

/**
 *
 */
BOOL CPotal::CheckCreature( IW3DCreature*	pCreature )
{
	if( !pCreature )	return TRUE;

	if( m_bViewAll || !m_pArea )
	{
		m_listCreature.push_back( pCreature );
		return TRUE;
	} //if

	if( !m_pArea->Check( pCreature->GetPosition() ) )
	{
		return FALSE;
	} //if

	m_listCreature.push_back( pCreature );
	return TRUE;
} //CPotal::CheckCreature

/**
 *
 */
void CPotal::Render( BOOL bShadow )
{
	if( m_listCreature.empty() )		return;

	IW3DCreature*	pCharacter = NULL;

	CreatureList::iterator	itor = m_listCreature.begin();

	while( itor != m_listCreature.end() )
	{
		pCharacter = *itor;

		if( pCharacter )
		{
			if( !bShadow )
			{
				pCharacter->Draw();
			}
			else
			{
				pCharacter->DrawShadow();
			} //if..else..
		} //if

		itor++;
	} //while

} //CPotal::Render

/**
 *
 */
BOOL CPotal::IsInArea( IW3DCreature* pCreature )
{
	if( !pCreature )	return FALSE;
	if( m_bViewAll || !m_pArea ) return TRUE;

	Vector3& vPos = pCreature->GetPosition();
	Vector2 v( vPos.x, vPos.y );

	return m_pArea->Check( v );
} //CPotal::IsInArea

/**
 *
 */
BOOL CPotal::IsInArea( Vector3& vPos )
{
	if( m_bViewAll || !m_pArea ) return TRUE;

	Vector2 v( vPos.x, vPos.y );

	return m_pArea->Check( v );
} //CPotal::IsInArea
 




/**
 *
 */
CInArea::CInArea()
:	m_avVector	( NULL ),
	m_abSign	( NULL ),
	m_nCount	( 0 ),
	m_nEnd		( 0 ),
	m_nSign		( 0 )
{


} //CInArea::CInArea

/**
 *
 */
CInArea::~CInArea()
{
	SAFE_DELETE_ARRAY( m_avVector );
	SAFE_DELETE_ARRAY( m_abSign );
} //CInArea::~CInArea

/**
 * 
 */
BOOL CInArea::Init( const int nCount )
{
	Assert( nCount );

	if( nCount <= 0 ) return FALSE;

	m_nCount = nCount;

	m_avVector	= new Vector2[m_nCount];
	m_abSign	= new BOOL[m_nCount];
	
	if( !m_avVector || !m_abSign )
	{
		return FALSE;
	} //if
	
	m_nSign = 0;
	m_nEnd = 0;

	return TRUE;
} //CInArea::Init

/**
 *
 */
void CInArea::AddPoint( const Vector2& point )
{
	if( m_nCount < m_nEnd )
	{		
		Assert( !"많은 점을 추가하려 합니다." );
		return ;
	} //if

	m_avVector[m_nEnd] = point;
	m_nEnd++;
} //CInArea::AddPoint

/**
 *
 */
BOOL CInArea::Check( const Vector3& point )
{
	Vector2 vPoint( point.x, point.y );
	return Check( vPoint );
} //CInArea::Check

/**
 *
 */
BOOL CInArea::Check( Vector2& point )
{
	m_nSign = 0;
	m_nDiffSign = 0;
	BOOL bFlag = TRUE;

	// 마지막 전까지만 한다.
	for( int i = 0 ; i < m_nEnd - 1; i++ )
	{
		if( !Calculation( m_avVector[i], m_avVector[i+1], point ) )
		{
			bFlag = FALSE;
			break;
		} //if
	} //for

	if( !bFlag )
	{
		return FALSE;
	} //if

	// 마지막것은, 다시 처음 위치와 비교 해야 하므로, 따로 해준다.
	bFlag = Calculation( m_avVector[m_nEnd - 1], m_avVector[0], point );

	return bFlag;
} //CInArea::Check

/**
 *
 */
BOOL CInArea::Calculation( Vector2& vStart, Vector2& vEnd, Vector2& vCheck )
{
	Vector2 vDir1, vDir2;

	vDir1 = vStart - vEnd;
	vDir2 = vStart - vCheck;

	float fValue = ( vDir1.x * vDir2.y ) - ( vDir1.y * vDir2.x );

	if( fValue == 0 )
	{
		// 0 일경우, 그냥 무시한다. 
		return TRUE; 
	} //if

	if( CheckSign( IsPlus( fValue ) ) )
	{
		AddSign( IsPlus( fValue ) );
		return TRUE;
	} //if

	return FALSE;
} //CInArea::Calculation