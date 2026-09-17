/**
 * Map 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-19 7:11p $
 * @version	$Revision: 19 $
 *
 * <HR>
 * $Log: /Engine/Src/Map.cpp $
 * 
 * 19    05-02-19 7:11p Sk8snow2
 * 
 * 18    04-12-09 6:28p Sk8snow2
 * 
 * 17    04-02-06 4:13p Kjmgo
 * 카메라 적용
 * 
 * 16    04-01-20 10:24a Kjmgo
 * Map마다, Camera 높이 조절 가능하도록 수정
 * 
 * 15    04-01-11 8:26p Kjmgo
 * 
 *
 * <HR>
 *
 * @file	Map.cpp
 */

#include "Engine_Export.h"
#include "Map.h"

/**
 *
 */
CMap::CMap()
:	m_pQuadTree		( NULL ),
	m_pWorld		( NULL ),
	m_pNPCMgr		( NULL ),
	m_pPotal		( NULL ),
	m_nTime			( 0 )
{
} //CMap::CMap

/**
 *
 */
CMap::~CMap()
{
	CleanEntity();
	CleanupModel();
	CleanupPotal();

	SAFE_DELETE( m_pPotal );
	SAFE_DELETE( m_pNPCMgr );
	SAFE_DELETE( m_pQuadTree );
} //CMap::~CMap

/**
 *
 */
void CMap::CleanEntity()
{
	m_listAlphaEntity.clear();
	m_listEffectEntity.clear();

	if( m_listEntity.empty() )		return;

	EntityList::iterator	itor = m_listEntity.begin();
	while( itor != m_listEntity.end() )
	{
		CEntity*	pEntity = *itor;
		SAFE_DELETE( pEntity );
		itor = m_listEntity.erase( itor );
	} //while

	Assert( m_listEntity.empty() );

} //CMap::CleanEntity

/**
 * 모델 클린업.
 */
void CMap::CleanupModel()
{
/*	if( m_mapModel.empty() )		return;

	ModelMap::iterator	itor = m_mapModel.begin();
	while( itor != m_mapModel.end() )
	{
		ModelMap::iterator	itorNext = itor;
		itorNext++;

		IW3DModel*		pModel = itor->second;
		SAFE_RELEASE( pModel );
		m_mapModel.erase( itor );

		itor = itorNext;
	} //while*/
} //CMap::CleanupModel

/**
 *
 */
void CMap::CleanupPotal()
{
	if( m_listPotal.empty() )	return;

	PotalList::iterator itor = m_listPotal.begin();

	while( itor != m_listPotal.end() )
	{
		SAFE_DELETE( *itor );
		itor = m_listPotal.erase( itor );
	} //while
} //CMap::CleanupPotal

/**
 *
 */
void CMap::CleanPotal()
{
	if( m_pPotal ) m_pPotal->Cleanup();
	if( m_listPotal.empty() )	return;

	PotalList::iterator itor = m_listPotal.begin();
	CPotal*	pPotal = NULL;

	while( itor != m_listPotal.end() )
	{
		pPotal = *itor;

		if( pPotal )
		{
			pPotal->Cleanup();
		} //if

		itor++;
	} //while

} //CMap::CleanPotal

/**
 *
 */
BOOL CMap::Init( IW3DWorld* pWorld )
{
	Assert( pWorld );

	SAFE_DELETE( m_pQuadTree );
	SAFE_DELETE( m_pNPCMgr );
	SAFE_DELETE( m_pPotal );

	CleanEntity();
	CleanupModel();
	CleanupPotal();

	m_pWorld = pWorld;
	m_pCamera = m_pWorld->GetCamera();

	m_pNPCMgr = new CDynamicNPCMgr();
	m_pNPCMgr->Init( m_pWorld->GetTerrain() );

	m_pPotal = new CPotal;
	m_pPotal->SetViewAll();	

	// Init QuadTree	
	m_pQuadTree = new CQuadTree;
	if( !m_pQuadTree || !m_pQuadTree->Init() )
	{
		return FALSE;
	} //if

	return TRUE;
} //CMap::Init

/**
 *
 */
