/**
 * 유저 인터페이스용 타임머 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-01-19 11:27a $
 * @version	$Revision: 4 $
 *
 * <HR>
 * $Log: /UInterface/Src/UITimer.h $
 * 
 * 4     04-01-19 11:27a Paran
 * 
 * 3     03-10-01 4:28p Paran
 * 
 * 2     03-09-23 9:43p Paran
 * 
 * <HR>
 *
 * @file	UITimer.h
 */

#ifndef __UITimer_Header__
#define __UITimer_Header__

extern BOOL		PushEvent( const char* szCtrlName, UIEVENT nEvent, DWORD dwParam1, DWORD dwParam2 );

class UITimer : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UITimer );

	enum
	{
		HORIZONTAL	= 0,
		VERTICAL	= 1
	}; //enum

	/** 
	 * 생성자.
	 */
	UITimer()
		: m_pGraph			( NULL ),
		  m_dwTotalTick		( 0 ),
		  m_dwCurTick		( 0 )
	{
	} //UITimer

	/**
	 * 소멸자.
	 */
	~UITimer()
	{
		SAFE_RELEASE( m_pGraph );
	} //~UITimer

	/** 
	 * 로드.
	 */
	BOOL		Load( const char* szFileName )
	{
		Assert( szFileName );

		m_nWidth	= m_rcRegion.right - m_rcRegion.left;
		m_nHeight	= m_rcRegion.bottom - m_rcRegion.top;

		m_pGraph = MAKE_COMPONENT( Sprite );
		CHECK_PTR_RETURN( m_pGraph );

		if( !m_pGraph->Load( szFileName, m_nWidth, m_nHeight ) )
		{
			Assert( !"그림 읽기 실패" );
			return FALSE;
		} //if

		return TRUE;
	} //Load

	/**
	 * 타입설정.
	 */
	void		SetType( const BYTE btType )
	{
		m_btType = btType;
	} //SetType

	/**
	 * 시간설정.
	 */
	void		SetTime( const DWORD dwTick )
	{
		m_dwTotalTick = dwTick;
		m_dwCurTick   = 0;
	} //SetTime

	/**
	 * 업뎃.
	 */
	void		Update( const DWORD dwTick )
	{
		if( !IsShow() )
		{
			return;
		} //if

		m_dwCurTick += dwTick;
		if( m_dwCurTick > m_dwTotalTick )
		{
			PushEvent( m_strName.c_str(), EVENT_TIMER, m_dwTotalTick, 0 );
			m_dwCurTick = m_dwTotalTick;
			m_pOwnerComp->Show( FALSE );
			return;
		} //if

		switch( m_btType )
		{
		case HORIZONTAL:	
			m_nCurrPos = m_nWidth - ( (float)m_nWidth / m_dwTotalTick ) * m_dwCurTick;
			break;

		case VERTICAL:
			m_nCurrPos = m_nHeight - ( (float)m_nHeight / m_dwTotalTick ) * m_dwCurTick;
			break;
		} //switch
	} //Update

	/**
	 * 그리기.
	 */
	void		Draw()
	{
		if( !IsShow() )
		{
			return;
		} //if

		switch( m_btType )
		{
		case HORIZONTAL:
			m_pGraph->Draw( 
				m_rcRegion.left, m_rcRegion.top, 
				0, 0, 
				m_nCurrPos, m_rcRegion.bottom - m_rcRegion.top  );
			break;

		case VERTICAL:
			m_pGraph->Draw( 
				m_rcRegion.left, m_rcRegion.bottom - m_nCurrPos,
				0, 0,
				m_rcRegion.right - m_rcRegion.left, m_nCurrPos );
			break;
		} //switch
	} //Draw

private:
	IW3DSprite*		m_pGraph;				///< 이미지.

	BYTE			m_btType;				///< 형태타입.
	int				m_nCurrPos;				///< 현재위치.

	DWORD			m_dwTotalTick;			///< 전체 틱.
	DWORD			m_dwCurTick;			///< 현재 틱.

	int				m_nWidth;				///< 가로.
	int				m_nHeight;				///< 세로.
}; //class UITimer

#endif //__UITimer_Header__