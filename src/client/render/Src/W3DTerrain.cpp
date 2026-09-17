/**
 * 지형 모듈 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-03-08 11:19a $
 * @version	$Revision: 118 $
 *
 * <HR>
 * $Log: /Render/Src/W3DTerrain.cpp $
 * 
 * 118   04-03-08 11:19a Paran
 * 
 * 117   04-02-02 4:47p Kjmgo
 * 
 * 116   04-01-31 1:08p Kjmgo
 * 
 * 115   04-01-29 6:57p Kjmgo
 * 
 * 114   04-01-13 9:43p Kjmgo
 * 퀄리티 설정 적용.
 * 
 * 113   04-01-08 8:50a Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	W3DTerrain.cpp
 */

#include "W3DTerrain.h"


IMPLEMENT_COMPONENT( CW3DTerrain, Terrain );

 /**
 * 생성자.
 */
CW3DTerrain::CW3DTerrain()
:	m_nSize				( 0 ),
	m_nLevel			( 0 ),
	m_nLayer			( 0 ),
	m_dwTick			( 0 ),
	m_nObjectSize		( 0 ),
	m_nQuality			( 0 ),
	m_fWaterHeight		( 0.0F ),
	m_fAddHeight		( 0.0F ),
	m_bMultiPass		( TRUE ),
	m_bShowCloud		( TRUE ),
	m_bUseLevel			( TRUE ),
	m_bUseObjectHeight	( TRUE ),
	m_bUseTexture		( TRUE ),
	m_bUseLight			( TRUE ),
	m_bUseWireframe		( FALSE ),
	m_bUseLevelOne		( FALSE ),
	m_bCompulsionUpdate	( FALSE ),	
	m_pfHeight			( NULL ),
	m_pdwColor			( NULL ),
	m_pvNormal			( NULL ),
	m_pBlockList		( NULL ),	
	m_pfEffectUV		( NULL ),
	m_pTextureForEffect ( NULL ),
	m_pWater			( NULL ),
	m_pVB				( NULL ),
	m_pTexture			( NULL ),
	m_pbTexture			( NULL ),		
	m_vPos				( 0.0F, 0.0F, 0.0F )
{
	for( int i = 0 ; i < MAX_LEVEL ; i++ )
	{
		m_pfObjectHeight[i]	= NULL;
		m_pbAttribute[i]	= NULL;
	} //for
} //CW3DTerrain::CW3DTerrain

/**
 * 소멸자.
 */
CW3DTerrain::~CW3DTerrain()
{
	for( int i = 0 ; i < MAX_LEVEL ; i++ )
	{
		// 메모리 경계 체크 
		CHECK_BOUNCE( m_pfObjectHeight[i] );
		CHECK_BOUNCE( m_pbAttribute[i] );
	} //if

	// 메모리 경계 체크 
	CHECK_BOUNCE( m_pvNormal );
	CHECK_BOUNCE( m_pdwColor );
	CHECK_BOUNCE( m_pfEffectUV );
	CHECK_BOUNCE( m_pfHeight );
	CHECK_BOUNCE( m_pbTexture );

	Cleanup();
} //CW3DTerrain::~CW3DTerrain

/**
 *
 */
void CW3DTerrain::Cleanup()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_RELEASE( m_pTextureForEffect );
	SAFE_RELEASE( m_pWater );

	for( int i = 0 ; i < MAX_LEVEL ; i++ )
	{
		SAFE_FREE( m_pfObjectHeight[i] );
		SAFE_FREE( m_pbAttribute[i] );
	} //if

	BlockList::iterator itor = m_listBlock.begin();

	while( itor != m_listBlock.end() )
	{
		SAFE_RELEASE( *itor );
		itor = m_listBlock.erase( itor );
	} //while

	SAFE_DELETE( m_pVB );
	SAFE_DELETE( m_pBlockList );

	SAFE_FREE( m_pvNormal );
	SAFE_FREE( m_pdwColor );
	SAFE_FREE( m_pfEffectUV );
	SAFE_FREE( m_pfHeight );
	SAFE_FREE( m_pbTexture );
} 

/**
 * 지형을 초기화한다.
 *
 * @return	초기화 성공 여부.
 */
