/**
 * Mesh를 이용한 Effect.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 6:18p $
 * @version	$Revision: 61 $
 *
 * <HR>
 * $Log: /Render/Src/W3DSpecialEffect.cpp $
 * 
 * 61    04-01-08 6:18p Kjmgo
 * 
 * 60    04-01-08 4:20p Kjmgo
 * 코드정리
 *
 * <HR>
 *
 * @file	W3DSpecialEffect.cpp
 */


#include "Render_Export.h"
#include "SpecialEffectMesh.h"
#include "Texture.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>

using std::vector;

// Keep solid ice opacity separate from additive rims, fog and magic circles.
// This is also the classifier used by the guarded ice-opacity-v1 Render.dll hook.
static BOOL UsesCocytusSurfaceAlpha( const std::string& fullName )
{
    const char* name = fullName.c_str();
    for( const char* p = name; *p; ++p )
        if( *p == '/' || *p == '\\' ) name = p + 1;
    const size_t size = strlen( name );
    if( size < 4 || _stricmp( name + size - 4, ".wed" ) != 0 ) return FALSE;
    return _stricmp( name, "mf_cocytus_pillar.wed" ) == 0
        || _strnicmp( name, "mf_eruption_", 12 ) == 0
        || _strnicmp( name, "mf_crown_spike", 14 ) == 0;
}


/**
 *
 */
struct	SpecialEffect;

CMeshMgr	g_MeshMgr;

/**
 * 특수효과 클래스.
 * @ingroup		Render
 */
class CW3DSpecialEffect : public IW3DSpecialEffect
{
public:
	UNDER_MEMORY_MGR( CW3DSpecialEffect );

	CW3DSpecialEffect();
	~CW3DSpecialEffect();

	BOOL Render( const int nINFrame, const Matrix4& matWorld, const BOOL bTool = false );
	BOOL Load( const char* szFile );
	BOOL LoadMesh(  const char* szFileName );
	BOOL SetTexture(  const char* szFileName );

	void SetBright( const DWORD dwColor );
	void SetWorld( const Matrix4& matWorld );
	void SetTextureAnimation( const int nId );
	void SetAnimation( const int nRow, const int nCols, const int nId );
	
	void SetColor( const DWORD dwColor )
	{
		m_bSetColor = TRUE;
		m_dwColor = dwColor;
	} //SetColor

	int  GetMaxFrame();

private:
	CTexture*		m_pTexture;				///<	Texture
	CEffectMesh*	m_pMesh;				///<	Mesh	
	std::string		m_sName;				///<	Name
	DWORD			m_dwBright;				///<	Bright Value
	DWORD			m_dwColor;				///<	Color
	
	BOOL			m_bTextureAnimation;	///<	Is Use Texture Animation
	BOOL			m_bSetColor;			///<	Settting Color
	BOOL			m_bTool;				///<	Tool

	int				m_nMaxFrame;			///<	Max Frame
	int				m_nStartFrame;			///<	Start Frame
	int				m_nTextureId;			///<	
	int				m_nTextureRow;			///<	텍스처 분배
	int				m_nTextureCol;			///<	텍스처 분배

	SpecialEffect*	m_pEffect;				///<	Special Effect

	int		CheckCount( int nFrame );
	void	AnimationTexture( const int nFrame );
	void	ApplyEffect( int nFrame, const Matrix4& matWorld );
	void	SetRenderState();
}; //class CW3DSpecialEffect


IMPLEMENT_COMPONENT( CW3DSpecialEffect, SpecialEffect );

/**
 *
 */
struct SpecialEffect
{
	UNDER_MEMORY_MGR( SpecialEffect );

	Vector3		vPos;			///<	Position
	Quaternion	qRot;			///<	Rotation
	float		fScale;			///<	Scale

	float		fScaleHeight;	///<	Scale
	float		fScaleX;		///<	X Axis Scale
	float		fScaleZ;		///<	Z Axis Scale

	DWORD		dwColor;		///<	Color Value
	int			nTexture;		///<	Texture Id
}; //struct SpecialEffect


