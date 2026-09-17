/**
 * Creature의 Action 관리자
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-12 6:41p $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /GameClient/Src/ActionMgr.h $
 * 
 * 12    04-03-12 6:41p Sckyj
 * 운영자 작업중
 * 
 * 11    03-12-23 9:10p Kjmgo
 * 쓰지 않는 무기에 대한 애니메이션은 생성하지 아니하도록 수정
 * 
 * 10    03-10-17 11:23a Sckyj
 * 
 * 9     03-10-16 4:41p Sckyj
 * sound재생관련 코드 수정
 * 
 * 8     03-10-01 6:40p Kjmgo
 * Skill적용
 * 
 * 7     03-09-12 6:42p Kjmgo
 * Vehicle 관련 수정 및 추가
 * 
 * 6     03-09-07 12:57p Kjmgo
 * 
 * 5     03-09-02 5:49a Kjmgo
 * Monster 추가작업
 * 
 * 4     03-08-31 6:18p Kjmgo
 * Character Action 관련 추가
 * 
 * 3     03-08-30 11:07a Kjmgo
 * Character Animation Data 읽는 부분 추가
 * 
 * 2     03-08-29 9:19a Kjmgo
 * Animation 바꿈
 * 
 * 1     03-08-29 2:20a Kjmgo
 * <HR>
 *
 * @file	ActionMgr.h
 */

#ifndef __ActionMgr_Header__
#define __ActionMgr_Header__

#include "CharacterAction.h"
#include "MonsterAction.h"
#include "VehicleAction.h"
#include "Skill.h"
#include <map>
#include <string>
#include <list>

using std::string;
using std::map;
using std::list;

/**
 *
 */
class CCharacterState
{
public:
	UNDER_MEMORY_MGR( CCharacterState );	

	/**
	 * 생성자
	 */
	CCharacterState();

	/**
	 * 소멸자
	 */
	~CCharacterState();

	/**
	 * Character Motion Data 로드 
	 */
	BOOL Load( const char* szFile );

	/**
	 *
	 */
	Character::SWeapon* GetWeapon( UINT nSex, WORD wWeapon );

	/**
	 *
	 */
	SAction* GetAction( UINT nSex, WORD wWeapon, WORD wMode, WORD wAction, WORD wLevel );

	/**
	 *
	 */
	const char*	GetFileName( DWORD dwId );


private:

	//////////////////////////////////////
	// 관리자
	//////////////////////////////////////
	IW3DAnimationMgr*		m_pAnimationMgr;	///< 애니메이션 파일 관리자( 싱글톤 )

	//////////////////////////////////////
	// 파일 이름 관리자 
	//////////////////////////////////////
	typedef map<DWORD, string>	StringMap;
	StringMap	m_mapString;

	//////////////////////////////////////
	// 무기 동작
	//////////////////////////////////////
//	BOOL		m_bSex;							///< 성별 ( FALSE = 남자 , TRUE = 여자 )
	UINT		m_nSex;							///< 성별 ( 0 : 남자, 1 : 여자, 2 : 운영자 )

	Character::SWeapon*		m_aMaleWeapon[MAX_WEAPON];		///< 남자 무기 동작
	Character::SWeapon*		m_aFemaleWeapon[MAX_WEAPON];	///< 여자 무기 동작
	Character::SWeapon*		m_aMasterWeapon;				///< 운영자 무기 동작 -- 맨손동작

	SAction*				m_pAction;
	
private:

	/**
	 * Character Motion Data 내의 File Name List를 읽어 들인다.
	 */
	BOOL LoadFileName( FILE* fp );

	/**
	 *
	 */
	BOOL LoadWeapon( FILE* fp, int nWeapon, UINT nSex );

	/**
	 *
	 */
	BOOL LoadMode( FILE* fp, Character::SWeapon* pWeapon );

	/**
	 *
	 */
	BOOL LoadAct( FILE* fp, Character::SMotion* pMotion );

	///////////////////////////////////////////////


}; //class CCharacterState

