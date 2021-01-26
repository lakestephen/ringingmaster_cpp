////////////////////////////////////////////////////////////////////////////
//	File:		FlatHeaderCtrl.h
//	Version:	1.0.2.0
//
//	Author:		Maarten Hoeben
//	E-mail:		hoeben@nwn.com
//
//	Specification of the FlatHeaderCtrl and associated classes.
//
//	You are free to use, distribute or modify this code
//	as long as the header is not removed or modified.
//
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLATHEADERCTRL_H__2162BEB4_A882_11D2_B18A_B294B34D6940__INCLUDED_)
#define AFX_FLATHEADERCTRL_H__2162BEB4_A882_11D2_B18A_B294B34D6940__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlatHeaderCtrl.h : header file
//


#define FLATHEADER_TEXT_MAX	80

/////////////////////////////////////////////////////////////////////////////
// FlatHeaderCtrl window

class FlatHeaderCtrl;
class FHDragWnd;
class FHDropWnd;

#define FH_PROPERTY_SPACING			1
#define FH_PROPERTY_ARROW			2
#define FH_PROPERTY_STATICBORDER	3
#define FH_PROPERTY_DONTDROPCURSOR	4
#define FH_PROPERTY_DROPTARGET		5

#define HDF_EX_AUTOWIDTH	0x0001
#define HDF_EX_INCLUDESORT	0x0002
#define HDF_EX_FIXEDWIDTH	0x0004

typedef struct _HDITEMEX
{
	UINT	nStyle;
	INT		iMinWidth;
	INT		iMaxWidth;

	_HDITEMEX() : nStyle(0), iMinWidth(0), iMaxWidth(-1) {};

} HDITEMEX, FAR* LPHDITEMEX;

class FlatHeaderCtrl : public CHeaderCtrl
{
	friend class FHDragWnd;						   

    DECLARE_DYNCREATE(FlatHeaderCtrl)

// Construction
public:
	FlatHeaderCtrl();

// Attributes
public:
	BOOL ModifyProperty(WPARAM wParam, LPARAM lParam);

	BOOL GetItemEx(INT iPos, HDITEMEX* phditemex) const;
	BOOL SetItemEx(INT iPos, HDITEMEX* phditemex);

	INT GetItemWidth(LPHDITEM lphdi, BOOL bIncludeSort = FALSE);

	void SetSortColumn(INT iPos, BOOL bSortAscending);
	INT GetSortColumn(BOOL* pbSortAscending = NULL);

	INT GetDropResult();

// Overrides
public:
	virtual ~FlatHeaderCtrl();

	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bSort, BOOL bSortAscending);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FlatHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bDoubleBuffer;
	INT m_iSpacing;
	SIZE m_sizeImage;
	SIZE m_sizeArrow;
	BOOL m_bStaticBorder;
	UINT m_nDontDropCursor;
	HWND m_hDropTarget;
	CRect m_rcDropTarget;
	INT m_iDropResult;

	INT m_iHotIndex;
	INT m_iHotOrder;
	BOOL m_bHotItemResizable;
	HDHITTESTINFO m_hdhtiHotItem;
	HDITEM m_hdiHotItem;
	TCHAR m_szHotItemText[FLATHEADER_TEXT_MAX];

	BOOL m_bResizing;

	INT m_iHotDivider;
	COLORREF m_crHotDivider;
	FHDropWnd* m_pDropWnd;

	BOOL m_bDragging;
	FHDragWnd* m_pDragWnd;

	UINT m_nClickFlags;
	CPoint m_ptClickPoint;

	BOOL m_bSortAscending;
	INT m_iSortColumn;
	CArray<HDITEMEX, HDITEMEX> m_arrayHdrItemEx;

	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crText;

	void DrawCtrl(CDC* pDC);
	INT DrawImage(CDC* pDC, CRect rect, LPHDITEM hdi, BOOL bRight);
	INT DrawBitmap(CDC* pDC, CRect rect, LPHDITEM hdi, CBitmap* pBitmap, BITMAP* pBitmapInfo, BOOL bRight);
	INT DrawText (CDC* pDC, CRect rect, LPHDITEM lphdi);
	INT DrawArrow(CDC* pDC, CRect rect, BOOL bSortAscending, BOOL bRight);

// Generated message map functions
protected:
	//{{AFX_MSG(FlatHeaderCtrl)
	afx_msg LRESULT OnInsertItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnDeleteItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetImageList(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetHotDivider(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnLayout(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// FHDragWnd window

#define FHDRAGWND_CLASSNAME	_T("MFFHDragWnd")

class FHDragWnd : public CWnd
{
// Construction
public:
	FHDragWnd();

// Attributes
public:

// Operations
public:

// Overrrides
protected:
    // Drawing
    virtual void OnDraw(CDC* pDC);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FHDragWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FHDragWnd();
	virtual BOOL Create(CRect rect, FlatHeaderCtrl* pFlatHeaderCtrl, INT iItem, LPHDITEM lphdiItem);

protected:
	FlatHeaderCtrl* m_pFlatHeaderCtrl;
	INT m_iItem;
	LPHDITEM m_lphdiItem;

	// Generated message map functions
protected:
	//{{AFX_MSG(FHDragWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// FHDropWnd window

#define FHDROPWND_CLASSNAME	_T("MFFHDropWnd")

class FHDropWnd : public CWnd
{
// Construction
public:
	FHDropWnd(COLORREF crColor);

// Attributes
public:

// Operations
public:
	void SetWindowPos(INT x, INT y);

// Overrrides
protected:
    // Drawing

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FHDropWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~FHDropWnd();
	virtual BOOL Create(INT iHeight);

protected:
	CBrush m_brush;
	CRgn m_rgn;

	INT m_iHeight;

	// Generated message map functions
protected:
	//{{AFX_MSG(FHDropWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATHEADERCTRL_H__2162BEB4_A882_11D2_B18A_B294B34D6940__INCLUDED_)
