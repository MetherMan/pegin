/********************************************************************

	Progress.cpp

********************************************************************/

#include "stdafx.h"
#include "Progress.h"


//
//	CProgress::CProgress
//
CProgress::CProgress( void )
:m_nWidth( 0 ), m_nHeight( 0 ), m_nRange( 0 ), m_nStep( 0 )
{
}

//
//	CProgress::~CProgress
//
CProgress::~CProgress( void )
{
}

//
//	CProgress::Create
//
BOOL CProgress::Create( int nWidth, int nHeight )
{
	m_nWidth	= nWidth;
	m_nHeight	= nHeight;

	return TRUE;
}

//
//	CProgress::SetRange
//
void CProgress::SetRange( int nRange )
{
	m_nRange	= nRange;
}

//
//	CProgress::SetPos
//
void CProgress::SetPos( int nPos )
{
	// 파일의 크기가 잘못 읽혀질경우 progress bar가 늘어나는 현상방지
	// 어차피 엡뎃 실패 오류가 뜬다.
	nPos = nPos > m_nRange ? m_nRange : nPos;	

	m_nStep	= (int)((float)m_nWidth / (float)m_nRange * nPos);
}

//
//	CProgress::Draw
//
void CProgress::Draw( int x, int y, HDC hDC )
{
	HPEN hPen		= CreatePen( PS_SOLID, m_nHeight, RGB( 206, 63, 14 ) );	
	HPEN hOldPen	= (HPEN)SelectObject( hDC, hPen );

	Rectangle( hDC, x, y, x + m_nStep, y + m_nHeight );

	SelectObject( hDC, hOldPen );
	DeleteObject( hPen );
}
