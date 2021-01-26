#if !defined(AFX_EVENTWAITDLG_H__3B022026_7F09_11D4_B4FE_009027BB3286__INCLUDED_)
#define AFX_EVENTWAITDLG_H__3B022026_7F09_11D4_B4FE_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventWaitDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// EventWaitDlg dialog

class EventWaitDlg : public CDialog
{
// Construction
public:
	EventWaitDlg(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EventWaitDlg)
	enum { IDD = IDD_EVENT_WAIT };
	CAnimateCtrl	_animate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EventWaitDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EventWaitDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTWAITDLG_H__3B022026_7F09_11D4_B4FE_009027BB3286__INCLUDED_)
