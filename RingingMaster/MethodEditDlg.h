#if !defined(AFX_MethodEditDLG_H__C8A7C723_66D0_11D4_B4E6_009027BB3286__INCLUDED_)
#define AFX_MethodEditDLG_H__C8A7C723_66D0_11D4_B4E6_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MethodEditDlg.h : header file
//

#include "ResizeDialog.H"
#include "MethodEditWnd.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// MethodEditDlg dialog

class MethodEditDlg : public ResizeDialog
{
   	DECLARE_DYNAMIC(MethodEditDlg)
	
	// Construction
public:
	int _number;
	CString _notation;

	MethodEditDlg(CWnd* pParent,int number, CString notation, int blankSize = 0);
	
// Dialog Data
	//{{AFX_DATA(MethodEditDlg)
	enum { IDD = IDD_METHOD_EDIT };
	CBCGPButton	_undoBtn;
	CBCGPButton	_style;
	CBCGPButton	_clearAll;
	CBCGPButton	_cancleBtn;
	CBCGPButton	_okBtn;
	CBCGPButton	_remove;
	CBCGPButton	_add;
	CString	_bell;
	CString	_place;
	CString	_row;
	CString	_message;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MethodEditDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	protected:

	MethodEditWnd _methodEditWnd; 

	// Generated message map functions
	//{{AFX_MSG(MethodEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMethodEditAdd();
	afx_msg void OnMethodEditRemove();
	afx_msg void OnMethodEditStyle();
	afx_msg void OnMethodEditClearAll();
	afx_msg void OnMethodEditUndo();
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnUpdateOk(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClearAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUndo(CCmdUI* pCmdUI);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMethodPos(WPARAM wParam, LPARAM lParam);


	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MethodEditDLG_H__C8A7C723_66D0_11D4_B4E6_009027BB3286__INCLUDED_)
