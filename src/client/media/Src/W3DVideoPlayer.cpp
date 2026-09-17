/**
 * 비디오 플레이어.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-08-28 11:03a $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Media/src/W3DVideoPlayer.cpp $
 * 
 * 8     03-08-28 11:03a Sckyj
 * 
 * 7     03-04-10 6:28a Ahastudio
 * <HR>
 *
 * @file	W3DVideoPlayer.cpp
 */


#include <dshow.h>
#include <malloc.h>
#include "Media_Export.h"



/**
 * 비디오 플레이어.
 * @ingroup		Media
 */
class CW3DVideoPlayer : public IW3DVideoPlayer
{
public:
	UNDER_MEMORY_MGR( CW3DVideoPlayer );

	CW3DVideoPlayer();
	~CW3DVideoPlayer();

	BOOL	Init( const HWND hWnd );

	BOOL	Play( const char* szFileName, const BOOL bLoop );
	void	Stop();

	void	Update();

	BOOL	IsPlaying();

private:
	HWND			m_hWnd;

	IGraphBuilder*	m_pGraphBuilder;
	IMediaControl*	m_pMediaControl;
	IMediaEventEx*	m_pMediaEvent;
	IMediaSeeking*	m_pMediaSeeking;
	IVideoWindow*	m_pVideoWindow;
	IBasicVideo*	m_pBasicVideo;
	IBasicAudio*	m_pBasicAudio;

	BOOL			m_bLoop;
	BOOL			m_bPlaying;
}; //class CW3DVideoPlayer


IMPLEMENT_COMPONENT( CW3DVideoPlayer, VideoPlayer );


/**
 * 생성자.
 */
CW3DVideoPlayer::CW3DVideoPlayer()
:	m_hWnd			( NULL ),
	m_pGraphBuilder	( NULL ),
	m_pMediaControl	( NULL ),
	m_pMediaEvent	( NULL ),
	m_pMediaSeeking	( NULL ),
	m_pVideoWindow	( NULL ),
	m_pBasicVideo	( NULL ),
	m_pBasicAudio	( NULL ),
	m_bLoop			( FALSE ),
	m_bPlaying		( FALSE )
{
	CoInitialize( NULL );
} //CW3DVideoPlayer::CW3DVideoPlayer

/**
 * 소멸자.
 */
CW3DVideoPlayer::~CW3DVideoPlayer()
{
	Stop();

	SAFE_RELEASE( m_pBasicAudio   );
	SAFE_RELEASE( m_pBasicVideo   );
	SAFE_RELEASE( m_pVideoWindow  );
	SAFE_RELEASE( m_pMediaSeeking );
	SAFE_RELEASE( m_pMediaEvent   );
	SAFE_RELEASE( m_pMediaControl );
	SAFE_RELEASE( m_pGraphBuilder );

	CoUninitialize();
} //CW3DVideoPlayer::~CW3DVideoPlayer

/**
 * 비디오 플레이어를 초기화한다.
 */
