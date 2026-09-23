/**
 *	$Workfile: player.cpp $
 *
 *	Copyright (c) 2003 팀메이, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 60 $
 *	$Date: 04-12-31 12:08p $
 *     
 *	@Build Log :
 *	$Log: /GameServer/player.cpp $
 * 
 * 60    04-12-31 12:08p Sk8snow
 * 
 * 59    04-12-22 5:18p Sk8snow
 * 
 * 58    04-12-15 10:40a Sk8snow
 * 
 * 57    04-11-18 1:40a Sk8snow
 * 
 * 56    04-10-19 11:14p Sk8snow
 * 
 * 55    04-09-20 10:53p Sk8snow
 * 
 * 54    04-09-11 11:20p Sk8snow
 * 
 * 53    04-09-08 3:54p Sk8snow
 * 
 * 52    04-08-04 9:27a Sk8snow
 * 
 * 51    04-07-13 7:26a Sk8snow
 * 
 * 50    04-05-29 10:12p Sk8snow
 * 
 * 49    04-05-18 1:17a Sk8snow
 * 
 * 48    04-05-14 10:38p Sk8snow
 * 
 * 47    04-05-11 1:45a Sk8snow
 * 
 * 46    04-04-30 9:46a Sk8snow
 * 
 * 45    04-04-07 10:17p Sk8snow
 * 
 * 44    04-04-03 4:03p Sk8snow
 * 
 * 43    04-04-01 5:33p Sk8snow
 * 
 * 41    04-03-31 8:22a Sk8snow
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
#include "party.h"

#include "skill.h"
#include "mercenary.h"
#include "quest.h"

#include "house.h"

#include "itemdb.h"
#include "questdb.h"
#include "guild.h"

#include "effect.h"
#include "friends.h"

#include "pet.h"
#include "shop.h"

#include "war.h"

#include "billing.h"

int g_PLAYER_ABL_TABLE[dMAX_PLAYER_ABL_TABLE][dMAX_PLAYER_ABL_CLASS];
sSTATE_MPHP_TABLE g_STATE_MPHP_TABLE[2];
BYTE g_BASEHIT_TABLE[6][6];

void SendNpcInfo( sPDESC_DATA pPlayer, int mapNum );

// 직업 이름 테이블
const char g_CLASS_NAME[dMAX_CLASS][64] =
{
	"inoccupation",
	"Warrior",
	"Banditz",
	"Shadow Knights",
	"Paladin",
	"Assassin",
	"Archer",
	"Porter",
	"Ranger",
	"Blade Mage",
	"Wanderer",
	"The Wizard",
	"Shaman",
	"Monk",
	"Hawk",
	"Druid",
	"Cleric"
};



BYTE g_EXP_PROTECT[dMAX_EXP_PROTECT];

//
// Function Name: LoadExpProtectTable
// Date: 
// Description: 
//
BOOL LoadExpProtectTable()
{
	for( int i = 0; i < dMAX_EXP_PROTECT; i++ )
		g_EXP_PROTECT[i] = 0;

	FILE *fp = fopen( dEXP_PROTECT_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadExpProtectTable : Load file failed\r\n" );
		return 0;
	}

	int nArray = 1;
	char readBuf[128];

	while( !fp )
	{
		if( nArray >= dMAX_EXP_PROTECT )
			break;

		fgets( readBuf, 128, fp );

		g_EXP_PROTECT[nArray] = atoi( readBuf );

		nArray++;
	}

	return 1;
}

//
// Function Name: SendPlayerClass
// Date: 
// Description: 
//
void SendPlayerClass( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_CLASS, g_nPos );
	PutByte( g_Packet, GET_CLASS( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: dummyMagicIndex
// Date: 
// Description: 
//
#ifdef _DUMMY_MODE_

WORD dummyMagicIndex[18] =
{
	10103,		//	0
	10104,
	10105,
	10106,
	10107,
	10109,
	10110,
	10111,
	10112,
	10113,		// 10
	10114,
	10115,
	10116,
	10117,
	10118,
	10119,
	10120		// 17
};

//
// Function Name: DummyMove
// Date: 
// Description: 
//
void DummyMove( sPDESC_DATA pPlayer )
{
	if( number( 1, 5 ) == 5 )
	{
		if( pPlayer->w_next )
		{
			WORD magicNum = dummyMagicIndex[number( 0, 17 )];

			g_nPos = 2;
			
			PutWord( g_Packet, dPACKET_BEGIN_CASTMAGIC2, g_nPos );
			PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
			PutWord( g_Packet, magicNum, g_nPos );
			PutInteger( g_Packet, GET_IDX( pPlayer->w_next ), g_nPos );
			PutByte( g_Packet, dPLAYER, g_nPos );
			
			PutSize( g_Packet, g_nPos );
			
			SendToRange( pPlayer, g_Packet, g_nPos, dATTACK_PACKET_RANGE );

			//
			g_nPos = 2;
			
			PutWord( g_Packet, dPACKET_MAGIC_CAST_USER, g_nPos );
			PutWord( g_Packet, magicNum, g_nPos );
			PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
			PutInteger( g_Packet, GET_IDX( pPlayer->w_next ), g_nPos );
		
			PutByte( g_Packet, dPLAYER, g_nPos );
			
			if( GET_EFFECT_IDX( pPlayer ) >= 65000 )
				GET_EFFECT_IDX( pPlayer ) = dMAGIC_IDX_SEED;
			else
				GET_EFFECT_IDX( pPlayer )++;
			
			PutWord( g_Packet, GET_EFFECT_IDX( pPlayer ), g_nPos );		
			PutSize( g_Packet, g_nPos );
			
			SendToRange( pPlayer, g_Packet, g_nPos, dATTACK_PACKET_RANGE );
		}
		return;
	}

	BYTE where = number( 0, 7 );

	int posX = pPlayer->ch.posX;
	int posY = pPlayer->ch.posY;

	switch( where )
	{
	case 0:
		posY+=5;
		break;
	case 1:
		posY+=5;
		posX+=5;
		break;
	case 2:
		posX+=5;
		break;
	case 3:
		posX+=5;
		posY-=5;
		break;
	case 4:
		posY-=5;
		break;
	case 5:
		posX-=5;
		posY-=5;
		break;
	case 6:
		posX-=5;
		break;
	case 7:
		posX-=5;
		posY+=5;
		break;
	}

	if( posX >= 10 && posX < 450 && posY >= 10 && posY < 450 )
		MovePlayer( pPlayer, posX, posY , 0);
}

//
// Function Name: CreateDummy
// Date: 
// Description: 
//
void CreateDummy( int mapNum, int posX, int posY, char *name , int idxNum )
{
	sPDESC_DATA newDummy = (sPDESC_DATA)malloc(sizeof(sDESC_DATA));

	if( !newDummy )
		return;

	InitPlayerData( newDummy, idxNum, "127.0.0.1" );
	
	newDummy->isDummy = 1;
	newDummy->ch.max_hp = 10000;
	newDummy->ch.hp = 10000;
	newDummy->ch.max_mp = 10000;
	newDummy->ch.mp = 10000;
	newDummy->ch.str = 10;
	newDummy->ch.dex = 10;
	newDummy->ch.intelli = 10;

	GET_SATTACK( newDummy ) = 100;
	GET_LATTACK( newDummy ) = 100;
	GET_BMAGIC( newDummy ) = 100;
	GET_WMAGIC( newDummy ) = 100;

	// 여자더미 
	//	newDummy->ch.sex = 1;
	//	newDummy->ch.faceStyle = 544;
	//	newDummy->ch.headStyle = 557;

	// 남자 더미 
	newDummy->ch.sex = 0;
	newDummy->ch.faceStyle = 10081;
	newDummy->ch.headStyle = 10085;

	strcpy( newDummy->ch.name, name );
	strcpy( newDummy->ch.id, name );
	
	// 링크드 리스트 연결
	INSERT_TO_LIST( gPLAYERS.list, newDummy , prev, next );
	gPLAYERS.total++;

	PlayerIntoMap( newDummy, mapNum, posX, posY );
	GET_STATE( newDummy ) = dDESC_STATE_PLAYING;

}
#endif


//
// Function Name: LoadPlayerHPMPTable
// Date:
// Description: 
//
BOOL LoadPlayerHPMPTable()
{
	// Init
	for( int i = 0; i < 2; i++ )
	{
		g_STATE_MPHP_TABLE[i].SAttack = 0;
		g_STATE_MPHP_TABLE[i].LAttack = 0;
		g_STATE_MPHP_TABLE[i].BMagic = 0;
		g_STATE_MPHP_TABLE[i].WMagic = 0;
		g_STATE_MPHP_TABLE[i].minStr = 0;
		g_STATE_MPHP_TABLE[i].maxStr = 0;
		g_STATE_MPHP_TABLE[i].minDex = 0;
		g_STATE_MPHP_TABLE[i].maxDex = 0;
		g_STATE_MPHP_TABLE[i].minInt = 0;
		g_STATE_MPHP_TABLE[i].maxInt = 0;
	}
	// End Init

	FILE *fp;

	fp = fopen( dLEVEL_HPMP_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadPlayerHPMPTable : Load Failed!!\r\n" );
		return 0;
	}

	char readBuff[1024];

	int cnt = 0;
	
	int value[10];

	while( !feof( fp ) )
	{
		if( cnt >= 2 )
			break;

		fgets( readBuff, 1024, fp );

		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		if( sscanf( readBuff, "%d %d %d %d %d %d %d %d %d %d", 
			&value[0], 
			&value[1], 
			&value[2], 
			&value[3], 
			&value[4], 
			&value[5], 
			&value[6], 
			&value[7], 
			&value[8], 
			&value[9]
			) != 10 )
		{
			log( "LoadPlayerHPMPTable : Bad File Format\r\n" );
			fclose( fp );
			return 0;
		}

		g_STATE_MPHP_TABLE[cnt].SAttack = value[0];
		g_STATE_MPHP_TABLE[cnt].LAttack = value[1];
		g_STATE_MPHP_TABLE[cnt].BMagic = value[2];
		g_STATE_MPHP_TABLE[cnt].WMagic = value[3];
		g_STATE_MPHP_TABLE[cnt].minStr = value[4];
		g_STATE_MPHP_TABLE[cnt].maxStr = value[5];
		g_STATE_MPHP_TABLE[cnt].minDex = value[6];
		g_STATE_MPHP_TABLE[cnt].maxDex = value[7];
		g_STATE_MPHP_TABLE[cnt].minInt = value[8];
		g_STATE_MPHP_TABLE[cnt].maxInt = value[9];
		
		cnt++;
	}

	fclose(fp);

	return 1;
}

//
// Function Name: LoadPlayerAbilityTable
// Date:
// Description: 
//
BOOL LoadPlayerAbilityTable()
{
	// Init
	int x, y;

	for( y = 0; y < dMAX_PLAYER_ABL_TABLE; y++ )
	{
		for( x = 0; x < dMAX_PLAYER_ABL_CLASS; x++ )
		{
			g_PLAYER_ABL_TABLE[y][x] = 0;
		}
	}
	// End Init

	FILE *fp = fopen( dCHARPOINT_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadPlayerAbilityTable : ./DATA/char_def_point.txt Load Failed!!\r\n" );
		return 0;
	}

	char readBuff[1024];

	char sectionName[128];

	int cnt = 0;
	int nPos;

	while( !feof( fp ) )
	{
		nPos = 0;

		fgets( readBuff, 1024, fp );

		if( *readBuff == ';' )
			continue;
		
		if( *readBuff == '\n' ||
			*readBuff == '\r' ||
			*readBuff == '\0' )
			break;

		GetTabString( readBuff, sectionName, nPos );
		
		for( int i = 0; i < dMAX_PLAYER_ABL_CLASS; i++ )
		{
			g_PLAYER_ABL_TABLE[cnt][i] = GetTabNumberic( readBuff, nPos );
		}
	
		cnt++;
	}

	fclose(fp);

	return 1;
}

//
// Function Name: LoadBaseHitTable
// Date:
// Description: 
//
BOOL LoadBaseHitTable()
{
	int x, y;

	for( y = 0; y < 6; y++ )
	{
		for( x = 0; x < 6; x++ )
		{
			g_BASEHIT_TABLE[y][x] = 0;
		}
	}

	FILE *fp = fopen( dBASEHIT_TABLE_FILE, "rb" );

	if( !fp )
	{
		log( "LoadBaseHitTable() .. file Load Failed\r\n" );
		return 0;
	}

	int cnt = 0;
	int value[6];

	while( !feof( fp ) )
	{
		if( cnt >= 6 )
		{
			log( "LoadBaseHitTable : cnt over\r\n" );
			fclose( fp );
			return 0;
		}
		if( fscanf( fp,"%d %d %d %d %d %d", 
				&value[0], &value[1], &value[2],
				&value[3], &value[4], &value[5] ) != 6 )
		{
			log( "LoadBaseHitTable : bad file format\r\n" );
			fclose( fp );
			return 0;
		}
		
		g_BASEHIT_TABLE[cnt][0] = value[0];
		g_BASEHIT_TABLE[cnt][1] = value[1];
		g_BASEHIT_TABLE[cnt][2] = value[2];
		g_BASEHIT_TABLE[cnt][3] = value[3];
		g_BASEHIT_TABLE[cnt][4] = value[4];
		g_BASEHIT_TABLE[cnt][5] = value[5];

		cnt++;
	}

	fclose( fp );
	return 1;
}

//
// Function Name: InitPlayerData
// Date: 2003-02-12
// Description: 플레이어 데이터 초기화 
//
void InitDescData( sPDESC_DATA pData, SOCKET sock, char *hostname )
{
	int i;
	////////////////////////////////////
	// INIT DESCRIPTOR DATA
	////////////////////////////////////
	pData->sock = sock;
	
	strcpy( pData->ip, hostname );
	
	*pData->recvBuff = NULL_STR;	
	*pData->TemprecvBuff = NULL_STR;
	pData->recvSize = 0;	
	pData->recvPos = 0;
	
	*pData->sendBuff = NULL_STR;		
	pData->sendSize = 0;	
	
	pData->isWaitAllData = 0;
	pData->isWaitCharData = 0;
	pData->isWaitDelData = 0;
	
	// 전체 플레이어 리스트용
	pData->prev = NULL;
	pData->next = NULL;
	
	// 월드 데이터용 
	pData->w_prev = NULL;
	pData->w_next = NULL;
	
	// 케릭터명 리스트용 
	pData->name_next = NULL;
	pData->name_prev = NULL;
	
	// 아이디 리스트용 
	pData->id_next = NULL;
	pData->id_prev = NULL;
	
	// 길드용 
	pData->guild_next = NULL;
	pData->guild_prev = NULL;
	
	// 전체 체팅용 
	pData->c_next = NULL;
	pData->c_prev = NULL;
	
	// 용병용 
	pData->merc_prev = NULL;
	pData->merc_next = NULL;
    
	pData->fishing_prev = NULL;
	pData->fishing_next = NULL;
	
	GET_STATE( pData ) = dDESC_STATE_LOGIN;
	GET_DEAD_STATE( pData ) = dDEAD_STATE_NONE;
	pData->isNewCharacter = 0;
	
	pData->isInIdList = 0;
	pData->isInNameList = 0;
	
	pData->isAccepted = 0;
	
	pData->inven = NULL;				// 소지품 
	pData->invenIdx = 0;
	pData->garbageCnt = 0;
	pData->garbage = NULL;				// 창고
	pData->garbageMoney = 0;
	pData->useGarbageMoney = 0;
	pData->useCashMoney = 0;
	
	pData->smith = NULL;
	pData->smithCnt = 0;
	
	pData->billItems = NULL;			// 빌링 아이템들

	pData->chDataLoaded = 0;
    
	pData->enemyMob = NULL;
	pData->victPlayer = NULL;
	
	for( i = 0; i < dMAX_DATA_UPDATE; i++ )
		pData->isUpdate[i] = 0;
	pData->lastDataUpdateTime = g_CurrTime;
	
	pData->lastRecvTime = g_CurrTime;
	pData->lastAliveCheck = g_CurrTime;
	pData->isSpeedHack = 0;
	
	pData->aliveCheckTick = g_CurrRealTime;
	pData->aliveCnt = 0;
	
	pData->isAdmin = 0;					// 운영툴
	pData->adminLevel = 0;
	pData->isGodCharacter = 0;
	
	pData->disconnectFlag = 0;

	GET_CURRITEM_WEIGHT( pData ) = 0;
	
#ifdef _DUMMY_MODE_
	pData->isDummy = 0;
	pData->dummyMoveTime = 0;
#endif
	
	for( i = 0; i < dMAX_EQUIP; i++ )	// 장비
		pData->equip[i] = NULL;
	
	GET_USE_SKILL( pData ) = dSATTACK_SKILL_ITEM;
		
	pData->chatEndTime = g_CurrTime;
	
	pData->badUserType = dBADUSER_TYPE_NONE;
	
	for( i = 0; i < 3; i++ )
		pData->selectGuildLevel[i] = dGUILD_LEVEL_NONE;
}

void InitCharacterData( sPDESC_DATA pData, SOCKET sock, char *hostname )
{
	int i;

	////////////////////////////////////////
	// INIT CHARACTER DATA
	////////////////////////////////////////
	*pData->ch.id = NULL_STR;
	*pData->ch.name = NULL_STR;
	
	pData->ch.sex = number( 0, 1 );
	
	GET_CLASS( pData ) = dCLASS_NONE;
	
	pData->ch.hp = dDEF_PLAYER_HP;
	pData->ch.max_hp = dDEF_PLAYER_HP;
	
	GET_MAP_NUM( pData ) = 4;
	GET_POSX( pData ) = 297;
	GET_POSY( pData ) = 331;
	GET_MAP_KIND( pData ) = 0;
	
	GET_LAYER( pData ) = 0;
	
	GET_STR( pData ) = dDEF_STR;
	GET_DEX( pData ) = dDEF_DEX;
	GET_INT( pData ) = dDEF_INT;
	
	GET_STRADD( pData ) = 0;
	GET_DEXADD( pData ) = 0;
	GET_INTADD( pData ) = 0;
	
	GET_CHA( pData ) = 0;
	
	pData->ch.faceStyle = 0;
	pData->ch.headStyle = 0;
	
	for( i = 0; i < dCH_EQ_MAX; i++ )
		pData->ch.chEquip[i] = 0;
	
	int invenX, invenY;
	for( i = 0; i < dINVEN_PAGE; i++ )
	{
		for( invenY = 0; invenY < dINVEN_Y; invenY++ )
			for( invenX = 0; invenX < dINVEN_X; invenX++ )
				pData->invenArray[i][invenY][invenX] = 0;
	}
	
	pData->ch.money = 0;
	
	pData->ch.charPos = 0;
	
	GET_GUILD( pData ) = 0;
	GET_GUILD_LEVEL( pData ) = 0;
	GET_TMP_GUILD( pData ) = 0;
	
	GET_RIDE_TYPE( pData ) = 0;
	GET_RIDE_STATE( pData ) = 0;
	
	for( i = 0; i < dMAX_PLAYER_SKILL; i++ )
	{
		pData->ch.skill[i] = 1;
		pData->ch.skill_exp[i] = 0;
	}
	
	////////////////////////////////////
	// Mercenary
	////////////////////////////////////
	GET_MERC_TYPE( pData ) = 0;
	GET_MERC_LEVEL( pData ) = 0;
	GET_MERC_EXP( pData ) = 0;
}

void InitCharacterDataSub( sPDESC_DATA pData, SOCKET sock, char *hostname )
{
	int i;
	
	////////////////////////////////////
	// INIT CHARACTER DATA SUB
	////////////////////////////////////
	GET_GOTO_MAPNUM( pData ) = 1;
	GET_GOTO_MAPKIND( pData ) = 0;
	GET_GOTO_POSX( pData ) = 297;
	GET_GOTO_POSY( pData ) = 331;

	// Add
	GET_HPADD( pData ) = 0;
	GET_MPADD( pData ) = 0;
	
	GET_STRADD_EX( pData ) = 0;
	GET_DEXADD_EX( pData ) = 0;
	GET_INTADD_EX( pData ) = 0;

	GET_SATTACK_ADD( pData ) = 0;
	GET_LATTACK_ADD( pData ) = 0;
	GET_BMAGIC_ADD( pData ) = 0;
	GET_WMAGIC_ADD( pData ) = 0;
	// Add End

	pData->ch2.expJump = 1;

	pData->ch2.actionDelay = g_CurrTime;

	pData->ch2.endMapLoading = 1;

	GET_EFFECT_IDX( pData ) = dMAGIC_IDX_SEED;

	for( i = 0; i < dMAX_STATUS_DELAY; i++ )
		GET_STATUS_DELAY( pData, i ) = 0;

	pData->ch2.flag = 0;

	GET_GHOST_TIMER( pData ) = 0;


	pData->ch2.partyExp = 0;
	
	pData->ch2.friendList = NULL;
	pData->ch2.friendCnt = 0;

	pData->ch2.haveSkill = NULL;

	pData->ch2.btGuildKickOut = 0;
	pData->ch2.btFirstLoad = 1;

	// PET
#ifndef dNOT_USE_PET
	GET_PET_TYPE( pData ) = 0;
	GET_PET_LEVEL( pData ) = 0;
	GET_PET_HP( pData ) = 0;
	GET_PET_MP( pData ) = 0;
	GET_PET_EXP( pData ) = 0;
	GET_PET_STATE( pData ) = dPET_STATE_DISABLE;
	GET_PET_ANGERGAGE( pData ) = 0;
	*GET_PET_NAME( pData ) = NULL_STR;
#endif

	// 낚시 
    
	GET_VISIBLE( pData ) = 0;

#ifdef dUSE_AUTO_SHOP
	GET_CURR_OPENSHOP( pData ) = 1;
#endif

	for( i = 0; i < dSPESKILL_DELAY_MAX; i++ )
		pData->ch2.speSkillDelay[i] = 0;


	///////////////////////////////////////
	// FOR PARTY
	///////////////////////////////////////
	GET_PARTY_MASTER( pData ) = NULL;
//	PARTY_ClearMember( pData );
	for( i = 0; i < dMAX_PARTY_MEMBER; i++ )
	{
		pData->ch2.party.member[i] = NULL;
	}
	pData->ch2.party.memberCnt = 0;
	
	///////////////////////////////////////
	// SUMMON MOB
	///////////////////////////////////////
	GET_SUMMON_MOB( pData ) = NULL;
	GET_SUMMON_MOBLIFE( pData ) = 0;

	//////////////////////////////////////
	// FOR ITEM EXCHANGE
	//////////////////////////////////////
	InitDealData( pData );

	// 개인상점 
	InitMyShop( pData );

	pData->ch2.worldChatDelay = g_CurrRealTime;
	pData->ch2.sellingChatDelay = g_CurrRealTime;

	pData->ch2.evilChaTime = 0;
	pData->ch2.isEvilCha = 0;

	pData->ch2.lastCastMagic = 0;

#ifdef dUSE_DEAD_GHOST
	pData->ch2.isDeadGhost = 0;
#endif

	GET_PVPMODE( pData ) = dPVPMODE_NONE;

	for( i = 0; i < dMAX_BILLING_FLAG; i++ )
	{
		pData->ch2.billingFlag[i] = 0;
		pData->ch2.billingAddValue[i] = 0;
	}
	
	pData->ch2.haveBillRow = 0;
	pData->ch2.checkBillItemTime = g_CurrRealTime;
	pData->ch2.lastSaveBillInfoTime = g_CurrRealTime;

	for( i = 0; i < dMAX_PLAYER_EFFECT; i++ )
	{
		pData->ch2.skillEffect[i].skillNum = 0;
		pData->ch2.skillEffect[i].delay = 0;
		pData->ch2.skillEffect[i].effectType = 0;
		pData->ch2.skillEffect[i].value = 0;
		pData->ch2.skillEffect[i].valueType = 0;
	}

	pData->ch2.waitSkill.skillNum = 0;
	pData->ch2.waitSkill.targetIdx = 0;
	pData->ch2.waitSkill.targetType = 0;

	pData->ch2.EnchantNoFail = 0;
	pData->ch2.guildSummonTime = 0;

#ifdef _dUMENOKOJI_GHOST_GUILD_		//유령 길드 관련 전쟁포인트 테스트
	pData->ch2.getTime = g_CurrRealTime;
#endif
}

void InitPlayerData( sPDESC_DATA pData, SOCKET sock, char *hostname )
{
	InitDescData( pData, sock, hostname );
	InitCharacterData( pData, sock, hostname );
	InitCharacterDataSub( pData, sock, hostname );
}


//
// Function Name: InitDealData
// Date: 2003-05-11
// Description: 
//
void InitDealData( sPDESC_DATA pPlayer )
{
	pPlayer->ch2.dealVict = NULL;
	pPlayer->ch2.dealItems.Init();
	pPlayer->ch2.dealMoney = 0;
	pPlayer->ch2.dealCheckIn = 0;
	pPlayer->ch2.dealFinish = 0;
}


//
// Function Name: NewClient
// Date: 2003-02-12
// Description: 
//
sPDESC_DATA NewClient()
{
	sPDESC_DATA newClient;

	if( gPLAYERS.memory )
	{
		newClient = gPLAYERS.memory;
		
		REMOVE_FROM_LIST( gPLAYERS.memory, newClient , prev, next );

		gPLAYERS.total_memory--;
		
		return newClient;
	}
	else
	{
		newClient = (sPDESC_DATA)malloc(sizeof(sDESC_DATA));

		if( !newClient )
		{
			log( "NewClient malloc failed!\r\n" );
			return NULL;
		}
		
		return newClient;
	}
	return NULL;
}




///////////////////////////////////////////////////
// 케릭터 맵 이동 처리
///////////////////////////////////////////////////

//
// Function Name: MakeInPacket
// Date: 2003-02-12
// Description: 
//
void MakeInPacket( sPDESC_DATA pPlayer, BYTE isConnect )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHARACTER_INSERT, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutString( g_Packet, pPlayer->ch.name, g_nPos );
	PutByte( g_Packet, pPlayer->ch.sex, g_nPos );
	PutByte( g_Packet, isConnect, g_nPos );

	PutInteger( g_Packet, GET_POSX( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_POSY( pPlayer ), g_nPos );

	// 추가 
	PutWord( g_Packet, pPlayer->ch.headStyle, g_nPos );
	PutWord( g_Packet, pPlayer->ch.faceStyle, g_nPos );

#ifndef _DUMMY_MODE_
	PutWord( g_Packet, pPlayer->equip[dEQUIP_BOW] ? pPlayer->equip[dEQUIP_BOW]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_WEAPON] ? pPlayer->equip[dEQUIP_WEAPON]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_GLOVE] ? pPlayer->equip[dEQUIP_GLOVE]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_HELMET] ? pPlayer->equip[dEQUIP_HELMET]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_JACKET] ? pPlayer->equip[dEQUIP_JACKET]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_PANTS] ? pPlayer->equip[dEQUIP_PANTS]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_SHIELD] ? pPlayer->equip[dEQUIP_SHIELD]->itemNum : 0, g_nPos );
	PutWord( g_Packet, pPlayer->equip[dEQUIP_SHOES] ? pPlayer->equip[dEQUIP_SHOES]->itemNum : 0, g_nPos );
#else
	if( pPlayer->isDummy )
	{
		PutWord( g_Packet, (WORD)number( 2710, 3150) , g_nPos );
		PutWord( g_Packet, (WORD)number( 1, 1071 ), g_nPos );
		PutWord( g_Packet, (WORD)number( 4978, 5124 ), g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_HELMET] ? pPlayer->equip[dEQUIP_HELMET]->itemNum : 0, g_nPos );
		PutWord( g_Packet, (WORD)number( 4033, 4431 ), g_nPos );
		PutWord( g_Packet, (WORD)number( 5986, 6195 ), g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_SHIELD] ? pPlayer->equip[dEQUIP_SHIELD]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_SHOES] ? pPlayer->equip[dEQUIP_SHOES]->itemNum : 0, g_nPos );
	}
	else
	{
		PutWord( g_Packet, pPlayer->equip[dEQUIP_BOW] ? pPlayer->equip[dEQUIP_BOW]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_WEAPON] ? pPlayer->equip[dEQUIP_WEAPON]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_GLOVE] ? pPlayer->equip[dEQUIP_GLOVE]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_HELMET] ? pPlayer->equip[dEQUIP_HELMET]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_JACKET] ? pPlayer->equip[dEQUIP_JACKET]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_PANTS] ? pPlayer->equip[dEQUIP_PANTS]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_SHIELD] ? pPlayer->equip[dEQUIP_SHIELD]->itemNum : 0, g_nPos );
		PutWord( g_Packet, pPlayer->equip[dEQUIP_SHOES] ? pPlayer->equip[dEQUIP_SHOES]->itemNum : 0, g_nPos );
	}
#endif
	// 용병
	PutByte( g_Packet, GET_MERC_TYPE( pPlayer ) > 0 ? GET_MERC_TYPE( pPlayer ) : 0, g_nPos );
	
	// 레이어 값 
	PutByte( g_Packet, GET_LAYER( pPlayer ), g_nPos );
	
	// 스킬 번호 
	PutWord( g_Packet, GET_USE_SKILL( pPlayer ), g_nPos );

	// 탈것 정보 
	PutWord( g_Packet, GET_RIDE_STATE( pPlayer ) ? GET_RIDE_TYPE( pPlayer ) : 0, g_nPos );
	PutByte( g_Packet, GetRideSpeed( pPlayer ), g_nPos );

	// 죽은 상태?
	PutByte( g_Packet, GET_DEAD_STATE( pPlayer ) == dDEAD_STATE_CORPSE ? 1 : 0, g_nPos );

	// 길드 정보
	// 길드명
	if( GET_GUILD( pPlayer ) && !GET_TMP_GUILD( pPlayer ) )
	{
		if( IS_VALID_GUILD( GET_GUILD( pPlayer ) ) )
		{
			PutString( g_Packet, GET_GUILD_DATA( GET_GUILD( pPlayer ) )->guildName, g_nPos );
			PutWord( g_Packet, GET_GUILD_DATA( GET_GUILD( pPlayer ) )->guildMark, g_nPos );
		}
		else
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, 0, g_nPos );
		}
	}
	else if( GET_GUILD( pPlayer ) && GET_TMP_GUILD( pPlayer ) )
	{
		if( IS_VALID_GUILD( GET_GUILD( pPlayer ) ) &&
			IS_VALID_GUILD( GET_TMP_GUILD( pPlayer ) ) )
		{
			PutString( g_Packet, GET_GUILD_DATA( GET_TMP_GUILD( pPlayer ) )->guildName, g_nPos );
			PutWord( g_Packet, GET_GUILD_DATA( GET_TMP_GUILD( pPlayer ) )->guildMark, g_nPos );
		}
		else
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, 0, g_nPos );
		}
	}
	else if( !GET_GUILD( pPlayer ) && GET_TMP_GUILD( pPlayer ) )
	{
		if( IS_VALID_GUILD( GET_TMP_GUILD( pPlayer ) ) )
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, GET_GUILD_DATA( GET_TMP_GUILD( pPlayer ) )->guildMark, g_nPos );
		}
		else
		{
			PutString( g_Packet, " ", g_nPos );
			PutWord( g_Packet, 0, g_nPos );
		}
	}
	else
	{
		PutString( g_Packet, " ", g_nPos );
		PutWord( g_Packet, 0, g_nPos );
	}  
	// 길드정보 END

	// Visible 설정
	PutByte( g_Packet, ( GET_DEAD_STATE( pPlayer ) == dDEAD_STATE_GHOST || GET_VISIBLE( pPlayer ) ) ? 1 : 0, g_nPos );	

	// PET INFO
#ifndef dNOT_USE_PET
	if( IS_USING_PET( pPlayer ) )
		PutByte( g_Packet, GET_PET_TYPE( pPlayer ), g_nPos );
	else
		PutByte( g_Packet, 0, g_nPos );
#else
	PutByte( g_Packet, 0, g_nPos );
#endif

	// 낚시 상태?
	PutByte( g_Packet, 0, g_nPos );
	PutInteger( g_Packet, 0, g_nPos );
	PutInteger( g_Packet, 0, g_nPos );

	// PET INFO
#ifndef dNOT_USE_PET
	if( IS_USING_PET( pPlayer ) )
		PutString( g_Packet, GET_PET_NAME( pPlayer ), g_nPos );
	else
		PutString( g_Packet, " ", g_nPos );
#else
	PutString( g_Packet, " ", g_nPos );
#endif
	
	PutByte( g_Packet, GET_CLASS( pPlayer ), g_nPos );				// 직업 
	PutInteger( g_Packet, GET_ALL_SKILL( pPlayer ), g_nPos );		// 레벨
	
	PutByte( g_Packet, GET_GUILD_LEVEL( pPlayer ), g_nPos );		// 길드 등급

	// 개인상점
	if( pPlayer->ch2.myShop.isMyShopOpen )
	{
		PutByte( g_Packet, 1, g_nPos );
		PutString( g_Packet, pPlayer->ch2.myShop.myShopName, g_nPos );
	}
	else
	{
		PutByte( g_Packet, 0, g_nPos );
		PutString( g_Packet, " ", g_nPos );
	}

	PutInteger( g_Packet, GET_GUILD( pPlayer ), g_nPos );

#ifdef __CHINA_VER__
	sNAME_COLOR nameColor;

//	if( pPlayer->ch2.isEvilCha )
	//{
	//	GetNameColor( dNAMECOLOR_EVILCHA, &nameColor );
//	}
//	else
//	{
		GetPlayerNameColor( pPlayer, &nameColor );
//	}
	
	PutByte( g_Packet, nameColor.r, g_nPos );
	PutByte( g_Packet, nameColor.g, g_nPos );
	PutByte( g_Packet, nameColor.b, g_nPos );
#else

	if( GET_CHA( pPlayer ) <= -30000 )
	{
		PutByte( g_Packet, 255, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
		PutByte( g_Packet, 0, g_nPos );
	}
	else
	{
		PutByte( g_Packet, 192, g_nPos );
		PutByte( g_Packet, 192, g_nPos );
		PutByte( g_Packet, 192, g_nPos );
	}
#endif

	PutSize( g_Packet, g_nPos );	
}

//
// Function Name: MakeOutPacket
// Date: 2003-02-12
// Description: 
//
void MakeOutPacket( sPDESC_DATA pPlayer , BYTE isDisconnect )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHARACTER_REMOVE, g_nPos );
	PutInteger( g_Packet, pPlayer->sock, g_nPos );
	PutByte( g_Packet, isDisconnect, g_nPos );
	PutSize( g_Packet, g_nPos );
}

//
// Function Name: SendToSector
// Date: 2003-02-12
// Description: 
//
void SendToSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, char *data, int size , BYTE mode )
{
	sPDESC_DATA d, next_d;

	if( mode == dSENDMODE_TOALL )
	{
		LIST_WHILE( tile, d, next_d , w_next, WMgr );
		SendData( d, data, size );
		LIST_WHILEEND( tile, d, next_d );	
	}
	else
	{
		LIST_WHILE( tile, d, next_d, w_next, WMgr );
		if( d != pPlayer )
			SendData( d, data, size );
		LIST_WHILEEND( tile, d, next_d );
	}
}

//
// Function Name: SendToRangeSector
// Date: 2003-06-30
// Description: 
//
void SendToRangeSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, char *data, int size, int range )
{
	sPDESC_DATA d, next_d;
	int rangeX, rangeY;

	LIST_WHILE( tile, d, next_d , w_next, WMgr );
	rangeX = abs( GET_POSX( pPlayer ) - GET_POSX( d ) );
	rangeY = abs( GET_POSY( pPlayer ) - GET_POSY( d ) );
	if( rangeX <= range && rangeY <= range )
		SendData( d, data, size );
	LIST_WHILEEND( tile, d, next_d );
}


//
// Function Name: GetPlayerSector
// Date: 2003-02-12
// Description: 
//
void GetPlayerSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->player[y][x], d, next_d, w_next, WMgr )

	if( d != pPlayer )
	{
		MakeInPacket( d , 0 );
		SendData( pPlayer, g_Packet, g_nPos );
	}
	LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->player[y][x], d, next_d );
}

//
// Function Name: GetPlayerVSector
// Date: 2003-02-12
// Description: 
//
void GetPlayerVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				GetPlayerSector( pPlayer,  x, y );
		}
	}
}

//
// Function Name: GetPlayerHSector
// Date: 2003-02-12
// Description: 
//
void GetPlayerHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				GetPlayerSector( pPlayer, x, y );
		}
	}
}

//
// Function Name: OutPlayerSector
// Date: 2003-02-12
// Description: 
//
void OutPlayerSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->player[y][x], d, next_d , w_next, WMgr );
	if( d != pPlayer )
	{
		if( d->victPlayer == pPlayer )
			d->victPlayer = NULL;

		MakeOutPacket( d , 0 );
		SendData( pPlayer, g_Packet, g_nPos );
	}
	LIST_WHILEEND( gWORLD[pPlayer->ch.mapNum]->player[y][x], d, next_d );
}


//
// Function Name: OutPlayerVSector
// Date: 2003-02-12
// Description: 
//
void OutPlayerVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				OutPlayerSector( pPlayer,  x, y );
		}
	}
}


//
// Function Name: OutPlayerHSector
// Date: 2003-02-12
// Description: 
//
void OutPlayerHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				OutPlayerSector( pPlayer, x, y );
		}
	}
}

//
// Function Name: SendToVSector
// Date: 2003-02-12
// Description: 
//
void SendToVSector( int x, int startY, int endY, sPDESC_DATA pPlayer, char *data, int size )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{
		int mapNum = pPlayer->ch.mapNum;
		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				SendToSector( pPlayer, gWORLD[mapNum]->player[y][x], data, size , dSENDMODE_TOOTHER );
		}
	}
}

//
// Function Name: SendToHSector
// Date: 2003-02-12
// Description: 
//
void SendToHSector( int y, int startX, int endX, sPDESC_DATA pPlayer, char *data, int size )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		int mapNum = pPlayer->ch.mapNum;

		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				SendToSector( pPlayer, gWORLD[mapNum]->player[y][x], data, size, dSENDMODE_TOOTHER );
		}
	}
}


//
// Function Name: SendToHouse
// Date: 
// Description: 
//
void SendToHouse( sPDESC_DATA pPlayer, char *data, int size , BYTE mode )
{
	sPDESC_DATA player, next_player;

	if( mode == dSENDMODE_TOALL )
	{
		LIST_WHILE( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player, w_next, WMgr );
		SendData( player , data, size );
		LIST_WHILEEND( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player );
	}
	else
	{
		LIST_WHILE( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player, w_next, WMgr );
		if( player != pPlayer )
			SendData( player , data, size );
		LIST_WHILEEND( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player );
	}
}




//
// Function Name: SendToArea
// Date: 2003-02-12
// Description: 
//
void SendToArea( sPDESC_DATA pPlayer , char *data, int size, BYTE mode )
{
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		SendToHouse( pPlayer, data, size, mode );
		return;
	}

	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);

	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				SendToSector( pPlayer, gWORLD[pPlayer->ch.mapNum]->player[y][x], data, size, mode );
}


//
// Function Name: SendToRange
// Date: 2003-06-30
// Description: 
//
void SendToRange( sPDESC_DATA pPlayer, char *data, int size , int range )
{
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		SendToHouse( pPlayer, data, size, dSENDMODE_TOALL );
		return;
	}

	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);

	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				SendToRangeSector( pPlayer, gWORLD[pPlayer->ch.mapNum]->player[y][x], data, size , range );
}


//
// Function Name: MovePlayer
// Date: 2003-02-12
// Description: 
//
BOOL MovePlayer( sPDESC_DATA pPlayer , int posX, int posY , BYTE layer )
{
	if( posX < 0 || posX >= dONE_MAP_SIZE ||
		posY < 0 || posY >= dONE_MAP_SIZE )
		return 1;

	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING &&
		GET_STATE( pPlayer ) != dDESC_STATE_FIGHT )
		return 1;	

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE &&
		GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_GHOST )
		return 1;

	if( abs( GET_POSX( pPlayer ) - posX ) > dPLAYER_MOVE_RANGE ||
		abs( GET_POSY( pPlayer ) - posY ) > dPLAYER_MOVE_RANGE )
		return 0;

	if( pPlayer->ch2.waitSkill.skillNum )
	{ 
		pPlayer->ch2.waitSkill.skillNum = 0;
		pPlayer->ch2.waitSkill.targetIdx = 0;
		pPlayer->ch2.waitSkill.targetType = 0;
	}

	// 집 안이면
	if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
	{
		GET_POSX( pPlayer ) = posX;
		GET_POSY( pPlayer ) = posY;
		GET_LAYER( pPlayer ) = layer;	

		if( GET_DEAD_STATE( pPlayer ) == dDEAD_STATE_NONE )
		{
			g_nPos = 2;
			
			PutWord( g_Packet, dPACKET_CHARACTER_MOVE, g_nPos );
			PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
			PutInteger( g_Packet, GET_POSX( pPlayer ), g_nPos );
			PutInteger( g_Packet, GET_POSY( pPlayer ), g_nPos );
			PutByte( g_Packet, GET_LAYER( pPlayer ), g_nPos );
			PutSize( g_Packet, g_nPos );
			
			SendToHouse( pPlayer , g_Packet, g_nPos , dSENDMODE_TOOTHER );
			
			if( GET_PARTY_MASTER( pPlayer ) )
				PARTY_SendMovePacket( pPlayer );
		}
		return 1;
	}
	int newX, newY, oldX, oldY, mapNum = pPlayer->ch.mapNum, plusNum ;
	
	newX = (int)(posX / dONE_TILE);
	newY = (int)(posY / dONE_TILE);
	oldX = (int)(pPlayer->ch.posX / dONE_TILE);
	oldY = (int)(pPlayer->ch.posY / dONE_TILE);

	plusNum = 1;


	if( newX >= dONE_MAP_SIZE || newY >= dONE_MAP_SIZE )
		return 0;
	
	// stop_fighting_player
	if( pPlayer->enemyMob )
	{
		if( posX != GET_POSX( pPlayer ) ||
			posY != GET_POSY( pPlayer ) )
		{
			pPlayer->enemyMob = NULL;
			GET_STATE( pPlayer ) = dDESC_STATE_PLAYING;
		}
	}
	else if( pPlayer->victPlayer )
	{
		if( posX != GET_POSX( pPlayer ) ||
			posY != GET_POSY( pPlayer ) )
		{
			sPDESC_DATA pTmpPlayer = pPlayer->victPlayer;
			stop_fighting_player( pPlayer );
			stop_fighting_player( pTmpPlayer );
		}
	}

	gWORLD[pPlayer->ch.mapNum]->playerMoveAttr[pPlayer->ch.posY][pPlayer->ch.posX] = 0;

	GET_POSX( pPlayer ) = posX;
	GET_POSY( pPlayer ) = posY;
	GET_LAYER( pPlayer ) = layer;	

	gWORLD[pPlayer->ch.mapNum]->playerMoveAttr[pPlayer->ch.posY][pPlayer->ch.posX] = 1;

	if( newX != oldX || newY != oldY )
	{
		REMOVE_FROM_LIST( gWORLD[mapNum]->player[oldY][oldX], pPlayer, w_prev, w_next );
		INSERT_TO_LIST( gWORLD[mapNum]->player[newY][newX], pPlayer, w_prev, w_next );
		
		if( newY < oldY && newX == oldX ) // Y 감소
		{
			MakeOutPacket( pPlayer , 0 );
			SendToHSector( oldY + plusNum , oldX - plusNum , oldX + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerHSector( oldY + plusNum , oldX - plusNum , oldX + plusNum , pPlayer );
			OutMobHSector( oldY + plusNum , oldX - plusNum , oldX + plusNum , pPlayer );  // Mob
			OutItemHSector( oldY + plusNum , oldX - plusNum , oldX + plusNum , pPlayer );  // Item
			
			MakeInPacket( pPlayer , 0 );
			SendToHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer, g_Packet, g_nPos );

			GetPlayerHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetMobHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer ); // mob
			GetItemHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer ); // Item
		}
		else if( newX > oldX && newY < oldY ) // X증가 Y 감소
		{
			MakeOutPacket( pPlayer , 0 );
			SendToHSector( oldY + plusNum , oldX, oldX + plusNum , pPlayer, g_Packet, g_nPos );
			SendToVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerHSector( oldY + plusNum , oldX, oldX + plusNum , pPlayer );
			OutPlayerVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			
			// mob
			OutMobHSector( oldY + plusNum , oldX, oldX + plusNum , pPlayer );
			OutMobVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			// Item
			OutItemHSector( oldY + plusNum , oldX, oldX + plusNum , pPlayer );
			OutItemVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			
			MakeInPacket( pPlayer , 0 );
			SendToHSector( newY - plusNum , newX - plusNum , newX, pPlayer, g_Packet, g_nPos );
			SendToVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer, g_Packet, g_nPos );

			GetPlayerHSector( newY - plusNum , newX - plusNum , newX, pPlayer );
			GetPlayerVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			
			// mob
			GetMobHSector( newY - plusNum , newX - plusNum , newX, pPlayer );
			GetMobVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			// Item
			GetItemHSector( newY - plusNum , newX - plusNum , newX, pPlayer );
			GetItemVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
		}
		else if( newX > oldX && newY == oldY ) // X 증가
		{
			MakeOutPacket( pPlayer , 0 );
			SendToVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutMobVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer ); // mob
			OutItemVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer ); // Item
			
			MakeInPacket( pPlayer , 0 );
			SendToVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer, g_Packet, g_nPos );
			
			GetPlayerVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetMobVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer ); // mob
			GetItemVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer ); // Item
		}
		else if( newX > oldX && newY > oldY ) // X 증가 Y 증가
		{
			MakeOutPacket( pPlayer, 0 );
			SendToVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer, g_Packet, g_nPos );
			SendToHSector( oldY - plusNum , oldX, oldX + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutPlayerHSector( oldY - plusNum , oldX, oldX + plusNum , pPlayer );
			//Mob
			OutMobVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutMobHSector( oldY - plusNum , oldX, oldX + plusNum , pPlayer );
			//Item
			OutItemVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutItemHSector( oldY - plusNum , oldX, oldX + plusNum , pPlayer );
			
			MakeInPacket( pPlayer , 0 );
			SendToVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer, g_Packet, g_nPos );
			SendToHSector( newY + plusNum , newX - plusNum , newX, pPlayer, g_Packet, g_nPos );
			
			GetPlayerVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetPlayerHSector( newY + plusNum , newX - plusNum , newX, pPlayer );
			// mob
			GetMobVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetMobHSector( newY + plusNum , newX - plusNum , newX, pPlayer );
			// Item
			GetItemVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetItemHSector( newY + plusNum , newX - plusNum , newX, pPlayer );
		}
		else if( newY > oldY && newX == oldX ) // Y 증가
		{
			MakeOutPacket( pPlayer, 0 );
			SendToHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );
			OutMobHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer ); // mob
			OutItemHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer ); // Item
			
			MakeInPacket( pPlayer, 0 );
			SendToHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer, g_Packet, g_nPos );


			GetPlayerHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetMobHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer ); // mob
			GetItemHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer ); // Item
		}
		else if( newX < oldX && newY > oldY ) // X 감소 Y 증가
		{
			MakeOutPacket( pPlayer, 0 );
			SendToHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer, g_Packet, g_nPos );
			SendToVSector( oldX + plusNum , oldY, oldY + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );
			OutPlayerVSector( oldX + plusNum , oldY, oldY + plusNum , pPlayer );
			// mob
			OutMobHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );
			OutMobVSector( oldX + plusNum , oldY, oldY + plusNum , pPlayer );
			// Item
			OutItemHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );
			OutItemVSector( oldX + plusNum , oldY, oldY + plusNum , pPlayer );
			
			MakeInPacket( pPlayer , 0 );
			SendToHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer, g_Packet, g_nPos );
			SendToVSector( newX - plusNum , newY - plusNum , newY, pPlayer, g_Packet, g_nPos );
				
			GetPlayerHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetPlayerVSector( newX - plusNum , newY - plusNum , newY, pPlayer );
			// Mob
			GetMobHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetMobVSector( newX - plusNum , newY - plusNum , newY, pPlayer );
			// Item
			GetItemHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetItemVSector( newX - plusNum , newY - plusNum , newY, pPlayer );
		}
		else if( newX < oldX && newY == oldY ) // X 감소
		{
			MakeOutPacket( pPlayer, 0 );
			SendToVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutMobVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );// Mob
			OutItemVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );// Item
			
			MakeInPacket( pPlayer , 0 );
			SendToVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer, g_Packet, g_nPos );
			
			GetPlayerVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetMobVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );// Mob
			GetItemVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );// Item
		}
		else if( newX < oldX && newY < oldY ) // X감소 Y 감소
		{
			MakeOutPacket( pPlayer, 0 );
			SendToVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer, g_Packet, g_nPos );
			SendToHSector( oldY + plusNum , oldX - plusNum , oldX, pPlayer, g_Packet, g_nPos );
			
			OutPlayerVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutPlayerHSector( oldY + plusNum , oldX - plusNum , oldX, pPlayer );
			// Mob
			OutMobVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutMobHSector( oldY + plusNum , oldX - plusNum , oldX, pPlayer );
			// Item
			OutItemVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutItemHSector( oldY + plusNum , oldX - plusNum , oldX, pPlayer );
			
			MakeInPacket( pPlayer , 0 );
			SendToVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer, g_Packet, g_nPos );
			SendToHSector( newY - plusNum , newX, newX + plusNum , pPlayer, g_Packet, g_nPos );
			
			GetPlayerVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetPlayerHSector( newY - plusNum , newX, newX + plusNum , pPlayer );
			// Mob
			GetMobVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetMobHSector( newY - plusNum , newX, newX + plusNum , pPlayer );
			// Item
			GetItemVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetItemHSector( newY - plusNum , newX, newX + plusNum , pPlayer );
		}
	}

	g_nPos = 2;
	
	PutWord( g_Packet, dPACKET_CHARACTER_MOVE, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_POSX( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_POSY( pPlayer ), g_nPos );
	PutByte( g_Packet, GET_LAYER( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOOTHER );

	if( GET_DEAD_STATE( pPlayer ) == dDEAD_STATE_NONE )
	{
		if( GET_PARTY_MASTER( pPlayer ) )
			PARTY_SendMovePacket( pPlayer );
		
		CheckLoadingPoint( pPlayer ); 
	}

	return 1;
}

//
// Function Name: PlayerIntoIndoor
// Date:2003/04/12
// Description: 
//
void PlayerIntoIndoor( sPDESC_DATA pPlayer, int mapNum, int posX, int posY, BYTE posMapKind )
{
	if( posMapKind != dMAP_KIND_INDOOR )
	{
		log( "PlayerIntoIndoor : posMapKind != dMAP_KIND_INDOOR [%s/%d]\r\n",
			GET_ID( pPlayer ), GET_CHAR_POS( pPlayer ) );
		return;
	}

	pPlayer->aliveCheckTick = g_CurrRealTime;
	pPlayer->aliveCnt = 0;

	pPlayer->ch.mapNum = mapNum;
	pPlayer->ch.posX = posX;
	pPlayer->ch.posY = posY;
	pPlayer->ch.posMapKind = posMapKind;

	INSERT_TO_LIST( gHOUSE[mapNum]->in_player, pPlayer, w_prev, w_next );
	
	DATASERV_SendUpdateMapInfo( pPlayer );

	gHOUSE[mapNum]->currPlayerCnt++;

	MakeInPacket( pPlayer , 1 );
	SendToHouse( pPlayer, g_Packet, g_nPos, dSENDMODE_TOOTHER );

	GetCurrHousePlayer( pPlayer );
	// 몬스터
	GetCurrHouseMob( pPlayer );
	GetCurrHouseItem( pPlayer );

	SendPlayerMainSkill( pPlayer );
	SendPlayerAttackRange( pPlayer );
	MAGIC_SendMagicInfo( pPlayer );
	SendAllEffect( pPlayer );

	pPlayer->ch2.endMapLoading = 1;
}

//
// Function Name: PlayerIntoMap
// Date: 2003-02-12
// Description: 
//
void PlayerIntoMap( sPDESC_DATA pPlayer , int mapNum , int posX, int posY , BYTE posMapKind )
{
	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "PlayerIntoMap : IS_VALID_WORLD %s/%s\r\n", GET_NAME( pPlayer ), GET_ID( pPlayer ) );
		pPlayer->disconnectFlag = 1;
		return;
	}

	pPlayer->aliveCheckTick = g_CurrRealTime + 60;
	pPlayer->aliveCnt = 0;

	SendNpcInfo( pPlayer, mapNum );

#ifdef _dUMENOKOJI_QUEST
	SendQuestInfo( pPlayer, mapNum );
#endif
	

	pPlayer->ch.mapNum = mapNum;
	pPlayer->ch.posX = posX;
	pPlayer->ch.posY = posY;
	pPlayer->ch.posMapKind = posMapKind;

	//
	WAR_SendWarCampInfo( pPlayer );
	WAR_SendWarInfo( pPlayer, mapNum );

	int destX = ( pPlayer->ch.posX / dONE_TILE ), destY = ( pPlayer->ch.posY / dONE_TILE );

	// Area List
	INSERT_TO_LIST( gWORLD[mapNum]->player[destY][destX], pPlayer, w_prev, w_next );
	
	// Chat List
	INSERT_TO_LIST( gWORLD[mapNum]->pChatList, pPlayer, c_prev, c_next );

	gWORLD[mapNum]->currPlayerCnt++;

	MakeInPacket( pPlayer, 1 );
	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOOTHER );

	GetCurrAreaPlayer( pPlayer );
	// 몬스터
	GetCurrAreaMob( pPlayer );
	GetCurrAreaItem( pPlayer );

	// 시간 보내기
	SendWorldTime( pPlayer );

	SendPlayerMainSkill( pPlayer );
	SendPlayerAttackRange( pPlayer );
	MAGIC_SendMagicInfo( pPlayer );

	pPlayer->ch2.endMapLoading = 1;

	gWORLD[pPlayer->ch.mapNum]->playerMoveAttr[pPlayer->ch.posY][pPlayer->ch.posX] = 1;

	// 용병 정보
	if( GET_MERC_TYPE( pPlayer ) > 0 )
		MERC_SendAddMerc( pPlayer );

	if( GET_GUILD( pPlayer ) )
		GUILD_SendGuildInfo( pPlayer, dSENDMODE_TOOWNER );

#ifndef dNOT_USE_PET
	if( GET_ALL_SKILL( pPlayer ) > 10 && !GET_PET_TYPE( pPlayer ) )
		PET_SupplyPet( pPlayer );	
	else
	{
		PET_SendPetInfo( pPlayer );
		PET_UpdatePetGage( pPlayer );
		
		if( IS_USING_PET( pPlayer ) )
			PET_SendPetOn( pPlayer );
	}
#endif

	// 추방당한 멤버의 경우
	if( pPlayer->ch2.btGuildKickOut )
	{
		SendSystemMsg( pPlayer, g_LANG_STR[169] );
		pPlayer->ch2.btGuildKickOut = 0;
	}

	// 길드공지 전송
	if( pPlayer->ch2.btFirstLoad )
	{
		if( GET_GUILD( pPlayer ) )
		{
			sPGUILD_DATA pGuild = GET_GUILD_DATA( GET_GUILD( pPlayer ) );

			if( pGuild )
			{			
				if( *pGuild->notice != NULL_STR )
				{
					g_nPos = 2;
					PutWord( g_Packet, dPACKET_GUILD_CHAT, g_nPos );
					PutString( g_Packet, "NOTICE", g_nPos );
					PutString( g_Packet, pGuild->notice, g_nPos );
					PutSize( g_Packet, g_nPos );
					SendData( pPlayer, g_Packet, g_nPos );
				}
			}
		}
		pPlayer->ch2.btFirstLoad = 0;
	}
	else
	{
		SendAllEffect( pPlayer );
	}

#ifdef USE_ITEM_WEIGHT
	SendWeightGageInfo( pPlayer );
#endif

#ifdef dUSE_DEAD_GHOST
	if( pPlayer->ch2.isDeadGhost )
	{
		GET_DEAD_STATE( pPlayer ) = dDEAD_STATE_GHOST;

		SetVisible( pPlayer, 1 );
		SetMovable( pPlayer, 1 );

		GET_GHOST_TIMER( pPlayer ) = g_CurrTime;

		pPlayer->ch2.isDeadGhost = 0;
	}
#endif

#ifdef USE_GUILD_WAR
	WAR_SendEnemyInfo( pPlayer ); 
#endif

	// 이름색 보내기
	SendPlayerNameColor( pPlayer );

	UpdateMoney( pPlayer );
	UpdateCash( pPlayer );
}


//
// Function Name: GetCurrAreaPlayer
// Date: 2003-03-22
// Description: 
//
void GetCurrAreaPlayer( sPDESC_DATA pPlayer )
{
	int startX = (int)(pPlayer->ch.posX/dONE_TILE);
	int startY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = startY - 1; y <= startY + 1; y++ )
		for( int x = startX - 1; x <= startX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				GetPlayerSector( pPlayer, x, y );
}

//
// Function Name: GetCurrHousePlayer
// Date: 
// Description: 
//
void GetCurrHousePlayer( sPDESC_DATA pPlayer )
{
	sPDESC_DATA player, next_player;
	
	LIST_WHILE( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player, w_next, WMgr );
	
	if( player != pPlayer )
	{
		MakeInPacket( player, 0 );
		SendData( pPlayer );
	}

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player );
}

//
// Function Name: OutCurrAreaPlayer
// Date: 2003-03-22
// Description: 
//
void OutCurrAreaPlayer( sPDESC_DATA pPlayer )
{
	int destX = ( pPlayer->ch.posX / dONE_TILE ), destY = ( pPlayer->ch.posY / dONE_TILE );

	for( int y = destY - 1; y <= destY + 1; y++ )
		for( int x = destX - 1; x <= destX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				OutPlayerSector( pPlayer, x, y );
}

//
// Function Name: OutCurrHousePlayer
// Date: 
// Description: 
//
void OutCurrHousePlayer( sPDESC_DATA pPlayer )
{
	sPDESC_DATA player, next_player;

	LIST_WHILE( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player, w_next, WMgr );

	MakeOutPacket( player, 0 );
	SendData( pPlayer );

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player );
}


//
// Function Name: PlayerFromIndoor
// Date: 
// Description: 
//
void PlayerFromIndoor( sPDESC_DATA pPlayer )
{
	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING &&
		GET_STATE( pPlayer ) != dDESC_STATE_FIGHT )
		return;

	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR )
	{
		log( "PlayerFromIndoor : GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR [%s/%d]\r\n", 
			GET_ID( pPlayer ), GET_CHAR_POS( pPlayer ) );
		return;
	}
	
	DATASERV_SendUpdateMapInfo( pPlayer );

	OutCurrHouseItem( pPlayer );
	OutCurrHouseMob( pPlayer );
	OutCurrHousePlayer( pPlayer );

	MakeOutPacket( pPlayer, 1 );
	SendToHouse( pPlayer, g_Packet, g_nPos, dSENDMODE_TOOTHER );

	REMOVE_FROM_LIST( gHOUSE[GET_MAP_NUM(pPlayer)]->in_player, pPlayer, w_prev, w_next );
	gHOUSE[GET_MAP_NUM(pPlayer)]->currPlayerCnt--;
}

//
// Function Name: PlayerFromMap
// Date: 2003-02-12
// Description: 
//
void PlayerFromMap( sPDESC_DATA pPlayer )
{
	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING &&
		GET_STATE( pPlayer ) != dDESC_STATE_FIGHT )
		return;

	// 스킬 효과 아이콘 정리
	ClearPlayerSkillEffect( pPlayer );

	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR )
	{
		sPWORLD_PARTY_BOARD pArticle = FindPartyBoardArticle( pPlayer );
		
		if( pArticle )
		{
			REMOVE_FROM_LIST( gWORLD[GET_MAP_NUM( pPlayer )]->partyBoard, pArticle, prev, next );
			INSERT_TO_LIST( gPARTY_BOARD_MEMORY, pArticle, prev, next );
		}
	}

	// 싸움중인 몹이 있을 경우
	if( pPlayer->enemyMob )
		stop_fighting_player( pPlayer );

	// PVP 일 경우 
	if( pPlayer->victPlayer )
		pPlayer->victPlayer = NULL;
	
	stop_fighting_enemy( pPlayer );

	// 거래 중이면
	if( pPlayer->ch2.dealVict )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_DEAL_FINISH, g_nPos );
		PutSize( g_Packet, g_nPos );
		SendData( pPlayer->ch2.dealVict );
		SendData( pPlayer );
		InitDealData( pPlayer->ch2.dealVict );
		InitDealData( pPlayer );
	}

	// 개인상점 초기화 
	ClearMyShop( pPlayer );

	DATASERV_SendUpdateMapInfo( pPlayer );

	OutCurrAreaItem( pPlayer );
	OutCurrAreaMob( pPlayer );
	OutCurrAreaPlayer( pPlayer );

	// 용병 
	if( GET_MERC_TYPE( pPlayer ) > 0 )
		MERC_SendRemoveMerc( pPlayer );

	int destX = ( pPlayer->ch.posX / dONE_TILE ), destY = ( pPlayer->ch.posY / dONE_TILE );

	MakeOutPacket( pPlayer, 1 );
	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOOTHER );		

	// Chat List
	REMOVE_FROM_LIST( gWORLD[GET_MAP_NUM( pPlayer )]->pChatList, pPlayer, c_prev, c_next );

	// Area List
	REMOVE_FROM_LIST( gWORLD[pPlayer->ch.mapNum]->player[destY][destX], pPlayer, w_prev, w_next );
	gWORLD[pPlayer->ch.mapNum]->currPlayerCnt--;

	gWORLD[pPlayer->ch.mapNum]->playerMoveAttr[pPlayer->ch.posY][pPlayer->ch.posX] = 0;

}



//
// Function Name: CheckLoadingPoint
// Date: 2003-05-08
// Description: 
//
void CheckLoadingPoint( sPDESC_DATA pPlayer )
{
	if( !IS_VALID_WORLD( GET_MAP_NUM( pPlayer ) ) )
		return;

	if( gWORLD[GET_MAP_NUM( pPlayer )]->objectMap[GET_POSY( pPlayer )][GET_POSX( pPlayer )] !=
		dWORLD_OBJ_LOADPOINT )
	{	
		pPlayer->ch2.endMapLoading = 0;
		return;
	}

	sLOAD_POINT loadPoint;

	
#ifdef _DUMMY_MODE_
	if( !pPlayer->isDummy )
	{
#endif
		if( IsLoadPoint( &loadPoint, pPlayer->ch.mapNum, pPlayer->ch.posX, pPlayer->ch.posY ) )
		{
			if( pPlayer->ch2.endMapLoading )
				return;
		
			if( !IS_VALID_WORLD( loadPoint.mapNum ) )
			{
				log( "CheckLoadingPoint : IS_VALID_WORLD [%s/%s]\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );
				return;
			}

			// Local play: this portal has no character-level gate.

			// Local play: this portal has no character-level gate.

			// Local play: this portal has no character-level gate.

			// 고스맵 30레벨 제한 Gourse map level 75 limit
			// Local play: this portal has no character-level gate.

			if( loadPoint.mapNum == 20 )
			{
				if( GET_RIDE_STATE( pPlayer ) )
				{
					SendSystemMsg( pPlayer, g_LANG_STR[233] );
					return;
				}
			}

			if( GET_MAP_KIND( pPlayer ) == dMAP_KIND_INDOOR )
				PlayerFromIndoor( pPlayer );
			else
				PlayerFromMap( pPlayer );

			// 거래맵일 경우 
			if( GET_MAP_NUM( pPlayer ) == dDEAL_MAP_NUM )
			{
				if( !IS_VALID_WORLD( pPlayer->ch.prevMapNum ) )
					PlayerToHomeTown( pPlayer );
				else
				{
#ifdef dDAUM_BILLING
					if( pPlayer->ch.prevMapNum == dPRIMIUM_ZONE1 ||
							pPlayer->ch.prevMapNum == dPRIMIUM_ZONE2 ||
							pPlayer->ch.prevMapNum == dPRIMIUM_ZONE3 )
					{
						pPlayer->ch2.gotoMapNum = 3;
						pPlayer->ch2.gotoPosX = 250;
						pPlayer->ch2.gotoPosY = 250;
						pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
					}
					else
					{
#endif
						pPlayer->ch2.gotoMapNum = pPlayer->ch.prevMapNum;
						pPlayer->ch2.gotoPosX = pPlayer->ch.prevPosX;
						pPlayer->ch2.gotoPosY = pPlayer->ch.prevPosY;
						pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
#ifdef dDAUM_BILLING
					}
#endif
				}
			}
			else // 아닐 경우 if not.
			{
				pPlayer->ch2.gotoMapNum = loadPoint.mapNum;
				pPlayer->ch2.gotoPosX = loadPoint.posX;
				pPlayer->ch2.gotoPosY = loadPoint.posY;
				pPlayer->ch2.gotoPosMapKind = loadPoint.posMapKind;
			}
			SendMapLoading( pPlayer );
		}
		else
			pPlayer->ch2.endMapLoading = 0;
#ifdef _DUMMY_MODE_
	}
#endif
}

//
// Function Name: SendPlayerHP
// Date: 
// Description: 
//
void SendPlayerHP( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_HPGAIN_USER, g_nPos );
	PutInteger( g_Packet, GET_HP( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: SendPlayerMaxHPMP
// Date: 
// Description: 
//
void SendPlayerMaxHPMP( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_MAXHPMP, g_nPos );
	PutInteger( g_Packet, GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ), g_nPos );
	PutInteger( g_Packet, GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: SendPlayerMP
// Date: 
// Description: 
//
void SendPlayerMP( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MPGAIN_USER, g_nPos );
	PutInteger( g_Packet, GET_MP( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: PlayerPointGain
// Date: 2003-02-15
// Description: 플레이어 체력, 마나력 등 회복
//
void PlayerPointGain( sPDESC_DATA pPlayer )
{
	BYTE isGain = 0;
	int pluseValue;

	if( GET_DEAD_STATE( pPlayer ) != dDEAD_STATE_NONE ||
		( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING && GET_STATE( pPlayer ) != dDESC_STATE_FIGHT ) )
		return;
	
	///////////////////////////////////////
	// HP GAIN
	///////////////////////////////////////
	if( GET_HP( pPlayer ) < ( GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) ) )
	{
		if( CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_LIONSENDURANCE ) )
			pluseValue = (int)( GET_MAX_HP( pPlayer ) * 0.03 ) + number( 1, 3 );
		else
			pluseValue = (int)( GET_MAX_HP( pPlayer ) * 0.01 ) + number( 1, 3 );

		// 리제너레이션
		if( IsHaveSkillEffect( pPlayer, dSPE_SKILL_REGENERATION ) )
			pluseValue += (int)( pluseValue * 2 );
		
		GET_HP( pPlayer ) = MAX( GET_HP( pPlayer ) + pluseValue, GET_MAX_HP( pPlayer ) + GET_HPADD( pPlayer ) );

		SendPlayerHP( pPlayer );
		
		isGain = 1;
	}
	else
	{
		SendPlayerHP( pPlayer );
	}

	
	////////////////////////////////////////
	// MP GAIN
	////////////////////////////////////////
	if( GET_MP( pPlayer ) < ( GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) ) )
	{
		if( CheckEquipItem( pPlayer, dEQUIP_NECK, dAMULET_ARCANEMAGIC ) )
			pluseValue = (int)( GET_MAX_MP( pPlayer ) * 0.03 ) + number( 1, 3 );					
		else
			pluseValue = (int)( GET_MAX_MP( pPlayer ) * 0.01 ) + number( 1, 3 );					

		// 리쥬버네이션
		if( IsHaveSkillEffect( pPlayer, dSPE_SKILL_REJYUVERNATION ) )
			pluseValue += (int)( pluseValue * 1.5 );

		GET_MP( pPlayer ) = MAX( GET_MP( pPlayer ) + pluseValue, GET_MAX_MP( pPlayer ) + GET_MPADD( pPlayer ) );
	
		SendPlayerMP( pPlayer );
		
		isGain = 1;
	}
	else
	{
		SendPlayerMP( pPlayer );
	}

	if( isGain && GET_PARTY_MASTER( pPlayer ) )
		PARTY_SendHPMP( pPlayer );
}



//
// Function Name: FindWaitAllPlayer
// Date: 2003-03-20
// Description: 
//
sPDESC_DATA FindWaitAllPlayer( char *id )
{
	sPLIST_BLOCK block_list, block, block_next;
	sPDESC_DATA tmpPlayer;

	block_list = gPLAYERS.WaitAllData.GetFirstBlock();

	LIST_WHILE( block_list, block, block_next, next, WMgr );	
	tmpPlayer = (sPDESC_DATA)block->data;
	
	if( !strcmp( tmpPlayer->ch.id, id ) )
		return tmpPlayer;
	LIST_WHILEEND( block_list, block, block_next );

	return NULL;
}


//
// Function Name: FindWaitCharPlayer
// Date: 2003-03-20
// Description: 
//
sPDESC_DATA FindWaitCharPlayer( char *id )
{
	sPLIST_BLOCK block_list,block,block_next;
	sPDESC_DATA tmpPlayer;

	block_list = gPLAYERS.WaitCharData.GetFirstBlock();
	LIST_WHILE( block_list, block, block_next , next, WMgr );
	tmpPlayer = (sPDESC_DATA)block->data;
	
	if( !strcmp( tmpPlayer->ch.id, id ) )
		return tmpPlayer;
	LIST_WHILEEND( block_list, block, block_next );

	return NULL;
}


//
// Function Name: FindWaitDelPlayer
// Date: 2003-03-20
// Description: 
//
sPDESC_DATA FindWaitDelPlayer( char *id )
{
	sPLIST_BLOCK block_list, block, block_next;
	sPDESC_DATA tmpPlayer;

	block_list = gPLAYERS.WaitDelData.GetFirstBlock();
	LIST_WHILE( block_list, block, block_next , next, WMgr );
	tmpPlayer = (sPDESC_DATA)block->data;
	
	if( !strcmp( tmpPlayer->ch.id, id ) )
		return tmpPlayer;
	LIST_WHILEEND( block_list, block, block_next );
	
	return NULL;
}

//
// Function Name: GetPlayerItemDepense
// Date: 2003-03-20
// Description: 
//
BOOL	g_tDamage = FALSE, g_tMDamage = FALSE ;
#ifdef IS_TEST_SERVE
int GetPlayerItemDepense( sPDESC_DATA pPlayer )
{/*
#define GET_ARMOR_POINT( equPos )	 \
	{\
	if( pPlayer->equip[equPos] )			\
	{\
		if( btClass != GET_ITEM_NEEDCLASS( pPlayer->equip[equPos] ) || btClass == dCLASS_TYPE_ANY )\
			//IRPoint += (int)( GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] ) * 0.5 );		원본 --방어력 50% 하락	20050218
			//수정 방어력 5%상승	20050218
			IRPoint += GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] );\		
			//IRPoint += (int)( GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] ) * 0.05 );\
		
		 else\
		{\
			IRPoint += GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] );\
			//수정된 부분
			IRPoint += (int)( GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] ) * 0.05 );\
	}}}	

	int IRPoint = 0;
	BYTE btClass = GetPlayerClass( pPlayer );
#ifdef IS_TEST_SERVER	
	GET_ARMOR_POINT( dEQUIP_JACKET );		//상의
	GET_ARMOR_POINT( dEQUIP_PANTS );		//바지
	GET_ARMOR_POINT( dEQUIP_SHOES );		//신발
	GET_ARMOR_POINT( dEQUIP_GLOVE );		//장갑	
#else
	GET_ARMOR_POINT( dEQUIP_NECK );			// 목걸이
	GET_ARMOR_POINT( dEQUIP_GLOVE );		// 장갑 
	GET_ARMOR_POINT( dEQUIP_HELMET );		// 투구 
	GET_ARMOR_POINT( dEQUIP_JACKET );		// 상의
	GET_ARMOR_POINT( dEQUIP_BELT );			// 허리띠 
	GET_ARMOR_POINT( dEQUIP_PANTS );		// 바지
	GET_ARMOR_POINT( dEQUIP_EARRING );		// 귀걸이 
	GET_ARMOR_POINT( dEQUIP_RING );			// 반지
#endif*/

	if( IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
	{
		if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM )
		{
			if( !pPlayer->equip[dEQUIP_WEAPON] )
			{
				GET_ARMOR_POINT( dEQUIP_SHIELD );
			}
			else
			{	
				if(	GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDSWORD &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALSWORD &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDAXE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALAXE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDBLUNT &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALBLUNT &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DONPA &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_SPHERE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_BONG &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
				{
					GET_ARMOR_POINT( dEQUIP_SHIELD );
				}
			}
		}
	}
	GET_ARMOR_POINT( dEQUIP_SHOES );

	// 스킬에 따른 방어력
	IRPoint += g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].SAttackArmor;
	IRPoint += g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].LAttackArmor;
	IRPoint += g_DAMAGE_TABLE[GET_BMAGIC( pPlayer )].BMagicArmor;
	IRPoint += g_DAMAGE_TABLE[GET_WMAGIC( pPlayer )].WMagicArmor;
	

	return IRPoint;
}
#else
int GetPlayerItemDepense( sPDESC_DATA pPlayer )
{
#define GET_ARMOR_POINT( equPos, num )\
	{\
	if( pPlayer->equip[equPos] )\
	{\
	if( btClass != GET_ITEM_NEEDCLASS( pPlayer->equip[equPos] ) || btClass == dCLASS_TYPE_ANY )\
	{\
		bCheck[num] = FALSE;\
	}\
	else\
	{\
		bCheck[num] = TRUE;\
}}}

	int IRPoint = 0;
	BOOL	bCheck[4];
	BYTE	btClass = GetPlayerClass( pPlayer );

	GET_ARMOR_POINT( dEQUIP_JACKET, 0 );		//상의
	GET_ARMOR_POINT( dEQUIP_PANTS, 1 );		//바지
	GET_ARMOR_POINT( dEQUIP_SHOES, 2 );		//신발
	GET_ARMOR_POINT( dEQUIP_GLOVE, 3 );		//장갑	
	
	if( bCheck[0] == TRUE && bCheck[1] == TRUE && bCheck[2] == TRUE && bCheck[3] == TRUE )
	{
		g_tDamage = TRUE;
	}
	
	IRPoint += pPlayer->equip[dEQUIP_NECK] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_NECK] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_GLOVE] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_GLOVE] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_HELMET] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_HELMET] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_JACKET] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_JACKET] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_BELT] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_BELT] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_PANTS] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_PANTS] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_EARRING] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_EARRING] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_RING] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_RING] ) : 0;

	if( IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
	{
		if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM )
		{
			if( !pPlayer->equip[dEQUIP_WEAPON] )
			{
				IRPoint += pPlayer->equip[dEQUIP_SHIELD] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_SHIELD] ) : 0;
			}
			else
			{	
				if(	GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDSWORD &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALSWORD &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDAXE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALAXE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDBLUNT &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALBLUNT &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DONPA &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_SPHERE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_BONG &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
				{
					IRPoint += pPlayer->equip[dEQUIP_SHIELD] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_SHIELD] ) : 0;
				}
			}
		}
	}
	IRPoint += pPlayer->equip[dEQUIP_SHOES] ? GET_ITEM_MINDAMAGE( pPlayer->equip[dEQUIP_SHOES] ) : 0;
	
	// 스킬에 따른 방어력
	IRPoint += g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].SAttackArmor;
	IRPoint += g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].LAttackArmor;
	IRPoint += g_DAMAGE_TABLE[GET_BMAGIC( pPlayer )].BMagicArmor;
	IRPoint += g_DAMAGE_TABLE[GET_WMAGIC( pPlayer )].WMagicArmor;
	
	return IRPoint;
}
#endif

