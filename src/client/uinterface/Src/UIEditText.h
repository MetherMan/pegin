/**
 * 유저 인터페이스용 문자열 입력 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-12-24 6:09p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIEditText.h $
 * 
 * 7     03-12-24 6:09p Paran
 * 
 * 6     03-11-28 1:46p Paran
 * 
 * 5     03-11-25 10:25p Paran
 * IME 기능수정 및 추가
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
 * @file	UIEditText.h
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
	char*			m_pText;
	char*			m_pResult;
	char			m_szComp[3];
	BOOL			m_bEditing;
	BOOL			m_bPw;
	UINT			m_nLimit;
	int				m_nShowBegin;
	int				m_nShowEnd;
	BOOL			m_btIsID;

public:
	enum { ALIGN_LEFT = 0, ALIGN_CENTER = 1, ALIGN_RIGHT = 2 };

	UIEditText();
	~UIEditText();

	void			SetMaskMode( BOOL bPW_ )			{ m_bPw = bPW_; }
	BOOL			GetMaskMode()						{ return m_bPw; }
	void			SetFont( IW3DFont* pFont_ )			{ m_pFont = pFont_; }
	IW3DFont*		GetFont()							{ return m_pFont; }
	void			SetLimit( UINT nLimit_ );
	UINT			GetLimit()							{ return m_nLimit; }
	void			SetAlign( int nAlign_);

	void			SetText( const char* szText_ );
	void			InsertText( const char* szText, int nInsertPos );
	void			DeleteText( int nDeletePos, int nDeleteCnt );
	void			SetCompositionText( const char* szComp_ );
	void			SetTxtColor( BYTE r, BYTE g, BYTE b )	  { m_dwTxtColor = 0xff000000 | r << 16 | g << 8 | b; }
	char*			GetText();

	void			ResetShowCharPos();
	char*			GetCompositionText()				{ return m_szComp; }

	void			EditOn();
	void			EditOff();
	
	void			Cleanup();
	
	void			Reflesh();
	void			Draw();

	void			SetLoginMode( BOOL btIs ) { m_btIsID = btIs;};
	BOOL			GetLoginMode(  ){ return m_btIsID; };

	UNDER_MEMORY_MGR( UIEditText );
}; //UIEditText

#endif //__UIEditText_Header__