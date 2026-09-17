/**
 *	$Workfile: quest.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 5 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/quest.h $
 * 
 * 5     04-12-15 10:40a Sk8snow
 * 
 * 4     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __QUEST_H__
#define __QUEST_H__

#endif
extern	sPQUEST_DEF_INFO	g_QUESTINFO[dMAX_QUESTINFO];


BOOL	LoadQuestData(  );															//퀘스트데이터테이블 읽기
void	SendQuestData( sPQUEST_DEF_INFO pQDI );										//퀘스트데이타 보내기
void	SendQuestInfo( sPDESC_DATA pPlayer, int mapNum );							//
void	DestroyQuestInfo();


/////////////////////////////////////////////////////////////
//	D E F I N E	F U N T I O N	T A B L E
/////////////////////////////////////////////////////////////
#define GET_QUEST_NUM( questNum )		( g_QUESTINFO[questNum]->questNum )			//퀘스트번호	
#define GET_QUEST_NAME( questNum )		( g_QUESTINFO[questNum]->questName )		//퀘스트이름
#define	GET_QUEST_CODE( questNum )		( g_QUESTINFO[questNum]->questCode )		//퀘스트코드
#define GET_QUEST_OUTLINE( questNum )	( g_QUESTINFO[questNum]->questOutLine )		//퀘스트개요
//EOF




