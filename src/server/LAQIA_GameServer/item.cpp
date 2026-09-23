/**   |
 *	$Workfile: item.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 66 $
 *	$Date: 04-12-22 5:18p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/item.cpp $
 * 
 * 66    04-12-22 5:18p Sk8snow
 * 
 * 65    04-12-15 10:40a Sk8snow
 * 
 * 64    04-11-18 1:40a Sk8snow
 * 
 * 63    04-10-29 11:56p Sk8snow
 * 
 * 62    04-10-19 11:14p Sk8snow
 * 
 * 61    04-09-20 10:53p Sk8snow
 * 
 * 60    04-09-11 11:20p Sk8snow
 * 
 * 59    04-09-08 3:54p Sk8snow
 * 
 * 58    04-08-04 9:27a Sk8snow
 * 
 * 57    04-07-13 7:26a Sk8snow
 * 
 * 56    04-05-29 10:12p Sk8snow
 * 
 * 55    04-05-18 1:17a Sk8snow
 * 
 * 54    04-05-11 1:45a Sk8snow
 * 
 * 53    04-04-30 9:46a Sk8snow
 * 
 * 52    04-04-07 10:17p Sk8snow
 * 
 * 51    04-04-03 4:03p Sk8snow
 * 
 * 50    04-04-01 5:33p Sk8snow
 * 
 * 49    04-03-31 8:22a Sk8snow
 *        
 */
#include "def.h"

#include "structs.h"
#include "extern.h"
#include "message.h"

#include "player.h"
#include "world.h"

#include "monster.h"
#include "item.h"

#include "db_rows.h"

#include "skill.h"
#include "local_rates.h"

#include "itemdb.h"

#include "house.h"
#include "effect.h"

#include "pet.h"
#include "guild.h"
#include "billing.h"

#include "party.h"
#include "war.h"

///////////////////////////////////////////////////////
// E X T E R N S
///////////////////////////////////////////////////////
sPITEM_DEF_INFO		g_ItemInfo[dMAX_ITEMINFO];

BOOL g_ItemEvent[dMAX_ITEM_EVENT];
BOOL smithCheckWp=FALSE, smithCheckCard=FALSE;


void RebuildItemSerial( char *host, char *dbName, char *tableName, int maxIdx )
{
	MYSQL *pDB = DBConnect( host, //g_ITEMDBINFO.host,
						3306,
						"root",
						"ageofcignare2215",
						"LAQIA_GAMEDB"); //dbName );

	
	if( !pDB )
		return;
	
	char query[1024];

	for( int i = 0; i < maxIdx; i++ )
	{
		sprintf( query, "update %s set serial = '%s' where idx = %d limit 1", tableName, GetItemSerial(), i );

		mysql_query( pDB, query );
	}

	mysql_close( pDB );
}

//
// Function Name: PrintUserInfo
// Date: 
// Description: 
//
void PrintUserInfo()
{
	MYSQL *pDB = DBConnect( "118.35.89.85", //g_ITEMDBINFO.host,
						g_ITEMDBINFO.port,
						g_ITEMDBINFO.id,
						g_ITEMDBINFO.pw,
						g_ITEMDBINFO.dbName );
	
	if( !pDB )
		return;
	
	char query[dMAX_QUERY_SIZE];
	
	sprintf( query, "select id, name, skill_level0+skill_level1+skill_level2+skill_level3, skill_level0, skill_level1, skill_level2, skill_level3 , str_point, dex_point, int_point, str_add, int_add, dex_add, money from UserTable order by skill_level0 desc, skill_level1 desc, skill_level2 desc, skill_level3 desc" );
//	sprintf( query, "select id, name, level, skill_level0, skill_level1, skill_level2, skill_level3 , str_point, dex_point, int_point, str_add, int_add, dex_add, money from BACKUP_UserTable order by level desc, skill_level0 desc, skill_level1 desc, skill_level2 desc, skill_level3 desc" );
	
	MYSQL_RES *result;
	MYSQL_ROW row;

	FILE *fp;

	fp = fopen( "userlist.txt", "wb" );

	if( !fp )
	{
		mysql_close( pDB );
		return;
	}
	
	if( !mysql_query( pDB, query ) )
	{
		result = mysql_store_result( pDB );
		
		if( result )
		{
			while( ( row = mysql_fetch_row( result ) ) )
			{
				fprintf( fp, "%-15s%-30s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s%-10s\r\n", 
					row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8], row[9], row[10], row[11], row[12], row[13] );
			}
			mysql_free_result( result );
		}
	}
	else
	{
		fclose( fp );
		mysql_close( pDB );
		return;
	}
	
	fclose( fp );
	mysql_close( pDB );
}

//
// Function Name: LoadItemInfo
// Date:2003/03/03
// Description: 아이템 정보를 DB에서 읽어온다
//
BOOL LoadItemInfo()
{
	FILE *fp = fopen( dITEM_TABLE_FILE, "rb" );
	
	if( !fp )
	{
		log( "Can't Find ITEM_DATA.TXT\r\n" );
		return 0;
	}
	
	//
	int itemNum;			// 아이템번호 
	int isUniq;				// 유니크 
	char hName[64];			// 한글이름 
	char eName[64];			// 영문이름
	int itemType;			// 아이템 종류
	
	WORD itemLevel;			// 아이템 레벨 

	int minDamage;			// 최소 공격력
	int maxDamage;			// 최대 공격력 
	int minMana;			// 최소 마법력
	int maxMana;			// 최대 마법력

	BYTE slotCnt;			// 아이템슬롯갯수
	BYTE slotDice;			// 아이템슬롯확률
	int needClass;			// 착용직업 

	int attackRange;		// 공격 범위  
	int durability;			// 내구력 
	int durabilityExp;		// 내구력 경험치
	int attackSpeed;		// 공격 속도 
	int itemAttr;			// 아이템 속성
	int itemDispos;			// 아이템 성향 

	WORD weight;			// 아이템 무게 

	int needStr;			// 필요힘 
	int needInt;			// 필요지혜
	int needDex;			// 필요민첩
	int needCha;			// 필요카리스마
	int pixPrice;			// 수리 가격 
	int sellPrice;			// 판매 가격 
	int Price;				// 구입 가격 
	int pluseStr;			// 힘+
	int pluseInt;			// 지혜+
	int pluseDex;			// 민첩+
	int pluseCha;			// 카리스마+
	int pluseHP;			// 체력+
	int pluseMP;			// 마나+

	int pluseAttackDmg;		// 물리데미지+
	int pluseMagicDmg;		// 마법데미지+
	int pluseDefense;		// 방어력+
	int pluseMagicDefense;	// 마법저항+

	int pluseSAttack;		// 근거리
	int pluseLAttack;		// 원거리
	int pluseBMagic;		// 흑마법
	int pluseWMagic;		// 백마법 
	int minusStr;			// 힘-
	int minusInt;			// 지혜-
	int minusDex;			// 민첩-
	int minusCha;			// 카리스마-
	int minusHP;			// 체력-
	int minusMP;			// 마나-
	
	int minusAttackDmg;		// 물리데미지
	int minusMagicDmg;		// 마법데미지
	int minusDefense;		// 방어력
	int minusMagicDefense;	// 마법저항

	int minusSAttack;		// 근거리-
	int minusLAttack;		// 원거리-
	int minusBMagic;		// 흑마법-
	int minusWMagic;		// 백마법-
	char manImage[5][64];	// 남자 이미지 
	int manHaveSkin;		// 남자 have_skin
	char womanImage[5][64];	// 여자 이미지
	int womanHaveSkin;		// 여자 have_skin

	//

	char readBuff[1024];

	CWhileMgr WMgr;

	int totalItemCnt = 0;

	while( !feof( fp ) )
	{
		WMgr.CheckDump( __FILE__, __LINE__ );

		int nPos = 0;

		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		itemNum = GetTabNumberic( readBuff, nPos );				// 아이템번호 1
		isUniq = GetTabNumberic( readBuff, nPos );				// 유니크 2
		GetTabString( readBuff, hName, nPos );					// 한글이름3 
		GetTabString( readBuff, eName, nPos );					// 영문이름4
		itemType = GetTabNumberic( readBuff, nPos );			// 아이템 종류 5

		itemLevel = GetTabNumberic( readBuff, nPos );			// 아이템 레벨6
		
		minDamage = GetTabNumberic( readBuff, nPos );			// 최소 공격력7
		maxDamage = GetTabNumberic( readBuff, nPos );			// 최대 공격력 8
		minMana = GetTabNumberic( readBuff, nPos );				// 최소 마나9
		maxMana = GetTabNumberic( readBuff, nPos );				// 최대 마나10

		slotCnt = GetTabNumberic( readBuff, nPos );				// 아이템슬롯갯수12
		slotDice = GetTabNumberic( readBuff, nPos );			// 아이템슬롯확률13
		needClass = GetTabNumberic( readBuff, nPos );			// 착용직업 14

		attackRange = GetTabNumberic( readBuff, nPos );			// 공격 범위 15
		durability = GetTabNumberic( readBuff, nPos );			// 내구력 16
		durabilityExp = GetTabNumberic( readBuff, nPos );		// 내구력 경험치17
		attackSpeed = GetTabNumberic( readBuff, nPos );			// 공격 속도 18
		itemAttr = GetTabNumberic( readBuff, nPos );			// 아이템 속성19
		itemDispos = GetTabNumberic( readBuff, nPos );			// 아이템 성향 20
		
		weight = GetTabNumberic( readBuff, nPos );				// 아이템 무게 21

		needStr = GetTabNumberic( readBuff, nPos );				// 필요힘 22
		needInt = GetTabNumberic( readBuff, nPos );				// 필요지혜23
		needDex = GetTabNumberic( readBuff, nPos );				// 필요민첩24
		needCha = GetTabNumberic( readBuff, nPos );				// 필요카리스마25
		pixPrice = GetTabNumberic( readBuff, nPos );			// 수리 가격 26
		sellPrice = GetTabNumberic( readBuff, nPos );			// 판매 가격 27
		Price = GetTabNumberic( readBuff, nPos );				// 구입 가격 28
		pluseStr = GetTabNumberic( readBuff, nPos );			// 힘+29
		pluseInt = GetTabNumberic( readBuff, nPos );			// 지혜+30
		pluseDex = GetTabNumberic( readBuff, nPos );			// 민첩+31
		pluseCha = GetTabNumberic( readBuff, nPos );			// 카리스마+32
		pluseHP = GetTabNumberic( readBuff, nPos );				// 체력+33
		pluseMP = GetTabNumberic( readBuff, nPos );				// 마나+34
		
		pluseAttackDmg = GetTabNumberic( readBuff, nPos );		// 물리데미지+35
		pluseMagicDmg = GetTabNumberic( readBuff, nPos );		// 마법데미지+36
		pluseDefense = GetTabNumberic( readBuff, nPos );		// 방어력+37
		pluseMagicDefense = GetTabNumberic( readBuff, nPos );	// 마법저항+38

		pluseSAttack = GetTabNumberic( readBuff, nPos );		// 근거리39
		pluseLAttack = GetTabNumberic( readBuff, nPos );		// 원거리40
		pluseBMagic = GetTabNumberic( readBuff, nPos );			// 흑마법41
		pluseWMagic = GetTabNumberic( readBuff, nPos );			// 백마법 42
		minusStr = GetTabNumberic( readBuff, nPos );			// 힘-43
		minusInt = GetTabNumberic( readBuff, nPos );			// 지혜-44
		minusDex = GetTabNumberic( readBuff, nPos );			// 민첩-45
		minusCha = GetTabNumberic( readBuff, nPos );			// 카리스마-46
		minusHP = GetTabNumberic( readBuff, nPos );				// 체력-47
		minusMP = GetTabNumberic( readBuff, nPos );				// 마나-48

		minusAttackDmg = GetTabNumberic( readBuff, nPos );		// 물리데미지49
		minusMagicDmg = GetTabNumberic( readBuff, nPos );		// 마법데미지50
		minusDefense = GetTabNumberic( readBuff, nPos );		// 방어력51
		minusMagicDefense = GetTabNumberic( readBuff, nPos );	// 마법저항52

		minusSAttack = GetTabNumberic( readBuff, nPos );		// 근거리-53
		minusLAttack = GetTabNumberic( readBuff, nPos );		// 원거리-54
		minusBMagic = GetTabNumberic( readBuff, nPos );			// 흑마법-55
		minusWMagic = GetTabNumberic( readBuff, nPos );			// 백마법-56
		GetTabString( readBuff, manImage[0], nPos );			// 남자 이미지 57
		GetTabString( readBuff, manImage[1], nPos );//58
		GetTabString( readBuff, manImage[2], nPos );//59
		GetTabString( readBuff, manImage[3], nPos );//60
		GetTabString( readBuff, manImage[4], nPos );//61
		manHaveSkin = GetTabNumberic( readBuff, nPos );			// 남자 have_skin62
		GetTabString( readBuff, womanImage[0], nPos );			// 여자 이미지63
		GetTabString( readBuff, womanImage[1], nPos );//64
		GetTabString( readBuff, womanImage[2], nPos );//65
		GetTabString( readBuff, womanImage[3], nPos );//66
		GetTabString( readBuff, womanImage[4], nPos );//67
		womanHaveSkin = GetTabNumberic( readBuff, nPos );		// 여자 have_skin68
		
		if( itemNum >= dMAX_ITEMINFO )
		{
			log( "LoadItemInfo : itemNum >= dMAX_ITEMINFO\r\n" );
			fclose( fp );
			return 0;
		}
		
		if( !g_ItemInfo[itemNum] )
		{
			g_ItemInfo[itemNum] = (sPITEM_DEF_INFO)malloc(sizeof(sITEM_DEF_INFO));
			
			if( !g_ItemInfo[itemNum] )
			{
				log( "LoadItemInfo : malloc Failed\r\n" );
				fclose( fp );
				return 0;
			}
		}
		
		g_ItemInfo[itemNum]->itemNum = itemNum;					// 아이템번호 
		g_ItemInfo[itemNum]->isUniq = isUniq;					// 유니크 
		strcpy( g_ItemInfo[itemNum]->hName, hName );			// 한글이름 
		strcpy( g_ItemInfo[itemNum]->eName, eName );			// 영문이름
		g_ItemInfo[itemNum]->itemType = itemType;				// 아이템 종류 

		g_ItemInfo[itemNum]->itemLevel = itemLevel;				// 아이템 레벨

		g_ItemInfo[itemNum]->minDamage = minDamage;				// 최소 공격력
		g_ItemInfo[itemNum]->maxDamage = maxDamage;				// 최대 공격력 
		g_ItemInfo[itemNum]->minMana = minMana;					// 최소 마법력
		g_ItemInfo[itemNum]->maxMana = maxMana;					// 최대 마법력

		g_ItemInfo[itemNum]->slotCnt = slotCnt;					// 아이템슬롯갯수
		g_ItemInfo[itemNum]->slotDice = slotDice;				// 아이템슬롯확률
		g_ItemInfo[itemNum]->needClass = needClass;				// 착용직업 

		g_ItemInfo[itemNum]->attackRange = attackRange;			// 공격 범위 
		g_ItemInfo[itemNum]->durability = durability;			// 내구력 
		g_ItemInfo[itemNum]->attackSpeed = attackSpeed;			// 공격 속도 
		g_ItemInfo[itemNum]->itemAttr = itemAttr;				// 아이템 속성
		g_ItemInfo[itemNum]->itemDispos = itemDispos;			// 아이템 성향 
		
		g_ItemInfo[itemNum]->weight = weight;					// 아이템 무게

		g_ItemInfo[itemNum]->needStr = needStr;					// 필요힘 
		g_ItemInfo[itemNum]->needInt = needInt;					// 필요지혜
		g_ItemInfo[itemNum]->needDex = needDex;					// 필요민첩
		g_ItemInfo[itemNum]->needCha = needCha;					// 필요카리스마
		g_ItemInfo[itemNum]->pixPrice = pixPrice;				// 수리 가격 
		g_ItemInfo[itemNum]->sellPrice = sellPrice;				// 판매 가격 
		g_ItemInfo[itemNum]->Price = Price;						// 구입 가격 
		g_ItemInfo[itemNum]->pluseStr = pluseStr;				// 힘+
		g_ItemInfo[itemNum]->pluseInt = pluseInt;				// 지혜+
		g_ItemInfo[itemNum]->pluseDex = pluseDex;				// 민첩+
		g_ItemInfo[itemNum]->pluseCha = pluseCha;				// 카리스마+
		g_ItemInfo[itemNum]->pluseHP = pluseHP;					// 체력+
		g_ItemInfo[itemNum]->pluseMP = pluseMP;					// 마나+
		
		g_ItemInfo[itemNum]->pluseAttackDmg = pluseAttackDmg;	// 물리데미지+
		g_ItemInfo[itemNum]->pluseMagicDmg = pluseMagicDmg;		// 마법데미지+
		g_ItemInfo[itemNum]->pluseDefense = pluseDefense;		// 방어력+
		g_ItemInfo[itemNum]->pluseMagicDefense = pluseMagicDefense;	// 마법저항+

		g_ItemInfo[itemNum]->pluseSAttack = pluseSAttack;		// 근거리
		g_ItemInfo[itemNum]->pluseLAttack = pluseLAttack;		// 원거리
		g_ItemInfo[itemNum]->pluseBMagic = pluseBMagic;			// 흑마법
		g_ItemInfo[itemNum]->pluseWMagic = pluseWMagic;			// 백마법 
		g_ItemInfo[itemNum]->minusStr = minusStr;				// 힘-
		g_ItemInfo[itemNum]->minusInt = minusInt;				// 지혜-
		g_ItemInfo[itemNum]->minusDex = minusDex;				// 민첩-
		g_ItemInfo[itemNum]->minusCha = minusCha;				// 카리스마-
		g_ItemInfo[itemNum]->minusHP = minusHP;					// 체력-
		g_ItemInfo[itemNum]->minusMP = minusMP;					// 마나-

		g_ItemInfo[itemNum]->minusAttackDmg = minusAttackDmg;	// 물리데미지
		g_ItemInfo[itemNum]->minusMagicDmg = minusMagicDmg;		// 마법데미지 
		g_ItemInfo[itemNum]->minusDefense = minusDefense;		// 방어력 
		g_ItemInfo[itemNum]->minusMagicDefense = minusMagicDefense;	// 마법저항 

		g_ItemInfo[itemNum]->minusSAttack = minusSAttack;		// 근거리-
		g_ItemInfo[itemNum]->minusLAttack = minusLAttack;		// 원거리-
		g_ItemInfo[itemNum]->minusBMagic = minusBMagic;			// 흑마법-
		g_ItemInfo[itemNum]->minusWMagic = minusWMagic;			// 백마법-


		g_ItemInfo[itemNum]->needItemLog = 0;

		if( GET_ITEM_TYPE2( itemNum ) == dITEMTYPE_MAGICSCROLL )
			g_ItemInfo[itemNum]->skillIdx = FindSkillIdx( itemNum );
		else
			g_ItemInfo[itemNum]->skillIdx = 0;

		totalItemCnt++;

		if( g_itemRemoverOn == 1 )
		{
			if( g_ItemInfo[itemNum]->isUniq == g_itemRemoverNum )
				ITEMDB_DeleteAllItem( itemNum );
		}

		if( IsBillingItem( itemNum ) )
			g_ItemInfo[itemNum]->isBillingItem = 1;
		else
			g_ItemInfo[itemNum]->isBillingItem = 0;
	}
	
	fclose( fp );

	log( "Load Item Info : Total Item Count : %d / %d\r\n", totalItemCnt , dMAX_ITEMINFO );

	return 1;
}


