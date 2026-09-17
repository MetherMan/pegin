/**
 * 빌보드 컴포넌트 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 2:01p $
 * @version	$Revision: 29 $
 *
 * <HR>
 * $Log: /Render/Src/W3DBillboard.cpp $
 * 
 * 29    04-01-08 2:01p Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	W3DBillboard.cpp
 */


#include "Render_Export.h"
#include "VertexBuffer.h"
#include "Texture.h"

#define CHECK_KEYWORD( s )	!strcmp( szKeyword, s )

/**
 * 빌보드 컴포넌트 클래스 구현부.
 * @ingroup		Render
 */
class CW3DBillboard : public IW3DBillboard
{
public:
	UNDER_MEMORY_MGR( CW3DBillboard );

	/**
	 *
	 */
	CW3DBillboard();

	/**
	 *
	 */
	~CW3DBillboard();

	BOOL	Load( const char* szFileName );
	BOOL	Load( const char* szFileName, float fHeight, float fWidth );

	void	Update( const DWORD dwTick );
	void	Draw( const Vector3& vPos );
	void	Draw( const Vector3& vPos, DWORD dwTime );

	void	SetLocal( const Matrix4& matLocal )
	{
		m_matLocal = matLocal;
	} //SetLocal
	
private:
	/**
	 *
	 */
	struct VERTEX
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		float	x, y, z;
		DWORD	color;
		float	tu, tv;
	}; //struct VERTEX

	// Billboard Size
	float				m_fWidth;				///< 가로 길이.
	float				m_fHeight;				///< 세로 길이.

	// Texture 
	char				m_szTexture[256];		///< Texture File Name
	CTexture*			m_pTexture;				///< 텍스처.

	// Texture Animation
	BOOL				m_bUseTextureAnimation;	///< Texture Animation 사용 여부
	int					m_nTextureRow;			///< 텍스처 분배
	int					m_nTextureCol;			///< 텍스처 분배
	int					m_nTotalFrame;			///< 총 프레임수
	DWORD				m_dwFrameTime;			///< 한 프레임당 걸리는 시간

	Matrix4				m_matTexture;			///< Texture Matrix
	Matrix4				m_matLocal;				///< Local Matrix

	BOOL				m_bLocal;				///< Local Matrix 사용 여부.
	
	// Time
	DWORD				m_dwTime;				///< 내부 누적 틱
	int					m_nFrame;				///< Frame

	// Axis
	BOOL				m_bZAxis;				///< Z축만 쓰는지의 여부 ( False의경우 XZ축 사용 )

	CVertexBuffer*		m_pVB;			///< 버텍스 버퍼.

	BOOL	Create();
	void	CreateVertices( VERTEX* pVertices );
	void	TextureAnimation();
	void	SetRenderState();

}; //class CW3DBillboard


IMPLEMENT_COMPONENT( CW3DBillboard, Billboard );


/**
 * 생성자.
 */
CW3DBillboard::CW3DBillboard()
:	m_pTexture	( NULL ),
	m_pVB		( NULL ),
	m_bUseTextureAnimation( FALSE ),
	m_nFrame		( 0 ),
	m_dwTime		( 0 ),
	m_nTextureRow	( 0 ),
	m_nTextureCol	( 0 )
{
	m_matTexture.SetIdentity();
	m_matLocal.SetIdentity();
} //CW3DBillboard::CW3DBillboard

/**
 * 소멸자.
 */
CW3DBillboard::~CW3DBillboard()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_DELETE( m_pVB );
} //CW3DBillboard::~CW3DBillboard

/**
 * 빌보드 생성.
 *
 * @return	생성 성공 여부
 */
