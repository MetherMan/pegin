/**
 * State For Creature 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-08-19 11:52a $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /GameClient/Src/CreatureState.h $
 * 
 * 10    03-08-19 11:52a Kjmgo
 * 
 * 9     03-08-16 12:38p Sckyj
 * 
 * 8     03-08-13 5:26p Kjmgo
 * 
 * 7     03-08-13 10:11a Kjmgo
 * Vehicle 부분 수정
 * 
 * 6     03-08-10 9:32p Admin
 * 
 * 5     03-08-08 2:19p Admin
 * 
 * 4     03-07-21 7:40p Kjmgo
 * 
 * 3     03-07-19 11:18a Kjmgo
 * 
 * 2     03-07-18 3:30p Kjmgo
 * 
 * 1     03-07-18 12:33p Kjmgo
 *
 * <HR>
 *
 * @file	CreatureState.h
 */

#ifndef __CreatureState_Header__
#define __CreatureState_Header__

#include "Global.h"
#include <string>
#include <map>
#include <list>

using std::string;
using std::map;
using std::list;

struct SMotion;
struct SWeapon;

#define	PEACEMODETIME	4000	

#define MAGIC_NORMAL	101
#define MAGIC_ATTACK	102
#define MAGIC_CAST		103

#define SET_CHAR( c, d ) \
		sprintf( c, "%s", d );

/**
 *
 */
class CCreatureState
{
public:
	UNDER_MEMORY_MGR( CCreatureState );

	/**
	 *
	 */
	CCreatureState();

	/**
	 *
	 */
	~CCreatureState();

	/**
	 * State 파일을 연다.
	 */
	BOOL Load( const char* szName );

	/**
	 * Update
	 */
	void Update( DWORD dwTick );

	/**
	 * 무기를 바꾼다.
	 */
	void SetWeapon( const int nWeapon );

	/**
	 *
	 */
	void SetWithHorse( const BOOL bUse )
	{
		m_bUseHorse = bUse;
		if( bUse )
		{
			SET_CHAR( m_szHorse, "말" );
		}
		else
		{
			SET_CHAR( m_szHorse, "" );
		} //if
	} //SetWithHorse

	/**
	 *
	 */
	void SetWithShield( const BOOL bUse )
	{
		if( bUse && m_bCanWithShield )
		{
		//	sprintf(
			SET_CHAR( m_szShield, "방패" );
		}
		else
		{
			SET_CHAR( m_szShield, "" );
		} //if..else..
	} //SetWithShield

	/**
	 * 무기를 장착 해제 한다.
	 */
	void SetOffWeapon( const int nWeapon );

	/**
	 * 상태를 바꾼다.
	 */
	void SetState( const int nState );

	/**
	 *
	 */
	void Attack()
	{
		m_bPeace = FALSE;
		m_bMagic = FALSE;
		m_dwTick = 0;
	} //if

	/**
	 *
	 */
	void BeginMagic()
	{
		DEBUG_OUT( "CreatureState::BeginMagic() == Run" );
		m_bMagic = TRUE;
	} //if

	/**
	 *
	 */
	void EndMagic()
	{
		DEBUG_OUT( "CreatureState::EndMagic() == Run" );
		m_bMagic = FALSE;
	}
	
	/**
	 * 현재 악숀(!)을 가져온다.
	 */
	const char* GetAction();

	/**
	 * 상태를 가져온다.
	 */
	int	GetState();

private:

	typedef map<string, SWeapon*> WeaponMap;
	WeaponMap	m_mapWeapon;		///< Weapon List
	SWeapon*	m_pWeapon;			///< 현재 들고 있는 Weapon
	SWeapon*	m_pMagic;			///< 현재 들고 있는 Magic

	DWORD		m_dwTick;			///< 내부 시간

