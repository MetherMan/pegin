/**
 *	$Workfile: mercenary.cpp $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 3 $
 *	$Date: 04-03-31 8:22a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/mercenary.cpp $
 * 
 * 3     04-03-31 8:22a Sk8snow
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

#include "mercenary.h"


//
// Function Name: MERC_SendAddMerc
// Date: 2003-05-30
// Description: 
//
void MERC_SendAddMerc( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_ADD_HERO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutByte( g_Packet, GET_MERC_TYPE( pPlayer ), g_nPos );
	
	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}

//
// Function Name: MERC_SendRemoveMerc
// Date: 2003-05-30
// Description: 
//
void MERC_SendRemoveMerc( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_REMOVE_HERO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}

//
// Function Name: MERC_Create
// Date: 2003-05-30
// Description: 
//
BOOL MERC_Create( sPDESC_DATA pPlayer , BYTE mercType )
{
	if( GET_MERC_TYPE( pPlayer ) != 0 )
		return 0;

	GET_MERC_TYPE( pPlayer ) = mercType;
	GET_MERC_LEVEL( pPlayer ) = 20;
	GET_MERC_EXP( pPlayer ) = 0;

	DATASERV_SendUpdateMercInfo( pPlayer );

	MERC_SendAddMerc( pPlayer );
	return 1;
}

//
// Function Name: MERC_Delete
// Date: 2003-05-30
// Description: 
//
void MERC_Delete( sPDESC_DATA pPlayer )
{
	GET_MERC_TYPE( pPlayer ) = 0;
	GET_MERC_LEVEL( pPlayer ) = 0;
	GET_MERC_EXP( pPlayer ) = 0;

	DATASERV_SendUpdateMercInfo( pPlayer );

	MERC_SendRemoveMerc( pPlayer );
}


//


