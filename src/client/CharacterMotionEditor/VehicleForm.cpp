#include "StdAfx.h"
#include "VehicleForm.h"

/**
 *
 */
CVehicleForm::CVehicleForm()
{
	static DWORD dwId = 5000;

	m_dwId = dwId;
	dwId += 100;
} //CVehicleForm::CVehicleForm

/**
 *
 */
CVehicleForm::~CVehicleForm()
{

} //CVehicleForm::~CVehicleForm

/**
 *
 */
BOOL CVehicleForm::Create( int nX, int nY, CWnd* pWnd )
{	
	DWORD dwStyle = ES_AUTOHSCROLL | WS_TABSTOP;

	RECT rect;

	rect.left	= nX;
	rect.right	= nX + 90;
	rect.top	= nY;
	rect.bottom = nY + 22;
	
	for( int i = 0 ; i < 6 ; i++ )
	{
		rect.top	= nY + ( i * 22 );
		rect.bottom = nY + ( i * 22 ) + 18;

		m_aEdit[i].Create( dwStyle, rect, pWnd, m_dwId++ );
		m_aEdit[i].ShowWindow( TRUE );
	}  //if

	rect.top	= nY + 150;
	rect.bottom = nY + 150 + 18;
	m_aEdit[6].Create( dwStyle, rect, pWnd, m_dwId++ );
	m_aEdit[6].ShowWindow( TRUE );

	rect.top	= nY + 192;
	rect.bottom = nY + 192 + 18;
	m_aEdit[7].Create( dwStyle, rect, pWnd, m_dwId++ );
	m_aEdit[7].ShowWindow( TRUE );

	rect.top	= nY + 254;
	rect.bottom = nY + 254 + 18;
	m_aEdit[8].Create( dwStyle, rect, pWnd, m_dwId++ );
	m_aEdit[8].ShowWindow( TRUE );

	rect.top	= nY + 275;
	rect.bottom = nY + 275 + 18;
	m_aEdit[9].Create( dwStyle, rect, pWnd, m_dwId++ );
	m_aEdit[9].ShowWindow( TRUE );

	rect.top	= nY + 296;
	rect.bottom = nY + 296 + 18;
	m_aEdit[10].Create( dwStyle, rect, pWnd, m_dwId++ );
	m_aEdit[10].ShowWindow( TRUE );

	rect.top	= nY + 317;
	rect.bottom = nY + 317 + 18;
	m_aEdit[11].Create( dwStyle, rect, pWnd, m_dwId++ );
	m_aEdit[11].ShowWindow( TRUE );
	
	rect.top	= nY + 360;
	rect.bottom = nY + 360 + 18;
	m_aEdit[12].Create( dwStyle, rect, pWnd, m_dwId++ );
	m_aEdit[12].ShowWindow( TRUE );

	return TRUE;
} //CVehicleForm::Create
