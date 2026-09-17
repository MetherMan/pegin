/**
 *	$Workfile: war.h $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 30 $
 *	$Date: 04-09-08 3:54p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/war.h $
 * 
 * 30    04-09-08 3:54p Sk8snow
 * 
 * 29    04-07-13 7:26a Sk8snow
 * 
 * 28    04-05-29 10:12p Sk8snow
 * 
 * 27    04-05-21 11:37a Sk8snow
 * 
 * 26    04-05-11 1:45a Sk8snow
 * 
 * 25    04-04-30 9:46a Sk8snow
 * 
 * 24    04-04-07 10:17p Sk8snow
 * 
 * 23    04-03-31 8:22a Sk8snow
 *        
 */

#ifndef __WAR_H__
#define __WAR_H__

#define dWARCRISTAL_HP			100000

#define dWAR_WIN_POINT				100
#define dWARCRASTAL_CRASH_POINT		20

typedef struct 
{
	sPWAR_DATA camp[2];
	sPWAR_DATA linkedCastle[dMAX_LINKED_CASTLE];
} sWARDATA_SET, *sPWARDATA_SET;

//////////////////////////////////////////////////////////
// G L O B A L  F U N C
//////////////////////////////////////////////////////////
BYTE LoadWarInfo();

sPWAR_DATA NewWarData();
void WAR_DestroyWarData();
sPWAR_DATA WAR_FindData2( int ownerGuild, int enemyGuild, BYTE type );
sPWAR_DATA WAR_FindOwnerData( int ownerGuild );
sPWAR_DATA WAR_FindEnemyData( int enemyGuild );
sPWAR_DATA WAR_FindData( int ownerGuild, int enemyGuild );
sPWAR_DATA WAR_FindDataByType( int ownerGuild, BYTE type );

void WAR_MakeCamp( sPDESC_DATA pPlayer ); 
void WAR_ClearWorldMob( int mapNum );
BYTE IsCanMakeCampDate();

BYTE IsBeginWarTime( BYTE mode );
BYTE IsFinishWarTime( BYTE mode );

void BuildWarDataList();

void WAR_CheckWarTime( BYTE noTimeCheck );

BOOL LoadWarCristalInfo();
BOOL LoadCastleDoorInfo();

void WAR_PopMapObject( int mapNum );
void WAR_PopWarCristal( int mapNum );
void WAR_FindCampData( sPWAR_DATA camp[2], int mapNum );

void WAR_SendWarCampInfo( sPDESC_DATA pPlayer, BYTE toWorld = 0 );
void WAR_SendWarInfo( sPDESC_DATA pPlayer, int mapNum , BYTE toWorld = 0 );

BOOL LoadWarData();

void WAR_WriteWarData( sPWAR_DATA pWarData );
void WAR_UpdateWarData( sPWAR_DATA pWarData );

void LoadMapOwnerGuild();

void WAR_SummonWarPosition( sPWORLD pWorld , int ownerGuild );

BOOL WAR_FindOwnCastle( int guildNum );

void WAR_CheckEnd( sPWORLD pWorld , int point );
BOOL WAR_MakeWarDataSet( sPWORLD pWorld , sPWARDATA_SET pDataSet );
sPWAR_DATA WAR_FindDataByMap( int ownerGuild, int enemyGuild, int mapNum );

BYTE IsPointTime();

void SPoilsCastleTaxBank( int guildNum, int taxValue );

void WAR_SendEnemyInfo( sPDESC_DATA pPlayer );

BOOL BattleSystem( sPDESC_DATA pAttacker, sPDESC_DATA pVic );

#endif



//EOF


