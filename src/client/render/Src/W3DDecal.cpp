/**
 * 데칼 
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-23 11:10a $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /Render/Src/W3DDecal.cpp $
 * 
 * 3     03-12-23 11:10a Paran
 * 
 * 2     03-11-10 5:33p Kjmgo
 * 
 * 1     03-10-15 4:10p Kjmgo
 * 
 * <HR>
 *
 * @file	IW3DDecal.cpp
 */

#include "Render_Export.h"
#include "VertexBuffer.h"
#include "Texture.h"

#define MAX_DECAL_VERTICES	256
#define DECAL_EPSILON		0.25F


/**
 *
 */
struct Vector4 
{
	float x, y, z, w;

	Vector4()
	{
		x = y = z = 0.0F;
		w = 1.0F;
	} //Vector4

	Vector4( float a, float b, float c, float d )
	{
		x = a;
		y = b;
		z = c;
		w = d;
	} //Vector4
}; //struct Vector4 

/**
 *
 */
struct ColorRGBA
{
	float		fRed;
	float		fGreen;
	float		fBlue;
	float		fAlpha;
	
	ColorRGBA() {}
	
	ColorRGBA(float r, float g, float b, float a)
	{
		fRed	= r;
		fGreen	= g;
		fBlue	= b;
		fAlpha	= a;
	} //ColorRGBA
}; //struct ColorRGBA

/**
 *
 */
struct TriangleIndex
{
	DWORD	dwIndex[3];
};

/**
 *
 */
class CW3DDecal : public IW3DDecal
{
public:
	MEMORY_UNDER_MGR( CW3DDecal );

	/**
	 * 생성자
	 */
	CW3DDecal();

	/**
	 * 소멸자
	 */
	~CW3DDecal();

	/**
	 *	데칼 초기화
	 */
	void Init( IW3DTerrain* pTerrain );

	/**
	 *
	 */
	void Init(	const Vector3& vCenter, 
				const Vector3& vNormal, 
				const Vector3& vTangent, 
				float fWidth, 
				float fHeight, 
				float fDepth 
				);

	/**
	 *
	 */
	BOOL AddPolygon( WORD wVertexCount, const Vector3* pVertex, const Vector3* pNormal );
	
	/**
	 * 데칼 스크립트 로드
	 */
	BOOL Load( const char* szFile, int nRepeat = 1 );

	/**
	 * 데칼 초기화, 내부 변수를 초기화 시킨다. 
	 */
	void DecalInit();

	/**
	 * 그린다.
	 */
	void Draw();

	/**
	 * 업데이트 
	 * 끝났는지의 여부, 자동적으로 Disappear을 호출하고, 이것마저 끝난 후에
	 * FALSE를 리턴... ( 무한 반복이면, 계속 True를 리턴 )
	 *
	 */
	BOOL Update( const Vector3& vPos, DWORD dwTick );

	/**
	 *	 천천히 사라지면서( 알파 블렌딩 ) 끝낸다. ( 자체 소멸?;;; )
	 */
	void Disappear();

private:

	/**
	 *
	 */
	struct DecalVertex
	{
		Vector3 p;
		float	u, v;
	}; //struct DecalVertex


	inline float DotProduct(const Vector4& p, const Vector3& q)
	{
		return (p.x * q.x + p.y * q.y + p.z * q.z + p.w);
	} //DotProduct


	Vector3			m_vDecalCenter;				///< 중심
	Vector3			m_vDecalNormal;			///< 법선
		
	Vector4			m_vLeftPlane;				///< 왼쪽 
	Vector4			m_vRightPlane;				///< 오른쪽
	Vector4			m_vBottomPlane;				///< 아래쪽
	Vector4			m_vTopPlane;				///< 위쪽
	Vector4			m_vFrontPlane;				///< 전단면
	Vector4			m_vBackPlane;				///< 후단면

	WORD			m_wDecalVertexCount;		///< 버텍스 갯수
	WORD			m_wDecalTriangleCount;		///< 폴리곤 갯수

	Vector3			m_aVertexArray[MAX_DECAL_VERTICES];
	Vector2			m_aTexcoordArray[MAX_DECAL_VERTICES];
	ColorRGBA		m_aColorArray[MAX_DECAL_VERTICES];
	TriangleIndex	m_aTriangleArray[MAX_DECAL_VERTICES];

	/////////////////////////////////////////
	// 변하지 않는 것들
	/////////////////////////////////////////
	IW3DTerrain*		m_pTerrain;
	BOOL				m_bAnimation;		///< 텍스처 애니메이션의 여부
	int					m_nRow;				///< 가로 ( 텍스처 배분 )
	int					m_nCol;				///< 세로 ( 텍스처 배분 ) 
	int					m_nTotalFrame;		///< 총 텍스처 애니메이션 프레임 수
	int					m_nFrameRate;		///< 프레임 간의 속도
	int					m_nRepeat;			///< 반복 횟수 ( 0일때 무한 반복 )

	/////////////////////////////////////////
	// 변하는 것들 ( 초기화 해야 하는 것들 )
	/////////////////////////////////////////

	int					m_nFrame;			///< 현재 프레임
	Vector3				m_vPos;				///< 데칼의 위치
	
	int					m_nVertex;			///< 버텍스 갯수
	int					m_nFace;			///< 폴리곤 갯수 ( 페이스 갯수 )
	DecalVertex*		m_apVertex;			///< 버텍스
	
private:

	/////////////////////////////////////////
	// 내부 함수들 
	/////////////////////////////////////////


}; //class CW3DDecal

