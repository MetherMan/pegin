// Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CharacterMotionEditor.h"
#include "Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg dialog

CDlg::CDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg)
	DDX_Control(pDX, IDC_COMBO_EMOTICON_LEVEL, m_cbLevelEmoticon);
	DDX_Control(pDX, IDC_COMBO_MAGIC3_LEVEL, m_cbLevelMagic3);
	DDX_Control(pDX, IDC_COMBO_RUNATTACK, m_cbRunAttack);
	DDX_Control(pDX, IDC_COMBO_MAGIC2_LEVEL, m_cbLevelMagic2);
	DDX_Control(pDX, IDC_COMBO_MAGIC1_LEVEL, m_cbLevelMagic1);
	DDX_Control(pDX, IDC_COMBO_MAGIC4_LEVEL, m_cbLevelMagic4);
	DDX_Control(pDX, IDC_COMBO_MAGICCASTING, m_cbMagic);
	DDX_Control(pDX, IDC_COMBO_EMOTICON, m_cbEmoticon);
	DDX_Control(pDX, IDC_COMBO_ATTACK_LEVEL, m_cbLevelAttack);
	DDX_Control(pDX, IDC_COMBO_ATTACK, m_cbAttack);
	DDX_Control(pDX, IDC_COMBO_RUN, m_cbLevelRun);
	DDX_Control(pDX, IDC_COMBO_WEAPON, m_comboWeapon);
	DDX_Control(pDX, IDC_COMBO_VEHICLE, m_comboVehicle);
	DDX_Control(pDX, IDC_COMBO_SEX, m_comboSex);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlg, CDialog)
	//{{AFX_MSG_MAP(CDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK2, OnExit)
	ON_CBN_SELCHANGE(IDC_COMBO_RUN, OnSelchangeComboRun)
	ON_CBN_SELCHANGE(IDC_COMBO_MAGIC1_LEVEL, OnSelchangeComboMagic1Level)
	ON_CBN_SELCHANGE(IDC_COMBO_MAGIC2_LEVEL, OnSelchangeComboMagic2Level)
	ON_CBN_SELCHANGE(IDC_COMBO_MAGIC3_LEVEL, OnSelchangeComboMagic3Level)
	ON_CBN_SELCHANGE(IDC_COMBO_MAGIC4_LEVEL, OnSelchangeComboMagic4Level)
	ON_CBN_SELCHANGE(IDC_COMBO_EMOTICON_LEVEL, OnSelchangeComboEmoticonLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_SEX, OnSelchangeComboSex)
	ON_CBN_SELCHANGE(IDC_COMBO_ATTACK, OnSelchangeComboAttack)
	ON_CBN_SELCHANGE(IDC_COMBO_WEAPON, OnSelchangeComboWeapon)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_MAGICCASTING, OnSelchangeComboMagiccasting)
	ON_CBN_SELCHANGE(IDC_COMBO_RUNATTACK, OnSelchangeComboRunattack)
	ON_CBN_SELCHANGE(IDC_COMBO_VEHICLE, OnSelchangeComboVehicle)
	ON_CBN_SELCHANGE(IDC_COMBO_ATTACK_LEVEL, OnSelchangeComboAttackLevel)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_VKEYTOITEM()
	ON_WM_RBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_EMOTICON, OnSelchangeComboEmoticon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg message handlers

/**
 *
 */
BOOL CDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	CreateInputBox();	// TODO: Add extra initialization here
	InitComboBox();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

/**
 *
 */
void CDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
/**
 *
 */
void CDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/**
 *
 */
void CDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

/**
 *
 */
void CDlg::OnExit() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

/**
 *
 */
