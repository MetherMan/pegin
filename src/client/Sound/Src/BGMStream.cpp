/**
 * BGM Stream Class Source File
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sckyj
 * @date	$Date: 04-03-05 7:54p $
 * @version	$Revision: 22 $
 *
 * <HR>
 * $Log: /Sound/Src/BGMStream.cpp $  
 * 
 * 22    04-03-05 7:54p Sckyj
 * map bgm 무한루프 기능 추가
 * 
 * 21    03-12-21 8:13p Sckyj
 * 
 * 20    03-12-17 11:11a Sckyj
 * 사용하지 않는 BGM의 경우는 Update하지 않도록 수정 ( m_bUse )
 * 
 * 19    03-12-15 4:23p Sckyj
 * 3차 완료.. 추후 버그발견시마다 오류 수정해야함..-_-
 * 
 * 18    03-12-15 12:42p Sckyj
 * 작업중
 * 
 * 17    03-12-15 11:42a Sckyj
 * Reverb 작업중
 * 
 * 16    03-12-14 6:00p Sckyj
 * 2차 완료
 * 
 * 15    03-12-14 3:11p Sckyj
 * MediaPlayer 개조 1차작업
 * 
 * 14    03-12-09 1:20p Sckyj
 *  
 * <HR>
 *
 * @file	BGMStream.cpp
 */

#include "BGMStream.h"

/**
 * 생성자
 */
CBGMStream::CBGMStream() :
m_nCurrBGMPlayType	( 0 ),
m_nCurrBGMPlayNumber( 0 ),
m_btCurrType		( BGM_TYPE_EVENT ),
m_bUse				( FALSE )
{	
	ZeroMemory( &m_sStream, sizeof( Stream ) );
	ZeroMemory( &m_sStreamInfo, sizeof( sBGMInfo ) );
	ZeroMemory( m_szCurrFile, 60 );
	ZeroMemory( m_szAdvanceFile, 60 );
} //CW3DBGMStream::CW3DBGMStream

/**
 * 소멸자
 */
CBGMStream::~CBGMStream()
{	
} //CBGMStream::~CBGMStream()

/**
 * 초기화
 */
void CBGMStream::Init( IW3DMediaPlayer* pMediaPlayer )
{
	Assert( pMediaPlayer );

	m_pMediaplayer		= pMediaPlayer;

	Clear();
} //CBGMStream::Init

/**
 * BGM Info를 설정한다.
 */ 
void CBGMStream::SetBGMInfo( sBGMInfo sBgm )
{
	m_sStreamInfo = sBgm;
} // CBGMStream::SetBGMInfo

/**
 * 초기화
 */
void CBGMStream::Clear()
{	
	m_bCrossPlay	= FALSE;
	m_bJustFadeOut	= FALSE;
	m_bIn			= FALSE;
	m_bOut			= FALSE;
	m_bEnd			= TRUE;

	m_bUse			= FALSE;

	ZeroMemory( &m_sStream, sizeof( struct Stream ) );
	ZeroMemory( &m_nTime, sizeof( int ) * 2 );

	m_sStream.fLVolume	= 0.4f;		// 초기 volume값	
	m_sStream.fRVolume	= 0.4f;

	m_fJustFadeVolume	= 0.0f;
	
	m_dwTick		= 0;

	m_nCurrTime		= 0;			// 현재 시간
} // CBGMStream::Clear

/**
 * 소멸자
 */