	int			m_nState;			///< 현재 상태 
	int			m_nWeapon;			///< 현재 무기 상태
	BOOL		m_bPeace;			///< 평화 모드 인지의 여부
	BOOL		m_bMagic;			///< 마법 동작인지의 여부
	BOOL		m_bUseHorse;
	BOOL		m_bCanWithShield;	///< 방패를 착용 할수 있는가의 여부

	
	char		m_szAction[80];
	char		m_szWeapon[80];
	char		m_szState[80];
	char		m_szHorse[10];
	char		m_szShield[10];

	//==================================================
	//				Read Script Fuction
	//==================================================

	/**
	 *	
	 */
	BOOL ReadWeapon( FILE* fp );

	/**
	 *
	 */
	BOOL ReadMotion( FILE* fp, SWeapon* pWeapon );

	/**
	 *
	 */
	void AddWeapon( SWeapon* pWeapon );

	//==================================================
	//				State Setting Fuction
	//==================================================

	/**
	 *
	 */
	void SetStateForMagic( const int nState );

}; //class CCreatureState

/**
 *
 */
struct SMotion
{
	UNDER_MEMORY_MGR( SMotion );

	struct Motion
	{
		UNDER_MEMORY_MGR( Motion );

		DWORD dwId;
		char szName[40];

		//string sName;
	}; //struct Motion

	//string	sName;				///< 동작 이름
	char	szName[80];

	int		nMotionCount;		///< 동작 갯수
	int		nEmoticonCount;		///< 이모티콘 갯수

	DWORD	dwEmoticonTime;		///< 이모티콘이 발생되는 시간

	Motion* apMotion;			///< 모션 동적 배열 
	Motion* apEmoticon;			///< 이모티콘 동적 배열 

	/**
	 *
	 */
	SMotion()
	{
		nMotionCount = nEmoticonCount = dwEmoticonTime	= 0;
		apMotion	 = apEmoticon	= NULL;
	} //SMotion

	/**
	 *
	 */
	~SMotion()
	{
		ReleaseMotion();
	} //~SMotion

	/**
	 *
	 */
	void ReleaseMotion()
	{
		SAFE_DELETE_ARRAY( apMotion );	
		SAFE_DELETE_ARRAY( apEmoticon );
	} //ReleaseEmoticon
}; //struct SMotion

/**
 *
 */
struct SWeapon
{
	UNDER_MEMORY_MGR( SWeapon );

	char	szName[80];
//	string	sName;			///< 무기 종류

	typedef map<string, SMotion*> MotionMap;
	MotionMap	mapMotion;

	/**
	 *
	 */
	~SWeapon()
	{
		ReleaseMotion();
	} //~SWeapon

	/**
	 *
	 */
	void AddMotion( SMotion* pMotion )
	{
		Assert( pMotion );

		if( pMotion )
		{
			// 이미 같은 이름의 모션이 있는지 검사
			MotionMap::iterator itor = mapMotion.find( string( pMotion->szName ) );

			if( itor == mapMotion.end() )
			{
				// 같은 이름의 모션이 없다면!
				// 등록하자
				mapMotion.insert( std::make_pair( string( pMotion->szName ), pMotion ) );
			}
			else
			{
				// 같은 이름의 모션이 있다면, 최근에 들어온걸 지우자
				SAFE_DELETE( pMotion );
			} //if..else..			
		} //if
	} //AddMotion

	/**
	 *
	 */
	SMotion* GetMotion( string sMotion )
	{
		MotionMap::iterator itor = mapMotion.find( sMotion );

		if( itor == mapMotion.end() )
		{
			// 같은 이름의 모션이 없다면!
			return NULL;
		}
		else
		{
			return itor->second;
		} //if..else..					
	} //GetMotion

	/**
	 *
	 */
	void ReleaseMotion()
	{
		MotionMap::iterator itor = mapMotion.begin();
		MotionMap::iterator itorNext;

		while( itor != mapMotion.end() )
		{
			itorNext = itor;
			itorNext++;

			SMotion* pMotion = itor->second;
			
			SAFE_DELETE( pMotion );

			mapMotion.erase( itor );
			itor = itorNext;
		} //while
	} //ReleaseMotion
}; //struct SWeapon

#endif //__CreatureState_Header__