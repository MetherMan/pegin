/**
 * 유저 인터페이스용 체크버튼 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-08-14 2:40p $
 * @version	$Revision: 3 $
 *
 * <HR>
 * $Log: /UInterface/Src/UICheckButton.h $
 * 
 * 3     03-08-14 2:40p Paran
 * 
 * 2     03-07-16 7:30p Paran
 * 
 * <HR>
 *
 * @file	UICheckButton.h
 */

#ifndef __UICheckButton_Header__
#define __UICheckButton_Header__

/**
 * 유저 인터페이스 버튼.
 */
class UICheckButton : public UICComponent
{
public:
	UNDER_MEMORY_MGR( UICheckButton );

	enum
	{
		ALIGN_LEFT		= 0,	///< 왼쪽 정렬
		ALIGN_CENTER	= 1,	///< 가운데 정렬
		ALIGN_RIGHT		= 2		///< 오른쪽 정렬
	}; //enum

	BOOL			Load( const char* szFileNameDN, const char* szFileNameUP );

	void			SetAlign( int nAlign_);
	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }
	void			SetText( const char* szText_ )			{ m_strText = szText_; }
	void			SetTxtColor( BYTE r, BYTE g, BYTE b )	{ m_dwTxtColor = 0xff000000 | r << 16 | g << 8 | b; }
	
	const char*		GetText()								{ return m_strText.c_str(); }
	
	BOOL			IsPush()								{ return m_bPush; }
	void			Push( int bPush_ );

	BOOL			IsCheck()								{ return m_bCheck; }
	void			Check( BOOL bCheck );

	void			ReleaseAll();	
	void			Cleanup();
	
	void			Draw();

	UICheckButton();
	~UICheckButton();

private:
	IW3DSprite*		m_pSpriteUP;
	IW3DSprite*		m_pSpriteDN;

	DWORD			m_dwAlign;
	
	BOOL			m_bPush;
	BOOL			m_bCheck;

	IW3DFont*		m_pFont;
	DWORD			m_dwTxtColor;
	std::string		m_strText;
}; //UICheckButton

#endif //__UICheckButton_Header__
