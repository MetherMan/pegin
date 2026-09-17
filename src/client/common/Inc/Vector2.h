/**
 * 2차원 벡터.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-22 10:30p $
 * @version	$Revision: 1 $
 *
 * <HR>
 * $Log: /Common/Inc/Vector2.h $
 * 
 * 1     03-12-22 10:30p Kjmgo
 * 
 * <HR>
 *
 * @file	Vector2.h
 */

#ifndef __Wind3D_Common_Vector2_Header__
#define __Wind3D_Common_Vector2_Header__

/**
 *
 */
struct COMMON_API Vector2
{
	float	x, y;

	/**
	 * 생성자.
	 */
	Vector2()
	{
	} //Vector2

	/**
	 * 좌표를 입력하는 생성자.
	 *
	 * @param	fX		[IN] X 좌표
	 * @param	fY		[IN] Y 좌표
	 */
	Vector2( float fX, float fY )
	{
		x = fX;
		y = fY;
	} //Vector2

	/**
	 * 양의 부호.
	 */
	Vector2		operator + () const
	{
		return Vector2( x, y );
	} //operator +

	/**
	 * 음의 부호.
	 */
	Vector2		operator - () const
	{
		return Vector2( -x, -y );
	} //operator -

	/**
	 * 벡터를 더한다.
	 */
	Vector2		operator + ( const Vector2& v ) const
	{
		return Vector2( x + v.x, y + v.y );
	} //operator +

	/**
	 * 벡터를 뺀다.
	 */
	Vector2		operator - ( const Vector2& v ) const
	{
		return Vector2( x - v.x, y - v.y );
	} //operator -

	/**
	 * 실수를 곱한다.
	 */
	Vector2		operator * ( const float s ) const
	{
		return Vector2( x * s, y * s );
	} //operator *

	/**
	 * 실수로 나눈다.
	 */
	Vector2		operator / ( const float s ) const
	{
		return Vector2( x / s, y / s );
	} //operator /

	/**
	 * 벡터를 더한다.
	 */
	Vector2&	operator += ( const Vector2& v )
	{
		x += v.x;
		y += v.y;
		return *this;
	} //operator +=

	/**
	 * 벡터를 뺀다.
	 */
	Vector2&	operator -= ( const Vector2& v )
	{
		x -= v.x;
		y -= v.y;
		return *this;
	} //operator -=

	/**
	 * 실수를 곱한다.
	 */
	Vector2&	operator *= ( const float s )
	{
		x *= s;
		y *= s;
		return *this;
	} //operator *=

	/**
	 * 실수로 나눈다.
	 */
	Vector2&	operator /= ( const float s )
	{
		x /= s;
		y /= s;
		return *this;
	} //operator /=

	/**
	 * 비교 연산자.
	 */
	BOOL	operator == ( const Vector2& v ) const
	{
		return ( x == v.x ) && ( y == v.y ) ;
	} //operator ==

	/**
	 * 비교 연산자.
	 */
	BOOL	operator != ( const Vector2& v ) const
	{
		return ( x != v.x ) || ( y != v.y );
	} //operator !=

	/**
	 *
	 */
	float	Length() const
	{
		return sqrtf( x * x + y * y );
	} //Length

	/**
	 * 단위 벡터를 얻는다.
	 */
	Vector2		Normal() const
	{
		return *this / Length();
	} //Normal

	/**
	 *
	 */
	float	DotProduct( const Vector2& v ) const
	{
		return x * v.x + y * v.y;
	} //DotProduct

	/**
	 *
	 */
	void	Normalize()
	{
		*this /= Length();
	} //Normalize
}; //struct COMMON_API Vector2

#endif //#ifndef __Wind3D_Common_Vector2_Header__