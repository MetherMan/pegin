/**
 * 프로젝션 그림자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 11:34a $
 * @version	$Revision: 53 $
 *
 * <HR>
 * $Log: /Render/Src/ProjectiveShadow.cpp $
 * 
 * 53    04-01-08 11:34a Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	ProjectiveShadow.cpp
 */


#include "Render_Export.h"
#include "ProjectiveShadow.h"


//#define SHOWSHADOWMAP


/**
 * 생성자.
 */
CProjectiveShadow::CProjectiveShadow()
:	m_pRenderToSurface		( NULL ),
	m_pShadowTexture		( NULL ),
	m_pShadowSurface		( NULL ),
	m_pBlocker				( NULL ),
	m_bShadowMap			( FALSE ),
	m_nTextureLevel			( 0 ),
	m_nVertexCount			( 0 ),
	m_nFaces				( 0 ),
	m_nType					( 0 ),
	m_nShadowSize			( 1 ),
	m_nTextureSize			( 256 ),
	m_vXYMax				( 0.0F, 0.0F, 0.0F ),
	m_vLight				( 0.0F, 0.0F, 0.0F )

{
	m_matUV.SetIdentity();
	m_matWorldToLight.SetIdentity();
	m_matBlocker.SetIdentity();
	m_matWorldToLight.SetIdentity();
	m_matLocalToWorld.SetIdentity();
	m_matShadowProj.SetIdentity();
	m_vecModel.clear();
} //CProjectiveShadow::CProjectiveShadow

/**
 * 소멸자.
 */
CProjectiveShadow::~CProjectiveShadow()
{
	SAFE_RELEASE( m_pRenderToSurface );
	SAFE_RELEASE( m_pShadowSurface );
	SAFE_RELEASE( m_pShadowTexture );
} //CProjectiveShadow::~CProjectiveShadow

/**
 * 그림자를 만든다.
 *
 * @param	nSize		[IN] 그림자 크기
 *
 * @retrun	그림자 생성 성공 여부
 */
BOOL CProjectiveShadow::CreateShadow( int nSize )
{
	Assert( nSize > 0 && nSize < 10 );

	m_nShadowSize = nSize;

	//- Create Texture Type : RenderTarget

	HRESULT		hr;

	// D3DPOOL_MANAGED 로 설정해 주게 되면 오류가 뜹니다.
	// D3DPOOL_DEFAULT 로 해주어야만 합니다.
	// 참고로, 여기서 생성되는 텍스처는
	// 비디오 메모리로 올라가게 됩니다.
	hr = D3DXCreateTexture(	g_pd3dDevice,
							m_nTextureSize,
							m_nTextureSize,
							1,
							D3DUSAGE_RENDERTARGET,
							g_d3dDesktopMode.Format,
							D3DPOOL_DEFAULT,
							&m_pShadowTexture );
	if( FAILED( hr ) )
	{
		return FALSE;
	}

	//- Create Texture Type : Normal

	D3DSURFACE_DESC desc;
   
	m_pShadowTexture->GetSurfaceLevel( 0, &m_pShadowSurface );
    m_pShadowSurface->GetDesc( &desc );

	//- Create RenderToSurface

	hr = D3DXCreateRenderToSurface( g_pd3dDevice, desc.Width, desc.Height, desc.Format,
									FALSE, D3DFMT_UNKNOWN, &m_pRenderToSurface );
	if( FAILED( hr ) )
	{
		return FALSE;
	} //if


	return TRUE;
} //CProjectiveShadow::CreateShadow

/**
 *
 */
void CProjectiveShadow::Cleanup()
{
	SAFE_RELEASE( m_pRenderToSurface );
	SAFE_RELEASE( m_pShadowSurface );
	SAFE_RELEASE( m_pShadowTexture );
} //CProjectiveShadow::Cleanup

/**
 *
 */
void CProjectiveShadow::Restore()
{
	SAFE_RELEASE( m_pRenderToSurface );
	SAFE_RELEASE( m_pShadowSurface );
	SAFE_RELEASE( m_pShadowTexture );

	CreateShadow( m_nShadowSize );
} //CprojectiveShadow::Restore

