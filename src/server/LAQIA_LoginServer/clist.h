/*
	범용 링크드 리스트 해더

	Copyright (c) 2003 팀메이, All rights reserved.

	Author	sk8snow

	File	clist.h

	Build log...
*/

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

#define LIST_WHILE( list, item, temp_d , next )				\
	for( item = list; item; item = temp_d )					\
{															\
	temp_d = item->next;									



#define LIST_WHILEEND()		}   
#define LIST_SKIP()			{continue;}	


struct sLIST_BLOCK
{
	void *data;

	sLIST_BLOCK *prev;
	sLIST_BLOCK *next;
};

typedef struct sLIST_BLOCK *sPLIST_BLOCK;



#endif


