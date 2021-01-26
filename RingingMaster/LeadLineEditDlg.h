#if !defined(AFX_LEADLINEEDITDLG_H__7A5ADC04_1DDF_11D5_B5A5_009027BB3286__INCLUDED_)
#define AFX_LEADLINEEDITDLG_H__7A5ADC04_1DDF_11D5_B5A5_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeadLineEditDlg.h : header file
//
#include "Notation.h"

/////////////////////////////////////////////////////////////////////////////
// LeadLineEditDlg dialog

class LeadLineEditDlg : public CDialog
{
// Construction
public:
	int _maxRows;

	LeadLineEditDlg(CWnd* pParent, Notation & notation, int pos = -1);

// Dialog Data
	//{{AFX_DATA(LeadLineEditDlg)
	enum { IDD = IDD_LEAD_LINES };
	CBCGPButton	_leadLineEdit;
	CSpinButtonCtrl	_leadSpin;
	int		_leadLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LeadLineEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(LeadLineEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLeadLineEdit();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEADLINEEDITDLG_H__7A5ADC04_1DDF_11D5_B5A5_009027BB3286__INCLUDED_)
