#if !defined(AFX_TOWERADVANCEDDLG_H__378ABADB_DD2B_44BC_A5D9_E2E0BC61CC66__INCLUDED_)
#define AFX_TOWERADVANCEDDLG_H__378ABADB_DD2B_44BC_A5D9_E2E0BC61CC66__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TowerAdvancedDlg.h : header file
//

#include "NumericEdit.h"


class TowerBellItem;
/////////////////////////////////////////////////////////////////////////////
// TowerAdvancedDlg dialog

class TowerAdvancedDlg : public CDialog
{
// Construction
public:
	TowerAdvancedDlg(CWnd* pParent,TowerBellItem& towerBellItem);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TowerAdvancedDlg)
	enum { IDD = IDD_TOWER_ADVANCED };
	CBCGPButton	_btnVolumeReset;
	CBCGPButton	_btnOverallFrequencyReset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TowerAdvancedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	TowerBellItem& _towerBellItem;
	bool _inited;

	NumericEdit _freq;
	NumericEdit _vol;


	// Generated message map functions
	//{{AFX_MSG(TowerAdvancedDlg)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnOverallFrequencyReset();
	afx_msg void OnVolumeReset();
	//}}AFX_MSG
	 
	afx_msg LRESULT OnNumericEditChange(WPARAM,LPARAM);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOWERADVANCEDDLG_H__378ABADB_DD2B_44BC_A5D9_E2E0BC61CC66__INCLUDED_)
