/**
 * State For Creature 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 18 $
 *
 * <HR>
 * $Log: /GameClient/Src/CreatureState.cpp $
 * 
 * 18    05-02-18 7:38p Sk8snow2
 * 
 * 17    04-12-09 6:28p Sk8snow2
 * 
 * 16    03-08-19 11:52a Kjmgo
 * 
 * 15    03-08-17 4:52p Paran
 * 
 * 14    03-08-17 4:30p Paran
 * 
 * 13    03-08-15 6:01a Kjmgo
 * 
 * 12    03-08-14 5:46p Kjmgo
 * 
 * 11    03-08-14 3:44p Kjmgo
 * 
 * 10    03-08-14 3:14p Kjmgo
 * 
 * 9     03-08-14 3:07p Kjmgo
 * 
 * 8     03-08-13 10:11a Kjmgo
 * Vehicle 부분 수정
 * 
 * 7     03-08-10 9:32p Admin
 * 
 * 6     03-08-08 2:19p Admin
 * 
 * 5     03-07-25 4:41p Kjmgo
 * 
 * 4     03-07-21 7:40p Kjmgo
 * 
 * 3     03-07-19 11:18a Kjmgo
 * 
 * 2     03-07-18 3:30p Kjmgo
 * 
 *
 * <HR>
 *
 * @file	CreatureState.cpp
 */

#include "CreatureState.h"

#define CHECK_KEYWORD( s )	!strcmp( szKeyword, s )

/**
 *
 */
CCreatureState::CCreatureState()
{
	SET_CHAR( m_szHorse, "" );
	SET_CHAR( m_szWeapon, "맨손" );
	SET_CHAR( m_szState, "정지" );
	SET_CHAR( m_szShield, "" );
	SET_CHAR( m_szAction, "맨손_정지" );
	//m_sState	= "정지";
	m_pMagic	= NULL;
	m_pWeapon	= NULL;
	m_bCanWithShield	 = FALSE;
	m_bMagic	= FALSE;
	m_bPeace	= FALSE;
} //CCreatureState::CCreatureState

/**
 *
 */
CCreatureState::~CCreatureState()
{
	WeaponMap::iterator itor = m_mapWeapon.begin();
	WeaponMap::iterator itorNext;

	while( itor != m_mapWeapon.end() )
	{
		itorNext = itor;
		itorNext++;

		SAFE_DELETE( itor->second );

		m_mapWeapon.erase( itor );

		itor = itorNext;
	} //while
} //CCreatureState::~CCreatureState

/**
 *
 */
void CCreatureState::Update( DWORD dwTick )
{	
	if( m_nState != STATE_MOVE )
	{
		// Move상태가 아닐때에만 Timer 가동 
		m_dwTick += dwTick;
	} //if

	if( !m_bPeace )
	{
		// 만약 평화 모드가 아닌 전투 모드 라면, PEACEMODETIME 후에 푼다.
		if( m_dwTick >= PEACEMODETIME )
		{
			m_bPeace = TRUE;
			m_dwTick -= PEACEMODETIME;
		} //if
	} //if
} //CCreatureState::Update


///////////////////////////////////////////////////////
//
//		Change State
//
///////////////////////////////////////////////////////


/**
 * 무기를 바꾼다.
 */