#ifdef _dUMENOKOJI_
//
// Function Name: GetPlayerItemMagicDepense
// Date: 2005-01-20
// Description: 마법 방어력 얻어오기
//
#ifdef IS_TEST_SERVE
int	GetPlayerItemMagicDepense( sPDESC_DATA pPlayer )
{
/*#define GET_ARMOR_POINT( equPos )	 \
{\
if( pPlayer->equip[equPos] )			\
{\
if( btClass != GET_ITEM_NEEDCLASS( pPlayer->equip[equPos] ) || btClass == dCLASS_TYPE_ANY )\
	IRPoint += GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] );\
		IRPoint += (int)( GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] ) * 0.05 );\
else\
{\
IRPoint += GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] );\
IRPoint += (int)( GET_ITEM_MINDAMAGE( pPlayer->equip[equPos] ) * 0.05 );\
}}}		//GetPlayerItemMagicDepense에서 참조 해서 사용할것....
		
int IRPoint = 0;
	BYTE btClass = GetPlayerClass( pPlayer );
	#ifdef IS_TEST_SERVER
	GET_ARMOR_POINT( dEQUIP_JACKET );		//상의
	GET_ARMOR_POINT( dEQUIP_PANTS );		//바지
	GET_ARMOR_POINT( dEQUIP_SHOES );		//신발
	GET_ARMOR_POINT( dEQUIP_GLOVE );		//장갑	
	#else
	GET_ARMOR_POINT( dEQUIP_NECK );			// 목걸이
	GET_ARMOR_POINT( dEQUIP_GLOVE );		// 장갑 
	GET_ARMOR_POINT( dEQUIP_HELMET );		// 투구 
	GET_ARMOR_POINT( dEQUIP_JACKET );		// 상의
	GET_ARMOR_POINT( dEQUIP_BELT );			// 허리띠 
	GET_ARMOR_POINT( dEQUIP_PANTS );		// 바지
	GET_ARMOR_POINT( dEQUIP_EARRING );		// 귀걸이 
	GET_ARMOR_POINT( dEQUIP_RING );			// 반지
		#endif
*/
		if( IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
		{
			if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM )
			{
				if( !pPlayer->equip[dEQUIP_WEAPON] )
				{
					GET_ARMOR_POINT( dEQUIP_SHIELD );
				}
				else
				{	
					if(	GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDSWORD &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALSWORD &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDAXE &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALAXE &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDBLUNT &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALBLUNT &&

						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DONPA &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_SPHERE &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_BONG &&
						GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
					{
						GET_ARMOR_POINT( dEQUIP_SHIELD );
					}
				}
			}
		}
		GET_ARMOR_POINT( dEQUIP_SHOES );

		// 스킬에 따른 방어력
		IRPoint += g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].SAttackArmor;
		IRPoint += g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].LAttackArmor;
		IRPoint += g_DAMAGE_TABLE[GET_BMAGIC( pPlayer )].BMagicArmor;
		IRPoint += g_DAMAGE_TABLE[GET_WMAGIC( pPlayer )].WMagicArmor;
	
		return IRPoint;
}

