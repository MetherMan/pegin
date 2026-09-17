/**
 *	$Workfile: mercenary.h $
 *
 *	Copyright (c) 2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 2 $
 *	$Date: 04-03-31 8:22a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/mercenary.h $
 * 
 * 2     04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __MERCENARY_H__
#define __MERCENARY_H__

#define GET_MERC_TYPE( p ) (p->ch.mercType)
#define GET_MERC_LEVEL( p ) (p->ch.mercLevel)
#define GET_MERC_EXP( p ) (p->ch.mercExp )

void MERC_SendAddMerc( sPDESC_DATA pPlayer );
void MERC_SendRemoveMerc( sPDESC_DATA pPlayer );
BOOL MERC_Create( sPDESC_DATA pPlayer , BYTE mercType );
void MERC_Delete( sPDESC_DATA pPlayer );

#endif

//



