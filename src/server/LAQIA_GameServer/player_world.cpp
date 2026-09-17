#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"
#include "message.h"

#include "world.h"

#include "player.h"
#include "player_world.h"

#include "item_world.h"

#include "mob.h"
#include "mob_world.h"

#include "npc.h"
#include "npc_world.h"

#include "party.h"

#include "skills.h"
#include "data.h"
#include "vehicle.h"

/*
 *	Make Character insert packet 
 */
void MakeInPacket( sPDESC_DATA pPlayer )
{
	PutHeader( dPACKET_INSERT_CHARACTER );
	PutInteger( GET_IDX( pPlayer ) );							// 고유번호
	PutString( GET_NAME( pPlayer ) );							// 이름
	PutInteger( GET_LEVEL( pPlayer ) );							// 레벨 
	PutByte( GET_SEX( pPlayer ) );								// 성별 
	PutInteger( GET_CLASS( pPlayer ) );							// 직업
	PutInteger( GET_HP( pPlayer ) );							// HP
	PutInteger( GET_MAXHP( pPlayer ) );							// MAX HP
	PutInteger( GET_MP( pPlayer ) );							// MP
	PutInteger( GET_MAXMP( pPlayer ) );							// MAX MP
	
	sHEAD_TYPE sHeadType;
	GetHeadNumber( GET_HEADTYPE( pPlayer ), &sHeadType );
	
	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_HEAD ) )
		PutInteger( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_HEAD )->itemNum );	// 헬멧
	else
		PutInteger( sHeadType.hair );										// 머리카락 
	PutInteger( sHeadType.face );											// 얼굴 
	
	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_BODY ) )
		PutInteger( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_BODY )->itemNum );	// 갑옷 
	else
		PutInteger( GetBodyNumber( GET_BODYTYPE( pPlayer ) ) );				// 몸통

	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_HAND ) )
		PutInteger( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_HAND )->itemNum );	// 장갑
	else
		PutInteger( GetHandNumber( GET_BODYTYPE( pPlayer ) ) );				// 손 

	if( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_FOOT ) )
		PutInteger( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_FOOT )->itemNum );	// 신발 
	else
		PutInteger( GetFootNumber( GET_BODYTYPE( pPlayer ) ) );				// 발

	PutInteger( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON ) ? 
		GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_WEAPON )->itemNum : 0 );		// 무기
	PutInteger( GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD ) ? 
		GET_EQUIP_ITEM( pPlayer, dEQUIP_POS_SHIELD )->itemNum : 0 );		// 방패

	PutInteger( GET_POSX( pPlayer ) );
	PutInteger( GET_POSY( pPlayer ) );
	PutInteger( GET_POSZ( pPlayer ) );
	
	PutByte( pPlayer->ch.myshopOpen );
	PutString( pPlayer->ch.myshopTitle );

	PutByte( GET_DEAD_STATE( pPlayer ) );									// 죽은 상태
	
	if( GET_VEHICLE_STATE( pPlayer ) )
		PutByte( dNPC_VEHICLE1 );
	else
		PutByte( GET_VEHICLE_NUM( pPlayer ) );									// 말 번호 

	PutByte( GET_ATTACKRANGE( pPlayer ) );

	PutByte( GET_SUBSTATE( pPlayer ) );

	if( GET_VEHICLE_STATE( pPlayer ) )
		PutInteger( dNPC_VEHICLE_SPEED1 );
	else 
		PutInteger( GetVehicleSpeed( pPlayer ) );

	PutByte( GET_COUNTRY( pPlayer ) );
}

/*
 *	Make Character remove packet 
 */
void MakeOutPacket( sPDESC_DATA pPlayer )
{
	PutHeader( dPACKET_REMOVE_CHARACTER );
	PutInteger( GET_IDX( pPlayer ) );
}


/*
 *	SendToSector
 */
void SendToSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, const char *data, int size , BYTE mode )
{
	sPDESC_DATA d, next_d;

	if( mode == dSENDMODE_TOALL )
	{
		LIST_WHILE( tile, d, next_d , w_next );
		SendData( d, data, size );
		LIST_WHILEEND();	
	}
	else
	{
		LIST_WHILE( tile, d, next_d, w_next );
		if( d != pPlayer )
			SendData( d, data, size );
		LIST_WHILEEND();
	}
}

