/**
 *	$Workfile: clist.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 9 $
 *	$Date: 04-07-13 7:26a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/clist.cpp $
 * 
 * 9     04-07-13 7:26a Sk8snow
 * 
 * 8     04-03-31 8:22a Sk8snow
 *        
 */
#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "clist.h"
void log( char *buf, ... );

//
// Function Name: GetLastData
// Date: 2003-02-14
// Description: 리스트 데이터의 가장 마지막 데이터를 가져온다
//
sLIST_BLOCK *CList::GetLastBlock()
{
	sLIST_BLOCK *b, *last_b = NULL;
	sLIST_BLOCK *temp_b;

	LIST_WHILE( m_link, b, temp_b, next, WMgr );
	last_b = b;
	LIST_WHILEEND( m_link, b, temp_b );

	return last_b;
}

//
// Function Name: GetLastData
// Date:2003/03/03
// Description: 리스트의 맨 마지막 데이터 추출
//

void *CList::GetLastData()
{
	sLIST_BLOCK *b, *last_b = NULL;
	sLIST_BLOCK *temp_b;

	LIST_WHILE( m_link, b, temp_b, next, WMgr );
	last_b = b;
	LIST_WHILEEND( m_link, b,  temp_b );

	return last_b->data;
}


//
// Function Name: pop
// Date: 2003-02-17
// Description: 
//
void *CList::pop()
{
	void *popData;

	if( m_link )
	{
		popData = m_link;
		remove( popData );
	}
	else
		return NULL;
	return NULL;
}

//
// Function Name: Destroy
// Date: 2003-02-14
// Description: 리스트 데이터를 전부 삭제
//
void CList::Destroy()
{
	sPLIST_BLOCK b, next_b;
	
	LIST_WHILE( m_link, b, next_b, next, WMgr );	
	b->data = NULL;
	SAFE_FREE( b );
	LIST_WHILEEND( m_link, b, next_b );

	LIST_WHILE( m_memory, b, next_b , next, WMgr2);
	b->data = NULL;
	SAFE_FREE( b );
	LIST_WHILEEND( m_memory, b, next_b );

	m_link = NULL;
	m_memory = NULL;
	m_totalCnt = 0;
	m_totalMemory = 0;
}

//
// Function Name: DestroyWithData
// Date: 
// Description: 
//
void CList::DestroyWithData()
{
	sPLIST_BLOCK b, next_b;
	
	LIST_WHILE( m_link, b, next_b, next, WMgr );	
	SAFE_FREE( b->data );
	SAFE_FREE( b );
	LIST_WHILEEND( m_link, b, next_b );

	LIST_WHILE( m_memory, b, next_b , next, WMgr2 );
	b->data = NULL;
	SAFE_FREE( b );
	LIST_WHILEEND( m_memory, b, next_b );

	m_link = NULL;
	m_memory = NULL;
	m_totalCnt = 0;
	m_totalMemory = 0;
}

//
// Function Name: Init
// Date: 2003-02-14
// Description: 
//
void CList::Init()
{
	m_totalCnt = 0;
	m_totalMemory = 0;

	m_link = NULL;
	m_memory = NULL;
}

//
// Function Name: clear
// Date: 2003-02-14
// Description: 리스트 데이터 비우기
//
void CList::clear()
{
	sPLIST_BLOCK b, next_b;

	LIST_WHILE( m_link, b, next_b , next, WMgr );
	REMOVE_FROM_LIST( m_link, b, prev, next );
	INSERT_TO_LIST( m_memory, b, prev, next );
	
	b->data = NULL;
	b->next = NULL;
	b->prev = NULL;
	
	m_totalCnt--;
	m_totalMemory++;
	LIST_WHILEEND( m_link, b, next_b );
}

//
// Function Name: FindData
// Date: 2003-02-14
// Description: 
//
sPLIST_BLOCK CList::FindData( void *data )
{
	sPLIST_BLOCK b, temp_b;

	LIST_WHILE( m_link, b, temp_b, next, WMgr );
	if( b->data == data )
		return b;
	LIST_WHILEEND( m_link, b, temp_b );

	return NULL;
}

//
// Function Name: remove
// Date: 2003-02-14
// Description: sLIST_BLOCK 의 data 멤버를 찾아 삭제
//
void CList::remove( void *data )
{
	sLIST_BLOCK *find = FindData( data );

	if( !find )
		return;

	m_totalCnt--;
	m_totalMemory++;	

	REMOVE_FROM_LIST( m_link, find, prev, next );
	INSERT_TO_LIST( m_memory, find, prev, next );
}


//
// Function Name: remove
// Date: 2003-02-14
// Description: sLIST_BLOCK 데이터로 직접 접근하여 삭제
//
void CList::remove( sPLIST_BLOCK block )
{
	m_totalCnt--;
	m_totalMemory++;

	REMOVE_FROM_LIST( m_link, block, prev, next );
	INSERT_TO_LIST( m_memory, block, prev, next );
}

//
// Function Name: CreateBlock
// Date: 2003-02-14
// Description: 새로운 데이터 생성 
//
sLIST_BLOCK *CList::CreateBlock()
{
	sPLIST_BLOCK data;

	if( m_memory )
	{
		data = m_memory;
		REMOVE_FROM_LIST( m_memory, data, prev, next );
		m_totalMemory--;
		return data;
	}
	else
	{
		data = (sPLIST_BLOCK)malloc( sizeof(sLIST_BLOCK) );

		if( !data )
			return NULL;
		else
		{
			data->data = NULL;
			data->next = NULL;
			data->prev = NULL;
			return data;
		}
	}
	return NULL;    
}

//
// Function Name: push
// Date: 2003-02-14
// Description: 데이터 집어 넣기
//
void CList::push( void *data )
{
	sPLIST_BLOCK tmpData = CreateBlock();

	if( !tmpData )
		return;

	tmpData->data = data;
	m_totalCnt++;

	INSERT_TO_LIST( m_link, tmpData, prev, next );
}

//
// Function Name: 생성자
// Date: 2003-02-14
// Description: 
//
CList::CList()				
{
	m_totalCnt = 0;
	m_totalMemory = 0;

	m_link = NULL;
	m_memory = NULL;
}

//
// Function Name: 소멸자
// Date: 2003-02-14
// Description: 
//
CList::~CList()				
{
	Destroy();
}



////////////////////////////////////////////
// 루프 메니져
////////////////////////////////////////////
CWhileMgr::CWhileMgr()
{
	Init( 100000000 );
}

CWhileMgr::~CWhileMgr()
{

}

void CWhileMgr::Init( unsigned long dwLimit )
{
	m_checkCnt = 0;
	m_limit = dwLimit;
}

void CWhileMgr::CheckDump( char *file, int line )
{
	m_checkCnt++;
	
	if( m_checkCnt >= m_limit )
	{
		log( "# CheckDump / %s : %d\r\n", file, line );
		abort();
		return;
	}
}



//



