#if !defined(AFX_NotationCalls_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationCalls_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//
#include "PropList.h"

class Notation;
class NotationDlg;



class NotationCalls : public CBCGPPropertyPage
{
// Construction
public:
	NotationCalls(Notation &notation);
	~NotationCalls();

// Dialog Data
	//{{AFX_DATA(NotationCalls)
	enum { IDD = IDD_NOTATION_CALLS };
	CBCGPButton	_defNear;
	CBCGPButton	_defExtream;
	CEdit	_leadHead;
	CBCGPButton	_stdCall;
	CBCGPButton	_viewLeadHeads;
	CButton	_useDefault;
	PropList _calls;
	CBCGPButton	_removeCall;
	CBCGPButton	_addCall;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationCalls)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void createList();

	void update();

	Notation & _notation;

	// Generated message map functions
	//{{AFX_MSG(NotationCalls)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddCall();
	afx_msg void OnRemoveCall();
	afx_msg void OnUseDefaultCalls();
	afx_msg void OnViewLeadheads();
	afx_msg void OnSetStdCall();
	afx_msg void OnAddNearPlaceCall();
	afx_msg void OnAddExtreamPlaceCall();
	//}}AFX_MSG

	afx_msg LRESULT OnEditCall(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateAddCall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRemoveCall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDefaultCall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddNear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddExtreme(CCmdUI* pCmdUI);

	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
													 
	DECLARE_MESSAGE_MAP()

};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationCalls_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