BOOL CBGMStream::Play( int nType, int nTypeNum )
{
	switch( nType )
	{
	case BGM_PLAY_EVENT :	
		if( PlayEventBGM( nTypeNum ) )	// Event Play
		{
			m_nCurrBGMPlayType		= nType;
			m_nCurrBGMPlayNumber	= nTypeNum;	
			m_bUse					= TRUE;
			return TRUE;	
		}
		break;
	case BGM_PLAY_CROSS :
		if( PlayAreaBGM( nTypeNum ) )	// Cross Play
		{
			m_nCurrBGMPlayType		= nType;
			m_nCurrBGMPlayNumber	= nTypeNum;			
			m_bUse					= TRUE;
			return TRUE;
		}	
		break;
	case BGM_PLAY_FADING :
		if( PlayFadeOut( nTypeNum ) )	// Just Fade Out
		{
			m_nCurrBGMPlayType		= nType;
			m_nCurrBGMPlayNumber	= nTypeNum;
			m_bUse					= TRUE;
			return TRUE;
		}
	case BGM_PLAY_AMBIENT :
		if( PlayAmbientBGM( nTypeNum ) )
		{
			m_bUse					= TRUE;
			return TRUE;
		}
		break;
	}
	return FALSE;
} //CBGMStream::Play

/**
 *
 */
BOOL CBGMStream::Replay()
{
	if( Play( 1, m_nCurrBGMPlayNumber ) )
	{
		return TRUE;
	}
	return FALSE;
} // CBGMStream::Replay

/**
 * 이벤트 음악을 연주한다.
 */
BOOL CBGMStream::PlayEventBGM( int nTypeNum )
{
	if( !m_pMediaplayer )
	{
		return FALSE;
	} //if

	// Event BGM은 Fading 효과를 주지않고 바로 Play한다
	int		nBGMNum		= m_sStreamInfo.nBGMNo;
	m_btCurrType		= BGM_TYPE_EVENT;

	if( -1 == nBGMNum ) return FALSE;	
	char*	szBGMName	= m_pMediaplayer->GetBGMName( nBGMNum );
	if( !szBGMName )
	{
		return FALSE;
	} //if


	if( !strcmp( m_szCurrFile, szBGMName ) ) 
	{
		return FALSE;
	}
	else
	{
		Stop();
		strcpy( m_szCurrFile, szBGMName);
	} //if..else.. 

	if( !OpenStream( m_szCurrFile, FALSE ) )
	{
//		Assert(!"BGM 파일 열기 실패");
		DEBUG_OUT( "PlayEventBGM() - OpenStream()실패" );
		return FALSE;
	} //if

	m_pMediaplayer->SetVolume( m_pMediaplayer->GetLastVolume() );
	StartStream();							// 연주시작	

	return TRUE;
} //CBGMStream::PlayEventBGM


/**
 * Cross play 음악을 연주한다
 */
BOOL CBGMStream::PlayAreaBGM( int nTypeNum )
{
	if( !m_pMediaplayer )
	{
		return FALSE;
	} //if

	int		nBGMNum;
	char*	szBGMName;

	if( -1 == nTypeNum ) 
	{	// -1일경우는 Area가 아니라 Map이다		
		m_btCurrType	= BGM_TYPE_MAP;
	}
	else
	{
		m_btCurrType	= BGM_TYPE_AREA;
	} //if..else		

	nBGMNum	= m_sStreamInfo.nBGMNo;
	if( nBGMNum < 0 )
	{
		return FALSE;
	} //if

	szBGMName	= m_pMediaplayer->GetBGMName( nBGMNum );
	if( !szBGMName )
	{
		return FALSE;
	} //if

	if( strcmp( m_szCurrFile, szBGMName ) ) 
	{
		strcpy( m_szCurrFile, szBGMName );
		strcpy( m_szAdvanceFile, szBGMName );
	} //if

	if( !m_bCrossPlay )	
	{	
		m_bCrossPlay		= TRUE;		
		m_fTargetVolume		= GetVolume();
		m_pMediaplayer->SetLastVolume( m_fTargetVolume );

		if( m_bJustFadeOut == TRUE )
		{
			m_bJustFadeOut	= FALSE;				// just fade out 도중 cross play가 될때
			m_fTargetVolume = m_fJustFadeVolume;
		} //if

		m_fCrossFadeVolume	= m_fTargetVolume / CROSSFADETIME;
	}	
	else	
	{	// cross play중 다시 cross play를 시작
		strcpy( m_szAdvanceFile, szBGMName );
		
		// Area BGM은 Looping play를 한다.		
		PrepareStream( m_sStreamInfo.bLoop );
	} //if..else..			

	AIL_stream_ms_position( 
		m_sStream.hStream, 
		&m_sStream.nTotalTime, 
		&m_sStream.nCurrTime );

	m_lcurrTime = m_sStream.nCurrTime;	// 기준이 되는 time값

	return TRUE;
} //CBGMStream::PlayAreaBGM