BOOL CW3DTerrain::Init(
	int			nSize,			///< [IN] 지형 크기 (2m단위)
	const char*	szTextureName,	///< [IN] 지형 텍스처 이름
	BOOL		bLight
	)
{
	Assert( nSize > 0 );
	Assert( szTextureName );
	Assert( *szTextureName );

	Cleanup();

	m_bUseLight = bLight;
	m_nSize = nSize;
	m_nObjectSize = m_nSize * TILE_SIZE;

	// Height Buffer
	m_pfHeight = (float*)_MemAlloc( sizeof( float ) * m_nSize * m_nSize );
	CHECK_PTR_RETURN( m_pfHeight );
	_ZeroMemory( m_pfHeight, sizeof( float ) * m_nSize * m_nSize );

	m_pfEffectUV = (float*)_MemAlloc( sizeof( float ) * m_nSize * m_nSize );
	CHECK_PTR_RETURN( m_pfEffectUV );
	_ZeroMemory( m_pfEffectUV, sizeof( float ) * m_nSize * m_nSize );

	// Texture Buffer
	m_pbTexture = (BYTE*)_MemAlloc( sizeof( BYTE ) * m_nSize * m_nSize );
	CHECK_PTR_RETURN( m_pbTexture );
	_ZeroMemory( m_pbTexture, sizeof( BYTE ) * m_nSize * m_nSize );

	// Color Buffer
	m_pdwColor = (DWORD*)_MemAlloc( sizeof( DWORD ) * m_nSize * m_nSize );
	CHECK_PTR_RETURN( m_pdwColor );
	_DWMemSet( m_pdwColor, 0xFF808080, m_nSize * m_nSize );

	// Normal Buffer
	m_pvNormal = (Vector3*)_MemAlloc( sizeof( Vector3 ) * m_nSize * m_nSize );
	CHECK_PTR_RETURN( m_pvNormal );
	CalculateNormals();

	// Height Buffer
	for( int i = 0 ; i < MAX_LEVEL ; i++ )
	{
		m_pfObjectHeight[i] = (float*)_MemAlloc( sizeof( float ) * m_nSize * m_nSize * TILE_SIZE * TILE_SIZE );
		CHECK_PTR_RETURN( m_pfObjectHeight[i] );
		_ZeroMemory( m_pfObjectHeight[i], sizeof( float ) * m_nSize * m_nSize * TILE_SIZE * TILE_SIZE );
		
		// Attribute Buffer
		m_pbAttribute[i] = (BYTE*)_MemAlloc( sizeof( BYTE ) * m_nSize * m_nSize );
		CHECK_PTR_RETURN( m_pbAttribute[i] );
		_ZeroMemory( m_pbAttribute[i], sizeof( BYTE ) * m_nSize * m_nSize );
	} //for

	// Vertex Buffer
	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );
	CHECK_PTR_RETURN( m_pVB );

	// Say : 버텍스 버퍼를 약 250* 250 크기일때 250000개를 생성한다.
	//		하지만 정작 쓰는건 1만개 정도이니 나머지 24만개가 놀게 된다.
	//		그래서 최대 한도로 생성한다. ( 평균 250*맵에서 9650개 정도만 사용한다.  )
	m_pVB->Create( MAX_VERTICES );

	// Texture Load
	m_pTexture = g_pTextureMgr->LoadTexture( "Texture/Terrain/", szTextureName );
	m_pTextureForEffect = g_pTextureMgr->LoadTexture( "Texture/Terrain/", "cloud.bmp" );

	m_pBlockList = new CBlockList;
	m_pBlockList->Init( m_nSize * TILE_SIZE, 20, this );

	m_pWater = MAKE_COMPONENT( Water );

	m_pWater->Create( "water.bmp", 10, 50, m_fWaterHeight );
	m_pWater->SetPosition( Vector3( 250, 250, 0.0F ) );

	m_bMultiPass		= TRUE;

	return TRUE;
} //CW3DTerrain::Init

/**
 * 업데이트.
 *
 * @param	bUseTexture		[IN] 텍스처 사용 여부
 */
