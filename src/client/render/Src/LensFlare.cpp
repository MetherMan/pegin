/**
 * ·»Áî ÇÃ·¹¾î
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 04-03-25 5:20p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Render/Src/LensFlare.cpp $
 * 
 * 2     04-03-25 5:20p Kjmgo
 * 
 * 1     04-03-25 4:33p Kjmgo
 * 
 * <HR>
 *
 * @file	LensFlare.cpp
 */

// Include
	#include "Render_Export.h"
	#include "Texture.h"
	#include "VertexBuffer.h"
	#include "IndexBuffer.h"
	#include "LensFlare.h"
//


static WORD LensId[MAX_HALO] = 
{
	1,6,3,4,5,6,3,4,6,3,1,4
}; //WORD LensId

//---------------------------
//	Constructor
//---------------------------
CLensFlare::CLensFlare()
:	m_pLightTex			( NULL ),
	m_vDir				( 0.0F, 0.0F, 1.0F ),
	m_vPos				( 0.0F, 0.0F, 0.0F ),
	m_fSize				( 0 ),
	m_nLightPosX		( 0 ),
	m_nLightPosY		( 0 ),
	m_nCenterX			( 0 ),
	m_nCenterY			( 0 ),
	m_dwColor			( 0xFFFFFFFF ),
	m_pVB				( NULL )
{
	for( int i = 0 ; i < MAX_HALO ; i++ )
	{
		m_apHalo[i] = NULL;
	} //for
} //CLensFlare::CLensFlare

//---------------------------
//	Destructor
//---------------------------
CLensFlare::~CLensFlare()
{
	SAFE_DELETE( m_pVB );
	SAFE_RELEASE( m_pLightTex );

	for( int i = 0 ; i < MAX_HALO ; i++ )
	{
		SAFE_RELEASE( m_apHalo[i] );
	} //for
} //CLensFlare::~CLensFlare

//---------------------------
// Init
//---------------------------
void CLensFlare::Init( const Vector3& vPos )
{
	m_vPos	= vPos;
	m_fSize = 20.0F;
	m_dwColor = ( 255 << 24 ) | ( 251 << 16 ) | ( 54 << 8 ) | 57;
	//N3D_RGB( 255, 251, 54, 57 );

	m_pScene = MAKE_COMPONENT( Scene );
	if( !m_pScene )
		return ;

	// Get Screen Pos
	m_pScene->GetViewport( m_nCenterX, m_nCenterY );

	m_nCenterX = m_nCenterX / 2;
	m_nCenterY = m_nCenterY / 2;
	
	m_vCenter = Vector2( (float)m_nCenterX, (float)m_nCenterY );
	// Create Light
	CreateLight();

	// Create Halos
	CreateHalo();

} //CLensFlare::Init

//---------------------------
// Create Light
//---------------------------
BOOL CLensFlare::CreateLight()
{
	// Load Texture
		m_pLightTex = g_pTextureMgr->LoadTexture( "Texture/LensFlare/", "flare02.bmp", CTextureMgr::SPRITE );
		Assert( m_pLightTex );
	// ~Load Texture

	// Allocation Vertex Buffer
		m_pVB = new CVertexBuffer( Vertex::FVF, sizeof( Vertex ) );
		CHECK_PTR_RETURN( m_pVB );

		if( !m_pVB->Create( 4 ) )
		{
			Assert( !"Create Vertex Buffer Fail." );
			return FALSE;
		} //if
	// ~Allocation Vertex Buffer

	// Vertex Setting
	Vertex* apVertices = (Vertex*) m_pVB->Lock();

	// 0
		apVertices[0].p		= Vector3( -m_fSize / 2.0F, m_fSize / 2.0F, 0.0F );
		apVertices[0].color	= m_dwColor;
		apVertices[0].tu	= 0.0F;
		apVertices[0].tv	= 0.0F;

	// 1
		apVertices[1].p		= Vector3( m_fSize / 2.0F, m_fSize / 2.0F, 0.0F );
		apVertices[1].color	= m_dwColor;
		apVertices[1].tu	= 1.0F;
		apVertices[1].tv	= 0.0F;
	// 2
		apVertices[2].p		= Vector3( -m_fSize / 2.0F, -m_fSize / 2.0F, 0.0F );
		apVertices[2].color	= m_dwColor;
		apVertices[2].tu	= 0.0F;
		apVertices[2].tv	= 1.0F;
	// 3
		apVertices[3].p		= Vector3( m_fSize / 2.0F, -m_fSize / 2.0F, 0.0F );
		apVertices[3].color	= m_dwColor;
		apVertices[3].tu	= 1.0F;
		apVertices[3].tv	= 1.0F;

	m_pVB->Unlock();
	// ~Vertex Setting

	return TRUE;
} //CLensFlare::CreateLight

