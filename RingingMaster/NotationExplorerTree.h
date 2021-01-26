#if !defined(AFX_NotationExplorerTREE_H__7A3DD862_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
#define AFX_NotationExplorerTREE_H__7A3DD862_3698_11D5_BB7F_9A54F804D91E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationExplorerTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NotationExplorerTree window
#include "CJTreeCtrl.h"
#include "MetSection.h"
#include "StdAfx.h"	// Added by ClassView


class NotationExplorerDlg;


class NotationExplorerTree : public CCJTreeCtrl
{

	friend class NotationExplorerList;
// Construction
public:
	NotationExplorerTree();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotationExplorerTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	void selectItem(unsigned long item);
	CString getItemName(unsigned long item);
	void getChildrenFolderID(UnsignedLongs& items, bool recurse);
	CString tidyFolderName(CString folderName);
	unsigned long getSectionFolderID(MetSection* section);
	void init();
	void createBlankCCDir();
	HTREEITEM _lastContextItem;
	void openFolder(unsigned long folderID);
	HTREEITEM createNewFolder(HTREEITEM hItem, BOOL user, CString name = "New Folder");
	void setParent(NotationExplorerDlg* parent);
	unsigned long getCurFolderID();
	void saveDirectory();
	void loadDirectory();
	virtual ~NotationExplorerTree();
	ExplorerFolder* createFolderFromHTREEITEM(HTREEITEM hItem);

	// Generated message map functions
protected:


	CImageList _imageList;
	NotationExplorerDlg* _parent;


	//{{AFX_MSG(NotationExplorerTree)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMeNewFolder();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMeDeleteFolder();
	afx_msg void OnMeNewMethod();
	afx_msg void OnMeRenameFolder();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationExplorerTREE_H__7A3DD862_3698_11D5_BB7F_9A54F804D91E__INCLUDED_)
