#if !defined(AFX_SCROLLWND_H__44917C53_32D0_11D4_B4B3_009027BB3286__INCLUDED_)
#define AFX_SCROLLWND_H__44917C53_32D0_11D4_B4B3_009027BB3286__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ScrollWnd window

class ScrollWnd : public CWnd
{
DECLARE_DYNAMIC(ScrollWnd)
	
// Construction
public:
	ScrollWnd();
	virtual ~ScrollWnd();

// Attributes
public:
	void SetScrollSizes(CSize totalSize, const CSize pageSize = CSize(0,0), 
											const CSize lineSize = CSize(0,0));

	CPoint GetScrollPosition() const; //Top left corner
	CSize GetTotalSize() const {return _totalSize;};
	void GetScrollSizes(SIZE& total, SIZE& page, SIZE& line) const;
// Operations
public:

	void ScrollToPosition(POINT scrollPos);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ScrollWnd)
	public:
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) ;
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
#endif //__DEBUG

	// Generated message map functions
protected:
	virtual void OnDraw(CDC* pDC);
	
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	void GetScrollBarSizes(CSize& scrollBar);
	void GetScrollBarState(CSize client, CSize &needScrollBar, CSize &range, 
											CPoint &move, BOOL InsideClient);

	void UpdateBars();

	CSize _totalSize; //'canvas' size in pixels
	CSize _lineSize;	//line size in pixels
	CSize _pageSize;	//page size in pixels
	BOOL _insideUpdate;	//lock out for OnScroll		 
	
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

	//{{AFX_MSG(ScrollWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLWND_H__44917C53_32D0_11D4_B4B3_009027BB3286__INCLUDED_)
