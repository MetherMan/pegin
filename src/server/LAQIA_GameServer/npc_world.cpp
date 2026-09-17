#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "world.h"
#include "player.h"
#include "item.h"
#include "mob.h"
#include "mob_world.h"
#include "player_world.h"

#include "npc.h"
#include "npc_world.h"

#include "combat.h"
#include "quest.h"

#include "message.h"


/*
 *	MakeNpcButton
 */
sPNPC_BUTTON MakeNpcButton( sPNPC_DATA pNpc )
{
	static sNPC_BUTTON btn;

	btn.btn[0] = dNPC_BUTTON_NONE;
	btn.btn[1] = dNPC_BUTTON_NONE;
	btn.btn[2] = dNPC_BUTTON_NONE;
	btn.btn[3] = dNPC_BUTTON_NONE;

	int array = 0;

#define CHECK_ARRAY()	{ if( array > 3 ) return &btn; }

	if( pNpc->shopNum > 0 )
	{
		btn.btn[array] = dNPC_BUTTON_SHOP;
		array++;
	}

	if( pNpc->isHaveQuest )
	{
		btn.btn[array] = dNPC_BUTTON_QUEST;
		array++;
	}	

	if( pNpc->isWarehouse )
	{
		btn.btn[array] = dNPC_BUTTON_WAREHOUSE;
		array++;
	}

	if( pNpc->skillMaster > 0 )
	{
		btn.btn[array] = dNPC_BUTTON_SKILL;
		array++;
	}

	//
	CHECK_ARRAY();

	if( pNpc->jobMaster > 0 )
	{
		btn.btn[array] = dNPC_BUTTON_CHANGEJOB;
		array++;
	}

	CHECK_ARRAY();

	if( pNpc->storyNum )
	{
		btn.btn[array] = dNPC_BUTTON_10JUN;
		array++;
	}

	CHECK_ARRAY();

	if( pNpc->vehicle )
	{
		btn.btn[array] = dNPC_BUTTON_VEHICLE;
		array++;
	}

	CHECK_ARRAY();

	if( pNpc->resetJobMakster )
	{
		btn.btn[array] = dNPC_BUTTON_RESETJOB;
		array++;
	}

	CHECK_ARRAY();

	if( pNpc->npcNum == 228 ||
		pNpc->npcNum == 229 ||
		pNpc->npcNum == 230 )
	{
		btn.btn[array] = dNPC_BUTTON_CHANGECOUNTRY;
		array++;
	}

#undef CHECK_ARRAY
	return &btn;
}

/*
 *	MakeNpcInPacket
 */
void MakeNpcInPacket( sPNPC_DATA pNpc )
{
	sPNPC_BUTTON pNpcBtn = MakeNpcButton( pNpc );

	PutHeader( dPACKET_INSERT_NPC );
	PutInteger( pNpc->idxNum );
	PutInteger( pNpc->npcNum );
	PutString( pNpc->name );
	PutInteger( pNpc->npcType );
	PutInteger( pNpc->posX );
	PutInteger( pNpc->posY );
	PutFloat( pNpc->posZ );
	PutInteger( pNpc->angle );
	PutInteger( pNpc->defTalkNum );					// 기본 대화 번호  

	PutByte( pNpcBtn->btn[0] );
	PutByte( pNpcBtn->btn[1] );
	PutByte( pNpcBtn->btn[2] );
	PutByte( pNpcBtn->btn[3] );

	PutInteger( pNpc->storyNum );				// 열전번호
}

/*
 *	MakeNpcOutPacket
 */
void MakeNpcOutPacket( sPNPC_DATA pNpc )
{
	PutHeader( dPACKET_REMOVE_NPC );
	PutInteger( pNpc->idxNum );
}


/*
 *	GetNpcSector
 */
void GetNpcSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPNPC_DATA npc, next_npc;

	LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->npc[y][x], npc, next_npc, w_next );
	MakeNpcInPacket( npc );
	SendData( pPlayer, g_Packet, g_nPos );

	if( npc->isWarehouse || npc->shopNum )
	{
		GET_PRE_MAPNUM( pPlayer ) = npc->mapNum;
		GET_PRE_POSX( pPlayer ) = npc->posX + 1;
		GET_PRE_POSY( pPlayer ) = npc->posY + 1;
		GET_PRE_POSZ( pPlayer ) = (int)npc->posZ;
	}

	LIST_WHILEEND();
}

/*
 *	GetNpcVSector
 */
void GetNpcVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				GetNpcSector( pPlayer,  x, y );
		}
	}
}

/*
 *	GetNpcHSector
 */
void GetNpcHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				GetNpcSector( pPlayer, x, y );
		}
	}
}


/*
 *	OutNpcSector
 */
void OutNpcSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPNPC_DATA npc, next_npc;

	LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->npc[y][x], npc, next_npc, w_next );
	MakeNpcOutPacket( npc );
	SendData( pPlayer, g_Packet, g_nPos );
	LIST_WHILEEND();
}

/*
 *	OutNpcVSector
 */
void OutNpcVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				OutNpcSector( pPlayer,  x, y );
		}
	}	
}


/*
 *	OutNpcHSector
 */
void OutNpcHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				OutNpcSector( pPlayer, x, y );
		}
	}
}

/*
 *	SendToNpcVSector
 */
void SendToNpcVSector( int x, int startY, int endY, sPNPC_DATA pNpc, char *data, int size )
{
	int mapNum = pNpc->mapNum;

	if( x >= 0 && x < dONE_TILE_SIZE )
	{
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[mapNum]->player[y][x], data, size , dSENDMODE_TOALL );
		}
	}	
}

/*
 *	SendToNpcHSector
 */
void SendToNpcHSector( int y, int startX, int endX, sPNPC_DATA pNpc, char *data, int size )
{
	int mapNum = pNpc->mapNum;

	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
		}
	}	
}

/*
 *	SendToNpcArea
 */
void SendToNpcArea( sPNPC_DATA pNpc, char *data, int size )
{
	int posX = (int)(pNpc->posX/dONE_TILE);
	int posY = (int)(pNpc->posY/dONE_TILE);
		
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				if( gWORLD[pNpc->mapNum]->player[y][x] )
				{
					SendToSector( NULL, gWORLD[pNpc->mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
				}
			}
		}
	}
}

/*
 *	GetCurrAreaNpc
 */
void GetCurrAreaNpc( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				GetNpcSector( pPlayer, x, y );	
}

/*
 *	OutCurrAreaNpc
 */
void OutCurrAreaNpc( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				OutNpcSector( pPlayer, x, y );
}

/*
 *	FindMobInTile
 */
sPNPC_DATA FindNpcInTile( sPNPC_DATA list, int idxNum )
{
	sPNPC_DATA npc, next_npc;

	LIST_WHILE( list, npc, next_npc, w_next );
	if( npc->idxNum == idxNum )
		return npc;
	LIST_WHILEEND();

	return NULL;
}

/*
 *	FindNpcInArea
 */
sPNPC_DATA FindNpcInArea( int mapNum, int posX, int posY, int idxNum )
{
	sPNPC_DATA pNpc;

	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				pNpc = FindNpcInTile( gWORLD[mapNum]->npc[y][x], idxNum );

				if( pNpc )
					return pNpc;
			}
		}
	}
	return NULL;
}


/*
 *	NPC_QuestTalk
 */
