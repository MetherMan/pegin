/**
 * 지형용 그리드 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-10-03 1:15p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Render/Src/TerrainGrid.cpp $
 * 
 * 1     03-10-03 1:15p Kjmgo
 * 
 * <HR>
 *
 * @file	TerrainGrid.cpp
 */

#include "Render_Export.h"
#include "TerrainGrid.h"

/**
 *
 */
CTerrainGrid::CTerrainGrid()
:	m_pTexture			( NULL ),
	m_pVB				( NULL ),
	m_nFaces			( 0 ),
	m_nGrid				( 0 ),
	m_nSize				( 0 )
{
} //CTerrainGrid::CTerrainGrid

/**
 *
 */
CTerrainGrid::~CTerrainGrid()
{
	SAFE_DELETE( m_pVB );
} //CTerrainGrid::~CTerrainGrid

/**
 *
 */
BOOL CTerrainGrid::Init( int nGrid, int nSize )
{
	Assert( nGrid > 0 );
	Assert( nSize > 0 );

	m_nGrid	= nGrid;
	nSize	= nSize;

	// Vertex Buffer
	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );
	CHECK_PTR_RETURN( m_pVB );

	//  
	if( !m_pVB->Create( m_nGrid * m_nGrid * 4 * 3 ) )
	{
		return FALSE;
	} //if

	m_nFaces	= m_nGrid * m_nGrid * 2;

	return TRUE;

} //CTerrainGrid::Init

/**
 *
 */
void CTerrainGrid::SetTexture( CTexture* pTexture )
{
	/*
		CAUTION: 여기서 주의할점은 넘어온 텍스처는 
				여기서 해제 하지 않는 다는 점이다.
	*/
	m_pTexture = pTexture;
} //CTerrainGrid::SetTexture

/**
 *
 */
void CTerrainGrid::Draw()
{
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	

	if( m_pVB )
	{
		m_pVB->Draw( D3DPT_TRIANGLELIST, m_nFaces );
	} //if
} //CTerrainGrid::Draw