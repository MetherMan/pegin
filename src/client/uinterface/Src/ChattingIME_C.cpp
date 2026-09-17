/**
 * IME 모듈 소스 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-04 9:13p $
 * @version	$Revision: 9 $
 *
 * <HR>
 * $Log: /UInterface/Src/ChattingIME.cpp $
 * 
 * 9     03-12-04 9:13p Sckyj
 * SetConversion() 추가
 * 
 * 8     03-12-03 2:36p Sckyj
 * SetContext()함수 추가 - 채팅 모드가 아닐경우는 IME Context를
 * Disable시킨다
 * 
 * 7     03-11-26 11:40p Paran
 * 
 * 6     03-11-25 10:25p Paran
 * IME 기능수정 및 추가
 * 
 * 5     03-10-07 9:07p Paran
 * 
 * 4     03-10-07 9:24a Kjmgo
 * MessageProc부분에서 m_hWnd 깨졌을때 리턴하던것 주석처리( 임시 )
 * 
 * 3     03-06-28 5:06p Paran
 * 
 * 2     03-05-17 3:43p Paran
 * 
 * 1     03-05-17 12:00a Paran
 * 
 * 21    03-04-04 5:15a Ahastudio
 * <HR>
 *
 * @file	ChattingIME.cpp
 */


#pragma comment( lib, "IMM32.LIB" )


#include "UInterface_Export.h"
#include "Chattingime_C.h"

CChatIME		g_ChatIME;

/**
 * 생성자.
 */
CChatIME::CChatIME()
:	m_xInsertPos	( 0 ),
	m_xEndPos		( 0 ),
	m_hIMC			( NULL ),
	m_hWnd			( NULL ),
	m_pCaretRect	( NULL ),
	m_hKeyLayout	( NULL ),
	m_property		( 0 ),
	m_bStart		( FALSE ),
	m_nCompLen		( 0 ),
	m_nCompWinX		( -100 ),
	m_nCompWinY		( -100 )
{
    memset( m_szBuffer,		  0, MAX_CHAR );
    memset( m_szBufferBackup, 0, MAX_CHAR );	
	memset( m_szTextComp,	  0, 3 );
} //CChatIME::CChatIME

/**
 * 소멸자.
 */
CChatIME::~CChatIME()
{
	Release();
} //CChatIME::~CChatIME

/**
 * 해제
 */
void CChatIME::Release()
{
	SAFE_RELEASE( m_pCaretRect );
} //CChatIME::Release

/**
 * IME 초기화.
 *
 * @param	hWnd	[IN] 윈도우 핸들.
 *
 * @return	초기화 성공 여부.
 */
BOOL CChatIME::Init( HWND hWnd )
{
	Assert( hWnd );
	SAFE_RELEASE( m_pCaretRect );

	m_hWnd			= hWnd;
	m_hKeyLayout	= GetKeyboardLayout( 0 );
	m_property		= ImmGetProperty( m_hKeyLayout, IGP_PROPERTY );

	m_pCaretRect = MAKE_COMPONENT( Rectangle );
	if( !m_pCaretRect )
	{
		Assert( !"할당실패" );
		return FALSE;
	} //if

	return TRUE;
} //CChatIME::Init

/**
 * IME용 윈도우 메세지 프로세스.
 */
BOOL CChatIME::MessageProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
/*	if( !m_hWnd )
	{
	//	Assert( m_hWnd );
		return FALSE;
	} //if*/

	switch( msg ) 
	{ 
	case WM_IME_STARTCOMPOSITION:     // 글자 조합의 시작. WM_IME_COMPOSITION 메시지를 받을 준비를 한다. 
		IMEStartComposition( wParam, lParam );
		return TRUE;  

	case WM_IME_ENDCOMPOSITION:       // 글자 조합의 끝. 조합된 문자열 처리를 끝낸다
		IMEEndComposition( wParam, lParam );
		return TRUE;    

	case WM_IME_COMPOSITION:          // 현재 조합중인 문자열이나 완성된 문자열을 얻어서 화면 출력 
		IMEComposition( wParam, lParam );
		return TRUE;   

	case WM_IME_SETCONTEXT:			  // 글자 조합 윈도우와 한자 변환 윈도우를 표시하지 않게 바꿈 
		IMESetContext( wParam, lParam );
		return TRUE;

	case WM_IME_NOTIFY:        
		IMENotify( wParam, lParam );
		return TRUE;             

	case WM_INPUTLANGCHANGE:		  // 키보드 레이아웃이 바뀌는 경우 IME를 초기화      
		InputLangChange( wParam,lParam );
       return TRUE;    

	case WM_IME_CONTROL:
		IMEControl( wParam, lParam );
		return TRUE;

	case WM_IME_COMPOSITIONFULL:
		IMECompositionFull( wParam, lParam );
		return TRUE;

	case WM_CHAR:
		return Char( wParam );

	case WM_KEYDOWN:
		return IMEKeyProcess( msg, (UINT)wParam, (UINT)lParam );
    } //switch

	return FALSE;
} //CChatIME::MessageProc

