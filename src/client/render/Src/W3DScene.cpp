/**
 * Scene 구현부 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-08-03 10:01p $
 * @version	$Revision: 90 $
 *
 * <HR>
 * $Log: /Render/Src/W3DScene.cpp $
 * 
 * 90    04-08-03 10:01p Paran
 * 
 * 89    04-05-28 1:38p Sckyj
 * D3D::SetSoftwareVertexProcessing() 함수 사용안하도록 수정
 * 
 * 88    04-05-10 7:32p Kjmgo
 * Mix Vertex Processing 적용
 * 
 * 87    04-03-25 4:33p Kjmgo
 * 
 * 86    04-02-10 3:28p Paran
 * 
 * 85    04-01-14 9:46a Kjmgo
 * FPS를 얻어 오는 함수 추가
 * 
 * 84    03-12-17 11:38p Kjmgo
 * Alt + Tab 지원
 * 
 * 83    03-12-14 3:09a Paran
 * 
 * 82    03-12-02 11:56a Sckyj
 * InitFont()함수 수정 - CHINAVERSION에서는 중문 CHASRSET사용
 * 
 * 81    03-11-13 5:46p Paran
 * 
 * 80    03-11-10 5:34p Kjmgo
 * Gamma Ramp 조절 함수 수정
 * 
 * 79    03-10-15 6:42p Kjmgo
 * 라이트 관리자 추가 ^-^ 
 * 
 * 78    03-08-29 9:19a Kjmgo
 * Animation 바꿈
 * 
 * 77    03-08-28 2:45p Paran
 * 정만 : Animation Mgr 삭제
 * 
 * 75    03-08-05 5:18p Admin
 * 
 * 74    03-07-18 1:56p Paran
 * 
 * 73    03-07-18 3:32a Kjmgo
 * 
 * 72    03-07-16 4:30p Kjmgo
 * 
 * 71    03-07-16 11:52a Paran
 * 
 * 70    03-05-24 5:38a Ahastudio
 *
 * 68    03-05-17 11:07a Ahastudio		Capture() 함수 추가.
 *
 * 64    03-04-18 4:49p Ahastudio		Restore 함수 분리. Device 복구 실패할 경우, 다른 객체의 복구 시도를 막는다.
 *
 * 56    03-03-19 1:46a Paran			폰트 맵 추가.
 *
 * 55    03-03-18 11:32p Ahastudio		SetFog() 변경.
 *
 * 52    03-03-15 7:39a Ahastudio		선형 안개로 수정.
 *
 * 51    03-03-14 8:53p Ahastudio		DesktopMode의 위치 변경.
 *
 * 50    03-03-13 4:27p Ahastudio		텍스처 관리자 사용. 긴 함수 분리.
 * <HR>
 *
 * @file	W3DScene.cpp
 */


#include "Render_Export.h"
#include "Texture.h"
#include "Animation.h"
#include <stdio.h>
#include <string>
#include <map>
#include <list>

using std::string;
using std::map;


DWORD GetDXVersion();		///< DirectX 버전을 구한다. ( ex. 0x0801 )


D3DDISPLAYMODE			g_d3dDesktopMode;			///< 데스크탑 모드
LPDIRECT3DDEVICE8		g_pd3dDevice = NULL;		///< Direct3D Device
Frustum					g_Frustum;					///< View Frustum

typedef map<string, IW3DFont*>		MAP_FONT;		///< 외부폰트 맵
typedef std::list<IW3DShadow*>		LIST_SHADOW;	///< 그림자중, 수동으로 복구 해야 하는 리스트


/**
 * Scene 클래스.
 * @ingroup		Render
 */
class CW3DScene : public IW3DScene
{
	SINGLETON_COMPONENT( CW3DScene );

public:
	UNDER_MEMORY_MGR( CW3DScene );

	/**
	 * 생성자.
	 */
	CW3DScene()
	:	m_hWnd				( NULL ),
		m_pD3D				( NULL ),
		m_dwSavedStateBlock	( NULL ),
		m_pAnimationMgr		( NULL ),
		m_dwClearColor		( 0x00000000 ),
		m_pTextureMgr		( NULL ),
		m_pFont				( NULL ),
		m_fFPS				( 0 ),
		m_dwFrameOldTime	( 0 ),
		m_dwFrame			( 0 ),
		m_bOutputFPS		( FALSE )
	{
		g_pd3dDevice		= NULL;
	} //CW3DScene

	/**
	 * 소멸자.
	 */
	~CW3DScene()
	{
		SAFE_RELEASE( m_pAnimationMgr );
		SAFE_RELEASE( m_pFont );
		SAFE_DELETE( m_pTextureMgr );

		RestoreGamma();
		ReleaseFonts();

		if( m_dwSavedStateBlock )
		{
//			g_pd3dDevice->DeleteStateBlock( m_dwSavedStateBlock );
			m_dwSavedStateBlock = NULL;
		} //if

		SAFE_RELEASE( g_pd3dDevice );
		SAFE_RELEASE( m_pD3D );
	} //~CW3DScene