void CW3DTerrain::Update( DWORD dwTick, BOOL bUseTexture )
{
#define HEIGHT_MAP()		m_pfHeight[( y + k % m_nSize ) * m_nSize + ( x + l % m_nSize )]
#define COLOR_MAP()			m_pdwColor[( y + k % m_nSize ) * m_nSize + ( x + l % m_nSize )]
#define NORMAL_MAP()		m_pvNormal[( y + k % m_nSize ) * m_nSize + ( x + l % m_nSize )]

	Assert( m_pVB );

	static int s_nOldSizeX, s_nOldSizeY;
	static int s_nOldStartX, s_nOldStartY;
	
	static Matrix4 matrix;
	static Vector3 vDir;
	static Vector3 vPick;

	//---------------------------------------------
	//	World Matrix Setting
	//---------------------------------------------
	static Matrix4 matWorld;
	matWorld.SetIdentity();
	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	// Set RenderState
	SetRenderState();

	// Set Texture
	if( bUseTexture && !m_bUseWireframe )
	{
		SET_TEXTURE( 0, m_pTexture );

		if( m_bShowCloud && ( m_nQuality < 2 ) )
			SET_TEXTURE( 1, m_pTextureForEffect );
		else
			g_pd3dDevice->SetTexture( 1, NULL );		
	}
	else
	{
		g_pd3dDevice->SetTexture( 0, NULL );
		g_pd3dDevice->SetTexture( 1, NULL );
	} //if..else..

	// 출력 범위 설정.
	matrix	= g_Frustum.m_matView.Inverse();
	vDir	= Vector3( matrix._31, matrix._32, matrix._33 );
	vPick	= m_vPos;
	
	int		nRange = 30; //20

	vDir *= 12.0F;
	m_vPos = vPick;

	int nStartX	= (int)vPick.x / TILE_SIZE - nRange + (int)vDir.x;
	int nEndX	= (int)vPick.x / TILE_SIZE + nRange + (int)vDir.x;
	int nStartY	= (int)vPick.y / TILE_SIZE - nRange + (int)vDir.y;
	int nEndY	= (int)vPick.y / TILE_SIZE + nRange + (int)vDir.y;

	if( nStartX < 0 )			nStartX = 0;
	if( nStartY < 0 )			nStartY = 0;
	if( nEndX < 0	)			nEndX   = 0;
	if( nEndY < 0 )				nEndY   = 0;
	if( nStartX >= m_nSize )	nStartX = m_nSize - 1;
	if( nStartY >= m_nSize )	nStartY = m_nSize - 1;
	if( nEndX >= m_nSize )		nEndX   = m_nSize - 1;
	if( nEndY >= m_nSize )		nEndY   = m_nSize - 1;

	int		nSizeX = MAX( 0, nEndX - nStartX + 1 );
	int		nSizeY = MAX( 0, nEndY - nStartY + 1 );

	m_numFaces	= nSizeX * nSizeY * 2;
	Assert( m_numFaces > 0 );

	int	numVertices	= nSizeX * nSizeY * 4;

	if( numVertices  > MAX_VERTICES )
	{
		Assert( !"버텍스 오버" );
		return;
	} //if

	//------------------------------------------
	//	Compulsion Update
	//------------------------------------------
	if( !m_bCompulsionUpdate )
	{
		if( ( s_nOldSizeX == nSizeX )	  && 
			( s_nOldSizeY == nSizeY )	  &&
			( s_nOldStartX == nStartX )   &&
			( s_nOldStartY == nStartY ) )
		{
			return;
		} //if
	} //if

	s_nOldSizeX  = nSizeX;
	s_nOldSizeY  = nSizeY;
	s_nOldStartX = nStartX;
	s_nOldStartY = nStartY;

	VERTEX*		pVertices = (VERTEX*)m_pVB->Lock();
	if( !pVertices )
	{
		Assert( !"버텍스 락 실패" );
		return;
	} //if

	int		nTexture = 0;
	int		nTextureX;	
	int		nTextureY;
	int		nTextureCount = TEXTURE_SIZE / TILE_TEXTURE_SIZE;
	VERTEX vertices[4];

	m_bShowWater = FALSE;
		
	for( int i = 0 ; i < nSizeY - 1; i++ )
	{
		for( int j = 0 ; j < nSizeX - 1; j++ )
		{
			int		x = nStartX + j;
			int		y = nStartY + i;
		
			nTexture = m_pbTexture[y * m_nSize + x];
			nTextureX = nTexture % ( nTextureCount );
			nTextureY = nTexture / ( nTextureCount );

			int nCount = 0;
			for( int k = 0, mk = 1 ; k < 2 ; k++, mk *= -1 )
			{
				for( int l = 0, ml = 1 ; l < 2 ; l++, ml *= -1 )
				{
					vertices[nCount].p.x	= (float)( x + l ) * TILE_SIZE;
					vertices[nCount].p.y	= (float)( y + k ) * TILE_SIZE;
					vertices[nCount].p.z	= HEIGHT_MAP();
					vertices[nCount].n		= NORMAL_MAP();
					vertices[nCount].color	= COLOR_MAP();

					if( vertices[nCount].p.z < 0.0F  )
					{
						m_bShowWater = TRUE;
					} //if

					vertices[nCount].tv[0].tu	= ( nTextureX + l ) * (float)TILE_TEXTURE_SIZE / TEXTURE_SIZE + ( 0.0005F * ml );
					vertices[nCount].tv[0].tv	= ( nTextureY + k ) * (float)TILE_TEXTURE_SIZE / TEXTURE_SIZE + ( 0.0005F * mk );
					vertices[nCount].tv[1].tu	= (float)( x + l ) / m_nSize * 3;
					vertices[nCount].tv[1].tv	= (float)( y + k ) / m_nSize * 3;
					
					nCount++;
				} //for
			} //for

			int		pos = ( i * nSizeX + j ) * 6;

			if( ( x + y ) % 2 == 0 )
			{
				pVertices[pos    ] = vertices[0];
				pVertices[pos + 1] = vertices[1];
				pVertices[pos + 2] = vertices[2];

				pVertices[pos + 3] = vertices[2];
				pVertices[pos + 4] = vertices[1];
				pVertices[pos + 5] = vertices[3];
			}
			else
			{
				pVertices[pos    ] = vertices[0];
				pVertices[pos + 1] = vertices[1];
				pVertices[pos + 2] = vertices[3];

				pVertices[pos + 3] = vertices[0];
				pVertices[pos + 4] = vertices[3];
				pVertices[pos + 5] = vertices[2];
			} //if..else..
		} //for
	} //for

	m_pVB->Unlock();

#undef NORMAL_MAP
#undef COLOR_MAP
#undef HEIGHT_MAP
} //CW3DTerrain::Update

