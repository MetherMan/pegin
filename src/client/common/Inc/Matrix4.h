/**
 * 4x4 행렬.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-07-30 3:14p $
 * @version	$Revision: 14 $
 *
 * <HR>
 * $Log: /Common/Inc/Matrix4.h $
 * 
 * 14    03-07-30 3:14p Kjmgo
 * 
 * 13    03-04-25 7:21p Ahastudio
 *
 * 12    03-03-14 5:09p Ahastudio		위치 관련 함수 추가.
 *
 * 11    03-03-09 6:30p Ahastudio		Matrix4x4_Inverse의 Input 인자를 const로 변경.
 *
 * 10    03-03-09 5:25p Ahastudio		가우스 소거법을 이용한 역행렬 구하는 함수 추가.
 * <HR>
 *
 * @file	Matrix4.h
 */


#ifndef __Wind3D_Common_Matrix4_Header__
#define __Wind3D_Common_Matrix4_Header__


/**
 * 4x4 행렬.
 * @ingroup		Common
 */
struct COMMON_API Matrix4
{
	union
	{
		struct
		{
			float	_11, _12, _13, _14;
			float	_21, _22, _23, _24;
			float	_31, _32, _33, _34;
			float	_41, _42, _43, _44;
		}; //struct
		float	m[4][4];
	}; //union

	/**
	 * 생성자.
	 */
	Matrix4()	{}

	Matrix4&	operator * ( const Matrix4& m_ ) const;

	/**
	 * 행렬을 곱한다.
	 */
	Matrix4&	operator *= ( const Matrix4& m_ )
	{
		*this = *this * m_;
		return *this;
	} //operator *=

	/**
	 * ...
	 */
	const float&	operator () ( const int nRow, const int nColumn )
	{
		Assert( nRow < 4 );
		Assert( nColumn < 4 );
		return m[nRow][nColumn];
	} //operator ()

	Matrix4&		Inverse() const;

	Quaternion&		GetQuaternion() const;

	/**
	 * 단위 행렬로 설정한다.
	 */
	void	SetIdentity()
	{
		_12 = _13 = _14 = 0.0F;
		_21 = _23 = _24 = 0.0F;
		_31 = _32 = _34 = 0.0F;
		_41 = _42 = _43 = 0.0F;
		_11 = _22 = _33 = _44 = 1.0F;
	} //SetIdentity

	void	SetRotationX( const float fAngle );
	void	SetRotationY( const float fAngle );
	void	SetRotationZ( const float fAngle );
	void	SetRotation( const Vector3& vINAxis, const float fAngle );

	void	SetView( const Vector3& vPos, const Vector3& vINDir, const Vector3& vINUp );
	void	SetProjection( const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane );

	void	SetWorld( const Vector3& vPos );
	void	SetWorld( const Vector3& vPos, const Quaternion& qRot );
	void	SetWorld( const Vector3& vPos, const Vector3& vDir, const Vector3& vUp );

	void	Translate( const Vector3& vPos );

	/**
	 * 위치 설정.
	 */
	void	SetPosition( const Vector3& vPos )
	{
		_41 = vPos.x;
		_42 = vPos.y;
		_43 = vPos.z;
	} //SetPosition

	/**
	 * 위치를 얻는다.
	 */
	Vector3		GetPosition() const
	{
		return Vector3( _41, _42, _43 );
	} //GetPosition

	Matrix4		GetBillboard() const;
	Matrix4		GetBillboardX() const;
	Matrix4		GetBillboardZ() const;
}; //struct COMMON_API Matrix4


#endif // #ifndef __Wind3D_Common_Matrix4_Header__
