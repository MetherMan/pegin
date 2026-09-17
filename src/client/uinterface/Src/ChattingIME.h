/**
 * IME 모듈 헤더 파일.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-04-30 8:30p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /UInterface/Src/ChattingIME.h $
 * 
 * 11    04-04-30 8:30p Paran
 * 
 * 10    03-12-21 11:14p Paran			버퍼사이즈 1024바이트로
 * 
 * 8     03-12-04 9:13p Sckyj			SetConversion() 추가
 * 
 * 7     03-12-03 2:36p Sckyj			SetContext()함수 추가 - 채팅 모드가 아닐경우는 IME Context를 Disable시킨다
 * 
 * 5     03-11-25 10:25p Paran			IME 기능수정 및 추가
 * 
 * <HR>
 *
 * @file	ChattingIME.h
 */


#ifndef __ChattingIME_Header__
#define __ChattingIME_Header__


#include <imm.h>

#define MAX_CHAR				1024

/**
 * 채팅용 IME 클래스.
 * @ingroup		Engine
 */
class CChatIME
{
public:
	UNDER_MEMORY_MGR( CChatIME );

	CChatIME();
	~CChatIME();

	BOOL				Init( HWND hWnd );
	void				Release();
	BOOL				MessageProc( UINT msg, WPARAM wParam, LPARAM lParam );
	void				Draw( DWORD dwTick );

	void				SetLimit( UINT nLimit );
	void				SetContext( BOOL bTrue );// WM_IME_SETCONTEXT
	void				SetConversion( BOOL bTrue );

	void				PutString( LPSTR str );
	char*				GetString();
	char*				GetCompositionString();
	void				EndData();
	void				ClearData();
	void				SetString( const char* szString );
	void				SetCharMode();

	int					GetInsertPos();

	void				SetCaretPos( const int& nX, const int& nY );

private:
	HIMC				m_hIMC;
	HWND				m_hWnd;
	HKL					m_hKeyLayout;
	DWORD				m_property;
	IW3DRectangle*		m_pCaretRect;

	char				m_szBuffer[MAX_CHAR];
	char				m_szBufferBackup[MAX_CHAR];
	char				m_szTextComp[3];
	
	UINT				m_nLimit;

	BOOL				m_bStart;
	int					m_xInsertPos;
	int					m_xEndPos;
	int					m_nCompLen;

	int					m_nCompWinX;
	int					m_nCompWinY;

	BOOL				IMEStartComposition( WPARAM dwCommand, LPARAM dwData );
	BOOL				IMEEndComposition( WPARAM dwCommand, LPARAM dwData );
	BOOL				IMEComposition( WPARAM dwCommand, LPARAM dwData );
	BOOL				IMESetContext( WPARAM dwCommand, LPARAM dwData );
	BOOL				IMENotify( WPARAM dwCommand, LPARAM dwData );
	LONG				InputLangChange( WPARAM dwCommand, LPARAM dwData );
	LONG				IMEControl( WPARAM dwCommand, LPARAM dwData );
	LONG				IMECompositionFull( WPARAM dwCommand, LPARAM dwData );

	BOOL				OpenCandidate(LONG lParam);
	BOOL				CloseCandidate( LONG CandList );
	BOOL				ChangeCandidate( LONG CandList );
	BOOL				SetCompositionFont( LPLOGFONT lf );
	BOOL				SetOpenStatus();
	BOOL				GetResultString();
	BOOL				GetCompString( LONG flag );
	void				ProcessResultString( LPSTR str );
	void				ProcessCompString( LPSTR str, LPSTR strAttr );
	int					GetCompCursorPos();
	BOOL				IMEKeyProcess( UINT nChar, UINT nKeyCode, UINT nKeyData );
	void				SetCompositionWindow();

	inline BOOL			Enter();
	inline void			Leave();
	inline BOOL			Check();

	BOOL				Char( UINT nChar );
}; //class CChatIME

extern CChatIME		g_ChatIME;

#endif // #ifndef __ChattingIME_Header__