/*
 *	SendToRangeSector
 */
void SendToRangeSector( sPDESC_DATA pPlayer, sPDESC_DATA tile, const char *data, int size , int range,  BYTE mode )
{
	sPDESC_DATA d, next_d;

	if( mode == dSENDMODE_TOALL )
	{
		LIST_WHILE( tile, d, next_d , w_next );
		if( GetDistance( GET_POSX( pPlayer ), GET_POSY( pPlayer ), GET_POSX( d ), GET_POSY( d ) ) <= range )
			SendData( d, data, size );
		LIST_WHILEEND();	
	}
	else
	{
		LIST_WHILE( tile, d, next_d, w_next );
		if( d != pPlayer )
		{
			if( GetDistance( GET_POSX( pPlayer ), GET_POSY( pPlayer ), GET_POSX( d ), GET_POSY( d ) ) <= range )				
				SendData( d, data, size );
		}
		LIST_WHILEEND();
	}
}



/*
 *	SendToVSector
 */
void SendToVSector( int x, int startY, int endY, sPDESC_DATA pPlayer, const char *data, int size )
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


/*
 *	SendToHSector
 */
void SendToHSector( int y, int startX, int endX, sPDESC_DATA pPlayer, const char *data, int size )
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

/*
 *	SendToArea
 */
void SendToArea( sPDESC_DATA pPlayer , const char *data, int size, BYTE mode )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);

	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				SendToSector( pPlayer, gWORLD[pPlayer->ch.mapNum]->player[y][x], data, size, mode );
}

/*
 *	SendToRange
 */
void SendToRange( sPDESC_DATA pPlayer , const char *data, int size, int range, BYTE mode )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);

	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				SendToRangeSector( pPlayer, gWORLD[pPlayer->ch.mapNum]->player[y][x], data, size, range, mode );
}




/*
 *	SendToShoutChat
 */
void SendToShoutChat( sPDESC_DATA pPlayer , const char *data, int size, int sector)
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);

	for( int y = posY - sector; y <= posY + sector; y++ )
		for( int x = posX - sector; x <= posX + sector; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				SendToSector( pPlayer, gWORLD[pPlayer->ch.mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
}

void SendToShoutChat(const char* data, int size)
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gPLAYERS.list, d, next_d , next );
	if( GET_STATE( d ) == dDESC_STATE_PLAYING )
	{
		SendData( d, data, size );
	}
	LIST_WHILEEND();
}
/*
 *	GetPlayerSector
 */
void GetPlayerSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->player[y][x], d, next_d, w_next )

	if( d != pPlayer )
	{
		MakeInPacket( d );
		SendData( pPlayer, g_Packet, g_nPos );
	}
	LIST_WHILEEND();
}

/*
 *	GetCurrAreaPlayer
 */
void GetCurrAreaPlayer( sPDESC_DATA pPlayer )
{
	int startX = (int)(pPlayer->ch.posX/dONE_TILE);
	int startY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = startY - 1; y <= startY + 1; y++ )
		for( int x = startX - 1; x <= startX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				GetPlayerSector( pPlayer, x, y );
}

/*
 *	OutPlayerSector
 */
void OutPlayerSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPDESC_DATA d, next_d;

	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->player[y][x], d, next_d , w_next );
	if( d != pPlayer )
	{
		MakeOutPacket( d );
		SendData( pPlayer, g_Packet, g_nPos );
	}
	LIST_WHILEEND();
}

/*
 *	OutCurrAreaPlayer
 */
