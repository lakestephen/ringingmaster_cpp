#if !defined(AFX_MoreDlg_H__0AA0C184_BB99_11D4_B543_009027BB3286__INCLUDED_)
#define AFX_MoreDlg_H__0AA0C184_BB99_11D4_B543_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoreDlg.h : header file
//
#include "ResizeDialog.h"
#include "CJListBox.h"
/////////////////////////////////////////////////////////////////////////////
// MoreDlg dialog

class MoreDlg : public ResizeDialog
{
// Construction
public:
	int getIndex();
	MoreDlg(CWnd* pParent, CStringArray& strings, CString name);   // standard constructor

// Dialog Data
	//{{AFX_DATA(MoreDlg)
	enum { IDD = IDD_STYLES_MORE };
	CCJListBox	_list;
	CString	_staticName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MoreDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  	
	int _index;
	const CStringArray& _strings;
	CString _name;

	// Generated message map functions
	//{{AFX_MSG(MoreDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MoreDlg_H__0AA0C184_BB99_11D4_B543_009027BB3286__INCLUDED_)
