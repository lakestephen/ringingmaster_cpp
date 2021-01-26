#if !defined(AFX_CALLPOSLEADEDITDLG_H__D9647454_12D9_11D5_B598_009027BB3286__INCLUDED_)
#define AFX_CALLPOSLEADEDITDLG_H__D9647454_12D9_11D5_B598_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallPosLeadEditDlg.h : header file
//
#include "Notation.h"

/////////////////////////////////////////////////////////////////////////////
// CallPosLeadEditDlg dialog

class CallPosLeadEditDlg : public CDialog
{
// Construction
public:
	int _maxRows;
	CallPosLeadEditDlg(CWnd* pParent, Notation &notation, int pos = -1);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CallPosLeadEditDlg)
	enum { IDD = IDD_CALL_POS_LEAD };
	CSpinButtonCtrl	_leadSpin;
	CBCGPButton	_callPosEdit;
	int		_callPosLead;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CallPosLeadEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Notation & _notation;


	// Generated message map functions
	//{{AFX_MSG(CallPosLeadEditDlg)
	afx_msg void OnCallPosEdit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLPOSLEADEDITDLG_H__D9647454_12D9_11D5_B598_009027BB3286__INCLUDED_)
