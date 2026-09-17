/**
 * Quad Tree
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-11 7:55p $
 * @version	$Revision: 13 $
 *
 * <HR>
 * $Log: /Engine/Src/QuadTree.cpp $
 * 
 * 13    04-01-11 7:55p Kjmgo
 * 코드정리
 * 
 * <HR>
 *
 * @file	QuadTree.cpp
 */

#include "Engine_Export.h"
#include "QuadTree.h"

ObjectList g_listObject;

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// QuadTree
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/**
 * 생성자
 */
CQuadTree::CQuadTree()
:	m_fSize		( 500.0F ),
	m_nDepth	( 3 ),
	m_pRoot		( NULL ),
	m_nCount	( 0 ),
	m_vCamera	( 0.0F, 0.0F, 0.0F )
{
	
} //CQuadTree::CQuadTree

/**
 * 소멸자
 */
CQuadTree::~CQuadTree()
{
	Clean();
	
	SAFE_DELETE( m_pRoot );
} //CQuadTree::~CQuadTree

/**
 *
 */
BOOL CQuadTree::Init()
{
	Assert( !m_pRoot );
	SAFE_DELETE( m_pRoot );

	m_pRoot = new CNode;

	if( !m_pRoot )
	{
		return FALSE;
	} //if

	return TRUE;
} //CQuadTree::Init

/**
 *	전체 크기를 설정
 */
void CQuadTree::SetSize( float fSize )
{
	Assert( fSize > 0 );

	m_fSize = fSize;
} //CQuadTree::SetSize

/**
 * 깊이를 설정
 */
void CQuadTree::SetDepth( int nDepth )
{
	Assert( nDepth > 1 );

	m_nDepth = nDepth;
} //CQuadTree::SetDepth

/**
 * 분할한다.
 */
void CQuadTree::Separate()
{
	Assert( m_pRoot );
	if( m_pRoot )
	{
		m_pRoot->Separate( 0, 0, m_fSize, m_nDepth );
	} //if
} //CQuadTree::Separate

/**
 * 오브젝트를 집어 넣는다.
 */
void CQuadTree::Push_Back( IObject* pObject )
{
	if( pObject )
	{
		m_nCount++;
		m_pRoot->Push_Back( pObject );
	} //if
} //CQuadTree::Push_Back

/**
 * 오브젝트를 집어 넣는다.
 */
void CQuadTree::Push_Front( IObject* pObject )
{
	if( pObject )
	{
		m_nCount++;
		m_pRoot->Push_Front( pObject );
	} //if
} //CQuadTree::Push_Front

/**
 * 모든걸 처음으로 돌린다.
 */
void CQuadTree::Clean()
{
	SAFE_DELETE( m_pRoot );

	m_pRoot = new CNode;

	if( !m_pRoot )
	{
		Assert( !"메모리 생성 실패" );
		return ;
	} //if
	
	m_pRoot->Clean();
	m_nCount = 0;
} //CQuadTree::Clean

/**
 *
 */
BOOL CQuadTree::MakeList( const Vector3& vPos )
{
	// 카메라 위치가 바뀌지 않았다면 굳이 List를 갱신할 필요는 없다.
	if( m_vCamera == vPos )
	{
		return FALSE;
	} //if

	g_listObject.clear();

	m_pRoot->MakeList( vPos );

	m_vCamera = vPos;

	return TRUE;
 
} //CQuadTree::Draw

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// Node
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

/**
 *
 */
CNode::CNode()
{
	for( int i = 0 ; i < 4 ; i++ )
	{
		m_pChild[i] = NULL;
	} //for
} //CNode::CNode

/**
 *
 */
CNode::~CNode()
{
	Clean();
} //CNode::~CNode

/**
 *
 */
void CNode::Push_Back( IObject* pObject )
{
	if( pObject )
	{
		m_listObject.push_back( pObject );
	} //if
} //CNode::Push_Back

/**
 *
 */
void CNode::Push_Front( IObject* pObject )
{
	if( pObject )
	{
		m_listObject.push_front( pObject );
	} //if
} //CNode::Push_Front

/**
 *
 */
BOOL CNode::IsInRect( float x, float y )
{
	if( x >= m_fStartX && y >= m_fStartY )
	{
		if( x < m_fEndX && y < m_fEndY )
		{
			return TRUE;
		} //if
	} //if
	return FALSE;
} //CNode::IsInRect
/**
 *
 */
