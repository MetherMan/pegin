/**
 *	$Workfile: billing.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 9 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/billing.cpp $
 * 
 *10	 05-02-12 10:45a umenokoji	알빙고 관련 상품코드 추가
 *  
 * 9     04-12-15 10:40a Sk8snow
 * 
 * 8     04-11-18 1:40a Sk8snow
 * 
 * 7     04-10-19 11:14p Sk8snow
 * 
 * 6     04-09-08 3:54p Sk8snow
 * 
 * 5     04-07-13 7:26a Sk8snow
 * 
 * 4     04-03-31 8:22a Sk8snow
 *        
 */
#include "def.h"
#include "structs.h"
#include "extern.h"
#include "message.h"

#include "player.h"

#include "admin.h"
#include "db_rows.h"

#include "item.h"
#include "itemdb.h"

#include "billing.h"


MYSQL *g_BILL_SQL = NULL;
#ifdef __CHINA_VER__
sBILL_INFO g_BILL_INFO;
#endif
sDB_INFO g_BILL_DBINFO;

sBILLING_TABLE g_BILLING_TABLE[dMAX_BILLING_ITEM_INFO];

BOOL LoadBillingTable()
{
	for( int i = 0; i < dMAX_BILLING_ITEM_INFO; i++ )
	{
		g_BILLING_TABLE[i].itemNum = 0;
		g_BILLING_TABLE[i].privateCode = 0;
		g_BILLING_TABLE[i].pcbangCode = 0;
		g_BILLING_TABLE[i].cnt = 0;
	}

	int idx;
	int itemNum;
	int privateCode;
	int pcbangCode;
	int cnt;
	
	FILE *fp = fopen( dBILLING_TABLE_FILE, "rb" );

	if( !fp )
		return 0;
	
	char readBuff[1024];
	char tmp[128];

	CWhileMgr WMgr;
		
	while( !feof( fp ) )
	{
		WMgr.CheckDump( __FILE__, __LINE__ );
		
		int nPos = 0;
		
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '#' )
			break;

		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		idx = GetTabNumberic( readBuff, nPos );
		itemNum = GetTabNumberic( readBuff, nPos );
		privateCode = GetTabNumberic( readBuff, nPos );
		pcbangCode = GetTabNumberic( readBuff, nPos );
		cnt = GetTabNumberic( readBuff, nPos );
		GetTabString( readBuff, tmp, nPos );

		if( idx >= dMAX_BILLING_ITEM_INFO )
		{
			log( "LoadBillingTable : idx >= dMAX_BILLING_ITEM_INFO\r\n" );
			fclose( fp );
			return 0;
		}

		if( !IS_VALID_ITEM( itemNum ) )
		{
			log( "LoadBillingTable : Invalid Item Number %d\r\n", itemNum );
			return 0;
		}

		g_BILLING_TABLE[idx].itemNum = itemNum;
		g_BILLING_TABLE[idx].privateCode = privateCode;
		g_BILLING_TABLE[idx].pcbangCode = pcbangCode;
		g_BILLING_TABLE[idx].cnt = cnt;
	}

	fclose( fp );
	return 1;
}

void SaveUserBillEffect( sPDESC_DATA pPlayer )
{
	if( !pPlayer->ch2.haveBillRow )
		return;

	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	int updateCnt = 0;

	for( int i = 0; i < dMAX_BILLING_FLAG; i++ )
	{
		if( pPlayer->ch2.billingFlag[i] )
			updateCnt++;
	}

	if( updateCnt )
	{
		sprintf( query, "update BillItemInfo set "
			"effect1 = %ld,"
			"effect2 = %ld,"
			"effect3 = %ld,"
			"effect4 = %ld,"
			"effect5 = %ld,"
			"effect6 = %ld,"
			"effect7 = %ld,"
			"effect8 = %ld,"
			"effect9 = %ld,"
			"effect10 = %ld,"
			"effect11 = %ld,"
			"effect12 = %ld,"
			"effect13 = %ld,"
			"effect14 = %ld,"
			"effect15 = %ld,"
			"effect16 = %ld,"
			"effect17 = %ld,"
			"effect18 = %ld,"
			"effect19 = %ld,"
			"effect20 = %ld "
			"where ownerID = '%s' and ownerPos = %d limit 1",
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16],
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17],
			(DWORD)0,
			(DWORD)0,
			(DWORD)0,
			escID,
			GET_CHAR_POS( pPlayer ) );
		
		if( mysql_query( g_MYSQL, query ) )
			log( "%s",mysql_error( g_MYSQL ) );
	}
}

//
// Function Name: LoadPlayerBillingInfo
// Date:
// Description: 
//
void LoadPlayerBillingInfo( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	sprintf( query, "select * from BillItemInfo where ownerID = '%s' and ownerPos = %d limit 1", escID, GET_CHAR_POS( pPlayer ) );
	
	if( mysql_query( g_MYSQL, query ) )
	{
		log( "%s",mysql_error( g_MYSQL ) );
		return;
	}
	else
	{
		MYSQL_ROW row;
		MYSQL_RES *result;
		
		result = mysql_store_result( g_MYSQL );
		
		if( result )
		{			
			if( ( row = mysql_fetch_row( result ) ) )
			{
				pPlayer->ch2.haveBillRow = 1;
				
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] = atol( row[BILLITEM_EFFECT1] );		//경험치 1.3배		
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] = atol( row[BILLITEM_EFFECT2] );		//경험치 1.5배	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] = atol( row[BILLITEM_EFFECT3] );		//사냥시 아이템 드랍률 1.3배	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] = atol( row[BILLITEM_EFFECT4] );		//사냥시 아이템 드랍률 1.5배	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] = atol( row[BILLITEM_EFFECT5] );		//프리미엄 존에서 사냥가능	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = atol( row[BILLITEM_EFFECT6] );		//프리미엄 존 플러스
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] = atol( row[BILLITEM_EFFECT7] );		//인챈트 카드의 드랍률 증가	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] = atol( row[BILLITEM_EFFECT8] );		//이동속도 증가.	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] = atol( row[BILLITEM_EFFECT9] );		//무게에 대한 힘 증가 ( 20% , 1시간 )	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] = atol( row[BILLITEM_EFFECT10] );		//1시간동안 HP량 증가	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] = atol( row[BILLITEM_EFFECT11] );		//1시간 동안 MP 증가	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] = atol( row[BILLITEM_EFFECT12] );		//1시간 동안 파티원의 경험치가 1.3배 증가	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] = atol( row[BILLITEM_EFFECT13] );		//1시간동안 파티원 HP 20% 증가	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] = atol( row[BILLITEM_EFFECT14] );		//1시간동안 파티원 MP 20% 증가	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] = atol( row[BILLITEM_EFFECT15] );		//1시간동안 군단원 HP 20% 증가	
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] = atol( row[BILLITEM_EFFECT16] );		//1시간동안 군단원 MP 20% 증가
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] = atol( row[BILLITEM_EFFECT17] );		// 1시간동안 성향치 습득 2배
				
				// HP 드링크
				if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] )
				{
					pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT10] = (int)( GET_MAX_HP( pPlayer ) * 0.2 );
					GET_HPADD( pPlayer ) += (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT10];
					
					SendPlayerMaxHPMP( pPlayer );
				}
				
				if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] && 
					pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
				{
					pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT15] = (int)( GET_MAX_HP( pPlayer ) * 0.2 );
					GET_HPADD( pPlayer ) += (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT15];
					SendPlayerMaxHPMP( pPlayer );
				}
				
				if( GET_HP( pPlayer ) > GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) )
				{
					GET_HP( pPlayer ) = GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer );
					SendPlayerHP( pPlayer );
				}
				
				
				// MP 드링크
				if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] )
				{
					pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT11] = (int)( GET_MAX_MP( pPlayer ) * 0.2 );
					GET_MPADD( pPlayer ) += (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT11];
					SendPlayerMaxHPMP( pPlayer );
				}
				
				if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] && 
					pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
				{
					pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT16] = (int)( GET_MAX_MP( pPlayer ) * 0.2 );
					GET_MPADD( pPlayer ) += (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT16];
					SendPlayerMaxHPMP( pPlayer );
				}
				
				if( GET_MP( pPlayer ) > GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) )
				{
					GET_MP( pPlayer ) = GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer );
					SendPlayerMP( pPlayer );
				}
			}
			else
			{
				pPlayer->ch2.haveBillRow = 0;
			}
			mysql_free_result( result );
		}
	}
}

