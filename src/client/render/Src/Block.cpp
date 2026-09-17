/**
 * Block ( Terrain )
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-05-10 7:32p $
 * @version	$Revision: 26 $
 *
 * <HR>
 * $Log: /Render/Src/Block.cpp $
 * 
 * 26    04-05-10 7:32p Kjmgo
 * Mix Vertex Processing 적용
 * 
 * 25    04-02-02 11:43a Kjmgo
 * 밉맵 적용
 * 
 * 24    04-01-13 9:43p Kjmgo
 * 퀄리티 설정 적용.
 * 
 * 23    04-01-08 8:50a Kjmgo
 * 코드 정리
 * 
 * 22    03-12-03 8:03p Kjmgo
 * 버그 수정
 * 
 * 21    03-12-03 5:43p Kjmgo
 * Block은 Object Height 영향을 안받도록 한다.
 * <HR>
 *
 * @file	Block.cpp
 */

#include "Render_Export.h"
#include "Block.h"

#define TILE_SIZE 4

/**
 *
 */
CBlockList::CBlockList()
:	m_apBlock		( NULL ),
	m_nQuality		( 0 ),
	m_wMax			( 70 )
{
	//-- NULL --//
} //CBlockList::CBlockList

/**
 *
 */
CBlockList::~CBlockList()
{
	for( int i = 0 ; i < m_nSize * m_nSize ; i++ )
	{
		BLOCK* pBlock = &m_apBlock[i];
		pBlock->m_listBlock.clear();
	} //for

	SAFE_DELETE_ARRAY( m_apBlock );
} //CBlockList::~CBlockList

/**
 *	블럭 리스트를 초기화
 */
BOOL CBlockList::Init( const int nSize, const int nBlockSize, IW3DTerrain* pTerrain )
{
	Assert( nSize > 0 && nBlockSize > 0 );
	Assert( pTerrain );

	m_nSize = nSize /  nBlockSize;		// 블럭이 총 들어 갈수 있는 크기를 구한다.
	m_nBlockSize = nBlockSize;			// 

	if( m_nSize <= 0 )
	{
		m_nSize = 1;
		m_nBlockSize = nSize;
	} //if	

	m_pTerrain = pTerrain;
	// 정사각형으로, 2차원으로 잡는다.
	m_apBlock = new BLOCK[ m_nSize * m_nSize ];
	
	//CHECK_PTR_RETURN( m_apBlock );
	//_ZeroMemory( m_apBlock, sizeof( BLOCK ) * m_nSize * m_nSize );

	//(BYTE*)_MemAlloc( sizeof( BYTE ) *
	
	// 블럭 크기의 반을 구한다.
	float fHalfSize = m_nBlockSize / 2.0F;
	
	// 총 Block을 설정한다.
	for( int i = 0 ; i < m_nSize ; i++ )
	{
		for( int j = 0 ; j < m_nSize ; j++ )
		{
			BLOCK* pBlock = &m_apBlock[ ( i * m_nSize ) + j ];

			// Block List Clear
			pBlock->m_listBlock.clear();

			// 가운데 점을 구한다.
			pBlock->vPos.x = (float)( j * m_nBlockSize + fHalfSize );
			pBlock->vPos.y = (float)( i * m_nBlockSize + fHalfSize );

			// 높이를 구한다.
			pBlock->vPos.z = pTerrain->GetHeight( pBlock->vPos.x, pBlock->vPos.y );
		} //for
	} //for
	return TRUE;
} //CBlockList::CBlockList

/**
 *	블럭을 추가
 */
BOOL CBlockList::Add( IW3DBlock* pBlock )
{
	Assert( pBlock );
	
	// 한개의 블럭의 사이즈로, 현재 위치하고 있는 블럭의
	// 위치를 추적한다.
	WORD nX = pBlock->GetPosition().x / m_nBlockSize;
	WORD nY = pBlock->GetPosition().y / m_nBlockSize;

	if( nX >= m_nSize || nY >= m_nSize )
	{
		Assert( !"CBlockList::Add - 좌표 오류입니다. 존재 하지 않습니다." );
		return FALSE;
	} //if

	// 배열 번호가 올바른지 검사한다.
	int nArrayId = ( nY * m_nSize ) + nX;
	Assert( nArrayId >= 0 );
	Assert( nArrayId < m_nSize * m_nSize );

	// 리스트에 블럭을 추가한다.
	{
		BLOCK* pList = &m_apBlock[ nArrayId ];
		pList->nCount++;
		pList->m_listBlock.push_back( pBlock );
	} // 리스트에 블럭을 추가한다.

	return TRUE;
} //CBlockList::CBlockList

