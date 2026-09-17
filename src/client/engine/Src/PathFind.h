/**
 * 길 찾기.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-10 8:33p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Engine/Src/PathFind.h $
 * 
 * 9     03-12-10 8:33p Kjmgo
 * 길찾기 수정, Optimal을 선택적으로 구하도록 수정
 * 
 * 8     03-04-26 11:39a Ahastudio
 * IsVisit(), SetVisit() 추가.
 * <HR>
 *
 * @file	PathFind.h
 */


#ifndef __PathFind_Header__
#define __PathFind_Header__


#include "AStar.h"
#include <vector>


using std::vector;


/**
 * 길 찾기 클래스.
 * @ingroup		Engine
 */
class CPathFind : public CAStar
{
public:
	UNDER_MEMORY_MGR( CPathFind );

	CPathFind( const DWORD dwSize );
	~CPathFind();

	void			SetMovable( BYTE* pbtMovable );

	vector<Vector3>	Find( const Vector3& vStart, const Vector3& vTarget, BOOL bOptimal = FALSE );

private:
	DWORD			m_dwSize;			///< 맵 크기
	BYTE*			m_pbtMovable;		///< 이동 속성 맵
	BYTE*			m_pbtVisit;			///< 검색 여부 맵

	BOOL			IsMove( const int nX, const int nY );
	BOOL			IsVisit( const int nX, const int nY );

	void			SetVisit( const int nX, const int nY );
}; //class CPathFind


#endif // #ifndef __PathFind_Header__
