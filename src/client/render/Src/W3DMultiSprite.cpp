/**
 * 2D 멀티 스프라이트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란
 * @date	$Date: 04-08-03 10:01p $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Render/Src/W3DMultiSprite.cpp $
 * 
 * 5     04-08-03 10:01p Paran
 * 
 * 4     04-05-29 10:06p Paran
 * 
 * 3     04-05-14 3:24p Paran
 * 
 * 2     04-04-29 5:30p Paran
 * MultiSprite 추가
 * 
 * <HR>
 *
 * @file	W3DMultiSprite.cpp
 */


#include "Render_Export.h"
#include "Texture.h"
#include <vector>

#define IDX_BOXEDGE_LT		0
#define IDX_BOXEDGE_CT		1
#define IDX_BOXEDGE_RT		2
#define IDX_BOXEDGE_LC		3
#define IDX_BOXEDGE_RC		4
#define IDX_BOXEDGE_LB		5
#define IDX_BOXEDGE_CB		6
#define IDX_BOXEDGE_RB		7

/**
 * 2D 멀티스프라이트 클래스.
 * @ingroup		Render
 */
class CW3DMultiSprite : public IW3DMultiSprite
{
public:
	UNDER_MEMORY_MGR( CW3DMultiSprite );

	/**
	 * 생성자.
	 */
	CW3DMultiSprite()
	{
		m_dwColor		= 0xFFFFFFFF;
		m_Vertices[0].x	= -0.5F;
		m_Vertices[0].y	= -0.5F;
		m_Vertices[0].z	=  1.0F;
		m_Vertices[1].x	= +0.5F;
		m_Vertices[1].y	= -0.5F;
		m_Vertices[1].z	=  1.0F;
		m_Vertices[2].x	= -0.5F;
		m_Vertices[2].y	= +0.5F;
		m_Vertices[2].z	=  1.0F;
		m_Vertices[3].x	= +0.5F;
		m_Vertices[3].y	= +0.5F;
		m_Vertices[3].z	=  1.0F;

		m_dwVertexShader	= 0 ;
		m_matTexture.SetIdentity();
		m_matMoveScale.SetIdentity();

		DWORD			dwDeclaration[MAX_FVF_DECL_SIZE];
		if( FAILED( D3DXDeclaratorFromFVF( VERTEX::FVF, dwDeclaration ) ) )
		{
			Assert( !"Failed, The Declaration" );
			return;
		} //if

		static const char s_cVertexShaderSource[] = "\
			vs.1.1								\
			mul  r0.xy, v0.xy, c0.zw			\
			mov  r0.zw, c2.zw					\
			dp4  r1.x, r0, c4					\
			dp4  r1.y, r0, c5					\
			add  r1.xy, r1.xy, c0.xy			\
			mov  r1.zw, c2.zw					\
			m4x4 oPos, r1, c8					\
			mad  oT0.xy, v0.xy, c1.zw, c1.xy	\
			";
		
		LPD3DXBUFFER	pCode;
		if( FAILED( D3DXAssembleShader( s_cVertexShaderSource, (UINT)strlen( s_cVertexShaderSource ), 0, NULL, &pCode, NULL ) ) )
		{
			Assert( !"Failed, The Assemble" );
			return;
		} //if

		if( FAILED( g_pd3dDevice->CreateVertexShader( dwDeclaration, (DWORD*)pCode->GetBufferPointer(), &m_dwVertexShader, 0 ) ) )
		{
			Assert( !"Failed, Create The VertexShader" );
			SAFE_RELEASE( pCode );
			return;
		} //if
		SAFE_RELEASE( pCode );

		D3DXMATRIX		projection;
		D3DXMatrixOrthoOffCenterLH( &projection, 0, 800, 600, 0, 0, 1 );		
		D3DXMATRIX offset;
		D3DXMatrixTranslation( &offset, -0.5, -0.5, 0 );
		projection = offset * projection;
		//g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &projection );
		D3DXMatrixTranspose( &projection, &projection );
	
		g_pd3dDevice->SetVertexShaderConstant(  8, (float*)&projection,  4 );

		D3DXMATRIX		identity;
		D3DXMatrixIdentity( &identity );
		g_pd3dDevice->SetVertexShaderConstant( 4, (float*)&identity, 4 );

		float			afConst_2[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
		g_pd3dDevice->SetVertexShaderConstant( 2, (float*)afConst_2, 1 );
	} //CW3DMultiSprite

	/**
	 * 소멸자.
	 */
	~CW3DMultiSprite()
	{
		for( UINT nCnt = 0; nCnt < m_vtTextureEx.size(); nCnt++ )
		{
			SAFE_RELEASE( m_vtTextureEx[nCnt]->pTexture );
			SAFE_FREE( m_vtTextureEx[nCnt] );
		} //for

		g_pd3dDevice->DeleteVertexShader( m_dwVertexShader );
	} //~CW3DMultiSprite

	/**
	 * 스프라이트 텍스처를 읽고 및 가로 세로 길이를 설정한다.
	 */
	int		AddSprite(
		const char*		szFileName		///< [IN] 텍스처 파일명
		)
	{
		Assert( g_pd3dDevice );

		CTexture*		pTexture = g_pTextureMgr->LoadTexture( "", szFileName, CTextureMgr::SPRITE );
		if( !pTexture )
		{
			Assert( !"Load Not File" );
			return -1;
		} //if

		STextureEx*			psTextureEx = (STextureEx*)_MemAlloc( sizeof( STextureEx ) );
		if( !psTextureEx )
		{
			Assert( !"Allocate Not Memory" );
			return -1;
		} //if

		D3DSURFACE_DESC		desc;
		HRESULT		hr;
		hr = pTexture->GetD3dTexture()->GetLevelDesc( 0, &desc );
		if( FAILED( hr ) )
		{
			Assert( !"텍스처 정보 얻기 실패" );
			SAFE_FREE( psTextureEx );
			return -1;
		} //if

		psTextureEx->pTexture = pTexture;
		psTextureEx->nWidth   = desc.Width;
		psTextureEx->nHeight  = desc.Height;

		m_vtTextureEx.push_back( psTextureEx );
		UINT	nIndex = m_vtTextureEx.size() - 1;
		if( nIndex < 0 )
		{
			Assert( !"Wrong Size" );
			return -1;
		} //if

		return nIndex;
	} //Load

	/**
	 * 스프라이트 색상 설정.
	 */
	void	SetColor( const DWORD dwColor )
	{
		m_dwColor = dwColor;
	} //SetColor

	/**
	 * 2D 스프라이트를 그린다.
	 */
	void	Draw( 
		const int	nIndex,				///< [IN] Index of Texture
		const int	nX,					///< [IN] Drawing coord X
		const int	nY					///< [IN] Drawing coord Y
	)
	{
		if( m_vtTextureEx.empty() ||
			m_vtTextureEx.size() <= nIndex ||
			nIndex < 0 )
		{
			Assert( !"Wrong the Image Index" );
			return;
		} //if

		STextureEx*		pTextureEx = m_vtTextureEx[nIndex];
		Assert( pTextureEx );

		CTexture*		pTexture = pTextureEx->pTexture;
		if( !pTexture )
		{
			Assert( !"Empty the Texture" );
			return;
		} //if

		int		nTexWidth	= m_vtTextureEx[nIndex]->nWidth;
		int		nTexHeight	= m_vtTextureEx[nIndex]->nHeight;

//		m_matMoveScale._11 = (float)nTexWidth;
//		m_matMoveScale._22 = (float)nTexHeight;
//		m_matMoveScale._41 = (float)nX;  
//		m_matMoveScale._42 = (float)nY;
 
		pTexture->SetTexture( 0 );
		SetVertices( nX, nY, nTexWidth, nTexHeight, 0.0F, 0.0F, 1.0F, 1.0F, m_dwColor );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof( VERTEX ) );
	} //

