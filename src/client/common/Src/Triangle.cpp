/**
 * 삼각형.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-04-25 7:22p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /Common/Src/Triangle.cpp $
 * 
 * 4     03-04-25 7:22p Ahastudio
 * <HR>
 *
 * @file	Triangle.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"


/**
 *
 */
BOOL Triangle::Intersect(
	const Vector3&	vOrig,		///< [IN]
	const Vector3&	vDir,		///< [IN]
	float*			pfT,		///< [OUT]
	float*			pfU,		///< [OUT]
	float*			pfV			///< [OUT]
	)
{
	float u, v;

	// Find vectors for two edges sharing vert0

	Vector3		vEdge1 = b - a;
	Vector3		vEdge2 = c - a;

	// Begin calculating determinant - also used to calculate U parameter

	Vector3		vP = vDir.CrossProduct( vEdge2 );

	// If determinant is near zero, ray lies in plane of triangle

	float fDet = vEdge1.DotProduct( vP );
	if( fDet < 0.0001F )
	{
		return FALSE;
	} //if

	// Calculate distance from vert0 to ray origin

	Vector3		vT = vOrig - a;

	// Calculate U parameter and test bounds

	u = vT.DotProduct( vP );
	if( u < 0.0F || u > fDet )
	{
		return FALSE;
	} //if

	// Prepare to test V parameter

	Vector3 vQ;
	vQ = vT.CrossProduct( vEdge1 );

	// Calculate V parameter and test bounds

	v = vDir.DotProduct( vQ );

	if( v < 0.0F || u + v > fDet )
	{
		return FALSE;
	} //if

	// Calculate t, scale parameters, ray intersects triangle

	float fInvDet = 1.0F / fDet;

	if( pfT )	*pfT = vEdge2.DotProduct( vQ ) * fInvDet;
	if( pfU )	*pfU = u * fInvDet;
	if( pfV )	*pfV = v * fInvDet;

	return TRUE;
} //Triangle::Intersect

/**
 * 점과 삼각형의 거리를 구한다.
 * 그냥 갔다 붙인 함수......
 * 반드시 수정하자 -_-;;;;;;
 *
 * @param	vPos		[IN] 점의 위치
 *
 * @return	거리
 */