void CCreatureState::SetWeapon( const int nWeapon )
{
	if( dITEMTYPE_SHIELD == nWeapon )
	{
		SetWithShield( TRUE );
		return ; 
	} //if

	m_nWeapon = nWeapon;
	m_bCanWithShield = FALSE;
	
	switch( nWeapon )
	{
	case dITEMTYPE_NONE:			SET_CHAR( m_szWeapon, "맨손" );		m_bCanWithShield = TRUE; break;
	case dITEMTYPE_ONEHANDSWORD:	SET_CHAR( m_szWeapon, "한손칼" );	m_bCanWithShield = TRUE; break;
	case dITEMTYPE_TWOHANDSWORD:	SET_CHAR( m_szWeapon, "양손칼" );	SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_DUALSWORD:		SET_CHAR( m_szWeapon, "쌍칼" );		SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_STAFF:			SET_CHAR( m_szWeapon, "지팡이" );	SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_ONEHANDAXE:		SET_CHAR( m_szWeapon, "한손도끼" );	m_bCanWithShield = TRUE; break;
	case dITEMTYPE_TWOHANDAXE:		SET_CHAR( m_szWeapon, "양손도끼" );	SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_DUALAXE:			SET_CHAR( m_szWeapon, "쌍도끼" );	SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_ONEHANDBLUNT:	SET_CHAR( m_szWeapon, "한손둔기" );	m_bCanWithShield = TRUE; break;
	case dITEMTYPE_TWOHANDBLUNT:	SET_CHAR( m_szWeapon, "양손둔기" );	SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_DUALBLUNT:		SET_CHAR( m_szWeapon, "쌍둔기" );	SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_DONPA:			SET_CHAR( m_szWeapon, "돈파" );		SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_SHORTBOW:		SET_CHAR( m_szWeapon, "단궁" );		SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_LONGBOW:			SET_CHAR( m_szWeapon, "장궁" );		SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_DDABALBOW:		SET_CHAR( m_szWeapon, "연노" );		SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_CROSSBOW:		SET_CHAR( m_szWeapon, "석궁" );		SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_SPHERE:			SET_CHAR( m_szWeapon, "창" );		SET_CHAR( m_szShield, "" ); break;
	case dITEMTYPE_BONG:			SET_CHAR( m_szWeapon, "봉" );		SET_CHAR( m_szShield, "" ); break;
	}; //switch

	WeaponMap::iterator itor = m_mapWeapon.find( m_szWeapon );

	if( itor != m_mapWeapon.end() )
	{	// 존재 한다면..
		m_pWeapon = itor->second;
	}
	else
	{
		m_pWeapon = NULL;
	}//if

	DEBUG_OUT( "Motion - Weapon : " + m_szWeapon );
} //CCreatureState::SetWeapon

/**
 *
 */
void CCreatureState::SetOffWeapon( const int nWeapon )
{
	if( dITEMTYPE_SHIELD == nWeapon )
	{
		SetWithShield( FALSE );
		return ; 
	} //if

	SET_CHAR( m_szWeapon, "맨손" );
	
	WeaponMap::iterator itor = m_mapWeapon.find( m_szWeapon );

	if( itor != m_mapWeapon.end() )
	{	// 존재 한다면..
		m_pWeapon = itor->second;
	}
	else
	{
		m_pWeapon = NULL;
	}//if
} //CCreatureState::SetOffWeapon

/**
 * 상태를 바꾼다.
 */
void CCreatureState::SetState( const int nState )
{
	if( m_bMagic )
	{
		SetStateForMagic( nState );
		return ;
	} //if

	m_nState = nState;

	switch( nState )
	{
	case STATE_MOVE:		sprintf( m_szState, "%s이동", m_szShield );		break;
	case STATE_STOP:		SET_CHAR( m_szState, "정지" );		break;
	case STATE_ATTACK:		SET_CHAR( m_szState, "공격" );		break;
	case STATE_MAGIC:		SET_CHAR( m_szState, "마법" );		break;
	case STATE_DEAD:		SET_CHAR( m_szState, "사망" );		SET_CHAR( m_szAction, "사망" );	return;
	}; //switch

	if( m_bUseHorse )
	{
		sprintf( m_szState, "%s말", m_szState );
	} //if

	if( !m_pWeapon )
	{
		WeaponMap::iterator itor = m_mapWeapon.find( m_szWeapon );

		if( itor != m_mapWeapon.end() )
		{	// 존재 한다면..
			m_pWeapon = itor->second;
		}
		else
		{
			return ;
		}//if
	} //if

	SMotion* pMotion = m_pWeapon->GetMotion( m_szState );

	if( pMotion && pMotion->nMotionCount > 0 )
	{
		// Motion Count 만큼 랜덤하게 돌린다.
		int nRandom = random( pMotion->nMotionCount );
		sprintf( m_szAction, "%s_%s", m_szWeapon, pMotion->apMotion[nRandom].szName );
	
	}
	else
	{
		sprintf( m_szAction, "맨손_%s", m_szState );
	}//if	
} //CCreatureState::SetState