BOOL CMap::Load( const char* szFile )
{
	Assert( szFile );

	char szPath[256];
	sprintf( szPath, "Map/%s.mds", szFile );

	FILE* fp = fopen( szPath, "rb" );

	if( NULL == fp )
	{
		m_pWorld->SetWeather( 0 );
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

		if( !strcmp( szKeyword, "DynamicNPC:" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			m_pNPCMgr->Load( szKeyword );
		}
		else if( !strcmp( szKeyword, "Potal:" ) )
		{
			if( sscanf( szBuffer, "%s %s", szKeyword, szKeyword ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			CPotal* pPotal = new CPotal;
			if( !pPotal ||
				!pPotal->Load( szKeyword ) )
			{
				SAFE_DELETE( pPotal );
				continue;
			} //if

			m_listPotal.push_back( pPotal );
		}
		else if( !strcmp( szKeyword, "Camera:" ) )
		{
			float fHeight = 0.0F;
			if( sscanf( szBuffer, "%s %f", szKeyword, &fHeight ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			m_pWorld->GetCamera()->SetMaxMapZoom( fHeight );
		}
		else if( !strcmp( szKeyword, "Weather:" ) )
		{
			int nWeather = 0;
			if( sscanf( szBuffer, "%s %d", szKeyword, &nWeather ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}


			//nWeather -= 1;

			m_pWorld->SetWeather( nWeather );
		} //if..else..

	} //while


	fclose( fp );

	return TRUE;
} //CMap::Load

/**
 *
 */
BOOL CMap::AddEntity( CEntity* pEntity, BOOL bBack )
{
	Assert( pEntity );
	if( !pEntity )	return FALSE;

	if( bBack )
	{
		m_listEntity.push_back( pEntity );
	}
	else
	{
		m_listEntity.push_front( pEntity );
	} //if..else..

	return TRUE;
} //CMap::AddEntity

/**
 *
 */
void CMap::AddCreature( IW3DCreature* pCreature )
{
	Assert( pCreature );
	if( !pCreature )	return ;

	CPotal*	pPotal = NULL;
	BOOL bFlag = FALSE;

	PotalList::iterator itor = m_listPotal.begin();
	
	while( itor != m_listPotal.end() )
	{
		pPotal = *itor;

		if( pPotal && pPotal->CheckCreature( pCreature ) )
		{
			bFlag = TRUE;
			break;
		} //if

		itor++;
	} //while

	if( !bFlag && m_pPotal )
	{
		m_pPotal->CheckCreature( pCreature );
	} //if
} //CMap::AddCreature

/**
 *
 */
void CMap::UpdateEntities( const DWORD dwTick )
{
	if( m_listEntity.empty() )		return;
	
	if( m_pNPCMgr )
	{
		m_pNPCMgr->Update( dwTick );
	} //if


	BOOL bNewMakeList = m_pQuadTree->MakeList( m_pCamera->GetPosition() );
	
	if( bNewMakeList )
	{
		m_listEffectEntity.clear();
	} //if

	CEntity*	pEntity = NULL;
	ObjectList::iterator itor = g_listObject.begin();
	
	while( itor != g_listObject.end() )
	{
		pEntity = (CEntity*)(*itor);

		if( !pEntity )
		{
			itor = g_listObject.erase( itor );
			continue;
		} //if

		if( !pEntity->Update( dwTick, bNewMakeList ) )
		{
			m_listEffectEntity.push_back( pEntity ); 
		} //if

		itor++;
	} //while
} //CMap::UpdateEntities

/**
 *
 */
void CMap::DrawEntities( const DWORD dwTime, BOOL bFront  )
{
	if( m_listEntity.empty() )		return;
	if( g_listObject.empty() )		return;

	m_dwTime = dwTime;
	DWORD dwTick = dwTime;
	CEntity*	pEntity = NULL;

	m_listAlphaEntity.clear();
	//float	fCameraDistance = m_pCamera->GetFront().DotProduct( m_pCamera->GetPosition() - m_pCamera->GetEye() );
	float	fCameraDistance = m_pCamera->GetFront().DotProduct( ( m_pCamera->GetPosition() - m_pCamera->GetEye() ) );
//	float	fCameraDistance = 0;

	ObjectList::iterator itor = g_listObject.begin();
		
	while( itor != g_listObject.end() )
	{
		pEntity = (CEntity*)(*itor);

		if( !pEntity )
		{
			itor = g_listObject.erase( itor );
			continue;
		} //if

		if( !pEntity->Draw( dwTick, m_pCamera, bFront, fCameraDistance ) )
		{
			// Alpha
			m_listAlphaEntity.push_back( pEntity );
		} //if

		itor++;
		dwTick += 1333;			//< 동작 차이... ^^
	} //while

	if( m_pNPCMgr )
	{
		m_pNPCMgr->Draw();
	} //if

	DrawEntitiesAlpha( TRUE );
	DrawEntitiesEffect( TRUE );

	
} //CMap::DrawEntities

/**
 *
 */
void CMap::DrawEntitiesAlpha( const BOOL bFront )
{
	CEntity*	pEntity = NULL;
	EntityList::iterator	itorAlpha = m_listAlphaEntity.begin();
	
	while( itorAlpha != m_listAlphaEntity.end() )
	{
		pEntity = *itorAlpha;

		if( !pEntity )
		{
			itorAlpha = m_listAlphaEntity.erase( itorAlpha );
			continue;
		} //if

		pEntity->DrawAlpha( m_dwTime );
		
		itorAlpha = m_listAlphaEntity.erase( itorAlpha );
	} //while
} //CMap::DrawEntitiesAlpha

/**
 *
 */
void CMap::DrawEntitiesEffect( const BOOL bFront )
{
	CEntity*	pEntity = NULL;

	EntityList::iterator itor = m_listEffectEntity.begin();
		
	while( itor != m_listEffectEntity.end() )
	{
		//pEntity = (CEntity*)(*itor);
		pEntity = *itor;

		if( pEntity )
		{
			pEntity->DrawEffect( m_nTime );
			itor++;
		}
		else
		{
			itor = m_listEffectEntity.erase( itor );
		} //if			
	} //while
} //CMap::DrawEntitiesEffect

/**
 *
 */
void CMap::DrawCreatures( Vector3& vPos, BOOL bShadow )
{
	CPotal*	pPotal = NULL;
	BOOL	bFlag = FALSE;

	PotalList::iterator itor = m_listPotal.begin();
	while( itor != m_listPotal.end() )
	{
		pPotal = *itor;

		if( pPotal && pPotal->IsInArea( vPos ) )
		{
			bFlag = TRUE;
			pPotal->Render( bShadow );
			break;
		} //if

		itor++;
	} //while

	if( !bFlag && m_pPotal )
	{
		m_pPotal->Render( bShadow );
	} //if
} //CMap::DrawCreatures
