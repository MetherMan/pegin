/**
 * A* 길찾기 모듈 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-10 10:56p $
 * @version	$Revision: 15 $
 *
 * <HR>
 * $Log: /Engine/Src/AStar.cpp $
 * 
 * 15    03-12-10 10:56p Kjmgo
 * 길찾기, 피킹 1차 완료
 * 
 * 14    03-12-10 8:33p Kjmgo
 * 길찾기 수정, Optimal을 선택적으로 구하도록 수정
 * 
 * 13    03-12-10 2:00p Kjmgo
 * 길찾기및, 피킹 수정
 * 
 * 12    03-12-10 9:50a Kjmgo
 * Assert 주석처리
 * 
 * 11    03-12-05 6:34a Kjmgo
 * 
 * 10    03-06-28 10:16a Kjmgo
 * Assert 추가
 * 
 * 9     03-05-10 12:33p Ahastudio
 *
 * 7     03-04-26 11:39a Ahastudio		Node 메모리 할당을 Node Pooling으로 교체.
 * <HR>
 *
 * @file	AStar.cpp
 */


#include "Engine_Export.h"
#include "PathFind.h"
#include <vector>
#include <queue>


using std::vector;
using std::queue;


/**
 * 생성자.
 */
CAStar::CAStar()
:	m_iNodePoolPos( 0 )
{
} //CAStar::CAStar

/**
 * 소멸자.
 */
CAStar::~CAStar()
{
	ClearOpenList();
	ClearClosedList();
} //CAStar::~CAStar

/**
 * Open List를 비운다.
 */
void CAStar::ClearOpenList()
{
	SPathNode*	pNode = m_OpenList.pNext;

	while( pNode )
	{
		SPathNode*	pTemp = pNode;
		pNode = pNode->pNext;
	} //while

	m_OpenList.pNext = NULL;
} //CAStar::ClearOpenList

/**
 * Closed List를 비운다.
 */
void CAStar::ClearClosedList()
{
	SPathNode*	pNode = m_ClosedList.pNext;

	while( pNode )
	{
		SPathNode*	pTemp = pNode;
		pNode = pNode->pNext;
	} //while

	m_ClosedList.pNext = NULL;
} //CAStar::ClearClosedList

/**
 * 노드를 Open List에 추가한다.
 * Fitness Sorting 순으로 Open List에 넣는다.
 */
void CAStar::InsertOpenNode( SPathNode* pNode )
{
	if( NULL == m_OpenList.pNext )
	{
		m_OpenList.pNext = pNode;
		return;
	} //if

	SPathNode*	pPrev = &m_OpenList;
	SPathNode*	pTemp = pPrev->pNext;

	while( pTemp && ( pTemp->nTotalCost < pNode->nTotalCost ) )
	{
		pPrev = pTemp;
		pTemp = pTemp->pNext;
	} //while

	pNode->pNext = pTemp;
	pPrev->pNext = pNode;
} //CAStar::InsertNode

/**
 * Open List에서 노드를 찾는다.
 */
SPathNode* CAStar::GetOpenNode(
	const int	nX,
	const int	nY
	)
{
	SPathNode*	pNode = m_OpenList.pNext;

	while( pNode )
	{
		if( ( nX == pNode->nX ) &&
			( nY == pNode->nY ) )
		{
			return pNode;
		} //if

		pNode = pNode->pNext;
	} //while

	return NULL;
} //CAStar::GetOpenNode

/**
 * Closed List에서 노드를 찾는다.
 */
SPathNode* CAStar::GetClosedNode(
	const int	nX,
	const int	nY
	)
{
	SPathNode*	pNode = m_ClosedList.pNext;

	while( pNode )
	{
		if( ( nX == pNode->nX ) &&
			( nY == pNode->nY ) )
		{
			return pNode;
		} //if

		pNode = pNode->pNext;
	} //while

	return NULL;
} //CAStar::GetClosedNode

/**
 * 노드를 할당한다.
 */
SPathNode* CAStar::AllocNode()
{
	if( m_iNodePoolPos >= NODE_POOL_SIZE )
	{	// 노드 할당 실패.
		return NULL;
	} //if

	SPathNode*	pNode = m_aNodePool + m_iNodePoolPos;
	m_iNodePoolPos++;

	pNode->Clear();

	return pNode;
} //CAStar::AllocNode

/**
 * 길 찾기.
 */
