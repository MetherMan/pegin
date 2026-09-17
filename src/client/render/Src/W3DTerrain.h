/**
 * 지형 모듈 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 04-01-13 9:43p $
 * @version	$Revision: 5 $
 *
 * <HR>
 * $Log: /Render/Src/W3DTerrain.h $
 * 
 * 5     04-01-13 9:43p Kjmgo
 * 퀄리티 설정 적용.
 * 
 * 4     04-01-08 8:50a Kjmgo
 * 코드 정리
 * 
 * 3     03-12-23 5:24p Kjmgo
 * 멀티 패스를 옵션으로 조절 가능하도록 추가
 * 
 * 2     03-12-10 12:19a Kjmgo
 * Terrain 에서 Picking 하는 것 분리
 * 
 * 1     03-12-09 10:07p Kjmgo
 * 
 * <HR>
 *
 * @file	W3DTerrain.h
 */

#ifndef __CW3DTerrain_Header__
#define __CW3DTerrain_Header__

#include "Render_Export.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Block.h"
#include <string>


using std::string;

#define MAX_LEVEL 3
#define MAX_LAYER 2

/**
 * 지형 클래스.
 * @ingroup		Render
 */
class CW3DTerrain : public IW3DTerrain
{
public:
	UNDER_MEMORY_MGR( CW3DTerrain )

	CW3DTerrain();
	~CW3DTerrain();

	BOOL	Init( int nSize, const char* szTextureName, BOOL bLight );
	BOOL	Pick( const float fHeight, const Vector3& vOrig, const Vector3& vDir, Vector3* pvPick );
	BOOL	LoadTexture( const char* szTexture );

	void	SetQuality( int nQuality )
	{
		m_nQuality = nQuality;
	} //SetQuality

	int		GetQuality()
	{
		return m_nQuality;
	} //GetQuality

	void	Render( DWORD dwTick, BOOL bUseTexture );
	void	Update( DWORD dwTick, BOOL bUseTexture );
	void	DrawWater( DWORD dwTick );
	void	SetHeight( int nX, int nY, float fHeight );
	void	SetTexture( int nX, int nY, int nTexture );
	void	SetLevel( int nLevel );
	void	CalculateNormals();
	void	UseWireframe( BOOL bUse );
	void	InsertShadow( IW3DShadow* pShadow, const Vector3 vPos );
	void	AddPass( const SMultiPass* pPass );		
	
	float	GetHeight( float fX, float fY );
	
	int		GetLevel()	{ return m_nLevel; }
	int		GetTexture( int nX, int nY );
 	int		GetSize();

	float*	GetHeightMap();
	float*	GetObjectHeightMap();
	BYTE*	GetAttributeMap();
	BYTE*	GetTextureMap();
	DWORD*	GetColorMap();

	BYTE	GetAttribute( float fX, float fY );

	/**
	 *
	 */
	void	SetUseObjectHeight( BOOL bUse )
	{
		m_bUseObjectHeight = bUse;
	} //SetUseObjectHeight

	/**
	 *
	 */
	void	SetUseLevel( BOOL bUse )
	{
		m_bUseLevel = bUse;
	} //SetUseLevel

	/**
	 *
	 */
	void	SetLayer( int nLayer )
	{
		m_nLayer = nLayer;
	} //SetLayer

	/**
	 *
	 */
	void	SetPosition( const Vector3& vPos )
	{
		m_vPos = vPos;
	} //SetPosition

	/**
	 *
	 */
	float	GetWaterHeight()
	{
		return m_fWaterHeight;
	} //GetWaterHeight

	/**
	 *
	 */
	void	ShowCloud( BOOL bShow )
	{
		m_bShowCloud = bShow;
	} //ShowCloud

	/**
	 *
	 */
	void	SetLevelOne( BOOL bUse )
	{
		m_bUseLevelOne = bUse;
	} //SetLevelOne

	/**
	 *
	 */
	void	CompulsionUpdate( BOOL bUse )
	{
		m_bCompulsionUpdate = bUse;
	} //CompulsionUpdate

	/**
	 *
	 */
	void	SetMultiPass( BOOL bUse )
	{
		m_bMultiPass = bUse;
	} //SetMultiPass
	
private:
	/**
	 *
	 */
	struct TVERTEX
	{
		float tu, tv;
	}; //struct TVERTEX

