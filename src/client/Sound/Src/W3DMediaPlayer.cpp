/**
 * 미디어 플레이어.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-01-05 3:44p $
 * @version	$Revision: 36 $
 *
 * <HR>
 * $Log: /Sound/Src/W3DMediaPlayer.cpp $
 * 
 * 36    04-01-05 3:44p Sckyj
 * Event BGM의 Event Volume을 parsing 하는 코드 삭제
 * 
 * 35    03-12-17 4:16p Sckyj
 * EventMgr에서 재생중인 사운드를 다시 재생시 걍 리턴하도록 수정
 * 
 * 34    03-12-17 11:11a Sckyj
 * 사용하지 않는 BGM의 경우는 Update하지 않도록 수정 ( m_bUse )
 * 
 * 33    03-12-15 4:23p Sckyj
 * 3차 완료.. 추후 버그발견시마다 오류 수정해야함..-_-
 * 
 * 32    03-12-15 1:47p Sckyj
 * 
 * 31    03-12-15 12:42p Sckyj
 * 작업중
 * 
 * 30    03-12-15 11:42a Sckyj
 * Reverb 작업중
 * 
 * 29    03-12-15 11:21a Sckyj
 * 3차 도중.-_-
 * 
 * 28    03-12-14 6:00p Sckyj
 * 2차 완료
 * 
 * 27    03-12-14 3:11p Sckyj
 * MediaPlayer 개조 1차작업
 * 
 * 26    03-12-04 9:45a Sckyj
 * GetBGMInfo()에서 없는 Area, Map등일 경우 bgmInfo.nBGMNo = -1로 하여
 * 반환토록 변경
 * 
 * 25    03-12-03 3:24p Sckyj
 * Play(), PlayAmbient() 수정 - GetMapBGMInfo()함수에서 bgmInfo.nBGMNo ==
 * -1일 경우는 bgm을 연주하지않고 return
 * 
 * 24    03-12-01 11:08a Sckyj
 * #define MAXBGM 위치변경
 * 
 * 23    03-11-28 10:47p Sckyj
 * 뒤엎어따.. 다 빼고 다넣고, 이제 아무거나 Loopin control 가능하다
 * 
 * 22    03-11-28 11:04a Sckyj
 * 오류 수정
 * 
 * 21    03-11-28 10:16a Sckyj
 * Map, Area 변경에 따른 특징적인 잔향( Reverb type )을 설정할수 있도록
 * 기능추가
 * 
 * 20    03-11-26 3:51p Sckyj
 * Ambient sBGM 시간대별 재생기능 추가
 * 
 * 19    03-11-12 5:46p Kjmgo
 * 임시로, Assert 주석 처리.ㅠ.ㅜ
 * 
 * 18    03-11-08 12:02p Sckyj
 * Ambient sBGM을 cross play 되도록 기능 추가
 * 
 * 17    03-11-05 11:37a Sckyj
 * Ambient sBGM관련 코드 추가(1차완료)
 * 
 * 16    03-10-10 11:07a Kjmgo
 * 메모리 해제시 stl::map 해제 수정, 함수중 nIndex를 받는 함수들에게
 * nIndex 범위 어설트,
 * 
 * 15    03-10-04 6:12a Sckyj
 * 
 * 14    03-10-03 9:09p Sckyj
 * 
 * 13    03-10-03 5:52p Sckyj
 * CBGMStream을 새로 만들었음
 * 
 * 12    03-10-03 12:01p Sckyj		Just fade out 기능 보강
 * 
 * 9     03-10-02 9:22a Sckyj
 * just fade out기능 추가 
 * 
 * 1     03-09-06 1:45p Sckyj
 * 
 * 14    03-09-06 11:29a Sckyj
 * 
 * 13    03-09-04 11:31a Paran
 * 
 * 12    03-09-03 9:19p Paran
 * Map 사용 수정.
 * 
 * 10    03-08-31 6:41p Sckyj			fade in bug 수정
 * 
 * 8     03-08-29 11:42p Sckyj			sBGM player변경, Crossplay 추가
 * 
 * <HR>
 *
 * @file	W3DMediaPlayer.cpp
 */
/*
#include <dshow.h>
#include <malloc.h>
#include "Media_Export.h"
*/

