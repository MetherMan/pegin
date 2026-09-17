/////////////////////////////////////////////////////
//
// Render.h
// Created By : sckyj
//
/////////////////////////////////////////////////////

#ifndef __Render_Header__
#define __Render_Header__

// extern 
	extern HWND			g_hWnd;
	extern HINSTANCE	g_hInstance;
//~extern

class CRender
{
public :	

	enum MODE{ MODE_PATCH, MODE_SERVERSELECT };
	
	CRender();
	~CRender();

	// Start Engine
	BOOL Init();

	// End Engine
	BOOL Clear();

	// Render
	BOOL Render();

	// Check Button
	BOOL LDown( POINT pt );
	BYTE LUp( POINT pt );

	// Change Mode
	BOOL ChangeMode();
	MODE GetMode()		{ return m_eMode; }	

private :	

	IMode*			m_pPatch;
	IMode*			m_pArea;
	IMode*			m_pMode;
	MODE			m_eMode;	
}; // class CRender

#endif // #ifndef __Render_Header__