SPathNode* CAStar::FindPath(
	const int	nStartX,		///< [IN] 시작 X
	const int	nStartY,		///< [IN] 시작 Y
	const int	nTargetX,		///< [IN] 목표 X
	const int	nTargetY		///< [IN] 목표 Y
	)
{
	ClearOpenList();
	ClearClosedList();

	m_iNodePoolPos = 0;		// Node Pool을 확보한다.

	// Open List에 시작 노드를 넣는다.

	SPathNode*		pStart = AllocNode();
	CHECK_PTR_RETURN( pStart );

	int		dx = ABS( nTargetX - nStartX ) * 2;
	int		dy = ABS( nTargetY - nStartY ) * 2;

	pStart->nX				= nStartX;
	pStart->nY				= nStartY;
	pStart->nCostFromStart	= 0;
	pStart->nCostToGoal		= dx * dx + dy * dy;
	pStart->nTotalCost		= pStart->nCostToGoal;

	m_OpenList.pNext = pStart;

	int nLimit = 0;

	while( 1 )
	{
		SPathNode*	pBest = m_OpenList.pNext;	// Open List에서 노드를 하나 가져온다.

		if( NULL == pBest )
		{	// 목적지에 도달할 수 없다.
			DEBUG_OUT( "길찾기 실패" );
			return NULL;
		} //if

		if( ( nTargetX == pBest->nX ) &&
			( nTargetY == pBest->nY ) )
		{	// 목적지에 도착했다면 종료.
//			DEBUG_OUT( "길찾기 완료" );
			return pBest;
		} //if

		// Open List에서 Closed List로 노드를 이동한다.

		m_OpenList.pNext	= pBest->pNext;

		pBest->pNext		= m_ClosedList.pNext;
		m_ClosedList.pNext	= pBest;

		// 주변을 탐색한다.

		if( !MakeChild( pBest, nTargetX, nTargetY ) )
		{	// 길찾기 실패.
			break;
		} //if
		
		nLimit++;

		if( nLimit >= 200 )
		{
			break;
		} //if

	} //while

	DEBUG_OUT( "길찾기 실패" );
	return NULL;	// 목적지까지 탐색 실패.
} //CAStar::Find

/**
 * 패스를 얻는다.
 */
vector<SPath>& CAStar::GetPath( SPathNode* pNode, BOOL bOptimal )
{
	SPathNode*		pBest = pNode;
	vector<SPath>	aPath;
	aPath.clear();

	int		iMax = NODE_POOL_SIZE;

	while( pBest && pBest->nCostFromStart > 1 )
	{
		SPath	path;
		path.nX = pBest->nX;
		path.nY = pBest->nY;
		aPath.push_back( path );

		pBest = pBest->pPrev;

		if( --iMax <= 0 )
		{
			DEBUG_OUT( "길찾기 최대 패스 초과" );
			break;
		} //if
	} //while

	ClearOpenList();
	ClearClosedList();

	static vector<SPath>	s_aPath;
	s_aPath.resize( aPath.size() );

	for( int i = 0 ; i < (int)aPath.size() ; i++ )
	{
		s_aPath[i] = aPath[aPath.size() - i - 1];
	} //for

	if( bOptimal )
	{
		return GetPathOptimal( s_aPath );
	}
	else
	{
		return s_aPath;
	} //if..else..

	return s_aPath;
} //CAStar::GetPath

/**
 * 경로를 최적화한다.
 */
vector<SPath>& CAStar::GetPathOptimal( const vector<SPath>& aOrigPath )
{
	static vector<SPath>	s_aPath;
	s_aPath.clear();

	if( aOrigPath.empty() )
	{
		return s_aPath;
	} //if

	s_aPath.push_back( aOrigPath[0] );

	for( int i = 0 ; i < (int)aOrigPath.size() ; i++ )
	{
		int		nStartX = aOrigPath[i].nX;
		int		nStartY = aOrigPath[i].nY;

		int		nTarget;

		for( nTarget = i + 1 ; nTarget < (int)aOrigPath.size() ; nTarget++ )
		{
			int		nEndX = aOrigPath[nTarget].nX;
			int		nEndY = aOrigPath[nTarget].nY;

			if( !IsStraight( nStartX, nStartY, nEndX, nEndY ) )
			{
				nTarget--;
				break;
			} //if
		} //for

		if( nTarget >= (int)aOrigPath.size() )
		{	// 마지막 녀석일 경우
			s_aPath.push_back( aOrigPath[aOrigPath.size() - 1] );
			break;
		} //if

		s_aPath.push_back( aOrigPath[nTarget] );

		i = nTarget;
	} //for

//	Assert( s_aPath.size() <= aOrigPath.size() );

	return s_aPath;
} //CAStar::GetPathOptimal

