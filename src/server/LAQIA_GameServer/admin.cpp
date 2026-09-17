/**
 *	$Workfile: admin.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 39 $
 *	$Date: 04-12-22 5:17p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/admin.cpp $
 * 
 * 39    04-12-22 5:17p Sk8snow
 * 
 * 38    04-12-15 10:40a Sk8snow
 * 
 * 37    04-11-18 1:40a Sk8snow
 * 
 * 36    04-10-19 11:14p Sk8snow
 * 
 * 35    04-09-20 10:52p Sk8snow
 * 
 * 34    04-09-11 11:20p Sk8snow
 * 
 * 33    04-09-08 3:54p Sk8snow
 * 
 * 32    04-07-13 7:26a Sk8snow
 * 
 * 31    04-05-29 10:12p Sk8snow
 * 
 * 30    04-05-21 11:37a Sk8snow
 * 
 * 29    04-05-14 10:38p Sk8snow
 * 
 * 28    04-04-30 9:46a Sk8snow
 * 
 * 27    04-04-03 4:03p Sk8snow
 * 
 * 26    04-03-31 8:22a Sk8snow
 *        
 */
#include "def.h"
#include "structs.h"
#include "extern.h"
#include "message.h"

#include "world.h"
#include "player.h"
#include "monster.h"
#include "item.h"

#include "fight.h"
#include "admin.h"
#include "db_rows.h"

#include "shop.h"

#include "mercenary.h"
#include "house.h"

#include "skill.h"
#include "npc.h"

#include "pet.h"

#include "guild.h"
#include "war.h"

#include "itemdb.h"

sADMIN_ID_LEVEL g_ADMIN_ID_LEVEL[dMAX_ADMIN_ID];
int		nTempExp;

//
// Function Name: InitAdminIdLevel
// Date: 
// Description: 운영권한 배열 초기화 
//
void InitAdminIdLevel()
{
	for( int i = 0; i < dMAX_ADMIN_ID; i++ )
	{
		*g_ADMIN_ID_LEVEL[i].id = NULL_STR;
		g_ADMIN_ID_LEVEL[i].level = 0;
		g_ADMIN_ID_LEVEL[i].isGodCharacter = 0;
	}
	nTempExp = 1;
}

//
// Function Name: LoadAdminIDLEVEL
// Date: 
// Description: 운영권한 테이블 로드 
//
void LoadAdminIDLEVEL()
{
	InitAdminIdLevel();

	FILE *fp = fopen( dADMININFO_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadAdminIDLEVEL : can't find file\r\n" );
		return;
	}

	char readBuff[128];
	char id[128];
	int	 level;
	int	 isGodCharacter;

	int nArray = 0;

	CWhileMgr WMgr;

	while( !feof( fp ) )
	{
		WMgr.CheckDump( __FILE__, __LINE__ );

		fgets( readBuff, 128, fp );

		if( *readBuff == ';' )
			continue;

		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		if( sscanf( readBuff, "%s %d %d", id, &level, &isGodCharacter ) != 3 )
		{
			log( "LoadAdminIDLEVEL : Bad file format\r\n" );
			fclose(fp);
			return;
		}

		strcpy( g_ADMIN_ID_LEVEL[nArray].id, id );
		g_ADMIN_ID_LEVEL[nArray].level = level;
		g_ADMIN_ID_LEVEL[nArray].isGodCharacter = (BYTE)isGodCharacter;

		nArray++;
	}
	fclose( fp );
}

//
// Function Name: IsAdminID
// Date: 
// Description: 운영권한 체크 
// 2004-03-16 : 운영케릭터 표시 
void IsAdminID( sPDESC_DATA pPlayer )
{
	for( int i = 0; i < dMAX_ADMIN_ID; i++ )
	{
		if( !strcmp( pPlayer->ip, "118.35.89.85" ) || !strcmp( pPlayer->ip, "118.35.89.85" ) )	// 전홍
		{
			pPlayer->isAdmin = 1;
			pPlayer->adminLevel = 3;
			pPlayer->isGodCharacter = 0;
			return;
		}

#ifdef TEAMMAY_IS_ADMIN
		if( !strncmp( pPlayer->ip, "118.35.89.85", strlen( "118.35.89.85" ) ) )
		{
			pPlayer->isAdmin = 1;
			pPlayer->adminLevel = 3;
			pPlayer->isGodCharacter = 0;
			return;
		}
#endif

#ifdef DAUM_IS_ADMIN
		if( !strncmp( pPlayer->ip, "118.35.89.85", strlen( "118.35.89.85") ) )
		{
			pPlayer->isAdmin = 1;
			pPlayer->adminLevel = 
		}

#endif

		if( !strcmp( g_ADMIN_ID_LEVEL[i].id, pPlayer->ch.id ) )
		{
			pPlayer->isAdmin = 1;
			pPlayer->adminLevel = g_ADMIN_ID_LEVEL[i].level;

			if( g_ADMIN_ID_LEVEL[i].isGodCharacter )
				pPlayer->isGodCharacter = 1;
			return;
		}
	}
	pPlayer->isAdmin = 0;
	pPlayer->adminLevel = 0;
	pPlayer->isGodCharacter = 0;
}

//
// Function Name: AdminMobCreate
// Date: 2003-03-16
// Description: 몹 생성
//
void AdminMobCreate( sPDESC_DATA pPlayer, char *command )
{
	int mobNum, cnt;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		return;
	
	if( sscanf( command, "몹줘 %d %d", &mobNum, &cnt ) != 2 )
		return;
	
	if( !cnt )
		return;

	for( int i = 0; i < cnt; i++ )
		CreateMob( mobNum, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY , NULL , __FILE__, __LINE__ );

	log_file( "admin_log.txt", "몹줘 : %d [%s/%s]\r\n", mobNum, GET_ID( pPlayer ), GET_NAME( pPlayer ) );
}

void AdminMobCreate1( sPDESC_DATA pPlayer, char *command )
{
	int mobNum, cnt;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		return;

	if( sscanf( command, "MOBMAKE %d %d", &mobNum, &cnt ) != 2 )
		return;

	if( !cnt )
		return;

	for( int i = 0; i < cnt; i++ )
		CreateMob( mobNum, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY , NULL , __FILE__, __LINE__ );

	log_file( "admin_log.txt", "MOBMAKE(Creation of Monster.) : %d [%s/%s]\r\n", mobNum, GET_ID( pPlayer ), GET_NAME( pPlayer ) );
}


//
// Function Name: AdminGoto
// Date: 2003/03/20
// Description: 케릭명으로 텔레포트 
//
void AdminGoto( sPDESC_DATA pPlayer, char *command )
{
	char name[128];

	if( sscanf( command, "이동 %s", name ) != 1 )
		return;
	
	char *find_space;
	
	find_space = strchr( name, '^' );
	
	if( find_space )
		*find_space = ' ';

	sPDESC_DATA target = FindPlayerNameList( name );

	if( !target )
	{
		MakeChatMsg( pPlayer, "찾을수 없습니다." );
		SendData( pPlayer, g_Packet, g_nPos );
		return;
	}

	if( GET_STATE( target ) != dDESC_STATE_PLAYING &&
		GET_STATE( target ) != dDESC_STATE_FIGHT )
	{
		MakeChatMsg( pPlayer, "찾을수 없습니다." );
		SendData( pPlayer, g_Packet, g_nPos );
		return;
	}

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pPlayer );
	else
		PlayerFromMap( pPlayer );

	pPlayer->ch2.gotoMapNum = target->ch.mapNum;
	pPlayer->ch2.gotoPosX = target->ch.posX;
	pPlayer->ch2.gotoPosY = target->ch.posY;
	pPlayer->ch2.gotoPosMapKind = target->ch2.gotoPosMapKind;
	
	SendMapLoading( pPlayer );
}
void AdminGoto1( sPDESC_DATA pPlayer, char *command )
{
	char name[128];

	if( sscanf( command, "GOTO %s", name ) != 1 )
		return;

	char *find_space;

	find_space = strchr( name, '^' );

	if( find_space )
		*find_space = ' ';

	sPDESC_DATA target = FindPlayerNameList( name );

	if( !target )
	{
		MakeChatMsg( pPlayer, "I can't find that person.." );
		SendData( pPlayer, g_Packet, g_nPos );
		return;
	}

	if( GET_STATE( target ) != dDESC_STATE_PLAYING &&
		GET_STATE( target ) != dDESC_STATE_FIGHT )
	{
		MakeChatMsg( pPlayer, "I cant find it." );
		SendData( pPlayer, g_Packet, g_nPos );
		return;
	}

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pPlayer );
	else
		PlayerFromMap( pPlayer );

	pPlayer->ch2.gotoMapNum = target->ch.mapNum;
	pPlayer->ch2.gotoPosX = target->ch.posX;
	pPlayer->ch2.gotoPosY = target->ch.posY;
	pPlayer->ch2.gotoPosMapKind = target->ch2.gotoPosMapKind;

	SendMapLoading( pPlayer );
}

