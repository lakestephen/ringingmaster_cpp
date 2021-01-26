#if !defined(AFX_NOTESDLG_H__99B32465_E4E2_11D6_B629_000255162CF0__INCLUDED_)
#define AFX_NOTESDLG_H__99B32465_E4E2_11D6_B629_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotesDlg.h : header file
//

#include "ResizeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// NotesDlg dialog

class NotesDlg : public ResizeDialog
{
// Construction
public:
	NotesDlg(CWnd* pParent, CString notes, CPoint requestPosition = CPoint(-1,-1));   // standard constructor

// Dialog Data
	//{{AFX_DATA(NotesDlg)
	enum { IDD = IDD_NOTES };
	CString	_notes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotesDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPoint _requestPosition;

	// Generated message map functions
	//{{AFX_MSG(NotesDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTESDLG_H__99B32465_E4E2_11D6_B629_000255162CF0__INCLUDED_)