#else
int	GetPlayerItemMagicDepense( sPDESC_DATA pPlayer )
{
/*#define GET_ARMOR_POINT( equPos, num )\
{\
if( pPlayer->equip[equPos] )\
{\
if( btClass != GET_ITEM_NEEDCLASS( pPlayer->equip[equPos] ) || btClass == dCLASS_TYPE_ANY )\
{\
bCheck[num] = FALSE;\
}\
else\
{\
bCheck[num] = TRUE;\
}}}*/

	int	IRPoint = 0;
	BOOL	bCheck[4];
	BYTE	btClass = GetPlayerClass( pPlayer );

	GET_ARMOR_POINT( dEQUIP_JACKET, 0 );		//상의
	GET_ARMOR_POINT( dEQUIP_PANTS, 1 );		//바지
	GET_ARMOR_POINT( dEQUIP_SHOES, 2 );		//신발
	GET_ARMOR_POINT( dEQUIP_GLOVE, 3 );		//장갑	

	if( bCheck[0] == TRUE && bCheck[1] == TRUE && bCheck[2] == TRUE && bCheck[3] == TRUE )
	{
		g_tMDamage = TRUE;
	}
		
	IRPoint += pPlayer->equip[dEQUIP_NECK] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_NECK] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_GLOVE] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_GLOVE] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_HELMET] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_HELMET] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_JACKET] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_JACKET] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_BELT] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_BELT] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_PANTS] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_PANTS] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_EARRING] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_EARRING] ) : 0;
	IRPoint += pPlayer->equip[dEQUIP_RING] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_RING] ) : 0;
	
	/*if( IS_VALID_ITEM( GET_USE_SKILL( pPlayer ) ) )
	{
		if( GET_USE_SKILL( pPlayer ) == dSATTACK_SKILL_ITEM )
		{
			if( !pPlayer->equip[dEQUIP_WEAPON] )
			{
				IRPoint += pPlayer->equip[dEQUIP_SHIELD] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_SHIELD] ) : 0;
			}
			else
			{	
				if(	GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDSWORD &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALSWORD &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDAXE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALAXE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_TWOHANDBLUNT &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DUALBLUNT &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_DONPA &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_SPHERE &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_BONG &&
					GET_ITEM_TYPE( pPlayer->equip[dEQUIP_WEAPON] ) != dITEMTYPE_STAFF )
				{
					IRPoint += pPlayer->equip[dEQUIP_SHIELD] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_SHIELD] ) : 0;
				}
			}
		}
	}*/
	IRPoint += pPlayer->equip[dEQUIP_SHOES] ? GET_ITEM_MAGIC_DEFENSE( pPlayer->equip[dEQUIP_SHOES] ) : 0;
		
	// 스킬에 따른 방어력
	IRPoint += g_DAMAGE_TABLE[GET_SATTACK( pPlayer )].SAttackDefense;
	IRPoint += g_DAMAGE_TABLE[GET_LATTACK( pPlayer )].LAttackDefense;
	IRPoint += g_DAMAGE_TABLE[GET_BMAGIC( pPlayer )].BMagicDefense;
	IRPoint += g_DAMAGE_TABLE[GET_WMAGIC( pPlayer )].WMagicDefense;
	
	return IRPoint;
}
#endif	//IS_TEST_SERVER
#endif