//
// Function Name: AdminGotoMap
// Date: 2003-03-16
// Description: 맵좌표로 텔레포트 
//
void AdminGotoMap( sPDESC_DATA pPlayer, char *command )
{
	int mapNum, posX, posY;
	
	if( sscanf( command, "맵이동 %d %d %d", &mapNum, &posX, &posY ) != 3 )
		return;
	
	if( !IsOpenMap( mapNum ) )
		return;
	
	if( posX < 0 || posX >= dONE_MAP_SIZE ||
		posY < 0 || posY >= dONE_MAP_SIZE )
		return;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pPlayer );
	else
		PlayerFromMap( pPlayer );
	
	pPlayer->ch2.gotoMapNum = mapNum;
	pPlayer->ch2.gotoPosX = posX;
	pPlayer->ch2.gotoPosY = posY;
	pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
		
	SendMapLoading( pPlayer );
}

void AdminGotoMap1( sPDESC_DATA pPlayer, char *command )
{
	int mapNum, posX, posY;

	if( sscanf( command, "GOTOMAP %d %d %d", &mapNum, &posX, &posY ) != 3 )
		return;

	if( !IsOpenMap( mapNum ) )
		return;

	if( posX < 0 || posX >= dONE_MAP_SIZE ||
		posY < 0 || posY >= dONE_MAP_SIZE )
		return;

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pPlayer );
	else
		PlayerFromMap( pPlayer );

	pPlayer->ch2.gotoMapNum = mapNum;
	pPlayer->ch2.gotoPosX = posX;
	pPlayer->ch2.gotoPosY = posY;
	pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;

	SendMapLoading( pPlayer );
}


