/**
 * 게임 월드.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-12-09 6:28p $
 * @version	$Revision: 259 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DWorld.cpp $
 * 
 * 259   04-12-09 6:28p Sk8snow2
 * 
 * 258   04-08-27 8:01p Paran
 * 
 * 248   04-03-12 2:11p Paran			Collision
 * 
 * 247   04-03-10 6:17p Paran			기능조물 추가
 * 
 * 245   04-02-06 4:13p Kjmgo			카메라 적용
 * 
 * 241   04-01-20 10:24a Kjmgo			Map마다, Camera 높이 조절 가능하도록 수정
 * 
 * 238   04-01-13 9:43p Kjmgo			사양 설정 적용
 * 
 * <HR>
 *
 * @file	W3DWorld.cpp
 */

#include "W3DWorld.h"

IMPLEMENT_SINGLETON_COMPONENT( CW3DWorld, World );


/**
 * 생성자.
 */
CW3DWorld::CW3DWorld()
:	m_pScene		( NULL ),
	m_dwTick		( 0 ),
	m_dwOldTick		( 0 ),
	m_nLayer		( 0 ),
	m_nTime			( 0 ),
	m_nMapQuality	( 0 ),
	m_pItemInfoMgr	( NULL ),
	m_pParticles	( NULL ),
	m_pTraceEffect	( NULL ),
	m_pMagic		( NULL ),
	m_pTerrain		( NULL ),
	m_pFlyingObject ( NULL ),
	m_pHit			( NULL ),
	m_pGrass		( NULL ),
	m_pLightMgr		( NULL ),
	m_pTimeMgr		( NULL ),
	m_pWeather		( NULL ),
	m_pQuadTree		( NULL ),
	m_pMap			( NULL ),
	m_pShadowDummy	( NULL ),
	m_pDoor			( NULL ),
	m_pIndoor		( NULL ),
	m_bIndoor		( FALSE ),
	m_dwFogColor	( 0x00000000 ),
	m_fFogStart		( 1000.0F ),
	m_fFogEnd		( 1000.0F )
{
	for( int i = 0 ; i < MAX_LAYER ; i++ )
	{
		m_pbMovable[i] = NULL;
		m_pPathFind[i] = NULL;
	} //for

	m_pLight[0]		= NULL;
	m_pLight[1]		= NULL;
	m_pLight[2]		= NULL;
} //CW3DWorld::CW3DWorld

/**
 * 소멸자.
 */
CW3DWorld::~CW3DWorld()
{
	Cleanup();

	SAFE_DELETE( m_pMap );
	SAFE_DELETE( m_pTimeMgr );
	SAFE_DELETE( m_pWeather );
	SAFE_DELETE( m_pDoor );
	SAFE_DELETE( m_pIndoor );
	SAFE_DELETE( m_pHit );

	SAFE_RELEASE( m_pLightMgr );
	SAFE_RELEASE( m_pShadowDummy );
	SAFE_RELEASE( m_pMagic );
	SAFE_RELEASE( m_pTraceEffect );
	SAFE_RELEASE( m_pParticles );
} //CW3DWorld::~CW3DWorld

/**
 * World 초기화.
 *
 * @param	pScene		[IN] Scene 객체.
 *
 * @return	초기화 성공 여부 (TRUE/FALSE)
 */
BOOL CW3DWorld::Init( IW3DScene* pScene, IW3DCamera* pCamera )
{
	Assert( pScene );
	Assert( pCamera );
	Assert( !m_pParticles );
	Assert( !m_pTraceEffect );
	Assert( !m_pMagic );

	m_pScene = pScene;
	m_pCamera = pCamera;

	m_pItemInfoMgr   = MAKE_COMPONENT( ItemInfoMgr );
	CHECK_PTR_RETURN( m_pItemInfoMgr );
	
	// Set Fog

	m_dwFogColor	= 0x00000000;
	m_fFogStart		= 1000.0F;
	m_fFogEnd		= 1000.0F;

	// Create Light

	m_pLightMgr		= MAKE_COMPONENT( LightMgr );

	if( !m_pLightMgr )
	{
		return FALSE;
	} //if

	int nMaxActiveLights = m_pScene->GetMaxActiveLights();

	Log( "-=-= Light Init %d : -=-=", nMaxActiveLights );

	m_pLightMgr->Init( m_pScene->GetMaxActiveLights() );

	Vector3	vDir = Vector3( 1.0F, 1.0F, -1.0F );
	DWORD	dwDiffuse = 0xFFFFFFFF;
	DWORD	dwAmbient = 0x80808080;

	m_pLight[0] = m_pLightMgr->GetLight( 0 );
	m_pLight[1] = m_pLightMgr->GetLight( 1 );

	if( m_pLight[0] )
	{
		m_pLight[0]->SetDirectional();
		m_pLight[0]->SetDiffuse( dwDiffuse );
		m_pLight[0]->SetAmbient( dwAmbient );
		m_pLight[0]->SetDirection( vDir );
	} //if

	if( m_pLight[1] )
	{
		m_pLight[1]->SetDirectional();
		m_pLight[1]->SetDiffuse( dwDiffuse );
		m_pLight[1]->SetAmbient( dwAmbient );
		m_pLight[1]->SetDirection( vDir );
	} //if

	m_pWeather = new CWeather;
	m_pWeather->Create( this );

	m_pTimeMgr = new CTimeMgr;
	m_pTimeMgr->Init( this );

	m_pMap = new CMap;
	// Create Particles

	m_pParticles = MAKE_COMPONENT( Particles );

	if( !m_pParticles->Init() )
	{
		Assert( !"파티클 초기화 실패" );
		return FALSE;
	} //if

	m_pParticles->LoadParticleTypeFile( "Effect/particle.ptc" );

	// Init Trace Effect

	m_pTraceEffect = MAKE_COMPONENT( TraceEffect );

	if( !m_pTraceEffect->Init() )
	{
		Assert( !"Trace Effect 초기화 실패" );
		return FALSE;
	} //if

	m_pTraceEffect->Load( "Effect/Trace.tdf" );

	// Init	Magic

	m_pMagic = MAKE_COMPONENT( MagicMgr );

	if( !m_pMagic->Init( this ) )
	{
		Assert( !"마법 초기화 실패" );
		return FALSE;
	} //if

	m_pMagic->Load( "MagicList.ml" );

	// Init 

	// Init Hit Indication

	m_pHit = new CHitIndication;

	if( !m_pHit || !m_pHit->Create( "damage.tga", 18, 23 ) )
	{
		Assert( !"Hit Damage Texture Load Fail." );
		return FALSE;
	} //if

	// Init Character Shadow Dummy

	m_pShadowDummy = MAKE_COMPONENT( Model );

	if( !m_pShadowDummy || !m_pShadowDummy->Load( "Body", "ShadowDummy.mod" ) )
	{
		//return FALSE;
	} //if

	m_pShadowDummy->LoadAnimation( "Body", "ShadowDummy.ani" );

	// Init Door
	m_pDoor = new CDoor;
	if( !m_pDoor )
	{
		return FALSE;
	} //if

	// Init Indoor
	m_pIndoor = new CIndoor;

	if( !m_pIndoor )
	{
		return FALSE;
	} //if

	m_pIndoor->Init( this, m_pCamera );

	//m_pTimeMgr->Load

	return TRUE;
} //CW3DWorld::Init

/**
 * Clean Up.
 */
void CW3DWorld::Cleanup()
{
	CleanupNPC();
	CleanupObjects();
	CleanupModel();
	CleanupEffectObject();
	CleanupEffect();
	CleanupSpecialEffect();
	CleanupEntity();
	CleanupFuncEntity();

	SAFE_RELEASE( m_pGrass );

	SAFE_DELETE( m_pFlyingObject );

	for( int i = 0 ; i < MAX_LAYER ; i++ )
	{
		SAFE_DELETE( m_pPathFind[i] );
		SAFE_FREE( m_pbMovable[i] );
	} //for

	SAFE_RELEASE( m_pTerrain );

	// Assert

//	Assert( m_listEntity.empty() );
	Assert( m_listEffectObject.empty() );
	Assert( m_mapModel.empty() );
	Assert( m_mapEffect.empty() );

	Assert( m_mapCharacter.empty() );
	Assert( m_mapMonster.empty() );
	Assert( m_listNPC.empty() );
	Assert( m_listEffect.empty() );
} //CW3DWorld::Cleanup

/**
 * 맵 오브젝트 클린업.
 */
void CW3DWorld::CleanupEntity()
{
	m_pMap->CleanEntity();
} //CW3DWorld::CleanupEntity

/**
 * Effect Object 클린업.
 */
void CW3DWorld::CleanupEffectObject()
{
	if( m_listEffectObject.empty() )	return;

	EffectObjectList::iterator	itor = m_listEffectObject.begin();
	while( itor != m_listEffectObject.end() )
	{
		IW3DEffectObject*	pObject = *itor;
		SAFE_RELEASE( pObject );
		itor = m_listEffectObject.erase( itor );
	} //while
} //CW3DWorld::CleanupEffectObject

/**
 * 모델 클린업.
 */
void CW3DWorld::CleanupModel()
{
	if( m_mapModel.empty() )		return;

	ModelMap::iterator	itor = m_mapModel.begin();
	while( itor != m_mapModel.end() )
	{
		ModelMap::iterator	itorNext = itor;
		itorNext++;

		IW3DModel*		pModel = itor->second;
		SAFE_RELEASE( pModel );
		m_mapModel.erase( itor );

		itor = itorNext;
	} //while
} //CW3DWorld::CleanupModel

/**
 * 게임 오브젝트 클린업.
 */
void CW3DWorld::CleanupObjects()
{
	CleanupItem();
	CleanupMonster();
	CleanupMercenary();
	CleanupCharacter();

	Assert( m_mapCharacter.empty() );
	Assert( m_mapMonster.empty() );
	Assert( m_mapItem.empty() );
	Assert( m_listWaitItem.empty() );
} //CW3DWorld::Cleanup

/**
 * 캐릭터 클린업.
 */
void CW3DWorld::CleanupCharacter()
{
	if( m_mapCharacter.empty() )		return;

	CreatureMap::iterator	itor = m_mapCharacter.begin();
	while( itor != m_mapCharacter.end() )
	{
		CreatureMap::iterator	itorNext = itor;
		itorNext++;

		IW3DCreature*		pCharacter = itor->second;
		SAFE_RELEASE( pCharacter );
		m_mapCharacter.erase( itor );

		itor = itorNext;
	} //while
} //CW3DWorld::CleanupCharacter

