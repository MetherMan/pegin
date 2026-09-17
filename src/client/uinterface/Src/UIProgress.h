/**
 * 유저 인터페이스용 진행바 모듈.
 *
 * Copyright (c) 2002-2003 팀메이, All rights reserved.
 *
 * @author	파란~*
 * @date	$Date: 04-09-10 11:28p $
 * @version	$Revision: 11 $
 *
 * <HR>
 * $Log: /UInterface/Src/UIProgress.h $
 * 
 * 11    04-09-10 11:28p Paran
 * 
 * 2     03-05-17 4:20a Paran		각 콤포넌트 분리.
 * <HR>
 *
 * @file	UIProgress.h
 */


#ifndef __UIProgress_Header__
#define __UIProgress_Header__


/**
 * 유저 인터페이스 진행바 클래스.
 */
class UIProgress : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIProgress );

	enum
	{
		HORIZONTAL	= 0,
		VERTICAL	= 1
	}; //enum

	UIProgress();
	~UIProgress();

	BOOL		Load( const char* szFileName, const int nRow, const int nCol );
	BOOL		CreateRectangle( const DWORD dwColor );

	void		ReleaseAll();
	void		Cleanup();

	void		SetMinMax( int nMin, int nMax )		{ m_nMax = nMax; m_nMin = nMin; }
	void		SetPos( int nPos, BOOL bProgress = TRUE );
	void		SetType( BOOL bType )				{ m_bType = bType; }
	void		SetColor( const DWORD dwColor )		{ m_dwColor = dwColor; }
	
	int			GetPos()							{ return m_nValue; }
	int			GetMax()							{ return m_nMax; }
	
	void		Update( const DWORD dwTick );
	void		DrawRectangle();
	void		DrawImage();
	void		Draw();

private:
	IW3DSprite*		m_pGraph;			///< Sprite
	IW3DRectangle*	m_pRectangle;		///< Rectangle

	int				m_nMax;					///<
	int				m_nMin;					///<

	int				m_nCurrPos;				///<
	int				m_nGoalPos;				///<
	int				m_nValue;				///< 실재 값

	DWORD			m_dwColor;				///< Color
	
	BOOL			m_bType;				///<
	DWORD			m_dwTick;				///< 현재 틱.
}; //class UIProgress



/**
 * 유저 인터페이스 확장진행바 클래스.
 */
class UIProgressEx : public UICComponent
{
public:
	UNDER_MEMORY_MANGER( UIProgressEx );

	UIProgressEx();
	~UIProgressEx();

	BOOL	Load( const char* szGraphFile, const char* szBackFile, const int nNum );

	void	ReleaseAll();
	void	Cleanup();

	void	SetMinMax( int nMin, int nMax );
	int		GetMax();
	void	SetPos( int nPos, BOOL bProgress = TRUE );
	int		GetPos();

	void	Update( const DWORD dwTick );
	void	Draw();

private:
	IW3DSprite*		m_pGraphBack;			///<
	IW3DSprite*		m_pGraph;				///<

	int				m_nMax;					///<
	int				m_nMin;					///<

	int				m_nCurrPos;				///<
	int				m_nGoalPos;				///<

	int				m_nValue;				///< 실재 값

	int				m_nPartCurrPos;			///<
	int				m_nFillNum;
	int				m_nMaxNum;		
	int				m_nLength;

	DWORD			m_dwTick;
}; //class UIProgressEx


/**
 * 원 그래프 
 */
class UIProgressCircular
{
public:
	UIProgressCircular();
	~UIProgressCircular();

	void		SetColor( const DWORD dwColor )		{ m_dwColor = dwColor; }
	void		SetPosition( int nX, int nY )		{ m_nX = nX; m_nY = nY; }
	void		SetTime( const DWORD dwTick );
	
	void		Update( const DWORD dwTick );
	void		DrawFan( int nPart, int nFrame );
	void		Draw();

private:
	BOOL		Load();

	IW3DSprite*		m_apImage[4];

	int				m_nX;
	int				m_nY;
	DWORD			m_dwColor;
	DWORD			m_dwTotalTick;
	DWORD			m_dwCurTick;
}; //class UIProgressCircular


#endif //__UIProgress_Header__
