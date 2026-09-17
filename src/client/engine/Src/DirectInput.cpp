/**
 * Direct Input Wapper 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 03-08-28 11:01a $
 * @version	$Revision: 8 $
 *
 * <HR>
 * $Log: /Engine/Src/DirectInput.cpp $
 * 
 * 8     03-08-28 11:01a Sckyj
 * 
 * 7     03-03-25 3:38a Ahastudio
 * <HR>
 *
 * @file	DirectInput.cpp
 */


#pragma comment( lib, "DXGUID.LIB" )
#pragma comment( lib, "DINPUT8.LIB" )


#include "../../Common/Inc/Common.h"
#include "DirectInput.h"


/**
 * 생성자.
 */
CDInput::CDInput()
{
	m_pDI		= NULL;
	m_pKeyboard	= NULL;
	m_pMouse	= NULL;

	_ZeroMemory( m_keybuf, sizeof( m_keybuf ) );
	_ZeroMemory( &m_dims, sizeof( m_dims ) );
} //CDInput::CDInput

/**
 * 소멸자.
 */
CDInput::~CDInput()
{
	if( m_pKeyboard )
	{
		m_pKeyboard->Unacquire(); 
		SAFE_RELEASE( m_pKeyboard );
	} //if
	if( m_pMouse )
	{
		m_pMouse->Unacquire(); 
		SAFE_RELEASE( m_pMouse );
	} //if
	SAFE_RELEASE( m_pDI );
} //CDInput::~CDInput

/**
 * 초기화.
 *
 * @param	hInstance		[IN] 어플리케이션 인스턴스.
 *
 * @return	초기화 성공 여부.
 */
BOOL CDInput::Init( HINSTANCE hInstance )
{
	HRESULT		hr;

	hr = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL );
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	return TRUE;
} //CDInput::Init

/**
 * 키보드 초기화.
 *
 * @param	hWnd		[IN] 윈도우 핸들.
 *
 * @return	초기화 성공 여부.
 */
BOOL CDInput::InitKeyboard( HWND hWnd )
{
	HRESULT		hr;

	// Create Keyboard Device...
	hr = m_pDI->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, NULL );
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	// Set Keyboard Data Format...
	hr = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	// Set Keyboard Cooperative Level...
	hr = m_pKeyboard->SetCooperativeLevel( hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	// Gain Keyboard Access...
	hr = m_pKeyboard->Acquire();	
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	return TRUE;
} //CDInput::InitKeyboard

/**
 * 마우스 초기화.
 *
 * @param	hWnd		[IN] 윈도우 핸들.
 *
 * @return	초기화 성공 여부.
 */
BOOL CDInput::InitMouse( HWND hWnd )
{
	HRESULT		hr;

	// Create Mouse Device...
	hr = m_pDI->CreateDevice( GUID_SysMouse, &m_pMouse, NULL );
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	// Set Mouse Data Format...
	hr = m_pMouse->SetDataFormat( &c_dfDIMouse );
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	// Set Mouse Cooperative Level...
//	hr = m_pMouse->SetCooperativeLevel( hWnd, DISCL_BACKGROUND );
	hr = m_pMouse->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	// Gain Mouse Access...
	hr = m_pMouse->Acquire();
	if( FAILED( hr ) )
	{
		throw "입력장치 초기화 실패";
		return FALSE;
	} //if

	return TRUE;
} //CDInput::InitMouse

/**
 * 키보드 입력 상태를 얻는다.
 *
 * @return	키보드 입력 상태 얻기 성공 여부.
 */
BOOL CDInput::GetKeyboardState()
{
	HRESULT		hr;

	hr = m_pKeyboard->GetDeviceState( 256, m_keybuf );
	if( FAILED( hr ) )
	{
		hr = m_pKeyboard->Acquire();
		if( FAILED( hr ) )
		{
			return FALSE;
		} //if

		hr = m_pKeyboard->GetDeviceState( 256, m_keybuf );
		if( FAILED( hr ) )
		{
			return FALSE;
		} //if
	} //if

	return TRUE;
} //CDInput::GetKeyboardState

/**
 * 마우스 입력 상태 얻기.
 *
 * @return	마우스 입력 상태 얻기 성공 여부.
 */
BOOL CDInput::GetMouseState()
{
	HRESULT		hr;

	hr = m_pMouse->GetDeviceState( sizeof( m_dims ), (void*)&m_dims );
	if( FAILED( hr ) )
	{
		hr = m_pMouse->Acquire();
		if( FAILED( hr ) )
		{
			return FALSE;
		} //if

		hr = m_pMouse->GetDeviceState( sizeof( m_dims ), (void*)&m_dims );
		if( FAILED( hr ) )
		{
			return FALSE;
		} //if
	} //if

	return TRUE;
} //CDInput::GetMouseState
