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
 * $Log: /GameClient/Src/NetworkProcess.h $
 * 
 * 1     04-01-26 11:27a Kjmgo
 * <HR>
 *
 * @file	NetworkProcess.h
 */

#ifndef __NetworkProcess_Header__
#define __NetworkProcess_Header__

/**
 *
 */
class CNetworkProcess
{
public:
	MEMORY_UNDER_MGR( CNetworkProcess );

	// Constructor And Destructor
		CNetworkProcess();
		~CNetworkProcess();
	// ~Constructor And Destructor 

	// Create
	BOOL Create( CGame* pGame );

	/**
	 *
	 */
	static DWORD _stdcall Process( void* agr );


private:
	
	/**
	 *
	 */
	BOOL Open();

	/**
	 *
	 */
	void ShutDown();


}; //class CNetworkProcess

#endif //#ifndef __NetworkProcess_Header__