#define DIGITALRATE			44100
#define DIGITALBITS			16
#define DIGITALCHANNELS		2

#include "Sound_Export.h"
#include <mss.h>
#include <map>
#include <string>

using std::string;
using std::map;

typedef		map<int, sBGM*>		mapBGMList;
typedef		map<int, sBGMInfo*>	mapBGMInfo;

/**
 * sBGM 플레이어 클래스.
 * @ingroup		Sound
 */
class CW3DMediaPlayer : public IW3DMediaPlayer
{
public:		
	UNDER_MEMORY_MGR( CW3DMediaPlayer );

	CW3DMediaPlayer();
	~CW3DMediaPlayer();

	BOOL		Init( HWND hWnd );									///< MediaPlayer 초기화
	BOOL		Clear();											

	void		Update( DWORD dwTick, int nCurrTime );				///< BGM을 update한다.

	BOOL		Play( int nBGMType, int nTypeNum );					///< Map, Event음을 재생한다.
	BOOL		PlayArea( int nAreaCount, int* nAreaType );			///< Area BGM을 재생한다.

	void		StopAll();											///< 재생 멈춤
	void		Stop( int nIndex );						
	
	BOOL		SetVolume( float fVolume );							///< 음량설정	
	float		GetVolume();										///< 음량 얻기
	
	void		SetLastVolume( float fVolume );						
	float		GetLastVolume();	
	
	char*		GetMapBGMReverb( int nArea );						///< Area의 Reverb type을 반환한다.
	char*		GetAreaBGMReverb( int nMap );						///< Map의 Reverb type을 반환한다.

	BOOL		GetMapBGMInfo( int nMap, sBGMInfo* bgmInfo );		///< Map BGM Info를 반환한다.	
	BOOL		GetEventBGMInfo( int nEvent, sBGMInfo* bgmInfo );	///< Event BGM Info를 반환한다.
	BOOL		GetAmbientBGMInfo( int nAmbi, sBGMInfo* bgmInfo );	///< Ambient BGM Info를 반환한다.
	BOOL		GetAreaBGMInfo( int nAreaCount, int* nArea, sBGMInfo* bgmInfo );	///< Area BGM Info를 반환한다.

	char*		GetBGMName(	int nBGMNum );							///< BGM Name을 반환한다.

private :

	BOOL		MakeBGMList();										///< BGM List를 만든다	
	
	BOOL		PlayEvent( int nTypeNum );
	BOOL		PlayMap( int nTypeNum );	
	BOOL		PlayAmbient( int nTypeNum );

	CBGMMapMgr*		m_pMapMgr;
	CBGMEventMgr*	m_pEventMgr;
	CBGMAreaMgr*	m_pAreaMgr;
	CBGMAmbientMgr* m_pAmbiMgr;

	mapBGMList	m_mapBGMList;						///< BGM List
	mapBGMInfo	m_mapEventBGM;						///< Event BGM
	mapBGMInfo	m_mapMapBGM;						///< Map BGM
	mapBGMInfo	m_mapAreaBGM;						///< Area BGM
	mapBGMInfo	m_mapAmbientBGM;					///< Ambient BGM

	float		m_fLastVolume;						///< FadeOut 하기전의 원음량을 저장
	float		m_fVolume;							///< 음량
}; //CW3DMediaPlayer

IMPLEMENT_COMPONENT( CW3DMediaPlayer, MediaPlayer );

/**
 * 생성자.
 */
CW3DMediaPlayer::CW3DMediaPlayer() :
m_fVolume		( 0.2f ),
m_fLastVolume	( 0 ),
m_pMapMgr		( NULL ),
m_pAreaMgr		( NULL ),
m_pEventMgr		( NULL ),
m_pAmbiMgr		( NULL )
{	
} //CW3DMediaPlayer::CW3DMediaPlayer

/**
 * 소멸자.
 */
