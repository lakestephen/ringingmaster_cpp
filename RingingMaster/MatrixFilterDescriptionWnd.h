#pragma once


#include "ScrollWnd.h"

class MatrixFilterSort;


enum  MatrixFilterDescriptionType 
{
	mfdt_none, 
	mfdt_filter, 
	mfdt_sort, 
	mfdt_column,
};


class MatrixFilterDescriptionWnd : public ScrollWnd
{
// Construction
public:
	MatrixFilterDescriptionWnd(MatrixFilterSort& filterSort);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MatrixFilterDescriptionWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void setSelectedSort(int index);
	int getSelectedSort();
	
	void setSelectedColumn(int index);
	int getSelectedColumn();

	void setSelectedFilter(int index);
	int getSelectedFilter();

	CSize _lastScrollSize;
	CSize drawItems(CDC* pDC, MatrixFilterDescriptionType type );
	void update();	
	BOOL Create(const RECT& rect, CWnd* pParentWnd);
	virtual ~MatrixFilterDescriptionWnd();

	// Generated message map functions
protected:
	void hitTest(CPoint point, MatrixFilterDescriptionType& type,  int& index);
	int _selectedFilter;
	int _selectedSort;
	int _selectedColumn;
	//{{AFX_MSG(MatrixFilterDescriptionWnd)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	void OnDraw(CDC* pDC);
	MatrixFilterSort& _filterSort;

	CFont _stdFont;
	CFont _underlineFont;




	DECLARE_MESSAGE_MAP()
};