	/**
	 * Scene을 초기화한다.
	 *
	 * @return		초기화 성공 여부
	 */
	BOOL	Init(
		const HWND	hWnd,			///< [IN] 윈도우 핸들
		const int	nWidth,			///< [IN] 가로 해상도
		const int	nHeight,		///< [IN] 세로 해상도
		const int	nColorDepth,	///< [IN] Color Depth (전체 화면일 때만 적용됨)
		const BOOL	bFullscreen		///< [IN] 전체 화면 여부
		)
	{
		HRESULT		hr;

		m_hWnd			= hWnd;
		m_nWidth		= nWidth;
		m_nHeight		= nHeight;
		m_nColorDepth	= nColorDepth;
		m_bFullscreen	= bFullscreen;

		if( GetDXVersion() < 0x0800 )
		{
			throw W3DException( "DirectX 8.0이 필요합니다." );
			return FALSE;
		} //if

		m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
		if( NULL == m_pD3D )
		{
			throw W3DException( "DirectX 초기화 실패 (DirectX)" );
			return FALSE;
		} //if

		hr = m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3dCaps );
		if( FAILED( hr ) )
		{
			Assert( !"D3D->GetDeviceCaps 실패" );
			return FALSE;
		} //if

		hr = m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &g_d3dDesktopMode );
		if( FAILED( hr ) )
		{
			throw W3DException( "데스크탑 모드 얻기 실패" );
			return FALSE;
		} //if

		if( !CreateDevice() )
		{
			throw W3DException( "DirectX 초기화 실패 (D3DDevice)" );
			return FALSE;
		} //if

		SetDefaultDevice();

		SetViewport( 0, 0, m_nWidth, m_nHeight );

		CreateSavedStateBlock();

		m_pTextureMgr = new CTextureMgr( g_pd3dDevice );

		if( !InitFont( "굴림" ) )
		{
			throw W3DException( "폰트 초기화 실패" );
			return FALSE;
		} //if

		m_pAnimationMgr = MAKE_COMPONENT( AnimationMgr );

		if( !m_pAnimationMgr )
		{
			throw W3DException( "애니메이션 매니저 초기화 실패" );
			return FALSE;
		} //if


		DEBUG_OUT( "그래픽 초기화 성공..." );

		g_pd3dDevice->GetGammaRamp( &m_OldGammaRamp );

		return TRUE;
	} //Init

	/**
	 * 텍스쳐 레벨 설정
	 */
	void	SetTexLevel( BYTE btLevel )
	{
		if( !m_pTextureMgr )
		{
			Assert( m_pTextureMgr );
			return;
		} //if
		m_pTextureMgr->SetLevel( btLevel );
	} //SetTexLevel

	/**
	 * 감마를 컨트롤 한다.
	 */
	BOOL	SetGammaControl( float fRed, float fGreen, float fBlue )
	{
		WORD wMixVal = 0xFFFF;
		
		for( int i = 0 ; i < 256 ; i++ )
		{
			m_GammaRamp.red[i] = m_GammaRamp.red[i+256] = m_GammaRamp.red[i+512] = 
			(WORD)min(65535, max(0, pow((i+1) / 256.0, fRed) * 65535 + 0.5));

			m_GammaRamp.green[i] = m_GammaRamp.green[i+256] = m_GammaRamp.green[i+512] = 
			(WORD)min(65535, max(0, pow((i+1) / 256.0, fGreen) * 65535 + 0.5));

			m_GammaRamp.blue[i] = m_GammaRamp.blue[i+256] = m_GammaRamp.blue[i+512] = 
			(WORD)min(65535, max(0, pow((i+1) / 256.0, fBlue) * 65535 + 0.5));
		} //for


		g_pd3dDevice->SetGammaRamp( 0, &m_GammaRamp );
			
		return TRUE;
	} //SetGammaControl

	/**
	 * 감마를 초기 설정으로 돌린다.
	 */
	BOOL	RestoreGamma()
	{
		g_pd3dDevice->SetGammaRamp( D3DSGR_CALIBRATE  , &m_OldGammaRamp );
		return FALSE;
	} //RestoreGamma

	/**
	 * Frame 시작.
	 *
	 * @return	Frame 시작 성공 여부
	 */
	BOOL	BeginFrame()
	{
		HRESULT		hr;
	
		hr = g_pd3dDevice->TestCooperativeLevel();

		if( FAILED( hr ) )
		{
			if( D3DERR_DEVICELOST == hr )
			{	// Device가 없어졌음에도 불구하고, 현시점에서 복원할 수 없는 경우
				// ( ex. Full Screen Device가 Focus를 잃은 경우 )
				return FALSE;
			}
			else if( D3DERR_DEVICENOTRESET == hr )
			{
				Restore();
				return FALSE;
			}
			else
			{
				Assert( !"CW3DScene::BeginFrame - TestCooperativeLevel - 알 수 없는 에러" );
				return FALSE;
			} //if..else if..else..
		} //if

		g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_dwClearColor, 1.0F, 0 );

		hr = g_pd3dDevice->BeginScene();
		if( FAILED( hr ) )
		{
			return FALSE;
		} //if

		return TRUE;
	} //BeginFrame

	/**
	 * Frame 끝.
	 *
	 * @return	화면 출력 성공 여부
	 */
	BOOL	EndFrame()
	{
		Assert( g_pd3dDevice );

		CalculateFrameRate();

		if( m_bOutputFPS )
		{
			DrawText( 4, 4, 0x80FFFFFF, "[ FPS : %3.1f ]", m_fFPS );
		} //if

		g_pd3dDevice->EndScene();

		Show();

		return TRUE;
	} //EndFrame

	/**
	 * 뷰포트를 설정한다.
	 */
	void	SetViewport( int nX, int nY, int nWidth, int nHeight )
	{
		Assert( g_pd3dDevice );

		SetRect( &m_rcViewport, nX, nY, nX + nWidth, nY + nHeight );

		m_Viewport.X		= nX;
		m_Viewport.Y		= nY;
		m_Viewport.Width	= nWidth;
		m_Viewport.Height	= nHeight;
		m_Viewport.MinZ		= 0.0F;
		m_Viewport.MaxZ		= 1.0F;

		g_pd3dDevice->SetViewport( &m_Viewport );
	} //SetViewport

	/**
	 * 뷰포트.
	 */
	void	GetViewport( int& nWidth, int& nHeight )
	{
		nWidth	= m_Viewport.Width;
		nHeight = m_Viewport.Height;
	} //GetViewPort

	/**
	 * 카메라를 설정한다.
	 *
	 * @param	vEye		[IN] 카메라의 위치.
	 * @param	vAt			[IN] 카메라가 쳐다볼 위치.
	 * @param	vUp			[IN] 카메라의 Up 벡터.
	 */
	void	LookAt( const Vector3& vEye, const Vector3& vAt, const Vector3& vUp )
	{
		m_matView.SetView( vEye, ( vAt - vEye ), vUp );
		g_pd3dDevice->SetTransform( D3DTS_VIEW, reinterpret_cast<D3DMATRIX*>( &m_matView ) );
		g_Frustum.SetView( m_matView );
	} //LookAt

	/**
	 * 프로젝션을 설정한다.
	 *
	 * @param	fFOV			[IN] Field of View
	 * @param	fNearPlane		[IN] 근단면
	 * @param	fFarPlane		[IN] 원단면
	 */
	void	SetProjection( float fFOV, float fNearPlane, float fFarPlane )
	{
		Assert( g_pd3dDevice );

		float fAspect;

		if( m_bFullscreen )
		{
			fAspect = (float)m_nHeight / (float)m_nWidth;
		}
		else
		{
			fAspect = (float)( m_rcViewport.bottom - m_rcViewport.top )
					/ (float)( m_rcViewport.right  - m_rcViewport.left );
		} //if..else..

		m_matProj.SetProjection( fFOV, fAspect, fNearPlane, fFarPlane );
		g_pd3dDevice->SetTransform( D3DTS_PROJECTION, reinterpret_cast<D3DMATRIX*>( &m_matProj ) );
		g_Frustum.SetProjection( fFOV, fAspect, fNearPlane, fFarPlane );
	} //SetProjection

	/**
	 * 3D 좌표를 화면상의 좌표로 변환한다.
	 *
	 * @param	vPos		[IN] 3D 좌표.
	 *
	 * @return	화면상의 좌표.
	 */
	Vector3		GetScreenPos( const Vector3& vPos )
	{
		Vector3		point = vPos * m_matView * m_matProj;

		point.x = (  point.x + 1 ) * m_Viewport.Width  / 2;
		point.y = ( -point.y + 1 ) * m_Viewport.Height / 2;

		return point;
	} //GetScreenPos

	/**
	 * 안개 설정.
	 *
	 * @param	dwColor		[IN] 안개 색
	 * @param	fStart		[IN] 시작 거리
	 * @param	fEnd		[IN] 끝 거리
	 */
	void	SetFog( DWORD dwColor, float fStart, float fEnd )
	{
		Assert( g_pd3dDevice );

		m_dwClearColor = dwColor;

		g_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,		dwColor );
		g_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,	D3DFOG_LINEAR );
		g_pd3dDevice->SetRenderState( D3DRS_FOGSTART,		*reinterpret_cast<DWORD*>( &fStart ) );
		g_pd3dDevice->SetRenderState( D3DRS_FOGEND,			*reinterpret_cast<DWORD*>( &fEnd ) );
	} //SetFog

	/**
	 * View Frustum을 얻는다.
	 *
	 * @return	View Frustum
	 */
	Frustum&	GetFrustum()
	{
		return g_Frustum;
	} //GetFrustum

	/**
	 * 피킹용 반직선을 얻는다.
	 *
	 * @param	nX			[IN] 화면상의 X 좌표.
	 * @param	nY			[IN] 화면상의 Y 좌표.
	 * @param	pvOrig		[OUT] 반직선의 시작점.
	 * @param	pvDir		[OUT] 반직선의 방향.
	 */
	void	GetPickRay( int nX, int nY, Vector3* pvOrig, Vector3* pvDir )
	{
		float	x =  ( ( ( nX - m_Viewport.X ) * 2.0F / m_Viewport.Width  ) - 1.0F );
		float	y = -( ( ( nY - m_Viewport.Y ) * 2.0F / m_Viewport.Height ) - 1.0F );

		Vector3		v;
		v.x = ( x - m_matProj._31 ) / m_matProj._11;
		v.y = ( y - m_matProj._32 ) / m_matProj._22;
		v.z =  1.0F;

		Matrix4&	m = m_matView.Inverse();

		pvDir->x = v.x * m._11 + v.y * m._21 + v.z * m._31;
		pvDir->y = v.x * m._12 + v.y * m._22 + v.z * m._32;
		pvDir->z = v.x * m._13 + v.y * m._23 + v.z * m._33;

		pvOrig->x = m._41;
		pvOrig->y = m._42;
		pvOrig->z = m._43;
	} //GetPickRay

	/**
	 * 문자열 사이즈.
	 */
	void	GetStringSize( int& nWidth, int& nHeight, const char* szText )
	{
		RECT	rcText;
		if( FAILED( m_pFont->DrawText( szText, -1, &rcText, DT_CALCRECT, 0xFFFFFFFF ) ) )
		{
			Assert( !"문자열 사이즈 실패" );
			return;
		}

		nWidth  = rcText.right - rcText.left;
		nHeight = rcText.bottom - rcText.top;
	} //GetStringSize
	
	/**
	 * 문자열를 출력한다.
	 *
	 * @param	nX			[IN] X 좌표
	 * @param	nY			[IN] Y 좌표
	 * @param	dwColor		[IN] 글자색
	 * @param	szFormat	[IN] 문자열 포맷
	 * @param	...			[IN] 인자들
	 */
	void	DrawText( int nX, int nY, DWORD dwColor, const char* szFormat, ... )
	{
		Assert( g_pd3dDevice );
		Assert( m_dwSavedStateBlock );
		Assert( m_pFont );

		char		szText[256];

		va_list		argptr;
		va_start( argptr, szFormat );
		vsprintf( szText, szFormat, argptr );
		va_end( argptr );

		m_pFont->Begin();

		g_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );

		g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );

		RECT	rc;
		SetRect( &rc, nX, nY, nX + strlen( szText ) * 9, nY + 12 );

		m_pFont->DrawText( szText, -1, &rc, DT_LEFT | DT_TOP, dwColor );

		g_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );

		m_pFont->End();
	} //DrawText

	void	DrawText( int nX, int nY, int nWidth, int nHeight,
					  DWORD dwColor, const char* szFormat, ... )
	{
		Assert( g_pd3dDevice );
		Assert( m_dwSavedStateBlock );
		Assert( m_pFont );

		char		szText[256];

		va_list		argptr;
		va_start( argptr, szFormat );
		vsprintf( szText, szFormat, argptr );
		va_end( argptr );

		m_pFont->Begin();

		g_pd3dDevice->CaptureStateBlock( m_dwSavedStateBlock );

		g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );

		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,  FALSE );
		g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE );

		RECT	rc;
		SetRect( &rc, nX, nY, nX + nWidth, nY + nHeight );

		//m_pFont->DrawText( szText, -1, &rc, DT_LEFT | DT_TOP, dwColor );
		m_pFont->DrawText( szText, -1, &rc, DT_LEFT | DT_TOP | DT_WORDBREAK, dwColor );

		g_pd3dDevice->ApplyStateBlock( m_dwSavedStateBlock );

		m_pFont->End();
	} //DrawText

	/**
	 * 새로운 폰트를 추가한다.
	 */
	BOOL	AddFont(
		IW3DFont**		ppFont,			///< [OUT] 생성 추가한 폰트
		const char*		szKeyName,		///< [IN] 추가할 때 키 이름
		const char*		szFace,			///< [IN] 폰트
		const int		nSize,			///< [IN] 폰트 크기
		const long		lWeight,		///< [IN] 굵기
		const BOOL		bULine,			///< [IN] 밑줄
		const BOOL		bItalic			///< [IN] 기울임
		)
	{
		Assert( szKeyName );
		Assert( szFace );

		MAP_FONT::iterator	iterMap_Font = m_mapFont.find( szKeyName );

		if( iterMap_Font != m_mapFont.end() )
		{
			*ppFont = iterMap_Font->second;
			return TRUE;
		} //if

		IW3DFont*		pFont = MAKE_COMPONENT( Font );
		CHECK_PTR_RETURN( pFont );

		if( !pFont->Init( szFace, nSize, lWeight, bULine, bItalic ) )
		{
			Assert( !"폰트 생성 실패" );
			return FALSE;
		} //if

		m_mapFont.insert( std::make_pair( string( szKeyName ), pFont ) );

		if( ppFont )
		{
			*ppFont = pFont;
		} //if

		return TRUE;
	} //AddFont

	/**
	 * 폰트를 찾는다.
	 *
	 * @param	ppFont		[OUT] 찾은 폰트 / NULL
	 * @param	szKeyName	[IN] 찾을 키이름
	 */
	BOOL	FindFont( IW3DFont** ppFont, const char* szKeyName )
	{
		Assert( ppFont );
		Assert( szKeyName );

		MAP_FONT::iterator	itor = m_mapFont.find( szKeyName );
		if( itor == m_mapFont.end() )
		{
			Assert( !"생성되지 않은 폰트입니다." );
			return FALSE;
		} //if

		*ppFont = itor->second;

		return TRUE;
	} //FindFont

	/**
	 * Frame Rate를 보일지 설정한다.
	 *
	 * @param	bShow			[IN] FPS를 보일지 여부
	 */
	void		ShowFrameRate( BOOL bShow )
	{
		m_bOutputFPS = bShow;
	} //ShowFrameRate

	/**
	 * 게임 화면을 캡쳐한다.
	 *
	 * @param	szName		[IN] 파일 이름
	 *
	 * @return	화면 캡쳐 성공 여부
	 */
	BOOL	Capture( const char* szName, SYSTEMTIME* pSt )
	{
		int			nWidth	= GetSystemMetrics( SM_CXSCREEN );
		int			nHeight	= GetSystemMetrics( SM_CYSCREEN );

		HDC			hdc		= CreateDC( "DISPLAY", NULL, NULL, NULL );

		HBITMAP		hbmp	= CreateCompatibleBitmap( hdc, nWidth, nHeight );
		HDC			hdcBmp	= CreateCompatibleDC( hdc );
		SelectObject( hdcBmp, hbmp );

		StretchBlt( hdcBmp, 0, 0, nWidth, nHeight, hdc, 0, 0, nWidth, nHeight, SRCCOPY );

		if( pSt )
		{
			char		szDateTime[32];
			sprintf( 
				szDateTime, 
				"%02d-%02d-%02d, %02d:%02d:%02d", 
				pSt->wYear, pSt->wMonth, pSt->wDay, pSt->wHour, pSt->wMinute, pSt->wSecond 
				);
			TextOut( hdcBmp, 3, nHeight - 13, szDateTime, strlen( szDateTime ) );
		} //if

		FILE*		pFile = fopen( szName, "wb" );
		CHECK_PTR_RETURN( pFile );
		PBITMAPINFO	pBmp = CreateBitmapInfo( hbmp );
		CreateBitmapFile( pFile, pBmp, hbmp, hdc );
		fclose( pFile );
		return TRUE;
	} //Capture

	/**
	 * 비트맵 인포 생성.
	 */
	PBITMAPINFO			CreateBitmapInfo( HBITMAP hBMP )
	{
		BITMAP		bmp; 
		PBITMAPINFO pbmi; 
		WORD		wClrBits; 
		
		if ( !GetObject( hBMP, sizeof( BITMAP ), (LPSTR)&bmp ) ) 
		{
			return NULL;
		}
		
		wClrBits = (WORD)( bmp.bmPlanes * bmp.bmBitsPixel ); 
		if ( wClrBits == 1 )			wClrBits = 1; 
		else if ( wClrBits <= 4 ) 		wClrBits = 4; 
		else if ( wClrBits <= 8 ) 		wClrBits = 8; 
		else if ( wClrBits <= 16 )		wClrBits = 16; 
		else if ( wClrBits <= 24 )		wClrBits = 24; 
		else wClrBits = 32; 
		
		if ( wClrBits != 24 ) 
		{
			pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
			sizeof(BITMAPINFOHEADER) + 
			sizeof(RGBQUAD) * ( 1 << wClrBits )); 
		}
		else 
		{
			pbmi = (PBITMAPINFO) LocalAlloc(LPTR, 
			sizeof(BITMAPINFOHEADER)); 
		} //if.else
		
		pbmi->bmiHeader.biSize		= sizeof( BITMAPINFOHEADER ); 
		pbmi->bmiHeader.biWidth		= bmp.bmWidth; 
		pbmi->bmiHeader.biHeight	= bmp.bmHeight; 
		pbmi->bmiHeader.biPlanes	= bmp.bmPlanes; 
		pbmi->bmiHeader.biBitCount	= bmp.bmBitsPixel; 
		
		if ( wClrBits < 24 ) 	
			pbmi->bmiHeader.biClrUsed = ( 1 << wClrBits ); 
		
		pbmi->bmiHeader.biCompression = BI_RGB; 
		pbmi->bmiHeader.biSizeImage = 
			( ( pbmi->bmiHeader.biWidth * wClrBits + 31 ) & ~31 ) / 8
			* pbmi->bmiHeader.biHeight; 

		pbmi->bmiHeader.biClrImportant = 0; 
		return pbmi; 
	} //CreateBitmapInfo

	/**
	 * 비트맵 파일 생성.
	 */
	void	CreateBitmapFile( FILE* pFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC )
	{
		BITMAPFILEHEADER	hdr;	// 비트맵 파일 헤더 
		PBITMAPINFOHEADER	pbih;	// 비트맵 정보 헤더 
		LPBYTE				lpBits;	// 비트맵 데이터를 담을 메모리 포인터 
		
		pbih   = (PBITMAPINFOHEADER) pbi; 
		lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
		
		if ( !lpBits ) 
		{
			return;
		}
		
		if ( !GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS) ) 
		{
			return;
		}

		// 비트맵 파일은 반드시 "BM" 문자를 가지고 있어야 한다.
		hdr.bfType = 0x4d42; // 0x42 = "B" 0x4d = "M" 
		hdr.bfSize = (DWORD)( sizeof( BITMAPFILEHEADER) + 
			pbih->biSize + pbih->biClrUsed 
			* sizeof(RGBQUAD) + pbih->biSizeImage); 
		hdr.bfReserved1 = 0; // 반드시 0으로 해줘야 한다.
		hdr.bfReserved2 = 0; // 반드시 0으로 해줘야 한다.
		
		// 비트맵 데이터(색상들)의 시작 위치 offset을 계산한다. 
		hdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + 
			pbih->biSize + pbih->biClrUsed 
			* sizeof(RGBQUAD); 
		
		// BITMAPFILEHEADER를 파일로 쓴다. 
		fwrite( &hdr, 1, sizeof(BITMAPFILEHEADER), pFile );
		
		// BITMAPINFOHEADER와 RGBQUAD를 파일로 쓴다. 
		fwrite( pbih, 1, sizeof(BITMAPINFOHEADER), pFile );
		
		// 비트맵 데이터(색상들)를 파일로 쓴다. 
		fwrite( lpBits, 1, pbih->biSizeImage, pFile);
		
		// 메모리를 해제 한다. 
		GlobalFree((HGLOBAL)lpBits);
	} //CreateBitmapFile

