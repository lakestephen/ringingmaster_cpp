#if !defined(AFX_CALLPOSAGREGATEEDITDLG_H__6F48F4C1_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_)
#define AFX_CALLPOSAGREGATEEDITDLG_H__6F48F4C1_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallPosAgregateEditDlg.h : header file
//

#include "NotationCallPosAgregate.h"
#include "Notation.h"
#include "PropList.h"

/////////////////////////////////////////////////////////////////////////////
// CallPosAgregateEditDlg dialog

class CallPosAgregateEditDlg : public CDialog
{
// Construction
public:
	void update();
	CallPosAgregateEditDlg(NotationCallPosAgregate * agregateCallingPos, Notation& notation);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CallPosAgregateEditDlg)
	enum { IDD = IDD_CALL_POS_AGREGATE };
	CBCGPButton	_up;
	CBCGPButton	_down;
	PropList	_agregate;
	CBCGPButton	_remove;
	CBCGPButton	_add;
	CString	_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CallPosAgregateEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CallPosAgregateEditDlg)
	afx_msg void OnAddAgregate();
	afx_msg void OnRemoveAgregate();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveUp();
	//}}AFX_MSG
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnUpdateRemove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDown(CCmdUI* pCmdUI);
	afx_msg LRESULT OnEditCallingPos(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()

	NotationCallPosAgregate * _originalAgregateCallingPos;
	NotationCallPosAgregate _agregateCallingPos;
	Notation& _notation;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLPOSAGREGATEEDITDLG_H__6F48F4C1_9CBA_11D5_BB7F_D87D631C4F79__INCLUDED_)
