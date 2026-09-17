/**
 * Sound Sync...
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-06-27 5:30p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /GameClient/Src/SoundSync.h $
 * 
 * 11    03-06-27 5:30p Kjmgo
 * Assert처리
 * 
 * 10    03-06-01 4:23p Kjmgo
 * 
 * 9     03-04-04 5:32a Ahastudio
 * <HR>
 *
 * @file	SoundSync.h
 */


#ifndef __SoundSync_Header__
#define __SoundSync_Header__


#include <string>
#include <map>
#include <list>


class CSoundSync;


/**
 * 동작에 맞는 효과음을 Play한다.
 * @ingroup		Engine
 */
class CSoundSyncMgr
{
public:
	UNDER_MEMORY_MGR( CSoundSyncMgr );

	CSoundSyncMgr();
	~CSoundSyncMgr();

	BOOL	Init();

	BOOL	Load( const char* szPath, const char* szFileName );
	BOOL	Load( const char* szFileName, const int nAction, const DWORD dwLimitTime );

	BOOL	SelectAction( int nAction );

	BOOL	Update( DWORD dwTick );

	void	SetUseSync( BOOL bUse )
	{
		m_bUseSync = bUse;
	} //SetUseSync

	void	SetPos( Vector3 vPos, Vector3 vDir )
	{
		m_pSound->SetPosition( vPos );
	}

private:
	typedef std::map<int, CSoundSync*>		SYNCMAP;

	SYNCMAP				m_mapSync;			///<
	SYNCMAP::iterator	m_itorSync;			///<
	CSoundSync*			m_pSync;			///<
	IW3DSoundObject*	m_pSound;			///<

	BOOL				m_bUseSync;			///<
}; //class CSoundSyncMgr


/**
 * 동작에 맞는 효과음을 Play한다.
 * @ingroup		Engine
 */
class CSoundSync
{
public:
	UNDER_MEMORY_MGR( CSoundSync );

	/**
	 * 생성자.
	 */
	CSoundSync()
	{
		m_dwTime		 = 0;
		m_dwLimitTime	 = 0;
		m_nState		 = 0;
	} //CSoundSync

	/**
	 *소멸자.
	 */
	~CSoundSync()
	{
		SYNCLIST::iterator	itor = m_listSync.begin();
		while( itor != m_listSync.end() )
		{
			SAFE_DELETE( *itor );
			itor = m_listSync.erase( itor );
		} //while
	} //~CSoundSync

	/**
	 * 초기화.
	 */
	BOOL	Init( int nState )
	{
		m_dwTime = 0;
		m_nState = nState;

		if( !m_listSync.empty() )
		{
			m_itorSync = m_listSync.begin();
			return TRUE;
		} //if

		return FALSE;
	} //Init

	/**
	 *
	 */
	int		GetState()
	{
		return m_nState;
	} //GetState

	/**
	 *
	 */
	void	SetLimitTime( DWORD dwTime )
	{
		m_dwLimitTime = dwTime;
	} //SetLimitTime

	/** 
	 * Sync 파일을 로드 한다..
	 *
	 * @param	szFileName		[IN]	Sync파일 
	 *
	 * @return	파일 로드 성공 여부
	 */
	BOOL	Load( const char* szFileName )
	{
		FILE*	fp = fopen( szFileName, "rb" );
		if( !fp )
		{
			Assert( !"파일이 없어요" );
			return FALSE;
		} //if

		int		nVersion = 0;
		int		nActionSize = 0;

		char	szAction[256];

		fread( &nVersion, 4, 1, fp );
		fread( &nActionSize, 4, 1, fp );

		fread( &szAction, nActionSize, 1, fp );

		int nCount = 0;

		fread( &nCount, 4, 1, fp );

		for( int i = 0 ; i < nCount ; i++ )
		{	
			DWORD dwTick = 0;
			fread( &dwTick, 4, 1, fp );

			int nSize = 0;
			char szFileName[256];

			fread( &nSize, 4, 1, fp );
			fread( szFileName, nSize, 1, fp );

			szFileName[nSize] = 0;

			SYNCDATA* pData = new SYNCDATA;

			if( pData )
			{
				pData->dwTime = dwTick;
				pData->sFile  = szFileName;

				m_listSync.push_back( pData );
			} //if
		} //for

		fclose( fp );

		m_itorSync = m_listSync.begin();

		return TRUE;
	} //Load

	/** 
	 * Update.
	 *
	 * @param	dwTick		[IN]	경과 시간
	 */
	BOOL Update( DWORD dwTick )
	{
		m_dwTime += dwTick;

		if( m_itorSync == m_listSync.end() )
		{
			return FALSE;
		} //if

		SYNCDATA*	pData = *m_itorSync;

		if( !pData )
		{
			return FALSE;
		} //if

		if( pData->dwTime <= m_dwTime )
		{
			// Play Sound
			m_dwTime -= pData->dwTime;
			return TRUE;
		} //if

		return FALSE;
	} //Update

	/** 
	 * Sound File명을 리턴한다.
	 */
	std::string GetSoundFile() 
	{	
		SYNCDATA*	pData = *m_itorSync;
		Assert( pData );

		if( !pData )
		{
			return "";
		} //if
		return pData->sFile;
	} //GetSoundFile
 
	/**
	 *
	 */
	BOOL NextStep()
	{
		m_itorSync++;
		return TRUE;
	} //NextStep

	/**
	 *
	 */
	BOOL IsEnd()
	{
		if( m_dwTime >= m_dwLimitTime )
		{
			return TRUE;
		} //if

//		if( m_itorSync == m_listSync.end() )
//		{
//			return TRUE;
//		} //if

		return FALSE;
	} //IfEnd

private:
	struct SYNCDATA
	{
		DWORD			dwTime;		///< 누적된 Tick
		std::string		sFile;
	}; //struct SYNCDATA

	typedef std::list<SYNCDATA*>	SYNCLIST;

	DWORD				m_dwTime;			///< 누적된 Time

	DWORD				m_dwLimitTime;		///< 총 플레이 Time
	SYNCLIST::iterator	m_itorSync;
	SYNCLIST			m_listSync;			///<
	int					m_nState;			///<
}; //class CSoundSync


#endif // #ifndef __SoundSync_Header__
