#if !defined(AFX_TouchBlueLineView_H__A3C39AC0_076E_11D5_BB7F_BB8C29C49536__INCLUDED_)
#define AFX_TouchBlueLineView_H__A3C39AC0_076E_11D5_BB7F_BB8C29C49536__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TouchBlueLineView.h : header file
//
#include "MethodWnd.h"	// Added by ClassView
#include "TouchDoc.h"
#include "MethodWndPrint.h"
#include "MethodWndBlueLine.h"

/////////////////////////////////////////////////////////////////////////////
// TouchBlueLineView view

class TouchBlueLineView : public CView
{
protected:
	TouchBlueLineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TouchBlueLineView)

// Attributes
public:

	TouchDoc* GetDocument();

	afx_msg void OnToolsStyle();


// Operations
public:
	CxImage* getImage();
	void setStyle(const Style & style);
	const Style & getStyle();
	MethodWndBlueLine& getMethodWnd();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchBlueLineView)
	public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

	MethodWndPrint* getMethodWndPrint(CDC* pDC, CPage* page, CPrintInfo* pInfo);

// Implementation
protected:
	virtual ~TouchBlueLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	MethodWndBlueLine _methodWnd;


	static const UINT _findDialogMessage;
	CFindReplaceDialog *_pFindDialog;
	void doFind(CString findText, bool searchDown);
	CString _searchStr;
	void doPrintBlueLineView(CDC* pDC, CPrintInfo* pInfo);



	//{{AFX_MSG(TouchBlueLineView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateToolsStyle(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintToolbar();
	afx_msg void OnFilePrintMenu();
	afx_msg void OnPrintAnalysis();
	afx_msg void OnPopupPrintAnalysis();
	afx_msg void OnUpdateStyleMenuSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnStyleMenuSaveAs();
	afx_msg void OnStyleMenuMore();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnEditFindPrevious();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnSetMethodPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStyleChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDoConvertToLeadBased(WPARAM wParam, LPARAM lParam);
	afx_msg void OnApplyStyle(UINT nID);
	afx_msg LRESULT OnFindDialogMessage(WPARAM wParam, LPARAM lParam);		
	afx_msg void OnFilePrintPreview();


	DECLARE_MESSAGE_MAP()

};

#ifndef _DEBUG  // debug version in TouchBlueLineView.cpp
inline TouchDoc * TouchBlueLineView::GetDocument()
   { return (TouchDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TouchBlueLineView_H__A3C39AC0_076E_11D5_BB7F_BB8C29C49536__INCLUDED_)