void CDlg::InitComboBox()
{
	m_comboSex.InsertString( 0, "남자" );
	m_comboSex.InsertString( 1, "여자" );
	m_comboSex.InsertString( 2, "운영자" );
	m_comboSex.SetCurSel( 0 );

	m_cbLevelAttack.InsertString( 0, "전" );
	m_cbLevelAttack.InsertString( 1, "후" );
	m_cbLevelAttack.SetCurSel( 0 );

	m_cbLevelRun.InsertString( 0, "전" );
	m_cbLevelRun.InsertString( 1, "중" );
	m_cbLevelRun.InsertString( 2, "후" );
	m_cbLevelRun.SetCurSel( 0 );

	m_cbRunAttack.InsertString( 0, "공격1" );
	m_cbRunAttack.InsertString( 1, "공격2" );
	m_cbRunAttack.InsertString( 2, "공격3" );
	m_cbRunAttack.SetCurSel( 0 );

	m_cbMagic.InsertString( 0, "공격" );
	m_cbMagic.InsertString( 1, "축복" );
	m_cbMagic.InsertString( 2, "소환" );
	m_cbMagic.InsertString( 3, "저주" );
	m_cbMagic.SetCurSel( 0 );

	m_cbAttack.InsertString( 0, "공격1" );
	m_cbAttack.InsertString( 1, "공격2" );
	m_cbAttack.InsertString( 2, "공격3" );
	m_cbAttack.InsertString( 3, "공격4" );
	m_cbAttack.InsertString( 4, "공격5" );
	m_cbAttack.SetCurSel( 0 );

	m_cbLevelMagic1.InsertString( 0, "전" );
	m_cbLevelMagic1.InsertString( 1, "중" );
	m_cbLevelMagic1.InsertString( 2, "후" );
	m_cbLevelMagic1.SetCurSel( 0 );
	
	m_cbLevelMagic2.InsertString( 0, "전" );
	m_cbLevelMagic2.InsertString( 1, "중" );
	m_cbLevelMagic2.InsertString( 2, "후" );
	m_cbLevelMagic2.SetCurSel( 0 );
	
	m_cbLevelMagic3.InsertString( 0, "전" );
	m_cbLevelMagic3.InsertString( 1, "중" );
	m_cbLevelMagic3.InsertString( 2, "후" );
	m_cbLevelMagic3.SetCurSel( 0 );

	m_cbLevelMagic4.InsertString( 0, "전" );
	m_cbLevelMagic4.InsertString( 1, "중" );
	m_cbLevelMagic4.InsertString( 2, "후" );	
	m_cbLevelMagic4.SetCurSel( 0 );

	LoadFormData();
} //

void CDlg::LoadFormData()
{
	FILE* fp = fopen( "weapon.data", "rt" );

	if( fp == NULL )
	{
		MessageBox( "Weapon.data파일이 없습니다. 확인하세요" );
		return ;
	} //if

	char szBuffer[256];

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );

		int nCount = 0;
		char szKeyword[80];

		sscanf( szBuffer, "%d %s", &nCount, szKeyword );

		m_comboWeapon.InsertString( nCount, szKeyword );
	} //while

	m_comboWeapon.SetCurSel( 0 );

	fclose( fp );

	fp = fopen( "emoticon.data", "rt" );

	if( fp == NULL )
	{
		MessageBox( "emoticon.data파일이 없습니다. 확인하세요" );
		return ;
	} //if

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );

		int nCount = 0;
		char szKeyword[80];

		sscanf( szBuffer, "%d %s", &nCount, szKeyword );

		m_cbEmoticon.InsertString( nCount, szKeyword );
	} //while

	m_cbEmoticon.SetCurSel( 0 );

	fclose( fp );

	fp = fopen( "vehicle.data", "rt" );

	if( fp == NULL )
	{
		MessageBox( "vehicle.data파일이 없습니다. 확인하세요" );
		return ;
	} //if

	while( !feof( fp ) )
	{
		fgets( szBuffer, 256, fp );

		int nCount = 0;
		char szKeyword[80];

		sscanf( szBuffer, "%d %s", &nCount, szKeyword );

		m_comboVehicle.InsertString( nCount, szKeyword );
	} //while

	m_comboVehicle.SetCurSel( 0 );

	fclose( fp );
} //CDlg::LoadFormData

/**
 *
 */