/**
 * 한 bgm이 crossplay 없이 끝날경우 그대로 fade out만 해준다.
 */
BOOL CBGMStream::PlayFadeOut( int nTypeNum )
{
	if( !m_pMediaplayer )
	{
		return FALSE;
	} //if

	if( m_bJustFadeOut || m_bCrossPlay )
		return TRUE;
		
	int		nBGMNum;
	if( -1 == nTypeNum ) 
	{	// -1일경우는 Area가 아니라 Map이다		
		m_btCurrType	= BGM_TYPE_MAP;
	}
	else
	{
		m_btCurrType	= BGM_TYPE_AREA;
	} //if..else		

	nBGMNum	= m_sStreamInfo.nBGMNo;
	if( nBGMNum < 0 )
	{
		return FALSE;
	} //if

	char*	pBGMName = m_pMediaplayer->GetBGMName( nBGMNum );
	if( !pBGMName )
	{
		return FALSE;
	} //if

	if( strcmp( m_szCurrFile, pBGMName ) ) 
		strcpy( m_szCurrFile, pBGMName );

	m_bJustFadeOut		= TRUE;	
	m_fJustFadeVolume	= m_fTargetVolume	= GetVolume();
	m_fCrossFadeVolume	= m_fTargetVolume / CROSSFADETIME;

	AIL_stream_ms_position( 
		m_sStream.hStream, 
		&m_sStream.nTotalTime, 
		&m_sStream.nCurrTime );

	m_lcurrTime = m_sStream.nCurrTime;	// 기준이 되는 time값

	return TRUE;
} //CBGMStream::PlayFadeOut

/**
 * 주변음을 연주한다.
 */
BOOL CBGMStream::PlayAmbientBGM( int nTypeNum )
{
	if( !m_pMediaplayer )
	{
		return FALSE;
	} // if
	
	int		nBGMNum		= m_sStreamInfo.nBGMNo;
	m_btCurrType		= BGM_TYPE_AMBIENT;
	if( nBGMNum < 0 )
	{
		return FALSE;
	} //if

	char*	pBGMName = m_pMediaplayer->GetBGMName( nBGMNum );
	if( !pBGMName )
	{
		return FALSE;
	} //if

	if( !strcmp( m_szCurrFile, pBGMName ) )
	{// 이미 연주하고 있는 bgm일 경우 return 한다.		
		return TRUE;
	}
	else
	{
		strcpy( m_szCurrFile, pBGMName );
		strcpy( m_szAdvanceFile, pBGMName );
	}

	if( !m_bCrossPlay )	
	{	
		m_bCrossPlay		= TRUE;		
		m_fTargetVolume		= GetVolume();
		m_pMediaplayer->SetLastVolume( m_fTargetVolume );

		m_fCrossFadeVolume	= m_fTargetVolume / CROSSFADETIME;
	}
	else
	{	// cross play중 다시 cross play를 시작
		strcpy( m_szAdvanceFile, pBGMName );
		PrepareStream( m_sStreamInfo.bLoop );
	} //if..else..			

	AIL_stream_ms_position( 
		m_sStream.hStream, 
		&m_sStream.nTotalTime, 
		&m_sStream.nCurrTime );

	m_lcurrTime = m_sStream.nCurrTime;	// 기준이 되는 time값

	return TRUE;

} // CBGMStream::PlayAmbientBGM( int nTypeNum )