/////////////////////////////////////////////////////


/**
 * 생성자.
 */
CW3DSpecialEffect::CW3DSpecialEffect()
:	m_pEffect		( NULL ),
	m_pMesh			( NULL ),
	m_pTexture		( NULL ),
	m_dwBright		( 0xFFFFFFFF ),
	m_dwColor		( 0xFFFFFFFF ),
	m_nTextureId	( 0 ),
	m_nTextureRow	( 1 ),
	m_nTextureCol	( 1 ),
	m_nMaxFrame		( 0 ),
	m_nStartFrame	( 0 ),
	m_bTool			( FALSE ),
	m_bTextureAnimation ( FALSE )
{
	g_MeshMgr.Add();
} //CW3DSpecialEffect::CW3DSpecialEffect

/**
 * 소멸자.
 */
CW3DSpecialEffect::~CW3DSpecialEffect()
{
	CHECK_BOUNCE( m_pEffect );
	SAFE_RELEASE( m_pTexture );
	SAFE_FREE( m_pEffect );
	
	g_MeshMgr.Remove();
} //CW3DSpecialEffect::~CW3DSpecialEffect

/**
 * 특수 효과 정보 파일을 읽어온다.
 */
BOOL CW3DSpecialEffect::Load( const char* szFile )
{
	char szFileName[256];
	char szHeaderDefine[32] = "Wind3D Special Effect Data V1.1";

	sprintf( szFileName, "Effect/%s", szFile );

	FILE*	fp = fopen( szFileName, "rb" );

	if( NULL == fp )
	{
//		Assert( !"파일을 열 수 없습니다" );
		return FALSE;
	} //if

	m_sName = szFileName;

	ArchiveFileReader	ar( fp );

	char szHeader[32];

	// Header
	ar.Serialize( szHeader, 31 );

	szHeader[31] = 0;

	if( strcmp( szHeader, szHeaderDefine ) )
	{
		// 안돼! 버전이 틀려!
		Assert( !"버전이 틀립니다.!" );
		ar.Close();
		return FALSE;
	} //if

	// Mesh Name

	int  nLen = 0;
	char szMeshName[128];

	ar.Serialize( &nLen, 4 );
	ar.Serialize( szMeshName, nLen );

	if( nLen > 1 )
	{
		LoadMesh( szMeshName );
	}
	else
	{
		Assert( !"메쉬가 없는 이펙트 따위 없어져 버려!" );
		ar.Close();
		return FALSE;
	} //if..else..

	// Texture Name

	char szTextureName[128];

	ar.Serialize( &nLen, 4 );
	ar.Serialize( szTextureName, nLen );

	if( nLen > 1 )
	{
		SetTexture( szTextureName );
	} //if

	// Max Frame
	ar.Serialize( &m_nStartFrame, 4 );
	ar.Serialize( &m_nMaxFrame, 4 );

	int nCount = 0;

	ar.Serialize( &nCount, 4 );
	
	// Texture....

	ar.Serialize( &m_nTextureRow, 4 );
	ar.Serialize( &m_nTextureCol, 4 );

	m_bTextureAnimation = TRUE;

	Assert( !m_pEffect );

	m_pEffect = (SpecialEffect*)_MemAlloc( sizeof( SpecialEffect ) * nCount );
	CHECK_PTR_RETURN( m_pEffect );
	_ZeroMemory( m_pEffect, sizeof( SpecialEffect ) * nCount );



	int nSize = sizeof( SpecialEffect );

	ar.Serialize( m_pEffect, nSize * nCount );
	ar.Close();

	return TRUE;
} //CW3DSpecialEffect::Load

/**
 *
 */
BOOL CW3DSpecialEffect::LoadMesh( const char* szFileName_ )
{	
	m_pMesh = g_MeshMgr.Load( szFileName_ );
	
	if( !m_pMesh )
	{
		return FALSE;
	} //if

	return TRUE;		
} //CW3DSpecialEffect::LoadMesh

/**
 *
 */
