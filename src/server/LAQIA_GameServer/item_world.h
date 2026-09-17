#ifndef __ITEM_WORLD_H__
#define __ITEM_WORLD_H__

void MakeItemInPacket( sPITEM_DATA pItem );
void MakeItemOutPacket( sPITEM_DATA pItem );
void SendToItemArea( sPITEM_DATA pItem, char *data, int size );
void GetItemSector( sPDESC_DATA pPlayer, int x, int y );
void GetItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void GetItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void OutItemSector( sPDESC_DATA pPlayer, int x, int y );
void OutItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer );
void OutItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer );
void GetCurrAreaItem( sPDESC_DATA pPlayer );
void OutCurrAreaItem( sPDESC_DATA pPlayer );
void CheckItemLifeInMap();

#endif

//EOF

