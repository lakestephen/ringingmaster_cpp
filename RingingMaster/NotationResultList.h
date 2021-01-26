#if !defined(AFX_NOTATIONRESULTLIST_H__8E1910A0_B77D_11D5_BB7F_F267AE288F6C__INCLUDED_)
#define AFX_NOTATIONRESULTLIST_H__8E1910A0_B77D_11D5_BB7F_F267AE288F6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationResultList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NotationResultList window
#include "NotationListCtrl.h"

class NotationResultsDlg;

class NotationResultList : public NotationListCtrl
{
// Construction
public:
	NotationResultList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotationResultList)
	//}}AFX_VIRTUAL

// Implementation
public:
	void setParent(NotationResultsDlg *parent);
	virtual ~NotationResultList();
	void init();

	// Generated message map functions
protected:

	bool SortList(int nCol, bool bAscending);

	NotationResultsDlg *_parent;

	//{{AFX_MSG(NotationResultList)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTATIONRESULTLIST_H__8E1910A0_B77D_11D5_BB7F_F267AE288F6C__INCLUDED_)
