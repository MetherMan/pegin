/**
 * 스카이 박스
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-05-25 2:57p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /Render/Src/Sky.cpp $
 * 
 * 4     04-05-25 2:57p Paran2
 * 
 * 3     04-03-25 5:20p Kjmgo
 * 
 * 2     04-03-25 4:33p Kjmgo
 * 
 * 1     04-03-25 3:49p Kjmgo
 * 
 * 1     04-03-25 3:25p Kjmgo
 * <HR>
 *
 * @file	Sky.cpp
 */

// Include
	#include "Render_Export.h"
	#include "Texture.h"
	#include "VertexBuffer.h"
	#include "IndexBuffer.h"
	#include "LensFlare.h"
//

/**
 *
 */
class CW3DSky : public IW3DSky
{
public:
	UNDER_MEMORY_MGR( CW3DSky );

	// Constructor And Destructor
		CW3DSky();
		~CW3DSky();
	// ~Constructor And Destructor

	// Init
	BOOL Init( const char* szCloudTex );

	// Render
	void Render( const DWORD dwTick, const Matrix4& matView );

	// Render
	void RenderLens( const DWORD dwTick );

private:
	// Private Functions

	// Create Cloud
	BOOL CreateCloud();

	// Draw Cloud
	void DrawCloud( int nX, int nY, float fHeight );

private:

	CVertexBuffer*	m_pVB;		// Vertex Buffer
	CIndexBuffer*	m_pIB;		// Index Buffer

	CTexture*	m_pCloudTex;	// Cloud Texture

	DWORD		m_dwWidth;		// 크기
	DWORD		m_dwTicks;		// 내부 누적 틱

	Vector2		m_vCloud[3];			// Cloud Vector
	Vector2		m_vCloudTexCoord[3];	// Texture Coord

	Matrix4		m_matView;		// View Matrix

	CLensFlare*		m_pLensFlare;	// Sun
}; //class CW3DSky

IMPLEMENT_COMPONENT( CW3DSky, Sky );

/**
 *
 */
CW3DSky::CW3DSky()
:	m_dwWidth		( 0 ),
	m_dwTicks		( 0 ),
	m_pVB			( NULL ),
	m_pIB			( NULL ),
	m_pLensFlare	( NULL ),
	m_pCloudTex		( NULL )
{
	// Vector Initialize
	m_vCloud[0] = Vector2( 1.0F, 0.2F );
	m_vCloud[1] = Vector2( 1.0F, 0.5F );
	m_vCloud[2]	= Vector2( 1.0F, 0.8F );

	for( int i = 0 ; i < 3 ; i++ )
	{	// Normalize
		m_vCloud[i].Normalize();
		m_vCloud[i] /= 30.0F;

		// Initialize Texture Coord
		m_vCloudTexCoord[i] = Vector2( 0.0F, 0.0F );
	} //for	
} //CW3DSky::CW3DSky

/**
 *
 */
CW3DSky::~CW3DSky()
{
	SAFE_RELEASE( m_pCloudTex );
	SAFE_DELETE( m_pLensFlare );
	SAFE_DELETE( m_pVB );
	SAFE_DELETE( m_pIB );
} //CW3DSky::CW3DSky

/**
 * Init
 */
BOOL CW3DSky::Init( const char* szCloudTex )
{
	Assert( szCloudTex );
	Assert( !m_pVB );

	m_dwWidth = 700;

	// Create Cloud
	CreateCloud();

	// Texture Create
	m_pCloudTex = g_pTextureMgr->LoadTexture( "Texture/Sky/", szCloudTex );

	m_pLensFlare = new CLensFlare;
	m_pLensFlare->Init( Vector3( 100.0F, 100.0F, 18.0F ) );
	
	return TRUE;
} //CW3DSky::Init

/**
 *
 */
void CW3DSky::RenderLens( const DWORD dwTick )
{
	if( m_pLensFlare ) 
	{
		m_pLensFlare->Draw( dwTick );
	} //if
} //CW3DSky::RenderLens

/**
 * Render
 */
void CW3DSky::Render( const DWORD dwTick, const Matrix4& matView )
{
	m_dwTicks += dwTick;
	m_matView = matView;	

	for( int k = 0 ; k < 8 ; k++ )
	{
		g_pd3dDevice->SetTexture( k, NULL );
	} //for

	// Set Render State
	// Z Buffer Off
		g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,        FALSE );	
		g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE ,  FALSE );

		g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	// ~Set Render State

	for( int i = 2 ; i >= 0 ; i-- )
	{
		m_vCloudTexCoord[i] += m_vCloud[i] * ( 0.1F ) ;//( dwTick / 100.0F );

		DrawCloud(	m_vCloudTexCoord[i].x,
					m_vCloudTexCoord[i].y,
					(float) i 
					);
	} //for

	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,        TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE ,  TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW ); 
} //CW3DSky::Render

