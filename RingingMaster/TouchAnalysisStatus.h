#pragma once

#include "PropList.h"
#include "TouchAnalysisBase.h"


class CPage; //todo remove when migrated to base class in all analysiss windows
class MethodAnalysis;		//todo remove when migrated to base class in all analysiss windows.

//todo have a base class for all analysis windows providing printing, copying services Etc

class TouchAnalysisStatus : public PropList, TouchAnalysisBase  
{
// Construction
public:
	TouchAnalysisStatus();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisStatus)
	public:
	virtual BOOL Create(CWnd * pParentWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
	void addLine(CString col0, CString col1,  COLORREF color = -1);
	void mainFrame_notifyUpdate(ViewType viewType);
	static double print(CPage *page, double nextRow);
	virtual ~TouchAnalysisStatus();

	// Generated message map functions
protected:
	CString copyLine(CString col0, CString col1);
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);
	//{{AFX_MSG(TouchAnalysisStatus)
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