BOOL CW3DBillboard::Create()
{
	m_pTexture = g_pTextureMgr->LoadTexture( "Texture/Effect/", m_szTexture );
	//Assert( m_pTexture );

	m_pVB = new CVertexBuffer( VERTEX::FVF, sizeof( VERTEX ) );
	CHECK_PTR_RETURN( m_pVB );

	if( !m_pVB->Create( 4 ) )
	{
		SAFE_DELETE( m_pVB );
		return FALSE;
	} //if

	VERTEX*		pVertices = (VERTEX*)m_pVB->Lock();
	if( !pVertices )
	{
		Assert( !"버텍스 버퍼 락 실패" );
		return FALSE;
	} //if

	CreateVertices( pVertices );

	m_pVB->Unlock();

	m_matTexture.SetIdentity();


	return TRUE;
} //CW3DBillboard::Create

/**
 * 빌보드 정보를 로드 한다.
 *
 */
BOOL CW3DBillboard::Load( const char* szFileName )
{
	string sFile = "Effect/";
	sFile += szFileName;
	FILE* fp = fopen( sFile.c_str(), "rt" );

	if( NULL == fp )
	{
		Assert( !"Billboard : 파일이 없습니다." );
		return FALSE;
	} //if

	// 파싱....

	char szBuffer[256];
	char szKeyword[80];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		sscanf( szBuffer, "%s", szKeyword );
		
		if( CHECK_KEYWORD( "[TEXTURE]" ) )
		{
			// Texture
			sscanf( szBuffer, "%s %s", szKeyword, m_szTexture );
		}
		else if( CHECK_KEYWORD( "[USEANIMATION]" ) ) 
		{
			// Use Animation
			m_bUseTextureAnimation = TRUE;
		}
		else if( CHECK_KEYWORD( "[ZAXIS]" ) )
		{
			m_bZAxis = TRUE;
		}
		else if( CHECK_KEYWORD( "[XZAXIS]" ) )
		{
			m_bZAxis = FALSE;
		}
		else if( CHECK_KEYWORD( "[TILE]" ) )
		{
			// TILE 갯수( Animation 용 )
			sscanf( szBuffer, "%s %d %d", szKeyword, &m_nTextureRow, &m_nTextureCol );
			m_nTotalFrame = m_nTextureRow * m_nTextureCol;
			m_nFrame = 0;
		}	
		else if( CHECK_KEYWORD( "[FRAMETIME]" ) )
		{
			sscanf( szBuffer, "%s %d", szKeyword, &m_dwFrameTime );
		}
		else if( CHECK_KEYWORD( "[WIDTH]" ) )
		{
			sscanf( szBuffer, "%s %f", szKeyword, &m_fWidth );
		}
		else if( CHECK_KEYWORD( "[HEIGHT]" ) )
		{
			sscanf( szBuffer, "%s %f", szKeyword, &m_fHeight );			
		} //if..elsee
	} //while

	fclose( fp );

	Create();

	return TRUE;
} //CW3DBillboard::Load

/**
 *
 */
BOOL CW3DBillboard::Load( const char* szFileName, float fWidth, float fHeight )
{
	string sFile = "Effect/";
	sFile += szFileName;
	FILE* fp = fopen( sFile.c_str(), "rt" );

	if( NULL == fp )
	{
		Assert( !"Billboard : 파일이 없습니다." );
		return FALSE;
	} //if

	m_fWidth = fWidth;
	m_fHeight = fHeight;

	char szBuffer[256];
	char szKeyword[80];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		sscanf( szBuffer, "%s", szKeyword );
		
		if( CHECK_KEYWORD( "[TEXTURE]" ) )
		{
			// Texture
			sscanf( szBuffer, "%s %s", szKeyword, m_szTexture );
		}
		else if( CHECK_KEYWORD( "[USEANIMATION]" ) ) 
		{
			// Use Animation
			m_bUseTextureAnimation = TRUE;
		}
		else if( CHECK_KEYWORD( "[ZAXIS]" ) )
		{
			m_bZAxis = TRUE;
		}
		else if( CHECK_KEYWORD( "[XZAXIS]" ) )
		{
			m_bZAxis = FALSE;
		}
		else if( CHECK_KEYWORD( "[TILE]" ) )
		{
			// TILE 갯수( Animation 용 )
			sscanf( szBuffer, "%s %d %d", szKeyword, &m_nTextureRow, &m_nTextureCol );
			m_nTotalFrame = m_nTextureRow * m_nTextureCol;
			m_nFrame = 0;
		}	
		else if( CHECK_KEYWORD( "[FRAMETIME]" ) )
		{
			sscanf( szBuffer, "%s %d", szKeyword, &m_dwFrameTime );
		}
	} //while

	fclose( fp );

	Create();

	return TRUE;
} //CW3DBillboard::Load

