/**
 * 수면 효과 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-11-13 5:46p $
 * @version	$Revision: 26 $
 *
 * <HR>
 * $Log: /Render/Src/W3DWater.cpp $
 * 
 * 26    03-11-13 5:46p Paran
 * 
 * 25    03-10-27 3:37p Kjmgo
 * 많이 쓰이는 변수들을 Static으로 선언
 * 
 * 24    03-09-18 10:58a Kjmgo
 * 
 * 23    03-08-08 3:23p Admin
 * 
 * 22    03-08-08 2:20p Admin
 * 
 * 21    03-07-30 2:08p Kjmgo
 * 
 * 20    03-05-03 11:53a Ahastudio
 *
 * 19    03-03-14 8:53p Ahastudio		물 크기 및 텍스처 필터링 수정.
 *
 * 18    03-03-14 3:35a Ahastudio		물 텍스처 반복 크기 설정 및 흐르는 속도 조절.
 * <HR>
 *
 * @file	W3DWater.cpp
 */


#include "Render_Export.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include <string>


using std::string;


/**
 * 수면 효과 클래스.
 * @ingroup		Render
 */
class CW3DWater : public IW3DWater
{
public:
	UNDER_MEMORY_MGR( CW3DWater );

	CW3DWater();
	~CW3DWater();

	BOOL	Create( char* szTextureName, int nCount, int nSize, float fHeight );
	void	Render( DWORD dwTick );

	/**
	 * 수면 높이를 얻어온다.
	 */
	float	GetHeight()
	{
		return m_fHeight;
	} //GetHeight

	/**
	 *
	 */
	void	SetPosition( const Vector3& vPos )
	{
		m_vPos.x = vPos.x;
		m_vPos.y = vPos.y;
	//	m_vPos.x = (int)( vPos.x / m_wSize ) * m_wSize;
	//	m_vPos.y = (int)( vPos.y / m_wSize ) * m_wSize;
	} //SetPosition

private:
	struct VERTEX
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_TEX2 };
		Vector3		p;
		float		tu1, tv1;
		float		tu2, tv2;
	}; //struct VERTEX

	struct VERTEXFORONE
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		Vector3		p;
		float		tu1, tv1;
	}; //struct VERTEX

	CVertexBuffer*		m_pVB;				///< 버텍스 버퍼.
	CVertexBuffer*		m_pVBForCloud;				///< 버텍스 버퍼.
	CTexture*			m_pTexture;			///< 텍스처.
	CTexture*			m_pTextureForCloud;			///< 텍스처.
	DWORD				m_dwSize;			///< 수면 크기.
	WORD				m_wCount;
	WORD				m_wSize;
	float				m_fHeight;			///< 수면 높이.
	DWORD				m_dwTick;			///< 누적 시간.

	Vector3				m_vPos;				///< 물의 위치 

	void	CreatePlan();
	void	SetRenderState();
	void	SetTextureTransform( int nStage, const Matrix4& matTransform );
}; //class CW3DWater


IMPLEMENT_COMPONENT( CW3DWater, Water );


/**
 * 생성자.
 */
CW3DWater::CW3DWater()
{
	m_pVB			= NULL;
	m_pVBForCloud	= NULL;
	m_pTexture		= NULL;
	m_pTextureForCloud = NULL;
	m_dwTick	= 0;
	m_vPos		= Vector3( 0.0F, 0.0F, 0.0F );
} //CW3DWater::CW3DWater

/**
 * 소멸자.
 */
CW3DWater::~CW3DWater()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pTextureForCloud );
	SAFE_DELETE( m_pVBForCloud );
	SAFE_DELETE( m_pVB );
} //CW3DWater::~CW3DWater

/**
 * 수면을 만든다.
 */
BOOL CW3DWater::Create( char* szTextureName, int nCount, int nSize, float fHeight )
{
	SAFE_DELETE( m_pVB );
	SAFE_RELEASE( m_pTexture );
	fHeight		-= 0.01F;
	m_dwSize	= nSize * nCount;
	m_fHeight	= fHeight;
	m_wCount	= nCount;
	m_wSize		= nSize;
	m_vPos.z	= fHeight;

	// 텍스처 로딩

	m_pTexture = g_pTextureMgr->LoadTexture( "Texture/Terrain/", szTextureName );

	if( !m_pTexture )
	{
		Assert( !"텍스처 로딩 실패" );
		return FALSE;
	} //if

	// VertexBuffer 생성

	CreatePlan();

	m_pVBForCloud = new CVertexBuffer( VERTEXFORONE::FVF, sizeof( VERTEXFORONE ) );
	if( !m_pVBForCloud ||
		!m_pVBForCloud->Create( 4 ) )
	{
		Assert( !"버텍스 버퍼 생성 실패" );
		SAFE_DELETE( m_pVBForCloud );
		return FALSE;
	} //if

	// Vertex 구성

	VERTEXFORONE*		pVertices = (VERTEXFORONE*)m_pVBForCloud->Lock();

	float fSize = m_dwSize / 2;

	pVertices[0].p   = Vector3( -fSize, -fSize, m_fHeight );
	pVertices[0].tu1 = 0.0F;
	pVertices[0].tv1 = 0.0F;

	pVertices[1].p   = Vector3( fSize, -fSize, m_fHeight );
	pVertices[1].tu1 = 0.8F;
	pVertices[1].tv1 = 0.0F;

	pVertices[2].p   = Vector3( -fSize, fSize, m_fHeight );
	pVertices[2].tu1 = 0.0F;
	pVertices[2].tv1 = 0.8F;

	pVertices[3].p   = Vector3( fSize, fSize, m_fHeight );
	pVertices[3].tu1 = 0.8F;
	pVertices[3].tv1 = 0.8F;

	m_pVBForCloud->Unlock();

	return TRUE;
} //CW3DWater::Create

