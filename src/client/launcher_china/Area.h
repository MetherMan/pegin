/////////////////////////////////////////////////////
//
// Area.h
// Created By : sckyj
//
/////////////////////////////////////////////////////

#ifndef __Area_Header__
#define __Area_Header__

#include "Servermgr.h"

class CArea : public IMode
{
public :

	CArea();
	~CArea();

	// Init
	BOOL	Init();

	// Clear
	void	Clear();

	// Render
	BOOL	Render();

	// Check Button
	BOOL	LDown( POINT pt );
	BYTE	LUp( POINT pt );

private :

	void	SetPage( int nPage );

	void	DrawBlock( HDC hDC );	
	void	DrawPage( HDC hDC );

	BOOL	AreaBlockLDown( POINT pt );
	BOOL	AreaPageLDown( POINT pt );
	BOOL	AreaBtnLDown( POINT pt );
	BOOL	AreaLUp( POINT pt );

private :

	enum SELECT
	{
		SELECT_TRUE			= 0,
		SELECT_FALSE		= 1,
		SELECT_CHANGEPAGE	= 2
	};

	HFONT			m_hFont;	

	CServer*		m_pServer;

	int				MAX_AREA;
	int				m_nPage;
	int				m_nPageCount;

	// Data
		int			m_nBtnState[2];

		int*		m_pnBlockState;
		int			m_anPageState[4];
		BOOL		m_bSelect;
	//~Data

	// Image
		my::CBitmap		m_bmBack;
		my::CBitmap		m_bmAreaBack;

		my::CBitmap		m_bmBlock[3];		// Released, Selected, NotSurpport

		my::CBitmap		m_bmPage[3];		

		my::CBitmap		m_bmStart[3];		// Released, Pressed, NotSurpport
		my::CBitmap		m_bmCancel[3];			

		RECT			AREA_START;
		RECT			AREA_CANCEL;
		RECT*			pAREA_BLOCK;
		RECT			aAREA_PAGE[4];
	//~Image

}; // class CArea

#endif // #ifndef __Area_Header__