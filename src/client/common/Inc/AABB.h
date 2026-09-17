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
 * $Log: /Common/Inc/AABB.h $
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
 * @file	AABB.h
 */

#ifndef __AABB_Header__
#define __AABB_Header__

/**
 *
 */
struct COMMON_API	AABB
{
	Plane		aPlane[6];			///< 바운드 박스
	Vector3		vP[8];				///< 바운드 박스 꼭지점

	void		MakeAABB( const Vector3& vMin, const Vector3& vMax );

	/**
	 *
	 */
	void		SetPlane( int n, Plane plane );

	/**
	 *	AABB와 점 충돌
	 */
	BOOL		CheckPoint( const Matrix4& matWorld, const Vector3& vPos );

	/**
	 * AABB와 스페어 충돌
	 */
	BOOL		CheckSphere( const Vector3& vPos, float fRadius );

	/**
	 *
	 */
	Vector3		GetNearPlane( const Vector3& vPos );

	/**
	 *  
	 */
	BOOL		Pick( const Matrix4& matWorld, const Vector3& vOrig, const Vector3& vDir );
	
}; //struct COMMON_API	AABB


#endif //#ifndef __AABB_Header__