void NPC_QuestTalk( sPNPC_DATA pNpc, sPDESC_DATA pPlayer )
{
	if( GetDistance( pNpc->posX, pNpc->posY, GET_POSX( pPlayer ), GET_POSY( pPlayer ) ) > 60 )
		return;

	if( *pNpc->npcTalk[0] == NULL_STR || *pNpc->npcTalk[1] == NULL_STR )
		return;

	sPNPC_QUEST quest, next_quest;

	LIST_WHILE( pNpc->questList, quest, next_quest, next );
	sPQUEST_INFO pQuest = g_QUEST[quest->questNum];

	if( !pQuest )
		LIST_SKIP();

	// 완료나 실패인 퀘스트 이면
	if( GET_QUEST_STATE( pPlayer, pQuest->questNum ) == dQUEST_STATE_COMPLITED ||
		GET_QUEST_STATE( pPlayer, pQuest->questNum ) == dQUEST_STATE_FAILED )
		LIST_SKIP();

	if( quest->isGrant == dQUEST_GRANTOPT_FINISH )
	{
		if( GET_QUEST_STATE( pPlayer, pQuest->questNum ) == dQUEST_STATE_BEING )
		{
			// 완료용
			PutHeader( dPACKET_SHOW_NPCTALKBALLOON );
			PutInteger( pNpc->idxNum );
			PutByte( dNPC_TALKTYPE_FINISHQUEST );
			PutString( pNpc->npcTalk[1] );
			SendData( pPlayer, g_Packet, g_nPos );
			return;
		}
	}
	else if( quest->isGrant == dQUEST_GRANTOPT_GRANT )
	{
		if( GET_QUEST_STATE( pPlayer, pQuest->questNum ) == dQUEST_STATE_NONE )
		{
			if( GET_LEVEL( pPlayer ) >= pQuest->startNeedLevel )
			{
				if( !QUEST_IsClassGroupOK( pPlayer, pQuest ) )
					LIST_SKIP();

				// 수행가능 직업
				if( pQuest->startNeedClass > -1 )
				{
					if( GET_CLASS( pPlayer ) != pQuest->startNeedClass )
						LIST_SKIP();
				}
				
				if( pQuest->startNeedQuest )
				{
					if( GET_QUEST_STATE( pPlayer, pQuest->startNeedQuest ) != pQuest->startNeedQuestState )
						LIST_SKIP();
				}

				PutHeader( dPACKET_SHOW_NPCTALKBALLOON );
				PutInteger( pNpc->idxNum );
				PutByte( dNPC_TALKTYPE_GRANTQUEST );
				PutString( pNpc->npcTalk[0] );
				SendData( pPlayer, g_Packet, g_nPos );
				return;
			}
		}
	}


	LIST_WHILEEND();

	NPC_NormalTalk( pNpc, pPlayer );
}

/*
 *	NPC_NormalTalk
 */
void NPC_NormalTalk( sPNPC_DATA pNpc, sPDESC_DATA pPlayer )
{
	if( GetDistance( pNpc->posX, pNpc->posY, GET_POSX( pPlayer ), GET_POSY( pPlayer ) ) > 60 )
		return;

	int talkNum = number( 2, 4 );

	if( *pNpc->npcTalk[talkNum] == NULL_STR )
		return;

	PutHeader( dPACKET_SHOW_NPCTALKBALLOON );
	PutInteger( pNpc->idxNum );
	PutByte( dNPC_TALKTYPE_NORMAL );
	PutString( pNpc->npcTalk[talkNum] );
	SendData( pPlayer, g_Packet, g_nPos );
}

/*
 *	NPC_Talk
 */
void NPC_Talk( sPNPC_DATA pNpc )
{
	pNpc->npcTalkTimer--;
	
	if( pNpc->npcTalkTimer <= 0 )
		pNpc->npcTalkTimer = number( 4, 6 );
	else
		return;

	sPDESC_DATA d, next_d;

	// 퀘스트 NPC 
	if( pNpc->questList )
	{
		int posX = (int)(pNpc->posX/dONE_TILE);
		int posY = (int)(pNpc->posY/dONE_TILE);
		
		for( int y = posY - 1; y <= posY + 1; y++ )
		{
			for( int x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[pNpc->mapNum]->player[y][x], d, next_d, w_next );
					NPC_QuestTalk( pNpc, d );
					LIST_WHILEEND()
				}
			}
		}
	}
	else // 일반 NPC 
	{
		int posX = (int)(pNpc->posX/dONE_TILE);
		int posY = (int)(pNpc->posY/dONE_TILE);
		
		for( int y = posY - 1; y <= posY + 1; y++ )
		{
			for( int x = posX - 1; x <= posX + 1; x++ )
			{
				if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				{
					LIST_WHILE( gWORLD[pNpc->mapNum]->player[y][x], d, next_d, w_next );
					NPC_NormalTalk( pNpc, d );
					LIST_WHILEEND()
				}
			}
		}
	}
}

