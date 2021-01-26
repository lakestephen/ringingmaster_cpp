#if !defined(AFX_NotationCallingPosLeadLead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationCallingPosLeadLead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//
#include "PropList.h"

class Notation;
class NotationDlg;


class NotationCallingPosLead : public CBCGPPropertyPage
{
// Construction
public:
	NotationCallingPosLead(Notation & _notation);
	~NotationCallingPosLead();		


// Dialog Data
	//{{AFX_DATA(NotationCallingPosLead)
	enum { IDD = IDD_NOTATION_CALL_POS_LEAD };
	CBCGPButton	_addDefault;
	PropList	_list;
	CButton	_check;
	CBCGPButton	_remove;
	CBCGPButton	_add;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationCallingPosLead)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void update();
	
	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(NotationCallingPosLead)
	virtual BOOL OnInitDialog();
	afx_msg void OnCallPosRemove();
	afx_msg void OnCallPosAdd();
	afx_msg void OnCallPosCheck();
	afx_msg void OnCallPosAddDefault();
	//}}AFX_MSG
	afx_msg void OnUpdateAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDefault(CCmdUI* pCmdUI);
	afx_msg LRESULT OnEditCallingPos(WPARAM wParam, LPARAM lParam);
	
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnSetActive();
};

			


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationCallingPosLeadLead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