CW3DMediaPlayer::~CW3DMediaPlayer()
{	
	StopAll();

	mapBGMList::iterator itorBGM = m_mapBGMList.begin();
	mapBGMList::iterator itorBGMNext;
	while( itorBGM != m_mapBGMList.end() )
	{
		itorBGMNext = itorBGM;
		itorBGMNext++;

		SAFE_FREE( itorBGM->second );
		m_mapBGMList.erase( itorBGM );

		itorBGM = itorBGMNext;
	} //while

	mapBGMInfo::iterator itorEvent = m_mapEventBGM.begin();
	mapBGMInfo::iterator itorEventNext;
	while( itorEvent != m_mapEventBGM.end() )
	{
		itorEventNext = itorEvent;
		itorEventNext++;

		SAFE_FREE( itorEvent->second );
		m_mapEventBGM.erase( itorEvent );

		itorEvent = itorEventNext;
	} //while

	mapBGMInfo::iterator itorMap = m_mapMapBGM.begin();
	mapBGMInfo::iterator itorMapNext;

	int count = 0;

	while( itorMap != m_mapMapBGM.end() )
	{
		itorMapNext = itorMap;
		itorMapNext++;

		SAFE_FREE( itorMap->second );
		m_mapMapBGM.erase( itorMap );

		itorMap = itorMapNext;

		count++;
	} //while

	mapBGMInfo::iterator itorArea = m_mapAreaBGM.begin();
	mapBGMInfo::iterator itorAreaNext;

	while( itorArea != m_mapAreaBGM.end() )
	{
		itorAreaNext = itorArea;
		itorAreaNext++;

		SAFE_FREE( itorArea->second );
		m_mapAreaBGM.erase( itorArea );

		itorArea = itorAreaNext;
	} //while

	mapBGMInfo::iterator itorAmbient = m_mapAmbientBGM.begin();
	mapBGMInfo::iterator itorAmbientNext;

	while( itorAmbient != m_mapAmbientBGM.end() )
	{
		itorAmbientNext = itorAmbient;
		itorAmbientNext++;

		SAFE_FREE( itorAmbient->second );
		m_mapAmbientBGM.erase( itorAmbient );

		itorAmbient = itorAmbientNext;
	} // while	

	SAFE_FREE( m_pMapMgr );
	SAFE_FREE( m_pEventMgr );
	SAFE_FREE( m_pAmbiMgr );
	SAFE_FREE( m_pAreaMgr );
} //CW3DMediaPlayer::~CW3DMediaPlayer

/**
 * Miles 플레이어를 초기화한다.
 */
BOOL CW3DMediaPlayer::Init( HWND hWnd )
{
	m_pMapMgr	= ( CBGMMapMgr*		)_MemAlloc( sizeof( CBGMMapMgr		) );
	m_pEventMgr	= ( CBGMEventMgr*	)_MemAlloc( sizeof( CBGMEventMgr	) );
	m_pAreaMgr	= ( CBGMAreaMgr*	)_MemAlloc( sizeof( CBGMAreaMgr		) );
	m_pAmbiMgr	= ( CBGMAmbientMgr* )_MemAlloc( sizeof( CBGMAmbientMgr	) );


	if( !MakeBGMList() )
	{		
		Assert( !"sBGM List 만들기 실패" );
		return FALSE;
	} //if

	m_pMapMgr->Init( this );
	m_pEventMgr->Init( this );
	m_pAreaMgr->Init( this );
	m_pAmbiMgr->Init( this );	

	return TRUE;
} //CW3DMediaPlayer::Init

/**
 * 
 */
BOOL CW3DMediaPlayer::Clear()
{
	m_fLastVolume = m_fVolume = 0;

	m_pMapMgr->Clear();
	m_pEventMgr->Clear();
	m_pAreaMgr->Clear();
	m_pAmbiMgr->Clear();

	return TRUE;
} // CW3DMediaPlayer::Clear()

/**
 * sBGM List를 만든다
 */
