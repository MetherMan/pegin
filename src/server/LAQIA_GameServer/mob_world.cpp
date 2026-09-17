#include "def.h"
#include "structs.h"
#include "externs.h"
#include "network.h"
#include "packet.h"

#include "world.h"
#include "player.h"
#include "item.h"
#include "mob.h"
#include "mob_world.h"
#include "player_world.h"
#include "skills.h"

#include "combat.h"

#include "message.h"

/*
 *	MakeMobInPacket
 */
void MakeMobInPacket( sPMOB_DATA pMob )
{
	PutHeader( dPACKET_INSERT_MONSTER );
	PutInteger( pMob->idxNum );					// 고유번호 
	PutInteger( pMob->mobNum );					// 몬스터 번호 
	PutInteger( pMob->posX );					// 좌표 
	PutInteger( pMob->posY );			
	if( pMob->enemy )
		PutInteger( GET_MOB_MOVESPEED( pMob ) + 2 );	// 이동속력
	else
		PutInteger( GET_MOB_MOVESPEED( pMob ) );
	PutInteger( GET_MOB_HP( pMob ) );
	PutInteger( GET_MOB_MAXHP( pMob ) );
	PutInteger( GET_MOB_MP( pMob ) );
	PutInteger( GET_MOB_MAXMP( pMob ) );

	if( pMob->enemy )
		PutByte( 1 );
	else
		PutByte( 0 );

	PutByte( (BYTE)GET_MOB_ATTACKRANGE( pMob ) );
	PutInteger( GET_MOB_LEVEL( pMob ) );

	if( IS_SRC_MOB( pMob ) )
		PutByte( 1 );
	else
		PutByte( 0 );
}

/*
 *	MakeMobOutPacket
 */
void MakeMobOutPacket( sPMOB_DATA pMob )
{
	PutHeader( dPACKET_REMOVE_MONSTER );
	PutInteger( pMob->idxNum );
}

/*
 *	GetMobSector
 */
void GetMobSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPMOB_DATA m, next_m;
	
	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m , w_next ); 

	if( !m->isAIMob )
	{
		INSERT_TO_LIST( gMOBS.list, m, prev, next );
		m->isAIMob = 1;
	}
	
	MakeMobInPacket( m );
	SendData( pPlayer, g_Packet, g_nPos );
	LIST_WHILEEND();
}

/*
 *	GetMobVSector
 */
void GetMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				GetMobSector( pPlayer,  x, y );
		}
	}
}

/*
 *	GetMobHSector	
 */
void GetMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				GetMobSector( pPlayer, x, y );
		}
	}
}

/*
 *	OutMobSector
 */
void OutMobSector( sPDESC_DATA pPlayer, int x, int y )
{
	sPMOB_DATA m, next_m;
	
	LIST_WHILE( gWORLD[pPlayer->ch.mapNum]->mob[y][x], m, next_m , w_next ); 

	if( m->enemy == pPlayer )
	{
		SET_MOB_ENEMY( m, NULL );
		ChangeMobState( m );
	}

	MakeMobOutPacket( m );
	SendData( pPlayer, g_Packet, g_nPos );

	LIST_WHILEEND();
}

/*
 *	OutMobVSector
 */
void OutMobVSector( int x, int startY, int endY, sPDESC_DATA pPlayer )
{
	if( x >= 0 && x < dONE_TILE_SIZE )
	{		
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				OutMobSector( pPlayer,  x, y );
		}
	}
}

/*
 *	OutMobHSector
 */
void OutMobHSector( int y, int startX, int endX, sPDESC_DATA pPlayer )
{
	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				OutMobSector( pPlayer, x, y );
		}
	}
}

/*
 *	SendToMobVSector
 */
void SendToMobVSector( int x, int startY, int endY, sPMOB_DATA pMob, char *data, int size )
{
	int mapNum = pMob->mapNum;

	if( x >= 0 && x < dONE_TILE_SIZE )
	{
		for( int y = startY; y <= endY; y++ )
		{
			if( y >= 0 && y < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[mapNum]->player[y][x], data, size , dSENDMODE_TOALL );
		}
	}
}

/*
 *	SendToMobHSector
 */
