/**
 * Skill
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-24 10:30a $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /GameClient/Src/Skill.h $
 * 
 * 6     03-12-24 10:30a Kjmgo
 * 스킬적용
 * 
 * 5     03-12-24 8:17a Kjmgo
 * 
 * 4     03-10-01 6:40p Kjmgo
 * Skill적용
 * 
 * 3     03-09-23 9:38p Kjmgo
 * Skill 관련 부분 작업
 * 
 * 2     03-09-22 9:07p Kjmgo
 * 스킬 쪽 관련 작업( 마법 관리자 )
 * 
 * 1     03-09-22 5:55p Kjmgo
 * 
 * <HR>
 *
 * @file	Skill.h
 */

#ifndef __Skill_Header__
#define __Skill_Header__

//#include "Global.h"
#include "ActionMgr.h"
#include <map>
#include <string>
#include <list>
#include <vector>

using std::string;
using std::map;
using std::list;

#define MAX_ACTION		10
#define MAX_SKILL		99
#define MAX_SKILLKIND	2
#define MAX_WEAPON2		26
///////////////////////////////////////////////////////////////

#define SEX_MALE						0
#define SEX_FEMALE						1

class CSkill;

/**
 *
 */
class CSkillMgr
{
public:
	MEMORY_UNDER_MGR( CSkillMgr );
	
	/**
	 *
	 */
	CSkillMgr();

	/**
	 *
	 */
	~CSkillMgr();

	/**
	 *
	 */
	void Cleanup();

	/**
	 *
	 */
	void Destory();

	/**
	 *
	 */
	BOOL Init( IW3DWorld* pWorld );

	/**
	 *
	 */
	BOOL IsUseSkill( const char* szName, int nWeapon );

	/**
	 *
	 */
	SAction* Add( IW3DCreature* pAttcker, IW3DCreature* pTarget, const char* szName, BOOL bSex );

	/**
	 *
	 */
	void Update( const DWORD dwTick )
	{
		//-- NULL --//
	} //Update

	/**
	 *
	 */
	void Draw();

private:

	IW3DWorld*			m_pWorld;			///< 월드 
	IW3DAnimationMgr*	m_pAnimationMgr;	///< 

	typedef std::list<int>	WeaponArray;

	/**
	 *
	 */
	struct SkillData
	{
		SAction*	pMaleAction;		///< 동작 
		SAction*	pFemaleAction;		///< 동작 
		char		szEffect[32];	///< 마법 스크립트 

		WeaponArray	aWeapon;		///< 사용가능한 무기
	}; //struct SkillData

	typedef map<string, SkillData*>	SkillDataMap;
	typedef map<DWORD, CSkill*>		SkillList;

	SkillDataMap		m_mapData;			///< 스킬 정보
	SkillList			m_mapSkill;		///< 스킬 

private:

	/**
	 *
	 */
	BOOL	Load( const char* szName );

	/**
	 *
	 */
	BOOL	LoadSkill( FILE* fp );

}; //class CSkillMgr

extern CSkillMgr	g_mgrSkill;

#endif //#ifndef __Skill_Header__