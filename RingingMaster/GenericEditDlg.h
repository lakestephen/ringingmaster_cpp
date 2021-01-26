#if !defined(AFX_GenericEditDlg_H__83A8B582_9824_11D3_A9EC_000000000000__INCLUDED_)
#define AFX_GenericEditDlg_H__83A8B582_9824_11D3_A9EC_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoSelEdit.h"
// GenericEditDlg.h : header file
//			   

/////////////////////////////////////////////////////////////////////////////
// GenericEditDlg dialog

class GenericEditDlg : public CDialog
{
// Construction
public:
	CString _strTitle;
	GenericEditDlg(CWnd* pParent,
							 CString strTitle = CString("Enter Data"), 
							 CString strPrompt = CString("Please Enter Data"),
							 CString strStartText = CString(""),
							 bool allowZeroLength = false);   // normal constructor

// Dialog Data
	//{{AFX_DATA(GenericEditDlg)
	enum { IDD = IDD_GENERIC_EDIT };
	AutoSelEdit	_autoSelEdit;
	CString	_strText;
	CString	_strGenericEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GenericEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool _allowZeroLength;

	// Generated message map functions
	//{{AFX_MSG(GenericEditDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GenericEditDlg_H__83A8B582_9824_11D3_A9EC_000000000000__INCLUDED_)
