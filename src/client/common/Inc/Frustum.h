/**
 * @file	Frustum.h
 *
 * 시야 절두체 모듈.
 *
 * @author	아샬
 * @date	2003-02-06 오후 10:58
 * @version	1.0
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 */


#ifndef __Wind3D_Common_Frustum_Header__
#define __Wind3D_Common_Frustum_Header__


/**
 * 시야 절두체.
 * @ingroup		Common
 *
 * @author	아샬
 * @date	2003-02-06 오후 10:58
 */
struct COMMON_API	Frustum
{
	Vector3		m_vDir[4];
	Vector3		m_vPos;
	Plane		m_Plane[4];
	float		m_fNearPlane;
	float		m_fFarPlane;
	Matrix4		m_matView;
	Matrix4		m_matBillboard;
	Matrix4		m_matBillboardY;
	Matrix4		m_matBillboardZ;

	/**
	 * 생성자.
	 *
	 * @author	아샬
	 * @date	2003-02-06 오후 10:58
	 */
	Frustum()	{}

	void	SetProjection( float fFOV, float fAspect, float fNearPlane, float fFarPlane );
	void	SetView( const Matrix4& matView );

	/**
	 * 빌보드 행렬을 얻는다.
	 *
	 * @return	빌보드 행렬.
	 *
	 * @author	아샬
	 * @date	2003-02-19 오전 7:32
	 */
	Matrix4		GetBillboard() const
	{
		return m_matBillboard;
	}

	/**
	 * Y 빌보드 행렬을 얻는다.
	 *
	 * @return	빌보드 행렬.
	 *
	 * @author	아샬
	 * @date	2003-02-19 오후 5:41
	 */
	Matrix4		GetBillboardY() const
	{
		return m_matBillboardY;
	}

	/**
	 * Z 빌보드 행렬을 얻는다.
	 *
	 * @return	빌보드 행렬.
	 *
	 * @author	아샬
	 * @date	2003-02-19 오전 7:32
	 */
	Matrix4		GetBillboardZ() const
	{
		return m_matBillboardZ;
	}

	BOOL	CheckPoint( const Vector3& vPos );
	BOOL	CheckSphere( const Vector3& vPos, float fRadius );
	BOOL	CheckCube( const Vector3& vPos, float fSize );
};


#endif