/**
 *
 */
void CBlockList::Draw( int nX, int nY )
{
	Assert( nX >= 0 && nY >= 0 );
	Assert( m_nBlockSize > 0 );
	
	if( !m_pTerrain )	return ;

	m_nQuality = m_pTerrain->GetQuality();

	// 역시나, 블럭의 위치를 구한다.
	nX = nX / m_nBlockSize;
	nY = nY / m_nBlockSize;

	int nMinerX = MIN( nX, 4 - m_nQuality );				// 최소 영역 설정
	int nMinerY = MIN( nY, 4 - m_nQuality );				// 최소 영역 설정
	int nPlusX	= MIN( nX + 4 - m_nQuality, m_nSize );	// 최대 영역 설정
	int nPlusY	= MIN( nY + 4 - m_nQuality, m_nSize );	// 최대 영역 설정
	
	// RenderState 설정
	SetRenderState();

	// 화면에 일정 갯수 이상 찍지 않기 위해 제한을 건다.
	int	nViewCount = 0;

	for( int i = nY - nMinerY ; i < nPlusY; i++ )
	{
		for( int j = nX - nMinerX ; j < nPlusX; j++ )
		{
			BLOCK* pList = NULL;
			
#ifdef _DEBUG
			// 배열이 올바른지 검사한다.
			int nArrayId = i * m_nSize + j;
			Assert( nArrayId >= 0 );
			Assert( nArrayId < m_nSize * m_nSize );
			pList = &m_apBlock[nArrayId];		
#else
			pList = &m_apBlock[ i * m_nSize + j ];
#endif //#ifdef _DEBUG
			Assert( pList );
			
			// 리스트에 블럭이 없거나, 보이지 않는 것일때
			if( pList->nCount <= 0 ||
				!g_Frustum.CheckSphere( pList->vPos, m_nBlockSize )	)
			{
				// 다음 블럭으로 간다.
				continue;
			} //if			

			// 리스트의 블럭을 그린다.
			pList->Draw( nViewCount );

			if( nViewCount >= m_wMax )
			{
				// 70개 이상의 멀티 패스를 찍었다면, 
				// 더이상 찍지 아니한다.
				break;
			} //if
		} //for
	} //for

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER,	D3DTEXF_NONE );
	D3D::SetSoftVertexProcessing( FALSE );
} //CBlockList::Draw

/**
 *
 */
void CBlockList::SetRenderState()
{
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );

	float fBias = 0.2F;
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER,	D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fBias)) );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPFILTER,	D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fBias)) );


	///////////////////////////////////////////////////////////////////////////////

    g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZFUNC,			D3DCMP_LESSEQUAL );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );
	///////////////////////////////////////////////////////////////////////////////

	D3D::Lighting( TRUE );
	D3D::FogEnable( TRUE );
	D3D::SetSoftVertexProcessing( TRUE );
} //CBlockList::SetRenderState

/////////////////////////////////////////////////////
// Block 

IMPLEMENT_COMPONENT( CBlock, Block );

/**
 *
 */
CBlock::CBlock()
:	m_nVertex	( 0 ),
	m_pTerrain	( NULL ),
	m_nRot		( 0 ),
	m_nFaces	( 0 )
{
	for( int i = 0 ; i < MAXLAYER ; i++ )
	{
		m_pTexture[i] = NULL;
	} //for
} //CBlock::CBlock

/**
 *
 */
CBlock::~CBlock()
{
	for( int i = 0 ; i < MAXLAYER ; i++ )
	{
		SAFE_RELEASE( m_pTexture[i] );
	} //for
} //CBlock::~CBlock

/**
 *
 */
