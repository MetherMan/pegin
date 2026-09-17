/**
 * 메쉬.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	Teammay
 * @date	$Date: 04-06-08 3:56p $
 * @version	$Revision: 42 $
 *
 * <HR>
 * $Log: /Render/Src/Mesh.h $
 * 
 * 42    04-06-08 3:56p Paran2
 * 
 * 41    04-03-22 8:44p Paran
 * 
 * 40    04-01-26 2:32p Kjmgo
 * 
 * 39    04-01-08 6:18p Kjmgo
 * 
 * 38    04-01-08 5:19p Kjmgo
 * 
 * 37    04-01-08 1:55p Kjmgo
 * 
 * 36    04-01-08 9:49a Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	Mesh.h
 */


#ifndef __Mesh_Header__
#define __Mesh_Header__


#include "Render_Export.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <string>
#include <vector>


using std::string;
using std::vector;


/**
 * 메쉬 클래스.
 * @ingroup		Render
 */
class CMesh
{
public:
	UNDER_MEMORY_MGR( CMesh );

//	string				m_sName;			///< 이름.
	Matrix4				m_matTM;			///< TM.

	int					m_nNumFaces;		///< 면 갯수.
	int					m_nNumVertices;		///< 버텍스 갯수
	int					m_nNumIndices;		///< Index 갯수

	Vector3*			m_apVertices;		///< 버텍스 배열
	WORD*				m_apIndices;		///< 인덱스 배열

	CVertexBuffer*		m_pVB;				///< 버텍스 버퍼.
	CIndexBuffer*		m_pIB;				///< 인덱스 버퍼.

	D3DMATERIAL8		m_d3dMaterial;		///< D3D Material.
	CTexture*			m_pTexture;			///< 텍스처.

	BOOL				m_bVertexBlend;		///< 버텍스 블렌딩 사용 여부.

	DWORD				m_dwColor;			///< 알파를 포함한 색상.
	BOOL				m_bUseTexture;		///< 텍스처 사용 여부.

	/**
	 * 생성자.
	 */
	CMesh()
	:	m_nNumFaces			( 0 ),
		m_nNumVertices		( 0 ),
		m_nNumIndices		( 0 ),
		m_pVB				( NULL ),
		m_pIB				( NULL ),
		m_apVertices		( NULL ),
		m_apIndices			( NULL ),
		m_pTexture			( NULL ),
		m_bVertexBlend		( FALSE ),
		m_dwColor			( 0xFFFFFFFF ),
		m_bUseTexture		( TRUE )
	{
		m_matTM.SetIdentity();
	} //CMesh

	/**
	 * 소멸자.
	 */
	~CMesh()
	{
		CHECK_BOUNCE( m_apIndices );
		CHECK_BOUNCE( m_apVertices );
		SAFE_FREE( m_apIndices );
		SAFE_FREE( m_apVertices );

		SAFE_RELEASE( m_pTexture );
		SAFE_DELETE( m_pIB );
		SAFE_DELETE( m_pVB );
	} //~CMesh

	/**
	 *
	 */
	BOOL	AllocVertex()
	{
		Assert( m_nNumVertices >= 0 );

		m_apVertices = (Vector3*)_MemAlloc( sizeof( Vector3 ) * m_nNumVertices );
		CHECK_PTR_RETURN( m_apVertices );
		_ZeroMemory( m_apVertices, sizeof( Vector3 ) * m_nNumVertices );

		return TRUE;
	} //AllocVertex

	/**
	 *
	 */
	BOOL	AllocIndex()
	{
		Assert( m_nNumIndices >= 0 );

		m_apIndices = (WORD*)_MemAlloc( sizeof( WORD ) * m_nNumIndices );
		CHECK_PTR_RETURN( m_apIndices );
		_ZeroMemory( m_apIndices, sizeof( WORD ) * m_nNumIndices );

		return TRUE;
	} //AllocIndex

	/**
	 *
	 */
	void	ClearMaterial()
	{
		m_d3dMaterial.Diffuse.r = 1.0F;
		m_d3dMaterial.Diffuse.g = 1.0F;
		m_d3dMaterial.Diffuse.b = 1.0F;
		m_d3dMaterial.Diffuse.a = 1.0F;

		m_d3dMaterial.Ambient.r = 0.5F;
		m_d3dMaterial.Ambient.g = 0.5F;
		m_d3dMaterial.Ambient.b = 0.5F;
		m_d3dMaterial.Ambient.a = 1.0F;

		m_d3dMaterial.Specular.a = 1.0F;
	} //ClearMaterial

