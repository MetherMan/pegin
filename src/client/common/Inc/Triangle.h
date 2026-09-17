/**
 * »ï°¢Çü.
 *
 * Copyright (c) 2002-2003 ÆÀ¸ÞÀÌ, All rights reserved.
 *
 * @author	¾Æ¼£
 * @date	$Date: 03-04-25 7:22p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /Common/Inc/Triangle.h $
 * 
 * 6     03-04-25 7:22p Ahastudio
 * <HR>
 *
 * @file	Triangle.h
 */


#ifndef __Wind3D_Common_Triangle_Header__
#define __Wind3D_Common_Triangle_Header__


/**
 * »ï°¢Çü.
 * @ingroup		Common
 */
struct COMMON_API Triangle
{
	Vector3		a, b, c;

	/**
	 * A constructor.
	 */
	Triangle()		{}

	/**
	 * »ý¼ºÀÚ.
	 */
	Triangle( const Vector3& vA, const Vector3& vB, const Vector3& vC )
	{
		a = vA;
		b = vB;
		c = vC;
	} //Triangle

	BOOL	Intersect(
		const Vector3&	vOrig,
		const Vector3&	vDir,
		float*			pfT = NULL,
		float*			pfU = NULL,
		float*			pfV = NULL
		);

	float	Distance( const Vector3& vPos ) const;
}; //struct COMMON_API Triangle


#endif
