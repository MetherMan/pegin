/**
 * 모델 컴포넌트 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	Teammay
 * @date	$Date: 04-06-08 10:28p $
 * @version	$Revision: 149 $
 *
 * <HR>
 * $Log: /Render/Src/W3DModel.cpp $
 * 
 * 149   04-06-08 10:28p Paran
 * 
 * 148   04-06-08 10:16p Paran
 * 
 * 147   04-06-08 3:56p Paran2
 * 
 * 146   04-05-28 1:38p Sckyj
 * D3D::SetSoftwareVertexProcessing() 함수 사용안하도록 수정
 * 
 * 145   04-05-11 11:33a Sckyj
 * SetInterpolateBlend 함수 추가 - 동작사이의 보간용
 * 
 * 144   04-05-10 7:32p Kjmgo
 * Mix Vertex Processing 적용
 * 
 * 143   04-03-26 7:02p Paran
 * 
 * 142   04-03-19 7:03p Paran
 * 
 * 141   04-03-08 3:01p Sckyj			GetModelName() 함수 추가
 * 
 * 140   04-02-02 11:43a Kjmgo			밉맵 적용
 * 
 * 134   04-01-08 2:01p Kjmgo			코드 정리
 * 
 * 133   04-01-08 9:52a Kjmgo			코드 정리
 * 
 * <HR>
 *
 * @file	W3DModel.cpp
 */


#include "Render_Export.h"
#include "Mesh.h"
#include "Animation.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#define CHECK_KEYWORD( s )	!strcmp( szKeyword, s )


using std::string;
using std::vector;
using std::map;

/**
 *
 */
struct face_t
{
	int			vertex;
	Vector3		n;
	float		tu, tv;
}; //struct face_t

/**
 * 모델 클래스.
 * @ingroup		Render
 */
class CW3DModel : public IW3DModel
{
public:
	UNDER_MEMORY_MGR( CW3DModel );

	CW3DModel();
	~CW3DModel();

	BOOL		Load( const char* szPathName, const char* szFileName );
	BOOL		LoadAnimation( const char* szPathName, const char* szFileName );

	void		InitAABB();

	BOOL		IsVisible();
	
	BOOL		Pick( const Vector3& vOrig, const Vector3& vDir, float* pfDistance );
	BOOL		CollisionPoint( const Vector3& vPos );
	
	BOOL		SetMultiActionVertexBlend( 
					const SAction& sBlendAction, 
					const DWORD    dwTick,
					const DWORD    dwBlendActTick 
					);
	
	void		SetWorld( const Vector3& vPos, const Quaternion& qRot );
	void		SetWorld( const Matrix4& matWorld );
	void		SetColor( const DWORD dwColor );
	void		SetUseTexutre( const BOOL bUse );
	void		SetAction( SAction* pAction );
	void		SetVertexBlend( const DWORD dwTick );	
	void		SetInterpolateBlend( DWORD		dwPrevTicks,		// Prev Animation Frame Ticks
									 DWORD		dwNextTicks,		// Next Animation Frame Ticks
									 SAction*	pInterAction,		// Interpolation Animation
									 float		fInterTime );		// Interpolation Time

	void		SetApplyWorld( BOOL bApply )
	{
		m_bApplyWorldMatrix = bApply;
	} //SetApplyWorld
	
	void		SetShadow( const BOOL bUse )
	{
		m_bShadow = bUse;
	} //SetShadow

	void		CalculateRadius();
	void		Render( const DWORD dwTick, const BOOL bLighting = TRUE, const BOOL bAlpha = TRUE );
	void		RenderEdge();


	Matrix4		GetBoneMatrix( const char* szName, const DWORD dwTick );
	Matrix4		GetTMMatrix( const DWORD dwTick );

	float			GetRadius();
	Vector3			GetSize();

	Vector3			GetXYMax( const Matrix4& vLight );
	const Vector3&	GetPivot();

	DWORD		GetActionTicks();
	DWORD		GetTime();

	const char* GetModelName();

private:

	/**
	 *
	 */
	struct	SVertex
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 };
		Vector3		p;
		Vector3		n;
		float		tu, tv;
	}; //struct	SVertex

	/**
	 *
	 */
	struct	SAniVertex
	{
		enum	{ FVF = D3DFVF_XYZB1 | D3DFVF_LASTBETA_UBYTE4 | D3DFVF_NORMAL | D3DFVF_TEX1 };
		Vector3		p;
		union
		{
			float	beta;
			BYTE	indices[4];
		}; //union
		Vector3		n;
		float		tu, tv;
	}; //struct	SAnimationVertex

	int					m_numMeshes;		///< Mesh 갯수
	CMesh*				m_apMesh;			///< Array Meshs
	SAction*			m_pAction;			///< 행동 포인터 ( 현재 쓰일 )
	Vector3				m_vPivot;			///< 모델의 중심
	Vector3				m_vSize;			///< 모델의 크기
	float				m_fRadius;			///< 모델의 반지름
	std::string			m_sName;			///< Model Name

	Matrix4				m_matWorld;			///< 월드 변환 행렬

	DWORD				m_dwTime;			///< Ticks
	DWORD				m_dwColor;			///< 알파를 포함한 색상
	BOOL				m_bUseTexture;		///< 텍스처 사용 여부
	BOOL				m_bShadow;			///< Shadow용 Render여부

	int					m_nFrameRate;		///< 애니메이션 프레임 레이트

	int					m_nAniType;			///< 애니메이션 타입
	int					m_nAction;			///< 현재 플레이 중인 동작

	Vector3				m_vMax;				///<
	BOOL				m_bVisible;			///< 모델 출력여부.
	BOOL				m_bApplyWorldMatrix;

	AABB				m_AABB;				///< AABB
	Vector3				m_avTest[6];			///< Test
	
	IW3DAnimation*		m_pAnimation;		///< 애니메이션 
	IW3DAnimation*		m_pPrevAnimation;	///< 보간용 애니메이션
	IW3DAnimationMgr*	m_pAnimationMgr;	///< 애니메이션 
	
