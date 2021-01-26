#pragma once


// GraphWnd

class GraphWnd : public CWnd
{
	DECLARE_DYNAMIC(GraphWnd)

public:
	GraphWnd();
	virtual ~GraphWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	afx_msg void OnPaint();
	void OnDraw(CDC* pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	void setXZeroPos(int xZeroPos);
	void setValue(int index, int value, COLORREF color);
	void setCount(int count);


protected:
	Ints _ints;
	CArray<COLORREF, COLORREF> _colors;
	int _xZeroPos;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