BOOL CW3DVideoPlayer::Init( const HWND hWnd )
{
	HRESULT hr;

	m_hWnd = hWnd;

	hr = CoCreateInstance(
			CLSID_FilterGraph,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IGraphBuilder,
			(void**)&m_pGraphBuilder );

	if( FAILED( hr ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DVideoPlayer::Init

/**
 * 비디오를 보여준다.
 */
BOOL CW3DVideoPlayer::Play(
	const char*		szFileName,
	const BOOL		bLoop
	)
{
	Stop();		// 먼저 플레이 되던 게 있으면 정지시킨다.

	// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	Assert( m_pGraphBuilder );

	HRESULT		hr;

	m_bLoop = bLoop;

	// 파일명을 UNICODE로 변환 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	WCHAR	wFileName[MAX_PATH];

	DWORD	dwAttr = GetFileAttributes( szFileName );
	if( -1 == dwAttr )
	{	// 파일이 존재하지 않음.
		return FALSE;
	} //if

	MultiByteToWideChar( CP_ACP, 0, szFileName, -1, wFileName, MAX_PATH );

	// Streaming 세팅 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pGraphBuilder->RenderFile( wFileName, NULL );
	if( FAILED( hr ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	// Media Control 생성 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pGraphBuilder->QueryInterface( IID_IMediaControl, (void**)&m_pMediaControl );
	if( FAILED( hr ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	// Media Event 생성 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pGraphBuilder->QueryInterface( IID_IMediaEventEx, (void**)&m_pMediaEvent );
	if( FAILED( hr ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	// Media Seeking 생성 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pGraphBuilder->QueryInterface( IID_IMediaSeeking, (void**)&m_pMediaSeeking );
	if( FAILED( hr ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	// Video Window 생성 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pGraphBuilder->QueryInterface( IID_IVideoWindow, (void**)&m_pVideoWindow );
	if( FAILED( hr ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	// Basic Video 생성 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pGraphBuilder->QueryInterface( IID_IBasicVideo, (void**)&m_pBasicVideo );
	if( FAILED( hr ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	// Basic Audio 생성 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pGraphBuilder->QueryInterface( IID_IBasicAudio, (void**)&m_pBasicAudio );
	if( FAILED( hr ) )
	{
		Assert( 0 );
		return FALSE;
	} //if

	// Video Window 세팅 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pVideoWindow->put_Owner( (OAHWND)m_hWnd );
	if( FAILED( hr ) )
	{
		Assert( !"Video Window Owener 설정 실패" );
		return FALSE;
	} //if

	hr = m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
	if( FAILED( hr ) )
	{
		Assert( !"Video Window 스타일 설정 실패" );
		return FALSE;
	} //if

	RECT	rc;
	GetClientRect( m_hWnd, &rc );

	hr = m_pVideoWindow->SetWindowPosition( rc.left, rc.top, rc.right, rc.bottom );
	if( FAILED( hr ) )
	{
		Assert( !"Video Window 위치 설정 실패" );
		return FALSE;
	} //if

	// Full Screen Mode !!!
//	LONG	lMode = OATRUE;
//	hr = m_pVideoWindow->put_FullScreenMode( lMode );

	// Play -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	hr = m_pMediaControl->Run();
	if( FAILED( hr ) )
	{
		Assert( !"동영상 플레이 실패" );
		return FALSE;
	} //if

	m_bPlaying = TRUE;

	return TRUE;
} //CW3DVideoPlayer::Play

//
// CW3DVideoPlayer::Stop
//
void CW3DVideoPlayer::Stop()
{
 	if( m_pMediaControl )
	{
		m_pMediaControl->Stop();
	}

	SAFE_RELEASE( m_pBasicAudio   );
	SAFE_RELEASE( m_pBasicVideo   );
	SAFE_RELEASE( m_pVideoWindow  );
	SAFE_RELEASE( m_pMediaSeeking );
	SAFE_RELEASE( m_pMediaEvent   );
	SAFE_RELEASE( m_pMediaControl );

	// 무조건 Loop 없음...으로 만들어서 만일의 사태를 막는다.
	m_bLoop = FALSE;
}

//
// CW3DVideoPlayer::Update
//
void CW3DVideoPlayer::Update()
{
	Assert( m_pMediaControl );
	Assert( m_pMediaEvent   );

	HRESULT hr;
	long lEventCode, lParam1, lParam2;

	hr = m_pMediaEvent->GetEvent( &lEventCode, &lParam1, &lParam2, 0 );
	if( FAILED( hr ) )
	{
		return;
	}

	if( EC_COMPLETE == lEventCode )
	{
		if( m_bLoop )
		{
			LONGLONG pos = 0;
			m_pMediaSeeking->SetPositions( &pos, AM_SEEKING_AbsolutePositioning, NULL, AM_SEEKING_NoPositioning );

			hr = m_pMediaControl->Run();
			if( FAILED( hr ) )
			{
				m_bPlaying = FALSE;
				return;
			}
		}
		else
		{
			m_bPlaying = FALSE;
		}
	}
}

/**
 * 플레이 중인지 얻는다.
 */
BOOL CW3DVideoPlayer::IsPlaying()
{
	return m_bPlaying;
} //CW3DVideoPlayer::IsPlaying
