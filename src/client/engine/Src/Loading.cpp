/**
 * 로딩 화면 출력 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 05-02-19 7:11p $
 * @version	$Revision: 25 $
 *
 * <HR>
 * $Log: /Engine/Src/Loading.cpp $
 * 
 * 25    05-02-19 7:11p Sk8snow2
 * 
 * 24    04-12-08 2:57a Paran2
 * 
 * 23    04-05-29 10:05p Paran
 * 
 * 22    04-05-25 3:00p Paran2
 * 
 * 21    04-05-25 12:36a Kjmgo
 * 
 * 20    04-03-30 5:20a Kjmgo
 * 
 * 19    04-03-30 5:06a Paran2
 * 정만: 임시적으로 정혁형 자리에서 풀었습니다.
 * 
 * 18    04-01-17 10:55p Paran
 * 
 * 17    03-12-14 4:42p Paran
 * 
 * 16    03-12-12 11:56a Paran
 * 수정
 * 
 * 15    03-09-24 7:47p Paran
 * 
 * 14    03-08-02 11:24a Paran
 * 
 * 13    03-07-25 11:14a Kjmgo
 * 
 * 12    03-06-24 7:14p Paran
 * 
 * 11    03-06-23 10:51p Paran
 * 로딩파일 경로 수정
 * 
 * 10    03-06-16 8:31p Paran			로딩 구분처리
 * 
 * 8     03-05-30 12:43a Paran			로딩이미지 수정.
 * 
 * 4     03-03-14 8:15p Ahastudio		Position을 Render 하나로 통일.
 * <HR>
 *
 * @file	Loading.cpp
 */


#include "Engine_Export.h"
#include "Loading.h"


enum
{
	LOADING_BAR_SPRITE_WIDTH	= 470,		///<
	LOADING_BAR_SPRITE_HEIGHT	= 5,		///<
	LOADING_ANI_SPRITE_WIDTH	= 166,		///<				// 231
	LOADING_ANI_SPRITE_HEIGHT	= 63		///<				// 39
//	LOADING_ANI_SPRITE_WIDTH	= 231,
//	LOADING_ANI_SPRITE_HEIGHT	= 39
}; //enum


/**
 * 생성자.
 */
CLoading::CLoading()
{
	m_pBackground = NULL;
	m_pLoadingBar = NULL;
	m_pLoadingAni = NULL;
	m_dwTick	  = 0;
} //CLoading::CLoading

/**
 * 소멸자.
 */
CLoading::~CLoading()
{
	SAFE_RELEASE( m_pLoadingBar );
	SAFE_RELEASE( m_pBackground );
	SAFE_RELEASE( m_pLoadingAni );
} //CLoading::~CLoading

/**
 * Loading 화면 초기화.
 *
 * @param	pScene		[IN] Scene 객체.
 *
 * @return	성공 여부
 */
BOOL CLoading::Init( IW3DScene* pScene, const char* szName )
{
	Assert( pScene );

	m_nScreenWidth  = GetSystemMetrics( SM_CXSCREEN );
	m_nScreenHeight	= GetSystemMetrics( SM_CYSCREEN );
	
	if( m_nScreenWidth == 1024 )
	{ //1024
		m_nLoadingBarX  = 0;
		m_nLoadingBarY  = 565;

		m_nLoadingBarX	= 358;
		m_nLoadingBarY	= 701;

		m_nLoadingAniX  = 717;
		m_nLoadingAniY  = 695;
	}
	else
	{ //800
		m_nScreenWidth  = 800;
		m_nScreenHeight = 600;
		m_nLoadingBarX	= 163;
		m_nLoadingBarY	= 547;
		m_nLoadingAniX  = 633;
		m_nLoadingAniY  = 521;
	} //if..else

	m_pScene		= pScene;

	m_pBackground	= MAKE_COMPONENT( Sprite );
	m_pLoadingBar	= MAKE_COMPONENT( Sprite );

	CHECK_PTR_RETURN( m_pBackground );
	CHECK_PTR_RETURN( m_pLoadingBar );

	char szFullName[256];

	sprintf( szFullName, "Map/Loading%s_%04d.bmp", szName+4, m_nScreenWidth );
	if( !m_pBackground->Load( szFullName, m_nScreenWidth, m_nScreenHeight ) )
	{
		SAFE_RELEASE( m_pBackground );
		Assert( !"Loading 화면용 Sprite 로딩 실패" );
		//return FALSE;
	} //if

	sprintf( szFullName, "Interface/loading-bar.bmp", m_nScreenWidth );
	if( !m_pLoadingBar->Load( szFullName, LOADING_BAR_SPRITE_WIDTH, LOADING_BAR_SPRITE_HEIGHT ) )
	{
		Assert( !"Loading 화면용 Sprite 로딩 실패" );
	//	return FALSE;
	} //if

#ifndef _CHINAVERSION
	m_pLoadingAni	= MAKE_COMPONENT( Sprite );
	CHECK_PTR_RETURN( m_pLoadingAni );
	if( !m_pLoadingAni->Load( 
		"Interface/loadingani.bmp", 
		LOADING_ANI_SPRITE_WIDTH, 
		LOADING_ANI_SPRITE_HEIGHT
		) )
	{
		Assert( !"Loading 화면용 Sprite 로딩 실패" );
	//	return FALSE;
	} //if
	//m_pLoadingAni->SetAnimation( 4, 2, 100 );
	m_pLoadingAni->SetAnimation( 2, 4, 100 );
#endif //_CHINAVERSION

	return TRUE;
} //CLoading::Init

/**
 * 로딩 화면 출력.
 *
 * @param	nPos	[IN] 진행율(%)
 */
void CLoading::Render( int nPos ) const
{
	Assert( 0 <= nPos && nPos <= 100 );

	int		nSize = LOADING_BAR_SPRITE_WIDTH * nPos / 100;

	if( !m_pScene->BeginFrame() )		return;

	if( m_pBackground )
		m_pBackground->Draw( 0, 0 );

	if( m_pLoadingBar )
	{
		m_pLoadingBar->Draw( m_nLoadingBarX, m_nLoadingBarY, 
							 0, 0, nSize, LOADING_BAR_SPRITE_HEIGHT );
	} //if

	
	if( m_pLoadingAni )
	{
		m_pLoadingAni->Update( 100 );
		m_pLoadingAni->Draw( m_nLoadingAniX, m_nLoadingAniY, 
							 0, 0, LOADING_ANI_SPRITE_WIDTH, LOADING_ANI_SPRITE_HEIGHT );
	} //if

	m_pScene->EndFrame();
} //CLoading::Render
