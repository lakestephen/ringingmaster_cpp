#if !defined(AFX_MUSICDLG_H__DA9FE080_BA92_11D5_BB7F_C0B9818D9A7E__INCLUDED_)
#define AFX_MUSICDLG_H__DA9FE080_BA92_11D5_BB7F_C0B9818D9A7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MusicDlg.h : header file
//
#include "MusicRule.h"

/////////////////////////////////////////////////////////////////////////////
// MusicDlg dialog

class MusicDlg : public CDialog
{
// Construction
public:
	MusicDlg(MusicRule* musicRule);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MusicDlg)
	enum { IDD = IDD_MUSIC };
	CBCGPColorButton 	_color;
	BOOL	_back;
	BOOL	_hand;
	CString	_bells;
	CString	_bellsStatic;
	CString	_name;
	BOOL	_useTouchStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MusicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	MusicRule* _musicRule;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MusicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnChangeBells();
	afx_msg void OnTouchStyleColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUSICDLG_H__DA9FE080_BA92_11D5_BB7F_C0B9818D9A7E__INCLUDED_)