BOOL CW3DMediaPlayer::MakeBGMList()
{
	int		num		= 0 ;
	int		num2	= 0 ;
	char	buf[128];	

	ZeroMemory( buf, 128 );

	FILE* fp = fopen( "Music/BGMList.ini", "rt" );
	if( NULL == fp )
	{
		Assert( !"sBGM List가 엄따" );
		return FALSE;
	} //if	

	while( fscanf( fp, "%s", buf ) != EOF)
	{		
		if( !strcmp( buf, "[BGM]" ) )
		{	
			sBGM* bgm = ( sBGM* )_MemAlloc( sizeof( sBGM ) );
			ZeroMemory( bgm, sizeof( sBGM ) );
			if( !bgm )
			{
				Assert( !"메모리할당 실패" );
				break;
			} //if
			
			fscanf( fp, "%d %s %s", &num, &bgm->szBGMName, &bgm->szBGMType );
			m_mapBGMList.insert( std::make_pair( num, bgm ) );
		}
		else
		if( !strcmp( buf, "[EVENT_BGM]" ) )
		{				
			sBGMInfo* bgmInfo = ( sBGMInfo* )_MemAlloc( sizeof( sBGMInfo ) );
			ZeroMemory( bgmInfo, sizeof( sBGMInfo ) );
			if( !bgmInfo )
			{
				Assert( !"메모리할당 실패" );
				break;
			} //if			
			
			fscanf( fp, "%d %s %d %d", &num, &bgmInfo->szInfo, &bgmInfo->bLoop, &bgmInfo->nBGMNo );			
			m_mapEventBGM.insert( std::make_pair( num, bgmInfo ) );
		}
		else
		if( !strcmp( buf, "[MAP_BGM]" ) )
		{
			sBGMInfo* bgmInfo = (sBGMInfo*)_MemAlloc( sizeof( sBGMInfo ) );
			ZeroMemory( bgmInfo, sizeof( sBGMInfo ) );
			if( !bgmInfo )
			{
				Assert( !"메모리할당 실패" );
				break;
			} //if			
			
			fscanf( fp, "%d %s %d %d", &num, &bgmInfo->szInfo, &bgmInfo->bLoop, &bgmInfo->nBGMNo );			
			m_mapMapBGM.insert( std::make_pair( num, bgmInfo ) );			
		}
		else
		if( !strcmp( buf, "[AREA_BGM]" ) )
		{
			sBGMInfo* bgmInfo = (sBGMInfo*)_MemAlloc( sizeof( sBGMInfo ) );
			ZeroMemory( bgmInfo, sizeof( sBGMInfo ) );
			if( !bgmInfo )
			{
				Assert( !"메모리할당 실패" );
				break;
			} //if

			fscanf( fp, "%d %s %d %d", &num, &bgmInfo->szInfo, &bgmInfo->bLoop, &bgmInfo->nBGMNo );	
			m_mapAreaBGM.insert( std::make_pair( num, bgmInfo ) );
		}
		else
		if( !strcmp( buf, "[AMBIENT_BGM]" ) )
		{
			sBGMInfo* bgmInfo = (sBGMInfo*)_MemAlloc( sizeof( sBGMInfo ) );
			ZeroMemory( bgmInfo, sizeof( sBGMInfo ) );
			if( !bgmInfo )
			{
				Assert( !"메모리할당 실패" );
				break;
			} //if

			fscanf( fp, "%d %s %d %d %d %d", &num, &bgmInfo->szInfo, &bgmInfo->bLoop, 
											 &bgmInfo->nBGMNo, &bgmInfo->nTime[0], &bgmInfo->nTime[1] );
			m_mapAmbientBGM.insert( std::make_pair( num, bgmInfo ) );
		} // if..else
	} // while

	fclose( fp );
	return TRUE;
} //CW3DMediaPlayer::MakeBGMList

/**
 * Map, Event BGM을 재생한다.  
 */
BOOL CW3DMediaPlayer::Play( int nType, int nTypeNum )
{
	Assert( BGM_TYPE_EVENT <= nType && nType <= BGM_TYPE_AMBIENT );

	switch( nType )
	{
	case BGM_TYPE_EVENT :	return PlayEvent( nTypeNum );
	case BGM_TYPE_MAP :		return PlayMap( nTypeNum );	
	case BGM_TYPE_AMBIENT :	return PlayAmbient( nTypeNum );
	}

	return FALSE;
} //CW3DMediaPlayer::Play

/**
 * Event BGM을 재생한다.
 */