/**
 * 몬스터 클린업.
 */
void CW3DWorld::CleanupMonster()
{
	if( m_mapMonster.empty() )		return;

	CreatureMap::iterator	itor = m_mapMonster.begin();
	while( itor != m_mapMonster.end() )
	{
		CreatureMap::iterator	itorNext = itor;
		itorNext++;

		IW3DCreature*		pMonster = itor->second;
		SAFE_RELEASE( pMonster );
		m_mapMonster.erase( itor );

		itor = itorNext;
	} //while
} //CW3DWorld::CleanupMonster

/**
 * NPC 클린업.
 */
void CW3DWorld::CleanupNPC()
{
	if( m_listNPC.empty() )			return;

	CreatureList::iterator	itor = m_listNPC.begin();
	while( itor != m_listNPC.end() )
	{
		IW3DCreature*	pCreature = *itor;
		SAFE_RELEASE( pCreature );
		itor = m_listNPC.erase( itor );
	} //while
} //CW3DWorld::CleanupNPC

/**
 * 용병 클린업.
 */
void CW3DWorld::CleanupMercenary()
{
	if( m_listMercenary.empty() )			return;

	CreatureList::iterator	itor = m_listMercenary.begin();
	while( itor != m_listMercenary.end() )
	{
		IW3DCreature*	pCreature = *itor;
		SAFE_RELEASE( pCreature );
		itor = m_listMercenary.erase( itor );
	} //while
} //CW3DWorld::CleanupMercenary

/**
 * 기능 모델 클린업
 */
void CW3DWorld::CleanupFuncEntity()
{
	FuncEntityMap::iterator		iter = m_mapFuncEntity.begin();
	while( iter != m_mapFuncEntity.end() )
	{
		FuncEntityMap::iterator		itorNext = iter;
		itorNext++;

		IW3DFuncEntity*		pFuncEntity = iter->second;
		SAFE_RELEASE( pFuncEntity );
		m_mapFuncEntity.erase( iter );
	
		iter = itorNext;
	} //while
} //CW3DWorld::CleanupFuncEntity

/**
 * 아이템 클린업.
 */
void CW3DWorld::CleanupItem()
{
	ItemMap::iterator	itor = m_mapItem.begin();
	while( itor != m_mapItem.end() )
	{
		ItemMap::iterator	itorNext = itor;
		itorNext++;

		IW3DItem*		pItem = itor->second;
		SAFE_RELEASE( pItem );
		m_mapItem.erase( itor );

		itor = itorNext;
	} //while

	SItemOwner*					pItemOwner;
	ItemOwnerList::iterator		itorList = m_listWaitItem.begin();
	while( itorList != m_listWaitItem.end() )
	{
		ItemOwnerList::iterator		itorNext = itorList;
		itorNext++;

		pItemOwner = (*itorList);
		
		if( pItemOwner )
		{
			SAFE_RELEASE( pItemOwner->pItem );
			SAFE_FREE( pItemOwner );
		} //if

		m_listWaitItem.erase( itorList );

		itorList = itorNext;
	} //while
} //CW3DWorld::CleanupItem
/**
 * 메모리상에 있는 Effect들을 지운다.
 */
void CW3DWorld::CleanupSpecialEffect()
{
	EffectMap::iterator	itor = m_mapEffect.begin();
	while( itor != m_mapEffect.end() )
	{
		EffectMap::iterator	itorNext = itor;
		itorNext++;

		IW3DSpecialEffect* pEffect = itor->second;
		string		sName = itor->first;

		SAFE_RELEASE( pEffect );
		m_mapEffect.erase( itor );

		itor = itorNext;
	} //while	
} //CleanupEffect

/**
 * 이펙트 클린업.
 */
void CW3DWorld::CleanupEffect()
{
	if( m_listEffect.empty() )		return;

	EffectList::iterator	itor = m_listEffect.begin();
	while( itor != m_listEffect.end() )
	{
		CEffect*	pEffect = *itor;
		string&		sName = pEffect->GetName();
		SAFE_DELETE( pEffect );
		itor = m_listEffect.erase( itor );
	} //while
} //CW3DWorld::CleanupEffect

/**
 * 지형을 생성한다.
 *
 * @return		지형 생성 성공 여부.
 */
BOOL CW3DWorld::CreateTerrain(
	const DWORD		dwSize,				///< [IN] 맵 크기 (2m 단위)
	const char*		szTextureName		///< [IN] 맵 텍스처 이름
	)
{
	m_dwSize = dwSize * TILE_SIZE;

	SAFE_RELEASE( m_pTerrain );

	for( int i = 0 ; i < MAX_LAYER ; i++ )
	{
		SAFE_DELETE( m_pPathFind[i] );
		SAFE_FREE( m_pbMovable[i] );
	} //for
	
	BOOL bUseLight = FALSE;

	if( m_btMapType == 0 )
		bUseLight = TRUE;

	m_pTerrain = MAKE_COMPONENT( Terrain );
	m_pTerrain->SetQuality( m_nMapQuality );
	if( !m_pTerrain->Init( dwSize, szTextureName, bUseLight ) )
	{
		Assert( !"지형 초기화 실패" );
		return FALSE;
	} //if

	for( i = 0 ; i < MAX_LAYER ; i++ )
	{
		m_pbMovable[i] = (BYTE*)_MemAlloc( m_dwSize * m_dwSize );
		CHECK_PTR_RETURN( m_pbMovable[i] );
		_ZeroMemory( m_pbMovable[i], m_dwSize * m_dwSize );

		m_pPathFind[i] = new CPathFind( m_dwSize );
		CHECK_PTR_RETURN( m_pPathFind[i] );
		m_pPathFind[i]->SetMovable( m_pbMovable[i] );
	} //for

	return TRUE;
} //CW3DWorld::CreateTerrain

/**
 * 맵 불러오기.
 *
 * @param	szName		[IN] 맵 이름
 *
 * @return	맵 로드 성공 (TRUE/FALSE)
 */
BOOL CW3DWorld::LoadMap( const char* szName, BOOL bReload )
{
	Log( "== loadmap : %s", szName );
	
	if( bReload )
	{
		LoadDoors( string( szName ) );
		return TRUE;
	} //if

	g_listObject.clear();
	
	m_vecAreaRgn.clear();
//	m_vecFishRgn.clear();

	if( m_bIndoor )
	{
		return LoadIndoor( szName );
	} //if

	m_pCamera->SetFreeYaw( TRUE );
	m_pCamera->SetMaxZoom( 35.0F );
	m_pCamera->SetMaxMapZoom( 15.0F );

	CLoading	Loading;

	if( !Loading.Init( m_pScene, szName ) )
	{
		Assert( !"로딩 화면 초기화 실패" );
		return FALSE;
	} //if


	Cleanup();
	//m_pQuadTree->Clean();

	string	sFileName = string( "Map/" ) + szName + ".map";

	FILE*	fp = fopen( sFileName.c_str(), "rb" );
	if( !fp )
	{
		Assert( !"파일 열기 실패" );
		return FALSE;
	} //if

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

	m_dwSize = dwSize * TILE_SIZE;		// 월드 크기.

	Assert( NULL == m_pTerrain );
	Assert( NULL == m_pbMovable[0] );
	Assert( NULL == m_pPathFind[0] );

	Log( "== CreateTerrain ==" );
	if( !CreateTerrain( dwSize, szTextureName ) )
	{
		return FALSE;
	} //if

	ar.Serialize( m_pTerrain->GetHeightMap(),	dwSize * dwSize * sizeof( float ) );
	ar.Serialize( m_pTerrain->GetTextureMap(),	dwSize * dwSize * sizeof( BYTE ) );
	ar.Serialize( m_pTerrain->GetColorMap(),	dwSize * dwSize * sizeof( DWORD ) );

	for( int i = 0 ; i < MAX_LAYER ; i++ )
	{
		m_pTerrain->SetLevel(i);
		SetLayer(i);
	
		ar.Serialize( m_pTerrain->GetObjectHeightMap(),	m_dwSize * m_dwSize * sizeof( float ) );
		ar.Serialize( m_pbMovable[i],					m_dwSize * m_dwSize * sizeof( BYTE ) );
		ar.Serialize( m_pTerrain->GetAttributeMap(),	dwSize * dwSize * sizeof( BYTE ) );
				
		m_pPathFind[i]->SetMovable( m_pbMovable[i] );

	} //for

	m_pTerrain->CalculateNormals();
	//m_pTerrain->CreateTerrain();

	/*
		TODO :	옵션에서 조절하도록 한다.
				임시적으로 사용을 한다.
	*/
	m_pTerrain->SetMultiPass( TRUE );

	///////////////////////////////////
	//
	///////////////////////////////////
	m_pMap->Init( this );
	m_pMap->Load( szName );

	m_pQuadTree = m_pMap->GetQuadTree();
	m_pQuadTree->Clean();
	
	Loading.Render( 10 );

	Log( "== LoadMapEntities ==" );
	LoadMapEntities( ar, &Loading );

	if( dwVersion >= 101 )
	{
		Log( "== LoadBlock ==" );
		LoadBlocks( ar, &Loading );
	} //if

	ar.Close();

	Log( "== Separate ==" );
	m_pQuadTree->Separate();
	m_pQuadTree->MakeList( m_pCamera->GetPosition() );


	///////////////////////////////////
	//
	///////////////////////////////////
	Log( "== LoadDoors ==" );
	LoadDoors( string( szName ) );

	m_pFlyingObject = new CFlyingObject;
	CHECK_PTR_RETURN( m_pFlyingObject );

	m_pFlyingObject->SetWorld( this );
	m_pFlyingObject->SetQuality( m_nMapQuality );

	Log( "== Load Fly ==" );

	char szMapNumber[4];

	int nLen = strlen( szName );
	for( i = 0 ; i < 4 ; i++ )
	{
		szMapNumber[i] = szName[nLen - ( 4 - i )];
	} //for

	if( !m_pFlyingObject->Load( atoi( szMapNumber ), "FlyingObject/FlyingObject.txt" ) )
	{
		Assert( !"FlyingObject 파일 로드 실패" );
	//	return FALSE;
	} //if

	Loading.Render( 100 );

	Log( "== Load Light ==" );
	if( m_btMapType == 1 )
	{
		LoadLight( "indoor.txt" );
	} //if

	Log( "== ShowCloud ==" );
	m_pTerrain->ShowCloud( TRUE );

	return TRUE;
} //CW3DWorld::LoadMap