/**
 *
 */
void CCreatureState::SetStateForMagic( const int nState )
{
	char szState[10];
	SET_CHAR( szState, "일반" );
	
	switch( nState )
	{
	case STATE_MOVE:		SET_CHAR( szState, "이동" );		break;
	case STATE_STOP:		SET_CHAR( szState, "정지" );		break;
	case STATE_MAGIC:		SET_CHAR( szState, "마법" );		break;
	case STATE_DEAD:		SET_CHAR( m_szState, "사망" ); return;		break;
	case MAGIC_CAST:		SET_CHAR( szState, "시전" );		break;
	case MAGIC_NORMAL:		SET_CHAR( szState, "일반" );		break;
	case MAGIC_ATTACK:		SET_CHAR( szState, "공격" );		break;
	}; //switch

	if( m_bUseHorse )
	{
		sprintf( m_szAction, "맨손_%s말", szState );
		return ;
	}

	if( !m_pMagic )
	{
		WeaponMap::iterator itor = m_mapWeapon.find( "마법" );

		if( itor != m_mapWeapon.end() )
		{	// 존재 한다면..
			m_pMagic = itor->second;
		}
		else
		{
			return ;
		}//if
	} //if

	SMotion* pMotion = m_pMagic->GetMotion( szState );

	if( pMotion && pMotion->nMotionCount > 0  )
	{
		// Motion Count 만큼 랜덤하게 돌린다.
		int nRandom = random( pMotion->nMotionCount );
		
		sprintf( m_szAction, "마법_%s", pMotion->apMotion[nRandom].szName );
	}
	else
	{
		sprintf( m_szAction, "맨손_%s", szState );
	} //if..else..
	
} //CCreatureState::SetStateForMagic

/**
 * 현재 악숀(!)을 가져온다.
 */
const char* CCreatureState::GetAction()
{
//	DEBUG_OUT( "Motion : " + m_sAction.c_str() );
	return m_szAction;
} //CCreatureState::GetAction

///////////////////////////////////////////////////////
//
//		Script Load 
//
///////////////////////////////////////////////////////
/**
 *
 */
BOOL CCreatureState::Load( const char* szName )
{
	FILE* fp = fopen( szName, "rt" );

	if( NULL == fp )
	{
		Assert( !"CreatureState : File Not Found." );
		return FALSE;
	} //if

	char szBuffer[256];
	char szKeyword[80];

	BOOL bFlag = TRUE;

	//-=-=-=-=-=-= Read File -=-=-=-=-=-=//
	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );	

		if( CHECK_KEYWORD( "[BEGIN]" ) )
		{
			if( !ReadWeapon( fp ) )
			{
				bFlag = FALSE;
				break;
			} //if
		} //if
	} //while
	
	fclose( fp );

	return bFlag;
} //CCreatureState::Load

/**
 *
 */