	/**
	 * 2D 패턴스프라이트를 그린다.
	 */
	void	DrawPettern( 
		const int	nIndex,				///< [IN] Index of Texture
		const int	nX,					///< [IN] Drawing coord X
		const int	nY,					///< [IN] Drawing coord Y
		const int	nWidth,				///< [IN] Number X Couple Texture
		const int	nHeight				///< [IN] Number Y Couple Texture
	)
	{
		if( m_vtTextureEx.empty() ||
			m_vtTextureEx.size() <= nIndex ||
			nIndex < 0 )
		{
			Assert( !"Wrong the Image Index" );
			return;
		} //if

		STextureEx*		pTextureEx = m_vtTextureEx[nIndex];
		Assert( pTextureEx );

		CTexture*		pTexture = pTextureEx->pTexture;
		if( !pTexture )
		{
			Assert( !"Empty the Texture" );
			return;
		} //if

		pTexture->SetTexture( 0 );

		int		nDrawWidth	= pTextureEx->nWidth;
		int		nDrawHeight	= pTextureEx->nHeight;
		int		nDrawX		= nX;
		int		nDrawY		= nY;
		int		nNumX		= nWidth / nDrawWidth;
		int		nNumY		= nHeight / nDrawHeight;
		float	fRemainW	= (float)( nWidth % pTextureEx->nWidth ) / nDrawWidth;
		float	fRemainH	= (float)( nHeight % pTextureEx->nHeight ) / nDrawHeight;

		for( int nCntY = 0; nCntY < nNumY; nCntY++ )
		{
			nDrawX		= nX;
			for( int nCntX = 0; nCntX < nNumX; nCntX++ )
			{
				SetVertices( nDrawX, nDrawY, nDrawWidth, nDrawHeight, 0.0F, 0.0F, 1.0F, 1.0F, m_dwColor );
				g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof( VERTEX ) );
				nDrawX += nDrawWidth;
			} //for

			if( fRemainW > 0.0F )
			{ //Remain Width
				float		fRemainDrawW = nWidth % pTextureEx->nWidth;
				SetVertices( nDrawX, nDrawY, fRemainDrawW, nDrawHeight, 0.0F, 0.0F, fRemainW, 1.0F, m_dwColor );
				g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof( VERTEX ) );
			} //if

