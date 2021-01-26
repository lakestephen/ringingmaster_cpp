#pragma once

#include "ScrollWnd.h" 
#include "TouchAnalysisBase.h"


class CPage;
class MethodAnalysis;

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisCallsMade window

class TouchAnalysisCallsMade : public ScrollWnd, TouchAnalysisBase  

{
// Construction
public:
	TouchAnalysisCallsMade();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisCallsMade)	  
	public:
	virtual BOOL Create(CWnd * pParentWnd);

	//}}AFX_VIRTUAL

// Implementation
public:
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);
	CSize _size;
	CSize getMaxStringSize(Method* method, CDC* pDC);
	CString getString(CStringArray * strings);
	virtual ~TouchAnalysisCallsMade();

	void mainFrame_notifyUpdate(ViewType viewType);
	
	void OnDraw(CDC* pDC);

	void drawOverload(CDC *pDC);

	static double print(CPage *page, double nextRow);

	// Generated message map functions
protected:
	//{{AFX_MSG(TouchAnalysisCallsMade)
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

