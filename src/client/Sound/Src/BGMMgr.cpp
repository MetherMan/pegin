/**
 * BGM Manager Class source File
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	sckyj
 * @date	$Date: 04-03-05 7:54p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /Sound/Src/BGMMgr.cpp $   
 * 
 * 7     04-03-05 7:54p Sckyj
 * map bgm 무한루프 기능 추가
 * 
 * 6     03-12-17 4:16p Sckyj
 * EventMgr에서 재생중인 사운드를 다시 재생시 걍 리턴하도록 수정
 * 
 * 5     03-12-17 11:11a Sckyj
 * 사용하지 않는 BGM의 경우는 Update하지 않도록 수정 ( m_bUse )
 * 
 * 4     03-12-15 4:23p Sckyj
 * 3차 완료.. 추후 버그발견시마다 오류 수정해야함..-_-
 * 
 * 3     03-12-15 11:21a Sckyj
 * 3차 도중.-_-
 * 
 * 2     03-12-14 6:00p Sckyj
 * 2차 완료
 * <HR>
 *
 * @file	BGMMgr.cpp
 */

#include "BGMStream.h"

/************************************************************************/
/*  Map BGM Manager Class Member                                        */
/************************************************************************/

/**
 *
 */
CBGMMapMgr::CBGMMapMgr()
{
}

/**
 *
 */
CBGMMapMgr::~CBGMMapMgr()
{
}

/**
 *	Map BGM을 재생한다.
 */
void CBGMMapMgr::Play( int nMap )
{	
	m_BGMStream.Play( BGM_PLAY_CROSS, nMap );

	m_nMap = nMap;	
} // CBGMMapMgr::Play

/**
 * Map BGM 재생을 멈춘다.
 */
void CBGMMapMgr::Stop()
{
	m_BGMStream.Stop();
} // CBGMMapMgr::Stop

/**
 * Map BGM을 Update한다.
 */
void CBGMMapMgr::Update( DWORD dwTick )
{
	if( !m_nMap )	return;
	
	m_BGMStream.Update( dwTick );

	if( m_BGMStream.IsEnd() )
	{
		if( m_BGMStream.GetLoop() )
		{
			m_BGMStream.Replay();
			m_dwTime = 0;
		}
		else
		{
			m_dwTime += dwTick;
	
			if( m_dwTime >= 300000 )					// 재생이 끝난후 5분이 경과할 경우 RePlay
			{				
				m_BGMStream.Replay();				
				m_dwTime = 0;
			} //if
		}		
	} //if
	else
	if( m_BGMStream.IsEnding() )
	{
		m_BGMStream.Play( BGM_PLAY_FADING, m_nMap );		// 재생이 끝나가면 fade out
	}
	
} // CBGMMapMgr::Update

/**
 * BGM Info를 설정한다.
 */
void CBGMMapMgr::SetBGMInfo( sBGMInfo bgmInfo )
{
	m_BGMStream.SetBGMInfo( bgmInfo );
} // CBGMMapMgr::SetBGMInfo

/**
 * BGM Volume을 설정한다.
 */
void CBGMMapMgr::SetVolume( float fVolume )
{
	m_BGMStream.SetVolume( fVolume );
} // CBGMMapMgr::SetVolume


/************************************************************************/
/* Event BGM Manager Class Member						                */
/************************************************************************/

/**
 *
 */
CBGMEventMgr::CBGMEventMgr()
{
}

/**
 *
 */
CBGMEventMgr::~CBGMEventMgr()
{
}

/**
 * Event BGM을 재생한다.
 */
void CBGMEventMgr::Play( int nEventNum )
{
	m_BGMStream.Play( BGM_PLAY_EVENT, nEventNum );

	m_nEvent = nEventNum;
} // CBGMEventMgr::Play

/**
 * Event BGM 재생을 멈춘다.
 */
void CBGMEventMgr::Stop()
{
	m_BGMStream.Stop();
} // CBGMEventMgr::Stop

/**
 * Event BGM을 Update한다.
 */
void CBGMEventMgr::Update( DWORD dwTick )
{
	if( !m_nEvent )	return;

	m_BGMStream.Update( dwTick );

	if( m_BGMStream.IsEnding() )
	{
		m_BGMStream.Play( BGM_PLAY_FADING, m_nEvent );		// 재생이 끝나가면 fade out
	}
} // CBGMEventMgr::Update

/**
 * BGM Info를 설정한다.
 */
void CBGMEventMgr::SetBGMInfo( sBGMInfo bgmInfo )
{
	m_BGMStream.SetBGMInfo( bgmInfo );
} // CBGMEventMgr::SetBGMInfo

/**
 * BGM Volume을 설정한다.
 */
void CBGMEventMgr::SetVolume( float fVolume )
{
	m_BGMStream.SetVolume( fVolume );
} // CBGMEventMgr::SetVolume

/**
 * 같은 번호의 BGM인가
 */
BOOL CBGMEventMgr::IsSame( int nEvent )
{
	if( nEvent == m_nEvent )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
} // CBGMEventMgr::IsSame

/**
 * 종료되었는가
 */
BOOL CBGMEventMgr::IsEnd()
{
	return m_BGMStream.IsEnd();
} // CBGMEventMgr::IsEnd

/************************************************************************/
/* Ambient BGM Manager Class Member                                     */
/************************************************************************/

/**
 *
 */
CBGMAmbientMgr::CBGMAmbientMgr()
{
}