/**
 *
 */
void CW3DWorld::LoadDoors( string& sName )
{
	SAFE_DELETE( m_pDoor );
	m_pDoor = new CDoor;
	
	sName += ".door";
	string sFile = "Map/" + sName;

	m_pDoor->Load( sFile.c_str() );

	DEBUG_OUT( "Door Loading : " + sFile.c_str() );
	
} //CW3DWorld::LoadDoors

/**
 * 맵을 불러온다.
 */
void CW3DWorld::LoadMapEntities( Archive& ar, const CLoading* pLoading )
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

		LoadMapEntity( szModel, vPos, fYaw );

		if( 0 == iEntity % 20 )
		{	// 로딩 진행률을 표시한다. ( 10~100% )
			pLoading->Render( 10 + 90 * iEntity / numEntities );
		} //if
	} //for
} //CW3DWorld::LoadMapEntities

/**
 *
 */
void CW3DWorld::LoadBlocks( Archive& ar, const CLoading* pLoading )
{
	Assert( pLoading );

	int		numBlocks;

	ar.Serialize( &numBlocks, sizeof( int ) );

	for( int i = 0 ; i < numBlocks ; i++ )
	{
		SMultiPass pass;	

		ar.Serialize( &pass, sizeof( SMultiPass ) );
		m_pTerrain->AddPass( &pass );
	} //for
} //CW3DWorld::LoadBlocks

/**
 * 맵 엔티티를 불러온다.
 */
void CW3DWorld::LoadMapEntity(
	const string&	sModelName,		///< [IN]
	const Vector3&	vPos,			///< [IN]
	const float		fYaw			///< [IN]
	)
{
	int		iNpc = GetNpcByModelName( sModelName );
	if( iNpc )
	{	// NPC라면 NPC로 교체한다.
		SW3DNPCInfo		infoNPC;
		infoNPC.wType	= iNpc;
		infoNPC.vPos	= vPos;
		infoNPC.fYaw	= fYaw;

		Log( "Add NPC - %d", infoNPC.wType );
		AddNPC( iNpc, &infoNPC );

		return;
	} //if

	IW3DModel*	pModel = LoadMapEntityModel( sModelName );
	if( !pModel )
	{
	//	Assert( !"모델 읽기 실패" );
		return;
	} //if

	if( sModelName.find( "swell" ) == 0 ||
		sModelName.find( "sal0" ) == 0 )
	{ // 옵젝은 필요 없을 경우.
		pModel->Visible( FALSE );
	} //if
	
	IW3DEffectObject*	pEffectObject = NULL;
	char	szEffectObject[256];
	int nResult = 0;

	// World Transform 을 먼저 세팅해줘야 한다.	
	Quaternion	qRot;
	qRot.SetRotationZ( ANGLE( fYaw ) );
	qRot = Vector3( 0.0F, 1.0F, 0.0F ).GetQuaternion() * qRot;

	if( m_nMapQuality < 2 )
	{
		pEffectObject = MAKE_COMPONENT( EffectObject );
		if( !pEffectObject )
		{
			Assert( !"메모리 할당 실패" );
			SAFE_RELEASE( pModel );
			return;
		} //if
		
		strcpy( szEffectObject, sModelName.c_str() );
		strcpy( szEffectObject + strlen( szEffectObject ) - 3, "weo" );

		pEffectObject->SetWorld( vPos, qRot );

		nResult = sModelName.find( "bd" );
		nResult += sModelName.find( "ca" );
		nResult += sModelName.find( "ot" );
		nResult += sModelName.find( "cf" );

		pEffectObject->SetFront( FALSE );

		if( nResult != -4 )
		{
			pEffectObject->SetFront( TRUE );
		} //if
	} //if
	
	CEntity*	pEntity = new CEntity;
	if( !pEntity )
	{
		Assert( !"메모리 할당 실패" );
		SAFE_RELEASE( pModel );
		SAFE_RELEASE( pEffectObject );
		return;
	} //if

	pEntity->m_sModel	= sModelName;
	pEntity->m_pModel	= pModel;
	pEntity->m_vPos		= vPos;
	pEntity->m_qRot		= qRot;
	pEntity->m_bFront	= FALSE;

	AddFuncEntity( sModelName, pEntity );

	if( pEffectObject )
	{
		if( pEffectObject->Create( this, pModel, szEffectObject ) )
		{	// Effect Object가 존재할 경우 Effect Object로 교체한다.
			m_listEffectObject.push_back( pEffectObject );
			pEntity->m_nType			= CEntity::EFFECTOBJECT;
			pEntity->m_pEffectObject	= pEffectObject;

			if( m_pMap->AddEntity( pEntity ) )
			{
				m_pQuadTree->Push_Back( pEntity );
			} //if		
			return;
		} //if

		SAFE_RELEASE( pEffectObject );
	} //if

	pEntity->m_nType	= CEntity::ENTITY;

	nResult = 0;
	nResult = sModelName.find( "bd" );
	nResult += sModelName.find( "ca" );
	nResult += sModelName.find( "ot" );
	nResult += sModelName.find( "tr" );
	nResult += sModelName.find( "cf" );

	if( nResult != -5 )
	{
		pEntity->m_bFront = TRUE;
	} //if

	if( m_pMap->AddEntity( pEntity, FALSE ) )
	{
		m_pQuadTree->Push_Front( pEntity );
	} //if

} //CW3DWorld::LoadMapEntity

/**
 * 맵 엔티티 모델을 불러온다.
 *
 * @param	sModelName		[IN] 읽어올 모델 이름
 */
IW3DModel* CW3DWorld::LoadMapEntityModel( const string& sModelName )
{
	ModelMap::iterator	itor = m_mapModel.find( sModelName );

	if( itor != m_mapModel.end() )
	{	// 기존에 있던 모델이라면 그냥 돌려준다.
		IW3DModel*	pModel = itor->second;
		return pModel;
	} //if

//	DEBUG_OUT( "Load Model - " + sModelName.c_str() );

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

	pModel->CalculateRadius();
	pModel->LoadAnimation( "Object", szAnimation );

	m_mapModel.insert( std::make_pair( sModelName, pModel ) );

	return pModel;
} //CW3DWorld::LoadMapEntityModel

/**
 * 내부 구현 Load
 */
BOOL CW3DWorld::LoadIndoor( const char* szName )
{
	m_bIndoor = m_pIndoor->Load( m_pScene, szName );
	LoadDoors( string( szName ) );

	LoadLight( "indoor.txt" );

	m_pCamera->SetFreeYaw( FALSE );
	m_pCamera->SetYawRange( 45.0F );
	return m_bIndoor;
} //CW3DWorld::LoadIndoor

/**
 * 맵 데이터의 모델 이름으로 NPC 번호를 얻는다.
 * NPC가 아닌 일반 모델이라면 0을 돌려준다.
 *
 * @param	sModelName		[IN] 맵 데이터의 모델 이름
 *
 * @return	NPC 번호. NPC가 아니라면 0.
 */
int CW3DWorld::GetNpcByModelName( const string& sModelName )
{
	char*	szModelName = const_cast<char*>( sModelName.c_str() );

	if( szModelName[0] != 'c' && szModelName[0] != 'C' )
	{
		return 0;
	} //if

	if( szModelName[1] != 'n' && szModelName[1] != 'N' )
	{
		return 0;
	} //if
	
	if( szModelName[2] != '_' )
	{
		return 0;
	} //if
	
	char		szNumber[5];
	ZeroMemory( szNumber, 5 );
	int			nNum;
	strncpy( szNumber, szModelName + 3, 4 );
	nNum = atoi( szNumber );

	return nNum;
} //CW3DWorld::GetNpcByModelName

/**
 * 빛 설정을 읽어온다.
 */
BOOL CW3DWorld::LoadLight( const char* szFileName )
{
	if( m_pTimeMgr&& !m_pTimeMgr->LoadLight( szFileName ) )
	{
		return FALSE;
	} //if

	return TRUE;
} //CW3DWorld::LoadLight

/**
 * World 내의 모든 오브젝트를 Update하는 함수.
 *
 * @param	dwTick		[IN] 경과 Tick ( 1000 tick = 1 sec. )
 */
void CW3DWorld::Update( const DWORD dwTick )
{
	if( m_pTimeMgr )
	{
		m_pTimeMgr->Update( dwTick );
	} //if

	if( m_bIndoor )
	{
		m_pIndoor->Update( dwTick );
	}
	else
	{
		UpdateEntities( dwTick );
	} //if..else..

	if( m_pMap )	m_pMap->CleanPotal();

	UpdateCharacters( dwTick );
	UpdateMonsters( dwTick );
	UpdateNPC( dwTick );
	UpdateMercenary( dwTick );
	UpdateItems( dwTick );
	UpdateFuncEntity( dwTick );

	if( m_pDoor )			m_pDoor->Update( dwTick );
	if( !m_bIndoor && m_pFlyingObject )   m_pFlyingObject->Update( dwTick );
	if( m_pTraceEffect )	m_pTraceEffect->Update( dwTick );
	if( m_pMagic )			m_pMagic->Update( dwTick );
	if( m_pHit )			m_pHit->Update( dwTick );
	if( m_pWeather )		m_pWeather->Update( dwTick );

	m_dwOldTick	= m_dwTick;
	m_dwTick	+= dwTick;
} //CW3DWorld::Update

/**
 * 배경 오브젝트를 Update한다.
 *
 * @param	dwTick		[IN] 경과 Tick
 */
void CW3DWorld::UpdateEntities( const DWORD dwTick )
{
	m_pMap->UpdateEntities( dwTick );
} //CW3DWorld::UpdateEntities

/**
 * 캐릭터를 업데이트한다.
 *
 * @param	dwTick		[IN] 경과 Tick
 */
void CW3DWorld::UpdateCharacters( const DWORD dwTick )
{
	if( m_mapCharacter.empty() )		return;

	CreatureMap::iterator	itor = m_mapCharacter.begin();
	CreatureMap::iterator	itorNext;
	IW3DCreature* pCharacter = NULL;

	while( itor != m_mapCharacter.end() )
	{
		itorNext = itor;
		itorNext++;

		pCharacter = itor->second;

		if( !pCharacter )
		{
			Assert( 0 );
			m_mapCharacter.erase( itor );
			itor = itorNext;
			continue;
		} //if

		pCharacter->SetLighting( TRUE );

		if( !pCharacter->Update( dwTick ) )
		{
			Assert( 0 );
			SAFE_RELEASE( pCharacter );
			m_mapCharacter.erase( itor );
		}
		else
		{
			if( pCharacter->IsShow() )
			{
				m_pMap->AddCreature( pCharacter );
			} //if
		} //if..else..


		itor = itorNext;
	} //while
} //CW3DWorld::UpdateCharacters