//---------------------------
// Create Halo
//---------------------------
BOOL CLensFlare::CreateHalo()
{
	char szFile[128];

	for( int i = 1 ; i <= MAX_HALO ; i++ )
	{
		sprintf( szFile, "Texture/LensFlare/flare0%d.bmp", LensId[i-1] );

		m_apHalo[i-1] = MAKE_COMPONENT( Sprite );

		if( !m_apHalo[i-1]->Load( szFile, 0, 0, TRUE ) )
		{
			Assert( !"Load Halo Fail." );
			//return FALSE;
		} //if
	} //for


	return TRUE;
} //CLensFlare::CreateHalo

//---------------------------
// Set Light Position
//---------------------------
void CLensFlare::SetPosition( const Vector3& vPos )
{
	m_vPos = vPos;
} //CLensFlare::SetPosition

//---------------------------
//	Set Light Texture
//---------------------------
BOOL CLensFlare::SetLightTexture( const char* szTexture )
{
	Assert( szTexture );

	return TRUE;
} //ClensFlare::SetLightTexture	

//---------------------------
//	Draw
//---------------------------
void CLensFlare::Draw( const DWORD dwTick )
{
	// Get Screen Position

	Frustum& frustum = m_pScene->GetFrustum();

	if( !frustum.CheckSphere( m_vPos, 35.0F ) )
	{
		return ; 
	} //if

	Vector3 vScreenPos = m_pScene->GetScreenPos( m_vPos );
	m_vLightPos.x = vScreenPos.x;
	m_vLightPos.y = vScreenPos.y;

	Vector2 vDir = ( m_vLightPos - m_vCenter ).Normal() * -1.0F;
	float fDist = ( m_vLightPos - m_vCenter ).Length();
	int nDist = (int) fDist / MAX_HALO;

	int nX = (int) m_vLightPos.x + ( vDir.x * nDist * -2.0F );
	int nY = (int) m_vLightPos.y + ( vDir.y * nDist * -2.0F );

	if( m_apHalo[0] )
	{
		m_apHalo[0]->DrawEffect( nX, nY );	
		m_apHalo[0]->SetZoom( 1.0F + ( nDist / 300.0F ) );
	} //if

	for( int i = 2 ; i <= MAX_HALO ; i++ )
	{
		nX = (int) m_vLightPos.x + ( vDir.x * nDist * ( ( i - 1 ) * 2.5F ) );
		nY = (int) m_vLightPos.y + ( vDir.y * nDist * ( ( i - 1 ) * 2.5F ) );

		if( m_apHalo[i-1] )
		{
			m_apHalo[i-1]->SetZoom( MAX( 1.5F - ( ( nDist * 7 ) / 300.0F ), 0.0F ) );
			m_apHalo[i-1]->DrawEffect( nX, nY );		
		} //if
	} //for

//	m_apHalo[0]->Draw( (int)m_vLightPos.x + ( vDir.x * nDist ), (int)m_vLightPos.y + ( vDir.y * nDist ) );
	//m_apHalo[0]->Draw( (int)m_vLightPos.x, (int)m_vLightPos.y );
	
	DrawLight();
} //CLensFlare::Draw

//---------------------------
//	Draw Light
//---------------------------
void CLensFlare::DrawLight()
{
	// Alpha Blend
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ONE );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_ONE );


	Frustum& frustum = m_pScene->GetFrustum();

	Matrix4	matWorld = frustum.GetBillboard();

	matWorld._41 = m_vPos.x;
	matWorld._42 = m_vPos.y;
	matWorld._43 = m_vPos.z;

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	m_pLightTex->SetTexture( 0 );

	m_pVB->Draw( D3DPT_TRIANGLESTRIP, 2 );
} //CLensFlare::DrawLight
