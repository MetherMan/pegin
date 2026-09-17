/**
 * UI 그래픽
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란
 * @date	$Date: 04-06-10 5:32p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Render/Src/W3DUIGraphic.cpp $
 * 
 * 3     04-06-10 5:32p Paran
 * 
 * 2     04-05-29 10:06p Paran
 * 
 * <HR>
 *
 * @file	W3DUIGraphic.cpp
 */

#include "Render_Export.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include <vector>
#include <list>

using std::vector;
using std::list;

/**
 * class UserInterface Graphic
 */
class CW3DUIGraphic : public IW3DUIGraphic
{
public:
	CW3DUIGraphic();
	~CW3DUIGraphic();

	void		Init();
	void		InitVertexShader();

	HUIG		CreateSprite( 
		const char* szTexFile, 
		const int nX, 
		const int nY, 
		const int nWidth, 
		const int nHeight,
		const int nSrcX,
		const int nSrcY,
		const int nSrcWidth,
		const int nSrcHeight
	);

	HUIG		CreateSprite( 
		const char* szTexFile, 
		const int nX, 
		const int nY, 
		const int nWidth, 
		const int nHeight
	);

	void		Move( HUIG hUIG, int nX, int nY );
	void		ReleaseSprite( HUIG hUIG );

	void		SetVSConst(
		float*		pfVSConst,		///< [IN/OUT] VertexShader Const
		const int	nX,				///< [IN] 2D X Coord
		const int	nY,				///< [IN] 2D Y Coord
		const int	nWidth,			///< [IN] 2D X Length
		const int	nHeight,		///< [IN] 2D Y Length
		const int	nSrcX,			///< [IN] Source X Coord
		const int	nSrcY,			///< [IN] Source Y Coord
		const int	nSrcWidth,		///< [IN] Source X Length
		const int	nSrcHeight,		///< [IN] Source Y Length
		const int	nTexSizeX,		///< [IN] Texture X Length
		const int	nTexSizeY		///< [IN] Texture Y Length
	);
	void		SetRenderState();

	void		DrawSprite( HUIG hUIG );
	void		Render();

	struct SUISprite
	{
		SUISprite()	{ pTexture = NULL; }

		CTexture*			pTexture;			///< Texture
		float				fCoordConst[8];		///< VertexShader Const
	}; //struct SUISprite
	typedef CHandleMgr<SUISprite, HUIG>		UISpriteMgr;
	typedef list<HUIG>						LIST_HUIG;
		
private:
	struct VERTEX
	{
		enum { FVF = D3DFVF_XYZ };
		float	x, y, z;
	}; //struct VERTEX

	CVertexBuffer*					m_pVB;
	DWORD							m_dwVertexShader;
	UISpriteMgr						m_UISpriteMgr;
	LIST_HUIG						m_listDrawHUIG;
}; //CW3DUIGraphic

IMPLEMENT_COMPONENT( CW3DUIGraphic, UIGraphic );

/**
 * Construction
 */
CW3DUIGraphic::CW3DUIGraphic()
:	m_dwVertexShader	( 0 ),
	m_pVB				( NULL )
{
} //CW3DUIGraphic::CW3DUIGraphic

/**
 * Destruction
 */
CW3DUIGraphic::~CW3DUIGraphic()
{
	g_pd3dDevice->DeleteVertexShader( m_dwVertexShader );
	SAFE_DELETE( m_pVB );
} //CW3DUIGraphic::~CW3DUIGraphi

/**
 * Initialize 
 */
void CW3DUIGraphic::Init()
{
	SAFE_DELETE( m_pVB );

	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );
	if( !m_pVB )
	{
		Assert( !"Construct Not VertexBuffer" );
		return;
	} //if

	if( !m_pVB->Create( 4 ) )
	{
		Assert( !"Failed, Create The VertexBuffer" );
		return;
	} //if

	VERTEX*		pVertices = (VERTEX*)m_pVB->Lock();
	Assert( pVertices );

	pVertices[0].x	= -0.5F;
	pVertices[0].y	= -0.5F;
	pVertices[0].z	=  1.0F;
	pVertices[1].x	= +0.5F;
	pVertices[1].y	= -0.5F;
	pVertices[1].z	=  1.0F;
	pVertices[2].x	= -0.5F;
	pVertices[2].y	= +0.5F;
	pVertices[2].z	=  1.0F;
	pVertices[3].x	= +0.5F;
	pVertices[3].y	= +0.5F;
	pVertices[3].z	=  1.0F;

	m_pVB->Unlock();

	InitVertexShader();
} //CW3DUIGraphic::Init

