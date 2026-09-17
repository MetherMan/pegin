/**
 *	$Workfile: house.h $
 *
 *	Copyright (c) 2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 4 $
 *	$Date: 04-07-13 7:26a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/house.h $
 * 
 * 4     04-07-13 7:26a Sk8snow
 * 
 * 3     04-03-31 8:22a Sk8snow
 *        
 */


#ifndef __HOUSE_H__
#define __HOUSE_H__

#define dMAX_HOUSE					512		// Áý

///////////////////////////////////////////////
// E  X  T  E  R  N  S
///////////////////////////////////////////////
extern sPHOUSE		gHOUSE[dMAX_HOUSE];

//////////////////////////////////////////////
// D  E  F  I  N  E  S
//////////////////////////////////////////////
#define IS_VALID_HOUSE( num ) ( num < dMAX_HOUSE && gHOUSE[num] )

/////////////////////////////////////////////
// F  U  N  C  T  I  O  N
/////////////////////////////////////////////
BOOL InitHouse( int hNum );
void DestroyWorldHouse();	
int GetHouseNum( sPDESC_DATA pPlayer, int doorNum );

void SendToHouse( int mapNum, char *data, int size );

int GetEmptyHouseNum();
BOOL LoadHouseInfo( int mapNum );

int FindHouse( int mapNum, int doorNum );

#endif



//