//
// Function Name: DeleteItemInfo
// Date:2003/03/03
// Description: 아이템 정보 삭제
//
void DeleteItemInfo()
{
	for( int i = 0; i < dMAX_ITEMINFO; i++ )
	{
		if( g_ItemInfo[i] )
			SAFE_FREE( g_ItemInfo[i] );
	}
}


//
//
// Function Name: MakeItemInPacket	
// Date: 2003-03-07
// Description: 
//
void MakeItemInPacket( sPITEM_DATA pItem , WORD mobIdx )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_INSERT_MAPITEM, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutWord( g_Packet, pItem->itemNum, g_nPos );
	PutInteger( g_Packet, pItem->mapX, g_nPos );
	PutInteger( g_Packet, pItem->mapY, g_nPos );
	PutInteger( g_Packet, pItem->itemNum == dMONEY_NUM ? GET_MONEY_AMOUNT( pItem ) : 0 , g_nPos );
	PutWord( g_Packet, mobIdx, g_nPos );

	PutSize( g_Packet, g_nPos );
}

//
// Function Name: MakeItemOutPacket
// Date: 2003-03-07
// Description: 
//
void MakeItemOutPacket( sPITEM_DATA pItem )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_REMOVE_MAPITEM, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );
}


//
// Function Name: GetItemSector
// Date: 2003-03-07
// Description: 
//
void GetItemSector( sPDESC_DATA pPlayer, int x, int y )
{
	if( !IS_VALID_WORLD( pPlayer->ch.mapNum ) )
		return;

	sPITEM_DATA item, next_item;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->item[y][x], item, next_item , w_next, WMgr );
	MakeItemInPacket( item );
	SendData( pPlayer, g_Packet, g_nPos );
	LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->item[y][x], item, next_item );
}


//
// Function Name: GetItemVSector
// Date: 2003-03-07
// Description: 
//
void GetItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				GetItemSector( pPlayer,  x, y );
		}
	}
}

//
// Function Name: GetItemHSector
// Date: 2003-03-07
// Description: 
//
void GetItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				GetItemSector( pPlayer, x, y );
		}
	}
}

//
// Function Name: OutItemSector
// Date: 2003-03-07
// Description: 
//
void OutItemSector( sPDESC_DATA pPlayer, int x, int y )
{
	if( !IS_VALID_WORLD( pPlayer->ch.mapNum ) )
		return;

	sPITEM_DATA item, next_item;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->item[y][x], item, next_item , w_next, WMgr );
	MakeItemOutPacket( item );
	SendData( pPlayer, g_Packet, g_nPos );
	LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->item[y][x], item, next_item );
}

//
// Function Name: OutItemVSector
// Date: 2003-03-07
// Description: 
//
void OutItemVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				OutItemSector( pPlayer,  x, y );
		}
	}
}

//
// Function Name: OutItemHSector
// Date: 2003-03-07
// Description: 
//
void OutItemHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				OutItemSector( pPlayer, x, y );
		}
	}
}



//
// Function Name: SendToItemArea
// Date: 2003-03-07
// Description: 
//
void SendToItemArea( sPITEM_DATA pItem, char *data, int size )
{
	if( !IS_VALID_WORLD( pItem->mapNum ) )
		return;

	int posX = (int)(pItem->mapX/dONE_TILE);
	int posY = (int)(pItem->mapY/dONE_TILE);
		
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[pItem->mapNum]->player[y][x], data, size , dSENDMODE_TOALL );
}

//
// Function Name: GetCurrAreaItem
// Date: 2003-03-07
// Description: 
//
void GetCurrAreaItem( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				GetItemSector( pPlayer, x, y );
}

//
// Function Name: GetCurrHouseItem
// Date: 
// Description: 
//
void GetCurrHouseItem( sPDESC_DATA pPlayer )
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( gHOUSE[GET_MAP_NUM(pPlayer)]->in_items, item, next_item, w_next, WMgr );

	MakeItemInPacket( item );
	SendData( pPlayer );

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM(pPlayer)]->in_items, item, next_item );
}



//
// Function Name: OutCurrAreaItem
// Date: 2003-03-07
// Description: 
//
void OutCurrAreaItem( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				OutItemSector( pPlayer, x, y );
}

//
// Function Name: OutCurrHouseItem
// Date: 
// Description: 
//
void OutCurrHouseItem( sPDESC_DATA pPlayer )
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( gHOUSE[GET_MAP_NUM(pPlayer)]->in_items, item, next_item, w_next, WMgr );
	
	MakeItemOutPacket( item );
	SendData( pPlayer );

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM(pPlayer)]->in_items, item, next_item );
}

//


//
// Function Name: NewItem
// Date:2003/03/03
// Description: 
//
sPITEM_DATA NewItem()
{
	sPITEM_DATA newItem = NULL;

	if( gITEMS.memory )
	{
		newItem = gITEMS.memory;
		REMOVE_FROM_LIST( gITEMS.memory , newItem, prev, next );
		return newItem;
	}
	else
	{
		newItem = (sPITEM_DATA)malloc(sizeof(sITEM_DATA));

		if( !newItem )
			return NULL;

		newItem->idxNum = 0;						// 아이템 고유번호
		*newItem->serial = NULL_STR;				// 아이템 시리얼 
		
		newItem->itemNum = 0;
		
		newItem->life = 0;
		newItem->life_in_map = 0;
		
		newItem->mapNum = 0;
		newItem->mapX = 0;
		newItem->mapY = 0;			
		newItem->equipWhere = 0;	
		
		newItem->invenX = 0;
		newItem->invenY = 0;
		newItem->invenPage = 0;
		
		*newItem->ownerName = NULL_STR;
		*newItem->masterName = NULL_STR;
		newItem->ownerCheckTick = 0;

		newItem->prev = NULL;
		newItem->next = NULL;
		
		// Inventory 처리용
		newItem->i_next = NULL;
		newItem->i_prev = NULL;
		
		// 맵 리스트 처리용 next
		newItem->w_prev = NULL;			
		newItem->w_next = NULL;

		return newItem;
	}
	return NULL;
}


//
// Function Name: CreateItem
// Date:2003/03/03
// Description: 
//
sPITEM_DATA CreateItem( int itemNum , char *commant, char *filename, int lineNum )
{
	if( !IS_VALID_ITEM( itemNum ) )
	{
		log( "CreateItem : Invalid Item Num %d [%s/%d]\r\n", itemNum, filename, lineNum );
		return NULL;
	}

	sPITEM_DATA newItem;

	newItem = NewItem();

	if( !newItem )
		return NULL;

	//
	newItem->idxNum = -1;							// 아이템 고유번호

#ifdef USE_ITEM_SERIAL
	strcpy( newItem->serial, GetItemSerial() );	
#else
	*newItem->serial = NULL_STR;					// 아이템 시리얼 
#endif

	newItem->itemNum = g_ItemInfo[itemNum]->itemNum;

	newItem->life = g_ItemInfo[itemNum]->durability;	//g_ItemInfo[itemNum]->life;

	newItem->mapNum = -1;
	newItem->mapX = -1;
	newItem->mapY = -1;		
	newItem->mapKind = 0;

	newItem->equipWhere = 0;	

	newItem->invenX = 255;
	newItem->invenY = 255;
	newItem->invenPage = 255;

	newItem->isMobDrop = 0;

	//
	for( int i = 0; i < 3; i++ )
		newItem->exVal[i] = 0;

	newItem->next = NULL;
	newItem->prev = NULL;

	// Inventory 처리용
	newItem->i_next = NULL;
	newItem->i_prev = NULL; 
	
	// 맵 리스트 처리용 next
	newItem->w_prev = NULL;			
	newItem->w_next = NULL;
	
	return newItem;
}



//
// Function Name: ItemToInventory
// Date:2003/03/24
// Description: 
//


BOOL ItemToInventory( sPDESC_DATA pPlayer, sPITEM_DATA pItem , sPCHECK_INVEN pCheckInven )
{
	if( !pPlayer || !pItem )
	{
		log( "ItemToInventory : pPlayer or pItem is NULL\r\n" );
		return 0;
	}

	if( !pCheckInven )
	{
		log( "ItemToInventory : pCheckInven is NULL \r\n" );
		return 0;
	}
	
	if (GetItemStackLimit(pItem) > 1 && pItem->exVal[0] < 1) pItem->exVal[0] = 1;
	pPlayer->invenIdx++;
	pItem->idxNum = pPlayer->invenIdx;
	pItem->invenPage = pCheckInven->page;
	pItem->invenY = pCheckInven->y;
	pItem->invenX = pCheckInven->x;

	pItem->isMobDrop = 0;

	FillInvenArray( pPlayer, pItem->invenPage, pItem->invenX, pItem->invenY );

	if( pPlayer->inven == pItem )
	{
		log( "ItemToInventory : pPlayer->invne == pItem\r\n" );
		return 1;
	}

	int exVal = 0;
	if( GetItemStackLimit( pItem ) > 1 )
	{
		exVal = pItem->exVal[0];
	}
	else
	{
		exVal = 0;
	}

	INSERT_TO_LIST( pPlayer->inven, pItem, i_prev, i_next );

	//
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_INSERT_INVENTORY, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutInteger( g_Packet, pItem->itemNum, g_nPos );
	PutByte( g_Packet, pItem->invenPage, g_nPos );
	PutByte( g_Packet, pItem->invenX, g_nPos );
	PutByte( g_Packet, pItem->invenY, g_nPos );
	PutWord( g_Packet, exVal, g_nPos );	// 수량
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );
	//

#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	ITEMDB_ToInven( pPlayer, pItem ); 
#endif

#ifdef USE_ITEM_WEIGHT
	AddCurrItemWeight( pPlayer, pItem );
	SendWeightGageInfo( pPlayer );
#endif

	return 1;
}

BOOL ItemToInventory( sPDESC_DATA pPlayer, sPITEM_DATA pItem , BYTE invenPage, BYTE invenX, BYTE invenY )
{
	sCHECK_INVEN checkInven;

	checkInven.page = invenPage;
	checkInven.x = invenX;
	checkInven.y = invenY;

	return ItemToInventory( pPlayer, pItem, &checkInven );
}


//
// Function Name: ItemFromInventory
// Date:2003/03/24
// Description: 
//
void ItemFromInventory( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pPlayer || !pItem )
	{
		log( "ItemFromPlayer : pPlayer or pItem is NULL\r\n" );
		return;
	}

	//
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_REMOVE_INVENTORY, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
	//

	REMOVE_FROM_LIST( pPlayer->inven, pItem, i_prev, i_next );

	UnFillInvenArray( pPlayer, pItem->invenPage, pItem->invenX, pItem->invenY );

#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	ITEMDB_DelInven( pPlayer, pItem );
#endif
	
#ifdef USE_ITEM_WEIGHT
	DelCurrItemWeight( pPlayer, pItem );
	SendWeightGageInfo( pPlayer );
#endif
}

//
// Function Name: ItemToHouse
// Date:
// Description: 
//
void ItemToHouse( sPITEM_DATA pItem, int mapNum, int mapX, int mapY )
{
	if( !pItem )
	{
		log( "ItemToHouse : pItem is NULL\r\n" );
		return;
	}
	
	if( !IS_VALID_HOUSE( mapNum ) )
		return;

	gHOUSE[mapNum]->itemIdxCnt++;

	if( gHOUSE[mapNum]->itemIdxCnt > dMAX_ITEM_IDX )
		gHOUSE[mapNum]->itemIdxCnt = 1;
	
	pItem->idxNum = gHOUSE[mapNum]->itemIdxCnt;
	
	INSERT_TO_LIST( gHOUSE[mapNum]->in_items, pItem, w_prev, w_next );
	INSERT_TO_LIST( gITEMS.list, pItem, prev, next );

	pItem->mapNum = mapNum;
	pItem->mapX = mapX;
	pItem->mapY = mapY;
	pItem->mapKind = dMAP_KIND_INDOOR;

	pItem->life_in_map = dMAP_ITEM_LIFE;		// 10 분

	//
	MakeItemInPacket( pItem );
	SendToHouse( mapNum, g_Packet, g_nPos );
}


//
// Function Name: ItemToMap
// Date:2003/03/03
// Description: 
//
void ItemToMap( sPITEM_DATA pItem, int mapNum, int mapX, int mapY, WORD mobIdx )
{
	if( !pItem )
	{
		log( "ItemToMap : pItem is NULL\r\n" );
		return;
	}

	if( !IS_VALID_WORLD( mapNum ) )
		return;

	int destX = (int)mapX/dONE_TILE;
	int destY = (int)mapY/dONE_TILE;

	gWORLD[mapNum]->itemIdxCnt++;
	if( gWORLD[mapNum]->itemIdxCnt > dMAX_ITEM_IDX )
		gWORLD[mapNum]->itemIdxCnt = 1;
	
	pItem->idxNum = gWORLD[mapNum]->itemIdxCnt;
	INSERT_TO_LIST( gWORLD[mapNum]->item[destY][destX], pItem, w_prev, w_next );
	INSERT_TO_LIST( gITEMS.list, pItem, prev, next );

	pItem->mapNum = mapNum;
	pItem->mapX = mapX;
	pItem->mapY = mapY;
	pItem->mapKind = dMAP_KIND_FIELD;

	pItem->life_in_map = dMAP_ITEM_LIFE;		// 10 분

	//
	MakeItemInPacket( pItem , mobIdx );
	SendToItemArea( pItem, g_Packet, g_nPos );
}


//
// Function Name: ItemFromHouse
// Date:
// Description: 
//
void ItemFromHouse( sPITEM_DATA pItem )
{
	if( !pItem )
	{
		log( "ItemFromHouse : pItem is NULL\r\n" );
		return;
	}

	if( pItem->mapKind != dMAP_KIND_INDOOR )
		return;

	if( !IS_VALID_HOUSE( pItem->mapNum ) )
		return;
	
	MakeItemOutPacket( pItem );
	SendToHouse( pItem->mapNum, g_Packet, g_nPos );

	REMOVE_FROM_LIST( gHOUSE[pItem->mapNum]->in_items, pItem, w_prev, w_next );
	REMOVE_FROM_LIST( gITEMS.list, pItem, prev, next );

	pItem->mapNum = pItem->mapX = pItem->mapY = 0;
	pItem->idxNum = -1;
}

//
// Function Name: ItemFromMap
// Date:2003/03/03
// Description: 
//
void ItemFromMap( sPITEM_DATA pItem )
{
	if( !pItem )
	{
		log( "ItemFromMap : pItem is NULL\r\n" );
		return;
	}

	if( !IS_VALID_WORLD( pItem->mapNum ) )
		return;
	
	MakeItemOutPacket( pItem );
	SendToItemArea( pItem, g_Packet, g_nPos );

	REMOVE_FROM_LIST( gWORLD[pItem->mapNum]->item[pItem->mapY/dONE_TILE][pItem->mapX/dONE_TILE], 
		pItem, w_prev, w_next );
	REMOVE_FROM_LIST( gITEMS.list, pItem, prev, next );

	pItem->mapNum = pItem->mapX = pItem->mapY = 0;

	pItem->idxNum = -1;
}