void SendToMobHSector( int y, int startX, int endX, sPMOB_DATA pMob, char *data, int size )
{
	int mapNum = pMob->mapNum;

	if( y >= 0 && y < dONE_TILE_SIZE )
	{
		for( int x = startX; x <= endX; x++ )
		{
			if( x >= 0 && x < dONE_TILE_SIZE )
				SendToSector( NULL, gWORLD[mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
		}
	}
}


/*
 *	SendToMobArea
 */
void SendToMobArea( sPMOB_DATA pMob, char *data, int size )
{
	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);
	
	BYTE findUser = 0;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				if( gWORLD[pMob->mapNum]->player[y][x] )
				{
					SendToSector( NULL, gWORLD[pMob->mapNum]->player[y][x], data, size, dSENDMODE_TOALL );
					findUser = 1;
				}
			}
		}
	}
	if( !findUser )
	{
		if( pMob->AiTick++ >= dDEF_MOB_AITICK )
		{
			REMOVE_FROM_LIST( gMOBS.list, pMob, prev, next );
			pMob->isAIMob = 0;
		}
	}
	else 
	{
		if( pMob->AiTick )
			pMob->AiTick = 0;
	}
}


/*
 *	SendToMobRange
 */
void SendToMobRange( sPMOB_DATA pMob, char *data, int size, int range )
{
	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);
	
	sPDESC_DATA d, next_d;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				if( gWORLD[pMob->mapNum]->player[y][x] )
				{
					LIST_WHILE( gWORLD[pMob->mapNum]->player[y][x], d, next_d, w_next );
					if( GetDistance( GET_POSX( d ), GET_POSY( d ), pMob->posX, pMob->posY ) <= range )
					{
						SendData( d, data, size );
					}
					LIST_WHILEEND();
				}
			}
		}
	}
}


/*
 *	GetCurrAreaMob
 */
void GetCurrAreaMob( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				GetMobSector( pPlayer, x, y );
}


/*
 *	OutCurrAreaMob
 */
