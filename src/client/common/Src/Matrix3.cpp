/*********************************************************************

	Matrix3.cpp

*********************************************************************/


#define COMMON_EXPORT

#include "../Inc/Common.h"
#include <math.h>

//
// Matrix3::operator *
//
Matrix3& Matrix3::operator * ( const Matrix3& m_ ) const
{
	static Matrix3 matrix;

	ZeroMemory( &matrix, sizeof( matrix ) );
	for( int i = 0 ; i < 3 ; i++ )
	{
		for( int j = 0 ; j < 3 ; j++ )
		{
			for( int k = 0 ; k < 4 ; k++ )
			{
				matrix.m[i][j] += m[i][k] * m_.m[k][j];
			}
		}
	}

	return matrix;
}

//
// Matrix3::GetQuaternion
//
Quaternion& Matrix3::GetQuaternion() const
{
	static Quaternion q;

	Assert( _11 + _22 + _33 > 0.0F );

	float s = sqrtf( _11 + _22 + _33 + 1.0F );

	q.x = ( _23 - _32 ) / ( 2.0F * s );
	q.y = ( _31 - _13 ) / ( 2.0F * s );
	q.z = ( _12 - _21 ) / ( 2.0F * s );
	q.w = 0.5F * s;

	return q;
}

//
// Matrix3::SetRotationX
//
void Matrix3::SetRotationX( float fAngle_ )
{
	float fCos = cosf( fAngle_ );
	float fSin = sinf( fAngle_ );
	SetIdentity();
	_22 = fCos;
	_23 = fSin;
	_32 = -fSin;
	_33 = fCos;
}

//
// Matrix3::SetRotationY
//
void Matrix3::SetRotationY( float fAngle_ )
{
	float fCos = cosf( fAngle_ );
	float fSin = sinf( fAngle_ );
	SetIdentity();
	_11 = fCos;
	_13 = -fSin;
	_31 = fSin;
	_33 = fCos;
}

//
// Matrix3::SetRotationZ
//
void Matrix3::SetRotationZ( float fAngle_ )
{
	float fCos = cosf( fAngle_ );
	float fSin = sinf( fAngle_ );
	SetIdentity();
	_11 = fCos;
	_12 = fSin;
	_21 = -fSin;
	_22 = fCos;
}
