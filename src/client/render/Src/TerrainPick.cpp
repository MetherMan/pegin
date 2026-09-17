/**
 * 지형 피킹 구현부
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-09-18 8:33a $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Render/Src/TerrainPick.cpp $
 * 
 * 6     04-09-18 8:33a Sk8snow2
 * 피킹관련 수정( 정만 )
 * 
 * 5     04-01-10 12:29p Kjmgo
 * 
 * 4     04-01-08 8:50a Kjmgo
 * 코드 정리
 * 
 * 3     03-12-10 11:01p Kjmgo
 * 피킹 1차 해결
 * 
 * 2     03-12-10 12:19a Kjmgo
 * Terrain 에서 Picking 하는 것 분리
 * 
 * 1     03-12-09 10:07p Kjmgo
 * 
 * <HR>
 *
 * @file	TerrainPick.cpp
 */

#include "W3DTerrain.h"

/**
 * 지형 피킹.
 *
 * @return	피킹 성공 여부. ( 에러를 뱉는 건 아니다 )
 */
BOOL CW3DTerrain::Pick(
	const float		fHeight,	///< [IN] 바닥 평면 높이
	const Vector3&	vOrig,		///< [IN] 피킹용 반직선 시작점
	const Vector3&	vDir,		///< [IN] 피킹용 반직선 방향
	Vector3*		pvPick		///< [OUT] 피킹 좌표
	)
{
	#define ATTRIBUTE_MAP( x, y )	m_pbAttribute[m_nLevel][( (y) % m_nSize ) * m_nSize + ( (x) % m_nSize )]

	Assert( pvPick );

	//-------------------------------------
	//	Static Variable
	//-------------------------------------
	static float s_fFarPlane = 0;
	static float s_fObjectHeight = 0;
	static float fDistance;

	m_vDir = vDir;
	m_vOrig = vOrig;

	// Set Static Variable.
	// Object Height Initialize ( Zero. )
	s_fObjectHeight = 0;

	// Far Plane Value Setting. - From Far Plane Value Of the Frustum 
	s_fFarPlane = g_Frustum.m_fFarPlane;

	// Far Plane Valuse Of The Frustum Setting, 200.0F
	g_Frustum.m_fFarPlane = 200.0F;

	//	Varialbe Initialize
	m_vPick = Vector3( 0.0F, 0.0F, 0.0F );
	m_fPickDistance = 10000.0F;

	BOOL		bPick			= FALSE;
	float		fPickDistance	= 10000.0F;

	m_bUseLevel = FALSE;

	//-------------------------------------
	//	Calculate Plane.
	//-------------------------------------
	Plane		plane = Plane( Vector3( 0.0F, 0.0F, 1.0F ), -fHeight );

	//-------------------------------------
	// Calculate Picking Point From Plane
	//-------------------------------------

	Vector3		vPick = plane.Pick( vOrig, vDir );
	Vector3		vPickDummy = vPick;

	//-------------------------------------
	//	Calculate Start Point And End Point
	//-------------------------------------
	int nPickX		= (int)( vPick.x / TILE_SIZE );
	int nPickY		= (int)( vPick.y / TILE_SIZE );

	int nStartX		= MAX( 0, nPickX - 10 );
	int nStartY		= MAX( 0, nPickY - 10 );
	int nEndX		= MIN( m_nSize - 1, nPickX + 10 );
	int nEndY		= MIN( m_nSize - 1, nPickY + 10 );
		
	Vector3 v[4];

	for( int nY = nStartY ; nY <= nEndY ; nY++ )
	{
		for( int nX = nStartX ; nX <= nEndX ; nX++ )
		{
			// Get Attrubute 
			BYTE btAttribute = ATTRIBUTE_MAP( nX, nY );

			if( btAttribute )
			{
				// Is Attribute
				if( !bPick )
					bPick = PickHasAttribute( nX, nY, v, btAttribute );
				else
					PickHasAttribute( nX, nY, v, btAttribute );
			}
			else
			{
				// Is not Attribute
				if( HasAreaObjectHeight( nX, nY ) )
				{
					// Has ObjectHeight
					if( !bPick )
						bPick = PickHasObjectHeight( nX, nY, v );
					else
						PickHasObjectHeight( nX, nY, v );
				}
				else
				{
					if( m_nLevel != 0 )
					{
						continue;
					} //if

					if( !bPick )
						bPick = PickHasNone( nX, nY, v );
					else
						PickHasNone( nX, nY, v );
				} //if..else..
			} //if..else..
		} //for

	} //for

	// Restore Far Plane.
	// And Use Level.
	g_Frustum.m_fFarPlane = s_fFarPlane;
	m_bUseLevel = TRUE;

	// If Has Picking Point, 
	if( bPick )
	{	// Memory Copy.
		Assert( m_vPick.x >= 0 && m_vPick.y >= 0 );
		*pvPick = m_vPick;
	} //if

	#undef ATTRIBUTE_MAP

	return bPick;
} //CW3DTerrain::Pick

/**
 *
 */
BOOL CW3DTerrain::HasAreaObjectHeight( int nX, int nY )
{
	int nStartX = nX * TILE_SIZE;
	int nStartY = nY * TILE_SIZE;
	
	for( int i = nStartY ; i < nStartY + TILE_SIZE ; i++ )
	{
		for( int j = nStartX ; j < nStartX + TILE_SIZE ; j++ )
		{
			if( GetObjectHeight( j, i ) )
			{
				return TRUE;
			} //if
		} //for
	} //for

	return FALSE;
} //CW3DTerrain::HasAreaObjectHeight

