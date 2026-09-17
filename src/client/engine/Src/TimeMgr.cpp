/**
 * 시간 관련
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 05-02-19 7:11p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /Engine/Src/TimeMgr.cpp $
 * 
 * 11    05-02-19 7:11p Sk8snow2
 * 
 * 10    04-12-09 6:28p Sk8snow2
 * 
 * 9     04-05-19 1:38p Paran2
 * 
 * 8     04-01-07 7:15a Kjmgo
 * 
 * 7     03-12-17 11:21a Kjmgo
 * 맵마다 시간 적용
 * 
 * 6     03-12-11 4:10a Kjmgo
 * 시간 적용( NextTime을 서버에서 얻어옴 )
 * 
 * 5     03-12-04 10:26a Kjmgo
 * 
 * 4     03-12-03 8:03p Kjmgo
 * 버그 수정
 * 
 * 3     03-12-03 5:46p Kjmgo
 * 시간을 Time으로 수정
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
 * @file	TimeMgr.cpp
 */

#include "Engine_Export.h"
#include "TimeMgr.h"

/**
 *
 */
CTimeMgr::CTimeMgr()
{
	m_bInterpolate	= FALSE;
	m_nTime			= 0;
	m_dwNextTime	= 0;
	m_dwTime		= 0;

	for( int i = 0 ; i < 2 ; i++ )
	{
		m_vDir[i]		= Vector3( 0.0F, 0.0F, 0.0F );
		m_vDirNext[i]	= Vector3( 0.0F, 0.0F, 0.0F );
		m_pLight[i]		= NULL;
	} //for

	for( int j = 0 ; j < TOTAL_TIME ; j++ )
	{
		m_aTable[j]			= j;			// 
		m_aTableBool[j]		= TRUE;			// 다음단계까지 보간한다.
		m_aTableNextTime[j] = 1000;		// 1 초 마다 갱신한다.
	} //for

} //CTimeMgr::CTimeMgr

/**
 *
 */
CTimeMgr::~CTimeMgr()
{

} //CTimeMgr::~CTimeMgr

/**
 *
 */
void CTimeMgr::Init( IW3DWorld* pWorld )
{
	Assert( pWorld );

	m_pWorld = pWorld;
	m_pLight[0] = pWorld->GetLight( 0 ); 
	m_pLight[1] = pWorld->GetLight( 1 ); 

	Assert( m_pLight[0] );
	Assert( m_pLight[1] );
} //CTimeMgr::Init

/**
 *
 */
BOOL CTimeMgr::LoadTable( const char* szFile, int nId )
{
	Assert( szFile );

	char szPath[128];
	sprintf( szPath, "Light/%s", szFile );

	FILE* fp = fopen( szPath, "rt" );

	if( NULL == fp )
	{
		return FALSE;
	} //if

	for( int j = 0 ; j < TOTAL_TIME ; j++ )
	{
		m_aTable[j]			= j;			// 
		m_aTableBool[j]		= TRUE;			// 다음단계까지 보간한다.
		m_aTableNextTime[j] = 1000;		// 1 초 마다 갱신한다.
	} //for

	char szBuffer[256];
	char szKeyword[80];

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
			int nMap = -1;
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

				if( !strcmp( szKeyword, "[/MAP]" ) )
				{
					break;
				} //if
				
				if( !strcmp( szKeyword, "[ALL]" ) )
				{
					int n = 0;
					if( sscanf( szBuffer, "%s %d", szKeyword, &n ) != 2 )
					{
						Assert( !"sscanf 오류" );
					}

					for( int j = 0 ; j < TOTAL_TIME ; j++ )
					{
						m_aTable[j]			= n;		// 
						m_aTableBool[j]		= FALSE;	// 다음단계까지 보간한다.
						m_aTableNextTime[j] = 1000;		// 1 초 마다 갱신한다.
					} //for
					continue;
				} //if

				int nId = 0;
				int nTime = 0;
				int nInterpolate;
				DWORD dwNextTime;
				
				if( sscanf( szBuffer, "%d %d %d %d %d", &nId, &nTime, &nInterpolate, &dwNextTime ) != 5 )
				{
					Assert( !"sscanf 오류" );
				}


				Assert( nId >= 0 && nId < TOTAL_TIME );

				m_aTable[nId] = nTime;
				m_aTableBool[nId] = nInterpolate;
				m_aTableNextTime[nId] = dwNextTime;
			} //while
		}
	} //while
	
	fclose( fp );
	return TRUE;
} //CTimeMgr::LoadTable