/**
 * 빛 설정.
 *
 * @param	vLight	[IN] Light 위치
 */
void CProjectiveShadow::SetLight( const Vector3& vLight )
{
	m_vLight = vLight + Vector3( 2.0F, -2.0F, 7.6F );
} //CProjectiveShadow::SetLight

/**
 * 그림자 UV 행렬을 생성한다.
 */
void CProjectiveShadow::ShadowMatrix( const Vector3& vBlockerPosition )
{
	// 각종 연산을 해준다.

	m_matLocalToWorld.SetIdentity();
	m_matLocalToWorld.Translate( vBlockerPosition );

	D3DXMatrixLookAtLH( (D3DXMATRIX*)&m_matWorldToLight, (D3DXVECTOR3*)&m_vLight, (D3DXVECTOR3*)&vBlockerPosition, &D3DXVECTOR3(0.0f,0.0f,1.0f));
	D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)&m_matShadowProj, D3DX_PI/4, 1.0f, 1.0f, 1000.0f );

} //CProjectiveShadow::ShadowMatrix

/**
 * 모델에 그림자를 그린다.
 */
void CProjectiveShadow::SetXYMax( IW3DModel* pModel )
{
	Assert( pModel );

	Matrix4		matLocalToLight = m_matLocalToWorld * m_matWorldToLight;

	Vector3		vProj = pModel->GetXYMax( matLocalToLight );

	float f = (float)m_nShadowSize * 0.1F;

	if( vProj.x > m_vXYMax.x )
	{
		m_vXYMax.x = vProj.x;
	} //if

	if( vProj.y > m_vXYMax.y )
	{
		m_vXYMax.y = vProj.y;
	} //if

	m_matShadowProj.m[0][0] = f / m_vXYMax.x;
	m_matShadowProj.m[1][1] = f / m_vXYMax.y;


} //CProjectiveShadow::SetXYMax

/**
 * 그림자 맵을 만듭니다.
 */
void CProjectiveShadow::MakeShadowMap()
{
	if( !BeginScene() )		return;

	m_pBlocker->SetUseTexutre( FALSE );

	g_pd3dDevice->SetTexture( 0, NULL );
	m_pBlocker->Render( 0 );

	m_pBlocker->SetUseTexutre( TRUE );

	EndScene();
} //CProjectiveShadow::MakeShadowMap

/**
 * Texture에 렌더링할 준비를 한다.
 *
 * @return	BeginScene 성공 여부.
 */
BOOL CProjectiveShadow::BeginScene()
{
	HRESULT		hr;

	hr = m_pRenderToSurface->BeginScene( m_pShadowSurface, NULL );

	if( FAILED( hr ) )
	{
		Assert( !"Render To Surface Fail." );
		return FALSE;
	} //if

	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB( 0, 255, 255, 255), 0, 0 );

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE,	D3DCULL_NONE );
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,	D3DZB_FALSE );

	g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB( 255, 115, 115, 115 ) );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TFACTOR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1);
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TFACTOR);

	g_pd3dDevice->SetTransform( D3DTS_WORLD,		(D3DMATRIX*)&m_matLocalToWorld	);
	g_pd3dDevice->SetTransform( D3DTS_VIEW,			(D3DMATRIX*)&m_matWorldToLight	);
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION,	(D3DMATRIX*)&m_matShadowProj	);

	return TRUE;
} //CProjectiveShadow::BeginScene

/**
 * Texture에 렌더링을 끝마친다.
 *
 * @return	EndScene 성공 여부.
 */
