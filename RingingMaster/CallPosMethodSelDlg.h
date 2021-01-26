#if !defined(AFX_CALLPOSMETHODSELDLG_H__6F48F4C3_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_)
#define AFX_CALLPOSMETHODSELDLG_H__6F48F4C3_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallPosMethodSelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CallPosMethodSelDlg dialog

class CallPosMethodSelDlg : public CDialog
{
// Construction
public:
	CallPosMethodSelDlg(CStringArray * strArr, int passedIndex = -1);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CallPosMethodSelDlg)
	enum { IDD = IDD_CALL_POS_METHOD_SEL_DLG };
	CComboBox	_selectionCombo;
	//}}AFX_DATA

	CString _selection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CallPosMethodSelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CallPosMethodSelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCallPosSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CStringArray * _stringArray;
	int _passedIndex;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLPOSMETHODSELDLG_H__6F48F4C3_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_)
