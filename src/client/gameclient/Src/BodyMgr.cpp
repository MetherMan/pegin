/**
 * Body 모델 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-02-02 11:43a $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /GameClient/Src/BodyMgr.cpp $
 * 
 * 2     04-02-02 11:43a Kjmgo
 * 바디 메니저 생성
 * 
 * 1     04-01-31 3:47p Kjmgo
 * 
 *  
 * <HR>
 *
 * @file	BodyMgr.cpp
 */

#include "Global.h"
#include "BodyMgr.h"

DWORD _stdcall ThreadProc( void* agr )
{
	if( !agr )
	{
		return 0;
	} //if

	IThread*	pThread = (IThread*)agr;

	return pThread->Process(NULL);
} //ThreadProc

/**
 *
 */
BOOL CBodyManager::Process( void* agr )
{
	SBlock* pBlock = NULL;

	
	while( !IsQuit() )
	{
		Sleep( 1 );

		pBlock = Pop();

		if( !pBlock )
		{
			Sleep( 10 );
			continue;
		} //if

		Load( pBlock );
		SAFE_DELETE( pBlock );
		pBlock = NULL;
	} //while
	return TRUE;
} //CBodyMgr::Process

/**
 *
 */
void CBodyManager::Load( SBlock* pBlock )
{
	if( !pBlock )	return ;

	IW3DModel* pModel = NULL;
	pModel = Find( pBlock->szType, pBlock->szName );
	if( pModel )
	{
		m_csMap.Lock();
			*pBlock->ppModel = pModel;
			pModel->AddRef();
		m_csMap.Unlock();
		return ;
	} //if
	
	pModel = MAKE_COMPONENT( Model );
	if( !pModel )	return ;
	
	char szBuffer[128];
	sprintf( szBuffer, "%s/%s", pBlock->szType, pBlock->szName );

	if( !pModel->Load( pBlock->szPath, szBuffer ) )
	{
		char	szFile[128];
		sprintf( szFile, "%s - 캐릭터 모델 로딩 실패", pBlock->szName );
		Assert( !szFile );
		SAFE_RELEASE( pModel );
		return ;
	} //if

	m_csMap.Lock();
		*pBlock->ppModel = pModel;
	//	m_mapModel.insert( std::make_pair( std::string( szBuffer ), pModel ) );
		m_mapModel.insert( std::make_pair( std::string( pBlock->szName ), pModel ) );
	m_csMap.Unlock();
} //CBodyManager::Load