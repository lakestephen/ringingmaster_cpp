// StrikeView.h : interface of the StrikeView class
//
#pragma once

#include "DataTip.h"
#include "ScrollView32Bit.h"


#define STRIKE_VIEW_PITCH 16
#define STRIKE_VIEW_HAND_BACK_WIDTH 12
#define STRIKE_VIEW_TEXT_GUTTER 3


struct StrikeHitTestPackage
{
	CRect _rect;
	CString _string;
};


class StrikeView : public CScrollView32Bit
{
protected: // create from serialization only
	StrikeView();
	DECLARE_DYNCREATE(StrikeView)

// Attributes
public:
	StrikeDoc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnInitialUpdate();
	
	void doDraw(CDC* pDC);

	CSize getSize();
	void updateScrollSizes();

// Implementation
public:
	virtual ~StrikeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CSize _lastScrollSize;

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI *pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnUpdateFilePrint(CCmdUI *pCmdUI);
	afx_msg void OnFilePrintMenu();
	afx_msg void OnUpdateFilePrintMenu(CCmdUI *pCmdUI);


	CImageList _imageList;
	DataTip _tip;

	CArray<StrikeHitTestPackage*, StrikeHitTestPackage*> _hitTestArray;
	void clearHitTestArray();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in StrikeView.cpp
inline StrikeDoc* StrikeView::GetDocument() const
   { return reinterpret_cast<StrikeDoc*>(m_pDocument); }
#endif

