/**
 * 길 찾기 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-10 8:33p $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /Engine/Src/PathFind.cpp $
 * 
 * 12    03-12-10 8:33p Kjmgo
 * 길찾기 수정, Optimal을 선택적으로 구하도록 수정
 * 
 * 11    03-05-10 12:33p Ahastudio
 *
 * 10    03-04-26 11:39a Ahastudio		IsVisit(), SetVisit() 추가.
 * <HR>
 *
 * @file	PathFind.cpp
 */


#include "Engine_Export.h"
#include "PathFind.h"


/**
 * 생성자.
 */
CPathFind::CPathFind( const DWORD dwSize )
:	m_dwSize	( dwSize ),
	m_pbtMovable( new BYTE [m_dwSize * m_dwSize] ),
	m_pbtVisit	( new BYTE [m_dwSize * m_dwSize] )
{
} //CPathFind::CPathFind

/**
 * 소멸자.
 */
CPathFind::~CPathFind()
{
	SAFE_DELETE( m_pbtVisit );
	SAFE_DELETE( m_pbtMovable );
} //CPathFind::~CPathFind

/**
 * 이동 속성을 설정한다.
 */
void CPathFind::SetMovable( BYTE* pbtMovable )
{
	Assert( pbtMovable );

	_MemCpy( m_pbtMovable, pbtMovable, m_dwSize * m_dwSize * sizeof( BYTE ) );
} //CPathFind::~CPathFind

/**
 * 길 찾기.
 */
vector<Vector3> CPathFind::Find(
	const Vector3&	vStart,		///< [IN] 시작점
	const Vector3&	vTarget,	///< [IN] 목표점
	BOOL bOptimal
	)
{
	Assert( m_pbtMovable );

	int		nStartX		= (int)vStart.x;
	int		nStartY		= (int)vStart.y;
	int		nTargetX	= (int)vTarget.x;
	int		nTargetY	= (int)vTarget.y;

	Assert( nStartX  >= 0 && nStartX  < (int)m_dwSize );
	Assert( nStartY  >= 0 && nStartY  < (int)m_dwSize );
	Assert( nTargetX >= 0 && nTargetX < (int)m_dwSize );
	Assert( nTargetY >= 0 && nTargetY < (int)m_dwSize );

	_ZeroMemory( m_pbtVisit, m_dwSize * m_dwSize * sizeof( BYTE ) );

	SPathNode*		pBest = FindPath( nStartX, nStartY, nTargetX, nTargetY );
	vector<SPath>&	aPath = GetPath( pBest, bOptimal );

	vector<Vector3>	avPath;
	avPath.clear();

	for( int i = 0 ; i < (int)aPath.size() ; i++ )
	{
		Vector3		vPos;
		vPos.x = (float)aPath[i].nX + 0.5F;
		vPos.y = (float)aPath[i].nY + 0.5F;
		avPath.push_back( vPos );
	} //for

	return avPath;
} //CPathFind::Find

/**
 * 이동 가능 여부를 얻는다.
 *
 * @return 이동 가능 여부 ( TRUE - 이동 가능 )
 */
BOOL CPathFind::IsMove( const int nX, const int nY )
{
	if( nX < 0 )				return FALSE;
	if( nY < 0 )				return FALSE;
	if( nX >= (int)m_dwSize )	return FALSE;
	if( nY >= (int)m_dwSize )	return FALSE;

	if( m_pbtMovable[nY * m_dwSize + nX] )
	{	//갈 수 없는 곳이다.
		return FALSE;
	} //if

	return TRUE;
} //CPathFind::IsMovable

/**
 * 검색 여부를 얻는다.
 *
 * @return 검색 여부 ( TRUE - 이미 검색 했음. )
 */
BOOL CPathFind::IsVisit( const int nX, const int nY )
{
	Assert( 0 <= nX && nX < (int)m_dwSize );
	Assert( 0 <= nY && nY < (int)m_dwSize );

	return m_pbtVisit[nY * m_dwSize + nX];
} //CPathFind::IsMovable

/**
 * 검색했음을 기록한다.
 */
void CPathFind::SetVisit( const int nX, const int nY )
{
	Assert( 0 <= nX && nX < (int)m_dwSize );
	Assert( 0 <= nY && nY < (int)m_dwSize );

	m_pbtVisit[nY * m_dwSize + nX] = TRUE;
} //CPathFind::SetVisit
