/**
 * A* 길찾기 모듈 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-10 8:33p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Engine/Src/AStar.h $
 * 
 * 9     03-12-10 8:33p Kjmgo
 * 길찾기 수정, Optimal을 선택적으로 구하도록 수정
 * 
 * 8     03-05-06 10:03p Ahastudio
 *
 * 7     03-04-26 11:39a Ahastudio		Node 메모리 할당을 Node Pooling으로 교체.
 * <HR>
 *
 * @file	AStar.h
 */


#ifndef __AStar_Header__
#define __AStar_Header__


#include <vector>


using std::vector;


/**
 * 길찾기 노드.
 * @ingroup		Engine
 */
struct SPathNode
{
	UNDER_MEMORY_MGR( SPathNode );

	int			nX;					///< X 좌표
	int			nY;					///< Y 좌표

	int			nCostFromStart;		///< 휴리스틱 : 출발점과의 거리(의 제곱)
	int			nCostToGoal;		///< 목표     : 목적지까지 거리(의 제곱)
	int			nTotalCost;			///< 적합도   : f(n) = g(n) + h(n)

	SPathNode*	pPrev;				///< 길찾기용 이전 노드
	SPathNode*	apDir[8];			///< 8방향 노드
	SPathNode*	pNext;				///< 단일 Linked List용 다음 노드

	/**
	 * 생성자.
	 */
	SPathNode()
	{
		Clear();
	} //SPathNode

	/**
	 * 초기화.
	 */
	void	Clear()
	{
		pPrev = NULL;
		pNext = NULL;

		for( int i = 0 ; i < 8 ; i++ )
		{
			apDir[i] = NULL;
		} //for
	} //SPathNode
}; //struct SPathNode


/**
 * 길 좌표.
 * @ingroup		Engine
 */
struct SPath
{
	int		nX;
	int		nY;
}; //struct SPath


/**
 * A* 길찾기 베이스 클래스.
 * @ingroup		Engine
 */
class CAStar
{
public:
	UNDER_MEMORY_MGR( CAStar );

	CAStar();
	~CAStar();

protected:
	SPathNode*		FindPath( const int nStartX, const int nStartY, const int nTargetX, const int nTargetY );
	vector<SPath>&	GetPath( SPathNode* pNode, BOOL bOptimal = FALSE );

	virtual BOOL	IsMove( const int nX, const int nY ) = 0;
	virtual BOOL	IsVisit( const int nX, const int nY ) = 0;

	virtual void	SetVisit( const int nX, const int nY ) = 0;

private:
	enum
	{
		NODE_POOL_SIZE = 500 * 500					///< Node Pool 크기
	}; //enum

	SPathNode		m_OpenList;						///< Open List
	SPathNode		m_ClosedList;					///< Closed List
	SPathNode		m_aNodePool[NODE_POOL_SIZE];	///< Node Pool
	int				m_iNodePoolPos;					///< Node 할당용 Position

	void			ClearOpenList();
	void			ClearClosedList();
	void			InsertOpenNode( SPathNode* pNode );

	SPathNode*		GetOpenNode( const int nX, const int nY );
	SPathNode*		GetClosedNode( const int nX, const int nY );

	SPathNode*		AllocNode();

	vector<SPath>&	GetPathOptimal( const vector<SPath>& aOrigPath );

	BOOL			MakeChild( SPathNode* pNode, const int nX, const int nY );
	BOOL			MakeChildSub( SPathNode* pNode, const int nX, const int nY, const int nTargetX, const int nTargetY );
	void			MakeDown( SPathNode* pNode );

	BOOL			IsStraight( const int nStartX, const int nStartY, const int nEndX, const int nEndY );
}; //class CPathFind


#endif // #ifndef __AStar_Header__