private:


	BOOL	LoadMesh( Archive& ar, const char* sPathName, CMesh* pMesh );
	BOOL	SetVertexBuffer( CMesh* pMesh, const vector<face_t>& aFaces );
	BOOL	SetAniVertexBuffer( CMesh* pMesh,  const vector<face_t>& aFaces, const vector<BYTE>& aPhysique );

	void	LoadTMAnimation( Archive& ar );
	void	SetRenderState( const BOOL bLighting = TRUE, const BOOL bAlpha = TRUE );
	void	Visible( const BOOL bVisible )		{ m_bVisible = bVisible; }
	void	CalculatePivot();
}; //class CW3DModel


IMPLEMENT_COMPONENT( CW3DModel, Model );

/**
 * 생성자.
 */
CW3DModel::CW3DModel()
:	m_vPivot		( 0.0F, 0.0F, 0.0F ),
	m_fRadius		( 0.0F ),
	m_pAction		( NULL ),
	m_pPrevAnimation( NULL ),
	m_dwColor		( 0xFFFFFFFF ),
	m_bUseTexture	( TRUE ),
	m_bApplyWorldMatrix( TRUE ),
	m_bShadow		( FALSE ),
	m_nFrameRate	( 35 ),
	m_nAction		( 0 ),
	m_numMeshes		( 0 ),
	m_pAnimation	( NULL ),
	m_apMesh		( NULL ),
	m_nAniType		( NONANIMATION ),
	m_bVisible		( TRUE )
{	
} //CW3DModel::CW3DModel

/**
 * 소멸자.
 */
CW3DModel::~CW3DModel()
{
	SAFE_DELETE_ARRAY( m_apMesh );
} //CW3DModel::~CW3DModel

/**
 * 모델을 불러온다.
 *
 * @return	읽기 성공 여부
 */
BOOL CW3DModel::Load(
	const char* szPathName,		///< [IN] 경로 이름
	const char* szFileName		///< [IN] 파일 이름
	)
{
	Assert( szPathName );
	Assert( szFileName );

	char szFile[128];

	sprintf( szFile, "%s/%s", szPathName, szFileName );

	FILE*	fp = fopen( szFile, "rb" );
	if( !fp )
	{
		DEBUG_OUT( "CW3DModel::Load - 파일 열기 실패 - " + szFile );
		return FALSE;
	} //if

	m_sName = szFile;

	ArchiveFileReader	ar( fp );

	char	szCode[32];
	ar.Serialize( &szCode, 32 );
	ar.Serialize( &m_numMeshes, sizeof( int ) );

	m_apMesh = new CMesh[m_numMeshes];
	CHECK_PTR_RETURN( m_apMesh );

	for( int iMesh = 0 ; iMesh < m_numMeshes ; iMesh++ )
	{
		CMesh*	pMesh = &m_apMesh[iMesh];
		
		if( !LoadMesh( ar, szPathName, pMesh ) )
		{
			Assert( !"Mesh 읽기 실패" );
			SAFE_DELETE( pMesh );
			ar.Close();
			return FALSE;
		} //if
	} //for

	ar.Close();

	InitAABB();

	return TRUE;
} //CW3DModel::LoadMod

/**
 * 메쉬를 읽어온다.
 *
 * @return	로드 성공 여부
 */
