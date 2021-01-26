// TouchSimulatorView.h : interface of the TouchSimulatorView class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SimulatorManagerEventListener.h"


class TouchSimulatorView : public CView	, SimulatorManagerEventListener
{
protected: // create from serialization only
	TouchSimulatorView();
	DECLARE_DYNCREATE(TouchSimulatorView)

// Attributes
public:
	TouchDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TouchSimulatorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TouchSimulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


// Generated message map functions
protected:
	//{{AFX_MSG(TouchSimulatorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG		 

	int _mouseDownBell;

	DECLARE_MESSAGE_MAP()

	void simulatorManager_notifyBellStateChangeFromSeperateThread(int bell);
	void simulatorManager_notifySimulatorConnected(bool connected);
	void simulatorManager_notifyProfileChange();

	void updateSimulatorGUI();

public:
	
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnStriking();
	afx_msg void OnUpdateStriking(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in TouchSimulatorView.cpp
inline TouchDoc* TouchSimulatorView::GetDocument()
   { return (TouchDoc*)m_pDocument; }
#endif