BOOL CBlock::Init( const SMultiPass* pPass, IW3DTerrain* pTerrain )
{
	Assert( pPass );
	Assert( pTerrain );

	if( !pPass || !pTerrain )
	{
		return FALSE;
	} //if

	m_pTerrain = pTerrain;

	// 멀티 패스 정보를 메모리 복사한다.
	m_MultiPass = *pPass;

	// 방향을 구한다. ( 8방향으로 )
	m_nRot		= m_MultiPass.btDirection * -90;

	// TILE_SIZE에 딱 떨더지도록 좌표를 수정한다.
	m_MultiPass.x = (int)( m_MultiPass.x / TILE_SIZE ) * TILE_SIZE;
	m_MultiPass.y = (int)( m_MultiPass.y / TILE_SIZE ) * TILE_SIZE;
	m_vPos.x = m_MultiPass.x;
	m_vPos.y = m_MultiPass.y;

	// 
	int nSize = m_MultiPass.wSize;

	// 높이 버퍼를 생성한다.	
	m_pTexture[0] = g_pTextureMgr->LoadTexture( "Texture/Terrain/", pPass->szTexture );

	//-=-=-=-=-=-=- Load Alpha Texture -=-=-=-=-=-=-//
	if( strlen( pPass->szAlphaTexture ) > 3 )
	{
		m_pTexture[1] = g_pTextureMgr->LoadTexture( "Texture/Terrain/", pPass->szAlphaTexture );
	} //if

	if( !m_pTexture[0] || !m_pTexture[1] )
	{
		SAFE_RELEASE( m_pTexture[0] );
		SAFE_RELEASE( m_pTexture[1] );
   		Assert( !"Block : LoadTexture Filed" );
		DEBUG_OUT( "Block : LoadTexture Filed" );
		return FALSE;
	} //if
	
	Make();

	m_matTexture.SetIdentity();
	m_matTexture.SetRotationZ( ANGLE( m_nRot ) );

	return TRUE;
} //CBlock::Init

/**
 *
 */
BOOL CBlock::IsInRect(	
		const float fStartX,	///< 시작 위치
		const float fStartY,	///< 시작 위치
		const float fSize		///< 크기
		)
{
	// 영역 안에 들어 가는지를 검사.
	if( m_MultiPass.x >= fStartX && m_MultiPass.x < fStartX + fSize &&
		m_MultiPass.y >= fStartY && m_MultiPass.y < fStartY + fSize )
	{
		return TRUE;
	} //if

 	return FALSE;
} //CBlock::IsInRect

/**
 *
 */
void CBlock::Make()
{
	Assert( m_pTerrain );

	float* pfHeight = m_pTerrain->GetHeightMap();

	//
	int nHalf = m_MultiPass.wSize / 4.0F;
	int nAdd = 0;

	// 블럭의 크기가 4로 나누어 떨어지지 않을때, 
	if( ( m_MultiPass.wSize % 4 ) != 0 )
	{
		// 4,6,8,10,12중, 6, 10 일때 적용된다.
		// 이럴땐, 각각 8, 12일때처럼 한번씩 더 연산해야 한다.
		nAdd = 1;
	} //if

	int nSize = m_pTerrain->GetSize();

	// 텍스처 좌표 ( U, V ) 가중치
	float fTexture = 1.0F / (float)( m_MultiPass.wSize / 2);

	// 레이어 속성을 사용하지 않는다.
	// 지형에만 딱 달라 붙게 된다.
	m_pTerrain->SetUseObjectHeight( FALSE );

	VERTEX	vP[4];
	Vector3 v( m_MultiPass.x, m_MultiPass.y, 0.0F );
		
	int nTextureX = 0;
	int nTextureY = 0;

	// 타일 사이즈의 제곱을 미리 계산한다.
//	WORD wDoubleTileSize = TILE_SIZE * TILE_SIZE;

	for( int i = nHalf * -1 ; i < nHalf + nAdd; i++, nTextureY++ )
	{
		for( int j = nHalf * -1 ; j < nHalf + nAdd; j++, nTextureX++ )
		{
			// 각 점의 Index를 구한다.
			int nIdxX = ( v.x / TILE_SIZE ) + j;
			int nIdxY = ( v.y / TILE_SIZE ) + i;

			// 실제 지형의 좌표를 구한다.
			int nX = ( v.x + ( j * TILE_SIZE ) + 0.5F ) / TILE_SIZE * TILE_SIZE;
			int nY = ( v.y + ( i * TILE_SIZE ) + 0.5F ) / TILE_SIZE * TILE_SIZE;

			int nCount = 0;
			for( int k = 0 ; k < 2 ; k++ )
			{
				for( int l = 0 ; l < 2 ; l++ )
				{
					float fX = (float)( nX + ( l * TILE_SIZE ) ) / TILE_SIZE * TILE_SIZE;
					float fY = (float)( nY + ( k * TILE_SIZE ) ) / TILE_SIZE * TILE_SIZE;
					vP[nCount].p.x = fX;
					vP[nCount].p.y = fY;

					if( nX + l < 0 || nY + k < 0 )
					{
						vP[nCount].p.z = 0.0F;
					}
					else
					{
						vP[nCount].p.z = m_pTerrain->GetHeight( fX , fY ) + 0.02F;
					} //if..else..

					vP[nCount].u = (float)( fTexture * nTextureX ) + ( l * fTexture );
					vP[nCount].v = (float)( fTexture * nTextureY ) + ( k * fTexture );
					nCount++;
				} //for
			} //for
			
			if( ( nIdxX + nIdxY ) % 2 == 0 )
			{
				InsertVertex( vP[0] );
				InsertVertex( vP[1] );
				InsertVertex( vP[2] );

				InsertVertex( vP[2] );
				InsertVertex( vP[1] );
				InsertVertex( vP[3] );
			}
			else
			{
				InsertVertex( vP[0] );
				InsertVertex( vP[1] );
				InsertVertex( vP[3] );

				InsertVertex( vP[0] );
				InsertVertex( vP[3] );
				InsertVertex( vP[2] );
			} //if..else..
		} //for
	} //for

	// 레이어 속성을 다시 설정한다.
	m_pTerrain->SetUseObjectHeight();

	// 중심점중 높이를 설정한다.
	m_vPos.z /= ( m_MultiPass.wSize * m_MultiPass.wSize * 6 );
} //CBlock::Make