/**
 * 주변을 탐색한다.
 */
BOOL CAStar::MakeChild(
	SPathNode*	pNode,		///< 탐색할 노드
	const int	nTargetX,	///< 목적지 X 좌표
	const int	nTargetY	///< 목적지 Y 좌표
	)
{
	int		nX = pNode->nX;
	int		nY = pNode->nY;
	BOOL	abMovable[8];
	BOOL	bResult = TRUE;

	abMovable[0] = IsMove( nX + 1, nY     );
	abMovable[1] = IsMove( nX + 1, nY - 1 );
	abMovable[2] = IsMove( nX    , nY - 1 );
	abMovable[3] = IsMove( nX - 1, nY - 1 );
	abMovable[4] = IsMove( nX - 1, nY     );
	abMovable[5] = IsMove( nX - 1, nY + 1 );
	abMovable[6] = IsMove( nX    , nY + 1 );
	abMovable[7] = IsMove( nX + 1, nY + 1 );

	if( abMovable[2] )
	{	// 위
		bResult &= MakeChildSub( pNode, nX, nY - 1, nTargetX, nTargetY );
	} //if
	if( abMovable[6] )
	{	// 아래
		bResult &= MakeChildSub( pNode, nX, nY + 1, nTargetX, nTargetY );
	} //if
	if( abMovable[4] )
	{	// 왼쪽
		bResult &= MakeChildSub( pNode, nX - 1, nY, nTargetX, nTargetY );
	} //if
	if( abMovable[0] )
	{	// 오른쪽
		bResult &= MakeChildSub( pNode, nX + 1, nY, nTargetX, nTargetY );
	} //if
	if( abMovable[3] && abMovable[2] && abMovable[4] )
	{	// 위 + 왼쪽
		bResult &= MakeChildSub( pNode, nX - 1, nY - 1, nTargetX, nTargetY );
	} //if
	if( abMovable[1] && abMovable[2] && abMovable[0] )
	{	// 위 + 오른쪽
		bResult &= MakeChildSub( pNode, nX + 1, nY - 1, nTargetX, nTargetY );
	} //if
	if( abMovable[5] && abMovable[6] && abMovable[4] )
	{	// 아래 + 왼쪽
		bResult &= MakeChildSub( pNode, nX - 1, nY + 1, nTargetX, nTargetY );
	} //if
	if( abMovable[7] && abMovable[6] && abMovable[0] )
	{	// 아래 + 오른쪽
		bResult &= MakeChildSub( pNode, nX + 1, nY + 1, nTargetX, nTargetY );
	} //if

	return bResult;
} //CAStar::MakeChild

/**
 * 노드를 갱신 또는 추가한다.
 */
BOOL CAStar::MakeChildSub(
	SPathNode*	pNode,		///< [IN] 부모 노드
	const int	nX,			///< [IN] X 좌표
	const int	nY,			///< [IN] Y 좌표
	const int	nTargetX,	///< [IN] 목적지 X 좌표
	const int	nTargetY	///< [IN] 목적지 Y 좌표
	)
{
	const int		nNewCost = pNode->nCostToGoal + 1;

	SPathNode*	pChild;

/*
	if( pChild = GetOpenNode( nX, nY ) )
	{	// Open List에 존재하는 노드라면....
		for( int i = 0 ; i < 8 ; i++ )
		{
			if( NULL == pNode->apDir[i] )
			{
				pNode->apDir[i] = pChild;
				break;
			} //if
		} //for

		Assert( i < 8 );

		if( nNewCost < pChild->nCostFromStart )
		{
			pChild->nCostFromStart	= nNewCost;
			pChild->nTotalCost		= pChild->nCostFromStart + pChild->nCostToGoal;
			pChild->pPrev			= pNode;
		} //if
	}
	else if( pChild = GetClosedNode( nX, nY ) )
	{	// Closed List에 존재하는 노드라면....
		for( int i = 0 ; i < 8 ; i++ )
		{
			if( NULL == pNode->apDir[i] )
			{
				pNode->apDir[i] = pChild;
				break;
			} //if
		} //for

		Assert( i < 8 );

		if( nNewCost < pChild->nCostFromStart )
		{
			pChild->nCostFromStart	= nNewCost;
			pChild->nTotalCost		= pChild->nCostFromStart + pChild->nCostToGoal;
			pChild->pPrev			= pNode;

			MakeDown( pChild );
		} //if
	}
*/
	if( IsVisit( nX, nY ) )
	{
		/* NULL */
	}
	else
	{	// 새로운 노드 추가.
		pChild = AllocNode();
		if( !pChild )
		{	// 노드 할당 실패.
			return FALSE;
		} //if

		int		dx = ABS( nTargetX - nX ) * 2;
		int		dy = ABS( nTargetY - nY ) * 2;

		pChild->nX				= nX;
		pChild->nY				= nY;
		pChild->nCostFromStart	= nNewCost;
		pChild->nCostToGoal		= dx * dx + dy * dy;
		pChild->nTotalCost		= pChild->nCostFromStart + pChild->nCostToGoal;
		pChild->pPrev			= pNode;

		InsertOpenNode( pChild );

		for( int i = 0 ; i < 8 ; i++ )
		{
			if( NULL == pNode->apDir[i] )
			{
				pNode->apDir[i] = pChild;
				break;
			} //if
		} //for

		Assert( i < 8 );

		SetVisit( nX, nY );
	} //if..else if..else..

	return TRUE;
} //CAStar::MakeChildSub

