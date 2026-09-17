/**
 * Wind3D 메모리 관리자.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-12-08 3:06a $
 * @version	$Revision: 34 $
 *
 * <HR>
 * $Log: /Common/Src/Memory.cpp $
 * 
 * 34    04-12-08 3:06a Sk8snow2
 * 
 * 33    04-04-06 6:00p Servantes
 * 
 * 32    04-01-16 8:03p Kjmgo
 * 
 * 31    04-01-16 8:01p Kjmgo
 * 
 * 30    04-01-08 10:49a Kjmgo
 * 
 * 29    03-12-20 6:18a Kjmgo
 * 메모리 관리
 * 
 * 28    03-12-14 2:33p Kjmgo
 * 
 * 27    03-11-27 5:27p Kjmgo
 * 메모리 생성시 1메가 단위로, 추가 생성시에, DEBUG_HELPER에 출력하도록
 * 수정
 * 
 * 26    03-09-18 10:10p Kjmgo
 * 
 * 25    03-08-22 5:27p Kjmgo
 * 
 * 24    03-04-08 12:54a Ahastudio
 *
 * 18    03-03-17 7:32a Ahastudio	DEBUG에서 메모리 할당시 디버그 정보 입력 방법 변경.
 * <HR>
 *
 * @file	Memory.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"
#include <memory.h>
#include <string>
#include <map>


using namespace std;

MemoryMgr	GlobalMemory;		///< 전역 메모리 관리자.


/**
 * 생성자.
 */
MemoryMgr::MemoryMgr()
{
	m_nNumMemAlloc	= 0;
	m_dwTotalSize	= 0;
	m_dwGarbageSize	= 0;
#ifdef _DEBUG_SK8SNOW
	_ZeroMemory( m_aMemBlock, sizeof( m_aMemBlock ) );
#endif
} //MemoryMgr::MemoryMgr

/**
 * 소멸자.
 */
MemoryMgr::~MemoryMgr()
{
	CheckFree();

	Assert( 0 == m_nNumMemAlloc && "메모리 할당/해제에 문제가 있었습니다" );
} //MemoryMgr::~MemoryMgr

/**
 * 메모리 할당.
 */
void* MemoryMgr::Alloc( int size )
{
#ifdef _DEBUG_SK8SNOW
	if( m_nNumMemAlloc >= MAX_ALLOCATION )
	{
		Assert( !"메모리 할당 갯수를 넘었습니다." );
		throw	bad_alloc();
		return NULL;
	} //if

	if( size <= 0 )
	{
		DEBUG_OUT( "Size 가 0인 메모리를 생성." );
		Assert( 0 );
		return NULL;
	} //if

	int		nSize	= size + PADDING_SIZE * 2;
	void*	ptr		= malloc( nSize );

	if( NULL == ptr )
	{
		Assert( !"메모리 할당 실패.." );
		throw bad_alloc();
		return NULL;
	} //if

	_MemSet( ptr, GARBAGE, nSize );

	m_nNumMemAlloc++;

	MemBlockInfo	info;
	info.bAllocated  = TRUE;
	info.pActualPtr  = ptr;
	info.nActualSize = nSize;
	info.ptr         = (BYTE*)ptr + PADDING_SIZE;
	info.size        = size;
	info.nLine       = m_nLine;
	strcpy( info.szFile, m_szFile );

	for( int i = 0 ; i < MAX_ALLOCATION ; i++ )
	{
		if( !m_aMemBlock[i].bAllocated )
		{
			m_aMemBlock[i] = info;
			break;
		} //if
	} //for

	Assert( i < MAX_ALLOCATION );

	*m_szFile = NULL;
	m_nLine = 0;
	
	m_dwTotalSize += size;
	m_dwGarbageSize += info.nActualSize;

	return info.ptr;
#else		// Release Mode..
	return malloc( size );

#endif // #ifdef _DEBUG_SK8SNOW

} //MemoryMgr::Alloc

/**
 * 메모리 할당.
 */
void* MemoryMgr::Alloc( int nSize, const char* szFile, int nLine )
{
	Assert( szFile );
	Assert( nLine > 0 );
	strcpy( m_szFile, szFile );
	m_nLine = nLine;

	return Alloc( nSize );
} //MemoryMgr::Alloc

/**
 * 메모리 해제.
 */
