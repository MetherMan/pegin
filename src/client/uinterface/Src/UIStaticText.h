/**
 * 유저 인터페이스용 정적 텍스트 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-08-14 2:40p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIStaticText.h $
 * 
 * 3     03-08-14 2:40p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIStaticText.h
 */

#ifndef __UIStaticText_Header__
#define __UIStaticText_Header__

/**
 * 유저 인터페이스 정적 문자열.
 */
class UIStaticText : public UICComponent
{
private:
	IW3DFont*		m_pFont;
	DWORD			m_dwAlign;					// 텍스트 정렬방식
	std::string		m_strText;
	DWORD			m_dwTxtColor;

public:
	enum { ALIGN_LEFT = 0, ALIGN_CENTER = 1, ALIGN_RIGHT = 2 };
	UIStaticText();
	
	void			SetFont( IW3DFont* pFont_ )			{ m_pFont = pFont_; }
	void			SetAlign( int nAlign_);
	void			SetText( const char* szText_ );
	char*			GetText()							{ return (char*)m_strText.c_str(); }
	void			SetTxtColor( BYTE r, BYTE g, BYTE b )	{ m_dwTxtColor = 0xff000000 | r << 16 | g << 8 | b; }
	void			Cleanup();
	void			Draw();

	UNDER_MEMORY_MGR( UIStaticText );
};

#endif //__UIStaticText_Header__