/**
 * Initialize Vertex Shader 
 */
void CW3DUIGraphic::InitVertexShader()
{
	DWORD			dwDeclaration[MAX_FVF_DECL_SIZE];
	if( FAILED( D3DXDeclaratorFromFVF( VERTEX::FVF, dwDeclaration ) ) )
	{
		Assert( !"Failed, The Declaration" );
		return;
	} //if

	static const char cVertexShaderSource[] = "\
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
	
	HRESULT			hr;
	LPD3DXBUFFER	pAssemCode;

	hr = D3DXAssembleShader( cVertexShaderSource, (UINT)strlen( cVertexShaderSource ), 0, NULL, &pAssemCode, NULL );
	if( FAILED( hr ) )
	{
		Assert( !"Failed, The Assemble" );
		return;
	} //if

	hr = g_pd3dDevice->CreateVertexShader( dwDeclaration, (DWORD*)pAssemCode->GetBufferPointer(), &m_dwVertexShader, 0 );
	SAFE_RELEASE( pAssemCode );
	if( FAILED( hr ) )
	{
		Assert( !"Failed, Create The VertexShader" );
		return;
	} //if

	D3DXMATRIX			matProjection;
	D3DXMatrixOrthoOffCenterLH( &matProjection, 0, 800, 600, 0, 0, 1 );		
	D3DXMATRIX			matOffset;
	D3DXMatrixTranslation( &matOffset, -0.5, -0.5, 0 );
	matProjection = matOffset * matProjection;
	D3DXMatrixTranspose( &matProjection, &matProjection);

	g_pd3dDevice->SetVertexShaderConstant( 8, (float*)&matProjection, 4 );

	D3DXMATRIX			matIdentity;
	D3DXMatrixIdentity( &matIdentity );
	g_pd3dDevice->SetVertexShaderConstant( 4, (float*)&matIdentity, 4 );

	float			afConst_2[4] = { 1.0F, 1.0F, 1.0F, 1.0F };
	g_pd3dDevice->SetVertexShaderConstant( 2, (float*)afConst_2, 1 );
} //CW3DUIGraphic::InitVertexShader


/**
 * Setting RenderState
 */
void CW3DUIGraphic::SetRenderState()
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
} //CW3DUIGraphic::SetRenderState

/**
 * Create Sprite
 */
HUIG CW3DUIGraphic::CreateSprite( 
	const char* szTexFile, 
	const int nX, 
	const int nY, 
	const int nWidth, 
	const int nHeight,
	const int nSrcX,
	const int nSrcY,
	const int nSrcWidth,
	const int nSrcHeight
)
{
	Assert( szTexFile );

	HUIG	hUIG;

	SUISprite*		pSprite = m_UISpriteMgr.Acquire( hUIG );
	Assert( pSprite );

	pSprite->pTexture = g_pTextureMgr->LoadTexture( "", szTexFile, CTextureMgr::SPRITE );
	if( !pSprite->pTexture )
	{
		Assert( !"Failed, Load The Texture" );
	} //if
 
	DWORD		dwTexSizeX;
	DWORD		dwTexSizeY;

	D3DSURFACE_DESC		desc;
	HRESULT		hr;
	hr = pSprite->pTexture->GetD3dTexture()->GetLevelDesc( 0, &desc );
	if( FAILED( hr ) )
	{
		Assert( !"텍스처 정보 얻기 실패" );
		dwTexSizeX = nSrcWidth;
		dwTexSizeY = nSrcHeight;
	}
	else
	{
		dwTexSizeX = desc.Width;
		dwTexSizeY = desc.Height;
	} //if..else

	SetVSConst( pSprite->fCoordConst, nX, nY, nWidth, nHeight, nSrcX, nSrcY, nSrcWidth, nSrcHeight, dwTexSizeX, dwTexSizeY );

	return hUIG;
} //CW3DUIGraphic::CreateSprite

/**
 * Create Sprite
 */