//
// Function Name: FindPlayerNameList
// Date: 2003-03-20
// Description: 
//
sPDESC_DATA FindPlayerNameList( char *name )
{
	if( name == NULL )
		return NULL;

	int hashIdx = GetStrHashIndex( name );
	
	sPDESC_DATA d, next_d;

	//log_file( "FindPlayerNameList.txt", "[ name : %s, hashIdx : %d ]\r\n", name, hashIdx );
	
	LIST_WHILE( gPLAYERS.name_list[hashIdx], d, next_d , name_next, WMgr ); 
	if( !strcmp( name, d->ch.name ) )
		return d;
	LIST_WHILEEND( gPLAYERS.name_list[hashIdx], d, next_d );
	return NULL;
}

//
// Function Name: FindPlayerNameList
// Date: 2003-05-28
// Description: 
//
sPDESC_DATA FindPlayerIdList( char *id )
{
	int hashIdx = GetStrHashIndex( id );

	sPDESC_DATA d, next_d;
	
	LIST_WHILE( gPLAYERS.id_list[hashIdx], d, next_d , id_next, WMgr ); 
	if( !strcmp( id, d->ch.id ) )
		return d;
	LIST_WHILEEND( gPLAYERS.id_list[hashIdx], d, next_d );
	return NULL;
}



