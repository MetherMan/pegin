
#ifndef __MOB_WORLD_H__
#define __MOB_WORLD_H__

/*
 *	E X T E R N   F U N C T I O N S
 */
void MakeMobInPacket( sPMOB_DATA pMob );
void MakeMobOutPacket( sPMOB_DATA pMob );
void GetMobSector( sPDESC_DATA pPlayer, int x, int y );
void GetMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void GetMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void OutMobSector( sPDESC_DATA pPlayer, int x, int y );
void OutMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void OutMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void SendToMobVSector( int x, int startY, int endY, sPMOB_DATA pMob, char *data, int size );
void SendToMobHSector( int y, int startX, int endX, sPMOB_DATA pMob, char *data, int size );
void SendToMobArea( sPMOB_DATA pMob, char *data, int size );
void GetCurrAreaMob( sPDESC_DATA pPlayer );
void OutCurrAreaMob( sPDESC_DATA pPlayer );
void CheckMobSectorEnemy( sPMOB_DATA pMob, int x, int y );
void CheckMobEnemyArea( sPMOB_DATA pMob );
void SendToMobRange( sPMOB_DATA pMob, char *data, int size, int range );

void MoveMonsterProc();
int IsMovable( sPMOB_DATA pMob, int destX, int destY );
void MoveMonster( sPMOB_DATA pMob, int posX, int posY );

sPMOB_DATA FindMobInTile( sPMOB_DATA list, int mobNum );
sPMOB_DATA FindMobInArea( int mapNum, int posX, int posY, int mobNum );

void MonsterStop( sPMOB_DATA pMob );
void GetAttackPosition( sPDESC_DATA pPlayer, sPMOB_DATA pMob, int &aX, int &aY );

#endif

//EOF