/**
 * 지형을 그린다.
 *
 * @param	bUseTexture		[IN] 텍스처 사용 여부
 */
void CW3DTerrain::Render( DWORD dwTick, BOOL bUseTexture )
{
	Update( dwTick, bUseTexture );

	m_dwTick += dwTick;

	if( m_bShowCloud && ( m_nQuality < 2 ) )
	{
		static Matrix4	matTexture;
		matTexture.SetIdentity();
		matTexture._32 = 0.01F * m_dwTick / 1000;

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

		g_pd3dDevice->SetTransform( D3DTS_TEXTURE1, (D3DMATRIX*)&matTexture );
	} //if

	//m_pQuadTree->Draw();
	m_pVB->Draw( D3DPT_TRIANGLELIST, m_numFaces );

	if( m_bMultiPass && m_nQuality < 3 )
	{
		m_pBlockList->Draw( (int)m_vPos.x, (int)m_vPos.y );
	} //if
	
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
} //CW3DTerrain::Render

/**
 *
 */
void CW3DTerrain::DrawWater( DWORD dwTick )
{
	if( m_bShowWater )
	{
		m_pWater->Render( dwTick );
	} //if
} //CW3DTerrain::DrawWater

/**
 * 렌더링 상태 설정.
 */
void CW3DTerrain::SetRenderState()
{
	D3D::ZEnable( TRUE );
	D3D::ZWriteEnable( TRUE );

	D3D::AlphaBlend( FALSE );
	D3D::AlphaTest( FALSE );

	D3D::Wireframe( m_bUseWireframe );
	
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	
	if( m_bUseWireframe || !m_bUseLight )
	{
		D3D::Lighting( FALSE );
		D3D::FogEnable( FALSE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_SELECTARG1 );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	}
	else
	{
		D3D::Lighting( TRUE );
		D3D::FogEnable( TRUE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,	0 );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_ADDSIGNED  );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX,	1 );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER,	D3DTEXF_LINEAR );

		float fBias = 0.09F;
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fBias)) );

	} //if..else..


} //CW3DTerrain::SetRenderState

/**
 * Height Field의 높이를 설정한다.
 *
 * @param	nX			[IN] X 좌표.
 * @param	nY			[IN] Y 좌표.
 * @param	fHeight		[IN] 높이.
 */
void CW3DTerrain::SetHeight( int nX, int nY, float fHeight )
{
	Assert( m_pVB );
	Assert( m_pfHeight );

	if( nX < 0 || nX >= m_nSize || nY < 0 || nY >= m_nSize )	return;

	m_pfHeight[nY * m_nSize + nX] = fHeight;

	for( int i = -1 ; i <= 1 ; i++ )
	{
		for( int j = -1 ; j <= 1 ; j++ )
		{
			CalculateNormal( nX + j, nY + i );
		} //for
	} //for
} //CW3DTerrain::SetHeight

/**
 * Height Field의 높이를 얻는다.
 *
 * @param	nX			[IN] X 좌표.
 * @param	nY			[IN] Y 좌표.
 *
 * @return	높이.
 */