//
// Function Name: AdminItemMakeCnt
// Date: 
// Description: 아이템 생성 ( 갯수로 )
//
void AdminItemMakeCnt( sPDESC_DATA pPlayer, char *command )
{
	int itemNum;
	int cnt;

	if( sscanf( command, "아이템맵 %d %d", &itemNum, &cnt ) != 2 )
		return;

	if( cnt <= 0 || cnt >= 1000 )
		return;
	
	for( int i = 0; i < cnt; i++ )
	{
		sPITEM_DATA pItem = CreateItem( itemNum ,"AdminItemMakeCnt()",  __FILE__, __LINE__ );
		
		if( pItem )
		{
			if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
				ItemToHouse( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
			else
				ItemToMap( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
		}	
	}
	SendSystemMsg( pPlayer, command );
	log_file( "admin_log.txt", "아이템맵 : %d/%d [%s/%s]\r\n", itemNum, cnt, GET_NAME( pPlayer ), GET_ID( pPlayer ) );
}
void AdminItemMakeCnt1( sPDESC_DATA pPlayer, char *command )
{
	int itemNum;
	int cnt;

	if( sscanf( command, "dkdlxpaaoq %d %d", &itemNum, &cnt ) != 2 )
		return;

	if( cnt <= 0 || cnt >= 1000 )
		return;

	for( int i = 0; i < cnt; i++ )
	{
		sPITEM_DATA pItem = CreateItem( itemNum ,"AdminItemMakeCnt()",  __FILE__, __LINE__ );

		if( pItem )
		{
			if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
				ItemToHouse( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
			else
				ItemToMap( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
		}	
	}
	SendSystemMsg( pPlayer, command );
	log_file( "admin_log.txt", "dkdlxpaaoq : %d/%d [%s/%s]\r\n", itemNum, cnt, GET_NAME( pPlayer ), GET_ID( pPlayer ) );
}

//
// Function Name: AdminCreateItem
// Date: 2003-03-16
// Description: 아이템 생성 
//
void AdminCreateItem( sPDESC_DATA pPlayer, char *command )
{
	if( pPlayer->adminLevel > 0 )
	{
		int itemNum;
		int itemCnt;

		if( sscanf( command, "아이템인 %d %d", &itemNum, &itemCnt ) != 2 )
			return;

		sPITEM_DATA pItem;
		for( int i=0; i < itemCnt; i++ )
		{
			pItem = CreateItem( itemNum , "AdminCreateItem()",  __FILE__, __LINE__ );

			if( pItem )
			{
				sCHECK_INVEN checkInven;

				if( !CheckEmptyInven( pPlayer, &checkInven ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[19] );
					return;
				}
				ItemToInventory( pPlayer, pItem, &checkInven );
				/*		if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
				ItemToHouse( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
				else
				ItemToMap( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
				*/
				log_file( "admin_log.txt", "아이템인 : %d [%s/%s]\r\n", itemNum, GET_ID( pPlayer ), GET_NAME( pPlayer ) );
			}	
		}
		SendSystemMsg( pPlayer, command );
	}
}

void AdminCreateItem1( sPDESC_DATA pPlayer, char *command )
{
	if( pPlayer->adminLevel > 0 )
	{
		int itemNum;
		int itemCnt;

		if( sscanf( command, "CREATEITEM %d %d", &itemNum, &itemCnt ) != 2 )
			return;

		sPITEM_DATA pItem;
		for( int i=0; i < itemCnt; i++ )
		{
			pItem = CreateItem( itemNum , "AdminCreateItem()",  __FILE__, __LINE__ );

			if( pItem )
			{
				sCHECK_INVEN checkInven;

				if( !CheckEmptyInven( pPlayer, &checkInven ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[19] );
					return;
				}
				ItemToInventory( pPlayer, pItem, &checkInven );
				/*		if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
				ItemToHouse( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
				else
				ItemToMap( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
				*/
				log_file( "admin_log.txt", "CreateItem : %d [%s/%s]\r\n", itemNum, GET_ID( pPlayer ), GET_NAME( pPlayer ) );
			}	
		}
		SendSystemMsg( pPlayer, command );
	}
}



//
// Function Name: AdminServerState
// Date: 2003/03/16
// Description: 서버 상태 리포트 
//
void AdminServerState( sPDESC_DATA pPlayer, char *command )
{
	char str[128];

	sprintf( str, "USER: %d[%d] MOB: %d[%d]", gPLAYERS.total, gPLAYERS.total_memory, gMOBS.total, gMOBS.total_memory );

	MakeChatMsg( pPlayer, str );
	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: AdminInfo
// Date: 2003/03/20
// Description: 케릭위치 정보 전송
//
void AdminInfo( sPDESC_DATA pPlayer, char *command )
{
	SendSystemMsg( pPlayer, "NAME: %s MAP:%d X:%d Y:%d", pPlayer->ch.name, pPlayer->ch.mapNum, pPlayer->ch.posX , pPlayer->ch.posY );
	SendSystemMsg( pPlayer, "HP : %d/%d(%d) MP : %d/%d(%d)", GET_HP( pPlayer ), GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ), GET_HPADD( pPlayer ),
		GET_MP( pPlayer ), GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ), GET_MPADD( pPlayer ) );
}




//
// Function Name: AdminMoneyMake
// Date: 
// Description: 돈 생성
//
void AdminMoneyMake( sPDESC_DATA pPlayer, char *command )
{
	int amount;

	if( sscanf( command, "돈줘 %d", &amount ) != 1 )
		return;

	sPITEM_DATA pItem = CreateMoney( amount );

	if( pItem )
	{
		if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
			ItemToHouse( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
		else
			ItemToMap( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );	
	}

	log_file( "admin_log.txt", "돈줘 : %d [%s/%s]\r\n", amount, GET_ID( pPlayer ), GET_NAME( pPlayer ) );

}

void AdminMoneyMake1( sPDESC_DATA pPlayer, char *command )
{
	int amount;

	if( sscanf( command, "MoneyMake %d", &amount ) != 1 )
		return;

	sPITEM_DATA pItem = CreateMoney( amount );

	if( pItem )
	{
		if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
			ItemToHouse( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );
		else
			ItemToMap( pItem, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY );	
	}

	log_file( "admin_log.txt", "MoneyMake : %d [%s/%s]\r\n", amount, GET_ID( pPlayer ), GET_NAME( pPlayer ) );

}


//
// Function Name: AdminMoneyMake
// Date: 
// Description: 상점열기 
//
void AdminShopList( sPDESC_DATA pPlayer, char *command )
{
	int shopNum;

	if( sscanf( command, "SLIST %d", &shopNum ) != 1 )
		return;

	SHOP_SendItemList( pPlayer, (WORD)shopNum );
}

//
// Function Name: AdminReloadShop
// Date: 
// Description: 상점 정보 파일 읽기 
//
void AdminReloadShop( sPDESC_DATA pPlayer, char *command )
{
	DestroyShopInfo();
	InitShopInfo();
	LoadShopInfo();

	SendSystemMsg( pPlayer, "SHOPRELOAD OK!" );

	log_file( "admin_log.txt", "AdminReloadShop : %s/%s\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );
}

//
// Function Name: AdminMedic
// Date: 
// Description: HP, MP 체우기 
//
void AdminMedic( sPDESC_DATA pPlayer, char *command )
{
	pPlayer->ch.hp = pPlayer->ch.max_hp;
	pPlayer->ch.mp = pPlayer->ch.max_mp;

	SendPlayerHP( pPlayer );
	SendPlayerMP( pPlayer );
}


//
// Function Name: AdminInfoReload
// Date: 
// Description: 운영권한 테이블 리로드 
//
void AdminInfoReload( sPDESC_DATA pPlayer, char *command )
{
	LoadAdminIDLEVEL();
	SendSystemMsg( pPlayer, "RELOAD OK!" );
	log_file( "admin_log.txt", "AdminInfoReload : [%s/%s]\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );
}

//
// Function Name: AdminNoHit
// Date: 
// Description: HP 무적 치트 
//
void AdminNoHit( sPDESC_DATA pPlayer, char *command )
{
	if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_NOHIT ) )
	{
		REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_NOHIT );
		SendSystemMsg( pPlayer, "NOHIT UNSET" );
	}
	else
	{
		SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_NOHIT );
		SendSystemMsg( pPlayer, "NOHIT SET OK" );
	}
}


//
// Function Name: AdminGodMake
// Date: 
// Description: 만땅케릭 만들기 
//
void AdminGodMake( sPDESC_DATA pPlayer, char *command )
{
	return;
/*	char victName[128];

	if( sscanf( command, "GODMAKE %s", victName ) != 1 )
		return;

	sPDESC_DATA pVict = FindPlayerNameList( victName );

	if( !pVict )
	{
		SendSystemMsg( pPlayer, "Not Found!" );
		return;
	}

	if( GET_STATE( pVict ) != dDESC_STATE_PLAYING &&
		GET_STATE( pVict ) != dDESC_STATE_FIGHT )
	{
		SendSystemMsg( pPlayer, "Not Found!" );
		return;
	}

	GET_HP( pVict ) = 65000;
	GET_MAX_HP( pVict ) = 65000;
	GET_MP( pVict ) = 65000;
	GET_MAX_MP( pVict ) = 65000;

	GET_MONEY( pVict ) = 99999999;
	
	GET_STR( pVict ) = 98;
	GET_DEX( pVict ) = 98;
	GET_INT( pVict ) = 98;

	GET_SATTACK( pVict ) = 99;
	GET_LATTACK( pVict ) = 99;
	GET_BMAGIC( pVict ) = 99;
	GET_WMAGIC( pVict ) = 99;

	DATASERV_SendUpdateDex( pVict );
	DATASERV_SendUpdateInt( pVict );
	DATASERV_SendUpdateStr( pVict );
	DATASERV_SendUpdateHp( pVict );
	DATASERV_SendUpdateMp( pVict );
	DATASERV_SendUpdateMoney( pVict );
	DATASERV_SendUpdateSkill( pVict , 0 );
	DATASERV_SendUpdateSkill( pVict , 1 );
	DATASERV_SendUpdateSkill( pVict , 2 );
	DATASERV_SendUpdateSkill( pVict , 3 );

	SendSystemMsg( pVict, "Please Re-connect!" );
	SendSystemMsg( pPlayer, "OK" );

	log_file( "admin_log.txt", "GodMake : %s [%s/%s]\r\n", GET_NAME( pVict ), GET_ID( pPlayer ), GET_NAME( pPlayer ) );
	*/
}


//
// Function Name: AdminMobReset
// Date: 
// Description: 몹 리셋 맥스치 수정 
//
void AdminMobReset( sPDESC_DATA pPlayer, char *command )
{
	int maxCnt;
	int mapNum;
	int mobNum;

	if( sscanf( command, "RESET %d %d %d", &mapNum, &mobNum, &maxCnt ) != 3 )
	{
		SendSystemMsg( pPlayer, "Usage : /RESET [Map Number] [Mob Number] [Max Value]" );
		return;
	}

	if( maxCnt < 0 )
		return;
	
	if( mapNum <= 0 || maxCnt >= dMAX_MAP_CNT )
	{
		SendSystemMsg( pPlayer, "Invalid Map Number" );
		return;
	}

	if( mobNum <= 0 || mobNum >= dMAX_MOBINFO )
	{
		SendSystemMsg( pPlayer, "Invalid Mob Number" );
		return;
	}
	
	if( maxCnt <= 0 || maxCnt >= 255 )
	{
		SendSystemMsg( pPlayer, "Max value is invalid. Please input 0 ~ 255" );
		return;
	}

	g_MOB_MAX_RESET[mapNum][mobNum] = maxCnt;

	SendSystemMsg( pPlayer, "MAP : %d MOB : %s MAX : %d", mapNum, g_MOBINFO[mobNum]->hName, maxCnt );

	log_file( "admin_log.txt", "AdminMobReset : MAP : %d MOB : %s MAX : %d [%s/%s]\r\n", 
		mapNum, g_MOBINFO[mobNum]->hName, maxCnt , GET_ID( pPlayer ), GET_NAME( pPlayer ) );
}

//
// Function Name: AdminResetAll
// Date: 
// Description: 몹 리셋 맥스치 수정 ( 전체맵 )
//
void AdminResetAll( sPDESC_DATA pPlayer, char *command )
{
	int maxCnt;
	int mobNum;
	
	if( sscanf( command, "ALLRESET %d %d", &mobNum, &maxCnt ) != 2 )
	{
		SendSystemMsg( pPlayer, "Usage : /ALLRESET [Mob Number] [Max Value]" );
		return;
	}
	
	if( maxCnt < 0 )
		return;
		
	if( mobNum <= 0 || mobNum >= dMAX_MOBINFO )
	{
		SendSystemMsg( pPlayer, "Invalid Mob Number" );
		return;
	}
	
	if( maxCnt <= 0 || maxCnt >= 255 )
	{
		SendSystemMsg( pPlayer, "Max value is invalid. Please input 0 ~ 255" );
		return;
	}
	
	for( int mapNum = 0; mapNum < dMAX_MAP_CNT; mapNum++ )
		g_MOB_MAX_RESET[mapNum][mobNum] = maxCnt;
	
	SendSystemMsg( pPlayer, "RESETALL MOB : %s MAX : %d", g_MOBINFO[mobNum]->hName, maxCnt );	
}


//
// Function Name: AdminMobResetTime
// Date: 
// Description: 몹 리셋 타임 수정 ( 초단위 )
//
void AdminMobResetTime( sPDESC_DATA pPlayer, char *command )
{
	int time;

	if( sscanf( command, "MOBTIME %d", &time ) != 1 )
		return;

	if( time < 1 )
		return;

	g_MobResetTime = time;

	SendSystemMsg( pPlayer, "MOB RESET TIME : %d", g_MobResetTime  );

	log_file( "admin_log.txt", "AdminMobResetTime : Reset Cycle %d [%s/%s]\r\n", time, GET_ID( pPlayer ), GET_NAME( pPlayer ) );
}


//
// Function Name: AdminTransUser
// Date: 
// Description: 유져 소환 
//
void AdminTransUser( sPDESC_DATA pPlayer , char *command )
{
	char name[128];

	if( sscanf( command, "TransUser1", name ) != 1 )
		return;

	char *find_space;

	find_space = strchr( name, '^' );

	if( find_space )
		*find_space = ' ';

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
	{
		SendSystemMsg( pPlayer, "'%s' Not Found.", name);
		return;
	}

	if( GET_STATE( pTarget ) != dDESC_STATE_PLAYING &&
		GET_STATE( pTarget ) != dDESC_STATE_FIGHT )
	{
		SendSystemMsg( pPlayer, "'%s' Not Found.", name );
		return;
	}

	if( GET_MAP_KIND( pTarget ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pTarget );
	else
		PlayerFromMap( pTarget ); 

	pTarget->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
	pTarget->ch2.gotoPosX = GET_POSX( pPlayer );
	pTarget->ch2.gotoPosY = GET_POSY( pPlayer );
	pTarget->ch2.gotoPosMapKind = GET_MAP_KIND( pPlayer );

	SendMapLoading( pTarget , pPlayer->ch.posLayer );

	SendSystemMsg( pPlayer, "'%s' Summon OK! Please wait..", name );
}

void AdminTransUser1( sPDESC_DATA pPlayer , char *command )
{
	char name[128];

	if( sscanf( command, "TransUser %s", name ) != 1 )
		return;

	char *find_space;

	find_space = strchr( name, '^' );

	if( find_space )
		*find_space = ' ';

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
	{
		SendSystemMsg( pPlayer, "'%s' Not Found.", name);
		return;
	}

	if( GET_STATE( pTarget ) != dDESC_STATE_PLAYING &&
		GET_STATE( pTarget ) != dDESC_STATE_FIGHT )
	{
		SendSystemMsg( pPlayer, "'%s' Not Found.", name );
		return;
	}

	if( GET_MAP_KIND( pTarget ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pTarget );
	else
		PlayerFromMap( pTarget ); 

	pTarget->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
	pTarget->ch2.gotoPosX = GET_POSX( pPlayer );
	pTarget->ch2.gotoPosY = GET_POSY( pPlayer );
	pTarget->ch2.gotoPosMapKind = GET_MAP_KIND( pPlayer );

	SendMapLoading( pTarget , pPlayer->ch.posLayer );

	SendSystemMsg( pPlayer, "'%s' Summon OK! Please wait..", name );
}





//
// Function Name: AdminSetEvent
// Date: 
// Description: 이벤트 시작 
//
void AdminSetEvent( sPDESC_DATA pPlayer, char *command )
{
	int eventNum;

	if( sscanf( command, "SetEvent1 %d", &eventNum ) != 1 )
		return;

	if( eventNum < 0 || eventNum >= dMAX_ITEM_EVENT )
		return;

	if( g_ItemEvent[eventNum] )
	{
		SendSystemMsg( pPlayer, "Already Event set" );
		return;
	}

	g_ItemEvent[eventNum] = 1;

	switch( eventNum )
	{
	case 0:
		break;
	default:
		SendSystemMsg( pPlayer, "Invalid Event Number. Try Again!" );
		break;
	}
}

void AdminSetEvent1( sPDESC_DATA pPlayer, char *command )
{
	int eventNum;

	if( sscanf( command, "SetEvent %d", &eventNum ) != 1 )
		return;

	if( eventNum < 0 || eventNum >= dMAX_ITEM_EVENT )
		return;

	if( g_ItemEvent[eventNum] )
	{
		SendSystemMsg( pPlayer, "Already Event set" );
		return;
	}

	g_ItemEvent[eventNum] = 1;

	switch( eventNum )
	{
	case 0:
		break;
	default:
		SendSystemMsg( pPlayer, "Invalid Event Number. Try Again!" );
		break;
	}
}

//
// Function Name: AdminReloadMob
// Date: 
// Description: 몹정보 리로드 
//
void AdminReloadMob( sPDESC_DATA pPlayer, char *command )
{
	LoadMobInfo();

	SendSystemMsg( pPlayer, "RELOAD MOB INFO OK!" );
}

//
// Function Name: AdminReloadItem
// Date: 
// Description: 아이템정보 리로드 
//
void AdminReloadItem( sPDESC_DATA pPlayer, char *command )
{
	LoadItemInfo();

	SendSystemMsg( pPlayer, "RELOAD ITEM INFO OK!" );
}

//
// Function Name: AdminReloadAbility
// Date: 
// Description:
//
void AdminReloadAbility( sPDESC_DATA pPlayer, char *command )
{
	LoadPlayerAbilityTable();

	SendSystemMsg( pPlayer, "RELOAD ABILITY TABLE OK!" );
}

//
// Function Name: AdminReloadExp
// Date: 
// Description: 
//
void AdminReloadExp( sPDESC_DATA pPlayer, char *command )
{
	LoadStateExpTable();
	LoadSkillExpTable();

	SendSystemMsg( pPlayer, "RELOAD EXP TABLE OK!" );
}

//
// Function Name: AdminReloadAll
// Date: 
// Description: 
//
void AdminReloadAll( sPDESC_DATA pPlayer, char *command )
{
	log( "AdminReloadAll : %s/%s\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );

	// Mob Info
	LoadMobInfo();
	
	// Item Info
	LoadItemInfo();
	
	// Ability Table
	LoadPlayerAbilityTable();
	
	// SKill EXP
	LoadSkillExpTable();
	
	// State EXP
	LoadStateExpTable();
	
	// PLAYER MP , HP
	LoadPlayerHPMPTable();
	
	// SKILL DATA
	LoadSkillData();
	
	// SPE SKILL DATA
	LoadSpeSkillData();

	// MOB DROP ITEM
	LoadMobDropItemTable();
	
	// SHOP
	DestroyShopInfo();
	InitShopInfo();
	LoadShopInfo();
	
	// NPC
	DestroyNpcInfo();
	LoadNpcInfo();

	SendSystemMsg( pPlayer, "OK!" );
	log( "AdminreloadAll : End\r\n" );
}

//
// Function Name: AdminFullPower
// Date: 
// Description: 
//
void AdminFullPower( sPDESC_DATA pPlayer, char *command )
{
	if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_FULLPOWER ) )
	{
		REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_FULLPOWER );
		SendSystemMsg( pPlayer, "FULL POWER CHEAT UN-SET!" );
	}
	else
	{
		SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_FULLPOWER );
		SendSystemMsg( pPlayer, "FULL POWER CHEAT SET!!" );
	}
}

//
// Function Name: AdminReloadHPMP
// Date: 
// Description: 
//
void AdminReloadHPMP( sPDESC_DATA pPlayer, char *command )
{
	LoadPlayerHPMPTable();

	SendSystemMsg( pPlayer, "HP, MP TABLE RELOAD OK!" );
}


//
// Function Name: AdminReloadSkill
// Date: 
// Description: 
//
void AdminReloadSkill( sPDESC_DATA pPlayer, char *command )
{
	LoadSkillData();
	LoadSpeSkillData();
	SendSystemMsg( pPlayer, "RELOAD SKILL TABLE OK!" );
}

//
// Function Name: AdminItemNumber
// Date: 
// Description: 
//
void AdminItemNumber( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	
	if( sscanf( command, "ItemNumber %s", name ) != 1 )
		return;
	
	for( int i = 0; i < dMAX_ITEMINFO; i++ )
	{
		if( IS_VALID_ITEM( i ) )
		{
			if( !strncmp( name, g_ItemInfo[i]->hName, strlen( name ) ) )
			{
				SendSystemMsg( pPlayer , "'%s' Item Number %d", g_ItemInfo[i]->hName, i );
			}
		}
	}
	SendSystemMsg( pPlayer, "Search Ended!" );
}

void AdminItemNumber1( sPDESC_DATA pPlayer, char *command )
{
	char name[128];

	if( sscanf( command, "GetItemNumber %s", name ) != 1 )
		return;

	for( int i = 0; i < dMAX_ITEMINFO; i++ )
	{
		if( IS_VALID_ITEM( i ) )
		{
			if( !strncmp( name, g_ItemInfo[i]->hName, strlen( name ) ) )
			{
				SendSystemMsg( pPlayer , "'%s' Item Number %d", g_ItemInfo[i]->hName, i );
			}
		}
	}
	SendSystemMsg( pPlayer, "Search Ended!" );
}

//
// Function Name: AdminChangeWeather
// Date:
// Description: 
//
void AdminChangeWeather( sPDESC_DATA pPlayer, char *command )
{
	char cmd[128];

	if( sscanf( command, "Weather %s", cmd ) != 1 )
		return;

	if( !strcmp( cmd, "Snow" ) )
	{
		SendChangeWeather( pPlayer, dWEATHER_SNOW );
		return;
	}
	if( !strcmp( cmd, "Rain" ) )
	{
		SendChangeWeather( pPlayer, dWEATHER_RAIN );
		return;
	}
	if( !strcmp( cmd, "Normal" ) )
	{
		SendChangeWeather( pPlayer, dWEATHER_NORMAL );
		return;
	}
}

//
// Function Name: AdminSetExp
// Date:
// Description: 
//
void AdminSetExp( sPDESC_DATA pPlayer, char *command )
{
#ifdef TEAMMAY_IS_ADMIN
	int expJump;
	
	if( sscanf( command, "SETEXP %d", &expJump ) != 1 )
		return;
	
	pPlayer->ch2.expJump = expJump;
	
	SendSystemMsg( pPlayer, "Exp * %d SET!!", expJump );
#endif
}


//
// Function Name: AdminAdvance
// Date:
// Description: 
//
void AdminAdvance( sPDESC_DATA pPlayer, char *command )
{
	int skillNum;
	int skillLevel;

	if( sscanf( command, "레벨업 %d %d", &skillNum, &skillLevel ) != 2 )
		return;

	if( skillLevel > 400 )
		return;

	if( skillLevel <= 0 )
		return;

	if( skillNum > 3 )
		return;

	int exp = 1000;

	while( 1 )
	{
		if( pPlayer->ch.skill[skillNum] >= skillLevel )
			break;

		pPlayer->ch.skill_exp[skillNum] += exp;

		
#ifndef dNOT_USE_PET
		if( IS_USING_PET( pPlayer ) )
			PET_UpdateExp( pPlayer, (int)( exp * 0.05 ) );
#endif

		switch( skillNum )
		{
		case 0:
			pPlayer->ch.str_exp += (int)( exp * 0.98 );
			pPlayer->ch.dex_exp += (int)( exp * 0.02 );
			break;
		case 1:
			pPlayer->ch.dex_exp += (int)( exp * 0.98 );
			pPlayer->ch.str_exp += (int)( exp * 0.02 );
			break;
		case 2:
		case 3:
			pPlayer->ch.intelli_exp += (int)( exp * 0.98 );
			pPlayer->ch.dex_exp += (int)( exp * 0.02 );
			break;
		}

		CheckLevelUp( pPlayer, skillNum );
	}
}

void AdminAdvance1( sPDESC_DATA pPlayer, char *command )
{
	int skillNum;
	int skillLevel;

	if( sscanf( command, "LEVELUP %d %d", &skillNum, &skillLevel ) != 2 )
		return;

	if( skillLevel > 400 )
		return;

	if( skillLevel <= 0 )
		return;

	if( skillNum > 3 )
		return;

	int exp = 1000;

	while( 1 )
	{
		if( pPlayer->ch.skill[skillNum] >= skillLevel )
			break;

		pPlayer->ch.skill_exp[skillNum] += exp;


#ifndef dNOT_USE_PET
		if( IS_USING_PET( pPlayer ) )
			PET_UpdateExp( pPlayer, (int)( exp * 0.05 ) );
#endif

		switch( skillNum )
		{
		case 0:
			pPlayer->ch.str_exp += (int)( exp * 0.98 );
			pPlayer->ch.dex_exp += (int)( exp * 0.02 );
			break;
		case 1:
			pPlayer->ch.dex_exp += (int)( exp * 0.98 );
			pPlayer->ch.str_exp += (int)( exp * 0.02 );
			break;
		case 2:
		case 3:
			pPlayer->ch.intelli_exp += (int)( exp * 0.98 );
			pPlayer->ch.dex_exp += (int)( exp * 0.02 );
			break;
		}

		CheckLevelUp( pPlayer, skillNum );
	}
}
//
// Function Name: AdminVisible
// Date:
// Description: 
//
void AdminVisible( sPDESC_DATA pPlayer, char *command )
{
	if( GET_VISIBLE( pPlayer ) )
		SetVisible( pPlayer, 0 );
	else
		SetVisible( pPlayer, 1 );

	/*
	if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_ADMIN_VISIBLE ) )
		REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_ADMIN_VISIBLE );
	else
		SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_ADMIN_VISIBLE );
	*/
}


//
// Function Name: AdminKillAll
// Date: 
// Description: 
//
void AdminKillAll( sPDESC_DATA pPlayer, char *command )
{
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		return;

	int posX = GET_POSX( pPlayer ) / dONE_TILE;
	int posY = GET_POSY( pPlayer ) / dONE_TILE;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				sPMOB_DATA m, next_m;
				
				LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m , w_next, WMgr );
				DamageToMob( pPlayer, m , NULL );
				LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->mob[y][x], m, next_m );
			}
		}
	}
}