			nDrawY += nDrawHeight;
		} //for

		
		if( fRemainH > 0.0F )
		{ //Remain Height
			nDrawX		= nX;
			float		fRemainDrawH = nHeight % pTextureEx->nHeight;
			for( int nCntX = 0; nCntX < nNumX; nCntX++ )
			{
				SetVertices( nDrawX, nDrawY, nDrawWidth, fRemainDrawH, 0.0F, 0.0F, 1.0F, fRemainH, m_dwColor );
				g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof( VERTEX ) );
				nDrawX += nDrawWidth;
			} //for
		} //if
	} //DrawPattern
	
	/**
	 * 2D 조합 스프라이트를 그린다.
	 */
	void	Draw( const int	nX,					///< [IN] Drawing coord X
				  const int	nY,					///< [IN] Drawing coord Y
				  const int	nWidth,				///< [IN] Size X
				  const int	nHeight,			///< [IN] Size Y
				  const int	nBoxColor			///< [IN] Color Box
	)			
	{
		g_pd3dDevice->SetVertexShader( m_dwVertexShader );

		SetRenderState();
//		g_pd3dDevice->SetVertexShader( VERTEX::FVF );

		STextureEx*		pTextureEx;
		int				nDrawX, nDrawY;
		
		nDrawX = nX;
		nDrawY = nY;
		Draw( IDX_BOXEDGE_LT, nDrawX, nDrawY );

		pTextureEx = m_vtTextureEx[IDX_BOXEDGE_RT];
		Assert( pTextureEx );
		nDrawX = nX + nWidth - pTextureEx->nWidth;
		nDrawY = nY;
		Draw( IDX_BOXEDGE_RT, nDrawX, nDrawY );

		pTextureEx = m_vtTextureEx[IDX_BOXEDGE_LB];
		Assert( pTextureEx );
		nDrawX = nX;
		nDrawY = nY + nHeight - pTextureEx->nHeight;
		Draw( IDX_BOXEDGE_LB, nDrawX, nDrawY );

		pTextureEx = m_vtTextureEx[IDX_BOXEDGE_RB];
		Assert( pTextureEx );
		nDrawX = nX + nWidth - pTextureEx->nWidth;
		nDrawY = nY + nHeight - pTextureEx->nHeight;
		Draw( IDX_BOXEDGE_RB, nDrawX, nDrawY );

		nDrawX = nX + pTextureEx->nWidth;
		nDrawY = nY;
		DrawPettern( IDX_BOXEDGE_CT, nDrawX, nDrawY, nWidth - ( pTextureEx->nWidth << 1 ), pTextureEx->nHeight );

		nDrawX = nX;
		nDrawY = nY + pTextureEx->nHeight;
		DrawPettern( IDX_BOXEDGE_LC, nDrawX, nDrawY, pTextureEx->nWidth, nHeight - ( pTextureEx->nHeight << 1 ) );

		nDrawX = nX + ( nWidth - pTextureEx->nWidth );
		nDrawY = nY + pTextureEx->nHeight;
		DrawPettern( IDX_BOXEDGE_RC, nDrawX, nDrawY, pTextureEx->nWidth, nHeight - ( pTextureEx->nHeight << 1 ) );

		nDrawX = nX + pTextureEx->nWidth;
		nDrawY = nY + ( nHeight - pTextureEx->nHeight );
		DrawPettern( IDX_BOXEDGE_CB, nDrawX, nDrawY, nWidth - ( pTextureEx->nWidth << 1 ), pTextureEx->nHeight );

		nDrawX = nX + pTextureEx->nWidth;
		nDrawY = nY + pTextureEx->nHeight;
		SetVertices( 
			nDrawX, nDrawY, nWidth - ( pTextureEx->nWidth << 1 ), nHeight - ( pTextureEx->nHeight << 1 ),
			0.0F, 0.0F, 1.0F, 1.0F,
			nBoxColor 
		);

		g_pd3dDevice->SetTexture( 0, NULL );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_Vertices, sizeof( VERTEX ) );

		D3DXMATRIX identity;
		D3DXMatrixIdentity( &identity );
		g_pd3dDevice->SetVertexShaderConstant(  4, (float*)&identity,  4 );
	} //Draw

