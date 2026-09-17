/**
 * 모델.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-06-08 3:56p $
 * @version	$Revision: 49 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DModel.h $
 * 
 * 49    04-06-08 3:56p Paran2
 * 
 * 48    04-05-11 11:33a Sckyj
 * SetInterpolateBlend 함수 추가 - 동작사이의 보간용
 * 
 * 47    04-03-19 7:03p Paran
 * 
 * 46    04-03-08 3:01p Sckyj			GetModelName() 함수 추가
 * 
 * 43    03-10-15 4:47p Kjmgo			Load 할시, Pivot과, Radius를 계산 하던 것을 Radius계산하는 것을 따로 뺴고, Radius에서, Pivot도 계산하도록 수정
 * 
 * 42    03-10-13 4:36p Kjmgo			SetVertexBlend함수 추가
 * 
 * 41    03-10-13 2:51p Kjmgo			Animation 수정, Model의 경우 SetApplyWorldMatrix함수를 추가하고, Render함수 내부에서, WorldMatrix를 외부에서 적용시킬수 있도록 수정
 * 
 * 40    03-10-13 2:21p Kjmgo			Animation 최적화 1단계
 * 
 * 38    03-09-18 10:07p Kjmgo			GetTime() 추가
 * 
 * 37    03-08-29 9:19a Kjmgo			Animation 바꿈
 * 
 * 35    03-06-10 10:41a Paran			메쉬 TM 얻기 추가
 * 
 * 32    03-04-13 8:53p Kjmgo			그림자 추가.
 *
 * 29    03-03-16 12:58a Ahastudio		캐릭터 애니메이션용 Vertex Blending 추가.
 *
 * 28    03-03-14 8:57p Ahastudio		CreateBox() 제거. Pivot, Radius 추가. SetWorld() 수정.
 *
 * 24    03-02-26 12:42p Ahastudio		새로운 모델 읽기 적용.
 * <HR>
 *
 * @file	IW3DModel.h
 */


#ifndef __Wind3D_Render_IW3DModel_Header__
#define __Wind3D_Render_IW3DModel_Header__

/**
 * 모델 컴포넌트 인터페이스.
 * @ingroup		Render
 */
class IW3DModel : public IComponent
{
public:
	/**
	 * 모델을 불러온다.
	 *
	 * @return	읽기 성공 여부
	 */
	virtual BOOL	Load(
		const char*		szPathName,		///< [IN] 경로 이름
		const char*		szFileName		///< [IN] 파일 이름
		) = 0;

	/**
	 * 애니메이션 파일을 읽는다.
	 */
	virtual BOOL	LoadAnimation(
		const char* szPathName,		///< [IN] 경로 이름
		const char* szFileName		///< [IN] 파일 이름
		) = 0;

	/**
	 * 월드 변환 행렬을 설정한다.
	 */
	virtual void	SetWorld(
		const Vector3&		vPos,
		const Quaternion&	qRot
		) = 0;

	/**
	 * 월드 변환 행렬을 설정한다.
	 */
	virtual void	SetWorld( const Matrix4& matWorld ) = 0;

	/**
	 * 모델이 화면에 보이는지 검사한다.
	 */
	virtual BOOL	IsVisible() = 0;

	/**
	 * 모델을 화면에 보일지 여부.
	 */
	virtual void	Visible( const BOOL bVisible ) = 0;

	/**
	 * 모델 출력 생상을 설정한다.
	 */
	virtual void	SetColor( const DWORD dwColor ) = 0;

	/**
	 * 텍스처 사용 여부를 설정한다.
	 */
	virtual void	SetUseTexutre( const BOOL bUse ) = 0;

	/**
	 *	Shadow용 렌더인지 여부를 설정 
	 */
	virtual void	SetShadow( const BOOL bUse ) = 0;
	
	/**
	 * 화면에 모델을 출력한다.
	 *
	 * @param	dwTick			[IN] 애니메이션 Tick
	 */
	virtual void	Render( const DWORD dwTick, const BOOL bLighting = TRUE, const BOOL bAlpha = TRUE ) = 0;

	/**
	 * 모델 피킹.
	 */
	virtual BOOL	Pick(
		const Vector3&	vOrig,				///< [IN] 반직선 시작점
		const Vector3&	vDir,				///< [IN] 반직선 방향
		float*			pfDistance = NULL	///< [OUT] 피킹 거리
		) = 0;

	/**
	 * ...
	 */
	virtual DWORD	GetActionTicks() = 0;

	/**
	 * ...
	 */
	virtual void	SetAction( SAction* pAction ) = 0;

	/**
	 * Bone의 애니메이션 TM을 얻는다.
	 */
	virtual Matrix4		GetBoneMatrix( const char* szName, const DWORD dwTick ) = 0;

	/**
 	 * 애니메이션 TM을 얻는다.
	 */
	virtual	Matrix4		GetTMMatrix( const DWORD dwTick ) = 0;

	/**
	 * 모델의 중심 좌표를 얻는다.
	 */
	virtual const Vector3&		GetPivot() = 0;

	/**
	 * 모델의 반지름을 얻는다.
	 */
	virtual float		GetRadius() = 0;

	/**
	 * 모델의 크기를 얻는다. 
	 */
	virtual Vector3		GetSize() = 0;

	/**
	 * X, Y 중 가장 큰 값을 구한다.
	 */
	virtual Vector3		GetXYMax( const Matrix4& matLight ) = 0;

	/**
	 *
	 */
	virtual DWORD		GetTime() = 0;

	/**
	 * Check Collision Occurs Point Into Box
	 */
	virtual BOOL		CollisionPoint( const Vector3& vPos ) = 0;

	/**
	 *	내부적으로 World Matrix를 적용할것인지의 여부
	 *  FALSE를 넘기면, 내부적으로 World Matrix 연산을 하지 아니한다.
	 */
	virtual void		SetApplyWorld( BOOL bApply = TRUE ) = 0;

	/**
	 *
	 */
	virtual void		SetVertexBlend( const DWORD dwTick ) = 0;
	
	/**
	 * Intierpolation Vertex Blending
	 */	
	void				SetInterpolateBlend( DWORD		dwPrevTicks,		// Prev Animation Frame Ticks
											 DWORD		dwNextTicks,		// Next Animation Frame Ticks
											 SAction*	pInterAction,		// Interpolation Animation
											 float		fInterTime );		// Interpolation Time

	
	/**
	 * 현재 동작중에 다른 동작을 함께 적용한다.
	 */
	virtual BOOL		SetMultiActionVertexBlend( 
		const SAction& sBlendAction,			///< 현재 동작에 적용은 다른 동작.
		const DWORD    dwTick,					///< 현재 틱
		const DWORD    dwBlendActTick			///< 적용하는 동작 틱
		) = 0;

	/**
	 * 반지름은 따로 계산을 해야 한다.
	 */
	virtual	void		CalculateRadius() = 0;
	
	virtual const char* GetModelName() = 0;

}; //class IW3DModel


#endif // #ifndef __Wind3D_Render_IW3DModel_Header__
