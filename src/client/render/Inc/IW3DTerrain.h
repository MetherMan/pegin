/**
 * 지형 모듈 인터페이스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	Teammay
 * @date	$Date: 04-01-13 9:43p $
 * @version	$Revision: 34 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DTerrain.h $
 * 
 * 34    04-01-13 9:43p Kjmgo
 * 퀄리티 설정 적용.
 * 
 * 33    04-01-08 9:52a Kjmgo
 * 코드 정리
 * 
 * <HR>
 *
 * @file	IW3DTerrain.h
 */


#ifndef __Wind3D_Render_IW3DTerrain_Header__
#define __Wind3D_Render_IW3DTerrain_Header__

/**
 *
 */
struct SMultiPass
{
	float x;						///< x좌표
	float y;						///< y좌표
	BYTE  btDirection;				///< 방향 ( 8방향 )
	WORD  wSize;					///< 크기 
	char  szTexture[256];			///< 기본 Texture
	char  szAlphaTexture[256];		///< Alpha Texture
}; //struct 

/**
 *
 */
class IW3DBlock : public IComponent
{
public:

	/**
	 *
	 */
	virtual BOOL Init( const SMultiPass* pPass, IW3DTerrain* pTerrain ) = 0;

	/**
	 *
	 */
	virtual void Draw() = 0;

	/**
	 *
	 */
	virtual void SetAlpha( BYTE btAlpha ) = 0;

	/**
	 *
	 */
	virtual void RenderState() = 0;

	/**
	 *
	 */
	virtual Vector3 GetPosition() = 0;

	/**
	 *
	 */
	virtual BOOL	Pick( const int nX, const int nY ) = 0;

	/**
	 *
	 */
	virtual void	Move( const int nX, const int nY ) = 0;

	/**
	 *
	 */
	virtual SMultiPass GetPass() = 0;


}; //class IW3DBlock

/**
 * 지형 인터페이스.
 * @ingroup		Render
 */
class IW3DTerrain : public IComponent
{
public:
	enum
	{
		TEXTURE_SIZE		= 1024,			///< 전체 텍스처 크기.
		TILE_TEXTURE_SIZE	= 64,			///< 한 타일의 텍스처 크기.
		TILE_SIZE			= 4,			///< 한 타일의 크기.
		MAX_VERTICES		= 30000,		///< 할당할 버텍스 갯수.
	}; //enum

	/**
	 * 지형을 초기화한다.
	 *
	 * @param	nSize		[IN] 지형 크기.
	 *
	 * @return	초기화 성공 여부.
	 */
	virtual BOOL	Init( int nSize, const char* szTextureName, BOOL bLight = TRUE ) = 0;

	/**
	 *
	 */
	virtual void	SetQuality( int nQuality ) = 0;

	/**
	 *
	 */
	virtual int		GetQuality() = 0;

	/**
	 *
	 *
	 */
	virtual void	SetMultiPass( BOOL bUse = TRUE ) = 0;

	/**
	 *
	 *
	 */
	virtual void	CompulsionUpdate( BOOL bUse = TRUE ) = 0;

	/**
	 * 지형을 그린다.
	 */
	virtual void	Render( DWORD dwTick, BOOL bUseTexture = TRUE ) = 0;

	/**
	 *
	 */
	virtual void	DrawWater( DWORD dwTick ) = 0;

	/**
	 * 지형 피킹.
	 *
	 * @return	피킹 성공 여부. ( 에러를 뱉는 건 아니다 )
	 */
	virtual BOOL	Pick(	const float		fHeight,	///< [IN] 바닥 평면 높이.
							const Vector3&	vOrig,		///< [IN] 피킹용 반직선 시작점.
							const Vector3&	vDir,		///< [IN] 피킹용 반직선 방향.
							Vector3*		pvPick		///< [OUT] 피킹 좌표.
							) = 0;

	/**
	 *
	 *
	 */
	virtual void	SetUseObjectHeight( BOOL bUse = TRUE ) = 0;

	/**
	 * Height Field의 높이를 설정한다.
	 *
	 * @param	nX			[IN] X 좌표.
	 * @param	nY			[IN] Y 좌표.
	 * @param	fHeight		[IN] 높이.
	 */
	virtual void	SetHeight( int nX, int nY, float fHeight ) = 0;

