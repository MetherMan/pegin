/**
 * BGM Stream Class Header File
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sckyj
 * @date	$Date: 04-03-05 7:54p $
 * @version	$Revision: 17 $
 *
 * <HR>
 * $Log: /Sound/Src/BGMStream.h $  
 * 
 * 17    04-03-05 7:54p Sckyj
 * map bgm 무한루프 기능 추가
 * 
 * 16    03-12-17 4:16p Sckyj
 * EventMgr에서 재생중인 사운드를 다시 재생시 걍 리턴하도록 수정
 * 
 * 15    03-12-17 11:11a Sckyj
 * 사용하지 않는 BGM의 경우는 Update하지 않도록 수정 ( m_bUse )
 * 
 * 14    03-12-15 4:23p Sckyj
 * 3차 완료.. 추후 버그발견시마다 오류 수정해야함..-_-
 * 
 * 13    03-12-15 11:42a Sckyj
 * Reverb 작업중
 * 
 * 12    03-12-15 11:21a Sckyj
 * 3차 도중.-_-
 * 
 * 11    03-12-14 6:00p Sckyj
 * 2차 완료
 * 
 * 10    03-12-14 3:11p Sckyj
 * MediaPlayer 개조 1차작업
 * 
 * 9     03-12-09 12:53p Sckyj
 * CBGMMusic
 * CBGMArea
 * CBGMAmbient
 * CBGMEvent       class header 추가
 * <HR>
 *
 * @file	BGMStream.h
 */

#ifndef	__BGMSTREAM__HEADER__
#define	__BGMSTREAM__HEADER__

#include "Sound_Export.h"

#define MAXVOLUME			0.5f
#define MINVOLUME			0.0f

#define CROSSFADETIME		10								// 몇번에 걸쳐 fading을 할 것인가.
#define CROSSFADEOFFSET		2000							// 몇초동안 fading을 할 것인가.
#define	CROSSFADETICK		CROSSFADEOFFSET / CROSSFADETIME	// fading이 되는 단위 시간

struct Stream
{
	HSTREAM		hStream;			//	Stream Handle
	F32			fRVolume;			//	우측음량
	F32			fLVolume;			//	좌측음량
	int			nPosition;			//	현재 play중인 위치
	long		nTotalTime;			//	총 play 시간(milli second)
	long		nCurrTime;			//	현재 play 시간	

	S32			nDataRate;			//	stream file의 주파수(44100 Hz)
	S32			nType;				//	stream file의 type
	S32			nLength;			//	총길이(byte)
	S32			nMemory;			//	RAM에 올라가는 stream단위(byte)
}; //Stream

/**
 * BGM Stream 클래스
 * @ingroup		Sound
 */
class CBGMStream
{
public :
	UNDER_MEMORY_MGR( CBGMStream );

	CBGMStream();
	virtual ~CBGMStream();
	
	void		Init( IW3DMediaPlayer* pMediaPlayer );	///< 초기화
	void		Clear();								///< 초기화

	void		SetBGMInfo( sBGMInfo sBgm );			///< 스크립트에서 읽어들인 bgm정보를 입력한다
	sBGMInfo	GetBGMInfo()
	{
		return m_sStreamInfo;
	}

	BOOL		Play( int nBGMType, int nTypeNum );	///< type별 배경음악 플레이
	BOOL		Replay();							///< 리플레이 - event 제외	

	void		Stop();								///< 재생 멈춤
	
	BOOL		Update( DWORD dwTick );				///< Crossplay시 fade in,out을 update	
	
	BOOL		SetVolume( float fVolume );			///< 음량조절	
	float		GetVolume();						///< 음량 얻기
	BOOL		GetLoop();							///< Looping info를 얻는다.

	BOOL		IsEnding();							///< bgm이 끝나가는지 여부를 반환한다.
	BOOL		IsEnd();							///< bgm이 끝났는지 여부를 반환한다.

private :	