BOOL CW3DModel::LoadMesh(
	Archive&		ar,				///< [IN] 아카이브
	const char*		szPathName,		///< [IN] Path Name
	CMesh*			pMesh			///< [IN] 메쉬
	)
{
	Assert( pMesh );

	ar.Serialize( &pMesh->m_matTM, sizeof( Matrix4 ) );

	int				numVertices;
	int				numFaces;
	vector<face_t>	aFaces;

	ar.Serialize( &numVertices, sizeof( int ) );
	pMesh->m_nNumVertices = numVertices;
	
	pMesh->AllocVertex();

	ar.Serialize( pMesh->m_apVertices, numVertices * sizeof( Vector3 ) );

	ar.Serialize( &numFaces, sizeof( int ) );
	aFaces.resize( numFaces * 3 );

	ar.Serialize( aFaces.begin(), numFaces * 3 * sizeof( face_t ) );

	pMesh->m_nNumIndices = numFaces * 3;
	pMesh->AllocIndex();

	for( int iIndex = 0 ; iIndex < numFaces * 3 ; iIndex++ )
	{
		pMesh->m_apIndices[iIndex] = aFaces[iIndex].vertex;
	} //for

	pMesh->m_nNumFaces = numFaces;

	// Material
	_ZeroMemory( &pMesh->m_d3dMaterial, sizeof( D3DMATERIAL8 ) );

	pMesh->ClearMaterial();

	ar.Serialize( &pMesh->m_d3dMaterial.Specular.r, sizeof( float ) );
	ar.Serialize( &pMesh->m_d3dMaterial.Specular.g, sizeof( float ) );
	ar.Serialize( &pMesh->m_d3dMaterial.Specular.b, sizeof( float ) );
	ar.Serialize( &pMesh->m_d3dMaterial.Power, sizeof( float ) );
	pMesh->m_d3dMaterial.Power *= 100.0F;

	if( pMesh->m_d3dMaterial.Power <= 0.0F )
	{
		pMesh->m_d3dMaterial.Specular.r = 0.0F;
		pMesh->m_d3dMaterial.Specular.g = 0.0F;
		pMesh->m_d3dMaterial.Specular.b = 0.0F;
	} //if

	// Texture
	char szTexture[40];
	char szPathFile[128];
	ar.Serialize( szTexture, 32 );

	sprintf( szPathFile, "Texture/%s/", szPathName );
	pMesh->m_pTexture = g_pTextureMgr->LoadTexture( szPathFile, szTexture );

	// AniType
	int		nAniType;
	ar.Serialize( &nAniType, sizeof( int ) );
	m_nAniType = nAniType;

	switch( nAniType )
	{
	case NONANIMATION:		// 애니메이션 없음.
	case NONBIPED:		// TM 애니메이션.
		if( !SetVertexBuffer( pMesh, aFaces ) )
		{
			return FALSE;
		} //if
		pMesh->m_bVertexBlend = FALSE;
		break;

	case BIPED:		// 캐릭터.
		{
			vector<BYTE>	aPhysique;
			aPhysique.resize( numVertices );
			ar.Serialize( aPhysique.begin(), sizeof( BYTE ) * numVertices );

			if( !SetAniVertexBuffer( pMesh, aFaces, aPhysique ) )
			{
				return FALSE;
			} //if
		}
		pMesh->m_bVertexBlend = TRUE;
		break;
	} //switch

	return TRUE;
} //CW3DModel::LoadMesh

/**
 * Calculate  Axis-Aligned Bounding Box
 */
void CW3DModel::InitAABB()
{
	int			numVertices;
	Vector3		vMax( 0.0F, 0.0F, 0.0F );
	Vector3		vMin( 9999.0F, 9999.0F, 9999.0F );
	
	for( int iMesh = 0 ; iMesh < m_numMeshes ; iMesh++ )
	{
		CMesh*		pMesh = &m_apMesh[iMesh];
		numVertices = pMesh->m_nNumVertices;

		for( int iVertex = 0 ; iVertex < numVertices ; iVertex++ )
		{
			Vector3 v = pMesh->m_apVertices[iVertex];

			// X
			if( v.x > vMax.x )
				vMax.x = v.x;
			else if( v.x < vMin.x )
				vMin.x = v.x;

			// Y
			if( v.y > vMax.y )
				vMax.y = v.y;
			else if( v.y < vMin.y )
				vMin.y = v.y;

			// Z
			if( v.z > vMax.z )
				vMax.z = v.z;
			else if( v.z < vMin.z )
				vMin.z = v.z;
		} //for
	} //for

	m_AABB.MakeAABB( vMin, vMax );

	m_vSize = vMax - vMin;
	m_fRadius = (float)	m_vSize.Length() / 2;
} //CW3DModel::InitAABB

/**
 * 버텍스 버퍼 설정.
 */
BOOL CW3DModel::SetVertexBuffer(
	CMesh*					pMesh,		///< [OUT] 메쉬
	const vector<face_t>&	aFaces		///< [IN] 페이스
	)
{
	int		numFaces = pMesh->m_nNumFaces;

	pMesh->m_pVB = new CVertexBuffer( SVertex::FVF, sizeof( SVertex ) );
	CHECK_PTR_RETURN( pMesh->m_pVB );

	if( !pMesh->m_pVB->Create( numFaces * 3 ) )
	{
		Assert( !"Vertex Buffer Create Failed...." );
		return FALSE;
	} //if

	SVertex*	pVertices = (SVertex*)pMesh->m_pVB->Lock();
	for( int i = 0 ; i < numFaces * 3 ; i++ )
	{
		pVertices[i].p	= pMesh->m_apVertices[ aFaces[i].vertex ];
		pVertices[i].n	= aFaces[i].n;
		pVertices[i].tu	= aFaces[i].tu;
		pVertices[i].tv	= aFaces[i].tv;
	} //for
	pMesh->m_pVB->Unlock();

	return TRUE;
} //CW3DModel::SetVertexBuffer

/**
 * 애니메이션 버텍스 버퍼 설정.
 */