float CW3DTerrain::GetHeight( float fX, float fY )
{
	#define HEIGHT_MAP( x, y )		m_pfHeight[( (y) % m_nSize ) * m_nSize + ( (x) % m_nSize )]
	#define ATTRIBUTE_MAP( x, y )	m_pbAttribute[m_nLevel][( (y) % m_nSize ) * m_nSize + ( (x) % m_nSize )]

	int		nX = (int)( fX / TILE_SIZE );
	int		nY = (int)( fY / TILE_SIZE );
	
	int		nObjectX = (int)( fX );
	int		nObjectY = (int)( fY );

	if( nX < 0 || nX >= m_nSize || nY < 0 || nY >= m_nSize )
	{
		return 0.0F;
	} //if

	float	fInnerX = ( fX / TILE_SIZE ) - nX;
	float	fInnerY = ( fY / TILE_SIZE ) - nY;

	if( m_bUseObjectHeight )
	{

		BYTE btAttribute = ATTRIBUTE_MAP( nX, nY );

		if( btAttribute != 0 )
		{
			for( int i = 0 ; i < 2 ; i++ )
			{
				for( int j = 0 ; j < 2 ; j++ )
				{
					float fObjectHeight = GetObjectHeight( nObjectX + j, nObjectY + i );

					if( m_bUseLevel && !m_bUseLevelOne )
					{
						if( fObjectHeight < 1.5 )
							m_nLevel = 0;
						else if( fObjectHeight < 9 )
							m_nLevel = 1;
						else 
							m_nLevel = 2;
					} //if
				} //for
			} //for
		} //if

		switch( btAttribute )
		{
		case 1:		//	절대 높이( High )
			return GetHighHeight( nX, nY );
			break;
		case 2:		// 절대 높이 ( Low )
			return GetLowHeight( nX, nY );
			break;
		} //switch
	} //if
	
	static		Vector3		vP[4];
	static		float s_fObjectHeight = 0.0F;
	s_fObjectHeight = 0.0F;
	float fObjectHeight = 0.0F;
	
	int nCount = 0;
	for( int i = 0 ; i < 2 ; i++ )
	{
		for( int j = 0 ; j < 2 ; j++ )
		{
			vP[nCount].x = (float)( nX + j ) * TILE_SIZE;
			vP[nCount].y = (float)( nY + i ) * TILE_SIZE;
			vP[nCount].z = HEIGHT_MAP( nX + j, nY + i );
			fObjectHeight = GetObjectHeight( nObjectX + j, nObjectY + i );
			
			vP[nCount].z += fObjectHeight;
			nCount++;
			s_fObjectHeight += fObjectHeight;
		} //for
	} //for

	if( m_bUseLevel && !m_bUseLevelOne )
	{
		s_fObjectHeight /= 4.0F;
		if( s_fObjectHeight < 1.5F )
			m_nLevel = 0;
		else if( s_fObjectHeight < 7.5F )
			m_nLevel = 1;
		else 
			m_nLevel = 2;
	} //if

	if( ( nX + nY ) % 2 == 0 )
	{
		if( fInnerX + fInnerY <= 1.0F )
		{
			static Plane	plane;
			plane = Plane( vP[0], vP[1], vP[2] );
			return	plane.GetZ( fX, fY );
		}
		else
		{
			static Plane	plane;
			plane = Plane( vP[2], vP[1], vP[3] );
			return	plane.GetZ( fX, fY );
		} //if..else..
	}
	else
	{	
		if( fInnerY <= fInnerX )
		{
			static Plane	plane;
			plane = Plane( vP[0], vP[1], vP[3] );
			return	plane.GetZ( fX, fY );
		}
		else
		{
			static Plane	plane;
			plane = Plane( vP[0], vP[3], vP[2] );
			return	plane.GetZ( fX, fY );
		} //if..else..	
	} //if..else..

	#undef	HEIGHT_MAP
	#undef	ATTRIBUTE_MAP
} //CW3DTerrain::GetHeight

/**
 * 지형에 텍스처를 입힌다.
 *
 * @param	nX			[IN] X 좌표.
 * @param	nY			[IN] Y 좌표.
 * @param	nTexture	[IN] 텍스처 번호.
 */
void CW3DTerrain::SetTexture( int nX, int nY, int nTexture )
{
	Assert( m_pVB );
	Assert( m_pbTexture );
	Assert( 0 <= nTexture && nTexture < 16 * 16 );

	if( nX < 0 || nX >= m_nSize || nY < 0 || nY >= m_nSize )	return;

	m_pbTexture[nY * m_nSize + nX] = nTexture;

} //CW3DTerrain::SetTexture

