#pragma once

#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// LogicDlg dialog

enum LogicType
{
	lt_and,
	lt_or,
};


class LogicDlg : public CDialog
{
// Construction
public:
	LogicDlg(CWnd* pParent, LogicType& logicType);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LogicDlg)
	enum { IDD = IDD_LOGIC };
	int		_logicType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LogicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	LogicType& _originalLogicType;

	// Generated message map functions
	//{{AFX_MSG(LogicDlg)
		virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