void OutCurrAreaMob( sPDESC_DATA pPlayer )
{
	int posX = (int)(pPlayer->ch.posX/dONE_TILE);
	int posY = (int)(pPlayer->ch.posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				OutMobSector( pPlayer, x, y );
}

/*
 *	CheckMobSectorEnemy
 */
void CheckMobSectorEnemy( sPMOB_DATA pMob, int x, int y )
{
	sPDESC_DATA d, next_d;
	
	LIST_WHILE( gWORLD[pMob->mapNum]->player[y][x], d, next_d , w_next );

	if( GET_IDX( d ) == pMob->agroLastVict )
		LIST_SKIP();

	// 선제 공격이 가능한 몹이거나 링크번호가 같은 몹을 공격중인 플레이어면
	// 감지거리 안에 들어올 경우
	
	if( GET_MOB_LINKNUM( pMob ) )
	{
		if( d->ch.lastAttackLink == GET_MOB_LINKNUM( pMob ) && GetDistance( GET_POSX( d ), GET_POSY( d ), pMob->posX, pMob->posY ) <= GET_MOB_VISIONOFRANGE( pMob ) * 2 )
		{
			if( !GET_DEAD_STATE( d ) &&
				!GET_VEHICLE_STATE( d ) &&
				GET_ENEMY_CNT( d ) < dMAX_AGRO_ENEMY 
#ifdef _DUMMY_MODE_
				&& !d->isDummy 
#endif
				)
			{
				if( IsValidAttackArea( d, pMob ) )
				{
					SET_MOB_ENEMY( pMob, d );
					MOB_SetAgro( pMob, d );
					ChangeMobState( pMob );
					return;
				}
			}		
			LIST_SKIP();
		}
	}
	
	if( GetDistance( GET_POSX( d ), GET_POSY( d ), pMob->posX, pMob->posY ) <= GET_MOB_VISIONOFRANGE( pMob ) )
	{
		if( !GET_DEAD_STATE( d ) &&
			!GET_VEHICLE_STATE( d ) &&
			( GET_LEVEL( d ) - GET_MOB_LEVEL( pMob ) ) <= dAGRO_LEVEL_GAP  &&
			GET_ENEMY_CNT( d ) < dMAX_AGRO_ENEMY 
#ifdef _DUMMY_MODE_
			&& !d->isDummy 
#endif
			)
		{
			if( IsValidAttackArea( d, pMob ) )
			{
				SET_MOB_ENEMY( pMob, d );
				MOB_SetAgro( pMob, d );
				ChangeMobState( pMob );
				return;
			}
		}
	}

	LIST_WHILEEND();
}


/*
 *	CheckMobEnemyArea
 */
void CheckMobEnemyArea( sPMOB_DATA pMob )
{
	if( pMob->enemy )
		return;

	int posX = (int)(pMob->posX/dONE_TILE);
	int posY = (int)(pMob->posY/dONE_TILE);
	
	for( int y = posY - 1; y <= posY + 1; y++ )
		for( int x = posX - 1; x <= posX + 1; x++ )
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
				CheckMobSectorEnemy( pMob, x, y );
}


/*
 *	MoveMonster
 */
void MoveMonster( sPMOB_DATA pMob, int posX, int posY )
{
	int newX, newY, oldX, oldY, mapNum = pMob->mapNum;

	newX = (int)(posX / dONE_TILE);
	newY = (int)(posY / dONE_TILE);
	oldX = (int)(pMob->posX / dONE_TILE);
	oldY = (int)(pMob->posY / dONE_TILE);

#ifdef __ONLY_ONE_MOB_IN_SECTOR__
	if( newX != oldX || newY != oldY )
	{
		if( gWORLD[pMob->mapNum].mob[newY][newX] )
		{
			pMob->moveDir = number( 0, 7 );
			return;
		}
	}
#endif

	//
	if( IS_SET( gWORLD[pMob->mapNum]->mobAttr[pMob->posY][pMob->posX], dMOB_MOB_BLOCK ) )
		REMOVE_BIT( gWORLD[pMob->mapNum]->mobAttr[pMob->posY][pMob->posX], dMOB_MOB_BLOCK );

	pMob->posX = posX;
	pMob->posY = posY;

	//
	if( !IS_SET( gWORLD[pMob->mapNum]->mobAttr[pMob->posY][pMob->posX], dMOB_MOB_BLOCK ) )
		SET_BIT( gWORLD[pMob->mapNum]->mobAttr[pMob->posY][pMob->posX], dMOB_MOB_BLOCK );
	
	if( newX != oldX || newY != oldY )
	{
		REMOVE_FROM_LIST( gWORLD[mapNum]->mob[oldY][oldX], pMob, w_prev, w_next );
		INSERT_TO_LIST( gWORLD[mapNum]->mob[newY][newX], pMob, w_prev, w_next );
		
		if( newY < oldY && newX == oldX ) // Y 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY+1, oldX-1, oldX+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobHSector( newY-1, newX-1, newX+1, pMob, g_Packet, g_nPos );			
		}
		else if( newX > oldX && newY < oldY ) // X증가 Y 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY+1, oldX, oldX+1, pMob, g_Packet, g_nPos );
			SendToMobVSector( oldX-1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobHSector( newY-1, newX-1, newX, pMob, g_Packet, g_nPos );
			SendToMobVSector( newX+1, newY-1, newY+1, pMob, g_Packet, g_nPos );
		}
		else if( newX > oldX && newY == oldY ) // X 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX-1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobVSector( newX+1, newY-1, newY+1, pMob, g_Packet, g_nPos );			
		}
		else if( newX > oldX && newY > oldY ) // X 증가 Y 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX-1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( oldY-1, oldX, oldX+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobVSector( newX+1, newY-1, newY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( newY+1, newX-1, newX, pMob, g_Packet, g_nPos );			
		}
		else if( newY > oldY && newX == oldX ) // Y 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY-1, oldX-1, oldX+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobHSector( newY+1, newX-1, newX+1, pMob, g_Packet, g_nPos );			
		}
		else if( newX < oldX && newY > oldY ) // X 감소 Y 증가
		{
			MakeMobOutPacket( pMob );
			SendToMobHSector( oldY-1, oldX-1, oldX+1, pMob, g_Packet, g_nPos );
			SendToMobVSector( oldX+1, oldY, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobHSector( newY+1, newX-1, newX+1, pMob, g_Packet, g_nPos );
			SendToMobVSector( newX-1, newY-1, newY, pMob, g_Packet, g_nPos );			
		}
		else if( newX < oldX && newY == oldY ) // X 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX+1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobVSector( newX-1, newY-1, newY+1, pMob, g_Packet, g_nPos );
		}
		else if( newX < oldX && newY < oldY ) // X감소 Y 감소
		{
			MakeMobOutPacket( pMob );
			SendToMobVSector( oldX+1, oldY-1, oldY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( oldY+1, oldX-1, oldX, pMob, g_Packet, g_nPos );
			
			MakeMobInPacket( pMob );
			SendToMobVSector( newX-1, newY-1, newY+1, pMob, g_Packet, g_nPos );
			SendToMobHSector( newY-1, newX, newX+1, pMob, g_Packet, g_nPos );			
		}
	}
	
	PutHeader( dPACKET_MOVE_MONSTER );
	PutInteger( pMob->idxNum );
	PutInteger( pMob->posX );
	PutInteger( pMob->posY );

	SendToMobArea( pMob, g_Packet, g_nPos );

	// 
	if( pMob->enemy )
	{
		if( IsMobAttackRange( pMob, pMob->enemy ) )
			pMob->lastMoveTime = g_CurrTime + 1000;//+ pMob->moveSpeed2;
	}
}

