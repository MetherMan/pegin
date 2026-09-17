#ifndef __DATA_H__
#define __DATA_H__

extern  SOCKET		g_DataSock;

/*
 *	D E F I N E S
 */
#define SEND_TO_DATA( data, size ) (send( g_DataSock, data, size, 0 ))

#define dPACKET_RESERVED						0

#define dDATA_LOGIN_USER						1	// 로그인
#define dDATA_LOGOUT_USER						2	// 로그아웃
#define dDATA_UPDATE_LEVEL						3	// 레벨 
#define dDATA_UPDATE_EXP						4	// 경험치
#define dDATA_UPDATE_CLASS						5	// 직업 
#define dDATA_UPDATE_HP						6	// HP
#define dDATA_UPDATE_MP						7	// MP
#define dDATA_UPDATE_MONEY						8	// 돈
#define dDATA_UPDATE_MAPINFO					9	// 맵정보 
#define dDATA_UPDATE_PREMAPINFO					10	// 이전맵정보

#define dDATA_PACKET_MAX						10

/*
 *	E X T E R N   F U N C T I O N S 
 */
void DATA_SendLogin( sPDESC_DATA pPlayer );
void DATA_SendLogout( sPDESC_DATA pPlayer );
void DATA_SendUpdateLevel( sPDESC_DATA pPlayer );
void DATA_SendUpdateExp( sPDESC_DATA pPlayer );
void DATA_SendUpdateClass( sPDESC_DATA pPlayer );
void DATA_SendUpdateHP( sPDESC_DATA pPlayer );
void DATA_SendUpdateMP( sPDESC_DATA pPlayer );
void DATA_SendUpdateMoney( sPDESC_DATA pPlayer );
void DATA_SendUpdateMapInfo( sPDESC_DATA pPlayer );
void DATA_SendUpdatePreMapInfo( sPDESC_DATA pPlayer );
BOOL InitDataSock();
void DestroyDataSock();

BOOL RecvFromDataServer();

#endif

//EOF