	BOOL		PlayEventBGM( int nTypeNum );	
	BOOL		PlayAreaBGM( int nTypeNum );	
	BOOL		PlayFadeOut( int nTypeNum );	
	BOOL		PlayAmbientBGM( int nTypeNum );

	BOOL		OpenStream( const char* szFileName, BOOL bLoop );	///< 배경음악 열기
	BOOL		StartStream();						///< 연주 시작

	BOOL		FadeIn();							///< 배경음악이 점점 커진다
	BOOL		FadeOut( long currTime );			///< 배경음악이 점점 작아진다

	void		PrepareStream( BOOL bLoop );		///< fade in이 될 배경음악을 준비하고, fade in시 발생하는
													///< 배경음악 변경 버그를 방지한다.	

	Stream		m_sStream;							///< Stream structure
	sBGMInfo	m_sStreamInfo;						///< Stream Info

	IW3DMediaPlayer* m_pMediaplayer;				
	
	char		m_szCurrFile[60];					///< 연주할 file name
	char		m_szAdvanceFile[60];				///< cross play용 보조 file name
	
	BYTE		m_btCurrType;						///< 현재 재생중인 bgm type - BGM_TYPE_MAP 등
	int			m_nCurrBGMPlayType;					///< BGM Type - BGM_EVENT 등
	int			m_nCurrBGMPlayNumber;				///< BGM Number
	
	BOOL		m_bCrossPlay;						///< CrossPlay를 하는가
	BOOL		m_bJustFadeOut;						///< Just Fade Out을 하는가
	BOOL		m_bIn;								///< Fade In 여부
	BOOL		m_bOut;								///< Fade Out 여부
	BOOL		m_bEnd;								///< bgm이 끝났는지 여부
	
	BOOL		m_bUse;								///< 사용하는 BGM Stream인가.

	float		m_fTargetVolume;					///< 언제까지 Fade In을 할것인가.
	float		m_fCrossFadeVolume;					///< 한번 update시에 fading을 할 volume의 양
	float		m_fJustFadeVolume;					///< Just fade out시에 목표 음량을 저장해놓는다.

	long		m_lcurrTime;						///< Crossplay를 시작할때 current stream의 play time값을 저장		

	int			m_nTime[2];							///< script에서 설정되는 재생시간( ambient bgm에서 사용 )
	int			m_nCurrTime;						///< world에서 설정하는 현재 시간

	DWORD		m_dwTick;							///< 누적 Tick
};

/************************************************************************/
/*  Map BGM Manager Class                                               */
/************************************************************************/
class CBGMMapMgr
{
public :
	UNDER_MEMORY_MGR( CBGMMapMgr );

	CBGMMapMgr();
	virtual ~CBGMMapMgr();

	void	Init( IW3DMediaPlayer* pMediaPlayer )
	{
		Assert( pMediaPlayer );
		m_BGMStream.Init( pMediaPlayer );

		Clear();
	}

	void	Play( int nInfo );				///< BGM을 재생한다.
	void	Stop();							///< BGM 재생을 멈춘다.
	void	Update( DWORD dwTick );			///< Map BGM을 Update한다.
	void	Clear()
	{
		m_nMap		= 0;
		m_dwTime	= 0;

		m_BGMStream.Clear();
	}
	
	void	SetBGMInfo( sBGMInfo bgmInfo );	///< BGMInfo를 설정한다.
	void	SetVolume( float fVolume );		///< volume을 설정한다.

	int		GetMapNum()						///< Map 번호를 반환한다.
	{
		return m_nMap;
	}

private :
	CBGMStream	m_BGMStream;				///< BGM Stream 멤버	

	int			m_nMap;						///< 재생중인 Map번호

	DWORD		m_dwTime;					///< 누적 time
};

/************************************************************************/
/*  Event BGM Manager Class                                             */
/************************************************************************/
class CBGMEventMgr
{
public :
	UNDER_MEMORY_MGR( CBGMEventMgr );