/**
 * 버텍스 정보를 입력한다.
 *
 * @param	pVertices		[OUT] 버텍스 버퍼
 */
void CW3DBillboard::CreateVertices( VERTEX* pVertices )
{
	Assert( pVertices );

	if( !m_bZAxis )
	{
		pVertices[0].x		= -m_fWidth / 2;
		pVertices[0].z		= 0.0F;
		pVertices[0].y		= m_fHeight / 2;
		pVertices[0].color	= 0xFFFFFFFF;
		pVertices[0].tu		= 0.0F;
		pVertices[0].tv		= 0.0F;

		pVertices[1].x		= m_fWidth / 2;
		pVertices[1].z		= 0.0F;
		pVertices[1].y		= m_fHeight / 2;
		pVertices[1].color	= 0xFFFFFFFF;
		pVertices[1].tu		= (float)( 1.0F / m_nTextureRow );
		pVertices[1].tv		= 0.0F;

		pVertices[2].x		= -m_fWidth / 2;
		pVertices[2].z		= 0.0F;
		pVertices[2].y		= -m_fHeight / 2;
		pVertices[2].color	= 0xFFFFFFFF;
		pVertices[2].tu		= 0.0F;
		pVertices[2].tv		= (float)( 1.0F / m_nTextureCol );

		pVertices[3].x		= m_fWidth / 2;
		pVertices[3].z		= 0.0F;
		pVertices[3].y		= -m_fHeight / 2;
		pVertices[3].color	= 0xFFFFFFFF;
		pVertices[3].tu		= (float)( 1.0F / m_nTextureRow );
		pVertices[3].tv		= (float)( 1.0F / m_nTextureCol );
	}
	else
	{
		pVertices[0].x		= -m_fWidth / 2;
		pVertices[0].y		= 0.0F;
		pVertices[0].z		= m_fHeight / 2;
		pVertices[0].color	= 0xFFFFFFFF;
		pVertices[0].tu		= 0.0F;
		pVertices[0].tv		= 0.0F;

		pVertices[1].x		= m_fWidth / 2;
		pVertices[1].y		= 0.0F;
		pVertices[1].z		= m_fHeight / 2;
		pVertices[1].color	= 0xFFFFFFFF;
		pVertices[1].tu		= (float)( 1.0F / m_nTextureRow );
		pVertices[1].tv		= 0.0F;

		pVertices[2].x		= -m_fWidth / 2;
		pVertices[2].y		= 0.0F;
		pVertices[2].z		= -m_fHeight / 2;
		pVertices[2].color	= 0xFFFFFFFF;
		pVertices[2].tu		= 0.0F;
		pVertices[2].tv		= (float)( 1.0F / m_nTextureCol );

		pVertices[3].x		= m_fWidth / 2;
		pVertices[3].y		= 0.0F;
		pVertices[3].z		= -m_fHeight / 2;
		pVertices[3].color	= 0xFFFFFFFF;
		pVertices[3].tu		= (float)( 1.0F / m_nTextureRow );

		pVertices[3].tv		= (float)( 1.0F / m_nTextureCol );
	} //if..else..
} //CW3DBillboard::CreateVertices

/**
 *	빌보드를 업데이트 한다.
 *
 * @param	dwTick		[IN] 경과 Tick
 *
 */
