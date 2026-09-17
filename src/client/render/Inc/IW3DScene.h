/**
 * Scene 인터페이스.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-03-25 4:33p $
 * @version	$Revision: 29 $
 *
 * <HR>
 * $Log: /Render/Inc/IW3DScene.h $
 * 
 * 29    04-03-25 4:33p Kjmgo
 * 
 * 28    04-02-10 3:28p Paran
 * 
 * 27    04-01-14 9:46a Kjmgo
 * FPS를 얻어 오는 함수 추가
 * 
 * 26    03-12-17 11:38p Kjmgo
 * Alt + Tab 지원
 * 
 * 25    03-12-14 3:11a Paran
 * 
 * 24    03-11-13 8:23p Paran
 * 
 * 23    03-10-15 6:42p Kjmgo			라이트 관리자 추가 ^-^ 
 * 
 * 20    03-05-17 10:19a Ahastudio		Capture() 함수 추가.
 *
 * 17    03-03-19 1:45a Paran			폰트 리스트 추가.
 *
 * 16    03-03-18 11:32p Ahastudio		SetFog() 변경.
 * <HR>
 *
 * @file	IW3DScene.h
 */


#ifndef __Wind3D_Render_IW3DScene_Header__
#define __Wind3D_Render_IW3DScene_Header__


/**
 * Scene 인터페이스.
 * @ingroup		Render
 */
class IW3DScene : public IComponent
{
public:
	/**
	 * Scene 초기화.
	 */
	virtual BOOL	Init(
		const HWND	hWnd,			///< [IN] 윈도우 핸들.
		const int	nWidth,			///< [IN] 가로 해상도.
		const int	nHeight,		///< [IN] 세로 해상도.
		const int	nColorDepth,	///< [IN] Color Depth (전체 화면일 때만 적용됨)
		const BOOL	bFullscreen		///< [IN] 전체 화면 여부.
		) = 0;

	/**
	 * Scene의 이번 Frame을 시작한다.
	 * 화면을 지우고, Rendering을 준비한다.
	 *
	 */
	virtual BOOL	BeginFrame() = 0;

	/**
	 * Scene의 이번 Frame을 마친다.
	 * Rendering이 완료될 때 까지 기다리고, 화면을 보여준다.
	 */
	virtual BOOL	EndFrame() = 0;

	/**
	 *
	 *
	 */
	virtual float	GetFPS() = 0;

	/**
	 * 뷰포트 설정.
	 *
	 * @param	nX			[IN] X
	 * @param	nY			[IN] Y
	 * @param	nWidth		[IN] Width
	 * @param	nHeight		[IN] Height
	 */
	virtual void	SetViewport( int nX, int nY, int nWidth, int nHeight ) = 0;

	/**
	 * 뷰포트.
	 */
	virtual void	GetViewport( int& nWidth, int& nHeight ) = 0;

	/**
	 * 카메라 설정.
	 */
	virtual void	LookAt(
		const Vector3&	vEye,	///< [IN] 카메라의 위치
		const Vector3&	vAt,	///< [IN] 카메라가 쳐다볼 위치
		const Vector3&	vUp		///< [IN] 카메라의 Up 벡터
		) = 0;

	/**
	 * 프로젝션 설정.
	 *
	 * @param	fFOV			[IN] Field of View
	 * @param	fNearPlane		[IN] 근단면
	 * @param	fFarPlane		[IN] 원단면
	 */
	virtual void	SetProjection( float fFOV, float fNearPlane, float fFarPlane ) = 0;

	/**
	 * 3D 좌표를 화면상의 좌표로 변환한다.
	 *
	 * @param	vPos		[IN] 3D 좌표.
	 */
	virtual Vector3	GetScreenPos( const Vector3& vPos ) = 0;

	/**
	 * 안개 설정.
	 *
	 * @param	dwColor		[IN] 안개 색
	 * @param	fStart		[IN] 시작 거리
	 * @param	fEnd		[IN] 끝 거리
	 */
	virtual void	SetFog( DWORD dwColor, float fStart, float fEnd ) = 0;

