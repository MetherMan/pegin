/**
 * 크리티컬 섹션
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-30 7:26p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Common/Inc/CriticalSection.h $
 * 
 * 3     04-01-30 7:26p Kjmgo
 * 
 * 2     04-01-29 10:09a Kjmgo
 * 
 * 1     04-01-29 9:55a Kjmgo
 * Critical Section 생성
 * 
 * 
 * <HR>
 *
 * @file	CriticalSection.h
 */

#ifndef __Common_CriticalSection_Header__
#define __Common_CriticalSection_Header__

/**
 *
 */
class COMMON_API CCriticalSection
{
public:
	UNDER_MEMORY_MGR( CCriticalSection );

	/**
	 *	Constructor
	 */
	CCriticalSection()
	{
		InitializeCriticalSection( &m_csLock );
	} //CCriticalSection

	/**
	 *	Destructor
	 */
	~CCriticalSection()
	{
		DeleteCriticalSection( &m_csLock );
	} //~CCriticalSection

	/**
	 *	Locking 
	 */
	void Lock()
	{
		EnterCriticalSection( &m_csLock );
	} //Lock

	/**
	 *
	 */
	void Unlock()
	{
		LeaveCriticalSection( &m_csLock );
	} //Unlock

private:
	CRITICAL_SECTION	m_csLock;

}; //class CCriticalSection

/**
 *
 */
class CLocker
{
public:
	// Constructor
	CLocker( CCriticalSection& cs )
		:	m_CriticalSection	( cs )
	{
		m_CriticalSection.Lock();
	} //CLocker

	// Destructor
	~CLocker()
	{
		m_CriticalSection.Unlock();
	} //~CLocker

private:
	CCriticalSection&	m_CriticalSection;
}; //class CLocker

#endif //#ifndef __Common_CriticalSection_Header__