#pragma once

/////////////////////////////////////////////////////////////////////////////
// GotoLeadRowDlg dialog

class Method;

class GotoLeadRowDlg : public CDialog
{
// Construction
public:
	void verify();
	GotoLeadRowDlg(CWnd* pParent, Method* method);  //todo change to not use Method - or change GotoRowDlg to use Method

// Dialog Data
	//{{AFX_DATA(GotoLeadRowDlg)
	enum { IDD = IDD_GOTO_LEAD_ROW };
	CEdit	_leadCtrl;
	CSpinButtonCtrl	_rowSpin;
	CSpinButtonCtrl	_leadSpin;
	static int		_lead;
	static int		_row;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GotoLeadRowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	Method* _method;
	bool _inited;

	// Generated message map functions
	//{{AFX_MSG(GotoLeadRowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLead();
	afx_msg void OnChangeRow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