/*
 *	IsMovable
 */
int IsMovable( sPMOB_DATA pMob, int destX, int destY )
{
	if( destX < 0 || destX >= dONE_MAP_SIZE || destY < 0 || destY >= dONE_MAP_SIZE )
		return MOVABLE_BLOCKEXC;

	if( gWORLD[pMob->mapNum]->mobAttr[destY][destX] )
		return MOVABLE_BLOCKMAP;
//	if( gWORLD[pMob->mapNum]->playerMoveAttr[destY][destX] )
//		return MOVABLE_BLOCKPLAYER;

//	if( IS_MOB_MOVABLE( pMob->mapNum, destX, destY ) )
//		return MOVABLE_BLOCKMAP;

	return MOVABLE_OK;
}


/*
 *	CheckMonsterStop
 */
void CheckMonsterStop( sPMOB_DATA pMob )
{
	if( pMob->enemy )
	{
		if( IsMobAttackRange( pMob, pMob->enemy ) )
		{
			MonsterStop( pMob );
		}
	}
}

/*
 *	MoveMonsterProc
 */
void MoveMonsterProc()
{
	sPMOB_DATA m, next_m;

	int posX, posY;
	int targetX, targetY;
	BYTE moveRange = 1;
	DWORD moveSpeed;

	LIST_WHILE( gMOBS.list, m, next_m , next );

	if( m->enemy )
		moveSpeed = m->moveSpeed2;
	else
		moveSpeed = m->moveSpeed;

	if( m->skillPoint[dEFF_VALUE_DELMOVESPEED] )		// 이동 속도 감소
		moveSpeed += (int)( moveSpeed * 15 );

	switch( m->moveDir )
	{
	case 1:
	case 3:
	case 5:
	case 7:
		moveSpeed += (int)( (float)moveSpeed * 0.4F );
		break;
	}

	if( !MonsterActionProc( m ) )
	{
		LIST_SKIP();
	}

	if( !MonsterDieProc( m ) )
	{
		LIST_SKIP();
	}

	//몬스터 에너지 체크하여 죽은거 체크
	
	if( IS_SRC_MOB( m ) )
	{
		LIST_SKIP();
	}

	// 1 초마다
	if( !( timePulse % 12 ) )
		SKILL_CheckMobDuration( m );

	/*
	 *	스턴 처리
	 */
	if( m->skillPoint[dEFF_VALUE_STURN] )		
	{
		LIST_SKIP();
	}

	if( !m->enemy && m->agroTimer > 0 && m->agroTimer + 20000 < g_CurrTime )
	{
		m->agroLastVict = 0;
		m->agroTimer = 0;
	}

	if( !( timePulse % (12*3) ) )
		MobPointGain( m );

	if( m->enemy )
	{
		if( IsMobAttackRange( m, m->enemy ) )
		{			
			if( m->lastAttackTime + m->attackDelay <= g_CurrTime && m->lastMoveTime + moveSpeed <= g_CurrTime )
			{			
//				MonsterStop( m );
				COMBAT_AttackToPlayer( m, m->enemy );
			}
			LIST_SKIP();
		}
	}
	else
	{
		if( m->AICheckTime + 1000 <= g_CurrTime )
		{
			if( !m->enemy && ( GET_MOB_ATTACKTYPE( m ) == dMOB_ATTACKTYPE_AGRO || GET_MOB_LINKNUM( m ) ) )
			{
				CheckMobEnemyArea( m );
			}
			m->AICheckTime = g_CurrTime;
		}
	}

	if( m->lastMoveTime + moveSpeed < g_CurrTime && m->lastAttackTime + m->attackDelay <= g_CurrTime )
	{
		m->lastMoveTime = g_CurrTime;

		// 이동처리 
		if( !m->enemy ) // 일반 이동
		{
			if( number( 1, 1000 ) <= 100 )
			{
				m->lastMoveTime += ( moveSpeed * 50 );
//				MonsterStop( m );
				LIST_SKIP();
			}

			BYTE tmpRnd = number( 1, 10 );
			
			if( tmpRnd == 1 )
				m->moveDir = number( 0, 7 );	//number( 0, 3 );
			
			posX = m->posX;
			posY = m->posY;
			
			switch( m->moveDir )
			{
			case 0:
				posY+=moveRange;
				break;
			case 1:
				posY+=moveRange;
				posX+=moveRange;
				break;
			case 2:
				posX+=moveRange;
				break;
			case 3:
				posY-=moveRange;
				posX+=moveRange;
				break;
			case 4:
				posY-=moveRange;
				break;
			case 5:
				posY-=moveRange;
				posX-=moveRange;
				break;
			case 6:
				posX-=moveRange;
				break;
			case 7:
				posY+=moveRange;
				posX-=moveRange;
				break;
			}

			if( posX >= 0 && posX < dONE_MAP_SIZE && posY >= 0 && posY < dONE_MAP_SIZE )
			{		
				if( IS_SET( gWORLD[m->mapNum]->mobAttr[posY][posX], dMOB_MAP_BLOCK ) ||
					IS_SET( gWORLD[m->mapNum]->mobAttr[posY][posX], dMOB_MOB_BLOCK ) )
				{
					m->moveDir = number( 0, 7 ); 
					LIST_SKIP();
				}
				
				MoveMonster( m, posX, posY );	
			}
		}
		else
		{
			// 몹몰이 방지
			if( m->agroTimer + 15000 <= g_CurrTime && 
				GetDistance( m->posX, m->posY, GET_POSX( m->enemy ), GET_POSY( m->enemy ) ) > GET_MOB_VISIONOFRANGE( m ) )
			{
				m->moveDir = number( 0, 7 );
				SET_MOB_ENEMY( m, NULL );
				ChangeMobState( m );

				if( m->hp < m->maxHp )
				{
					m->hp = m->maxHp;
					MOB_SendHPInfo( m );
				}

				if( m->mp < m->maxMp )
				{
					m->mp = m->maxMp;
					MOB_SendMPInfo( m );
				}

				LIST_SKIP();
			}
			else
				m->agroTimer = g_CurrTime;

			// 공격 대상이 있을경우
			posX = m->posX;
			posY = m->posY;
			
			GetAttackPosition( m->enemy, m, targetX, targetY );

			// 공격 가능 위치 없음
			if( targetX == -1 || targetY == -1 )
			{
				m->moveDir = number( 0, 7 );
				SET_MOB_ENEMY( m, NULL );
				ChangeMobState( m );
//				MonsterStop( m );
				LIST_SKIP();
			}

			if( posX < targetX )
				posX += moveRange;
			else if( posX > targetX )
				posX -= moveRange;

			if( posY < targetY )
				posY += moveRange;
			else if( posY > targetY )
				posY -= moveRange;

			if( !( posX >= 0 && posX < dONE_MAP_SIZE && posY >= 0 && posY < dONE_MAP_SIZE ) )
				LIST_SKIP();

			// 몬스터에게 막힌 경우
			if( IS_SET( gWORLD[m->mapNum]->mobAttr[posY][posX], dMOB_MOB_BLOCK ) )
			{
				int tmpPosX1 = MAX( posX + 1, dONE_MAP_SIZE-1 );
				int tmpPosX2 = MIN( posX - 1, 0 );

				if( !IS_SET( gWORLD[m->mapNum]->mobAttr[posY][tmpPosX1], dMOB_MOB_BLOCK ) &&
					!IS_SET( gWORLD[m->mapNum]->mobAttr[posY][tmpPosX1], dMOB_MAP_BLOCK ) )
				{
					MoveMonster( m, tmpPosX1, posY );
					LIST_SKIP();
				}
				else if( !IS_SET( gWORLD[m->mapNum]->mobAttr[posY][tmpPosX2], dMOB_MOB_BLOCK ) &&
					!IS_SET( gWORLD[m->mapNum]->mobAttr[posY][tmpPosX2], dMOB_MAP_BLOCK ) )
				{
					MoveMonster( m, tmpPosX2, posY );
					LIST_SKIP();
				}

				int tmpPosY1 = MAX( posY + 1, dONE_MAP_SIZE-1 );
				int tmpPosY2 = MIN( posY - 1, 0 );

				if( !IS_SET( gWORLD[m->mapNum]->mobAttr[tmpPosY1][posX], dMOB_MOB_BLOCK ) &&
					!IS_SET( gWORLD[m->mapNum]->mobAttr[tmpPosY1][posX], dMOB_MAP_BLOCK ) )
				{
					MoveMonster( m, posX, tmpPosY1 );	
					LIST_SKIP();
				}
				else if( !IS_SET( gWORLD[m->mapNum]->mobAttr[tmpPosY2][posX], dMOB_MOB_BLOCK ) &&
					!IS_SET( gWORLD[m->mapNum]->mobAttr[tmpPosY2][posX], dMOB_MAP_BLOCK ) )
				{
					MoveMonster( m, posX, tmpPosY2 );
					LIST_SKIP();
				}
			
				m->agroLastVict = 0;
				m->agroTimer = g_CurrTime;

				m->moveDir = number( 0, 7 );
				SET_MOB_ENEMY( m, NULL );
				ChangeMobState( m );
		//		MonsterStop( m );
				LIST_SKIP();
			}

			if( IS_SET( gWORLD[m->mapNum]->mobAttr[posY][posX], dMOB_MAP_BLOCK ) )
			{
				m->moveDir = number( 0, 7 );
				SET_MOB_ENEMY( m, NULL );
				ChangeMobState( m );
		//		MonsterStop( m );
				LIST_SKIP();
			}

			MoveMonster( m, posX, posY );
		}
	}

	LIST_WHILEEND();
}

