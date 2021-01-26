// GraphWnd.cpp : implementation file
//

#include "stdafx.h"
#include "RingingMaster.h"
#include "GraphWnd.h"
#include "Memdc.h"
#include ".\graphwnd.h"


// GraphWnd

IMPLEMENT_DYNAMIC(GraphWnd, CWnd)
GraphWnd::GraphWnd() : 
_xZeroPos(0)
{
	setCount(0);
}

GraphWnd::~GraphWnd()
{
	setCount(0);
}


BEGIN_MESSAGE_MAP(GraphWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// GraphWnd message handlers


BOOL GraphWnd::Create(DWORD dwExStyle, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	dwStyle |= WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN;	
	
	if (lpszWindowName == NULL) 
	{
		lpszWindowName = "GraphWnd";
	}

	return CWnd::CreateEx(dwExStyle, AfxRegisterWndClass(0, 0, 0, 0), 
		lpszWindowName, dwStyle, rect, pParentWnd, nID, NULL);
}

void GraphWnd::OnPaint()
{
	CPaintDC pDC(this); // device context for painting
	MemDC memdc(&pDC);

	CRect rect;
	GetClientRect(rect);	
	memdc->FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));  

	OnDraw(memdc);
}

void GraphWnd::OnDraw(CDC* pDC)
{
	pDC->SetMapMode(MM_TEXT);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));

	CRect clientRect;
	GetClientRect(&clientRect);


	//get the scale
	double dMax = 0;
	for (int i=0;i<_ints.GetSize();i++)
		dMax = max((double)_ints[i], dMax);

	double xInterval = (double)clientRect.Width()/100.0;
	double yScale = (double)(clientRect.Height()-20) / dMax;

	CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
	CPen* oldPen = pDC->SelectObject(&pen);
	CFont * oldFont = pDC->SelectObject(&app()->_fontStd);	

	pDC->MoveTo((int)(xInterval * (double)(_xZeroPos - 0.5)), 0);
	pDC->LineTo((int)(xInterval * (double)(_xZeroPos - 0.5)), clientRect.bottom);
	pDC->MoveTo(0, clientRect.bottom-10);
	pDC->LineTo(clientRect.right, clientRect.bottom-10);

	CRect rect(0,10,(int)xInterval,clientRect.bottom-10);
	for (int i=0;i<_ints.GetSize();i++)
	{
		rect.top = rect.bottom - (int)((double)_ints[i] * yScale) ;
		pDC->FillSolidRect(rect,_colors[i]);		
		rect.left  = (int)(xInterval * (double)(i + 0.5));
		rect.right = rect.left + 1;
	}
		 
	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldFont);
}

BOOL GraphWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void GraphWnd::setValue(int index, int value, COLORREF color)
{
	ASSERT(index >=0 && index <  _ints.GetSize()&& index <  _colors.GetSize());

	if (index >=0 && index <  _ints.GetSize() && index <  _colors.GetSize())
	{
		_ints.SetAt(index, value);
		_colors.SetAt(index, color);
	}
}

void GraphWnd::setXZeroPos(int xZeroPos)
{
	_xZeroPos = xZeroPos;
}

void GraphWnd::setCount(int count)
{
	_ints.RemoveAll();
	_colors.RemoveAll();

	_ints.SetSize(count);
	_colors.SetSize(count);

	for (int i=0;i<count;i++)
		_ints.SetAt(i,0);

	COLORREF color = ::GetSysColor(COLOR_3DSHADOW);

	for (int i=0;i<count;i++)
		_colors.SetAt(i,color);
	
}
void GraphWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	Invalidate();
}
