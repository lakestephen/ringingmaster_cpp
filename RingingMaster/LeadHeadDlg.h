#if !defined(AFX_LEADHEADDLG_H__ACDD8844_149D_11D5_B59B_009027BB3286__INCLUDED_)
#define AFX_LEADHEADDLG_H__ACDD8844_149D_11D5_B59B_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeadHeadDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LeadHeadDlg dialog

class LeadHeadDlg : public CDialog
{
// Construction
public:
	LeadHeadDlg(CWnd* pParent, UINT IDD);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LeadHeadDlg)
		CBCGPButton	_left;
		CBCGPButton	_right;
	//}}AFX_DATA

	UINT _IDD;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LeadHeadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LeadHeadDlg)
	afx_msg void OnHigher();
	afx_msg void OnLower();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEADHEADDLG_H__ACDD8844_149D_11D5_B59B_009027BB3286__INCLUDED_)
