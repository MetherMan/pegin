/**
 * Door
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-03-10 6:17p $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /Engine/Src/Door.cpp $
 * 
 * 10    04-03-10 6:17p Paran
 * 
 * 9     03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 8     03-09-29 5:08p Kjmgo
 * 
 * 7     03-09-04 9:21p Kjmgo
 * Scene
 * 
 * 6     03-08-30 3:35p Kjmgo
 * Scene쪽 관련 포인터 정리
 * 
 * 5     03-08-22 11:22a Kjmgo
 * 
 * 4     03-07-18 11:26a Kjmgo
 * 
 * 3     03-07-18 9:36a Kjmgo
 * 
 * 2     03-07-13 3:58p Kjmgo
 *
 * <HR>
 *
 * @file	Door.cpp
 */

#include "Engine_Export.h"
#include "Door.h"

/**
 *
 */
CDoor::CDoor()
{
	m_pPick = NULL;
	m_dwTick = 0;
	m_pScene = NULL;
} //CDoor::CDoor

/**
 *
 */
CDoor::~CDoor()
{
	//-=-=-=-=-=-=-= Release Model -=-=-=-=-=-=-=//

	ModelMap::iterator itorModel = m_mapModel.begin();
	ModelMap::iterator itorModelNext;

	while( itorModel != m_mapModel.end() )
	{
		itorModelNext = itorModel;
		itorModelNext++;

		IW3DModel* pModel = itorModel->second;

		SAFE_RELEASE( pModel );

		m_mapModel.erase( itorModel );

		itorModel = itorModelNext;
	} //while

	//-=-=-=-=-=-=-=-= Release Door -=-=-=-=-=-=-=-=//

	DoorMap::iterator itorDoor = m_mapDoor.begin();
	DoorMap::iterator itorDoorNext;

	while( itorDoor != m_mapDoor.end() )
	{
		itorDoorNext = itorDoor;
		itorDoorNext++;

		SDoor* pDoor = itorDoor->second;

		SAFE_DELETE( pDoor );

		m_mapDoor.erase( itorDoor );

		itorDoor = itorDoorNext;

	} //while
} //CDoor::CDoor

/**
 *
 */
BOOL CDoor::Load( const char* szName )
{
	m_pScene = MAKE_COMPONENT( Scene );

	FILE* fp = fopen( szName, "rb" );

	if( NULL == fp )
	{
		DEBUG_OUT( "Door: 파일이 없습니다." );
		return FALSE;
	} //if

	//-=-=-=-=-= Version -=-=-=-=-=//

	DWORD dwVersion = 0;
	fread( &dwVersion, 4, 1, fp );

	if( dwVersion != 100 )
	{
		Assert( !"Door:버전이 틀립니다." );
		return FALSE;
	} //if

	//-=-=-=-=-= Count -=-=-=-=-=//

	DWORD dwCount = 0;
	fread( &dwCount, 4, 1, fp );
	
	//-=-=-=-=-= Read Door -=-=-=-=-=//

	struct DOOR
	{
		DWORD dwId;
		DWORD dwMoveMap;
		BYTE  btType;
		float fX;
		float fY;
		char  szBuffer[128];
		
		Vector3 vPos;
		float fYaw;
	}; //struct DOOR


	static DOOR	Door; 

	for( int i = 0 ; i < dwCount ; i++ )
	{
		//-=-=-=-= Read -=-=-=-=//
		fread( &Door, sizeof( DOOR ), 1, fp );

		SDoor* pDoor = new SDoor;

		pDoor->btType	= Door.btType;
		pDoor->dwId		= Door.dwId + 1;
		pDoor->dwMoveMap = Door.dwMoveMap + 1;
		pDoor->fX		= Door.fX;
		pDoor->fY		= Door.fY;
		pDoor->vPos		= Door.vPos;
		pDoor->qRot		= Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion();

		pDoor->qRot.SetRotationZ( ANGLE( Door.fYaw ) );
		pDoor->qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion() * pDoor->qRot;

		//-=-=-=-=-= Load Model -=-=-=-=-=//
		pDoor->m_pModel = LoadModel( Door.szBuffer );
		
		if( !pDoor->m_pModel )
		{
			DEBUG_OUT( "Door: Load Model Filed" );
			SAFE_DELETE( pDoor );
			continue;
		} //if

		m_mapDoor.insert( std::make_pair( pDoor->dwId, pDoor ) );
	} //for
	
	fclose( fp );

	return TRUE;
} //CDoor::Load

/**
 *
 */
void CDoor::Update( DWORD dwTick )
{
	m_pPick = NULL;
	m_dwTick += dwTick;
} //if

/**
 *
 */
void CDoor::Draw()
{
	DoorMap::iterator itor = m_mapDoor.begin();
	
	while( itor != m_mapDoor.end() )
	{
		SDoor* pDoor = itor->second;

		if( !pDoor )
		{
			itor++;
			continue;
		} //if

		pDoor->bView = FALSE;
		pDoor->m_pModel->SetWorld( pDoor->vPos, pDoor->qRot );

		if( pDoor->m_pModel->IsVisible() ) //&& m_pPick != pDoor )
		{
			pDoor->bView = TRUE;
		//	pDoor->m_pModel->Render( m_dwTick, TRUE );
		} //if

		itor++;
	} //while
} //CDoor::Draw

/**
 *
 */
IW3DModel* CDoor::LoadModel( const char* szName )
{
	string sModelName = szName;
	ModelMap::iterator	itor = m_mapModel.find( sModelName );

	if( itor != m_mapModel.end() )
	{	// 기존에 있던 모델이라면 그냥 돌려준다.
		IW3DModel*	pModel = itor->second;
		return pModel;
	} //if

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( pModel );

	if( !pModel->Load( "Object", sModelName.c_str() ) )
	{
		DEBUG_OUT( "모델 로드 실패 - " + sModelName.c_str() );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if

	m_mapModel.insert( std::make_pair( sModelName, pModel ) );

	return pModel;
} //CDoor::LoadModel

/**
 *
 */
BOOL CDoor::Pick( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir )
{
	if( !m_pScene )
	{
		return FALSE;
	} //if

	SDoor*		pDoor = NULL;

	DoorMap::iterator itor = m_mapDoor.begin();
	
	while( itor != m_mapDoor.end() )
	{
		pDoor = itor->second;

		if( !pDoor || !pDoor->bView )
		{
			itor++;
			continue;
		} //if

		if( ( vPick - pDoor->vPos ).Length() < 5.0F )
		{
			pDoor->m_pModel->SetWorld( pDoor->vPos, pDoor->qRot );

			if( pDoor->m_pModel->Pick( vOrig, vDir ) )
			{
				m_pPick = pDoor;
				return TRUE;
			} //if
		} //if

		itor++;
	} //while

	return FALSE;
} //CDoor::Pick