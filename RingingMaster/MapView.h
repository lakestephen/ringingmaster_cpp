#if !defined(AFX_MAPVIEW_H__93E03DE5_FAEF_11D6_B63D_000255162CF0__INCLUDED_)
#define AFX_MAPVIEW_H__93E03DE5_FAEF_11D6_B63D_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapView.h : header file
//

class MapDoc;
class PrintScaleObject;
class MapGridRef;

#include "DataTip.h"
#include "ScrollView32Bit.h"

enum MapViewHitTest
{
	mvht_none, 
	mvht_tower, 
	mvht_map, 
	mvht_border,
};


/////////////////////////////////////////////////////////////////////////////
// MapView view

class MapView : public CScrollView32Bit
{
protected:
	MapView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(MapView)

// Attributes
public:
	MapDoc* GetDocument();

// Operations
public:
	CBitmap* getCopyDataBitmap();
	RecordTower* hitTest(CPoint point);
	CPoint getTopLeft();
	CSize getSize();
	void updateScrollSizes();
	void doDraw(CDC *pDC);

	void doPrintMapView(CDC* pDC, CPrintInfo* pInfo);
															 

	CxImage* getImage();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapView)
	public:
	virtual void OnInitialUpdate();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	DataTip _tip;
	CBrush _brHatch;
	MapViewHitTest _mapViewHitTest;
	bool _draggingBackground;
	CPoint _lastBackgroundPos;


	void printDraw(CDC *pDC, PrintScaleObject& printScaleObject);

	
	virtual ~MapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void drawPin(CDC *pDC, CPoint pos, COLORREF color);
	CPoint getPointFromMapGridRef(const MapGridRef& mapGridref);
	MapGridRef getMapGridRefFromPoint(const CPoint& point);
	//{{AFX_MSG(MapView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintToolbar();
	afx_msg void OnFilePrintMenu();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnShowPushpins();
	afx_msg void OnUpdateShowPushpins(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateFilePrintMenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintToolbar(CCmdUI* pCmdUI);
	afx_msg void OnShowMap();
	afx_msg void OnUpdateShowMap(CCmdUI* pCmdUI);
	afx_msg void OnPopupGrabtower();
	afx_msg void OnUpdatePopupGrabtower(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


#ifndef _DEBUG  // debug version in TouchView.cpp
inline MapDoc * MapView::GetDocument()
   { return (MapDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPVIEW_H__93E03DE5_FAEF_11D6_B63D_000255162CF0__INCLUDED_)
