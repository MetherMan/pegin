#ifndef __WAREHOUSE_H__
#define __WAREHOUSE_H__

/*
 *	D E F I N E S
 */
#define dMAX_WAREHOUSE_ITEM			80


/*
 *	E X T E R N   F U N C T I O N S
 */
void WAREHOUSE_LoadData( sPDESC_DATA pPlayer );
BOOL ItemToWarehouseDB( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
BOOL ItemFromWareHouseDB( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
sPITEM_DATA FindItemFromWarehouse( sPDESC_DATA pPlayer, int idxNum );
void ItemToWarehouse( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ItemFromWarehouse( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void WAREHOUSE_UpdateMoney( sPDESC_DATA pPlayer );
int CountWarehouseItem( sPDESC_DATA pPlayer );

#endif


//EOF


