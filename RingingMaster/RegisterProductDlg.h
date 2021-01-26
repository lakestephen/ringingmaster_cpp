#if !defined(AFX_REGISTERPRODUCTDLG_H__DCF3E107_FE6F_11D4_B580_009027BB3286__INCLUDED_)
#define AFX_REGISTERPRODUCTDLG_H__DCF3E107_FE6F_11D4_B580_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegisterProductDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RegisterProductDlg dialog

class RegisterProductDlg : public CDialog
{
// Construction
public:
	RegisterProductDlg(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RegisterProductDlg)
	enum { IDD = IDD_REGISTER_PRODUCT };
	CString	_key;
	CString	_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RegisterProductDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RegisterProductDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTERPRODUCTDLG_H__DCF3E107_FE6F_11D4_B580_009027BB3286__INCLUDED_)
