#if !defined(AFX_ROWCHOOSERDLG_H__07D114A3_A1EC_11D5_B4E0_000255162CF0__INCLUDED_)
#define AFX_ROWCHOOSERDLG_H__07D114A3_A1EC_11D5_B4E0_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RowChooserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RowChooserDlg dialog

class RowChooserDlg : public CDialog
{
// Construction
public:
	RowChooserDlg(CWnd* pParent,CString title, int number, CString originalRow = CString(""), bool forceTenor = FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(RowChooserDlg)
	enum { IDD = IDD_ROW_CHOOSER };
	CEdit	_rowEdit;
	CStatic	_message;
	CString	_rowStr;
	BOOL	_rounds;
	//}}AFX_DATA

	CString _originalRow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RowChooserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString _title;
	int _number;
	CWnd* _pParent;
	bool _forceTenor;

	// Generated message map functions
	//{{AFX_MSG(RowChooserDlg)
	afx_msg void OnChangeRowEdit();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRounds();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROWCHOOSERDLG_H__07D114A3_A1EC_11D5_B4E0_000255162CF0__INCLUDED_)