/**
 * Key Process 
 */
BOOL CChatIME::IMEKeyProcess( UINT nChar, UINT nKeyCode, UINT nKeyData ) 
{
	// Do not accept char during composition if IME property does not have IME_PROP_AT_CARET
	if ( !( m_property & IME_PROP_AT_CARET ) && m_bStart )
		return FALSE;

	int nDelLen = 0;

	switch( nKeyCode )
	{
		// Left arrow key
		case VK_LEFT :
			if( m_xInsertPos == 0 ) break;

			if( IsDoubleByteCharset( &m_szBuffer[m_xInsertPos-2] ) )
			{
				m_xInsertPos -= 2;
			}
			else
			{
				m_xInsertPos -= 1;
			}			
		
		//	ShowCaretOnView();
			break;

		// Right arrow key
		case VK_RIGHT:
			if( m_xInsertPos == m_xEndPos ) break;

			if( IsDoubleByteCharset( &m_szBuffer[m_xInsertPos] ) )
			{
				m_xInsertPos += 2;
			}
			else
			{
				m_xInsertPos ++;
			}				

		//	ShowCaretOnView();
			break;

		// Home key
		case VK_HOME:
			if( m_xInsertPos == 0 ) break;
			m_xInsertPos = 0;
			
		//	ShowCaretOnView();
			break;

		// End key
		case VK_END:
			if( m_xInsertPos == m_xEndPos ) break;
			m_xInsertPos = m_xEndPos;
		
		//	ShowCaretOnView();
			break;

		// Delete key
		case VK_DELETE:
			if( m_xInsertPos == m_xEndPos ) break;

			if( IsDoubleByteCharset( &m_szBuffer[m_xInsertPos] ) )
			{
				nDelLen = 2;
			}
			else
			{
				nDelLen = 1;
			}
			
			memcpy( m_szBufferBackup, m_szBuffer + m_xInsertPos + nDelLen, m_xEndPos - m_xInsertPos - nDelLen );
			memcpy( m_szBuffer + m_xInsertPos, m_szBufferBackup, m_xEndPos - m_xInsertPos - nDelLen );

			m_xEndPos -= nDelLen;
			m_szBuffer[m_xEndPos] = 0;
			
			break;
	}
	
	return TRUE;
} // CChatIME::IMEKeyProcess( UINT nChar, UINT nRepCnt, UINT nFlags ) 

/**
 * 조합 시작.
 */
BOOL CChatIME::IMEStartComposition( WPARAM dwCommand, LPARAM dwData )
{
	if( Check() ) 
	{
		m_bStart = TRUE;
		m_nCompLen = 0;
		//IMEStartComposition( dwData );
		return TRUE;
	} 
	return FALSE;
} //CChatIME::IMEStartComposition

/**
 * 조합 끝.
 */	
BOOL CChatIME::IMEEndComposition( WPARAM dwCommand, LPARAM dwData )
{
	if( Check() ) 
	{
		m_nCompLen	= 0;
		m_bStart	= FALSE;
		return TRUE;
	} 
	else
	{
		return FALSE;
	} //if..else..
} //CChatIME::IMEEndComposition

/**
 * 조합.
 */
BOOL CChatIME::IMEComposition( WPARAM dwCommand, LPARAM dwData )
{
	if( Check() ) 
	{
		if( dwData & GCS_RESULTSTR )
		{
			GetResultString();
		}
		else if( dwData & GCS_COMPSTR )
		{
			GetCompString( dwData );
		} //if..else if..
	} //if

	return 0;
} //CChatIME::IMEComposition

/**
 * Context 설정.
 */
BOOL CChatIME::IMESetContext( WPARAM dwCommand, LPARAM dwData )
{
	if( m_property & IME_PROP_AT_CARET ) 
	{
		dwData &= ~( ISC_SHOWUICOMPOSITIONWINDOW | ISC_SHOWUIALLCANDIDATEWINDOW );
	} //if

	return DefWindowProc( m_hWnd,WM_IME_SETCONTEXT, dwCommand, dwData );
} //CChatIME::IMESetContext