void CNode::Separate( float fStartX, float fStartY, float fSize, int nDepth )
{
	if( nDepth <= 0 )
	{
		return ;
	} //if

	m_nDepth = nDepth;

	float fHalf = (int)( fSize / 2 );
	m_fStartX = fStartX - fHalf;
	m_fStartY = fStartY - fHalf;
	
	m_fSize = fSize + fHalf;
	m_fEndX = fStartX + m_fSize;
	m_fEndY = fStartY + m_fSize;

	for( int i = 0 ; i < 4 ; i++ )
	{
		if( !m_pChild[i] )
		{
			m_pChild[i] = new CNode;
		} //if
	} //for

	ObjectList::iterator itor = m_listObject.begin();
	IObject* pObject = NULL;

	while( itor != m_listObject.end() )
	{
		pObject = *itor;

		if( !pObject )
		{
			itor = m_listObject.erase( itor );
			continue;
		} //if

		if( pObject->IsInRect( fStartX, fStartY, fHalf ) )
			m_pChild[0]->Push_Back( pObject );

		if( pObject->IsInRect( fStartX + fHalf, fStartY, fHalf ) )
			m_pChild[1]->Push_Back( pObject );

		if( pObject->IsInRect( fStartX, fStartY + fHalf, fHalf ) )
			m_pChild[2]->Push_Back( pObject );

		if( pObject->IsInRect( fStartX + fHalf, fStartY + fHalf, fHalf ) )
			m_pChild[3]->Push_Back( pObject );

		itor++;
	} //while

	for( i = 0 ; i < 4 ; i++ )
	{
		if( m_pChild[i]->GetSize() <= 0 )
		{
			SAFE_DELETE( m_pChild[i] );
		} //if
	} //for

	if( nDepth != 1 )
	{
		ObjectList::iterator itor = m_listObject.begin();

		while( itor != m_listObject.end() )
		{
			itor = m_listObject.erase( itor );
		} //while

		m_listObject.clear();
	} //if

	nDepth--;

	if( m_pChild[0] )
		m_pChild[0]->Separate( fStartX			, fStartY		 , fHalf, nDepth );	// 1 : 1

	if( m_pChild[1] )
		m_pChild[1]->Separate( fStartX + fHalf	, fStartY		 , fHalf, nDepth );	// 2 : 1

	if( m_pChild[2] )
		m_pChild[2]->Separate( fStartX			, fStartY + fHalf, fHalf, nDepth );	// 1 : 2

	if( m_pChild[3] )
		m_pChild[3]->Separate( fStartX + fHalf	, fStartY + fHalf, fHalf, nDepth );	// 2 : 2
} //CNode::Separate

/**
 *
 */
void CNode::Clean()
{
	ObjectList::iterator itor = m_listObject.begin();

	while( itor != m_listObject.end() )
	{
		itor = m_listObject.erase( itor );
	} //while

	Assert( m_listObject.empty() );
	m_listObject.clear();

	for( int i = 0 ; i < 4 ; i++ )
	{
		if( m_pChild[i] )
		{
			m_pChild[i]->Clean();
			SAFE_DELETE( m_pChild[i] );
		} //if
	} //for

} //CNode::Clean

/**
 *
 */
void CNode::MakeList( const Vector3& vPos )
{
	int nCount = 0;

	for( int i = 0 ; i < 4 ; i++ )
	{
		if( m_pChild[i] && m_pChild[i]->IsInRect( vPos.x, vPos.y ) )
		{
			m_pChild[i]->MakeList( vPos );
		} //if
	} //for

	ObjectList::iterator itor = m_listObject.begin();
	IObject* pObject = NULL;

	while( itor != m_listObject.end() )
	{
		pObject = *itor;

		if( pObject )
		{
			g_listObject.push_back( pObject );
			nCount++;			
		} //if

		itor++;
	} //while
} //CNode::MakeList

/**
 *
 */
int CNode::GetCount( float x, float y )
{
	int nCount = 0;

	for( int i = 0 ; i < 4 ; i++ )
	{
		if( m_pChild[i] && m_pChild[i]->IsInRect( x, y ) )
		{
			nCount += m_pChild[i]->GetCount( x, y );
		} //if
	} //for

	ObjectList::iterator itor = m_listObject.begin();
	IObject* pObject = NULL;

	while( itor != m_listObject.end() )
	{
		pObject = *itor;
		nCount++;
		itor++;
	} //while

	return nCount;
} //CNode::GetCount

/**
 *
 */
int CNode::GetSize()
{
	return m_listObject.size();
} //CNode::GetSize

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=