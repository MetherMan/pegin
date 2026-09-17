/**
 * ³¯¾¾ °ü·Ã Effect
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	·Î³Ä
 * @date	$Date: 03-12-18 5:16p $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Engine/Src/WeatherEffect.h $
 * 
 * 5     03-12-18 5:16p Kjmgo
 * 
 * 4     03-12-18 4:12p Kjmgo
 * ³«¿±Ãß°¡
 * 
 * 3     03-12-03 9:57a Kjmgo
 * ´«°ú ºñÀÇ ¼ø¼­¸¦ ¹Ù²Þ
 * 
 * 2     03-12-01 4:17p Kjmgo
 * Weather Effect Ãß°¡
 * 
 * 1     03-12-01 10:43a Kjmgo
 *
 * <HR>
 *
 * @file	WeatherEffect.h
 */

#ifndef __WeatherEffect_Header__
#define __WeatherEffect_Header__

#define MAX_EFFECT 4

/**
 *
 */
class IWeatherEffect : public IComponent
{
public:
	
	/**
	 *
	 */
	virtual BOOL Create( const char* szFile, IW3DWorld* pWorld ) = 0;

	/**
	 *
	 */
	virtual BOOL Update( DWORD dwTick ) = 0;

	/**
	 *
	 */
	virtual void Draw() = 0;

	/**
	 *
	 */
	virtual void SetCount( int nCount ) = 0;

}; //class 

/**
 *	Effect¸¦ »ý¼ºÇÑ´Ù.
 */
IWeatherEffect*		MakeEffectRain( IW3DWorld* pWorld );
IWeatherEffect*		MakeEffectSnow( IW3DWorld* pWorld );
IWeatherEffect*		MakeEffectLeaf( IW3DWorld* pWorld );

/**
 *
 */
class CWeather
{
public:
	~CWeather();
	CWeather();

	/**
	 *
	 */
	BOOL	Create( IW3DWorld* pWorld );
	
	/**
	 *
	 */
	void	Update( const DWORD dwTick );

	/**
	 *
	 */
	void	Draw();

	/**
	 *
	 */
	void	SetCount( int nCount );

	/**
	 *
	 */
	void	SetWeather( int nWeather )
	{
		if( nWeather == 3 )
			nWeather = 0;

		m_nEffect = nWeather - 1;
	} //SetWeather

private:

	/**
	 *
	 */
	enum
	{
		NONE = -1,
		SNOW = 0,
		RAIN,
		LEAF,
	}; //enum

	int					m_nEffect;					///< ÇöÀç ÀÌÆåÆ® 
	IWeatherEffect*		m_pEffect[MAX_EFFECT];		///< ÀÌÆåÆ® ¹è¿­

	/**
	 *
	 */
	IWeatherEffect*		MakeEffect( int nId, IW3DWorld* pWorld );

}; //class CWeather



#endif //#ifndef __WeatherEffect_Header