//
// Function Name: GetEquipPositionByNum
// Date:
// Description: 
//
BYTE GetEquipPositionByNum( WORD itemNum )
{
	if( !IS_VALID_ITEM( itemNum ) )
		return 255;

	switch( GET_ITEM_TYPE2( itemNum ) )
	{
	case dITEMTYPE_NONE:				//"0.미설정",
	case dITEMTYPE_ARROW:				//"15.화살",
	case dITEMTYPE_JEWEL:				//"40.보석",
	case dITEMTYPE_SCROLL:				//"41.스크롤",
	case dITEMTYPE_MAGICSCROLL:			//"42.마법스크롤",
	case dITEMTYPE_CONTAINER:			//"43.가방(상자)",
	case dITEMTYPE_POTION:				//"44.포션",
	case dITEMTYPE_OBJECT:				//"45.오브젝트",
	case dITEMTYPE_FOOD:				//"46.먹을것",
	case dITEMTYPE_ONETIME:				//"47.일회용",
		return 255;
	case dITEMTYPE_ONEHANDSWORD:		//"1.한손검",
	case dITEMTYPE_TWOHANDSWORD:		//"2.양손검",
	case dITEMTYPE_DUALSWORD:			//"3.쌍칼",
	case dITEMTYPE_ONEHANDAXE:			//"4.한손도끼",
	case dITEMTYPE_TWOHANDAXE:			//"5.양손도끼",
	case dITEMTYPE_DUALAXE:				//"6.쌍도끼",
	case dITEMTYPE_ONEHANDBLUNT:		//"7.한손둔기",
	case dITEMTYPE_TWOHANDBLUNT:		//"8.양손둔기",
	case dITEMTYPE_DUALBLUNT:			//"9.쌍둔기",
	case dITEMTYPE_DONPA:				//"10.돈파",
	case dITEMTYPE_SPHERE:				//"16.창",
	case dITEMTYPE_BONG:				//"17.봉",
	case dITEMTYPE_STAFF:				//"18.지팡이",
		return dEQUIP_WEAPON;
	case dITEMTYPE_SHORTBOW:			//"11.단궁",
	case dITEMTYPE_LONGBOW:				//"12.장궁",
	case dITEMTYPE_DDABALBOW:			//"13.연노",
	case dITEMTYPE_CROSSBOW:			//"14.석궁",
		return dEQUIP_BOW;
	case dITEMTYPE_WORRIOR_UP_ARMOR:	//"19.전사갑옷(상의)",
	case dITEMTYPE_ARCHER_UP_ARMOR:		//"21.궁수갑옷(상의)",
	case dITEMTYPE_WIZARD_UP_ARMOR:		//"23.법사갑옷(상의)",
		return dEQUIP_JACKET;
	case dITEMTYPE_WORRIOR_LO_ARMOR:	//"20.전사갑옷(하의)",
	case dITEMTYPE_ARCHER_LO_ARMOR:		//"22.궁수갑옷(하의)",
	case dITEMTYPE_WIZARD_LO_ARMOR:		//"24.법사갑옷(하의)",
		return dEQUIP_PANTS;
	case dITEMTYPE_WORRIOR_HELMET:		//"25.전사투구",
	case dITEMTYPE_ARCHER_HELMET:		//"26.궁수투구",
	case dITEMTYPE_WIZARD_HELMET:		//"27.법사투구",
		return dEQUIP_HELMET;
	case dITEMTYPE_WARRIOR_BELT:		//"28.전사벨트",
	case dITEMTYPE_ARCHER_BELT:			//"29.궁수벨트",
	case dITEMTYPE_WIZZARD_BELT:		//"30.법사벨트",
		return dEQUIP_BELT;
	case dITEMTYPE_WARRIOR_GLOVE:		//"31.전사장갑",
	case dITEMTYPE_ARCHER_GLOVE:		//"32.궁수장갑",
	case dITEMTYPE_WIZZARD_GLOVE:		//"33.법사장갑",
		return dEQUIP_GLOVE;
	case dITEMTYPE_WARRIOR_SHOES:		//"34.전사신발",
	case dITEMTYPE_ARCHER_SHOES:		//"35.궁수신발",
	case dITEMTYPE_WIZZARD_SHOES:		//"36.법사신발",
		return dEQUIP_SHOES;
	case dITEMTYPE_SHIELD:				//"37.방패",
		return dEQUIP_SHIELD;
	case dITEMTYPE_NECKLACE:			//"38.목걸이",
		return dEQUIP_NECK;
	case dITEMTYPE_RING:				//"39.반지", 
		return dEQUIP_RING;
	default:
		return 255;
	}
	return 255;
}

//
// Function Name: GetEquipPosition
// Date:2003/03/27
// Description: 
//
BYTE GetEquipPosition( sPITEM_DATA pItem )
{
	return GetEquipPositionByNum( pItem->itemNum );
}




//
// Function Name: EquipItemFromInven
// Date:2003/03/03
// Description: 
//
BOOL ItemToEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem , BYTE equipWhere )
{	
	if( equipWhere >= dMAX_EQUIP )
	{
		log( "ItemToEquip : equipWhere >= dMAX_EQUIP\r\n" );
		return 0;
	}
	
	//100Lv제 아이템
/*	if( CheckItemLevel( pItem->itemNum ) )
	{
		if( GET_ALL_SKILL( pPlayer ) < 99 )
		{
			SendSystemMsg( pPlayer, "장착할 수 없는 아이템 입니다." );
			return 0;
		}
	}	*/
	
	BYTE hpmpAdded = 0;

	//여기서 아이템의 정보 체크

	// HP ADD
	if( GET_ITEM_PLUSEHP( pItem ) )
	{
		GET_HPADD( pPlayer ) += GET_ITEM_PLUSEHP( pItem );
		hpmpAdded = 1;
	}
	// MP ADD
	if( GET_ITEM_PLUSEMP( pItem ) )
	{
		GET_MPADD( pPlayer ) += GET_ITEM_PLUSEMP( pItem );
		hpmpAdded = 1;
	}
	
	if( hpmpAdded )
		SendPlayerMaxHPMP( pPlayer );

	// STR ADD
	if( GET_ITEM_PLUSESTR( pItem ) )
		GET_STRADD_EX( pPlayer ) += GET_ITEM_PLUSESTR( pItem );
	// DEX ADD
	if( GET_ITEM_PLUSEDEX( pItem ) )
		GET_DEXADD_EX( pPlayer ) += GET_ITEM_PLUSEDEX( pItem );
	// INT ADD
	if( GET_ITEM_PLUSEINT( pItem ) )
		GET_INTADD_EX( pPlayer ) += GET_ITEM_PLUSEINT( pItem );

	pPlayer->equip[equipWhere] = pItem;
	pItem->equipWhere = equipWhere;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_INSERT_EQUIP_ITEM, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );

#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	ITEMDB_ToEquip( pPlayer, pItem );
#endif

	SendChangeEquip( pPlayer, pItem, 1 );

#ifdef USE_ITEM_WEIGHT
	AddCurrItemWeight( pPlayer, pItem );
	SendWeightGageInfo( pPlayer );
#endif
	return 1;
}



BOOL CheckItemLevel( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
 // Local play: equipment grades do not require a character level.
 return pItem != NULL;
}


//
// Function Name: REmoveItemFromEquip
// Date:2003/03/03
// Description: 
//
BOOL ItemFromEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( pItem->equipWhere == -1 || pItem->equipWhere >= dMAX_EQUIP )
	{
		log( "ItemFromEquip : pItem->equipWhere is -1 || pItem->equipWhere >= dMAX_EQUIP\r\n" );
		return 0;
	}
	
	if( pPlayer->equip[pItem->equipWhere] != pItem )
	{
		log( "ItemFromEquip : pPlayer->equip[pItem->equipWhere] != pItem\r\n" );
		return 0;
	}

	BYTE hpmpAdded = 0;

	// HP ADD
	if( GET_ITEM_PLUSEHP( pItem ) )
	{
		GET_HPADD( pPlayer ) -= GET_ITEM_PLUSEHP( pItem );
		hpmpAdded = 1;

		if( GET_HP( pPlayer ) > ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
		{
			GET_HP( pPlayer ) = ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
			SendPlayerHP( pPlayer );
		}
	}

	// MP ADD
	if( GET_ITEM_PLUSEMP( pItem ) )
	{
		GET_MPADD( pPlayer ) -= GET_ITEM_PLUSEMP( pItem );
		hpmpAdded = 1;
	
		if( GET_MP( pPlayer ) > ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) ) )
		{
			GET_MP( pPlayer ) = ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
			SendPlayerMP( pPlayer );
		}
	}

	if( hpmpAdded )
		SendPlayerMaxHPMP( pPlayer );

	// STR ADD
	if( GET_ITEM_PLUSESTR( pItem ) )
		GET_STRADD_EX( pPlayer ) -= GET_ITEM_PLUSESTR( pItem );
	// DEX ADD
	if( GET_ITEM_PLUSEDEX( pItem ) )
		GET_DEXADD_EX( pPlayer ) -= GET_ITEM_PLUSEDEX( pItem );
	// INT ADD
	if( GET_ITEM_PLUSEINT( pItem ) )
		GET_INTADD_EX( pPlayer ) -= GET_ITEM_PLUSEINT( pItem );


#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	ITEMDB_DelEquip( pPlayer, pItem );
#endif

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_REMOVE_EQUIP_ITEM, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

	pPlayer->equip[pItem->equipWhere] = NULL;
	pItem->equipWhere = -1;

	SendChangeEquip( pPlayer, pItem, 0 );

#ifdef USE_ITEM_WEIGHT
	DelCurrItemWeight( pPlayer, pItem );
	SendWeightGageInfo( pPlayer );
#endif

	return 1;
}

//
// Function Name: ItemFromGarbage
// Date:
// Description: 
//
void ItemFromGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	ITEMDB_DelGarbage( pPlayer, pItem );

	REMOVE_FROM_LIST( pPlayer->garbage, pItem, i_prev, i_next );
	pPlayer->garbageCnt--;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_REMOVE_FROM_GARBAGE, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: ItemToGarbage
// Date:
// Description: 
//
void ItemToGarbage( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	INSERT_TO_LIST( pPlayer->garbage, pItem, i_prev, i_next );
	pPlayer->garbageCnt++;

	ITEMDB_ToGarbage( pPlayer, pItem );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_INSERT_TO_GARBAGE, g_nPos );
	PutWord( g_Packet, pItem->itemNum, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutWord( g_Packet, GetItemStackCount(pItem), g_nPos );
	
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );

}

//
// Function Name: FindItemFromInven
// Date:2003/03/08
// Description: 
//
sPITEM_DATA FindItemFromInven( sPDESC_DATA pPlayer, int idxNum )
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->inven, item, next_item , i_next, WMgr );
	if( item->idxNum == idxNum )
		return item;
	LIST_WHILEEND( pPlayer->inven, item, next_item );
	return NULL;
}

//
// Function Name: FindItemFromGarbage
// Date:
// Description: 
//
sPITEM_DATA FindItemFromGarbage( sPDESC_DATA pPlayer, int idxNum )
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->garbage, item, next_item , i_next, WMgr );
	if( item->idxNum == idxNum )
		return item;
	LIST_WHILEEND( pPlayer->garbage, item, next_item );
	return NULL;
}

//
// Function Name: FindItemFromGarbage
// Date:
// Description: 
//
sPITEM_DATA FindItemFromSmith( sPDESC_DATA pPlayer, int idxNum )
{
	sPITEM_DATA item, next_item;
	LIST_WHILE( pPlayer->smith, item, next_item, i_next, WMgr );
	if( item->idxNum == idxNum )
		return item;
	LIST_WHILEEND( pPlayer->smith, item, next_item );
	return NULL;
}


//
// Function Name: FindItemFromInven
// Date:2003/03/08
// Description: 
//
sPITEM_DATA FindItemFromInvenByNum( sPDESC_DATA pPlayer, int itemNum )
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->inven, item, next_item , i_next, WMgr );
	if( item->itemNum == itemNum )
		return item;
	LIST_WHILEEND( pPlayer->inven, item, next_item );
	return NULL;
}





//
// Function Name: FindItemFromInvenByPos
// Date : 2003/03/08
// Description: 
//
sPITEM_DATA FindItemFromInvenByPos( sPDESC_DATA pPlayer, BYTE page, BYTE invenX, BYTE invenY )
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( pPlayer->inven, item, next_item, i_next, WMgr );
	if( item->invenPage == page &&
		item->invenX == invenX &&
		item->invenY == invenY )
		return item;
	LIST_WHILEEND( pPlayer->inven, item, next_item );

	return NULL;
}


//
// Function Name: FindPotionItem
// Date: 
// Description: 
//
sPITEM_DATA FindPotionItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pItem || !pPlayer )
		return NULL;

	sPITEM_DATA item, next_item;
	
	LIST_WHILE( pPlayer->inven, item, next_item , i_next, WMgr );
	if( item->itemNum == pItem->itemNum )
	{
		if( GetItemStackCount(item) + GetItemStackCount(pItem) <= GetItemStackLimit(pItem) )
			return item;
	}
	LIST_WHILEEND( pPlayer->inven, item, next_item );
	return NULL;	
}

//
// Function Name: FindItemFromEquip
// Date:
// Description: 
//
sPITEM_DATA FindItemFromEquip( sPDESC_DATA pPlayer, int idxNum )
{
	for( int i = 0; i < dMAX_EQUIP; i++ )
	{
		if( pPlayer->equip[i] )
			if( pPlayer->equip[i]->idxNum == idxNum )
				return pPlayer->equip[i];
	}
	return NULL;
}


//
// Function Name: FindItemMyArea
// Date:
// Description: 
//
sPITEM_DATA FindItemMyArea( sPDESC_DATA pPlayer, int idxNum )
{
	if( !IS_VALID_WORLD( pPlayer->ch.mapNum ) )
		return NULL;

	sPITEM_DATA item, next_item;

	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->item[y][x],
					item, next_item , w_next , WMgr );
				if( item->idxNum == idxNum )
					return item;
				LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->item[y][x],
					item, next_item );
			}
		}
	}
	return NULL;
}

//
// Function Name: FindItemMyHouse
// Date:
// Description: 
//
sPITEM_DATA FindItemMyHouse( sPDESC_DATA pPlayer, int idxNum )
{
	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR )
		return NULL;

	sPITEM_DATA item, next_item;

	LIST_WHILE( gHOUSE[GET_MAP_NUM( pPlayer )]->in_items, item, next_item, w_next, WMgr );

	if( item->idxNum == idxNum )
		return item;

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM( pPlayer )]->in_items, item, next_item );

	return NULL;
}





//
// Function Name: ClearPlayerItem
// Date:
// Description: 
//
void ClearPlayerItem( sPDESC_DATA pPlayer )
{
	/////////////////////////////////////
	// I N V E N T O R Y    I T E M
	/////////////////////////////////////
	sPITEM_DATA item, item_next;

	LIST_WHILE( pPlayer->inven, item, item_next, i_next, WMgr );

	// 포션일 경우 수량 저장
	if( GetItemStackLimit( item ) > 1 )
		ITEMDB_UpdateInvenItem( pPlayer, item );
	
	INSERT_ITEM_TO_MEMORY( item );
	LIST_WHILEEND( pPlayer->inven, item, item_next );

	//////////////////////////////////////
	// S M I T H	I T E M
	//////////////////////////////////////
	/*sPITEM_DATA smithitem, smithitem_next;
	LIST_WHILE( pPlayer->smith, smithitem, smithitem_next, i_next, WMgr );
	INSERT_ITEM_TO_MEMORY( smithitem );
	LIST_WHILEEND( pPlayer->smith, smithitem, smithitem_next );*/

	/////////////////////////////////////
	// E Q U I P    I T E M
	/////////////////////////////////////
	int i;
	for( i = 0; i < dMAX_EQUIP; i++ )
	{
		if( pPlayer->equip[i] )
		{
			INSERT_ITEM_TO_MEMORY( pPlayer->equip[i] );
			pPlayer->equip[i] = NULL;
		}
	}

	/////////////////////////////////////
	// G A R B A G E   I T E M
	/////////////////////////////////////
	LIST_WHILE( pPlayer->garbage, item, item_next, i_next, WMgr2 );
	INSERT_ITEM_TO_MEMORY( item );
	LIST_WHILEEND( pPlayer->garbage, item, item_next );

	// 빌링
	sPBILL_ITEM_DATA bill, next_bill;

	LIST_WHILE( pPlayer->billItems, bill, next_bill, next, WMgr3 );
	REMOVE_FROM_LIST( pPlayer->billItems, bill, prev, next );
	INSERT_TO_LIST( pPlayer->billItems, bill, prev, next );
	LIST_WHILEEND( pPlayer->billItems, bill, next_bill );
}



//
// Function Name: SendPlayerItemInfo
// Date:
// Description: 
//
void SendPlayerItemInfo( sPDESC_DATA pPlayer )
{
	if( !pPlayer->chDataLoaded )
		return;

	sPITEM_DATA item, next_item;
	WORD invenCnt = 0;
	BYTE equipCnt = 0;

	/////////////////////////////////////
	// 소지품 개수 구하기
	/////////////////////////////////////
	LIST_WHILE( pPlayer->inven, item, next_item, i_next, WMgr );
	invenCnt++;
	LIST_WHILEEND( pPlayer->inven, item, next_item );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PLAYERS_ITEM_INFO, g_nPos );
	PutWord( g_Packet, invenCnt, g_nPos );

	LIST_WHILE( pPlayer->inven, item, next_item, i_next, WMgr2 );
	int exVal = 0;
	if( GetItemStackLimit( item ) > 1 )
	{
		exVal = item->exVal[0];
	}
	else
	{
		exVal = 0;
	}

	PutInteger( g_Packet, item->idxNum, g_nPos );
	PutWord( g_Packet, item->itemNum, g_nPos );
	PutByte( g_Packet, item->invenPage, g_nPos );
	PutByte( g_Packet, item->invenX, g_nPos );
	PutByte( g_Packet, item->invenY, g_nPos );
	PutWord( g_Packet, exVal, g_nPos );	// 수량
	//PutByte( g_Packet, GET_ITEM_TYPE( item ) == dITEMTYPE_POTION ? item->exVal[0] : 0 , g_nPos );	// 수량

	LIST_WHILEEND( pPlayer->inven, item, next_item );

	/////////////////////////////////////////////
	// 장착 장비 갯수 구하기
	/////////////////////////////////////////////
	int i;
	for( i = 0; i < dMAX_EQUIP; i++ )
	{
		if( pPlayer->equip[i] )
			equipCnt++;
	}
	
	PutByte( g_Packet, equipCnt, g_nPos );

	for( i = 0; i < dMAX_EQUIP; i++ )
	{
		if( pPlayer->equip[i] )
		{
			PutInteger( g_Packet, pPlayer->equip[i]->idxNum, g_nPos );
			PutWord( g_Packet, pPlayer->equip[i]->itemNum, g_nPos );
		}
	}

	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );

	if( GET_HPADD( pPlayer ) || GET_MPADD( pPlayer ) )
		SendPlayerMaxHPMP( pPlayer );
}