void CDlg::CreateInputBox()
{
	int nSize = 416 - 124;

	m_WarForm.Create( 124, 76, this );
	m_PeaceForm.Create( 416, 76, this );
	m_VehicleForm.Create( 416 + nSize , 76, this );
//	m_Vehicle.Create( 416 + nSize + nSize + 15, 76, this );  

	m_WarForm.Show( &g_ActionMgr.GetWeapon()->modeWar );
	m_PeaceForm.Show( &g_ActionMgr.GetWeapon()->modePeace );
	
//	m_VehicleForm.Show( g_ActionMgr.GetWeapon()->modeWar );
} //CDlg::CreateInputBox

/**
 *
 */
void CDlg::UpdateForm()
{
	m_WarForm.Show( &g_ActionMgr.GetWeapon()->modeWar );
	m_PeaceForm.Show( &g_ActionMgr.GetWeapon()->modePeace );
	m_VehicleForm.Show( &g_ActionMgr.GetWeapon()->amodeVehicle[ g_nVehicle ] );
} //CDlg::UpdateForm

///////////////////////////////////////////////////////////////
/*
		Level Combo Sel Change
*/
///////////////////////////////////////////////////////////////

/**
 *
 */
void CDlg::OnSelchangeComboRun() 
{
	// TODO: Add your control notification handler code here
	
	OnButtonSave();
	int nSel = m_cbLevelRun.GetCurSel();

	// 평화 모드
	SAction* pAction = g_ActionMgr.GetAction( ROW_RUN, 0 );
	pAction->nSelect = nSel;

	// 전쟁 모드 
	pAction = g_ActionMgr.GetAction( ROW_RUN, 1 );
	pAction->nSelect = nSel;

	/*
		TODO : 탈것에 대한 처리도 해야 한다.
	*/
	pAction = g_ActionMgr.GetAction( ROW_RUN, g_nVehicle + 2 );
	pAction->nSelect = nSel;

	UpdateForm();

	
} //CDlg::OnSelchangeComboRun

/**
 *
 */
void CDlg::OnSelchangeComboMagic1Level() 
{
	OnButtonSave();

	// TODO: Add your control notification handler code here
	int nSel = m_cbLevelMagic1.GetCurSel();

	// 평화 모드
	SAction* pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber, 0 );
	pAction->nSelect = nSel;

	// 전쟁 모드 
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber, 1 );
	pAction->nSelect = nSel;

	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber, g_nVehicle + 2 );
	pAction->nSelect = nSel;


	UpdateForm();
	
} //CDlg::OnSelchangeComboMagic1Level

/**
 *
 */
void CDlg::OnSelchangeComboMagic2Level() 
{
	OnButtonSave();

	// TODO: Add your control notification handler code here
	int nSel = m_cbLevelMagic2.GetCurSel();

	// 평화 모드
	SAction* pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_DOWNANDUP + g_nMagicNumber, 0 );
	pAction->nSelect = nSel;

	// 전쟁 모드 
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_DOWNANDUP + g_nMagicNumber, 1 );
	pAction->nSelect = nSel;

	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_DOWNANDUP + g_nMagicNumber, g_nVehicle + 2 );
	pAction->nSelect = nSel;


	UpdateForm();
	
} //CDlg::OnSelchangeComboMagic2Level

/**
 *
 */
void CDlg::OnSelchangeComboMagic3Level() 
{
	OnButtonSave();

	// TODO: Add your control notification handler code here
	int nSel = m_cbLevelMagic3.GetCurSel();

	// 평화 모드
	SAction* pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_GO + g_nMagicNumber, 0 );
	pAction->nSelect = nSel;

	// 전쟁 모드 
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_GO + g_nMagicNumber, 1 );
	pAction->nSelect = nSel;

	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_GO + g_nMagicNumber, g_nVehicle + 2 );
	pAction->nSelect = nSel;

	UpdateForm();
	
} //CDlg::OnSelchangeComboMagic3Level 


void CDlg::OnSelchangeComboAttackLevel() 
{
	OnButtonSave();

	// TODO: Add your control notification handler code here
	int nSel = m_cbLevelAttack.GetCurSel();

	// 평화 모드
	SAction* pAction = g_ActionMgr.GetAction( STATE_ATTACK1 + g_nAttackNumber, 0 );
	pAction->nSelect = nSel;

	// 전쟁 모드 
	pAction = g_ActionMgr.GetAction( STATE_ATTACK1 + g_nAttackNumber, 1 );
	pAction->nSelect = nSel;

	pAction = g_ActionMgr.GetAction( STATE_ATTACK1 + g_nAttackNumber, g_nVehicle + 2 );
	pAction->nSelect = nSel;

	UpdateForm();	

}


