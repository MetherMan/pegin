/**
 * Network Prcess ( Using Thread )
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 04-01-26 11:27a $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /GameClient/Src/NetworkProcess.cpp $
 * 
 * 1     04-01-26 11:27a Kjmgo
 * <HR>
 *
 * @file	NetworkProcess.cpp
 */

#include "Global.h"
//#include "Game.h"
//#include "NetworkProcess.h"

//
namespace 
{
	CGame*	g_pGame;		// class CGame
	BOOL	g_bExit;
} //namespace

/**
 *
 */
CNetworkProcess::CNetworkProcess()
{
	g_pGame = NULL;
	g_bExit = FALSE;
} //CNetworkProcess::CNetworkProcess

/**
 *
 */
CNetworkProcess::~CNetworkProcess()
{
	ShutDown();
} //CNetworkProcess::~CNetworkProcess

/**
 *
 */
BOOL CNetworkProcess::Create( CGame* pGame )
{
	Assert( pGame );

	if( !pGame )	return FALSE;

	g_pGame = pGame;

	return Open();
} //CNetworkProcess::Create

/**
 *
 */
BOOL CNetworkProcess::Open()
{
	HANDLE hResult = CreateThread( NULL, 0, Process, NULL, NULL, NULL );

	if( !hResult )
	{
		// Can't Create Thread
		return FALSE;
	} //if

	g_bExit = FALSE;
	return TRUE;
} //CNetworkProcess::Open

/**
 *
 */
void CNetworkProcess::ShutDown()
{
	g_bExit = TRUE;
} //CNetworkProcess::ShutDown

/**
 *
 */
DWORD _stdcall CNetworkProcess::Process( void* agr )
{
	DEBUG_OUT( "+*+*+*+*+ Start Network Thread +*+*+*+*+" );

	Assert( g_pInput );
	Assert( g_pClient );

	while( !g_bExit )
	{	
		// Sleep 1ms
		Sleep( 1 );

		g_pGame->UpdateNetwork();
	} //while
	return TRUE;
} //CNetworkProcess::Process
