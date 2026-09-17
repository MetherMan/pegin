/**
 * 인트로 프레임.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-06-23 12:08p $
 * @version	$Revision: 45 $
 *
 * <HR>
 * $Log: /GameClient/Src/Intro.cpp $
 * 
 * 45    04-06-23 12:08p Sckyj
 * 
 * 44    04-01-12 11:30p Paran
 * 
 * 43    04-01-05 3:43p Sckyj
 * BGM 재생관련 코드 수정
 * 
 * 42    04-01-02 9:02 Sckyj
 * 
 * 41    03-12-26 4:18p Sckyj
 * 
 * 40    03-12-26 3:18p Sckyj
 * 로딩, 캐릭선택시 BGM재생
 * 
 * 39    03-12-24 1:12a Paran
 * 
 * 38    03-12-18 11:28p Paran
 * 
 * 37    03-12-15 4:33p Sckyj
 * Login_portal bgm을 Intro에서부터 시작하던것을 Login쪽으로 변경
 * 
 * 36    03-12-14 6:10p Sckyj
 * BGM 재생관련 코드 수정
 * 
 * 35    03-12-14 3:11p Sckyj
 * BGM관련 함수 수정(작업중)
 * 
 * 34    03-12-14 10:54a Sckyj
 * 
 * 33    03-12-13 2:50p Paran
 * 
 * 32    03-12-12 6:45p Paran
 * 
 * 31    03-12-12 11:57a Paran
 * 
 * 30    03-12-11 12:55p Paran
 * 
 * 29    03-06-28 4:37p Paran
 * 
 * 28    03-06-27 5:43p Paran
 * 
 * 27    03-06-26 12:41a Paran
 * 사운드 추가.
 * 
 * 26    03-06-24 5:48p Paran
 * 
 * 25    03-05-09 7:43a Ahastudio
 *
 * 16    03-04-10 6:40a Ahastudio	TEAM MAY 로고 동영상 추가.
 * <HR>
 *
 * @file	Intro.cpp
 */


#include "Global.h"

extern BOOL		LoadGlobalData();

/**
 * 인트로 프레임 클래스.
 */
class CIntro : public IFrame
{
public:
	UNDER_MEMORY_MGR( CIntro );

	enum { FADE_TICK = 1500 };
	
	CIntro();
	~CIntro();

	BOOL	Init();
	BOOL	Update( const DWORD dwTick, string* psName );

private:
	IW3DVideoPlayer*	m_pVideoPlayer;		///< 비디오 플레이어
	IW3DSprite*			m_pScreen1;			///< 인트로 화면
	IW3DSprite*			m_pScreen2;			///< 인트로 화면
	IW3DSprite*			m_pCurrScreen;			///< 인트로 화면

	int					m_nStep;			///< Step
	DWORD				m_dwTick;			///< Delay용 Tick

	DWORD				m_dwColor;			///< Color 

	int					m_nDrawX;
	int					m_nDrawY;
	void	Render( const DWORD dwTick );
}; //class CIntro


IMPLEMENT_FRAME( CIntro, Intro );


/**
 * 생성자.
 */
CIntro::CIntro()
:	m_pVideoPlayer	( NULL ),
	m_pScreen1		( NULL ),
	m_pScreen2		( NULL ),
	m_pCurrScreen	( NULL ),
	m_nStep			( 0 ),
	m_dwTick		( 0 ),
	m_nDrawX		( 0 ),
	m_nDrawY		( 0 )
{
} //CIntro::CIntro

/**
 * 소멸자.
 */
CIntro::~CIntro()
{
	SAFE_RELEASE( m_pScreen1 );
	SAFE_RELEASE( m_pScreen2 );
	SAFE_RELEASE( m_pVideoPlayer );
} //CIntro::~CIntro

/**
 * 인트로를 초기화한다.
 */