//
// Function Name: UpdateBillItemInfo
// Date:
// Description: 
//
void UpdateBillItemInfo( sPDESC_DATA pPlayer, char *rowName, DWORD value )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	if( !pPlayer->ch2.haveBillRow )
	{
		sprintf( query, "insert into BillItemInfo( idx, ownerID, ownerPos ) values( '', '%s', %d )", escID, GET_CHAR_POS( pPlayer ) );

		if( mysql_query( g_MYSQL, query ) )
		{
			mysql_error( g_MYSQL );
			return;
		}
		pPlayer->ch2.haveBillRow = 1;
	}

	sprintf( query, "update BillItemInfo set %s = %ld where ownerID = '%s' and ownerPos = %d limit 1", rowName, value, escID, GET_CHAR_POS( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
	{
		mysql_error( g_MYSQL );
		return;
	}
}

//
// Function Name: DeleteBillingItemInfo
// Date:
// Description: 
//
void DeleteBillingItemInfo( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "delete from BillItemInfo where ownerID = '%s' and ownerPos = %d limit 1", escID, GET_CHAR_POS( pPlayer ) );

	if( mysql_query( g_MYSQL, query ) )
		log( "%s",mysql_error( g_MYSQL ) );
}

//
// Function Name: IsBillingItem
// Date:
// Description: 
//
BOOL IsBillingItem( int itemNum )
{
	switch( itemNum )
	{
	case dBILL_ITEM_NUM1:		//경험치 1.3배	
	case dBILL_ITEM_NUM2:		//경험치 1.3배		
	case dBILL_ITEM_NUM3:		//경험치 1.3배	
	case dBILL_ITEM_NUM4:		//경험치 1.5배	
	case dBILL_ITEM_NUM5:		//경험치 1.5배	
	case dBILL_ITEM_NUM6:		//경험치 1.5배	
	case dBILL_ITEM_NUM7:		//사망시 경험치 손실없음	
	case dBILL_ITEM_NUM8:		//사망시 경험치 손실없음	
	case dBILL_ITEM_NUM9:		//사망시 경험치 손실없음	
	case dBILL_ITEM_NUM10:		//사망시 아이템 유실 없음	
	case dBILL_ITEM_NUM11:		//사망시 아이템 유실 없음	
	case dBILL_ITEM_NUM12:		//사망시 아이템 유실 없음	
	case dBILL_ITEM_NUM13:		//맵 전체 외침	
	case dBILL_ITEM_NUM14:		//맵 전체 외침	
	case dBILL_ITEM_NUM15:		//맵 전체 외침	
	case dBILL_ITEM_NUM16:		//사냥시 아이템 드랍률 1.3배	
	case dBILL_ITEM_NUM17:		//사냥시 아이템 드랍률 1.3배	
	case dBILL_ITEM_NUM18:		//사냥시 아이템 드랍률 1.3배	
	case dBILL_ITEM_NUM19:		//사냥시 아이템 드랍률 1.5배	
	case dBILL_ITEM_NUM20:		//사냥시 아이템 드랍률 1.5배	
	case dBILL_ITEM_NUM21:		//사냥시 아이템 드랍률 1.5배	
	case dBILL_ITEM_NUM22:		//사망시 부활	
	case dBILL_ITEM_NUM23:		//사망시 부활	
	case dBILL_ITEM_NUM24:		//사망시 부활	
	case dBILL_ITEM_NUM25:		//길드공지	
	case dBILL_ITEM_NUM26:		//길드공지	
	case dBILL_ITEM_NUM27:		//길드공지	
	case dBILL_ITEM_NUM28:		//프리미엄 존에서 사냥가능	
	case dBILL_ITEM_NUM29:		//프리미엄 존에서 사냥가능	
	case dBILL_ITEM_NUM30:		//프리미엄 존에서 사냥가능	
	case dBILL_ITEM_NUM31:		//프리미엄 존 플러스, 경험치와 아이템 드랍률 2배	
	case dBILL_ITEM_NUM32:		//프리미엄 존 플러스, 경험치와 아이템 드랍률 2배	
	case dBILL_ITEM_NUM33:		//프리미엄 존 플러스, 경험치와 아이템 드랍률 2배	
	case dBILL_ITEM_NUM34:		//팻 먹이 
	case dBILL_ITEM_NUM35:		//팻 먹이 
	case dBILL_ITEM_NUM36:		//팻 먹이 
	case dBILL_ITEM_NUM37:		//인챈트 카드의 드랍률 증가
	case dBILL_ITEM_NUM38:		//인챈트 카드의 드랍률 증가
	case dBILL_ITEM_NUM39:		//인챈트 카드의 드랍률 증가
	case dBILL_ITEM_NUM40:		//인첸트 카드 변환 ( 아머 -> 웨폰 , 웨폰 -> 아머 )
	case dBILL_ITEM_NUM41:		//인첸트 카드 변환 ( 아머 -> 웨폰 , 웨폰 -> 아머 )
	case dBILL_ITEM_NUM42:		//인첸트 카드 변환 ( 아머 -> 웨폰 , 웨폰 -> 아머 )
	case dBILL_ITEM_NUM43:		//인첸트된 아이템을 인첸트 카드로 환원
	case dBILL_ITEM_NUM44:		//인첸트된 아이템을 인첸트 카드로 환원
	case dBILL_ITEM_NUM45:		//인첸트된 아이템을 인첸트 카드로 환원
	case dBILL_ITEM_NUM46:		//상대방 유져의 아이템정보를 훔쳐본다
	case dBILL_ITEM_NUM47:		//상대방 유져의 아이템정보를 훔쳐본다
	case dBILL_ITEM_NUM48:		//상대방 유져의 아이템정보를 훔쳐본다
	case dBILL_ITEM_NUM49:		//아이템룩을 사용하지 못하게 한다
	case dBILL_ITEM_NUM50:		//아이템룩을 사용하지 못하게 한다
	case dBILL_ITEM_NUM51:		//아이템룩을 사용하지 못하게 한다
	case dBILL_ITEM_NUM52:		//상대의 능력치를 볼 수 있다.
	case dBILL_ITEM_NUM53:		//상대의 능력치를 볼 수 있다.
	case dBILL_ITEM_NUM54:		//상대의 능력치를 볼 수 있다.
	case dBILL_ITEM_NUM55:		//레벨룩을 사용하지 못하게 한다.
	case dBILL_ITEM_NUM56:		//레벨룩을 사용하지 못하게 한다.
	case dBILL_ITEM_NUM57:		//레벨룩을 사용하지 못하게 한다.
	case dBILL_ITEM_NUM58:		//이동속도 증가.
	case dBILL_ITEM_NUM59:		//이동속도 증가.
	case dBILL_ITEM_NUM60:		//이동속도 증가.
	case dBILL_ITEM_NUM61:		//무게에 대한 힘 증가 ( 20% , 1시간 )
	case dBILL_ITEM_NUM62:		//무게에 대한 힘 증가 ( 20% , 1시간 )
	case dBILL_ITEM_NUM63:		//무게에 대한 힘 증가 ( 20% , 1시간 )
	case dBILL_ITEM_NUM64:		//1시간동안 HP량 20% 증가
	case dBILL_ITEM_NUM65:		//1시간동안 HP량 20% 증가
	case dBILL_ITEM_NUM66:		//1시간동안 HP량 20% 증가
	case dBILL_ITEM_NUM67:		//1시간 동안 MP 20% 증가
	case dBILL_ITEM_NUM68:		//1시간 동안 MP 20% 증가
	case dBILL_ITEM_NUM69:		//1시간 동안 MP 20% 증가
	case dBILL_ITEM_NUM70:		//1시간 동안 파티원의 경험치가 1.3배 증가
	case dBILL_ITEM_NUM71:		//1시간 동안 파티원의 경험치가 1.3배 증가
	case dBILL_ITEM_NUM72:		//1시간 동안 파티원의 경험치가 1.3배 증가
	case dBILL_ITEM_NUM73:		//1시간동안 파티원 HP 20% 증가
	case dBILL_ITEM_NUM74:		//1시간동안 파티원 HP 20% 증가
	case dBILL_ITEM_NUM75:		//1시간동안 파티원 HP 20% 증가
	case dBILL_ITEM_NUM76:		//1시간동안 파티원 MP 20% 증가
	case dBILL_ITEM_NUM77:		//1시간동안 파티원 MP 20% 증가
	case dBILL_ITEM_NUM78:		//1시간동안 파티원 MP 20% 증가
	case dBILL_ITEM_NUM79:		//1시간동안 군단원 HP 20% 증가
	case dBILL_ITEM_NUM80:		//1시간동안 군단원 HP 20% 증가
	case dBILL_ITEM_NUM81:		//1시간동안 군단원 HP 20% 증가
	case dBILL_ITEM_NUM82:		//1시간동안 군단원 MP 20% 증가
	case dBILL_ITEM_NUM83:		//1시간동안 군단원 MP 20% 증가
	case dBILL_ITEM_NUM84:		//1시간동안 군단원 MP 20% 증가
	case dBILL_ITEM_NUM85:		// 웜쓰오브 아라임 1.3배 ( 1달 )
	case dBILL_ITEM_NUM86:		// 웜쓰오브 아라임 1.5배 ( 1달 )
	case dBILL_ITEM_NUM87:		// 아이템 오브 아라임 1.3배 ( 1달 )
	case dBILL_ITEM_NUM88:		// 아이템 오브 아라임 1.5배 ( 1달 )
	case dBILL_ITEM_NUM89:		// 프리미엄 존 서비스 ( 1달 )
	case dBILL_ITEM_NUM90:		// 프리미엄 존 플러스 ( 1달 )
	case dBILL_ITEM_NUM91:		// 신의 축복 ( 1달 )
	case dBILL_ITEM_NUM92:		// 단원소환 
	case dBILL_ITEM_NUM93:		// 단원소환 
	case dBILL_ITEM_NUM94:		// 단원소환 
	case dBILL_ITEM_NUM95:		// 단원필승의 외침
	case dBILL_ITEM_NUM96:		// 단원필승의 외침
	case dBILL_ITEM_NUM97:		// 단원필승의 외침
	case dBILL_ITEM_NUM98:		// 단원 축복의 외침
	case dBILL_ITEM_NUM99:		// 단원 축복의 외침
	case dBILL_ITEM_NUM100:		// 단원 축복의 외침
	case dBILL_ITEM_NUM101:		// 단원 기원의 외침
	case dBILL_ITEM_NUM102:		// 단원 기원의 외침
	case dBILL_ITEM_NUM103:		// 단원 기원의 외침
	case dBILL_ITEM_NUM104:		// 단원 저주의 외침
	case dBILL_ITEM_NUM105:		// 단원 저주의 외침
	case dBILL_ITEM_NUM106:		// 단원 저주의 외침
	case dBILL_ITEM_NUM107:		// 단원 도발의 외침
	case dBILL_ITEM_NUM108:		// 단원 도발의 외침
	case dBILL_ITEM_NUM109:		// 단원 도발의 외침
	case dBILL_ITEM_NUM110:		// 마이너스
	case dBILL_ITEM_NUM111:		// 마이너스
	case dBILL_ITEM_NUM112:		// 마이너스
	case dBILL_ITEM_NUM113:		// 하트 이모티콘
	case dBILL_ITEM_NUM114:		// 하트 이모티콘
	case dBILL_ITEM_NUM115:		// 하트 이모티콘
	case dBILL_ITEM_NUM116:		// 기쁨 이모티콘
	case dBILL_ITEM_NUM117:		// 기쁨 이모티콘
	case dBILL_ITEM_NUM118:		// 기쁨 이모티콘
	case dBILL_ITEM_NUM119:		// 슬픔 이모티콘
	case dBILL_ITEM_NUM120:		// 슬픔 이모티콘
	case dBILL_ITEM_NUM121:		// 슬픔 이모티콘
	case dBILL_ITEM_NUM122:		// 분노 이모티콘
	case dBILL_ITEM_NUM123:		// 분노 이모티콘
	case dBILL_ITEM_NUM124:		// 분노 이모티콘
	case dBILL_ITEM_NUM125:		// 웨폰 프로텍트
	case dBILL_ITEM_NUM126:		// 아머 프로텍트
	case dBILL_ITEM_NUM127:		// 
	case dBILL_ITEM_NUM128:		// 
	case dBILL_ITEM_NUM129:		// 
	case dBILL_ITEM_NUM130:		// 
	case dBILL_ITEM_NUM131:		// 
	case dBILL_ITEM_NUM132:		// 
	case dBILL_ITEM_NUM133:		// 
	case dBILL_ITEM_NUM134:		// 
	case dBILL_ITEM_NUM135:		// 
	case dBILL_ITEM_NUM136:		// 
	case dBILL_ITEM_NUM137:		// 
	case dBILL_ITEM_NUM138:		// 
	case dBILL_ITEM_NUM139:		// 
	case dBILL_ITEM_NUM140:		// 
	case dBILL_ITEM_NUM141:		// 
	case dBILL_ITEM_NUM142:		// 
	case dBILL_ITEM_NUM143:		// 
	case dBILL_ITEM_NUM144:		// 
	case dBILL_ITEM_NUM145:		// 
	case dBILL_ITEM_NUM146:		// 
	case dBILL_ITEM_NUM147:		// 
	case dBILL_ITEM_NUM148:		// 
	case dBILL_ITEM_NUM149:		// 
		return 1;
	}
	return 0;
}