//
// Function Name: SendChangeEquip
// Date:
// Description: 
//
void SendChangeEquip( sPDESC_DATA pPlayer, sPITEM_DATA pItem, BYTE io )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHANGE_EQUIP, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, pItem->itemNum, g_nPos );
	PutByte( g_Packet, io, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOOTHER );
}


//
// Function Name: EventItemProcess
// Date:
// Description: 
//
void EventItemProcess( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{

}

//
// Function Name: GetMonsterItem
// Date: 
// Description: 
//
int GetMonsterItem( sPMOB_DATA pMob, char *ownerName, char *masterName, sPDESC_DATA pKiller )
{

#ifdef _JamesVer2
	int g_KrEvMobItem[89] =
	{		160,	
			181,	
			577,	
			598,	
			997,	
			1018,	
			1483,	
			1418,	
			1838,	
			2257,	
			2276,	
			1860,	
			2657,	
			2698,	
			3098,	
			3118,	
			3517,	
			3535,	
			4313,	
			4334,	
			4628,	
			4646,	
			4941,	
			4963,	
			5320,	
			5342,	
			5635,	
			5654,	
			5951,	
			5971,	
			6328,	
			6349,	
			6643,	
			6663,	
			6956,	
			6978,	
			7335,	
			7357,	
			7652,	
			7671,	
			7966,	
			7986,	
			8280,	
			8298,	
			10522,	
			10526,	
			10542,	
			10564,	
			10568,	
			10756,	
			10883,	
			10879,	
			10899,	
			11236,	
			11367,	
			11613,	
			11739,	
			10111,	
			10117,	
			10118,	
			10120,	
			10119,	
			10117,	
			10118,	
			10119,	
			10120,	
			10120,	
			10122,	
			10123,	
			10124,	
			10125,	
			10126,	
			10127,	
			10128,	
			10129,	
			10130,	
			10131,	
			10132,	
			10133,	
			10134,	
			10135,	
			10136,	
			10189,	
			10190,	
			10391,	
			10401,	
			10416,	
			10419,	
			10432,	
	};
#endif
	if( pMob->mobNum == 101 || pMob->mobNum == 102 || pMob->mobNum == 103 )
	{
		// 이벤트몹 방어구
		int itemNum;

		if( !g_MOBINFO[pMob->mobNum]->currDropItem )
			g_MOBINFO[pMob->mobNum]->currDropItem = g_MOBINFO[pMob->mobNum]->inven;

		itemNum = g_KrEvMobItem[number( 0, 88 )]; //James 

		//itemNum = g_MOBINFO[pMob->mobNum]->currDropItem->itemNum; g_KrEvMobItem[number( 0, 89 )];
		sPITEM_DATA pItem = CreateItem( itemNum, "GetMonsterItem - Event", __FILE__, __LINE__ );
		
		if( pItem )
		{
			pItem->isMobDrop = 2;
			
			ItemToMap( pItem, pMob->mapNum, 
				pMob->posX + number( -3, 3 ),
				pMob->posY + number( -3, 3 ),
				pMob->idxNum );
		}

		// 이벤트몹 무기
		if( !g_MOBINFO[pMob->mobNum]->currDropItem )
			g_MOBINFO[pMob->mobNum]->currDropItem = g_MOBINFO[pMob->mobNum]->inven;
		itemNum = itemNum = g_KrEvMobItem[number( 0, 88 )];
		//itemNum = g_MOBINFO[pMob->mobNum]->currDropItem->itemNum;
		pItem = CreateItem( itemNum, "GetMonsterItem - Event", __FILE__, __LINE__ );
		
		if( pItem )
		{
			pItem->isMobDrop = 2;

			ItemToMap( pItem, pMob->mapNum, 
				pMob->posX + number( -3, 3 ),
				pMob->posY + number( -3, 3 ),
				pMob->idxNum );
		}


		if(LocalCardChance(1000)>0){
		// 웨폰 인첸트 
		if( number( 1, 2 ) == 1 )
		{
			pItem = CreateItem( dENCHANT_CARD_WEAPON, "GetMonsterItem", __FILE__, __LINE__ );

			if( pItem )
			{
				pItem->isMobDrop = 1;

				ItemToMap( pItem, pMob->mapNum, 
					pMob->posX + number( -3, 3 ),
					pMob->posY + number( -3, 3 ),
					pMob->idxNum );
			}
		}
		else // 아머 인첸트 
		{
			pItem = CreateItem( dENCHANT_CARD_ARMOR, "GetMonsterItem", __FILE__, __LINE__ );

			if( pItem )
			{
				pItem->isMobDrop = 1;

				ItemToMap( pItem, pMob->mapNum, 
					pMob->posX + number( -3, 3 ),
					pMob->posY + number( -3, 3 ),
					pMob->idxNum );
			}
		}

		}
		// 돈 1000 실드씩 22개
		for( int i = 0; i < 9; i++ )
		{
			pItem = CreateMoney(  number(dEVENT_MOB_DROP_MONEY_MIN, dEVENT_MOB_DROP_MONEY_MAX) );
			
			if( pItem )
			{
				ItemToMap( pItem, pMob->mapNum, 
					pMob->posX + number( -3, 3 ),
					pMob->posY + number( -3, 3 ),
					pMob->idxNum );
			}
		}
		return 0;
	}
//#endif

	// 소유한 아이템이 없을 경우
	if( !g_MOBINFO[pMob->mobNum]->inven )
		return 0;

	// Draw a fresh candidate each kill. A shared sequential cursor clusters
	// the two card entries, then excludes cards for the rest of the cycle.
	// Uniform selection preserves each entry's original long-run probability.
	const int dropCount = g_MOBINFO[pMob->mobNum]->invenCnt;
	if( dropCount <= 0 )
		return 0;
	sPMOB_INVEN pMobInven = g_MOBINFO[pMob->mobNum]->inven;
	int dropIndex = number( 0, dropCount - 1 );
	while( dropIndex-- > 0 && pMobInven )
		pMobInven = pMobInven->next;
	if( !pMobInven )
		return 0;

	int dropPercent = pMobInven->percent;

	int createCnt = 0;
	sPITEM_DATA pItem = NULL;

	if( pKiller )
	{
		// 신의축복 ( 인첸트 카드 드랍확률 증가 )
		if( pMobInven->itemNum == dENCHANT_CARD_ARMOR || pMobInven->itemNum == dENCHANT_CARD_WEAPON )
		{
			if( pKiller->ch2.billingFlag[dBILL_ITEM_EFFECT7] )			// 신의축복 
				dropPercent = (int)( pMobInven->percent * 3 );
		}

		// 아이템 오브 아라임 1.3배 
		if( pKiller->ch2.billingFlag[dBILL_ITEM_EFFECT3] )
			dropPercent = (int)( pMobInven->percent * 1.3 );
		// 아이템 오브 아라임 1.5배
		else if( pKiller->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
			dropPercent = (int)( pMobInven->percent * 1.5 );
		
		// 프리미엄 계정
		if( pKiller->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
			dropPercent = (int)( pMobInven->percent * 2 );
	}

	//구정 이벤트
	//dropPercent = (int)( dropPercent * 2 );

/*	if( g_btIsUBIEventDrop == TRUE )
	{
		dropPercent = (int)( dropPercent * 2 );
	}*/
	
	if(pMobInven->itemNum==dENCHANT_CARD_ARMOR||pMobInven->itemNum==dENCHANT_CARD_WEAPON)
        dropPercent=LocalCardChance(dropPercent);
	if( number( 1, 1000 ) <= dropPercent )
	{
		pItem = CreateItem( pMobInven->itemNum, "GetMonsterItem()", __FILE__, __LINE__ );
		
		if( pItem )
		{
			// 소유자 정보 입력
			if( *ownerName != NULL_STR )
			{
				strcpy( pItem->ownerName, ownerName );
				if( *masterName != NULL_STR )
					strcpy( pItem->masterName, masterName );
				//아이템드랍시간 //30000 < -기존5분
				pItem->ownerCheckTick = g_CurrTime + 12000;
			}
			
			pItem->isMobDrop = 1;
			
			ItemToMap( pItem, pMob->mapNum, 
				pMob->posX,
				pMob->posY,
				pMob->idxNum );
			
			//
			createCnt++;	
		}
	}

	

	if( !createCnt && number( 1, 100 ) <= 70  && number( 1, 100 ) <= GET_MOB_DROPDICE( pMob ) ) 
	{
		pItem = CreateMoney( number( MIN( GET_MOB_MONEY( pMob ) - 10, 0 ), GET_MOB_MONEY( pMob ) ) );
		
		if( pItem )
		{
			// 소유자 정보 입력
			if( *ownerName != NULL_STR )
			{
				strcpy( pItem->ownerName, ownerName );
				if( *masterName != NULL_STR )
					strcpy( pItem->masterName, masterName );
				pItem->ownerCheckTick = g_CurrTime + 30000;
			}
			
			pItem->isMobDrop = 1;
			
			ItemToMap( pItem, pMob->mapNum, 
				pMob->posX,
				pMob->posY,
				pMob->idxNum );
			return 1;	
		}
	}
	else
		return -1;	
	
	return 0;
}


//
// Function Name: CreateMoney
// Date:2003/05/10
// Description: 
//
sPITEM_DATA CreateMoney( int amount )
{
	sPITEM_DATA pItem = CreateItem( dMONEY_NUM , "CreateMoney()", __FILE__, __LINE__ );

#ifdef IS_TEST_SERVER
	if( g_ExpJump > 1 )
	{
		amount = amount * g_ExpJump;
	}
#endif

	if( pItem )
	{
		pItem->exVal[0] = amount;
		return pItem;
	}
	return NULL;
}


//
// Function Name: CastTeleport
// Date:
// Description: 
//
BOOL CastTeleport( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	switch( pItem->itemNum )
	{
	case dTELEPORT_ZILS:			// 질스(40)
		PlayerToStartPoint( pPlayer, 40 );
		break;
	case dTELEPORT_GOURSE:		// 고스(22)
		PlayerToStartPoint( pPlayer, 22 );
		break;
	case dTELEPORT_VAIL:			// 베일(3)
		PlayerToStartPoint( pPlayer, 3 );
		break;
	case dTELEPORT_MIRAS:		// 미라스(4)
		PlayerToStartPoint( pPlayer, 4 );
		break;
	case dTELEPORT_MOHAS:		// 모하스(6) 
		PlayerToStartPoint( pPlayer, 6 );
		break;
	case dTELEPORT_NERIS:
		PlayerToStartPoint( pPlayer, 8 );
		break;
	case dTELEPORT_SUHER:		// 수헤르(11)
		PlayerToStartPoint( pPlayer, 11 );
		break;
	case dTELEPORT_TAROS:		// 타로스(14)
		PlayerToStartPoint( pPlayer, 14 );
		break;
	case dTELEPORT_MAP15:		// 로비탄(15)		
		PlayerToStartPoint( pPlayer, 15 );
		break;
	case dTELEPORT_MAP17:		// 레벤(17)
		PlayerToStartPoint( pPlayer, 17 );
		break;
	default:
		return 0;
	}

	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pPlayer );
	else
		PlayerFromMap( pPlayer );

	SendMapLoading( pPlayer );

	return 1;
}

//
// Function Name: CastItem
// Date:2003/05/10
// Description: 
//
BOOL CastItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( GET_ITEM_TYPE( pItem ) != dITEMTYPE_SCROLL )
		return 0;

	
	switch( pItem->itemNum )
	{

	case dTELEPORT_ZILS:						// 질스(40)
	case dTELEPORT_GOURSE:					// 고스(22)
	case dTELEPORT_VAIL:						// 베일(3)
	case dTELEPORT_MIRAS:					// 미라스(4) 
	case dTELEPORT_MOHAS:
	case dTELEPORT_NERIS:
	case dTELEPORT_SUHER:					// 모하스(6) 								
	case dTELEPORT_TAROS:					// 타로스(14)
	case dTELEPORT_MAP15:					// 로비탄(15)
	case dTELEPORT_MAP17:					// 레벤(17)
	return CastTeleport( pPlayer, pItem );



	// 첫번째 말
	case dRIDE_ITEM1:
	case dRIDE_ITEM2:
	case dRIDE_ITEM3:
	case dRIDE_ITEM4:
		// Local play: mounts have no character-level requirement.

		// 버로우 스텔스
		UnSetVisibleSkill( pPlayer );

		if( GET_RIDE_STATE( pPlayer ) )
		{			
			GET_RIDE_TYPE( pPlayer ) = 0;
			GET_RIDE_STATE( pPlayer ) = 0;
			pItem->exVal[0] = 0;
			ITEMDB_UpdateInvenItem( pPlayer, pItem );
		}
		else
		{
			if( GET_MAP_NUM( pPlayer ) == dDEAL_MAP_NUM )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[203] );
				return 0;
			}

			if( pItem->itemNum == dRIDE_ITEM1 )			// 갈색말 
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE1;
			else if( pItem->itemNum == dRIDE_ITEM2 )	// 검정말
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE2;
			else if( pItem->itemNum == dRIDE_ITEM3 )	// 하양말
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE3;
			else if( pItem->itemNum == dRIDE_ITEM4 )
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE4;
			else
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE3;
			
			GET_RIDE_STATE( pPlayer ) = 1;
			pItem->exVal[0] = 1;
			ITEMDB_UpdateInvenItem( pPlayer, pItem );
		}
		
		SendRideInfo( pPlayer );
		
		return 0;
	}

	return 0;
}

//
// Function Name: FindItemFromDeal
// Date:2003/05/11
// Description: 
//
sPITEM_DATA FindItemFromDeal( sPDESC_DATA pPlayer, int idxNum )
{
	sPLIST_BLOCK list, block, block_next;
	sPITEM_DATA pItem;

	list = pPlayer->ch2.dealItems.GetFirstBlock();

	LIST_WHILE( list, block, block_next, next, WMgr );
	pItem = (sPITEM_DATA)block->data;
	if( pItem->idxNum == idxNum )
		return pItem;
	LIST_WHILEEND( list, block, block_next );
	return NULL;
}





//
// Function Name: CheckItemLifeInMap
// Date: 2003-05-10
// Description: 
//
void CheckItemLifeInMap()
{
	sPITEM_DATA item, next_item;

	LIST_WHILE( gITEMS.list, item, next_item, next, WMgr );

	item->life_in_map--;

	if( item->life_in_map <= 0 )
	{
		// 아이템 소멸
		if( item->mapKind == dMAP_KIND_INDOOR )
			ItemFromHouse( item );
		else
			ItemFromMap( item );
		INSERT_ITEM_TO_MEMORY( item );
	}

	LIST_WHILEEND( gITEMS.list, item, next_item );
}


//
// Function Name: CheckEmptyInven
// Date: 
// Description: 
//
BOOL CheckEmptyInven( sPDESC_DATA pPlayer , sPCHECK_INVEN pInven )
{
	int page, invenX, invenY;

	for( page = 0; page < dINVEN_PAGE; page++ )
	{
		for( invenY = 0; invenY < dINVEN_Y; invenY++ )
		{
			for( invenX = 0; invenX < dINVEN_X; invenX++ )
			{
				if( !pPlayer->invenArray[page][invenY][invenX] )
				{
					pInven->page = page;
					pInven->y = invenY;
					pInven->x = invenX;
					return 1;
				}
			}
		}
	}
	return 0;
}


//
// Function Name: CountEmptyInven
// Date: 
// Description: 
//
int CountEmptyInven( sPDESC_DATA pPlayer )
{
	int cnt = 0;

	int page, invenX, invenY;
	
	for( page = 0; page < dINVEN_PAGE; page++ )
	{
		for( invenY = 0; invenY < dINVEN_Y; invenY++ )
		{
			for( invenX = 0; invenX < dINVEN_X; invenX++ )
			{
				if( !pPlayer->invenArray[page][invenY][invenX] )
				{
					cnt++;
				}
			}
		}
	}
	return cnt;
}


//
// Function Name: FillInvenArray
// Date: 
// Description: 
//
void FillInvenArray( sPDESC_DATA pPlayer, BYTE page, BYTE invenX, BYTE invenY )
{
	if( pPlayer->invenArray[page][invenY][invenX] )
	{
		log( "FillInvenArray : pPlayer->invenArray[page][invenY][invenX] / [%s/%s]\r\n", 
			GET_ID( pPlayer ), GET_NAME( pPlayer ) );
		return;
	}

	pPlayer->invenArray[page][invenY][invenX] = 1;
}

//
// Function Name: UnFillInvenArray
// Date: 
// Description: 
//
void UnFillInvenArray( sPDESC_DATA pPlayer, BYTE page, BYTE invenX, BYTE invenY )
{
	pPlayer->invenArray[page][invenY][invenX] = 0;
}

//
// Function Name: SendUpdatePotionCnt
// Date: 
// Description: 
//
void SendUpdatePotionCnt( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( GetItemStackLimit(pItem) <= 1 )
		return;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_POTIONCNT, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutWord( g_Packet, GetItemStackCount(pItem), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer , g_Packet, g_nPos );
	ITEMDB_UpdateInvenItem(pPlayer,pItem);
}


//
// Function Name: SendUpdatePotionCnt
// Date: 
// Description: 
//
void SendUpdateEnchantCnt( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( GET_ITEM_TYPE( pItem ) != dITEMTYPE_CONTAINER )
		return;
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_CARDCNT, g_nPos );
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutWord( g_Packet, GetItemStackCount(pItem), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer , g_Packet, g_nPos );
}