//
// Function Name: BroadCastingMsg
// Date: 2003-03-20
// Description: 
//
void BroadCastingMsg( char *str )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr );
	if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
		GET_STATE( d ) == dDESC_STATE_FIGHT )
	{
		g_nPos = 2;
		PutWord( g_Packet, dPACKET_CHAT_WHISPER, g_nPos );
		PutString( g_Packet, "Notice", g_nPos );
		PutString( g_Packet, str, g_nPos );
		PutSize( g_Packet, g_nPos );
		
		SendData( d, g_Packet, g_nPos );
	}
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
}

void BroadCastSystemMsg( char *str )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr );
	if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
		GET_STATE( d ) == dDESC_STATE_FIGHT )
	{
		SendSystemMsg( d, str );
	}
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
}

//
// Function Name: SendToAll
// Date: 2003-04-22
// Description: 
//
void SendToAll( char *data, int size )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gPLAYERS.list, d, next_d , next, WMgr );
	if( GET_STATE( d ) == dDESC_STATE_PLAYING ||
		GET_STATE( d ) == dDESC_STATE_FIGHT )
		SendData( d, data, size );
	LIST_WHILEEND( gPLAYERS.list, d, next_d );
}

//
// Function Name: UpdateMoney
// Date: 2003-05-05
// Description: 
//
void UpdateMoney( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_MONEY, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.money, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
}