BOOL CCreatureState::ReadWeapon( FILE* fp )
{
	Assert( fp );
	
	char szBuffer[256];			///< 버퍼
	char szKeyword[80];			///< 키워드 

	BOOL bFlag = FALSE;			///< 스크립트가 제대로 된놈인지.

	SWeapon* pWeapon = new SWeapon;	

	if( !pWeapon )
	{
		return FALSE;
	} //if

	//-=-=-=-=-=-= Read Weapon -=-=-=-=-=-=//
	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( CHECK_KEYWORD( "[NAME]" ) )
		{
			//-=-=-=-=-=-=-= Weapon Name -=-=-=-=-=-=-=-=//
			char szName[80];
			if( sscanf( szBuffer, "%s %s", szKeyword, szName ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			SET_CHAR( pWeapon->szName, szName );
			
			bFlag = TRUE;
		}
		else if( CHECK_KEYWORD( "[MOTIONBEGIN]" ) )
		{
			//-=-=-=-=-=-=-= Motion Begin -=-=-=-=-=-=-=-=//
			if( !ReadMotion( fp, pWeapon ) )
			{
				bFlag = FALSE;
			} //if
		}
		else if( CHECK_KEYWORD( "[END]" ) )
		{
			//-=-=-=-=-=-=-= End -=-=-=-=-=-=-=//
			break;
		}
		//if
	} //while

	// 제대로 된 스크립트라면.
	if( bFlag && pWeapon )
	{
		// Weapon을 등록한다.
		AddWeapon( pWeapon );
		return TRUE;
	} //if

	SAFE_DELETE( pWeapon );

	return FALSE;
} //CCreatureState::ReadWeapon

/**
 *
 */
BOOL CCreatureState::ReadMotion( FILE* fp, SWeapon* pWeapon )
{
	Assert( fp );
	Assert( pWeapon );

	char szBuffer[256];			//< 버퍼
	char szKeyword[80];			//< 키워드 
	int  nCount = 0;			//< Motion 갯수
	int  nId = 0;				//< Motion ID

	BOOL bFlag = FALSE;			//< 스크립트가 제대로 된놈인지. 체크 하는 플랙

	SMotion* pMotion = new SMotion;

	if( !pMotion ) 
	{
		return FALSE;
	} //if

	//-=-=-=-=-=-= Read Motion -=-=-=-=-=-=-=//
	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( CHECK_KEYWORD( "[NAME]" ) )
		{
			//-=-=-=-=-=-= 이름 -=-=-=-=-=-=//
			char szName[80];
			if( sscanf( szBuffer, "%s %s", szKeyword, szName ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			SET_CHAR( pMotion->szName, szName );
			bFlag = TRUE;
		}
		else if( CHECK_KEYWORD( "[COUNT]" ) )
		{
			// 갯수 얻어오기
			if( sscanf( szBuffer, "%s %d", szKeyword, &nCount ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			if( nCount <= 0 )
			{
				bFlag = FALSE;
				continue;
			} //if

			// 갯수 만큼 배열을 생성한다.
			pMotion->apMotion = new SMotion::Motion[nCount];
			pMotion->nMotionCount = nCount;

			if( !pMotion->apMotion )
			{
				bFlag = FALSE;
			} //if
			bFlag = TRUE;
		}
		else if( CHECK_KEYWORD( "[MOTION]" ) )
		{
			// 동작
			if( nCount <= 0 && nId >= nCount )
			{
				bFlag = FALSE;
				DEBUG_OUT( "CreatureState : Script Error [Check_keyword: \"[MOTION]\"] " );
				continue;
			} //if

			SMotion::Motion* p = &pMotion->apMotion[nId];

			// 생성된 배열에서 차례대로 Motion을 할당 받는다.
			if( p )
			{
				char szName[80];
				if( sscanf( szBuffer, "%s %s", szKeyword, szName ) != 2 )
				{
					Assert( !"sscanf 오류" );
				}

				p->dwId = nId;
				SET_CHAR( p->szName, szName );
				nId++;
			} //if
		}
		else if( CHECK_KEYWORD( "[MOTIONEND]" ) )
		{
			//-=-=-=-=-=-= End -=-=-=-=-=//
			break;
		} //if..else..else..else..

	} //while

	// 제대로 된 스크립트라면.
	if( bFlag && pMotion )
	{
		// Motion을 등록한다.
		pWeapon->AddMotion( pMotion );
		return TRUE;
	} //if

	SAFE_DELETE( pMotion );

	return FALSE;
} //CCreatureState::ReadMotion

/**
 *
 */
void CCreatureState::AddWeapon( SWeapon* pWeapon )
{
	Assert( pWeapon );

	if( pWeapon )
	{	
		// 이미 같은 이름의 Weapon이 있는지 검사.
		WeaponMap::iterator itor = m_mapWeapon.find( pWeapon->szName );

		// 만약 있지 않다면 ( 없다면 )
		if( itor == m_mapWeapon.end() )
		{
			// 등록한다.
			m_mapWeapon.insert( std::make_pair( string( pWeapon->szName ), pWeapon ) );
		}
		else
		{
			// 현재 들어온걸 지운다.
			SAFE_DELETE( pWeapon );
		} //if..else..
	} //if
} //CCreatureState::AddWeapon

