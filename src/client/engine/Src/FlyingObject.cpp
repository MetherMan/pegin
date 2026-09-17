/**
 * 날으는 오브젝트 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-19 7:11p $
 * @version	$Revision: 33 $
 *
 * <HR>
 * $Log: /Engine/Src/FlyingObject.cpp $
 * 
 * 33    05-02-19 7:11p Sk8snow2
 * 
 * 32    04-12-09 6:28p Sk8snow2
 * 
 * 31    04-01-13 9:43p Kjmgo
 * 사양 설정 적용
 * 
 * 30    04-01-06 12:01a Kjmgo
 * 버그 수정및, 코드 정리
 * 
 * 29    03-12-22 4:59p Sckyj
 * 2D, 3D sound 분리
 * 
 * 28    03-12-21 3:56a Sckyj
 * 사운드 재생시 stop3D()호출하던것 지움
 * 
 * 27    03-12-20 9:08p Kjmgo
 * 버그수정 ㅠ.ㅜ
 * 
 * 26    03-12-20 6:21a Kjmgo
 * 메모리 관리
 * 
 * 25    03-12-20 3:33a Sckyj
 * 사운드 재생 거리(Max Script)를 스크립트로 처리함
 * 
 * 24    03-12-19 10:02p Kjmgo
 * 
 * 23    03-12-19 9:38p Kjmgo
 * 
 * 22    03-12-18 5:47p Kjmgo
 * 구조 변경
 * 
 * 21    03-12-17 11:01p Kjmgo
 * Map마다 따로 적용가능 하도록 수정 ( 'Map' ごとに 的用 )
 * 
 * 20    03-12-12 8:03p Sckyj
 * Sound 재생시 파일이 없을경우 재생안하도록 수정
 *  
 * 17    03-12-12 6:37p Sckyj
 * Sound 재생 기능 추가
 * 
 * 16    03-12-12 5:45a Kjmgo
 * 수정
 * 
 * 15    03-12-12 12:18a Kjmgo
 * 랜덤하게, 2.0F미터까지 높이 조절
 * 
 * 14    03-12-11 10:47a Kjmgo
 * 
 * 13    03-12-01 10:05a Kjmgo
 * 필요없는 코드 삭제, Scene 을 생성자에서 생성
 * 
 * 12    03-08-22 11:12a Kjmgo
 * 
 * 11    03-05-13 11:24p Kjmgo
 * <HR>
 *
 * @file	FlyingObject.cpp
 */


#include "Engine_Export.h"
#include "FlyingObject.h"

/**
 * 랜덤한 위치를 얻는다.
 */
static Vector3 GetRandomPos()
{
	Vector3		vPos;

	vPos.x = (float)random( 500 );
	vPos.y = (float)random( 500 );
	vPos.z = 0.0F;

	return vPos;
} //GetRandomPos

/**
 * 랜덤한 위치를 얻는다.
 */
static Vector3 GetRandomPos( int nX, int nY, int nRange)
{
	Vector3		vPos;

	int n = random( 10 );

	if( n >= 5 )
	{
		vPos.x = nX + (float)random( nRange );
	}
	else
	{
		vPos.x = nX - (float)random( nRange );
	} //if..else..

	n = random( 10 );

	if( n >= 5 )
	{
		vPos.y = nY + (float)random( nRange );
	}
	else
	{
		vPos.y = nY - (float)random( nRange );
	} //if..else..

	vPos.z = 0.0F;

	return vPos;
} //GetRandomPos


/**
 * 목표까지 일정 속도로 움직인 좌표를 얻는다.
 */
static Vector3 Move(
	const Vector3&	vPos,		///< [IN] 시작 위치
	const Vector3&	vTarget,	///< [IN] 목표 좌표
	const DWORD		dwSpeed		///< [IN] 이동 속도
	)
{
	Vector3	vDist = vTarget - vPos;

	float	fDistance	= vDist.Length();
	float	fOffset		= static_cast<float>( dwSpeed ) / 1000;

	Vector3 vDir = vDist.Normal();

	if( fDistance < fOffset )
	{
		fOffset = vDist.Length();
	} //if

	return vPos + vDir * fOffset;
} //Move