BOOL CW3DSpecialEffect::SetTexture( const char* szFileName_ )
{
	SAFE_RELEASE( m_pTexture );

	m_pTexture = g_pTextureMgr->LoadTexture( "Texture/Effect/", szFileName_ );

	if( !m_pTexture )
	{
		Assert( !"텍스처 로딩 실패" );
		return FALSE;
	} //if
 
	return TRUE;
} //CW3DSpecialEffect::SetTexture

/**
 * 특수효과를 그린다.
 */
BOOL CW3DSpecialEffect::Render( const int nINFrame, const Matrix4& matWorld, const BOOL bTool )
{
	m_bTool = bTool;

	if( !m_pMesh )
	{
		return FALSE;
	} //if

	if( nINFrame >= m_nMaxFrame && !bTool )
	{
		return FALSE;
	} //if

    const BOOL bIceAlpha = UsesCocytusSurfaceAlpha( m_sName );
    DWORD alphaOp = 0, alphaArg1 = 0, alphaArg2 = 0;
    if( bIceAlpha )
    {
        // Never alter shared state unless every original value was captured.
        if( FAILED( g_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP, &alphaOp ) )
            || FAILED( g_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG1, &alphaArg1 ) )
            || FAILED( g_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAARG2, &alphaArg2 ) ) )
            return FALSE;
    }

	int		nFrame = CheckCount( nINFrame );

	if( !bTool )
	{
		ApplyEffect( nFrame, matWorld );
	} //if

	if( m_pTexture )
	{
		m_pTexture->SetTexture( 0 );
	}
	else
	{
		g_pd3dDevice->SetTexture( 0, NULL );
	} //if..else..

	AnimationTexture( nFrame );

	SetRenderState();

    if( bIceAlpha )
    {
        // WED dwColor keeps its authored RGB; its high byte is true opacity.
        g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
        g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
    }

	m_pMesh->Render();

    if( bIceAlpha )
    {
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, alphaOp );
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, alphaArg1 );
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, alphaArg2 );
        g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
        g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
    }

	return TRUE;
} //CW3DSpecialEffect::Render

/**
 * Render State를 설정한다.
 */
void CW3DSpecialEffect::SetRenderState()
{
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	// Texture 관련 세팅
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_TFACTOR );

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,		D3DTADDRESS_WRAP );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,		D3DTADDRESS_WRAP );

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

	g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR,		m_dwBright );
} //CW3DSpecialEffect::SetRenderState

/**
 *
 */
void CW3DSpecialEffect::SetWorld( const Matrix4& matWorld_ )
{
	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld_ );
}

/**
 *
 */
void CW3DSpecialEffect::SetBright( const DWORD dwColor_ )
{
	m_dwBright = dwColor_;
	g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, m_dwBright );
}

/**
 *
 */
void CW3DSpecialEffect::SetTextureAnimation( const int nId_ )
{
	m_nTextureId = nId_;
}

/**
 *
 */
void CW3DSpecialEffect::AnimationTexture( int nFrame_ )
{
	Assert( m_nTextureRow );
	Assert( m_nTextureCol );

	if( !m_bTool )
	{
		m_nTextureId = m_pEffect[ nFrame_ ].nTexture;
	}

	if( m_nTextureId < 1 )
	{
		return ;
	}

	int nID = m_nTextureId - 1;

	// X, Y 의 Part를 구한다.

	int nX = nID % ( m_nTextureRow );	// 몇번째 칸.
	int nY = nID / ( m_nTextureRow );  // 몇번째 줄.


	float fX = ( 1 / (float)m_nTextureRow ) * ( nX );	// 오른쪽으로 넘어갈 만큼	
	float fY = ( 1 / (float)m_nTextureCol ) * ( nY );	// 아래로 넘어갈 만큼


	VERTEX *pVertex = NULL;
			
	m_pMesh->m_pVB->Lock( 0, 0, (BYTE**)&pVertex,0 );	//D3DLOCK_DISCARD );

	for( int i = 0 ; i < m_pMesh->m_nNumVertices ; i++ )
	{
		// 처음 UV를 Row 와 Col 만큼 축소 시킨다음, 좌표를 이동시켜 준다.
		pVertex[ i ].tu = ( m_pMesh->m_pVerticesClone[ i ].tu / (float)m_nTextureRow  ) + fX;
		pVertex[ i ].tv = ( m_pMesh->m_pVerticesClone[ i ].tv / (float)m_nTextureCol ) + fY;
	}

	m_pMesh->m_pVB->Unlock();
} //CW3DSpecialEffect::AnimationTexture