private:
	/**
	 * Direct3D Device를 생성한다.
	 *
	 * @return	Device 생성 성공 여부.
	 */
	BOOL	CreateDevice()
	{
		HRESULT		hr;
		D3DFORMAT	d3dfmt;

		if( m_bFullscreen )
		{
			d3dfmt = GetPixelFormat();
		}
		else
		{
			d3dfmt = g_d3dDesktopMode.Format;
		} //if..else..

		if( D3DFMT_UNKNOWN == d3dfmt )
		{
			return FALSE;
		} //if

		SetPresentParameters( m_PresentParameters, m_bFullscreen, d3dfmt );

		DWORD		dwBehaviorFlags;

		if( m_d3dCaps.VertexShaderVersion < D3DVS_VERSION( 1, 0 ) )
		{
			DEBUG_OUT( "D3D - Software Vertex Processing" );
			dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
		else
		{
			DEBUG_OUT( "D3D - Mixed Vertex Processing" );
			dwBehaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
		} //if..else..

		dwBehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		hr = m_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			m_hWnd,
			dwBehaviorFlags,
			&m_PresentParameters,
			&g_pd3dDevice );

		if( FAILED( hr ) )
		{
			Assert( !"Direct3D Device 생성 실패" );
			return FALSE;
		} //if

		return TRUE;
	} //Init

	/**
	 * 픽셀 포맷을 얻는다.
	 */
	D3DFORMAT	GetPixelFormat()
	{
		HRESULT		hr;
		D3DFORMAT	d3dfmt;

		if( 16 == m_nColorDepth )
		{
			D3DFORMAT fmtArray[] =
			{
				D3DFMT_R5G6B5,
				D3DFMT_X1R5G5B5,
				D3DFMT_A1R5G5B5,
			};
			int numFmts = sizeof( fmtArray ) / sizeof( fmtArray[0] );

			for( int iFmt = 0 ; iFmt < numFmts ; iFmt++ )
			{
				hr = m_pD3D->CheckDeviceType(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						fmtArray[iFmt],
						fmtArray[iFmt],
						FALSE );

				if( SUCCEEDED( hr ) )
				{
					d3dfmt = fmtArray[iFmt];
					break;
				} //if
			} //for

			if( iFmt == numFmts )
			{
				Assert( !"Get Pixel Format Failed...." );
				return D3DFMT_UNKNOWN;
			} //if
		}
		else	if( 32 == m_nColorDepth )
		{
			D3DFORMAT fmtArray[] =
			{
				D3DFMT_X8R8G8B8,
				D3DFMT_A8R8G8B8,
			};
			int numFmts = sizeof( fmtArray ) / sizeof( fmtArray[0] );

			for( int iFmt = 0 ; iFmt < numFmts ; iFmt++ )
			{
				hr = m_pD3D->CheckDeviceType(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						fmtArray[iFmt],
						fmtArray[iFmt],
						FALSE );

				if( SUCCEEDED( hr ) )
				{
					d3dfmt = fmtArray[iFmt];
					break;
				} //if
			} //for

			if( iFmt == numFmts )
			{
				Assert( !"Get Pixel Format Failed...." );
				return D3DFMT_UNKNOWN;
			} //if
		}
		else
		{
			Assert( !"지원하지 않는 컬러입니다" );
			return D3DFMT_UNKNOWN;
		} //if..else if..else..

		return d3dfmt;
	} //GetPixelFormat

	/**
	 * Present Parameters를 설정한다.
	 */
	void	SetPresentParameters(
		D3DPRESENT_PARAMETERS&	d3dpp,			///< [OUT] Present Parameters
		const BOOL				bFullscreen,	///< [IN] 전체 화면 사용 여부
		const D3DFORMAT&		d3dfmt			///< [IN] 사용할 화면 픽셀 포맷
		)
	{
		if( bFullscreen )
		{
			ZeroMemory( &d3dpp, sizeof( D3DPRESENT_PARAMETERS ) );
			d3dpp.Windowed               = FALSE;
			d3dpp.BackBufferCount        = 1;
			d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			d3dpp.hDeviceWindow          = m_hWnd;
			d3dpp.BackBufferWidth        = m_nWidth;
			d3dpp.BackBufferHeight       = m_nHeight;
			d3dpp.BackBufferFormat       = d3dfmt;
			d3dpp.Flags				 = 0;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}
		else
		{
			ZeroMemory( &m_PresentParameters, sizeof( D3DPRESENT_PARAMETERS ) );
			d3dpp.Windowed               = TRUE;
			d3dpp.BackBufferCount        = 1;
			d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
			d3dpp.EnableAutoDepthStencil = TRUE;
			d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			d3dpp.hDeviceWindow          = NULL;
			d3dpp.BackBufferFormat       = d3dfmt;
		} //if..else..
	} //SetPresentParameters

	/**
	 * 기본적인 Device 설정을 한다.
	 */
	void	SetDefaultDevice()
	{
		// 기본적인 Material 세팅
		D3DMATERIAL8 mtrl;
		ZeroMemory( &mtrl, sizeof( D3DMATERIAL8 ) );
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0F;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0F;
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0F;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0F;
		g_pd3dDevice->SetMaterial( &mtrl );

		// Alpha Blend 관련 세팅
		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

		// Alpha Test 관련 세팅
		g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
		g_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

		// Texture 관련 세팅
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );

		// Light 관련 세팅
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,		TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE,	FALSE );//TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_AMBIENT,		0x00000000 );
	} //SetDefaultDevice

	/**
	 * 렌더링 상태 저장용 상태 블럭을 만든다.
	 */
	void	CreateSavedStateBlock()
	{
		g_pd3dDevice->BeginStateBlock();

			g_pd3dDevice->SetRenderState( D3DRS_ZENABLE,			D3DZB_TRUE );
			g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		TRUE );

			g_pd3dDevice->SetRenderState( D3DRS_LIGHTING,			FALSE );
			g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,			FALSE );

			g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
			g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA );
			g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA );

			g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE,	FALSE );

		g_pd3dDevice->EndStateBlock( &m_dwSavedStateBlock );
	} //CreateSavedStateBlock

	/**
	 * 전체 화면에서 렌더링 화면을 보여준다.
	 *
	 * @return	화면 출력 성공 여부.
	 */
	BOOL	Show()
	{
		Assert( m_pD3D );
		Assert( g_pd3dDevice );

		HRESULT		hr;

		if( m_bFullscreen )
		{
			hr = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
		}
		else
		{
			hr = g_pd3dDevice->Present( &m_rcViewport, &m_rcViewport, NULL, NULL );
		} //if..else..

		if( FAILED( hr ) )
		{
			return FALSE;
		} //if

		return TRUE;
	} //ShowFullscreen

	/**
	 * Direct3D 관련 객체들을 복구한다.
	 *
	 * @return	복구 성공 여부.
	 */
	BOOL	Restore()
	{
		Assert( g_pd3dDevice );

		HRESULT		hr;

		SAFE_RELEASE( m_pFont );
		CleanupFonts();
		CleanupShadow();
		//
		
		hr = g_pd3dDevice->Reset( &m_PresentParameters );

		if( FAILED( hr ) )
		{
			return FALSE;
		} //if

		InitFont( "굴림" );

		RestoreFonts();
		RestoreShadow();
		
		return TRUE;
	} //ShowFullscreen

	/**
	 * 기본 문자열 출력 폰트를 초기화한다.
	 *
	 * @param	szName		[IN] 폰트 이름
	 *
	 * @return	폰트 초기화 성공 여부
	 */
	BOOL	InitFont( const char* szName )
	{
		Assert( g_pd3dDevice );

		SAFE_RELEASE( m_pFont );

		LOGFONT LogicalFont;
	/*	
#ifdef _CHINAVERSION
		LogicalFont.lfHeight			= 12;
		LogicalFont.lfWidth				= 0;
		LogicalFont.lfEscapement		= 0;
		LogicalFont.lfOrientation		= 0;
		LogicalFont.lfWeight			= FW_NORMAL;
		LogicalFont.lfItalic			= FALSE;
		LogicalFont.lfUnderline			= FALSE;
		LogicalFont.lfStrikeOut			= FALSE;
		LogicalFont.lfCharSet			= IME_CMODE_ALPHANUMERIC;//GB2312_CHARSET;
		LogicalFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
		LogicalFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
		LogicalFont.lfQuality			= ANTIALIASED_QUALITY;
		LogicalFont.lfPitchAndFamily	= VARIABLE_PITCH;
		//strcpy( LogicalFont.lfFaceName, "NSimSun" );
		strcpy( LogicalFont.lfFaceName, "Arial" );
#else*/
		LogicalFont.lfHeight			= 12;
		LogicalFont.lfWidth				= 0;
		LogicalFont.lfEscapement		= 0;
		LogicalFont.lfOrientation		= 0;
		LogicalFont.lfWeight			= FW_NORMAL;
		LogicalFont.lfItalic			= FALSE;
		LogicalFont.lfUnderline			= FALSE;
		LogicalFont.lfStrikeOut			= FALSE;
		LogicalFont.lfCharSet			= HANGUL_CHARSET;
		LogicalFont.lfOutPrecision		= OUT_DEFAULT_PRECIS;
		LogicalFont.lfClipPrecision		= CLIP_DEFAULT_PRECIS;
		LogicalFont.lfQuality			= PROOF_QUALITY;
		LogicalFont.lfPitchAndFamily	= FIXED_PITCH | FF_MODERN;
		strcpy( LogicalFont.lfFaceName, szName );
//#endif	

		HRESULT		hr;

		hr = D3DXCreateFontIndirect( g_pd3dDevice, &LogicalFont, &m_pFont );
		if( FAILED( hr ) )
		{
			return FALSE;
		} //if

		return TRUE;
	} //InitFont

	/**
	 * 폰트들 클린업.
	 */
	void	CleanupFonts()
	{
		MAP_FONT::iterator	itor = m_mapFont.begin();
		while( itor != m_mapFont.end() )
		{
			IW3DFont*	pFont = itor->second;
			pFont->Cleanup();
			itor++;
		} //while
	} //CleanupFonts

	/**
	 *
	 */
	void	CleanupShadow()
	{
		LIST_SHADOW::iterator itor = m_listShadow.begin();
		IW3DShadow* p = NULL;

		while( itor != m_listShadow.end() )
		{
			p = *itor;

			if( p )
				p->Cleanup();

			itor++;
		} //while
	} //CleanupShadow

	/**
	 * 폰트를 다시 생성한다.
	 */
	void	RestoreFonts()
	{
		MAP_FONT::iterator	itor = m_mapFont.begin();

		while( itor != m_mapFont.end() )
		{
			IW3DFont*	pFont = itor->second;
			Assert( pFont );
			pFont->Restore();
			itor++;
		} //for
	} //RestoreFonts

	/**
	 * 폰트를 모두 해제한다.
	 */
	void	ReleaseFonts()
	{
		MAP_FONT::iterator	itor = m_mapFont.begin();
		MAP_FONT::iterator	itorNext;

		while( itor != m_mapFont.end() )
		{
			itorNext = itor;
			itorNext++;

			IW3DFont*	pFont = itor->second;
			SAFE_RELEASE( pFont );
			m_mapFont.erase( itor );

			itor = itorNext;
		} //while
	} //ReleaseFonts

	/**
	 * 프레임 레이트를 계산한다.
	 */
	void	CalculateFrameRate()
	{
		DWORD	dwFrameTime = GetTickCount() - m_dwFrameOldTime;

		if( dwFrameTime > 1000 )
		{
			m_fFPS				= m_dwFrame * 1000.0F / dwFrameTime;
			m_dwFrameOldTime	= GetTickCount();
			m_dwFrame			= 0;
		} //if

		m_dwFrame++;
	} //CalculateFrameRate

	/**
	 *
	 */
	float	GetFPS()
	{
		return m_fFPS;
	} //GetFPS

	/**
	 *
	 */
	int		GetMaxActiveLights()
	{
		return m_d3dCaps.MaxActiveLights;
	} //GetActiveMaxLight

	/**
	 *
	 */
	void	AddShadow( IW3DShadow* pShadow )
	{
		if( pShadow )
		{		
			m_listShadow.push_back( pShadow );
		} //if	
	} //AddShadow

	/**
	 *
	 */
	void RestoreShadow()
	{
		LIST_SHADOW::iterator itor = m_listShadow.begin();
		IW3DShadow* p = NULL;

		while( itor != m_listShadow.end() )
		{
			p = *itor;

			if( p )
				p->Restore();

			itor++;
		} //while
	} //RestoreShadow