/**
 * 생성자.
 */
CFlyingObject::CFlyingObject()
:	m_nQuality		( 0 ),
	m_dwTick		( 0 ),
	m_nCount		( 0 ),
	m_pScene		( NULL )
{
	m_pScene = MAKE_COMPONENT( Scene );

	Assert( m_pScene );
} //CFlyingObject::CFlyingObject

/**.
 * 소멸자.
 */
CFlyingObject::~CFlyingObject()
{
	ObjectArray::iterator itorArray = m_aObject.begin();

	while( itorArray != m_aObject.end() )
	{
		SAFE_DELETE( *itorArray );
		itorArray = m_aObject.erase( itorArray );
	} //while

	ObjectList::iterator itor = m_listObject.begin();
	ObjectList::iterator itorNext;

	while( itor != m_listObject.end() )
	{	
		itorNext = itor;
		itorNext++;

		SFlyingObject*	pObject = *itor;

		SAFE_DELETE( pObject );

		m_listObject.erase( itor );

		itor = itorNext;
	} //while

	ModelMap::iterator itorModel = m_mapModel.begin();
	ModelMap::iterator itorModelNext;

	while( itorModel != m_mapModel.end() )
	{
		itorModelNext = itorModel;
		itorModelNext++;

		IW3DModel*	pModel = itorModel->second;

		SAFE_RELEASE( pModel );

		m_mapModel.erase( itorModel );

		itorModel = itorModelNext;
	} //while	

	Assert( m_aObject.empty() );
	Assert( m_mapModel.empty() );
	Assert( m_listObject.empty() );

} //CFlyingObject::~CFlyingObject

/**
 * 객체들의 정보를 담은 파일을 읽는다.
 *
 * @return	성공 여부
 */
BOOL CFlyingObject::Load( const int nId, const char* szFileName )
{
	Assert( m_pWorld );

	//
	// If User's Choice Is Low Quality, Not Load.
	//
	if( m_nQuality >= 2 )
	{
		return TRUE;
	} //if

	FILE* fp = fopen( szFileName, "rb" );

	if( NULL == fp )
	{
		Assert( !"파일이 없다." );
		return FALSE;
	} //if

	char	szBuffer[256];
	char	szKeyword[80];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );
		*szKeyword = NULL;
		sscanf( szBuffer, "%s", szKeyword );
		
		if( szKeyword[0] == NULL )
		{
			continue;
		} //if

		if( !strcmp( szKeyword, "[MAP]" ) )
		{
			int nMap = 0;
			if( sscanf( szBuffer, "%s %d", szKeyword, &nMap ) != 2 )
			{
				Assert( !"sscanf 오류" );
			}

			if( nMap != nId )
			{
				continue;
			} //if

			while( !feof( fp ) )
			{
				fgets( szBuffer, 256, fp );
				*szKeyword = NULL;
				sscanf( szBuffer, "%s", szKeyword );

				if( szKeyword[0] == NULL )
				{
					continue;
				} //if
				
				if( !strcmp( szKeyword, "[OBJECT]" ) )
				{
					if( sscanf( szBuffer, "%s %d", szKeyword, &m_nCount ) != 2 )
					{
						Assert( !"sscanf 오류" );
					}

					// if User's Choice is Middle Quality, 
					// Separate a Count into two.
					
					if( m_nQuality == 1 )
					{
						m_nCount = m_nCount / 2;
					} //if

					SFlyingObject* pObject = ReadObject( fp );
					m_aObject.push_back( pObject );

					
				}
				else if( !strcmp( szKeyword, "[/MAP]" ) )
				{
					break;
				} //if...else...
			} //while

		} //if

	} //while

	fclose( fp );

	return TRUE;
} //CFlyingObject::Load

/**
 * 객체들의 정보를 담은 파일을 읽는다.
 *
 * @return	성공 여부
 */
