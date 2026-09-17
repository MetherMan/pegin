/**
 * Direct Input Wrapper 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-03-25 3:38a $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /Engine/Src/DirectInput.h $
 * 
 * 9     03-03-25 3:38a Ahastudio
 * <HR>
 *
 * @file	DirectInput.h
 */


#ifndef __DirectInput_Header__
#define __DirectInput_Header__


#define DIRECTINPUT_VERSION		0x0800
#include <DINPUT.H>


/**
 * Direct Input Wrapper.
 * @ingroup		Engine
 */
class CDInput
{
public:
	UNDER_MEMORY_MGR( CDInput );

	char			m_keybuf[256];			///< 키 입력 상태.
	DIMOUSESTATE	m_dims;					///< 마우스 입력 상태.

	CDInput();
	~CDInput();

	BOOL Init( HINSTANCE hInstance );
	BOOL InitKeyboard( HWND hWnd );
	BOOL InitMouse( HWND hWnd );

	BOOL GetKeyboardState();
	BOOL GetMouseState();

private:
	LPDIRECTINPUT8			m_pDI;			///< Direct Input 객체.
	LPDIRECTINPUTDEVICE8	m_pKeyboard;	///< 키보드 입력장치 객체.
	LPDIRECTINPUTDEVICE8	m_pMouse;		///< 마우스 입력장치 객체.
}; //class CDInput


#endif // #ifndef __DirectInput_Header__