/**
 *
 */
BOOL CW3DTerrain::PickHasObjectHeight( int nX, int nY, Vector3* pv )
{
	int nCount = 0;

	nX = nX * TILE_SIZE;
	nY = nY * TILE_SIZE;

	for( int i = 0 ; i < TILE_SIZE ; i++ )
	{
		for( int j = 0 ; j < TILE_SIZE ; j++ )
		{
			int nStartX = nX + j;
			int nStartY = nY + i; 

			nCount = 0;

			for( int k = 0 ; k < 2 ; k++ )
			{
				for( int l = 0 ; l < 2 ; l++ )
				{
					pv[nCount].x = (float)( nStartX + l );
					pv[nCount].y = (float)( nStartY + k );
					pv[nCount].z = GetHeight( pv[nCount].x, pv[nCount].y );
					nCount++;
				} //for
			} //for

			if( PickPolygon( pv, ( nStartX + nStartY ) % 2 ) )
			{
				return TRUE;
			} //if
		} //for
	} //for

	return FALSE;
} //CW3DTerrain::PickHasObjectHeight

/**
 *
 */
BOOL CW3DTerrain::PickHasNone( int nX, int nY, Vector3* pv )
{
	int nCount = 0;

	for( int k = 0 ; k < 2 ; k++ )
	{
		for( int l = 0 ; l < 2 ; l++ )
		{
			int x = ( nX + l ) % m_nSize;
			int y = ( nY + k ) % m_nSize;

			pv[nCount].x = (float)( x ) * TILE_SIZE;
			pv[nCount].y = (float)( y ) * TILE_SIZE;
			pv[nCount].z =  m_pfHeight[nY * m_nSize + nX];
			nCount++;
		} //for
	} //for

	return PickPolygon( pv, ( nX + nY ) % 2 );
} //CW3DTerrain::PickHasNone

/**
 *
 */
BOOL CW3DTerrain::PickHasAttribute( int nX, int nY, Vector3* pv, BYTE btType )
{
	float fAddHeight = 0.0F;

	switch( btType )
	{
	case 1:
		fAddHeight = GetHighHeight( nX, nY );
		break;
	case 2:
		fAddHeight = GetLowHeight( nX, nY );
		break;
	} //switch

	int nCount = 0;

	for( int k = 0 ; k < 2 ; k++ )
	{
		for( int l = 0 ; l < 2 ; l++ )
		{
			int x = ( nX + l ) % m_nSize;
			int y = ( nY + k ) % m_nSize;

			pv[nCount].x = (float)( x ) * TILE_SIZE;
			pv[nCount].y = (float)( y ) * TILE_SIZE;
			pv[nCount].z = fAddHeight;
			nCount++;
		} //for
	} //for

	return PickPolygon( pv, ( nX + nY ) % 2 );
} //CW3DTerrain::PickHasAttribute

/**
 *
 */
BOOL CW3DTerrain::PickPolygon( Vector3* pv, int nType )
{
	int nViewVertex = 0;
	for( int nVertex = 0 ; nVertex < 4 ; nVertex++ )
	{
		if( g_Frustum.CheckPoint( pv[nVertex] ) )
		{
			nViewVertex++;
			if( nViewVertex > 1 )
			{
				break;
			} //if

		} //if
	} //for

	if( nViewVertex <= 0 )
	{
		return FALSE;
	} //if

	Triangle tri;
	BOOL bPick = FALSE;

	if( nType == 0 )
		tri = Triangle( pv[0], pv[1], pv[2] );
	else
		tri = Triangle( pv[0], pv[1], pv[3] );
	//tri = Triangle( v[0], v[1], v[2] );

	float fDistance;
	float fU, fV;

	if( tri.Intersect( m_vOrig, m_vDir, &fDistance, &fU, &fV ) )
	{
		if( fDistance >= 0.0F &&
			fDistance < m_fPickDistance 
			)
		{
			m_vPick = m_vOrig + m_vDir * fDistance;
			//vPick = v[0] + ( ( v[1] - v[0] ) * fU ) + ( ( v[2] - v[0] ) * fV );//vOrig + vDir * fDistance;
			m_fPickDistance = fDistance;
			bPick = TRUE;
		} //if
	} //if


	if( nType == 0 )
		tri = Triangle( pv[2], pv[1], pv[3] );
	else
		tri = Triangle( pv[0], pv[3], pv[2] );
		
	//tri = Triangle( v[0], v[3], v[2] );
	if( tri.Intersect( m_vOrig, m_vDir, &fDistance, &fU, &fV ) )
	{
		if( fDistance >= 0.0F && 
			fDistance < m_fPickDistance 
			)
		{
			m_vPick = m_vOrig + m_vDir * fDistance;
			//vPick = v[1] + ( ( v[3] - v[1] ) * fU ) + ( ( v[2] - v[1] ) * fV );//vOrig + vDir * fDistance;
			m_fPickDistance = fDistance;
			bPick = TRUE;
		} //if
	} //if

	return bPick;
} //CW3DTerrain::PickPolygon