/**
 *
 */
CBGMAmbientMgr::~CBGMAmbientMgr()
{
}

/**
 * Ambient BGM을 재생한다.
 */
void CBGMAmbientMgr::Play( int nAmbiNum )
{
	if( !m_bPlay )
		m_nAmbi = nAmbiNum;
} // CBGMAmbientMgr::Play

/**
 * Ambient BGM을 멈춘다.
 */
void CBGMAmbientMgr::Stop()
{
	m_BGMStream.Stop();
} // CBGMAmbientMgr::Stop

/**
 * Ambient BGM을 Update한다.
 */
void CBGMAmbientMgr::Update( DWORD dwTick, int nCurrTime )
{	
	if( !m_nAmbi ) return;
	
	m_BGMStream.Update( dwTick );

	int nIndex = nCurrTime <= 12 ? 0 : 1;	
	int* nTime = m_BGMStream.GetBGMInfo().nTime;
	if( nTime[nIndex] == TRUE && !m_bPlay )
	{
		m_BGMStream.Play( BGM_PLAY_AMBIENT, m_nAmbi );
		m_bPlay = TRUE;
	}
	else
	if( nTime[nIndex] == FALSE && m_bPlay )
	{
		m_BGMStream.Stop();
		m_bPlay = FALSE;
	}	
} // CBGMAmbientMgr::Update

/**
 * BGM Info를 설정한다.
 */
void CBGMAmbientMgr::SetBGMInfo( sBGMInfo bgmInfo )
{
	m_BGMStream.SetBGMInfo( bgmInfo );
} // CBGMAmbientMgr::SetBGMInfo

/**
 * BGM Volume을 설정한다.
 */
void CBGMAmbientMgr::SetVolume( float fVolume )
{
	m_BGMStream.SetVolume( fVolume );
} // CBGMAmbientMgr::SetVolume

/************************************************************************/
/* Area BGM Manager Class Member	                                    */
/************************************************************************/

/**
 *
 */
CBGMAreaMgr::CBGMAreaMgr()
{
}

/**
 *
 */
CBGMAreaMgr::~CBGMAreaMgr()
{
}

/**
 * Area BGM을 재생한다.
 */
void CBGMAreaMgr::Play( int nCount, int* nInfo )
{
	int nNum;
	int i;
		// 지역 유지				 // Field
	if( m_nLastCount == nCount && 0 == nCount )
	{
/*		for( int i=0 ; i<MAXBGM ; i++ )
		{
			m_BGMStream[i].Play( BGM_PLAY_FADING, m_nLastInfo[i] );
		}*/
	}
	else
		// 지역 유지				 // Area
	if( m_nLastCount == nCount && 0 != nCount )
	{
		for( i=0 ; i<nCount ; i++ )
		{
			if( m_nLastInfo[i] != nInfo[i] )						// 지역 type이 다를경우
			{
				m_BGMStream[i].Play( BGM_PLAY_CROSS, nInfo[i] );	// cross play
			}
		}
	}
	else
		// 지역 변경				 // Area --> Field
	if( m_nLastCount != nCount && 0 == nCount )
	{		
		for( i=0 ; i<m_nLastCount ; i++ )							// 그 외의 재생중이던 BGM들은 fading을 한다.
		{
			if( 0 != m_nLastInfo[i] )
				m_BGMStream[i].Play( BGM_PLAY_FADING, m_nLastInfo[i] );
		}
	}
	else
		// 지역 변경				 // Area 갯수변경 or Field --> Area
	if( m_nLastCount != nCount && 0 != nCount )
	{
		nNum = m_nLastCount > nCount ? m_nLastCount : nCount;

		for( i=0 ; i<nNum ; i++ )
		{
			if( m_nLastInfo[i] != nInfo[i] )
			{
				if( nInfo[i] )
				{
					m_BGMStream[i].Play( BGM_PLAY_CROSS, nInfo[i] );
				}
				else
				{
					m_BGMStream[i].Play( BGM_PLAY_FADING, m_nLastInfo[i] );
				}
			}			
		}
	}

	memcpy( m_nLastInfo, nInfo, sizeof( int ) * MAXBGM );
	m_nLastCount = nCount;
} // CBGMAreaMgr::Play

/**
 * Area BGM을 멈춘다.
 */
void CBGMAreaMgr::Stop()
{
	for( int i=0 ; i<MAXBGM ; i++ )
	{
		m_BGMStream[i].Stop();
	}
} // CBGMAreaMgr::Stop

/**
 * Area BGM을 Update한다.
 */
void CBGMAreaMgr::Update( DWORD dwTick )
{
	if( !m_nLastCount ) return;

	for( int i=0 ; i<MAXBGM ; i++ )
	{
		m_BGMStream[i].Update( dwTick );
	}	
} // CBGMAreaMgr::Update

/**
 *
 */
void CBGMAreaMgr::SetBGMInfo( int nCount, sBGMInfo* bgmInfo )
{
	for( int i=0 ; i<nCount ; i++ )
	{
		m_BGMStream[i].SetBGMInfo( bgmInfo[i] );
	}
} // CBGMAreaMgr::SetBGMInfo

/**
 *
 */
void CBGMAreaMgr::SetVolume( float fVolume )
{
	for( int i=0 ; i<MAXBGM ; i++ )
	{
		m_BGMStream[i].SetVolume( fVolume );
	}
} // CBGMAreaMgr::SetVolume