//
// Function Name: UseHPPotion
// Date: 
// Description: Use item Number
//
void UseHPPotion( sPDESC_DATA pPlayer, int itemNum )
{
	sPITEM_DATA pItem = NULL;
	
	pItem = FindItemFromInvenByNum( pPlayer, itemNum );

	if( pItem )
	{		
		if( GET_HP( pPlayer ) >= ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
			return;
		GET_HP( pPlayer ) = MAX( GET_HP( pPlayer ) + GET_ITEM_MINDAMAGE( pItem ), GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
		SendPlayerHP( pPlayer );

		// 수량 
		if( pItem->exVal[0] <= 0 )
		{
			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
		}
		else
		{
			pItem->exVal[0]--;	 		 

			if( pItem->exVal[0] == 0 )
			{
				ItemFromInventory( pPlayer, pItem );
				INSERT_ITEM_TO_MEMORY( pItem );
			}
			else
			{
				SendUpdatePotionCnt( pPlayer, pItem );
#ifdef USE_ITEM_WEIGHT
				GET_CURRITEM_WEIGHT( pPlayer ) -= GET_ITEM_WEIGHT( pItem );
				SendWeightGageInfo( pPlayer );
#endif
			}
		}	
		// Sound Effect
		switch( itemNum )
		{
		case dHP_POSION_S:
			SendSoundEffect( pPlayer, SOUND_EFFECT_HPPOTION_S );
			break;
		case dHP_POSION_M:
			SendSoundEffect( pPlayer, SOUND_EFFECT_HPPOTION_M );
			break;
		case dHP_POSION_L:
			SendSoundEffect( pPlayer, SOUND_EFFECT_HPPOTION_L );
			break;
		}
	}
}

//
// Function Name: UseMPPotion
// Date: 
// Description: Use Item Number
//
void UseMPPotion( sPDESC_DATA pPlayer, int itemNum )
{
	sPITEM_DATA pItem = NULL;
	
	pItem = FindItemFromInvenByNum( pPlayer, itemNum );
	
	if( pItem )
	{
		if( GET_MP( pPlayer ) >= GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) )
			return;
		
		GET_MP( pPlayer ) = MAX( GET_MP( pPlayer ) + GET_ITEM_MINDAMAGE( pItem ), GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
		SendPlayerMP( pPlayer );

		// 수량 
		if( pItem->exVal[0] <= 0 )
		{
			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
		}
		else
		{
			pItem->exVal[0]--;
		
			if( pItem->exVal[0] == 0 )
			{
				ItemFromInventory( pPlayer, pItem );
				INSERT_ITEM_TO_MEMORY( pItem );
			}
			else
			{
				SendUpdatePotionCnt( pPlayer, pItem );
#ifdef USE_ITEM_WEIGHT
				GET_CURRITEM_WEIGHT( pPlayer ) -= GET_ITEM_WEIGHT( pItem );
				SendWeightGageInfo( pPlayer );
#endif
			}
		}
		// Sound Effect
		switch( itemNum )
		{
		case dMP_POSION_S:
			SendSoundEffect( pPlayer, SOUND_EFFECT_MPPOTION_S );
			break;
		case dMP_POSION_M:
			SendSoundEffect( pPlayer, SOUND_EFFECT_MPPOTION_M );
			break;
		case dMP_POSION_L:
			SendSoundEffect( pPlayer, SOUND_EFFECT_MPPOTION_L );
			break;
		}
	}
}

//
// Function Name: CheckInvalidDealItem
// Date: 
// Description: 
//
void UseEnchantCardBox( sPDESC_DATA pPlayer, sPITEM_DATA pItem , int useCnt )
{
	if( pItem )
	{
		// 수량 
		if( pItem->exVal[0] <= 0 )
		{
			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );

		}
		else
		{
			pItem->exVal[0] = pItem->exVal[0] - useCnt;
			
			if( pItem->exVal[0] == 0 )
			{
				ItemFromInventory( pPlayer, pItem );
				INSERT_ITEM_TO_MEMORY( pItem );
			}
			else
			{
				SendUpdateEnchantCnt( pPlayer, pItem );
			}
		}
	}
}
	

//
// Function Name: CheckInvalidDealItem
// Date: 
// Description: 
//
BOOL CheckInvalidDealItem( sPDESC_DATA pPlayer ) 
{
	// 아이템 들
	sPLIST_BLOCK list, block, next_block;
	sPITEM_DATA pDealItem;
	sPITEM_DATA pTmpItem;
	
	// pClient ==> pDealVict
	list = pPlayer->ch2.dealItems.GetFirstBlock();
	
	LIST_WHILE( list, block, next_block, next, WMgr );
	
	pDealItem = (sPITEM_DATA)block->data;
	
	pTmpItem = FindItemFromInven( pPlayer, pDealItem->idxNum );

	if( !pTmpItem )
		return 0;
	
	LIST_WHILEEND( list, block, next_block );
	
	return 1;
}


//
// Function Name: CheckItemEffect
// Date: 
// Description: 이벤트 아이템 체크
//
void CheckItemEffect( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{


}

//
// Function Name: CheckItemValue
// Date:2003/04/12
// Description: 
//
void CheckItemValue( sPDESC_DATA pPlayer , sPITEM_DATA pItem )
{
	switch( pItem->itemNum )
	{
	case dRIDE_ITEM1:
	case dRIDE_ITEM2:
	case dRIDE_ITEM3:
	case dRIDE_ITEM4:
		if( pItem->exVal[0] )
		{
			// 레벨이 낮은데 말을 타고 있을 경우
			// Preserve low-level mounts across login.

			if( pItem->itemNum == dRIDE_ITEM1 )
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE1;
			else if( pItem->itemNum == dRIDE_ITEM2 )
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE2;
			else if( pItem->itemNum == dRIDE_ITEM4 )
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE4;
			else
				GET_RIDE_TYPE( pPlayer ) = dRIDE_TYPE3;

			GET_RIDE_STATE( pPlayer ) = pItem->exVal[0];
		}
		break;
	}
}

//
// Function Name: SendItemEventResult
// Date:
// Description: 
//
void SendItemEventResult( sPDESC_DATA pPlayer, BYTE res )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_ITEM_EVENT, g_nPos );
	PutByte( g_Packet, res, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: LoadNeedLogTable
// Date:
// Description: 
//
BYTE LoadNeedLogTable()
{
	FILE *fp = fopen( dNEED_ITEMLOG_LIST_FILE, "rb" );

	if( !fp )
	{
		log( "LoadNeedLogTable : file not found\r\n" );
		return 0;
	}

	char readBuff[1024];
	int itemNum = 0;	

	while( !feof( fp ) )
	{	
		int nPos = 0;
		
		fgets( readBuff, 1024, fp );
		
		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;
	
		itemNum = GetTabNumberic( readBuff, nPos );
		
		if( !IS_VALID_ITEM( itemNum ) )
			continue;

		g_ItemInfo[itemNum]->needItemLog = 1;
	}
	fclose( fp );
	
	// 반지 목걸이들
	g_ItemInfo[dAMULET_TRACKING]->needItemLog = 1;		
	g_ItemInfo[dAMULET_REINCARNATION]->needItemLog = 1;	
	g_ItemInfo[dAMULET_INVISIBILITY]->needItemLog = 1;	
	g_ItemInfo[dAMULET_TRAMPING]->needItemLog = 1;		
	g_ItemInfo[dAMULET_CHANNELING]->needItemLog = 1;		
	g_ItemInfo[dAMULET_LIONSENDURANCE]->needItemLog = 1;	
	g_ItemInfo[dAMULET_ARCANEMAGIC]->needItemLog = 1;		
	
	g_ItemInfo[dRING_BARTERMASTERY]->needItemLog = 1;		
	g_ItemInfo[dRING_HOMEWARD]->needItemLog = 1;			
	g_ItemInfo[dRING_WIZARDRY]->needItemLog = 1;			
	g_ItemInfo[dRING_DISTURBANCE_1]->needItemLog = 1;		
	g_ItemInfo[dRING_DISTURBANCE_2]->needItemLog = 1;		
	g_ItemInfo[dRING_DISTURBANCE_3]->needItemLog = 1;		
	g_ItemInfo[dRING_FEAR_1]->needItemLog = 1;			
	g_ItemInfo[dRING_FEAR_2]->needItemLog = 1;			
	g_ItemInfo[dRING_FEAR_3]->needItemLog = 1;	

#ifdef _dUMENOKOJI_				//마나 베리어 링 관련
	g_ItemInfo[dRING_BARRIER]->needItemLog = 1;
#endif
	

	return 1;
}


//
// Function Name: ChangeEquipItem
// Date:
// Description: 
//
void ChangeEquipItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem, BYTE equipWhere )
{
	if( equipWhere == 255 )
	{
		log( "ChangeEquipItem : equipWhere is 255\r\n" );
		return;
	}
	

	if( pItem == NULL )
	{
		log( "pItem == NULL\r\n " );
		return;
	}
	
	if( !CheckItemLevel( pPlayer, pItem ) )
	{
		SendSystemMsg( pPlayer, "%s", g_LANG_STR[283] );
		return ;
		
	}

	sPITEM_DATA equipItem = NULL;
	sCHECK_INVEN checkInven;

	equipItem = pPlayer->equip[equipWhere];

	// 이미 해당 위치에 아이템이 있는 경우 
	if( equipItem )
	{
		checkInven.page = pItem->invenPage;
		checkInven.x = pItem->invenX;
		checkInven.y = pItem->invenY;
		
		ItemFromEquip( pPlayer, equipItem );
		ItemFromInventory( pPlayer, pItem );
		ItemToInventory( pPlayer, equipItem, &checkInven );
		ItemToEquip( pPlayer, pItem, equipWhere );
	}
	else
	{
		ItemFromInventory( pPlayer, pItem );
		ItemToEquip( pPlayer, pItem, equipWhere );
	}
}

//
// Function Name: CheckEquipItem
// Date:
// Description: 
//
BOOL CheckEquipItem( sPDESC_DATA pPlayer, BYTE equipWhere, int itemNum )
{
	if( !pPlayer->equip[equipWhere] )
		return 0;
	
	if( pPlayer->equip[equipWhere]->itemNum != itemNum )
		return 0;

	return 1;
}


//
// Function Name: ItemFuncUserFind
// Date:
// Description: 
//
void ItemFuncUserFind( sPDESC_DATA pPlayer, char *command )
{
	if( !CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_TRACKING ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[142], GET_ITEM_HNAME2( dAMULET_TRACKING ) );
		return;
	}

	char name[128];
	/*
#ifndef __CHINA_VER__
	if( sscanf( command, "탐색 %s", name ) != 1 )
#else
	if( sscanf( command, "FIND %s", name ) != 1 )
#endif*/
//	if( sscanf( command, "FIND %s", name ) != 1 )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[114] );
		return;
	}

	if( GET_STATE( pTarget ) != dDESC_STATE_PLAYING &&
		GET_STATE( pTarget ) != dDESC_STATE_FIGHT )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[143] );
		return;
	}

	StrSendSysMsg( pPlayer, 144, GET_NAME( pTarget ), gWORLD[GET_MAP_NUM( pTarget )]->mapName, GET_POSX( pTarget ), GET_POSY( pTarget ) );
	//SendSystemMsg( pPlayer, g_LANG_STR[144], GET_NAME( pTarget ), gWORLD[GET_MAP_NUM( pTarget )]->mapName, GET_POSX( pTarget ), GET_POSY( pTarget ) );
}

void ItemFuncUserFindENG( sPDESC_DATA pPlayer, char *command )
{
	if( !CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_TRACKING ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[142], GET_ITEM_HNAME2( dAMULET_TRACKING ) );
		return;
	}

	char name[128];
	
	if( sscanf( command, "FIND %s", name ) != 1 ||
		sscanf( command, "find %s", name ) )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[114] );
		return;
	}

	if( GET_STATE( pTarget ) != dDESC_STATE_PLAYING &&
		GET_STATE( pTarget ) != dDESC_STATE_FIGHT )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[143] );
		return;
	}

	StrSendSysMsg( pPlayer, 144, GET_NAME( pTarget ), gWORLD[GET_MAP_NUM( pTarget )]->mapName, GET_POSX( pTarget ), GET_POSY( pTarget ) );
	//SendSystemMsg( pPlayer, g_LANG_STR[144], GET_NAME( pTarget ), gWORLD[GET_MAP_NUM( pTarget )]->mapName, GET_POSX( pTarget ), GET_POSY( pTarget ) );
}

//
// Function Name: ItemFuncRevival
// Date:
// Description: 
//
void ItemFuncRevival( sPDESC_DATA pPlayer, char *command )
{
	if( !CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_REINCARNATION ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[142] , GET_ITEM_HNAME2( dAMULET_REINCARNATION ) );
		return;
	}
	
	char name[128];
	
//#ifndef __CHINA_VER__
	if( sscanf( command, "부활 %s", name ) != 1 )
//#else
//	if( sscanf( command, "REVERSE %s", name ) != 1 )
//#endif
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );
	
	if( !pTarget )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[114] );
		return;
	}

	if( pTarget == pPlayer )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[89] );
		return;
	}

	if( GET_MAP_NUM( pPlayer ) != GET_MAP_NUM( pTarget ) ||
		abs( GET_POSX( pPlayer ) - GET_POSX( pTarget ) ) > 20 ||
		abs( GET_POSY( pPlayer ) - GET_POSY( pTarget ) ) > 20 )
		return;

	if( GET_DEAD_STATE( pTarget ) != dDEAD_STATE_CORPSE )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[145] );
		return;
	}
	
	if( GET_MAP_KIND( pTarget ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pTarget );
	else
		PlayerFromMap( pTarget );
	
#ifdef dUSE_DEAD_GHOST
	pPlayer->ch2.isDeadGhost = 0;
#endif

	GET_HP( pTarget ) = (int)( GET_MAX_HP( pTarget ) * 0.3 );

	SendPlayerHP( pTarget );
	SendPlayerMP( pTarget );
	
	GET_GOTO_MAPKIND( pTarget ) = GET_MAP_KIND( pTarget );
	GET_GOTO_MAPNUM( pTarget ) = GET_MAP_NUM( pTarget );
	GET_GOTO_POSX( pTarget ) = GET_POSX( pTarget );
	GET_GOTO_POSY( pTarget ) = GET_POSY( pTarget );
	
	GET_DEAD_STATE( pTarget ) = dDEAD_STATE_NONE;

	SendMapLoading( pTarget );
}

void ItemFuncRevivalENG( sPDESC_DATA pPlayer, char *command )
{
	if( !CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_REINCARNATION ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[142] , GET_ITEM_HNAME2( dAMULET_REINCARNATION ) );
		return;
	}

	char name[128];

	//#ifndef __CHINA_VER__
	if( sscanf( command, "REVIVE %s", name ) != 1 ||
		sscanf( command, "revive %s", name) )
		return;

	sPDESC_DATA pTarget = FindPlayerNameList( name );

	if( !pTarget )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[114] );
		return;
	}

	if( pTarget == pPlayer )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[89] );
		return;
	}

	if( GET_MAP_NUM( pPlayer ) != GET_MAP_NUM( pTarget ) ||
		abs( GET_POSX( pPlayer ) - GET_POSX( pTarget ) ) > 20 ||
		abs( GET_POSY( pPlayer ) - GET_POSY( pTarget ) ) > 20 )
		return;

	if( GET_DEAD_STATE( pTarget ) != dDEAD_STATE_CORPSE )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[145] );
		return;
	}

	if( GET_MAP_KIND( pTarget ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pTarget );
	else
		PlayerFromMap( pTarget );

#ifdef dUSE_DEAD_GHOST
	pPlayer->ch2.isDeadGhost = 0;
#endif

	GET_HP( pTarget ) = (int)( GET_MAX_HP( pTarget ) * 0.3 );

	SendPlayerHP( pTarget );
	SendPlayerMP( pTarget );

	GET_GOTO_MAPKIND( pTarget ) = GET_MAP_KIND( pTarget );
	GET_GOTO_MAPNUM( pTarget ) = GET_MAP_NUM( pTarget );
	GET_GOTO_POSX( pTarget ) = GET_POSX( pTarget );
	GET_GOTO_POSY( pTarget ) = GET_POSY( pTarget );

	GET_DEAD_STATE( pTarget ) = dDEAD_STATE_NONE;

	SendMapLoading( pTarget );
}

//
// Function Name: ItemFuncGotoHome
// Date:
// Description: 
//
void ItemFuncGotoHome( sPDESC_DATA pPlayer, char *command )
{
	if( !CheckEquipItem( pPlayer, dEQUIP_RING, dRING_HOMEWARD ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[142] , GET_ITEM_HNAME2( dRING_HOMEWARD ) );
		return;
	}

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return;

	if( GET_HP( pPlayer ) < (int)( ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) * 0.5 ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[146] );
		return;
	}
	
	if( pPlayer->ch2.speSkillDelay[dSPESKILL_DELAYIDX_HOMEWARDRING] + 600 > g_CurrRealTime )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[149], ( pPlayer->ch2.speSkillDelay[dSPESKILL_DELAYIDX_HOMEWARDRING] + 600 ) - g_CurrRealTime );
		return;
	}
	else
		pPlayer->ch2.speSkillDelay[dSPESKILL_DELAYIDX_HOMEWARDRING] = g_CurrRealTime;


	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
		PlayerFromIndoor( pPlayer );
	else
		PlayerFromMap( pPlayer );
			
	if( GET_MAP_NUM( pPlayer ) == dDEAL_MAP_NUM )
	{
		pPlayer->ch2.gotoMapNum = pPlayer->ch.prevMapNum;
		pPlayer->ch2.gotoPosX = pPlayer->ch.prevPosX;
		pPlayer->ch2.gotoPosY = pPlayer->ch.prevPosY;
		pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
	}
	else
		PlayerToStartPoint( pPlayer ); 
	
	SendMapLoading( pPlayer );
}