extern CCharacterState*		g_pCharacterState;

/**
 *
 */
class CMonsterState
{
public:
	UNDER_MEMORY_MGR( CMonsterState );

	/**
	 *
	 */
	CMonsterState();

	/**
	 *
	 */
	~CMonsterState();

	/**
	 *
	 */
	BOOL Load( const char* szFile );

	/**
	 *
	 */
	Monster::SMonster*	GetMonster( DWORD dwId );

	/**
	 *
	 */
	const char*	GetFileName( DWORD dwId );

private:

	//////////////////////////////////////
	// 관리자
	//////////////////////////////////////
	IW3DAnimationMgr*		m_pAnimationMgr;	///< 애니메이션 파일 관리자( 싱글톤 )

	//////////////////////////////////////
	// 파일 이름 관리자 
	//////////////////////////////////////
	typedef map<DWORD, string>	StringMap;
	StringMap	m_mapString;

	//////////////////////////////////////
	// 몬스터 Map
	//////////////////////////////////////

	typedef list<Monster::SMonster*>		MonsterList;
	typedef map<DWORD, Monster::SMonster*>	MonsterMap;
	MonsterMap					m_mapMonster;
	MonsterList					m_listMonster;

private:
	/**
	 * Character Motion Data 내의 File Name List를 읽어 들인다.
	 */
	BOOL LoadFileName( FILE* fp );

	/**
	 *
	 */
	void LoadMonster( FILE* fp );

	/**
	 *
	 */
	void MakeMonster( MonsterDummy::SMonster monster );

	///////////////////////////////////////////////


}; //class CMonsterState

extern CMonsterState*		g_pMonsterState;

/**
 *
 */
class CVehicleState
{
public:
	UNDER_MEMORY_MGR( CVehicleState );

	CVehicleState();
	~CVehicleState();

	/**
	 * Character Motion Data 로드 
	 */
	BOOL Load( const char* szFile );

	/**
	 *
	 */
	Vehicle::SWeapon* GetWeapon( UINT nSex, WORD wWeapon );

	/**
	 *
	 */
	SAction* GetAction( UINT nSex, WORD wWeapon, WORD wMode, WORD wAction, WORD wLevel );

	/**
	 *
	 */
	const char*	GetFileName( DWORD dwId );

private:

	//////////////////////////////////////
	// 관리자
	//////////////////////////////////////
	IW3DAnimationMgr*		m_pAnimationMgr;				///< 애니메이션 파일 관리자( 싱글톤 )

	//////////////////////////////////////
	// 파일 이름 관리자 
	//////////////////////////////////////
	typedef map<DWORD, string>	StringMap;
	StringMap	m_mapString;

	//////////////////////////////////////
	// 무기 동작
	//////////////////////////////////////
//	BOOL		m_bSex;										///< 성별 ( FALSE = 남자 , TRUE = 여자 )
	UINT		m_nSex;										///< 0 : 남자, 1 : 여자, 2 : 운영자

	Vehicle::SWeapon		m_aMaleWeapon[MAX_WEAPON];		///< 남자 무기 동작
	Vehicle::SWeapon		m_aFemaleWeapon[MAX_WEAPON];	///< 여자 무기 동작
	Vehicle::SWeapon		m_aMasterWeapon;				///< 운영자 무기 동작

	SAction*				m_pAction;
	
private:

	/**
	 * Character Motion Data 내의 File Name List를 읽어 들인다.
	 */
	BOOL LoadFileName( FILE* fp );

	/**
	 *
	 */
	BOOL LoadWeapon( FILE* fp, int nWeapon, UINT nSex );

	/**
	 *
	 */
	BOOL LoadMode( FILE* fp, Vehicle::SWeapon* pWeapon );

	/**
	 *
	 */
	BOOL LoadAct( FILE* fp, Vehicle::SMotion* pMotion );

	///////////////////////////////////////////////


}; //class CVehicleState

extern CVehicleState*		g_pVehicleState;

#endif //#ifndef __ActionMgr_Header__