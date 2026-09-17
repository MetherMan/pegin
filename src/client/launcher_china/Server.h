/////////////////////////////////////////////////////
//
// Server.h
// Created By : sckyj
//
/////////////////////////////////////////////////////

#ifndef __Server_Header__
#define __Server_Header__

#include "Servermgr.h"

class CServer
{
public :

	enum { MAX_SERVER = 10 };

	CServer();
	~CServer();

	// Init
	void	Init();

	// Render
	BOOL	Render( HDC hDC, BOOL bTrue );

	// Set Server Info
	void	SetArea( SArea* pArea );	

	// Check Button
	BOOL	LDown( POINT pt );
	BYTE	LUp( POINT pt );

private :

	SArea*	m_pArea;
	int		m_nCount;

	BOOL	m_bSelect;

	HFONT	m_hFont;

	int		m_nBlockState[MAX_SERVER];

	// Image
		my::CBitmap		m_bmServerBack;

		my::CBitmap		m_bmBlock[3];
		RECT			SERVER_BLOCK[MAX_SERVER];
	//~Image	
};

#endif // #ifndef __Server_Header__