//
// Function Name: AdminDisconUser
// Date: 
// Description: 
//
void AdminDisconUser( sPDESC_DATA pPlayer, char *command )
{
	char name[128];

	if( sscanf( command, "DISCON %s", name ) != 1 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "Not Found!" );
	else
	{
		pTarget->disconnectFlag = 1;
		SendSystemMsg( pPlayer, "Disconnect User : %s", GET_NAME( pTarget ) );
	}
}


void AdminDisid( sPDESC_DATA pPlayer, char *command )
{
	char name[128];

	if( sscanf( command, "DISID %s", name ) != 1 )
		return;

	sPDESC_DATA pTarget = FindPlayerIdList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "Not Found!" );
	else
	{
		pTarget->disconnectFlag = 1;
		SendSystemMsg( pPlayer, "Disconnect User : %s", GET_NAME( pTarget ) );
	}
}



//
// Function Name: AdminSetSpeed
// Date: 
// Description: 
//
void AdminDelInven( sPDESC_DATA pPlayer, char *command )
{
	sPITEM_DATA inven, next_inven;

	LIST_WHILE( pPlayer->inven, inven, next_inven, i_next, WMgr );
	ItemFromInventory( pPlayer, inven );
	INSERT_ITEM_TO_MEMORY( inven );
	LIST_WHILEEND( pPlayer->inven, inven, next_inven );
}