/**
 * 애니메이션 정보를 설정한다.
 */
void CW3DSpecialEffect::SetAnimation(
										const int nRow,		///< [IN]
										const int nCols,	///< [IN]
										const int nId		///< [IN]
										)
{
	m_bTextureAnimation	= true;
	m_nTextureRow		= nRow;
	m_nTextureCol		= nCols;

	m_nTextureId		= nId;
} //CW3DSpecialEffect::AnimationTexture

/**
 *
 */
int CW3DSpecialEffect::CheckCount( int nFrame_ )
{
	if( NULL == m_pEffect )
	{
		return m_nMaxFrame + 1;
	} //if

	int nFPS = 30;

	int nFrame = nFrame_;

	while( nFrame < 0 )
	{
		nFrame += m_nMaxFrame;
	} //while

	nFrame %= m_nMaxFrame;

	if( m_nMaxFrame - 1 ==  nFrame && !m_bTool )
	{
		return m_nMaxFrame;
	} //if

	if( nFrame_ <= m_nStartFrame )
	{
		nFrame = m_nStartFrame;
	} //if

	return nFrame;
} //CW3DSpecialEffect::CheckCount

/**
 * 원하는 프레임에 해당하는 이펙트를 적용한다.
 */
void CW3DSpecialEffect::ApplyEffect( int nFrame, const Matrix4& matWorld )
{
	if( nFrame > m_nMaxFrame )
	{
		nFrame = m_nMaxFrame;
	} //if

	Assert( nFrame >= 0 );

	// Animation Matrix
	Matrix4		matAnimation;
	Matrix4		matScale;

	// Rot
	Vector3		vRot;
	Vector3		vPos			= m_pEffect[nFrame].vPos;
	float		fScale			= m_pEffect[nFrame].fScale;
	float		fScaleX			= m_pEffect[nFrame].fScaleX;
	float		fScaleZ			= m_pEffect[nFrame].fScaleZ;
	float		fScaleHeight	= m_pEffect[nFrame].fScaleHeight;

	// Apply Rotation
	vRot.x = m_pEffect[nFrame].qRot.x;
	vRot.y = m_pEffect[nFrame].qRot.y;
	vRot.z = m_pEffect[nFrame].qRot.z;

	matAnimation.SetIdentity();
	matScale.SetIdentity();

	float		fRad = MATH_PI / 180;
	matAnimation.SetRotation( vRot, fRad * m_pEffect[nFrame].qRot.w );

	matScale._11 *= fScaleX;
	matScale._22 *= fScaleHeight;
	matScale._33 *= fScaleZ;

	matAnimation._41 = vPos.x * fScale;
	matAnimation._42 = vPos.y * fScale;
	matAnimation._43 = vPos.z * fScale;
	matAnimation._44 = fScale;

	matAnimation *= matScale;

	SetWorld( matAnimation * matWorld );

	DWORD dwColor;

	if( m_bSetColor )
	{
		dwColor = m_dwColor;
	}
	else
	{
		dwColor = m_pEffect[nFrame].dwColor;
	} //if..else..

	SetBright( dwColor );

	m_bSetColor = FALSE;
} //CW3DSpecialEffect::ApplyEffect

/**
 * 특수 효과 애니메이션의 최대 프레임을 얻는다.
 */
int CW3DSpecialEffect::GetMaxFrame()
{
	return m_nMaxFrame;
} //CW3DSpecialEffect::GetMaxFrame
