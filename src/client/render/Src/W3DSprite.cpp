/**
 * 2D 스프라이트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-09-10 11:30p $
 * @version	$Revision: 44 $
 *
 * <HR>
 * $Log: /Render/Src/W3DSprite.cpp $
 * 
 * 44    04-09-10 11:30p Paran
 * 
 * 43    04-03-25 5:20p Kjmgo
 * 
 * 42    04-03-25 4:33p Kjmgo
 * 
 * 41    04-01-15 4:33p Kjmgo
 * Texture없을시 예외처리 추가
 * 
 * 40    03-11-17 10:44a Kjmgo
 * Draw시 마다 4개의 버텍스를 생성하던것을 Static으로 뺐음
 * 
 * 39    03-11-13 5:46p Paran
 * 
 * 38    03-10-06 5:02p Paran
 * 
 * 37    03-10-02 6:52p Paran
 * 
 * 36    03-09-30 5:36p Paran
 * 
 * 35    03-09-24 8:28p Paran
 * 
 * 34    03-09-23 2:53a Paran
 * 
 * 33    03-09-22 11:54a Kjmgo
 * 
 * 32    03-09-09 7:22p Paran
 * 
 * 31    03-07-03 6:15p Paran
 * 
 * 30    03-05-30 12:30a Paran
 * 텍스쳐 생성 매니져로...
 * 
 * 29    03-05-24 5:18a Ahastudio
 *
 * 26    03-05-07 5:19a Ahastudio	텍스처 로딩 및 UV 값 지정하는 부분을 수정. 더욱 깨끗한 스프라이트 출력 가능.
 * <HR>
 *
 * @file	W3DSprite.cpp
 */


#include "Render_Export.h"
#include "Texture.h"



/**
 * 2D 스프라이트 클래스.
 * @ingroup		Render
 */
class CW3DSprite : public IW3DSprite
{
public:
	UNDER_MEMORY_MGR( CW3DSprite );

	/**
	 * 생성자.
	 */
	CW3DSprite()
	:	m_pTexture	( NULL ),
		m_dwSizeX	( 0 ),
		m_dwSizeY	( 0 ),
		m_nWidth	( 0 ),
		m_nHeight	( 0 ),
		m_nCenterX	( 0 ),
		m_nCenterY	( 0 ),
		m_nCol		( 0 ),
		m_nRow		( 0 ),
		m_dwDelay	( 0 ),
		m_nAddX		( 0 ),
		m_nAddY		( 0 ),
		m_nAniX		( 0 ),
		m_bAnimation( FALSE ),
		m_nAniY		( 0 ),
		m_nFrame	( 0 ),
		m_nTotalFrame( 0 ),
		m_dwColor	( 0xFFFFFFFF )
	{
	} //CW3DSprite

	/**
	 * 소멸자.
	 */
	~CW3DSprite()
	{
		SAFE_RELEASE( m_pTexture );
	} //~CW3DSprite

	/**
	 * 스프라이트 텍스처를 읽고 및 가로 세로 길이를 설정한다.
	 */
	BOOL	Load(
		const char*		szFileName,		///< [IN] 텍스처 파일명
		const int		nWidth,			///< [IN] 가로 길이
		const int		nHeight,			///< [IN] 세로 길이
		const BOOL		bAutoSize
		)
	{
		Assert( g_pd3dDevice );

		SAFE_RELEASE( m_pTexture );

		m_pTexture = g_pTextureMgr->LoadTexture(  "", szFileName, CTextureMgr::SPRITE );
		if( !m_pTexture )
		{
			return FALSE;
		} //if

		D3DSURFACE_DESC		desc;
		HRESULT		hr;
		hr = m_pTexture->GetD3dTexture()->GetLevelDesc( 0, &desc );
		if( FAILED( hr ) )
		{
			Assert( !"텍스처 정보 얻기 실패" );
			return FALSE;
		} //if
		
		m_dwSizeX = desc.Width;
		m_dwSizeY = desc.Height;

		if( bAutoSize )
			SetSize( m_dwSizeX, m_dwSizeY );
		else
			SetSize( nWidth, nHeight );

		m_bAnimation = FALSE;
		return TRUE;
	} //Load

	/**
	 * 2D 스프라이트 크기 설정.
	 */
	void	SetSize( const int nWidth, const int nHeight )
	{
		m_nWidth  = nWidth;
		m_nHeight = nHeight;
	} //SetSize

