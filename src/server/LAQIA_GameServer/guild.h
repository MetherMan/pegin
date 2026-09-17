/**
 *	$Workfile: guild.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 14 $
 *	$Date: 04-10-29 11:56p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/guild.h $
 * 
 * 14    04-10-29 11:56p Sk8snow
 * 
 * 13    04-07-13 7:26a Sk8snow
 * 
 * 12    04-05-29 10:12p Sk8snow
 * 
 * 11    04-05-11 1:45a Sk8snow
 * 
 * 10    04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __GUILD_H__
#define __GUILD_H__

#define dGUILD_LEVEL_NONE					0		// None
#define dGUILD_LEVEL_MEMBER					1		// 용병단원 , 군단원 
#define dGUILD_LEVEL_SECONDMASTER			2		// 부단장 , 부군단장 
#define dGUILD_LEVEL_MASTER					3		// 용병단장 , 군단장 

#define dGUILD_TYPE_NONE					0		// None
#define dGUILD_TYPE_MERCENARY				1		// 용병단
#define dGUILD_TYPE_CORPS					2		// 군단


extern sGUILD gGUILDS;

#define IS_VALID_GUILD( num ) (gGUILDS.array[num])
#define GET_GUILD_DATA( num ) (gGUILDS.array[num])

BOOL GUILD_LoadGuildInfo();
int GetNewGuildNum();
sPGUILD_DATA NewGuildData();
void DestroyGuildData();
BOOL CreateNewGuild( sPDESC_DATA pMaster , char *guildName , int guildMark);
void CrashGuild( int guildNo );													//길드 아작

void GUILD_DropMercenary( sPDESC_DATA pMember, int guildNo );
void GUILD_AddMercenary( sPDESC_DATA pNewbie, int guildNo );
void GUILD_AddMember( sPDESC_DATA pNewbie, int guildNo );
void GUILD_DropMember( sPDESC_DATA pMember, int guildNo );

void GUILD_SendGuildInfo( sPDESC_DATA pPlayer , BYTE mode );

void GUILD_SendToMember( sPGUILD_DATA pGuild , char *msg, int size );
void GUILD_SendToMercenery( sPGUILD_DATA pGuild, char *msg, int size );

void GUILD_ChangeMaster( sPDESC_DATA pMaster, sPDESC_DATA pNewMaster );

sPMEMBER_INFO GUILD_NewMemberInfo();
sPMEMBER_INFO GUILD_FindMemberInfoFromList( sPGUILD_DATA pGuild, char *name );

void GUILD_ChangeMemberInfo( sPDESC_DATA pPlayer );
void GUILD_LoadMemberInfo();
void GUILD_SendGuildMember( sPDESC_DATA pPlayer );
void GUILD_AdvanceLevel( sPDESC_DATA pPlayer, BYTE guildLevel );

int GUILD_CountGuildLevel( sPGUILD_DATA pGuild, BYTE guildLevel );
int GUILD_CountHaveCastle( int guildNum );

int GUILD_MemberCount( sPGUILD_DATA pGuild );

#endif