BOOL CW3DMediaPlayer::PlayEvent( int nEventNum )
{
	sBGMInfo bgmInfo;
	if( !GetEventBGMInfo( nEventNum, &bgmInfo ) )
	{
		return FALSE;
	}	

	// 같은 번호의 Event BGM을 play 중일경우 return
	if( m_pEventMgr->IsSame( nEventNum ) && !m_pEventMgr->IsEnd() )
	{
		return TRUE;
	}

	StopAll();		// 여타 재생중이던 모든 bgm을 멈춘다.

	m_pEventMgr->SetBGMInfo( bgmInfo );
	m_pEventMgr->Play( nEventNum );	

	return TRUE;
} // CW3DMediaPlayer::PlayEvent

/**
 * Map BGM을 재생한다.
 */
BOOL CW3DMediaPlayer::PlayMap( int nMapNum )
{
	sBGMInfo bgmInfo;
	if( !GetMapBGMInfo( nMapNum, &bgmInfo ) )
	{		
		return FALSE;
	}
	
	m_pMapMgr->SetBGMInfo( bgmInfo );
	m_pMapMgr->Play( nMapNum );

	return TRUE;
} // CW3DMediaPlayer::PlayMap

/**
 * Area BGM을 재생한다.
 */
BOOL CW3DMediaPlayer::PlayArea( int nAreaCount, int* nAreaType )
{	
	sBGMInfo bgmInfo[MAXBGM];
	if( !GetAreaBGMInfo( nAreaCount, nAreaType, bgmInfo ) )
	{		
		return FALSE;
	}
	
	m_pAreaMgr->SetBGMInfo( nAreaCount, bgmInfo );
	m_pAreaMgr->Play( nAreaCount, nAreaType );

	return TRUE;
} // CW3DMediaPlayer::PlayArea

/**
 * Ambient BGM을 재생한다.
 */
BOOL CW3DMediaPlayer::PlayAmbient( int nAmbiNum )
{
	sBGMInfo bgmInfo;
	if( !GetAmbientBGMInfo( nAmbiNum, &bgmInfo ) )
	{
		return FALSE;
	}	

	m_pAmbiMgr->SetBGMInfo( bgmInfo );
	m_pAmbiMgr->Play( nAmbiNum );

	return TRUE;
} //CW3DMediaPlayer::Play

/**
 * 미디어의 볼륨을 설정한다.
 * 볼륨은 0~100 사이의 값으로 설정한다.
 *
 * @param	volume			[IN} 볼륨
 */
BOOL CW3DMediaPlayer::SetVolume( float fVolume )
{
	if( fVolume >= MAXVOLUME )
	{
		m_fVolume = MAXVOLUME;		
	}
	else
	if( fVolume <= MINVOLUME )
	{
		m_fVolume = MINVOLUME;		
	}
	else
	{
		m_fVolume = fVolume;
		m_pMapMgr->SetVolume( m_fVolume );		
		m_pAmbiMgr->SetVolume( m_fVolume );
		m_pAreaMgr->SetVolume( m_fVolume );
	}	

	return TRUE;
} //CW3DMediaPlayer::SetVolume

/**
 * 미디어의 볼륨을 얻어온다
 */
float CW3DMediaPlayer::GetVolume()
{
	return m_fVolume;
} //CW3DMediaPlayer::GetVolume

/**
 *
 */
void CW3DMediaPlayer::SetLastVolume( float fVolume )
{
	Assert( fVolume >= 0 );
	m_fLastVolume = fVolume;
} // CW3DMediaPlayer::SetLastVolume

/**
 * 
 */
float CW3DMediaPlayer::GetLastVolume()
{
	return m_fLastVolume;
} // CW3DMediaPlayer::GetLastVolume

/**
 * 연주를 멈춘다. 
 */
void CW3DMediaPlayer::StopAll()
{
	m_pMapMgr->Stop();
	m_pEventMgr->Stop();
	m_pAreaMgr->Stop();
	m_pAmbiMgr->Stop();
} //CW3DMediaPlayer::Stop

/**
 * MediaPlayer를 업데이트한다 
 */
void CW3DMediaPlayer::Update( DWORD dwTick, int nCurrTime )
{
	m_pMapMgr->Update( dwTick );
	m_pEventMgr->Update( dwTick );
	m_pAmbiMgr->Update( dwTick, nCurrTime );
	m_pAreaMgr->Update( dwTick );	
} //CW3DMediaPlayer::Update

/**
 * BGM File Name을 반환한다.
 *
 * @param	nBGM		[IN] sBGM Number
 */
