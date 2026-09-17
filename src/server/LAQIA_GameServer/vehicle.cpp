#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "message.h"

#include "player.h"
#include "player_world.h"
#include "item.h"
#include "player.h"
#include "party.h"
#include "deal.h"

#include "ship.h"
#include "vehicle.h"
#include "npc.h"

/*
 *	GetVehicleNum
 */
int GetVehicleNum( int itemNum )
{
	switch( itemNum )
	{
	case dVEHICLE_HORSE1:		return 1;
	case dVEHICLE_HORSE2:		return 2;
	case dVEHICLE_HORSE3:		return 3;
	case dVEHICLE_HORSE4:		return 4;
	case dVEHICLE_HORSE5:		return 5;	
	case dVEHICLE_HORSE6:		return 6;	
	case dVEHICLE_HORSE7:		return 7;	
	case dVEHICLE_HORSE8:		return 8;	
	case dVEHICLE_HORSE9:		return 9;	
	case dVEHICLE_HORSE10:		return 10;
	case dVEHICLE_HORSE11:		return 11;
	case dVEHICLE_HORSE12:		return 12;
	case dVEHICLE_HORSE13:		return 13;
	case dVEHICLE_HORSE14:		return 14;
	case dVEHICLE_HORSE15:		return 15;
	default:
		return 0;
	}

	return 0;
}


/*
 *	IsVehicle
 */
BOOL IsVehicle( int itemNum )
{
	if( itemNum >= 1184 && itemNum <= 1198 )
		return 1;
	else
		return 0;


	switch( itemNum )
	{
	case dVEHICLE_HORSE1:		
	case dVEHICLE_HORSE2:		
	case dVEHICLE_HORSE3:		
	case dVEHICLE_HORSE4:	
		return 1;
	default:
		return 0;
	}
	return 0;
}
/*
 *	CheckVehicle 어떠한 말을 타고 있는지
 */
int CheckVehicle( int itemNum )
{
		switch( itemNum )
	{
	case 1:		return dVEHICLE_HORSE1;
	case 2:		return dVEHICLE_HORSE2;
	case 3:		return dVEHICLE_HORSE3;
	case 4:		return dVEHICLE_HORSE4;
	case 5:		return dVEHICLE_HORSE5;	
	case 6:		return dVEHICLE_HORSE6;	
	case 7:		return dVEHICLE_HORSE7;	
	case 8:		return dVEHICLE_HORSE8;	
	case 9:		return dVEHICLE_HORSE9;	
	case 10:		return dVEHICLE_HORSE10;
	case 11:		return dVEHICLE_HORSE11;
	case 12:		return dVEHICLE_HORSE12;
	case 13:		return dVEHICLE_HORSE13;
	case 14:		return dVEHICLE_HORSE14;
	case 15:		return dVEHICLE_HORSE15;
	default:
		return 0;
	}

	return 0;
}



/*
 *	GetVehicleSpeed
 */
int GetVehicleSpeed( sPDESC_DATA pPlayer )
{
	int speed = 70;

	switch( GET_VEHICLE_NUM( pPlayer ) )
	{
	case 1:		// dVEHICLE_HORSE1			1184
	case 2:		// dVEHICLE_HORSE2			1185
		speed = 115;
		break;
	case 3:		// dVEHICLE_HORSE3			1186
		speed = 105;
		break;
	case 4:		// dVEHICLE_HORSE4			1187
		speed = 95;
		break;
	case 5:		// dVEHICLE_HORSE5			1188
	case 6:		// dVEHICLE_HORSE6			1189
		speed = 105;
		break;
	case 7:		// dVEHICLE_HORSE7			1190
		speed = 95;
		break;
	case 8:		// dVEHICLE_HORSE8			1191
		speed = 85;
		break;
	case 9:		// dVEHICLE_HORSE9			1192
	case 10:	// dVEHICLE_HORSE10			1193
		speed = 105;
		break;
	case 11:	// dVEHICLE_HORSE11			1194
		speed = 95;
		break;
	case 12:	// dVEHICLE_HORSE12			1195
		speed = 85;
		break;
	case 13:	// dVEHICLE_HORSE13			1196
	case 14:	// dVEHICLE_HORSE14			1197
	case 15:	// dVEHICLE_HORSE15			1198
		speed = 100;
		break;
	}

	if( GET_SKILL_POINT( pPlayer, dEFF_VALUE_ADDMOVESPEED ) )
		speed += (int)( speed * 0.5 );
	else if( GET_ITEM_POINT( pPlayer, dEFF_VALUE_ADDMOVESPEED ) )
		speed += (int)( speed * 0.5 );

	if( GET_SKILL_POINT( pPlayer, dEFF_VALUE_DELMOVESPEED ) )
		speed -= (int)( speed * 0.5 );
	else if( GET_ITEM_POINT( pPlayer, dEFF_VALUE_DELMOVESPEED ) )
		speed -= (int)( speed * 0.5 );

	return speed;
}

/*
 *	LoadAutoVehicle
 */
void LoadAutoVehicle( int npcNum )
{
	CLoadDataFile cLoad;
	char filename[128];
	sprintf( filename, "./DATA/NPC_VEHICLE/%d.TXT", npcNum );

	if( !cLoad.Load( filename ) )
		return;
	
	int			pointNum;
	char		name[64];
	int			price;

	sPNPC_DATA pNpc = FindNpcByNum( npcNum );

	if( !pNpc )
		return;

	while( 1 )
	{
		BYTE res = cLoad.GetLine();

		if( res == CLoadDataFile::IS_EOF )
			break;

		if( res == CLoadDataFile::IS_SKIP )
			continue;

		pointNum = cLoad.Get();
		cLoad.Get( name );
		price = cLoad.Get();

		sPNPC_VEHICLE pData = (sPNPC_VEHICLE)malloc(sizeof(sNPC_VEHICLE));

		if( !pData )
		{
			log( "LoadAutoVehicle : malloc failed\r\n" );
			continue;
		}

		pData->pointNum = pointNum;
		strcpy( pData->name, name );
		pData->price = price;
		pData->prev = NULL;
		pData->next = NULL;

		INSERT_TO_LIST( pNpc->vehicle, pData, prev, next );
	}
}

/*
 *	FindVehicle
 */
sPITEM_DATA FindVehicle( sPDESC_DATA pPlayer )
{
	sPITEM_DATA pEqVehicle = GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_BAG1 );

	if( pEqVehicle )
	{
		if( IsVehicle( pEqVehicle->itemNum ) )
			return pEqVehicle;
	}

	sPITEM_DATA item, next_item;

	int vehicleCnt = 0;
	sPITEM_DATA pVehicle = NULL;

	LIST_WHILE( pPlayer->ch.inven, item, next_item, inven_next );
	if( IsVehicle( item->itemNum ) )
	{
		vehicleCnt++;
		pVehicle = item;
		if( item->invenArray == 0 )
			return item;
	}
	LIST_WHILEEND();

	if( vehicleCnt == 1 && pVehicle )
		return pVehicle;

	return NULL;
}



//EOF


