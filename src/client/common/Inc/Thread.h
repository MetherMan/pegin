/**
 * Thread
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 04-01-30 9:24p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Common/Inc/Thread.h $
 * 
 * 2     04-01-30 9:24p Kjmgo
 * 
 * 1     04-01-30 7:31p Kjmgo
 * 
 * 1     03-12-05 4:55a Kjmgo
 * 
 * <HR>
 *
 * @file	Thread.h
 */

#ifndef __Thread_Header__
#define __Thread_Header__

// Thread Process
	DWORD _stdcall ThreadProc( void* agr );
// ~Thread Process

//-------------------------
//	Thread
//-------------------------
class COMMON_API IThread
{
public:

	// Constructor
	IThread()
	:	m_hThread	( NULL ),
		m_bClose	( FALSE ),
		m_bQuit		( FALSE )
	{
	} //CThread
	// ~Constructor

	//-------------------------
	//	Create Thread
	//-------------------------
	BOOL	Create()
	{
		m_bQuit = FALSE;
		m_bClose = FALSE;
		m_hThread = CreateThread( NULL, 0, ThreadProc, this, NULL, NULL );

		if( !m_hThread )
		{
			return FALSE;
		} //if

		return TRUE;
	} //Create

	// Kill Process
		void	Kill()
		{
			m_bQuit	= TRUE;

			for( int i = 0 ; i < 10 ; i++ )
			{
				if( m_bClose )
				{
					break;
				} //if

				Sleep( 500 );

				if( i > 4 )
				{
					Terminate();
					m_bClose = TRUE;
					break;
				} //if
				i++;
			} //while

		} //Kill
	// ~Kill Process

	void Terminate()
	{
		TerminateThread( m_hThread, 0 );
	} //Terminate

	// Quit
	BOOL	IsQuit()
	{
		return m_bQuit;
	} //IsQuit

	void	SetClose( BOOL bClose )
	{
		m_bClose = bClose;
	} //SetClose

	// Virtual Function
	virtual BOOL	Process( void* agr ) = 0;	

private:
	HANDLE		m_hThread;	// Handle
	BOOL		m_bQuit;	// Quit	
	BOOL		m_bClose;
}; //class IThread

#endif //#ifndef __Thread_Header__