	/**
	 * View Frustum을 얻음.
	 *
	 * @return	View Frustum.
	 */
	virtual Frustum&	GetFrustum() = 0;

	/**
	 * 피킹용 반직선을 얻는다.
	 *
	 * @param	nX			[IN] 화면상의 X 좌표.
	 * @param	nY			[IN] 화면상의 Y 좌표.
	 * @param	pvOrig		[OUT] 반직선의 시작점.
	 * @param	pvDir		[OUT] 반직선의 방향.
	 */
	virtual void	GetPickRay( int nX, int nY, Vector3* pvOrig, Vector3* pvDir ) = 0;

	/**
	 * 문자열 사이즈.
	 */
	virtual void	GetStringSize( int& nWidth, int& nHeight, const char* szText ) = 0;
	
	/**
	 * 문자열을 출력한다.
	 *
	 * @param	nX			[IN] X 좌표.
	 * @param	nY			[IN] Y 좌표.
	 * @param	dwColor		[IN] 글자색.
	 * @param	szFormat	[IN] 문자열 포맷.
	 * @param	...			[IN] 인자들.
	 */
	virtual void	DrawText( int nX, int nY, DWORD dwColor, const char* szFormat, ... ) = 0;
	
	/**
	 * 문자열을 출력한다.
	 */
	virtual void	DrawText( 
		int nX,						///< [IN] Y 좌표.
		int nY,						///< [IN] X 좌표.
		int nWidth,					///< [IN] 가로크기.
		int nHeight,				///< [IN] 세로크기.
		DWORD dwColor,				///< [IN] 글자색.
		const char* szFormat, ...
		) = 0;

	/**
	 * 폰트를 추가한다.
	 *
	 * @return	성공/실패
	 */	
	virtual BOOL	AddFont(
		IW3DFont**		ppFont,			///< [OUT] 생성 추가한 폰트
		const char*		szKeyName,		///< [IN] 추가할 때 키 이름
		const char*		szFace,			///< [IN] 폰트
		const int		nSize,			///< [IN] 폰트 크기
		const long		lWeight,		///< [IN] 굵기
		const BOOL		bULine,			///< [IN] 밑줄
		const BOOL		bItalic			///< [IN] 기울임
		) = 0;

	/**
	 * 폰트를 찾는다.
	 *
	 * @return	성공/실패
	 */
	virtual BOOL	FindFont(
		IW3DFont**		ppFont,			///< [OUT] 찾은 폰트
		const char*		szKeyName		///< [IN] 찾을 폰트 키 이름
		) = 0;

	/**
	 * Frame Rate를 보일지 결정한다.
	 *
	 * @param	bShow			[IN] FPS를 보일지 여부
	 */
	virtual void	ShowFrameRate( BOOL bShow ) = 0;

	/**
	 * 게임 화면을 캡쳐한다.
	 *
	 * @param	szName		[IN] 파일 이름
	 *
	 * @return	화면 캡쳐 성공 여부
	 */
	virtual	BOOL	Capture( const char* szName, SYSTEMTIME* pSt ) = 0;

	/**
	 * 감마를 컨트롤 한다.
	 */
	virtual BOOL	SetGammaControl( float fRed, float fGreen, float fBlue ) = 0;

	/**
	 * 감마를 초기 설정으로 돌린다.
	 */
	virtual BOOL	RestoreGamma() = 0;

	/**
	 * 라이트의 총 갯수 ( 쓸수 있는 )
	 */
	virtual int		GetMaxActiveLights() = 0;

	/**
	 * 텍스쳐 레벨 설정
	 */
	virtual void	SetTexLevel( BYTE btLevel ) = 0;	

	/**
	 *
	 */
	virtual void	AddShadow( IW3DShadow* pShadow ) = 0;

}; //class IW3DScene


#endif // #ifndef __Wind3D_Render_IW3DScene_Header__
