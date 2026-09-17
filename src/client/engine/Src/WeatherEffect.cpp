/**
 * 날씨 관련 Effect
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-18 4:12p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /Engine/Src/WeatherEffect.cpp $
 * 
 * 7     03-12-18 4:12p Kjmgo
 * 낙엽추가
 * 
 * 6     03-12-08 3:39p Kjmgo
 * 기본은 맑음이다.
 * 
 * 4     03-12-03 9:57a Kjmgo
 * 눈과 비의 순서를 바꿈
 * 
 * 3     03-12-02 9:20p Kjmgo
 * 몇가시 수정
 * 
 * 2     03-12-01 4:21p Kjmgo
 * 일단 기본으로 Weather Effect를 사용하지 않음
 * 
 * 1     03-12-01 4:17p Kjmgo
 * Weather Effect 생성
 * 
 * <HR>
 *
 * @file	WeatherEffect.cpp
 */

#include "Engine_Export.h"
#include "WeatherEffect.h"

/**
 *
 */
CWeather::CWeather()
{
	for( int i = 0 ; i < MAX_EFFECT ; i++ )
	{
		m_pEffect[i] = NULL;
	} //for

	m_nEffect = -1;

} //CWeather::CWeather

/**
 *
 */
CWeather::~CWeather()
{
	for( int i = 0 ; i < MAX_EFFECT ; i++ )
	{
		SAFE_RELEASE( m_pEffect[i] );
	} //for
}  //CWeather::~CWeather

/**
 *
 */
BOOL CWeather::Create( IW3DWorld* pWorld )
{
	for( int i = 0 ; i < MAX_EFFECT ; i++ )
	{
		m_pEffect[i] = MakeEffect( i, pWorld );

		if( !m_pEffect[i] )
		{
			//Assert( !"Effect 생성 실패입니다." );
			//return FALSE;
		} //if
	} //for

	return TRUE;
} //CWeather::Create

/**
 *
 */
void CWeather::Update( const DWORD dwTick )
{
	if( m_nEffect < 0 )
	{
		return ;
	} //if

	if( m_pEffect[m_nEffect] )
	{
		m_pEffect[m_nEffect]->Update( dwTick );
	} //if
} //CWeather::Update

/**
 *
 */
void CWeather::Draw()
{
	if( m_nEffect < 0 )
	{
		return ;
	} //if	

	if( m_pEffect[m_nEffect] )
	{
		m_pEffect[m_nEffect]->Draw();
	} //if
} //CWeather::Draw

/**
 *
 */
IWeatherEffect*	CWeather::MakeEffect( int nId, IW3DWorld* pWorld )
{
	switch( nId )
	{
	case SNOW:
		return MakeEffectSnow( pWorld );
		break;
	case RAIN:
		return MakeEffectRain( pWorld );
		break;
	case LEAF:
		return MakeEffectLeaf( pWorld );
		break;
	} //switch

	return NULL;
} //CWeather::MakeEffect