BOOL CIntro::Init()
{
	m_pScreen1 = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pScreen1 );

	char		szIntro[64];
	if( g_nWidth >= 1024 )
	{
		sprintf( szIntro, "Interface/loading-d1-%04d.bmp", 1024 ); //g_nWidth );
	}
	else
	{
		sprintf( szIntro, "Interface/loading-d1-%04d.bmp", g_nWidth );
	}
	
	if( !m_pScreen1->Load( szIntro, g_nWidth, g_nHeight ) )
	{
		Assert( !"비트맵 파일 로드 실패" );
		return FALSE;
	} //if

	if( g_nWidth >= 1024 )
	{
		m_nDrawX = ( g_nWidth / 2 ) - ( 1024 / 2 );
		m_nDrawY = ( g_nHeight / 2 ) - ( 768 / 2 );
	}	

	m_pScreen2 = MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pScreen2 );

	if( g_nWidth >= 1024 )
	{
		sprintf( szIntro, "Interface/loading-d2-%04d.bmp", 1024 ); //g_nWidth );
	}
	else
	{
		sprintf( szIntro, "Interface/loading-d2-%04d.bmp", g_nWidth );
	}
	if( !m_pScreen2->Load( szIntro, g_nWidth, g_nHeight ) )
	{
		Assert( !"비트맵 파일 로드 실패" );
		return FALSE;
	} //if

	m_nStep = 0;
	m_pCurrScreen = m_pScreen1; 
	
	g_pMediaPlayer->StopAll();
	g_pMediaPlayer->Play( BGM_TYPE_EVENT, BGM_EVENT_INTRO );

	return TRUE;
} //CIntro::Init
 
/**
 * 인트로 프레임을 갱신한다.
 *
 * @return	애플리케이션 실행 여부
 */
BOOL CIntro::Update(
	const DWORD		dwTick,		///< [IN] 경과 Tick
	string*			psName		///< [OUT] 다른 게임 상태 프레임으로 넘길 경우,
								///<       넘어갈 프레임 이름
	)
{
	Assert( psName );
	Assert( g_pScene );

	g_pInput->Update();
	
	BYTE		btColor;
	switch( m_nStep )
	{
	case 0:			// Laqia Logo		
		btColor = ( 255.0F / (float)FADE_TICK ) * m_dwTick;
		m_pCurrScreen->SetColor( COLOR_ARGB( 255, btColor, btColor, btColor ) );
		break;

	case 1:
		if( !LoadGlobalData() )
		{
			*psName = "";
			return FALSE;
		} //if
		m_nStep++;
		m_dwTick = 0;
		break;
		
	case 2:
		g_pMediaPlayer->StopAll();
		g_pMediaPlayer->Play( BGM_TYPE_EVENT, BGM_EVENT_INTRO );

		btColor = 255 - ( 255.0F / (float)FADE_TICK ) * m_dwTick;
		m_pCurrScreen->SetColor( COLOR_ARGB( 255, btColor, btColor, btColor ) );
		m_nStep = 6;
		break;

	case 3:
		m_nStep++;
		m_dwTick = 0;
		m_pCurrScreen = m_pScreen2;

	case 4:
		btColor = ( 255.0F / (float)FADE_TICK ) * m_dwTick;
		m_pCurrScreen->SetColor( COLOR_ARGB( 255, btColor, btColor, btColor ) );
		break;

	case 6:
		btColor = 255 - ( 255.0F / (float)FADE_TICK ) * m_dwTick;
		m_pCurrScreen->SetColor( COLOR_ARGB( 255, btColor, btColor, btColor ) );
		break;

	case 7:
#ifdef _CHINAVERSION
		*psName = "Login";
#else
		*psName = "Login";
		//*psName = "SelectCharacter";
#endif //_CHINAVERSION
		return TRUE;
	} //switch

	m_dwTick += dwTick;

	if( m_dwTick > FADE_TICK )
	{
		m_dwTick = 0;
		m_nStep++;
	} //if

	Render( dwTick );

	return TRUE;
} //CIntro::Update

/**
 * 화면을 그린다.
 */
void CIntro::Render( const DWORD dwTick )
{
	Assert( g_pScene );

	if( !g_pScene->BeginFrame() )		return;

	if( m_pCurrScreen )
	{
		m_pCurrScreen->Draw( m_nDrawX, m_nDrawY, 0, 0, 1024, 768 );
	} //if

	g_pScene->EndFrame();
} //CIntro::Render
