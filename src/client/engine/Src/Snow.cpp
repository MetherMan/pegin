/**
 * 날씨 관련 Effect
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-18 4:12p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Engine/Src/Snow.cpp $
 * 
 * 3     03-12-18 4:12p Kjmgo
 * 낙엽추가
 * 
 * 2     03-12-02 9:20p Kjmgo
 * 몇가시 수정
 * 
 * 1     03-12-01 4:17p Kjmgo
 * Weather Effect 생성
 * 
 * <HR>
 *
 * @file	Snow.cpp
 */

#include "Engine_Export.h"
#include "WeatherEffect.h"


/**
 *
 */
class CSnow : public IWeatherEffect
{
public:
	UNDER_MEMORY_MGR( CSnow );

	~CSnow();
	CSnow();

	/**
	 *
	 */
	BOOL Create( const char* szFile, IW3DWorld* pWorld );

	/**
	 *
	 */
	BOOL Update( DWORD dwTick );

	/**
	 *
	 */
	void Draw();

	/**
	 *
	 */
	void SetCount( int nCount );

private:

	IW3DScene*	m_pScene;
	IW3DWorld*	m_pWorld;

	int			m_nMaxCount;
	int			m_nCount;		///< 현재 출력되고 있는 파티클 갯수 ( 예상 갯수 )
	DWORD		m_dwTime;		///< 


	/**
	 *
	 */
	Vector3	GetRandomPos();

}; //class CSnow

/**
 *
 */
IWeatherEffect* MakeEffectSnow( IW3DWorld* pWorld )
{
	CSnow* pSnow = new CSnow;

	if( !pSnow || 
		!pSnow->Create( "Effect/snow.wes", pWorld ) )
	{
		SAFE_DELETE( pSnow );
		return NULL;
	} //if

	return pSnow;
} //MakeEffectSnow

/**
 *
 */
CSnow::CSnow()
{
	m_nCount = 0;
	m_dwTime = 0;
	m_pScene = MAKE_COMPONENT( Scene );
	m_nMaxCount = 200;
} //CSnow::CSnow

/**
 *
 */
CSnow::~CSnow()
{
	
} //CSnow::~CSnow

/**
 *
 */
BOOL CSnow::Create( const char* szFile, IW3DWorld* pWorld )
{
	m_pWorld = pWorld;
	m_nCount = 0;

	return TRUE;
} //CSnow::Create

/**
 *
 */
BOOL CSnow::Update( DWORD dwTick )
{
	static DWORD dwTime = 0;
	m_dwTime += dwTick;
	dwTime += dwTick;

	if( m_dwTime >= 10000 )
	{
		m_nCount--;
	} //if

	if( dwTime >= 1000 && m_nCount < m_nMaxCount )
	{
		m_nCount++;
		m_pWorld->AddParticle( "snow", GetRandomPos() );
	} //if


	return FALSE;
} //CSnow::Update

/**
 *
 */
void CSnow::Draw()
{

} //CSnow::Draw

/**
 *
 */
void CSnow::SetCount( int nCount )
{

} //CSnow::SetCount

/**
 *
 */
Vector3 CSnow::GetRandomPos()
{
	Frustum& frustum = m_pScene->GetFrustum();

	Vector3 vPos;
	
	int nRandom = random( 25 );
	int nMinus = random( 2 );

	if( nMinus )
	{
		vPos.x = frustum.m_vPos.x - nRandom;
	}
	else
	{
		vPos.x = frustum.m_vPos.x + nRandom;
	} //if..else..

	nRandom = random( 25 );
	nMinus = random( 2 );

	if( nMinus )
	{
		vPos.y = frustum.m_vPos.y - nRandom;
	}
	else
	{
		vPos.y = frustum.m_vPos.y + nRandom;
	} //if..else..

	vPos.z = frustum.m_vPos.z + 15.0F;

	return vPos;
} //CSnow::GetRandomPos