/**
 * AABB 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-06-08 3:57p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Common/Src/AABB.cpp $
 * 
 * 3     04-06-08 3:57p Paran2
 * 
 * 2     03-10-02 2:42p Kjmgo
 * AABB
 * 
 * 1     03-10-02 4:22a Kjmgo
 * 
 * <HR>
 *
 * @file	AABB.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"

/**
 *
 */
void AABB::SetPlane( int n, Plane plane )
{
	Assert( n > 0 && n < 6 );
	aPlane[n] = plane;
} //AABB::SetPlane

/**
 *
 */
void AABB::MakeAABB( const Vector3& vMin, const Vector3& vMax )
{
	vP[0] = Vector3( vMin.x, vMin.y, vMax.z );
	vP[1] = Vector3( vMax.x, vMin.y, vMax.z );
	vP[2] = Vector3( vMin.x, vMin.y, vMin.z );
	vP[3] = Vector3( vMax.x, vMin.y, vMin.z );
	vP[4] = Vector3( vMin.x, vMax.y, vMax.z );
	vP[5] = Vector3( vMax.x, vMax.y, vMax.z );
	vP[6] = Vector3( vMin.x, vMax.y, vMin.z );
	vP[7] = Vector3( vMax.x, vMax.y, vMin.z );
} //AABB::MakeAABB

/**
 *	AABB와 점 충돌
 */
BOOL AABB::CheckPoint( const Matrix4& matWorld, const Vector3& vPos )
{
	aPlane[0] = Plane( vP[2] * matWorld, vP[3] * matWorld, vP[0] * matWorld );
	aPlane[1] = Plane( vP[2] * matWorld, vP[0] * matWorld, vP[6] * matWorld );
	aPlane[2] = Plane( vP[4] * matWorld, vP[5] * matWorld, vP[6] * matWorld );
	aPlane[3] = Plane( vP[1] * matWorld, vP[3] * matWorld, vP[5] * matWorld );
	aPlane[4] = Plane( vP[0] * matWorld, vP[1] * matWorld, vP[4] * matWorld );
	aPlane[5] = Plane( vP[2] * matWorld, vP[6] * matWorld, vP[3] * matWorld );

	for( int i = 0 ; i < 6 ; i++ )
	{
		float fDist = aPlane[i].Distance( vPos );

		if( fDist > 0 )
		{
			return FALSE;
		} //if
	} //for

	return TRUE;
} //AABB::CheckPoint

/**
 * AABB와 스페어 충돌
 */
BOOL AABB::CheckSphere( const Vector3& vPos, float fRadius )
{
	return FALSE;
} //AABB::CheckSphere

/**
 *
 */
Vector3	AABB::GetNearPlane( const Vector3& vPos )
{
	return Vector3( 0.0F, 0.0F, 0.0F );
} //AABB::GetNearPlane

/**
 *  
 */
BOOL AABB::Pick( const Matrix4& matWorld, const Vector3& vOrig, const Vector3& vDir )
{
	static Vector3			s_avEdge[8];

	int			nCnt;
	for( nCnt = 0; nCnt < 8; nCnt++ )
	{
		s_avEdge[nCnt] = vP[nCnt] * matWorld;
	} //for

	static Triangle			s_aTriangle[12];

	s_aTriangle[0].a = s_avEdge[2];		s_aTriangle[0].b = s_avEdge[1];		s_aTriangle[0].c = s_avEdge[0];
	s_aTriangle[1].a = s_avEdge[1];		s_aTriangle[1].b = s_avEdge[2];		s_aTriangle[1].c = s_avEdge[3];

	s_aTriangle[2].a = s_avEdge[4];		s_aTriangle[2].b = s_avEdge[5];		s_aTriangle[2].c = s_avEdge[6];
	s_aTriangle[3].a = s_avEdge[7];		s_aTriangle[3].b = s_avEdge[6];		s_aTriangle[3].c = s_avEdge[5];

	s_aTriangle[4].a = s_avEdge[0];		s_aTriangle[4].b = s_avEdge[4];		s_aTriangle[4].c = s_avEdge[6];
	s_aTriangle[5].a = s_avEdge[2];		s_aTriangle[5].b = s_avEdge[0];		s_aTriangle[5].c = s_avEdge[6];

	s_aTriangle[6].a = s_avEdge[7];		s_aTriangle[6].b = s_avEdge[5];		s_aTriangle[6].c = s_avEdge[1];
	s_aTriangle[7].a = s_avEdge[7];		s_aTriangle[7].b = s_avEdge[1];		s_aTriangle[7].c = s_avEdge[3];

	s_aTriangle[8].a = s_avEdge[5];		s_aTriangle[8].b = s_avEdge[4];		s_aTriangle[8].c = s_avEdge[0];
	s_aTriangle[9].a = s_avEdge[5];		s_aTriangle[9].b = s_avEdge[0];		s_aTriangle[9].c = s_avEdge[1];
	
	s_aTriangle[10].a = s_avEdge[2];	s_aTriangle[10].b = s_avEdge[6];	s_aTriangle[10].c = s_avEdge[7];
	s_aTriangle[11].a = s_avEdge[3];	s_aTriangle[11].b = s_avEdge[2];	s_aTriangle[11].c = s_avEdge[7];

	for( nCnt = 0; nCnt < 12; nCnt++ )
	{
		if( s_aTriangle[nCnt].Intersect( vOrig, vDir ) )
		{
			return TRUE;
		} //if
	} //for

	return FALSE;
} //AABB::Pick