//
// Function Name: FindBillingItem
// Date:
// Description: 
//
sPBILLING_TABLE FindBillingItem( int PurCode )
{
	for( int i = 0; i < dMAX_BILLING_ITEM_INFO; i++ )
	{
		if( g_BILLING_TABLE[i].privateCode == PurCode ||
			g_BILLING_TABLE[i].pcbangCode == PurCode )
		{
			return &g_BILLING_TABLE[i];
		}
		if( g_BILLING_TABLE[i].itemNum == 0 )
			return NULL;
	}
	return NULL;
}

//
// Function Name: NewBillItem
// Date:
// Description: 
//
sPBILL_ITEM_DATA NewBillItem()
{
	sPBILL_ITEM_DATA pNewItem = NULL;

	if( gITEMS.billItem_Memory )
	{
		pNewItem = gITEMS.billItem_Memory;

		REMOVE_FROM_LIST( gITEMS.billItem_Memory, pNewItem, prev, next );
	}
	else
	{
		pNewItem = (sPBILL_ITEM_DATA)malloc(sizeof(sBILL_ITEM_DATA));

		if( !pNewItem )
			return NULL;
	}

	pNewItem->idxNum = 0;
	pNewItem->itemNum = 0;

	*pNewItem->PurKey = NULL_STR;
	pNewItem->ProductNum = 0;

	pNewItem->prev = NULL;
	pNewItem->next = NULL;

	return pNewItem;
}

