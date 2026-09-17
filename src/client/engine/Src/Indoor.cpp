/**
 * 내부 구현+_+
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-01 10:25a $
 * @version	$Revision: 14 $
 *
 * <HR>
 * $Log: /Engine/Src/Indoor.cpp $
 * 
 * 14    03-12-01 10:25a Kjmgo
 * 코드 정리
 * 
 * 13    03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 12    03-10-23 12:41a Kjmgo
 * 집안 내부에서는 Object Culling을 하지 않는다.
 * 
 * 11    03-09-30 5:39p Kjmgo
 * 알파값 적용 부분을 뻇음
 * 
 * 10    03-09-30 4:14p Kjmgo
 * 
 * 9     03-09-24 1:38a Kjmgo
 * 
 * 8     03-09-24 1:13a Kjmgo
 * 
 * 7     03-07-25 11:14a Kjmgo
 * 
 * 6     03-07-17 10:02p Kjmgo
 * 
 * 5     03-07-10 10:36a Kjmgo
 * 
 * 4     03-06-27 10:43a Kjmgo
 * 
 * 3     03-06-24 5:38p Kjmgo
 * 
 * 2     03-06-24 3:23a Kjmgo
 * <HR>
 *
 * @file	Indoor.h
 */

#include "Engine_Export.h"
#include "Loading.h"
#include "Indoor.h"


/**
 * 생성자
 */
CIndoor::CIndoor()
{
	m_pCamera	= NULL;
	m_pWorld	= NULL;
	m_pTerrain	= NULL;
	m_pbMovable	= NULL;
	m_pPathFind = NULL;
} //CIndoor::CIndoor

/**
 * 소멸자
 */
CIndoor::~CIndoor()
{
	Cleanup();
	SAFE_DELETE( m_pPathFind );
	SAFE_FREE( m_pbMovable );

	SAFE_RELEASE( m_pTerrain );
} //CIndoor::CIndoor

/**
 *
 */
void CIndoor::Cleanup()
{
	if( !m_listEntity.empty() )
	{
		EntityList::iterator	itor = m_listEntity.begin();
		while( itor != m_listEntity.end() )
		{
			CEntity*	pEntity = *itor;

			if( pEntity->m_nType == CEntity::EFFECTOBJECT )
			{
				SAFE_RELEASE( pEntity->m_pEffectObject );
			} //if
			SAFE_DELETE( pEntity );
			itor = m_listEntity.erase( itor );
		} //while
	} //if

	if( !m_mapModel.empty() )
	{
		ModelMap::iterator	itorModel = m_mapModel.begin();
		while( itorModel != m_mapModel.end() )
		{
			ModelMap::iterator	itorNext = itorModel;
			itorNext++;

			IW3DModel*		pModel = itorModel->second;
			SAFE_RELEASE( pModel );
			m_mapModel.erase( itorModel );

			itorModel = itorNext;
		} //while
	} //if
} //

/**
 *
 */
void CIndoor::Init( IW3DWorld* pWorld, IW3DCamera* pCamera )
{
	Assert( pWorld );
	SAFE_RELEASE( m_pTerrain );

	m_pWorld = pWorld;
	m_pCamera = pCamera;

	DWORD dwSize = 10;

	m_pbMovable = (BYTE*)_MemAlloc( dwSize * 2 * dwSize * 2 );
//	CHECK_PTR_RETURN( m_pbMovable );
	_ZeroMemory( m_pbMovable, dwSize * 2 * dwSize * 2 );

	m_pPathFind = new CPathFind( dwSize * 2 );
//	CHECK_PTR_RETURN( m_pPathFind );
	m_pPathFind->SetMovable( m_pbMovable );
} //CIndoor::Init
 
/**
 *
 */