//
// Function Name: ItemFuncGotoHome
// Date:
// Description: 
//

void ItemFuncCallGuild( sPDESC_DATA pPlayer, char *command )
{

	if( !CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_CALLGUILD ) )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[142] , GET_ITEM_HNAME2( dAMULET_CALLGUILD ) );
		return;
	}
	

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE )
		return;

	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[212] );
				return;
			}

	if( pPlayer->ch2.guildSummonTime + GetSecondMin( 10 ) > g_CurrRealTime )
			{
				int minVal = (int)( ( pPlayer->ch2.guildSummonTime + GetSecondMin( 10 ) - g_CurrRealTime ) / 60 );

				SendSystemMsg( pPlayer, g_LANG_STR[213], minVal );
				return;
			}
	
	sPDESC_DATA d, next_d;
	int cnt = 0;

	LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
		if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
				GET_STATE( d ) == dDESC_STATE_FIGHT )
			{
				if( d != pPlayer && GET_GUILD( d ) == GET_GUILD( pPlayer ) )
					{
					d->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
					d->ch2.gotoPosX = GET_POSX( pPlayer );
					d->ch2.gotoPosY = GET_POSY( pPlayer );
					d->ch2.gotoPosMapKind = dMAP_KIND_FIELD;

					PlayerFromMap( d );
					SendMapLoading( d );
					cnt++;
						
					if( cnt >= 3 )
					break;
					}
			}
	
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
	
//	useOk = 1;
	
	pPlayer->ch2.guildSummonTime = g_CurrRealTime;
//	SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL29 );

}


//
// Function Name: ItemFuncWarPoint
// Date:
// Description: 
//
void ItemFuncWarPoint( sPDESC_DATA pPlayer, char *command )
{
	if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_SECONDMASTER &&
		GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
		return;

	sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];

	if( !pWorld )
		return;

	if( !pWorld->underAttack )
		return;

	if( pWorld->ownerGuild != GET_GUILD( pPlayer ) &&
		pWorld->warCristal.allowGuild[1] != GET_GUILD( pPlayer ) )
		return;
	
	/*StrSendSysMsg( pPlayer, 195, 
		pWorld->mapName, 
		GET_GUILD_DATA( pWorld->warCristal.allowGuild[1] )->guildName,
		" castle has %d war point.", pWorld->warPoint );*/
	SendSystemMsg( pPlayer, g_LANG_STR[195], pWorld->mapName, GET_GUILD_DATA( pWorld->warCristal.allowGuild[1] )->guildName, pWorld->warPoint );
}


void ItemFuncEnchant( sPDESC_DATA pPlayer, char *command )
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


void ItemFuncGetMember( sPDESC_DATA pPlayer, char *command )
{

	sPDESC_DATA d, next_d;
	sPDESC_DATA pTarget = NULL;

	//if( GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_SECONDMASTER &&
	//	GET_GUILD_LEVEL( pPlayer ) != dGUILD_LEVEL_MASTER )
	//	return;
	if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		return;

	
	LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
	
	if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
			GET_STATE( d ) == dDESC_STATE_FIGHT ||
				GET_STATE( d ) == dDESC_STATE_LOADING ){
		if( GET_GUILD( d ) == GET_GUILD( pPlayer ) ){
			pTarget = d;
			sPWORLD pWorld = gWORLD[GET_MAP_NUM( pTarget )];
			SendSystemMsg( pPlayer, "\t%s:\t%s",pWorld->mapName, pTarget->ch.name);	
		}
	}
	
	LIST_WHILEEND( gPLAYERS.list, d, next_d );

		return;

}



BOOL SendBillItemInfoStr( sPDESC_DATA pPlayer, int effectNum, char *itemName )
{
	if( effectNum >= dMAX_BILLING_FLAG )
		return 0;

	if( pPlayer->ch2.billingFlag[effectNum] )
	{
		if( pPlayer->ch2.billingFlag[effectNum] > dBILL_BEGIN_TIMESTAMP )
			SendSystemMsg( pPlayer, g_LANG_STR[204], itemName, GetRemindTimeStr( abs( (int)(pPlayer->ch2.billingFlag[effectNum] - g_CurrRealTime) ) ) );
		else
			SendSystemMsg( pPlayer, g_LANG_STR[204], itemName, GetRemindTimeStr( pPlayer->ch2.billingFlag[effectNum] ) );
		return 1;
	}
	return 0;
}

void ItemFuncBillInfo( sPDESC_DATA pPlayer, char *command )
{
#ifndef dDAUM_BILLING
	return;
#endif
	BOOL Ret = 0;

	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT1, "Warmth of Araim x1.3" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT2, "Warmth of Araim x1.5" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT3, "Item of Araim x1.3" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT4, "Item of Araim x1.5" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT5, "Premium Zone" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT6, "Premium Zone+" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT7, "God Blessing" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT8, "Potion of Haste" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT9, "Weight Drink" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT10, "HP Drink" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT11, "MP Drink" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT12, "Group Warmth of Araim" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT13, "Group HP Drink" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT14, "Group MP Drink" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT15, "Guild HP Drink" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT16, "Guild MP Drink" );
	Ret += SendBillItemInfoStr( pPlayer, dBILL_ITEM_EFFECT17, "Minus Card" );

	if( !Ret )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[205] );
	}
}



//
// Function Name: const struct admin_command admin_cmd[]
// Date: 
// Description: 
//
const struct item_command item_cmd[] =
{
/*#ifndef __CHINA_VER__
	{	"탐색",						ItemFuncUserFind		},
	{	"부활",						ItemFuncRevival			},
	{	"귀환",						ItemFuncGotoHome		},
#ifdef dDAUM_BILLING
	{	"정보",						ItemFuncBillInfo		},
#endif
#else
	{	"home",						ItemFuncGotoHome		},
	{	"HOME",						ItemFuncGotoHome		},
	{	"call",						ItemFuncCallGuild		},
	{	"CALL",						ItemFuncCallGuild		},	
	{	"memberlist",				ItemFuncGetMember		},
	{	"MEMBERLIST",				ItemFuncGetMember		},
//	{	"deicidepw$#@!",				ItemFuncEnchant		},
#endif
	{	"point",					ItemFuncWarPoint		},
	{	"POINT",					ItemFuncWarPoint		},
	{	"cash",						ItemFuncBillInfo		},
	{	"CASH",						ItemFuncBillInfo		},
	{	"\n",						NULL	},					// End*/
	{	"탐색",						ItemFuncUserFind		},
	{	"find",						ItemFuncUserFindENG		},
	{	"FIND",						ItemFuncUserFindENG		},

	{	"부활",						ItemFuncRevival			},
	{	"reverse",					ItemFuncRevivalENG		},
	{	"REVIVE",					ItemFuncRevivalENG		},

	{	"귀환",						ItemFuncGotoHome		},
	{	"home",						ItemFuncGotoHome		},
	{	"HOME",						ItemFuncGotoHome		},

	{	"집합",						ItemFuncCallGuild		},
	{	"call",						ItemFuncCallGuild		},
	{	"CALL",						ItemFuncCallGuild		},	

	{	"전쟁포인트",				ItemFuncWarPoint		},
	{	"warpoint",					ItemFuncWarPoint		},
	{	"WARPOINT",					ItemFuncWarPoint		},

	{	"유료템",					ItemFuncBillInfo		},
	{	"cash",						ItemFuncBillInfo		},
	{	"CASH",						ItemFuncBillInfo		},

	{	"길원",						ItemFuncGetMember		},
	{	"memberlist",				ItemFuncGetMember		},
	{	"MEMBERLIST",				ItemFuncGetMember		},
	
	{	"\n",						NULL	},					// End*/
};


//
// Function Name: CheckAdminCommand
// Date: 2003/03/16
// Description: 
//
BOOL CheckItemCommand( sPDESC_DATA pPlayer, char *command )
{
	int cmd = 0;

	CWhileMgr WMgr;

	while( 1 )
	{
		WMgr.CheckDump( __FILE__, __LINE__ );
	
		if( *item_cmd[cmd].command == '\n' )
			return 0;

		if( !strncmp( item_cmd[cmd].command, command, strlen( item_cmd[cmd].command ) ) )
		{
			item_cmd[cmd].item_func( pPlayer, command );
			return 1;
		}
		cmd++;		
	}
	return 0;
}


//
// Function Name: ITEMMGR_ItemToGarbage
// Date: 2003/03/16
// Description: 
//
void ITEMMGR_ItemToGarbage( char *ip, int itemNum, int cnt )
{
	return;
	MYSQL *pDB = DBConnect( ip, 
							3306, 
							"root", 
							"ageofcignare2215",
							"LAQIA_GAMEDB" );

	if( !pDB )
	{
		log( "ITEMMGR_ItemToGarbage : !pDB\r\n" );
		return;
	}

	FILE *fp = fopen( "ITEMMGR_ItemToGarbage.txt", "wb" );

	char query[1024];

	sprintf( query, "select id from UserTable group by id" );

	MYSQL_RES *result;
	MYSQL_ROW row;

	if( !mysql_query( pDB, query ) )
	{
		result = mysql_store_result( pDB );
		
		if( result )
		{
			while( ( row = mysql_fetch_row( result ) ) )
			{
				char escID[128];
				
				mysql_escape_string( escID, row[0], strlen( row[0] ) );

#ifdef USE_ITEM_SERIAL
				sprintf( query, "insert into UserGarbage values( '', '%s', %d, %d, %d, %d, %d, '%s' )",
					escID, itemNum, 0, 0, 0, 0, GetItemSerial() );
#else
				sprintf( query, "insert into UserGarbage values( '', '%s', %d, %d, %d, %d, %d )",
					escID, itemNum, 0, 0, 0, 0 );
#endif
				for( int i = 0; i < cnt; i++ )
				{
					mysql_query( pDB, query );
				}

				fprintf( fp, "%s\t%d\t%d\r\n", row[0], itemNum, cnt );
			}
			mysql_free_result( result );
		}
	}
	
	fclose( fp );
	mysql_close( pDB );
}

//
// Function Name: SendPlayerGarbageMoney
// Date: 
// Description: 
// 
void SendPlayerGarbageMoney( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_GARBAGEMONEY, g_nPos );
	PutInteger( g_Packet, pPlayer->garbageMoney, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: SendPlayerGarbageMoney
// Date: 
// Description: 
// 
void SendPlayerCashMoney( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_CASH_MONEY, g_nPos  );
	PutInteger( g_Packet, GET_CASHMONEY(pPlayer), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: GetItemSerial
// Date: 
// Description: 
// 
int g_ItemSerialIdx = 0;

char *GetItemSerial()
{
	static char serial[128];

	g_ItemSerialIdx++;

	time_t currTime = time( NULL );

	struct tm *currTM = localtime( &currTime );

	sprintf( serial, "%02d%02d%02d%02d%02d-%08d", 
		currTM->tm_year - 100, 
		currTM->tm_mon + 1, 
		currTM->tm_mday, 
		currTM->tm_hour, 
		currTM->tm_min, 
		g_ItemSerialIdx );

	if( g_ItemSerialIdx >= 99999999 )
		g_ItemSerialIdx++;

	return serial;
}

//
// Function Name: UseOtherInvenItem
// Date: 
// Description: 
// 
BOOL UseOtherInvenItem( sPDESC_DATA pPlayer, int itemNum )
{
	sPITEM_DATA pItem = NULL;

	pItem = FindItemFromInvenByNum( pPlayer, itemNum );

	if( pItem )
	{
		if( pItem->exVal[0] > 1 )
		{
			pItem->exVal[0] = MIN( pItem->exVal[0] - 1, 0 );
			SendUpdatePotionCnt( pPlayer, pItem );
			// 로그
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );

			return 1;
		}
		else
		{
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );

			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
			// 로그

			return 1;
		}
	}
	return 0;
}

BOOL UseOtherInvenItemCnt( sPDESC_DATA pPlayer, int itemNum, int cnt )
{
	sPITEM_DATA pItem = NULL;

	pItem = FindItemFromInvenByNum( pPlayer, itemNum );

	if( pItem )
	{
		if( pItem->exVal[0] == 0 )
		{
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );

			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
			// 로그
			ITEMDB_UpdateInvenItem( pPlayer, pItem );

			return 1;
		}

		if( pItem->exVal[0] >= cnt )
		{
			pItem->exVal[0] = MIN( pItem->exVal[0] - cnt, 0 );
			if( pItem->exVal[0] == 0 )
			{
				ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );

				ItemFromInventory( pPlayer, pItem );
				INSERT_ITEM_TO_MEMORY( pItem );
				// 로그
				ITEMDB_UpdateInvenItem( pPlayer, pItem );

				return 1;
			}
			else
			{

				SendUpdatePotionCnt( pPlayer, pItem );
				// 로그
				ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );
				ITEMDB_UpdateInvenItem( pPlayer, pItem );
			}
			return 1;
		}
	}
	return 0;
}


