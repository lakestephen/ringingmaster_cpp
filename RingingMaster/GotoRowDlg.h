#if !defined(AFX_GOTOROWDLG_H__02FF56A4_D7AD_11D5_BB7F_881130CACA74__INCLUDED_)
#define AFX_GOTOROWDLG_H__02FF56A4_D7AD_11D5_BB7F_881130CACA74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GotoRowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GotoRowDlg dialog

class GotoRowDlg : public CDialog
{
// Construction
public:
	GotoRowDlg(CWnd* pParent, int maxRows);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GotoRowDlg)
	enum { IDD = IDD_GOTO_ROW };
	CEdit	_rowCtrl;
	CSpinButtonCtrl	_spin;
	static int		_row;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GotoRowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int _maxRows;

	// Generated message map functions
	//{{AFX_MSG(GotoRowDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOTOROWDLG_H__02FF56A4_D7AD_11D5_BB7F_881130CACA74__INCLUDED_)