/*
 *	FindMobInTile
 */
sPMOB_DATA FindMobInTile( sPMOB_DATA list, int mobNum )
{
	sPMOB_DATA m, next_m;
	
	LIST_WHILE( list, m, next_m , w_next );
	if( m->idxNum == mobNum )
		return m;
	LIST_WHILEEND();

	return NULL;
}

/*
 *	FindMobInArea
 */
sPMOB_DATA FindMobInArea( int mapNum, int posX, int posY, int mobNum )
{
	sPMOB_DATA m;
	
	for( int y = posY - 1; y <= posY + 1; y++ )
	{
		for( int x = posX - 1; x <= posX + 1; x++ )
		{ 
			if( x >= 0 && y >= 0 && x < dONE_TILE_SIZE && y < dONE_TILE_SIZE )
			{
				m = FindMobInTile( gWORLD[mapNum]->mob[y][x], mobNum );

				if( m )
					return m;
			}
		}
	}
	return NULL;
}

/*
 *	MonsterStop
 */
void MonsterStop( sPMOB_DATA pMob )
{
	return;

	PutHeader( dPACKET_MONSTER_STOP );
	PutInteger( pMob->idxNum );
	PutInteger( pMob->posX );
	PutInteger( pMob->posY );
	SendToMobArea( pMob, g_Packet, g_nPos );
}