//
// Function Name: FindBillItemByIdx
// Date:
// Description: 
//
sPBILL_ITEM_DATA FindBillItemByIdx( sPDESC_DATA pPlayer, int idxNum )
{
	sPBILL_ITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->billItems, item, next_item, next, WMgr );
	if( item->idxNum == idxNum )
		return item;
	LIST_WHILEEND( pPlayer->billItems, item, next_item );

	return NULL;
}

//
// Function Name: FindBillItemByNum
// Date:
// Description: 
//
sPBILL_ITEM_DATA FindBillItemByNum( sPDESC_DATA pPlayer, int itemNum )
{
	sPBILL_ITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->billItems, item, next_item, next, WMgr );
	if( item->itemNum == itemNum )
		return item;
	LIST_WHILEEND( pPlayer->billItems, item, next_item );

	return NULL;
}

//
// Function Name: LoadNonUsedBillItems
// Date:
// Description: 
//
void LoadNonUsedBillItems( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	sprintf( query, "select * from PurSell where UserUID = '%s' and PurFlag = 0", escID );
	
	if( mysql_query( g_BILL_SQL, query ) )
		log( "%s" ,mysql_error( g_BILL_SQL ) );
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;
		
		result = mysql_store_result( g_BILL_SQL );
		
		if( result )
		{
			while( ( row = mysql_fetch_row( result ) ) )
			{
				if( atoi( row[PURROW_PRODUCTNUM] ) < 0 )
				{
					switch( atoi( row[PURROW_PRODUCTNUM] ) )
					{
					// 웜쓰오브 아라임 1.3배 10시간
					// 웨이트 드링크 5개
					// HP드링크 5개
					// 아머 1장, 웨폰 1장
					case -100:
						if( CountEmptyInven( pPlayer ) >= 5 )
						{
							sPITEM_DATA pWomth = NULL;
							sPITEM_DATA pWeight = NULL;
							sPITEM_DATA pHPDrink = NULL;
							sPITEM_DATA pWeapon = NULL;
							sPITEM_DATA pArmor = NULL;
							
							pWomth = CreateItem( 11907, "", "", 0 );
							pWeight = CreateItem( 11970, "", "", 0 );
							pHPDrink = CreateItem( 11973, "", "", 0 );
							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
							pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
							
							sCHECK_INVEN checkInven;

							if( pWomth )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWomth, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWomth, ITEMLOG_ACT_EVENTINPUT );
							}

							if( pWeight )
							{
								pWeight->exVal[0] = 5;
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeight, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeight, ITEMLOG_ACT_EVENTINPUT );
							}

							if( pHPDrink )
							{
								pHPDrink->exVal[0] = 5;
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pHPDrink, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pHPDrink, ITEMLOG_ACT_EVENTINPUT );
							}

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}
							
							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}
							
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						break;
					case -1:		// 웨폰 1장 아머 1장
						if( CountEmptyInven( pPlayer ) >= 2 )
						{
							sPITEM_DATA pWeapon = NULL;
							sPITEM_DATA pArmor = NULL;

							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
							pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );

							sCHECK_INVEN checkInven;

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}
							
							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}
							
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						break;
					case -2:		// 웨폰 3장 아머 2장 
						if( CountEmptyInven( pPlayer ) >= 5 )
						{
							sPITEM_DATA pWeapon = NULL;
							sPITEM_DATA pArmor = NULL;

							sCHECK_INVEN checkInven;
							
							int i;

							for( i = 0; i < 3; i++ )
							{
								pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
								
								if( pWeapon )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pWeapon, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							
							for( i = 0; i < 2; i++ )
							{
								pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
																
								if( pArmor )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pArmor, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
								}
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						break;
					case -3:		// 웨폰 5장 아머 3장 
						if( CountEmptyInven( pPlayer ) >= 8 )
						{
							sPITEM_DATA pWeapon = NULL;
							sPITEM_DATA pArmor = NULL;

							sCHECK_INVEN checkInven;
							
							int i;

							for( i = 0; i < 5; i++ )
							{
								pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
								
								if( pWeapon )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pWeapon, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							
							for( i = 0; i < 3; i++ )
							{
								pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
																
								if( pArmor )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pArmor, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						break;
					case -4:		// 웨폰 8장 아머 7장
						if( CountEmptyInven( pPlayer ) >= 15 )
						{
							sPITEM_DATA pWeapon = NULL;
							sPITEM_DATA pArmor = NULL;
							
							sCHECK_INVEN checkInven;
							
							int i;

							for( i = 0; i < 8; i++ )
							{
								pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
								
								if( pWeapon )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pWeapon, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							
							for( i = 0; i < 7; i++ )
							{
								pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
								
								if( pArmor )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pArmor, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						break;

					//아머인첸트카드 1장 + 5만 실드
					case -5 :
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pArmor = NULL;
							sPITEM_DATA		pMoney = NULL;
							
							sCHECK_INVEN	checkInven;
							
							pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
							pMoney = CreateMoney( 50000 );
							
							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}
							
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1",row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
							
							
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;

					//아머인첸트카드 1장 + 10만 실드
					case -6 :
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pArmor	=	NULL;
							sPITEM_DATA		pMoney	=	NULL;
							
							sCHECK_INVEN	checkInven;

							pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
							pMoney = CreateMoney( 100000 );

							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}

							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;

					//웨폰인첸트카드 1장 + 5만 실드						
					case -7 :
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA	pWeapon = NULL;
							sPITEM_DATA pMoney	= NULL;

							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
							pMoney	= CreateMoney( 50000 );

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;

					//웨폰인첸트카드 1장 + 10만 실드					
					case -8 :
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA	pWeapon = NULL;
							sPITEM_DATA pMoney	= NULL;
							
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
							pMoney	= CreateMoney( 100000 );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}
							
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;

					//아머인첸트카드 2장 + 5만 실드
					case -9 :
						if( CountEmptyInven( pPlayer ) >= 2 )
						{
							sPITEM_DATA	pArmor = NULL;
							sPITEM_DATA pMoney	= NULL;

							sCHECK_INVEN	checkInven;
																		
							//인첸 n개수 만큼 만들기
							for( int i=0; i<2; i++ )
							{
								pArmor	= CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
								if( pArmor )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pArmor, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
								}
							}

							//돈만들기
							pMoney	= CreateMoney( 50000 );							
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 2 );
						}
						break;

					//아머인첸트카드 2장 + 10만 실드
					case -10 :
						if( CountEmptyInven( pPlayer ) >= 2 )
						{
							sPITEM_DATA	pArmor = NULL;
							sPITEM_DATA pMoney	= NULL;

							sCHECK_INVEN	checkInven;
							
							//인첸 n개수 만큼 만들기
							for( int i=0; i<2; i++ )
							{
								pArmor	= CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
								if( pArmor )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pArmor, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							
							//돈만들기
							pMoney	= CreateMoney( 100000 );							
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 2 );
						}
						break;

					//웨폰인첸트카드 2장 + 5만 실드
					case -11 :
						if( CountEmptyInven( pPlayer ) >= 2 )
						{
							sPITEM_DATA	pWeapon = NULL;
							sPITEM_DATA pMoney	= NULL;

							sCHECK_INVEN	checkInven;
							
							for( int i=0; i<2; i++ )
							{
								pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
								
								if( pWeapon )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pWeapon, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							
							pMoney	= CreateMoney( 50000 );
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 2 );
						}
						break;

					//웨폰인첸트카드 2장 + 10만 실드
					case -12 :
						if( CountEmptyInven( pPlayer ) >= 2 )
						{
							sPITEM_DATA	pWeapon = NULL;
							sPITEM_DATA pMoney	= NULL;

							sCHECK_INVEN	checkInven;
							
							for( int i=0; i<2; i++ )
							{
								pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );
								
								if( pWeapon )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pWeapon, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							
							pMoney	= CreateMoney( 100000 );
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 2 );
						}
						break;

					//아머인첸트카드 2장 + 웨폰인첸트카드 1장 + 10만 실드
					case -13 :
						if( CountEmptyInven( pPlayer ) >= 3 )
						{
							sPITEM_DATA pArmor = NULL;
							sPITEM_DATA	pWeapon = NULL;
							sPITEM_DATA pMoney	= NULL;

							sCHECK_INVEN	checkInven;
							
							//아머인첸트 2장
							for( int i=0; i<2; i++)
							{
								pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
								if( pArmor )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pArmor, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
								}
							}

							//웨폰 인첸트 1장
							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );													
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}
							
							//돈 10만
							pMoney	= CreateMoney( 100000 );
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 3 );
						}
						break;
					
					//아머인첸트카드 2장 + 웨폰인첸트카드 1장 + 20만 실드
					case -14 :
						if( CountEmptyInven( pPlayer ) >= 3 )
						{
							sPITEM_DATA pArmor = NULL;
							sPITEM_DATA	pWeapon = NULL;
							sPITEM_DATA pMoney	= NULL;
							
							sCHECK_INVEN	checkInven;
							
							//아머인첸트 2장
							for( int i=0; i<2; i++)
							{
								pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
								if( pArmor )
								{
									CheckEmptyInven( pPlayer, &checkInven );
									ItemToInventory( pPlayer, pArmor, &checkInven );
									ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
								}
							}
							
							//웨폰 인첸트 1장
							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );													
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}
							
							//돈 20만
							pMoney	= CreateMoney( 200000 );
							if( pMoney )
							{
								if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
									return;
								
								GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
								UpdateMoney( pPlayer );
								DATASERV_SendUpdateMoney( pPlayer );
								
								SendSystemMsg( pPlayer, g_LANG_STR[9], GET_MONEY_AMOUNT( pMoney ) );
								
								//
								INSERT_ITEM_TO_MEMORY( pMoney );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 3 );
						}
						break;

							case -15: //1. 레드나이트대거 +9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11286, "", "", 0  );

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -16: //2. 대지의정령로브 +5
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pArmor = NULL;
							sCHECK_INVEN	checkInven;

							pArmor = CreateItem( 10505, "", "", 0 );

							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -17: //3. 레드나이트도끼 +9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11475, "", "", 0  );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -18: //4. 레드나이트아머 +5
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pArmor = NULL;
							sCHECK_INVEN	checkInven;

							pArmor = CreateItem( 10463, "", "", 0  );
							
							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -19: //5. 레드헌터튜닉 +5
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pArmor = NULL;
							sCHECK_INVEN	checkInven;

							pArmor = CreateItem( 10484, "", "", 0  );
							
							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -20: //6. 레드헌터 롱보우 +9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11727, "", "", 0  );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -21: //7. 레드나이트메이스 +9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11538, "", "", 0  );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -22: //8. 대지의정령스태프 +9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11853, "", "", 0  );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}
							
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -23: //9. 레드나이트스피어+9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11601, "", "", 0  );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -24: //10. 아머, 웨폰 인첸트 각각 1장씩
						if( CountEmptyInven( pPlayer ) >= 2 )
						{
							sPITEM_DATA		pArmor = NULL;
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );
							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}
							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}
							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 2 );
						}
						break;
					case -25: //11. 아머 인첸트 1장씩
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pArmor = NULL;
							sCHECK_INVEN	checkInven;

							pArmor = CreateItem( dENCHANT_CARD_ARMOR, "", "", 0 );

							if( pArmor )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pArmor, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -26: //12. 레드나이트양손검 +9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11412, "", "", 0  );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -27: //13. 웨폰 인첸트 1장씩
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( dENCHANT_CARD_WEAPON, "", "", 0 );

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );

						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					case -28: //14. 레드나이트한손검 +9
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11349, "", "", 0  );
							
							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;

					case -1000://15. 영웅유저
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11349, "", "", 0  );

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;

					case -1001://16. VIP유저
						if( CountEmptyInven( pPlayer ) >= 1 )
						{
							sPITEM_DATA		pWeapon = NULL;
							sCHECK_INVEN	checkInven;

							pWeapon = CreateItem( 11349, "", "", 0  );

							if( pWeapon )
							{
								CheckEmptyInven( pPlayer, &checkInven );
								ItemToInventory( pPlayer, pWeapon, &checkInven );
								ITEMLOG_ItemLog( pPlayer, NULL, pWeapon, ITEMLOG_ACT_EVENTINPUT );
							}

							char query[1024];
							sprintf( query, "update PurSell set PurFlag = 1, purChgDate = CURRENT_TIMESTAMP() where PurKey = '%s' limit 1", row[PURROW_PURKEY] );
							mysql_query( g_BILL_SQL, query );
						}
						else
						{
							SendSystemMsg( pPlayer, g_LANG_STR[277], 1 );
						}
						break;
					}
				}
				else
				{
					sPBILLING_TABLE pBillInfo = FindBillingItem( atoi( row[PURROW_PRODUCTNUM] ) );
					
					if( !pBillInfo )
					{
						log( "LoadNonUsedBillItems : return NULL : %s / %d\r\n", __FILE__, __LINE__ );
						continue;
					}
					
					sPBILL_ITEM_DATA pNewBillItem = NewBillItem();
					
					if( !pNewBillItem )
					{
						log( "LoadNonUsedBillItems : malloc failed\r\n" );
						continue;
					}
					
					pPlayer->invenIdx++;
					pNewBillItem->idxNum = pPlayer->invenIdx;
					pNewBillItem->itemNum = pBillInfo->itemNum;
					pNewBillItem->cnt = pBillInfo->cnt;
					pNewBillItem->ProductNum = atoi( row[PURROW_PRODUCTNUM] );
					strcpy( pNewBillItem->PurKey, row[PURROW_PURKEY] );
					
					pNewBillItem->prev = NULL;
					pNewBillItem->next = NULL;
					
					INSERT_TO_LIST( pPlayer->billItems, pNewBillItem, prev, next );
				}
			}
			mysql_free_result( result );
		}
	}
}