float Triangle::Distance( const Vector3& vPos ) const
{
	Vector3		vEdge1 = b - a;
	Vector3		vEdge2 = c - a;

	Vector3		vDiff = a - vPos;

	float	fA00 = vEdge1.Length() * vEdge1.Length();
	float	fA01 = vEdge1.DotProduct( vEdge2 );
	float	fA11 = vEdge2.Length() * vEdge2.Length();

	float	fB0  = vDiff.DotProduct( vEdge1 );
	float	fB1  = vDiff.DotProduct( vEdge2 );

	float	fC   = vDiff.Length() * vDiff.Length();

	float	fDet = ABS( fA00 * fA11 - fA01 * fA01 );

	float	fS = fA01 * fB1 - fA11 * fB0;
	float	fT = fA01 * fB0 - fA00 * fB1;
	float	fSqrDist;

	if( fS + fT <= fDet )
	{
		if( fS < 0.0F )
		{
			if( fT < 0.0f )			// region 4
			{
				if( fB0 < 0.0F )
				{
					fT = 0.0F;
					if( -fB0 >= fA00 )
					{
						fS = 1.0F;
						fSqrDist = fA00 + 2.0F * fB0 + fC;

						Assert( ABS( fSqrDist ) > MATH_EPSILON );
					}
					else
					{
						fS = -fB0 / fA00;
						fSqrDist = fB0 * fS + fC;

						Assert( ABS( fSqrDist ) > MATH_EPSILON );
					}
				}
				else
				{
					fS = 0.0F;
					if( fB1 >= 0.0F )
					{
						fT = 0.0F;
						fSqrDist = fC;

						Assert( ABS( fSqrDist ) > MATH_EPSILON );
					}
					else	if( -fB1 >= fA11 )
					{
						fT = 1.0F;
						fSqrDist = fA11 + 2.0F * fB1 + fC;

						Assert( ABS( fSqrDist ) > MATH_EPSILON );
					}
					else
					{
						fT = -fB1 / fA11;
						fSqrDist = fB1 * fT + fC;

						Assert( ABS( fSqrDist ) > MATH_EPSILON );
					}
				}
			}
			else			// region 3
			{
				fS = 0.0F;
				if( fB1 >= 0.0F )
				{
					fT = 0.0F;
					fSqrDist = fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else	if( -fB1 >= fA11 )
				{
					fT = 1.0F;
					fSqrDist = fA11 + 2.0F * fB1 + fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else
				{
					fT = -fB1 / fA11;
					fSqrDist = fB1 * fT + fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
			}
		}
		else	if( fT < 0.0F )		// region 5
		{
			fT = 0.0F;
			if( fB0 >= 0.0F )
			{
				fS = 0.0F;
				fSqrDist = fC;

				Assert( ABS( fSqrDist ) > MATH_EPSILON );
			}
			else	if( -fB0 >= fA00 )
			{
				fS = 1.0F;
				fSqrDist = fA00 + 2.0F * fB0 + fC;

				Assert( ABS( fSqrDist ) > MATH_EPSILON );
			}
			else
			{
				fS = -fB0 / fA00;
				fSqrDist = fB0 * fS + fC;

				Assert( ABS( fSqrDist ) > MATH_EPSILON );
			}
		}
		else	// region 0
		{
			// minimum at interior point

			if( ABS( fDet ) < MATH_EPSILON )
			{
				return 100000.0F;
			}

			Assert( ABS( fDet ) > MATH_EPSILON );

			float fInvDet = 1.0F / fDet;
			fS *= fInvDet;
			fT *= fInvDet;
			fSqrDist = fS * ( fA00 * fS + fA01 * fT + 2.0F * fB0 ) +
					   fT * ( fA01 * fS + fA11 * fT + 2.0F * fB1 ) + fC;

			Assert( ABS( fSqrDist ) > MATH_EPSILON );
		}
	}
	else
	{
		float	fTmp0, fTmp1, fNumer, fDenom;

		if( fS < 0.0F )  // region 2
		{
			fTmp0 = fA01 + fB0;
			fTmp1 = fA11 + fB1;
			if( fTmp1 > fTmp0 )
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00 - 2.0F * fA01 + fA11;
				if( fNumer >= fDenom )
				{
					fS = 1.0F;
					fT = 0.0F;
					fSqrDist = fA00 + 2.0F * fB0 + fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else
				{
					fS = fNumer / fDenom;
					fT = 1.0F - fS;
					fSqrDist = fS * ( fA00 * fS + fA01 * fT + 2.0F * fB0 ) +
							   fT * ( fA01 * fS + fA11 * fT + 2.0F * fB1 ) + fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
			}
			else
			{
				fS = 0.0F;
				if ( fTmp1 <= 0.0f )
				{
					fT = 1.0f;
					fSqrDist = fA11+2.0f*fB1+fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else if ( fB1 >= 0.0f )
				{
					fT = 0.0f;
					fSqrDist = fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else
				{
					fT = -fB1/fA11;
					fSqrDist = fB1*fT+fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
			}
		}
		else if ( fT < 0.0f )  // region 6
		{
			fTmp0 = fA01 + fB1;
			fTmp1 = fA00 + fB0;
			if ( fTmp1 > fTmp0 )
			{
				fNumer = fTmp1 - fTmp0;
				fDenom = fA00-2.0f*fA01+fA11;
				if ( fNumer >= fDenom )
				{
					fT = 1.0f;
					fS = 0.0f;
					fSqrDist = fA11+2.0f*fB1+fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else
				{
					fT = fNumer/fDenom;
					fS = 1.0f - fT;
					fSqrDist =	fS * ( fA00 * fS + fA01 * fT + 2.0F * fB0 ) +
								fT * ( fA01 * fS + fA11 * fT + 2.0F * fB1 ) + fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
			}
			else
			{
				fT = 0.0f;
				if ( fTmp1 <= 0.0f )
				{
					fS = 1.0f;
					fSqrDist = fA00+2.0f*fB0+fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else if ( fB0 >= 0.0f )
				{
					fS = 0.0f;
					fSqrDist = fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else
				{
					fS = -fB0/fA00;
					fSqrDist = fB0*fS+fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
			}
		}
		else  // region 1
		{
			fNumer = fA11 + fB1 - fA01 - fB0;
			if ( fNumer <= 0.0f )
			{
				fS = 0.0f;
				fT = 1.0f;
				fSqrDist = fA11+2.0f*fB1+fC;

				Assert( ABS( fSqrDist ) > MATH_EPSILON );
			}
			else
			{
				fDenom = fA00-2.0f*fA01+fA11;
				if ( fNumer >= fDenom )
				{
					fS = 1.0f;
					fT = 0.0f;
					fSqrDist = fA00+2.0f*fB0+fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
				else
				{
					fS = fNumer/fDenom;
					fT = 1.0f - fS;
					fSqrDist = fS*(fA00*fS+fA01*fT+2.0f*fB0) +
						fT*(fA01*fS+fA11*fT+2.0f*fB1)+fC;

					Assert( ABS( fSqrDist ) > MATH_EPSILON );
				}
			}
		}
	}

	return sqrtf( ABS( fSqrDist ) );
} //Triangle::Distance
