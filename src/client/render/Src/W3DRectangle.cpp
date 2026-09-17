/**
 * 2D 사각.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-08-06 2:48p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Render/Src/W3DRectangle.cpp $
 * 
 * 3     03-08-06 2:48p Paran
 * 
 * 2     03-08-04 5:15p Paran
 * W3DRectangle  추가
 * 
 * <HR>
 *
 * @file	W3DRectangle.cpp
 */


#include "Render_Export.h"

/**
 * 2D 사각 클래스.
 * @ingroup		Render
 */
class CW3DRectangle : public IW3DRectangle
{
public:
	UNDER_MEMORY_MGR( CW3DRectangle );

	/**
	 * 2D 사각을 그린다.
	 */
	void	Draw( const int nX, const int nY, const int nWidth, const int nHeight, const DWORD dwColor )
	{
		VERTEX	Vertices[4];
		SetVertices( Vertices, nX, nY, nWidth, nHeight, dwColor );

		SetRenderState();
		g_pd3dDevice->SetTexture( 0, NULL );
		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof( VERTEX ) );
	} //Draw

private:
	struct VERTEX
	{
		enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE };
		float	x, y, z;
		float	rhw;
		DWORD	color;
	}; //struct VERTEX

	/**
	 * 버텍스 설정.
	 */
	void	SetVertices(
		VERTEX*		pVertices,		///< [IN] 버텍스 배열
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nWidth,			///< [IN] 가로크기
		const int	nHeight,		///< [IN] 세로크기
		const DWORD	dwColor			///< [IN] 색상
		)
	{
		pVertices[0].x     = nX - 0.5F;
		pVertices[0].y     = nY - 0.5F;
		pVertices[0].z     = 0.9F;
		pVertices[0].rhw   = 1.0F;
		pVertices[0].color = dwColor;

		pVertices[1].x     = nX + nWidth - 0.5F;
		pVertices[1].y     = nY - 0.5F;
		pVertices[1].z     = 0.9F;
		pVertices[1].rhw   = 1.0F;
		pVertices[1].color = dwColor;

		pVertices[2].x     = nX - 0.5F;
		pVertices[2].y     = nY + nHeight - 0.5F;
		pVertices[2].z     = 0.9F;
		pVertices[2].rhw   = 1.0F;
		pVertices[2].color = dwColor;

		pVertices[3].x     = nX + nWidth  - 0.5F;
		pVertices[3].y     = nY + nHeight - 0.5F;
		pVertices[3].z     = 0.9F;
		pVertices[3].rhw   = 1.0F;
		pVertices[3].color = dwColor;
	} //SetVertices

	/**
	 * 렌더링 상태를 설정한다.
	 */
	void	SetRenderState()
	{
		g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	} //SetRenderState

}; //CW3DSprite

IMPLEMENT_COMPONENT( CW3DRectangle, Rectangle );