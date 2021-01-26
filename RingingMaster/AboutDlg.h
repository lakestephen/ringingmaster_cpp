#if !defined(AFX_ABOUTDLG_H__55BAE275_D72A_11D4_B555_009027BB3286__INCLUDED_)
#define AFX_ABOUTDLG_H__55BAE275_D72A_11D4_B555_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDlg.h : header file
//
#include "Credits.h"
#include "afxwin.h"


/////////////////////////////////////////////////////////////////////////////
// AboutDlg dialog




class AboutDlg : public CDialog
{
public:
	void OnWizard1();
	void OnWizard();

	AboutDlg();

// Dialog Data
	//{{AFX_DATA(AboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CButton	_ok;
	CBCGPURLLinkButton	_url;
	CBCGPURLLinkButton	_mail;
	Credits	_static;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV support
	virtual BOOL OnInitDialog();

	//}}AFX_VIRTUAL

// Implementation
protected:
	CString getDisk();
	CString getMemory();
	void setCredits();
	//{{AFX_MSG(AboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic _resolution;
	afx_msg void OnStnClickedVersion();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__55BAE275_D72A_11D4_B555_009027BB3286__INCLUDED_)