/**
 *
 */
BOOL CW3DSky::CreateCloud()
{
	// Allocation Vertex Buffer
	m_pVB = new CVertexBuffer( D3DFVF_XYZ, sizeof( Vector3 ) );
	CHECK_PTR_RETURN( m_pVB );

	if( !m_pVB->Create( 16 * 16 ) )
	{
		SAFE_DELETE( m_pVB );
		return FALSE;
	} //if
	// ~Allocation Vertex Buffer

	//////////////////
	float fCircle = (float)m_dwWidth;
	float fValueZ = 0.0F;
	float vi = 0.0F;

	Vector3* pVertices = (Vector3*) m_pVB->Lock();

		for( int i = 0 ; i < 16 ; i++ )
		{
			for( int j = 0 ; j < 16 ; j++ )
			{
				pVertices[i*16+j].x = (float)( j - 8 ) / 8.0F * m_dwWidth * 2.0F;
				pVertices[i*16+j].y = (float)( i - 8 ) / 8.0F * m_dwWidth * 2.0F;

				pVertices[i*16+j].z = 0.0F;  //fValueZ;
			} //for
		} //for	

	m_pVB->Unlock();

	// Allocation Index Buffer
	m_pIB = new CIndexBuffer;
	CHECK_PTR_RETURN ( m_pIB );

	if( !m_pIB->Create( 15 * 15 * 6 ) )
	{
		return FALSE;
	} //if
	// ~Allocation Index Buffer

	// Lock
	WORD* pIndices = m_pIB->Lock();

	int nIdx = 0;	
	for( i = 0 ; i < 15 ; i++ )
	{
		for( int j = 0 ; j < 15 ; j++ )
		{
			pIndices[nIdx] = i			* 16 + j;			nIdx++;
			pIndices[nIdx] = ( i + 1 )	* 16 + j;			nIdx++;
			pIndices[nIdx] = i			* 16 + ( j + 1 );	nIdx++;
			
			pIndices[nIdx] = ( i + 1 )	* 16 + j;			nIdx++;
			pIndices[nIdx] = i			* 16 + ( j + 1 );	nIdx++;
			pIndices[nIdx] = ( i + 1 )	* 16 + ( j + 1 );	nIdx++;			
		} //for
	} //for

	m_pIB->Unlock();

	return TRUE;
} //CW3DSky::CreateCloud

/**
 *
 */
void CW3DSky::DrawCloud( int nX, int nY, float fHeight )
{
	if( !m_pCloudTex )
	{
		return;
	} //if

//	Matrix4 matView		= Direct3D::g_pDirect3D->GetCamera()->GetViewMatrix();
	Matrix4 matInverse	= m_matView.Inverse();
	Matrix4 matScale;
	Matrix4 matTex;

	matScale.SetIdentity();

	if( fHeight == 1.0F )
	{
		matScale._11 = 1.0F / ( 100 * fHeight );
		matScale._22 = 1.0F / ( 200 * fHeight );
		matScale._31 = 0.022F * ( m_dwTicks ) / 40000;
		matScale._32 = 0.0F;
	} 
	else if( fHeight == 2.0F )
	{
		matScale._11 = 1.0F / ( 400 * fHeight );
		matScale._22 = 1.0F / ( 400 * fHeight );
		matScale._31 = 0.022F * m_dwTicks / 35000;
		matScale._32 = 0.011F * m_dwTicks / 35000;
	} 
	else
	{
		matScale._11 = 1.0F / ( 400 * fHeight );
		matScale._22 = 1.0F / ( 400 * fHeight );
		matScale._31 = 0.0F * m_dwTicks / 55000;
		matScale._32 = 0.022F * m_dwTicks / 55000;
	} //if..else..

	matTex = matInverse * matScale;

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	g_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DXMATRIX*)&matTex ); 

	m_pCloudTex->SetTexture( 0 );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER,	D3DTEXF_ANISOTROPIC );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER,	D3DTEXF_ANISOTROPIC );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER,	D3DTEXF_ANISOTROPIC );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR   );
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE );

	Matrix4 matWorld;
	matWorld.SetIdentity();
	matWorld.Translate( Vector3( 0.0F, 0.0F, 20.0F + ( fHeight * 30.0F ) ) );

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	m_pIB->SetIndex();
	m_pVB->DrawIndexed( D3DPT_TRIANGLELIST,  15 * 15 * 2 );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,	FALSE );
} //CW3DSky::DrawCloud