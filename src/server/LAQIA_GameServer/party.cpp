/**
 *	$Workfile: party.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 9 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/party.cpp $
 * 
 * 9     04-12-22 5:18p Sk8snow
 * 
 * 8     04-12-15 10:40a Sk8snow
 * 
 * 7     04-09-08 3:54p Sk8snow
 * 
 * 6     04-07-13 7:26a Sk8snow
 * 
 * 5     04-04-30 9:46a Sk8snow
 * 
 * 4     04-03-31 8:22a Sk8snow
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

#include "billing.h"
#include "effect.h"

//
// Function Name: PARTY_SendToAll
// Date: 2003-05-09
// Description: 
//
void PARTY_SendToAll( sPDESC_DATA pPlayer, char *msg, int size, BYTE mode )
{
	if( !GET_PARTY_MASTER( pPlayer ) )
		return;

	if( mode == dSENDMODE_TOALL )
	{
		SendData( GET_PARTY_MASTER( pPlayer ), msg, size );
		
		for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
		{
			if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
				SendData( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
		}
	}
	else if( mode == dSENDMODE_TOOTHER )
	{
		if( GET_PARTY_MASTER( pPlayer ) != pPlayer )
			SendData( GET_PARTY_MASTER( pPlayer ), msg, size );
		
		for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
		{
			if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
				if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] != pPlayer )
					SendData( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
		}
	}
}


//
// Function Name: PARTY_SendToMember
// Date: 2003-05-09
// Description: 
//
void PARTY_SendToMember( sPDESC_DATA pPlayer, char *msg, int size )
{
	for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
			SendData( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
	}
}

//
// Function Name: PARTY_Create
// Date: 2003-05-09
// Description: 
//
BOOL PARTY_Create( sPDESC_DATA pPlayer )
{
	if( GET_PARTY_MASTER( pPlayer ) )
		return 0;

	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR )
	{
		sPWORLD_PARTY_BOARD pArticle = FindPartyBoardArticle( pPlayer );
		
		if( pArticle )
		{
			REMOVE_FROM_LIST( gWORLD[GET_MAP_NUM( pPlayer )]->partyBoard, pArticle, prev, next );
			INSERT_TO_LIST( gPARTY_BOARD_MEMORY, pArticle, prev, next );
		}
	}

	GET_PARTY_MASTER( pPlayer ) = pPlayer;
	
	return 1;
}

//
// Function Name: PARTY_Create
// Date: 2003-05-13
// Description: 
//
void PARTY_Destroy( sPDESC_DATA pPlayer )
{
	if( GET_PARTY_MASTER( pPlayer ) == NULL )
		return;

	if( GET_PARTY_MASTER( pPlayer ) != pPlayer )
		return;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTY_DESTROYED, g_nPos );
	PutSize( g_Packet, g_nPos );
	PARTY_SendToAll( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );

	for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
			GET_PARTY_MASTER( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) = NULL;
	}
	// 파티 마스터의 파티정보 초기화
	GET_PARTY_MASTER( pPlayer ) = NULL;
	PARTY_ClearMember( pPlayer );
}

//
// Function Name: MakePartyInPacket
// Date: 
// Description: 
//
void MakePartyInPacket( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTY_ADDMEMBER, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutString( g_Packet, pPlayer->ch.name, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.hp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.max_hp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.mp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.max_mp, g_nPos );
	PutSize( g_Packet, g_nPos );
}

//
// Function Name: PARTY_AddNewMember
// Date: 2003-05-22
// Description: 
//
void PARTY_AddNewMember( sPDESC_DATA pMaster, sPDESC_DATA pNewMember )
{
	if( GET_PARTY_MASTER( pMaster ) != pMaster )
		return;

	if( GET_PARTY_MASTER( pNewMember ) )
		return;

	if( pMaster->ch2.party.memberCnt >= dMAX_PARTY_MEMBER )
		return;

	if( GET_MAP_KIND( pNewMember ) != dMAP_KIND_INDOOR )
	{
		sPWORLD_PARTY_BOARD pArticle = FindPartyBoardArticle( pNewMember );

		if( pArticle )
		{
			REMOVE_FROM_LIST( gWORLD[GET_MAP_NUM( pNewMember )]->partyBoard, pArticle, prev, next );
			INSERT_TO_LIST( gPARTY_BOARD_MEMORY, pArticle, prev, next );
		}
	}

	// 완료
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTY_JOIN_RESULT, g_nPos );
	PutByte( g_Packet, 3, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pNewMember );

	// 새로 추가되는 맴버의 정보를 기존의 멤버들에게 전송 
	MakePartyInPacket( pNewMember );
	PARTY_SendToAll( pMaster, g_Packet, g_nPos, dSENDMODE_TOALL );

	// 기존의 맴버와 파티장의 정보를 가져옴
	MakePartyInPacket( pMaster );
	SendData( pNewMember );

	int i;

	for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( pMaster->ch2.party.member[i] )
		{
			MakePartyInPacket( pMaster->ch2.party.member[i] );
			SendData( pNewMember );
		}
	}

	for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( !pMaster->ch2.party.member[i] )
		{
			pMaster->ch2.party.member[i] = pNewMember;
			break;
		}
	}
	
	GET_PARTY_MASTER( pNewMember ) = pMaster;
	pMaster->ch2.party.memberCnt++;

#ifdef dDAUM_BILLING
	sPDESC_DATA pTarget = NULL;

	// 그룹 HP 드링크
	if( pMaster->ch2.billingFlag[dBILL_ITEM_EFFECT13] )
	{		
		pTarget = pMaster;
		
		// 개인 , 군단원 HP 증가가 없을 경우만
		if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT10] && 
			!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
		{
			if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
			{
				pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
				GET_HPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13];	
				
				SendPlayerHP( pTarget );
				SendPlayerMaxHPMP( pTarget );
				
				SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL23 );
			}
		}
			
		pTarget = pNewMember;
		
		if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT10] && 
			!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
		{
			if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
			{
				pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
				GET_HPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13];	

				SendPlayerHP( pTarget );
				SendPlayerMaxHPMP( pTarget );
				
				SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL23 );
			}
		}
	}

	// 그룹 MP 드링크 
	if( pMaster->ch2.billingFlag[dBILL_ITEM_EFFECT14] )
	{
		pTarget = pMaster;
		
		// 개인 , 군단원 HP 증가가 없을 경우만
		if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT11] && 
			!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
		{
			if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
			{
				pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
				GET_MPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14];	

				SendPlayerMP( pTarget );
				SendPlayerMaxHPMP( pTarget );
				
				SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL24 );
			}
		}

		pTarget = pNewMember;

		// 개인 , 군단원 HP 증가가 없을 경우만
		if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT11] && 
			!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
		{
			if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
			{
				pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
				GET_MPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14];	
				
				SendPlayerMP( pTarget );
				SendPlayerMaxHPMP( pTarget );
				
				SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL24 );
			}
		}
	}
#endif
}

//
// Function Name: PARTY_FindMember
// Date: 2003-05-22
// Description: 
//
sPDESC_DATA PARTY_FindMember( sPDESC_DATA pPlayer, int idxNum )
{
	for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
		{
			if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i]->sock == idxNum )
				return GET_PARTY_MASTER( pPlayer )->ch2.party.member[i];
		}
	}
	return NULL;
}


//
// Function Name: PARTY_OutMember
// Date: 2003-05-22
// Description: 
//
void PARTY_OutMember( sPDESC_DATA pPlayer )
{
	sPDESC_DATA pMaster = GET_PARTY_MASTER( pPlayer );

	if( !pMaster )
		return;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTY_DELMEMBER, g_nPos );
	PutInteger( g_Packet, pPlayer->sock, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	PARTY_SendToAll( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );

	SendData( GET_PARTY_MASTER( pPlayer ) );

	for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
			if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] == pPlayer )
				GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] = NULL;
	}

	GET_PARTY_MASTER( pPlayer )->ch2.party.memberCnt--;
	GET_PARTY_MASTER( pPlayer ) = NULL;

	int memberCnt = pMaster->ch2.party.memberCnt;

#ifdef dDAUM_BILLING
	if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
	{
		GET_HPADD( pPlayer ) -= pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13];
		pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = 0;

		if( GET_HP( pPlayer ) > ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
		{
			GET_HP( pPlayer ) = ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
		}
		SendPlayerHP( pPlayer );
		SendPlayerMaxHPMP( pPlayer );
	}
	
	if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
	{
		GET_MPADD( pPlayer ) -= pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14];
		pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = 0;
		
		if( GET_MP( pPlayer ) > ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) ) )
		{
			GET_MP( pPlayer ) = ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
		}
		SendPlayerMP( pPlayer );
		SendPlayerMaxHPMP( pPlayer );
	}
	
	if( memberCnt == 0 )
	{
		if( pMaster->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
		{
			GET_HPADD( pMaster ) -= pMaster->ch2.billingAddValue[dBILL_ITEM_EFFECT13];
			pMaster->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = 0;

			if( GET_HP( pMaster ) > ( GET_MAX_HP( pMaster ) + GET_HPADD( pMaster ) ) )
			{
				GET_HP( pMaster ) = ( GET_MAX_HP( pMaster ) + GET_HPADD( pMaster ) );
			}			
			SendPlayerHP( pMaster );
			SendPlayerMaxHPMP( pMaster );
		}
		
		if( pMaster->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
		{
			GET_MPADD( pMaster ) -= pMaster->ch2.billingAddValue[dBILL_ITEM_EFFECT14];
			pMaster->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = 0;

			if( GET_MP( pMaster ) > ( GET_MAX_MP( pMaster ) + GET_MPADD( pMaster ) ) )
			{
				GET_MP( pMaster ) = ( GET_MAX_MP( pMaster ) + GET_MPADD( pMaster ) );
			}
			SendPlayerMP( pMaster );			
			SendPlayerMaxHPMP( pMaster );
		}
	}
#endif
	if( pMaster )
	{
		if( GET_PARTY_MEMBER_CNT( pMaster ) == 0 )
			PARTY_Destroy( pMaster );
	}
}


//
// Function Name: PARTY_ClearMember
// Date: 2003-05-22
// Description: 
//
void PARTY_ClearMember( sPDESC_DATA pPlayer )
{
#ifdef dDAUM_BILLING
	if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
	{
		GET_HPADD( pPlayer ) -= pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13];
		pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = 0;

		if( GET_HP( pPlayer ) > ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
		{
			GET_HP( pPlayer ) = ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
		}
		SendPlayerHP( pPlayer );		
		SendPlayerMaxHPMP( pPlayer );
	}
	
	if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
	{
		GET_MPADD( pPlayer ) -= pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14];
		pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = 0;
		
		if( GET_MP( pPlayer ) > ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) ) )
		{
			GET_MP( pPlayer ) = ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
		}
		SendPlayerMP( pPlayer );		
		SendPlayerMaxHPMP( pPlayer );
	}
#endif

	for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
#ifdef dDAUM_BILLING
		if( pPlayer->ch2.party.member[i] )
		{
			if( pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
			{
				GET_HPADD( pPlayer->ch2.party.member[i] ) -= pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT13];
				pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = 0;
				
				if( GET_HP( pPlayer->ch2.party.member[i] ) > ( GET_MAX_HP( pPlayer->ch2.party.member[i] ) + GET_HPADD( pPlayer->ch2.party.member[i] ) ) )
				{
					GET_HP( pPlayer->ch2.party.member[i] ) = ( GET_MAX_HP( pPlayer->ch2.party.member[i] ) + GET_HPADD( pPlayer->ch2.party.member[i] ) );
				}
				SendPlayerHP( pPlayer->ch2.party.member[i] );
				SendPlayerMaxHPMP( pPlayer->ch2.party.member[i] );
			}
			
			if( pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
			{
				GET_MPADD( pPlayer->ch2.party.member[i] ) -= pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT14];
				pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = 0;
				
				if( GET_MP( pPlayer->ch2.party.member[i] ) > ( GET_MAX_MP( pPlayer->ch2.party.member[i] ) + GET_MPADD( pPlayer->ch2.party.member[i] ) ) )
				{
					GET_MP( pPlayer->ch2.party.member[i] ) = ( GET_MAX_MP( pPlayer->ch2.party.member[i] ) + GET_MPADD( pPlayer->ch2.party.member[i] ) );
				}
				SendPlayerMP( pPlayer->ch2.party.member[i] );
				SendPlayerMaxHPMP( pPlayer->ch2.party.member[i] );
			}		
		}
#endif
		pPlayer->ch2.party.member[i] = NULL;
	}
	pPlayer->ch2.party.memberCnt = 0;
}

//
// Function Name: PARTY_SendMovePacket
// Date: 2003-05-22
// Description: 
//
void PARTY_SendMovePacket( sPDESC_DATA pPlayer )
{
	if( !GET_PARTY_MASTER( pPlayer ) )
		return;

	g_nPos = 2;

	PutWord( g_Packet, dPACKET_PARTY_MEMBER_MAPINFO, g_nPos );
	PutInteger( g_Packet, pPlayer->sock, g_nPos );
	PutWord( g_Packet, pPlayer->ch.mapNum, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.posX, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.posY, g_nPos );
	PutSize( g_Packet, g_nPos );

	PARTY_SendToAll( pPlayer, g_Packet, g_nPos, dSENDMODE_TOOTHER );
}


//
// Function Name: PARTY_SendHPMP
// Date: 2003-05-22
// Description: 
//
void PARTY_SendHPMP( sPDESC_DATA pPlayer )
{
	if( !GET_PARTY_MASTER( pPlayer ) )
		return;

	g_nPos = 2;

	PutWord( g_Packet, dPACKET_PARTY_MEMBER_HPMP, g_nPos );

	PutInteger( g_Packet, pPlayer->sock, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.hp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.max_hp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.mp, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.max_mp, g_nPos );
	
	PutSize( g_Packet, g_nPos );

	PARTY_SendToAll( pPlayer, g_Packet, g_nPos, dSENDMODE_TOOTHER );
}

//
// Function Name: PARTY_ItemShare
// Date: 
// Description: 
//
sPDESC_DATA PARTY_ItemShare( sPDESC_DATA pPlayer )
{
	if( !GET_PARTY_MASTER( pPlayer ) )
		return NULL;

	for( int i = 0; i < 5; i++ )
	{
		BYTE slot = number( 0, 7 );

		sPDESC_DATA pMember = NULL;

		// 파티 마스터 
		if( slot == 7 )
		{
			pMember = GET_PARTY_MASTER( pPlayer );
		}
		else
		{
			pMember = GET_PARTY_MASTER( pPlayer )->ch2.party.member[slot];
		}

		if( !pMember )
			continue;

		if( GET_MAP_NUM( pPlayer ) == GET_MAP_NUM( pMember ) &&
			abs( GET_POSX( pPlayer ) - GET_POSX( pMember ) ) <= 50 &&
			abs( GET_POSY( pPlayer ) - GET_POSY( pMember ) ) <= 50 )
		{
			return pMember;
		}
	}

	return NULL;
}

//
// Function Name: PARTY_MoneyShare
// Date: 
// Description: 
//
void PARTY_MoneyShare( sPDESC_DATA pPlayer, int amount )
{
	if( !GET_PARTY_MASTER( pPlayer ) )
		return;

	int i;
	int memberCnt = 0;

	if( GET_MAP_NUM( pPlayer ) == GET_MAP_NUM( GET_PARTY_MASTER( pPlayer ) ) &&
		abs( GET_POSX( pPlayer ) - GET_POSX( GET_PARTY_MASTER( pPlayer ) ) ) <= 50 &&
		abs( GET_POSY( pPlayer ) - GET_POSY( GET_PARTY_MASTER( pPlayer ) ) ) <= 50 )
		memberCnt++;

	for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
		{
			if( GET_MAP_NUM( pPlayer ) == GET_MAP_NUM( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) &&
				abs( GET_POSX( pPlayer ) - GET_POSX( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) ) <= 50 &&
				abs( GET_POSY( pPlayer ) - GET_POSY( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) ) <= 50 )
				memberCnt++;
		}
	}

	int surplus = amount % memberCnt;
	int realAmount = amount - surplus;
	int amountPerMember = realAmount / MIN( memberCnt, 1 );

	
	if( pPlayer == GET_PARTY_MASTER( pPlayer ) )
	{
		GET_MONEY( pPlayer ) += amountPerMember + surplus;
	
		UpdateMoney( pPlayer );
		
		DATASERV_SendUpdateMoney( pPlayer );
		
		SendSystemMsg( pPlayer, g_LANG_STR[9], amountPerMember + surplus );

		for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
		{
			if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
			{
				if( GET_MAP_NUM( pPlayer ) == GET_MAP_NUM( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) &&
					abs( GET_POSX( pPlayer ) - GET_POSX( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) ) <= 50 &&
					abs( GET_POSY( pPlayer ) - GET_POSY( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) ) <= 50 )
				{
					GET_PARTY_MASTER( pPlayer )->ch2.party.member[i]->ch.money += amountPerMember;
					UpdateMoney( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
					DATASERV_SendUpdateMoney( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
					SendSystemMsg( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i], g_LANG_STR[9], amountPerMember );				
				}
			}
		}
	}
	else
	{
		if( GET_MAP_NUM( pPlayer ) == GET_MAP_NUM( GET_PARTY_MASTER( pPlayer ) ) &&
			abs( GET_POSX( pPlayer ) - GET_POSX( GET_PARTY_MASTER( pPlayer ) ) ) <= 50 &&
			abs( GET_POSY( pPlayer ) - GET_POSY( GET_PARTY_MASTER( pPlayer ) ) ) <= 50 )
		{	
			GET_PARTY_MASTER( pPlayer )->ch.money += amountPerMember;
			
			UpdateMoney( GET_PARTY_MASTER( pPlayer ) );
			
			DATASERV_SendUpdateMoney( GET_PARTY_MASTER( pPlayer ) );
			
			SendSystemMsg( GET_PARTY_MASTER( pPlayer ), g_LANG_STR[9], amountPerMember);
		}		

		for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
		{
			if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] )
			{
				if( GET_MAP_NUM( pPlayer ) == GET_MAP_NUM( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) &&
					abs( GET_POSX( pPlayer ) - GET_POSX( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) ) <= 50 &&
					abs( GET_POSY( pPlayer ) - GET_POSY( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] ) ) <= 50 )
				{
					
					if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] == pPlayer )
						GET_PARTY_MASTER( pPlayer )->ch2.party.member[i]->ch.money += amountPerMember + surplus;
					else
						GET_PARTY_MASTER( pPlayer )->ch2.party.member[i]->ch.money += amountPerMember;
					
					UpdateMoney( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
					
					DATASERV_SendUpdateMoney( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] );
					
					if( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i] == pPlayer )				
						SendSystemMsg( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i], g_LANG_STR[9], amountPerMember + surplus );				
					else
						SendSystemMsg( GET_PARTY_MASTER( pPlayer )->ch2.party.member[i], g_LANG_STR[9], amountPerMember );				
				}
			}
		}
	}
}

//