char* CW3DMediaPlayer::GetBGMName( int nBGM )
{
	mapBGMList::iterator itor = m_mapBGMList.find( nBGM );
	if( itor == m_mapBGMList.end() )
	{
	//	Assert( !"없는 BGM" );
		return NULL;
	} //if

	return itor->second->szBGMName;
} //CW3DMediaPlayer::GetBGMName


/**
 *	Event BGM Info를 반환한다.
 *
 * @param	nEventNum	[IN] Event 번호를 입력한다.
 *
 */
BOOL CW3DMediaPlayer::GetEventBGMInfo( int nEventNum, sBGMInfo* bgmInfo )
{
	mapBGMInfo::iterator itor = m_mapEventBGM.find( nEventNum );
	if( itor == m_mapEventBGM.end() )
	{
		bgmInfo = NULL;
		return FALSE;
	} //if

	memcpy( bgmInfo, itor->second, sizeof( sBGMInfo ) );

	return TRUE;
} // CW3DMediaPlayer::GetEventBGMInfo

/**
 *	Map BGM Info를 반환한다.
 *
 * @param	nMapNum		[IN] 맵번호를 입력한다
 *
 */
BOOL CW3DMediaPlayer::GetMapBGMInfo( int nMapNum, sBGMInfo* bgmInfo )
{	
	mapBGMInfo::iterator itor = m_mapMapBGM.find( nMapNum );
	if( itor == m_mapMapBGM.end() )
	{
		bgmInfo = NULL;
		return FALSE;
	} //if

	memcpy( bgmInfo, itor->second, sizeof( sBGMInfo ) );

	return TRUE;
} // CW3DMediaPlayer::GetMapBGMInfo

/**
 * Area BGM Info를 반환한다.
 *
 * @param	nAreaNum	[IN] 지역번호를 입력한다. - 지역번호를 모든 맵들이 공유한다.
 *
 */
BOOL CW3DMediaPlayer::GetAreaBGMInfo( int nAreaCount, int* nAreaNum, sBGMInfo* bgmInfo )
{	
	for( int i=0 ; i<nAreaCount ; i++ )
	{
		mapBGMInfo::iterator itor = m_mapAreaBGM.find( nAreaNum[i] );
		if( itor == m_mapAreaBGM.end() )
		{
			return FALSE;
		} //if
		else
		{
			memcpy( bgmInfo + i, itor->second, sizeof( sBGMInfo ) );			
		}
	}	

	return TRUE;
} // CW3DMediaPlayer::GetAreaBGMInfo

/**
 * Ambient BGM Info를 반환한다.
 *
 * @param	nAmbiNum	[IN] 맵번호를 입력한다 - 환경음은 맵마다 부여하기때문에 맵번호를 입력
 *
 */
BOOL CW3DMediaPlayer::GetAmbientBGMInfo( int nAmbiNum, sBGMInfo* bgmInfo )
{	
	mapBGMInfo::iterator itor = m_mapAmbientBGM.find( nAmbiNum );
	if( itor == m_mapAmbientBGM.end() )
	{
		bgmInfo = NULL;
		return FALSE;
	} //if

	memcpy( bgmInfo, itor->second, sizeof( sBGMInfo ) );
	
	return TRUE;
} // CW3DMediaPlayer::GetAmbientBGMInfo

/**
 * Map BGM Reverb type을 반환한다.
 */
char* CW3DMediaPlayer::GetMapBGMReverb( int nMap )
{
	mapBGMInfo::iterator itor = m_mapMapBGM.find( nMap );
	if( itor == m_mapMapBGM.end() )
	{
	//	Assert( !"없는 Map" );
		return "";
	} //if

	return itor->second->szInfo;
} // CW3DMediaPlayer::GetMapBGMReverb

/**
 * Area BGM Reverb Type을 반환한다.
 */
char* CW3DMediaPlayer::GetAreaBGMReverb( int nArea )
{
	mapBGMInfo::iterator itor = m_mapAreaBGM.find( nArea );
	if( itor == m_mapAreaBGM.end() )
	{
	////	Assert( !"없는 Area" );
		return "";
	} //if

	return itor->second->szInfo;
} // CW3DMediaPlayer::GetAreaBGMReverb