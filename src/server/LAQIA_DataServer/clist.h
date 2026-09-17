/*
	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	clist.h

	Build log...
*/


///////////////////////////////////////////////////////////////
// 범용 링크드 리스트 클래스
///////////////////////////////////////////////////////////////
#ifndef __CLIST_H__
#define __CLIST_H__

///////////////////////////////////////////////////
// 2Way 링크드 리스트 매크로
///////////////////////////////////////////////////
#define INSERT_TO_LIST( list, item , prev, next ) \
if( list == NULL )\
{\
	list = item;\
	item->next = NULL;\
	item->prev = NULL;\
}\
else\
{\
	list->prev = item;\
	item->next = list;\
	list = item;\
	item->prev = NULL;\
}

#define REMOVE_FROM_LIST( list, item , prev, next )\
if( list == item )\
{\
	if( item->next ) \
		item->next->prev = NULL;\
	list = item->next;\
	item->prev = NULL;\
	item->next = NULL;\
}\
else\
{\
	if( item->prev ) \
		item->prev->next = item->next;\
	if( item->next ) \
		item->next->prev = item->prev;\
	item->prev = NULL;\
	item->next = NULL;\
}


struct sLIST_BLOCK
{
	void *data;

	sLIST_BLOCK *prev;
	sLIST_BLOCK *next;
};

typedef struct sLIST_BLOCK *sPLIST_BLOCK;

class CList
{
private:
	int m_totalCnt;				// 총 데이터 갯수
	int m_totalMemory;			// 총 메모리 갯수

public:
	CList();				// 생성자
	~CList();				// 파괴자

	void push( void *data );	// 데이터 넣기 
	void *pop();				// 데이터 빼기
	sLIST_BLOCK *FindData( void *data );			// 데이터 찾기
	sLIST_BLOCK *CreateBlock();		// sLIST_BLOCK 데이터 만들기
	void *GetLastData();
	void *GetFirstData() { return m_link->data; }
	sLIST_BLOCK *GetLastBlock();
	sLIST_BLOCK *GetFirstBlock() { return m_link; }
	void remove( void *data );	// 데이터 지우기
	void remove( sPLIST_BLOCK block ); 
	void Destroy();	
	void Init();			
	void clear();				// 리스트 비우기
	int GetSize() { return m_totalCnt; }		// 총 데이터 갯수
	int GetMemSize() { return m_totalMemory; }  // 총 메모리 상의 데아터 갯수

	sLIST_BLOCK	*m_link;		// 실질 리스트 데이터
	sLIST_BLOCK *m_memory;	// sLIST_BLOCK 데이터 메모리 링커
};

#endif


