/**
 * 로딩 화면 출력 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-12 11:56a $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Engine/Src/Loading.h $
 * 
 * 8     03-12-12 11:56a Paran
 * 수정
 * 
 * 7     03-09-24 7:47p Paran
 * 
 * 6     03-08-02 11:24a Paran
 * 
 * 5     03-06-16 8:31p Paran
 * 로딩 구분처리
 * 
 * 4     03-03-14 8:15p Ahastudio
 * Position을 Render 하나로 통일.
 * <HR>
 *
 * @file	Loading.h
 */


#ifndef __Loading_Header__
#define __Loading_Header__


/**
 * 로딩 화면 출력 객체.
 * @ingroup		Engine
 */
class CLoading
{
public:
	UNDER_MEMORY_MGR( CLoading );

	CLoading();
	~CLoading();

	BOOL	Init( IW3DScene* pScene, const char* szName );
	void	Render( const int nPos ) const;

private:
	int				m_nLoadingBarX;		///< 로딩바 X.
	int				m_nLoadingBarY;		///< 로딩바 Y.
	int				m_nLoadingAniX;		///< 로딩바 X.
	int				m_nLoadingAniY;		///< 로딩바 Y.
	int				m_nScreenWidth;		///< 화면 가로.
	int				m_nScreenHeight;	///< 화면 세로.
	
	DWORD			m_dwTick;
	IW3DScene*		m_pScene;			///< Scene 객체.
	IW3DSprite*		m_pBackground;		///< 배경 이미지
	IW3DSprite*		m_pLoadingBar;		///< 로딩 진행바 이미지
	IW3DSprite*		m_pLoadingAni;		///< 에니 이미지
}; //class CLoading


#endif // #ifndef __Loading_Header__

