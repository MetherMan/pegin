/**
 * 입력장치 컴포넌트 구현부.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	아샬
 * @date	$Date: 04-01-29 10:32a $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /Engine/Src/W3DInput.cpp $
 * 
 * 11    04-01-29 10:32a Paran
 * 
 * 10    04-01-13 2:18a Kjmgo
 * 
 * 9     03-03-22 5:29a Ahastudio
 *
 * 8     03-02-21 1:22p Ahastudio
 * <HR>
 *
 * @file	W3DInput.cpp
 */


#include "Engine_Export.h"
#include "DirectInput.h"


/**
 * 입력 처리 모듈.
 * @ingroup		Engine
 */
class CW3DInput : public IW3DInput
{
public:
	UNDER_MEMORY_MGR( W3DInput )

	CW3DInput();
	~CW3DInput();

	BOOL	Init( HINSTANCE hInstance, HWND hWnd, BOOL bWinMouse );

	void	SetViewport( int nX, int nY, int nWidth, int nHeight )
	{
		m_nX		= nX;
		m_nY		= nY;
		m_nWidth	= nWidth;
		m_nHeight	= nHeight;

		m_ptMouse.x		= m_nWidth  / 2;
		m_ptMouse.y		= m_nHeight / 2;
		m_ptPrevMouse.x	= m_ptMouse.x;
		m_ptPrevMouse.y	= m_ptMouse.y;
	}

	void	SetMouseWheel( WORD wDelta );

	void	Update();

	BOOL	GetKeyDown( int nKey )
	{
		Assert( nKey < 256 );
		Assert( m_pInput );
		return ( m_pInput->m_keybuf[nKey] & 0x80 );
	}

	int		GetMouseX()
	{
		return m_ptMouse.x;
	}

	int		GetMouseY()
	{
		return m_ptMouse.y;
	}

	int		GetMouseZ()
	{
		return m_nMouseZ;
	}

	int		GetMouseMoveX()
	{
		if( m_bWinMouse )
		{
			return m_ptMouse.x - m_ptPrevMouse.x;
		} //if
		return m_pInput->m_dims.lX;
	}

	int		GetMouseMoveY()
	{
		if( m_bWinMouse )
		{
			return m_ptMouse.y - m_ptPrevMouse.y;
		} //if
		return m_pInput->m_dims.lY;
	}

	BOOL	GetMouseL()
	{
		if( m_bWinMouse )
		{
			if( GetAsyncKeyState( 0x01 ) < 0 )		return TRUE;
			else									return FALSE;
		} //if
		Assert( m_pInput );
		return m_pInput->m_dims.rgbButtons[0];
	}

	BOOL	GetMouseR()
	{
		if( m_bWinMouse )
		{
			if( GetAsyncKeyState( 0x02 ) < 0 )		return TRUE;
			else									return FALSE;
		} //if
		Assert( m_pInput );
		return m_pInput->m_dims.rgbButtons[1];
	}

	BOOL	GetMouseM()
	{
		if( m_bWinMouse )
		{
			if( GetAsyncKeyState( 0x04 ) < 0 )		return TRUE;
			else									return FALSE;
		} //if
		Assert( m_pInput );
		return m_pInput->m_dims.rgbButtons[2];
	}

	BOOL	GetMouseLClick()
	{
		if( m_bWinMouse )
		{
			if( m_bOldMouseL && !m_bMouseL )	return TRUE;
			else								return FALSE;
		} //if

		Assert( m_pInput );
		return ( m_bOldMouseL && !m_pInput->m_dims.rgbButtons[0] );
	}

	BOOL	GetMouseRClick()
	{
		if( m_bWinMouse )
		{
			if( m_bOldMouseR && !m_bMouseR )	return TRUE;
			else								return FALSE;
		} //if

		Assert( m_pInput );
		return ( m_bOldMouseR && !m_pInput->m_dims.rgbButtons[1] );
	}

	BOOL	GetMouseMClick()
	{
		if( m_bWinMouse )
		{
			if( m_bOldMouseM && !m_bMouseM )	return TRUE;
			else								return FALSE;
		} //if

		Assert( m_pInput );
		return ( m_bOldMouseM && !m_pInput->m_dims.rgbButtons[2] );
	}

private:
	CDInput*	m_pInput;			// Direct Input Object..
	HWND		m_hWnd;

	BOOL		m_bWinMouse;

	int		m_nX;
	int		m_nY;
	int		m_nWidth;
	int		m_nHeight;

