/**
 *	$Workfile: clist.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 9 $
 *	$Date: 04-09-20 10:53p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/clist.h $
 * 
 * 9     04-09-20 10:53p Sk8snow
 * 
 * 8     04-07-13 7:26a Sk8snow
 * 
 * 7     04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __CLIST_H__
#define __CLIST_H__

#define USE_BADWHILE_CHECK

#ifndef SAFE_FREE 
#define SAFE_FREE( mem )	{ free(mem); mem = NULL; }
#endif

#define INSERT_TO_LIST( head, item, prev, next )			\
{															\
item->next = head;											\
head = item;												\
}

#define REMOVE_FROM_LIST(head, item, prev, next)			\
if ((item) == (head))										\
	head = (item)->next;									\
else														\
{															\
	item->prev = head;										\
	while (item->prev && (item->prev->next != (item)))		\
	item->prev = item->prev->next;							\
	if (item->prev)											\
         item->prev->next = (item)->next;					\
}													


#define LIST_WHILE( list, item, temp_d , next, mgr )	    \
CWhileMgr mgr;												\
for( item = list; item; item = temp_d )						\
{															\
	temp_d = item->next;									\
	mgr.CheckDump( __FILE__, __LINE__ );					


#define LIST_WHILEEND( list , item , temp_d )   			\
}   


#define LIST_SKIP( item, temp_d )							\
	{														\
		continue;											\
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
	int m_totalCnt;									// 총 데이터 갯수
	int m_totalMemory;								// 총 메모리 갯수

public:
	CList();										// 생성자
	~CList();										// 파괴자

	void push( void *data );						// 데이터 넣기 
	void *pop();									// 데이터 빼기
	sLIST_BLOCK *FindData( void *data );			// 데이터 찾기
	sLIST_BLOCK *CreateBlock();						// sLIST_BLOCK 데이터 만들기
	void *GetLastData();
	void *GetFirstData() { return m_link->data; }
	sLIST_BLOCK *GetLastBlock();
	sLIST_BLOCK *GetFirstBlock() { return m_link; }
	void remove( void *data );						// 데이터 지우기
	void remove( sPLIST_BLOCK block ); 
	void Destroy();	
	void DestroyWithData();
	void Init();			
	void clear();									// 리스트 비우기
	int GetSize() { return m_totalCnt; }			// 총 데이터 갯수
	int GetMemSize() { return m_totalMemory; }		// 총 메모리 상의 데아터 갯수

	sLIST_BLOCK	*m_link;							// 실질 리스트 데이터
	sLIST_BLOCK *m_memory;							// sLIST_BLOCK 데이터 메모리 링커
};

class CWhileMgr
{
private:
	unsigned long		m_checkCnt;
	unsigned long		m_limit;

public:
	CWhileMgr();									
	~CWhileMgr();

	void		Init( unsigned long dwLimit );
	void		CheckDump( char *file, int line );	
};


#endif