/**
 * 연주할 stream 파일을 연다
 *
 * @param	szFileName		[IN] 미디어 파일 이름
 */
BOOL CBGMStream::OpenStream( const char* szFileName, BOOL bLoop )
{
	if( !g_pMilesSound )
	{
		return FALSE;
	} //if

	m_sStream.hStream = AIL_open_stream( g_pMilesSound->GetDriver(), szFileName, 0 );
	if (NULL == m_sStream.hStream)
	{
//		Assert(!"BGM 파일 열기 실패");	
		DEBUG_OUT( "OpenStream() 실패" );
		AIL_close_stream( m_sStream.hStream );
	
		return FALSE;
	} //if

	AIL_set_stream_loop_count( m_sStream.hStream, !bLoop );

	AIL_set_stream_volume_levels( m_sStream.hStream, GetVolume(), GetVolume() );

	AIL_stream_info(m_sStream.hStream, 
					&m_sStream.nDataRate,		
					&m_sStream.nType, 
					&m_sStream.nLength,
					&m_sStream.nMemory);

	return TRUE;
} //CBGMStream::OpenStream

/**
 * stream을 시작한다 
 */
BOOL CBGMStream::StartStream()
{
	if( m_sStream.hStream )
		AIL_start_stream( m_sStream.hStream );

	return TRUE;
} //CBGMStream::StartStream

/**
 * volume을 설정한다.
 */
BOOL CBGMStream::SetVolume( float fVolume )
{
	m_sStream.fLVolume = m_sStream.fRVolume = fVolume;

	AIL_set_stream_volume_levels(
		m_sStream.hStream, 
		m_sStream.fLVolume,
		m_sStream.fRVolume );

	return TRUE;
} // CBGMStream::SetVolume

/**
 * volume을 얻어온다.
 */
float CBGMStream::GetVolume()
{
	return m_sStream.fLVolume;
} // CBGMStream::GetVolume

/**
 * Looping 여부를 반환한다.
 */
BOOL CBGMStream::GetLoop()
{
	return m_sStreamInfo.bLoop;
} // CBGMStream::GetLoop()


/**
 * BGM Play type을 반환한다.
 */
/*
BYTE CBGMStream::GetBGMType()
{
	return m_btCurrType;
} // CBGMStream::GetBGMType()
*/

/**
 *	최근에 플레이한 type( event, cross, fading)을 반환한다.
 */
/*
int CBGMStream::GetBGMPlayType()
{
	return m_nCurrBGMPlayType;
} // CBGMStream::GetBGMTypm
*/

/**
 *	최근에 플레이한 type number(bgm number) 를 반환한다.
 */
/*
int CBGMStream::GetBGMPlayTypeNum()
{
	return m_nCurrBGMPlayNumber;
} // CBGMStream::GetBGMTypeNum
*/

/**
 * 연주를 멈춘다.
 */
void CBGMStream::Stop()
{
	if( m_sStream.hStream != NULL )
	{
		AIL_pause_stream(m_sStream.hStream, FALSE);
		AIL_close_stream(m_sStream.hStream);
	}

	m_sStream.hStream = NULL;
	ZeroMemory( m_szCurrFile, strlen( m_szCurrFile ) );	
} // CBGMStream::GetVolume

/**
 * Stream을 Update한다.
 */
BOOL CBGMStream::Update( DWORD dwTick )
{
	if( !m_bUse )	return FALSE;

	if( !m_bCrossPlay && !m_bJustFadeOut )	return FALSE;	

	if( m_dwTick + dwTick < CROSSFADETICK )
	{
		m_dwTick += dwTick;
		return FALSE;
	}	
	m_dwTick = 0;

	if( !m_bOut )	// fade out
	{		
		m_bOut	= FadeOut( m_lcurrTime );

		if( m_bOut && !m_bJustFadeOut )		// cross play
		{
			PrepareStream( m_sStreamInfo.bLoop );
		}
		else
		if( m_bOut && m_bJustFadeOut )		// just fade out
		{
			m_bIn = TRUE;			
		}
	}
	else
	if( !m_bIn )	// fade in
	{
		m_bIn	= FadeIn();
	}		
	else
	{		
		if( m_bJustFadeOut )
		{
			Stop();
			SetVolume( m_fTargetVolume );
		}

		m_bOut = m_bIn	= FALSE;
		m_bCrossPlay	= FALSE;
		m_bJustFadeOut	= FALSE;
	} // if..else

	return TRUE;
} // CBGMStream::Update

