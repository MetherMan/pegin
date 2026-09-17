/////////////////////////////////////////////////////
//
// Patch.h
// Created By : sckyj
//
/////////////////////////////////////////////////////

#ifndef __Patch_Header__
#define __Patch_Header__

// define
	#define	WM_UPDATE_DATA		WM_USER+1
	#define WM_UPDATE_FINISH	WM_USER+2
//~define

class CPatch : public IMode
{
public :
	CPatch();
	virtual ~CPatch();

	// Init
	BOOL	Init();

	// Clear
	void	Clear();

	// Render
	BOOL	Render();

	// Thread
	DWORD	ThreadProc();

	// Check Button
	BOOL	LDown( POINT pt );
	BYTE	LUp( POINT pt );

private :

	// Update Patch
	BOOL	UpdateProcess();
	BOOL	Process_VersionCheck();
	BOOL	Process_PatchInfoDown();
	BOOL	Process_Download();

	BOOL	IsValidPatchFile();
	int		HttpFileTransfer( const char *url );

	// SendMessage
	void	Send( BOOL bFinish, char* szMsg = "" );
	
	// About "Version.dat"
	DWORD	SetCurrVersion( DWORD dwVersion );
	DWORD	GetCurrVersion();

private :

	HANDLE			m_hThread;			// Process Thread
	HFONT			m_hFont;

	int				m_nBtnState[4];

	BOOL			m_bFinish;			// Update Finish

	// Update data
		DWORD		m_dwCurrVersion;	// Current Version
		DWORD		m_dwFileSize;
		DWORD		m_dwTotalSize;		
		int			m_nTotalFile;
		int			m_nFileCount;

		CString		m_sFileInfo;
		CString		m_sTotalInfo;
	//~Update data

	// Image
		my::CBitmap		m_bmBack;
			
		my::CBitmap		m_bmWeb;
		my::CBitmap		m_bmProgress;

		my::CBitmap		m_bmReg[3];
		my::CBitmap		m_bmStart[3];
		my::CBitmap		m_bmCancel[3];
		my::CBitmap		m_bmSetup[3];
		
		CProgress	    m_pgProgress[2];

		RECT			PATCH_START;
		RECT			PATCH_OPTION;
		RECT			PATCH_CANCEL;
		RECT			PATCH_REG;
	//~Image
};

#endif // #ifndef __Patch_Header__