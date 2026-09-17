/**
 *  기능조물 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-08-23 8:39p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DFuncEntity.cpp $
 * 
 * 7     04-08-23 8:39p Paran
 * 
 * 6     04-06-08 3:57p Paran2
 * 
 * 5     04-05-25 3:00a Paran2
 * 
 * 4     04-05-25 12:59a Paran2
 * 
 * 3     04-03-17 10:48p Paran
 * 
 * 2     04-03-10 6:17p Paran
 * 기능조물 추가
 * 
 * <HR>
 *
 * @file	CW3DFuncEntity.h
 */

#include "Engine_Export.h"
#include <string>

/**
 * Class Funcentity
 */
class CW3DFuncEntity : public IW3DFuncEntity
{
public: //Construction & Destruction
	CW3DFuncEntity();
	~CW3DFuncEntity();

public: //Operator
	void			SetEntity( DWORD dwId, BYTE byType, IW3DModel* pModel, const Vector3& vPos );
	BOOL			LoadAnimation( const char* szPath, const char* szAniName );

	DWORD			GetId()				{ return m_dwId; }
	BYTE			GetType()			{ return m_byType; }
	DWORD			GetParam()			{ return m_dwParam; }
	Vector3*		GetPosition()		{ return &m_vPos; }

	void			SetName( const char* szName );
	void			SetLighting( const BOOL bLighting );
	
	BOOL			Pick( const Vector3& vOrg, const Vector3& vDir );
	void			SetCollisionEntity( BOOL bCollision );
	BOOL			CollisionPoint( const Vector3& vPos );

	BOOL			Update( DWORD dwTick );	
	void			DrawName();
	void			DrawShadow();

private: //Attribute
	DWORD			m_dwId;				///< Id
	BYTE			m_byType;			///< Type (FUNCMODEL_ENCAMP)
	std::string		m_strName;			///< Name
	Vector3			m_vPos;				///< Position
	IW3DModel*		m_pModel;			///< Model
	DWORD			m_dwParam;			///< Param
	BOOL			m_bCollision;		///< Whether Collision

	BOOL			m_bLighting;
	BOOL			m_bView;

	IW3DScene*		m_pScene;
	IW3DRectangle*	m_pRectangle;
}; //class CW3DFuncEntity 

IMPLEMENT_COMPONENT( CW3DFuncEntity, FuncEntity );

/** 
 * Construction
 */
CW3DFuncEntity::CW3DFuncEntity()
:	m_dwId			( 0 ),
	m_byType		( FUNCENTITY_NONE ),
	m_pModel		( NULL ),
	m_dwParam		( 0 ),
	m_vPos			( 0.0f, 0.0f, 0.0f ),
	m_bCollision	( FALSE ),
	m_strName		( "" ),
	m_pScene		( NULL ),
	m_pRectangle	( NULL )
{
} //CW3DFuncEntity::CW3DFuncEntity

/** 
 * Destruction
 */
CW3DFuncEntity::~CW3DFuncEntity()
{
	SAFE_RELEASE( m_pRectangle );
} //CW3DFuncEntity::~CW3DFuncEntity

/**
 * Setting Info
 */
void CW3DFuncEntity::SetEntity( 
	DWORD			dwId, 
	BYTE			byType, 
	IW3DModel*		pModel, 
	const Vector3&	vPos
	)
{
	Assert( pModel );

	m_pScene = MAKE_COMPONENT( Scene );
	Assert( m_pScene );

	SAFE_RELEASE( m_pRectangle );
	m_pRectangle = MAKE_COMPONENT( Rectangle );
	Assert( m_pRectangle );

	m_dwId	 = dwId;
	m_byType = byType;
	m_pModel = pModel;
	m_vPos	 = vPos;
} //CW3DFuncEntity::SetEntity

/**
 * Load Animation File
 */
BOOL CW3DFuncEntity::LoadAnimation( const char* szPath, const char* szAniName )
{
	if( !m_pModel )
	{
		Assert( !"Empty FuncEntity Model" );
		return FALSE;
	} //if

	return m_pModel->LoadAnimation( szPath, szAniName );
} //CW3DFuncEntity::LoadAnimation

/**
 * FuncEntity Pick
 */
BOOL CW3DFuncEntity::Pick( const Vector3& vOrg, const Vector3& vDir )
{
	if( !m_pModel )		return FALSE;

	return m_pModel->Pick( vOrg, vDir );
} //CW3DFuncEntity::Pick

/**
 * Set Collision Entity
 */
void CW3DFuncEntity::SetCollisionEntity( BOOL bCollision )
{
	m_bCollision = bCollision;
} //CW3DFuncEntity::SetCollisionEntity

/**
 * Check Collision
 */
BOOL CW3DFuncEntity::CollisionPoint( const Vector3& vPos )
{
	if( !m_pModel || !m_bCollision )
	{
		return FALSE;
	} //if
	return FALSE;

	//return m_pModel->CollisionPoint( m_matWorld, vPos );
} //CW3DFuncEntity::CollisionPoint

/**
 * Set Name 
 */
void CW3DFuncEntity::SetName( const char* szName )
{
	Assert( szName );
	m_strName = szName;
} //CW3DFuncEntity::SetName

/**
 * 라이트 상태 설정 
 */
void CW3DFuncEntity::SetLighting( const BOOL bLighting )
{
	m_bLighting = bLighting;
} //CW3DFuncEntity::SetLighting

/**
 * 업데이트.
 *
 * @param	dwTick		[IN] 경과 tick
 *
 * @return	계속 존재하는지 여부
 */
BOOL CW3DFuncEntity::Update( DWORD dwTick )
{
	m_bView = FALSE;

	Frustum&	frustum = m_pScene->GetFrustum();
	if( frustum.CheckSphere( m_vPos, 1.0F ) )	
	{
		m_bView = TRUE;
	} //if

	return TRUE;
} //CW3DFuncEntity::Update

/**
 * 이름 찍기.
 */
void CW3DFuncEntity::DrawName()
{
	if( m_strName.size() > 0 )
	{
		float		fHeight = 3.0f;
		static	Vector3	vTextPos;
		vTextPos = m_pScene->GetScreenPos( m_vPos + Vector3( 0.0F, 0.0F, 0.5F ) );

		int		nX = (int)vTextPos.x - m_strName.size() * 6 / 2;
		int		nY = (int)vTextPos.y;
		
		int		nTxtWidth, nTxtHeight;
		m_pScene->GetStringSize( nTxtWidth, nTxtHeight, m_strName.c_str() );

		m_pRectangle->Draw( nX-3, nY-3, nTxtWidth+6, nTxtHeight+6, 0xFFC80000 );
		m_pRectangle->Draw( nX-1, nY-1, nTxtWidth+2, nTxtHeight+2, 0xFF000000 );		
		m_pScene->DrawText( nX, nY, 0xFFD1D1D1, m_strName.c_str() );
	} //if
} //CW3DFuncEntity::DrawName

/**
 * 그림자 그리기.
 */
void CW3DFuncEntity::DrawShadow()
{
	if( !m_bView )
	{
		return;
	} //if
	
	DrawName();
} //CW3DFuncEntity::DrawShadow