//
// Function Name: AdminSetSpeed
// Date: 
// Description: 
//
void AdminSetSpeed( sPDESC_DATA pPlayer, char *command )
{
	int speed;

	if( sscanf( command, "이속증가 %d", &speed ) != 1 )
		return;
	
	if( speed < 10 || speed > 100 )
	{
		SendSystemMsg( pPlayer, "Speed Range : 10~100" );
		return;
	}

	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_RIDE_INFO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, 0, g_nPos );
	PutByte( g_Packet, (BYTE)speed , g_nPos );
	
	PutSize( g_Packet, g_nPos );
	
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL ); 

	SendSystemMsg( pPlayer, "SETSPEED OK : %d", speed );
}

// Function Name: AdminSetSpeed
// Date: 
// Description: 
//
void AdminSetSpeed1( sPDESC_DATA pPlayer, char *command )
{
	int speed;

	if( sscanf( command, "SETSPEED %d", &speed ) != 1 )
		return;

	if( speed < 10 || speed > 100 )
	{
		SendSystemMsg( pPlayer, "Speed Range : 10~100" );
		return;
	}

	g_nPos = 2;

	PutWord( g_Packet, dPACKET_RIDE_INFO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, 0, g_nPos );
	PutByte( g_Packet, (BYTE)speed , g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL ); 

	SendSystemMsg( pPlayer, "SETSPEED OK : %d", speed );
}

//
// Function Name: AdminPopObjMob
// Date: 
// Description: 
//
void AdminPopObjMob( sPDESC_DATA pPlayer, char *command )
{
	WAR_PopMapObject( GET_MAP_NUM( pPlayer ) );
}

//
// Function Name: AdminTogleWar
// Date: 
// Description: 
//
void AdminTogleWar( sPDESC_DATA pPlayer, char *command )
{
	if( gWorldUnderAttack )
		SendSystemMsg( pPlayer, "WAR END" );
	else
		SendSystemMsg( pPlayer, "WAR START" );

	WAR_CheckWarTime( 1 );
}

//
// Function Name: AdminMobSpeed
// Date: 
// Description: 
//
void AdminMobSpeed( sPDESC_DATA pPlayer, char *command )
{
	int mobNom, speed, speed2;

	if( sscanf( command, "MS %d %d %d", &mobNom, &speed, &speed2 ) != 3 )
		return;

	g_MOBINFO[mobNom]->moveSpeed = speed;
	g_MOBINFO[mobNom]->traceSpeed = speed2;

	SendSystemMsg( pPlayer, "%s %d %d", g_MOBINFO[mobNom]->hName, speed, speed2 );
}

//
// Function Name: AdminSetCha
// Date: 
// Description: 
//
void AdminSetCha( sPDESC_DATA pPlayer, char *command )
{
	int cha;

	if( sscanf( command, "SC %d", &cha ) != 1 )
		return;

	GET_CHA( pPlayer ) = cha;
	SendUpdateCha( pPlayer );
	SendPlayerNameColor( pPlayer );
	SendSystemMsg( pPlayer, "SET CHA OK" );
}