void OutCurrAreaPlayer( sPDESC_DATA pPlayer )
{
	int destX = ( pPlayer->ch.posX / dONE_TILE ), destY = ( pPlayer->ch.posY / dONE_TILE );

	for( int y = destY - 1; y <= destY + 1; y++ )
		for( int x = destX - 1; x <= destX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				OutPlayerSector( pPlayer, x, y );
}

/*
 *	GetPlayerHSector
 */
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

/*
 *	GetPlayerVSector
 */
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

/*
 *	OutPlayerHSector
 */
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

/*
 *	OutPlayerVSector
 */
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

/*
 *	PlayerFromMap
 */
void PlayerFromMap( sPDESC_DATA pPlayer )
{
	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING )
		return;

	DATA_SendUpdateMapInfo( pPlayer );
	DATA_SendUpdatePreMapInfo( pPlayer );

	SKILL_ClearWaitingDuration( pPlayer );

	OutCurrAreaPlayer( pPlayer );		// 플레이어
	OutCurrAreaMob( pPlayer );			// 몬스터
	OutCurrAreaItem( pPlayer );			// 아이템
	OutCurrAreaNpc( pPlayer );			// NPC

	int destX = ( pPlayer->ch.posX / dONE_TILE ), destY = ( pPlayer->ch.posY / dONE_TILE );

	MakeOutPacket( pPlayer );
	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOOTHER );		

	// Area List
	REMOVE_FROM_LIST( gWORLD[pPlayer->ch.mapNum]->player[destY][destX], pPlayer, w_prev, w_next );
	gWORLD[pPlayer->ch.mapNum]->currPlayerCnt--;

	GET_ENEMY_CNT( pPlayer ) = 0;

	SKILL_ClearWaitingDuration( pPlayer );
}

/*
 *	PlayerIntoMap
 */
void PlayerIntoMap( sPDESC_DATA pPlayer , int mapNum , int posX, int posY, int posZ )
{
	if( !IS_VALID_WORLD( mapNum ) )
	{
		log( "PlayerIntoMap : IS_VALID_WORLD %s/%s\r\n", GET_NAME( pPlayer ), GET_ID( pPlayer ) );
		pPlayer->disconnectFlag = 1;
		return;
	}

	GET_MAP_NUM( pPlayer ) = mapNum;
	GET_POSX( pPlayer ) = posX;
	GET_POSY( pPlayer ) = posY;

	int destX = ( pPlayer->ch.posX / dONE_TILE ), destY = ( pPlayer->ch.posY / dONE_TILE );

	// Area List
	INSERT_TO_LIST( gWORLD[mapNum]->player[destY][destX], pPlayer, w_prev, w_next );
	
	gWORLD[mapNum]->currPlayerCnt++;

	MakeInPacket( pPlayer );
	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOOTHER );

	GetCurrAreaPlayer( pPlayer );		// 플레이어
	GetCurrAreaMob( pPlayer );			// 몬스터
	GetCurrAreaItem( pPlayer );			// 아이템
	GetCurrAreaNpc( pPlayer );			// NPC

	pPlayer->ch.endMapLoading = 1;

	PLAYER_SendAttackRange( pPlayer );
	PLAYER_UpdateArmor( pPlayer );
	PLAYER_UpdateDamage( pPlayer );
	PLAYER_UpdateHP( pPlayer );
	PLAYER_UpdateMP( pPlayer );

	pPlayer->ch.endMapLoading = 1;

	CheckAreaBGM( pPlayer );

	SendMilesPostInfo( pPlayer );
}

/*
 *	CheckLoadingPoint
 */
BOOL CheckLoadingPoint( sPDESC_DATA pPlayer )
{
	BYTE	fishing = 0;

#define CHECK_LOADING( mapNum, x, y, targetMap, targetX, targetY )\
	if( GET_MAP_NUM( pPlayer ) == mapNum && GetDistance( GET_POSX( pPlayer ), GET_POSY( pPlayer ), x, y ) <= 5 )\
	{\
		if( !pPlayer->ch.endMapLoading )\
		{\
			GET_GOTO_MAPNUM( pPlayer ) = targetMap;\
			GET_GOTO_POSX( pPlayer ) = targetX;\
			GET_GOTO_POSY( pPlayer ) = targetY;\
			GET_GOTO_POSZ( pPlayer ) = 0;\
			return 1;\
		}\
		fishing = 1;\
	}

	CHECK_LOADING( 1, 334, 3401, 2, 3859, 2328 );
	CHECK_LOADING( 2, 3859, 2328, 1, 334, 3401 );
	CHECK_LOADING( 1, 545, 3777, 3, 640, 144 );
	CHECK_LOADING( 3, 640, 144, 1, 545, 3777 );
	CHECK_LOADING( 2, 3815, 3103, 3, 280, 570 );
	CHECK_LOADING( 3, 280, 570, 2, 3815, 3103 );
	CHECK_LOADING( 2, 3854, 3417, 3, 234, 815 );
	CHECK_LOADING( 3, 234, 815, 2, 3854, 3417 );
	
	if( fishing == 0 )
		pPlayer->ch.endMapLoading = 0;

#undef CHECK_LOADING
	return 0;
}

