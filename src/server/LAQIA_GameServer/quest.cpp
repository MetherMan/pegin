/**
 *	$Workfile: quest.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 9 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/quest.cpp $
 * 
 * 9     04-12-15 10:40a Sk8snow
 * 
 * 8     04-03-31 8:22a Sk8snow
 *        
 */

#include "def.h"
#include "structs.h"
#include "extern.h"
#include "message.h"

#include "world.h"
#include "player.h"
#include "monster.h"
#include "item.h"

#include "fight.h"
#include "party.h"

#include "skill.h"
#include "mercenary.h"

#include "quest.h"
#include "questdb.h"

#include "db_rows.h"

sPQUEST_DEF_INFO	g_QUESTINFO[dMAX_QUESTINFO];

/*
 *	
 */
BOOL LoadQuestData()
{
	FILE	*fp;

	fp = fopen( dQUEST_TABLE_FILE, "rb" );
	
	if( !fp )
	{
		log( "Can't Find Quest_data.txt\r\n" );
		return 0;
	}
	
	int		Number;						
	int		questNum;					//퀘스트 번호
	int		questStartMapNum;			//퀘스트 시작맵번호
	int		questStartNpcNum;			//퀘스트 시작 NPC번호
	int		questAllLvLimit;			//모든 레벨 제한
	int		questShortAttackLimit;		//근거리 제한
	int		questLognAttackLimit;		//원거리 제한
	int		questBMagicLimit;			//흑마법 제한
	int		questWMagicLimit;			//백마법 제한
	int		questTimeLimit;				//시간제한
	int		questNeedPeople;			//필요인원
	int		questNeedObject;			//시작 아이템 번호
	int		questLoop;					//퀘스트 반복회수 0 = 반복없음, 1 = 1번, 2 = 2번 .....
	int		questPrequest;				//선결퀘스트
	int		questOverlap;				//중복가능 퀘스트
	int		questItemNum;				//수집할 아이템 번호
	int		questMobNum;				//잡을 몬스터번호
	int		questNeedItemMobCnt;		//필요한 아이템 / 몬스터 개수
	int		questEndNpcMap;				//종료NPC맵
	int		questEndNpcNum;				//종료NPC번호
	int		questMoneyCompen;			//실드 보상
	int		questExpCompen;				//exp보상
	int		questItemCompen;			//아이템 보상

	char	readBuffer[1024];
	int		nPos;

	int		totalQuestCnt = 0;

	while( !feof( fp ) )
	{
		nPos = 0;

		fgets( readBuffer, 1024, fp );
		
		if( *readBuffer == ';' )
			continue;

		if( *readBuffer == '\n' ||
			*readBuffer == '\r' ||
			*readBuffer == '\0' )
			break;
		
		Number					= GetTabNumberic( readBuffer, nPos );
		questNum				= GetTabNumberic( readBuffer, nPos );				
		questStartMapNum		= GetTabNumberic( readBuffer, nPos );		
		questStartNpcNum		= GetTabNumberic( readBuffer, nPos );		
		questAllLvLimit			= GetTabNumberic( readBuffer, nPos );
		questShortAttackLimit	= GetTabNumberic( readBuffer, nPos );	
		questLognAttackLimit	= GetTabNumberic( readBuffer, nPos );			
		questBMagicLimit		= GetTabNumberic( readBuffer, nPos );		
		questWMagicLimit		= GetTabNumberic( readBuffer, nPos );		
		questTimeLimit			= GetTabNumberic( readBuffer, nPos );			
		questNeedPeople			= GetTabNumberic( readBuffer, nPos );		
		questNeedObject			= GetTabNumberic( readBuffer, nPos );		
		questLoop				= GetTabNumberic( readBuffer, nPos );				
		questPrequest			= GetTabNumberic( readBuffer, nPos );			
		questOverlap			= GetTabNumberic( readBuffer, nPos );			
		questItemNum			= GetTabNumberic( readBuffer, nPos );
		questMobNum				= GetTabNumberic( readBuffer, nPos );
		questNeedItemMobCnt		= GetTabNumberic( readBuffer, nPos );	
		questEndNpcMap			= GetTabNumberic( readBuffer, nPos );			
		questEndNpcNum			= GetTabNumberic( readBuffer, nPos );	
		questMoneyCompen		= GetTabNumberic( readBuffer, nPos );
		questExpCompen			= GetTabNumberic( readBuffer, nPos );
		questItemCompen			= GetTabNumberic( readBuffer, nPos );

		if( Number > dMAX_QUEST )
		{
			log( "LoadQuestInfo : invalid QuestNum\r\n" );
			fclose( fp );
			return 0;
		}

		if( !g_QUESTINFO[Number] )
		{
			g_QUESTINFO[Number] = (sPQUEST_DEF_INFO)malloc( sizeof(sQUEST_DEF_INFO) );

			if( !g_QUESTINFO[Number] )
			{
				log( "LoadQuestInfo : malloc failed\r\n" );
				fclose( fp );
				return 0;
			}
		}

		g_QUESTINFO[Number]->questNum					=	questNum;
		g_QUESTINFO[Number]->questStartMapNum			=	questStartMapNum;
		g_QUESTINFO[Number]->questStartNpcNum			=	questStartNpcNum;
		g_QUESTINFO[Number]->questAllLvLimit			=	questAllLvLimit;
		g_QUESTINFO[Number]->questShortAttackLimit	=	questShortAttackLimit;
		g_QUESTINFO[Number]->questLognAttackLimit		=	questLognAttackLimit;
		g_QUESTINFO[Number]->questBMagicLimit			=	questBMagicLimit;
		g_QUESTINFO[Number]->questWMagicLimit			=	questWMagicLimit;
		g_QUESTINFO[Number]->questTimeLimit			=	questTimeLimit;
		g_QUESTINFO[Number]->questNeedPeople			=	questNeedPeople;
		g_QUESTINFO[Number]->questNeedObject			=	questNeedObject;
		g_QUESTINFO[Number]->questLoop				=	questLoop;
		g_QUESTINFO[Number]->questPrequest			=	questPrequest;
		g_QUESTINFO[Number]->questOverlap				=	questOverlap;
		g_QUESTINFO[Number]->questItemNum				=	questItemNum;
		g_QUESTINFO[Number]->questMobNum				=	questMobNum;
		g_QUESTINFO[Number]->questNeedItemMobCnt		=	questNeedItemMobCnt;
		g_QUESTINFO[Number]->questEndNpcMap			=	questEndNpcMap;
		g_QUESTINFO[Number]->questEndNpcNum			=	questEndNpcNum;
		g_QUESTINFO[Number]->questMoneyCompen			=	questMoneyCompen;
		g_QUESTINFO[Number]->questExpCompen			=	questExpCompen;
		g_QUESTINFO[Number]->questItemCompen			=	questItemCompen;

		totalQuestCnt ++;
		log_file( "QuestTest.txt", "QuestNum[%04d] QuestStartMapNum[%d] QuestAllLvLimit[%d]\r\n", 
			g_QUESTINFO[Number]->questNum, g_QUESTINFO[Number]->questStartMapNum, g_QUESTINFO[Number]->questAllLvLimit );
	}
	
	fclose( fp );

	log( "LoadQuestInfo : total Quest Count : %d / %d\r\n", totalQuestCnt, dMAX_QUEST );

	return 1;
}

