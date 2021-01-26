#if !defined(AFX_NotationExplorerDLG_H__3722EE6A_3313_11D5_B5B7_009027BB3286__INCLUDED_)
#define AFX_NotationExplorerDLG_H__3722EE6A_3313_11D5_B5B7_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotationExplorerDlg.h : header file
//
#include "ResizeDialog.h"
#include "NotationExplorerTree.h"
#include "NotationExplorerList.h"
#include "NotationLite.h"
#include "MetSection.h"
#include "EventWaitDlg.h"
#include "CJFlatComboBox.h"


class ImportLog;
/////////////////////////////////////////////////////////////////////////////
// NotationExplorerDlg dialog

enum LOOKINGFOR
{
	EM_TYPE,
	EM_HEAD,
	EM_METH,
	EM_END
};


class NotationExplorerDlg : public ResizeDialog
{
// Construction
public:
	void updateFlattenImage();
	bool isFlattened();

	void updateFilterImage();
	void updateStatus();
	CStringArray _logStrings;
	EventWaitDlg* _eventWaitDlg;
	void addSections(MetSections& sections);
	void addString(CString str);
	void doImport();
	void makeSelection();

	bool fillMet(CString& line, MetSection& section);
 	void extract(CStringArray& lines, MetSections& sections, CString fileName);
	bool splitString(CString& originalBuff, CStringArray& lines);
	void load(CString& originalBuff, CString fileName);

	void deleteMethodsInFolder(unsigned long folderID);
	
	bool _deviding;

	bool _flattenedState;
	
	bool _filterState;
	int _filterNumber;

	NotationExplorerDlg(CWnd* pParent, NotationLites& selection, bool databaseMaintainance = false, bool filterState = false, int filterNumber = 8, bool import = false);   // standard constructor
	virtual ~NotationExplorerDlg();

// Dialog Data
	//{{AFX_DATA(NotationExplorerDlg)
	enum { IDD = IDD_NOTATION_EXPLORE };
	CBCGPButton	_cutBtn;
	CBCGPButton	_pasteBtn;
	CBCGPButton	_copyBtn;
	CBCGPButton	_flattenBtn;
	CStatic	_statusCtrl;
	CButton	_ok;
	CButton	_cancel;
	CBCGPButton	_search;
	CCJFlatComboBox	_filterCombo;
	CBCGPButton	_filterOnOff;
	NotationExplorerTree _tree;
	NotationExplorerList _list;
	CString	_status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NotationExplorerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL



// Implementation


protected:
	
	int _devider;
	NotationLites& _selection;
	bool _import;
	bool _databaseMaintainance;

	CStringArray _columnNames;


	// Generated message map functions
	//{{AFX_MSG(NotationExplorerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnKickIdle(WPARAM , LPARAM );
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnFilterButt();
	afx_msg void OnSelchangeFilterCombo();
	afx_msg void OnExplorerFind();
	afx_msg void OnFlattenButt();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);

	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()

	friend class NotationExplorerList;
	friend class NotationExplorerTree;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NotationExplorerDLG_H__3722EE6A_3313_11D5_B5B7_009027BB3286__INCLUDED_)