private:
	/**
	 * 버텍스 설정.
	 */
	void	SetVertices(
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nWidth,			///< [IN] 가로 길이
		const int	nHeight,		///< [IN] 세로 길이
		const float	fTexX,			///< [IN] 출력할 부분의 X 시작 좌표
		const float	fTexY,			///< [IN] 출력할 부분의 Y 시작 좌표
		const float	fTexWidth,		///< [IN] 출력할 부분의 가로 길이
		const float	fTexHeight,		///< [IN] 출력할 부분의 세로 길이
		const DWORD dwColor			///< [IN] 색상
		)
	{
		float		fConst01[8] = 
		{
			(float)nX+nWidth*0.5F,		 (float)nY+nHeight*0.5F,	   (float)nWidth,	 (float)nHeight,
			(float)fTexX+fTexWidth*0.5F, (float)fTexY+fTexHeight*0.5F, (float)fTexWidth, (float)fTexHeight
		}; //fConst01
		g_pd3dDevice->SetVertexShaderConstant( 0, fConst01, 2 );
		return;
/*
		m_Vertices[0].x     = nX;
		m_Vertices[0].y     = nY;
		m_Vertices[0].z     = 0.9F;
		m_Vertices[0].color = dwColor;

		m_Vertices[1].x     = nX + nSrcWidth;
		m_Vertices[1].y     = nY;
		m_Vertices[1].z     = 0.9F;
		m_Vertices[1].color = dwColor;

		m_Vertices[2].x     = nX;
		m_Vertices[2].y     = nY + nSrcHeight;
		m_Vertices[2].z     = 0.9F;
		m_Vertices[2].color = dwColor;

		m_Vertices[3].x     = nX + nSrcWidth;
		m_Vertices[3].y     = nY + nSrcHeight;
		m_Vertices[3].z     = 0.9F;
		m_Vertices[3].color = dwColor;

		m_Vertices[0].tu    = (float)nSrcX / nWidth;
		m_Vertices[0].tv    = (float)nSrcY / nHeight;
		m_Vertices[1].tu    = (float)( nSrcX + nSrcWidth  ) / nWidth;
		m_Vertices[1].tv    = (float)nSrcY                  / nHeight;
		m_Vertices[2].tu    = (float)nSrcX                  / nWidth;
		m_Vertices[2].tv    = (float)( nSrcY + nSrcHeight ) / nHeight;
		m_Vertices[3].tu    = (float)( nSrcX + nSrcWidth  ) / nWidth;
		m_Vertices[3].tv    = (float)( nSrcY + nSrcHeight ) / nHeight;
*/
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

		// Texture 관련 세팅
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE ); 

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );

		g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	} //SetRenderState

private:
	struct VERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
		float	x, y, z;
	}; //struct VERTEX

	struct STextureEx
	{
		CTexture*			pTexture;		///< 텍스처
		int					nWidth;			///< 가로크기
		int					nHeight;		///< 가로크기
	}; //struct STextureEx
	typedef std::vector<STextureEx*>		VEC_TEXTUREEX;

	VEC_TEXTUREEX		m_vtTextureEx;
	DWORD				m_dwColor;			///< 컬러
	int					m_nMode;			///< 모드
	VERTEX				m_Vertices[4];
	Matrix4				m_matTexture;
	Matrix4				m_matMoveScale;
	DWORD				m_dwVertexShader;
}; //class CW3DMultiSprite

IMPLEMENT_COMPONENT( CW3DMultiSprite, MultiSprite );