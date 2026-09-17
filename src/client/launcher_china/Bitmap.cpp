/********************************************************************

	Bitmap.cpp

********************************************************************/


#include "StdAfx.h"
#include "Bitmap.h"


namespace my
{

/**
 * 생성자.
 */
CBitmap::CBitmap()
:	m_hBitmap( NULL )
{
} //CBitmap::CBitmap

/**
 * 소멸자.
 */
CBitmap::~CBitmap( void )
{
	if( m_hBitmap )
	{
		DeleteObject( m_hBitmap );
		m_hBitmap = NULL;
	} //if
} //CBitmap::~CBitmap

/**
 * 비트맵 파일을 읽는다.
 */
HRESULT CBitmap::Load( LPCSTR szFile )
{
	m_hBitmap = (HBITMAP)LoadImage(	NULL,
									szFile,
									IMAGE_BITMAP,
									0,
									0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION );

	if( NULL == m_hBitmap )
	{
		return E_FAIL;
	} //if	

	BITMAP	bitmap;
	if( GetObject( m_hBitmap, sizeof( BITMAP ), &bitmap ) == NULL )
	{
		return E_FAIL;
	} //if

	m_nWidth	= bitmap.bmWidth;
	m_nHeight	= bitmap.bmHeight;

	return S_OK;
} //CBitmap::Load

HRESULT CBitmap::ReLoad( LPCTSTR szFile )
{
	if( m_hBitmap != NULL )
	{
		DeleteObject( m_hBitmap );
		m_hBitmap = NULL;
	}

	return Load( szFile );
} // CBitmap::ReLoad

/**
 * 비트맵을 그린다.
 */
HRESULT	CBitmap::Draw( int x, int y, HDC hDest )
{
	if( !m_hBitmap )	return E_FAIL;

	HDC hBitmapDC = CreateCompatibleDC( hDest );
	SelectObject( hBitmapDC, m_hBitmap );

	BitBlt( hDest, x, y, m_nWidth, m_nHeight, hBitmapDC, 0, 0, SRCCOPY );

	DeleteDC( hBitmapDC );

	return S_OK;
} //CBitmap::Draw

} //namespace my
