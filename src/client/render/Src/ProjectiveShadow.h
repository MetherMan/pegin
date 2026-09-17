/**
 * 프로젝션 그림자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 11:44a $
 * @version	$Revision: 27 $
 *
 * <HR>
 * $Log: /Render/Src/ProjectiveShadow.h $
 * 
 * 27    04-01-08 11:44a Kjmgo
 * 코드 정리
 * 
 *
 * @file	ProjectiveShadow.h
 */


#ifndef __ProjectiveShadow_Header__
#define __ProjectiveShadow_Header__
 

#include "Render_Export.h"
#include "Mesh.h"
#include <vector>


#define	MAXPOLYGON		100


/**
 * Projective Shadow.
 * @ingroup		Render
 */
class CProjectiveShadow
{
public:
	UNDER_MEMORY_MGR( CProjectiveShadow );

	/**
	 *
	 */
	CProjectiveShadow();

	/**
	 *
	 */
	~CProjectiveShadow();

	/**
	 *
	 */
	void	Cleanup();

	/**
	 *
	 */
	void	Restore();
	
	/**
	 *
	 */
	BOOL	CreateShadow( int nSize );	

	/**
	 *
	 */
	void	MakeShadowMap();
	
	/**
	 *
	 */
	BOOL	BeginScene();

	/**
	 *
	 */
	BOOL	EndScene();

	/**
	 *
	 */
	BOOL	BeginRenderToModel();

	/**
	 *
	 */
	BOOL	EndRenderToModel();

	/**
	 *
	 */
	void	InsertModel( IW3DModel* pModel );

	/**
	 *
	 */
	void	Update( const Matrix4& matWorld );

	/**
	 *
	 */
	BOOL	DrawToTerrain();

	/**
	 *
	 */
	BOOL	DrawToObject();

	/**
	 *
	 */
	void	SetTexture();

	/**
	 *
	 */
	void	SetLight( const Vector3& vLight );

	/**
	 *
	 */
	void	InsertVertex( const Vector3& vVertex );

	/**
	 *
	 */
	void	ShadowMatrix( const Vector3& vBlockerPosition );

	/**
	 *
	 */
	void	SetBlocker( IW3DModel* pBlocker )
	{
		m_pBlocker = pBlocker;
	} //SetBlocker

	/**
	 *
	 */
	void	SetXYMax( IW3DModel* pModel );

	/**
	 *
	 */
	Matrix4	GetLocalToWorld()
	{
		return m_matLocalToWorld;
	} //GetLocalToWorld

	/**
	 *
	 */
	void	SetShadowMap( BOOL bFlag ) 
	{
		m_bShadowMap = bFlag; 
	} //SetShadowMap

private:
	LPD3DXRENDERTOSURFACE		m_pRenderToSurface;			///< Render To Surface
		
	LPDIRECT3DTEXTURE8			m_pShadowTexture;			///< Shadow Texture
    LPDIRECT3DSURFACE8			m_pShadowSurface;			///< Shadow Surface

	BOOL						m_bShadowMap;				///< Is Use ShadowMap

	int							m_nShadowSize;				///< Shadow Size
	int							m_nTextureSize;				///< Shadow Texture Size
	int							m_nTextureLevel;			///< Texture Level
	int							m_nVertexCount;				///< 추가된 버텍스의 갯수
	int							m_nFaces;					///< Face Count
	int							m_nType;					///< Shadow Type

	Vector3						m_vLight;					///< Light Vector
	Vector3						m_vXYMax;					///< MAX XY Vector

	Vector3						m_vTerrain[MAXPOLYGON * 3];	///<

	Matrix4						m_matBlocker;				///< Blocker Matrix
	Matrix4						m_matUV;					///< Texture UV Matrix
	Matrix4						m_matWorldToLight;			///< World To Light Matrix
	Matrix4						m_matLocalToLight;			///< Local To Light Matrix
	Matrix4						m_matLocalToWorld;			///< Local To World Matrix
	Matrix4						m_matShadowProj;			///< Shadow Projection Matrix

	IW3DModel*					m_pBlocker;					///< Blocker's Model

	typedef std::vector< IW3DModel* >	ModelVector;		// Array Model
	ModelVector					m_vecModel;					///< Array Model.

	/**
	 *
	 */
	void	SetRenderState();
}; //class CProjectiveShadow


#endif // #ifndef __ProjectiveShadow_Header__
