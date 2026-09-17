#ifndef __VEHICLE_H__
#define __VEHICLE_H__

/*
 *	D E F I N ES
 */

/*struct vehicle_point
{
	BYTE		Type;

	int			posX;
	int			posY;
} sVEHICLE_POINT, *sPVEHICLE_POINT;
*/
#define dVEHICLE_HORSE1			1184		// 적토마 
#define dVEHICLE_HORSE2			1185		// 적노마
#define dVEHICLE_HORSE3			1186		// 대완마
#define dVEHICLE_HORSE4			1187		// 과하마
#define dVEHICLE_HORSE5			1188		
#define dVEHICLE_HORSE6			1189	
#define dVEHICLE_HORSE7			1190
#define dVEHICLE_HORSE8			1191
#define dVEHICLE_HORSE9			1192
#define dVEHICLE_HORSE10		1193
#define dVEHICLE_HORSE11		1194
#define dVEHICLE_HORSE12		1195
#define dVEHICLE_HORSE13		1196	
#define dVEHICLE_HORSE14		1197
#define dVEHICLE_HORSE15		1198

#define dNPC_VEHICLE1			21			// 마차

#define dNPC_VEHICLE_SPEED1		200

#define dNORMAL_PLAYER_SPEED	70

/*
 *	F U N C T I O N S
 */

int GetVehicleNum( int itemNum );
BOOL IsVehicle( int itemNum );
int GetVehicleSpeed( sPDESC_DATA pPlayer );
sPITEM_DATA FindVehicle( sPDESC_DATA pPlayer );

// 12월 27일 추가 
int CheckVehicle( int itemNum );




/*
 *	V A R I A B L E S
 */


#endif

//EOF

