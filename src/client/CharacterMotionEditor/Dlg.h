// Dlg.h : header file
//

#if !defined(AFX_DLG_H__6F2CFE5C_850D_4DA7_AF96_F298BA66CA60__INCLUDED_)
#define AFX_DLG_H__6F2CFE5C_850D_4DA7_AF96_F298BA66CA60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ModeForm.h"
#include "VehicleForm.h"
/////////////////////////////////////////////////////////////////////////////
// CDlg dialog

#define MAXEDITBOX	15

class CDlg : public CDialog
{
// Construction
public:
	CDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg)
	enum { IDD = IDD_CHARACTERMOTIONEDITOR_DIALOG };
	CComboBox	m_cbLevelEmoticon;
	CComboBox	m_cbLevelMagic3;
	CComboBox	m_cbRunAttack;
	CComboBox	m_cbLevelMagic2;
	CComboBox	m_cbLevelMagic1;
	CComboBox	m_cbLevelMagic4;
	CComboBox	m_cbMagic;
	CComboBox	m_cbEmoticon;
	CComboBox	m_cbLevelAttack;
	CComboBox	m_cbAttack;
	CComboBox	m_cbLevelRun;
	CComboBox	m_comboWeapon;
	CComboBox	m_comboVehicle;
	CComboBox	m_comboSex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void InitComboBox();
	void LoadFormData();
	void CreateInputBox();
	void UpdateForm();
	void Save();


	CModeForm	 m_WarForm;
	CModeForm	 m_PeaceForm;
	CModeForm	 m_VehicleForm;
	CModeForm	 m_Vehicle;
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnExit();
	afx_msg void OnSelchangeComboRun();
	afx_msg void OnSelchangeComboMagic1Level();
	afx_msg void OnSelchangeComboMagic2Level();
	afx_msg void OnSelchangeComboMagic3Level();
	afx_msg void OnSelchangeComboMagic4Level();
	afx_msg void OnSelchangeComboEmoticonLevel();
	afx_msg void OnSelchangeComboSex();
	afx_msg void OnSelchangeComboAttack();
	afx_msg void OnSelchangeComboWeapon();
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeComboMagiccasting();
	afx_msg void OnSelchangeComboRunattack();
	afx_msg void OnSelchangeComboVehicle();
	afx_msg void OnSelchangeComboAttackLevel();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonLoad();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeComboEmoticon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_H__6F2CFE5C_850D_4DA7_AF96_F298BA66CA60__INCLUDED_)
