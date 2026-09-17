/**
 * 유저 인터페이스용 문자열 입력 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-01 4:21p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIEditText_C.h $
 * 
 * 3     03-12-01 4:21p Sckyj
 * void EditOn(); void EditOff(); 추가
 * 
 * 2     03-11-19 8:08p Sckyj
 * 중국어 IME관련 작업 진행중
 * 
 * 4     03-08-19 11:14a Paran
 * 
 * 3     03-08-14 3:59a Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIEditText_C.h
 */

#ifndef __UIEditText_Header__
#define __UIEditText_Header__

/**
 * 유저 인터페이스 문자열 입력.
 */
class UIEditText : public UICComponent
{
private:
	IW3DFont*		m_pFont;
	DWORD			m_dwAlign;					// 텍스트 정렬방식
	DWORD			m_dwTxtColor;
	std::string		m_strText;
	std::string		m_strResult;
	char			m_szComp[3];
	BOOL			m_bPw;
	UINT			m_nLimit;
	int				m_nShowLimit;

public:
	enum { ALIGN_LEFT = 0, ALIGN_CENTER = 1, ALIGN_RIGHT = 2 };

	UIEditText();

	void			SetMaskMode( BOOL bPW_ )			{ m_bPw = bPW_; }
	BOOL			GetMaskMode()						{ return m_bPw; }
	void			SetFont( IW3DFont* pFont_ )			{ m_pFont = pFont_; }
	void			SetLimit( UINT nLimit_ )			{ m_nLimit = nLimit_; }
	UINT			GetLimit()							{ return m_nLimit; }
	void			SetAlign( int nAlign_);
	void			SetText( const char* szText_ );
	void			SetCompositionText( const char* szComp_ );
	void			SetTxtColor( BYTE r, BYTE g, BYTE b )	  { m_dwTxtColor = 0xff000000 | r << 16 | g << 8 | b; }
	const char*		GetText();

	BOOL			IsHangul( char* pBuff );
	
	char*			GetCompositionText()				{ return m_szComp; }
	void			LeaveFocus();
	
	void			EditOn();
	void			EditOff();
	
	void			Cleanup();
	void			Clear()								{ m_strText = ""; memset( m_szComp, 0, 3 ); }
	
	void			Update( const DWORD dwTick );
	void			Draw();

	UNDER_MEMORY_MGR( UIEditText );
}; //UIEditText

#endif //__UIEditText_Header__