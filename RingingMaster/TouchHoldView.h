#if !defined(AFX_TouchHoldView_H__CA9CBE6C_5889_11D4_B4DB_009027BB3286__INCLUDED_)
#define AFX_TouchHoldView_H__CA9CBE6C_5889_11D4_B4DB_009027BB3286__INCLUDED_

#include "TouchDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TouchHoldView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TouchHoldView view

class TouchHoldView : public CBCGPTabView
{
protected:
	TouchHoldView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(TouchHoldView)

// Attributes
public:
	TouchDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchHoldView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL



// Implementation
protected:
	void DrawLines(CDC* pDC);
	virtual ~TouchHoldView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(TouchHoldView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL DestroyWindow();
};

#ifndef _DEBUG  // debug version in TouchHoldView.cpp
inline TouchDoc * TouchHoldView::GetDocument()
   { return (TouchDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TouchHoldView_H__CA9CBE6C_5889_11D4_B4DB_009027BB3286__INCLUDED_)
