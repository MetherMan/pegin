/**
 *	$Workfile: npc.h $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 6 $
 *	$Date: 04-09-08 3:54p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/npc.h $
 * 
 * 6     04-09-08 3:54p Sk8snow
 * 
 * 5     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __NPC_H__

#define dNPC_AREA		20

////////////////////////////////////
// G L O B A L   F U N C
////////////////////////////////////
BOOL LoadNpcInfo();
void SendNpcInfo( sPDESC_DATA pPlayer, int mapNum );

void DestroyNpcInfo();
void SetNpcPos( int mapNum, int posX, int posY, int npcNum );

BOOL IsGarbageArea( sPDESC_DATA pPlayer );
BOOL IsShopKeeperArea( sPDESC_DATA pPlayer, int shopNum );
BOOL IsGuildKeeperArea( sPDESC_DATA pPlayer );
BOOL LoadNpcPos( int mapNum );

BOOL FindNpcInMap( sPDESC_DATA pPlayer, int npcNum );

#endif


//