/**
 *
 */
BOOL CTimeMgr::LoadLight( const char* szFile )
{
///////////////////////////////////////////////////////////
#define READ_RGB( s )		\
			fscanf( fp, "%d %d %d", &r, &g, &b );	\
			(s).SetColor( r, g, b );
///////////////////////////////////////////////////////////

	Assert( szFile );

	char szPath[256];
	sprintf( szPath, "Light/%s", szFile );

	FILE*	fp = fopen( szPath, "rt" );
	if( NULL == fp )
	{
		Assert( !"파일 열기 실패" );
		return FALSE;
	} //if

	m_pLight[0] = m_pWorld->GetLight( 0 ); 
	m_pLight[1] = m_pWorld->GetLight( 1 ); 

	int r,g,b;

	READ_RGB( m_FogColor[0] );

	fscanf( fp, "%f", &m_fFogStart[0] );
	fscanf( fp, "%f", &m_fFogEnd[0] );
	
	m_pWorld->SetFogState( m_FogColor[0].GetColor(), m_fFogStart[0], m_fFogEnd[0] );

	fscanf( fp, "%f %f %f", &m_vDir[0].x, &m_vDir[0].y, &m_vDir[0].z );

	READ_RGB( m_Diffuse[0] );
	READ_RGB( m_Ambient[0] );
	
	if( m_pLight[0] )
	{
		m_pLight[0]->SetDirectional();
		m_pLight[0]->SetDiffuse( m_Diffuse[0].GetColor() );
		m_pLight[0]->SetAmbient( m_Ambient[0].GetColor() );
		m_pLight[0]->SetSpecular( 0xFFFFFF );
		m_pLight[0]->SetDirection( m_vDir[0] );
	} //if

	fscanf( fp, "%f %f %f", &m_vDir[1].x, &m_vDir[1].y, &m_vDir[1].z );

	READ_RGB( m_Diffuse[1] );
	READ_RGB( m_Ambient[1] );

	if( m_pLight[1] )
	{
		m_pLight[1]->SetDirectional();
		m_pLight[1]->SetDiffuse( m_Diffuse[1].GetColor() );
		m_pLight[1]->SetAmbient( m_Ambient[1].GetColor() );
		m_pLight[1]->SetDirection( m_vDir[1] );
	} //if

	fclose( fp );

#undef READ_RGB

	return TRUE;
} //CTimeMgr::LoadLight

/**
 *
 */
BOOL CTimeMgr::LoadLightNext( const char* szFile )
{

///////////////////////////////////////////////////////////
#define READ_RGB( s )		\
			fscanf( fp, "%d %d %d", &r, &g, &b );	\
			(s).SetColor( r, g, b );
///////////////////////////////////////////////////////////

	Assert( szFile );

	char szPath[256];
	sprintf( szPath, "Light/%s", szFile );

	FILE*	fp = fopen( szPath, "rt" );
	if( NULL == fp )
	{
		Assert( !"파일 열기 실패" );
		return FALSE;
	} //if
	int		r, g, b;
	
	fscanf( fp, "%d %d %d", &r, &g, &b );

	m_FogColor[1].SetColor( r, g, b );
	
	fscanf( fp, "%f", &m_fFogStart[1] );
	fscanf( fp, "%f", &m_fFogEnd[1] );
	fscanf( fp, "%f %f %f", &m_vDirNext[0].x, &m_vDirNext[0].y, &m_vDirNext[0].z );

	READ_RGB( m_DiffuseNext[0] );
	READ_RGB( m_AmbientNext[0] );

	fscanf( fp, "%f %f %f", &m_vDirNext[1].x, &m_vDirNext[1].y, &m_vDirNext[1].z );

	READ_RGB( m_DiffuseNext[1] );
	READ_RGB( m_AmbientNext[1] );

	fclose( fp );

#undef READ_RGB

	return TRUE;
} //CTimeMgr::LoadLightNext

