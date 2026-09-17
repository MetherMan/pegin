/*********************************************************************

	Matrix3.h

*********************************************************************/


#ifndef __Wind3D_Common_Matrix3_Header__
#define __Wind3D_Common_Matrix3_Header__


/**
 * 3x3 행렬
 * @ingroup		Common
 *
 * @author	아샬
 * @date	2003-02-06 오후 10:58
 */
struct COMMON_API Matrix3
{
	union
	{
		struct
		{
			float	_11, _12, _13;
			float	_21, _22, _23;
			float	_31, _32, _33;
		};
		float	m[3][3];
	};

	/**
	 *
	 */
	Matrix3()	{}

	/**
	 *
	 */
	Matrix3&	operator * ( const Matrix3& m ) const;

	/**
	 *
	 */
	const float&	operator () ( int nRow, int nColumn )
	{
		Assert( nRow < 3 );
		Assert( nColumn < 3 );
		return m[nRow][nColumn];
	}

	/**
	 *
	 */
	Quaternion&		GetQuaternion() const;

	/**
	 *
	 */
	void	SetIdentity()
	{
		_12 = _13 = 0.0F;
		_21 = _23 = 0.0F;
		_31 = _32 = 0.0F;
		_11 = _22 = _33 = 1.0F;
	}

	/**
	 *
	 */
	void	SetRotationX( float fAngle );

	/**
	 *
	 */
	void	SetRotationY( float fAngle );

	/**
	 *
	 */
	void	SetRotationZ( float fAngle );

	/**
	 *
	 */
	void	SetRotation( const Vector3 &vAxis, float fAngle );
};


#endif
