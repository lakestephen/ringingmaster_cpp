#if !defined(AFX_NotationSplicePos_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationSplicePos_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationPlain.h : header file
//
#include "PropList.h"

class Notation;
class NotationDlg;


class NotationSplicePos : public CBCGPPropertyPage
{

// Construction
public:
	NotationSplicePos(Notation &notation);
	~NotationSplicePos();

// Dialog Data
	//{{AFX_DATA(NotationSplicePos)
	enum { IDD = IDD_NOTATION_SPLICE_POS };
	CButton	_useDefaultSplice;
	CBCGPButton	_addDefault;
	CBCGPButton	_removeSplice;
	CBCGPButton	_addSplice;
	PropList	_splice;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(NotationSplicePos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	Notation & _notation;

	void createSpliceList();
	
	// Generated message map functions
	//{{AFX_MSG(NotationSplicePos)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddDefaultSplices();
	afx_msg void OnAddSplice();
	afx_msg void OnRemoveSplice();
	afx_msg void OnUseDefaultSplices();
	//}}AFX_MSG
	afx_msg LRESULT OnEditSplice(WPARAM wParam, LPARAM lParam);
 	afx_msg void OnUpdateAddSplice(CCmdUI* pCmdUI);
 	afx_msg void OnUpdateRemoveSplice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddDefaultSplice(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
};

	 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationSplicePos_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)

