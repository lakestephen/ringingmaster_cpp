#if !defined(AFX_THREEDIMEDITWND_H__094D40C9_87A0_4798_882C_9C15C7780EDA__INCLUDED_)
#define AFX_THREEDIMEDITWND_H__094D40C9_87A0_4798_882C_9C15C7780EDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThreeDimEditWnd.h : header file
//

#include "ScrollWnd.h"
#include "ThreeDimEditCollection.h"
#include "DataTip.h"



/////////////////////////////////////////////////////////////////////////////
// ThreeDimEditWnd window

class ThreeDimEditWnd : public ScrollWnd
{
// Construction
public:
	ThreeDimEditWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ThreeDimEditWnd)
	//}}AFX_VIRTUAL

	BOOL Create(CWnd *pParentWnd, CRect rect, ThreeDimEditCollection* threeDimEditCollection);

// Implementation
public:
	int hitTest(CPoint point);
	void drawGrid(CDC* pDC);
	void drawItems(CDC* pDC);
	virtual ~ThreeDimEditWnd();

	// Generated message map functions
protected:
	
	ThreeDimEditCollection* _threeDimEditCollection;
	int _selectedItem;
	bool _draggingItem;
	bool _draggingBackground;
	CPoint _lastBackgroundPos;

	DataTip _tip;
	
	void OnDraw(CDC* pDC);

	//{{AFX_MSG(ThreeDimEditWnd)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREEDIMEDITWND_H__094D40C9_87A0_4798_882C_9C15C7780EDA__INCLUDED_)
