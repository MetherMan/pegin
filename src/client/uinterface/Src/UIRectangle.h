/**
 * 유저 인터페이스용 사각형 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-10-10 7:28p $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIRectangle.h $
 * 
 * 4     03-10-10 7:28p Paran
 * 
 * 3     03-09-14 2:53a Paran
 * 
 * 2     03-08-06 10:50p Paran
 * 
 * <HR>
 *
 * @file	UIRectangle.h
 */
#ifndef __UIRectangle_Header__
#define __UIRectangle_Header__

class UIRectangle : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIRectangle );
	
	/**
	 * 생성자.
	 */
	UIRectangle()
	{
		m_rectBox = MAKE_COMPONENT( Rectangle );
		Assert( m_rectBox );

		m_dwDelay	= 0;
		m_dwTick	= 0;
	} //UIRectangle

	/**
	 * 소멸자.
	 */
	~UIRectangle()
	{
		SAFE_RELEASE( m_rectBox );
	} //~UIRectangle

	/**
	 * 사용여부설정.
	 */
	void		Enable( BOOL bEnable )
	{
		m_bEnable = bEnable;
		m_dwTick = 0;
	} //UICComponent::Enable
	
	/**
	 * 사각 색지정.
	 */
	void		SetColor( const DWORD dwColor )
	{
		m_dwColor = dwColor;
	} //SetColor;

	/**
	 * 보여주는 시간을 설정 ('0' - 항상)
	 */
	void		SetDelay( const DWORD dwDelay = 0 )
	{
		m_dwDelay = dwDelay;
		m_dwTick  = m_dwDelay + 1;
		m_bShow   = FALSE;
	} //SetDelay

	/**
	 * 업데이트.
	 */
	void		Update( const DWORD dwTick )
	{
		if( m_dwDelay == 0	||
			!m_bShow		||
			!m_bEnable )	
		{
			return;
		} //if

		m_dwTick += dwTick;
		if( m_dwTick > m_dwDelay )
		{
			m_dwTick = 0;
			m_bEnable = FALSE;
		} //if
	};

	/**
	 * 그리기.
	 */
	void		Draw()
	{
		if( !m_bShow	||
			!m_rectBox	||
			!m_bEnable )
		{
			return;	
		} //if

		int		nWidth  = m_rcRegion.right - m_rcRegion.left;
		int		nHeight = m_rcRegion.bottom - m_rcRegion.top;
		m_rectBox->Draw( m_rcRegion.left, m_rcRegion.top, nWidth, nHeight, m_dwColor );
	} //Draw
	
private:
	IW3DRectangle*			m_rectBox;
	DWORD					m_dwColor;
	
	DWORD					m_dwDelay;
	DWORD					m_dwTick;
}; //UIRectangle

#endif //__UIRectangle_Header__