private:
	// Variables -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	HWND					m_hWnd;					///< 렌더러 윈도우 핸들
	int						m_nWidth;				///< 가로 해상도
	int						m_nHeight;				///< 세로 해상도
	int						m_nColorDepth;			///< 컬러 깊이
	BOOL					m_bFullscreen;			///< 전체 화면 여부
	// Direct3D Device -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	LPDIRECT3D8				m_pD3D;					///< Direct3D 객체
	D3DCAPS8				m_d3dCaps;				///< 디바이스 고유 정보
	D3DPRESENT_PARAMETERS	m_PresentParameters;	///< Present 파라메터
	DWORD					m_dwSavedStateBlock;	///< 상태 저장 블럭
	// Viewport  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	DWORD					m_dwClearColor;			///< Clear 색상
	RECT					m_rcViewport;			///< 뷰포트 영역
	D3DVIEWPORT8			m_Viewport;				///< 뷰포트
	// Transform -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	Matrix4					m_matView;				///< 뷰 행렬
	Matrix4					m_matProj;				///< 프로젝션 행렬
	// Texture -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	CTextureMgr*			m_pTextureMgr;			///< 텍스처 관리자
	// Animation -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	IW3DAnimationMgr*		m_pAnimationMgr;		///< 
	// Font  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	ID3DXFont*				m_pFont;				///< 폰트
	MAP_FONT				m_mapFont;				///< 폰트 리스트
	// FrameRate -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	float					m_fFPS;					///< Frame Rate
	DWORD					m_dwFrameOldTime;		///< 전 Frame의 시간
	DWORD					m_dwFrame;				///< Frame 횟수
	BOOL					m_bOutputFPS;			///< FPS 출력 여부

	D3DGAMMARAMP			m_GammaRamp;
	D3DGAMMARAMP			m_OldGammaRamp;

	
	LIST_SHADOW				m_listShadow;

}; //class CW3DScene


IMPLEMENT_SINGLETON_COMPONENT( CW3DScene, Scene );
