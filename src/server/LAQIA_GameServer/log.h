#ifndef __LOG_H__
#define __LOG_H__

#define dITEMLOG_MAP			1			// 맵
#define dITEMLOG_SHOP			2			// 상점 
#define dITEMLOG_DEAL			3			// 개인거래 
#define dITEMLOG_MYSHOP			4			// 개인상점 
#define dITEMLOG_WAREHOUSE		5			// 창고
#define dITEMLOG_ENCHANT		6			// 인첸트

#define dLOGFLAG_OUT			0			
#define dLOGFLAG_IN				1

#define dLOGINLOG_LOGOUT		0
#define dLOGINLOG_LOGIN			1


void LOG_ItemLog( sPITEM_DATA pItem, sPDESC_DATA pOwner, sPDESC_DATA pTarget, BYTE action, BYTE actionFlag );
void LOG_LevelLog( sPDESC_DATA pPlayer, char *name, int level );
void LOG_EnchantLog( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void LOG_BankLog( sPDESC_DATA pPlayer, int action, int amount );
void LOG_LoginLog( sPDESC_DATA pPlayer, int action );

void EVENTLOG_Start( sPDESC_DATA pPlayer, char *name, BYTE charPos, BYTE charClass );
void EVENTLOG_End( sPDESC_DATA pPlayer );
void EVENTLOG_Delete( char *id, BYTE charPos );

void LOG_CreateLog( char *id, BYTE charPos, char *name, BYTE action );

void NOTICE_CheckNotice();
void RESQUE_CheckResque();

#endif


//EOF


