/**
 * Effect가 있는 오브젝트.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-06-24 5:59p $
 * @version	$Revision: 29 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DEffectObject.cpp $
 * 
 * 29    04-06-24 5:59p Paran2
 * 
 * 28    04-03-08 3:01p Sckyj
 * SoundModel은 Update시 사운드 재생만 하고 리턴함
 * 
 * 27    04-03-06 10:03a Sckyj
 * Sound Effect에 가청거리 요소 추가
 * 
 * 26    03-12-26 11:51a Sckyj
 * EffectObject sound max_distance 변경
 * 
 * 25    03-12-22 4:59p Sckyj
 * 2D, 3D sound 분리
 * 
 * 24    02-12-12 4:17p Sckyj
 * Sound 객체 변경
 * 
 * 23    03-12-21 3:56a Sckyj
 * 사운드 재생시 stop3D()호출하던것 지움
 * 
 * 22    03-12-20 6:21a Kjmgo
 * 메모리 관리
 * 
 * 21    03-12-20 12:53a Sckyj
 * EffectObject Sound의 Max Distance 거리를 25로 증가시킴
 * 
 * 20    03-12-16 6:47p Sckyj
 * sound distance를 10m로 낮춤
 * 
 * 19    03-11-06 10:02a Kjmgo
 * 
 * 18    03-10-27 3:24p Kjmgo
 * 코드 최적화
 * 
 * 17    03-10-01 9:22p Kjmgo
 * 소스 정리
 * 
 * 16    03-09-11 10:45p Sckyj
 * 
 * 15    03-08-02 2:42p Admin
 * 
 * 14    03-06-26 12:34p Paran
 * 
 * 13    03-06-02 8:36p Kjmgo
 * 
 * 12    03-06-02 8:31p Kjmgo
 * 
 * 11    03-05-20 5:13p Kjmgo
 * 
 * 10    03-05-20 4:52p Paran
 * 
 * 9     03-05-20 4:58p Kjmgo
 * 시간에 따른 이펙트 적용
 * 
 * 8     03-05-18 7:54a Kjmgo
 * Effect를 확실하게 Release하도록 수정
 * 
 * 7     03-05-10 1:51a Kjmgo
 * EffectObject 작업
 * 
 * 6     03-05-09 7:42a Ahastudio
 * <HR>
 *
 * @file	W3DEffectObject.cpp
 */


#include "Engine_Export.h"
#include "Effect.h"
#include <stdio.h>
#include <string>
#include <list>
#include <map>


using std::string;
using std::list;
using std::map;

struct SParticle
{
	UNDER_MEMORY_MGR( SParticle );

	string		sName;		///< 파티클 이름
	Vector3		vPos;		///< 파티클 위치
	BOOL		bAdd;		///< 추가 할것인지의 여부
	DWORD		dwTime;		///< 내부 누적 Tick
	DWORD		dwLimit;	///< 갱신 제한 시간
	BOOL		bType[12];	///< 시간대별 적용.
}; //struct SParticle

struct SEffect
{
	UNDER_MEMORY_MGR( SEffect );

	BOOL		bType[12];	///< 시간대별 적용.
	CEffect*	pEffect;	///< SpecialEffect
}; //strucy SEffect

struct SBillboard
{
	UNDER_MEMORY_MGR( SBillboard );

	BOOL			bType[12];	///< 시간대별 적용
	Vector3			vPos;		///< 위치

	IW3DBillboard*	pBillboard;	///< 빌보드
}; //struct SBillboard

struct SSound
{
	UNDER_MEMORY_MGR( SSound );

	BOOL			bType[12];	///< 시간대별 적용
	Vector3			vPos;		///< 위치
	DWORD			dwTime;
	DWORD		    dwLimit;	///< 갱신 제한 시간
	string			sSound;		///< 사운드 파일 
	UINT			nDis;		///< 가청 거리 	