/**
 * 몬스터를 업데이트한다.
 */
void CW3DWorld::UpdateMonsters( const DWORD dwTick )
{
	if( m_mapMonster.empty() )		return;

	CreatureMap::iterator	itor = m_mapMonster.begin();
	CreatureMap::iterator	itorNext;
	IW3DCreature*			pMonster = NULL;

	while( itor != m_mapMonster.end() )
	{
		itorNext = itor;
		itorNext++;

		pMonster = itor->second;
		
		if( !pMonster )
		{
			m_mapMonster.erase( itor );
			itor = itorNext;
			continue;
		} //if

		pMonster->SetLighting( TRUE );

		if( !pMonster->Update( dwTick ) )
		{
			SAFE_RELEASE( pMonster );
			m_mapMonster.erase( itor );
		} 
		else
		{//if
			if( pMonster->IsShow() )
			{
				m_pMap->AddCreature( pMonster );
			} //if
		} //if..else..

		itor = itorNext;
	} //while
} //CW3DWorld::UpdateMonsters

/**
 * NPC를 업데이트한다.
 */
void CW3DWorld::UpdateNPC( const DWORD dwTick )
{
	if( m_listNPC.empty() )		return;

	CreatureList::iterator	itor = m_listNPC.begin();
	IW3DCreature*			pNPC = NULL;

	while( itor != m_listNPC.end() )
	{
		pNPC = *itor;

		if( !pNPC )
		{
			itor = m_listNPC.erase( itor );
			continue;
		} //if

		pNPC->SetLighting( TRUE );

		if( !pNPC->Update( dwTick ) )
		{
			SAFE_RELEASE( pNPC );
			itor = m_listNPC.erase( itor );
		}
		else
		{
			if( pNPC->IsShow() )
			{
				m_pMap->AddCreature( pNPC );
			} //if

			itor++;
		} //if..else..
	} //while
} //CW3DWorld::UpdateNPC

/**
 * 용병을 업데이트한다.
 */
void CW3DWorld::UpdateMercenary( const DWORD dwTick )
{
	if( m_listMercenary.empty() )		return;

	CreatureList::iterator	itor = m_listMercenary.begin();
	IW3DCreature*			pMerc = NULL;

	while( itor != m_listMercenary.end() )
	{
		pMerc =  *itor;
		
		if( !pMerc )
		{
			itor = m_listMercenary.erase( itor );
			continue;
		} //if

		pMerc->SetLighting( TRUE );

		if( !pMerc->Update( dwTick ) )
		{
			SAFE_RELEASE( pMerc );
			itor = m_listMercenary.erase( itor );
		}
		else
		{
			itor++;
		} //if..else..
	} //while
} //CW3DWorld::UpdateMercenary

/**
 * 아이템을 업데이트한다.
 */
void CW3DWorld::UpdateItems( const DWORD dwTick )
{
	if( m_mapItem.empty() )			return;

	ItemMap::iterator	itor = m_mapItem.begin();
	ItemMap::iterator	itorNext;
	IW3DItem*			pItem = NULL;

	while( itor != m_mapItem.end() )
	{
		itorNext = itor;
		itorNext++;

		pItem = itor->second;
				
		if( !pItem )
		{
			m_mapItem.erase( itor );
			itor = itorNext;
			continue;
		} //if
		
		pItem->SetLighting( TRUE );

		if( !pItem->Update( dwTick ) )
		{
			SAFE_RELEASE( pItem );
			m_mapItem.erase( itor );
		} //if

		itor = itorNext;
	} //while
} //CW3DWorld::UpdateItems

/**
 * 아이템을 업데이트한다.
 */
void CW3DWorld::UpdateFuncEntity( const DWORD dwTick )
{
	if( m_mapFuncEntity.empty() )			return;

	FuncEntityMap::iterator	itor = m_mapFuncEntity.begin();
	FuncEntityMap::iterator	itorNext;
	IW3DFuncEntity*		pFuncEntity = NULL;

	while( itor != m_mapFuncEntity.end() )
	{
		itorNext = itor;
		itorNext++;

		pFuncEntity = itor->second;
				
		if( !pFuncEntity )
		{
			m_mapFuncEntity.erase( itor );
			itor = itorNext;
			continue;
		} //if
		
		pFuncEntity->SetLighting( TRUE );
		pFuncEntity->Update( dwTick );

		itor = itorNext;
	} //while
} //CW3DWorld::UpdateItems

/**
 * World 내의 모든 오브젝트를 그린다.
 */
void CW3DWorld::Draw()
{
	Assert( m_pScene );

	m_pScene->SetFog( m_dwFogColor, m_fFogStart, m_fFogEnd );

	if( m_bIndoor )
	{
		if( m_pIndoor->GetTerrain() )	m_pIndoor->GetTerrain()->Render( m_dwTick - m_dwOldTick );
	}
	else
	{
		if( m_pTerrain )			m_pTerrain->Render( m_dwTick - m_dwOldTick );
	} //if..else..

	if( m_pLightMgr ) m_pLightMgr->Apply();

//	DrawMonsters();
//	DrawNPC();
	DrawMercenary();
	DrawItems();
	if( m_pMap )	m_pMap->DrawCreatures( m_pCamera->GetPosition(), FALSE );

//	DrawCharacters();

	if( !m_bIndoor )
	{
		m_pMap->DrawEntities( m_dwTick, TRUE );
		
		if( m_pFlyingObject )		
		{
			m_pFlyingObject->Draw();
			m_pFlyingObject->DrawShadow();
		} //if

		DrawFuncEntity( TRUE );
	}
	else
	{
		m_pIndoor->Draw();
	}//if

 	if( m_pMap )	m_pMap->DrawCreatures( m_pCamera->GetPosition(), TRUE );
//	DrawMonsters( TRUE );
//	DrawNPC( TRUE );
	DrawItems( TRUE );
//	DrawCharacters( TRUE );

	if( m_pDoor )				m_pDoor->Draw();
	if( m_pMagic )				m_pMagic->Draw();
	if( m_pTraceEffect )		m_pTraceEffect->Render();
	if( m_pParticles )			m_pParticles->Render( m_dwTick );
	if( m_pHit )				m_pHit->Draw();

	DrawEffects();

	if( m_pTerrain && !m_bIndoor )			m_pTerrain->DrawWater( m_dwTick - m_dwOldTick );
	if( m_pWeather )		m_pWeather->Draw();
//	DrawEntities( TRUE );
} //CW3DWorld::Draw

/**
 * Draw Characters
 *
 * @param	bShadow		[IN] 그림자 출력인지 여부
 */
void CW3DWorld::DrawCharacters( const BOOL bShadow )
{
	if( m_mapCharacter.empty() )		return;

	IW3DCreature*	pCharacter = NULL;

	CreatureMap::iterator	itor = m_mapCharacter.begin();
	while( itor != m_mapCharacter.end() )
	{
		pCharacter = itor->second;

		if( pCharacter )
		{
			if( !bShadow )
			{
				pCharacter->Draw();
			}
			else
			{
				pCharacter->DrawShadow();
			} //if..else..
		} //if

		itor++;
	} //while
} //CW3DWorld::DrawCharacters

/**
 * 몬스터를 그린다.
 *
 * @param	bShadow		[IN] 그림자 출력인지 여부
 */
void CW3DWorld::DrawMonsters( const BOOL bShadow )
{
	if( m_mapMonster.empty() )		return;

	CreatureMap::iterator	itor = m_mapMonster.begin();
	IW3DCreature* pMonster = NULL;

	while( itor != m_mapMonster.end() )
	{
		pMonster = itor->second;

		if( pMonster && pMonster->IsShow() )
		{
			if( !bShadow )
			{
				pMonster->Draw();
			}
			else
			{
				pMonster->DrawShadow();
			} //if..else..
		} //if

		itor++;
	} //while
} //CW3DWorld::DrawMonsters

/**
 * NPC를 그린다.
 *
 * @param	bShadow		[IN] 그림자 출력인지 여부
 */
void CW3DWorld::DrawNPC( const BOOL bShadow )
{
	if( m_listNPC.empty() )		return;

	CreatureList::iterator	itor = m_listNPC.begin();
	IW3DCreature* pCreature = NULL;

	static Vector3	vDist;
	static Vector3	vCamera;
	vCamera = m_pCamera->GetPosition();

	while( itor != m_listNPC.end() )
	{
		pCreature = *itor;
		
		if( pCreature )
		{
			vDist = vCamera - pCreature->GetPosition();

			if( vDist.Length() < 60.0F )
			{
				if( !bShadow )
				{
					pCreature->Draw();
				}
				else
				{
					pCreature->DrawShadow();
				} //if...else...
			} //if
		} //if
		itor++;
	} //for
} //CW3DWorld::DrawNPC

/**
 * 용병을 그린다.
 *
 * @param	bShadow		[IN] 그림자 출력인지 여부
 */
void CW3DWorld::DrawMercenary( const BOOL bShadow )
{
	if( m_listMercenary.empty() )		return;

	CreatureList::iterator	itor = m_listMercenary.begin();
	IW3DCreature*	pCreature = NULL;

	while( itor != m_listMercenary.end() )
	{
		pCreature = *itor;

		if( pCreature )
		{
			if( !bShadow )
			{
				pCreature->Draw();
			}
			else
			{
				pCreature->DrawShadow();
			} //if...else...
		} //if
		itor++;
	} //for
} //CW3DWorld::DrawMercenary

/**
 * 아이템을 그린다.
 */
void CW3DWorld::DrawItems( const BOOL bShadow )
{
	if( m_mapItem.empty() )			return;

	ItemMap::iterator	itor = m_mapItem.begin();
	IW3DItem*	pItem = NULL;
	while( itor != m_mapItem.end() )
	{
		pItem = itor->second;

		if( pItem )
		{
			if( !bShadow )
			{
				pItem->Draw();
			}
			else
			{
				pItem->DrawShadow();
			} //if..else
		} //if

		itor++;
	} //while
} //CW3DWorld::DrawItems

/**
 * 기능성 옵젝 그리기.
 */
