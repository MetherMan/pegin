/*
	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	main.cpp

	Build log...
*/
#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "clist.h"

//
// Function Name: GetLastData
// Date: 2003-02-14
// Description: 리스트 데이터의 가장 마지막 데이터를 가져온다
//
sLIST_BLOCK *CList::GetLastBlock()
{
	sLIST_BLOCK *b, *last_b = NULL;

	for( b = m_link; b; b = b->next )
		last_b = b;
	return last_b;
}

void *CList::GetLastData()
{
	sLIST_BLOCK *b, *last_b = NULL;

	for( b = m_link; b; b = b->next )
		last_b = b;
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
	
	for( b = m_link; b; b = next_b )
	{
		next_b = b->next;
		
		b->data = NULL;
		free(b);
		b=NULL;
	}
	for( b = m_memory; b; b = next_b )
	{
		next_b = b->next;
		
		b->data = NULL;
		free(b);
		b=NULL;
	}
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

	for( b = m_link; b; b = next_b )
	{
		next_b = b->next;

		REMOVE_FROM_LIST( m_link, b, prev, next );
		INSERT_TO_LIST( m_memory, b, prev, next );
		
		b->data = NULL;
		b->next = NULL;
		b->prev = NULL;

		m_totalCnt--;
		m_totalMemory++;
	}
}

//
// Function Name: FindData
// Date: 2003-02-14
// Description: 
//
sPLIST_BLOCK CList::FindData( void *data )
{
	sPLIST_BLOCK b;

	for( b = m_link; b; b = b->next )
		if( b->data == data )
			return b;
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


CList::CList()				// 생성자
{
	m_totalCnt = 0;
	m_totalMemory = 0;

	m_link = NULL;
	m_memory = NULL;
}

CList::~CList()				// 소멸자
{
	Destroy();
}


