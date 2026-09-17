/**
 *	$Workfile: admin.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 5 $
 *	$Date: 04-03-31 8:22a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/admin.h $
 * 
 * 5     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __ADMIN_H__
#define __ADMIN_H__

#define dADMIN_IMPL				1
#define dADMIN_GRGOD				2
#define dADMIN_GOD				3

struct admin_command
{
	char command[50];
	void (*admin_func)(sPDESC_DATA pPlayer, char *command );
	BYTE	level;						// 운영자 레벨	
};

#define dMAX_ADMIN_ID			256

typedef struct 
{
	char id[50];
	BYTE level;
	BYTE isGodCharacter;
} sADMIN_ID_LEVEL, *sPADMIN_ID_LEVEL;

#define dADMIN_IP	"10.0.0.122"			// C Class

/////////////////////////////////////////////////////////////
// F U N C S
/////////////////////////////////////////////////////////////
BOOL CheckAdminCommand( sPDESC_DATA pPlayer, char *command );
void InitAdminIdLevel();

#endif



