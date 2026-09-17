/**
 * 3차원 벡터.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-12-23 1:07a $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Common/Src/Vector3.cpp $
 * 
 * 9     03-12-23 1:07a Kjmgo
 * 벡터의 길이를 구할때, 제곱근을 구하지 않는 함수 추가
 * 
 * 8     03-04-25 7:22p Ahastudio
 * <HR>
 *
 * @file	Vector3.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"
#include <math.h>


/**
 * 3차원 벡터와 4x4 행렬을 곱한다.
 * 왼손 좌표계에 맞춰진 함수이므로 "벡터 * 행렬"의 연산을 한다.
 */
Vector3 Vector3::operator * ( const Matrix4 &m ) const
{
	float	fX = x * m._11 + y * m._21 + z * m._31 + m._41;
	float	fY = x * m._12 + y * m._22 + z * m._32 + m._42;
	float	fZ = x * m._13 + y * m._23 + z * m._33 + m._43;
	float	fW = x * m._14 + y * m._24 + z * m._34 + m._44;
	float	fRHW = 1.0F / fW;		// 곱하기 연산이 나누기보다 빠르므로 RHW를 사용한다.
	return Vector3( fX * fRHW, fY * fRHW, fZ * fRHW );
} //Vector3::operator *

/**
 * 벡터의 길이를 구한다.
 */
float Vector3::Length() const
{
	return sqrtf( x * x + y * y + z * z );
} //Vector3::Length

/**
 * 벡터의 길이를 구하되, 제곱근을 구하지 아니한다.
 */
float Vector3::LengthNoSqrt() const
{
	return x * x + y * y + z * z;
} //Vector3::LengthNoSqrt

/**
 * X축을 중심으로 회전한다.
 */
void Vector3::RotateX( float fAngle )
{
	Matrix4		matRot;
	matRot.SetRotationX( fAngle );
	*this *= matRot;
} //Vector3::RotateX

/**
 * Y축을 중심으로 회전한다.
 */
void Vector3::RotateY( float fAngle )
{
	Matrix4		matRot;
	matRot.SetRotationY( fAngle );
	*this *= matRot;
} //Vector3::RotateY

/**
 * Z축을 중심으로 회전한다.
 */
void Vector3::RotateZ( float fAngle )
{
	Matrix4		matRot;
	matRot.SetRotationZ( fAngle );
	*this *= matRot;
} //Vector3::RotateZ

/**
 * 임의의 축을 중심으로 회전한다.
 */
void Vector3::Rotate( const Vector3& vAxis, float fAngle )
{
	Matrix4		matRot;
	matRot.SetRotation( vAxis, fAngle );
	*this *= matRot;
} //Vector3::Rotate

/**
 * 방향 벡터로 간주하고 사원수를 얻는다.
 */
Quaternion Vector3::GetQuaternion()
{
	Vector3		vDir;
	vDir.x = x;
	vDir.y = y;
	vDir.z = 0.0F;
	vDir.Normalize();
	vDir *= -1;

	Matrix4		matWorld;
	matWorld.SetWorld( Vector3( 0.0F, 0.0F, 0.0F ), vDir, Vector3( 0.0F, 0.0F, 1.0F ) );

	return matWorld.GetQuaternion();
} //Vector3::GetQuaternion
