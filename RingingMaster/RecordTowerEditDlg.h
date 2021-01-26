#if !defined(AFX_RECORDTOWEREDITDLG_H__D27EC0CF_64FA_11D7_B69E_000255162CF0__INCLUDED_)
#define AFX_RECORDTOWEREDITDLG_H__D27EC0CF_64FA_11D7_B69E_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordTowerEditDlg.h : header file
//

#include "RecordAnalysisTowerTower.h"	
#include "ResizeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// RecordTowerEditDlg dialog

class RecordTowerEditDlg : public ResizeDialog
{
// Construction
public:
	RecordTowerEditDlg(CWnd* pParent, RecordTower* tower );   // standard constructor

// Dialog Data
	//{{AFX_DATA(RecordTowerEditDlg)
	enum { IDD = IDD_TOWER_DB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RecordTowerEditDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	RecordAnalysisTowerTower _analysisTowerTower;
	RecordTower* _tower; 
	// Generated message map functions
	//{{AFX_MSG(RecordTowerEditDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDTOWEREDITDLG_H__D27EC0CF_64FA_11D7_B69E_000255162CF0__INCLUDED_)
