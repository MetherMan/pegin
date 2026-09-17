/**
 * Sound Sync.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 16 $
 *
 * <HR>
 * $Log: /GameClient/Src/SoundSync.cpp $
 * 
 * 16    05-02-18 7:38p Sk8snow2
 * 
 * 15    03-09-10 4:39p Sckyj
 * 
 * 14    03-07-23 3:41p Kjmgo
 * 
 * 13    03-06-27 5:30p Kjmgo
 * Assert처리
 * 
 * 12    03-05-10 12:54p Ahastudio
 * Engine에서 Creature Implement 관련 클래스를 Game으로 이동함.
 *
 * 10    03-04-02 10:21p Kjmgo		메모리 해제 관련 버그 한개 수정
 * <HR>
 *
 * @file	SoundSync.cpp
 */


#include "Global.h"
#include "SoundSync.h"


/**
 * 생성자.
 */
CSoundSyncMgr::CSoundSyncMgr()
:	m_pSync( NULL ),
	m_pSound( NULL ),
	m_bUseSync( FALSE )
{
} //CSoundSyncMgr::CSoundSyncMgr

/**
 * 소멸자.
 */
CSoundSyncMgr::~CSoundSyncMgr()
{
	SYNCMAP::iterator	itor = m_mapSync.begin();
	SYNCMAP::iterator	itorNext;

	while( itor != m_mapSync.end() )
	{
		itorNext = itor;
		itorNext++;

		SAFE_DELETE( itor->second );
		m_mapSync.erase( itor );

		itor = itorNext;
	} //while

	SAFE_RELEASE( m_pSound );
} //CSoundSyncMgr::~CSoundSyncMgr

/**
 * 초기화.
 */
BOOL CSoundSyncMgr::Init()
{
	m_pSync = NULL;

	m_pSound = MAKE_COMPONENT( SoundObject );
	CHECK_PTR_RETURN( m_pSound );

	m_bUseSync = FALSE;

	return TRUE;
} //CSoundSyncMgr::Init

/**
 *
 */
BOOL CSoundSyncMgr::Load( const char* szPath, const char* szFileName )
{
	std::string		sFile = szPath;
	sFile += "/";
	sFile += szFileName;
	sFile += ".wsl";

	FILE*	fp = fopen( sFile.c_str(), "rt" );
	if( !fp )
	{
		return FALSE;
	} //if
 
	char szBuffer[256];
	char szFile[256];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );

		int nAction = 0;
		DWORD dwLimitTime = 0;
		
		if( sscanf( szBuffer, "%d %s %d", &nAction, szFile, &dwLimitTime ) != 3 )
		{
			Assert( !"sscanf 오류" );
		}


		dwLimitTime = ( dwLimitTime * 1000 ) / 30;

		sFile = szPath;
		sFile += "/";
		sFile += szFile;
		sFile += ".wsd";
	
		Load( sFile.c_str() , nAction, dwLimitTime );
	} //while

	fclose( fp );

	return TRUE;
}//CSoundSyncMgr::Load

/**
 *	Sync Data 파일을 로드하고, Key로 사용될 Action을 입력받는다.
 */
BOOL CSoundSyncMgr::Load(
	const char*		szFileName,		///< [IN]
	const int		nAction,		///< [IN]
	const DWORD		dwLimitTime		///< [IN]
	)
{
	CSoundSync*		pSync = new CSoundSync;
	CHECK_PTR_RETURN( pSync );

	if( !pSync->Load( szFileName ) || !pSync->Init( nAction ) )
	{
		SAFE_DELETE( pSync );
		return FALSE;
	} //if

	pSync->SetLimitTime( dwLimitTime );

	SYNCMAP::iterator itor = m_mapSync.find( nAction );

	if( itor != m_mapSync.end() )
	{
		SAFE_DELETE( pSync );
		return FALSE;
	} //if

	m_mapSync.insert( std::make_pair( nAction, pSync ) );
	
	return TRUE;
} //CSoundSyncMgr::Load

/**
 *	현재 사용되고 있는 Action을 변경한다.
 */
BOOL CSoundSyncMgr::SelectAction( int nAction )
{
	if( m_pSync )
	{
		if( m_pSync->GetState() != nAction )
		{
			m_bUseSync = FALSE;
		} //if
	} //if

	SYNCMAP::iterator	itor = m_mapSync.find( nAction );

	if( !m_bUseSync && itor != m_mapSync.end()  )
	{	// 존재하는 액션인 경우...
		m_itorSync = itor;

		m_pSync = m_itorSync->second;
		Assert( m_pSync );

		if( !m_pSync )
		{
			return FALSE;
		} //if

		if( m_pSync->Init( nAction ) )
		{
			m_bUseSync = TRUE;	
			return TRUE;
		} //if
	} //if

	return FALSE;
} //CSoundSyncMgr::SelectAction

/**
 *
 */
BOOL CSoundSyncMgr::Update( DWORD dwTick )
{
	if( !m_pSync || m_pSync->IsEnd() )
	{
		m_bUseSync = FALSE;
		return FALSE;
	} //if

	if( m_pSync->Update( dwTick ) )
	{
		std::string sFile = m_pSync->GetSoundFile();		
		m_pSound->Play3D( sFile.c_str() );

		if( m_pSync->NextStep() )
		{
			return TRUE;
		} //if
	} //if

	return TRUE;
} //CSoundSyncMgr::Update
