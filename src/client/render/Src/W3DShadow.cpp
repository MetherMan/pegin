/**
 * 그림자 클래스 구현부 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-17 11:38p $
 * @version	$Revision: 39 $
 *
 * <HR>
 * $Log: /Render/Src/W3DShadow.cpp $
 * 
 * 39    03-12-17 11:38p Kjmgo
 * Alt + Tab 지원
 * 
 * 38    03-11-13 5:46p Paran
 * 
 * 37    03-05-22 5:17a Ahastudio
 *
 * 33    03-04-19 7:03p Kjmgo		그림자 버그 수정.
 *
 * 32    03-04-13 8:53p Kjmgo		그림자 추가.
 *
 * 25    03-03-13 4:29p Ahastudio	텍스처 로딩 부분 변경.
 *
 * 24    03-03-11 8:27p Kjmgo		필요 없는 함수 제거 및 복수 Blocker 처리로 함수를 합침.
 * <HR>
 *
 * @file	W3DShadow.cpp
 */


#include "Render_Export.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "ProjectiveShadow.h"


/**
 * 그림자 클래스 구현.
 * @ingroup		Render
 */
class CW3DShadow : public IW3DShadow
{
public:
	UNDER_MEMORY_MGR( CW3DShadow );

	CW3DShadow();
	~CW3DShadow();

	BOOL	Init( int nType );
	void	SetSize( int nSize );

	void	SetLight( const Vector3& vLight );

	void	Render( const Matrix4& matWorld );

	// Projective Shadow를 위한 함수들 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	void	InsertModel( IW3DModel* pModel );

	void	MakeShadowMap( DWORD dwTick );
	void	ShadowMatrix( const Vector3& vBlockerPosition );
	void	InsertBlocker( IW3DModel* pBlocker );

	/**
	 * 모델의 XY좌표 최대 값을 설정한다.
	 */
	void	SetXYMax( IW3DModel* pModel )
	{
		m_pProjectiveShadow->SetXYMax( pModel );
	} //SetXYMax

	void	InsertVertex( const Vector3& vVertex );

	BOOL	IsProjection();

	void	Restore();
	void	Cleanup();

private:
	struct VERTEX
	{
		enum		{ FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		Vector3		p;
		float		tu, tv;
	}; //struct VERTEX

	CVertexBuffer*		m_pVB;					///<
	CTexture*			m_pTexture;				///<

	typedef std::vector< IW3DModel* >	ModelVector;
	ModelVector	m_vecBlocker;

	CProjectiveShadow*	m_pProjectiveShadow;	///<

	int					m_nSize;				///<
	int					m_nType;				///< 그림자 종류 ( 0 - Circle Shadow(default), 1 - Projective Shadow )

	BOOL	CreateShadow();
	BOOL	CreateProjectiveShadow();

	void	DrawShadow( const Matrix4& matWorld );
	void	DrawProjectiveShadow( const Matrix4& matWorld );
}; //class CW3DShadow


IMPLEMENT_COMPONENT( CW3DShadow, Shadow );


/**
 * 생성자.
 */
CW3DShadow::CW3DShadow()
{
	m_pVB				= NULL;
	m_pTexture			= NULL;
	m_pProjectiveShadow	= NULL;

	m_nSize				= 3;
	m_nType				= 0;

	m_vecBlocker.clear();
} //CW3DShadow::CW3DShadow

/**
 * 소멸자.
 */
CW3DShadow::~CW3DShadow()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_DELETE( m_pVB );
	SAFE_DELETE( m_pProjectiveShadow );
}

/**
 * 그림자 초기화
 *
 * @param	nType		[IN] 그림자 종류
 *							- 0 : 원형
 *							- 1 : Projective Shadow Model
 *							- 2 : Projective Shadow Terrain
 *
 * @return	초기화 성공 여부
 */
