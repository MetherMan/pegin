/**
 * 날씨 관련 Effect
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-02 9:20p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Engine/Src/Rain.cpp $
 * 
 * 2     03-12-02 9:20p Kjmgo
 * 몇가시 수정
 * 
 * 1     03-12-01 4:17p Kjmgo
 * Weather Effect 생성
 * 
 * <HR>
 *
 * @file	Rain.cpp
 */

#include "Engine_Export.h"
#include "WeatherEffect.h"
#include <list>

/**
 *
 */
class CRain : public IWeatherEffect
{
public:
	UNDER_MEMORY_MGR( CRain );

	~CRain();
	CRain();

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
	void SetDirection( Vector3 vDir );

	/**
	 *
	 */
	void SetCount( int nCount );

private:

	/**
	 *
	 */
	struct SRain
	{
		Vector3 vPos;
		float	fPower;
	}; //struct SRain

	typedef std::list<SRain> RainList;

	RainList			m_listRain;		///< List
	IW3DBillboard*		m_pBoard;		///< Billboard
	int					m_nTotalRain;	///< 비의 총 갯수 ( 한계 )
	int					m_nRain;		///< 현재 비의 갯수
	Vector3				m_vDir;			///< 비가 내릴 방향( 내리는 속도와는 관계 없음 )
	IW3DScene*			m_pScene;		///< Scene

	/**
	 *
	 */
	BOOL		Move( Vector3* vPos, float fPower );

	/**
	 *
	 */
	void		GetRandomPos( Vector3* vPos );
}; //class CRain

/**
 *
 */
IWeatherEffect* MakeEffectRain( IW3DWorld* pWorld )
{
	CRain* pRain = new CRain;

	if( !pRain || 
		!pRain->Create( "Effect/rain.wes", pWorld ) )
	{
		SAFE_DELETE( pRain );
		return NULL;
	} //if

	return pRain;
} //MakeEffectRain

/**
 *
 */
CRain::CRain()
{
	m_pBoard = NULL;
	m_nTotalRain = 150;
	m_nRain = 0;
	m_vDir = Vector3( 0.01F, 0.01F, 0.0F );
	m_pScene = MAKE_COMPONENT( Scene );
} //CRain::CRain

/**
 *
 */
CRain::~CRain()
{
	//m_listRain.clear();
	SAFE_RELEASE( m_pBoard );
} //CRain::~CRain

/**
 *
 */
BOOL CRain::Create( const char* szFile, IW3DWorld* pWorld )
{
	Assert( szFile );

	for( int i = 0 ; i < m_nTotalRain ; i++ )
	{
		SRain rain;
		//rain = GetRandom
		GetRandomPos( &rain.vPos );
		rain.fPower = 0.3F + ( random( 10 ) / 40.0F );
		m_nRain++;

		m_listRain.push_back( rain );
	} //for

//	char szBillboard[128];
	m_pBoard = MAKE_COMPONENT( Billboard );

	if( !m_pBoard ||
		!m_pBoard->Load( "rain.bbs" ) 
 		)
	{
		return FALSE;
	} //if

	return TRUE;
} //CRain::Create

/**
 *
 */
BOOL CRain::Update( DWORD dwTick )
{
	RainList::iterator itor = m_listRain.begin();

	SRain* pRain = NULL;
	static	DWORD dwTime = 0;
	dwTime += dwTick;

	if( dwTime <= 20 )
	{
		return TRUE;
	} //if

	dwTime = 0;

	while( itor != m_listRain.end() )
	{
		pRain = &(*itor);

		if( Move( &pRain->vPos, pRain->fPower ) )
		{
			// 새로운 좌표로 갱신
			GetRandomPos( &pRain->vPos );
		} //if

		itor++;
	} //while

	return FALSE;
} //CRain::Update

/**
 *
 */
void CRain::Draw()
{
	RainList::iterator itor = m_listRain.begin();

	SRain* pRain = NULL;

	while( itor != m_listRain.end() )
	{
		pRain = &(*itor);

		m_pBoard->Draw( pRain->vPos );

		itor++;
	} //while
} //CRain::Draw

/**
 *
 */
BOOL CRain::Move( Vector3* vPos, float fPower )
{
	if( vPos->z <= 0.0F )
	{
		return TRUE;
	} //if

	float fLength = ( m_pScene->GetFrustum().m_vPos - *vPos ).Length();

	if( fLength >= 80.0F )
	{
		return TRUE;
	} //if

	*vPos = *vPos + m_vDir;
	vPos->z -= fPower;

	return FALSE;
} //CRain::Move

/**
 *
 */
void CRain::GetRandomPos( Vector3* vPos )
{
	Frustum& frustum = m_pScene->GetFrustum();
	
	int nRandom = random( 25 );
	int nMinus = random( 2 );

	if( nMinus )
	{
		vPos->x = frustum.m_vPos.x - nRandom;
	}
	else
	{
		vPos->x = frustum.m_vPos.x + nRandom;
	} //if..else..

	nRandom = random( 25 );
	nMinus = random( 2 );

	if( nMinus )
	{
		vPos->y = frustum.m_vPos.y - nRandom;
	}
	else
	{
		vPos->y = frustum.m_vPos.y + nRandom;
	} //if..else..

	vPos->z = frustum.m_vPos.z + 15.0F;

} //CRain::GetRandomPos

/**
 *
 */
void CRain::SetCount( int nCount )
{
	if( m_nRain == nCount )
	{
		return ;
	} //if

	if( m_nRain < nCount )
	{
		for( int i = 0 ; i < nCount - m_nRain ; i++ )
		{
			SRain rain;
			//rain = GetRandom
			GetRandomPos( &rain.vPos );
			rain.fPower = random( 10 ) / 10.0F;
			m_nRain++;

			m_listRain.push_back( rain );
		} //for
	}
	else
	{

		RainList::iterator itor = m_listRain.begin();

		while( itor != m_listRain.end() )
		{
			if( m_nRain <= nCount )
			{
				break;
			} //if

			m_nRain--;
			itor = m_listRain.erase( itor );
		} //while

	} //if..else..

} //CRain::SetCount