/**
 * 지형에 텍스처를 입혀진 텍스처를 얻는다.
 *
 * @param	nX			[IN] X 좌표.
 * @param	nY			[IN] Y 좌표.
 *
 * @return	텍스처 번호.
 */
int CW3DTerrain::GetTexture( int nX, int nY )
{
	Assert( m_pbTexture );

	if( nX < 0 || nX >= m_nSize || nY < 0 || nY >= m_nSize )
	{
		return 0;
	} //if

	return m_pbTexture[nY * m_nSize + nX];
} //CW3DTerrain::GetTexture

/**
 * 지형 크기를 얻는다.
 *
 * @return	지형의 크기. ( 가로, 세로 동일 )
 */
int CW3DTerrain::GetSize()
{
	return m_nSize;
} //CW3DTerrain::GetSize

/**
 * 하이트 맵의 버퍼를 얻는다.
 *
 * @return	하이트 맵 버퍼.
 */
float* CW3DTerrain::GetHeightMap()
{
	return m_pfHeight;
} //CW3DTerrain::GetHeightMap

/**
 * 하이트 맵의 버퍼를 얻는다.
 *
 * @return	하이트 맵 버퍼.
 */
float* CW3DTerrain::GetObjectHeightMap()
{
	return m_pfObjectHeight[m_nLevel];
} //CW3DTerrain::GetObjectHeightMap

/**
 * 텍스처 번호 맵의 버퍼를 얻는다.
 *
 * @return	텍스처 번호 맵.
 */
BYTE* CW3DTerrain::GetTextureMap()
{
	return m_pbTexture;
} //CW3DTerrain::GetTextureMap

/**
 * 속성 맵의 버퍼를 얻는다.
 *
 * @return	속성 맵.
 */
BYTE* CW3DTerrain::GetAttributeMap()
{
	return m_pbAttribute[m_nLevel];
} //CW3DTerrain::GetAttributeMap


/**
 * 컬러 맵의 버퍼를 얻는다.
 *
 * @return	컬러 맵의 버퍼
 */
DWORD* CW3DTerrain::GetColorMap()
{
	return m_pdwColor;
} //CW3DTerrain::GetColorMap

/**
 * 지형 전체 버텍스의 Normal 값을 계산한다.
 */
void CW3DTerrain::CalculateNormals()
{
	for( int i = 0 ; i < m_nSize ; i++ )
	{
		for( int j = 0 ; j < m_nSize ; j++ )
		{
			CalculateNormal( j, i );
		} //for
	} //for


} //CW3DTerrain::CalculateNormals

/**
 * 지형 버텍스의 Normal 값을 계산한다.
 *
 * @param	nX		[IN] X 좌표.
 * @param	nY		[IN] Y 좌표.
 */
void CW3DTerrain::CalculateNormal( int nX, int nY )
{
#define HEIGHT_MAP( x, y )		m_pfHeight[( (y) % m_nSize ) * m_nSize + ( (x) % m_nSize )] + 1

	if( nX < 0 || nY < 0 || nX >= m_nSize || nY >= m_nSize )		return;

	if( nX <= 0 || nY <= 0 || nX >= m_nSize - 1 || nY >= m_nSize - 1 )
	{
		m_pvNormal[nY * m_nSize + nX] = Vector3( 0.0F, 0.0F, 1.0F );
		return;
	} //if

	Vector3		vNormal = Vector3( 0.0F, 0.0F, 1.0F );
	Plane		plane;
	Vector3		a, b, c;

	// 왼쪽 위

	a.x = (float)( nX - 1.0F ) * TILE_SIZE;
	a.y = (float)nY            * TILE_SIZE;
	a.z = HEIGHT_MAP( nX - 1, nY );

	b.x = (float)nX         * TILE_SIZE;
	b.y = (float)( nY - 1 ) * TILE_SIZE;
	b.z = HEIGHT_MAP( nX, nY - 1 );

	c.x = (float)nX         * TILE_SIZE;
	c.y = (float)nY         * TILE_SIZE;
	c.z = HEIGHT_MAP( nX, nY );

	plane = Plane( a, b, c );
	vNormal += plane.N;

	// 오른쪽 위

	a.x = (float)nX         * TILE_SIZE;
	a.y = (float)( nY - 1 ) * TILE_SIZE;
	a.z = HEIGHT_MAP( nX, nY - 1 );

	b.x = (float)( nX + 1 ) * TILE_SIZE;
	b.y = (float)nY         * TILE_SIZE;
	b.z = HEIGHT_MAP( nX + 1, nY );

	c.x = (float)nX         * TILE_SIZE;
	c.y = (float)nY         * TILE_SIZE;
	c.z = HEIGHT_MAP( nX, nY );

	plane = Plane( a, b, c );
	vNormal += plane.N;

	// 왼쪽 아래

	a.x = (float)( nX - 1 ) * TILE_SIZE;
	a.y = (float)nY         * TILE_SIZE;
	a.z = HEIGHT_MAP( nX - 1, nY );

	b.x = (float)nX         * TILE_SIZE;
	b.y = (float)nY         * TILE_SIZE;
	b.z = HEIGHT_MAP( nX, nY );

	c.x = (float)nX         * TILE_SIZE;
	c.y = (float)( nY + 1 ) * TILE_SIZE;
	c.z = HEIGHT_MAP( nX, nY + 1 );

	plane = Plane( a, b, c );
	vNormal += plane.N;

	// 오른쪽 아래

	a.x = (float)nX         * TILE_SIZE;
	a.y = (float)nY         * TILE_SIZE;
	a.z = HEIGHT_MAP( nX, nY );

	b.x = (float)( nX + 1 ) * TILE_SIZE;
	b.y = (float)nY         * TILE_SIZE;
	b.z = HEIGHT_MAP( nX + 1, nY );

	c.x = (float)nX         * TILE_SIZE;
	c.y = (float)( nY + 1 ) * TILE_SIZE;
	c.z = HEIGHT_MAP( nX, nY + 1 );

	plane = Plane( a, b, c );
	vNormal += plane.N;

	// Normal 값을 정리..

	if( nX <= 0 )	nX = 0;
	if( nY <= 0 )	nY = 0;


	m_pvNormal[nY * m_nSize + nX] = vNormal.Normal();

#undef HEIGHT_MAP
} //CW3DTerrain::CalculateNormal

