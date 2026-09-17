/**
 * Animation Manager
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-08 6:18p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Render/Src/Animation.h $
 * 
 * 9     04-01-08 6:18p Kjmgo
 * 
 * 8     04-01-08 9:48a Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	Animation.h
 */

#ifndef __Animation_Manager_Header__
#define __Animation_Manager_Header__

#include "Render_Export.h"

#define	NONANIMATION	0
#define NONBIPED		1
#define BIPED			2

/**
 *
 */
struct SBone
{
	MEMORY_UNDER_MGR( SBone );

	char				szName[20];	///< 이름
	Matrix4				matTM;		///< TM	
	Matrix4				matInverse;	///< TM 역행렬
	int					nParent;	///< 부모

	Vector3*			apPos;		///< 애니메이션 - 위치
	Quaternion*			apRot;		///< 애니메이션 - 회전
	
	/**
	 *
	 */
	SBone()
	{
		apPos = NULL;
		apRot = NULL;
	} //SBone;

	/**
	 *
	 */
	~SBone()
	{
		CHECK_BOUNCE( apPos );
		CHECK_BOUNCE( apRot );

		SAFE_FREE( apPos );
		SAFE_FREE( apRot );
	} //~SBone

	/**
	 *
	 */
	BOOL Alloc( int nPos, int nRot )
	{
		Assert( nPos >= 0 );
		Assert( nRot >= 0 );

		apPos = (Vector3*)_MemAlloc( sizeof( Vector3 ) * nPos );
		CHECK_PTR_RETURN( apPos );
		_ZeroMemory( apPos, sizeof( Vector3 ) * nPos );

		apRot = (Quaternion*)_MemAlloc( sizeof( Quaternion ) * nRot );
		CHECK_PTR_RETURN( apRot );
		_ZeroMemory( apRot, sizeof( Quaternion ) * nRot );

		return TRUE;
	} //Alloc
}; //struct SBone

/**
 *
 */
struct SMesh
{
	MEMORY_UNDER_MGR( SMesh );

	BOOL			bAnimation;		///< TM 애니메이션 사용 여부.

	int				nMesh;			///< 메쉬 번호
	Vector3*		apPos;			///< TM 애니메이션 - 위치.
	Quaternion*		apRot;			///< TM 애니메이션 - 회전.

	/**
	 *
	 */
	SMesh()
	{
		apPos = NULL;
		apRot = NULL;
		nMesh = -1;
	} //SMesh

	/**
	 *
	 */
	~SMesh()
	{
		CHECK_BOUNCE( apPos );
		CHECK_BOUNCE( apRot );

		SAFE_FREE( apPos );
		SAFE_FREE( apRot );
	} //~SMesh

	/**
	 *
	 */
	BOOL Alloc( int nPos, int nRot )
	{
		Assert( nPos >= 0 );
		Assert( nRot >= 0 );

		apPos = (Vector3*)_MemAlloc( sizeof( Vector3 ) * nPos );
		CHECK_PTR_RETURN( apPos );
		_ZeroMemory( apPos, sizeof( Vector3 ) * nPos );

		apRot = (Quaternion*)_MemAlloc( sizeof( Quaternion ) * nRot );
		CHECK_PTR_RETURN( apRot );
		_ZeroMemory( apRot, sizeof( Quaternion ) * nRot );

		return TRUE;
	} //Alloc

}; //struct SMesh

#endif //__Animation_Manager_Header__