BOOL CIndoor::Load( IW3DScene* pScene, const char* szFileName )
{
	Assert( m_pWorld );
	SAFE_RELEASE( m_pTerrain );
	SAFE_DELETE( m_pPathFind );
	SAFE_FREE( m_pbMovable );

	CLoading	Loading;

	if( !Loading.Init( pScene, "land9999" ) )
	{
	//	Assert( !"로딩 화면 초기화 실패" );
	//	return FALSE;
	} //if

	string	sFileName = string( "Map/" ) + szFileName + ".map";

	FILE*	fp = fopen( sFileName.c_str(), "rb" );
	if( !fp )
	{
		Assert( !"파일 열기 실패" );
		return FALSE;
	} //if

	Cleanup();

	ArchiveFileReader	ar( fp );

	DWORD	dwSize;
	DWORD	dwVersion;
	char	szTextureName[32];

	Loading.Render( 5 );

	ar.Serialize( &dwVersion, sizeof( DWORD ) );

	if( dwVersion != 100 && dwVersion != 101 && dwVersion != 102 )
	{
		ar.Close();
		DEBUG_OUT( "Load Map Error !! : It's Wrong Version. " );
		return FALSE;
	} //if

	ar.Serialize( &dwSize, sizeof( DWORD ) );
	ar.Serialize( szTextureName, 32 );

	m_wSize = dwSize * 4;		// 월드 크기.

	Assert( NULL == m_pTerrain );
	Assert( NULL == m_pbMovable );
	Assert( NULL == m_pPathFind );

	m_pTerrain = MAKE_COMPONENT( Terrain );
	if( !m_pTerrain->Init( dwSize, szTextureName, FALSE ) )
	{
		Assert( !"지형 초기화 실패" );
		return FALSE;
	} //if

	m_pbMovable = (BYTE*)_MemAlloc( dwSize * 4 * dwSize * 4 );
	CHECK_PTR_RETURN( m_pbMovable );
	_ZeroMemory( m_pbMovable, dwSize * 4 * dwSize * 4 );

	m_pPathFind = new CPathFind( dwSize * 4 );
	CHECK_PTR_RETURN( m_pPathFind );
	m_pPathFind->SetMovable( m_pbMovable );

	ar.Serialize( m_pTerrain->GetHeightMap(),	dwSize * dwSize * sizeof( float ) );
	ar.Serialize( m_pTerrain->GetTextureMap(),	dwSize * dwSize * sizeof( BYTE ) );
	ar.Serialize( m_pTerrain->GetColorMap(),	dwSize * dwSize * sizeof( DWORD ) );

	m_pTerrain->SetLevel(0);
	
	ar.Serialize( m_pTerrain->GetObjectHeightMap(),	dwSize * dwSize * 16 * sizeof( float ) );
	ar.Serialize( m_pbMovable,					dwSize * 4 * dwSize * 4 * sizeof( BYTE ) );
	ar.Seek( ar.GetPosition() + dwSize * dwSize );

	m_pPathFind->SetMovable( m_pbMovable );

	/*
		Indoor에서는 layer를 단한개만 쓴다...
		그러므로 나머지 것들은 그냥 Skip한다.
	*/
	for( int i = 1 ; i < MAX_LAYER ; i++ )
	{
		ar.Seek( ar.GetPosition() + dwSize * dwSize * 16 * sizeof( float ) );
		ar.Seek( ar.GetPosition() + dwSize * 4 * dwSize * 4 * sizeof( BYTE ) );
		ar.Seek( ar.GetPosition() + dwSize * dwSize );
	} //for

	m_pTerrain->CalculateNormals();

	Loading.Render( 10 );

	LoadMapEntities( ar, &Loading );

	ar.Close();

	Loading.Render( 100 );
	m_pTerrain->ShowCloud( FALSE );

	return TRUE;
} //CIndoor::Load

/**
 *
 */
BOOL CIndoor::Update( DWORD dwTick )
{
	if( m_listEntity.empty() )		return TRUE;

	EntityList::iterator itor = m_listEntity.begin();
	CEntity*	pEntity = NULL;

	while( itor != m_listEntity.end() )
	{
		pEntity = (*itor);

		if( !pEntity )
		{
			itor = m_listEntity.erase( itor );
			continue;
		} //if'

	/*	switch( pEntity->m_nType )
		{
		case CEntity::EFFECTOBJECT:
			if( pEntity->m_pEffectObject )
				pEntity->m_pEffectObject->Update( dwTick );
			break;
		}; //switch*/

		itor++;
	} //while
	return TRUE;
} //CIndoor::Update

/**
 *
 */