void CW3DBillboard::Update( const DWORD dwTick )
{
	m_dwTime += dwTick;

	if( m_dwTime < m_dwFrameTime )
	{ // Frame Time에 도달하지 못하면 그냥 Skip한다.
		return ;
	} //if

	m_nFrame++;

	if( m_nFrame >= m_nTotalFrame )
	{
		m_nFrame = 0;
	} //if
	
	m_dwTime = 0;

	if( m_bUseTextureAnimation )
	{
		TextureAnimation();
	} //if
} //CW3DBillboard::Update



/**
 * 빌보드를 그린다.
 * 빌보드의 가운데 아래 좌표를 이용한다.
 *
 * @param	vPos		[IN] 출력할 좌표.
 */
void CW3DBillboard::Draw( const Vector3& vPos )
{
	Assert( m_pTexture );
	Assert( m_pVB );

	if( !m_pTexture || !m_pVB )
	{
		return ;
	} //if

	///////////////// RenderState

	SetRenderState();

	///////////////// World 
	Matrix4		matWorld;

	if( m_bZAxis )
	{
		matWorld = g_Frustum.GetBillboardZ();
	}
	else
	{
		matWorld = g_Frustum.GetBillboard();
	} //if..else..

	matWorld = m_matLocal * matWorld;

	//matWorld = matWorld * m_matLocal;

	matWorld._41 = vPos.x;
	matWorld._42 = vPos.y;
	matWorld._43 = vPos.z;

	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
	m_pTexture->SetTexture( 0 );

	////////////////// Draw

	m_pVB->Draw( D3DPT_TRIANGLESTRIP, 2 );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );


} //CW3DBillboard::Draw

/**
 *
 */
void CW3DBillboard::Draw( const Vector3& vPos, DWORD dwTime )
{
	m_nFrame = ( dwTime / m_dwFrameTime ) % m_nTotalFrame;
	
	if( m_bUseTextureAnimation )
	{
		TextureAnimation();
	} //if

	Draw( vPos );
} //CW3DBillboard::Draw

/**
 *
 */
void CW3DBillboard::TextureAnimation()
{
	 /*
		1. 각 행과 열의 번지수를 알아 낸다.
		2. UV를 최소 단위로 축소 시킨다.
		3. 알아낸 번지수로 가중치를 얻어낸다.
		4. 최소단위로 축소시킨 UV를 가중치로 곱한다.
	 */

	// 각 행과 열의 번지수
	int nRow = m_nFrame % m_nTextureRow;
	int nCol = m_nFrame / m_nTextureRow;

	// 행과 열의 가중치 구하기
	float fAddX = (float)( 1.0 / m_nTextureRow ) * ( nRow );	// 오른쪽으로 넘어갈 만큼	
	float fAddY = (float)( 1.0 / m_nTextureCol ) * ( nCol );	// 아래로 넘어갈 만큼

	// 처음 생성할때 UV는 최소 단위로 축소가 되어 있으므로...
	// 가중치를 더하기만 하면 된다.

	m_matTexture._31 = fAddX;
	m_matTexture._32 = fAddY;
} //CW3DBillboard::TextureAnimation


/**
 *
 */
void CW3DBillboard::SetRenderState()
{
	D3D::ZEnable( TRUE );
	D3D::ZWriteEnable( FALSE );

	D3D::Lighting( FALSE );

	D3D::AlphaBlend( TRUE );

	// Texture 관련 세팅
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_TFACTOR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,		D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,		D3DTADDRESS_WRAP );

	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	// Alpha Blend 관련 세팅
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_ONE );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_ONE );

	// Alpha Test 관련 세팅
	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );

	// Light 관련 세팅
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,			FALSE );
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT,			0xFFFFFFFF );

	g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,			FALSE );

	g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,		0xFFFFFFFF );


	// Texture
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
	g_pd3dDevice->SetTransform( D3DTS_TEXTURE0, (D3DMATRIX*)&m_matTexture );
} //CW3DBillboard::SetRenderState
