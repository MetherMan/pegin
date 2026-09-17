/**
 * 평면.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-11-13 5:46p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /Render/Src/W3DPlane.cpp $
 * 
 * 7     03-11-13 5:46p Paran
 * 
 * 6     03-06-13 9:16p Kjmgo
 * 
 * 5     03-06-12 7:48p Kjmgo
 * 
 * 4     03-06-12 5:35p Kjmgo
 * 
 * 3     03-06-12 5:26p Kjmgo
 * 
 * 2     03-06-12 5:25p Kjmgo
 * Plane 추가
 * <HR>
 *
 * @file	IW3DPlane.cpp
 */

#include "Render_Export.h"
#include "VertexBuffer.h"
#include "Texture.h"

class CW3DPlane : public IW3DPlane
{
public:
	UNDER_MEMORY_MGR( CW3DPlane );

	CW3DPlane();
	~CW3DPlane();

	BOOL Create( const float fWidth, const float fHeight, const DWORD dwColor );

	void SetWorld( const Vector3& vPos );
	void SetWorld( const Vector3& vPos, const Quaternion& qRot );
	void SetWorld( const Matrix4& matWorld );

	void SetTexture( const char* szTexutre );

	Vector3 GetPosition();
	BOOL IsVisible();
	void Draw();


private:
	struct VERTEX
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		float	x, y, z;
		DWORD	color;
		float	tu, tv;
	}; //struct VERTEX

	float				m_fWidth;		///< 가로 길이.
	float				m_fHeight;		///< 세로 길이.
	DWORD				m_dwColor;		///< 
	CTexture*			m_pTexture;		///< 텍스처.
	CVertexBuffer*		m_pVB;			///< 버텍스 버퍼.
	Matrix4				m_matWorld;		///< 

	void	CreateVertices( VERTEX* pVertices );

}; //class CW3DPlane

IMPLEMENT_COMPONENT( CW3DPlane, Plane );


/**
 *
 */
CW3DPlane::CW3DPlane() 
:		m_fWidth( 0.0F ),
		m_fHeight( 0.0F ),
		m_pTexture( NULL ),
		m_pVB( NULL )
{

} //CW3DPlane::CW3DPlane

/**
 *
 */
CW3DPlane::~CW3DPlane()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_DELETE( m_pVB );
} //CW3DPlane::~CW3DPlane

/**
 *
 */
BOOL CW3DPlane::Create( const float fWidth, const float fHeight, const DWORD dwColor )
{
	m_fWidth	= fWidth;
	m_fHeight	= fHeight;
	m_dwColor	= dwColor;

	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );
	CHECK_PTR_RETURN( m_pVB );

	if( !m_pVB->Create( 4 ) )
	{
		SAFE_DELETE( m_pVB );
		return FALSE;
	} //if

	VERTEX*		pVertices = (VERTEX*)m_pVB->Lock();
	if( !pVertices )
	{
		Assert( !"버텍스 버퍼 락 실패" );
		return FALSE;
	} //if

	CreateVertices( pVertices );

	m_pVB->Unlock();

	return TRUE;
} //CW3DPlane::Create

/**
 *
 */
void CW3DPlane::CreateVertices( VERTEX* pVertices )
{
	Assert( pVertices );

	pVertices[0].x		= -m_fWidth / 2;
	pVertices[0].y		= 0.0F;
	pVertices[0].z		= m_fHeight / 2;
	pVertices[0].color	= m_dwColor;
	pVertices[0].tu		= 0.0F;
	pVertices[0].tv		= 0.0F;

	pVertices[1].x		= m_fWidth / 2;
	pVertices[1].y		= 0.0F;
	pVertices[1].z		= m_fHeight / 2;
	pVertices[1].color	= m_dwColor;
	pVertices[1].tu		= 1.0F;
	pVertices[1].tv		= 0.0F;

	pVertices[2].x		= -m_fWidth / 2;
	pVertices[2].y		= 0.0F;
	pVertices[2].z		= -m_fHeight / 2;
	pVertices[2].color	= m_dwColor;
	pVertices[2].tu		= 0.0F;
	pVertices[2].tv		= 1.0F;

	pVertices[3].x		= m_fWidth / 2;
	pVertices[3].y		= 0.0F;
	pVertices[3].z		= -m_fHeight / 2;
	pVertices[3].color	= m_dwColor;
	pVertices[3].tu		= 1.0F;
	pVertices[3].tv		= 1.0F;
} //CW3DPlane::CreateVertices

/**
 *
 */
BOOL CW3DPlane::IsVisible()
{	
	Vector3 vPos = m_matWorld.GetPosition();

	if( g_Frustum.CheckSphere( vPos, 1.0F ) )	
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	} //if..else..
} //CW3DPlane::IsVisible

/**
 *
 */
void CW3DPlane::SetWorld( const Vector3& vPos )
{
	m_matWorld.SetWorld( vPos );
} //CW3DPlane::SetWorld

/**
 *
 */
void CW3DPlane::SetWorld( const Vector3& vPos, const Quaternion& qRot )
{
	m_matWorld.SetWorld( vPos, qRot );
} //CW3DPlane::SetWorld

/**
 *
 */
void CW3DPlane::SetWorld( const Matrix4& matWorld )
{
	m_matWorld = matWorld;
} //CW3DPlane::SetWorld

/**
 *
 */
void CW3DPlane::SetTexture( const char* szTexutre )
{
	if( m_pTexture )
	{
		SAFE_RELEASE( m_pTexture );
	} //if

	m_pTexture = g_pTextureMgr->LoadTexture( "", szTexutre );
	Assert( m_pTexture );
} //CW3DPlane::SetTexture

/**
 *
 */
Vector3 CW3DPlane::GetPosition()
{
	return m_matWorld.GetPosition();
} //CW3DPlane::GetPosition
  
/**
 *
 */
void CW3DPlane::Draw()
{
//	Assert( m_pTexture );
	Assert( m_pVB );

	D3D::AlphaBlend( FALSE );

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&m_matWorld );

	if( m_pTexture )
	{
		m_pTexture->SetTexture( 0 );
	}
	else
	{
		g_pd3dDevice->SetTexture( 0, NULL );
	} //if

	m_pVB->Draw( D3DPT_TRIANGLESTRIP, 2 );
} //CW3DPlane::Draw
