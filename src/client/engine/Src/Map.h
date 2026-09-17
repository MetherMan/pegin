/**
 * Map 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-23 4:57p $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Engine/Src/Map.h $
 * 
 * 5     03-12-23 4:57p Kjmgo
 * Potal개념을 추가.
 * 
 * 4     03-12-11 4:08a Kjmgo
 * Map Load시 버그 수정
 * 
 * 3     03-12-09 2:04a Kjmgo
 * World에서 Entity분리및, Map분리, 그리고 Map내부에, Dynamic NPC 추가
 * 
 * 2     03-12-08 10:10p Kjmgo
 * World에서 Entity관리 관련을 CMap으로 분리 
 * 
 * 1     03-12-08 8:47p Kjmgo
 * 
 *
 * <HR>
 *
 * @file	Map.h
 */

#ifndef __Map_Header__
#define __Map_Header__

#include "Entity.h"
#include "QuadTree.h"
#include "DynamicNPC.h"
#include "Potal.h"

#include <map>
#include <string>
#include <list>

using std::map;
using std::string;
using std::list;


/**
 *
 */
class CMap
{
public:
	MEMORY_UNDER_MGR( CMap );

	/**
	 *
	 */
	CMap();

	/**
	 *
	 */
	~CMap();

	/**
	 *
	 */
	BOOL Init( IW3DWorld* pWorld );

	/**
	 *
	 */
	BOOL Load( const char* szFile );

	/**
	 *
	 */
	void CleanPotal();

	/**
	 *
	 */
	BOOL AddEntity( CEntity* pEntity, BOOL bBack = TRUE );

	/**
	 *
	 */
	void AddCreature( IW3DCreature* pCreature );

	/**
	 *
	 */
	void CleanEntity();

	/**
	 *
	 */
	void CleanupModel();

	/**
	 *
	 */
	void CleanupPotal();

	/**
	 *
	 */
	void UpdateEntities( const DWORD dwTick );

	/**
	 *
	 */
	void DrawEntities( const DWORD dwTime, BOOL bFront );

	/**
	 *
	 */
	void DrawCreatures( Vector3& vPos, BOOL bShadow );

	/**
	 *
	 */
	CQuadTree*	GetQuadTree()
	{
		return m_pQuadTree;
	} //GetQuadTree

	/**
	 *
	 */
	void SetTime( int nTime )
	{
		m_nTime = nTime;
	} //SetTime


private:
	typedef list<CEntity*>					EntityList;
	typedef list<CPotal*>					PotalList;


	IW3DWorld*			m_pWorld;			///< World
	IW3DCamera*			m_pCamera;

	DWORD				m_dwTime;
	int					m_nTime;

//	ModelMap			m_mapModel;

	EntityList			m_listEntity;		///< 엔티티 리스트
	EntityList			m_listAlphaEntity;	///< 엔티티 리스트
	EntityList			m_listEffectEntity;	///< 
	
	CQuadTree*			m_pQuadTree;

	CDynamicNPCMgr*		m_pNPCMgr;
	PotalList			m_listPotal;			///< 포탈 배열
	CPotal*				m_pPotal;				///< 나머지것들을 남을 포탈

	/**
	 *
	 */
	void DrawEntitiesAlpha( const BOOL bFront );

	/**
	 *
	 */
	void DrawEntitiesEffect( const BOOL bFront );

}; //class CMap

#endif //#ifndef __Map_Header__