//아이템 보상 관련 부분(서버 뻣었을시 사용해라)
void LoadReward( sPDESC_DATA pPlayer )
{
	char query[dMAX_QUERY_SIZE];
	char escID[128];
	
	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );

	sprintf( query, "select * from REWARD where UserUID = '%s'", escID );

	if( mysql_query( g_BILL_SQL, query ) )
		log( "%s" ,mysql_error( g_BILL_SQL ) );
	else
	{
		MYSQL_RES *result;
		MYSQL_ROW row;

		BOOL	btCheck = FALSE;

		result = mysql_store_result( g_BILL_SQL );

		if( result )
		{
			if(( row = mysql_fetch_row( result ) ))
			{		
				//플래그를 검사하고 그 플래그가 1이면 돈지급한 캐릭터 하지만 프리미엄존+는 지급하지 않은 대상
				//프리미엄존+을 7일 추가시켜준다.
				int n = atoi( row[1] );
				if( n == 1)
				{
					//인첸 n개수 만큼 만들기
					if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] != 0 )
					{
							//프리미엄존플러스 7일짜리를 여기서 지급한다.
							//먼저 프리미엄 종료일을 출력하고
							SendSystemMsg( pPlayer, g_LANG_STR[204], "기존 프리미엄 존 +", GetRemindTimeStr( abs( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] - g_CurrRealTime ) ) );
							pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] + GetSecondDay( 7 );
							//DB
							SendSystemMsg( pPlayer, g_LANG_STR[204], "현재 프리미엄 존 +", GetRemindTimeStr( abs( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] - g_CurrRealTime ) ) );
							UpdateBillItemInfo( pPlayer, "effect6", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] );
							SendSystemMsg( pPlayer, "실드는 이미 보상하여 지급하지 않았습니다." );
							SendSystemMsg( pPlayer, "감사합니다." );
							btCheck = TRUE;
					}

					if( btCheck == FALSE )
					{
						SendSystemMsg( pPlayer, "기존 프리미엄 존+ 효과가 2012년 1월 7일 이전에 종료되었거나" );
						SendSystemMsg( pPlayer, "사용하신적이 없으시며, 실드보상은 이미 보상하여 지급하지 않았습니다." );
						SendSystemMsg( pPlayer, "감사합니다." );
					}

					sprintf( query, "update REWARD set purFlag = %d where UserUID='%s'", 2, escID );
					mysql_query( g_BILL_SQL, query );
				}
				mysql_free_result( result );
			}
			else
			{
				//여기서 먼저
				//mysql에 자료가 있는지 체크
				//인벤토리체크
				//if( CountEmptyInven( pPlayer ) >= 6 )
				{
					sPITEM_DATA		pMoney = NULL;
	
					//인첸 n개수 만큼 만들기
					if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] != 0 )
					{
							//시작하는 날보다 크고

							//프리미엄존플러스 7일짜리를 여기서 지급한다.
							//먼저 프리미엄 종료일을 출력하고
							SendSystemMsg( pPlayer, g_LANG_STR[204], "기존 프리미엄 존 +", GetRemindTimeStr( abs( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] - g_CurrRealTime ) ) );
							pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] + GetSecondDay( 7 );
							//DB
							SendSystemMsg( pPlayer, g_LANG_STR[204], "현재 프리미엄 존 +", GetRemindTimeStr( abs( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] - g_CurrRealTime ) ) );
							UpdateBillItemInfo( pPlayer, "effect6", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] );
							btCheck = TRUE;

						
					}
					else
					{
						SendSystemMsg( pPlayer, "기존 프리미엄 존+ 효과가 2012년 1월 7일 이전에 종료되었거나" );
						SendSystemMsg( pPlayer, "사용하신적이 없어 지급하지 않았으며" );
					}

					//돈만들기
					if( GET_ALL_SKILL( pPlayer ) >= 20 )
					{
						pMoney	= CreateMoney( 40000000 );							
						btCheck = TRUE;
					}

					if( pMoney )
					{
						if( GET_MONEY( pPlayer ) + GET_MONEY_AMOUNT( pMoney ) > dMAX_HAVE_MONEY )
							return;

						GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
						UpdateMoney( pPlayer );
						DATASERV_SendUpdateMoney( pPlayer );

						SendSystemMsg( pPlayer, "실드 보상하였습니다." );
						SendSystemMsg( pPlayer, "감사합니다" );
						//
						INSERT_ITEM_TO_MEMORY( pMoney );
					}

					if( btCheck == FALSE )
					{
						SendSystemMsg( pPlayer, "기존 프리미엄 존+ 효과가 2012년 1월 7일 이전에 종료되었거나" );
						SendSystemMsg( pPlayer, "사용하신적이 없어 지급하지 않았으며" );
						SendSystemMsg( pPlayer, "4천만 실드 또한 레벨이 20보다 작으셔서 보상대상이 아닙니다." );
						SendSystemMsg( pPlayer, "감사합니다" );
					}

					sprintf( query, "insert into REWARD( UserUID,  PurFlag ) values( '%s', %d )", escID, 2 );
					mysql_query( g_BILL_SQL, query );
				}
				mysql_free_result( result );
			}
		}

	}
}

