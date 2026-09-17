/////////////////////////////////////////////////////
//
// Render.cpp
// Created By : sckyj
//
/////////////////////////////////////////////////////

// include 	
	#include "stdafx.h"
	#include "Global.h"	
//~include 

// extern 
	HWND		g_hWnd		= NULL;
	HINSTANCE	g_hInstance	= NULL;
//~extern

CRender::CRender()
:	m_pPatch	( NULL ),
	m_eMode		( MODE_PATCH )
{	
} // CRender::CRender

CRender::~CRender()
{
} // CRender::~CRender

BOOL CRender::Init()
{
	m_pPatch = new CPatch;
	if( !m_pPatch || !m_pPatch->Init() )
	{
		return FALSE;
	} // if	

	m_pArea = new CArea;
	if( !m_pArea || !m_pArea->Init() )
	{
		return FALSE;
	} // if

	m_pMode = m_pPatch;

	return TRUE;
} // CRender::StartEngine

// End Engine
BOOL CRender::Clear()
{	
	m_pPatch->Clear();	
	m_pArea->Clear();

	m_pMode = NULL;

	SAFE_DELETE( m_pPatch );
	SAFE_DELETE( m_pArea );	

	return TRUE;
} // CRender::EndEndgine

BOOL CRender::Render()
{
	if( !m_pMode )	return FALSE;

	m_pMode->Render();
	
	return TRUE;
} // CRender::Render

BOOL CRender::LDown( POINT pt )
{
	if( !m_pMode )	return FALSE;

	return m_pMode->LDown( pt );
} // CRender::LDown

BYTE CRender::LUp( POINT pt )
{
	if( !m_pMode )	return FALSE;

	return m_pMode->LUp( pt );
} // CRender::LUp

BOOL CRender::ChangeMode()
{
	if( m_eMode == MODE_PATCH )
	{
		m_eMode = MODE_SERVERSELECT;
		m_pMode	= m_pArea;
	}		
	else
	{
		m_eMode = MODE_PATCH;
		m_pMode = m_pPatch;
	} // if..else..		

	return m_eMode;
} // CRender::ChangeMode