BOOL CW3DModel::SetAniVertexBuffer(
	CMesh*					pMesh,		///< [OUT] 메쉬
	const vector<face_t>&	aFaces,		///< [IN] 면
	const vector<BYTE>&		aPhysique	///< [IN] 피지크
	)
{
	int		numFaces = pMesh->m_nNumFaces;

	pMesh->m_pVB = new CVertexBuffer( SAniVertex::FVF, sizeof( SAniVertex ) );
	CHECK_PTR_RETURN( pMesh->m_pVB );

	if( !pMesh->m_pVB->Create( numFaces * 3 ) )
	{
		Assert( !"Vertex Buffer Create Failed...." );
		return FALSE;
	} //if

	SAniVertex*		pVertices = (SAniVertex*)pMesh->m_pVB->Lock();
	for( int i = 0 ; i < numFaces * 3 ; i++ )
	{
		int		vertex = aFaces[i].vertex;

		if( vertex >= (int)pMesh->m_nNumVertices ||
			vertex >= (int)aPhysique.size()
			)
		{
			pMesh->m_pVB->Unlock();
			return FALSE;
		} //if

		Assert( vertex < (int)pMesh->m_nNumVertices );
		Assert( vertex < (int)aPhysique.size() );

		pVertices[i].p	= pMesh->m_apVertices[ vertex ];
		pVertices[i].n	= aFaces[i].n;
		pVertices[i].tu	= aFaces[i].tu;
		pVertices[i].tv	= aFaces[i].tv;

		pVertices[i].indices[0] = aPhysique[ vertex ];
		pVertices[i].indices[1] = 0;
		pVertices[i].indices[2] = 0;
		pVertices[i].indices[3] = 0;
	} //for
	pMesh->m_pVB->Unlock();

	return TRUE;
} //CW3DModel::SetAniVertexBuffer

/**
 * 애니메이션 파일을 읽음.
 */
BOOL CW3DModel::LoadAnimation(
	const char* szPathName,		///< [IN] 경로 이름
	const char* szFileName		///< [IN] 파일 이름
	)
{
	/*
		TODO : SAni을 쓰는곳을 모두 수정해야 한다.
				현재는 주석 처리
	*/
	m_pAnimationMgr = MAKE_COMPONENT( AnimationMgr );
	m_pAction = NULL;
	m_pAction = m_pAnimationMgr->LoadAnimation( szPathName, szFileName );

	if( !m_pAction )
	{
		return FALSE;
	} //if

	m_pAnimation = m_pAction->pAnimation; 
	return TRUE;
} //CW3DModel::LoadAnimation

/** 
 * 월드 변환 행렬 설정.
 */
void CW3DModel::SetWorld(
	const Vector3& vPos,
	const Quaternion& qRot
	)
{
	m_matWorld.SetWorld( vPos, qRot );
} //CW3DModel::SetPosition

/**
 * 월드 변환 행렬 설정.
 */
void CW3DModel::SetWorld( const Matrix4& matWorld )
{
	m_matWorld = matWorld;
} //CW3DModel::SetPosition

/**
 * 모델이 화면에 보이는지 검사한다.
 */
BOOL CW3DModel::IsVisible()
{
	if( g_Frustum.CheckSphere( -m_vPivot * m_matWorld, m_fRadius ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	} //if..else..
} //CW3DModel::CheckVisible

/**
 * 알파값을 설정한다.
 */
void CW3DModel::SetColor( const DWORD dwColor )
{
	m_dwColor = dwColor;
} //CW3DModel::SetAlpha

/**
 * 텍스처 사용 여부를 설정한다.
 */
void CW3DModel::SetUseTexutre( const BOOL bUse )
{
	m_bUseTexture = bUse;
} //CW3DModel::SetUseTexutre

/**
 * 모델을 그린다.
 */
void CW3DModel::Render( const DWORD dwTick, const BOOL bLighting, const BOOL bAlpha )
{
	if( !m_bVisible )		return;
	if( !m_apMesh )			return;

	m_dwTime = dwTick;
	Assert( g_pd3dDevice );

	SetRenderState( bLighting, bAlpha );
	CMesh* pMesh = NULL;

	for( int i = 0 ; i < m_numMeshes ; i++ )
	{
		pMesh = &m_apMesh[i];

		if( !pMesh )	continue;

		if( pMesh->m_bVertexBlend )
		{	// 버텍스 블렌딩..
			D3D::SetSoftVertexProcessing( TRUE );

			if( m_bApplyWorldMatrix )
			{
				SetVertexBlend( dwTick );
			}
			else
			{
				D3D::VertexBlend( D3DVBF_0WEIGHTS );
				D3D::IndexedVertexBlend( TRUE );
			} //if..else..
		}
		else
		{	// non-vertex blending
			static Matrix4	matWorld;

			if( m_pAnimation )
			{		
				SMesh*	pAnimation = (SMesh*) m_pAnimation->GetTMAnimation( i );
							
				if( pAnimation && pAnimation->bAnimation )
				{	// TM 애니메이션.
					int		nMaxFrame = 0;
					int		nStart;
					int		nEnd;

					if( m_pAction )
					{	// 액션 테이블이 존재할 경우..
						nMaxFrame = m_pAction->nFrames;
						Assert( nMaxFrame > 0 );

						nStart	= m_pAction->nStart;
						nEnd	= m_pAction->nEnd;
					}
					else
					{	// 일반 애니메이션..
						nStart	= 0;
						nEnd	= nMaxFrame - 1;
					} //if..else..

					Assert( nStart >= 0 );
					Assert( nEnd >= 0 );

					int		nFrame = nStart + ( dwTick * m_nFrameRate / 1000 ) % ( nEnd - nStart + 1 );

					Assert( nFrame >= 0 );
					Assert( nFrame < (int)m_pAnimation->GetMaxFrame() );

					matWorld.SetWorld( pAnimation->apPos[nFrame], pAnimation->apRot[nFrame] );
				}
				else
				{	// 정지.
					matWorld = pMesh->m_matTM;
				} //if..else..
			}
			else
			{
				matWorld = pMesh->m_matTM;
			} //if..else..

			matWorld *= m_matWorld;

			g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
		} //if..else..

		pMesh->m_dwColor		= m_dwColor;
		pMesh->m_bUseTexture	= m_bUseTexture;

		pMesh->Render();

		if( pMesh->m_bVertexBlend )
		{	// 버텍스 블랜딩..
			D3D::VertexBlend( D3DVBF_DISABLE );
			D3D::IndexedVertexBlend( FALSE );
			D3D::SetSoftVertexProcessing( FALSE );
		} //if
	} //for

	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER,	D3DTEXF_NONE );
//	RenderEdge();
} //CW3DModel::Render