/**
 *
 */
void CTimeMgr::Update( const DWORD dwTick )
{
/*	if( !m_bInterpolate )
	{
		return ;
	} //if*/

	static DWORD s_dwTick = 0;

	s_dwTick += dwTick;
	m_dwTime += dwTick;

	// 지정된 시간이 지나면, 
	if( m_aTableNextTime[m_nTime] <= s_dwTick && m_dwTime <= m_dwNextTime )
	{
		float f = ( m_dwTime / (float)m_dwNextTime );

		float		fFogStart;
		float		fFogEnd;
		
		DWORD		dwFogColor;
		Vector3		vDir[2];
		DWORD		dwDiffuse[2];
		DWORD		dwAmbient[2];

		//
		// Fog 거리를 보간한다.
		//
		fFogStart	= Interpolate( m_fFogStart[0], m_fFogStart[1], f );
		fFogEnd		= Interpolate( m_fFogEnd[0], m_fFogEnd[1], f );

		//
		// Fog 색을 보간한다.
		//
		dwFogColor	 = m_FogColor[0].InterpolateToColor( m_FogColor[1], f );

		//
		// Diffuse 칼라를 보간한다.
		//
		dwDiffuse[0] = m_Diffuse[0].InterpolateToColor( m_DiffuseNext[0], f );
		dwDiffuse[1] = m_Diffuse[1].InterpolateToColor( m_DiffuseNext[1], f );

		//
		// Ambient 를 보간한다.
		//
		dwAmbient[0] = m_Ambient[0].InterpolateToColor( m_AmbientNext[0], f );
		dwAmbient[1] = m_Ambient[1].InterpolateToColor( m_AmbientNext[1], f );

		//
		// 빛의 방향을 보간한다.
		//
		vDir[0]		= Interpolation( m_vDir[0], m_vDirNext[0], f );
		vDir[1]		= Interpolation( m_vDir[1], m_vDirNext[1], f );

		//
		// Fog를 적용한다.
		//
		m_pWorld->SetFogState( dwFogColor, fFogStart, fFogEnd );

		//
		// 0번 Light 에 적용시킨다.
		//
		if( m_pLight[0] )
		{
			m_pLight[0]->SetDirectional();
			m_pLight[0]->SetDiffuse( dwDiffuse[0] );
			m_pLight[0]->SetAmbient( dwAmbient[0] );
			m_pLight[0]->SetDirection( vDir[0] );
		} //if

		//
		// 1번 Light 에 적용시킨다.
		//
		if( m_pLight[1] )
		{
			m_pLight[1]->SetDirectional();
			m_pLight[1]->SetDiffuse( dwDiffuse[1] );
			m_pLight[1]->SetAmbient( dwAmbient[1] );
			m_pLight[1]->SetDirection( vDir[1] );
		} //if

		s_dwTick = 0;
	} //if
} //CTimeMgr::Update

/**
 *
 */
void CTimeMgr::SetTime( int nTime, int nNextTime, DWORD dwNext )
{	
	Assert( nTime >= 0 && nTime < 24 );

	char filename[32];
	sprintf( filename, "time%02d.txt", m_aTable[nTime] );
	LoadLight( filename );

	sprintf( filename, "time%02d.txt", m_aTable[nNextTime] );
	LoadLightNext( filename );

	m_nTime = nTime;
	m_dwNextTime = dwNext;
	m_dwTime = 0;

	m_bInterpolate = TRUE;

} //CTimeMgr::SetTime

