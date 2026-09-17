/**
 * 그림자 컴포넌트 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-12-17 11:38p $
 * @version	$Revision: 17 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DShadow.h $
 * 
 * 17    03-12-17 11:38p Kjmgo
 * Alt + Tab 지원
 * 
 * 16    03-04-23 5:19a Ahastudio
 *
 * 13    03-03-11 8:27p Kjmgo		필요 없는 함수 제거 및 복수 Blocker 처리로 함수를 합침.
 * <HR>
 *
 * @file	IW3DShadow.h
 */


#ifndef __Wind3D_Render_IW3DShadow_Header__
#define __Wind3D_Render_IW3DShadow_Header__


/**
 * 그림자 인터페이스 클래스.
 * @ingroup		Render
 */
class IW3DShadow : public IComponent
{
public:
	/**
	 * 그림자 초기화
	 *
	 * @param	nType		[IN] 그림자 종류
	 *							- 0 : 원형
	 *							- 1 : Projective Shadow Model
	 *							- 2 : Projective Shadow Terrain
	 *
	 * @return	초기화 성공 여부
	 */
	virtual BOOL	Init( int nType ) = 0;

	/**
	 * 그림자의 Size를 설정한다.
	 *
	 * @param	nSize	[IN] 그림자 텍스처의 크기.
	 *					     Projective Shadow일 때만 적용한다 ( Default : 64 )
	 */
	virtual void	SetSize( int nSize ) = 0;

	/**
	 * 그림자 렌더링
	 *
	 * @param	matWorld	[IN] World Matrix.
	 *						     Projection Shadow일 경우 World가 아니라 View이다.
	 */
	virtual void	Render( const Matrix4& matWorld ) = 0;

	// Projective Shadow를 위한 함수들 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	/**
	 * 그림자를 만들 객체를 추가합니다.
	 */
	virtual void	InsertBlocker( IW3DModel* pBlocker ) = 0;

	/**
	 * ShadowMap을 만듭니다.
	 * 그림자를 그려줄 Model이 하나일 때.
	 */
	virtual void	MakeShadowMap( DWORD dwTick ) = 0;

	/**
	 * 그림자를 그릴 Model을 추가한다.
	 */
	virtual void	InsertModel( IW3DModel* pModel ) = 0;

	/**
	 * 광원 위치를 설정한다.
	 */
	virtual void	SetLight( const Vector3& vLight ) = 0;

	/**
	 * 그림자 행렬.
	 */
	virtual void	ShadowMatrix( const Vector3& vBlockerPosition ) = 0;

	/**
	 * 지형에 그릴 버텍스를 추가한다.
	 */
	virtual void	InsertVertex( const Vector3& vVertex ) = 0;

	/**
	 * Projection Shadow 사용 여부를 얻는다.
	 */
	virtual BOOL	IsProjection() = 0;

	/**
	 *
	 */
	virtual	void	Restore() = 0;

	/**
	 *
	 */
	virtual void	Cleanup() = 0;
}; //class IW3DShadow


#endif // #ifndef __Wind3D_Render_IW3DShadow_Header__