void DestroyBillItemMemory()
{
	sPBILL_ITEM_DATA item, next_item;

	LIST_WHILE( gITEMS.billItem_Memory, item, next_item, next, WMGR );
	REMOVE_FROM_LIST( gITEMS.billItem_Memory, item, prev, next );
	SAFE_FREE( item );
	LIST_WHILEEND( gITEMS.billItem_Memory, item, next_item );
}



#ifndef __CHINA_VER__
SOCKET	g_BillSoket = INVALID_SOCKET ;
char	g_BillRecvBuff[dMAX_DATA_SOCK_BUFF];
int		g_BillRecvSize = 0;
DWORD	g_dwLastPingTime = 0;
BOOL	g_bGameStar = FALSE;

/*----------------------------------------------------------------------------------
	Name :	StartBilling( int ServerNum )
	Desc :	
----------------------------------------------------------------------------------*/
void StartBilling( int ServerNum )
{
	char	szPacket[256];
	int		nPacketNum = dUSER_START_BILLING;
	int		nSize = 0;

	memset( &szPacket, 0, 256 );

	memcpy( &szPacket[0], &nPacketNum, sizeof(int) );
	nSize += sizeof(int);
	memcpy( &szPacket[nSize], &ServerNum, sizeof( int ) );
	nSize += sizeof(int);

	send( g_BillSoket, szPacket, nSize, 0 );
}