	IW3DSoundObject3D* pSound;	///< 사운드 

	void Clear()
	{
		ZeroMemory( bType, 12 );
		vPos = Vector3( 0, 0, 0 );
		dwTime = dwLimit = 0;
		nDis = 0;
	}
}; //struct


static const DWORD	VERSION = 102;


/**
 * Effect Object 클래스.
 * @ingroup		Engine
 */
class CW3DEffectObject : public IW3DEffectObject
{
public:
	UNDER_MEMORY_MGR( CW3DEffectObject );

	CW3DEffectObject();
	~CW3DEffectObject();

	BOOL		Create( IW3DWorld* pWorld, IW3DModel* pModel, const char* szFileName );
	void		SetWorld( const Vector3& vPos, const Quaternion& qRot );

	BOOL		Update( const DWORD dwTick );
	void		Draw();

	/**
	 *
	 */
	void		SetFront( BOOL bFlag )
	{
		m_bFront = bFlag;
	} //SetFront

	/**
	 *
	 */
	BOOL		GetFront()
	{
		return m_bFront;
	} //GetFront

	/**
	 *
	 */
	void		SetTime( const int nTime )
	{
		m_nTime = nTime;
	}; //SetTime

	IW3DModel*	GetModel();

	/**
	 *
	 */
	Vector3& GetPosition()
	{
		return m_vPos;
	} // GetPosition

private:
	typedef list<SEffect*>			EffectList;
	typedef map<string,IW3DSpecialEffect*>	EffectMap;
	typedef list<SParticle*>		ParticleList;
	typedef list<SBillboard*>		BillboardList;
	typedef list<SSound*>			SoundList;
	
	IW3DScene*		m_pScene;			///< Scene 객체 
	IW3DWorld*		m_pWorld;			///< 월드 객체
	IW3DModel*		m_pModel;			///< 중심이 되는 Model
	EffectMap		m_mapEffect;
	EffectList		m_listEffect;		///< Effect 리스트
	ParticleList	m_listParticle;		///< 파티클 리스트
	SoundList		m_listSound;		///< 사운드 리스트
	BillboardList	m_listBillboard;	///< 빌보드 리스트

	Vector3			m_vPos;				///< 기본이 되는 위치
	Quaternion		m_qRot;				///< 기본이 되는 회전
	Matrix4			m_matWorld;			///< World Transform Matrix

	DWORD			m_dwTick;			///< 내부 경과 틱
	DWORD			m_dwTime;			///< 내부 누적 틱
	DWORD			m_dwVersion;		///< 버전
	BOOL			m_bSkip;			///< 그냥 넘어갈지의 여부	
	BOOL			m_bSoundModel;		///< Point 3D Sound용 Object인지 여부
	int				m_nTime;			///< 시간.
	BOOL			m_bFront;

	void		Cleanup();

	BOOL		LoadParticle( FILE* fp );
	BOOL		LoadSPE( FILE* fp );
	BOOL		LoadBillboard( FILE* fp );
	BOOL		LoadSound( FILE* fp );


	void		UpdateBillboard( DWORD dwTick );
	
	void		PlaySound( DWORD dwTick );
	void		DrawBillboard();
}; //class CW3DEffectObject


IMPLEMENT_COMPONENT( CW3DEffectObject, EffectObject );


/**
 * 생성자.
 */
CW3DEffectObject::CW3DEffectObject()
:	m_pWorld( NULL ),
	m_pModel( NULL ),
	m_bFront( FALSE ),
	m_bSkip( TRUE ),	
	m_bSoundModel( FALSE )
{
	m_dwTime = 0;
	m_dwTick = 0;
	m_nTime = 0;
} //CW3DEffectObject::CW3DEffectObject

/**
 * 소멸자 .
 */
