
#pragma once

#include "PropList.h"
#include "TouchAnalysisBase.h"


class CPage;
class MethodAnalysis;


/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisSplice window

class TouchAnalysisSplice : public PropList, TouchAnalysisBase  
{
// Construction
public:
	TouchAnalysisSplice();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisSplice)
	public:
	virtual BOOL Create(CWnd * pParentWnd);

	//}}AFX_VIRTUAL

// Implementation
public:
	
	void addLine(CString col0, CString col1, COLORREF color = -1);
	void mainFrame_notifyUpdate(ViewType viewType);
	virtual ~TouchAnalysisSplice();

	static double print(CPage *page, double nextRow);

	// Generated message map functions
protected:
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);
	//{{AFX_MSG(TouchAnalysisSplice)
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

