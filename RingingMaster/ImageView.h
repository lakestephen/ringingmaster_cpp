// ImageView.h : interface of the ImageView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEVIEW_H__09D3C251_DF94_11D7_B6FB_000255162CF0__INCLUDED_)
#define AFX_IMAGEVIEW_H__09D3C251_DF94_11D7_B6FB_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ScrollView32Bit.h"

class PrintScaleObject;


class ImageView : public CScrollView32Bit
{
protected: // create from serialization only
	ImageView();
	DECLARE_DYNCREATE(ImageView)

// Attributes
public:
	ImageDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ImageView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CPoint getTopLeft();
	void updateScrollSizes();
	CBrush _brHatch;


	void doDraw(CDC *pDC);
	void printDraw(CDC *pDC, PrintScaleObject& printScaleObject);

	CSize getSize();
	void doPrintImageView(CDC* pDC, CPrintInfo* pInfo);



// Generated message map functions
protected:
	//{{AFX_MSG(ImageView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUpdateFilePrintMenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintToolbar(CCmdUI* pCmdUI);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintToolbar();
	afx_msg void OnFilePrintMenu();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ImageView.cpp
inline ImageDoc* ImageView::GetDocument()
   { return (ImageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEVIEW_H__09D3C251_DF94_11D7_B6FB_000255162CF0__INCLUDED_)
