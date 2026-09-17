/**
 * Entity 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-13 11:02a $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Engine/Src/Entity.h $
 * 
 * 3     03-12-13 11:02a Kjmgo
 * 거리 제한
 * 
 * 2     03-11-12 5:42p Kjmgo
 * Entity World에서 완전 분리, Entity를 Fade In/out.
 * 
 * 1     03-11-12 3:44p Kjmgo
 * World에 있던 Entity 를 독립
 *
 * <HR>
 *
 * @file	Entity.h
 */

#ifndef __Entity_Header__
#define __Entity_Header__

// Include 

#include "QuadTree.h"

/**
 *	class CEntity
 */
class CEntity	: public IObject
{
public:
	UNDER_MEMORY_MGR( CEntity );

	/**
	 *
	 */
	enum
	{
		ENTITY = 1,	 
		EFFECTOBJECT,
		NPC
	};

	/**
	 *
	 */
	CEntity();

	/**
	 *
	 */
	~CEntity();

	/**
	 *
	 */
	BOOL Update( const DWORD dwTick, BOOL bNewMakeList );

	/**
	 *	그린다.
	 *	알파 ( 즉 뒤쪽에 그려지는 ... 일때는 False를 리턴 )
	 */
	BOOL Draw( const DWORD dwTick, IW3DCamera* pCamera, BOOL bFront, float fCameraDistance );

	/**
	 *	그린다. ( 알파 버전 )
	 */
	BOOL DrawAlpha( const DWORD dwTick );

	/**
	 *
	 */
	BOOL DrawEffect( const int nTime );

	/**
	 * 이 Object가 구역안에 들어가는가?
	 */
	BOOL IsInRect(	const float fStartX, const float fStartY, const float fSize	);


public:
	int					m_nType;			///< Entity Type
	std::string			m_sModel;			///< 모델 이름

	IW3DModel*			m_pModel;			///< 모델
	IW3DCreature*		m_pNPC;				///< NPC
	IW3DEffectObject*	m_pEffectObject;	///< Effect Object

	Vector3				m_vPos;				///< 위치
	Quaternion			m_qRot;				///< 방향
	Vector3				m_vCameraPos;		///< 카메라의 방향
	BOOL				m_bFront;			///< 반투명 처리 여부
}; //class CEntity


#endif //#ifndef __Entity_Header__