CW3DEffectObject::~CW3DEffectObject()
{
	Cleanup();

	Assert( m_mapEffect.empty() );
	Assert( m_listParticle.empty() );
	Assert( m_listEffect.empty() );
	Assert( m_listSound.empty() );
	Assert( m_listBillboard.empty() );
} //CW3DEffectObject::~CW3DEffectObject

/**
 * Clean Up.
 */
void CW3DEffectObject::Cleanup()
{
	
	EffectList::iterator	itorEffect = m_listEffect.begin();
	CEffect* pEffect = NULL;

	while( itorEffect != m_listEffect.end() )
	{
	//	*itorEffect
		
		pEffect = (*itorEffect)->pEffect;

		if( pEffect )
		{
			m_pWorld->UnloadEffect( pEffect->GetEffect(), pEffect->GetName().c_str() );
		} //if

		SAFE_DELETE( (*itorEffect)->pEffect );
		SAFE_DELETE( *itorEffect );

		itorEffect = m_listEffect.erase( itorEffect );
	} //while

	ParticleList::iterator	itorParticle = m_listParticle.begin();

	while( itorParticle != m_listParticle.end() )
	{
		SParticle*	pParticle = *itorParticle;

		SAFE_DELETE( pParticle );

		itorParticle = m_listParticle.erase( itorParticle );
	} //whlie

	SoundList::iterator	itorSound = m_listSound.begin();

	while( itorSound != m_listSound.end() )
	{
		SSound*	pSound = *itorSound;

		SAFE_RELEASE( pSound->pSound );
		SAFE_DELETE( pSound );

		itorSound = m_listSound.erase( itorSound );
	} //whlie

	BillboardList::iterator	itorBillboard = m_listBillboard.begin();

	while( itorBillboard != m_listBillboard.end() )
	{
		SBillboard*	pBillboard = *itorBillboard;

		SAFE_RELEASE( pBillboard->pBillboard );
		SAFE_DELETE( pBillboard );

		itorBillboard = m_listBillboard.erase( itorBillboard );
	} //whlie

	EffectMap::iterator	itor = m_mapEffect.begin();
	while( itor != m_mapEffect.end() )
	{
		EffectMap::iterator	itorNext = itor;
		itorNext++;

		IW3DSpecialEffect* pEffect = itor->second;
		SAFE_RELEASE( pEffect );
		m_mapEffect.erase( itor );

		itor = itorNext;
	} //while	

} //CW3DEffectObject::Cleanup

/**
 * 오브젝트를 생성한다.
 */
BOOL CW3DEffectObject::Create(
	IW3DWorld*	pWorld,
	IW3DModel*	pModel,
	const char*	szFileName
	)
{
	Assert( pWorld );
	Assert( pModel );	

	string	strFileName = "Object/";
	strFileName += szFileName;	

	FILE*	fp = fopen( strFileName.c_str(), "rb" );
	if( NULL == fp )
	{
		return FALSE;
	} //if	

	char szFile[128] = "\0";
	strcpy( szFile, strFileName.c_str() );
	_strlwr( szFile );
	if( strstr( szFile, "sound_" ) )
	{
		m_bSoundModel = TRUE;
	}

	m_pWorld = pWorld;
	m_pModel = pModel;
	m_pScene = MAKE_COMPONENT( Scene );

	if( !m_pScene )
	{
		// Scene 생성 실패
		fclose( fp );
		return FALSE;
	} //if

	// Version Check -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	DWORD	dwVersion = 0;
	int		nSize = 0;
	char	szModelName[256];

	fread( &dwVersion, 4, 1, fp );

	m_dwVersion = dwVersion;

	if( dwVersion != VERSION )
	{
//		fclose( fp );
//		return FALSE;
	} //if

	// read Model -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	fread( &nSize, 4, 1, fp );
	fread( szModelName, nSize, 1, fp );

	///////////////////////////////////////////////////////////////////////// 
	// World에 귀속되는 것이므로... Model은 외부에서 입력하게 한다.
	//

	// read Particle -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	LoadParticle( fp );

	// read Special Effect -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	LoadSPE( fp );

	/////////////////

	// 1.02버전에서는 사운드와 빌보드가 추가 되었다.
	if( dwVersion == 102 )
	{
		// read Billboard -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		LoadBillboard( fp );

		// read Sound -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		LoadSound( fp );		
	} //if

	fclose( fp );

	return TRUE;
} //CW3DEffectObject::Create

