#if !defined(AFX_NotationExplorerLIST_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationExplorerLIST_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationExplorerList.h : header file
//
class NotationExplorerDlg;


#include "NotationListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// NotationExplorerList window

class NotationExplorerList : public NotationListCtrl
{

	friend class NotationExplorerTree;
	// Construction
public:
	NotationExplorerList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotationExplorerList)
	//}}AFX_VIRTUAL

// Implementation
public:				   
	int getSelectedCount();
	int getChildFoldersCount();
	CString getFolderName(unsigned long item);
	CString getEmptyListText();
	bool SortList(int nCol, bool bAscending);
	
	void init();
	void setParent(NotationExplorerDlg* parent);
	void showMethods();
	virtual ~NotationExplorerList(); 
	// Generated message map functions


protected:

	NotationExplorerDlg* _parent;
	//{{AFX_MSG(NotationExplorerList)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnExplorerListEdit();
	afx_msg void OnMeNewMethod();
	afx_msg void OnMeListDelete();
	afx_msg void OnMeNewFolder();
	afx_msg void OnMeRenameMethod();
	afx_msg void OnMeListView();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditCut();
	//}}AFX_MSG

	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);


	DECLARE_MESSAGE_MAP()


	friend class NotationExplorerDlg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationExplorerLIST_H__7A3DD863_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