/*----------------------------------------------------------------------------------
	Name :	EndBilling( int ServerNum )
	Desc :	
----------------------------------------------------------------------------------*/
void EndBilling( int ServerNum )
{
	char	szPacket[256];
	int		nPacketNum = dUSER_END_BILLING;
	int		nSize = 0;

	memset( &szPacket, 0, 256 );

	memcpy( &szPacket[0], &nPacketNum, sizeof(int) );
	nSize += sizeof(int);
	memcpy( &szPacket[nSize], &ServerNum, sizeof(int) );
	nSize += sizeof(int);
	send( g_BillSoket, szPacket, nSize, 0 );
}


/*----------------------------------------------------------------------------------
	Name :	 PingBilling( int ServerNum )
	Desc :	게임서버 핑 ( 게임서버 -> 빌링 )
----------------------------------------------------------------------------------*/
void PingBilling( int ServerNum )
{
	if( g_BillSoket != INVALID_SOCKET )
	{
		if( g_dwLastPingTime + 15000 <= timeGetTime() )
		{
			g_dwLastPingTime = timeGetTime();

			char szPacket[256];
			int  nPacketNum = dUSER_GAME_PING;//게임서버핑
			int  nSize = 0;

			memset( &szPacket, 0, 256 );

			memcpy( &szPacket[0], &nPacketNum, sizeof(int) );
			nSize += sizeof(int);
			memcpy( &szPacket[nSize], &ServerNum, sizeof(int) );
			nSize += sizeof(int);

			send( g_BillSoket, szPacket, nSize, 0 );
		}
	}
}

/*----------------------------------------------------------------------------------
	Name :	GetBillingInfo( char *szID, char *szMac, char *szIP, int level, int ServerNum )
	Desc :	유져 접속 ( 게임 -> 빌링 )
----------------------------------------------------------------------------------*/
void GetBillingInfo( char *szID, char *szMac, char *szIP, int level, int ServerNum )
{
	char	szPacket[256];
	int		nPacketNum = dUSER_LOGIN_BILLING;
	int		nSize = 0;
	int		nState = 0;

	memset( szPacket, 0, 256 );

	memcpy( &szPacket[0], &nPacketNum, sizeof(int) );
	nSize += sizeof(int);
	memcpy( &szPacket[nSize], szID, strlen(szID) );
	nSize += 50;
	memcpy( &szPacket[nSize], szMac, strlen(szMac) );
	nSize += 32;
	memcpy( &szPacket[nSize], szIP, strlen(szIP) );
	nSize += 20;
	memcpy( &szPacket[nSize], &ServerNum, sizeof(int) );
	nSize += sizeof( int );
	memcpy( &szPacket[nSize], &nState, sizeof(int) );
	nSize += sizeof( int );
	memcpy( &szPacket[nSize], &level, sizeof(int) );
	nSize += sizeof( int );

	int nSendSize;

	nSendSize = send( g_BillSoket, szPacket, nSize, 0 );
}


/*----------------------------------------------------------------------------------
	Name :	OutBillingInfo( char *szID, int ServerNum )
	Desc :	유져 게임 종료 ( 게임 -> 빌링 )
----------------------------------------------------------------------------------*/
void OutBillingInfo( char *szID, int ServerNum )
{
	char szPacket[256];
	int  nPacketNum = dUSER_LOGOUT;
	int	 nState = 0;
	int  nValue = 0;
	int	 nSize = 0;

	memset( &szPacket, 0, 256 );

	memcpy( &szPacket[0], &nPacketNum, sizeof(int) );
	nSize += sizeof(int);
	memcpy( &szPacket[nSize], szID, strlen(szID) );
	nSize += 50;
	memcpy( &szPacket[nSize], &ServerNum, sizeof(int) );
	nSize += sizeof(int);
	memcpy( &szPacket[nSize], &nState, sizeof(int) );
	nSize += sizeof(int);
	memcpy( &szPacket[nSize], &nValue, sizeof(int) );
	nSize += sizeof(int);

	send( g_BillSoket, szPacket, nSize, 0 );
}

/*----------------------------------------------------------------------------------
	Name :	
	Desc :	
----------------------------------------------------------------------------------*/
int RecvBillingPacket()
{
	int recvSize = 0;
	char recvBuff[dMAX_SOCK_BUFF];

	recvSize = recv( g_BillSoket, recvBuff, 1024, 0 );

	if( recvSize == 0 )
		return 0;

	if( recvSize < 0 )
	{
#if defined(WIN32)
		if( WSAGetLastError() != WSAEWOULDBLOCK )
			return 0;
#else
		if( errno != EWOULDBLOCK )
			return 0;
#endif
		else 
			return 1;
	}
	
	if( ( g_BillRecvSize + recvSize ) >= dMAX_DATA_SOCK_BUFF )
	{
		log( "Billing Socket Over flow\r\n" );
		return 0;
	}

	memcpy( &g_BillRecvBuff[g_BillRecvSize], recvBuff, recvSize );
	g_BillRecvSize += recvSize;

	return 1;
}