/*
 *	NPC_TalkProc
 */
void NPC_TalkProc()
{
	sPNPC_DATA npc, next_npc;

	LIST_WHILE( g_NPCS.list, npc, next_npc, next );
	NPC_Talk( npc );
	LIST_WHILEEND();
}


/*
 *	MoveNPC
 */
void MoveNPC( sPNPC_DATA pNpc, int posX, int posY )
{
	int newX, newY, oldX, oldY, mapNum = pNpc->mapNum;
	
	newX = (int)(posX / dONE_TILE);
	newY = (int)(posY / dONE_TILE);
	oldX = (int)(pNpc->posX / dONE_TILE);
	oldY = (int)(pNpc->posY / dONE_TILE);

	pNpc->posX = posX;
	pNpc->posY = posY;
		
	if( newX != oldX || newY != oldY )
	{
		REMOVE_FROM_LIST( gWORLD[mapNum]->npc[oldY][oldX], pNpc, w_prev, w_next );
		INSERT_TO_LIST( gWORLD[mapNum]->npc[newY][newX], pNpc, w_prev, w_next );
		
		if( newY < oldY && newX == oldX ) // Y 감소
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcHSector( oldY+1, oldX-1, oldX+1, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcHSector( newY-1, newX-1, newX+1, pNpc, g_Packet, g_nPos );			
		}
		else if( newX > oldX && newY < oldY ) // X증가 Y 감소
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcHSector( oldY+1, oldX, oldX+1, pNpc, g_Packet, g_nPos );
			SendToNpcVSector( oldX-1, oldY-1, oldY+1, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcHSector( newY-1, newX-1, newX, pNpc, g_Packet, g_nPos );
			SendToNpcVSector( newX+1, newY-1, newY+1, pNpc, g_Packet, g_nPos );
		}
		else if( newX > oldX && newY == oldY ) // X 증가
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcVSector( oldX-1, oldY-1, oldY+1, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcVSector( newX+1, newY-1, newY+1, pNpc, g_Packet, g_nPos );			
		}
		else if( newX > oldX && newY > oldY ) // X 증가 Y 증가
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcVSector( oldX-1, oldY-1, oldY+1, pNpc, g_Packet, g_nPos );
			SendToNpcHSector( oldY-1, oldX, oldX+1, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcVSector( newX+1, newY-1, newY+1, pNpc, g_Packet, g_nPos );
			SendToNpcHSector( newY+1, newX-1, newX, pNpc, g_Packet, g_nPos );			
		}
		else if( newY > oldY && newX == oldX ) // Y 증가
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcHSector( oldY-1, oldX-1, oldX+1, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcHSector( newY+1, newX-1, newX+1, pNpc, g_Packet, g_nPos );			
		}
		else if( newX < oldX && newY > oldY ) // X 감소 Y 증가
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcHSector( oldY-1, oldX-1, oldX+1, pNpc, g_Packet, g_nPos );
			SendToNpcVSector( oldX+1, oldY, oldY+1, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcHSector( newY+1, newX-1, newX+1, pNpc, g_Packet, g_nPos );
			SendToNpcVSector( newX-1, newY-1, newY, pNpc, g_Packet, g_nPos );			
		}
		else if( newX < oldX && newY == oldY ) // X 감소
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcVSector( oldX+1, oldY-1, oldY+1, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcVSector( newX-1, newY-1, newY+1, pNpc, g_Packet, g_nPos );
		}
		else if( newX < oldX && newY < oldY ) // X감소 Y 감소
		{
			MakeNpcOutPacket( pNpc );
			SendToNpcVSector( oldX+1, oldY-1, oldY+1, pNpc, g_Packet, g_nPos );
			SendToNpcHSector( oldY+1, oldX-1, oldX, pNpc, g_Packet, g_nPos );
			
			MakeNpcInPacket( pNpc );
			SendToNpcVSector( newX-1, newY-1, newY+1, pNpc, g_Packet, g_nPos );
			SendToNpcHSector( newY-1, newX, newX+1, pNpc, g_Packet, g_nPos );			
		}
	}
	
	PutHeader( dPACKET_MOVE_NPC );
	PutInteger( pNpc->idxNum );
	PutInteger( pNpc->posX );
	PutInteger( pNpc->posY );
	SendToNpcArea( pNpc, g_Packet, g_nPos );
}