/*
 *	MovePlayer
 */
BOOL MovePlayer( sPDESC_DATA pPlayer , int posX, int posY , int posZ )
{
	if( posX < 0 || posX >= dONE_MAP_SIZE ||
		posY < 0 || posY >= dONE_MAP_SIZE )
		return 0;

	if( GET_STATE( pPlayer ) != dDESC_STATE_PLAYING )
		return 1;	

	if( GET_SUBSTATE( pPlayer ) == dSUBSTATE_SITTING )
	{
		GET_SUBSTATE( pPlayer ) = dSUBSTATE_STANDING;
//		PLAYER_UpdateSubState( pPlayer );
	}

//	if( abs( GET_POSX( pPlayer ) - posX ) > dPLAYER_MOVE_RANGE ||
//		abs( GET_POSY( pPlayer ) - posY ) > dPLAYER_MOVE_RANGE )
//		return 0;

	int newX, newY, oldX, oldY, mapNum = pPlayer->ch.mapNum, plusNum;
	
	newX = (int)(posX * 2);// / dONE_TILE);
	newY = (int)(posY * 2); // dONE_TILE);
	oldX = (int)(pPlayer->ch.posX * 2 );// dONE_TILE);
	oldY = (int)(pPlayer->ch.posY * 2 );// dONE_TILE);

	if( newX >= dONE_MAP_SIZE || newY >= dONE_MAP_SIZE )
		return 0;
	
	GET_POSX( pPlayer ) = posX;
	GET_POSY( pPlayer ) = posY;
	GET_POSZ( pPlayer ) = posZ;
	plusNum = 1;

	if( newX != oldX || newY != oldY )
	{
		REMOVE_FROM_LIST( gWORLD[mapNum]->player[oldY][oldX], pPlayer, w_prev, w_next );
		INSERT_TO_LIST( gWORLD[mapNum]->player[newY][newX], pPlayer, w_prev, w_next );
		
		if( newY < oldY && newX == oldX ) // Y 감소
		{
			MakeOutPacket( pPlayer );
			SendToHSector( oldY+plusNum, oldX-plusNum, oldX+plusNum, pPlayer, g_Packet, g_nPos );
			
			OutPlayerHSector( oldY + plusNum, oldX-plusNum, oldX+plusNum, pPlayer );
			OutMobHSector( oldY + plusNum, oldX-plusNum, oldX+plusNum, pPlayer );		// Mob
			OutItemHSector( oldY + plusNum , oldX - plusNum , oldX + plusNum , pPlayer );		// Item
			OutNpcHSector( oldY + plusNum , oldX - plusNum , oldX + plusNum , pPlayer );		// NPC
			
			MakeInPacket( pPlayer );
			SendToHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer, g_Packet, g_nPos );

			GetPlayerHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetMobHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer );		// mob
			GetItemHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer );		// Item
			GetNpcHSector( newY - plusNum , newX - plusNum , newX + plusNum , pPlayer );		// NPC
		}
		else if( newX > oldX && newY < oldY ) // X증가 Y 감소
		{
			MakeOutPacket( pPlayer );
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
			// NPC
			OutNpcHSector( oldY + plusNum , oldX, oldX + plusNum , pPlayer );
			OutNpcVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			
			MakeInPacket( pPlayer );
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
			// NPC
			GetNpcHSector( newY - plusNum , newX - plusNum , newX, pPlayer );
			GetNpcVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
		}
		else if( newX > oldX && newY == oldY ) // X 증가
		{
			MakeOutPacket( pPlayer );
			SendToVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutMobVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );		// mob
			OutItemVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );		// Item
			OutNpcVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );		// NPC
			
			MakeInPacket( pPlayer );
			SendToVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer, g_Packet, g_nPos );
			
			GetPlayerVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetMobVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );		// mob
			GetItemVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );		// Item
			GetNpcVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );		// NPC
		}
		else if( newX > oldX && newY > oldY ) // X 증가 Y 증가
		{
			MakeOutPacket( pPlayer );
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
			// NPC
			OutNpcVSector( oldX - plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutNpcHSector( oldY - plusNum , oldX, oldX + plusNum , pPlayer );
			
			MakeInPacket( pPlayer );
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
			// NPC
			GetNpcVSector( newX + plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetNpcHSector( newY + plusNum , newX - plusNum , newX, pPlayer );
		}
		else if( newY > oldY && newX == oldX ) // Y 증가
		{
			MakeOutPacket( pPlayer );
			SendToHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );
			OutMobHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );		// mob
			OutItemHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );		// Item
			OutNpcHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );		// NPC

			MakeInPacket( pPlayer );
			SendToHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer, g_Packet, g_nPos );


			GetPlayerHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetMobHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );		// mob
			GetItemHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );		// Item
			GetNpcHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );		// NPC
		}
		else if( newX < oldX && newY > oldY ) // X 감소 Y 증가
		{
			MakeOutPacket( pPlayer );
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
			// NPC
			OutNpcHSector( oldY - plusNum , oldX - plusNum , oldX + plusNum , pPlayer );
			OutNpcVSector( oldX + plusNum , oldY, oldY + plusNum , pPlayer );
			
			MakeInPacket( pPlayer );
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
			// NPC
			GetNpcHSector( newY + plusNum , newX - plusNum , newX + plusNum , pPlayer );
			GetNpcVSector( newX - plusNum , newY - plusNum , newY, pPlayer );
		}
		else if( newX < oldX && newY == oldY ) // X 감소
		{
			MakeOutPacket( pPlayer );
			SendToVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer, g_Packet, g_nPos );
			
			OutPlayerVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutMobVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );		// Mob
			OutItemVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );		// Item
			OutNpcVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );		// NPC
			
			MakeInPacket( pPlayer );
			SendToVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer, g_Packet, g_nPos );
			
			GetPlayerVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetMobVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );		// Mob
			GetItemVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );		// Item
			GetNpcVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );		// NPC
		}
		else if( newX < oldX && newY < oldY ) // X감소 Y 감소
		{
			MakeOutPacket( pPlayer );
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
			// NPC
			OutNpcVSector( oldX + plusNum , oldY - plusNum , oldY + plusNum , pPlayer );
			OutNpcHSector( oldY + plusNum , oldX - plusNum , oldX, pPlayer );
			
			MakeInPacket( pPlayer );
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
			// NPC
			GetNpcVSector( newX - plusNum , newY - plusNum , newY + plusNum , pPlayer );
			GetNpcHSector( newY - plusNum , newX, newX + plusNum , pPlayer );
		}
	}
	
	PutHeader( dPACKET_MOVE_CHARACTER );
	PutInteger( GET_IDX( pPlayer ) );
	PutInteger( GET_POSX( pPlayer ) );
	PutInteger( GET_POSY( pPlayer ) );
	PutInteger( GET_POSZ( pPlayer ) );
	
	SendToArea( pPlayer, g_Packet, g_nPos , dSENDMODE_TOOTHER );

	// 맵 타이틀 처리
	if( GET_AREA_ATTR( pPlayer ) )
	{
		if( GET_AREA_MAP( pPlayer ) != GET_MAP_NUM( pPlayer ) )
		{
			SendMapTitle( pPlayer, 
				gWORLD[GET_MAP_NUM( pPlayer )]->areaInfo[GET_AREA_ATTR( pPlayer )]->mainTitle,
				gWORLD[GET_MAP_NUM( pPlayer )]->areaInfo[GET_AREA_ATTR( pPlayer )]->subTitle );

			GET_AREA_MAP( pPlayer ) = GET_MAP_NUM( pPlayer );
			GET_AREA_NUM( pPlayer ) = GET_AREA_ATTR( pPlayer );

			CheckAreaBGM( pPlayer );
		}
		else
		{
			if( GET_AREA_NUM( pPlayer ) != GET_AREA_ATTR( pPlayer ) )
			{
				SendMapTitle( pPlayer, 
					gWORLD[GET_MAP_NUM( pPlayer )]->areaInfo[GET_AREA_ATTR( pPlayer )]->mainTitle,
					gWORLD[GET_MAP_NUM( pPlayer )]->areaInfo[GET_AREA_ATTR( pPlayer )]->subTitle );
				
				GET_AREA_MAP( pPlayer ) = GET_MAP_NUM( pPlayer );
				GET_AREA_NUM( pPlayer ) = GET_AREA_ATTR( pPlayer );

				CheckAreaBGM( pPlayer );
			}
		}
	}
	else
	{
		if( GET_AREA_NUM( pPlayer ) != GET_AREA_ATTR( pPlayer ) )
			CheckAreaBGM( pPlayer );
		
		GET_AREA_MAP( pPlayer ) = 0;
		GET_AREA_NUM( pPlayer ) = 0;
	}

	// 맵로딩
	if( CheckLoadingPoint( pPlayer ) )
	{
		PlayerFromMap( pPlayer );
		SendMapLoading( pPlayer );
		return 1;
	}

	pPlayer->ch.lastAttackLink = 0;

	PARTY_SendMovePacket( pPlayer );
	
	return 1;
}