/**
 * Notify 처리.
 */
BOOL CChatIME::IMENotify( WPARAM dwCommand, LPARAM dwData )
{
    switch( dwCommand ) 
	{
    case IMN_OPENCANDIDATE:
		return OpenCandidate( dwData );

    case IMN_CLOSECANDIDATE:
		return CloseCandidate( dwData );

    case IMN_CHANGECANDIDATE:
		return ChangeCandidate( dwData );

    case IMN_SETOPENSTATUS:
		return SetOpenStatus();
    } //switch

    return DefWindowProc( m_hWnd,WM_IME_NOTIFY, dwCommand, dwData );
} //CChatIME::IMENotify

/**
 * 언어 변경.
 */
LONG CChatIME::InputLangChange( WPARAM dwCommand, LPARAM dwData )
{
    if( ImmIsIME( m_hKeyLayout ) && m_property & IME_PROP_AT_CARET )
	{
		ClearData();
	} //if

	m_hKeyLayout	= GetKeyboardLayout( 0 );
	m_property		= ImmGetProperty( m_hKeyLayout, IGP_PROPERTY );

	if( Enter() ) 
	{
		Leave();
	} //if

	return DefWindowProc( m_hWnd, WM_INPUTLANGCHANGE, dwCommand, dwData );
} //CChatIME::InputLangChange

/**
 * IME 컨트롤.
 */
LONG CChatIME::IMEControl( WPARAM dwCommand, LPARAM dwData )
{
	return 0;
} //CChatIME::IMEControl

/**
 * 조합 Full.
 */
LONG CChatIME::IMECompositionFull( WPARAM dwCommand, LPARAM dwData )
{
	return 0;
} //CChatIME::IMECompositionFull

/**
 * 문자 입력 처리.
 */
BOOL CChatIME::Char( UINT nChar )
{
	if( ( nChar == 8 ) || ( nChar == 27 ) )
	{
		if( m_xInsertPos >= 2 )
		{
			if( IsDoubleByteCharset( &m_szBuffer[m_xInsertPos-2] ) )
			{
				memcpy( m_szBufferBackup, m_szBuffer + m_xInsertPos, m_xEndPos - m_xInsertPos );
				memcpy( m_szBuffer + m_xInsertPos - 2, m_szBufferBackup, m_xEndPos - m_xInsertPos );

				memset( &m_szBuffer[m_xEndPos-2], 0, 2 );
				m_xInsertPos -= 2;
				m_xEndPos	 -= 2;
				return TRUE;
			} //if
		} //if
		if( m_xInsertPos )
		{
			memcpy( m_szBufferBackup, m_szBuffer + m_xInsertPos, m_xEndPos - m_xInsertPos );
			memcpy( m_szBuffer + m_xInsertPos - 1, m_szBufferBackup, m_xEndPos - m_xInsertPos );

			memset( &m_szBuffer[m_xEndPos - 1], 0, 1 );
				
			m_xEndPos -= 1;	
			m_xInsertPos -= 1;
		} //if
	}
	else if( nChar == 13 )		///< Enter
	{
		return FALSE;		
	}
	else if( nChar == 9 )		///< Tab
	{
		return FALSE;
	}
	else
	{
		char str[2];
		str[0] = nChar & 0xff;
		str[1] = 0;
		PutString( str );
	} //if..else if..else..

	return TRUE;
} //CChatIME::Char

/**
 * 문자열 추가.
 */
void CChatIME::PutString( LPSTR str )
{
	int		iLen = strlen( str );

	if( ( iLen + m_xEndPos ) <= (int)m_nLimit )
	{
		memcpy( m_szBufferBackup, m_szBuffer + m_xInsertPos, m_xEndPos - m_xInsertPos );
		memset( &m_szBuffer[m_xInsertPos], 0, m_xEndPos - m_xInsertPos );
	
		strncat( m_szBuffer + m_xInsertPos, str, iLen );
		strncat( m_szBuffer + m_xInsertPos + iLen, m_szBufferBackup, m_xEndPos - m_xInsertPos );

		m_xEndPos += iLen;
		m_xInsertPos += iLen;
	} //if
} //CChatIME::PutString

/**
 * 문자열을 얻는다.
 */
char* CChatIME::GetString()
{
	return m_szBuffer;
} //CChatIME::GetString

/**
 * 조합 문자를 얻는다.
 */
char* CChatIME::GetCompositionString()
{
	return m_szTextComp;
} //CChatIME::GetCompositionString