void	MakeQuestInPacket( sPQUEST_DEF_INFO pQDI )
{
		g_nPos = 2;
		
		PutInteger( g_Packet, pQDI->questNum, g_nPos );				//퀘스트번호
		PutInteger( g_Packet, pQDI->questStartMapNum, g_nPos );		//퀘스트시작맵번호
		PutInteger( g_Packet, pQDI->questStartNpcNum, g_nPos );		//퀘스트시작NPC번호
		
		PutSize( g_Packet, g_nPos );
}


void	SendQuestInfo( sPDESC_DATA pPlayer, int mapNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "SendQuestInfo : invalid map num [%s/%s]\r\n", GET_NAME( pPlayer ), GET_ID( pPlayer ) );
		return;
	}
	//BYTE	questCnt = 0;
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_QUEST_LIST, g_nPos );
//	LIST_WHILE( gWORLD[mapNum]->questList, quest, next_quest, next, WMgr );
/*	PutInteger( g_Packet, quest->questNum, g_nPos );					//퀘스트번호 
	PutInteger( g_Packet, quest->questStartMapNum, g_nPos );			//시작맵번호
	PutInteger( g_Packet, quest->questStartNpcNum, g_nPos );			//시작NPC번호
	PutInteger( g_Packet, quest->questAllLvLimit, g_nPos );				//통합 레벨
	PutInteger( g_Packet, quest->questShortAttackLimit, g_nPos );		//근거리 제약
	PutInteger( g_Packet, quest->questLognAttackLimit, g_nPos );		//원거리 제약
	PutInteger( g_Packet, quest->questBMagicLimit, g_nPos );			//흑마법 제약
	PutInteger( g_Packet, quest->questWMagicLimit, g_nPos );			//백마법 제약
	PutInteger( g_Packet, quest->questTimeLimit, g_nPos );				//제한 시간
	PutInteger( g_Packet, quest->questNeedPeople, g_nPos );				//필요인원
	PutInteger( g_Packet, quest->questNeedObject, g_nPos );				//시작 아이템
	PutInteger( g_Packet, quest->questLoop, g_nPos );					//반복가능
	PutInteger( g_Packet, quest->questPrequest, g_nPos );				//선결 퀘스트
	PutInteger( g_Packet, quest->questOverlap, g_nPos );				//퀘스트중복
	PutInteger( g_Packet, quest->questItemNum, g_nPos );				//아이템번호
	PutInteger( g_Packet, quest->questMobNum, g_nPos );					//몬스터 번호
	PutInteger( g_Packet, quest->questNeedItemMobCnt, g_nPos );			//필요한 아이템 / 몬스터 개수
	PutInteger( g_Packet, quest->questEndNpcMap, g_nPos );				//종료 NPC맵
	PutInteger( g_Packet, quest->questEndNpcNum, g_nPos );				//종료 NPC번호
	PutInteger( g_Packet, quest->questMoneyCompen, g_nPos );			//돈 보상
	PutInteger( g_Packet, quest->questExpCompen, g_nPos );				//경험치 보상
	PutInteger( g_Packet, quest->questItemCompen, g_nPos );				//아이템 보상*/

	PutInteger( g_Packet, g_QUESTINFO[1]->questNum, g_nPos );
	PutInteger( g_Packet, g_QUESTINFO[1]->questStartMapNum, g_nPos );
	PutInteger( g_Packet, g_QUESTINFO[1]->questStartNpcNum, g_nPos );
	PutInteger( g_Packet, g_QUESTINFO[1]->questAllLvLimit, g_nPos );
//	questCnt++;
//	LIST_WHILEEND( g_WORLD[mapNum]->questList, quest, next_quest ); 
//	*(BYTE*)( g_Packet + 4 ) = questCnt;
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );

	
	log_file( "QuestPacket.txt", "QuestNum[%d] QuestStartMapNum[%d] QuestStartNpcNum[%d] QuestAllLvLimit[%d]\r\n",
		g_QUESTINFO[1]->questNum, g_QUESTINFO[1]->questStartMapNum, g_QUESTINFO[1]->questStartNpcNum, g_QUESTINFO[1]->questAllLvLimit );
}

void	DestroyQuestInfo()
{
	sPQUEST_DEF_INFO	quest,next_quest;
	
	for( int i=0; i<dMAX_MAP_CNT; i++ )
	{
		if( gWORLD[i] )
		{
			LIST_WHILE( gWORLD[i]->questList, quest, next_quest, next, WMgr );
			REMOVE_FROM_LIST( gWORLD[i]->questList, quest, prev, next );
			SAFE_FREE( quest );
			LIST_WHILEEND( gWORLD[i]->questList, quest, next_quest );
		}
	}
}



// EOF