/**
 *
 */
void CBlock::InsertVertex( VERTEX& vP )
{
	Assert( m_nFaces < MAXPOLYGON );

	if( m_nFaces < MAXPOLYGON )
	{
		m_vPos.z += vP.p.z;
		m_vTerrain[m_nVertex] = vP;

		m_nVertex++;
		m_nFaces = m_nVertex / 3;
	}
	else
	{
		m_nFaces = MAXPOLYGON - 1;
	}//if

} //CBlock::InsertVertex

/**
 *
 */
void CBlock::Draw()
{
	Assert( m_pTexture[0] );

	m_pTexture[0]->SetTexture( 0 );

	if( m_pTexture[1] )
	{
		m_pTexture[1]->SetTexture( 1 );
	}
	else
	{
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}//if

	////////////////////////////

	g_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX*)&m_matTexture );
	g_pd3dDevice->SetTransform( D3DTS_TEXTURE1, (D3DMATRIX*)&m_matTexture );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

	//////////////////////////////
	static Matrix4		matWorld;
	matWorld.SetIdentity();

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );

	g_pd3dDevice->SetVertexShader( VERTEX::FVF );
	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, m_nFaces, m_vTerrain, sizeof( VERTEX ) );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	
	{
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	} //if

	g_pd3dDevice->SetTexture( 1, NULL );
} //CBlock::Draw

/**
*
*/
BOOL CBlock::Pick( const int nX, const int nY )
{
	int nHalf = m_MultiPass.wSize / 2;
	int nAdd = 0;

	if( m_MultiPass.wSize % 4 )
	{
		nAdd = 1;
	} //if

	if( nX >= m_vPos.x - nHalf && nY >= m_vPos.y - nHalf )
	{
		if( nX <= m_vPos.x + nHalf + nAdd && nY <= m_vPos.y + nHalf + nAdd )
		{
			return TRUE;
		} //if
	} //if
	return FALSE;
} //CBlock::Pick

 /**
 *
 */
void CBlock::Move( const int nX, const int nY )
{
	m_nVertex = 0;
	m_nFaces = 0;

	m_MultiPass.x = (int)( nX / TILE_SIZE ) * TILE_SIZE;
	m_MultiPass.y = (int)( nY / TILE_SIZE ) * TILE_SIZE;

	m_vPos.x = m_MultiPass.x;
	m_vPos.y = m_MultiPass.y;

	if( m_pTerrain )
	{
		Make();
	} //if
} //CBlock::Move

/**
 *
 */
void CBlock::RenderState()
{
	///////////////////////////////////////////////////////////////////////////////
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );

	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_MINFILTER, D3DTEXF_LINEAR );


	///////////////////////////////////////////////////////////////////////////////

    g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );


	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZFUNC,			D3DCMP_LESSEQUAL );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );
	///////////////////////////////////////////////////////////////////////////////

	D3D::Lighting( TRUE );
	D3D::FogEnable( TRUE );

} //CBlock::RenderState