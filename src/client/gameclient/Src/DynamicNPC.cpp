/**
 * 메이
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /GameClient/Src/DynamicNPC.cpp $
 * 
 * 4     05-02-18 7:38p Sk8snow2
 * 
 * 3     04-12-09 6:28p Sk8snow2
 * 
 * 2     03-12-11 4:12a Kjmgo
 * 
 * 1     03-12-08 10:18a Kjmgo
 * 
 * <HR>
 *
 * @file	DynamicNPC.cpp
 */

#include "Global.h"
#include "DynamicNPC.h"

IW3DTerrain* g_pTerrain = NULL;


/**
 *
 */
CDynamicNPCMgr::CDynamicNPCMgr()
{

} //CDynamicNPCMgr::CDynamicNPCMgr

/**
 *
 */
CDynamicNPCMgr::~CDynamicNPCMgr()
{
	NPCList::iterator itor = m_listNPC.begin();

	while( itor != m_listNPC.end() )
	{
		SAFE_DELETE( *itor );
		itor = m_listNPC.erase( itor );
	} //while
} //CDynamicNPCMgr::~CDynamicNPCMgr

/**
 *
 */
void CDynamicNPCMgr::Init( IW3DTerrain* pTerrain )
{
	Assert( pTerrain );
	g_pTerrain = pTerrain;
} //CDynamicNPCMgr::Init

/**
 *
 */
BOOL CDynamicNPCMgr::Load( const char* szFile )
{
	Assert( szFile );

	char szPath[256];

	sprintf( szPath, "DynamicNPC/%s", szFile );

	FILE* fp = fopen( szPath, "rt" );

	if( NULL == fp )
	{
		return FALSE;
	} //if

	char szBuffer[256];
	char szKeyword[128];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "End" ) )
		{
			break;
		}
		else if( !strcmp( szKeyword, "Script:" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			CDynamicNPC* pNPC = new CDynamicNPC;

			if( !pNPC ||
				!pNPC->Load( szKeyword ) )
			{
				SAFE_DELETE( pNPC );
				Assert( !"NPC Script 읽기 오류" );
			}
			else
			{
				m_listNPC.push_back( pNPC );
			}//if
		} //if..else..
	} //while

	fclose( fp );
	return TRUE;
} //CDynamicNPCMgr::Load



/**
 *
 */
CDynamicNPC::CDynamicNPC()
{

} //CDynamicNPC::CDynamicNPC

/**
 *
 */
CDynamicNPC::~CDynamicNPC()
{

} //CDynamicNPC::~CDynamicNPC

/**
 *
 */
BOOL CDynamicNPC::Load( const char* szFile )
{
	Assert( szFile );

	char szPath[256];

	sprintf( szPath, "DynamicNPC/%s", szFile );

	FILE* fp = fopen( szPath, "rt" );

	if( NULL == fp )
	{
		return FALSE;
	} //if

	char szBuffer[256];
	char szKeyword[128];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );

		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "End" ) )
		{
			break;
		}
		else if( !strcmp( szKeyword, "Model:" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			m_pModel = MAKE_COMPONENT( Model );
			
			if( !m_pModel ||
				!m_pModel->Load( "DynamicNPC/Model/", szKeyword ) )
			{
				SAFE_RELEASE( m_pModel );
				Assert( !"모델이 없네용" );
			} //if

			char	szAnimation[256];
			strcpy( szAnimation, szKeyword );
			strcpy( szAnimation + strlen( szKeyword ) - 3, "ani" );

			m_pModel->CalculateRadius();
			m_pModel->LoadAnimation( "DynamicNPC/Model/", szAnimation );
		}
		else if( !strcmp( szKeyword, "Start:" ) )
		{
			if( sscanf( szBuffer, "%s %d", szKeyword, &m_nStart ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( !strcmp( szKeyword, "Speed:" ) )
		{
			if( sscanf( szBuffer, "%s %f", szKeyword, &m_fSpeed ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

		}
		else if( !strcmp( szKeyword, "Waypoint:" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}
			//LoadWaypoint( szKeyword );			
		} //if..else..
	} //while

	fclose( fp );
	return TRUE;
} //CDynamicNPC::Load

/**
 *
 */
BOOL CDynamicNPC::LoadWaypoint( const char* szFile )
{
	Assert( szFile );

	char szPath[256];
	sprintf( szPath, "DynamicNPC/Waypoint/%s", szFile );

	FILE* fp = fopen( szPath, "rt" );
	
	if( NULL == fp )
	{
		return FALSE;
	} //if

	char szBuffer[256];
	fgets( szBuffer, 256, fp );

	int nCount = 0;
	if( sscanf( szBuffer, "%d", &nCount ) != 1 )
	{
		Assert( !"sscanf 오류" );
	}


	m_aPath.clear();

	for( int i = 0 ; i < nCount ; i++ )
	{
		int nId = 0;
		Vector3 vPos;
		fgets( szBuffer, 256, fp );
		if( sscanf( szBuffer, "%d %f %f", &nId, &vPos.x, &vPos.y ) != 3 )
		{
			Assert( !"sscanf 오류" );
		}


		vPos.z = g_pTerrain->GetHeight( vPos.x, vPos.y );
		m_aPath.push_back( vPos );
	} //for

	fclose( fp );

	return TRUE;
} //CDynamicNPC::LoadWaypoint