	POINT	m_ptMouse;
	int		m_nMouseZ;
	POINT	m_ptPrevMouse;	
	int		m_nPrevMouseZ;

	BOOL	m_bMouseL;
	BOOL	m_bMouseR;
	BOOL	m_bMouseM;
	BOOL	m_bOldMouseL;
	BOOL	m_bOldMouseR;
	BOOL	m_bOldMouseM;
}; //class CW3DInput


IMPLEMENT_COMPONENT( CW3DInput, Input );


//
// CW3DInput::CW3DInput
//
CW3DInput::CW3DInput()
{
	m_pInput	= NULL;
	m_bWinMouse	= FALSE;
	
	m_nX			= 0;
	m_nY			= 0;
	m_nWidth		= 0;
	m_nHeight		= 0;

	m_ptMouse.x		= 0;
	m_ptMouse.y		= 0;
	m_nMouseZ		= 0;
	m_ptPrevMouse.x	= 0;
	m_ptPrevMouse.y	= 0;
	m_nPrevMouseZ	= 0;

	m_bMouseL		= FALSE;
	m_bMouseR		= FALSE;
	m_bMouseM		= FALSE;
	m_bOldMouseL	= FALSE;
	m_bOldMouseR	= FALSE;
	m_bOldMouseM	= FALSE;
}

//
// CW3DInput::~CW3DInput
//
CW3DInput::~CW3DInput()
{
	SAFE_DELETE( m_pInput );
}

//
// CW3DInput::CW3DInput
//
BOOL CW3DInput::Init( HINSTANCE hInstance, HWND hWnd, BOOL bWinMouse )
{
	// Init Direct Input
	m_hWnd = hWnd;

	m_pInput = new CDInput;
	m_pInput->Init( hInstance );
	m_pInput->InitKeyboard( hWnd );

	m_bWinMouse = bWinMouse;
	if( !bWinMouse )
	{
		m_pInput->InitMouse( hWnd );
	} //if

	return TRUE;
}

//
// CW3DInput::SetMouseWheel
//
void CW3DInput::SetMouseWheel( WORD wDelta )
{
	if( m_bWinMouse )
	{
		if( wDelta > 32768 )
		{
			m_nPrevMouseZ = -(65536 - wDelta);
		}
		else
		{
			m_nPrevMouseZ = wDelta;
		} //if..else
	} //if
} //CW3DInput::SetMouseWheel

//
// CW3DInput::Update
//
void CW3DInput::Update()
{
	Assert( m_pInput );

	if( m_bWinMouse )
	{
		m_bOldMouseL = m_bMouseL;
		m_bOldMouseR = m_bMouseR;
		m_bOldMouseM = m_bMouseM;

		if( GetAsyncKeyState( 0x01 ) < 0 )	m_bMouseL = TRUE;
		else								m_bMouseL = FALSE;

		if( GetAsyncKeyState( 0x02 ) < 0 )	m_bMouseR = TRUE;
		else								m_bMouseR = FALSE;

		if( GetAsyncKeyState( 0x04 ) < 0 )	m_bMouseM = TRUE;
		else								m_bMouseM = FALSE;


		m_ptPrevMouse	= m_ptMouse;
		GetCursorPos( &m_ptMouse );
		ScreenToClient( m_hWnd, &m_ptMouse );
		m_nMouseZ = m_nPrevMouseZ;
		m_nPrevMouseZ = 0;
	}
	else
	{
		m_bOldMouseL = m_pInput->m_dims.rgbButtons[0];
		m_bOldMouseR = m_pInput->m_dims.rgbButtons[1];
		m_bOldMouseM = m_pInput->m_dims.rgbButtons[2];

		m_pInput->GetMouseState();

		m_ptMouse.x += m_pInput->m_dims.lX;
		m_ptMouse.y += m_pInput->m_dims.lY;
		if( m_nMouseZ	!=  m_pInput->m_dims.lZ )
		{
			m_nMouseZ	=  m_pInput->m_dims.lZ;
		} //
		m_nMouseZ	=  m_pInput->m_dims.lZ;
	} //if..else

	m_pInput->GetKeyboardState();

	if( m_ptMouse.x < m_nX )					m_ptMouse.x = m_nX;
	if( m_ptMouse.y < m_nY )					m_ptMouse.y = m_nY;
	if( m_ptMouse.x >= m_nX + m_nWidth )		m_ptMouse.x = m_nX + m_nWidth  - 1;
	if( m_ptMouse.y >= m_nY + m_nHeight )		m_ptMouse.y = m_nY + m_nHeight - 1;
}
