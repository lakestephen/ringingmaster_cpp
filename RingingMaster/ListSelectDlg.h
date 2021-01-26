#if !defined(AFX_LISTSELECTDLG_H__3F9B7DAA_9ECF_49AD_82BA_8A374B186195__INCLUDED_)
#define AFX_LISTSELECTDLG_H__3F9B7DAA_9ECF_49AD_82BA_8A374B186195__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListSelectDlg.h : header file
//

#include "ListSelectItem.h"
#include "ListBoxHS.h"
#include "ResizeDialog.h"

/////////////////////////////////////////////////////////////////////////////
// ListSelectDlg dialog

class ListSelectDlg : public ResizeDialog
{
// Construction
public:
	ListSelectDlg(CWnd* pParent, CString title,ListSelectItems& available, ListSelectItems& selected,  UINT ID = ListSelectDlg::IDD);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ListSelectDlg)
	enum { IDD = IDD_LIST_SELECT };
	ListBoxHS	_listLeft;
	CBCGPButton	_right;
	ListBoxHS	_listRight;
	CBCGPButton	_left;
	CBCGPButton	_allLeft;
	CBCGPButton	_allRight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ListSelectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString _title;
	void update();
	BOOL isOnAvailableList(ListSelectItem* item);
	BOOL isOnSelectedList(ListSelectItem* item);

	ListSelectItems& _available;
	ListSelectItems& _originalSelected;
	ListSelectItems _selected;

	// Generated message map functions
	//{{AFX_MSG(ListSelectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAllLeft();
	afx_msg void OnAllRight();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg void OnUpdateAllLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAllRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRight(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTSELECTDLG_H__3F9B7DAA_9ECF_49AD_82BA_8A374B186195__INCLUDED_)
