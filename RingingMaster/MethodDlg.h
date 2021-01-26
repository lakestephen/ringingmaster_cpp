#if !defined(AFX_METHODDLG_H__CA750DC0_0773_11D5_BB7F_BB8C29C49536__INCLUDED_)
#define AFX_METHODDLG_H__CA750DC0_0773_11D5_BB7F_BB8C29C49536__INCLUDED_

#include "MethodWnd.h"	// Added by ClassView
#include "ResizeDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MethodDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MethodDlg dialog


class MethodDlg : public ResizeDialog
{
// Construction
public:
	~MethodDlg();
	MethodWnd * _methodWnd;
	MethodDlg(CWnd* pParent, Method* method, CString token);

// Dialog Data
	//{{AFX_DATA(MethodDlg)
	enum { IDD = IDD_METHOD };
	CString	_bell;
	CString	_lead;
	CString	_place;
	CString	_row;
	CString	_message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Method * _method;
	CString _token;
	Ints* _callPositions;

	// Generated message map functions
	//{{AFX_MSG(MethodDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnSetMethodPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMethodWndTerminate(WPARAM wParam, LPARAM lParam);

	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_METHODDLG_H__CA750DC0_0773_11D5_BB7F_BB8C29C49536__INCLUDED_)