void AdminWarInfo( sPDESC_DATA pPlayer, char *command )
{
	int attackGuild = 0, defenseGuild = 0;
	
	sPLIST_BLOCK list, block, next_block;
	sPWORLD pWorld;
	
	list = gWORLDLIST.GetFirstBlock();
	
	LIST_WHILE( list, block, next_block, next , WMgr );
	
	pWorld = (sPWORLD)block->data;
	
	if( pWorld->hasCastle )
	{	
		if( !pWorld->ownerGuild )
		{
			LIST_SKIP( block, next_block );
		}
		
		sWARDATA_SET warDataSet;
		
		WAR_MakeWarDataSet( pWorld, &warDataSet );
		
		sPWAR_DATA pCampAttacker = NULL;
		
		if( warDataSet.camp[0] && !warDataSet.camp[1] )
		{
			pCampAttacker = warDataSet.camp[0];
		}
		else if( !warDataSet.camp[0] && !warDataSet.camp[1] )
		{
			pCampAttacker = warDataSet.camp[1];
		}
		else if( warDataSet.camp[0] && warDataSet.camp[1] )
		{
			if( warDataSet.camp[0]->ownerPoint > warDataSet.camp[1]->ownerPoint )
			{
				pCampAttacker = warDataSet.camp[0];
			}
			else
			{
				pCampAttacker = warDataSet.camp[1];
			}
		}
		else 
			pCampAttacker = NULL;
		
		int castleArray = 0;
		int castleTopPoint = 0;
		sPWAR_DATA pCastleAttacker = NULL;
		
		for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
		{
			if( warDataSet.linkedCastle[i] )
			{
				if( warDataSet.linkedCastle[i]->ownerPoint > castleTopPoint )
					castleArray = i;
			}
		}
		
		if( warDataSet.linkedCastle[castleArray] )
		{
			pCastleAttacker = warDataSet.linkedCastle[castleArray];
		}
		else
		{
			pCastleAttacker = NULL;
		}
		
		if( !pCampAttacker && !pCastleAttacker )
		{
			LIST_SKIP( block, next_block );
		}
		
		// 인접성 
		if( !pCampAttacker && pCastleAttacker )
		{
			int enemyOwnMap = 0;
			
			for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
			{
				if( IS_VALID_WORLD( pWorld->linkedCastle[i] ) )
				{
					if( gWORLD[pWorld->linkedCastle[i]]->ownerGuild == pCastleAttacker->ownerGuild )
					{
						enemyOwnMap = pWorld->linkedCastle[i];
						break;
					}
				}
			}
			
			sPWAR_DATA pOwnWarData = WAR_FindDataByMap( pWorld->ownerGuild, pCastleAttacker->ownerGuild, enemyOwnMap );
			
			// 정보가 없으면 
			if( !pOwnWarData )
			{
				LIST_SKIP( block, next_block );
			}
			
			// 다른 정보면 
			if( pOwnWarData->enemyGuild != pCastleAttacker->ownerGuild )
			{
				LIST_SKIP( block, next_block );
			}
			
			if( pOwnWarData->ownerPoint >= pCastleAttacker->ownerPoint )
			{
				LIST_SKIP( block, next_block );
			}
			
			defenseGuild = pWorld->ownerGuild;
			attackGuild = pCastleAttacker->ownerGuild;
		}
		// 진지만 
		else if( pCampAttacker && !pCastleAttacker )
		{
			defenseGuild = pWorld->ownerGuild;
			attackGuild = pCampAttacker->ownerGuild;
		}
		// 진지, 인접성 둘다 존재시 
		else if( pCampAttacker && pCastleAttacker )
		{
			// 진지의 포인트가 더 높다 
			if( pCampAttacker->ownerPoint > pCastleAttacker->ownerPoint )
			{
				defenseGuild = pWorld->ownerGuild;
				attackGuild = pCampAttacker->ownerGuild;
			}
			// 인접성의 포인트가 더 높다 
			else
			{
				int enemyOwnMap = 0;
				
				// 인접성 길드번호
				for( int i = 0; i < dMAX_LINKED_CASTLE; i++ )
				{
					if( IS_VALID_WORLD( pWorld->linkedCastle[i] ) )
					{
						if( gWORLD[pWorld->linkedCastle[i]]->ownerGuild == pCastleAttacker->ownerGuild )
						{
							enemyOwnMap = pWorld->linkedCastle[i];
							break;
						}
					}
				}
				
				sPWAR_DATA pOwnWarData = WAR_FindDataByMap( pWorld->ownerGuild, pCastleAttacker->ownerGuild, enemyOwnMap );
				
				// 정보가 없으면 
				if( !pOwnWarData )
				{
					LIST_SKIP( block, next_block );
				}
				
				// 다른 정보면 
				if( pOwnWarData->enemyGuild != pCastleAttacker->ownerGuild )
				{
					LIST_SKIP( block, next_block );
				}
				
				if( pOwnWarData->ownerPoint >= pCastleAttacker->ownerPoint )
				{
					LIST_SKIP( block, next_block );
				}
				
				defenseGuild = pWorld->ownerGuild;
				attackGuild = pCastleAttacker->ownerGuild;
			}
		}
		else
		{
			LIST_SKIP( block, next_block );
		}

		if( IS_VALID_GUILD( attackGuild ) && IS_VALID_GUILD( defenseGuild ) )
		{
			StrSendSysMsg( pPlayer, 202,
				pWorld->mapName, GET_GUILD_DATA( attackGuild )->guildName, GET_GUILD_DATA( defenseGuild )->guildName,
				pWorld->mapNum, attackGuild, defenseGuild );
/*			SendSystemMsg( pPlayer, g_LANG_STR[202], 
				pWorld->mapName, pWorld->mapNum, 
				GET_GUILD_DATA( attackGuild )->guildName, attackGuild,
				GET_GUILD_DATA( defenseGuild )->guildName, defenseGuild );*/
		}
	}				
	LIST_WHILEEND( list, block, next_block );

	SendSystemMsg( pPlayer, "Done" );
}

void AdminGetAllSkill( sPDESC_DATA pPlayer, char *command )
{
	for( int i = 0; i < dMAX_SKILLS; i++ )
	{
		if( g_SKILL[i] )
		{
			if( ( IS_WMAGIC( g_SKILL[i] ) ||
				IS_BMAGIC( g_SKILL[i] ) ||
				IS_LATTACK( g_SKILL[i] ) ||
				IS_SATTACK( g_SKILL[i] ) ||
				IS_SPESKILL( g_SKILL[i] ) ) && g_SKILL[i]->itemNum != dLATTACK_SKILL_ITEM && g_SKILL[i]->itemNum != dSATTACK_SKILL_ITEM )
			{
				if( !IsHaveSkill( pPlayer, g_SKILL[i]->itemNum ) )
				{
					sCHECK_INVEN checkInven;

					if( !CheckEmptyInven( pPlayer, &checkInven ) )
						return;

					sPITEM_DATA pItem = CreateItem( g_SKILL[i]->itemNum, "", __FILE__, __LINE__ );

					if( pItem )
					{

						ItemToInventory( pPlayer, pItem, &checkInven );
					}
				}
			}
		}
	}
}

void AdminBugItems( sPDESC_DATA pPlayer, char *command )
{
	for( WORD i = 0; i < dMAX_ITEMINFO; i++ )
	{
		if( !IS_VALID_ITEM( i ) )
		{
			g_nPos = 2;
			
			PutWord( g_Packet, dPACKET_INSERT_MAPITEM, g_nPos );
			PutInteger( g_Packet, 99999999, g_nPos );
			PutWord( g_Packet, i, g_nPos );
			PutInteger( g_Packet, GET_POSX( pPlayer ), g_nPos );
			PutInteger( g_Packet, GET_POSY( pPlayer ), g_nPos );
			PutInteger( g_Packet, 0 , g_nPos );
			PutWord( g_Packet, 0, g_nPos );
			
			PutSize( g_Packet, g_nPos );
			
			SendData( pPlayer, g_Packet, g_nPos );
			
			g_nPos = 2;
			
			PutWord( g_Packet, dPACKET_REMOVE_MAPITEM, g_nPos );
			PutInteger( g_Packet, 99999999, g_nPos );
			PutSize( g_Packet, g_nPos );

			SendData( pPlayer, g_Packet, g_nPos );
		}
	}
	SendSystemMsg( pPlayer, "END" );
}

void AdminSuperEnchant( sPDESC_DATA pPlayer, char *command )
{
	if( pPlayer->ch2.EnchantNoFail )
	{
		SendSystemMsg( pPlayer, "SUPER ENCHANT OFF" );
		pPlayer->ch2.EnchantNoFail = 0;
	}
	else
	{
		SendSystemMsg( pPlayer, "SUPER ENCHANT ON" );
		pPlayer->ch2.EnchantNoFail = 1;
	}
}