//
// Function Name: UpdateMoney
// Date: 2003-05-05
// Description: 
//
void UpdateCash( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_CASH_MONEY, g_nPos );
	PutInteger( g_Packet, pPlayer->ch.cash, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: FindPlayerInTile
// Date: 2003-05-10
// Description: 
//
sPDESC_DATA FindPlayerInTile( sPDESC_DATA list, int idxNum )
{
	sPDESC_DATA d, next_d;
	
	LIST_WHILE( list, d, next_d , w_next, WMgr );
	if( d->sock == idxNum )
		return d;
	LIST_WHILEEND( list, d, next_d );

	return NULL;
}

//
// Function Name: FindPlayerInArea
// Date: 2003-05-10
// Description: 
//
sPDESC_DATA FindPlayerInArea( int mapNum, int posX, int posY, int idxNum )
{
	if( !IS_VALID_WORLD( mapNum ) )
		return NULL;

	sPDESC_DATA d;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				d = FindPlayerInTile( gWORLD[mapNum]->player[y][x], idxNum );

				if( d )
					return d;
			}
		}
	}
	return NULL;
}

//
// Function Name: FindPlayerInHouse
// Date:
// Description: 
//
sPDESC_DATA FindPlayerInHouse( sPDESC_DATA pPlayer , int idxNum )
{
	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_INDOOR )
		return NULL;

	if( !IS_VALID_HOUSE( GET_MAP_NUM( pPlayer ) ) )
		return NULL;

	sPDESC_DATA player, next_player;

	LIST_WHILE( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player, w_next, WMgr );

	if( GET_IDX( player ) == idxNum )
		return player;

	LIST_WHILEEND( gHOUSE[GET_MAP_NUM( pPlayer )]->in_player, player, next_player );
	return NULL;
}


//
// Function Name: SendPlayerMainSkill
// Date: 
// Description: 
//
void SendPlayerMainSkill( sPDESC_DATA pPlayer )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_CHANGE_MAIN_SKILL, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, !GET_USE_SKILL( pPlayer ) ? dSATTACK_SKILL_ITEM : GET_USE_SKILL( pPlayer ) , g_nPos );
	PutSize( g_Packet, g_nPos );
	
	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOALL );	
}

//
// Function Name: SendPlayerAttackRange
// Date: 2003-05-10
// Description: 
//
void SendPlayerAttackRange( sPDESC_DATA pPlayer )
{
	BYTE range = GetAttackRange( pPlayer );

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SET_ATTACKRANGE, g_nPos );
	PutByte( g_Packet, range, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer );
}


//
// Function Name: SendMapLoading
// Date: 2003-05-10
// Description: 
//
void SendMapLoading( sPDESC_DATA pPlayer , BYTE btLayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_MAPLOADING, g_nPos );
	if( pPlayer->ch2.gotoPosMapKind == dMAP_KIND_INDOOR )
		PutWord( g_Packet, gHOUSE[pPlayer->ch2.gotoMapNum]->houseType, g_nPos );
	else
		PutWord( g_Packet, pPlayer->ch2.gotoMapNum, g_nPos );
	PutInteger( g_Packet, pPlayer->ch2.gotoPosX, g_nPos );
	PutInteger( g_Packet, pPlayer->ch2.gotoPosY, g_nPos );
	PutByte( g_Packet, btLayer, g_nPos );
	PutByte( g_Packet, pPlayer->ch2.gotoPosMapKind, g_nPos );
	
	// 전쟁 정보
	sPWORLD pWorld = gWORLD[pPlayer->ch2.gotoMapNum];

	if( !pWorld )
	{
		PutByte( g_Packet, 0, g_nPos );
		PutInteger( g_Packet, 0, g_nPos );
		PutInteger( g_Packet, 0, g_nPos );
	}
	else
	{
		if( !pWorld->underAttack )
		{
			PutByte( g_Packet, 0, g_nPos );
			PutInteger( g_Packet, 0, g_nPos );
			PutInteger( g_Packet, 0, g_nPos );
		}
		else
		{
			PutByte( g_Packet, 1, g_nPos );
			PutInteger( g_Packet, pWorld->warCristal.allowGuild[0], g_nPos );
			PutInteger( g_Packet, pWorld->warCristal.allowGuild[1], g_nPos );
		}
	}

	PutSize( g_Packet, g_nPos );
	
	SendData( pPlayer, g_Packet, g_nPos );	

	GET_STATE( pPlayer ) = dDESC_STATE_LOADING;

}



//
// Function Name: SendRideInfo
// Date: 
// Description: 
//
void SendRideInfo( sPDESC_DATA pPlayer )
{
	g_nPos = 2;

	PutWord( g_Packet, dPACKET_RIDE_INFO, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, GET_RIDE_STATE( pPlayer ) ? GET_RIDE_TYPE( pPlayer ) : 0, g_nPos );
	PutByte( g_Packet, GetRideSpeed( pPlayer ) , g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}

//
// Function Name: GetRideSpeed
// Date: 
// Description: 
//
BYTE GetRideSpeed( sPDESC_DATA pPlayer )
{
	BYTE speed = dDEF_PLAYER_SPEED;

	if( !GET_RIDE_TYPE( pPlayer ) || !GET_RIDE_STATE( pPlayer ) )
		speed = dDEF_PLAYER_SPEED;
	else
	{
		switch( GET_RIDE_TYPE( pPlayer ) )
		{
		case 1:					// 갈색말 Brown horse
			speed = 70;
			break;
		case 2:					// 검정말 Black Horse
			speed = 75;
			break;
		case 3:					// 하양말 White Horse
			speed = 80;
			break;
		case dRIDE_TYPE4:
			speed = 100;
			break;
		}
	}

	// 기동성의 물약 1미터 증가
#ifdef dDAUM_BILLING	
	if( !GET_RIDE_TYPE( pPlayer ) && pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT8] )
		speed += 15;
#endif
	
	return speed;
}

//
// Function Name: PlayerToHomeTown
// Date: 
// Description: 
//
void PlayerToHomeTown( sPDESC_DATA pPlayer )
{
	pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
	
	pPlayer->ch2.gotoMapNum = 3;
	
	pPlayer->ch2.gotoPosX = 250 + number( 0, 2 );
	pPlayer->ch2.gotoPosY = 250 + number( 0, 2 );
}

//
// Function Name: PlayerToStartPoint
// Date: 
// Description: 
//
void PlayerToStartPoint( sPDESC_DATA pPlayer, int mapNum )
{
	pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
	
	int nMapNum = 0;

	if( mapNum > 0 )
		nMapNum = mapNum;
	else
		nMapNum = GET_MAP_NUM( pPlayer );

	switch( nMapNum )
	{
	case 3:
		pPlayer->ch2.gotoMapNum = 3;//베일
		
		pPlayer->ch2.gotoPosX = 248 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 264 + number( 0, 2 );
		break;
	case 4:
	case 5:
		pPlayer->ch2.gotoMapNum = 4;
		
		pPlayer->ch2.gotoPosX = 364 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 95 + number( 0, 2 );
		break;
	case 7:
	case 6:
		pPlayer->ch2.gotoMapNum = 6;
		
		pPlayer->ch2.gotoPosX = 315 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 195 + number( 0, 2 );
		break;
	case 8:
	case 9:
	case 10:
	case 1003:
		pPlayer->ch2.gotoMapNum = 8;
		
		pPlayer->ch2.gotoPosX = 392 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 368 + number( 0, 2 );
		break;		
	case 11:
	case 12:
	case 13:
		pPlayer->ch2.gotoMapNum = 11;
		
		pPlayer->ch2.gotoPosX = 148 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 92 + number( 0, 2 );
		break;
	case 14:
		pPlayer->ch2.gotoMapNum = 14;
		
		pPlayer->ch2.gotoPosX = 381 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 333 + number( 0, 2 );
		break;
	case 15:
		pPlayer->ch2.gotoMapNum = 15;
		
		pPlayer->ch2.gotoPosX = 312 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 344 + number( 0, 2 );
		break;
	case 16:
	case 17:
		pPlayer->ch2.gotoMapNum = 17;
		
		pPlayer->ch2.gotoPosX = 311 + number( 0, 2 );
		pPlayer->ch2.gotoPosY = 84 + number( 0, 2 );
		break;
	case 22:
		pPlayer->ch2.gotoMapNum = 22;
		
		pPlayer->ch2.gotoPosX = 284 + number( 0, 0 );
		pPlayer->ch2.gotoPosY = 393 + number( 0, 0 );
		break;
	case 40:
		pPlayer->ch2.gotoMapNum = 40;
		
		pPlayer->ch2.gotoPosX = 303 + number( 0, 0 );
		pPlayer->ch2.gotoPosY = 259 + number( 0, 0 );
		break;

#ifdef dDAUM_BILLING
	case 31:
	case 32:
	case 33:
		pPlayer->ch2.gotoMapNum = 31;
		
		pPlayer->ch2.gotoPosX = 108;
		pPlayer->ch2.gotoPosY = 142;
		break;
#endif

	default:
		PlayerToHomeTown( pPlayer );
		break;
	}
}

int GetPlayerMainMap( sPDESC_DATA pPlayer )
{
 
	
	int nMapNum = 0;
	nMapNum = GET_MAP_NUM( pPlayer );

	switch( nMapNum )
	{
	case 3:
		return 3;
		break;
	case 4:
	case 5:
	case 6:
		return 6;
		break;
	case 7:
	case 8:
	case 1003:
		return 8;
		break;
	case 9:
	case 10:
	case 11:
		return 11;
		break;
	case 12:
	case 13:
	case 14:
		return 14;
		break;
	case 15:
		return 15;
		break;
	case 16:
	case 17:
		return 17;
		break;

#ifdef dDAUM_BILLING
	case 31:
	case 32:
	case 33:
		return 31;
		break;
#endif

	default:
		return 3;
		break;
	}
		return 3;
}

//
// Function Name: GetStartPoint
// Date:
// Description: 
//
void GetStartPoint( sPDESC_DATA pPlayer )
{
	GET_MAP_NUM( pPlayer ) = 3;
	
#ifdef __CHINA_VER__
	
	int randem = number( 0, 2 );
	
	switch( randem ) {
	case 0:
		pPlayer->ch.posX = 297;
		pPlayer->ch.posY = 215;
		break;
	case 1:
		pPlayer->ch.posX = 253;
		pPlayer->ch.posY = 207;
		break;
	case 2:
		pPlayer->ch.posX = 195;
		pPlayer->ch.posY = 240;
		break;
	default:
		pPlayer->ch.posX = 250 + number( 0, 2 );
		pPlayer->ch.posY = 250 + number( 0, 2 );
		break;
		
	}
	
#else
	pPlayer->ch.posX = 250 + number( 0, 2 );
	pPlayer->ch.posY = 250 + number( 0, 2 );
#endif
}



//
// Function Name: NewCharacterProc
// Date:
// Description: 
//
void NewCharacterProc( sPDESC_DATA pPlayer ) 
{	
	int i = 0;	

	GET_STR_EXP( pPlayer ) = 0;
	GET_INT_EXP( pPlayer ) = 0;
	GET_DEX_EXP( pPlayer ) = 0;
	
	GET_SATTACK( pPlayer ) = 0;
	GET_LATTACK( pPlayer ) = 0;
	GET_BMAGIC( pPlayer ) = 0;
	GET_WMAGIC( pPlayer ) = 0;
	
	GET_SATTACK_EXP( pPlayer ) = 0;
	GET_LATTACK_EXP( pPlayer ) = 0;
	GET_BMAGIC_EXP( pPlayer ) = 0;
	GET_WMAGIC_EXP( pPlayer ) = 0;
	
	pPlayer->isNewCharacter = 1;
	
	GetStartPoint( pPlayer );
	
	pPlayer->ch.posMapKind = dMAP_KIND_FIELD;
	
	pPlayer->ch2.gotoMapNum = pPlayer->ch.mapNum;
	pPlayer->ch2.gotoPosX = pPlayer->ch.posX;
	pPlayer->ch2.gotoPosY = pPlayer->ch.posY;
	pPlayer->ch2.gotoPosMapKind = pPlayer->ch.posMapKind;
	
	// Stat 지정 
	GET_STR( pPlayer ) = g_PLAYER_ABL_TABLE[0][dCLASS_NONE];
	GET_INT( pPlayer ) = g_PLAYER_ABL_TABLE[1][dCLASS_NONE];
	GET_DEX( pPlayer ) = g_PLAYER_ABL_TABLE[2][dCLASS_NONE];
	
	// 최대 체력 구하기 
	GET_MAX_HP( pPlayer ) = g_PLAYER_ABL_TABLE[4][dCLASS_NONE];
	
	for( i = 0; i < GET_STRADD( pPlayer ); i++ )
		GET_MAX_HP( pPlayer ) += g_STATE_MPHP_TABLE[0].minStr;

	for( i = 0; i < GET_INTADD( pPlayer ); i++ )
		GET_MAX_HP( pPlayer ) += g_STATE_MPHP_TABLE[0].minInt;

	for( i = 0; i < GET_DEXADD( pPlayer ); i++ )
		GET_MAX_HP( pPlayer ) += g_STATE_MPHP_TABLE[0].minDex;
	
	GET_HP( pPlayer ) = GET_MAX_HP( pPlayer );
	
	GET_MAX_MP( pPlayer ) = g_PLAYER_ABL_TABLE[5][dCLASS_NONE];
	
	for( i = 0; i < GET_STRADD( pPlayer ); i++ )
		GET_MAX_MP( pPlayer ) += g_STATE_MPHP_TABLE[1].minStr;

	for( i = 0; i < GET_INTADD( pPlayer ); i++ )
		GET_MAX_MP( pPlayer ) += g_STATE_MPHP_TABLE[1].minInt;

	for( i = 0; i < GET_DEXADD( pPlayer ); i++ )
		GET_MAX_MP( pPlayer ) += g_STATE_MPHP_TABLE[1].minDex;
	
	GET_MP( pPlayer ) = GET_MAX_MP( pPlayer );
		
	DATASERV_SendUpdateStr( pPlayer );
	pPlayer->isUpdate[dDATA_UPDATE_STREXP] = 1;
	DATASERV_SendUpdateInt( pPlayer );
	pPlayer->isUpdate[dDATA_UPDATE_INTEXP] = 1;
	DATASERV_SendUpdateDex( pPlayer );
	pPlayer->isUpdate[dDATA_UPDATE_DEXEXP] = 1;
	
	DATASERV_SendUpdateSkill( pPlayer, 0 );
	DATASERV_SendUpdateSkill( pPlayer, 1 );
	DATASERV_SendUpdateSkill( pPlayer, 2 );
	DATASERV_SendUpdateSkill( pPlayer, 3 );
	
	pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP1] = 1;
	pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP2] = 1;
	pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP3] = 1;
	pPlayer->isUpdate[dDATA_UPDATE_SKILLEXP4] = 1;
	
	DATASERV_SendUpdateHp( pPlayer );
	DATASERV_SendUpdateMp( pPlayer );
	DATASERV_SendUpdateMapInfo( pPlayer );
	
	////////////////////////////////////////////////////////////
	// 기본 아이템 지급
	////////////////////////////////////////////////////////////