//
// Function Name: UseOtherPotion
// Date: 
// Description: 
// 
BOOL UseOtherPotion( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
#ifndef dDAUM_BILLING
	return 0;
#endif
	if( !pPlayer || !pItem )
		return 0;
	
	BYTE useOk = 0;
	
	switch( pItem->itemNum )
	{
	// 웨이트 드링크 ( 무게 20% 증가 )
	case dBILL_ITEM_NUM61:
	case dBILL_ITEM_NUM62:
	case dBILL_ITEM_NUM63:
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[206] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] = GetSecondTime( 1 );

		//DB
		UpdateBillItemInfo( pPlayer, "effect9", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL19 );
		SendWeightGageInfo( pPlayer );
		
		break;
	// HP 드링크 ( 1시간동안 HP 20% 증가 )
	case dBILL_ITEM_NUM64:
	case dBILL_ITEM_NUM65:
	case dBILL_ITEM_NUM66:
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] ||
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] ||
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[207] );
				return 0;
			}
			
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] = GetSecondTime( 1 );
			
			int addValue = (int)( GET_MAX_HP( pPlayer ) * 0.2 );
			
			GET_HPADD( pPlayer ) += addValue;
			
			pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT10] = addValue;
			//DB 
			UpdateBillItemInfo( pPlayer, "effect10", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] );

			SendPlayerMaxHPMP( pPlayer );
			
			useOk = 1;

			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL20 );
		}	
		break;
	// MP 드링크 ( 1시간동안 MP 20% 증가 )
	case dBILL_ITEM_NUM67:
	case dBILL_ITEM_NUM68:
	case dBILL_ITEM_NUM69:
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] ||
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] ||
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[207] );
				return 0;
			}
			
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] = GetSecondTime( 1 );
			
			int addValue = (int)( GET_MAX_MP( pPlayer ) * 0.2 );
			
			GET_MPADD( pPlayer ) += addValue;
			
			pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT11] = addValue;
			//DB
			UpdateBillItemInfo( pPlayer, "effect11", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] );
			
			SendPlayerMaxHPMP( pPlayer );
			
			useOk = 1;
			
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL21 );
			
		}
		break;

	// 기동성 물약 ( 이동속도 증가 )
	case dBILL_ITEM_NUM58:		// 1일
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[208] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] = g_CurrRealTime + GetSecondDay( 1 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect8", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] );
		useOk = 1;

		SendRideInfo( pPlayer );

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL18 );

		break;
	case dBILL_ITEM_NUM59:		// 7일 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[208] );
			return 0;
		}
		
		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] = g_CurrRealTime + GetSecondDay( 7 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect8", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] );

		useOk = 1;

		SendRideInfo( pPlayer );

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL18 );

		break;
	case dBILL_ITEM_NUM60:		// 1달
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[208] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] = g_CurrRealTime + GetSecondDay( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect8", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] );

		useOk = 1;

		SendRideInfo( pPlayer );

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL18 );
		
		break;

	// 그룹 HP 드링크 ( 1시간동안 HP 20 % 증가 )
	case dBILL_ITEM_NUM73:
	case dBILL_ITEM_NUM74:
	case dBILL_ITEM_NUM75:
		{
			if( !GET_PARTY_MASTER( pPlayer ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[209] );
				return 0;
			}

			if( GET_PARTY_MASTER( pPlayer ) != pPlayer )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[210] );
				return 0;
			}

			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[211] );
				return 0;
			}
		
			sPDESC_DATA pTarget = NULL;
			
			pTarget = GET_PARTY_MASTER( pPlayer );
			
			// 개인 , 군단원 HP 증가가 없을 경우만
			if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT10] && 
				!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
			{
				if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
				{
					pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
					GET_HPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13];	
					
					SendPlayerMaxHPMP( pTarget );

					SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL23 );
				}
			}
			
			pTarget = NULL;
			
			int i;
			
			for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
			{
				pTarget = GET_PARTY_MASTER( pPlayer )->ch2.party.member[i];
				
				if( pTarget )
				{
					if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT10] && 
						!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
					{
						if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
						{
							pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
							GET_HPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13];	
							
							SendPlayerMaxHPMP( pTarget );
							
							SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL23 );
						}
					}
				}
			}
			
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] = GetSecondTime( 1 );
			UpdateBillItemInfo( pPlayer, "effect13", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] );
			
			useOk = 1;
		}
		break;
		// 그룹 MP 드링크 ( 1시간동안 MP 20 % 증가 )
	case dBILL_ITEM_NUM76:
	case dBILL_ITEM_NUM77:
	case dBILL_ITEM_NUM78:
		{
			if( !GET_PARTY_MASTER( pPlayer ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[209] );
				return 0;
			}
			
			if( GET_PARTY_MASTER( pPlayer ) != pPlayer )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[210] );
				return 0;
			}
			
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[211] );
				return 0;
			}
			
			sPDESC_DATA pTarget = NULL;
			
			pTarget = GET_PARTY_MASTER( pPlayer );
			
			// 개인 , 군단원 HP 증가가 없을 경우만
			if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT11] && 
				!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
			{
				if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
				{
					pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
					GET_MPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14];	
					
					SendPlayerMaxHPMP( pTarget );
					
					SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL24 );
				}
			}
			
			pTarget = NULL;
			
			int i;
			
			for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
			{
				pTarget = GET_PARTY_MASTER( pPlayer )->ch2.party.member[i];
				
				if( pTarget )
				{
					if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT11] && 
						!pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
					{
						if( !pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
						{
							pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
							GET_MPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14];	
							
							SendPlayerMaxHPMP( pTarget );
							
							SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL24 );
						}
					}
				}
			}
			
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] = GetSecondTime( 1 );
			UpdateBillItemInfo( pPlayer, "effect14", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] );
			
			useOk = 1;
		}
		break;
	// 군단원 HP 드링크
	case dBILL_ITEM_NUM79:
	case dBILL_ITEM_NUM80:
	case dBILL_ITEM_NUM81:
		{
			if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[212] );
				return 0;
			}
			
			sPDESC_DATA d, next_d;
			sPDESC_DATA pTarget = NULL;

			LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );

			if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
				GET_STATE( d ) == dDESC_STATE_FIGHT ||
				GET_STATE( d ) == dDESC_STATE_LOADING )
			{
				if( GET_GUILD( d ) == GET_GUILD( pPlayer ) )
				{
					pTarget = d;
					
					if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT10] &&
						!pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
					{
						if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
						{
							pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT15] = (int)( GET_MAX_HP( pTarget ) * 0.2 );
							GET_HPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT15];
							pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT15] = GetSecondTime( 1 );
							
							SendPlayerMaxHPMP( pTarget );				

							// DB
							UpdateBillItemInfo( pTarget, "effect15", pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT15] );
							
							SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL25 );
						}
					}
				}
			}

			LIST_WHILEEND( gPLAYERS.list, d, next_d );

			useOk = 1;
		}
		break;
	// 군단원 MP 드링크
	case dBILL_ITEM_NUM82:
	case dBILL_ITEM_NUM83:
	case dBILL_ITEM_NUM84:
		{
			if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[212] );
				return 0;
			}
			
			sPDESC_DATA d, next_d;
			sPDESC_DATA pTarget = NULL;
			
			LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
			
			if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
				GET_STATE( d ) == dDESC_STATE_FIGHT ||
				GET_STATE( d ) == dDESC_STATE_LOADING )
			{
				if( GET_GUILD( d ) == GET_GUILD( pPlayer ) )
				{
					pTarget = d;
					
					if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT11] &&
						!pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
					{
						if( !pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
						{
							pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT16] = (int)( GET_MAX_MP( pTarget ) * 0.2 );
							GET_MPADD( pTarget ) += (WORD)pTarget->ch2.billingAddValue[dBILL_ITEM_EFFECT16];
							pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT16] += GetSecondTime( 1 );
							
							SendPlayerMaxHPMP( pTarget );				

							// DB
							UpdateBillItemInfo( pTarget, "effect16", pTarget->ch2.billingFlag[dBILL_ITEM_EFFECT16] );
							
							SendPlayerEffect( pTarget, PLAYER_EFFECT_BILL26 );
						}
					}
				}
			}
			
			LIST_WHILEEND( gPLAYERS.list, d, next_d );
			
			useOk = 1;
		}
		break;

		// 부활의 두루마리
	case dBILL_ITEM_NUM22:
	case dBILL_ITEM_NUM23:
	case dBILL_ITEM_NUM24:
		{	
			if( GET_DEAD_STATE( pPlayer ) == dDEAD_STATE_NONE )
				return 0;
			
			// 부활
#ifdef dUSE_DEAD_GHOST
			pPlayer->ch2.isDeadGhost = 0;
#endif

			GET_HP( pPlayer ) = (int)( GET_MAX_HP( pPlayer ) * 0.3 );
			GET_MP( pPlayer ) = (int)( GET_MAX_MP( pPlayer ) * 0.3 );

			SendPlayerHP( pPlayer );
			SendPlayerMP( pPlayer );
			
			GET_DEAD_STATE( pPlayer ) = dDEAD_STATE_NONE;
			
			pPlayer->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
			pPlayer->ch2.gotoPosX = GET_POSX( pPlayer );
			pPlayer->ch2.gotoPosY = GET_POSY( pPlayer );
			
			PlayerFromMap( pPlayer );
			SendMapLoading( pPlayer );
			

			useOk = 1;
			
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL6 );
		}
		
		break;
		// 단원 소환
	case dBILL_ITEM_NUM92:
	case dBILL_ITEM_NUM93:
	case dBILL_ITEM_NUM94:
		{
			if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[212] );
				return 0;
			}

			if( pPlayer->ch2.guildSummonTime + GetSecondMin( 10 ) > g_CurrRealTime )
			{
				int minVal = (int)( ( pPlayer->ch2.guildSummonTime + GetSecondMin( 10 ) - g_CurrRealTime ) / 60 );

				SendSystemMsg( pPlayer, g_LANG_STR[213], minVal );
				return 0;
			}

			sPDESC_DATA d, next_d;

			int cnt = 0;

			LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
			
			if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
				GET_STATE( d ) == dDESC_STATE_FIGHT )
			{
				if( d != pPlayer && GET_GUILD( d ) == GET_GUILD( pPlayer ) )
				{
					d->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
					d->ch2.gotoPosX = GET_POSX( pPlayer );
					d->ch2.gotoPosY = GET_POSY( pPlayer );
					d->ch2.gotoPosMapKind = dMAP_KIND_FIELD;

					PlayerFromMap( d );
					SendMapLoading( d );
					cnt++;
					
					if( cnt >= 3 )
						break;
				}
			}

			LIST_WHILEEND( gPLAYERS.list, d, next_d );

			useOk = 1;

			pPlayer->ch2.guildSummonTime = g_CurrRealTime;
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL29 );
		}
		break;
		// 단원 필승의 외침
		// 단원 축복의 외침 
		// 단원 기원의 외침
	case dBILL_ITEM_NUM95:
	case dBILL_ITEM_NUM96:
	case dBILL_ITEM_NUM97:
	case dBILL_ITEM_NUM98:
	case dBILL_ITEM_NUM99:
	case dBILL_ITEM_NUM100:
	case dBILL_ITEM_NUM101:
	case dBILL_ITEM_NUM102:
	case dBILL_ITEM_NUM103:

		{
			if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[212] );
				return 0;
			}
			sPDESC_DATA d, next_d;
			
			LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
			
			if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
				GET_STATE( d ) == dDESC_STATE_FIGHT )
			{
				if( GET_GUILD( d ) == GET_GUILD( pPlayer ) )
				{
					switch( pItem->itemNum )
					{
					case dBILL_ITEM_NUM95:
					case dBILL_ITEM_NUM96:
					case dBILL_ITEM_NUM97:
						SendPlayerEffect( d, PLAYER_EFFECT_BILL30 );
						useOk = 1;
						break;
					case dBILL_ITEM_NUM98:
					case dBILL_ITEM_NUM99:
					case dBILL_ITEM_NUM100:
						SendPlayerEffect( d, PLAYER_EFFECT_BILL31 );
						useOk = 1;
						break;
					case dBILL_ITEM_NUM101:
					case dBILL_ITEM_NUM102:
					case dBILL_ITEM_NUM103:
						SendPlayerEffect( d, PLAYER_EFFECT_BILL32 );
						useOk = 1;
						break;
					}
				}
			}
			
			LIST_WHILEEND( gPLAYERS.list, d, next_d );
		}
		break;
		// 단원 저주의 외침 
		// 단원 도발의 외침
	case dBILL_ITEM_NUM104:
	case dBILL_ITEM_NUM105:
	case dBILL_ITEM_NUM106:
	case dBILL_ITEM_NUM107:
	case dBILL_ITEM_NUM108:
	case dBILL_ITEM_NUM109:
		{
			if( !IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[212] );
				return 0;
			}
			sPDESC_DATA d, next_d;
			
			LIST_WHILE( gPLAYERS.list, d, next_d, next, WMgr );
			
			if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
				GET_STATE( d ) == dDESC_STATE_FIGHT )
			{
				if( IS_VALID_GUILD( GET_GUILD( d ) ) && GET_GUILD( d ) != GET_GUILD( pPlayer ) )
				{
					if( WAR_FindData( GET_GUILD( pPlayer ), GET_GUILD( d ) ) )
					{
						switch( pItem->itemNum )
						{
							// 단원 저주의 외침 
						case dBILL_ITEM_NUM104:
						case dBILL_ITEM_NUM105:
						case dBILL_ITEM_NUM106:
							SendPlayerEffect( d, PLAYER_EFFECT_BILL33 );
							useOk = 1;
							break;
							// 단원 도발의 외침
						case dBILL_ITEM_NUM107:
						case dBILL_ITEM_NUM108:
						case dBILL_ITEM_NUM109:
							SendPlayerEffect( d, PLAYER_EFFECT_BILL34 );
							useOk = 1;
							break;
						}
					}
				}
			}
			
			LIST_WHILEEND( gPLAYERS.list, d, next_d );	
			
			if( !useOk )
				SendSystemMsg( pPlayer, g_LANG_STR[214] );
		}
		break;

	case dBILL_ITEM_NUM110:
	case dBILL_ITEM_NUM111:
	case dBILL_ITEM_NUM112:
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] )
			{
				SendSystemMsg( pPlayer, g_LANG_STR[215] );
				return 0;
			}

			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] = GetSecondTime( 1 );
			UpdateBillItemInfo( pPlayer, "effect17", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] );
			
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL35 );
			
			useOk = 1;
		}
		break;
		// 하트 이모티콘

	case dBILL_ITEM_NUM113:
	case dBILL_ITEM_NUM114:
	case dBILL_ITEM_NUM115:
		{
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL36 );

			useOk = 1;
		}
		break;

		// 기쁨 이모티콘
	case dBILL_ITEM_NUM116:
	case dBILL_ITEM_NUM117:
	case dBILL_ITEM_NUM118:
		{
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL37 );

			useOk = 1;
		}
		break;
		// 슬픔 이모티콘
	case dBILL_ITEM_NUM119:
	case dBILL_ITEM_NUM120:
	case dBILL_ITEM_NUM121:
		{
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL38 );

			useOk = 1;
		}
		break;
		// 분노 이모티콘
	case dBILL_ITEM_NUM122:
	case dBILL_ITEM_NUM123:
	case dBILL_ITEM_NUM124:
		{
			SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL39 );

			useOk = 1;
		}
		break;

	}

	if( useOk )
	{
		if( pItem->exVal[0] > 1 )
		{
			pItem->exVal[0] = MIN( pItem->exVal[0] - 1, 0 );
			SendUpdatePotionCnt( pPlayer, pItem );
			// 로그
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );
			ITEMLOG_BillItemLog( pPlayer, pItem->itemNum, BILLITEM_LOG_ACT_START );

			return 1;
		}
		else
		{
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );
			ITEMLOG_BillItemLog( pPlayer, pItem->itemNum, BILLITEM_LOG_ACT_START );
		
			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
			// 로그
			return 1;
		}
	}
	
	return 0;
}

//
// Function Name: CheckBillItemTime
// Date: 
// Description: 
// 
void CheckBillItemTime( sPDESC_DATA pPlayer )
{
	if( pPlayer->ch2.checkBillItemTime + 10 <= g_CurrRealTime )
		pPlayer->ch2.checkBillItemTime = g_CurrRealTime;
	else
		return;
	
	// 웜쓰 오브 아라임 경험치 1.3배
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] )
		{
			UpdateBillItemInfo( pPlayer, "effect1", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM1, BILLITEM_LOG_ACT_END );
		}
	}

	// 1.5배 
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] - 10, 0 );

		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			UpdateBillItemInfo( pPlayer, "effect2", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM4, BILLITEM_LOG_ACT_END );
		}
	}

	// 아이템 오브 아라임 , 사냥시 아이템 드랍 1.3배
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] )
		{
			UpdateBillItemInfo( pPlayer, "effect3", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM16, BILLITEM_LOG_ACT_END );
		}
	}

	// 1.5 배 
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			UpdateBillItemInfo( pPlayer, "effect4", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM19, BILLITEM_LOG_ACT_END );
		}
	}

	// 프리미엄존( 5 )
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] &&
		!pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{				
			UpdateBillItemInfo( pPlayer, "effect5", 0 );

			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM28, BILLITEM_LOG_ACT_END );
			
			if( GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE1 ||
				GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE2 ||
				GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE3 )
			{
				GET_GOTO_MAPKIND( pPlayer ) = dMAP_KIND_FIELD;
				
				GET_GOTO_MAPNUM( pPlayer ) = 20;
				GET_GOTO_POSX( pPlayer ) = 100;
				GET_GOTO_POSY( pPlayer ) = 60;
				
				PlayerFromMap( pPlayer );
				SendMapLoading( pPlayer );
			}
		}
	}

	// 프리미엄 존 플러스 ( 6 )
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			UpdateBillItemInfo( pPlayer, "effect6", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM31, BILLITEM_LOG_ACT_END );
	
			if( GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE1 ||
				GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE2 ||
				GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE3 )
			{
				GET_GOTO_MAPKIND( pPlayer ) = dMAP_KIND_FIELD;
				
				GET_GOTO_MAPNUM( pPlayer ) = 20;
				GET_GOTO_POSX( pPlayer ) = 100;
				GET_GOTO_POSY( pPlayer ) = 60;
				
				PlayerFromMap( pPlayer );
				SendMapLoading( pPlayer );
			}
		}
	}

	// 신의 축복 ( 인첸트 카드 드랍확률 증가 )
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] )
		{
			UpdateBillItemInfo( pPlayer, "effect7", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM37, BILLITEM_LOG_ACT_END );
		
		}
	}

	// 기동성의 물약
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] )
		{
			UpdateBillItemInfo( pPlayer, "effect8", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM58, BILLITEM_LOG_ACT_END );

			SendRideInfo( pPlayer );
		}
	}

	// 웨이트 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] - 10, 0 );

		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] )
		{
			UpdateBillItemInfo( pPlayer, "effect9", 0 );
			SendWeightGageInfo( pPlayer );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM61, BILLITEM_LOG_ACT_END );
		}
	}

	// HP 드링크 ( HP 20% 증가 1시간 )
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT10] )
		{				
			if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT10] )
			{
				GET_HPADD( pPlayer ) -= (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT10];
				pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT10] = 0;

				if( GET_HP( pPlayer ) > ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
				{
					GET_HP( pPlayer ) = ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
				}
				SendPlayerHP( pPlayer );
				SendPlayerMaxHPMP( pPlayer );
			}
			UpdateBillItemInfo( pPlayer, "effect10", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM64, BILLITEM_LOG_ACT_END );
			
		}
	}

	// MP 드링크 ( MP 20% 증가 1시간 )
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT11] )
		{
			if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT11] )
			{
				GET_MPADD( pPlayer ) -= (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT11];
				pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT11] = 0;

				if( GET_MP( pPlayer ) > ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) ) )
				{
					GET_MP( pPlayer ) = ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
				}
				SendPlayerMP( pPlayer );				
				SendPlayerMaxHPMP( pPlayer );
			}
			UpdateBillItemInfo( pPlayer, "effect11", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM67, BILLITEM_LOG_ACT_END );
			
		}
	}

	// 그룹 웜쓰 오브 아라임
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] )
		{
			UpdateBillItemInfo( pPlayer, "effect12", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM70, BILLITEM_LOG_ACT_END );
		}
	}
	
	// 그룹 HP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT13] )
		{		

			if( GET_PARTY_MASTER( pPlayer ) == pPlayer )
			{
				if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
				{
					GET_HPADD( pPlayer ) -= (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13];
					pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = 0;
					
					if( GET_HP( pPlayer ) > ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
					{
						GET_HP( pPlayer ) = ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
					}
					SendPlayerHP( pPlayer );					
					SendPlayerMaxHPMP( pPlayer );
				}
				
				for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
				{
					if( pPlayer->ch2.party.member[i] )
					{
						if( pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT13] )
						{
							GET_HPADD( pPlayer->ch2.party.member[i] ) -= (WORD)pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT13];
							pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT13] = 0;

							if( GET_HP( pPlayer->ch2.party.member[i] ) > ( GET_MAX_HP( pPlayer->ch2.party.member[i] ) + GET_HPADD( pPlayer->ch2.party.member[i] ) ) )
							{
								GET_HP( pPlayer->ch2.party.member[i] ) = ( GET_MAX_HP( pPlayer->ch2.party.member[i] ) + GET_HPADD( pPlayer->ch2.party.member[i] ) );
							}
							SendPlayerHP( pPlayer->ch2.party.member[i] );							
							SendPlayerMaxHPMP( pPlayer->ch2.party.member[i] );
						}						
					}
				}
			}
			UpdateBillItemInfo( pPlayer, "effect13", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM73, BILLITEM_LOG_ACT_END );

		}
	}

	// 그룹 MP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT14] )
		{		
			if( GET_PARTY_MASTER( pPlayer ) == pPlayer )
			{
				if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
				{
					GET_MPADD( pPlayer ) -= (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14];
					pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = 0;
					
					if( GET_MP( pPlayer ) > ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) ) )
					{
						GET_MP( pPlayer ) = ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
					}
					SendPlayerMP( pPlayer );					
					SendPlayerMaxHPMP( pPlayer );
				}
				
				for( int i = 0; i < dMAX_PARTY_MEMBER; i++ )
				{
					if( pPlayer->ch2.party.member[i] )
					{
						if( pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT14] )
						{
							GET_MPADD( pPlayer->ch2.party.member[i] ) -= (WORD)pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT14];
							pPlayer->ch2.party.member[i]->ch2.billingAddValue[dBILL_ITEM_EFFECT14] = 0;
							
							if( GET_MP( pPlayer->ch2.party.member[i] ) > ( GET_MAX_MP( pPlayer->ch2.party.member[i] ) + GET_MPADD( pPlayer->ch2.party.member[i] ) ) )
							{
								GET_MP( pPlayer->ch2.party.member[i] ) = ( GET_MAX_MP( pPlayer->ch2.party.member[i] ) + GET_MPADD( pPlayer->ch2.party.member[i] ) );
							}
							SendPlayerMP( pPlayer->ch2.party.member[i] );
							SendPlayerMaxHPMP( pPlayer->ch2.party.member[i] );
						}		
					}
				}
			}			
			UpdateBillItemInfo( pPlayer, "effect14", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM76, BILLITEM_LOG_ACT_END );
		}
	}

	// 군단원 HP 드링크 
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT15] )
		{			
			if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT15] )
			{
				GET_HPADD( pPlayer ) -= (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT15];
				pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT15] = 0;
				
				if( GET_HP( pPlayer ) > ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
				{
					GET_HP( pPlayer ) = ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );
				}
				SendPlayerHP( pPlayer );
				SendPlayerMaxHPMP( pPlayer );
			}
			
			UpdateBillItemInfo( pPlayer, "effect15", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM79, BILLITEM_LOG_ACT_END );
		}
	}

	// 군단원 MP 드링크
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] - 10, 0 );
		
		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT16] )
		{
			if( pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT16] )
			{
				GET_MPADD( pPlayer ) -= (WORD)pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT16];
				pPlayer->ch2.billingAddValue[dBILL_ITEM_EFFECT16] = 0;

				if( GET_MP( pPlayer ) > ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) ) )
				{
					GET_MP( pPlayer ) = ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
				}				
				SendPlayerMP( pPlayer );
				SendPlayerMaxHPMP( pPlayer );
			}
			
			UpdateBillItemInfo( pPlayer, "effect16", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM82, BILLITEM_LOG_ACT_END );
		}
	}

	// 마이너스 ( 성향치 2 배 )
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] )
	{
		// 월정액 체크
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] > dBILL_BEGIN_TIMESTAMP )
		{
			if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] <= g_CurrRealTime )
				pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] = 0;
		}
		else
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] = MIN( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] - 10, 0 );

		if( !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT17] )
		{
			UpdateBillItemInfo( pPlayer, "effect17", 0 );
			ITEMLOG_BillItemLog( pPlayer, dBILL_ITEM_NUM110, BILLITEM_LOG_ACT_END );
		}
	}

}

