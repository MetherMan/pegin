/**
 * 특수효과 클래스의 인터페이스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	로냐
 * @date	$Date: 03-07-01 6:21p $
 * @version	$Revision: 19 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DSpecialEffect.h $
 * 
 * 19    03-07-01 6:21p Kjmgo
 * 
 * 18    03-03-13 4:55p Ahastudio
 * comment 수정.
 * <HR>
 *
 * @file	IW3DSpecialEffect.h
 */


#ifndef __Wind3D_Render_IW3DSpecialEffect_Header__
#define __Wind3D_Render_IW3DSpecialEffect_Header__


/**
 * 특수효과 클래스.
 * @ingroup		Render
 */
class IW3DSpecialEffect : public IComponent
{
public:
	/**
	 * Effect를 그려준다.
	 *
	 * @return	이펙트가 진행 중이면 TRUE, 끝났으면 FALSE
	 */
	virtual BOOL	Render(	const int nFrame,			///< [IN] Animation Frame
							const Matrix4& matWorld,	///< [IN] World Transform Matrix
							const BOOL bTool = false	///< [IN] 툴용으로 사용할지 여부
							) = 0;

	/**
	 * Effect 정보 파일을 읽어온다.
	 *
	 * @param	szFileName		[IN] 파일명 (*.wed)
	 *
	 * @return	파일 읽기 성공 여부
	 */
	virtual BOOL	Load( const char* szFileName ) = 0;

	// 아래는 툴을 위한 Interface들 -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

	/**
	 * WEM파일을 (Effect전용 Mesh 파일) 읽어 옵니다.
	 *
	 * @param	szFileName		[IN] 파일명 (*.wem)
	 *
	 * @return	파일 읽기 성공 여부
	 */
	virtual BOOL	LoadMesh(  const char* szFileName ) = 0;

	/**
	 * 텍스처 파일을 읽어옵니다.
	 *
	 * @param	szFileName		[IN] 텍스처 파일명
	 *
	 * @return	파일 읽기 성공 여부
	 */
	virtual BOOL    SetTexture( const char* szFileName ) = 0;

	/**
	 * 이동, 회전, 스케일링 등을 적용합니다.
	 *
	 * @param	matWorld		[IN] 이동, 회전, 스케일링이 적용된 Matrix
	 */
	virtual void	SetWorld( const Matrix4& matWorld ) = 0;

	/**
	 * Mesh의 밝기를 설정해 줍니다. (툴을 위한 함수)
	 *
	 * @param	dwColor			[IN] 밝기가 적용된 컬러값
	 */
	virtual void	SetBright( const DWORD dwColor ) = 0;

	/**
	 * Mesh의 색을 설정해 줍니다. ( 강제 적용 기존 정보를 날려 버림 )
	 *
	 * @param	dwColor			[IN] 밝기가 적용된 컬러값
	 */
	virtual void	SetColor( const DWORD dwColor ) = 0;

	/**
	 * 현재 Frame에 적용될 Texture 조각(ID)을 설정해 줍니다.
	 * (툴을 위한 함수)
	 *
	 * @param	nId				[IN] 1부터 시작하는 ID
	 */
	virtual void	SetTextureAnimation( const int nId ) = 0;

	/**
	 * .....................
	 */
	virtual void	SetAnimation(	const int nRow,		///< [IN]
									const int nCols,	///< [IN]
									const int nId		///< [IN]
									) = 0;

	/**
	 *	최고 Frame을 가져 온다.
	 *
	 * @return	최대 Frame
	 */
	virtual int		GetMaxFrame() = 0;
}; //class IW3DSpecialEffect


#endif // #ifndef __Wind3D_Render_IW3DSpecialEffect_Header__
