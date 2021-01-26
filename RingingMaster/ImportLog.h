#if !defined(AFX_IMPORTLOG_H__B3F08A60_B37C_11D5_BB7F_A392F5A3C375__INCLUDED_)
#define AFX_IMPORTLOG_H__B3F08A60_B37C_11D5_BB7F_A392F5A3C375__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportLog.h : header file
//

#include "ResizeDialog.h"
/////////////////////////////////////////////////////////////////////////////
// ImportLog dialog

class ImportLog : public ResizeDialog
{
// Construction
public:
	void addString(CString str);
	ImportLog(CStringArray& stringArr, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ImportLog)
	enum { IDD = IDD_IMPORT_LOG };
	CListBox	_log;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ImportLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CStringArray& _stringArr;

	// Generated message map functions
	//{{AFX_MSG(ImportLog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTLOG_H__B3F08A60_B37C_11D5_BB7F_A392F5A3C375__INCLUDED_)
