
#pragma once
#include "afxwin.h"

// OptionsMiscellaneous.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OptionsMiscellaneous dialog

class OptionsMiscellaneous : public CBCGPPropertyPage
{
// Construction
public:
	OptionsMiscellaneous();   // standard constructor

// Dialog Data
	//{{AFX_DATA(OptionsMiscellaneous)
	enum { IDD = IDD_OPTIONS_MISCELLANIOUS};
	CBCGPButton	_workingDirBut;
	BOOL	_autoOpen;
	CString	_workingDir;
	BOOL	_propertyHint;
	BOOL _useWorkingDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptionsMiscellaneous)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OptionsMiscellaneous)
	virtual BOOL OnInitDialog();
	afx_msg void OnWorkingDirButt();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUseWorkingDir();
	CEdit _workingDirCtrl;
};