/**
 * 문자열 설정.
 */
void CChatIME::SetString( const char* szString )
{
	strcpy( m_szBuffer, szString );
	m_xEndPos = strlen( szString );
	m_xInsertPos = m_xEndPos;
} //CChatIME::SetString

/**
 * 처리 데이터를 지운다.
 */
void CChatIME::ClearData()
{
	m_xInsertPos = m_xEndPos = 0;
    memset( m_szBuffer, 0, MAX_CHAR );
	memset( m_szTextComp, 0, 3 );
} //CChatIME::ClearData

/**
 * 데이터 처리 완료.
 */
void CChatIME::EndData()
{
	m_nCompLen = 0;
	if( Enter() ) 
	{
		ImmSetCompositionString( m_hIMC, SCS_SETSTR, 0, 0, 0, 0 );
		Leave();
	} //if
} //CChatIME::EndData

/**
 * 한자 등 Candidate를 연다.
 */
BOOL CChatIME::OpenCandidate( LONG lParam )
{
    if( !Enter() )
	{
        return FALSE;
	}
	else
	{
		return TRUE;
	} //if..else..
} //CChatIME::OpenCandidate

/**
 * 한자 등 Candidate를 닫는다.
 */
BOOL CChatIME::CloseCandidate( LONG CandList )
{
	return TRUE;
} //CChatIME::CloseCandidate

/**
 * 한자 등 Candidate를 바꾼다.
 */
BOOL CChatIME::ChangeCandidate( LONG CandList )
{
	if( !Enter() )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	} //if..else..
} //CChatIME::ChangeCandidate

/**
 * 열린 상태를 설정한다.
 */
BOOL CChatIME::SetOpenStatus()
{
    if( !Enter() )
	{
        return FALSE;
	} //if

    if( ImmGetOpenStatus( m_hIMC ) ) 
	{
    } 
	else 
	{
		m_nCompLen = 0;
    } //if..else..

	Leave();

	return TRUE;
} //CChatIME::SetOpenStatus

/**
 * 문자열 결과를 얻는다.
 *
 * @return	성공 여부.
 */
BOOL CChatIME::GetResultString()
{
	DWORD	len;	// Storage for length of result str.
	LPSTR	str;	// Pointer to result string.

	if( Enter() )
	{
		if( ( len = ImmGetCompositionString( m_hIMC, GCS_RESULTSTR, NULL, 0 ) ) > 0 ) 
		{
			str = new char[len + 1];
			ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, str, len);
			str[len] = 0;
			ProcessResultString(str);
			delete str;
		} //if
		Leave();
	} //if

	return TRUE;
} //CChatIME::GetResultString

/**
 * Get Comp String.
 */
BOOL CChatIME::GetCompString( LONG flag )
{
	DWORD	len;			// Stogare for len. of composition str
	LPSTR	str;			// Pointer to composition str.
	LPSTR	strAttr;		// Pointer to composition str array.
	DWORD	lenAttr;

	if( !Enter() )
	{
		return FALSE;
	} //if

    if( ( len = ImmGetCompositionString( m_hIMC, GCS_COMPSTR, NULL, 0 ) ) > 0 ) 
	{
		str = new char[len + 1];

		ImmGetCompositionString( m_hIMC, GCS_COMPSTR, str, len );

		str[len] = 0;
		strAttr = NULL;

		if( flag & GCS_COMPATTR )
		{
			if( ( lenAttr = ImmGetCompositionString( m_hIMC, GCS_COMPATTR, NULL, 0 ) ) > 0 ) 
			{
				strAttr = new char[lenAttr + 1];
				ImmGetCompositionString( m_hIMC, GCS_COMPATTR, strAttr, lenAttr );
				strAttr[lenAttr] = 0;
			} //if
		} //if

		// Override function
		ProcessCompString( str, strAttr );

		m_nCompLen = (UINT)len;

		SAFE_DELETE( strAttr );
		SAFE_DELETE( str );
	}
	else
	{
		memset( m_szTextComp, 0, 3 );
		m_nCompLen = 0;
	} //if..else..

	Leave();

	return TRUE;
} //CChatIME::GetCompString

/**
 * 문자열 결과를 처리한다.
 */
void CChatIME::ProcessResultString( LPSTR str )
{
	PutString( str );
	memset( m_szTextComp, 0, 3 );
} //CChatIME::ProcessResultString

/**
 * Process Comp String.
 */
void CChatIME::ProcessCompString( LPSTR str, LPSTR strAttr )
{
	if( lstrlen( str ) <= 2 )
	{
		strcpy( m_szTextComp, str );
	} //if
} //CChatIME::ProcessCompString