/**
 *
 */
void CDlg::OnSelchangeComboMagic4Level() 
{
	OnButtonSave();

	// TODO: Add your control notification handler code here
	int nSel = m_cbLevelMagic4.GetCurSel();

	// 평화 모드
	SAction* pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_AROUND + g_nMagicNumber, 0 );
	pAction->nSelect = nSel;

	// 전쟁 모드 
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_AROUND + g_nMagicNumber, 1 );
	pAction->nSelect = nSel;

	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_AROUND + g_nMagicNumber, g_nVehicle + 2 );
	pAction->nSelect = nSel;


	UpdateForm();
	
} //CDlg::OnSelchangeComboMagic4Level

/**
 *
 */
void CDlg::OnSelchangeComboEmoticonLevel() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_cbLevelEmoticon.GetCurSel();
	UpdateForm();
	
} //CDlg::OnSelchangeComboEmoticonLevel


///////////////////////////////////////////////////////////////
/*
		Sex Combo Sel Change
*/
///////////////////////////////////////////////////////////////

/**
 *
 */
void CDlg::OnSelchangeComboSex() 
{
	int nSex = m_comboSex.GetCurSel();
	OnButtonSave();

	g_ActionMgr.SetSex( nSex );
	g_VehicleMgr.SetSex( nSex );
	UpdateForm();
} //CDlg::OnSelchangeComboSex

/**
 *
 */
void CDlg::OnSelchangeComboWeapon() 
{
	DWORD dwId = m_comboWeapon.GetCurSel();
	OnButtonSave();

	if( _MASTER != m_comboSex.GetCurSel() )
	{// 일반 남, 여 용 함수 호출
		g_ActionMgr.SetWeapon( dwId );
		g_VehicleMgr.SetWeapon( dwId );
	}	
	else
	{// 운영자 용 함수 호출
		g_ActionMgr.SetWeapon();
		g_VehicleMgr.SetWeapon();
	}

	UpdateForm();
} //CDlg::OnSelchangeComboWeapon

/**
 *
 */
void CDlg::OnSelchangeComboVehicle() 
{
	OnButtonSave();

	g_nVehicle = m_comboVehicle.GetCurSel();
	UpdateForm();
} //CDlg::OnSelchangeComboVehicle


/**
 *
 */
void CDlg::OnSelchangeComboAttack() 
{
	OnButtonSave();

	DWORD dwId = m_cbAttack.GetCurSel();
	g_nAttackNumber = dwId;	

	SAction* pAction = g_ActionMgr.GetAction( STATE_ATTACK1 + g_nAttackNumber, 0 );
	m_cbLevelAttack.SetCurSel( pAction->nSelect );

	pAction = g_ActionMgr.GetAction( STATE_ATTACK1 + g_nAttackNumber, 1 );
	m_cbLevelAttack.SetCurSel( pAction->nSelect );

	pAction = g_ActionMgr.GetAction( STATE_ATTACK1 + g_nAttackNumber, g_nVehicle + 2 );
	m_cbLevelAttack.SetCurSel( pAction->nSelect );

	UpdateForm();
} //CDlg::OnSelchangeComboAttack

/**
 *
 */
void CDlg::OnSelchangeComboRunattack() 
{
	OnButtonSave();

	DWORD dwId = m_cbRunAttack.GetCurSel();
	g_nRunAttackNumber = dwId;	
	UpdateForm();
} //CDlg::OnSelchangeComboRunattack

/**
 *
 */
