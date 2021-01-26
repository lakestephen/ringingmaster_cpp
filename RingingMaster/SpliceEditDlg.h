#if !defined(AFX_SPLICEEDITDLG_H__631A3B20_D485_11D5_BB7F_A182FAB5DD7F__INCLUDED_)
#define AFX_SPLICEEDITDLG_H__631A3B20_D485_11D5_BB7F_A182FAB5DD7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpliceEditDlg.h : header file
//

#include "Notation.h"

/////////////////////////////////////////////////////////////////////////////
// SpliceEditDlg dialog

class SpliceEditDlg : public CDialog
{
// Construction
public:
	int _maxRows;
	SpliceEditDlg(CWnd* pParent, Notation &notation, int pos = -1);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SpliceEditDlg)
	enum { IDD = IDD_SPLICE };
	CBCGPButton	_spliceEdit;
	CSpinButtonCtrl	_spliceSpin;
	int		_splice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SpliceEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(SpliceEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSpliceEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLICEEDITDLG_H__631A3B20_D485_11D5_BB7F_A182FAB5DD7F__INCLUDED_)
