#if !defined(AFX_TOWERTTSITEMDLG_H__C6498DAD_F0B6_4D1E_B420_F1030C96AFC4__INCLUDED_)
#define AFX_TOWERTTSITEMDLG_H__C6498DAD_F0B6_4D1E_B420_F1030C96AFC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerTTSItemDlg.h : header file
//

#include "TowerTTSItem.h"

/////////////////////////////////////////////////////////////////////////////
// TowerTTSItemDlg dialog

class TowerTTSItemDlg : public CDialog
{
// Construction
public:
	void enable();
	TowerTTSItemDlg(CWnd* parent, TowerTTSItem& itemTowerTTSItem);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TowerTTSItemDlg)
	enum { IDD = IDD_TOWER_TTS };
	CBCGPButton	_reserCtrl;
	CEdit	_textCtrl;
	CStatic	_labelCtrl;
	CButton	_enableWnd;
	BOOL	_enable;
	CString	_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerTTSItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	TowerTTSItem& _originalTowerTTSItem;
	// Generated message map functions
	//{{AFX_MSG(TowerTTSItemDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnable();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOWERTTSITEMDLG_H__C6498DAD_F0B6_4D1E_B420_F1030C96AFC4__INCLUDED_)