void CW3DWorld::DrawFuncEntity( const BOOL bShadow )
{
	if( m_mapFuncEntity.empty() )			return;

	FuncEntityMap::iterator	itor = m_mapFuncEntity.begin();
	IW3DFuncEntity*		pFuncEntity;
	while( itor != m_mapFuncEntity.end() )
	{
		pFuncEntity = itor->second;
		if( pFuncEntity )
		{
			if( bShadow )
			{
				pFuncEntity->DrawShadow();
			} //if
		} //if

		itor++;
	} //while
} //CW3DWorld::DrawItems

/**
 * 이펙트를 그린다.
 */
void CW3DWorld::DrawEffects()
{
	if( m_listEffect.empty() )		return;

	static DWORD dwTick = 0;
	dwTick = m_dwTick - m_dwOldTick;

	CEffect* pEffect = NULL;
	EffectList::iterator	itor = m_listEffect.begin();
	while( itor != m_listEffect.end() )
	{
		pEffect = *itor;
		
		if( !pEffect || !pEffect->Render( m_pScene, dwTick ) )
		{			
			IW3DSpecialEffect*	pSpecialEffect = pEffect->GetEffect();
			UnloadEffect( pSpecialEffect, pEffect->GetName().c_str() );
			SAFE_DELETE( pEffect );
			itor = m_listEffect.erase( itor );
			continue;
		} //if

		itor++;
	} //while
} //CW3DWorld::DrawEffects

/**
 * 시간을 설정한다.
 */
void CW3DWorld::SetTime( const int nTime, const int nNextTime, const DWORD dwTime )
{
	if( m_btMapType > 0 )
	{
		return ;
	} //if

	m_nTime = nTime;
	m_pMap->SetTime( nTime );
	
	if( m_pTimeMgr )
	{
		m_pTimeMgr->SetTime( nTime, nNextTime, dwTime );
	} //if

	//LoadLight( filename );
	

} //CW3DWorld::SetTime

/**
 * 현재 시간을 반환한다.
 */
int CW3DWorld::GetTime()
{
	return m_nTime;
} // CW3DWorld::GetTime

/**
 * World에 캐릭터를 추가한다.
 */
void CW3DWorld::AddCharacter(
	const DWORD		dwId,		///< [IN] 캐릭터 ID
	const void*		pData		///< [IN] 캐릭터 정보
	)
{
	#define IW3DCharacter	IW3DCreature

	CreatureMap::iterator	itor = m_mapCharacter.find( dwId );
	if( itor != m_mapCharacter.end() )
	{	// 이미 존재하는 캐릭터
		IW3DCreature*		pCharacter = itor->second;
		SAFE_RELEASE( pCharacter );
		m_mapCharacter.erase( itor );
	} //if

	IW3DCreature*	pCharacter = MAKE_COMPONENT( Character );

	if( !pCharacter->Create( this, dwId, pData ) )
	{
		Assert( !"Character 생성 실패" );
		SAFE_RELEASE( pCharacter );
		return;
	} //if

	pCharacter->SetShadowDummy( m_pShadowDummy );
	pCharacter->Stop();

	m_mapCharacter.insert( std::make_pair( dwId, pCharacter ) );

	#undef	IW3DCharacter
} //CW3DWorld::AddCharacter

/**
 * World에서 캐릭터를 삭제한다.
 *
 * @param	dwId		[IN] 캐릭터 ID
 */
void CW3DWorld::RemoveCharacter( DWORD dwId )
{
	CreatureMap::iterator	itor = m_mapCharacter.find( dwId );
	if( itor == m_mapCharacter.end() )
	{
		// 존재하지 않는 캐릭터
		return;
	} //if

	IW3DCreature*	pCharacter = itor->second;
	SAFE_RELEASE( pCharacter );

	m_mapCharacter.erase( itor );
} //CW3DWorld::RemoveCharacter

/**
 * 캐릭터 모델을 업데이트한다.
 */
void CW3DWorld::UpdateModelCharacter(
	const DWORD	dwId,		///< [IN] 캐릭터 ID
	const WORD	wItemType,	///< [IN] 업데이트 부분 ID
	const BOOL	bOn			///< [IN] 장착/해제
	)
{
	CreatureMap::iterator	itor = m_mapCharacter.find( dwId );
	if( itor == m_mapCharacter.end() )
	{	// 존재하지 않는 캐릭터
		return;
	} //if

	IW3DCreature* 	pCharacter = itor->second;
	Assert( pCharacter );

	if( !pCharacter )
	{
		m_mapCharacter.erase( itor );
		return ;
	} //if

	pCharacter->UpdateModel( wItemType, bOn );
} //CW3DWorld::UpdateCharacter

/**
 * 캐릭터를 Picking한다.
 */
IW3DCreature* CW3DWorld::PickCharacter(
	const Vector3&	vPick,		///< 바닥 좌표
	const Vector3&	vOrig,		///< 반직선의 시작점.
	const Vector3&	vDir		///< 반직선의 방향.
	)
{
	if( m_mapCharacter.empty() )		return NULL;

	static Vector3	vPos;

	CreatureMap::iterator	itor = m_mapCharacter.begin();
	IW3DCreature* pCharacter = NULL;
	while( itor != m_mapCharacter.end() )
	{
		pCharacter = itor->second;
		
		if( !pCharacter )
		{
			itor++;
			continue;
		} //if
		
		vPos = pCharacter->GetPosition();

		if( ( vPick - vPos ).Length() < 10.0F )
		{
			if( pCharacter->Pick( vOrig, vDir ) )
			{
				return pCharacter;
			} //if
		} //if

		itor++;
	} //while

	return NULL;
} //CW3DWorld::PickCharacter

/**
 * World에 몬스터를 추가한다.
 */
void CW3DWorld::AddMonster(
	const DWORD		dwId,		///< [IN] 몬스터 ID
	const void*		pData		///< [IN] 몬스터 생성 정보
	)
{
#define IW3DMonster		IW3DCreature

	CreatureMap::iterator	itor = m_mapMonster.find( dwId );
	if( itor != m_mapMonster.end() )
	{	// 이미 존재하는 몬스터
		return;
	} //if

	IW3DCreature*		pMonster = MAKE_COMPONENT( Monster );
	Assert( pMonster );

	if( !pMonster || !pMonster->Create( this, dwId, pData ) )
	{
		Assert( !"Monster 생성 실패" );
		SAFE_RELEASE( pMonster );
		return;
	} //if

	m_mapMonster.insert( std::make_pair( dwId, pMonster ) );

#undef	IW3DMonster
} //CW3DWorld::AddMonster

/**
 * World에서 몬스터를 삭제한다.
 *
 * @param	dwId		[IN] 캐릭터 ID
 */
void CW3DWorld::RemoveMonster( DWORD dwId )
{
	CreatureMap::iterator	itor = m_mapMonster.find( dwId );
	if( itor == m_mapMonster.end() )
	{	// 존재하지 않는 몬스터
		return;
	} //if

	IW3DCreature* pMonster = itor->second;
	SAFE_RELEASE( pMonster );

	m_mapMonster.erase( itor );
} //CW3DWorld::RemoveMonster

/**
 * 몬스터를 Picking한다.
 */
IW3DCreature* CW3DWorld::PickMonster(	
	const Vector3&	vPick,	///< 바닥 좌표.
	const Vector3&	vOrig,	///< 반직선의 시작점.
	const Vector3&	vDir	///< 반직선의 방향.
										)
{
	if( m_mapMonster.empty() )		return NULL;

	static Vector3	vPos;

	CreatureMap::iterator	itor = m_mapMonster.begin();
	IW3DCreature* pMonster = NULL;
	while( itor != m_mapMonster.end() )
	{
		pMonster = itor->second;
		
		if( !pMonster || !pMonster->IsShow() )
		{
			itor++;
			continue;
		} //if

		vPos = pMonster->GetPosition();

		if( ( vPick - vPos ).Length() < 35.0F )
		{
			if( pMonster->Pick( vOrig, vDir ) )
			{
				return pMonster;
			} //if
		} //if
		itor++;
	} //while

	return NULL;
} //CW3DWorld::PickMonster

/**
 * World에 NPC를 추가한다.
 */
void CW3DWorld::AddNPC(
	const DWORD		dwId,		///< [IN] NPC ID
	const void*		pData		///< [IN] NPC 생성 정보
	)
{
#define IW3DNPC		IW3DCreature

	IW3DCreature*		pNPC = MAKE_COMPONENT( NPC );
	if( NULL == pNPC )
	{
		Assert( !"메모리 할당 실패" );
		return;
	} //if

	if( !pNPC->Create( this, dwId, pData ) )
	{
		DEBUG_OUT( "NPC 생성 실패" );
		SAFE_RELEASE( pNPC );
		return;
	} //if

	if( m_bIndoor )
	{
		m_listNPCForIndoor.push_back( pNPC );
	}
	else
	{
		m_listNPC.push_back( pNPC );
	} //if..else..

#undef	IW3DNPC
} //CW3DWorld::AddNPC

/**
 * NPC를 Picking한다.
 */
IW3DCreature* CW3DWorld::PickNPC(
	const Vector3&	vPick,	///< 바닥 좌표.
	const Vector3&	vOrig,	///< 반직선의 시작점.
	const Vector3&	vDir	///< 반직선의 방향.
	)
{
	if( m_listNPC.empty() )		return NULL;

	static Vector3	vPos; 

	CreatureList::iterator	itor;
	CreatureList::iterator	itorEnd;

	if( m_bIndoor )
	{
		itor	= m_listNPCForIndoor.begin();
		itorEnd = m_listNPCForIndoor.end();
	}
	else
	{
		itor = m_listNPC.begin();
		itorEnd = m_listNPC.end();
	} //if..else..

	IW3DCreature*	pCreature = NULL;
	
	while( itor != itorEnd )
	{
		pCreature = *itor;
		
		if( !pCreature )
		{
			itor++;
			continue;
		} //if

		vPos = pCreature->GetPosition();

		if( ( vPick - vPos ).Length() < 10.0F )
		{
			if( pCreature->Pick( vOrig, vDir ) )
			{
				return pCreature;
			} //if
		} //if
		itor++;
	} //while

	return NULL;
} //CW3DWorld::PickNPC

/** 
 * 용병을 추가.
 */
