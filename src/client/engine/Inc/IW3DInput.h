/**
 * 입력장치 컴포넌트 인터페이스 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-01-29 10:32a $
 * @version	$Revision: 12 $
 *
 * <HR>
 * $Log: /Engine/Inc/IW3DInput.h $
 * 
 * 12    04-01-29 10:32a Paran
 * 
 * 11    03-04-04 4:57a Ahastudio
 * <HR>
 *
 * @file	IW3DInput.h
 */


#ifndef __Wind3D_Enigne_IW3DInput_Header__
#define __Wind3D_Enigne_IW3DInput_Header__


#include "KeyCode.h"


/**
 * 입력장치 객체.
 * @ingroup		Engine
 */
class IW3DInput : public IComponent
{
public:
	/**
	 * 입력장치 초기화.
	 */
	virtual BOOL	Init(
		HINSTANCE	hInstance,		///< [IN] 어플리케이션 인스턴스
		HWND		hWnd,			///< [IN] 윈도우 핸들
		BOOL		bWinMouse		///< [IN] 윈도우 마우스 사용여부
		) = 0;

	/**
	 * 뷰포트를 설정한다.
	 */
	virtual void	SetViewport(
		int		nX,			///< [IN] X
		int		nY,			///< [IN] Y
		int		nWidth,		///< [IN] 가로 길이
		int		nHeight		///< [IN] 세로 길이
		) = 0;

	/**
	 * 마우스 휠 델타 값을 저장한다
	 * (윈도우 이벤트시 처리, DxInput 마우스가 아닐 경우)
	 */
	virtual void	SetMouseWheel( WORD wDelta ) = 0;

	/**
	 * 입력장치 업데이트.
	 */
	virtual void	Update() = 0;

	/**
	 * 키 눌림 상태를 얻는다.
	 */
	virtual BOOL	GetKeyDown( int nKey ) = 0;

	/**
	 * 마우스 X 좌표를 얻는다.
	 */
	virtual int		GetMouseX() = 0;

	/**
	 * 마우스 Y 좌표를 얻는다.
	 */
	virtual int		GetMouseY() = 0;

	/**
	 * 마우스 Z 좌표를 얻는다.
	 */
	virtual int		GetMouseZ() = 0;

	/**
	 * 마우스 X 이동량을 얻는다.
	 */
	virtual	int		GetMouseMoveX() = 0;

	/**
	 * 마우스 Y 이동량을 얻는다.
	 */
	virtual	int		GetMouseMoveY() = 0;

	/**
	 * 마우스 왼쪽 버튼 눌림 상태를 얻는다.
	 */
	virtual BOOL	GetMouseL() = 0;

	/**
	 * 마우스 오른쪽 버튼 눌림 상태를 얻는다.
	 */
	virtual BOOL	GetMouseR() = 0;

	/**
	 * 마우스 가운데 버튼 눌림 상태를 얻는다.
	 */
	virtual BOOL	GetMouseM() = 0;

	/**
	 * 마우스 왼쪽 버튼 클릭 상태를 얻는다.
	 */
	virtual BOOL	GetMouseLClick() = 0;

	/**
	 * 마우스 오른쪽 버튼 클릭 상태를 얻는다.
	 */
	virtual BOOL	GetMouseRClick() = 0;

	/**
	 * 마우스 가운데 버튼 클릭 상태를 얻는다.
	 */
	virtual BOOL	GetMouseMClick() = 0;
}; //class IW3DInput


#endif // #ifndef __Wind3D_Enigne_IW3DInput_Header__
