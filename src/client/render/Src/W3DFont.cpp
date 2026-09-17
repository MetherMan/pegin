/**
 * 폰트 클래스 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-11-16 7:11p $
 * @version	$Revision: 21 $
 *
 * <HR>
 * $Log: /Render/Src/W3DFont.cpp $
 * 
 * 21    04-11-16 7:11p Sk8snow2
 * 
 * 20    04-02-13 6:31p Paran2
 * 
 * 19    04-02-12 12:19p Paran2
 * 
 * 18    03-11-19 8:08p Sckyj
 * 중국어 폰트 생성가능토록 변경
 * 
 * 17    03-11-13 5:46p Paran
 * 
 * 16    03-07-21 5:34a Paran
 * 
 * 15    03-07-16 11:53a Paran
 * 
 * 14    03-06-22 8:25p Paran
 * 
 * 13    03-06-21 1:03p Paran
 * Text 영역 구하는 함수 추가.
 * 
 * 12    03-04-09 11:43p Ahastudio
 *
 * 10    03-03-28 4:12p Paran		줄간 개선
 * <HR>
 *
 * @file	W3DFont.cpp
 */


#include "Render_Export.h"

/*
struct sTextData
{
	char	m_Text[256];

	RECT	m_Rect;

	struct sTextData *prev;
	struct sTextData *next;
}

class CW3DTextMgr
{
public:
	CW3DTextMgr(){};
	~CW3DTextMgr(){};

	void	Init( int mTexWidth, int nTexHeight );
	void	Release();

	void	DrawText( int nX, int nY, DWORD dwColor );



private:
	LPDIRECT3DTEXTURE8		m_pTexture;
};

void CW3DTextMgr::Init( int nTexWidth, int nTexHeight )
{
	if( m_pTexture != NULL )
		return;

	g_pd3dDevice->CreateTexture( nTexWidth, 
								 nTexHeight, 
								 0,
								 D3DUSAGE_WRITEONLY, 
								 D3DFMT_A4R4G4B4, 
								 D3DPOOL_MANAGED, 
								 &m_pTexture 
								 );
}

void CW3DTextMgr::Release()
{
	if( m_pTexture != NULL )
		SAFE_RELEASE( m_pTexture );
}
*/
/**
 * 폰트 클래스.
 * @ingroup		Render
 */
class CW3DFont : public IW3DFont
{
public:
	UNDER_MEMORY_MGR( CW3DFont );

	CW3DFont();
	~CW3DFont();

	BOOL	Init( const char* szName, const int nSize, const long lWeight, const BOOL bULine, const BOOL bItalic );
	void	Cleanup();
	BOOL	Restore();

	BOOL	GetStringSize( int* pnWidth, int* pnHeight, const char* szText );
	void	DrawText( const char* szText, const RECT* pRc, const DWORD dwAlign, const DWORD dwColor );

private:
	LOGFONT			m_LogFont;		///< 폰트 설정 데이터.
	ID3DXFont*		m_pFont;		///< D3DX 폰트 객체.
}; //class CW3DFont


IMPLEMENT_COMPONENT( CW3DFont, Font );


/**
 * 생성자.
 */
CW3DFont::CW3DFont()
{
	m_pFont = NULL;
} //CW3DFont::CW3DFont

/**
 * 소멸자.
 */
CW3DFont::~CW3DFont()
{
	SAFE_RELEASE( m_pFont );
} //CW3DFont::~CW3DFont

/**
 * 폰트 초기화.
 */
BOOL CW3DFont::Init(
	const char*		szName,		///< [IN] 폰트 이름
	const int		nSize,		///< [IN] 폰트 크기
	const long		lWeight,	///< [IN] 두께
	const BOOL		bULine,		///< [IN] 밑줄
	const BOOL		bItalic		///< [IN] 기울임
	)
{
	Assert( g_pd3dDevice );

	SAFE_RELEASE( m_pFont );
	
	m_LogFont.lfHeight			= nSize;
	m_LogFont.lfWidth			= 0;
	m_LogFont.lfEscapement		= 0;
	m_LogFont.lfOrientation		= 0;
	m_LogFont.lfWeight			= lWeight;
	m_LogFont.lfItalic			= bItalic;
	m_LogFont.lfUnderline		= bULine;
	m_LogFont.lfStrikeOut		= FALSE;
	m_LogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_LogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;	
	strcpy( m_LogFont.lfFaceName, szName );

/*#ifdef _CHINAVERSION

	m_LogFont.lfCharSet			= GB2312_CHARSET;	// Simplified Chinese
	m_LogFont.lfQuality			= DEFAULT_QUALITY;
	m_LogFont.lfPitchAndFamily	= FIXED_PITCH | FF_DONTCARE;
#else*/

	m_LogFont.lfCharSet			= HANGUL_CHARSET;			// Korean
	m_LogFont.lfQuality			= PROOF_QUALITY;
	m_LogFont.lfPitchAndFamily	= FIXED_PITCH | FF_MODERN;

//#endif		

	HRESULT		hr;
	hr = D3DXCreateFontIndirect( g_pd3dDevice, &m_LogFont, &m_pFont );
	if( FAILED( hr ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DFont::Init

/**
 * 클린업.
 */
void CW3DFont::Cleanup()
{
	Assert( g_pd3dDevice );
	SAFE_RELEASE( m_pFont );
} //CW3DFont::Cleanup

/**
 * 폰트를 원상태로 복구한다.
 */
BOOL CW3DFont::Restore()
{
	Assert( g_pd3dDevice );

	SAFE_RELEASE( m_pFont );

	HRESULT		hr;
	hr = D3DXCreateFontIndirect( g_pd3dDevice, &m_LogFont, &m_pFont );
	if( FAILED( hr ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DFont::Restore

/**
 * 문자열 사이즈.
 */
BOOL CW3DFont::GetStringSize( int* pnWidth, int* pnHeight, const char* szText )
{
	if( !szText || !m_pFont )
	{
		Assert( !"없당" );
		return FALSE;
	} //if
	
	if( FAILED( m_pFont->Begin() ) )	return FALSE;

	RECT	rcText;
	if( FAILED( m_pFont->DrawText( szText, -1, &rcText, DT_CALCRECT, 0xFFFFFFFF ) ) )
	{
		Assert( !"문자열 사이즈 실패" );
		return FALSE;
	}
	m_pFont->End();

	*pnWidth = rcText.right - rcText.left;
	*pnHeight = rcText.bottom - rcText.top;
	return TRUE;
} //CW3DFont::GetStringSize

/**
 * 문자열을 출력한다.
 */
void CW3DFont::DrawText(
	const char*		szText,		///< [IN] 문자열
	const RECT*		pRc,		///< [IN] 출력 영역
	const DWORD		dwAlign,	///< [IN] 정렬
	const DWORD		dwColor		///< [IN] 글자색
	)
{
	if( !szText || !m_pFont )
	{
		Assert( !"없당" );
		return;
	} //if

	if( FAILED( m_pFont->Begin() ) )	return;

	D3D::ZEnable( FALSE );
	D3D::ZWriteEnable( FALSE );

	D3D::Lighting( FALSE );

	D3D::AlphaBlend( TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	D3D::AlphaTest( FALSE );

	m_pFont->DrawText( szText, -1, (RECT*)pRc, dwAlign | DT_NOCLIP, dwColor );
	//m_pFont->DrawText( szText, -1, (RECT*)pRc, dwAlign | DT_NOCLIP, dwColor );
	m_pFont->End();
} //CW3DFont::DrawText