void CDlg::OnSelchangeComboMagiccasting() 
{
	OnButtonSave();

	// TODO: Add your control notification handler code here
	DWORD dwId = m_cbMagic.GetCurSel();
	g_nMagicNumber = dwId * 4;

	SAction* pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber, 0 );
	m_cbLevelMagic1.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 1, 0 );
	m_cbLevelMagic2.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 2, 0 );
	m_cbLevelMagic3.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 3, 0 );
	m_cbLevelMagic4.SetCurSel( pAction->nSelect );
	
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber, 1 );
	m_cbLevelMagic1.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 1, 1 );
	m_cbLevelMagic2.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 2, 1 );
	m_cbLevelMagic3.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 3, 1 );
	m_cbLevelMagic4.SetCurSel( pAction->nSelect );

	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber, g_nVehicle + 2 );
	m_cbLevelMagic1.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 1, g_nVehicle + 2 );
	m_cbLevelMagic2.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 2, g_nVehicle + 2 );
	m_cbLevelMagic3.SetCurSel( pAction->nSelect );
	pAction = g_ActionMgr.GetAction( STATE_MAGIC_ATTACK_UPANDDOWN + g_nMagicNumber + 3, g_nVehicle + 2 );
	m_cbLevelMagic4.SetCurSel( pAction->nSelect );


	UpdateForm();
} //CDlg::OnSelchangeComboMagiccasting


///////////////////////////////////////////////////////////////
/*
		Click Save Button
*/
///////////////////////////////////////////////////////////////

void CDlg::OnButtonSave() 
{	
	m_WarForm.Save( &g_ActionMgr.GetWeapon()->modeWar );
	m_PeaceForm.Save( &g_ActionMgr.GetWeapon()->modePeace );
	m_VehicleForm.Save( &g_ActionMgr.GetWeapon()->amodeVehicle[g_nVehicle] );

	Save();
} //CDlg::OnButtonSave


///////////////////////////////////////////////////////////////
/*
		Click Save Button
*/
///////////////////////////////////////////////////////////////

/**
 *
 */