/**
 * 플레이 되던 BGM을 Fade out 시킨다.
 */
BOOL CBGMStream::FadeOut(long currTime)
{
	AIL_stream_ms_position(
		m_sStream.hStream, 
		&m_sStream.nTotalTime,
		&m_sStream.nCurrTime );

	if( 
		( m_sStream.nCurrTime >= currTime + CROSSFADEOFFSET ) ||	
		( (m_sStream.nCurrTime + CROSSFADETICK ) > m_sStream.nTotalTime )
		)
	{		
		SetVolume( 0 );
		return TRUE;
	}
	else 
	{
		float fVolume = GetVolume() - m_fCrossFadeVolume;
		SetVolume( fVolume > MINVOLUME ? fVolume : MINVOLUME );
		if( fVolume <= MINVOLUME )
		{			
			return TRUE;
		} // if
	} // if..else..
	return FALSE;
} //CBGMStream::FadeOut

/**
 * 새로운 BGM을 Fade in 시킨다.
 */
BOOL CBGMStream::FadeIn()
{
	AIL_stream_ms_position(
		m_sStream.hStream, 
		&m_sStream.nTotalTime,
		&m_sStream.nCurrTime );

	if( 
		( m_sStream.nCurrTime >= CROSSFADEOFFSET ) ||		
		( ( m_sStream.nCurrTime + CROSSFADETICK ) > m_sStream.nTotalTime )
		)
	{	
		SetVolume( m_fTargetVolume );
		return TRUE;
	}
	else
	{
		float fVolume = GetVolume() + m_fCrossFadeVolume;
		SetVolume( fVolume < m_fTargetVolume ? fVolume : m_fTargetVolume );
		if( fVolume >= m_fTargetVolume )
		{
			return TRUE;
		} // if
	} // if..else
	return FALSE;
} //CBGMStream::FadeIn

/**
 * BGM을 연주하기 위해 stream을 준비힌다.
 */
void CBGMStream::PrepareStream( BOOL bLoop )
{	
	Stop();
	OpenStream( m_szAdvanceFile, bLoop );
	StartStream();
	SetVolume( 0 );
} //CBGMStream::PrepareStream

/**
 * bgm이 끝나가는지 여부를 반환한다.
 */
BOOL CBGMStream::IsEnding()
{
	if( !m_sStream.hStream )
		return FALSE;

	long lLeftTime = 0;
	
	AIL_stream_ms_position( 
		m_sStream.hStream,
		&m_sStream.nTotalTime,
		&m_sStream.nCurrTime );

	lLeftTime = m_sStream.nTotalTime - m_sStream.nCurrTime;
	if( lLeftTime < 3000 )	// 남은 연주시간이 3000초 이내라면 fade out 한다.
	{		
		return TRUE;
	} //if
	return FALSE;
} //CBGMStream::IsEnding

/**
 * bgm이 끝나가는지 여부를 반환한다.
 */
BOOL CBGMStream::IsEnd()
{
	/*
	S32 result;
	
	result = AIL_stream_status( m_sStream.hStream );

	if( result == SMP_DONE || result == SMP_STOPPED )
	{
		m_bEnd = TRUE;
	}
	else
	{
		m_bEnd = FALSE;
	}
	*/

	if( m_sStream.hStream == NULL )
	{
		m_bEnd = TRUE;
	}
	else m_bEnd = FALSE;
	
	return m_bEnd;
} // CBGMStream::IsEnd
