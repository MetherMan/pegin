/**
 * 유저 인터페이스용 스트링리스트 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-11-29 2:29p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIStringList.h $
 * 
 * 7     03-11-29 2:29p Paran
 * 
 * 6     03-11-28 1:46p Paran
 * 
 * 5     03-08-19 11:14a Paran
 * 
 * 4     03-08-14 2:40p Paran
 * 
 * 3     03-07-23 3:14p Paran
 * 
 * 2     03-05-17 4:20a Paran
 * 각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIStringList.h
 */

#ifndef __UIStringList_Header__
#define __UIStringList_Header__

/**
 * 유저 인터페이스 문자열 리스트.
 */
class UIStringList : public UICComponent
{
public:
	UNDER_MEMORY_MGR( UIStringList )

	enum
	{
		ALIGN_LEFT		= 0,
		ALIGN_CENTER	= 1,
		ALIGN_RIGHT		= 2
	}; //enum

	UIStringList();
	~UIStringList()
	{
	} //~UIStringList

	void			SetText( const char* szContent, DWORD dwColor = 0, BYTE btFontIndex = 0 );
	void			AddString( const char* szContent, DWORD dwColor = 0, BYTE btFontIndex = 0 );

	void			SetAlign( int nAlign_);
	void			SetFont( IW3DFont* pFont, BYTE btIndex );

	void			SetCount( int nDisplayRow_ );
	void			SetMax( int nMax_ );
	
	int				GetListSize();
	int				GetDisplayLineCount();
	void			SetPos( int nPos_ );
	
	void			SetTxtColor( BYTE r, BYTE g, BYTE b )	{ m_dwTxtColor = 0xff000000 | r << 16 | g << 8 | b; }
	
	void			Cleanup();
	void			Draw();

private:
	LIST_COLORSTRING	m_listString;
	IW3DFont*			m_apFont[4];
	DWORD				m_dwTxtColor;
	int					m_nMaxCount;
	int					m_nDisplayRow;
	int					m_nScrollPos;
	int					m_RowHeight;
	DWORD				m_dwAlign;
}; //class UIStringList


#endif //__UIStringList_Header__