/**
 * 와이어프레임 사용 여부를 결정한다.
 *
 * @param	bUse	[IN] 와이어프레임 사용 여부.
 */
void CW3DTerrain::UseWireframe( BOOL bUse )
{
	m_bUseWireframe = bUse;
} //CW3DTerrain::UseWireframe

/**
 * 그림자를 넣는다. ( Projection Shadow )
 *
 */
void CW3DTerrain::InsertShadow( IW3DShadow* pShadow, const Vector3 vPos )
{
	Assert( pShadow );

/*	for( int i = -1 ; i <= 0 ; i++ )
	{
		for( int j = -1 ; j <= 0 ; j++ )
		{
			InsertVertexForShadow( pShadow, vPos + Vector3( TILE_SIZE * i, TILE_SIZE * j, 0.0F ) );
		} //for
	} //for*/

	BOOL		bFlag = TRUE;

	if( m_nLevel > 0 )
	{
		return ;
	} //if

	for( int i = -2 ; i <= 1 ; i++ )
	{
		for( int j = -2 ; j <= 1 ; j++ )
		{
			InsertVertexForShadow( pShadow, vPos + Vector3( TILE_SIZE * i, TILE_SIZE * j, 0.0F ) );
		} //for
	} //for
} //CW3DTerrain::InsertShadow

/**
 * 그림자를 넣는다. ( Projection Shadow )
 *
 */
void CW3DTerrain::InsertVertexForShadow( IW3DShadow* pShadow, const Vector3 vPos )
{
#define HEIGHT_MAP( x, y )		m_pfHeight[( (y) % m_nSize ) * m_nSize + ( (x) % m_nSize )]

	float fX = vPos.x;
	float fY = vPos.y;

	int		nX = (int)( fX / TILE_SIZE );
	int		nY = (int)( fY / TILE_SIZE );
	int		nObjectX = (int)( fX );
	int		nObjectY = (int)( fY );


	if( nX < 0 || nX >= m_nSize || nY < 0 || nY >= m_nSize )
	{
		return ;
	} //if

	Vector3		vP[4];

	int nCount = 0;
	for( int i = 0 ; i < 2 ; i++ )
	{
		for( int j = 0 ; j < 2 ; j++ )
		{
			vP[nCount].x = (float)( nX + j ) * TILE_SIZE;
			vP[nCount].y = (float)( nY + i ) * TILE_SIZE;
			vP[nCount].z = HEIGHT_MAP( nX + j, nY + i ) + 0.1F;
			vP[nCount].z += GetObjectHeight( nObjectX + j , nObjectY + i );
			nCount++;
		} //for
	} //for

	if( ( nX + nY ) % 2 == 0 )
	{
		pShadow->InsertVertex( vP[0] );
		pShadow->InsertVertex( vP[1] );
		pShadow->InsertVertex( vP[2] );

		pShadow->InsertVertex( vP[2] );
		pShadow->InsertVertex( vP[1] );
		pShadow->InsertVertex( vP[3] );
	}
	else
	{
		pShadow->InsertVertex( vP[0] );
		pShadow->InsertVertex( vP[1] );
		pShadow->InsertVertex( vP[3] );

		pShadow->InsertVertex( vP[0] );
		pShadow->InsertVertex( vP[3] );
		pShadow->InsertVertex( vP[2] );
	} //if..else..

#undef HEIGHT_MAP
} //CW3DTerrain::InsertVertexForShadow