/*
 *	GetAttackPosition
 */
void GetAttackPosition( sPDESC_DATA pPlayer, sPMOB_DATA pMob, int &aX, int &aY )
{
	aX = -1;
	aY = -1;

	int attackRange = GET_MOB_ATTACKRANGE( pMob );

	int startX, startY;

	for( startY = GET_POSY( pPlayer ) - attackRange; startY <= GET_POSY( pPlayer ) + attackRange; startY++ )
	{
		for( startX = GET_POSX( pPlayer ) - attackRange; startX <= GET_POSX( pPlayer ) + attackRange; startX++ )
		{
			if( startX >= 0 && startX < dONE_MAP_SIZE && startY >= 0 && startY < dONE_MAP_SIZE )
			{
				if( GET_POSX( pPlayer ) == startX && GET_POSY( pPlayer ) == startY )
					continue;

				if( GetDistance( startX, startY, GET_POSX( pPlayer ), GET_POSY( pPlayer ) ) > attackRange )
					continue;

				if( IS_SET( gWORLD[pMob->mapNum]->mobAttr[startY][startX], dMOB_MOB_BLOCK ) ||
					IS_SET( gWORLD[pMob->mapNum]->mobAttr[startY][startX], dMOB_MAP_BLOCK ) )
					continue;

				aX = startX;
				aY = startY;
				return;
			}
		}
	}
	aX = -1;
	aY = -1;

	return;
}



//EOF




