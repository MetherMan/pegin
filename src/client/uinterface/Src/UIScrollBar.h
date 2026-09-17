/**
 * 유저 인터페이스용 스크롤 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-11-26 9:15p $
 * @version	$Revision: 7 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIScrollBar.h $
 * 
 * 7     03-11-26 9:15p Paran
 * 
 * 6     03-10-07 9:07p Paran
 * 
 * 2     03-05-17 4:20a Paran			각 콤포넌트 분리.
 * 
 * <HR>
 *
 * @file	UIScrollBar.h
 */

#ifndef __UIScrollBar_Header__
#define __UIScrollBar_Header__

/**
 * 유저 인터페이스 스크롤바.
 */
class UIScrollBar : public UICComponent
{
public:
	enum
	{
		HORIZONTAL	= 0,
		VERTICAL	= 1
	}; //enum
	

	UNDER_MEMORY_MGR( UIScrollBar );

	/**
	 * 생성자.
	 */
	UIScrollBar()
	:	m_pimgThumb		( NULL ),
		m_nMax			( 100 ),
		m_nPos			( 0 ),
		m_bType			( VERTICAL ),
		m_rectBox		( NULL )
	{
		m_pimgArrow[0] = NULL;
		m_pimgArrow[1] = NULL;
	} //UIScrollBar

	/**
	 * 소멸자.
	 */
	~UIScrollBar()
	{
		ReleaseAll();
	} //~UIScrollBar

	BOOL			Init();

	void			SetType( const int nType );
	int				GetType();

	int				GetMax();
	void			SetMax( int nMax );
	
	int				GetScrollPos();
	void			SetScrollPos( int nPos );
	
	void			SetPosition( int nX, int nY, int nLength );

	int				GetThumbY();
	void			MoveThumb( int nMousePos );
	void			ClickBar( const int nX, const int nY );

	void			ReleaseAll();
	void			Cleanup(){};
	
	void			Move( int nOffsetX, int nOffsetY );
	void			Draw();

private:
	IW3DSprite*		m_pimgArrow[2];
	IW3DSprite*		m_pimgThumb;
	IW3DRectangle*	m_rectBox;
	
	RECT			m_rcArrow[2];
	RECT			m_rcThumb;
	RECT			m_rcTrack;

	int				m_nMax;
	int				m_nPos;

	int				m_nTrackW;
	int				m_nTrackH;

	BOOL			m_bType;
}; //class UIScrollBar

#endif //__UIScrollBar_Header__