/*
 *	SendMapLoading
 */
void SendMapLoading( sPDESC_DATA pPlayer )
{
	PutHeader( dPACKET_BEGIN_MAPLOADING );
	PutInteger( GET_GOTO_MAPNUM( pPlayer ) );
	PutInteger( GET_GOTO_POSX( pPlayer ) );
	PutInteger( GET_GOTO_POSY( pPlayer ) );
	PutInteger( GET_GOTO_POSZ( pPlayer ) );
	
	SendData( pPlayer, g_Packet, g_nPos );	

	GET_STATE( pPlayer ) = dDESC_STATE_LOADING;
}


/*
 *	FindPlayerInTile
 */
sPDESC_DATA FindPlayerInTile( sPDESC_DATA list, int idxNum )
{
	sPDESC_DATA d, next_d;
	
	LIST_WHILE( list, d, next_d , w_next );
	if( d->sock == idxNum )
		return d;
	LIST_WHILEEND();

	return NULL;
}

/*
 *	FindPlayerInArea
 */
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

/*
 *	IsValidAttackArea
 */
BOOL IsValidAttackArea( sPDESC_DATA pPlayer, sPMOB_DATA pMob )
{
	int playerX, playerY;
	int mobX, mobY;

	playerX = GET_POSX( pPlayer );
	playerY = GET_POSY( pPlayer );

	mobX = pMob->posX;
	mobY = pMob->posY;

	if( IS_SET( gWORLD[GET_MAP_NUM( pPlayer )]->mobAttr[playerY][playerX], dMOB_MAP_BLOCK ) )
		return 0;

	for( int i = 0; i < dONE_MAP_SIZE; i++ )
	{
		if( mobX < playerX )
			mobX++;
		else if( mobX > playerX )
			mobX--;

		if( mobY < playerY )
			mobY++;
		else if( mobY > playerY )
			mobY--;

		if( IS_SET( gWORLD[GET_MAP_NUM( pPlayer )]->mobAttr[mobY][mobX], dMOB_MAP_BLOCK ) )
			return 0;

		if( mobX == playerX && mobY == playerY )
			return 1;
	}
	return 1;
}



