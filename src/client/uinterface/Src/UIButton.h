/**
 * 유저 인터페이스용 버튼 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-10-15 2:41p $
 * @version	$Revision: 6 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIButton.h $
 * 
 * 6     03-10-15 2:41p Paran
 * 
 * 3     03-05-20 9:35p Paran		계속 수정 중...
 * 
 * 2     03-05-17 4:19a Paran		각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIButton.h
 */

#ifndef __UIButton_Header__
#define __UIButton_Header__

/**
 * 유저 인터페이스 버튼.
 */
class UIButton : public UICComponent
{
public:
	UNDER_MEMORY_MGR( UIButton );

	enum
	{
		ALIGN_LEFT		= 0,	///< 왼쪽 정렬
		ALIGN_CENTER	= 1,	///< 가운데 정렬
		ALIGN_RIGHT		= 2		///< 오른쪽 정렬
	}; //enum

	BOOL			Load( const char* szFileNameDN, const char* szFileNameUP, 
						  const char* szFileNameNormal, const char* szFileNameDisable );

	void			SetAlign( int nAlign_);
	void			SetFont( IW3DFont* pFont_ )				{ m_pFont = pFont_; }
	void			SetText( const char* szText_ );
	void			SetInfoText( const char* szText );
	void			SetTxtColor( BYTE r, BYTE g, BYTE b )	{ m_dwTxtColor = 0xff000000 | r << 16 | g << 8 | b; }
	
	const char*		GetText()								{ return m_pText; }
	char*			GetInfoText()							{ return m_pInfoText; }
	
	BOOL			IsPush()								{ return m_bPush; }
	void			Push( int bPush_ );

	void			ReleaseAll();	
	void			Cleanup();
	
	void			Draw();
	UIButton();
	~UIButton();

private:
	IW3DSprite*		m_pSpriteUP;
	IW3DSprite*		m_pSpriteDN;
	IW3DSprite*		m_pSpriteNormal;
	IW3DSprite*		m_pSpriteDisable;

	DWORD			m_dwAlign;
	BOOL			m_bPush;
	IW3DFont*		m_pFont;
	DWORD			m_dwTxtColor;
	char*			m_pText;
	char*			m_pInfoText;
}; //class UIButton

#endif //__UIButton_Header__