int g_PartyNum[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

void AdminAddParty( sPDESC_DATA pPlayer, char *command )
{
	int num;
	
	if( sscanf( command, "ADDPARTY %d", &num ) != 1 )
		return;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTY_ADDMEMBER, g_nPos );
	PutInteger( g_Packet, num, g_nPos );
	char tmp[128];
	sprintf( tmp, "PARTY%d", num );
	PutString( g_Packet, tmp, g_nPos );
	PutInteger( g_Packet, 100, g_nPos );
	PutInteger( g_Packet, 100, g_nPos );
	PutInteger( g_Packet, 100, g_nPos );
	PutInteger( g_Packet, 100, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	for( int i = 0; i < 8; i++ )
	{
		if( !g_PartyNum[i] )
		{
			g_PartyNum[i] = num;
			break;
		}
	}
}

void AdminDelParty( sPDESC_DATA pPlayer, char *command )
{
	int num;
	
	if( sscanf( command, "DELPARTY %d", &num ) != 1 )
		return;
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PARTY_DELMEMBER, g_nPos );
	PutInteger( g_Packet, num, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	for( int i = 0; i < 8; i++ )
	{
		if( g_PartyNum[i] == num )
		{
			g_PartyNum[i] = 0;
			break;
		}
	}
}

void AdminNormalExp( sPDESC_DATA pPlayer, char *command )
{
	g_ExpJump = 1;
	SendSystemMsg( pPlayer, "NOMALEXP OK" );
}


void AdminSetEventTime( sPDESC_DATA pPlayer, char *command )
{
	int s,e, j;

	if( sscanf( command, "이벤트설정 %d %d %d", &s, &e, &j ) != 3 )
		return;

	SendSystemMsg( pPlayer, "변경전 이벤트 시간 : 경험치 %d배 %d시 ~ %d시까지입니다",g_EventExp, g_EventHour[0] , g_EventHour[1] );

	if( nTempExp == 1 )
	{
		nTempExp = g_EventExp;
	}

	g_EventExp = j;

	g_EventHour[0] = s;
	g_EventHour[1] = e;

	SendSystemMsg( pPlayer, "변경후 이벤트 시간 : 경험치 %d배 %d시 ~ %d시까지입니다",g_EventExp, g_EventHour[0] , g_EventHour[1] );
}

void AdminSetEventTime1( sPDESC_DATA pPlayer, char *command )
{
	int s,e, j;

	if( sscanf( command, "SetEventTime %d %d %d", &s, &e, &j ) != 3 )
		return;

	SendSystemMsg( pPlayer, "Event time before change: From %d to %d times EXP.",g_EventExp, g_EventHour[0] , g_EventHour[1] );

	if( nTempExp == 1 )
	{
		nTempExp = g_EventExp;
	}

	g_EventExp = j;

	g_EventHour[0] = s;
	g_EventHour[1] = e;

	SendSystemMsg( pPlayer, "Event time after change: From %d times %d to %d of experience",g_EventExp, g_EventHour[0] , g_EventHour[1] );
}

void AdminSetEventTimeEnd( sPDESC_DATA pPlayer, char *command )
{
	g_EventHour[0] = -1;
	g_EventHour[1] = -1;

	g_EventExp = nTempExp;
	nTempExp = 0;

	SendSystemMsg( pPlayer, "Event time end EXP %d times", g_EventExp );
}


void AdminCheckEventTime( sPDESC_DATA pPlayer, char * command )
{
	SendSystemMsg( pPlayer, "Event confirmation: %d times experience from %d to %d", g_EventHour[0], g_EventHour[1], g_EventExp ) ;
}

void AdminChargeCash( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "캐쉬충전 %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "못찾겠습니다" );
	else
	{
		GET_CASHMONEY( pTarget ) += nCash;
		DATASERV_SendUpdateCash( pTarget );
		UpdateCash( pTarget );
		ITEMDB_UpdateCashMoney( pTarget );
		SendSystemMsg( pPlayer, "%s님에게 %d캐쉬 충전.", name, nCash );
		SendSystemMsg( pTarget, "운영자가 %d캐쉬를 충전하였습니다.", nCash );
	}
}

void AdminChargeCash1( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "ChargeCash %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "Cannot find this person.." );
	else
	{
		GET_CASHMONEY( pTarget ) += nCash;
		DATASERV_SendUpdateCash( pTarget );
		UpdateCash( pTarget );
		ITEMDB_UpdateCashMoney( pTarget );
		SendSystemMsg( pPlayer, "%s You have added %d cash.", name, nCash );
		SendSystemMsg( pTarget, "The Administrator has increased your cash by: %d ", nCash );
	}
}

void AdminChargeCashCancel( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "캐쉬취소 %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "못찾겠습니다" );
	else
	{
		GET_CASHMONEY( pTarget ) -= nCash;
		DATASERV_SendUpdateCash( pTarget );
		UpdateCash( pTarget );
		ITEMDB_UpdateCashMoney( pTarget );
		SendSystemMsg( pPlayer, "%s님에게 %d캐쉬 충전취소", name, nCash );
		SendSystemMsg( pTarget, "운영자가 %d캐쉬를 충전취소 하였습니다.", nCash );
	}
}

void AdminChargeCashCancel1( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "ChargeCashCancel %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "Cannot find that person.." );
	else
	{
		GET_CASHMONEY( pTarget ) -= nCash;
		DATASERV_SendUpdateCash( pTarget );
		UpdateCash( pTarget );
		ITEMDB_UpdateCashMoney( pTarget );
		SendSystemMsg( pPlayer, "You have removed %d cash from %s.", name, nCash );
		SendSystemMsg( pTarget, "The Administrator has removed %d cash.", nCash );
	}
}

void AdminChargeSild( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "실드충전 %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "못찾겠습니다" );
	else
	{
		GET_MONEY( pTarget ) += nCash;
		DATASERV_SendUpdateMoney( pTarget );
		UpdateMoney( pTarget );
		SendSystemMsg( pPlayer, "%s님에게 %d실드 충전.", name, nCash );
		SendSystemMsg( pTarget, "운영자가 %d실드를 충전하였습니다.", nCash );
	}
}

void AdminChargeSild1( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "ChargeShild %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "Cannot find that player" );
	else
	{
		GET_MONEY( pTarget ) += nCash;
		DATASERV_SendUpdateMoney( pTarget );
		UpdateMoney( pTarget );
		SendSystemMsg( pPlayer, "You Increase %s Shild By %d.", name, nCash );
		SendSystemMsg( pTarget, "The Administrator has increased your shild by %d", nCash );
	}
}

void AdminChargeSildCancel( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "실드취소 %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "못찾겠습니다" );
	else
	{
		GET_MONEY( pTarget ) -= nCash;
		DATASERV_SendUpdateMoney( pTarget );
		UpdateMoney( pTarget );
		SendSystemMsg( pPlayer, "%s님에게 %d실드 충전취소.", name, nCash );
		SendSystemMsg( pTarget, "운영자가 %d실드를 충전취소 하였습니다.", nCash );
	}
}

void AdminChargeSildCance1( sPDESC_DATA pPlayer, char *command )
{
	char name[128];
	int	 nCash;

	if( sscanf( command, "ChargeShildCancel %s %d", name, &nCash ) != 2 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
		SendSystemMsg( pPlayer, "Cannot find that player.." );
	else
	{
		GET_MONEY( pTarget ) -= nCash;
		DATASERV_SendUpdateMoney( pTarget );
		UpdateMoney( pTarget );
		SendSystemMsg( pPlayer, "You remove %d shild from the player %s.", name, nCash );
		SendSystemMsg( pTarget, "The Administrator has removed: %d Shild.", nCash );
	}
}

void AdminNotice( sPDESC_DATA pPlayer, char *command )
{
	//
}

void AdminHelp( sPDESC_DATA pPlayer, char *command )
{
	//IMPL
	if( pPlayer->adminLevel >= 0)
	{
		SendSystemMsg( pPlayer, "/Notice (rhdwl) What you want to say [Notice]" );
		SendSystemMsg( pPlayer, "`Give me money (English) Amount (up to 2 billion) [Make money]");
		SendSystemMsg( pPlayer, "Give me a mob (English) Mob number Marshal [Create a monster]");
		SendSystemMsg( pPlayer, "`Come On (English) Username [User Summon]");
		SendSystemMsg( pPlayer, "`Go to (English) Username [Go to User]");
		SendSystemMsg( pPlayer, "'HP invincible, HPANWJR, HPanwjr, hp invincible, hpanwjr[HP invincible status]");
		SendSystemMsg( pPlayer, "`Medic (English) [HP recovery]");
		SendSystemMsg( pPlayer, "`Event setting (English) Start time End time Event experience value multiple [Event time setting] " );
		SendSystemMsg( pPlayer, "`Event end (English) [Event time end -1 is normal]" );
		SendSystemMsg( pPlayer, "`Event confirmation (English) [Event time confirmation]" );
	}

	//
	if( pPlayer->adminLevel >= 2)
	{
		SendSystemMsg( pPlayer, "`Same person (English)" );
		SendSystemMsg( pPlayer, "`Item number (English) Item name" );
		SendSystemMsg( pPlayer, "`Item In (English) Item No. [Inventory]" );
		SendSystemMsg( pPlayer, "`Item Map (English) Number of Item Numbers [Map]" );
	}

	//GOD
	if( pPlayer->adminLevel >= 3 )
	{
		SendSystemMsg( pPlayer, "`Map movement (English) Map number x coordinate y coordinate");
		SendSystemMsg( pPlayer, "`캐쉬충전(영문) 캐릭터이름 금액");
		SendSystemMsg( pPlayer, "`캐쉬취소(영문) 캐릭터이름 금액");
		SendSystemMsg( pPlayer, "`실드충전(영문) 캐릭터이름 금액");
		SendSystemMsg( pPlayer, "`실드취소(영문) 캐릭터이름 금액");
	}
}

void AdminEnchantInfo( sPDESC_DATA pPlayer, char *command )
{
	SendSystemMsg( pPlayer, "You have been enchanted %d times so far.", g_nEnchantTotal );
}


//
// Function Name: const struct admin_command admin_cmd[]
// Date: 
// Description: 
//
const struct admin_command admin_cmd[] =
{
//	{ "ADDPARTY",				AdminAddParty,			dADMIN_IMPL		},
//	{ "DELPARTY",				AdminDelParty,			dADMIN_IMPL		},
//	{ "날씨",					AdminChangeWeather,		dADMIN_IMPL		},
	{ "help",				AdminHelp,			dADMIN_IMPL	},
	{ "HELP",				AdminHelp,			dADMIN_IMPL	},

	{ "돈줘",				AdminMoneyMake,		dADMIN_IMPL	},
	{ "MoneyMake",				AdminMoneyMake1,		dADMIN_IMPL	},
	
	{ "몹줘",				AdminMobCreate,		dADMIN_IMPL	},	// 몹 생성 
	{ "MOBMAKE",				AdminMobCreate1,		dADMIN_IMPL	},	// 몹 생성 

	{ "TransUser1",				AdminTransUser,		dADMIN_IMPL	},
	{ "TransUser",				AdminTransUser1,		dADMIN_IMPL	},

	{ "메딕",				AdminMedic,			dADMIN_IMPL	},
	{ "Medic",				AdminMedic,			dADMIN_IMPL	},

	{ "HP무적",				AdminNoHit,			dADMIN_IMPL	},
	{ "NOHIT",				AdminNoHit,			dADMIN_IMPL	},
	{ "HPanwjr",				AdminNoHit,			dADMIN_IMPL	},
	{ "hp무적",				AdminNoHit,			dADMIN_IMPL	},
	{ "NOHIT",				AdminNoHit,			dADMIN_IMPL	},

	{ "이벤트설정",				AdminSetEventTime,		dADMIN_IMPL	},
	{ "SETEVENTTIME",			AdminSetEventTime1,		dADMIN_IMPL	},
	{ "이벤트종료",				AdminSetEventTimeEnd,	dADMIN_IMPL	},
	{ "SETEVENTTIMEEND",			AdminSetEventTimeEnd,	dADMIN_IMPL	},
	{ "이벤트확인",				AdminCheckEventTime,		dADMIN_IMPL	},
	{ "CHECKEVENTTIME",			AdminCheckEventTime,		dADMIN_IMPL	},
	{ "이벤트",				AdminSetEvent,		dADMIN_IMPL	},
	{ "SETEVENT",				AdminSetEvent1,		dADMIN_IMPL	},

	{ "DISCON",				AdminDisconUser,		dADMIN_IMPL	},
	{ "DISID",				AdminDisid,			dADMIN_IMPL	},
	
	{ "레벨업",		     		AdminAdvance,			dADMIN_IMPL	},
	{ "LEVELUP",				AdminAdvance,			dADMIN_IMPL	},
	
	{ "KILLALL",				AdminKillAll,			dADMIN_IMPL	},
	{ "POPOBJMOB",			AdminPopObjMob,		dADMIN_IMPL	},
	{ "ALLSKILL",				AdminGetAllSkill,		dADMIN_IMPL	},

	{ "아이템번호",				AdminItemNumber,		dADMIN_GRGOD	},
	{ "ITEMNUMBER",			AdminItemNumber1,		dADMIN_GRGOD	},
	{ "아이템인",				AdminCreateItem,		dADMIN_GRGOD	},		// 아이템 생성
	{ "CREATEITEM",			AdminCreateItem1,		dADMIN_GRGOD	},		// 아이템 생성
	{ "아이템맵",				AdminItemMakeCnt,		dADMIN_GRGOD	},
	{ "dkdlxpaaoq",			AdminItemMakeCnt1,		dADMIN_GRGOD	},

	{ "NORMALEXP",			AdminNormalExp,		dADMIN_GRGOD	},
	{ "SUPERENCHANT",			AdminSuperEnchant,		dADMIN_GRGOD	},		// 인첸트
	{ "BUGITEM",				AdminBugItems,		dADMIN_GRGOD	},		// 아이템 체크
	{ "SETEXP",				AdminSetExp,			dADMIN_GRGOD	},
	{ "동접자",				AdminServerState,		dADMIN_GRGOD	},		// 서버 상태
	{ "ServerState",				AdminServerState,		dADMIN_GRGOD	},		// 서버 상태
	{ "INFO",				AdminInfo,			dADMIN_GRGOD	},		// 정보보기 
	{ "info",				AdminInfo,			dADMIN_GRGOD	},		//
	{ "SHOPRELOAD",			AdminReloadShop,		dADMIN_GRGOD	},
	{ "SLIST",				AdminShopList,		dADMIN_GRGOD	},
	{ "ALLRESET",				AdminResetAll,		dADMIN_GRGOD	},
	{ "RESET",				AdminMobReset,		dADMIN_GRGOD	},
	{ "MOBTIME",				AdminMobResetTime,		dADMIN_GRGOD	},
	{ "RELOADMOB",			AdminReloadMob,		dADMIN_GRGOD	},
	{ "RELOADITEM",			AdminReloadItem,		dADMIN_GRGOD	},
	{ "RELOADABL",			AdminReloadAbility,		dADMIN_GRGOD	},
	{ "RELOADEXP",			AdminReloadExp,		dADMIN_GRGOD	},
	{ "RELOADALL",			AdminReloadAll,		dADMIN_GRGOD	},
	{ "RELOADHPMP",			AdminReloadHPMP,		dADMIN_GRGOD	},
	{ "RELOADSKILL",			AdminReloadSkill,		dADMIN_GRGOD	},

	{ "인벤지우기",				AdminDelInven,		dADMIN_GRGOD	},
	{ "DELINVEN",				AdminDelInven,		dADMIN_GRGOD	},

	{ "이속증가",				AdminSetSpeed,		dADMIN_GRGOD	},
	{ "SETSPEED",				AdminSetSpeed1,		dADMIN_GRGOD	},

	{ "맵이동",				AdminGotoMap,			dADMIN_GOD	},	// 맵 이동
	{ "GOTOMAP",				AdminGotoMap1,		dADMIN_GOD	},	// 맵 이동
	{ "이동",				AdminGoto,			dADMIN_GOD	},
	{ "GOTO",				AdminGoto1,			dADMIN_GOD	},
	{ "ADMINRELOAD",			AdminInfoReload,		dADMIN_GOD	},
	{ "GODMAKE",				AdminGodMake,			dADMIN_GOD	},
	{ "MS",				AdminMobSpeed,		dADMIN_IMPL	},
	{ "SC",				AdminSetCha,			dADMIN_IMPL	},
	{ "VISIBLE",				AdminVisible,			dADMIN_GOD	},

	{ "FULL",				AdminFullPower,		dADMIN_GOD	},
	{ "WAR",				AdminTogleWar,		dADMIN_GOD	},
	{ "WI",				AdminWarInfo,			dADMIN_GOD	},
	{ "캐쉬충전",				AdminChargeCash,		dADMIN_GOD	},
	{ "ChargeCash",			AdminChargeCash1,		dADMIN_GOD	},

	{ "캐쉬취소",				AdminChargeCashCancel,	dADMIN_GOD	},
	{ "ChargeCashCancel",			AdminChargeCashCancel1,	dADMIN_GOD	},
	
	{ "실드충전",				AdminChargeSild,		dADMIN_GOD	},
	{ "ChargeShild",			AdminChargeSild1,		dADMIN_GOD	},

	{ "실드취소",				AdminChargeSildCancel,	dADMIN_GOD	},
	{ "ChargeShildCancel",			AdminChargeSildCance1,	dADMIN_GOD	},

	{ "EnchantInfo",				AdminEnchantInfo,		dADMIN_GOD	},

	{ "\n",						NULL,					0				},					// End
};


//
// Function Name: CheckAdminCommand
// Date: 2003/03/16
// Description: 
//
BOOL CheckAdminCommand( sPDESC_DATA pPlayer, char *command )
{
	int cmd = 0;

	CWhileMgr WMgr;
	
	for( int i = 0; i < 512; i++ )
	{
		WMgr.CheckDump( __FILE__, __LINE__ );

		if( *admin_cmd[cmd].command == '\n' )
			return 0;

		if( !strncmp( admin_cmd[cmd].command, command, strlen( admin_cmd[cmd].command ) ) )
		{
			if( admin_cmd[cmd].level > pPlayer->adminLevel )
				return 0;

			admin_cmd[cmd].admin_func( pPlayer, command );
			return 1;
		}
		cmd++;		
	}
	return 0;
}