/*
 *	PlayerToHometown
 */
void PlayerToHometown( sPDESC_DATA pPlayer )
{
	if( IS_VALID_WORLD( GET_PRE_MAPNUM( pPlayer ) ) )
	{
		GET_GOTO_MAPNUM( pPlayer ) = GET_PRE_MAPNUM( pPlayer );
		GET_GOTO_POSX( pPlayer ) = GET_PRE_POSX( pPlayer );
		GET_GOTO_POSY( pPlayer ) = GET_PRE_POSY( pPlayer );
		GET_GOTO_POSZ( pPlayer ) = GET_PRE_POSZ( pPlayer );		
	}
	else
	{
		switch( GET_MAP_NUM( pPlayer ) )
		{
		case 1:
			GET_GOTO_MAPNUM( pPlayer ) = dDEF_PLAYER_MAPNUM;
			GET_GOTO_POSX( pPlayer ) = dDEF_PLAYER_POSX;
			GET_GOTO_POSY( pPlayer ) = dDEF_PLAYER_POSY;
			GET_GOTO_POSZ( pPlayer ) = dDEF_PLAYER_POSZ;
			break;
		case 2:
			GET_GOTO_MAPNUM( pPlayer ) = 2;
			GET_GOTO_POSX( pPlayer ) = 2892;
			GET_GOTO_POSY( pPlayer ) = 2005;
			GET_GOTO_POSZ( pPlayer ) = 0;
			break;
		}
	}

	PlayerFromMap( pPlayer );
	SendMapLoading( pPlayer );
}

//EOF