SFlyingObject* CFlyingObject::ReadObject( FILE* fp )
{
	Assert( fp );

	char	szBuffer[256];
	char	szKeyword[80];
	char	szDummy[80];	

	// 이펙트  정보

	SFlyingObject* pObject = new SFlyingObject;

	pObject->dwStopTime = 0;
	pObject->fRange = 30.0F;
	pObject->fHeight = 6.0F + ( random( 200 ) / 100.0F );
	pObject->dwTick = 0;
	pObject->nSpeed = 100;

	// Model data 읽기
	fgets( szBuffer, 256, fp );
	*szKeyword = NULL;
	sscanf( szBuffer, "%s", szKeyword );

	if( !strcmp( szKeyword, "[MODEL]" ) )
	{
		if( sscanf( szBuffer, "%s %s %d %d %f", szKeyword, szDummy, &pObject->nSpeed, &pObject->dwStopTime, &pObject->fRange ) != 5 )
		{
			Assert( !"sscanf 오류" );
		}

		pObject->pModel = LoadModel( szDummy );
		
		if( !pObject->pModel )
		{
			Assert( !"모델이 없대>_<b" );				
		} //if
	}
	else
	{
		SAFE_DELETE( pObject );
		return NULL;
	} //if...else...
	
	// Sound data 읽기
	fgets( szBuffer, 256, fp );
	*szKeyword = NULL;
	sscanf( szBuffer, "%s", szKeyword );

	if( !strcmp( szKeyword, "[SOUND]" ) )
	{
		if( sscanf( szBuffer, "%s %s %d %d", szKeyword, pObject->szSound, &pObject->nLimit, &pObject->nDistance ) != 4 )
		{
			Assert( !"sscanf 오류" );
		}

	}	

	return pObject;
} //CFlyingObject::ReadObject

/**
 * 날으는 객체를 추가한다.
 *
 * @return	성공 여부
 */
BOOL CFlyingObject::Add()
{
	return TRUE;
} //CFlyingObject::Add

/**
 * 모델을 읽는다.
 */
IW3DModel* CFlyingObject::LoadModel( const string& sFileName )
{
	ModelMap::iterator itor = m_mapModel.find( sFileName );

	if( itor != m_mapModel.end() )
	{	// 이미 읽어온 모델
		IW3DModel*	pModel = itor->second;
		Assert( pModel );
		return pModel;
	} //if

	IW3DModel*	pModel = MAKE_COMPONENT( Model );
	CHECK_PTR_RETURN( pModel );

	if( !pModel->Load( "FlyingObject", ( sFileName + ".mod" ).c_str() ) )
	{
		Assert( !"모델 읽기 실패" );
		SAFE_RELEASE( pModel );
		return NULL;
	} //if

	pModel->LoadAnimation( "FlyingObject", ( sFileName + ".ani" ).c_str() );
	m_mapModel.insert( std::make_pair( sFileName, pModel ) );

	return pModel;
} //CFlyingObject::LoadModel

/**
 * 날으는 오브젝트를 갱신한다.
 */