/**
 * 테두리 그리기 
 */
void CW3DModel::RenderEdge()
{
	D3D::Wireframe( TRUE );
	D3D::Lighting( FALSE );
	D3D::FogEnable( FALSE );
	
	Vector3		av[36];
	av[0] = m_AABB.vP[2];		av[1] = m_AABB.vP[1];		av[2] = m_AABB.vP[0];
	av[3] = m_AABB.vP[1];		av[4] = m_AABB.vP[2];		av[5] = m_AABB.vP[3];

	av[6] = m_AABB.vP[4];		av[7] = m_AABB.vP[5];		av[8] = m_AABB.vP[6];
	av[9] = m_AABB.vP[7];		av[10] = m_AABB.vP[6];		av[11] = m_AABB.vP[5];

	av[12] = m_AABB.vP[0];		av[13] = m_AABB.vP[4];		av[14] = m_AABB.vP[6];
	av[15] = m_AABB.vP[2];		av[16] = m_AABB.vP[0];		av[17] = m_AABB.vP[6];

	av[18] = m_AABB.vP[7];		av[19] = m_AABB.vP[5];		av[20] = m_AABB.vP[1];
	av[21] = m_AABB.vP[7];		av[22] = m_AABB.vP[1];		av[23] = m_AABB.vP[3];

	av[24] = m_AABB.vP[5];		av[25] = m_AABB.vP[4];		av[26] = m_AABB.vP[0];
	av[27] = m_AABB.vP[5];		av[28] = m_AABB.vP[0];		av[29] = m_AABB.vP[1];
	
	av[30] = m_AABB.vP[2];		av[31] = m_AABB.vP[6];		av[32] = m_AABB.vP[7];
	av[33] = m_AABB.vP[3];		av[34] = m_AABB.vP[2];		av[35] = m_AABB.vP[7];

	g_pd3dDevice->SetTexture( 0, NULL );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&m_matWorld );

	g_pd3dDevice->SetVertexShader( D3DFVF_XYZ );
	g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 12, av, sizeof( Vector3 ) );
	
	D3D::Wireframe( FALSE );
} //CW3DModel::RenderEdge

/**
 * 버텍스 블렌딩 월드 설정.
 */
void CW3DModel::SetVertexBlend( const DWORD dwTick )
{
	if( !m_pAction || !m_pAnimation )
	{
		static Matrix4	matWorld;
		matWorld.SetIdentity();
		g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
		D3D::VertexBlend( D3DVBF_DISABLE );
		D3D::IndexedVertexBlend( FALSE );
		return ;
	} //if

	int		nStart;
	int		nEnd;

	nStart	= m_pAction->nStart;
	nEnd	= m_pAction->nEnd;

	Assert( nStart >= 0 );
	Assert( nEnd >= 0 );

	if( nStart > nEnd )
	{
		static Matrix4	matWorld;
		matWorld.SetIdentity();
		g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
		D3D::VertexBlend( D3DVBF_DISABLE );
		D3D::IndexedVertexBlend( FALSE );
		return ;
	} //if	

	D3D::VertexBlend( D3DVBF_0WEIGHTS );
	D3D::IndexedVertexBlend( TRUE );

	int		nFrame = nStart + ( dwTick * m_nFrameRate / 1000 ) % ( nEnd - nStart + 1 );
	int		numBones = m_pAnimation->GetNumBone();
	
	static SBone*	pBone = NULL;
	static Matrix4	matBone;
	static Matrix4	matWorld;

	matBone.SetIdentity();
	pBone = NULL;

	for( int i = 0 ; i < numBones ; i++ )
	{
		pBone = (SBone*)m_pAnimation->GetBone(i);

		Vector3		vPos = pBone->apPos[nFrame];
		Quaternion	qRot = pBone->apRot[nFrame];

		matBone.SetWorld( vPos, qRot );

		matWorld = pBone->matInverse;
		matWorld *= matBone;
		matWorld *= m_matWorld;
		g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), (D3DMATRIX*)&matWorld );
	} //for
} //CW3DModel::SetVertexBlend