/**
 *
 */
void CW3DEffectObject::SetWorld(
	const Vector3&		vPos,	///< [IN]
	const Quaternion&	qRot	///< [IN]
	)
{
	m_vPos = vPos;
	m_qRot = qRot;
	m_matWorld.SetWorld( m_vPos, m_qRot );
} //CW3DEffectObject::SetWorld

/**
 * 프레임 갱신을 한다.
 *
 * @param	dwTick		[IN] 경과 tick
 *
 * @return	...
 */
BOOL CW3DEffectObject::Update( const DWORD dwTick )
{
	static Frustum		frustum;
	static Vector3		vPos;

	frustum	= m_pScene->GetFrustum();
	vPos	= m_vPos;

	vPos.z += 1.0F;

	m_dwTick = dwTick;
	m_dwTime += dwTick;

	m_pModel->SetWorld( m_vPos, m_qRot );
	m_bSkip = !m_pModel->IsVisible();	

	if( m_bSoundModel )
	{
		PlaySound( dwTick );
		return TRUE;
	}	
	
	if( m_bSkip )
	{
		return TRUE;
	} //if
	
	// Particle
	ParticleList::iterator	itorParticle = m_listParticle.begin();
	SParticle*	pParticle = NULL;

	while( itorParticle != m_listParticle.end() )
	{
		pParticle = *itorParticle;

		pParticle->dwTime += dwTick;
		
		if( pParticle->dwTime >= pParticle->dwLimit )
		{
			pParticle->dwTime -= pParticle->dwLimit;
			pParticle->bAdd = TRUE;
		}
		else
		{
			pParticle->bAdd = FALSE;
		} //if .. else ..
		
		itorParticle++;
	} //whlie

	UpdateBillboard( dwTick );	
	PlaySound( dwTick );

	return TRUE;
} //CW3DEffectObject::Update

/**
 *
 */
void CW3DEffectObject::UpdateBillboard( DWORD dwTick )
{
	BillboardList::iterator	itorBillboard = m_listBillboard.begin();
	SBillboard*	pBillboard = NULL;

	while( itorBillboard != m_listBillboard.end() )
	{
		pBillboard = *itorBillboard;

		if( pBillboard && pBillboard->pBillboard )
		{
			pBillboard->pBillboard->Update( dwTick ) ;
		} //if

		itorBillboard++;
	} //whlie
} //CW3DEffectObject::UpdateBillboard

/**
 * 오브젝트를 그려준다.
 */
void CW3DEffectObject::Draw()
{
	if( m_bSkip )
	{
		return ;
	} //if

	if( !m_listEffect.empty() )
	{
		EffectList::iterator	itorEffect = m_listEffect.begin();

		SEffect*	p = NULL;
		CEffect*	pEffect = NULL;

		while( itorEffect != m_listEffect.end() )
		{
			p = *itorEffect;
			pEffect = p->pEffect;

			if( pEffect && p->bType[ m_nTime / 2 ] )
			{
				pEffect->Render( m_pScene, m_dwTick );
			} //if

			itorEffect++;
		} //while
	} //if


	if( !m_listParticle.empty() )
	{
		ParticleList::iterator	itorParticle = m_listParticle.begin();

		SParticle*	pParticle = NULL;
		while( itorParticle != m_listParticle.end() )
		{
			pParticle = *itorParticle;

			if( pParticle &&
				pParticle->bAdd && pParticle->bType[ m_nTime / 2 ] )
			{
				m_pWorld->AddParticle( pParticle->sName.c_str(), pParticle->vPos );
			} //if

			itorParticle++;
		} //whlie
	} //if


	if( !m_listBillboard.empty() )
	{
		BillboardList::iterator	itorBillboard = m_listBillboard.begin();

		SBillboard*	pBillboard = NULL;

		while( itorBillboard != m_listBillboard.end() )
		{
			pBillboard = *itorBillboard;

			if( pBillboard && pBillboard->bType[ m_nTime / 2 ] &&
				pBillboard->pBillboard
				)
			{
				pBillboard->pBillboard->Draw( pBillboard->vPos );
			} //if

			itorBillboard++;
		} //whlie
	} //if
} //CW3DEffectObject::Draw

