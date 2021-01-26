#if !defined(AFX_NotationLeadLines_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationLeadLines_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//
#include "PropList.h"

class Notation;
class NotationDlg;

class NotationLeadLines : public CBCGPPropertyPage
{

// Construction
public:
	NotationLeadLines(Notation & notation);
	~NotationLeadLines();

	void createLeadLineList();


// Dialog Data
	//{{AFX_DATA(NotationLeadLines)
	enum { IDD = IDD_NOTATION_LEAD_LINES };
	CBCGPButton	_addDefault;
	CButton	_useDefaultLeadLine;
	PropList	_leadLine;
	CBCGPButton	_addLeadLine;
	CBCGPButton	_removeLeadLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationLeadLines)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(NotationLeadLines)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaultLeadLines();
	afx_msg void OnAddLeadLine();
	afx_msg void OnRemoveLeadLine();
	afx_msg void OnAddDefaultLeadLine();
	//}}AFX_MSG
 	afx_msg void OnUpdateAddLeadLine(CCmdUI* pCmdUI);
 	afx_msg void OnUpdateRemoveLeadLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddDefaultLeadLines(CCmdUI* pCmdUI);
	afx_msg LRESULT OnEditLeadLine(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
};

		  




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationLeadLines_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