	/**
	 * Height Field의 높이를 얻는다.
	 *
	 * @param	nX			[IN] X 좌표.
	 * @param	nY			[IN] Y 좌표.
	 *
	 * @return	높이.
	 */
	virtual float	GetHeight( float fX, float fY ) = 0;

	/**
	 * 지형에 텍스처를 입힌다.
	 *
	 * @param	nX			[IN] X 좌표.
	 * @param	nY			[IN] Y 좌표.
	 * @param	nTexture	[IN] 텍스처 번호.
	 */
	virtual void	SetTexture( int nX, int nY, int nTexture ) = 0;

	/**
	 * 지형에 쓸 텍스처를 읽어 온다.
	 *
	 *
	 */
	virtual BOOL	LoadTexture( const char* szTexture ) = 0;

	/**
	 * 지형에 텍스처를 입혀진 텍스처를 얻는다.
	 *
	 * @param	nX			[IN] X 좌표.
	 * @param	nY			[IN] Y 좌표.
	 *
	 * @return	텍스처 번호.
	 */
	virtual int		GetTexture( int nX, int nY ) = 0;

	/**
	 * 지형 크기를 얻는다.
	 *
	 * @return	지형의 크기. ( 가로, 세로 동일 )
	 */
	virtual int		GetSize() = 0;

	/**
	 * 하이트 맵의 버퍼를 얻는다.
	 *
	 * @return	하이트 맵 버퍼.
	 */
	virtual float*	GetHeightMap() = 0;


	/**
	 * 텍스처 번호 맵의 버퍼를 얻는다.
	 *
	 * @return	텍스처 번호 맵.
	 */
	virtual BYTE*	GetTextureMap() = 0;

	/**
	 * 컬러 맵의 버퍼를 얻는다.
	 *
	 * @return	컬러 맵의 버퍼
	 */
	virtual DWORD*	GetColorMap() = 0;
	
	/**
	 * 오브젝트 맵의 버퍼를 얻는다.
	 *
	 * @return	하이트 맵 버퍼.
	 */
	virtual float*	GetObjectHeightMap() = 0;

	/**
	 * 물의 높이를 얻는다.
	 *
	 * @return  물의 높이
	 */
	virtual float	GetWaterHeight() = 0;

	/**
	 * 지형의 속성을 얻는다.
	 *
	 * @return	속성 맵 버퍼.
	 */
	virtual BYTE*	GetAttributeMap() = 0;
	
	/**
	 * 지형 전체 버텍스의 Normal 값을 계산한다.
	 */
	virtual void	CalculateNormals() = 0;

	/**
	 * 와이어프레임 사용 여부를 결정한다.
	 *
	 * @param	bUse	[IN] 와이어프레임 사용 여부.
	 */
	virtual void	UseWireframe( BOOL bUse ) = 0;

	/**
	 * 그림자를 그려주기위한 버텍스들을 추가한다.
	 */
	virtual void	InsertShadow( IW3DShadow* pShadow, const Vector3 vPos ) = 0;

	/**
	 *
	 */
	virtual void	SetLevel( int nLevel = 0 ) = 0;

	/**
	 *
	 */
	virtual void	SetLayer( int nLayer = 0 ) = 0;

	/**
	 *
	 */
	virtual int		GetLevel() = 0;

	/**
	 *
	 */
	virtual	BYTE	GetAttribute( float fX, float fY ) = 0;

	/**
	 *
	 */
	virtual void	SetUseLevel( BOOL bUse ) = 0;

	/**
	 *
	 */
	virtual void	AddPass( const SMultiPass* pPass ) = 0;

	/**
	 *
	 */
	virtual void	SetPosition( const Vector3& vPos ) = 0;

	/**
	 *
	 */
	virtual void	ShowCloud( BOOL bShow ) = 0;

	/**
	 *
	 */
	virtual void	SetLevelOne( BOOL bUse ) = 0;

}; //class IW3DTerrain


#endif // #ifndef __Wind3D_Render_IW3DTerrain_Header__