/**
 *
 */
void CW3DEffectObject::PlaySound( DWORD dwTick )
{
	if( m_listSound.empty() )
	{
		return ;
	} //if

	SoundList::iterator	itor = m_listSound.begin();
	SSound* pSound = NULL;

	while( itor != m_listSound.end() )
	{
		pSound = *itor;

		if( pSound )
		{
			if( pSound->bType[ m_nTime / 2 ] )
			{
				pSound->dwTime += dwTick;

				if( pSound->dwTime >= pSound->dwLimit )
				{
					pSound->dwTime = 0;
					pSound->pSound->SetPosition( pSound->vPos );
					pSound->pSound->Play3D( pSound->sSound.c_str(), 1, pSound->nDis );
				} //if
			} 
			else
			{
				// 재생 시간이 아닌때에 재생중일경우 재생 중지
				if( !pSound->pSound->IsEnd() )
				{
					pSound->pSound->Stop3D();
				}
			} // if..else
		}		

		itor++;
	} //whlie	
} //CW3DEffectObject::PlaySound

/** 
 *	파일에서 파티클 부분을 읽어 온다.
 */
BOOL CW3DEffectObject::LoadParticle( FILE* fp )
{
	Assert( fp );

	// Count
	int nCount = 0;
	int nSize = 0;
	char strBuffer[256];

	fread( &nCount, 4, 1, fp );

	if( nCount <= 0 )
	{
		return FALSE;
	} //if

	for( int i = 0 ; i < nCount ; i++ )
	{
		SParticle* pParticle = new SParticle;
		
		pParticle->bAdd = FALSE;
		pParticle->dwTime = 0;
		
		// Name
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );

		// Particle Type
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );
		pParticle->sName = strBuffer;

		// Pos, Limit Time
		fread( &pParticle->vPos, sizeof( Vector3 ), 1, fp );
		fread( &pParticle->dwLimit, sizeof( DWORD ), 1, fp );

		if( m_dwVersion >= 101 )
		{
			fread( &pParticle->bType, sizeof( BOOL ) * 12 , 1, fp );
		}
		else
		{
			for( int i = 0 ; i < 12 ; i++ )
				pParticle->bType[i] = TRUE;
		} //if..else..

//		pParticle->vPos = pParticle->vPos * m_matWorld;
		pParticle->vPos *= m_matWorld;

		m_listParticle.push_back( pParticle );
	} //for

	return TRUE;
} //CW3DEffectObject::LoadParticle

/** 
 *	파일에서 SPE 부분을 읽어 온다.
 */
