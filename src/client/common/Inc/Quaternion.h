/**
 * 사원수 모듈 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-04-25 7:22p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /Common/Inc/Quaternion.h $
 * 
 * 7     03-04-25 7:22p Ahastudio
 *
 * 5     03-03-14 5:10p Ahastudio		행렬과 곱하는 함수 추가.
 * <HR>
 *
 * @file	Quaternion.h
 */


#ifndef __Wind3D_Common_Quaternion_Header__
#define __Wind3D_Common_Quaternion_Header__


/**
 * 사원수.
 * @ingroup		Common
 */
struct COMMON_API Quaternion
{
	float	x, y, z, w;

	/**
	 * 생성자.
	 */
	Quaternion()
	{
	} //Quaternion

	/**
	 * 생성자.
	 */
	Quaternion( const float fX, const float fY, const float fZ, const float fW )
	{
		x = fX;
		y = fY;
		z = fZ;
		w = fW;
	} //Quaternion

	Quaternion( const Vector3& vAxis, const float fAngle );

	/**
	 * 사원수와 곱하기.
	 */
	Quaternion		operator * ( const Quaternion& q )
	{
		return Quaternion(
			 x * q.w + y * q.z - z * q.y + w * q.x,
			-x * q.z + y * q.w + z * q.x + w * q.y,
			 x * q.y - y * q.x + z * q.w + w * q.z,
			-x * q.x - y * q.y - z * q.z + w * q.w );
	} //operator *

	/**
	 * 행렬과 곱한다.
	 */
	Quaternion		operator * ( const Matrix4& m )
	{
		return *this * m.GetQuaternion();
	} //operator *

	/**
	 * 사원수와 곱한다.
	 */
	Quaternion&		operator *= ( const Quaternion& q )
	{
		*this = *this * q;
		return *this;
	} //operator *=

	const Quaternion&	Interpolate( const Quaternion& qNext, const float fTime ) const;

	const Matrix4&		GetMatrix4() const;

	Vector3				GetDirection() const;

	void	SetRotationX( const float fAngle );
	void	SetRotationY( const float fAngle );
	void	SetRotationZ( const float fAngle );

	void	SetOrientation( const Vector3& vDir, const Vector3& vUp );
}; //struct COMMON_API Quaternion


#endif // #ifndef __Wind3D_Common_Quaternion_Header__
