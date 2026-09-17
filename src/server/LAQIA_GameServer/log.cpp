#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "message.h"

#include "world.h"
#include "player.h"
#include "player_world.h"
#include "item.h"
#include "player.h"
#include "party.h"
#include "deal.h"
#include "vehicle.h"
#include "quest.h"

#include "data.h"

#include "log.h"


/*
 *	LOG_ItemLog
 */
void LOG_ItemLog( sPITEM_DATA pItem, sPDESC_DATA pOwner, sPDESC_DATA pTarget, BYTE action, BYTE actionFlag )
{
	if( !pOwner || !pItem )
		return;

	if( IS_STACK_ITEM( pItem->itemNum ) )
		return;

/*	if( *pItem->maker == NULL_STR )
	{
		if( action != dITEMLOG_WAREHOUSE )
		{
			if( !pItem->exAttr[0] && !pItem->exAttr[1] && 
				!pItem->exAttr[2] && !pItem->exAttr[3] && !pItem->exAttr[4] &&
				pItem->itemNum != dARMOR_ENCHANT_STONE &&
				pItem->itemNum != dWEAPON_ENCHANT_STONE )
				return;
		}
	}*/

	char escOwnerID[128];
	char escTargetID[128];

	if( g_LogSql.Query( "insert into item_log values( '', %d, '%s','%s', %d, %d, %d, '%s', %d, %ld )",
		pItem->itemNum,
		g_ItemSql.StringConvert( pItem->serial, escSerial ), 
		g_ItemSql.StringConvert( GET_ID( pOwner ), escOwnerID ),
		GET_CHARPOS( pOwner ),
		action, 
		actionFlag,
		pTarget ? g_ItemSql.StringConvert( GET_ID( pTarget ), escTargetID ) : "",
		pTarget ? GET_CHARPOS( pOwner ) : 0,
		g_CurrRealTime ) )
	{
		log( "LOG_ItemLog : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	LOG_EnchantLog
 */
void LOG_EnchantLog( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pPlayer || !pItem )
		return;

	if( g_LogSql.Query( "insert into enchant_log values( '', %d, '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %ld )", 
		pItem->itemNum,
		g_LogSql.StringConvert( pItem->serial, escSerial ),
		g_LogSql.StringConvert( GET_ID( pPlayer ), escID ),
		GET_CHARPOS( pPlayer ),
		pItem->exAttr[0],
		pItem->exValue[0],
		pItem->exAttr[1],
		pItem->exValue[1],
		pItem->exAttr[2],
		pItem->exValue[2],
		pItem->exAttr[3],
		pItem->exValue[3],
		pItem->exAttr[4],
		pItem->exValue[4],
		g_CurrRealTime ) )
	{
		log( "LOG_EnchantLog : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	LOG_LevelLog
 */
void LOG_LevelLog( sPDESC_DATA pPlayer, char *name, int level )
{
	if( !pPlayer )
		return;

	if( g_LogSql.Query( "insert into level_log values( '', '%s', %d, '%s', %d, %ld )",
		g_LogSql.StringConvert( GET_ID( pPlayer ), escID ),
		GET_CHARPOS( pPlayer ),
		g_LogSql.StringConvert( name, escName ),
		level,
		g_CurrRealTime ) )
	{
		log( "LOG_LevelLog : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	LOG_BankLog
 */
void LOG_BankLog( sPDESC_DATA pPlayer, int action, int amount )
{
	if( !pPlayer )
		return;

	if( g_LogSql.Query( "insert into bank_log values( '', '%s', %d, %d, %d, %ld )",
			g_LogSql.StringConvert( GET_ID( pPlayer ), escID ),
			GET_CHARPOS( pPlayer ),
			amount,
			action,
			g_CurrRealTime ) ) 
	{
		log( "LOG_BankLog : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	LOG_LoginLog
 */
void LOG_LoginLog( sPDESC_DATA pPlayer, int action )
{
	if( !pPlayer )
		return;

	if( g_LogSql.Query( "insert into login_log values( '', '%s', %d, '%s', %d, '%s', %ld )",
		g_LogSql.StringConvert( GET_ID( pPlayer ), escID ),
		GET_CHARPOS( pPlayer ),
		g_LogSql.StringConvert( GET_NAME( pPlayer ), escName ),
		action,
		pPlayer->ip,
		g_CurrRealTime ) ) 
	{
		log( "LOG_LoginLog : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	EVENTLOG_Start
 */
void EVENTLOG_Start( sPDESC_DATA pPlayer, char *name, BYTE charPos, BYTE charClass )
{
	if( g_LogSql.Query( "insert into event_log values( '', '%s', %d, '%s', %d, %ld, %ld )",
		g_LogSql.StringConvert( GET_ID( pPlayer ), escID ),
		charPos,
		g_LogSql.StringConvert( name, escName ),
		charClass,
		g_CurrRealTime,
		0 ) )
	{
		log( "EVENTLOG_Start : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	EVENTLOG_End
 */
void EVENTLOG_End( sPDESC_DATA pPlayer )
{
	if( g_LogSql.Query( "update event_log set endTime=%ld-startTime where ownerID = '%s' and ownerPos = %d limit 1",
		g_CurrRealTime,
		g_LogSql.StringConvert( GET_ID( pPlayer ), escID ),
		GET_CHARPOS( pPlayer ) ) )
	{
		log( "EVENTLOG_End : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	EVENTLOG_Delete
 */
void EVENTLOG_Delete( char *id, BYTE charPos )
{
	if( g_LogSql.Query( "delete from event_log where ownerID = '%s' and ownerPos = %d limit 1",
		g_LogSql.StringConvert( id, escID ), charPos ) )
	{
		log( "EVENTLOG_Delete : %s\r\n", g_LogSql.GetLastError() );
	}
}

/*
 *	NOTICE_CheckNotice
 */
void NOTICE_CheckNotice()
{
	int noticeIdx = -1;

	if( !g_LogSql.Query( "select * from notice_table order by idx asc limit 1" ) )
	{
		if( g_LogSql.GetResult() )
		{
			if( g_LogSql.GetRowData() )
			{
				noticeIdx = g_LogSql.GetInt( 0 );
				BroadCastNoticeMsg( g_LogSql.GetRow( 1 ) );
			}
			g_LogSql.FreeResult();
		}
	}
	else
	{
		log( "NOTICE_CheckNotice : %s\r\n", g_LogSql.GetLastError() );
	}

	if( noticeIdx != -1 )
	{
		if( g_LogSql.Query( "delete from notice_table where idx = %d limit 1", noticeIdx ) )
		{
			log( "NOTICE_CheckNotice : %s\r\n", g_LogSql.GetLastError() );
		}
	}
}

/*
 *	RESQUE_CheckResque
 */
void RESQUE_CheckResque()
{
	if( !g_LogSql.Query( "select * from resque_table" ) )
	{
		if( g_LogSql.GetResult() )
		{
			while( g_LogSql.GetRowData() )
			{
				int idxNum = g_LogSql.GetInt( 0 );

				sPDESC_DATA pTarget = FindPlayerNameList( g_LogSql.GetRow( 1 ) );

				if( pTarget )
				{
					if( GET_STATE( pTarget ) == dDESC_STATE_PLAYING )
					{
						SendSystemMsg( pTarget, "이동요청에 의해 캐릭터의 위치를 이동합니다." );

						if( GET_DEAD_STATE( pTarget ) )
							GET_DEAD_STATE( pTarget ) = 0;

						PlayerToHometown( pTarget );

						if( g_LogSql.Query( "delete from resque_table where idx = %d", idxNum ) )
							log( "RESQUE_CheckResque : %s\r\n", g_LogSql.GetLastError() );
					}
				}
			}
		}
		g_LogSql.FreeResult();
	}
	else
	{
		log( "RESQUE_CheckResque : %s\r\n", g_LogSql.GetLastError() );
	}
}


/*
 *	LOG_CreateLog
 */
void LOG_CreateLog( char *id, BYTE charPos, char *name, BYTE action )
{
	if( g_LogSql.Query( "insert into create_log values( '', '%s', %d, '%s', %d, %ld )",
		g_LogSql.StringConvert( id, escID ),
		charPos,
		g_LogSql.StringConvert( name, escName ),
		action,
		g_CurrRealTime ) )
	{
		log( "LOG_CreateLog : %s\r\n", g_LogSql.GetLastError() );
	}
}

//EOF