void CW3DModel::SetInterpolateBlend( DWORD		dwPrevTicks,		// Prev Animation Frame Ticks
									 DWORD		dwNextTicks,		// Next Animation Frame Ticks
									 SAction*	pInterAction,		// Interpolation Animation
									 float		fInterTime )		// Interpolation Time
{
	if( !pInterAction )
	{
		SetVertexBlend( dwPrevTicks );
		return;
	} // if

	IW3DAnimation* pInterAnimation = pInterAction->pAnimation;
	if( !pInterAnimation )
	{
		SetVertexBlend( dwPrevTicks );
	} // if

	int		nStart, nInterStart;
	int		nEnd, nInterEnd;
	static Matrix4	matWorld;

	nStart	= m_pAction->nStart;
	nEnd	= m_pAction->nEnd;
	Assert( nStart >= 0 );
	Assert( nEnd >= 0 );

	if( nStart > nEnd )
	{
		matWorld.SetIdentity();
		g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
		D3D::VertexBlend( D3DVBF_DISABLE );
		D3D::IndexedVertexBlend( FALSE );		
		return ;
	} //if	

	nInterStart = pInterAction->nStart;
	nInterEnd	= pInterAction->nEnd;
	Assert( nInterStart >= 0 );
	Assert( nInterEnd >= 0 );
	
	if( nInterStart > nInterEnd )
	{
		matWorld.SetIdentity();
		g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
		D3D::VertexBlend( D3DVBF_DISABLE );
		D3D::IndexedVertexBlend( FALSE );
		return ;
	} // if

	int nFrame			= nStart + ( dwPrevTicks * m_nFrameRate / 1000 ) % ( nEnd - nStart + 1 );
	int nInterFrame		= nInterStart + ( dwNextTicks * m_nFrameRate / 1000 ) % ( nInterEnd - nInterStart + 1 );
	int numBones		= m_pAnimation->GetNumBone();
	int numInterBones	= pInterAnimation->GetNumBone();
 
	if( numBones != numInterBones )
	{
		Assert( 0 );
		
		g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
		D3D::VertexBlend( D3DVBF_DISABLE );
		D3D::IndexedVertexBlend( FALSE );
		return ;
	} // if

//	D3D::SetSoftVertexProcessing( TRUE );

	D3D::VertexBlend( D3DVBF_0WEIGHTS );
	D3D::IndexedVertexBlend( TRUE );
	
	static SBone*	pBone = NULL;
	static SBone*	pInterBone = NULL;
	static Matrix4	matBone;

	matBone.SetIdentity();
	pBone		= NULL;	
	pInterBone	= NULL;

	for( int i = 0 ; i < numBones ; i++ )
	{
		pBone		= ( SBone* )m_pAnimation->GetBone( i );
		pInterBone	= ( SBone* )pInterAnimation->GetBone( i );

		Vector3		vPos		= pBone->apPos[nFrame];
		Quaternion	qRot		= pBone->apRot[nFrame];

		Vector3		vInterPos	= pInterBone->apPos[nInterFrame];
		Quaternion	qInterRot	= pInterBone->apRot[nInterFrame];

		vPos = vPos.Interpolate( vInterPos, fInterTime );
		qRot = qRot.Interpolate( qInterRot, fInterTime );

		matBone.SetWorld( vPos, qRot );

		matWorld = pBone->matInverse;
		matWorld *= matBone;
		matWorld *= m_matWorld;

		g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), (D3DMATRIX*)&matWorld );
	} //for		

//	D3D::SetSoftVertexProcessing( FALSE );
} // CModel::SetInterpolateBlend

/**
 * 동작은 혼합한다.
 */
BOOL CW3DModel::SetMultiActionVertexBlend( 
	const SAction& sBlendAction, 
	const DWORD    dwTick,
	const DWORD    dwBlendActTick )
{
	IW3DAnimation*	pAnimation = sBlendAction.pAnimation;
	if( !pAnimation )
	{
		SetVertexBlend( dwTick );
		return FALSE;
	} //if

	int		nStart;
	int		nEnd;

	nStart	= m_pAction->nStart;
	nEnd	= m_pAction->nEnd;

	Assert( nStart >= 0 );
	Assert( nEnd >= 0 );

	if( nStart > nEnd )
	{
		static Matrix4	matWorld;
		matWorld.SetIdentity();
		g_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DMATRIX*)&matWorld );
		D3D::VertexBlend( D3DVBF_DISABLE );
		D3D::IndexedVertexBlend( FALSE );
		return FALSE;
	} //if	

	D3D::VertexBlend( D3DVBF_0WEIGHTS );
	D3D::IndexedVertexBlend( TRUE );

	int		nFrame			= nStart + ( dwTick * m_nFrameRate / 1000 ) % ( nEnd - nStart + 1 );
	int		nBlendActFrame	= sBlendAction.nStart + ( dwBlendActTick * m_nFrameRate / 1000 );

	if( sBlendAction.nFrames < nBlendActFrame )
	{
		return FALSE;
	} //if

	int		numBones		= m_pAnimation->GetNumBone();
	
	SBone*			pBone = NULL;
	Matrix4			matBone;
	Matrix4			matWorld;

	SBone*			pBlendBone = NULL;
	Matrix4			matBlendBone;
	Vector3			vPos;
	Quaternion		qRot;

	matBone.SetIdentity();
	matBlendBone.SetIdentity();
	pBone = NULL;

	for( int i = 0 ; i < numBones ; i++ )
	{
		pBone	   = (SBone*)m_pAnimation->GetBone( i );
		pBlendBone = (SBone*)pAnimation->GetBone( i );

		vPos = pBone->apPos[nFrame];
		qRot = pBone->apRot[nFrame];
		matBone.SetWorld( vPos, qRot );

		vPos = pBlendBone->apPos[nBlendActFrame];
		qRot = pBlendBone->apRot[nBlendActFrame];
		matBlendBone.SetWorld( vPos, qRot );

		matWorld = pBlendBone->matInverse;//pBone->matInverse;
		//matWorld *= matBone;
		matWorld *= matBlendBone;
		matWorld *= m_matWorld;
		g_pd3dDevice->SetTransform( D3DTS_WORLDMATRIX( i ), (D3DMATRIX*)&matWorld );
	} //for

	return TRUE;
} //CW3DModel::SetMultiActionVertexBlend

