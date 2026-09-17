/**
 * Quad Tree
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-11 7:55p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Engine/Src/QuadTree.h $
 * 
 * 9     04-01-11 7:55p Kjmgo
 * 코드정리
 * 
 * <HR>
 *
 * @file	QuadTree.h
 */

#ifndef __QuadTree_Header__
#define __QuadTree_Header__

#include <list>
#include <map>

using std::list;
using std::map;

/**
 *
 */
class IObject
{
public:
	UNDER_MEMORY_MGR( IObject );
	/**
	 * 이 Object가 구역안에 들어가는가?
	 */
	virtual BOOL IsInRect(	
		const float fStartX,	///< 시작 위치
		const float fStartY,	///< 시작 위치
		const float fSize		///< 크기
		) = 0;
}; //class CObject

typedef list<IObject*>	ObjectList;
extern ObjectList g_listObject;

/**
 *
 */
class CNode;

/**
 *
 */
class CQuadTree
{
public:
	UNDER_MEMORY_MGR( CQuadTree );

	CQuadTree();
	~CQuadTree();

	BOOL	Init();

	void	Push_Back( IObject* pObject );
	void	Push_Front( IObject* pObject );
	void	SetSize( float fSize );
	void	SetDepth( int nDepth );
	void	Separate();
	void	Clean();

	BOOL	MakeList( const Vector3& vPos );
	
private:
	float		m_fSize;		///< World Size
	int			m_nDepth;		///< Tree 단계
	int			m_nCount;	

	Vector3		m_vCamera;		///< 카메라 위치

	ObjectList	m_listObject;	///< List
	CNode*		m_pRoot;		///< Root Node
}; //class CQuadTree

/**
 *
 */
class CNode
{
public:
	UNDER_MEMORY_MGR( CNode );
	
	CNode();
	~CNode();

	void	Push_Back( IObject* pObject );
	void	Push_Front( IObject* pObject );
	void	Separate( float fStartX, float fStartY, float fSize, int nDepth );
	void	MakeList( const Vector3& vPos );
	void	Clean();

	BOOL	IsInRect( float x, float y );
	

	int		GetCount( float x, float y );
	int		GetSize();
	
private:
	float	m_fStartX;			///< 영역 
	float	m_fStartY;			///<
	float	m_fEndX;			///<
	float	m_fEndY;			///<
	float	m_fSize;			///< 크기
	int		m_nDepth;			///<

	class CNode* m_pChild[4];		///<
	ObjectList	m_listObject;		///< List
}; //class CNode

#endif //__QuadTree_Header__