#pragma once


#include "ScrollWnd.h"
#include "TouchAnalysisBase.h"


class CPage;
class MethodAnalysis;

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisCourseHead window

class TouchAnalysisCourseHead : public ScrollWnd, TouchAnalysisBase  
{
// Construction
public:
	TouchAnalysisCourseHead();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisCourseHead)
	public:
	virtual BOOL Create(CWnd * pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	void mainFrame_notifyUpdate(ViewType viewType);
	virtual ~TouchAnalysisCourseHead();
	
	void OnDraw(CDC* pDC);

	static double print(CPage *page, double nextRow);   

	// Generated message map functions
protected:
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);
	//{{AFX_MSG(TouchAnalysisCourseHead)
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
