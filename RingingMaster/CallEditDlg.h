#if !defined(AFX_CALLEDITDLG_H__4B5A8681_10D5_11D5_BB7F_8E0990B7BB36__INCLUDED_)
#define AFX_CALLEDITDLG_H__4B5A8681_10D5_11D5_BB7F_8E0990B7BB36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallEditDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CallEditDlg dialog

class CallEditDlg : public CDialog
{
// Construction
public:
	CallEditDlg(CWnd* pParent,int number, CString nameShorthand, CString nameLonghand, CString notation);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CallEditDlg)
	enum { IDD = IDD_CALL };
	CBCGPButton	_visualEdit;
	CString	_notation;
	CString	_nameShorthand;
	CString	_nameLonghand;
	CString	_notationStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CallEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int _number;

	// Generated message map functions
	//{{AFX_MSG(CallEditDlg)
	afx_msg void OnCallEdit();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCallNotation();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLEDITDLG_H__4B5A8681_10D5_11D5_BB7F_8E0990B7BB36__INCLUDED_)