/*
 *	LoadNpcLoadPoint
 */
void LoadNpcMovePoint( int npcNum )
{
	sPNPC_DATA pNpc = FindNpcByNum( npcNum );

	if( !pNpc )
		return;

	CLoadDataFile cLoad;

	char fileName[128];

	sprintf( fileName, "./DATA/MOVE_NPC/%d.TXT", npcNum );

	if( !cLoad.Load( fileName ) )
		return;

	int posX, posY;

	while( 1 )
	{
		BYTE res = cLoad.GetLine();

		if( res == CLoadDataFile::IS_EOF )
			break;

		if( res == CLoadDataFile::IS_SKIP )
			continue;
		
		posX = cLoad.Get();
		posY = cLoad.Get();

		if( posX < 0 || posX >= dONE_MAP_SIZE || posY < 0 || posY >= dONE_MAP_SIZE )
			continue;

		if( pNpc )
			AddNpcMovePoint( pNpc, posX, posY );
	}

	pNpc->moveDest = pNpc->moveList;
	pNpc->isNextContinue = 1;

	if( !pNpc->btIsInMoveList )
	{
		INSERT_TO_LIST( g_NPCS.move_list, pNpc, move_prev, move_next );
		pNpc->btIsInMoveList = 0;
	}

	return;
}

/*
 *	AddNpcMovePoint
 */
void AddNpcMovePoint( sPNPC_DATA pNpc, int posX, int posY )
{
	sPNPC_MOVE_POINT pPoint = (sPNPC_MOVE_POINT)malloc(sizeof(sNPC_MOVE_POINT));

	if( !pPoint )
		return;

	pPoint->posX = posX;
	pPoint->posY = posY;

	pPoint->prev = NULL;
	pPoint->next = NULL;

	INSERT_TO_LIST( pNpc->moveList, pPoint, prev, next );
}

/*
 *	DestroyNpcMovePoint
 */
void DestroyNpcMovePoint( sPNPC_DATA pNpc )
{
	sPNPC_MOVE_POINT point, next_point;

	LIST_WHILE( pNpc->moveList, point, next_point, next );
	REMOVE_FROM_LIST( pNpc->moveList, point, prev, next );
	SAFE_FREE( point );
	LIST_WHILEEND();
}

/*
 *	NpcMoveProc
 */
void NpcMoveProc()
{
	sPNPC_DATA npc, next_npc;

	LIST_WHILE( g_NPCS.move_list, npc, next_npc, move_next );

	if( npc->moveDest )
	{
		int posX = npc->posX;
		int posY = npc->posY;
		
		if( posX < npc->moveDest->posX )
			posX += 1;
		else if( posX > npc->moveDest->posX )
			posX -= 1;
		
		if( posY < npc->moveDest->posY )
			posY += 1;
		else if( posY > npc->moveDest->posY )
			posY -= 1;

		if( posX >= 0 && posX < dONE_MAP_SIZE && posY >= 0 && posY < dONE_MAP_SIZE )
			MoveNPC( npc, posX, posY );

		// 목표 갱신
		if( npc->moveDest->posX == posX &&
			npc->moveDest->posY == posY )
		{
			if( npc->isNextContinue )
			{
				if( npc->moveDest->next )
					npc->moveDest = npc->moveDest->next;
				else
				{
					npc->isNextContinue = 0;
					npc->moveDest = npc->moveDest->prev;
				}
			}
			else
			{
				if( npc->moveDest->prev )
					npc->moveDest = npc->moveDest->prev;
				else
				{
					npc->isNextContinue = 1;
					npc->moveDest = npc->moveDest->next;
				}
			}
		}
	}

	LIST_WHILEEND();
}

//EOF