void CW3DWorld::AddMercenary( const BYTE btMercType, const DWORD dwOwnerId )
{
#define IW3DMercenary		IW3DCreature

	IW3DCreature*		pMerc = MAKE_COMPONENT( Mercenary );
	if( NULL == pMerc )
	{
		Assert( !"메모리 할당 실패" );
		return;
	} //if

	IW3DCreature*	pOwnerChar = GetCharacter( dwOwnerId );
	Assert( pOwnerChar );

	SW3DMercenaryInfo		sMercInfo;
	sMercInfo.wType	= btMercType;
	sMercInfo.nX	= pOwnerChar->GetPosition().x;
	sMercInfo.nY	= pOwnerChar->GetPosition().y;

	if( !pMerc->Create( this, dwOwnerId, &sMercInfo ) )
	{
		DEBUG_OUT( "용병 생성 실패" );
		SAFE_RELEASE( pMerc );
		return;
	} //if

	m_listMercenary.push_back( pMerc );

#undef	IW3DMercenary
} //CW3DWorld::AddMercenary

/**
 * 용병제거.
 */
IW3DCreature* CW3DWorld::PickMercenary( 
	const Vector3& vPick, 
	const Vector3& vOrig,		///< 반직선의 시작점.
	const Vector3& vDir			///< 반직선의 방향.
	)
{
	if( m_listMercenary.empty() )		return NULL;

	static Vector3		vPos;

	CreatureList::iterator	itor = m_listMercenary.begin();
	IW3DCreature*	pCreature = NULL;

	while( itor != m_listMercenary.end() )
	{
		IW3DCreature*	pCreature = *itor;
		
		if( !pCreature )
		{
			itor = m_listMercenary.erase( itor );
			continue;
		} //if

		vPos = pCreature->GetPosition();

		if( ( vPick - vPos ).Length() < 10.0F )
		{
			if( pCreature->Pick( vOrig, vDir ) )
			{
				return pCreature;
			} //if
		} //if

		itor++;
	} //while

	return NULL;
} //CW3DWorld::PickMercenary

/**
 * 기능 모델을 관리 리스트에 추가한다
 */
BOOL CW3DWorld::AddFuncEntity( const string& strModelName, const CEntity* pEntity )
{
	Assert( pEntity );

	char			szId[4];
	BYTE			byType;
	BOOL			bCollision = FALSE;
	char*			szModelName = const_cast<char*>( strModelName.c_str() );
	static DWORD	dwId = 0;
	
	if( ( szModelName[0] == 'b' || szModelName[0] == 'B' ) &&
		( szModelName[1] == 'd' || szModelName[1] == 'D' ) &&
		  szModelName[3] == '0' && szModelName[4] == '2'   &&
		  szModelName[5] == '3' && szModelName[6] == '8' )
	{ //Encamp
		byType = IW3DFuncEntity::FUNCENTITY_ENCAMP;
		if( ++dwId > 2 )	dwId = 1;
		bCollision = TRUE;
	}
	else if( ( szModelName[0] == 'g' || szModelName[0] == 'G' ) &&
			 ( szModelName[1] == 'a' || szModelName[1] == 'A' ) &&
			 ( szModelName[2] == 't' || szModelName[2] == 'T' ) &&
			 ( szModelName[3] == 'e' || szModelName[3] == 'E' ) )
	{ //Gate
		byType = IW3DFuncEntity::FUNCENTITY_GATE;
		strncpy( szId, szModelName + 5, 2 );
		dwId = atoi( szId );
		bCollision = TRUE;
	}
	else
	{ //None
		return FALSE;
	} //if..else

	IW3DFuncEntity*		pFuncEntity = MAKE_COMPONENT( FuncEntity );
	if( !pFuncEntity )
	{
		Assert( !"Failed Allocate Memory for IW3DFuncEntity" );
		return FALSE;
	} //if

	pFuncEntity->SetEntity( dwId, byType, pEntity->m_pModel, pEntity->m_vPos );
	pFuncEntity->SetCollisionEntity( bCollision );

	FuncEntityMap::iterator		iter = m_mapFuncEntity.find( dwId );
	if( iter != m_mapFuncEntity.end() )
	{
		Assert( !"Already Exist The Identity Number" );
		SAFE_RELEASE( pFuncEntity );
		return FALSE;
	} //if
	m_mapFuncEntity.insert( std::make_pair( dwId, pFuncEntity ) );

	return TRUE;
} //CW3DWorld::AddFuncEntity

/**
 * 기능 모델 Picking한다.
 */
IW3DFuncEntity* CW3DWorld::PickFuncEntity(
	const Vector3&	vPick,		///< 바닥 좌표.
	const Vector3&	vOrig,		///< 반직선의 시작점.
	const Vector3&	vDir		///< 반직선의 방향.
	)
{
	if( m_mapFuncEntity.empty() )		return NULL;

	FuncEntityMap::iterator		itor = m_mapFuncEntity.begin();
	IW3DFuncEntity*				pFuncEntity = NULL;
	while( itor != m_mapFuncEntity.end() )
	{
		pFuncEntity = itor->second;
		Assert( pFuncEntity );

		Vector3*	pvPos = pFuncEntity->GetPosition();
		if( !pvPos )
		{
			Assert( !"Empty FuncEntity" );
			continue;
		} //if

		if( ( vPick - (*pvPos) ).Length() < 10.0F )
		{
			if( pFuncEntity->Pick( vOrig, vDir ) )
			{
				return pFuncEntity;
			} //if
		} //if..else..
		itor++;
	} //while
	return NULL;
}// CW3DWorld::PickItem

/**
 * World에 아이템을 추가한다.
 */
void CW3DWorld::AddItem(
	const DWORD	dwId,		///< [IN] 아이템 ID
	const WORD	wType,		///< [IN] 아이템 종류
	const int	nX,			///< [IN] 맵 상의 X 좌표
	const int	nY,			///< [IN] 맵 상의 Y 좌표
	const int	nVal,		///< [IN] 아이텝 추가 정보
	const WORD	wModId		///< [IN] 소유한 몹 Id
	)
{
	ItemMap::iterator	itor = m_mapItem.find( dwId );
	if( itor != m_mapItem.end() )
	{	// 이미 존재하는 아이템
		return;
	} //if

	IW3DItem*		pItem = MAKE_COMPONENT( Item );
	if( !pItem )
	{
		Assert( !"메모리 할당 실패" );
		return;
	} //if

	if( !pItem->Create( this, dwId, wType, nX, nY ) )
	{
		DEBUG_OUT( "아이템 생성 실패 - ID : " + dwId + ", TYPE : " + wType + " (" + nX + "," + nY + ")" );
		SAFE_RELEASE( pItem );
		return;
	} //if

	if( wType == ITEM_MONEY )
	{ // 실드
		char	szName[64];
		sprintf( szName, "%s %d", STR_ITEMINFO_SHILD, nVal );
		pItem->SetName( szName );
	} //if

	if( wModId )
	{
		CreatureMap::iterator	iterMob = m_mapMonster.find( wModId );
		if( iterMob == m_mapMonster.end() )
		{
			m_mapItem.insert( std::make_pair( dwId, pItem ) );
		} //if

		SItemOwner*		pItemOwner = (SItemOwner*)_MemAlloc( sizeof( SItemOwner ) );
		if( !pItemOwner )
		{
			Assert( !"할당실패" );
			return;
		} //if
		pItemOwner->btOwnerType = MONSTER;
		pItemOwner->dwOwnerId	= wModId;
		pItemOwner->pItem		= pItem;

		m_listWaitItem.push_back( pItemOwner );
	}
	else
	{
		m_mapItem.insert( std::make_pair( dwId, pItem ) );
	} //if..else
} //CW3DWorld::AddItem

/**
 * World에 타켓에서 얻은 아이템을 추가한다.
 */
void CW3DWorld::AddItemFromTarget( 
	BYTE btTargetType,			///< [IN] 타겟 타입
	DWORD dwId					///< [IN] 타겟 아이디
	)
{
	SItemOwner*			pItemOwner;
	ItemOwnerList::iterator		iterItem = m_listWaitItem.begin();
	while( iterItem != m_listWaitItem.end() )
	{
		pItemOwner = (*iterItem);
		if( btTargetType == pItemOwner->btOwnerType &&
			dwId		 == pItemOwner->dwOwnerId )
		{
			IW3DItem*	pItem = pItemOwner->pItem;
			SAFE_FREE( pItemOwner );
			if( !pItem )
			{
				Assert( pItem );
				iterItem++;
				continue;
			} //if
			m_mapItem.insert( std::make_pair( pItem->GetId(), pItem ) );
			iterItem = m_listWaitItem.erase( iterItem );
			continue;
		} //if

		iterItem++;
	} //while
} //CW3DWorld::AddItemFromTarget

/**
 * World에서 아이템을 삭제한다.
 *
 * @param	dwId		[IN] 아이템 ID
 */
void CW3DWorld::RemoveItem( DWORD dwId )
{
	ItemMap::iterator	itor = m_mapItem.find( dwId );

	IW3DItem*		pItem = NULL;
	if( itor != m_mapItem.end() )
	{	// 맵에 존재하는 아이템
		pItem = itor->second;
		SAFE_RELEASE( pItem );
		m_mapItem.erase( itor );
		return;
	}
	else
	{
		SItemOwner*					pItemOwner;
		ItemOwnerList::iterator		iterItem = m_listWaitItem.begin();
		while( iterItem != m_listWaitItem.end() )
		{
			pItemOwner = (*iterItem);
			if( pItemOwner && pItemOwner->pItem )
			{
				pItem = pItemOwner->pItem;
				if( dwId == pItem->GetId() )
				{
					SAFE_RELEASE( pItem );
					m_listWaitItem.erase( iterItem );
					return;
				} //if
			} //if

			iterItem++;
		} //while
	} //if..else

	Assert( !"없는 아이템을 삭제한다" );
} //CW3DWorld::RemoveItem

/**
 * 아이템을 Picking한다.
 */
IW3DItem*  CW3DWorld::PickItem(
	const Vector3&	vPick,		///< 바닥 좌표.
	const Vector3&	vOrig,		///< 반직선의 시작점.
	const Vector3&	vDir		///< 반직선의 방향.
	)
{
	if( m_mapItem.empty() )		return NULL;

	static Vector3		vPos;

	ItemMap::iterator	itor = m_mapItem.begin();
	IW3DItem* pItem = NULL;
	while( itor != m_mapItem.end() )
	{
		pItem = itor->second;
		
		if( !pItem )
		{
			itor++;
			continue;
		} //if

		vPos = pItem->GetPosition();

		if( ( vPick - vPos ).Length() < 10.0F )
		{
			if( pItem->Pick( vOrig, vDir ) )
			{
				return pItem;
			} //if
		} //if..else..
		itor++;
	} //while
	return NULL;
}// CW3DWorld::PickItem