/*----------------------------------------------------------------------------------
	Name :	
	Desc :	
----------------------------------------------------------------------------------*/
int	ReadBillingBuffer()
{
#define dMAX_BILL_PACKET 9
	int nPacketSize[dMAX_BILL_PACKET] = 
	{
		0,								//0 사용하지 않는다.
		4 + 50 + 32 + 20 + 4 + 4 + 4,	//1 유저의 접속	(게임->빌링 )
		4 + 50 + 4 + 4,					//2 유져 빌링 정보( 빌링->게임 )
		4 + 50 + 4 + 4 + 4,				//3 유져 게임 종료( 게임 -> 빌링 )
		4 + 50 + 4 + 4 ,				//4 사용시간 리포트( 빌링 -> 게임 )
		4 + 50 + 4 + 4 ,				//5 사용시간 만료( 빌링 -> 게임 )
		4 + 4,							//6 게임서버 시작( 게임 -> 빌링 )
		4 + 4,							//7 게임서버 종료( 게임 -> 빌링 )
		4 + 4,							//8 게임서버 핑( 빌링 -> 게임 )
	};
	
	//최소 4바이트
	while( g_BillRecvSize > 4 )
	{
		int nPacket = *(int*)g_BillRecvBuff;

		if( nPacket <= 0 || nPacket >= dMAX_BILL_PACKET )
		{
			char szTmp[128];
			sprintf( szTmp, "Bad Billing Packet : %d\r\n", nPacket );
			log( szTmp );
			return 0;
		}

		if( g_BillRecvSize < nPacketSize[nPacket] )
			return 1;

		int readSize;
		int	 nState;
		int  nValue;

		char szID[50];
		char szTem[256];
		
	
		memset( &szID, 0, 50 );
		memset( &szTem, 0, 256 );

		switch( nPacket )
		{
		case dUSER_BILLING_INFOR:
			{
				//빌링 정보 받기
				readSize = sizeof(int);
				memcpy( &szID, &g_BillRecvBuff[readSize], 50 );
				readSize += 50;
				memcpy( &nState, &g_BillRecvBuff[readSize], sizeof(int) );
				readSize += sizeof(int);
				memcpy( &nValue, &g_BillRecvBuff[readSize], sizeof(int) );
				readSize += sizeof(int);
				szID[49] = NULL_STR;
				
				sPDESC_DATA pTarget = FindPlayerIdList( szID );
				if( pTarget )
				{	
		//		sprintf( szTem, "ID[%s], PacketNumber[%d], Point[%d]", GET_ID(pTarget), nState, nValue );

				if( nState == 0 && nValue == 1 )
				{
					//사용시간이 없어 게임을 할수 없다는 패킷을 클라이언트에 보낸다.
					g_nPos = 2;
					PutWord( g_Packet, dPACKET_BILLING_DISCONNECT, g_nPos );
					PutSize( g_Packet, g_nPos );
					SendData( pTarget );		
		//			DebugMsg( "Have Not Time - %s -\r\n", szTem );
					pTarget->disconnectFlag=1;
				}
				else
				{
					//사용시간이 있어 사용할수 있다는 패킷을 클라이어트로 보낸다.
					g_nPos = 2;
					PutWord( g_Packet, dPACKET_BILLING_CONNECT, g_nPos );
					PutInteger( g_Packet, nValue, g_nPos );
					PutSize( g_Packet, g_nPos );
					SendData( pTarget, g_Packet, g_nPos );
		//			DebugMsg( "Have Time - %s -\r\n", szTem );
				}
				}
				memmove( &g_BillRecvBuff[0], &g_BillRecvBuff[readSize], readSize );
				g_BillRecvSize -= readSize;
			}
			break;
		case dUSER_TIME_REPORT:
			{
				//사용시간 리포트
				readSize = sizeof( int );
				
				memcpy( &szID, &g_BillRecvBuff[readSize], 50 );
				readSize += 50;
				memcpy( &nState, &g_BillRecvBuff[readSize], sizeof( int ) );
				readSize += sizeof( int );
				memcpy( &nValue, &g_BillRecvBuff[readSize], sizeof( int ) );
				readSize += sizeof( int );
				
				szID[49] = NULL_STR;

				sPDESC_DATA pTarget = FindPlayerIdList( szID );

				if( pTarget )
				{	
		//		sprintf( szTem, "ID[%s], PacketNumber[%d], Point[%d]", GET_ID(pTarget), nState, nValue );
				

				//남은 빌링 시간을 보내준다.
				g_nPos = 2;
				PutWord( g_Packet, dPACKET_BILLING_USE_TIME, g_nPos );
				PutInteger( g_Packet, nValue, g_nPos );
				PutSize( g_Packet, g_nPos );
				SendData( pTarget, g_Packet, g_nPos );
				
		//		DebugMsg( "Time left over - %s -\r\n", szTem );
				}
				
				memmove( &g_BillRecvBuff[0], &g_BillRecvBuff[readSize], readSize );
				g_BillRecvSize -= readSize;
			}
			break;

		case dUSER_TIMEOUT:
			{
 			//사용시간 만료
				readSize = sizeof(int);
				
				memcpy( &szID, &g_BillRecvBuff[readSize], 50 );
				readSize += 50;
				memcpy( &nState, &g_BillRecvBuff[readSize], sizeof(int) );
				readSize += sizeof(int);
				memcpy( &nValue, &g_BillRecvBuff[readSize], sizeof(int) );
				readSize += sizeof(int);
				szID[49] = NULL_STR;
				
				sPDESC_DATA pTarget = FindPlayerIdList( szID );

				if( pTarget ) 
				{
				//종료메시지 보냄
				if( nState == 0 && nValue == 1 )
				{
					//사용시간이 없어 게임을 할수 없다는 패킷을 클라이언트에 보낸다.
					g_nPos = 2;
					PutWord( g_Packet, dPACKET_BILLING_DISCONNECT, g_nPos );
					PutSize( g_Packet, g_nPos );
					SendData( pTarget );		
				//	DebugMsg( "Have Not Time - %s -\r\n", szTem );
				//	sprintf( szTem, "ID[%s], PacketNumber[%d], Point[%d]", szID, nState, nValue );
				//	DebugMsg( "Time over - %s -\r\n", szTem );
					pTarget->disconnectFlag=1;
				}
	//			pTarget->disconnectFlag = 1;
				}
				
				memmove( &g_BillRecvBuff[0], &g_BillRecvBuff[readSize], readSize );
				g_BillRecvSize -= readSize;
			}
			break;
		}
	}
	return 1;
}


/*----------------------------------------------------------------------------------
	Name :	
	Desc :	
----------------------------------------------------------------------------------*/
#endif




//EOF






