void CDlg::Save()
{
	char	szModuleFileName[MAX_PATH];
	GetModuleFileName( NULL, szModuleFileName, MAX_PATH );

	char	szDrive[MAX_PATH];
	char	szDir[MAX_PATH];
	char	szName[MAX_PATH];
	char	szExt[MAX_PATH];
	_splitpath( szModuleFileName, szDrive, szDir, szName, szExt );

	char	szPathName[MAX_PATH];
	sprintf( szPathName, "%s%s", szDrive, szDir );

	SetCurrentDirectory( szPathName );

	FILE* fp = fopen( "character.wad", "wb" );

	// Version
	DWORD dwVersion = 100;
	fwrite( &dwVersion, 4, 1, fp );

	// File NameList

	g_FileMgr.Save( fp );

	///////////////
	// Male 
	///////////////

	DWORD dwCount = MAX_WEAPON;		// 무기 갯수
	fwrite( &dwCount, 4, 1, fp );

	for( int i = 0 ; i < MAX_WEAPON ; i++ )
	{	
		// Weapon
		{
			SWeapon* pWeapon = g_ActionMgr.GetMaleWeapon( i );

			SMode* pMode = NULL;
			// Mode
			{
				pMode = &pWeapon->modePeace;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];

					for( int j = 0 ; j < 3 ; j++ )
					{
						fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				pMode = &pWeapon->modeWar;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];

					for( int j = 0 ; j < 3 ; j++ )
					{
						fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				for( int k = 0 ; k < 10 ; k++ )
				{
					pMode = &pWeapon->amodeVehicle[k];

					for( int i = 0 ; i < 50 ; i++ )
					{
						SAction* pAction = &pMode->aAction[i];

						for( int j = 0 ; j < 3 ; j++ )
						{
							fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
						} //for
					} //for
				} //for
			} //Mode

		} //Weapon

		
	//	fwrite( g_ActionMgr.GetMaleWeapon( i ), sizeof( struct SWeapon ), 1, fp );
	} //for

	///////////////
	// Female
	///////////////

	dwCount = MAX_WEAPON;		// 무기 갯수
	fwrite( &dwCount, 4, 1, fp );

	for( i = 0 ; i < MAX_WEAPON ; i++ )
	{
	//	fwrite( g_ActionMgr.GetFemaleWeapon( i ), sizeof( struct SWeapon ), 1, fp );

		// Weapon
		{
			SWeapon* pWeapon = g_ActionMgr.GetFemaleWeapon( i );

			SMode* pMode = NULL;
			// Mode
			{
				pMode = &pWeapon->modePeace;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];

					for( int j = 0 ; j < 3 ; j++ )
					{
						fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				pMode = &pWeapon->modeWar;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];

					for( int j = 0 ; j < 3 ; j++ )
					{
						fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				for( int k = 0 ; k < 10 ; k++ )
				{
					pMode = &pWeapon->amodeVehicle[k];

					for( int i = 0 ; i < 50 ; i++ )
					{
						SAction* pAction = &pMode->aAction[i];

						for( int j = 0 ; j < 3 ; j++ )
						{
							fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
						} //for
					} //for
				} //for
			} //Mode

		} //Weapon

	} //for

	///////////////
	// Master
	///////////////

	dwCount = 1;		// 무기 갯수 -- 운영자는 맨손만 사용하므로 1
	fwrite( &dwCount, 4, 1, fp );

	SWeapon* pWeapon = g_ActionMgr.GetMasterWeapon();
				
	SMode* pMode = NULL;
	// Mode
	{
		pMode = &pWeapon->modePeace;
			
		for( int i = 0 ; i < 50 ; i++ )
		{
			SAction* pAction = &pMode->aAction[i];
						
			for( int j = 0 ; j < 3 ; j++ )
			{
				fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
			} //for
		} //for
		
	} //Mode
				
	// Mode
	{
		pMode = &pWeapon->modeWar;
		
		for( int i = 0 ; i < 50 ; i++ )
		{
			SAction* pAction = &pMode->aAction[i];
			
			for( int j = 0 ; j < 3 ; j++ )
			{
				fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
			} //for
		} //for
		
	} //Mode
	
	// Mode
	{
		for( int k = 0 ; k < 10 ; k++ )
		{
			pMode = &pWeapon->amodeVehicle[k];
			
			for( int i = 0 ; i < 50 ; i++ )
			{
				SAction* pAction = &pMode->aAction[i];
				
				for( int j = 0 ; j < 3 ; j++ )
				{
					fwrite( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
				} //for
			} //for
		} //for
	} //Mode

	fclose( fp );
} //CDlg::Save

/**
 *
 */
void CDlg::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	
	g_FileMgr.Clear();
	g_ActionMgr.Init();

	UpdateForm();
} //CDlg::OnButtonNew

