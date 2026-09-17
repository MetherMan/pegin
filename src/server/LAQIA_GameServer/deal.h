#ifndef __DEAL_H__
#define __DEAL_H__

/*
 *	E X T E R N  F U N C T I O N S
 */
void DEAL_Init( sPDESC_DATA pPlayer );
void DEAL_Cancel( sPDESC_DATA pPlayer );
void DEAL_Finish( sPDESC_DATA pPlayer );
void DEAL_Clear( sPDESC_DATA pPlayer );

BOOL CheckInvalidDealItem( sPDESC_DATA pPlayer );
sPITEM_DATA FindItemFromDeal( sPDESC_DATA pPlayer, int idxNum );

void MYSHOP_Init( sPDESC_DATA pPlayer );
void MYSHOP_Close( sPDESC_DATA pPlayer );
void MYSHOP_SendItemList( sPDESC_DATA pPlayer, sPDESC_DATA pShopKeeper );

#endif

//EOF