/**
 * 아이템 이름 출력 여부.
 */
void CW3DWorld::ShowItemName( BOOL bShow )
{
	ItemMap::iterator	itor = m_mapItem.begin();
	IW3DItem* pItem = NULL;
	while( itor != m_mapItem.end() )
	{
		pItem = itor->second;
		
		if( pItem )
		{
			pItem->ShowName( bShow );
		} //if
		itor++;
	}
} //CW3DWorld::ShowItemName

/**
 * World에 Effect를 추가한다.
 */
DWORD CW3DWorld::AddEffect(
	const char*			szName,		///< [IN] 이펙트 이름.
	const Vector3&		vPos,		///< [IN] 이펙트 위치.
	const Quaternion&	qRot,		///< [IN] 이펙트 방향.
	const BOOL			bLoop		///< [IN] 반복 여부.
	)
{
	CEffect*	pEffect = new CEffect;
	if( !pEffect )
	{
		Assert( !"할당실패" );
		return 0;
	} //if

	IW3DSpecialEffect* pSpecialEffect = LoadEffect( szName );
	if( !pSpecialEffect )
	{
		SAFE_DELETE( pEffect );
		Assert( !"없는 이펙트" );
		return 0;
	} //if

	if( !pEffect->Create( szName, vPos, qRot, bLoop, pSpecialEffect ) )
	{
		Assert( !"Effect 생성 실패" );
		UnloadEffect( pSpecialEffect, szName );
		SAFE_DELETE( pEffect );
		return 0;
	} //if
	
	m_listEffect.push_back( pEffect );

	return reinterpret_cast<DWORD>( pEffect );
} //CW3DWorld::AddEffect

/**
 * World에서 Effect를 삭제한다.
 */
void CW3DWorld::RemoveEffect( DWORD dwId )
{
	EffectList::iterator	itor = m_listEffect.begin();
	while( itor != m_listEffect.end() )
	{
		CEffect*	pEffect = *itor;
		if( dwId == reinterpret_cast<DWORD>( pEffect ) )
		{
			IW3DSpecialEffect* p = pEffect->GetEffect();
			
			UnloadEffect( p, pEffect->GetName().c_str() );
			
			SAFE_DELETE( pEffect );
			m_listEffect.erase( itor );
			return;
		} //if
		itor++;
	} //while
} //CW3DWorld::AddEffect

/**
 * World 에서 Effect를 Load한다.
 */
IW3DSpecialEffect* CW3DWorld::LoadEffect( const char* szName )
{
	IW3DSpecialEffect* pSpecialEffect = NULL;

	EffectMap::iterator itor = m_mapEffect.find( string( szName ) );

	if( itor != m_mapEffect.end() )
	{
		pSpecialEffect = itor->second;
		pSpecialEffect->AddRef();
	}
	else
	{
		pSpecialEffect = MAKE_COMPONENT( SpecialEffect );
		CHECK_PTR_RETURN( pSpecialEffect );

		if( !pSpecialEffect->Load( szName ) )
		{
			SAFE_RELEASE( pSpecialEffect );
			return NULL;
		} //if

		m_mapEffect.insert( std::make_pair( string( szName ) , pSpecialEffect ) );
	} //if..else..

	return pSpecialEffect;
} //CW3DWorld::LoadEffect

/**
 *
 */
void CW3DWorld::UnloadEffect( IW3DSpecialEffect* pEffect, const char* szName )
{
	if( !pEffect )
	{
		return;
	} //if
	
	if( pEffect->GetRefCount() <= 1 )
	{
		EffectMap::iterator itor = m_mapEffect.find( szName );

		if( itor != m_mapEffect.end() )
		{
			m_mapEffect.erase( itor );
		} //if
	} //if

	SAFE_RELEASE( pEffect );

} //CW3DWorld::UnloadEffect

/**
 * World에 파티클을 추가한다.
 */
void CW3DWorld::AddParticle(
	const char*		szType,		///< 파티클 종류
	const Vector3&	vPos		///< 파티클 위치
	)
{
	Assert( m_pParticles );
	Assert( szType );

	m_pParticles->AddParticle( szType, vPos );
} //CW3DWorld::AddParticle

/**
 * 마법을 추가한다.
 */
IW3DMagic* CW3DWorld::AddMagic(
	const char*		szMagic,	///< [IN] 마법 이름
	IW3DCreature*	pAttacker,	///< [IN] 시전자
	IW3DCreature*	pTarget,		///< [IN] 피시전자
	BOOL			bEndAttack
	)
{
	Assert( m_pMagic );
	Assert( pAttacker );
	Assert( pTarget );

	if( m_pMagic )
	{
		IW3DMagic* pMagic = NULL;
		pMagic = m_pMagic->AddMagic( szMagic, pAttacker, pTarget, bEndAttack );
		return pMagic;
	} //if

	return NULL;
} //CW3DWorld::AddMagic

/**
 * 마법을 추가한다.
 */
IW3DMagic* CW3DWorld::AddSkill(
	const char*		szMagic,	///< [IN] 마법 이름
	IW3DCreature*	pAttacker,	///< [IN] 시전자
	IW3DCreature*	pTarget
	)
{
	Assert( m_pMagic );
	Assert( pAttacker );
	Assert( pTarget );

	IW3DMagic* pMagic = NULL;
	if( m_pMagic )
	{
		pMagic = m_pMagic->AddSkill( szMagic, pAttacker, pTarget );
	} //if

	return pMagic;
} //CW3DWorld::AddMagic
/**
 * 타격치를 추가한다.
 */
void CW3DWorld::AddHit(
	DWORD dwDamage,				///< [IN] 데미지
	Vector3 vPos,				///< [IN] 데미지 입은 위치	
	DWORD dwColor,				///< [IN] 타격치 색
	BYTE  bCritical				///< [IN] 크리티컬 여부 
	)
{
	Assert( m_pHit );

	if( m_pHit )
	{
		if( dwDamage == 0 )
		{
			// Miss
			m_pHit->AddMiss( vPos + Vector3( 0.0F, 0.0F, 2.0F ), dwColor );
		}
		else
		{
			m_pHit->AddHit( dwDamage, vPos + Vector3( 0.0F, 0.0F, 2.0F ), dwColor, bCritical );
		} // if .. else ..
	} //if
} //CW3DWorld::AddHit

/**
 * 바닥의 높이를 구한다.
 */
float CW3DWorld::GetHeight( const Vector3& vPos )
{
	float	fHeight = 0.0F;

	if( m_bIndoor )
	{
		if( m_pIndoor )
		{
			fHeight = m_pIndoor->GetTerrain()->GetHeight( vPos.x, vPos.y );
		} //if
	}
	else
	{
		if( m_pTerrain )
		{
			m_pTerrain->SetLevel( m_nLayer );
			fHeight = m_pTerrain->GetHeight( vPos.x, vPos.y );
			SetLayer( m_pTerrain->GetLevel() );
		} //if
	} //if..else..

	return	fHeight;
} //CW3DWorld::GetHeight

/**
 * 수면의 높이를 구한다.
 */
float CW3DWorld::GetWaterHeight()
{
	if( m_bIndoor )
	{
		return 0.0F;
	} //if
	Assert( m_pTerrain );
	return m_pTerrain->GetWaterHeight();
} //CW3DWorld::GetWaterHeight

/**
 * 이동 가능한 부분인지 검사한다.
 */
BOOL CW3DWorld::IsMovable( int nX, int nY,int nLayer )
{
	BOOL bFlag = FALSE;

	if( !m_bIndoor )
	{
		Assert( m_pbMovable[m_nLayer] );
		Assert( nX >= 0 && nX < m_dwSize );
		Assert( nY >= 0 && nY < m_dwSize );


		//for( int i = m_nLa

		if( nLayer != -1 )
		{
			for( int i = nLayer ; i >= 0 ; i-- )
			{
				bFlag = m_pbMovable[i][nY * m_dwSize + nX];

				if( !bFlag )
				{
					break;
				} //if
			} //for
		}
		else
			bFlag = m_pbMovable[m_nLayer][nY * m_dwSize + nX];
	}
	else
	{
		Assert( m_pIndoor );

		bFlag = m_pIndoor->IsMovable( nX, nY );
	} //if..else..

	return !bFlag;
} //CW3DWorld::IsMovable

/**
 * 바닥을 Picking한다.
 *
 * @return	Picking 됐는지 여부
 */
int CW3DWorld::PickTerrain(
	const int		nLayer,		///< [IN] 레이어
	const float		fHeight,	///< [IN] 바닥 높이.
	const int		nX,			///< [IN] 화면상의 X좌표
	const int		nY,			///< [IN] 화면상의 Y좌표
	Vector3*		pvPick		///< [OUT] Picking된 좌표
	)
{
	Assert( pvPick );
	Assert( m_pScene );

	static Vector3 vOrig, vDir;
	m_pScene->GetPickRay( nX, nY, &vOrig, &vDir );
	
	int nFlag = 0;
	BOOL bFlag = FALSE;

	if( m_bIndoor )
	{
		m_pIndoor->GetTerrain()->SetLevelOne( TRUE );
		nFlag = m_pIndoor->GetTerrain()->Pick( fHeight, vOrig, vDir, pvPick );
	}
	else
	{
		Assert( m_pTerrain );
		m_pTerrain->SetLevelOne( FALSE );
		m_pTerrain->SetUseObjectHeight();

		for( int i = nLayer ; i >= 0 ; i-- )
		{
			m_pTerrain->SetLevel( i );

			if( m_pTerrain->Pick( fHeight, vOrig, vDir, pvPick ) )
			{
				nFlag = i + 1;
				break;
			} //if
		} //for
	} //if..else..

	return nFlag;
} //CW3DWorld::PickTerrain

/**
 * 길찾기를 한다.
 */
int CW3DWorld::PathFind(
	const Vector3&		vStart,		///< [IN] 시작점
	const Vector3&		vTarget,	///< [IN] 목적지
	Vector3**			ppPath,		///< [OUT] 경로
	BOOL				bOptimal
	)
{
	Assert( m_pPathFind );

	vector<Vector3>		aPath;
	
	
	if( m_bIndoor )
	{
		aPath = m_pIndoor->GetPathFind()->Find( vStart, vTarget, bOptimal );
	}
	else
	{
		aPath = m_pPathFind[m_nLayer]->Find( vStart, vTarget, bOptimal );
	} //if..else..

	if( aPath.empty() )
	{
		*ppPath = NULL;
		return 0;
	} //if

	*ppPath = (Vector3*)_MemAlloc( aPath.size() * sizeof( Vector3 ) );
	_MemCpy( *ppPath, &aPath[0], aPath.size() * sizeof( Vector3 ) );

	return (int)aPath.size();
} //CW3DWorld::PathFind

