/**
 *	$Workfile: party.h $
 *
 *	Copyright (c) 2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 6 $
 *	$Date: 04-07-13 7:26a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/party.h $
 * 
 * 6     04-07-13 7:26a Sk8snow
 * 
 * 5     04-04-30 9:46a Sk8snow
 * 
 * 4     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __PARTY_H__
#define __PARTY_H__


#define GET_PARTY_MASTER( p ) ( p->ch2.party.master)
#define GET_PARTY_MEMBER( p ) ( p->ch2.party.member)
#define GET_PARTY_MEMBER_CNT( p ) (p->ch2.party.memberCnt)

void PARTY_SendToAll( sPDESC_DATA pPlayer, char *msg, int size, BYTE mode );
void PARTY_SendToMember( sPDESC_DATA pPlayer, char *msg, int size );
BOOL PARTY_Create( sPDESC_DATA pPlayer );
void PARTY_Destroy( sPDESC_DATA pPlayer );

void PARTY_AddNewMember( sPDESC_DATA pMaster, sPDESC_DATA pNewMember );
sPDESC_DATA PARTY_FindMember( sPDESC_DATA pPlayer, int idxNum );
void PARTY_OutMember( sPDESC_DATA pPlayer );

void PARTY_ClearMember( sPDESC_DATA pPlayer );

void PARTY_SendHPMP( sPDESC_DATA pPlayer );

sPDESC_DATA PARTY_ItemShare( sPDESC_DATA pPlayer );
void PARTY_MoneyShare( sPDESC_DATA pPlayer, int amount );


#endif

//


