#if !defined(AFX_CALLPOSMETHODEDITDLG_H__D0BBE1E0_1F01_11D5_BB7F_9FDC85C81828__INCLUDED_)
#define AFX_CALLPOSMETHODEDITDLG_H__D0BBE1E0_1F01_11D5_BB7F_9FDC85C81828__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallPosMethodEditDlg.h : header file
//

#include "Notation.h"

/////////////////////////////////////////////////////////////////////////////
// CallPosMethodEditDlg dialog

class CallPosMethodEditDlg : public CDialog
{
// Construction
public:

	CallPosMethodEditDlg(CWnd* pParent, Notation &notation, CString name = "", int lead = -1, int row = -1);

// Dialog Data
	//{{AFX_DATA(CallPosMethodEditDlg)
	enum { IDD = IDD_CALL_POS_METHOD };
	CBCGPButton	_editButt;
	CComboBox	_pos;
	CSpinButtonCtrl	_leadSpin;
	CString	_name;
	int		_lead;
	//}}AFX_DATA

	int _row;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CallPosMethodEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int _maxLeads;

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(CallPosMethodEditDlg)
	afx_msg void OnCallPosEdit();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLPOSMETHODEDITDLG_H__D0BBE1E0_1F01_11D5_BB7F_9FDC85C81828__INCLUDED_)
