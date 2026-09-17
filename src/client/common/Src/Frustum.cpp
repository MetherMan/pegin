/**
 * 시야 절두체.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-08-04 11:14a $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /Common/Src/Frustum.cpp $
 * 
 * 10    03-08-04 11:14a Admin
 * 
 * 9     03-04-03 4:28a Ahastudio
 * <HR>
 *
 * @file	Frustum.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"


/**
 * Projection 설정.
 *
 * @param	fFOV		[IN] Field Of View
 * @param	fAspect		[IN] Aspect = height / width
 * @param	fNearPlane	[IN] 근단면
 * @param	fFarPlane	[IN] 원단면
 */
void Frustum::SetProjection( float fFOV, float fAspect, float fNearPlane, float fFarPlane )
{
	Assert( fAspect > MATH_EPSILON );

	// 왼쪽 위
	m_vDir[0].x = -fFarPlane * ( tanf( fFOV * 0.5F ) / fAspect );
	m_vDir[0].y =  fFarPlane *   tanf( fFOV * 0.5F );
	m_vDir[0].z =  fFarPlane;

	// 오른쪽 위
	m_vDir[1].x =  fFarPlane * ( tanf( fFOV * 0.5F ) / fAspect );
	m_vDir[1].y =  fFarPlane *   tanf( fFOV * 0.5F );
	m_vDir[1].z =  fFarPlane;

	// 왼쪽 아래
	m_vDir[2].x = -fFarPlane * ( tanf( fFOV * 0.5F ) / fAspect );
	m_vDir[2].y = -fFarPlane *   tanf( fFOV * 0.5F );
	m_vDir[2].z =  fFarPlane;

	// 오른쪽 아래
	m_vDir[3].x =  fFarPlane * ( tanf( fFOV * 0.5F ) / fAspect );
	m_vDir[3].y = -fFarPlane *   tanf( fFOV * 0.5F );
	m_vDir[3].z =  fFarPlane;

	// Frustum 평면 생성.
	m_Plane[0] = Plane( Vector3( 0.0F, 0.0F, 0.0F ), m_vDir[0], m_vDir[2] );	// 왼쪽
	m_Plane[1] = Plane( Vector3( 0.0F, 0.0F, 0.0F ), m_vDir[3], m_vDir[1] );	// 오른쪽
	m_Plane[2] = Plane( Vector3( 0.0F, 0.0F, 0.0F ), m_vDir[1], m_vDir[0] );	// 위
	m_Plane[3] = Plane( Vector3( 0.0F, 0.0F, 0.0F ), m_vDir[2], m_vDir[3] );	// 아래

	// 앞/뒤
	m_fNearPlane	= fNearPlane;
	m_fFarPlane		= fFarPlane;
} //Frustum::SetProjection

/**
 * 뷰 행렬을 적용시킨다.
 *
 * @param	matView		[IN] View Matrix
 */
void Frustum::SetView( const Matrix4& matView )
{
	m_matView		= matView;
	m_matBillboard	= matView.GetBillboard();
	m_matBillboardY	= matView.GetBillboardX();
	m_matBillboardZ	= matView.GetBillboardZ();
} //Frustum::SetView

/**
 * 점이 절두체 내부에 있는지 검사한다.
 *
 * @param	vPos		[IN] 점 위치
 *
 * @return	Frustum 내부에 있는지 여부.
 */
BOOL Frustum::CheckPoint( const Vector3& vPos )
{
	Vector3		v = vPos * m_matView;

	if( v.z <= m_fNearPlane )
	{	// 앞
		return FALSE;
	} //if

	if( v.z >= m_fFarPlane )
	{	// 뒤
		return FALSE;
	} //if

	if( m_Plane[0].Distance( v ) <= 0 )
	{	// 왼쪽
		return FALSE;
	} //if

	if( m_Plane[1].Distance( v ) <= 0 )
	{	// 오른쪽
		return FALSE;
	} //if

	if( m_Plane[2].Distance( v ) <= 0 )
	{	// 위
		return FALSE;
	} //if

	if( m_Plane[3].Distance( v ) <= 0 )
	{	// 아래
		return FALSE;
	} //if

	return TRUE;
} //Frustum::CheckPoint

/**
 * 구가 절두체 내부에 있는지 검사한다.
 *
 * @param	vPos		[IN] 중심
 * @param	fRadius		[IN] 반지름
 *
 * @return	Frustum 내부에 있는지 여부
 */
BOOL Frustum::CheckSphere( const Vector3& vPos, float fRadius )
{
	Vector3		v = vPos * m_matView;

	if( v.z + fRadius <= m_fNearPlane )
	{	// 앞
		return FALSE;
	} //if

	if( v.z - fRadius >= m_fFarPlane )
	{	// 뒤
		return FALSE;
	} //if

	if( m_Plane[0].Distance( v ) <= -fRadius )
	{	// 왼쪽
		return FALSE;
	} //if

	if( m_Plane[1].Distance( v ) <= -fRadius )
	{	// 오른쪽
		return FALSE;
	} //if

	if( m_Plane[2].Distance( v ) <= -fRadius )
	{	// 위
		return FALSE;
	} //if

	if( m_Plane[3].Distance( v ) <= -fRadius )
	{	// 아래
		return FALSE;
	} //if

	return TRUE;
} //Frustum::CheckSphere

/**
 * 상자가 절두체 내부에 있는지 검사한다.
 *
 * @param	vPos		[IN] 중심
 * @param	fRadius		[IN] 크기
 *
 * @return	Frustum 내부에 있는지 여부
 */
BOOL Frustum::CheckCube( const Vector3& vPos, float fSize )
{
	return TRUE;
} //Frustum::CheckCube
