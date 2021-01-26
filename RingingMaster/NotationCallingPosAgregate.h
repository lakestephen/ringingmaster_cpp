#if !defined(AFX_NotationCallingPosAgregateLead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationCallingPosAgregateLead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//
#include "PropList.h"

class Notation;
class NotationDlg;


class NotationCallingPosAgregate : public CBCGPPropertyPage
{
// Construction
public:
	NotationCallingPosAgregate(Notation & _notation);
	~NotationCallingPosAgregate();		

// Dialog Data
	//{{AFX_DATA(NotationCallingPosAgregate)
	enum { IDD = IDD_NOTATION_CALL_POS_AGREGATE };
	PropList	_list;
	CBCGPButton	_remove;
	CBCGPButton	_add;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationCallingPosAgregate)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void update();
	

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(NotationCallingPosAgregate)
	virtual BOOL OnInitDialog();
	afx_msg void OnCallPosRemove();
	afx_msg void OnCallPosAdd();
	//}}AFX_MSG
	afx_msg void OnUpdateAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);
	afx_msg LRESULT OnEditCallingPos(WPARAM wParam, LPARAM lParam);
	

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnSetActive();
};

			


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationCallingPosAgregateLead_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