	CBGMEventMgr();
	virtual ~CBGMEventMgr();

	void	Init( IW3DMediaPlayer* pMediaPlayer )
	{
		Assert( pMediaPlayer );
		m_BGMStream.Init( pMediaPlayer );

		Clear();
	}

	void	Play( int nInfo );				///< BGM을 재생한다.
	void	Stop();							///< BGM재생을 멈춘다.
	void	Update( DWORD dwTick );			///< Event BGM을 Update한다		
	void	Clear()
	{
		m_nEvent = 0;

		m_BGMStream.Clear();
	}

	void	SetBGMInfo( sBGMInfo bgmInfo );	///< BGMInfo를 설정한다.	
	void	SetVolume( float fVolume );		///< volume을 설정한다.

	BOOL	IsSame( int nEvent );			///< 이전과 같은 번호의 BGM인가
	BOOL	IsEnd();						///< 종료되었는가

private :		
	CBGMStream	m_BGMStream;				///< BGM Stream 멤버
	
	int			m_nEvent;					///< 재생중인 Event번호
};

/************************************************************************/
/* Ambient BGM Manager Class                                            */
/************************************************************************/
class CBGMAmbientMgr
{
public :
	UNDER_MEMORY_MGR( CBGMAmbientMgr );

	CBGMAmbientMgr();
	virtual ~CBGMAmbientMgr();

	void	Init( IW3DMediaPlayer* pMediaPlayer )
	{
		Assert( pMediaPlayer );
		m_BGMStream.Init( pMediaPlayer );

		Clear();
	}

	void	Play( int nInfo );				///< BGM을 재생한다.
	void	Stop();							///< BGM 재생을 멈춘다.
	void	Update( DWORD dwTick, int nCurrTime );		///< Ambient BGM을 Update한다		
	void	Clear()
	{
		m_nAmbi = 0;
		m_bPlay = 0;

		m_BGMStream.Clear();
	}
	
	void	SetBGMInfo( sBGMInfo bgmInfo );	///< BGMInfo를 설정한다.	
	void	SetVolume( float fVolume );		///< volume을 설정한다.

private :	
	CBGMStream	m_BGMStream;				///< BGM Stream 멤버
	
	int			m_nAmbi;					///< 재생중인 Ambient 번호
	BOOL		m_bPlay;					///< Ambient BGM이 재생중인가.
};

/************************************************************************/
/* Area BGM Manager Class                                               */
/************************************************************************/
class CBGMAreaMgr
{
public :
	UNDER_MEMORY_MGR( CBGMAreaMgr );

	CBGMAreaMgr();
	virtual ~CBGMAreaMgr();

	void	Init( IW3DMediaPlayer* pMediaPlayer )
	{
		Assert( pMediaPlayer );

		for( int i=0 ; i<MAXBGM ; i++ )
		{
			m_BGMStream[i].Init( pMediaPlayer );
		}		

		Clear();
	}

	void	Play( int nCount, int* nInfo );	///< BGM을 재생한다.
	void	Stop();							///< BGM 재생을 멈춘다.
	void	Update( DWORD dwTick );			///< Area BGM을 Update한다
	void	Clear()
	{
		m_nLastCount = 0;
		ZeroMemory( m_nLastInfo, sizeof( int ) * 3 );

		for( int i=0 ; i<MAXBGM ; i++ )
		{
			m_BGMStream[i].Clear();
		}
	}

	void	SetBGMInfo( int nCount, sBGMInfo* bgmInfo );///< BGMInfo를 설정한다.
	void	SetVolume( float fVolume );		///< volume을 설정한다.

private :			

	CBGMStream	m_BGMStream[MAXBGM];		///< BGM Stream 멤버

	int			m_nLastCount;				///< 이전 Tick에 재생하던 Area 수
	int			m_nLastInfo[MAXBGM];		///< 이전 Tick에 재생하던 Area Type
};

#endif // #ifndef	__BGMSTREAM__HEADER__