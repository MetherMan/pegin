#ifndef __PLAYER_WORLD_H__
#define __PLAYER_WORLD_H__


/*
 *	E X T E R N   F U N C T I O N S
 */
void MakeInPacket( sPDESC_DATA pPlayer );
void MakeOutPacket( sPDESC_DATA pPlayer );
void SendToSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, const char *data, int size , BYTE mode );
void SendToVSector( int x, int startY, int endY, sPDESC_DATA pPlayer, const char *data, int size );
void SendToHSector( int y, int startX, int endX, sPDESC_DATA pPlayer, const char *data, int size );
void SendToArea( sPDESC_DATA pPlayer , const char *data, int size, BYTE mode );
void GetPlayerSector( sPDESC_DATA pPlayer, int x, int y );
void GetCurrAreaPlayer( sPDESC_DATA pPlayer );
void OutPlayerSector( sPDESC_DATA pPlayer, int x, int y );
void OutCurrAreaPlayer( sPDESC_DATA pPlayer );
void GetPlayerHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void GetPlayerVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void OutPlayerHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void OutPlayerVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void PlayerFromMap( sPDESC_DATA pPlayer );
void PlayerIntoMap( sPDESC_DATA pPlayer , int mapNum , int posX, int posY, int posZ );
BOOL MovePlayer( sPDESC_DATA pPlayer , int posX, int posY , int posZ );
void SendMapLoading( sPDESC_DATA pPlayer );
sPDESC_DATA FindPlayerInArea( int mapNum, int posX, int posY, int idxNum );
BOOL IsValidAttackArea( sPDESC_DATA pPlayer, sPMOB_DATA pMob );

void SendToRangeSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, const char *data, int size , int range,  BYTE mode );
void SendToRange( sPDESC_DATA pPlayer , const char *data, int size, int range, BYTE mode );

void PlayerToHometown( sPDESC_DATA pPlayer );


//////////////////////////////////////////////////////////////////////////

void SendToShoutChat(sPDESC_DATA pPlayer, const char *data, int size, int sector);
void SendToShoutChat(const char* data, int size);
#endif

//EOF