void MemoryMgr::Free( void* ptr )
{
#ifdef _DEBUG_SK8SNOW

	Assert( ptr && "NULL 포인터를 메모리 해제하려고 했습니다." );

	int i;

	// 메모리 블럭 정보 검색..

	for( i = 0 ; i < MAX_ALLOCATION ; i++ )
	{
		if( m_aMemBlock[i].bAllocated &&
			m_aMemBlock[i].ptr == ptr ) 
		{
			break;
		} //if
	} //for

	if( i >= MAX_ALLOCATION )
	{
		Assert( !"할당되지 않은 메모리를 해제하려고 했습니다." );
		return;
	} //if

	MemBlockInfo&	info = m_aMemBlock[i];

	// 메모리 여유 할당 영역( 앞, 뒤 )에 Garbage 값이 아닌
	// 다른 값이 들어있다면 메모리 경계 위반이다 +_+

	for( i = 0 ; i < PADDING_SIZE ; i++ )
	{
		BYTE data;

		data = *( (BYTE*)info.pActualPtr + i );
		Assert( data == GARBAGE && "메모리 경계 위반" );

		data = *( (BYTE*)info.pActualPtr + PADDING_SIZE + info.size + i );
		Assert( data == GARBAGE && "메모리 경계 위반" );
	} //for

	// 메모리를 Garbage 값으로 채워준다.

	_MemSet( info.pActualPtr, GARBAGE, info.nActualSize );
	m_dwGarbageSize -= info.nActualSize;

	// 메모리 해제

	free( info.pActualPtr );

	// 메모리 블럭 정보 삭제

	info.bAllocated = FALSE;

	m_nNumMemAlloc--;
	m_dwTotalSize -= info.size;

#else		// Release Mode..

	free( ptr );

#endif // #ifdef _DEBUG_SK8SNOW

} //MemoryMgr::Free

//
// MemoryMgr::CheckBounce
//
BOOL MemoryMgr::CheckBounce( void* ptr )
{
#ifdef _DEBUG_SK8SNOW

	Assert( ptr && "NULL 포인터를 메모리 경계 검사하려고 했습니다." );

	int i;

	// 메모리 블럭 정보 검색..

	for( i = 0 ; i < MAX_ALLOCATION ; i++ )
	{
		if( m_aMemBlock[i].bAllocated &&
			m_aMemBlock[i].ptr == ptr )
		{
			break;
		} //if
	} //for

	if( i >= MAX_ALLOCATION )
	{
		Assert( !"할당되지 않은 메모리의 경계 검사를 하려고 했습니다." );
		return FALSE;
	} //if

	MemBlockInfo&	info = m_aMemBlock[i];

	// 메모리 여유 할당 영역( 앞, 뒤 )에 Garbage 값이 아닌
	// 다른 값이 들어있다면 메모리 경계 위반이다 +_+

	for( i = 0 ; i < PADDING_SIZE ; i++ )
	{
		BYTE data;

		data = *( (BYTE*)info.pActualPtr + i );
		if( data != GARBAGE )
		{
			return FALSE;
		} //if

		data = *( (BYTE*)info.pActualPtr + PADDING_SIZE + info.size + i );
		if( data != GARBAGE )
		{
			return FALSE;
		} //if
	} //for

	return TRUE;
#else		// Release Mode..
	return TRUE;
#endif // #ifdef _DEBUG_SK8SNOW
} //MemoryMgr::CheckBounce

/**
 * 해제되지 않은 메모리 블럭이 존재하나 검사한다.
 */
BOOL MemoryMgr::CheckFree()
{
#ifdef _DEBUG_SK8SNOW 
	if( m_dwGarbageSize > 0 )
	{
		Assert( !"해제 되지 않은 메모리" );
	} //if

	if( m_nNumMemAlloc )
	{
		for( int i = 0 ; i < MAX_ALLOCATION ; i++ )
		{
			if( m_aMemBlock[i].bAllocated )
			{
				char*	szFile	= m_aMemBlock[i].szFile;
				int		nLine	= m_aMemBlock[i].nLine;

				DEBUG_OUT( "해제되지 않은 메모리 블럭 - " + szFile + " " + nLine );
				Assert( !"해제되지 않은 메모리 블럭" );
			} //if
		} //for

		return FALSE;
	} //if
#endif // #ifdef _DEBUG_SK8SNOW

	return TRUE;
} //MemoryMgr::CheckFree
