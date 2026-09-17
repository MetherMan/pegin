/**
 * 사원수 모듈 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-04-26 10:19a $
 * @version	$Revision: 14 $
 *
 * <HR>
 * $Log: /Common/Src/Quaternion.cpp $
 * 
 * 14    03-04-26 10:19a Ahastudio
 *
 * 12    03-03-14 6:02p Ahastudio	구 선형 보간 함수 수정.
 * <HR>
 *
 * @file	Quaternion.cpp
 */


#define COMMON_EXPORT

#include "../Inc/Common.h"
#include <math.h>


/**
 * 생성자.
 */
Quaternion::Quaternion( const Vector3& vAxis, const float fAngle )
{
	float s = sinf( fAngle / 2 );
	float c = cosf( fAngle / 2 );
	x = s * vAxis.x;
	y = s * vAxis.y;
	z = s * vAxis.z;
	w = c;
} //Quaternion::Quaternion

/**
 * 사원수 구 선형 보간.
 */
const Quaternion& Quaternion::Interpolate( const Quaternion& qNext, const float fTime ) const
{
	static Quaternion	qC;
	const Quaternion&	qA = *this;
	Quaternion			qB = qNext;

	float	fCos =
		qA.x * qB.x +
		qA.y * qB.y +
		qA.z * qB.z +
		qA.w * qB.w;

	if( fCos < 0.0F )
	{	// 쿼터니언 뒤집기.
		qB.x = -qB.x;
		qB.y = -qB.y;
		qB.z = -qB.z;
		qB.w = -qB.w;
		fCos = -fCos;
	} //if

	float	fAlpha	= fTime;
	float	fBeta	= 1.0F - fTime;

	if( ( 1.0F - fCos ) > 0.001F )
	{	// 구 보간 적용.
		float	fTheta = acosf( fCos );

		fBeta	= sinf( fTheta * ( 1.0F - fTime ) ) / sinf( fTheta );
		fAlpha	= sinf( fTheta * fTime            ) / sinf( fTheta );
	} //if

	// 선형 보간.
	qC.x = fBeta * qA.x + fAlpha * qB.x;
	qC.y = fBeta * qA.y + fAlpha * qB.y;
	qC.z = fBeta * qA.z + fAlpha * qB.z;
	qC.w = fBeta * qA.w + fAlpha * qB.w;

	return qC;
} //Quaternion::Interpolate

/**
 * 4x4 행렬을 얻는다.
 */
const Matrix4& Quaternion::GetMatrix4() const
{
//	Assert( ABS( ( x * x + y * y + z * z + w * w ) - 1.0F ) < MATH_EPSILON );

	static Matrix4 m;

	float	xx = x * x;
	float	yy = y * y;
	float	zz = z * z;
	float	xy = x * y;
	float	yz = y * z;
	float	zx = z * x;
	float	wx = w * x;
	float	wy = w * y;
	float	wz = w * z;

	m._11 = 1.0F - 2.0F * ( yy + zz );
	m._12 =        2.0F * ( xy - wz );
	m._13 =        2.0F * ( zx + wy );

	m._21 =        2.0F * ( xy + wz );
	m._22 = 1.0F - 2.0F * ( xx + zz );
	m._23 =        2.0F * ( yz - wx );

	m._31 =        2.0F * ( zx - wy );
	m._32 =        2.0F * ( yz + wx );
	m._33 = 1.0F - 2.0F * ( xx + yy );

	m._14 = m._24 = m._34 = 0.0F;
	m._41 = m._42 = m._43 = 0.0F;
	m._44 = 1.0F;

	return m;
} //Quaternion::GetMatrix4

/**
 * 방향 벡터를 얻음.
 */
Vector3 Quaternion::GetDirection() const
{
//	Matrix4		matRot = GetMatrix4();
//	return	-Vector3( matRot._31, matRot._32, matRot._33 );

	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float yz = y * z;
	float zx = z * x;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	return -Vector3( 2.0F * ( zx - wy ), 0.0F, 1.0F - 2.0F * ( xx + yy ) ).Normal();
} //Quaternion::GetDirection

/**
 * X축을 중심으로 회전한다.
 */
void Quaternion::SetRotationX( const float fAngle )
{
	float s = sinf( fAngle / 2 );
	float c = cosf( fAngle / 2 );
	x = s * 1.0F;
	y = s * 0.0F;
	z = s * 0.0F;
	w = c;
} //Quaternion::SetRotationX

/**
 * Y축을 중심으로 회전한다.
 */
void Quaternion::SetRotationY( const float fAngle )
{
	float s = sinf( fAngle / 2 );
	float c = cosf( fAngle / 2 );
	x = s * 0.0F;
	y = s * 1.0F;
	z = s * 0.0F;
	w = c;
} //Quaternion::SetRotationY

/**
 * Z축을 중심으로 회전한다.
 */
void Quaternion::SetRotationZ( const float fAngle )
{
	float s = sinf( fAngle / 2 );
	float c = cosf( fAngle / 2 );
	x = s * 0.0F;
	y = s * 0.0F;
	z = s * 1.0F;
	w = c;
} //Quaternion::SetRotationZ

/**
 * 방향을 설정한다.
 */
void Quaternion::SetOrientation( const Vector3& vDir, const Vector3& vUp )
{
	Matrix4 matWorld;
	matWorld.SetWorld( Vector3( 0.0F, 0.0F, 0.0F ), vDir, vUp );
	*this = matWorld.GetQuaternion();
} //Quaternion::SetOrientation
