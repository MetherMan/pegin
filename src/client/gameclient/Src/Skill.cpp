/**
 * Skill
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /GameClient/Src/Skill.cpp $
 * 
 * 12    05-02-18 7:38p Sk8snow2
 * 
 * 11    04-12-09 6:28p Sk8snow2
 * 
 * 10    04-01-11 3:05 Sckyj
 * 
 * 9     03-12-26 2:16p Kjmgo
 * 
 * 8     03-12-26 9:54a Kjmgo
 * 
 * 7     03-12-24 10:30a Kjmgo
 * 스킬적용
 * 
 * 6     03-12-24 8:17a Kjmgo
 * 
 * 5     03-10-17 10:35p Kjmgo
 * 폴더..
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
 * @file	Skill.cpp
 */

#include "Skill.h"

CSkillMgr	g_mgrSkill;

/**
 *
 */
CSkillMgr::CSkillMgr()
{
	m_pWorld = NULL;
	m_pAnimationMgr = NULL;

} //CSkillMgr::CSkillMgr

/**
 *
 */
CSkillMgr::~CSkillMgr()
{
	
} //CSkillMgr::~CSkillMgr

/**
 *
 */
void CSkillMgr::Destory()
{
	Cleanup();
} //CSkillMgr::Destory

/**
 *
 */
void CSkillMgr::Cleanup()
{
	SkillDataMap::iterator itor = m_mapData.begin();
	SkillDataMap::iterator itorNext;

	while( itor != m_mapData.end() )
	{
		itorNext = itor;
		itorNext++;

		SkillData* pData = itor->second;
		SAFE_DELETE( pData );

		m_mapData.erase( itor );
		itor = itorNext;
	} //while
} //CSkillMgr::Cleanup

/**
 *
 */
BOOL CSkillMgr::Init( IW3DWorld* pWorld )
{
	Assert( pWorld );
	m_pWorld = pWorld;

	m_pAnimationMgr = MAKE_COMPONENT( AnimationMgr );

	if( !Load( "Skill/Skill.dat" ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CSkillMgr::Init

/**
 *
 */
SAction* CSkillMgr::Add( IW3DCreature* pAttacker, IW3DCreature* pTarget, const char* szName, BOOL bSex )
{
	Assert( pAttacker );
	Assert( pTarget );

	if( !pAttacker ||
		m_mapData.empty() ||
		!pTarget )
	{
		return NULL;
	} //if

	SkillDataMap::iterator itor = m_mapData.find( string( szName ) );

	if( itor != m_mapData.end() )
	{
		SkillData* pData = itor->second;

		if( !pData )		return NULL;

		m_pWorld->AddSkill( pData->szEffect, pAttacker, pTarget );

		if( bSex )
		{
			return pData->pFemaleAction;	
		} 
		else
		{
			return pData->pMaleAction;
		} //if..else..
	} //if

	return NULL;	
} //CSkillMgr::Add

/**
 *
 */
BOOL CSkillMgr::Load( const char* szName )
{
	Assert( szName );

	FILE* fp = fopen( szName, "rb" );

	if( NULL == fp )
	{
		return FALSE;
	} //if

	char szBuffer[256];
	char szKeyword[80];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "[SKILL]" ) )
		{
			LoadSkill( fp );
		}
		else if( !strcmp( szKeyword, "End" ) ||
				 !strcmp( szKeyword, "[END]" ) )
		{
			break;
		} //if..else..

	} //while

	fclose( fp );
	return TRUE;
} //CSkillMgr::Load

/**
 *
 */
BOOL CSkillMgr::LoadSkill( FILE* fp )
{
	Assert( fp );

	char szBuffer[256];
	char szKeyword[80];
	char szSkillName[32];

	SkillData*	pData = new SkillData;

	int nRef = 0;

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "[/SKILL]" ) )
		{
			nRef++;
			break;
		}
		else if( !strcmp( szKeyword, "[NAME]" ) )
		{
			nRef++;
			if( sscanf( szBuffer, "%s %s", szKeyword, szSkillName ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( !strcmp( szKeyword, "[WEAPON]" ) )
		{
			nRef++;
			int nWeapon = 0;
			if( sscanf( szBuffer, "%s %d", szKeyword, &nWeapon ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			pData->aWeapon.push_back( nWeapon );
		}
		else if( !strcmp( szKeyword, "[EFFECT]" ) )
		{
			nRef++;
			if( sscanf( szBuffer, "%s %s", szKeyword, pData->szEffect ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( !strcmp( szKeyword, "[MALE_ACTION]" ) )
		{
			nRef++;
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}
			
			pData->pMaleAction = m_pAnimationMgr->LoadAnimation( "Skill/Ani", szKeyword );

			if( pData->pMaleAction )
			{
				pData->pMaleAction->nStart = 1;
			} //if
		} 
		else if( !strcmp( szKeyword, "[FEMALE_ACTION]" ) )
		{
			nRef++;
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			
			pData->pFemaleAction = m_pAnimationMgr->LoadAnimation( "Skill/Ani", szKeyword );
			
			if( pData->pFemaleAction )
			{
				pData->pFemaleAction->nStart = 1;
			} //if			
		} //if..else..else..
	} //while

	if( nRef < 6 )
	{
		Assert( !"잘못된 스킬 데이타 입니다." );
		SAFE_DELETE( pData );
		return FALSE;
	} //if

	m_mapData.insert( std::make_pair( string( szSkillName ), pData ) );

	return TRUE;
} //CSkillMgr::LoadSkill

/**
 *
 */
BOOL CSkillMgr::IsUseSkill( const char* szName, int nWeapon )
{
	SkillDataMap::iterator itor = m_mapData.find( string( szName ) );

	if( itor == m_mapData.end() )
	{
		// 없다냥!
		return FALSE;
	} //if

	SkillData* pData = itor->second;	

	WeaponArray::iterator itorFind = pData->aWeapon.begin();

	while( itorFind != pData->aWeapon.end() )
	{
		if( nWeapon == *itorFind )
		{
			return TRUE;
			break;	
		} //if
		itorFind++;
	} //while

	return FALSE;	
} //CSkillMgr::IsUseSkill
