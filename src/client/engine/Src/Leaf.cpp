/**
 * ³¯¾¾ °ü·Ã Effect
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 03-12-18 4:12p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /Engine/Src/Leaf.cpp $
 * 
 * 2     03-12-18 4:12p Kjmgo
 * ³«¿±Ãß°¡
 * 
 * 1     03-12-17 3:45p Kjmgo
 * 
 * 
 * <HR>
 *
 * @file	Leaf.cpp
 */

#include "Engine_Export.h"
#include "WeatherEffect.h"
#include <list>

/**
 *
 */
class CLeaf : public IWeatherEffect
{
public:
	UNDER_MEMORY_MGR( CLeaf );

	~CLeaf();
	CLeaf();

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
	int			m_nCount;		///< ÇöÀç Ãâ·ÂµÇ°í ÀÖ´Â ÆÄÆ¼Å¬ °¹¼ö ( ¿¹»ó °¹¼ö )
	DWORD		m_dwTime;		///< 


	/**
	 *
	 */
	Vector3	GetRandomPos();

}; //class CLeaf

/**
 *
 */
IWeatherEffect* MakeEffectLeaf( IW3DWorld* pWorld )
{
	CLeaf* pSnow = new CLeaf;

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
CLeaf::CLeaf()
{
	m_nCount = 0;
	m_dwTime = 0;
	m_pScene = MAKE_COMPONENT( Scene );
	m_nMaxCount = 200;
} //CLeaf::CLeaf

/**
 *
 */
CLeaf::~CLeaf()
{
	
} //CLeaf::~CLeaf

/**
 *
 */
BOOL CLeaf::Create( const char* szFile, IW3DWorld* pWorld )
{
	m_pWorld = pWorld;

	m_nCount = 0;
	return TRUE;
} //CLeaf::Create

/**
 *
 */
BOOL CLeaf::Update( DWORD dwTick )
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

		int nRandom = random( 10 );

		if( nRandom >= 5 )
		{
			m_pWorld->AddParticle( "leaf", GetRandomPos() );
		}
		else
		{
			m_pWorld->AddParticle( "leaf2", GetRandomPos() );
		} //if..else..
			
	} //if


	return FALSE;
} //CLeaf::Update

/**
 *
 */
void CLeaf::Draw()
{

} //CLeaf::Draw

/**
 *
 */
void CLeaf::SetCount( int nCount )
{

} //CLeaf::SetCount

/**
 *
 */
Vector3 CLeaf::GetRandomPos()
{
	Frustum& frustum = m_pScene->GetFrustum();

	Vector3 vPos;
	
	int nRandom = random( 60 );
	int nMinus = random( 2 );

	if( nMinus )
	{
		vPos.x = frustum.m_vPos.x - nRandom;
	}
	else
	{
		vPos.x = frustum.m_vPos.x + nRandom;
	} //if..else..

	nRandom = random( 60 );
	nMinus = random( 2 );

	if( nMinus )
	{
		vPos.y = frustum.m_vPos.y - nRandom;
	}
	else
	{
		vPos.y = frustum.m_vPos.y + nRandom;
	} //if..else..

	vPos.z = m_pWorld->GetHeight( frustum.m_vPos ) + 0.5F;
	//vPos.z = frustum.m_vPos.z + 2.0F;

	return vPos;
} //CLeaf::GetRandomPos