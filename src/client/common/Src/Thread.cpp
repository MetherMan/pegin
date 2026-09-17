/**
 * Thread
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 04-01-30 7:32p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Common/Src/Thread.cpp $
 * 
 * 1     04-01-30 7:32p Kjmgo
 * 
 * 1     04-01-30 7:31p Kjmgo
 * 
 * 1     03-12-05 4:55a Kjmgo
 * 
 * <HR>
 *
 * @file	Thread.cpp
 */

#include "../Inc/Common.h"

//---------------------------
//	Thread Process
//---------------------------
DWORD _stdcall ThreadProc( void* agr )
{
	if( !agr )
	{
		return 0;
	} //if

	IThread*	pThread = (IThread*)agr;

	return pThread->Process(NULL);
} //ThreadProc

