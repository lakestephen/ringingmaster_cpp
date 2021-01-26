// CribView.h : interface of the CribView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRIBVIEW_H__52B32847_A89B_11D7_B6DE_000255162CF0__INCLUDED_)
#define AFX_CRIBVIEW_H__52B32847_A89B_11D7_B6DE_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CribDoc.h"
#include "MethodCribWnd.h"

class MethodWndPrintCrib;
class CPage;



class CribView : public CView
{
protected: // create from serialization only
	CribView();
	DECLARE_DYNCREATE(CribView)

// Attributes
public:
	CribDoc* GetDocument();

	void setStyle(const Style & style);
	const Style & getStyle();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CribView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);


	//}}AFX_VIRTUAL

// Implementation
public:
	MethodCribWnd& getMethodWnd();
	CxImage* getImage();

	virtual ~CribView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:	

private:	
	MethodCribWnd _methodWnd;


// Generated message map functions
protected:
	//{{AFX_MSG(CribView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnToolsStyle();
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnPopupPrintAnalysis();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintMenu();
	afx_msg void OnFilePrintToolbar();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG

	afx_msg LRESULT OnSetMethodPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStyleChanged(WPARAM wParam, LPARAM lParam);

	void doPrintCrib(CDC* pDC, CPrintInfo* pInfo);
	MethodWndPrintCrib* getMethodWndPrint(CDC* pDC, CPage* page, CPrintInfo* pInfo) ;

	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in CribView.cpp
inline CribDoc* CribView::GetDocument()
   { return (CribDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRIBVIEW_H__52B32847_A89B_11D7_B6DE_000255162CF0__INCLUDED_)
