/**
 * 수학 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-03 2:32p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Common/Inc/Math.h $
 * 
 * 9     03-12-03 2:32p Kjmgo
 * 
 * 8     03-12-03 2:28p Kjmgo
 * DWORD, int 보간 추가
 * 
 * 7     03-10-17 1:58p Kjmgo
 * 보간 함수 추가
 * 
 * 6     03-04-25 7:21p Ahastudio
 * <HR>
 *
 * @file	Math.h
 */


#ifndef __Wind3D_Common_Math_Header__
#define __Wind3D_Common_Math_Header__


#include <math.h>
#include <time.h>


#define random( x )		(int)( ( (long)rand() * (x) ) / ( RAND_MAX + 1 ) )
#define randomize()		srand( (unsigned)time( NULL ) )


const float MATH_PI = 3.141592F;
const float MATH_EPSILON = 1.0e-5F;


#define ABS( x )	( (x) >= 0 ? (x) : -(x) )

#define MAX( a, b )	( (a) > (b) ? (a) : (b) )
#define MIN( a, b )	( (a) < (b) ? (a) : (b) )

#define ANGLE( x )	( (x) * MATH_PI / 180.0F )
#define DEGREE( x ) ( (x) / 180 * MATH_PI );

/**
 * float 보간 
 */
inline float Interpolate( float s, float e, float f )
{
	return s + ( f * ( e - s ) );
} //Interpolate

/**
 *	DWORD 보간
 */
inline DWORD Interpolate( DWORD s, DWORD e, float f )
{
	return (DWORD)( s + (DWORD)( f * ( e - s ) ) );
} //Interpolate

/**
 *	int 보간
 */
inline int Interpolate( int s, int e, float f )
{
	return (int)( s + (int)( f * ( e - s ) ) );
} //Interpolate

#endif // #ifndef __Wind3D_Common_Math_Header__