	//--------------------------
	//	Set Position
	//--------------------------
	void	SetZoom( float fZoom )
	{
		if( m_pTexture )
		{
			D3DSURFACE_DESC		desc;
			HRESULT		hr;
			hr = m_pTexture->GetD3dTexture()->GetLevelDesc( 0, &desc );
			if( FAILED( hr ) )
			{
				Assert( !"텍스처 정보 얻기 실패" );
				return ;
			} //if

			SetSize( desc.Width * fZoom, desc.Height * fZoom );
			
			m_nCenterX = m_nWidth / 2;
			m_nCenterY = m_nHeight / 2;		

			m_dwSizeX = MAX( 1, m_nWidth );
			m_dwSizeY = MAX( 1, m_nHeight );
		} //if	
	} //CSprite::SetZoom

	/**
	 * 출력 보정 좌표 설정.
	 */
	void	SetCenter( const int nX, const int nY )
	{
		m_nCenterX = nX;
		m_nCenterY = nY;
	} //SetCenter

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
	void	Draw( const int nX, const int nY )
	{
		if( !m_pTexture )
		{
			return;
		} //if

		int x = nX - m_nCenterX;
		int y = nY - m_nCenterY;

		static VERTEX	Vertices[4];
		SetVertices( Vertices, x, y, 0, 0, m_nWidth, m_nHeight );

		SetRenderState();
		m_pTexture->SetTexture( 0 );
		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof( VERTEX ) );
	} //Draw

	/**
	 *
	 */
	void	DrawEffect(
		const int	nX,
		const int	nY
		)
	{
			if( !m_pTexture )
		{
			return;
		} //if

		int x = nX - m_nCenterX;
		int y = nY - m_nCenterY;

		static VERTEX	Vertices[4];
		SetVertices( Vertices, x, y, 0, 0, m_nWidth, m_nHeight );

		SetRenderState();
		
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);		
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR   );
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE );

		
		g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );



		m_pTexture->SetTexture( 0 );
		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof( VERTEX ) );
	} //DrawEffect

	/**
	 * 2D 스프라이트를 그린다.
	 */
	void	Draw(
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nSrcX,			///< [IN] 출력할 부분의 X 시작 좌표
		const int	nSrcY,			///< [IN] 출력할 부분의 Y 시작 좌표
		const int	nSrcWidth,		///< [IN] 출력할 부분의 가로 길이
		const int	nSrcHeight		///< [IN] 출력할 부분의 세로 길이
		)
	{
		if( !m_pTexture )
			return ;

		int x = nX - m_nCenterX;
		int y = nY - m_nCenterY;

		static VERTEX	Vertices[4];

		SetVertices( Vertices, x, y, nSrcX, nSrcY, nSrcWidth, nSrcHeight );

		SetRenderState();
		
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );

		m_pTexture->SetTexture( 0 );
		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof( VERTEX ) );
	} //Draw

	/**
	 * 2D 스프라이트를 그린다.
	 */
	void	Draw(
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nWidth,			///< [IN] 가로 길이
		const int	nHeight,		///< [IN] 세로 길이
		const int	nSrcX,			///< [IN] 출력할 부분의 X 시작 좌표
		const int	nSrcY,			///< [IN] 출력할 부분의 Y 시작 좌표
		const int	nSrcWidth,		///< [IN] 출력할 부분의 가로 길이
		const int	nSrcHeight		///< [IN] 출력할 부분의 세로 길이
		)
	{
		if( !m_pTexture )
			return ;

		int x = nX - m_nCenterX;
		int y = nY - m_nCenterY;

		static VERTEX	Vertices[4];

		SetVertices( Vertices, x, y, nSrcX, nSrcY, nSrcWidth, nSrcHeight );

		SetRenderState();
		m_pTexture->SetTexture( 0 );
		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof( VERTEX ) );
	} //Draw
	
	/**
	 * 2D 스프라이트를 그린다.
	 */
	void	DrawFrame(
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nFrame			///< [IN] 출력할 프레임
		)
	{
		if( !m_pTexture )
		{
			return;
		} //if

		int x = nX - m_nCenterX;
		int y = nY - m_nCenterY;

		static VERTEX	Vertices[4];
		SetVertices( Vertices, x, y, 0, 0, m_nWidth, m_nHeight, nFrame );

		SetRenderState();
		m_pTexture->SetTexture( 0 );
		g_pd3dDevice->SetVertexShader( VERTEX::FVF );
		g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Vertices, sizeof( VERTEX ) );
	} //

	/**
	 *
	 */
	void	SetAnimation(
		const int	nRow,
		const int	nCol,
		const DWORD	dwDelay
		)
	{
		m_dwDelay		= dwDelay;
		m_nRow			= nRow;
		m_nCol			= nCol;
		m_bAnimation	= TRUE;
	} //SetAnimation

	/**
	 *
	 */
	void Update( const DWORD dwTick )
	{
		m_dwTime += dwTick;
	} //SetFrame

	/**
	 *
	 */
	void Stop()
	{
		m_bAnimation	= FALSE;
	} //Stop

	/**
	 *
	 */
	void Play()
	{
		m_bAnimation	= TRUE;
	} //Play

	int		GetWidth()
	{
		return m_dwSizeX;
	}
	int		GetHeight()
	{
		return m_dwSizeY;
	}

