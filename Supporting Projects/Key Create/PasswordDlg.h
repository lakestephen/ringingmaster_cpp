#if !defined(AFX_PasswordDlg_H__374E7B34_DC74_44E8_9D5C_D215F75FD212__INCLUDED_)
#define AFX_PasswordDlg_H__374E7B34_DC74_44E8_9D5C_D215F75FD212__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PasswordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PasswordDlg dialog

class PasswordDlg : public CDialog
{
// Construction
public:
	PasswordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PasswordDlg)
	enum { IDD = IDD_PASSWORD };
	CEdit	_passwordCtrl;
	CString	_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PasswordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PasswordDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PasswordDlg_H__374E7B34_DC74_44E8_9D5C_D215F75FD212__INCLUDED_)
