/**
 * 평면.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-11-27 5:24p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /Common/Inc/Plane.h $
 * 
 * 7     03-11-27 5:24p Kjmgo
 * GetZ에 대한 Assert 주석 처리
 * 
 * 6     03-04-25 7:21p Ahastudio
 * <HR>
 *
 * @file	Plane.h
 */


#ifndef __Wind3D_Common_Plane_Header__
#define __Wind3D_Common_Plane_Header__


/**
 * 평면.
 * @ingroup		Common
 */
struct COMMON_API Plane
{
	Vector3		N;
	float		D;

	/**
	 * 생성자.
	 */
	Plane()
	{
	} //Plane

	/**
	 * 생성자.
	 */
	Plane( float a, float b, float c, float d )
	{
		N = Vector3( a, b, c ).Normal();
		D = d;
	} //Plane

	/**
	 * 생성자.
	 */
	Plane( const Vector3& vN, float d )
	{
		N = vN.Normal();
		D = d;
	} //Plane

	/**
	 * 생성자.
	 */
	Plane( const Vector3& vA, const Vector3& vB, const Vector3& vC )
	{
		Vector3 v0 = vC - vA;
		Vector3 v1 = vC - vB;

		N = v0.CrossProduct( v1 );
		N.Normalize();

		D = -N.DotProduct( vA );
	} //Plane

	/**
	 * 점과 평면의 거리를 구한다.
	 */
	float	Distance( const Vector3& vP ) const
	{
		return N.DotProduct( vP ) + D;
	} //Distance

	Vector3		Pick( const Vector3& vOrig, const Vector3& vDir ) const;

	/**
	 * 평면 상의 Y, Z 좌표를 이용해서 평면 상의 X 좌표를 구한다.
	 */
	float GetX( float fY, float fZ ) const
	{
		Assert( ABS( N.x ) > MATH_EPSILON );
		return -( N.y * fY + N.z * fZ + D ) / N.x;
	} //GetX

	/**
	 * 평면 상의 X, Z 좌표를 이용해서 평면 상의 Y 좌표를 구한다.
	 */
	float GetY( float fX, float fZ ) const
	{
		Assert( ABS( N.y ) > MATH_EPSILON );
		return -( N.x * fX + N.z * fZ + D ) / N.y;
	} //GetY

	/**
	 * 평면 상의 X, Y 좌표를 이용해서 평면 상의 Z 좌표를 구한다.
	 */
	float GetZ( float fX, float fY ) const
	{
		//Assert( ABS( N.z ) > MATH_EPSILON );
		return -( N.x * fX + N.y * fY + D ) / N.z;
	} //GetZ
}; //struct COMMON_API Plane


#endif // #ifndef __Wind3D_Common_Plane_Header__