#ifdef dUSE_DIRECT_ITEM_DB_ACCESS
	ITEMDB_DeleteAllEquip( pPlayer );
	ITEMDB_DeleteAllInven( pPlayer );
	ITEMDB_DelAllSkills( pPlayer );
	FRIEND_DeleteAllFriend( pPlayer );
#ifdef dDAUM_BILLING
	DeleteBillingItemInfo( pPlayer );
#endif
#ifndef dNOT_USE_PET
	PET_DeletePetData( pPlayer );
#endif

	// 상의
	if( pPlayer->ch.chEquip[dCH_EQ_JACKET] )
		ITEMDB_ToEquip2( GET_ID( pPlayer ), GET_CHAR_POS( pPlayer ), pPlayer->ch.chEquip[dCH_EQ_JACKET], dEQUIP_JACKET );
	
	// 하의 
	if( pPlayer->ch.chEquip[dCH_EQ_PANTS] )
		ITEMDB_ToEquip2( GET_ID( pPlayer ), GET_CHAR_POS( pPlayer ), pPlayer->ch.chEquip[dCH_EQ_PANTS], dEQUIP_PANTS );
	
	// 장갑
	if( pPlayer->ch.chEquip[dCH_EQ_HAND] )
		ITEMDB_ToEquip2( GET_ID( pPlayer ), GET_CHAR_POS( pPlayer ), pPlayer->ch.chEquip[dCH_EQ_HAND], dEQUIP_GLOVE );
	
	// 신발
	if( pPlayer->ch.chEquip[dCH_EQ_FOOT] )
		ITEMDB_ToEquip2( GET_ID( pPlayer ), GET_CHAR_POS( pPlayer ), pPlayer->ch.chEquip[dCH_EQ_FOOT], dEQUIP_SHOES );

	// 근거리 기본 무기 
	if( g_PLAYER_ABL_TABLE[6][dCLASS_NONE] != 0 )
		ITEMDB_ToEquip2( GET_ID( pPlayer ), pPlayer->ch.charPos, g_PLAYER_ABL_TABLE[6][dCLASS_NONE], dEQUIP_WEAPON );
	if( g_PLAYER_ABL_TABLE[7][dCLASS_NONE] != 0 )
		ITEMDB_ToEquip2( GET_ID( pPlayer ), pPlayer->ch.charPos, g_PLAYER_ABL_TABLE[7][dCLASS_NONE], dEQUIP_BOW );

	// 기본 지급 스킬
	for( i = 8; i <= 12; i++ )
	{
		if( g_PLAYER_ABL_TABLE[i][dCLASS_NONE] != 0 )
		{
			AddPlayerSkillDirect( pPlayer, g_PLAYER_ABL_TABLE[i][dCLASS_NONE] );
		}
	}
	
	// 기본 장비 부여
	int equipPos;
	
	for( i = 13; i <= 17; i++ )
	{
		if( g_PLAYER_ABL_TABLE[i][dCLASS_NONE] != 0 )
		{
			equipPos = GetEquipPositionByNum( g_PLAYER_ABL_TABLE[i][dCLASS_NONE] );
			
			if( equipPos == 255 )
			{
				log( "Add Player Default Equip Error[%s][%s]\r\n", GET_ID( pPlayer ), GET_NAME( pPlayer ) );
				continue;
			}
			
			ITEMDB_ToEquip2( GET_ID( pPlayer ), pPlayer->ch.charPos, g_PLAYER_ABL_TABLE[i][dCLASS_NONE], equipPos );
		}
	}
	
	// 기본 소지품
	sCHECK_INVEN checkInven;
	
	if( g_PLAYER_ABL_TABLE[18][dCLASS_NONE] )
	{
		checkInven.page = 0;
		checkInven.x = 0;
		checkInven.y = 0;
		
		ITEMDB_ToInven2( pPlayer, g_PLAYER_ABL_TABLE[18][dCLASS_NONE], &checkInven );
	}

	if( g_PLAYER_ABL_TABLE[19][dCLASS_NONE] )
	{
		checkInven.page = 0;
		checkInven.x = 1;
		checkInven.y = 0;
		
		ITEMDB_ToInven2( pPlayer, g_PLAYER_ABL_TABLE[19][dCLASS_NONE], &checkInven );
	}

	if( g_PLAYER_ABL_TABLE[20][dCLASS_NONE] )
	{
		checkInven.page = 0;
		checkInven.x = 2;
		checkInven.y = 0;
		
		ITEMDB_ToInven2( pPlayer, g_PLAYER_ABL_TABLE[20][dCLASS_NONE], &checkInven );
	}

	if( g_PLAYER_ABL_TABLE[21][dCLASS_NONE] )
	{
		checkInven.page = 0;
		checkInven.x = 3;
		checkInven.y = 0;
		
		ITEMDB_ToInven2( pPlayer, g_PLAYER_ABL_TABLE[21][dCLASS_NONE], &checkInven );
	}

	if( g_PLAYER_ABL_TABLE[22][dCLASS_NONE] )
	{
		checkInven.page = 0;
		checkInven.x = 4;
		checkInven.y = 0;
		
		ITEMDB_ToInven2( pPlayer, g_PLAYER_ABL_TABLE[22][dCLASS_NONE], &checkInven );
	}
	// 기본 소지품 End
	
#endif
}

//
// Function Name: SendUpdateCha
// Date: 
// Description: 
//
void SendUpdateCha( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_CHA, g_nPos );
	PutInteger( g_Packet, GET_CHA( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer , g_Packet, g_nPos );

//	if( !( GET_CHA( pPlayer ) % 5000 ) )
	SendPlayerNameColor( pPlayer );
}

//
// Function Name: TogglePlayerFlag
// Date: 
// Description: 
//
void TogglePlayerFlag( sPDESC_DATA pPlayer, int itemNum )
{
	switch( itemNum )
	{
	// 친구차단
	case dDENY_ITEM_FRIEND:		
		if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_DENY_FRIEND ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[30] );
			REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_FRIEND );
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[31] );
			SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_FRIEND );
		}
		break;
	// 채팅차단
	case dDENY_ITEM_CHAT:		
		if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_DENY_CHAT ) )
		{
			SendSystemMsg( pPlayer , g_LANG_STR[32] );
			REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_CHAT );
		}
		else
		{
			SendSystemMsg( pPlayer , g_LANG_STR[33] );
			SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_CHAT );
		}
		break;
	// 귓말차단
	case dDENY_ITEM_WHISPER:	
		if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_DENY_WHISPER ) )
		{
			SendSystemMsg( pPlayer , g_LANG_STR[34] );
			REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_WHISPER );
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[35] );
			SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_WHISPER );
		}
		break;
	// 파티차단
	case dDENY_ITEM_PARTY:		
		if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_DENY_PARTY ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[36] );
			REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_PARTY );
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[37] );
			SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_PARTY );
		}
		break;
	// 거래차단
	case dDENY_ITEM_DEAL:		
		if( IS_SET( pPlayer->ch2.flag, dPLAYER_FLG_DENY_DEAL ) )
		{
			SendSystemMsg( pPlayer, g_LANG_STR[38] );
			REMOVE_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_DEAL );
		}
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[39] );
			SET_BIT( pPlayer->ch2.flag, dPLAYER_FLG_DENY_DEAL );
		}
		break;
	}
}

BYTE GetPlayerClass( sPDESC_DATA pPlayer )
{
#define CHECK_CLASS( major, other1, other2, other3 ) \
	( ( major >= other1 && major >= other2 && major >= other3 ) || \
	( major > other1 && major > other2 && major > other3 ) )

	if( GET_SATTACK( pPlayer )+1 == GET_LATTACK( pPlayer )+1 == GET_BMAGIC( pPlayer )+1 == GET_WMAGIC( pPlayer )+1 )
		return dCLASS_TYPE_ANY;

	if( CHECK_CLASS( GET_SATTACK( pPlayer ), GET_LATTACK( pPlayer ), GET_BMAGIC( pPlayer ), GET_WMAGIC( pPlayer ) ) )
		return dCLASS_TYPE_WARRIOR;
	else if( CHECK_CLASS( GET_LATTACK( pPlayer ), GET_SATTACK( pPlayer ), GET_BMAGIC( pPlayer ), GET_WMAGIC( pPlayer ) ) )
		return dCLASS_TYPE_ARCHER;
	else if( CHECK_CLASS( GET_BMAGIC( pPlayer ), GET_SATTACK( pPlayer ), GET_LATTACK( pPlayer ), GET_WMAGIC( pPlayer ) ) ||
			 CHECK_CLASS( GET_WMAGIC( pPlayer ), GET_SATTACK( pPlayer ), GET_LATTACK( pPlayer ), GET_BMAGIC( pPlayer ) ) )
		return dCLASS_TYPE_MAGICUSER;
	
	return dCLASS_TYPE_NONE;
#undef CHECK_CLASS
}

//
// Function Name: IsEquipOk
// Date: 
// Description: 
//
BOOL IsEquipOk( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( !pItem || !pPlayer )
		return 0;

	if( GET_STR_TOTAL( pPlayer ) == GET_DEX_TOTAL( pPlayer ) == GET_INT_TOTAL( pPlayer ) )
		return 1;

#ifdef _dUMENOKOJI_
	BYTE btClass = GetPlayerClass( pPlayer );

	if( GET_ITEM_NEEDCLASS( pItem ) == dCLASS_TYPE_NONE || btClass == dCLASS_TYPE_ANY )
		return 1;
	else
	{
		if( btClass == GET_ITEM_NEEDCLASS( pItem ) )
			return 1;
		else
		{
			switch( GET_ITEM_NEEDCLASS( pItem ) )
			{
			case dCLASS_TYPE_WARRIOR:
				SendSystemMsg( pPlayer, g_LANG_STR[83] );
				return 0;
			case dCLASS_TYPE_ARCHER:
				SendSystemMsg( pPlayer, g_LANG_STR[84] );
				return 0;
			case dCLASS_TYPE_MAGICUSER:
				SendSystemMsg( pPlayer, g_LANG_STR[85] );
				return 0;
			}
		}
	}
#else
	switch( GET_ITEM_NEEDCLASS( pItem ) )
	{
	// 공통 
	case dCLASS_TYPE_NONE:	
		return 1;
	// 전사계열 
	case dCLASS_TYPE_WARRIOR:
		if( ( GET_STR_TOTAL( pPlayer ) >= GET_DEX_TOTAL( pPlayer ) && GET_STR_TOTAL( pPlayer ) >= GET_INT_TOTAL( pPlayer ) ) ||
			( GET_STR_TOTAL( pPlayer ) > GET_DEX_TOTAL( pPlayer ) && GET_STR_TOTAL( pPlayer ) > GET_INT_TOTAL( pPlayer ) ) )
			return 1;
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[83] );
			return 0;
		}
		break;
	// 궁수계열 
	case dCLASS_TYPE_ARCHER:
		if( ( GET_DEX_TOTAL( pPlayer ) >= GET_STR_TOTAL( pPlayer ) && GET_DEX_TOTAL( pPlayer ) >= GET_INT_TOTAL( pPlayer ) ) || 
			( GET_DEX_TOTAL( pPlayer ) > GET_STR_TOTAL( pPlayer ) && GET_DEX_TOTAL( pPlayer ) > GET_INT_TOTAL( pPlayer ) ) )
			return 1;
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[84] );
			return 0;
		}
		break;
	// 법사계열
	case dCLASS_TYPE_MAGICUSER:
		if( ( GET_INT_TOTAL( pPlayer ) >= GET_STR_TOTAL( pPlayer ) && GET_INT_TOTAL( pPlayer ) >= GET_DEX_TOTAL( pPlayer ) ) ||
			( GET_INT_TOTAL( pPlayer ) > GET_STR_TOTAL( pPlayer ) && GET_INT_TOTAL( pPlayer ) > GET_DEX_TOTAL( pPlayer ) ) )
			return 1;
		else
		{
			SendSystemMsg( pPlayer, g_LANG_STR[85] );
			return 0;
		}
		break;
	}
#endif

	return 0;
}


//
// Function Name: SetVisible
// Date: 
// Description: 
//
void SetVisible( sPDESC_DATA pPlayer, BYTE btVisible )
{
	GET_VISIBLE( pPlayer ) = btVisible;

	g_nPos = 2;
	PutWord( g_Packet, dPACKET_SET_VISIBLE, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutByte( g_Packet, btVisible, g_nPos );

	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}


//
// Function Name: GetCurrSkillLevel
// Date: 
// Description: 
//
int GetCurrSkillLevel( sPDESC_DATA pPlayer )
{
	sPSKILL_DATA pSKill = GET_SKILL_DATA( pPlayer );

	if( !pSKill )
		return 0;
	
	if( IS_SATTACK( pSKill ) )
		return GET_SATTACK( pPlayer );
	else if( IS_LATTACK( pSKill ) )
		return GET_LATTACK( pPlayer );
	else if( IS_BMAGIC( pSKill ) )
		return GET_BMAGIC( pPlayer );
	else if( IS_WMAGIC( pSKill ) )
		return GET_WMAGIC( pPlayer );

	return 0;
}

//
// Function Name: GetBadUserType
// Date: 
// Description: 
//
void GetBadUserType( sPDESC_DATA pPlayer )
{
	char query[1024];
	char escID[128];

	mysql_escape_string( escID, GET_ID( pPlayer ), strlen( GET_ID( pPlayer ) ) );
	
	sprintf( query, "select type from BADUser where id = '%s' limit 1", escID );

	if( mysql_query( g_MYSQL, query ) )
	{
		log( "GetBadUserType : %s\r\n", mysql_error( g_MYSQL ) );
		return;
	}
	
	MYSQL_RES *result;
	MYSQL_ROW row;
	
	result = mysql_store_result( g_MYSQL );
	
	if( result )
	{
		while( ( row = mysql_fetch_row( result ) ) )
		{
			pPlayer->badUserType = atoi( row[0] );
		}
		mysql_free_result( result );
	}

}


//
// Function Name: RandomTrans
// Date: 
// Description: 
//
void RandomTrans( sPDESC_DATA pPlayer )
{
	if( GET_MAP_KIND( pPlayer ) != dMAP_KIND_FIELD )
		return;

	int count = 1;
	int rndCnt = number( 1, gWORLD[GET_MAP_NUM( pPlayer )]->mobResetCnt );

	sPMOB_RESET reset, next_reset;

	LIST_WHILE( gWORLD[GET_MAP_NUM( pPlayer )]->mobReset, reset, next_reset, next, WMgr );
	
	if( count == rndCnt )
	{
		PlayerFromMap( pPlayer );
		
		pPlayer->ch2.gotoMapNum = reset->mapNum;
		pPlayer->ch2.gotoPosX = reset->posX;
		pPlayer->ch2.gotoPosY = reset->posY;
		pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
		
		SendMapLoading( pPlayer );	
		return;
	}
	count++;

	LIST_WHILEEND( gWORLD[GET_MAP_NUM( pPlayer )]->mobReset, reset, next_reset );
	
	if( gWORLD[GET_MAP_NUM( pPlayer )]->mobReset )
	{
		PlayerFromMap( pPlayer );
		
		pPlayer->ch2.gotoMapNum = gWORLD[GET_MAP_NUM( pPlayer )]->mobReset->mapNum;
		pPlayer->ch2.gotoPosX = gWORLD[GET_MAP_NUM( pPlayer )]->mobReset->posX;
		pPlayer->ch2.gotoPosY = gWORLD[GET_MAP_NUM( pPlayer )]->mobReset->posY;
		pPlayer->ch2.gotoPosMapKind = dMAP_KIND_FIELD;
		
		SendMapLoading( pPlayer );	
	}
}

//
// Function Name: SendPlayerEffect
// Date: 
// Description: 
//
void SendPlayerEffect( sPDESC_DATA pPlayer, WORD effect )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_PLAYER_POPEFFECT, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	PutWord( g_Packet, effect, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}


//
// Function Name: SendPlayerStatus
// Date: 
// Description: 
//
void SendPlayerStatus( sPDESC_DATA pPlayer, BYTE status )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHANGE_STATUS, g_nPos );
	PutByte( g_Packet, status, g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: SetPlayerStun
// Date: 
// Description: 
//
void SetPlayerStun( sPDESC_DATA pPlayer )
{
	GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) = g_CurrTime + 5000;
	
	SendPlayerEffect( pPlayer, PLAYER_EFFECT_STUN );
	
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHANGE_STATUS, g_nPos );
	PutByte( g_Packet, dCHANGE_STATUS_STUN, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer );
}

