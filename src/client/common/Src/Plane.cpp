/**
 * 평면.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-04-25 7:22p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Common/Src/Plane.cpp $
 * 
 * 3     03-04-25 7:22p Ahastudio
 * <HR>
 *
 * @file	Plane.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"

/**
 * 반직선과 평면의 교차점을 찾는다.
 */
Vector3 Plane::Pick( const Vector3& vOrig, const Vector3& vDir ) const
{
	return vOrig + vDir * Distance( vOrig ) / vDir.DotProduct( -N );
} //Plane::Pick