void CDlg::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here
	char	szModuleFileName[MAX_PATH];
	GetModuleFileName( NULL, szModuleFileName, MAX_PATH );

	char	szDrive[MAX_PATH];
	char	szDir[MAX_PATH];
	char	szName[MAX_PATH];
	char	szExt[MAX_PATH];
	_splitpath( szModuleFileName, szDrive, szDir, szName, szExt );

	char	szPathName[MAX_PATH];
	sprintf( szPathName, "%s%s", szDrive, szDir );

	SetCurrentDirectory( szPathName );


	FILE* fp = fopen( "character.wad", "rb" );

	if( fp == NULL )
	{
		MessageBox( "한번도 Save를 하지 않으셨군요" );
		return ;
	} //if

	// Version
	DWORD dwVersion = 0;
	fread( &dwVersion, 4, 1, fp );

	if( dwVersion != 100 )
	{
		MessageBox( "버전이 맞지 않는 파일입니다." );
	} //if

	// File NameList

	g_FileMgr.Load( fp );

	/////////////////////
	// Male 
	/////////////////////

	DWORD dwCount = 0;		// 무기 갯수
	fread( &dwCount, 4, 1, fp );

	for( int i = 0 ; i < MAX_WEAPON ; i++ )
	{	
//		fread( g_ActionMgr.GetMaleWeapon( i ), sizeof( struct SWeapon ), 1, fp );

		// Weapon
		{
			SWeapon* pWeapon = g_ActionMgr.GetMaleWeapon( i );
			pWeapon->Clear();

			SMode* pMode = NULL;
			// Mode
			{
				pMode = &pWeapon->modePeace;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];
				
					for( int j = 0 ; j < 3 ; j++ )
					{
						fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				pMode = &pWeapon->modeWar;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];

					for( int j = 0 ; j < 3 ; j++ )
					{
						fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				for( int k = 0 ; k < 10 ; k++ )
				{
					pMode = &pWeapon->amodeVehicle[k];

					for( int i = 0 ; i < 50 ; i++ )
					{
						SAction* pAction = &pMode->aAction[i];

						for( int j = 0 ; j < 3 ; j++ )
						{
							fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
						} //for
					} //for
				} //for
			} //Mode

		} //Weapon

	} //for

	/////////////////////
	// Female
	/////////////////////

	dwCount = MAX_WEAPON;		// 무기 갯수
	fread( &dwCount, 4, 1, fp );

	for( i = 0 ; i < MAX_WEAPON ; i++ )
	{
	//	fread( g_ActionMgr.GetFemaleWeapon( i ), sizeof( struct SWeapon ), 1, fp );
		// Weapon
		{
			SWeapon* pWeapon = g_ActionMgr.GetFemaleWeapon( i );
			pWeapon->Clear();

			SMode* pMode = NULL;
			// Mode
			{
				pMode = &pWeapon->modePeace;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];

					for( int j = 0 ; j < 3 ; j++ )
					{
						fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				pMode = &pWeapon->modeWar;

				for( int i = 0 ; i < 50 ; i++ )
				{
					SAction* pAction = &pMode->aAction[i];

					for( int j = 0 ; j < 3 ; j++ )
					{
						fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
					} //for
				} //for
				
			} //Mode

			// Mode
			{
				for( int k = 0 ; k < 10 ; k++ )
				{
					pMode = &pWeapon->amodeVehicle[k];

					for( int i = 0 ; i < 50 ; i++ )
					{
						SAction* pAction = &pMode->aAction[i];

						for( int j = 0 ; j < 3 ; j++ )
						{
							fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
						} //for
					} //for
				} //for
			} //Mode

		} //Weapon
	} //for


	/////////////////////
	// Master
	/////////////////////

	dwCount = 1;		// 무기 갯수	--  운영자는 맨손만 사용한다. 그러므로 1이답 --+
	fread( &dwCount, 4, 1, fp );

	SWeapon* pWeapon = g_ActionMgr.GetMasterWeapon();
	pWeapon->Clear();
	
	SMode* pMode = NULL;
	// Mode
	{
		pMode = &pWeapon->modePeace;
		
		for( int i = 0 ; i < 50 ; i++ )
		{
			SAction* pAction = &pMode->aAction[i];
			
			for( int j = 0 ; j < 3 ; j++ )
			{
				fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
			} //for
		} //for
		
	} //Mode
	
	// Mode
	{
		pMode = &pWeapon->modeWar;
		
		for( int i = 0 ; i < 50 ; i++ )
		{
			SAction* pAction = &pMode->aAction[i];
			
			for( int j = 0 ; j < 3 ; j++ )
			{
				fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
			} //for
		} //for
		
	} //Mode
	
	// Mode
	{
		for( int k = 0 ; k < 10 ; k++ )
		{
			pMode = &pWeapon->amodeVehicle[k];
			
			for( int i = 0 ; i < 50 ; i++ )
			{
				SAction* pAction = &pMode->aAction[i];
				
				for( int j = 0 ; j < 3 ; j++ )
				{
					fread( &pAction->aMotion[j], sizeof( SMotion ), 1, fp );
				} //for
			} //for
		} //for
	} //Mode		


	fclose( fp );		
	
	UpdateForm();

}

void CDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if( nFlags == VK_LCONTROL )
	{
		MessageBox( "sdf" );
	}
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

int CDlg::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnVKeyToItem(nKey, pListBox, nIndex);
}

void CDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnRButtonDown(nFlags, point);
}

void CDlg::OnSelchangeComboEmoticon() 
{
	// TODO: Add your control notification handler code here
	DWORD dwId = m_cbEmoticon.GetCurSel();
	g_nEmoticonNumber = dwId;	
	UpdateForm();
}