BOOL CW3DShadow::CreateShadow()
{
	// 텍스처 로딩
	m_pTexture = g_pTextureMgr->LoadTexture( "Texture/Effect/", "shadow.bmp" );

	if( !m_pTexture )
	{
		Assert( !"텍스처 읽기 실패" );
		return FALSE;
	} //if

	// VertexBuffer 생성

	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );

	if( !m_pVB ||
		!m_pVB->Create( 4 ) )
	{
		Assert( !"버텍스 버퍼 생성 실패" );
		SAFE_DELETE( m_pVB );
		return FALSE;
	} //if

	// Vertex 구성

	VERTEX*		pVertices = (VERTEX*)m_pVB->Lock();

	if( !pVertices )
	{
		Assert( !"Vertex Lock 실패" );
		return FALSE;
	} //if

	float	fRadius = 1.0F;

	pVertices[0].p.x = -fRadius;
	pVertices[0].p.y = -fRadius;
	pVertices[0].p.z = 0.0F;
	pVertices[0].tu  = 0.0F;
	pVertices[0].tv  = 0.0F;

	pVertices[1].p.x =  fRadius;
	pVertices[1].p.y = -fRadius;
	pVertices[1].p.z = 0.0F;
	pVertices[1].tu  = 1.0F;
	pVertices[1].tv  = 0.0F;

	pVertices[2].p.x = -fRadius;
	pVertices[2].p.y =  fRadius;
	pVertices[2].p.z = 0.0F;
	pVertices[2].tu  = 0.0F;
	pVertices[2].tv  = 1.0F;

	pVertices[3].p.x =  fRadius;
	pVertices[3].p.y =  fRadius;
	pVertices[3].p.z = 0.0F;
	pVertices[3].tu  = 1.0F;
	pVertices[3].tv  = 1.0F;

	m_pVB->Unlock();

	return TRUE;
} //CW3DShadow::CreateShadow

/**
 * Projective Shadow 생성.
 *
 * @return	성공 여부.
 */
BOOL CW3DShadow::CreateProjectiveShadow()
{
	m_pProjectiveShadow = new CProjectiveShadow;

	if( !m_pProjectiveShadow || !m_pProjectiveShadow->CreateShadow( m_nSize ) )
	{
		return FALSE;
	} //if

	IW3DScene* pScene = MAKE_COMPONENT( Scene );

	pScene->AddShadow( this );

	return TRUE;
} //CW3DShadow::CreateProjectiveShadow

/**
 * 그림자 초기화.
 *
 * @param	nType		[IN] 그림자 종류.
 *
 * @return	초기화 성공 여부.
 */
BOOL CW3DShadow::Init( int nType )
{
	Assert( NULL == m_pTexture );
	Assert( NULL == m_pVB );

	BOOL bFlag = FALSE;

	m_nType = nType;

	if( nType == 0 )	// 동그란 그림자
	{
		bFlag = CreateShadow();
	}
	else				// Projective Shadow
	{
		bFlag = CreateProjectiveShadow();
	} //if..else..

	return bFlag;
} //CW3DShadow::Init

/**
 * 그림자의 Size를 설정한다.
 *
 * @param	nSize	[IN] 그림자 텍스처의 크기.
 *						Projective Shadow일 때만 적용한다 ( Default : 64 )
 */
void CW3DShadow::SetSize( int nSize )
{
	Assert( nSize > 0 ); //&& nSize_ <= 512 );

	m_nSize = nSize;
} //CW3DShadow::SetSize

/**
 * 그림자 렌더링.
 *
 * @param	matWorld	[IN] 월드 행렬.
 */
void CW3DShadow::Render( const Matrix4& matWorld )
{
//	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	if( m_nType == 0 )	// Default : Circle Shadow 
	{
		DrawShadow( matWorld );
	}
	else
	{
		DrawProjectiveShadow( matWorld );
	} //if..else..
} //CW3DShadow::Render

/**
 * 동그란 그림자를 그린다.
 *
 * @param	matWorld	[IN] 월드 행렬.
 */
void CW3DShadow::DrawShadow( const Matrix4& matWorld )
{
	Matrix4		matrix;
	_ZeroMemory( &matrix, sizeof( matrix ) );
	matrix._11 =  1.0F;
	matrix._23 = -1.0F;
	matrix._32 =  1.0F;
	matrix._44 =  1.0F;
	matrix *= matWorld;

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matrix );

	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_DISABLE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,		D3DTADDRESS_CLAMP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,		D3DTADDRESS_CLAMP );

	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ZERO );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_SRCCOLOR );

	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,	FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );

	g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,	FALSE );

	m_pTexture->SetTexture( 0 );

	m_pVB->Draw( D3DPT_TRIANGLESTRIP, 2 );
} //CW3DShadow::DrawShadow

/**
 * Projective Shadow를 그린다.
 *
 * @param	matView	[IN] View Matrix
 */