IMPLEMENT_COMPONENT( CW3DDecal, Decal );

/**
 *
 */
CW3DDecal::CW3DDecal()
:		m_pTerrain			( NULL ),
		m_bAnimation		( FALSE ),
		m_nRow				( 0 ),
		m_nCol				( 0 ),
		m_nTotalFrame		( 0 ),
		m_nFrameRate		( 0 ),
		m_nFrame			( 0 ),
		m_nRepeat			( 1 )
{

} //CW3DDecal::CW3DDecal

/**
 *
 */
CW3DDecal::~CW3DDecal()
{

} //CW3DDecal::~CW3DDecal

/**
 *
 */
void CW3DDecal::Init( IW3DTerrain* pTerrain )
{

} //CW3DDecal::Init

/**
 *
 */
void CW3DDecal::Init(
				const Vector3& vCenter, 
				const Vector3& vNormal, 
				const Vector3& vTangent, 
				float fWidth, 
				float fHeight, 
				float fDepth 
			)
{
	m_vDecalCenter = vCenter;
	m_vDecalNormal = vNormal;
	
	Vector3 vBivNormal = vNormal.CrossProduct( vTangent );
	
	// Calculate boundary planes
	float d = vCenter.DotProduct( vTangent );

	m_vLeftPlane	= Vector4( vTangent.x, vTangent.y, vTangent.z, fWidth * 0.5F - d		);
	m_vRightPlane	= Vector4( -vTangent.x, -vTangent.y, -vTangent.z, fWidth * 0.5F + d		);
	
	d = vCenter.DotProduct( vBivNormal );

	m_vBottomPlane	= Vector4( vBivNormal.x, vBivNormal.y, vBivNormal.z, fHeight * 0.5F - d		);
	m_vTopPlane		= Vector4( -vBivNormal.x, -vBivNormal.y, -vBivNormal.z, fHeight * 0.5F + d	);
	
	d = vCenter.DotProduct( vNormal );

	m_vFrontPlane	= Vector4( -vNormal.x, -vNormal.y, -vNormal.z, fDepth + d	);
	m_vBackPlane	= Vector4( vNormal.x, vNormal.y, vNormal.z, fDepth - d		);
	
	// Begin with empty mesh
	m_wDecalVertexCount		= 0;
	m_wDecalTriangleCount	= 0;
	
	// Add this point, determine which surfaces may be affected by this decal
	// and call ClipMesh() for each one.
	
	// Assign texture mapping coordinates
	float one_over_w = 1.0F / fWidth;
	float one_over_h = 1.0F / fHeight;
	/*
	for (long a = 0; a < m_wDecalVertexCount; a++)
	{
		vector3 v = vertexArray[a] - center;
		float s = DotProduct(v, vTangent) * one_over_w + 0.5F;
		float t = DotProduct(v, vBivNormal) * one_over_h + 0.5F;
		texcoordArray[a] = vector2(s, t);
	}*/
} //CW3DDecal::Init

/**
 *
 */
BOOL CW3DDecal::AddPolygon( WORD wVertexCount, const Vector3* pVertex, const Vector3* pNormal )
{
	WORD wCount = m_wDecalVertexCount;

	if( wCount + wVertexCount >= MAX_DECAL_VERTICES )
	{
		return FALSE;
	} //if
	
	// Add polygon as a triangle fan
	TriangleIndex *pTriangle = ( m_aTriangleArray + m_wDecalTriangleCount );
	m_wDecalTriangleCount += wVertexCount - 2;

	for( int i = 2 ; i < wVertexCount ; i++)
	{
		pTriangle->dwIndex[0] = wCount;
		pTriangle->dwIndex[1] = ( wCount + i - 1 );
		pTriangle->dwIndex[2] = ( wCount + i );
		pTriangle++;
	} //for
	
	// Assign vertex colors
	float f = 1.0F / (1.0F - DECAL_EPSILON );

	for( int b = 0 ; b < wVertexCount ; b++ )
	{
		m_aVertexArray[wCount] = pVertex[b];

		const Vector3& n = pNormal[b];

		float fAlpha = ( m_vDecalNormal.DotProduct( n ) / n.Length() - DECAL_EPSILON ) * f;
		
		m_aColorArray[wCount] = ColorRGBA(1.0F, 1.0F, 1.0F, (fAlpha > 0.0F) ? fAlpha : 0.0F);
		wCount++;
	}
	 //for

	m_wDecalVertexCount = wCount;
	return TRUE;

} //CW3DDecal::AddPolygon

/**
 *
 */
BOOL CW3DDecal::Load( const char* szFile, int nRepeat )
{
	return TRUE;
} //CW3DDecal::Load

/**
 * 데칼 초기화, 내부 변수를 초기화 시킨다. 
 */
void CW3DDecal::DecalInit()
{
	
} //CW3DDecal::DecalInit

/**
 * 그린다.
 */
void CW3DDecal::Draw()
{	

} //CW3DDecal::Draw

/**
 * 업데이트 
 * 끝났는지의 여부, 자동적으로 Disappear을 호출하고, 이것마저 끝난 후에
 * FALSE를 리턴... ( 무한 반복이면, 계속 True를 리턴 )
 *
 */
BOOL CW3DDecal::Update( const Vector3& vPos, DWORD dwTick )
{
	return TRUE;
} //CW3DDecal::Update

/**
 *	 천천히 사라지면서( 알파 블렌딩 ) 끝낸다. ( 자체 소멸?;;; )
 */
void CW3DDecal::Disappear()
{

} //CW3DDecal::Disappear