	/**
	 * 메쉬를 그린다.
	 */
	void	Render()
	{
		Assert( g_pd3dDevice );
		Assert( m_pVB );

		BYTE	a = (BYTE)( m_dwColor >> 24 );
		BYTE	r = (BYTE)( m_dwColor >> 16 );
		BYTE	g = (BYTE)( m_dwColor >> 8 );
		BYTE	b = (BYTE) m_dwColor;

		D3DMATERIAL8	d3dMaterial = m_d3dMaterial;
		d3dMaterial.Diffuse.a *= ( a / 255.0F );
		d3dMaterial.Diffuse.r *= ( r / 255.0F );
		d3dMaterial.Diffuse.g *= ( g / 255.0F );
		d3dMaterial.Diffuse.b *= ( b / 255.0F );
		g_pd3dDevice->SetMaterial( &d3dMaterial );

		if( m_bUseTexture )
		{
			SET_TEXTURE( 0, m_pTexture );
		} //if

		if( m_pIB )
		{
			m_pIB->SetIndex();
			m_pVB->DrawIndexed( D3DPT_TRIANGLELIST, m_nNumFaces );
		}
		else
		{
			m_pVB->Draw( D3DPT_TRIANGLELIST, m_nNumFaces );
		} //if..else..

	} //Render

	/**
	 * 메쉬 피킹.
	 */
	BOOL	Pick(	const Matrix4& matWorld,	///< [IN] 월드 변환 행렬.
					const Vector3& vOrig,		///< [IN] 반직선 시작점.
					const Vector3& vDir,		///< [IN] 반직선 방향.
					float* pfDistance = NULL	///< [OUT] 피킹 거리.
					)
	{
		Assert( m_pVB );

		BOOL		bPick			= FALSE;
		float		fMinDistance	= 1000.0F;

		vector<Vector3>		aVertices;

		aVertices.resize( m_nNumVertices );

		for( int iVertex = 0 ; iVertex < m_nNumVertices ; iVertex++ )
		{
			aVertices[iVertex] = m_apVertices[iVertex] * matWorld;
		} //for

		for( int iFace = 0 ; iFace < m_nNumFaces ; iFace++ )
		{
			int		vertex[3];

			for( int i = 0 ; i < 3 ; i++ )
			{
				if( !m_nNumIndices )
				{
					vertex[i] = iFace * 3 + i;
				}
				else
				{
					vertex[i] = m_apIndices[iFace * 3 + i];
				} //if..else..
			} //for

			Triangle	tri = Triangle(	aVertices[ vertex[0] ],
										aVertices[ vertex[1] ],
										aVertices[ vertex[2] ] );

			float	fDistance;

			if( tri.Intersect( vOrig, vDir, &fDistance ) )
			{
				bPick = TRUE;
				fMinDistance = MIN( fMinDistance, fDistance );
			} //if
		} //for

		if( bPick && pfDistance )
		{
			*pfDistance = fMinDistance;
		} //if

		return bPick;
	} //Pick

	/**
	 *
	 */
	Vector3 GetXYMax( Matrix4 matLight )
	{
		if( !m_apVertices )
		{
			Vector3( 0.0F, 0.0F, 0.0F );
		} //if

		int	  numVertices = m_nNumVertices;
		float fXMax = 0.0F;
		float fYMax = 0.0F;

		for( int iVertex = 0 ; iVertex < numVertices ; iVertex++ )
		{
			Vector3 vOut = m_apVertices[iVertex] * matLight;

			if((float)fabs(vOut.x/vOut.z) > fXMax) fXMax = (float)fabs(vOut.x/vOut.z);
			if((float)fabs(vOut.y/vOut.z) > fYMax) fYMax = (float)fabs(vOut.y/vOut.z);
		} //for

		return Vector3( fXMax, fYMax, 0.0F );
	} //GetXYMax
}; //class CMesh


#endif //#ifndef __Mesh_Header__