/**
 * Cost From Start로 사방 노드를 끝까지 패스 정리한다.
 */
void CAStar::MakeDown( SPathNode* pNode )
{
	Assert( pNode );

	queue<SPathNode*>	queueNode;

	const int	nNewCost = pNode->nCostFromStart + 1;

	for( int i = 0 ; i < 8 ; i++ )
	{
		SPathNode*	pDir = pNode->apDir[i];
		if( NULL == pDir )		break;

		if( nNewCost < pDir->nCostFromStart )
		{
			pDir->nCostFromStart	= nNewCost;
			pDir->nTotalCost		= pDir->nCostFromStart + pDir->nCostToGoal;
			pDir->pPrev				= pNode;
			queueNode.push( pDir );
		} //if
	} //for

	while( !queueNode.empty() )
	{
		SPathNode*	pNode = queueNode.front();
		queueNode.pop();

		Assert( pNode );

		if( !pNode )
		{
			continue;
		}//

		const int	nNewCost = pNode->nCostFromStart + 1;

		for( int i = 0 ; i < 8 ; i++ )
		{
			SPathNode*	pDir = pNode->apDir[i];
			if( NULL == pDir )		break;

			if( nNewCost < pDir->nCostFromStart )
			{
				pDir->nCostFromStart	= nNewCost;
				pDir->nTotalCost		= pDir->nCostFromStart + pDir->nCostToGoal;
				pDir->pPrev				= pNode;
				queueNode.push( pDir );
			} //if
		} //for
	} //while
} //CAStar::MakeDown

/**
 * 직선 이동 가능한지 검색한다.
 * Bresenham 알고리즘으로 선분을 그으면서 중간에 장애물이 있는지 검사한다.
 *
 * @return	이동 가능일 경우 TRUE, 불가능일 경우 FALSE를 돌려준다.
 */
BOOL CAStar::IsStraight(
	const int	nStartX,	///< [IN] 시작점 X 좌표
	const int	nStartY,	///< [IN] 시작점 Y 좌표
	const int	nEndX,		///< [IN] 종착점 X 좌표
	const int	nEndY		///< [IN] 종착점 Y 좌표
	)
{
	int		nDeltaX = nEndX - nStartX;
	int		nDeltaY = nEndY - nStartY;

	int		nStepX;
	int		nStepY;

	if( nDeltaX > 0 )			nStepX = 1;
	else if( 0 == nDeltaX )		nStepX = 0;
	else						nStepX = -1;

	if( nDeltaY > 0 )			nStepY = 1;
	else if( 0 == nDeltaY )		nStepY = 0;
	else						nStepY = -1;

	nDeltaX = ABS( nDeltaX );
	nDeltaY = ABS( nDeltaY );

	int		nDistance = MAX( nDeltaX, nDeltaY );

	int		nErrorX = 0;
	int		nErrorY = 0;

	int		nX = nStartX;
	int		nY = nStartY;

	for( int i = 0 ; i < nDistance ; i++ )
	{
		if( !IsMove( nX, nY ) )
		{	// 직선 이동 불가능.
			return FALSE;
		} //if

		nErrorX += nDeltaX;
		nErrorY += nDeltaY;

		if( nErrorX > nDistance )
		{
			nErrorX -= nDistance;
			nX += nStepX;
		} //if

		if( nErrorY > nDistance )
		{
			nErrorY -= nDistance;
			nY += nStepY;
		} //if
	} //for

	return TRUE;
} //CAStar::IsStraight