//
// Function Name: UnSetPlayerStun
// Date: 
// Description: 
//
void UnSetPlayerStun( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHANGE_STATUS, g_nPos );
	PutByte( g_Packet, dCHANGE_STATUS_STUNEND, g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
	
	GET_STATUS_DELAY( pPlayer, dSTATUS_DELAY_STUN ) = 0;
}


struct PKDropData
{
	int chaValue;
	int equipRnd;
	int invenRnd;
};

struct PKDropData PKPointDropTable[] =
{
	{	-60000,		2000,		3000		},
	{	-55000,		1925,		1896		},
	{	-50000,		1850,		2792		},
	{	-45000,		1775,		2688		},
	{	-40000,		1700,		2583		},
	{	-35000,		1625,		2479		},
	{	-30000,		1550,		2375		},
	{	-25000,		1475,		2271		},
	{	-20000,		1400,		2167		},
	{	-15000,		1325,		2062		},
	{	-10000,		1250,		1958		},
	{	-5000,		1175,		1854		},
	{	0,			1100,		1750		},
	{	5000,		1025,		1646		},
	{	10000,		950,		1542		},
	{	15000,		875,		1437		},
	{	20000,		800,		1333		},
	{	25000,		725,		1229		},
	{	30000,		650,		1125		},
	{	35000,		575,		1021		},
	{	40000,		500,		917			},
	{	45000,		425,		812			},
	{	50000,		350,		708			},
	{	55000,		275,		604			},
	{	60000,		200,		500			},
	{	0,			0,			0			},
};


//
// Function Name: DropItemByCHA
// Date: 
// Description: 
//
void DropItemByCHA( sPDESC_DATA pPlayer )
{
	if( GET_CHA( pPlayer ) >= 0 )
		return;
	
#ifdef dDAUM_BILLING
	BYTE noItemDrop = 0;

	// 팬텀 게더러 ( 사망시 아이템 유실 없음 )
	if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM10 ) )
	{
		noItemDrop = 1;
	}
	
	if( !noItemDrop )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM11 ) )
		{
			noItemDrop = 1;
		}
	}
	
	if( !noItemDrop )
	{
		if( UseOtherInvenItem( pPlayer, dBILL_ITEM_NUM12 ) )
		{
			noItemDrop = 1;
		}
	}

	if( noItemDrop )
		return;
#endif

	int equipRnd, invenRnd;

	int line = 0;

	if( GET_CHA( pPlayer ) < -30000 )
	{
		CWhileMgr WMgr;
		for( int loopTmp = 0; loopTmp < 50; loopTmp++ )
		{
			WMgr.CheckDump( __FILE__, __LINE__ );
			
			if( PKPointDropTable[line].chaValue == 0 &&
				PKPointDropTable[line].equipRnd == 0 &&
				PKPointDropTable[line].invenRnd == 0 )
				break;
			
			if( PKPointDropTable[line].chaValue <= GET_CHA( pPlayer ) )
			{
				equipRnd = PKPointDropTable[line].equipRnd;
				invenRnd = PKPointDropTable[line].invenRnd;
				break;
			}
			
			line++;
		}
		
		for( int i = 0; i < dMAX_EQUIP; i++ )
		{
			if( pPlayer->equip[i] )
			{
				if( number( 1, 100000 ) <= equipRnd )
				{
					sPITEM_DATA pEquipItem = pPlayer->equip[i]; 
					
					ItemFromEquip( pPlayer, pEquipItem );
#ifdef __CHINA_VER__
					ItemToMap( pEquipItem, GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ), GET_POSY( pPlayer ) );
#else
					ITEMLOG_ItemLog( pPlayer, NULL, pEquipItem, ITEMLOG_ACT_DEADDROP );
					INSERT_ITEM_TO_MEMORY( pEquipItem );
#endif
					break;
				}
			}
		}
	}


	sPITEM_DATA inven, inven_next;

	LIST_WHILE( pPlayer->inven, inven, inven_next, i_next, WMgr );
	
	// 말은 일단 제외
	if( inven->itemNum == dRIDE_ITEM1 ||
		inven->itemNum == dRIDE_ITEM2 ||
		inven->itemNum == dRIDE_ITEM3 || inven->itemNum == dRIDE_ITEM4
#ifdef dDAUM_BILLING
		|| IS_BILLING_ITEM2( inven->itemNum )
#endif
		)
	{
		LIST_SKIP( inven, inven_next );
	}

	if( number( 1, 100000 ) <= invenRnd )
	{
		sPITEM_DATA pInvenItem = inven;

		ItemFromInventory( pPlayer, pInvenItem );
#ifdef __CHINA_VER__
		ItemToMap( pInvenItem, GET_MAP_NUM( pPlayer ), GET_POSX( pPlayer ), GET_POSY( pPlayer ) );
#else
		ITEMLOG_ItemLog( pPlayer, NULL, pInvenItem, ITEMLOG_ACT_DEADDROP );
		INSERT_ITEM_TO_MEMORY( pInvenItem );
#endif
		break;
	}

	LIST_WHILEEND( pPlayer->inven, inven, inven_next );
}


int GetItemWeight( sPITEM_DATA pItem )
{
	int weight = 0;

	if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION )
	{
		if( pItem->exVal[0] > 1 )
		{
			weight += (int)( GET_ITEM_WEIGHT( pItem ) * pItem->exVal[0] );
		}
		else
			weight += GET_ITEM_WEIGHT( pItem );
	}
	else
	{
		weight += GET_ITEM_WEIGHT( pItem );
	}
	return weight;
}
//
// Function Name: AddCurrItemWeight
// Date: 
// Description: 
//
void AddCurrItemWeight( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION )
	{
		if( pItem->exVal[0] > 1 )
		{
			GET_CURRITEM_WEIGHT( pPlayer ) += (int)( GET_ITEM_WEIGHT( pItem ) * pItem->exVal[0] );
		}
		else
			GET_CURRITEM_WEIGHT( pPlayer ) += GET_ITEM_WEIGHT( pItem );
	}
	else
	{
		GET_CURRITEM_WEIGHT( pPlayer ) += GET_ITEM_WEIGHT( pItem );
	}
}

//
// Function Name: DelCurrItemWeight
// Date: 
// Description: 
//
void DelCurrItemWeight( sPDESC_DATA pPlayer, sPITEM_DATA pItem )
{
	if( GET_ITEM_TYPE( pItem ) == dITEMTYPE_POTION )
	{
		if( pItem->exVal[0] > 1 )
		{
			GET_CURRITEM_WEIGHT( pPlayer ) -= (int)( GET_ITEM_WEIGHT( pItem ) * pItem->exVal[0] );
		}
		else
			GET_CURRITEM_WEIGHT( pPlayer ) -= GET_ITEM_WEIGHT( pItem );
	}
	else
	{
		GET_CURRITEM_WEIGHT( pPlayer ) -= GET_ITEM_WEIGHT( pItem );
	}
}


//
// Function Name: CalsAllowTotalWeight
// Date: 
// Description: 
//
int CalsAllowTotalWeight( sPDESC_DATA pPlayer )
{
	int value = ( 50 + ( GET_SATTACK( pPlayer ) * 25 ) + ( GET_LATTACK( pPlayer ) * 20 ) + ( GET_BMAGIC( pPlayer ) * 15 ) + ( GET_WMAGIC( pPlayer ) * 15 ) );

#ifdef dDAUM_BILLING
	// 웨이트 드링크 ( 힘 20% 증가 )
	if( pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT9] )
		value += (int)( value * 0.2 );
#endif

	return value;
}


//
// Function Name: SendWeightGageInfo
// Date: 
// Description: 
//
void SendWeightGageInfo( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_WEIGHTGAGE, g_nPos );
	PutInteger( g_Packet, GET_CURRITEM_WEIGHT( pPlayer ), g_nPos );
	PutInteger( g_Packet, CalsAllowTotalWeight( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );

	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: GetDealItemsTotalWeight
// Date: 
// Description: 
//
int GetDealItemsTotalWeight( sPDESC_DATA pPlayer )
{
	int weight = 0;

	// 아이템 들
	sPLIST_BLOCK list, block, next_block;
	sPITEM_DATA pDealItem;
	
	// pPlayer ==> pDealVict
	list = pPlayer->ch2.dealItems.GetFirstBlock();
	
	LIST_WHILE( list, block, next_block, next, WMgr );
	
	pDealItem = (sPITEM_DATA)block->data;

	if( GET_ITEM_TYPE( pDealItem ) == dITEMTYPE_POTION )
	{
		weight += GET_ITEM_WEIGHT( pDealItem ) * MIN( pDealItem->exVal[0], 1 );
	}
	else
	{
		weight += GET_ITEM_WEIGHT( pDealItem );
	}
	LIST_WHILEEND( list, block, next_block );
	
	return weight;
}

//
// Function Name: IsValidClassItem
// Date: 
// Description: 
//
BOOL IsValidClassItem( sPDESC_DATA pPlayer, int itemNum )
{
	if( GET_ITEM_NEEDCLASS2( itemNum ) == 0 )
		return 1;

	// 전사용 
	if( GET_ITEM_NEEDCLASS2( itemNum ) == 1 )
	{
		if( GET_SATTACK( pPlayer ) >= GET_LATTACK( pPlayer ) &&
			GET_SATTACK( pPlayer ) >= GET_BMAGIC( pPlayer ) &&
			GET_SATTACK( pPlayer ) >= GET_WMAGIC( pPlayer ) )
			return 1;
	}
	// 궁수용 
	else if( GET_ITEM_NEEDCLASS2( itemNum ) == 2 )
	{
		if( GET_LATTACK( pPlayer ) >= GET_SATTACK( pPlayer ) &&
			GET_LATTACK( pPlayer ) >= GET_BMAGIC( pPlayer ) &&
			GET_LATTACK( pPlayer ) >= GET_WMAGIC( pPlayer ) )
			return 1;
	}
	// 법사용
	else if( GET_ITEM_NEEDCLASS2( itemNum ) == 3 )
	{
		if( ( GET_BMAGIC( pPlayer ) >= GET_SATTACK( pPlayer ) &&
			  GET_BMAGIC( pPlayer ) >= GET_LATTACK( pPlayer ) &&
			  GET_BMAGIC( pPlayer ) >= GET_WMAGIC( pPlayer ) ) ||
			  GET_WMAGIC( pPlayer ) >= GET_SATTACK( pPlayer ) &&
			  GET_WMAGIC( pPlayer ) >= GET_LATTACK( pPlayer ) &&
			  GET_WMAGIC( pPlayer ) >= GET_BMAGIC( pPlayer ) )
			  return 1;
	}

	return 0;
}


//
// Function Name: GetPlayerNameColor
// Date: 
// Description: 
//
void GetPlayerNameColor( sPDESC_DATA pPlayer, sPNAME_COLOR pColor )
{
/*	if( pPlayer->ch2.isEvilCha )
		GetNameColor( dNAMECOLOR_EVILCHA, pColor );

	else if( GET_CHA( pPlayer ) <= -30000 )
	       GetNameColor( dNAMECOLOR_CAO, pColor );	
	else
		GetNameColor( dNAMECOLOR_DEFAULT, pColor );
	return;*/
	pColor->r = 192;
	pColor->g = 192;
	pColor->b = 192;

	if( GET_CHA( pPlayer ) == 0 )
		return;

	if( GET_CHA( pPlayer ) >= 0 )
	{
		if( GET_CHA( pPlayer ) >= 40000 )
		{
			pColor->r = 0;
			pColor->g = 176;
			pColor->b = 240;
			return;
		}
		else if( GET_CHA( pPlayer ) >= 20000 )
		{
			pColor->r = 141;
			pColor->g = 180;
			pColor->b = 227;
			return;
		}
		else if( GET_CHA( pPlayer ) >= 0 )
		{
			pColor->r = 197;
			pColor->g = 217;
			pColor->b = 241;
			return;
		}
	}
	else
	{
		if( GET_CHA( pPlayer ) >= -20000 )
		{
			pColor->r = 253;
			pColor->g = 233;
			pColor->b = 217;
			return;
		}
		else if( GET_CHA( pPlayer ) >= -40000 )
		{
			pColor->r = 250;
			pColor->g = 192;
			pColor->b = 144;
			return;
		}
		else if( GET_CHA( pPlayer ) >= -60000 )
		{
			pColor->r = 255;
			pColor->g = 0;
			pColor->b = 0;
			return;
		}
	}
}

//
// Function Name: GetNameColor
// Date: 
// Description: 
//
void GetNameColor( BYTE color, sPNAME_COLOR pColor )
{
	pColor->r = 192;
	pColor->g = 192;
	pColor->b = 192;

	switch( color )
	{
	// 기본 
	case 0:
		break;
	// PK 보호 적용 카오
	case 1:
		pColor->r = 152;
		pColor->g = 152;
		pColor->b = 152;
		break;
	case 2:
		pColor->r = 255;
		pColor->g = 0;
		pColor->b = 0;
		break;
	}
}

//
// Function Name: SendChangeNameColor
// Date: 
// Description: 
//
void SendChangeNameColor( sPDESC_DATA pPlayer, sPNAME_COLOR pColor )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_CHANGE_NAMECOLOR, g_nPos );
	PutInteger( g_Packet, GET_IDX( pPlayer ), g_nPos );
	
	PutByte( g_Packet, pColor->r, g_nPos );
	PutByte( g_Packet, pColor->g, g_nPos );
	PutByte( g_Packet, pColor->b, g_nPos );

	PutSize( g_Packet, g_nPos );
	
	SendToArea( pPlayer, g_Packet, g_nPos, dSENDMODE_TOALL );
}


//
// Function Name: SendPlayerNameColor
// Date: 
// Description: 
//
void SendPlayerNameColor( sPDESC_DATA pPlayer )
{
	sNAME_COLOR nameColor;

	GetPlayerNameColor( pPlayer, &nameColor );
	SendChangeNameColor( pPlayer, &nameColor );
}

//
// Function Name: SendPlayerPvPMode
// Date: 
// Description: 
//
void SendPlayerPvPMode( sPDESC_DATA pPlayer )
{
	g_nPos = 2;
	PutWord( g_Packet, dPACKET_UPDATE_PVP_MODE, g_nPos );
	PutByte( g_Packet, GET_PVPMODE( pPlayer ), g_nPos );
	PutSize( g_Packet, g_nPos );
	SendData( pPlayer, g_Packet, g_nPos );
}

//
// Function Name: IsPvPMode
// Date: 
// Description: 
//
BOOL IsPvPMode( sPDESC_DATA pPlayer )
{
	if( GET_PVPMODE( pPlayer ) == dPVPMODE_NONE )
		return 0;
	return 1;
}


//
// Function Name: IsValidPvPTarget
// Date: 
// Description: 
//
BOOL IsValidPvPTarget( sPDESC_DATA pPlayer, sPDESC_DATA pVict )
{
	// 길드 피케이 모드인데 대상이 같은 길드면 
	if( GET_PVPMODE( pPlayer ) == dPVPMODE_GUILD && 
		GET_GUILD( pPlayer ) && GET_GUILD( pVict ) && 
		GET_GUILD( pPlayer ) == GET_GUILD( pVict ) )
		return 0;
	// 파티 피케이 모드인데 대상이 같은 파티의 소속이면
	else if( GET_PVPMODE( pPlayer ) == dPVPMODE_PARTY && 
		GET_PARTY_MASTER( pPlayer ) && GET_PARTY_MASTER( pVict ) && 
		GET_PARTY_MASTER( pPlayer ) == GET_PARTY_MASTER( pVict ) )
		return 0;

	return 1;
}

//
// Function Name: IsPvPArea
// Date: 
// Description: 
//
BOOL IsPvPArea( sPDESC_DATA pPlayer )
{
	switch( GET_MAP_NUM( pPlayer ) )
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case dDEAL_MAP_NUM:
		SendSystemMsg( pPlayer, g_LANG_STR[25] );
		return 0;
	}
	return 1;
}


//

