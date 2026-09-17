/**
 * 내부 구현+_+
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-07-17 10:02p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /Engine/Src/Indoor.h $
 * 
 * 4     03-07-17 10:02p Kjmgo
 * 
 * 3     03-06-24 5:38p Kjmgo
 * 
 * 2     03-06-24 3:23a Kjmgo
 * <HR>
 *
 * @file	Indoor.h
 */

#ifndef __Indoor_Header__
#define __Indoor_Header__

#include "PathFind.h"
#include <string>
#include <list>
#include <map>

#define MAX_LAYER 3

using std::list;
using std::map;
using std::string;

/**
 *
 */
class CIndoor
{
public:
	UNDER_MEMORY_MGR( CIndoor );

	CIndoor();
	~CIndoor();

	void Init( IW3DWorld* pWorld, IW3DCamera* pCamera );
	BOOL Load( IW3DScene* pScene, const char* szFileName );
	BOOL Update( DWORD dwTick );
	void Draw();
	void GoOut();

	IW3DTerrain* GetTerrain()
	{
		m_pTerrain->SetLevel(0);
		return m_pTerrain;
	} //GetTerrain

	CPathFind* GetPathFind()
	{
		return m_pPathFind;
	} //GetPathFind

	BOOL IsMovable( int nX, int nY );

	BYTE*		GetMovable()
	{
		return m_pbMovable;
	} //if

	void LoadMapEntities( Archive& ar, const CLoading* pLoading );

	void Cleanup();

private:

	/**
	 *
	 */
	class CEntity
	{
	public:
		UNDER_MEMORY_MGR( CW3DWorld::CEntity );

		enum
		{
			ENTITY = 1,	 
			EFFECTOBJECT,
			NPC
		};

		int				m_nType;
		string			m_sModel;		///< 모델 이름
		IW3DModel*		m_pModel;		///< 모델
		IW3DEffectObject* m_pEffectObject; ///<

		Vector3			m_vPos;			///< 위치
		Quaternion		m_qRot;			///< 방향
	}; //class CMapObject

	typedef map<string, IW3DModel*>			ModelMap;
	typedef list<CEntity*>					EntityList;

	ModelMap			m_mapModel;
	EntityList			m_listEntity;
	EntityList			m_listAlphaEntity;
	WORD				m_wSize;
	IW3DTerrain*		m_pTerrain;
	IW3DWorld*			m_pWorld;
	IW3DCamera*			m_pCamera;

	DWORD				m_dwTick;

	BYTE*				m_pbMovable;		///< 이동 속성 버퍼
	CPathFind*			m_pPathFind;		///< 길찾기 객체

	//-=-=-=-=-=-=-=-= Load -=-=-=-=-=-=-=-=-=//
	IW3DModel*			LoadModel( char* szFileName );
	IW3DModel*			LoadNPC( char* szFileName );
}; //class CIndoor

#endif //__Indoor_Header__