BOOL CProjectiveShadow::EndScene()
{
	HRESULT		hr;

	hr = m_pRenderToSurface->EndScene();

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );                 

	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,			D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_TFACTOR );

	if( FAILED( hr ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CProjectiveShadow::EndScene

/**
 * 그림자 텍스처 사용.
 */
void CProjectiveShadow::SetTexture()
{
	Assert( g_pd3dDevice );
	Assert( m_pShadowTexture );

	g_pd3dDevice->SetTexture( 0 , m_pShadowTexture );
} //CProjectiveShadow::SetTexture

/**
 * Update.
 */
void CProjectiveShadow::Update( const Matrix4& matView )
{
	Matrix4 matCameraToWorld = matView.Inverse();
	Matrix4 matScale,matOffset;
	Vector3 vTrans = Vector3( 0.5F, 0.5F, 0.0F );

	matOffset.SetIdentity();
	matOffset.Translate( vTrans );

	matScale.SetIdentity();
	matScale._11 = 0.5F;
	matScale._22 = -0.5F;
	matScale._33 = 1.0F;

	m_matUV = matCameraToWorld * m_matWorldToLight;
	m_matUV = m_matUV * m_matShadowProj;
	m_matUV = m_matUV * matScale;
	m_matUV = m_matUV * matOffset;
	
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4|D3DTTFF_PROJECTED );

	g_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DXMATRIX*)&m_matUV );

} //CProjectiveShadow::Update

/**
 * 렌더링 상태 설정.
 */
void CProjectiveShadow::SetRenderState()
{
	///////////////////////////////////////////////////////////////////////////////

//	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_COLORARG1,D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_COLORARG2,D3DTA_CURRENT);

	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

	///////////////////////////////////////////////////////////////////////////////

	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ZERO );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_SRCCOLOR );

	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZFUNC,			D3DCMP_LESSEQUAL );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );
} //CProjectiveShadow::SetRenderState

/**
 * 지형에 그림자를 그린다.
 */
BOOL CProjectiveShadow::DrawToTerrain()
{
	if( m_nFaces < 1  || !m_bShadowMap )
	{
		m_nVertexCount = 0;
		m_nFaces	   = 0;
		return FALSE;
	} //9

	Matrix4 matWorld;
	matWorld.SetIdentity();

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	g_pd3dDevice->SetVertexShader( D3DFVF_XYZ );
	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST,  m_nFaces, m_vTerrain, sizeof( Vector3 ) );

	m_nVertexCount = 0;
	m_nFaces	   = 0;

	return TRUE;
} //CProjectiveShadow::DrawToTerrain

/**
 * 메쉬에 그림자를 그린다.
 */
BOOL CProjectiveShadow::DrawToObject()
{
	if( m_vecModel.empty() || !m_bShadowMap )
	{
		return FALSE;
	} //if

	ModelVector::iterator	itor = m_vecModel.begin();

	while( itor != m_vecModel.end() )
	{
		IW3DModel*	pModel = *itor;

		if( pModel )
		{
			pModel->SetUseTexutre( FALSE );
			SetTexture();
			pModel->Render( 0 );
			pModel->SetUseTexutre( TRUE );
		} //if

		itor = m_vecModel.erase( itor );
	} //while

	m_vecModel.clear();

	m_vXYMax = Vector3( 0.0F,0.0F,0.0F );

	return TRUE;
} //CProjectiveShadow::DrawToObject

/**
 * 모델에 그림자를 그린다.
 */
BOOL CProjectiveShadow::BeginRenderToModel()
{
	SetTexture();
	return TRUE;
} //CProjectiveShadow::BeginRenderToModel


/**
 * 모델에 그림자를 그린다.
 */
BOOL CProjectiveShadow::EndRenderToModel()
{
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
	g_pd3dDevice->SetTextureStageState( m_nTextureLevel, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
	
	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );

	m_bShadowMap = FALSE;
	
	return TRUE;
} //CProjectiveShadow::EndRenderToModel

/**
 * 그림자를 그릴 모델을 추가한다.
 */
void CProjectiveShadow::InsertModel( IW3DModel* pModel )
{
	m_vecModel.push_back( pModel );
} //CProjectiveShadow::InsertModel

/**
 * 지형에 그릴 버텍스를 추가한다.
 */
void CProjectiveShadow::InsertVertex( const Vector3& vVertex )
{
	if( m_nFaces < MAXPOLYGON || m_nVertexCount < MAXPOLYGON * 3 )
	{
		m_vTerrain[m_nVertexCount] = vVertex;

		m_nVertexCount++;

		m_nFaces = m_nVertexCount / 3;
	} 
	else
	{
		m_nFaces = MAXPOLYGON - 1;
		m_nVertexCount = m_nFaces * 3;
	} //if..else..
} //CProjectiveShadow::InsertVertex
