/**
 *	$Workfile: effect.cpp $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 18 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/effect.cpp $
 * 
 * 18    04-12-15 10:40a Sk8snow
 * 
 * 17    04-09-20 10:53p Sk8snow
 * 
 * 16    04-09-11 11:20p Sk8snow
 * 
 * 15    04-07-13 7:26a Sk8snow
 * 
 * 14    04-05-29 10:12p Sk8snow
 * 
 * 13    04-05-14 10:38p Sk8snow
 * 
 * 12    04-05-11 1:45a Sk8snow
 * 
 * 11    04-04-30 9:46a Sk8snow
 * 
 * 10    04-03-31 8:22a Sk8snow
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
#include "effect.h"

#include "message.h"


//
// Function Name: SendSoundEffect
// Date:
// Description: 
//
void SendSoundEffect( sPDESC_DATA pPlayer, WORD soundIdx )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SOUND_EFFECT, g_nPos );
	PutWord( g_Packet, soundIdx, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

void ClearPlayerSkillEffect( sPDESC_DATA pPlayer )
{
	for( int i = 0; i < dMAX_PLAYER_EFFECT; i++ )
	{
		if( pPlayer->ch2.skillEffect[i].delay > 0 )
			SendEffectOFF( pPlayer, pPlayer->ch2.skillEffect[i].skillNum );
	}
}

void SendAllEffect( sPDESC_DATA pPlayer )
{
	for( int i = 0; i < dMAX_PLAYER_EFFECT; i++ )
	{
		if( pPlayer->ch2.skillEffect[i].delay > 0 )
		{
			SendEffectON( pPlayer, pPlayer->ch2.skillEffect[i].skillNum, pPlayer->ch2.skillEffect[i].delay - g_CurrTime );
		}
	}
}

//
// Function Name: SendEffectON
// Date: 
// Description: 
//
void SendEffectON( sPDESC_DATA pPlayer, WORD skillNum, int time )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_ADD_EFFECT, g_nPos );
	PutWord( g_Packet, skillNum, g_nPos );
	PutInteger( g_Packet, time, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: SendEffectOFF
// Date: 
// Description: 
//
void SendEffectOFF( sPDESC_DATA pPlayer, WORD skillNum )
{
	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_REMOVE_EFFECT, g_nPos );
	PutWord( g_Packet, skillNum, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: EFFECT_CheckTime
// Date: 
// Description: 
//
void EFFECT_CheckTime( sPDESC_DATA pPlayer )
{
#ifndef dNON_PVP_MODE
	if( pPlayer->ch2.isEvilCha && pPlayer->ch2.evilChaTime + 30 <= g_CurrRealTime )
	{
		pPlayer->ch2.isEvilCha = 0;
		pPlayer->ch2.evilChaTime = 0;

		SendPlayerNameColor( pPlayer );
	}
#endif

	if( GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) )
	{
		if( GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) <= g_CurrTime )
			UnSetPlayerStun( pPlayer );
	}

	for( int i = 0; i < dMAX_PLAYER_EFFECT; i++ )
	{
		if( pPlayer->ch2.skillEffect[i].delay && pPlayer->ch2.skillEffect[i].delay <= g_CurrTime )
		{
			if( pPlayer->ch2.skillEffect[i].skillNum == dSPE_SKILL_BURROW )
			{
				SetVisible( pPlayer, 0 );
				SetMovable( pPlayer, 0 );
			}
			else if( pPlayer->ch2.skillEffect[i].skillNum == dSPE_SKILL_STELTH )
			{
				SetVisible( pPlayer, 0 );
			}

			SendEffectOFF( pPlayer, pPlayer->ch2.skillEffect[i].skillNum );
			ClearEffect( pPlayer, i );
		}
	}
}


//
// Function Name: SendPlayerMovable
// Date: 
// Description: 
//
void SetMovable( sPDESC_DATA pPlayer, BYTE flag )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SET_MOVABLE, g_nPos );
	PutByte( g_Packet, flag, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: FISHING_CheckTime
// Date: 
// Description: 
//
void FISHING_CheckTime( sPDESC_DATA pPlayer )
{	
}

// EOF