/**
 * 렌더링 상태를 설정한다.
 */
void CW3DModel::SetRenderState( const BOOL bLighting, const BOOL bAlpha )
{
	Assert( g_pd3dDevice );

	D3D::AlphaBlend( TRUE );
	D3D::Wireframe( FALSE );

	if( !m_bShadow )
	{
		D3D::ZEnable( TRUE );
		D3D::ZWriteEnable( TRUE );

		// Texture 관련 세팅
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,		D3DTADDRESS_WRAP );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,		D3DTADDRESS_WRAP );

		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER,	D3DTEXF_LINEAR );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER,	D3DTEXF_LINEAR );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER,	D3DTEXF_LINEAR );

		float fBias = 0.01F;
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPMAPLODBIAS, *((LPDWORD) (&fBias)) );
						
		if( bLighting )
		{
			D3D::Lighting( TRUE );
			D3D::FogEnable( TRUE );
			D3D::AlphaTest( bAlpha, 0x40 );
		}
		else
		{
			D3D::Lighting( FALSE );
			D3D::FogEnable( FALSE );
			D3D::AlphaTest( bAlpha, 0x40 );
		} //if..else..
	} //if

	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,	D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,	D3DBLEND_INVSRCALPHA );
} //CW3DModel::SetRenderState

/**
 * 모델 피킹.
 */
BOOL CW3DModel::Pick(
	const Vector3&	vOrig,		///< [IN] 반직선 시작점
	const Vector3&	vDir,		///< [IN] 반직선 방향
	float*			pfDistance	///< [OUT] 피킹 거리
	)
{
	if( !m_apMesh )	return FALSE;

	return m_AABB.Pick( m_matWorld, vOrig, vDir );

	BOOL	bPick        = FALSE;
	float	fMinDistance = 1000.0F;
	static Matrix4 matWorld;

	for( int i = 0 ; i < m_numMeshes ; i++ )
	{
		matWorld = m_apMesh[i].m_matTM * m_matWorld;
		float		fDistance;

		if( m_apMesh[i].Pick( matWorld, vOrig, vDir, &fDistance ) )
		{
			bPick = TRUE;
			fMinDistance = MIN( fMinDistance, fDistance );
		} //if
	} //for

	if( pfDistance && bPick )
	{
		*pfDistance = fMinDistance;
	} //if

	return bPick;
} //CW3DModel::Pick

/**
 *
 */
DWORD CW3DModel::GetActionTicks()
{
	if( !m_pAnimation || !m_pAction )	   return 0;
	if( m_pAnimation->GetMaxFrame() <= 0 ) return 0;
	
	int		nStart;
	int		nEnd;
	
	int nTotalFrames = m_pAction->nFrames;

	nStart	= m_pAction->nStart;
	nEnd	= m_pAction->nEnd;

	Assert( nStart >= 0 );
	Assert( nEnd >= 0  );

	if( nStart > nEnd )
	{
		nStart = nEnd;
	} //if

	return ( nEnd - nStart + 1 ) * 1000 / m_nFrameRate;
} //CW3DModel::GetActionTicks

/**
 *
 */
void CW3DModel::SetAction( SAction* pAction )
{
	if( m_pAnimation )
	{
		m_pPrevAnimation = m_pAnimation;
	}
	else
	{
		m_pPrevAnimation = NULL;
	} //if

	m_pAction	 = pAction;

	if( m_pAction )
	{
		m_pAnimation = pAction->pAnimation;
	}
	else
	{
		m_pAnimation = NULL;
	} //if
} //CW3DModel::SetAction

/**
 * Bone의 애니메이션 TM을 얻는다.
 */
Matrix4 CW3DModel::GetBoneMatrix( const char* szName, const DWORD dwTick )
{
	static Matrix4	matBone;
	if( !m_pAction || !m_pAnimation )
	{
		matBone.SetIdentity();
		return matBone;
	} //if

	int		nStart, nEnd;

	nStart	= m_pAction->nStart;
	nEnd	= m_pAction->nEnd;

	Assert( nStart >= 0 );
	Assert( nEnd >= 0 );
	
	if( nStart > nEnd )
	{
		matBone.SetIdentity();
		return matBone;
	} //if

	int		nFrame = nStart + ( dwTick * m_nFrameRate / 1000 ) % ( nEnd - nStart + 1 );
	SBone*	pBone = NULL;
	static Vector3		vPos;
	static Quaternion	qRot;

	for( int i = 0 ; i < (int) m_pAnimation->GetNumBone() ; i++ )
	{
		pBone = (SBone*)m_pAnimation->GetBone(i);

		if( pBone && !strcmp( pBone->szName, szName ) )
		{
			vPos = pBone->apPos[nFrame];
			qRot = pBone->apRot[nFrame];

			matBone.SetWorld( vPos, qRot );
			return matBone;
		} //if
	} //for

	matBone.SetIdentity();
	return matBone;
} //CW3DModel::GetBoneMatrix