void CIndoor::Draw()
{
	if( m_listEntity.empty() )		return;
	
	DWORD		dwTick = m_dwTick;
	CEntity*	pEntity = NULL;
	IW3DModel*	pModel = NULL;

	EntityList::iterator itor = m_listEntity.begin();

	while( itor != m_listEntity.end() )
	{
		pEntity = (CEntity*)(*itor);

		if( !pEntity )
		{
			itor = m_listEntity.erase( itor );
			continue;
		} //if

		pModel = pEntity->m_pModel;

		if( !pModel )
		{
			itor = m_listEntity.erase( itor );
			continue;
		} //if

		pModel->SetWorld( pEntity->m_vPos, pEntity->m_qRot );

	//	if( pModel->IsVisible() )
		{
			pModel->SetColor( 0xFFFFFFFF );
			pModel->Render( dwTick );

			switch( pEntity->m_nType )
			{
			case CEntity::ENTITY:
				pModel->Render( dwTick );
				break;
			case CEntity::EFFECTOBJECT:
				if( pEntity->m_pEffectObject )
				{
					pEntity->m_pEffectObject->SetTime( 0 );
					pEntity->m_pEffectObject->Draw();
				}
				break;
			}; //switch
		} //if

		itor++;

		dwTick += 1333;			//< 동작 차이... ^^
	} //while
} //CIndoor::Draw

/**
 *
 */
BOOL CIndoor::IsMovable( int nX, int nY )
{
	return m_pbMovable[nY * m_wSize + nX];
} //CIndoor::IsMovable


/**
 * 맵을 불러온다.
 */
void CIndoor::LoadMapEntities( Archive& ar, const CLoading* pLoading )
{
	Assert( pLoading );

	int		numEntities;

	ar.Serialize( &numEntities, sizeof( int ) );

	for( int iEntity = 0 ; iEntity < numEntities ; iEntity++ )
	{
		char		szModel[32];
		Vector3		vPos;
		float		fYaw;

		ar.Serialize( szModel, 32 );
		ar.Serialize( &vPos, sizeof( Vector3 ) );
		ar.Serialize( &fYaw, sizeof( float ) );

		IW3DModel* pModel = LoadModel( szModel );

		if( pModel )
		{
			CEntity* pEntity = new CEntity;

			pEntity->m_pModel = pModel;
			pEntity->m_sModel = szModel;
			pEntity->m_vPos = vPos;
			
			Quaternion	qRot;
			qRot.SetRotationZ( ANGLE( fYaw ) );
			qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion() * qRot;
			pEntity->m_qRot = qRot;

			IW3DEffectObject*	pEffectObject = MAKE_COMPONENT( EffectObject );
			if( !pEffectObject )
			{
				Assert( !"메모리 할당 실패" );
				SAFE_RELEASE( pModel );
				return;
			} //if

			char	szEffectObject[256];
			strcpy( szEffectObject, szModel );
			strcpy( szEffectObject + strlen( szEffectObject ) - 3, "weo" );

			// World Transform 을 먼저 세팅해줘야 한다.	
			pEffectObject->SetWorld( vPos, qRot );		

			if( pEffectObject->Create( m_pWorld, pModel, szEffectObject ) )
			{
				pEntity->m_nType = CEntity::EFFECTOBJECT;
				pEntity->m_pEffectObject = pEffectObject;
			}
			else
			{
				pEntity->m_nType = CEntity::ENTITY;
				SAFE_RELEASE( pEffectObject );
				pEntity->m_pEffectObject = NULL;
			} //if..else..

			m_listEntity.push_back( pEntity );

		} //if

		if( 0 == iEntity % 20 )
		{	// 로딩 진행률을 표시한다. ( 10~100% )
			pLoading->Render( 10 + 90 * iEntity / numEntities );
		} //if
	} //for
} //CIndoor::LoadMapEntities

/**
 *
 */
IW3DModel* CIndoor::LoadModel( char* szFileName )
{
	string sModelName = szFileName;

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

	char	szAnimation[256];
	strcpy( szAnimation, sModelName.c_str() );
	strcpy( szAnimation + strlen( szAnimation ) - 3, "ani" );

	pModel->LoadAnimation( "Object", szAnimation );

	m_mapModel.insert( std::make_pair( sModelName, pModel ) );

	return pModel;
} //CIndoor::LoadMapEntityModel