	/**
	 *
	 */
	struct VERTEX
	{
		enum	{ FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2 };
		Vector3	p;					///< 위치
		Vector3	n;					///< 노말
		DWORD	color;				///< 색
		TVERTEX tv[MAX_LAYER];		///< 텍스처 위치
	}; //struct VERTEX

	typedef		std::list<IW3DBlock*>	BlockList;
	BlockList			m_listBlock;					///< 블럭 리스트
	CBlockList*			m_pBlockList;					///< 블럭 리스트 관리자
	Vector3				m_vPos;							///< 위치
	int					m_numFaces;						///< 폴리곤 수
	int					m_nSize;						///< 전체 크기. ( m 단위 )
	int					m_nObjectSize;					///<
	int					m_nLevel;						///< Level
	int					m_nLayer;						///< Layer
	int					m_nQuality;						///< 퀄리티
	float				m_fAddHeight;					///< 부가 가치 높이
	float				m_fWaterHeight;					///< 물의 높이
	float*				m_pfHeight;						///< 높이맵
	float*				m_pfEffectUV;					///< 이펙트용 UV
	float*				m_pfObjectHeight[MAX_LEVEL];	///< 오브젝트 맵
	BYTE*				m_pbTexture;					///< 텍스처맵
	BYTE*				m_pbAttribute[MAX_LEVEL];		///< 텍스처맵
	DWORD*				m_pdwColor;						///< 컬러맵
	Vector3*			m_pvNormal;						///< 법선맵

	Vector3				m_vOrig;						///< 피킹용 반직선 시작점
	Vector3				m_vDir;							///< 피킹용 반직선 방향
	Vector3				m_vPick;						///< 피킹 좌표
	float				m_fPickDistance;				///< 내부적으로 지니고 있는 피킹 거리

	IW3DWater*			m_pWater;						///< 물 

	DWORD				m_dwTick;						///< 누적 틱

	CVertexBuffer*		m_pVB;							///< 버텍스 버퍼.
	CTexture*			m_pTexture;						///< 텍스처.
	CTexture*			m_pTextureForEffect;			///< 텍스처.

	BOOL				m_bUseWireframe;				///< 와이어 프레임 사용 여부.
	BOOL				m_bUseTexture;					///< 텍스처 사용 여부.
	BOOL				m_bUseObjectHeight;				///< 오브젝트 높이 사용여부
	BOOL				m_bUseLevel;					///< 레이어
	BOOL				m_bUseLight;					///< 빛의 사용여부
	BOOL				m_bShowWater;					///< 물을 보이게 할것인가의 여부
	BOOL				m_bShowCloud;					///< 구름이 보이게 할것인가의 여부
	BOOL				m_bUseLevelOne;					///< 1단계만 사용
	BOOL				m_bCompulsionUpdate;			///< 강제적으로 갱신
	BOOL				m_bMultiPass;					///< 멀티 패스 사용여부
	
	/**
	 *
	 */
	void	SetRenderState();

	/**
	 *
	 */
	void	CreateEffectUV();

	/**
	 *
	 */
	void	CalculateNormal( int nX, int nY );

	/**
	 *
	 */
	void	InsertVertexForShadow( IW3DShadow* pShadow, const Vector3 vPos );

	/**
	 *
	 */
	void	Cleanup();

	/**
	 *
	 */
	float	GetObjectHeight( int nX, int nY );

	/**
	 *
	 */
	float	GetHighHeight( int nX, int nY );

	/**
	 *
	 */
	float	GetLowHeight( int nX, int nY );

	/**
	 *
	 */
	BOOL	HasAreaObjectHeight( int nX, int nY );

	/**
	 *
	 */
	BOOL	PickHasObjectHeight( int nX, int nY, Vector3* pv );

	/**
	 *
	 */
	BOOL	PickHasNone( int nX, int nY, Vector3* pv );

	/**
	 *
	 */
	BOOL	PickHasAttribute( int nX, int nY, Vector3* pv, BYTE btType );

	/**
	 *
	 */
	BOOL	PickPolygon( Vector3* pv, int nType );
}; //class CW3DTerrain

#endif //#ifndef __CW3DTerrain_Header__