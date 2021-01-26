#pragma once

class MethodAnalysis;

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisLeadEnds window

#include "ScrollWnd.h"
#include "TouchAnalysisBase.h"


class CPage;


class TouchAnalysisLeadEnds : public ScrollWnd, TouchAnalysisBase  
{
// Construction
public:
	TouchAnalysisLeadEnds();

// Attributes
public:

// Operations
public:

	void OnDraw(CDC* pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisLeadEnds)
	public:
	virtual BOOL Create(CWnd * pParentWnd);	  
	//}}AFX_VIRTUAL

// Implementation
public:
	void mainFrame_notifyUpdate(ViewType viewType);
	virtual ~TouchAnalysisLeadEnds();

	static double print(CPage *page, double nextRow);	 


	// Generated message map functions
protected:
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);
	//{{AFX_MSG(TouchAnalysisLeadEnds)
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

