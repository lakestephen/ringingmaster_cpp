#pragma once

class MethodAnalysis;

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisFalseness window

#include "SuperGridCtrl.h"
#include "TouchAnalysisBase.h"

class FalseRow;
class CPage;


class TouchAnalysisFalseness : public SuperGridCtrl, TouchAnalysisBase  
{
// Construction
public:
	TouchAnalysisFalseness();

// Attributes
public:

// Operations
public:
		BOOL OnItemLButtonDown(LVHITTESTINFO& ht);
	CString getEmptyListText();
	CImageList* CreateDragImageEx(int nItem);
		int GetIcon(const SGTreeItem* pItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisFalseness)
	public:
	virtual BOOL Create(CWnd * pParentWnd);
	static double print(CPage *page, double nextRow);	 
	//}}AFX_VIRTUAL

// Implementation
public:
	void mainFrame_notifyUpdate(ViewType viewType);
	virtual ~TouchAnalysisFalseness();

	// Generated message map functions
protected:
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);

	CImageList _image;//Must provide an imagelist

	void addFalseNode(SGTreeItem *parent, FalseRow *curArr);


	//{{AFX_MSG(TouchAnalysisFalseness)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdatePopupPrintAnalysis(CCmdUI* pCmdUI);
	afx_msg void OnPopupPrintAnalysis();
	afx_msg void OnUpdatePrintAnalysis(CCmdUI* pCmdUI);
	afx_msg void OnPrintAnalysis();
	afx_msg void OnPopupEditCopy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
};