/**
 * 애니메이션 TM을 얻는다.
 */
Matrix4 CW3DModel::GetTMMatrix( const DWORD dwTick )
{
	static Matrix4	matWorld;
	matWorld.SetIdentity();

	if( !m_pAnimation	|| 
		!m_pAction		||
		m_pAction->nFrames <= 0 
		)	
	{
		return matWorld;
	} //if

	int		nStart;
	int		nEnd;

	nStart	= m_pAction->nStart;
	nEnd	= m_pAction->nEnd;
				
	Assert( nStart >= 0	);
	Assert( nEnd >= 0 );

	int		nFrame = nStart + ( dwTick * m_nFrameRate / 1000 ) % ( nEnd - nStart + 1 );

	SMesh*		pMesh = (SMesh*) m_pAnimation->GetTMAnimation( 0 );

	if( !pMesh )	return matWorld;

	Assert( nFrame >= 0 );
	Assert( nFrame < (int)m_pAnimation->GetMaxFrame() );
	
	matWorld.SetWorld( pMesh->apPos[nFrame], pMesh->apRot[nFrame] );
	return matWorld;
} //CW3DModel::GetTMMatrix

/**
 * 모델의 중심을 구한다.
 */
void CW3DModel::CalculatePivot()
{
	if( !m_apMesh )	return ;
	int		numVertices = 0;

	m_vPivot = Vector3( 0.0F, 0.0F, 0.0F );
	CMesh*	pMesh = NULL;

	for( int iMesh = 0 ; iMesh < m_numMeshes ; iMesh++ )
	{
		pMesh = &m_apMesh[iMesh];

		if( !pMesh )
		{
			continue;
		} //if

		for( int iVertex = 0 ; iVertex < pMesh->m_nNumVertices ; iVertex++ )
		{
			m_vPivot += pMesh->m_apVertices[iVertex];
			numVertices++;
		} //for
	} //for

	m_vPivot = Vector3( 0.0F, 0.0F, 0.0F );

	if( numVertices > 0 )
	{
		m_vPivot /= numVertices;
	} //if
} //CW3DModel::CalculatePivot

/**
 * 모델의 반지름을 구한다.
 */
void CW3DModel::CalculateRadius()
{
	if( !m_apMesh )	return ;

	if( m_vPivot == Vector3( 0.0F, 0.0F, 0.0F ) )
	{
		CalculatePivot();
	} //if

	m_fRadius = 0.0F;
	static Vector3 vPivot;
	vPivot = m_vPivot;
	vPivot.y = 0.0F;

	CMesh*	pMesh = NULL;

	for( int iMesh = 0 ; iMesh < m_numMeshes ; iMesh++ )
	{
		pMesh = &m_apMesh[iMesh];

		if( !pMesh )
		{
			continue;
		} //if

		for( int iVertex = 0 ; iVertex < pMesh->m_nNumVertices ; iVertex++ )
		{
			static Vector3 v;
			v = pMesh->m_apVertices[iVertex];
		
			float	fLength = ( v - vPivot ).Length();

			if( fLength > m_fRadius )
			{
				m_fRadius = fLength;
			} //if
		} //for
	} //for
} //CW3DModel::CalculateRadius

/**
 * Check Collision Occurs Point Into Box
 */
BOOL CW3DModel::CollisionPoint( const Vector3& vPos )
{
	return m_AABB.CheckPoint( m_matWorld, vPos + Vector3( 0.0F, 0.0F, 0.1F ) );
} //CW3DModel::CollisionPoint

/**
 * 모델의 중심 좌표를 얻는다.
 */
const Vector3& CW3DModel::GetPivot()
{
	return m_vPivot;
} //CW3DModel::GetPivot

/**
 * 모델의 반지름을 얻는다.
 */
float CW3DModel::GetRadius()
{
	return m_fRadius;
} //CW3DModel::GetRadius

/**
 * 모델의 사이즈를 구한다. 
 */
Vector3 CW3DModel::GetSize()
{
	return m_vSize;
} //CW3DModel::GetSize

/**
 * 타임을 가져온다.
 */
DWORD CW3DModel::GetTime()
{
	return m_dwTime;
} //CW3DModel::GetTime

/**
 * XY 최대 좌표를 얻는다.
 */
Vector3	CW3DModel::GetXYMax( const Matrix4& matLight )
{
	Vector3		vMax = Vector3( 0.0F, 0.0F, 0.0F );

	for( int i = 0 ; i < m_numMeshes ; i++ )
	{
		CMesh*	pMesh = &m_apMesh[i];
		float	fX = 0.0F;
		float	fY = 0.0F;

		if( pMesh )
		{
			Vector3		vNodeVertex = pMesh->GetXYMax( matLight );
			fX = vNodeVertex.x;
			fY = vNodeVertex.y;
		} //if

		if( vMax.x < fX )
		{
			vMax.x = fX;
		} //if

		if( vMax.y < fY )
		{
			vMax.y = fY;
		} //if
	} //for

	return vMax;
} //Vector3	CW3DModel::GetXYMax

/**
 * 모델 이름을 반환한다.
 */
const char* CW3DModel::GetModelName()
{
	return m_sName.c_str();
} // 모델 이름을 반환한다.

