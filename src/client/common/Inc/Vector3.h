/**
 * 3차원 벡터.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-05-11 11:33a $
 * @version	$Revision: 10 $
 *
 * <HR>
 * $Log: /Common/Inc/Vector3.h $
 * 
 * 10    04-05-11 11:33a Sckyj
 * Vector3 보간함수수정
 * 
 * 9     03-12-23 1:07a Kjmgo
 * 벡터의 길이를 구할때, 제곱근을 구하지 않는 함수 추가
 * 
 * 8     03-12-03 3:49p Kjmgo
 * 벡터 보간 추가
 * 
 * 7     03-04-25 7:22p Ahastudio
 * <HR>
 *
 * @file	Vector3.h
 */


#ifndef __Wind3D_Common_Vector3_Header__
#define __Wind3D_Common_Vector3_Header__

Vector3	Interpolation( Vector3& s, Vector3& e, float f );

/**
 * 3차원 벡터.
 * @ingroup		Common
 */
struct COMMON_API Vector3
{
	float	x, y, z;

	/**
	 * 생성자.
	 */
	Vector3()
	{
	} //Vector3

	/**
	 * 좌표를 입력하는 생성자.
	 *
	 * @param	fX		[IN] X 좌표
	 * @param	fY		[IN] Y 좌표
	 * @param	fZ		[IN] Z 좌표
	 */
	Vector3( float fX, float fY, float fZ )
	{
		x = fX;
		y = fY;
		z = fZ;
	} //Vector3

	/**
	 * 양의 부호.
	 */
	Vector3		operator + () const
	{
		return Vector3( x, y, z );
	} //operator +

	/**
	 * 음의 부호.
	 */
	Vector3		operator - () const
	{
		return Vector3( -x, -y, -z );
	} //operator -

	/**
	 * 벡터를 더한다.
	 */
	Vector3		operator + ( const Vector3& v ) const
	{
		return Vector3( x + v.x, y + v.y, z + v.z );
	} //operator +

	/**
	 * 벡터를 뺀다.
	 */
	Vector3		operator - ( const Vector3& v ) const
	{
		return Vector3( x - v.x, y - v.y, z - v.z );
	} //operator -

	/**
	 * 실수를 곱한다.
	 */
	Vector3		operator * ( const float s ) const
	{
		return Vector3( x * s, y * s, z * s );
	} //operator *

	/**
	 * 실수로 나눈다.
	 */
	Vector3		operator / ( const float s ) const
	{
		return Vector3( x / s, y / s, z / s );
	} //operator /

	Vector3		operator * ( const Matrix4& m ) const;

	/**
	 * 벡터를 더한다.
	 */
	Vector3&	operator += ( const Vector3& v )
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	} //operator +=

	/**
	 * 벡터를 뺀다.
	 */
	Vector3&	operator -= ( const Vector3& v )
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	} //operator -=

	/**
	 * 실수를 곱한다.
	 */
	Vector3&	operator *= ( const float s )
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	} //operator *=

	/**
	 * 실수로 나눈다.
	 */
	Vector3&	operator /= ( const float s )
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	} //operator /=

	/**
	 * 4x4행렬을 곱한다.
	 */
	Vector3&	operator *= ( const Matrix4& m )
	{
		*this = *this * m;
		return *this;
	} //operator *=

	/**
	 * 비교 연산자.
	 */
	BOOL	operator == ( const Vector3& v ) const
	{
		return ( x == v.x ) && ( y == v.y ) && ( z == v.z );
	} //operator ==

	/**
	 * 비교 연산자.
	 */
	BOOL	operator != ( const Vector3& v ) const
	{
		return ( x != v.x ) || ( y != v.y ) || ( z != v.z );
	} //operator !=

	float	Length() const;
	float	LengthNoSqrt() const;

	/**
	 * 단위 벡터를 얻는다.
	 */
	Vector3		Normal() const
	{
		return *this / Length();
	} //Normal

	/**
	 *
	 */
	float	DotProduct( const Vector3& v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	} //DotProduct

	/**
	 *
	 */
	Vector3		CrossProduct( const Vector3& v ) const
	{
		return Vector3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	} //CrossProduct

	/**
	 *
	 */
	void	Normalize()
	{
		*this /= Length();
	} //Normalize

	void	RotateX( float fAngle );
	void	RotateY( float fAngle );
	void	RotateZ( float fAngle );
	void	Rotate( const Vector3& vAxis, float fAngle );

	Quaternion		GetQuaternion();

	Vector3 Interpolate( Vector3& vInter, float fTime ) { return Interpolation( *this, vInter, fTime ); }

}; //struct COMMON_API Vector3

/**
 *
 */
inline Vector3	Interpolation( Vector3& s, Vector3& e, float f )
{
	return Vector3( Interpolate( s.x, e.x, f ),
					Interpolate( s.y, e.y, f ),
					Interpolate( s.z, e.z, f ) );
} //Interpolate



#endif // #ifndef __Wind3D_Common_Vector3_Header__