void CFlyingObject::Update( const DWORD dwTick )
{
	Assert( m_pWorld );

	if( m_nCount <= 0 || m_aObject.empty() ) return ;

	Frustum&	frustum = m_pScene->GetFrustum();
	Vector3& vPos = frustum.m_vPos;

	int nX = ( vPos.x );
	int nY = ( vPos.y );

	SFlyingObject*		pObject = NULL;
	ObjectList::iterator itor;

	int nCount = m_listObject.size();
	int nCloneCount = m_aObject.size();

	if( nCount < m_nCount )
	{
		int n = 0;
		for( int i = nCount ; i <= m_nCount ; i++ )
		{
			SFlyingObject* p = new SFlyingObject;

			if( !p )	continue;

			*p = *m_aObject[n];
			if( p->szSound )
			{
				p->pSound = MAKE_COMPONENT( SoundObject3D );
				if( !p->pSound )
				{
					Assert( !"사운드 생성 실패" );
				} //if
			} //if			
			
			p->vPos = GetRandomPos( nX, nY, 60 );
			p->vPos.z = m_pWorld->GetHeight( p->vPos ) + p->fHeight;
			
			p->vPos.z += ( random( 100 ) / 50.0F );
			p->vTarget = GetRandomPos( nX, nY, 60 );
			p->vTarget.z = m_pWorld->GetHeight( p->vTarget ) + p->fHeight;

			if( p->vTarget.z <= 0.0F )
			{
				p->vTarget.z = 0.0F;
			} //if

			p->vTarget.z += ( random( 100 ) / 50.0F );
			
			m_listObject.push_back( p );

			n++;

			if( n >= nCloneCount )
			{
				n = 0;
			} //if
		} //for
	} //if


	itor = m_listObject.begin();
	pObject = NULL;

	while( itor != m_listObject.end() )
	{
		pObject = *itor;

		if( pObject )
		{
			// 사운드 플레이
			if( pObject->dwSoundTick >= pObject->nLimit && pObject->szSound )
			{
				pObject->dwSoundTick = 0;

// 				pObject->pSound->Stop3D();
				pObject->pSound->SetPosition( pObject->vPos );
				pObject->pSound->Play3D( pObject->szSound, 1, pObject->nDistance );
			}
			else
			{
				pObject->dwSoundTick += dwTick;
			} //if..else..
			
			pObject->dwTick = 0;
		
			Vector3	vDist = pObject->vTarget - pObject->vPos;

			float	fDistance = vDist.Length();

			if( ( pObject->vTarget == pObject->vPos ) || fDistance <= 1.0F )
			{	
				// 도착 했다. 리스트에서 삭제 한다.	
				SAFE_DELETE( pObject );
				itor = m_listObject.erase( itor );
				continue;
			} //if

			// 지정된 목표로 이동한다.
			pObject->vPos	= Move( pObject->vPos, pObject->vTarget, pObject->nSpeed );
			Vector3	vDir = ( pObject->vTarget - pObject->vPos ).Normal();
			pObject->qRot = vDir.GetQuaternion();
		} //if 
		itor++;
	} //while

	m_pWorld->GetTerrain()->SetUseLevel( TRUE );
	m_dwTick += dwTick;	

} //CFlyingObject::Update

/**
 * 날으는 오브젝트를 그린다.
 */
void CFlyingObject::Draw()
{
	if( m_nCount <= 0 || m_aObject.empty() ) return ;

	Assert( m_pScene );
	Assert( m_pWorld );

	Frustum&	frustum = m_pScene->GetFrustum();
	Vector3& vPos = frustum.m_vPos;

	int nX = ( vPos.x / 10 );
	int nY = ( vPos.y / 10 );

	int nStartX = MAX( nX - 7, 0 );
	int nStartY = MAX( nY - 7, 0 );
	int nEndX = MIN( nX + 7, MAX_GRID );
	int nEndY = MIN( nY + 7, MAX_GRID );
	DWORD dwTime = m_dwTick;

	SFlyingObject*	pObject = NULL;
	ObjectList::iterator itor = m_listObject.begin();
	
	while( itor != m_listObject.end() )
	{
		pObject = *itor;

		if( pObject && pObject->pModel )
		{
			pObject->pModel->SetWorld( pObject->vPos, pObject->qRot );

			if( pObject->pModel->IsVisible() )
			{
				pObject->pModel->Render( dwTime );
				dwTime += 80;
			} //if
		} //if
		itor++;
	} //while	

} //CFlyingObject::Draw

/**
 * 날으는 오브젝트의 그림자를 그린다.
 */
void CFlyingObject::DrawShadow()
{
/*	Assert( m_pScene );

	Frustum&	frustum = m_pScene->GetFrustum();

	ObjectList::iterator itor = m_listObject.begin();

	while( itor != m_listObject.end() )
	{	
		SFlyingObject*	pObject = *itor;

		if( pObject && pObject->pModel )
		{
			pObject->pModel->SetWorld( pObject->vPos, pObject->qRot );

			if( pObject->pModel->IsVisible() )
			{

				if( pObject->pShadow->IsProjection() )
				{
					m_pWorld->GetTerrain()->InsertShadow( m_pShadow, m_vPos );

					pObject->pShadow->Render( pScene->GetFrustum().m_matView );
				}

			} //if
		} //if

		itor++;
	} //while*/
} //CFlyingObject::Draw