/**
 *
 */
void CW3DWater::CreatePlan()
{
	// VertexBuffer 생성

	int nCount = m_wCount;
	int nSize = m_wSize;
	int nHalf = nCount / 2;
	float fTextureAdd = 1.0F / (float)nCount;
	
	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );

	if( !m_pVB ||
		!m_pVB->Create( nCount * nCount * 2 * 3 ) )
	{
		Assert( !"버텍스 버퍼 생성 실패" );
		SAFE_DELETE( m_pVB );
		return ;
	} //if

	// Vertex 구성

	int nBlock = 0;

	VERTEX*		pVertices = (VERTEX*)m_pVB->Lock();

	for( int i = -nHalf ; i < nHalf; i++ )
	{
		for( int j = -nHalf ; j < nHalf; j++ )
		{
			VERTEX	vertices[4];

			int nCount = 0;

			for( int y = 0 ; y < 2 ; y++ )
			{
				for( int x = 0 ; x < 2 ; x++ )
				{
					vertices[nCount].p.x	= (float)( x + j ) * nSize;
					vertices[nCount].p.y	= (float)( y + i ) * nSize;
					vertices[nCount].p.z	= m_fHeight;
					vertices[nCount].tu1	= (float)( x + ( j + nHalf ) ) * fTextureAdd * 25;
					vertices[nCount].tv1	= (float)( y + ( i + nHalf ) ) * fTextureAdd * 25; 
					vertices[nCount].tu2	= (float)( y + ( j + nHalf ) ) * fTextureAdd * 27;
					vertices[nCount].tv2	= (float)( x + ( i + nHalf ) ) * fTextureAdd * 27;

					nCount++;
				} //for
			} //for
			Assert( nBlock >= 0 && nBlock <= m_wCount * m_wCount * 2 * 3 );
			pVertices[nBlock    ] = vertices[0];
			pVertices[nBlock + 1] = vertices[1];
			pVertices[nBlock + 2] = vertices[2];

			pVertices[nBlock + 3] = vertices[2];
			pVertices[nBlock + 4] = vertices[1];
			pVertices[nBlock + 5] = vertices[3];

			nBlock += 6;
		
		} //for
	} //for

	m_pVB->Unlock();	
} //CW3DWater::CreatePlan

/**
 * 수면 효과 그리기
 *
 * @param	dwTick		[IN] 경과 tick
 */
void CW3DWater::Render( DWORD dwTick )
{
	Assert( g_pd3dDevice );
	Assert( m_pTexture );

	SetRenderState();

	Matrix4		matWorld;
	matWorld.SetIdentity();
	matWorld.Translate( m_vPos );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	m_pTexture->SetTexture( 0 );
	m_pTexture->SetTexture( 1 );

	D3D::FogEnable( TRUE );

	Matrix4		matTexture;
	matTexture.SetIdentity();
	matTexture._41 = 0.0F;
	matTexture._42 = 0.022F  * m_dwTick / 1000;
	SetTextureTransform( 0, matTexture );

	matTexture.SetIdentity();
	matTexture._41 = 0.022F  * m_dwTick / 1000;
	matTexture._42 = 0.022F  * m_dwTick / 1000;
	SetTextureTransform( 1, matTexture );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
	m_pVB->Draw( D3DPT_TRIANGLELIST, 10 * 10 * 2 );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	m_dwTick += dwTick;
} //CW3DWater::Render

/**
 * 렌더링 상태 설정.
 */
void CW3DWater::SetRenderState()
{
	D3D::ZEnable( TRUE );
	D3D::ZWriteEnable( FALSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,	0 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,		D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,		D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER,	D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER,	D3DTEXF_LINEAR );

	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,	1 );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,		D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2,	D3DTA_CURRENT );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2,	D3DTA_TFACTOR );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU,		D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV,		D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER,	D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER,	D3DTEXF_LINEAR );

	g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,		0x40FFFFFF );

	D3D::AlphaBlend( TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA );

	D3D::AlphaTest( FALSE );

	D3D::Lighting( FALSE );
	D3D::FogEnable( TRUE );
} //CW3DWater::SetRenderState

/**
 * Texture Transform 세팅
 */
void CW3DWater::SetTextureTransform( int nStage, const Matrix4& matTransform )
{
	static Matrix4	matTexture;
	matTexture = matTransform;

	matTexture._31 = matTexture._41;
	matTexture._32 = matTexture._42;
	matTexture._41 = 0.0F;
	matTexture._42 = 0.0F;

	switch( nStage )
	{
	case 0:
		g_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX*)&matTexture );
		break;

	case 1:
		g_pd3dDevice->SetTransform( D3DTS_TEXTURE1, (D3DMATRIX*)&matTexture );
		break;
	} //switch
} //CW3DWater::SetTextureTransform

