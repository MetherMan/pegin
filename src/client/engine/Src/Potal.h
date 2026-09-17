/**
 * 포탈. ( FPS에서 쓰는 포탈과는 조금 개념이 다르다. ) 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-23 4:57p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Engine/Src/Potal.h $
 * 
 * 2     03-12-23 4:57p Kjmgo
 * Potal개념을 추가.
 * 
 * 1     03-12-23 12:56a Kjmgo
 * 
 *
 * <HR>
 *
 * @file	Potal.h
 */

#ifndef __Potal_Header__
#define __Potal_Header__

#define IsPlus( f ) f >= 0 ? TRUE : FALSE

#include <list>

using std::list;

class CInArea;

/**
 *
 */
class CPotal
{
public:
	MEMORY_UNDER_MGR( CPotal );

	CPotal();
	~CPotal();

	/**
	 *	따로 영역을 지정하지 않고, 넣는대로 찍는걸로 설정한다.
	 */
	void SetViewAll();

	/**
	 *
	 */
	BOOL Load( const char* szFile );

	/**
	 *
	 */
	void Cleanup();

	/**
	 *
	 */
	BOOL CheckCreature( IW3DCreature*	pCreature );

	/**
	 *
	 */
	void Render( BOOL bShadow );

	/**
	 *
	 */
	BOOL IsInArea( IW3DCreature* pCreature );

	/**
	 *
	 */
	BOOL IsInArea( Vector3& vPos );

private:
	typedef list<IW3DCreature*>			CreatureList;

	CreatureList		m_listCreature;
	CInArea*			m_pArea;

	BOOL				m_bViewAll;	

}; //class CPotal

/**
 *
 */
class CInArea
{
public:
	CInArea();
	~CInArea();

	/**
 	 * 
	 */
	BOOL Init( const int nCount );
	

	/**
	 *
	 */
	void AddPoint( const Vector2& point );

	/**
	 *
	 */
	BOOL Check( const Vector3& point );
	BOOL Check( Vector2& point );


private:
	enum 
	{
		MAX_POINT = 10,		///< 최고 10의 꼭지점을 가진 다각형까지 처리.
	}; //enum

	BOOL*		m_abSign;		///< 부호의 배열
	int			m_nSign;		///< 몇개의 부호가 등록되는가?
	int			m_nDiffSign;		///< 몇개의 부호가 등록되는가?

	Vector2*	m_avVector;		///< 벡터의 배열
	int			m_nCount;		///< 현재 등록되어 있는 Point의 갯수
	int			m_nEnd;			///< 마지막에 등록된 Point의 수
	
	/**
	 *
	 */
	BOOL		Calculation( Vector2& vStart, Vector2& vEnd, Vector2& vCheck );

	/**
	 *
	 */
	void		AddSign( BOOL bSign )
	{
		m_abSign[m_nSign] = bSign;
		m_nSign++;
	} //AddSign

	/**
	 *
	 */
	BOOL		CheckSign( BOOL bSign )
	{
		if( m_nSign <= 0 ) return TRUE;

		for( int i = 0 ; i < m_nSign ; i++ )
		{
			if( bSign != m_abSign[i] )
			{
				return FALSE;
			} //if
		} //for

		return TRUE;
	} //CheckSign
}; //class CInArea

#endif //#ifndef __Potal_Header__