/**
 * 유저 인터페이스용 시트 헤더.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 03-10-10 7:28p $
 * @version	$Revision: 2 $
 *
 * <HR>
 * $Log: /UInterface/Src/UISheet.h $
 * 
 * 2     03-10-10 7:28p Paran
 * 
 * <HR>
 *
 * @file	UISheet.h
 */
#ifndef __UISheet_Header__
#define __UISheet_Header__

#define SPACE_SHEET				2
#define ONEDATALENGTH			64

class UISheet : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UISheet );

	/**
	 * 생성자.
	 */
	UISheet()
		: m_pFont		( NULL ),
		  m_nLinterval	( 0 ),
		  m_nNumLine	( 0 ),
		  m_pRectangle	( NULL ),
		  m_nSelectedNum( -1 )
	{
//		m_dwColor[0] = 0xFF4B4923;
//		m_dwColor[1] = 0xFF4B3723;
//		m_dwColor[2] = 0xFF23374B;
	} //UISheet

	/**
	 * 소멸자.
	 */
	~UISheet()
	{
		LIST_LINECONTENT::iterator	iter = m_listLineContent.begin();
		while( iter != m_listLineContent.end() )
		{
			SAFE_DELETE_ARRAY( *iter );
			iter = m_listLineContent.erase( iter );
		} //while

		for( UINT nCnt = 0; nCnt < m_vecColTitle.size(); nCnt++ )
		{
			SAFE_FREE( m_vecColTitle[nCnt] );
		} //for
		SAFE_RELEASE( m_pRectangle );
	} //~UISheet

	/**
	 * 클린업.
	 */
	void		Cleanup()
	{
		UICComponent::Cleanup();

		LIST_LINECONTENT::iterator	iter = m_listLineContent.begin();
		while( iter != m_listLineContent.end() )
		{
			SAFE_DELETE_ARRAY( *iter );
			iter = m_listLineContent.erase( iter );
		} //while
	} //Cleanup
	
	/**
	 * 폰트생성.
	 */
	void		SetFont( IW3DFont* pFont )
	{
		m_pFont = pFont;
	} //SetFont

	/** 
	 * 행 설정.
	 */
	void		SetLineNum( const int nLineInterval, const int nNumLine )
	{
		m_nLinterval = nLineInterval;
		m_nNumLine = nNumLine;

		if( !m_pRectangle )
		{
			m_pRectangle = MAKE_COMPONENT( Rectangle );
			if( !m_pRectangle )
			{
				Assert( !"생성실패" );
				return;
			} //if
		} //if
	} //SetLineNum

	/** 
	 * 행 추가.
	 */
	void		AddColumn( const char* szColTitle, const int nColWidth )
	{
		char	*pTitle = (char*)_MemAlloc( 64 );
		if( !pTitle )
		{
			Assert( !"할당실패" );
			return;
		} //if
		strcpy( pTitle, szColTitle );
		
		m_vecColTitle.push_back( pTitle );
		m_vecColWidth.push_back( nColWidth );
	} //AddColumn

	/**
	 * 열 추가.
	 */
	void		AddRow( char* szText )
	{
		if( !szText )
		{
			Assert( !"내용 추가 실패" );
			return;
		} //if

		char*	pContent = new char[m_vecColTitle.size() * ONEDATALENGTH];
		if( !pContent )
		{
			Assert( !"할당실패" );
			return;
		} //if
		
		int		nOffset;
		for( UINT nCnt = 0; nCnt < m_vecColTitle.size(); nCnt++ )
		{
			nOffset = nCnt * ONEDATALENGTH;
			strcpy( pContent + nOffset, szText + nOffset );
		} //for
		m_listLineContent.push_back( pContent );
	} //AddRow

	/**
	 * 현재 선택 라인번호.
	 */
	int			GetSelectedNum()
	{
		return m_nSelectedNum;
	} //GetSelectedNum

	/**
	 * 현재 선택 중인 내용 얻기.
	 */
	char*		GetSelecedContent()
	{
		if( m_nSelectedNum >= m_listLineContent.size() )
		{
			return NULL;
		} //if

		LIST_LINECONTENT::iterator		itor = m_listLineContent.begin();
		std::advance( itor, m_nSelectedNum );
		if( itor == m_listLineContent.end() )
		{
			return NULL;
		} //if
		return *itor;		
	} //GetSelecedContent

	/**
	 * 현재 선택라인 처리.
	 */
	BOOL		PointIn( int nX, int nY )
	{
		if( !IsShow() )
		{
			return FALSE;
		} //if

		POINT		ptPos = { nX, nY };
		RECT		rcDraw;
		for( int nRow = 1; nRow <= m_nNumLine; nRow++ )
		{
			rcDraw.left		= m_rcRegion.left;
			rcDraw.top		= m_rcRegion.top + ( m_nLinterval + SPACE_SHEET ) * nRow;
			rcDraw.right	= m_rcRegion.right;
			rcDraw.bottom	= rcDraw.top + m_nLinterval;

			if( PtInRect( &rcDraw, ptPos ) )
			{
				if( nRow <= m_listLineContent.size() )
				{
					m_nSelectedNum = nRow - 1;
				}
				else
				{
					m_nSelectedNum = -1;
				} //if..else

				break;
			} //if
		} //for
			
		return UICComponent::PointIn( nX, nY );
	} //PointIn

	void		DrawColTitle()
	{
		RECT		rcDraw;
		rcDraw.left		= m_rcRegion.left;
		rcDraw.top		= m_rcRegion.top;
		rcDraw.right	= 0;
		rcDraw.bottom	= rcDraw.top + m_nLinterval;

		for( int nCol = 0; nCol < m_vecColTitle.size(); nCol++ )
		{
			rcDraw.right	= rcDraw.left  + m_vecColWidth[nCol];
			
			m_pRectangle->Draw( 
				rcDraw.left, rcDraw.top, 
				m_vecColWidth[nCol], 
				rcDraw.bottom - rcDraw.top, 
				0xFF4B4923 );
			m_pFont->DrawText( m_vecColTitle[nCol], &rcDraw, DT_CENTER | DT_VCENTER, 0XFFC2C2C2 );
			
			rcDraw.left = rcDraw.right + SPACE_SHEET;
		} //for
	} //DrawColTitle

	/** 
	 * 그리기.
	 */
	void		Draw()
	{
		Assert( m_pFont );
		Assert( m_pRectangle );

		DrawColTitle();

		LIST_LINECONTENT::iterator		iter = m_listLineContent.begin();
		RECT		rcDraw;
		char*		pContent = NULL;
		for( int nRow = 1; nRow <= m_nNumLine; nRow++ )
		{
			rcDraw.left		= m_rcRegion.left;
			rcDraw.top		= m_rcRegion.top + ( m_nLinterval + SPACE_SHEET ) * nRow;
			rcDraw.right	= m_rcRegion.right;
			rcDraw.bottom	= rcDraw.top + m_nLinterval;

			if( m_nSelectedNum == nRow-1 )
			{ //선택 바
				m_pRectangle->Draw( 
					rcDraw.left, rcDraw.top, 
					rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, 
					0XFF2F6662 );
			}
			else
			{ //비선택 바
				m_pRectangle->Draw( 
					rcDraw.left, rcDraw.top, 
					rcDraw.right - rcDraw.left, rcDraw.bottom - rcDraw.top, 
					0XFF1B1B1B );
			} //if..else

			if( iter != m_listLineContent.end() )
			{
				pContent = *iter;
				for( int nCol = 0; nCol < m_vecColWidth.size(); nCol++ )
				{
					rcDraw.right	= rcDraw.left  + m_vecColWidth[nCol];

					m_pFont->DrawText( pContent + ( nCol * ONEDATALENGTH ), &rcDraw, DT_CENTER | DT_VCENTER, 0XFFC2C2C2 );

					rcDraw.left = rcDraw.right + SPACE_SHEET;
				} //for
				iter++;
			} //if
		} //for
	} //Draw

private:
	typedef std::list<char*>		LIST_LINECONTENT;

	IW3DFont*				m_pFont;
	IW3DRectangle*			m_pRectangle;
		
	DWORD					m_dwColor[3];
	int						m_nLinterval;
	int						m_nNumLine;
	int						m_nSelectedNum;
	std::vector<char*>		m_vecColTitle;
	std::vector<int>		m_vecColWidth;
	LIST_LINECONTENT		m_listLineContent;
}; //class UISheet

#endif //__UISheet_Header__