//
// Function Name: UseOtherOneTimeItem
// Date: 
// Description: 
// 
BOOL UseOtherOneTimeItem( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pPlayer || !pItem )
		return 0;
	
	BYTE useOk = 0;
	
	switch( pItem->itemNum )
	{
	// 웜쓰 오브 아라임

	case dBILL_ITEM_NUM1:			// 웜쓰 오브 아라임 (10시간) 경험치 1.3배 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] = GetSecondTime( 10 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect1", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL1 );

		break;
	case dBILL_ITEM_NUM2:			// 웜쓰 오브 아라임 (20시간) 경험치 1.3배 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		
		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] = GetSecondTime( 20 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect1", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL1 );

		break;
	case dBILL_ITEM_NUM3:			// 웜쓰 오브 아라임 (30시간) 경험치 1.3배 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] = GetSecondTime( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect1", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL1 );

		break;

	case dBILL_ITEM_NUM85:			// 웜쓰 오브 아라임 (한달) 경험치 1.3배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] = g_CurrRealTime + GetSecondDay( 30 );

		//DB
		UpdateBillItemInfo( pPlayer, "effect1", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL1 );

		break;

	case dBILL_ITEM_NUM4:			// 웜쓰 오브 아라임 (10시간) 경험치 1.5배 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		
		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] = GetSecondTime( 10 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect2", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL27 );

		break;
	case dBILL_ITEM_NUM5:			// 웜쓰 오브 아라임 (20시간) 경험치 1.5배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}
	
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] = GetSecondTime( 20 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect2", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL27 );

		break;
	case dBILL_ITEM_NUM6:			// 웜쓰 오브 아라임 (30시간) 경험치 1.5배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] = GetSecondTime( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect2", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL27 );

		break;

	case dBILL_ITEM_NUM86:		// 웜쓰 오브 아라임 (한달) 경험치 1.5배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT1] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[216] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] = g_CurrRealTime + GetSecondDay( 30 );

		//DB
		UpdateBillItemInfo( pPlayer, "effect2", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT2] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL27 );

		break;

	case dBILL_ITEM_NUM16:		// 아이템 오브 아라임 ( 10시간 ) 사냥시 아이템 드랍 1.3배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}	

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] = GetSecondTime( 10 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect3", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL5 );
		break;

	// 아이템 오브 아라임 

	case dBILL_ITEM_NUM17:		// 아이템 오브 아라임 ( 20시간 ) 사냥시 아이템 드랍 1.3배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] = GetSecondTime( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect3", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL5 );

		break;
	case dBILL_ITEM_NUM18:		// 아이템 오브 아라임 ( 30시간 ) 사냥시 아이템 드랍 1.3배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}


		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] = GetSecondTime( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect3", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL5 );

		break;

	case dBILL_ITEM_NUM87:		// 아이템 오브 아라임 (한달) 사냥시 아이템 드랍 1.3배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}


		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] = g_CurrRealTime + GetSecondDay( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect3", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL5 );

		break;

	case dBILL_ITEM_NUM19:		// 아이템 오브 아라임 ( 10시간 ) 사냥시 아이템 드랍 1.5배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] = GetSecondTime( 10 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect4", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL28 );

		break;
	case dBILL_ITEM_NUM20:		// 아이템 오브 아라임 ( 20시간 ) 사냥시 아이템 드랍 1.5배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] = GetSecondTime( 20 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect4", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL28 );

		break;
	case dBILL_ITEM_NUM21:		// 아이템 오브 아라임 ( 30시간 ) 사냥시 아이템 드랍 1.5배
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] = GetSecondTime( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect4", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL28 );

		break;

	case dBILL_ITEM_NUM88:		// 아이템 오브 아라임 (한달) 1.5배
		
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT3] ||
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[218] );
			return 0;
		}
		
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}
		
		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] = g_CurrRealTime + GetSecondDay( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect4", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT4] );
		useOk = 1;
		
		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL28 );	
		break;

		
		// 신의 축복 인첸트카드 드랍 확률 증가

	case dBILL_ITEM_NUM37:			// 10시간 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[219] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}


		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] = GetSecondTime( 10 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect7", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL11 );

		break;
	case dBILL_ITEM_NUM38:			// 20시간 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[219] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] = GetSecondTime( 20 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect7", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL11 );

		break;
	case dBILL_ITEM_NUM39:			// 30시간
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[219] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}


		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] = GetSecondTime( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect7", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL11 );

		break;

	case dBILL_ITEM_NUM91:		// 신의축복 한달
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[219] );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[217] );
			return 0;
		}


		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] = g_CurrRealTime + GetSecondDay( 30 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect7", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT7] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL11 );
		break;

	// 그룹 웜쓰 오브 아라임
	case dBILL_ITEM_NUM70:
	case dBILL_ITEM_NUM71:
	case dBILL_ITEM_NUM72:
		if( !GET_PARTY_MASTER( pPlayer ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[209] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] = GetSecondTime( 1 );
		//DB
		UpdateBillItemInfo( pPlayer, "effect12", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT12] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL22 );

		break;
	// 프리미엄 존
	case dBILL_ITEM_NUM28:		// 10시간 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone plus item is expired." );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
		//	pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] += GetSecondTime( 10 ); 
			SendSystemMsg( pPlayer, g_LANG_STR[220] );
			return 0;
		}
		else
		{
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] = GetSecondTime( 10 );
		}
		//DB
		UpdateBillItemInfo( pPlayer, "effect5", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL8 );

		break;
	case dBILL_ITEM_NUM29:		// 20시간 

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone plus item is expired." );
			return 0;
		}
		
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
		//	pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] += GetSecondTime( 24 ); 
			SendSystemMsg( pPlayer, g_LANG_STR[220] );
			return 0;
		}
		else
		{
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] = GetSecondTime( 20 );
		}
		//DB
		UpdateBillItemInfo( pPlayer, "effect5", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL8 );

		break;
	case dBILL_ITEM_NUM30:		// 30시간

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone plus item is expired." );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
		//	pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] += GetSecondTime( 30 ); 
			SendSystemMsg( pPlayer, g_LANG_STR[220] );
			return 0;
		}
		else
		{
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] = GetSecondTime( 30 );
		}
		//DB
		UpdateBillItemInfo( pPlayer, "effect5", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL8 );

		break;

	case dBILL_ITEM_NUM89:		// 프리미엄 존 ( 한달 )

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone plus item is expired." );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[220] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] = g_CurrRealTime + GetSecondDay( 30 );
		
		//DB
		UpdateBillItemInfo( pPlayer, "effect5", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL8 );
		break;

	// 프리미엄 존 플러스
	case dBILL_ITEM_NUM31:		// 10시간 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone item is expired." );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[221] );
			return 0;
		}
		else
		{
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = GetSecondTime( 10 );			
		}
		//DB
		UpdateBillItemInfo( pPlayer, "effect6", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL9 );

		break;
	case dBILL_ITEM_NUM32:		// 20시간 
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone item is expired." );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[221] );
			return 0;
		}
		else
		{
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = GetSecondTime( 20 );
		}
		//DB
		UpdateBillItemInfo( pPlayer, "effect6", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL9 );

		break;
	case dBILL_ITEM_NUM33:		// 30시간
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone item is expired." );
			return 0;
		}

		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[221] );
			return 0;
		}
		else
		{
			pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = GetSecondTime( 30 );
		}
		//DB
		UpdateBillItemInfo( pPlayer, "effect6", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL9 );

		break;

	case dBILL_ITEM_NUM90:		// 프리미엄 존 플러스 (한달)
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] )
		{
			SendSystemMsg( pPlayer, "You cannot use this entrance item until your previous premium zone item is expired." );
			return 0;
		}
		if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[221] );
			return 0;
		}

		pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] = g_CurrRealTime + GetSecondDay( 30 );
		
		//DB
		UpdateBillItemInfo( pPlayer, "effect6", pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] );
		useOk = 1;

		SendPlayerEffect( pPlayer, PLAYER_EFFECT_BILL9 );
		break;

	}
	
	if( useOk )
	{
		if( pItem->exVal[0] > 1 )
		{
			pItem->exVal[0] = MIN( pItem->exVal[0] - 1, 0 );
			SendUpdatePotionCnt( pPlayer, pItem );
			// 로그
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );
			ITEMLOG_BillItemLog( pPlayer, pItem->itemNum, BILLITEM_LOG_ACT_START );
			return 1;
		}
		else
		{
			ITEMLOG_ItemLog( pPlayer, NULL, pItem, ITEMLOG_ACT_BILLITEM_OUT );
			ITEMLOG_BillItemLog( pPlayer, pItem->itemNum, BILLITEM_LOG_ACT_START );
			ItemFromInventory( pPlayer, pItem );
			INSERT_ITEM_TO_MEMORY( pItem );
			// 로그
			
			return 1;
		}
	}
	
	return 0;
}


/*


#ifdef _BillItemCaseV1
	case dBILL_ITEM_NUM155: //가이아의 축복
		{
			if( CountEmptyInven( pPlayer ) >= 5 )
			{
				sPITEM_DATA		pArmor = NULL;
				sPITEM_DATA		pMoney = NULL;

				sCHECK_INVEN	checkInven;

				//11996 11997 13560*3
				pArmor = CreateItem( 10508, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 10697, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 10886, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 11075, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}
				pArmor = CreateItem( 11726, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}
				useOk = 1;
			}
			else
			{
				SendSystemMsgNum( pPlayer, 11, 0, "a" );
				//SendSystemMsg( pPlayer, "인벤토리 공간이 없어 사용 할 수 없습니다." );
			}
		}
		break;
#else
	case dBILL_ITEM_NUM155:
		{
			if( CountEmptyInven( pPlayer ) >= 9 )
			{
				sPITEM_DATA		pArmor = NULL;
				sPITEM_DATA		pMoney = NULL;

				sCHECK_INVEN	checkInven;

				//11996 11997 13560*3
				pArmor = CreateItem( 12152, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 12341, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 12530, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 12719, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 13496, "", "", 0 );
				if( pArmor )
				{

					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 10416, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 10424, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pArmor = CreateItem( 10427, "", "", 0 );
				if( pArmor )
				{
					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}

				pMoney = CreateMoney( 10000000 );

				if( pMoney )
				{
					GET_MONEY( pPlayer ) += GET_MONEY_AMOUNT( pMoney );
					UpdateMoney( pPlayer );
					DATASERV_SendUpdateMoney( pPlayer );

					SendSystemMsgNum( pPlayer, 9, 1, "%d", GET_MONEY_AMOUNT( pMoney ) );

					INSERT_ITEM_TO_MEMORY( pMoney );
				}

				pArmor = CreateItem( 11996, "", "", 0 );
				if( pArmor )
				{

					CheckEmptyInven( pPlayer, &checkInven );
					ItemToInventory( pPlayer, pArmor, &checkInven );
					ITEMLOG_ItemLog( pPlayer, NULL, pArmor, ITEMLOG_ACT_EVENTINPUT );
				}
				useOk = 1;
			}
			else
			{
				SendSystemMsgNum( pPlayer, 11, 0, "a" );
				//SendSystemMsg( pPlayer, "인벤토리 공간이 없어 사용 할 수 없습니다." );
			}
		}
		break;
#endif


*/

















void SendItemTimer( sPDESC_DATA pPlayer, int itemNum, int timer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SHOW_ITEMTIMER, g_nPos );
	PutInteger( g_Packet, itemNum, g_nPos );
	PutInteger( g_Packet, timer, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

void ItemToSmith( sPDESC_DATA pPlayer, sPITEM_DATA pItem, int PlayerMoney )
{
//	INSERT_TO_LIST( pPlayer->smith, pItem, i_prev, i_next );	
	if( !pItem && !pPlayer ) 
	{
		return;
	}

	int  priceEnchent;
	
	if( pItem->itemNum == 12097 ||  pItem->itemNum == 12098 )
	{
		priceEnchent = 0;
		if( !smithCheckCard )
		{
			pPlayer->smithCnt++;
			smithCheckCard = TRUE;
		}
	}
	else
	{
		priceEnchent = CheckEnchentItemtoSmith( pItem );
		if( !smithCheckWp )
		{
			pPlayer->smithCnt++;
			smithCheckWp = TRUE;
		}
	}
	
	 if( PlayerMoney < priceEnchent )
	 {
	        SendSystemMsg( pPlayer, g_LANG_STR[235] );
	        return;
	 } 

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SMITH_ITEMADDED, g_nPos );
	
	PutInteger( g_Packet, pItem->idxNum, g_nPos );
	PutInteger( g_Packet, pItem->itemNum, g_nPos );
	PutInteger( g_Packet, priceEnchent, g_nPos );
	
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );
	
}

void ItemFromSmith( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pItem && !pPlayer )
	{
		return;
	}
	
	if( pItem->itemNum == 12097 ||  pItem->itemNum == 12098 )
	{
		if( smithCheckCard )
		{
			pPlayer->smithCnt--;
			smithCheckCard = FALSE;
		}
	}
	else
	{
		if( smithCheckWp )
		{
			pPlayer->smithCnt--;
			smithCheckWp = FALSE;
		}
	}
}

int CheckEnchentItemtoSmith( sPITEM_DATA pItem )
{
	if( !pItem )
		return 0;
	
	int ItemInfor = g_ItemInfo[pItem->itemNum]->isUniq;
	if( IS_ARMOR(  GET_ITEM_TYPE( pItem ) ) )
	{
		ItemInfor = ItemInfor + 2;
	}
	else
	{
		ItemInfor = ItemInfor;
	}
	
	switch( ItemInfor )
	{
	case 5:
		if( pItem->itemNum == 12098 )
		{
			return 300000;
		}
		return 400000;
		break;
	case 6:
		if( pItem->itemNum == 12098 )
		{
			return 600000;
		}
		return 500000;
		break;
	case 7:
		if( pItem->itemNum == 12098 )
		{
			return 1200000;
		}
		return 600000;
		break;
	case 8:
		return 800000;
		break;
	case 9:
		return 1000000;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

void ClearSmith( sPDESC_DATA pPlayer )
{
	pPlayer->smithCnt = 0;
	smithCheckWp = FALSE;
	smithCheckCard = FALSE;
	
/*	sPITEM_DATA item, next_item;
	
	LIST_WHILE( pPlayer->smith, item, next_item, next, WMgr );
	REMOVE_FROM_LIST( pPlayer->smith, item, prev, next );
	INSERT_TO_LIST( pPlayer->smith, item, prev, next );
	LIST_WHILEEND( pPlayer->smith, item, next_item );
*/	
/*	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SMITH_QUIT, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );*/
}


//


int GetItemStackLimit(sPITEM_DATA item)
{
    return item ? LaqiaStackLimit(GET_ITEM_TYPE(item),item->itemNum) : 1;
}
int GetItemStackCount(sPITEM_DATA item)
{
    return item ? LaqiaStackCount(GET_ITEM_TYPE(item),item->itemNum,item->exVal[0]) : 0;
}
void ConsumeOneEnchantCard(sPDESC_DATA player, sPITEM_DATA card)
{
    if (!player || !card) return;
#ifdef dUSE_ITEMLOG
    ITEMLOG_ItemLog(player,NULL,card,ITEMLOG_ACT_INCHENT_REMOVE);
#endif
    if (GetItemStackCount(card)>1) {
        --card->exVal[0];
        SendUpdatePotionCnt(player,card);
    } else {
        ItemFromInventory(player,card);
        INSERT_ITEM_TO_MEMORY(card);
    }
}