/**
 * 조합 폰트를 설정한다.
 */
BOOL CChatIME::SetCompositionFont( LPLOGFONT lf )
{
	BOOL	bRet;

	if( Enter() ) 
	{
		bRet = ImmSetCompositionFont( m_hIMC, lf );
		Leave();
	}
	else
	{
		bRet = FALSE;
	} //if..else..

	return bRet;
} //CChatIME::SetCompositionFont

/**
 * 커서 위치를 얻는다.
 */
int CChatIME::GetCompCursorPos()
{
	if( !Enter() )
	{
		return 0;
	} //if

	int		ret = LOWORD( ImmGetCompositionString( m_hIMC, GCS_CURSORPOS, NULL, 0 ) );

	Leave();

	return ret;
} //CChatIME::GetCompCursorPos

/**
 * 제한을 건다.
 */
void CChatIME::SetLimit( UINT nLimit )
{
	m_nLimit = nLimit;
} //CChatIME::SetLimit

/**
 * WM_IME_SETCONTEXT
 */
void CChatIME::SetContext( BOOL bTrue )
{
	SendMessage( m_hWnd, WM_IME_SETCONTEXT, ( WPARAM )bTrue, NULL );
} // CChatIME::SetContext

/**
 *
 */
void CChatIME::SetConversion( BOOL bTrue )
{
	DWORD dwConv, dwSent;
	if( Enter() )
	{
		ImmGetConversionStatus( m_hIMC, &dwConv, &dwSent );

		ImmSetConversionStatus( m_hIMC, bTrue, dwSent );

		Leave();
	}
} // CChatIME::SetConversion

/**
 * Enter.
 */
BOOL CChatIME::Enter() 
{
	if( m_hWnd != NULL )
	{
		m_hIMC = ImmGetContext( m_hWnd );
		return (BOOL)m_hIMC;
	}
	else 
	{
		return FALSE;
	} //if..else..
} //CChatIME::Enter

/**
 * Leave.
 */
void CChatIME::Leave() 
{
	ImmReleaseContext( m_hWnd, m_hIMC );
} //CChatIME::Leave

/**
 * Check.
 */
BOOL CChatIME::Check() 
{
	if( !( m_property & IME_PROP_SPECIAL_UI ) &&
		( m_property & IME_PROP_AT_CARET ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	} //if..else..
} //CChatIME::Check

/**
 * 문자 입력 상태 설정.
 */
void CChatIME::SetCharMode()
{
	ImmSetConversionStatus( m_hIMC, 0, 0 );
} //CChatIME::SetCharMode

/**
 * 문자 삽입 위치를 얻는다
 */
int CChatIME::GetInsertPos()
{
	return m_xInsertPos;
} //CChatIME::GetInsertPos

/**
 * Set Composition Window Position to Current Caret( insert ) position
 */
void CChatIME::SetCompositionWindow()
{
	COMPOSITIONFORM cf;

	if( Enter() )
	{	
		cf.dwStyle = CFS_POINT;
		cf.ptCurrentPos.x = m_nCompWinX;
		cf.ptCurrentPos.y = m_nCompWinY;
		
		ImmSetCompositionWindow( m_hIMC, &cf );

		Leave();
	}
} //CChatIME::SetCompositionWindow

/**
 * 캐럿 위치 설정
 */
void CChatIME::SetCaretPos( const int& nX, const int& nY )
{
	m_nCompWinX = nX;
	m_nCompWinY = nY;
} //CChatIME::SetCaretPos

/**
 * 그리기
 */
void CChatIME::Draw( DWORD dwTick )
{
	if( !m_pCaretRect )
	{
		Assert( !"없다" );
		return;
	} //if

	static DWORD	s_dwTick = 0;
	static BOOL		s_bShow  = FALSE;

	s_dwTick += dwTick;
	if( s_dwTick > 500 )
	{
		s_dwTick = 0;
		s_bShow = !s_bShow;
	} //if

	if( s_bShow )
	{
		m_pCaretRect->Draw( m_nCompWinX, m_nCompWinY, 2, 12, 0xFFC5C5C5 );
	} //if
} //CChatIME::Draw

/**
 * 한글인지 얻는다.
 */
BOOL IsDoubleByteCharset( const char* pBuff )
{
	if( IsDBCSLeadByte( pBuff[0] ) && 
		IsDBCSLeadByte( pBuff[1] ) )
	{
		return TRUE;
	}

	return FALSE;
} //IsDoubleByteCharset