BOOL CW3DEffectObject::LoadSPE( FILE* fp )
{
	Assert( fp );

	// Count
	int nCount = 0;
	int nSize = 0;
	Vector3		vPos( 0.0F, 0.0F, 0.0F );
	Quaternion	qRot;
	char strBuffer[256];

	fread( &nCount, 4, 1, fp );

	if( nCount <= 0 )
	{
		return FALSE;
	} //if

	for( int i = 0 ; i < nCount ; i++ )
	{
		SEffect* p = new SEffect;
		CEffect* pEffect = new CEffect;
		
		//pEffect->Create(
		
		// Name
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );

		// Particle Type
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );

		// Pos, Limit Time
		fread( &vPos, sizeof( Vector3 ), 1, fp );
		fread( &qRot, sizeof( Quaternion ), 1, fp );

		if( m_dwVersion >= 101 )
		{
			fread( &p->bType, sizeof( BOOL ) * 12 , 1, fp );
		}
		else
		{
			for( int i = 0 ; i < 12 ; i++ )
				p->bType[i] = TRUE;
		} //if..else..

		vPos *= m_matWorld;
		qRot = m_qRot;

		IW3DSpecialEffect* pSpecialEffect = NULL;
		
		pSpecialEffect = m_pWorld->LoadEffect( strBuffer );

		if( !pEffect->Create( strBuffer, vPos, qRot, TRUE, pSpecialEffect ) )
		{
			// 생성 실패는 단한가지, SpecialEffect가 NULL일 경우이다.
			Assert( !"Effect 생성 실패" );
			SAFE_DELETE( p );
			SAFE_DELETE( pEffect );
			continue;
		} //if

		p->pEffect = pEffect;

		m_listEffect.push_back( p );	
	} //for

	return TRUE;
} //CW3DEffectObject::LoadSPE

/**
 *
 */
BOOL CW3DEffectObject::LoadBillboard( FILE* fp )
{
	Assert( fp );

	// Count
	int nCount = 0;
	int nSize = 0;
	char strBuffer[256];

	fread( &nCount, 4, 1, fp );

	if( nCount <= 0 )
	{
		return FALSE;
	} //if

	for( int i = 0 ; i < nCount ; i++ )
	{
		SBillboard* pBillboard = new SBillboard;
		
		// Name
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );

		// Particle Type
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );

		pBillboard->pBillboard = MAKE_COMPONENT( Billboard );

		if( !pBillboard->pBillboard->Load( strBuffer ) )
		{ 
			SAFE_RELEASE( pBillboard->pBillboard );
		} //if

		// Pos, Limit Time
		fread( &pBillboard->vPos, sizeof( Vector3 ), 1, fp );
		fread( &pBillboard->bType, sizeof( BOOL ) * 12 , 1, fp );
	
		pBillboard->vPos *= m_matWorld;

		m_listBillboard.push_back( pBillboard );
	} //for

	return TRUE;
} //CW3DEffectObject::LoadBillboard

/**
 *
 */
BOOL CW3DEffectObject::LoadSound( FILE* fp )
{
	Assert( fp );

	// Count
	int nCount = 0;
	int nSize = 0;
	char strBuffer[256];

	fread( &nCount, 4, 1, fp );

	if( nCount <= 0 )
	{
		return FALSE;
	} //if

	for( int i = 0 ; i < nCount ; i++ )
	{
		SSound* pSound = new SSound;
		pSound->Clear();
		
		// Name
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );

		// Particle Type
		fread( &nSize, 4, 1, fp );
		fread( strBuffer, nSize, 1, fp );

		pSound->pSound = MAKE_COMPONENT( SoundObject3D );

		pSound->sSound = strBuffer;

		// Pos, Limit Time
		fread( &pSound->vPos, sizeof( Vector3 ), 1, fp );
		fread( &pSound->dwLimit, sizeof( DWORD ), 1, fp );
		fread( &pSound->bType, sizeof( BOOL ) * 12 , 1, fp );

		fread( &pSound->nDis, sizeof( UINT ), 1, fp );
	
		pSound->vPos *= m_matWorld;

		m_listSound.push_back( pSound );
	} //for

	return TRUE;
} //CW3DEffectObject::LoadSound

/** 
 *	Model을 가져온다.
 */
IW3DModel* CW3DEffectObject::GetModel()
{
	Assert( m_pModel );
	return m_pModel;
} //CW3DEffectObject::GetModel