HUIG CW3DUIGraphic::CreateSprite( 
	const char* szTexFile, 
	const int nX, 
	const int nY, 
	const int nWidth, 
	const int nHeight
)
{
	Assert( szTexFile );
	return CreateSprite( szTexFile, nX, nY, nWidth, nHeight, 0, 0, nWidth, nHeight );
} //CW3DUIGraphic::CreateSprite

/**
 * Release Sprite
 */
void CW3DUIGraphic::ReleaseSprite( HUIG hUIG )
{
	SUISprite*		pSprite = m_UISpriteMgr.Dereference( hUIG );
	if( pSprite )
	{
		SAFE_RELEASE( pSprite->pTexture );
	} //if
	
	m_UISpriteMgr.Release( hUIG );
} //CW3DUIGraphic::Release

/**
 * Move the UIG 
 */
void CW3DUIGraphic::Move( HUIG hUIG, int nX, int nY )
{
	SUISprite*		pSprite = m_UISpriteMgr.Dereference( hUIG );
	if( !pSprite )
	{
		Assert( 0 );
		return;
	} //if

	float*		pVSConst = pSprite->fCoordConst;
	Assert( pVSConst );

	pVSConst[0] = (float)nX + pVSConst[2] * 0.5F;
	pVSConst[1] = (float)nY + pVSConst[3] * 0.5F;
} //CW3DUIGraphic::Move

/**
 * Setting Vertices
 */
void CW3DUIGraphic::SetVSConst(
	float*		pfVSConst,		///< [IN/OUT] VertexShader Const
	const int	nX,				///< [IN] 2D X Coord
	const int	nY,				///< [IN] 2D Y Coord
	const int	nWidth,			///< [IN] 2D X Length
	const int	nHeight,		///< [IN] 2D Y Length
	const int	nSrcX,			///< [IN] Source X Coord
	const int	nSrcY,			///< [IN] Source Y Coord
	const int	nSrcWidth,		///< [IN] Source X Length
	const int	nSrcHeight,		///< [IN] Source Y Length
	const int	nTexSizeX,		///< [IN] Texture X Length
	const int	nTexSizeY		///< [IN] Texture Y Length
)
{
	Assert( pfVSConst );

	float		fTexStartX = (float)nSrcX / nTexSizeX;
	float		fTexStartY = (float)nSrcY / nTexSizeY;
	float		fTexEndX = (float)( nSrcX + nSrcWidth ) / nTexSizeX;
	float		fTexEndY = (float)( nSrcY + nSrcHeight ) / nTexSizeY;

	pfVSConst[0] = (float)nX + nWidth * 0.5F;
	pfVSConst[1] = (float)nY + nHeight * 0.5F;
	pfVSConst[2] = (float)nWidth;
	pfVSConst[3] = (float)nHeight;
	pfVSConst[4] = (float)fTexStartX + fTexEndX * 0.5F;
	pfVSConst[5] = (float)fTexStartY + fTexEndY * 0.5F;
	pfVSConst[6] = fTexEndX;
	pfVSConst[7] = fTexEndY;
} //CW3DUIGraphic::SetVSConst

/**
 * Draw Sprite 
 */
void CW3DUIGraphic::DrawSprite( HUIG hUIG )
{
	m_listDrawHUIG.push_back( hUIG );
} //CW3DUIGraphic::DrawSprite

/**
 * Drawing the UIG
 */
void CW3DUIGraphic::Render()
{
	if( m_listDrawHUIG.empty() )
	{
		return;
	} //if

	SetRenderState();

	SUISprite*				pUISprite;
	LIST_HUIG::iterator		iter = m_listDrawHUIG.begin();
	while( iter != m_listDrawHUIG.end() )
	{
		pUISprite = m_UISpriteMgr.Dereference( *iter ); 
		iter = m_listDrawHUIG.erase( iter );
		if( !pUISprite )
		{
			continue;
		} //if

		if( pUISprite->pTexture ) 
		{
			pUISprite->pTexture->SetTexture( 0 );
		} //if

		g_pd3dDevice->SetVertexShaderConstant( 0, pUISprite->fCoordConst, 2 );
		m_pVB->DrawShader( D3DPT_TRIANGLESTRIP, 2, m_dwVertexShader );
	} //while

	D3DXMATRIX identity;
	D3DXMatrixIdentity( &identity );
	g_pd3dDevice->SetVertexShaderConstant(  4, (float*)&identity,  4 );
} //CW3DUIGraphic::Draw