/**
 *
 *
 */
void CW3DTerrain::SetLevel( int nLevel )
{
	if( nLevel < 0 || nLevel >= MAX_LEVEL )
	{
		Assert( !"Level가 잘못 들어 왔습니다." );
		nLevel = m_nLevel;
	} //if

	m_nLevel = nLevel;
} //CW3DTerrain::SetLevel

/**
 *
 */
BYTE CW3DTerrain::GetAttribute( float fX, float fY )
{
	#define ATTRIBUTE_MAP( x, y )	m_pbAttribute[m_nLevel][( (y) % m_nSize ) * m_nSize + ( (x) % m_nSize )]

	int		nX = (int)( fX / TILE_SIZE );
	int		nY = (int)( fY / TILE_SIZE );
	
	if( nX <= 0 )	nX = 0;
	if( nY <= 0 )	nY = 0;

	return ATTRIBUTE_MAP( nX, nY );
} //CW3DTerrain::GetAttribute

/**
 *
 */
float CW3DTerrain::GetObjectHeight( int nX, int nY )
{
	if( !m_bUseObjectHeight )
	{
		return 0.0F;
	} //if

	nX = nX % m_nObjectSize;
	nY = nY % m_nObjectSize;

	if( nX <= 0 )	nX = 0;
	if( nY <= 0 )	nY = 0;

	if( m_bUseLevelOne )
	{
	//	m_nLevel = 0;
	} //if

	float fHeight = m_pfObjectHeight[m_nLevel][ nY * m_nObjectSize + nX];

	return fHeight;
} //CW3DTerrain::GetObjectHeight

/**
 *
 */
float CW3DTerrain::GetHighHeight( int nX, int nY )
{
	if( nX <= 0 )	nX = 0;
	if( nY <= 0 )	nY = 0;
	nX = nX % m_nObjectSize;
	nY = nY % m_nObjectSize;


	float fHigh = 0.0F;
	for( int i = 0 ; i < 2 ; i++ )
	{
		for( int j = 0 ; j < 2 ; j++ )
		{
			float fHeight = m_pfHeight[( ( nY + j ) % m_nSize ) * m_nSize + ( ( nX + j ) % m_nSize )];		

			if( fHigh <= fHeight )
			{
				fHigh = fHeight;
			} //if
		} //for
	} //for
	
	return fHigh;
} //CW3DTerrain::GetHighHeight

/**
 *
 */
float CW3DTerrain::GetLowHeight( int nX, int nY )
{
	if( nX <= 0 )	nX = 0;
	if( nY <= 0 )	nY = 0;
	nX = nX % m_nObjectSize;
	nY = nY % m_nObjectSize;

	float fLow = 0.0F;
	for( int i = 0 ; i < 2 ; i++ )
	{
		for( int j = 0 ; j < 2 ; j++ )
		{
			float fHeight = m_pfHeight[( ( nY + j ) % m_nSize ) * m_nSize + ( ( nX + j ) % m_nSize )];		

			if( fLow >= fHeight )
			{
				fLow = fHeight;
			} //if
		} //for
	} //for
	
	return fLow;
} //CW3DTerrain::GetHighHeight

/**
 *
 */
void CW3DTerrain::AddPass( const SMultiPass* pPass )
{
	IW3DBlock* pBlock = MAKE_COMPONENT( Block );

	if( pBlock )
	{
		if( !pBlock->Init( pPass, this ) )
		{
			SAFE_RELEASE( pBlock );
			return ;
		} //if
	} //if

	m_pBlockList->Add( pBlock );
	m_listBlock.push_back( pBlock );
} //CW3DTerrain::AddPass

/**
 *
 */
BOOL CW3DTerrain::LoadTexture( const char* szTexture )
{
	SAFE_RELEASE( m_pTexture );

		// Texture Load
	m_pTexture = g_pTextureMgr->LoadTexture( "Texture/Terrain/", szTexture );

	return TRUE;
} //