private:
	struct VERTEX
	{
		enum { FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		float	x, y, z;
		float	rhw;
		DWORD	color;
		float	tu, tv;
	}; //struct VERTEX

	/**
	 * 버텍스 설정.
	 */
	void	SetVertices(
		VERTEX*		pVertices,		///< [IN] 버텍스 배열
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nSrcX,			///< [IN] 출력할 부분의 X 시작 좌표
		const int	nSrcY,			///< [IN] 출력할 부분의 Y 시작 좌표
		const int	nSrcWidth,		///< [IN] 출력할 부분의 가로 길이
		const int	nSrcHeight		///< [IN] 출력할 부분의 세로 길이
		)
	{
		Assert( m_dwSizeX );
		Assert( m_dwSizeY );

		pVertices[0].x     = nX - 0.5F;
		pVertices[0].y     = nY - 0.5F;
		pVertices[0].z     = 0.9F;
		pVertices[0].rhw   = 1.0F;
		pVertices[0].color = m_dwColor;

		pVertices[1].x     = nX + nSrcWidth - 0.5F;
		pVertices[1].y     = nY - 0.5F;
		pVertices[1].z     = 0.9F;
		pVertices[1].rhw   = 1.0F;
		pVertices[1].color = m_dwColor;

		pVertices[2].x     = nX - 0.5F;
		pVertices[2].y     = nY + nSrcHeight - 0.5F;
		pVertices[2].z     = 0.9F;
		pVertices[2].rhw   = 1.0F;
		pVertices[2].color = m_dwColor;

		pVertices[3].x     = nX + nSrcWidth  - 0.5F;
		pVertices[3].y     = nY + nSrcHeight - 0.5F;
		pVertices[3].z     = 0.9F;
		pVertices[3].rhw   = 1.0F;
		pVertices[3].color = m_dwColor;

		if( m_bAnimation )
		{
			SetAnimation( pVertices, nSrcX, nSrcY, nSrcWidth, nSrcHeight );
		}
		else
		{
			pVertices[0].tu    = (float)nSrcX / m_dwSizeX;
			pVertices[0].tv    = (float)nSrcY / m_dwSizeY;
			pVertices[1].tu    = (float)( nSrcX + nSrcWidth  ) / m_dwSizeX;
			pVertices[1].tv    = (float)nSrcY                  / m_dwSizeY;
			pVertices[2].tu    = (float)nSrcX                  / m_dwSizeX;
			pVertices[2].tv    = (float)( nSrcY + nSrcHeight ) / m_dwSizeY;
			pVertices[3].tu    = (float)( nSrcX + nSrcWidth  ) / m_dwSizeX;
			pVertices[3].tv    = (float)( nSrcY + nSrcHeight ) / m_dwSizeY;
		}//if
	} //SetVertices

	/**
	 * 버텍스 설정.
	 */
	void	SetVertices(
		VERTEX*		pVertices,		///< [IN] 버텍스 배열
		const int	nX,				///< [IN] X 좌표
		const int	nY,				///< [IN] Y 좌표
		const int	nSrcX,			///< [IN] 출력할 부분의 X 시작 좌표
		const int	nSrcY,			///< [IN] 출력할 부분의 Y 시작 좌표
		const int	nSrcWidth,		///< [IN] 출력할 부분의 가로 길이
		const int	nSrcHeight,		///< [IN] 출력할 부분의 세로 길이
		const int	nFrame			///< [IN] 출력할 프레임
		)
	{
		Assert( m_dwSizeX );
		Assert( m_dwSizeY );

		pVertices[0].x     = nX - 0.5F;
		pVertices[0].y     = nY - 0.5F;
		pVertices[0].z     = 0.9F;
		pVertices[0].rhw   = 1.0F;
		pVertices[0].color = m_dwColor;

		pVertices[1].x     = nX + nSrcWidth - 0.5F;
		pVertices[1].y     = nY - 0.5F;
		pVertices[1].z     = 0.9F;
		pVertices[1].rhw   = 1.0F;
		pVertices[1].color = m_dwColor;

		pVertices[2].x     = nX - 0.5F;
		pVertices[2].y     = nY + nSrcHeight - 0.5F;
		pVertices[2].z     = 0.9F;
		pVertices[2].rhw   = 1.0F;
		pVertices[2].color = m_dwColor;

		pVertices[3].x     = nX + nSrcWidth  - 0.5F;
		pVertices[3].y     = nY + nSrcHeight - 0.5F;
		pVertices[3].z     = 0.9F;
		pVertices[3].rhw   = 1.0F;
		pVertices[3].color = m_dwColor;

		if( m_bAnimation )
		{
			m_nAniX = nFrame % m_nRow;
			m_nAniY = (float)nFrame / (float)m_nRow;
			if( m_nAniY > m_nCol )
			{
				m_nAniY = m_nAniY % m_nCol;
			} //if

			pVertices[0].tu    = (float)( ( m_nWidth * m_nAniX + nSrcX )	    / (float)( m_dwSizeX ) );
			pVertices[0].tv    = (float)( ( m_nHeight * m_nAniY + nSrcY )	    / (float)( m_dwSizeY ) );

			pVertices[1].tu    = (float)( ( m_nWidth * m_nAniX + nSrcWidth )	/ (float)( m_dwSizeX ) );
			pVertices[1].tv    = (float)( ( m_nHeight * m_nAniY + nSrcY )	    / (float)( m_dwSizeY ) );

			pVertices[2].tu    = (float)( ( m_nWidth * m_nAniX + nSrcX )		/ (float)( m_dwSizeX ) );
			pVertices[2].tv    = (float)( ( m_nHeight * m_nAniY + nSrcHeight )	/ (float)( m_dwSizeY ) );

			pVertices[3].tu    = (float)( ( m_nWidth * m_nAniX + nSrcWidth )	/ (float)( m_dwSizeX ) );
			pVertices[3].tv    = (float)( ( m_nHeight * m_nAniY + nSrcHeight )	/ (float)( m_dwSizeY ) );
		}
		else
		{
			pVertices[0].tu    = (float)nSrcX / m_dwSizeX;
			pVertices[0].tv    = (float)nSrcY / m_dwSizeY;
			pVertices[1].tu    = (float)( nSrcX + nSrcWidth  ) / m_dwSizeX;
			pVertices[1].tv    = (float)nSrcY                  / m_dwSizeY;
			pVertices[2].tu    = (float)nSrcX                  / m_dwSizeX;
			pVertices[2].tv    = (float)( nSrcY + nSrcHeight ) / m_dwSizeY;
			pVertices[3].tu    = (float)( nSrcX + nSrcWidth  ) / m_dwSizeX;
			pVertices[3].tv    = (float)( nSrcY + nSrcHeight ) / m_dwSizeY;
		}//if
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

	/**
	 *
	 */
	void	SetAnimation( VERTEX* pVertices, int nSrcX, int nSrcY, int nSrcWidth, int nSrcHeight )
	{
		m_nAddX		= m_nWidth;
		m_nAddY		= m_nHeight;

		if( m_dwDelay <= m_dwTime )
		{
			m_dwTime = 0;
			m_nFrame++;

			m_nAniX++;

			if( m_nAniX >= m_nRow )
			{
				m_nAniY++;
				m_nAniX = 0;

				if( m_nAniY >= m_nCol )
				{
					m_nAniY = 0;
				} //if
			} //if
		} //if

		if( m_nRow < 2 || m_nCol < 2 )
		{
			UVAnimation( pVertices, nSrcX, nSrcY, nSrcWidth, nSrcHeight );
		}
		else
		{
			SpriteAnimation( pVertices, nSrcX, nSrcY, nSrcWidth, nSrcHeight );
		} //if..else
	} //SetAnimation

	/**
	 * UV 에니메이션.
	 */
	void	UVAnimation( VERTEX* pVertices, int nSrcX, int nSrcY, int nSrcWidth, int nSrcHeight )
	{
		float		fOffsetX = 0.0F;
		float		fOffsetY = 0.0F;
		
		if( m_nRow == 1 )
		{
			fOffsetX = ( 0.01F / m_dwDelay ) * m_nFrame;
		} //if
		
		if( m_nCol == 1 )
		{
			fOffsetY = ( 0.01F / m_dwDelay ) * m_nFrame;
		} //if

		pVertices[0].tu    = (float)nSrcX / m_dwSizeX + fOffsetX;
		pVertices[0].tv    = (float)nSrcY / m_dwSizeY + fOffsetY;
		pVertices[1].tu    = (float)( nSrcX + nSrcWidth  ) / m_dwSizeX + fOffsetX;
		pVertices[1].tv    = (float)nSrcY                  / m_dwSizeY + fOffsetY;
		pVertices[2].tu    = (float)nSrcX                  / m_dwSizeX + fOffsetX;
		pVertices[2].tv    = (float)( nSrcY + nSrcHeight ) / m_dwSizeY + fOffsetY;
		pVertices[3].tu    = (float)( nSrcX + nSrcWidth  ) / m_dwSizeX + fOffsetX;
		pVertices[3].tv    = (float)( nSrcY + nSrcHeight ) / m_dwSizeY + fOffsetY;
	} //UVAnimation

	/**
	 * 스프라이트 에니메이션.
	 */
	void	SpriteAnimation( VERTEX* pVertices, int nSrcX, int nSrcY, int nSrcWidth, int nSrcHeight )
	{
		pVertices[0].tu    = (float)( ( m_nAddX * m_nAniX + nSrcX )	     / (float)( m_dwSizeX ) );
		pVertices[0].tv    = (float)( ( m_nAddY * m_nAniY + nSrcY )	     / (float)( m_dwSizeY ) );

		pVertices[1].tu    = (float)( ( m_nAddX * m_nAniX + nSrcWidth )  / (float)( m_dwSizeX ) );
		pVertices[1].tv    = (float)( ( m_nAddY * m_nAniY + nSrcY )	     / (float)( m_dwSizeY ) );

		pVertices[2].tu    = (float)( ( m_nAddX * m_nAniX + nSrcX )		 / (float)( m_dwSizeX ) );
		pVertices[2].tv    = (float)( ( m_nAddY * m_nAniY + nSrcHeight ) / (float)( m_dwSizeY ) );

		pVertices[3].tu    = (float)( ( m_nAddX * m_nAniX + nSrcWidth )  / (float)( m_dwSizeX ) );
		pVertices[3].tv    = (float)( ( m_nAddY * m_nAniY + nSrcHeight ) / (float)( m_dwSizeY ) );
	} //SpriteAnimation

private:
	CTexture*			m_pTexture;		///< 텍스처
	DWORD				m_dwSizeX;		///< 텍스처 가로 크기
	DWORD				m_dwSizeY;		///< 텍스처 세로 크기

	int					m_nWidth;		///< 가로 길이
	int					m_nHeight;		///< 세로 길이

	int					m_nCenterX;		///< 보정 X 좌표
	int					m_nCenterY;		///< 보정 Y 좌표
	DWORD				m_dwColor;		///< 컬러

	BOOL				m_bAnimation;	///< 애니메이션의 여부
	int					m_nTotalFrame;	///< 프레임총수
	int					m_nCol;			///< 애니메이션 용 세로 갯수 
	int					m_nRow;			///< 애니메이션 용 가로 갯수
	int					m_nAniX;
	int					m_nAniY;
	int					m_nAddX;		///< 가로 가중치
	int					m_nAddY;		///< 세로 가중치
	int					m_nFrame;		///< 프레임
	DWORD				m_dwDelay;		///< 딜레이

	DWORD				m_dwTime;		///< 타임
}; //class CW3DSprite


IMPLEMENT_COMPONENT( CW3DSprite, Sprite );
