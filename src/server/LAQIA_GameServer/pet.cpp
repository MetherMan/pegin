/**
 *	$Workfile: pet.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 12 $
 *	$Date: 04-07-13 7:26a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/pet.cpp $
 * 
 * 12    04-07-13 7:26a Sk8snow
 * 
 * 11    04-04-03 4:03p Sk8snow
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
#include "mercenary.h"
#include "quest.h"

#include "house.h"

#include "itemdb.h"
#include "questdb.h"
#include "guild.h"

#include "effect.h"
#include "friends.h"

#include "pet.h"
#include "db_rows.h"

#ifndef dNOT_USE_PET

sPET_INFO g_PET_INFO[dMAX_PET_INFO];


//
// Function Name: LoadPetInfo
// Date: 
// Description: 
//
BOOL LoadPetInfo()
{
	FILE *fp;

	fp = fopen( dPET_INFO_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadPetInfo : Open file failed!\r\n" );
		return 0;
	}

	int nPos = 0;
	char readBuff[1024];
	
	BYTE level;
	int needExp;
	
	int	 criticalExp;

	WORD minDamage;
	WORD maxDamage;
	
	WORD Str;
	WORD Dex;
	WORD Int;
	WORD HP;
	WORD MP;
	WORD AGI;
	
	WORD Str_p;
	WORD Dex_p;
	WORD Int_p;
	WORD HP_p;
	WORD MP_p;
	WORD AGI_p;

	while( !feof( fp ) )
	{	
		nPos = 0;
		
		fgets( readBuff, 1024, fp );
	
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
		
		level = GetTabNumberic( readBuff, nPos );
		needExp = GetTabNumberic( readBuff, nPos );
		
		criticalExp = GetTabNumberic( readBuff, nPos );

		minDamage = GetTabNumberic( readBuff, nPos );
		maxDamage = GetTabNumberic( readBuff, nPos );
		
		Str = GetTabNumberic( readBuff, nPos );
		Dex = GetTabNumberic( readBuff, nPos );
		Int = GetTabNumberic( readBuff, nPos );
		HP = GetTabNumberic( readBuff, nPos );
		MP = GetTabNumberic( readBuff, nPos );
		AGI = GetTabNumberic( readBuff, nPos );
		
		Str_p = GetTabNumberic( readBuff, nPos );
		Dex_p = GetTabNumberic( readBuff, nPos );
		Int_p = GetTabNumberic( readBuff, nPos );
		HP_p = GetTabNumberic( readBuff, nPos );
		MP_p = GetTabNumberic( readBuff, nPos );
		AGI_p = GetTabNumberic( readBuff, nPos );

		if( level >= dMAX_PET_INFO )
		{
			log( "LoadPetInfo : level >= dMAX_PET_INFO\r\n" );
			fclose( fp );
			return 0;
		}
		
		g_PET_INFO[level].level = level;
		g_PET_INFO[level].needExp = needExp;
		g_PET_INFO[level].criticalExp = criticalExp;

		g_PET_INFO[level].minDamage = minDamage;
		g_PET_INFO[level].maxDamage = maxDamage;
		
		g_PET_INFO[level].Str = Str;
		g_PET_INFO[level].Dex = Dex;
		g_PET_INFO[level].Int = Int;
		g_PET_INFO[level].HP = HP;
		g_PET_INFO[level].MP = MP;
		g_PET_INFO[level].AGI = AGI;
		
		g_PET_INFO[level].Str_p = Str_p;
		g_PET_INFO[level].Dex_p = Dex_p;
		g_PET_INFO[level].Int_p = Int_p;
		g_PET_INFO[level].HP_p = HP_p;
		g_PET_INFO[level].MP_p = MP_p;
		g_PET_INFO[level].AGI_p = AGI_p;
	}

	fclose( fp );
	
	return 1;
}

//
// Function Name: PET_SendPetInfo
// Date: 
// Description: 
//
void PET_SendPetInfo( sPDESC_DATA pPlayer )
{
	if( !GET_PET_TYPE( pPlayer ) )
		return;

	if( GET_PET_LEVEL( pPlayer ) >= 41 )
	{
		GET_PET_TYPE( pPlayer ) = 3;
	}
	else if( GET_PET_LEVEL( pPlayer ) >= 21 )
	{
		GET_PET_TYPE( pPlayer ) = 2;
	}
	else 
	{
		GET_PET_TYPE( pPlayer ) = 1;
	}

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PET_INFO, g_nPos );

	PutByte( g_Packet, GET_PET_TYPE( pPlayer ), g_nPos );									// 타입
	PutInteger( g_Packet, GET_PET_LEVEL( pPlayer ), g_nPos );								// 레벨
	PutInteger( g_Packet, GET_PET_EXP( pPlayer ), g_nPos );									// 현재 경험치
	PutInteger( g_Packet, g_PET_INFO[MAX(GET_PET_LEVEL( pPlayer ), 50 )].needExp, g_nPos ); // 목표 경험치
	PutWord( g_Packet, GET_PET_ANGERGAGE( pPlayer ), g_nPos );								// 분노 게이지
	PutString( g_Packet, GET_PET_NAME( pPlayer ), g_nPos );									// 팻 이름
	PutWord( g_Packet, GET_PET_HP( pPlayer ), g_nPos );										// HP
	PutWord( g_Packet, g_PET_INFO[GET_PET_LEVEL( pPlayer )].HP, g_nPos );					// MAX HP
	PutWord( g_Packet, GET_PET_MP( pPlayer ), g_nPos );										// MP
	PutWord( g_Packet, g_PET_INFO[GET_PET_LEVEL( pPlayer )].MP, g_nPos );					// MAX MP
	PutWord( g_Packet, g_PET_INFO[GET_PET_LEVEL( pPlayer )].minDamage, g_nPos );			// min AP
	PutWord( g_Packet, g_PET_INFO[GET_PET_LEVEL( pPlayer )].maxDamage, g_nPos );			// max AP
	PutWord( g_Packet, GET_PET_LEVEL( pPlayer ), g_nPos );									// DP

	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: PET_SendPetOff
// Date: 
// Description: 
//
void PET_SendPetOff( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PET_OFF, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
	GET_PET_STATE( pPlayer ) = dPET_STATE_DISABLE;
}

//
// Function Name: PET_SendPetOn
// Date: 
// Description: 
//
void PET_SendPetOn( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PET_ON, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutByte( g_Packet, GET_PET_TYPE( pPlayer ), g_nPos );
	PutString( g_Packet, GET_PET_NAME( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
	GET_PET_STATE( pPlayer ) = dPET_STATE_VISIBLE;
}

//
// Function Name: PET_AttackToMob
// Date: 
// Description: 
//
void PET_AttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	if( !pPlayer || !pMob )
		return;

	if( !IS_USING_PET( pPlayer ) )
		return;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PET_ATTACK, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutInteger( g_Packet, pMob->idxNum, g_nPos );
	PutByte( g_Packet, dMOB, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
	
	if( pMob->isDead )
		return;
			
	WORD damage = 0;
	
	damage = number( g_PET_INFO[GET_PET_LEVEL( pPlayer )].minDamage, g_PET_INFO[GET_PET_LEVEL( pPlayer )].maxDamage );

	if( damage > 0 )
	{		
		pMob->hp -= damage;		
		
		SendPlayerDamage( pPlayer, pMob, damage, dCRITICAL_NONE );
	}
	else
		SendPlayerDamage( pPlayer, pMob, 0 , dCRITICAL_NONE );
	
	CheckDeadMob( pPlayer, pMob );
}

//
// Function Name: PET_UpdatePetGage
// Date: 
// Description: 
//
void PET_UpdatePetGage( sPDESC_DATA pPlayer )
{
	if( !GET_PET_TYPE( pPlayer ) )
		return;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_PET_GAGE, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, MAX( GET_PET_ANGERGAGE( pPlayer ), dMAX_PET_ANGER ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: PET_UpdateExp
// Date: 
// Description: 
//
void PET_UpdateExp( sPDESC_DATA pPlayer , int exp )
{
	if( IS_USING_PET( pPlayer ) )
	{
		if( GET_PET_LEVEL( pPlayer ) >= dMAX_PET_LEVEL )
			return;

		// 경험치 
		GET_PET_EXP( pPlayer ) += MIN( exp, 1 );

		g_nPos = 2;
		PutWord( g_Packet, dPACKET_PET_UPDATEEXP, g_nPos );
		PutInteger( g_Packet, GET_PET_EXP( pPlayer ), g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer, g_Packet, g_nPos );

		// 레벨업 체크 
		if( g_PET_INFO[GET_PET_LEVEL( pPlayer )].needExp <= GET_PET_EXP( pPlayer ) )
		{
			GET_PET_LEVEL( pPlayer ) = MAX( GET_PET_LEVEL( pPlayer )++, dMAX_PET_LEVEL );
			GET_PET_EXP( pPlayer ) = 0;
			PET_SendPetInfo( pPlayer );
			SendSystemMsg( pPlayer, g_LANG_STR[130], GET_PET_LEVEL( pPlayer ) );

			if( GET_PET_LEVEL( pPlayer ) == 21 )
			{
				if( IS_USING_PET( pPlayer ) )
					PET_SendPetOff( pPlayer );
			
				GET_PET_TYPE( pPlayer ) = 2;
				PET_SendPetInfo( pPlayer );
				PET_SendPetOn( pPlayer );
				
				PET_SaveUserPet( pPlayer );
			}
			else if( GET_PET_LEVEL( pPlayer ) == 41 )
			{
				if( IS_USING_PET( pPlayer ) )
					PET_SendPetOff( pPlayer );

				GET_PET_TYPE( pPlayer ) = 3;
				PET_SendPetInfo( pPlayer );
				PET_SendPetOn( pPlayer );

				PET_SaveUserPet( pPlayer );
			}
		}
	}
}

//
// Function Name: PET_LoadUserPet
// Date: 
// Description: 
//
void PET_LoadUserPet( sPDESC_DATA pPlayer )
{
	char query[1024];

	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "select * from UserPet where ownerID = '%s' and ownerPos = %d limit 1", escID, GET_CHAR_POS( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;

		result = mysql_store_result( g_MYSQL );
		
		if( result )
		{
			while( ( row = mysql_fetch_row( result ) ) )
			{
				GET_PET_TYPE( pPlayer ) = atoi( row[PETDATA_TYPE] );
				GET_PET_LEVEL( pPlayer ) = atoi( row[PETDATA_LEVEL] );
				GET_PET_HP( pPlayer ) = atoi( row[PETDATA_HP] );
				GET_PET_MP( pPlayer ) = atoi( row[PETDATA_MP] );
				GET_PET_EXP( pPlayer ) = atoi( row[PETDATA_EXP] );
				GET_PET_STATE( pPlayer ) = atoi( row[PETDATA_STATE] );
				GET_PET_ANGERGAGE( pPlayer ) = atoi( row[PETDATA_ANGERGAGE] );
				strcpy( GET_PET_NAME( pPlayer ), row[PETDATA_NAME] );
			}
			mysql_free_result( result );
		}
	}
}

//
// Function Name: PET_SaveUserPet
// Date: 
// Description: 
//
void PET_SaveUserPet( sPDESC_DATA pPlayer )
{
	if( !GET_PET_TYPE( pPlayer ) )
		return;

	char query[1024];
	
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "update UserPet set "
		"type=%d,"
		"level=%d,"
		"hp=%d,"
		"mp=%d,"
		"exp=%d,"
		"state=%d,"
		"angerGage=%d "
		"where ownerID = '%s' and ownerPos = %d",

		GET_PET_TYPE( pPlayer ),
		GET_PET_LEVEL( pPlayer ),
		GET_PET_HP( pPlayer ),
		GET_PET_MP( pPlayer ),
		GET_PET_EXP( pPlayer ),
		GET_PET_STATE( pPlayer ),
		GET_PET_ANGERGAGE( pPlayer ),

		escID, GET_CHAR_POS( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}

//
// Function Name: PET_SaveUserPet
// Date: 
// Description: 
//
void PET_UpdatePetName( sPDESC_DATA pPlayer, char *name )
{
	if( !GET_PET_TYPE( pPlayer ) )
		return;

	if( strlen( name ) >= dPET_NAME_LEN )
		return;

	if( !strcmp( name , GET_PET_NAME( pPlayer ) ) )
		return;

	char query[1024];
	
	char escID[128];
	char escPetName[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	mysql_escape_string( escPetName, name, strlen( name ) );

	sprintf( query, "update UserPet set name = '%s' where ownerID = '%s' and ownerPos = %d limit 1",
		escPetName, escID, GET_CHAR_POS( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}

//
// Function Name: PET_InsertUserPetData
// Date: 
// Description: 
//
void PET_InsertUserPetData( sPDESC_DATA pPlayer )
{
	if( !GET_PET_TYPE( pPlayer ) )
		return;

	char query[1024];
	
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "insert into UserPet values( '', '%s', %d, %d, %d, %d, %d, %d, %d, %d, '%s' )",
		escID, 
		GET_CHAR_POS( pPlayer ), 
		GET_PET_TYPE( pPlayer ), 
		GET_PET_LEVEL( pPlayer ), 
		GET_PET_HP( pPlayer ), 
		GET_PET_MP( pPlayer ), 
		GET_PET_EXP( pPlayer ), 
		GET_PET_STATE( pPlayer ),
		GET_PET_ANGERGAGE( pPlayer ), 
		GET_PET_NAME( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}


//
// Function Name: PET_SupplyPet
// Date: 
// Description: 
//
void PET_SupplyPet( sPDESC_DATA pPlayer )
{
	if( GET_PET_TYPE( pPlayer ) )
		return;
	
	GET_PET_TYPE( pPlayer ) = 1;
	GET_PET_LEVEL( pPlayer ) = 0;
	GET_PET_HP( pPlayer ) = g_PET_INFO[GET_PET_LEVEL( pPlayer )].HP;
	GET_PET_MP( pPlayer ) = g_PET_INFO[GET_PET_LEVEL( pPlayer )].MP;
	GET_PET_EXP( pPlayer ) = 0;
	GET_PET_STATE( pPlayer ) = dPET_STATE_DISABLE;
	GET_PET_ANGERGAGE( pPlayer ) = 0;
	strcpy( GET_PET_NAME( pPlayer ), "PET" );

	PET_SendPetInfo( pPlayer );
	PET_SendPetOn( pPlayer );

	PET_InsertUserPetData( pPlayer );

	SendSystemMsg( pPlayer, g_LANG_STR[131] );
}

//
// Function Name: PET_DeletePetData
// Date: 
// Description: 
//
void PET_DeletePetData( sPDESC_DATA pPlayer )
{
	char query[1024];
	
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from UserPet where ownerID = '%s' and ownerPos = %d limit 1",
		escID, GET_CHAR_POS( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}


#endif // dNOT_USE_PET

// EOF











