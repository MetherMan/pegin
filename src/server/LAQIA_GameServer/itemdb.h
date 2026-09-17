/**
 *	$Workfile: itemdb.h $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 14 $
 *	$Date: 04-11-18 1:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/itemdb.h $
 * 
 * 14    04-11-18 1:40a Sk8snow
 * 
 * 13    04-09-08 3:54p Sk8snow
 * 
 * 12    04-07-13 7:26a Sk8snow
 * 
 * 11    04-05-29 10:03p Sk8snow
 * 
 * 10    04-05-18 1:17a Sk8snow
 * 
 * 9     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __ITEM_DB_H__
#define __ITEM_DB_H__

// 아이템 로그용 
enum
{
	ITEMLOG_ACT_DROP = 1,				// 1.맵에 버린다
	ITEMLOG_ACT_GET,					// 2.맵에서 줍는다 
	ITEMLOG_ACT_DEALGIVE,				// 3.거래로 넘겨준 아이템 
	ITEMLOG_ACT_DEALTAKE,				// 4.거래로 받은 아이템
	ITEMLOG_ACT_DEADDROP,				// 5.죽어서 떨군 아이템
	ITEMLOG_ACT_SELLITEM,				// 6.아이템 팔기
	ITEMLOG_ACT_INCHENT_REMOVE,			// 7.인첸트에 의해 사라짐
	ITEMLOG_ACT_INCHENT_CREATE,			// 8.인첸트에 의해 들어감
	ITEMLOG_ACT_BUYITEM,				// 9.아이템 사기
	ITEMLOG_ACT_MYSHOP_REMOVE,			// 10. 개인상점에 의해 팔림
	ITEMLOG_ACT_MYSHOP_CREATE,			// 11. 개인상점에서 구입
	ITEMLOG_ACT_BILLITEM_IN,			// 12. 유료 아이템 생성
	ITEMLOG_ACT_BILLITEM_OUT,			// 13. 유료 아이템 소멸
	ITEMLOG_ACT_ENCHANT_CHANGE1,		// 14. 인첸트 체인지(소멸)
	ITEMLOG_ACT_ENCHANG_CHANGE2,		// 15. 인첸트 체인지(생성)
	ITEMLOG_ACT_ENCHANT_COLLECT1,		// 16. 인첸트 콜렉션(소멸)
	ITEMLOG_ACT_ENCHANT_COLLECT2,		// 17. 인첸트 콜렉션(생성)
	ITEMLOG_ACT_EVENTINPUT,				// 18. 이벤트 지급
};

// 창고용
enum
{
	GARBAGE_ACT_PUT = 1,				// 보관 
	GARBAGE_ACT_GET,					// 찾기	
	GARBAGE_ACT_GODPUT,					// 운영자가 넣음
};

// 돈 
enum
{
	MONEYLOG_ACT_IN = 1,				// << IN
	MONEYLOG_ACT_OUT,					// << OUT
	MONEYLOG_ACT_DEAL_IN,
	MONEYLOG_ACT_DEAL_OUT,		
};

enum
{
	GARBAGEMONEY_LOG_ACT_IN = 1,
	GARBAGEMONEY_LOG_ACT_OUT,
};

// 유료아이템용
enum
{
	BILLITEM_LOG_ACT_START	= 1,
	BILLITEM_LOG_ACT_END,
};


void ITEMDB_DeleteAllEquip( sPDESC_DATA pPlayer );
void ITEMDB_DeleteAllInven( sPDESC_DATA pPlayer );
void ITEMDB_DelEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ITEMDB_DelInven( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ITEMDB_ToInven( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ITEMDB_ToInven2( sPDESC_DATA pPlayer, int itemNum, sPCHECK_INVEN inven );
void ITEMDB_ToEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ITEMDB_ToEquip2( char *id, BYTE charPos, WORD itemNum , BYTE equipWhere );
void ITEMDB_UpdateInvenItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

void ITEMDB_DelAllSkills( sPDESC_DATA pPlayer );

BOOL ITEMDB_Query( char *query );

BOOL ITEMDB_GetPlayerItem( sPDESC_DATA pPlayer );

void ITEMDB_ToGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem );
void ITEMDB_DelGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem );

void ITEMDB_DeleteAllItem( int itemNum );

// LOG
void ITEMLOG_ItemLog( sPDESC_DATA pPlayer, sPDESC_DATA pActionPlayer, sPITEM_DATA pItem , BYTE action );
void ITEMLOG_GarbageLog( sPDESC_DATA pPlayer, sPITEM_DATA pItem, BYTE action );
void ITEMLOG_MoneyLog( sPDESC_DATA pPlayer, sPDESC_DATA pActionPlayer, int amount, BYTE action );

#ifdef dUSE_LOGIN_LOG
void LOGINLOG_Write( sPDESC_DATA pPlayer );
#endif

void ITEMLOG_MyShopLog( sPDESC_DATA pPlayer, sPDESC_DATA pActionPlayer, sPITEM_DATA pItem, int price );

void ITEMDB_UpdateGarbageMoney( sPDESC_DATA pPlayer );
void ITEMDB_UpdateCashMoney( sPDESC_DATA pPlayer );
void ITEMLOG_GarbageMoneyLog( sPDESC_DATA pPlayer, int amount, BYTE action );
void ITEMLOG_BillItemLog( sPDESC_DATA pPlayer, int itemNum, BYTE type );;

#endif


//



