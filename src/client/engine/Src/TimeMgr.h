/**
 * 시간 관련
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-17 11:21a $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Engine/Src/TimeMgr.h $
 * 
 * 5     03-12-17 11:21a Kjmgo
 * 맵마다 시간 적용
 * 
 * 4     03-12-11 4:10a Kjmgo
 * 시간 적용( NextTime을 서버에서 얻어옴 )
 * 
 * 3     03-12-03 8:03p Kjmgo
 * 버그 수정
 * 
 * 2     03-12-03 5:26p Kjmgo
 * 시간 변화 추가
 * 
 * 1     03-12-03 3:58p Kjmgo
 * 처음 생성 >_<
 * 
 * 
 * <HR>
 *
 * @file	TimeMgr.h
 */

#ifndef __Time_Manager_Header__
#define __Time_Manager_Header__

#define TOTAL_TIME	24

#include <map>

/**
 *
 */
struct	SColor
{
	int nRed;
	int nGreen;
	int nBlue;

	/**
	 *
	 */
	SColor()
	{
		nRed = 255;
		nGreen = 255;
		nBlue = 255;
	} //SColor	

	void SetColor( int r, int g, int b )
	{
		nRed = r;
		nGreen = g;
		nBlue = b;
	} //SetColor

	/**
	 *
	 */
	DWORD GetColor()
	{
		return ( nRed << 16 ) | ( nGreen << 8 ) | nBlue;
	} //GetColor

	/**
	 *
	 */
	DWORD InterpolateToColor( SColor color, float f )
	{
		int red = Interpolate( nRed, color.nRed, f );
		int green = Interpolate( nGreen, color.nGreen, f );
		int blue = Interpolate( nBlue, color.nBlue, f );

		return ( red << 16 ) | ( green << 8 ) | blue;
	} //Interpolate
}; //struct SColor

/**
 *
 */
class CTimeMgr
{
public:
	MEMORY_UNDER_MGR( CTimeMgr );

	CTimeMgr();
	~CTimeMgr();

	/**
	 *
	 */
	void		Init( IW3DWorld* pWorld );

	/**
	 *
	 */
	BOOL		LoadTable( const char* szFile, int nId );

	/**
	 *
	 */
	void		Update( const DWORD dwTick );

	/**
	 *
	 */
	void		SetTime( int nTime, int nNextTime, DWORD dwNext );

	/**
	 *
	 */
	BOOL		LoadLight( const char* szFile );

	/**
	 *
	 */
	BOOL		LoadLightNext( const char* szFile );

private:

//	typedef std::map<int, int[TOTAL_TIME]> MapTable;
//	MapTable	m_aMap;

	WORD		m_wMapId;			///< Map의 번호
	int			m_aTable[TOTAL_TIME];
	BOOL		m_aTableBool[TOTAL_TIME];
	DWORD		m_aTableNextTime[TOTAL_TIME];

	int			m_nTime;			///< 현재 시간
	DWORD		m_dwNextTime;		///< 다음 시간까지의 시간
	DWORD		m_dwTime;			///< 진행 시간

	float		m_fFogStart[2];
	float		m_fFogEnd[2];


	SColor		m_FogColor[2];

	Vector3		m_vDir[2];

	SColor		m_Diffuse[2];
	SColor		m_Ambient[2];
	SColor		m_DiffuseNext[2];
	SColor		m_AmbientNext[2];

	Vector3		m_vDirNext[2];
	
	IW3DLight*	m_pLight[2];

	IW3DWorld*	m_pWorld;

	BOOL		m_bInterpolate;
}; //class CTimeMgr




#endif //#ifndef __Time_Manager_Header__
