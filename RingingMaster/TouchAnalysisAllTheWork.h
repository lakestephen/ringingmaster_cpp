
#pragma once

//	

#include "ScrollWnd.h"
#include "TouchAnalysisBase.h"



class AllTheWorkBell;
class Notation;
class CPage;
class MethodAnalysis;

/////////////////////////////////////////////////////////////////////////////
// TouchAnalysisAllTheWork window

class TouchAnalysisAllTheWork : public ScrollWnd, TouchAnalysisBase  
{
// Construction
public:
	TouchAnalysisAllTheWork();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchAnalysisAllTheWork)	
	public:
	virtual BOOL Create(CWnd * pParentWnd);

	//}}AFX_VIRTUAL

// Implementation
public:
	void mainFrame_notifyUpdate(ViewType viewType);
	virtual ~TouchAnalysisAllTheWork();

	void OnDraw(CDC* pDC);
	static double print(CPage *page, double nextRow);
	// Generated message map functions
protected:
	CString getCopyData(const Method& method, const MethodAnalysis& analysis);

	double printAllTheWorkGrid(CPage *page, double nextRow, double& nextColumn, double gridWidth,
									 AllTheWorkBell* aTW, int spliceIndex, bool drawSpliceTitle, 
									 Notation* not, bool subtractPlainCourseLength = false); 


	void calculateSizes(CDC* pDC);
	void drawGrid(CDC* pDC, AllTheWorkBell* aTW, CPoint origin, int spliceIndex, bool drawSpliceTitle, Notation* not, bool subtractPlainCourseLength = false);
	CString getCopyGrid(AllTheWorkBell* aTW, int spliceIndex, bool drawSpliceTitle, Notation* not, bool subtractPlainCourseLength= false);

	int _gridWidth;
	int _gridHeight;

	//{{AFX_MSG(TouchAnalysisAllTheWork)
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