void CW3DShadow::DrawProjectiveShadow( const Matrix4& matView )
{
	Assert( m_pProjectiveShadow );

	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_DISABLE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

//	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,		D3DTADDRESS_BORDER );
//	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,		D3DTADDRESS_BORDER );

	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ZERO );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_SRCCOLOR );

	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );

	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,	FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT,	0xFFFFFFFF );

	g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,	FALSE );

	m_pProjectiveShadow->Update( matView );

	// 전체적으로 들어온 것들은 그림자를 그려준다.
	m_pProjectiveShadow->BeginRenderToModel();

	// 지형이라고 되어 있지만 들어온 Vertex들에 그림자를 그려준다.
	m_pProjectiveShadow->DrawToTerrain();

	// List에 있는 모델들에 
	m_pProjectiveShadow->DrawToObject();
	m_pProjectiveShadow->EndRenderToModel();
} //CW3DShadow::DrawProjectiveShadow

/**
 * Set Light Position.
 */
void CW3DShadow::SetLight( const Vector3& vLight )
{
	Assert( m_pProjectiveShadow );

	m_pProjectiveShadow->SetLight( vLight );
} //CW3DShadow::SetLight

/**
 * 그림자를 그릴 Model을 추가한다.
 */
void CW3DShadow::InsertModel( IW3DModel* pModel )
{
	Assert( m_pProjectiveShadow );

	m_pProjectiveShadow->InsertModel( pModel );
} //CW3DShadow::InsertModel

/**
 * Shadow Matrix.
 */
void CW3DShadow::ShadowMatrix( const Vector3& vBlockerPosition )
{
	Assert( m_pProjectiveShadow );

	m_pProjectiveShadow->ShadowMatrix( vBlockerPosition );

	IW3DModel*	pBlocker = NULL;

	ModelVector::iterator	itor = m_vecBlocker.begin();

	while( itor != m_vecBlocker.end() )
	{	
		pBlocker = (*itor);

		if( pBlocker )
		{
			m_pProjectiveShadow->SetXYMax( pBlocker );
		} //if

		itor++;
	} //while

} //CW3DShadow::ShadowMatrix

/**
 *	Insert Blocker
 */
void CW3DShadow::InsertBlocker( IW3DModel* pBlocker )
{
	Assert( pBlocker );

	m_vecBlocker.push_back( pBlocker );
} //CW3DShadow::InsertBlocker

/**
 *
 */
void CW3DShadow::MakeShadowMap( DWORD dwTick_ )
{
	Assert( m_pProjectiveShadow );

	if( !m_pProjectiveShadow->BeginScene() )
	{
		return ;
	}

	IW3DModel*	pBlocker = NULL;
	BOOL		bFlag = FALSE;

	ModelVector::iterator	itor = m_vecBlocker.begin();

	while( itor != m_vecBlocker.end() )
	{	
		pBlocker = (*itor);

		if( pBlocker )
		{
			pBlocker->SetUseTexutre( FALSE );
			pBlocker->SetShadow( TRUE );

			g_pd3dDevice->SetTexture( 0, NULL );
			pBlocker->Render( dwTick_ );

			pBlocker->SetShadow( FALSE );
			pBlocker->SetUseTexutre( TRUE );

			bFlag = TRUE;
		
		} //if

		itor = m_vecBlocker.erase( itor );
	} //while

	m_vecBlocker.clear();

	m_pProjectiveShadow->SetShadowMap( bFlag );
	m_pProjectiveShadow->EndScene();

} //CW3DShadow::MakeShadowMap

/**
 * 지형에 그릴 버텍스를 추가한다.
 */
void CW3DShadow::InsertVertex( const Vector3& vVertex )
{
	Assert( m_pProjectiveShadow );

	m_pProjectiveShadow->InsertVertex( vVertex );
} //CW3DShadow::InsertVertex

/**
 *	Projection Shadow인지..
 */
BOOL CW3DShadow::IsProjection()
{
	if( m_nType == 1 )
	{
		return TRUE;
	}

	return FALSE;
} //CW3DShadow::IsProjection

/**
 *
 */
void CW3DShadow::Restore()
{
	if( m_nType != 1 )
	{
		return ;
	} //if

	m_pProjectiveShadow->Restore();
} //CW3DShadow::Restore

void CW3DShadow::Cleanup()
{
	if( m_nType != 1 )
	{
		return ;
	} //if

	m_pProjectiveShadow->Cleanup();
} //CW3DShadow::Cleanup