/**
 *  
 */
IW3DCreature* CW3DWorld::BeginCharacter()
{
	m_itorCharacter = m_mapCharacter.begin();
	if( m_itorCharacter == m_mapCharacter.end() )
	{
		return NULL;
	} //if
	return m_itorCharacter->second;
} //CW3DWorld::BeginCharacter

/**
 *  
 */
IW3DCreature* CW3DWorld::NextCharacter()
{
	m_itorCharacter++;
	if( m_itorCharacter == m_mapCharacter.end() )
	{
		return NULL;
	} //if
	return m_itorCharacter->second;
} //CW3DWorld::BeginCharacter

/**
 * 캐릭터 ID로 캐릭터 오브젝트를 얻는다.
 */
IW3DCreature* CW3DWorld::GetCharacter( DWORD dwId )
{
	CreatureMap::iterator	itor = m_mapCharacter.find( dwId );
	if( itor == m_mapCharacter.end() )
	{
		return NULL;
	} //if

	return itor->second;
} //CW3DWorld::GetCharacter

/**
 * 몬스터 ID로 몬스터 오브젝트를 얻는다.
 */
IW3DCreature* CW3DWorld::GetMonster( DWORD dwId )
{
	CreatureMap::iterator	itor = m_mapMonster.find( dwId );
	if( itor == m_mapMonster.end() )
	{
		return NULL;
	} //if

	return itor->second;
} //CW3DWorld::GetMonster

/**
 * NPC 오브젝트를 얻는다.
 *
 * @param		[IN]	wCount	NPC 리스트카운트
 * @return		NPC
 */
IW3DCreature* CW3DWorld::GetNPC( const WORD wCount )
{
	CreatureList::iterator	itor = m_listNPC.begin();
	std::advance( itor, wCount );
	if( itor == m_listNPC.end() )
	{
		return NULL;
	} //if
	IW3DCreature*	pNPC = *itor;

	return pNPC;
} //CW3DWorld::GetNPC

/**
 * 아이템 ID를 이용해서 아이템 오브젝트를 얻는다.
 */
IW3DItem* CW3DWorld::GetItem( DWORD dwId )
{
	Assert( dwId );

	ItemMap::iterator	itor = m_mapItem.find( dwId );
	if( itor == m_mapItem.end() )
	{
		return NULL;
	} //if

	return itor->second;
} //CW3DWorld::GetItem

/**
 * 기능옵젝을 얻는다 
 */
IW3DFuncEntity* CW3DWorld::GetFuncEntity( const DWORD dwId )
{
	FuncEntityMap::iterator	itor = m_mapFuncEntity.find( dwId );
	if( itor == m_mapFuncEntity.end() )
	{
		return NULL;
	} //if

	return itor->second;
} //CW3DWorld::GetFuncEntity

/**
 * Terrain을 얻는다.
 */
IW3DTerrain* CW3DWorld::GetTerrain()
{
	if( m_bIndoor )
	{	
		return m_pIndoor->GetTerrain();
	} //if

	return m_pTerrain;
} //CW3DWorld::GetTerrain

/**
 * Movable을 얻는다.
 */
BYTE* CW3DWorld::GetMovable()
{
	return m_pbMovable[m_nLayer];
} //CW3DWorld::GetMovable

/**
 * Trace Effect를 얻는다.
 */
IW3DTraceEffect* CW3DWorld::GetTraceEffect()
{
	return m_pTraceEffect;
} //CW3DWorld::GetTraceEffect

/**
 * Particles을 얻는다.
 */
IW3DParticles* CW3DWorld::GetParticles()
{
	return m_pParticles;
} //CW3DWorld::GetParticles

/**
 * 아이템 정보 관리자를 얻는다.
 */
IW3DItemInfoMgr* CW3DWorld::GetItemInfoMgr()
{
	return m_pItemInfoMgr;
} //CW3DWorld::GetItemInfoMgr

/**
 * 월드 크기를 얻는다. ( 미터 단위 )
 */
DWORD CW3DWorld::GetSize()
{
	return m_dwSize;
} //CW3DWorld::GetSize

/**
 * 맵 지역 설정.
 */
void CW3DWorld::AddMapArea( const HRGN	hRgn, const int nBGMType )
{
	_AreaRegion	area;
	area.hRgn		= hRgn;
	area.nBGMType	= nBGMType;

	m_vecAreaRgn.push_back( area );
} //CW3DWorld::AddMapArea

/**
 * 현재 지역의 BGM type을 얻는다
 */
int CW3DWorld::GetAreaBGMType( const int nX, const int nY, int* nAreaType )
{
	_AreaRegion	area;
	int			nCount = 0;

	for( int i = 0 ; i < m_vecAreaRgn.size() ; i++ )
	{
		area = m_vecAreaRgn[i];
		if( PtInRegion( area.hRgn , nX, nY ) )
		{			
			nAreaType[nCount] = area.nBGMType;
			nCount++;

			if( nCount == MAXBGM )
			{
				return nCount-1 ;
			}
		}	
	}
	
	return nCount;
} //CW3DWorld::GetAreaBGMType

/**
 * 낚시터인지 여부 반환
 *//*
BOOL CW3DWorld::IsFishingPlace( Vector3& vPos )
{
	_FishRegion area;

	for( int i=0 ; i<m_vecFishRgn.size() ; i++ )
	{
		area = m_vecFishRgn[i];
		if( PtInRegion( area.hRgn, vPos.x, vPos.y ) )
		{
			return TRUE;
		}
	}

	return FALSE;
} // CW3DWorld::IsFishingPlace( Vector3& vPos )*/

/**
 * 낚시터 영역 정보를 입력한다.
 *//*
void CW3DWorld::AddFishingArea( const HRGN hRgn, const int nPlaceNum )
{
	_FishRegion area;
	area.hRgn = hRgn;
	area.nNum = nPlaceNum;

	m_vecFishRgn.push_back( area );
} // CW3DWorld::AddFishingArea*/

/**
 * 레이어를 설정한다.
 */
void CW3DWorld::SetLayer( BYTE btLayer )
{
	if( !m_bIndoor )
	{
		m_nLayer = btLayer;
		m_pTerrain->SetLevel( btLayer );
	} //if
} //CW3DWorld::SetLayer

/**
 * Door를 Picking한다.
 */
DWORD CW3DWorld::PickDoor( const Vector3& vPick, const Vector3& vOrig, const Vector3& vDir )
{
	if( m_pDoor && m_pDoor->Pick( vPick, vOrig, vDir ) )
	{
		return m_pDoor->GetDoor();
	} //if

	return 0;
} //CW3DWorld::PickDoor

/**
 *
 */
void CW3DWorld::SetMapType( const BYTE btType )
{
	m_btMapType = btType;
	switch( btType )
	{
	case 0:
		m_bIndoor = FALSE;
		break;
	case 1:
		break;
	case 2:
	case 3:
		m_bIndoor = TRUE;
		break;
	}; //switch
} //CW3DWorld::SetMapType

/**
 *
 */
void CW3DWorld::LoadLightTable( int nId )
{
	m_pTimeMgr->LoadTable( "table.scr", nId );
} //CW3DWorld::LoadLightTable

/**
 *
 */
IW3DItem* CW3DWorld::GetNearItem( const Vector3& vPos )
{
	if( m_mapItem.empty() )		return NULL;

	ItemMap::iterator	itor = m_mapItem.begin();
	IW3DItem* pItem = NULL;

	while( itor != m_mapItem.end() )
	{
		pItem = itor->second;
		
		if( !pItem )
		{
			itor++;
			continue;
		} //if

		if( ( pItem->GetPosition() - vPos ).Length() < 2.5F )
		{
			return pItem;
		} //if..else..

		itor++;
	} //while

	return NULL;
} //CW3DWorld::GetNearItem

/**
 * Collision with a Creature
 */
IW3DCreature* CW3DWorld::CollisionWithCreature( const IW3DCreature* pPlayer, const Vector3& vPos, const Vector3& vCurrPos )
{
	IW3DCreature*	pCreature;
	Vector3			vDistance;
	float			fLength;

	CreatureMap::iterator	iter = m_mapCharacter.begin();
	while( iter != m_mapCharacter.end() )
	{
		pCreature = iter->second;
		iter++;

		if( !pCreature )
		{
			Assert( "Character is null" );
			continue;
		} //if

		if( pCreature->IsShow() )
		{
			vDistance = pCreature->GetPosition() - vPos;
			fLength = vDistance.Length();
			if( fLength < 0.5f && pCreature != pPlayer )
			{
				fLength = ( vCurrPos - pCreature->GetPosition() ).Length();
				if( fLength < 0.5F )
				{
					return NULL;
				} //if
				return pCreature;
			} //if
		} //if
	} //while
	
	iter = m_mapMonster.begin();
	while( iter != m_mapMonster.end() )
	{
		pCreature = iter->second;
		iter++;

		if( !pCreature )
		{
			Assert( "Character is null" );
			continue;
		} //if

		if( pCreature->IsShow() )
		{
			if( pCreature->CheckPoint( vPos ) )
			{
//				if( pCreature->CheckPoint( vCurrPos ) )
//				{
//					return NULL;
//				} //if
				return pCreature;
			} //if
		} //if
	} //while

	return NULL;
} //CW3DWorld::CollisionWithCreature

/**
 * Collision with the Entity
 */
IW3DFuncEntity* CW3DWorld::CollisionWithEntity( const Vector3& vPos, const Vector3& vCurrPos )
{
	FuncEntityMap::iterator		iter = m_mapFuncEntity.begin();
	while( iter != m_mapFuncEntity.end() )
	{
		IW3DFuncEntity*		pFuncEntity = iter->second;
		Assert( pFuncEntity );
		iter++;

		if( pFuncEntity->CollisionPoint( vPos ) )
		{
			if( pFuncEntity->CollisionPoint( vCurrPos ) )
			{
				return NULL;
			} //if
			return pFuncEntity;
		} //if
	} //while

	return NULL;
} //CW3DWorld::CollisionWithEntity